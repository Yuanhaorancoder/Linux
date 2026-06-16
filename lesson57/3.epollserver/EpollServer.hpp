#pragma once
#include <iostream>
#include <memory>
#include <sys/epoll.h>
#include "Socket.hpp"

static const uint16_t gport = 8080;
static const int gsize = 128;
static const int gnum = 64;

class EpollServer
{
public:
    EpollServer(uint16_t port = gport)
        : _port(port),
          _listensockfd(std::make_unique<TcpSocket>())
    {
        // 创建监听socket
        _listensockfd->BuildSocketMethod(_port);
        // 创建epoll模型
        _epfd = epoll_create(gsize);
        if (_epfd < 0)
        {
            LOG(LogLevel::FATAL) << "epoll_create error";
            exit(1);
        }
        LOG(LogLevel::INFO) << "listen sockfd: " << _listensockfd->Socketfd() << " " << _epfd;
    }

    void Accepter()
    {
        InetAddr clientaddr;
        int sockfd = _listensockfd->Accepter(&clientaddr);
        LOG(LogLevel::INFO) << "sockfd is: " << sockfd << "client addr: " << clientaddr.StringAddress();
        // 能不能直接recv(sockfd)？不能！
        // sockfd，读取数据？ IO = 等 + 拷贝，把sockfd托管给epoll
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = sockfd;
        int m = epoll_ctl(_epfd, EPOLL_CTL_ADD, sockfd, &ev);
        (void)m;
        LOG(LogLevel::INFO) << "epoll_ctl add event: " << sockfd;
    }

    void IOHandler(int fd)
    {
        // 普通fd到来
        char buffer[1024]; // bug
        ssize_t n = recv(fd, buffer, sizeof(buffer) - 1, 0);
        if (n > 0)
        {
            buffer[n] = 0;
            LOG(LogLevel::INFO) << "client say# " << buffer;
            std::string echo_string = "echo# ";
            echo_string += buffer;

            send(fd, echo_string.c_str(), echo_string.size(), 0);
        }
        else if (n == 0)
        {
            int n = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, nullptr); // 要删除fd，必须保证fd本身是合法的！
            LOG(LogLevel::INFO) << "client quit, epoll_ctl del event: " << fd;
            close(fd);
        }
        else
        {
            int n = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, nullptr); // 要删除fd，必须保证fd本身是合法的！
            LOG(LogLevel::WARNING) << "recv error, epoll_ctl del event: " << fd;
            close(fd);
        }
    }

    void EventsHandler(struct epoll_event revs[], int ready_num)
    {
        for (int i = 0; i < ready_num; i++)
        {
            uint32_t events = revs[i].events; // 哪些事件
            int fd = revs[i].data.fd;         // 哪一个fd
            if (events & EPOLLIN)
            {
                // listen sockfd就绪 还是 普通fd就绪呢？——进行判断
                if (fd == _listensockfd->Socketfd())
                {
                    // 链接事件到来
                    Accepter();
                }
                else
                {
                    // 普通fd到来
                    IOHandler(fd);
                }
            }
            else if (events & EPOLLOUT)
            {
            }
        }
    }

    void Dispatcher()
    {
        // 将listen sockfd 添加到epoll里
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = _listensockfd->Socketfd(); // 用户数据

        int n = epoll_ctl(_epfd, EPOLL_CTL_ADD, _listensockfd->Socketfd(), &ev);
        if (n >= 0)
        {
            LOG(LogLevel::DEBUG) << "epoll_ctl add : " << _listensockfd->Socketfd() << " success";
        }

        int timeout = 2000;
        struct epoll_event revs[gnum];
        while (true)
        {
            int n = epoll_wait(_epfd, revs, gnum, timeout);
            if (n == 0)
            {
                LOG(LogLevel::INFO) << "time out...";
            }
            else if (n < 0)
            {
                LOG(LogLevel::ERROR) << "epoll_wait error";
                break;
            }
            else // 就绪事件
            {
                EventsHandler(revs, n);
            }
        }
    }

    ~EpollServer()
    {
    }

private:
    uint16_t _port;
    // 监听socket
    std::unique_ptr<Socket> _listensockfd;
    // epoll 句柄
    int _epfd;
};