#pragma once

#include <iostream>
#include <unistd.h>
#include <memory>
#include <poll.h>
#include <sys/socket.h>
#include "InetAddr.hpp"
#include "Logger.hpp"
#include "Mutex.hpp"
#include "Socket.hpp"

#define NUM 1024
const int gdefaultfd = -1;

using namespace LogModule;

class PollServer
{
public:
    PollServer(uint16_t port)
        : _port(port),
          _listensockfd(std::make_unique<TcpSocket>())
    {
        _listensockfd->BuildSocketMethod(_port);
        for (int i = 0; i < NUM; i++)
        {
            _fdevent[i].fd = gdefaultfd;
            _fdevent[i].events = _fdevent[i].revents = 0;
        }

        // listen sockfd 添加到poll
        _fdevent[0].fd = _listensockfd->Socketfd(); // 首先把_listensockfd放到辅助数组里
        _fdevent[0].events |= POLLIN;
    }

    // 1.全部都是用来处理新连接到来的
    // 链接管理器
    void Accepter()
    {
        InetAddr clientaddr;
        int fd = _listensockfd->Accepter(&clientaddr); // 会不会被阻塞？不会
        LOG(LogLevel::INFO) << "get a new link: " << fd;
        // 得到了新连接，这个链接怎么处理？
        // recv(fd)？ 等+拷贝，不能 需要把fd托管给select，只有select才有 “等” 的能力 ——>只要把fd添加到辅助数组里即可
        if (fd >= 0)
        {
            int pos = 0;
            for (; pos < NUM; pos++)
            {
                if (_fdevent[pos].fd == gdefaultfd)
                    break;
            }
            if (pos >= NUM)
            {
                // poll这里，可以进行扩容
                LOG(LogLevel::WARNING) << "server is full!";
                close(fd);
            }
            else
            {
                _fdevent[pos].fd = fd;
                _fdevent[pos].events = POLLIN;
                _fdevent[pos].revents = 0;
            }    
        }
        else
        {
            LOG(LogLevel::ERROR) << "Accepter Error!"; // bug
        }
    }

    // IO处理器
    void IOHandler(int i)
    {
        // 普通的 fd _bug
        char buffer[1024];
        ssize_t n = recv(_fdevent[i].fd, buffer, sizeof(buffer) - 1, 0); // 会不会被阻塞? 不会！
        if (n > 0)
        {
            buffer[n] = 0;
            std::cout << "client say@ " << buffer << std::endl;
            ;

            // 直接写？还是交给 select 帮我关心写事件就绪
            std::string sendstring = "echo# ";
            sendstring += buffer;
            send(_fdevent[i].fd, sendstring.c_str(), sendstring.size(), 0);
        }
        else if (n == 0)
        {
            LOG(LogLevel::INFO) << "sockfd is close: " << _fdevent[i].fd;
            close(_fdevent[i].fd); // 1.关闭 fd
            _fdevent[i].events = _fdevent[i].revents = 0;
            _fdevent[i].fd = gdefaultfd;
        }
        else
        {
            LOG(LogLevel::ERROR) << "recv error!" << _fdevent[i].fd;
            close(_fdevent[i].fd); // 1.关闭 fd
            _fdevent[i].events = _fdevent[i].revents = 0;
            _fdevent[i].fd = gdefaultfd;
        }
    }

    void EventHandler()
    {
        // 就绪事件的fd,不止一个了
        for (int i = 0; i < NUM; i++)
        {
            if (_fdevent[i].fd == gdefaultfd)
                continue;
            // 合法的fd && 就绪
            if (_fdevent[i].revents & POLLIN )
            {
                if (_fdevent[i].fd == _listensockfd->Socketfd())
                {
                    // 这个 fd 是 listensockfd
                    Accepter();
                }
                else
                {
                    IOHandler(i);
                }
            }
        }
    }

    void PrintFds()
    {
        std::cout << "Poll Server fds list: ";
        for (int i = 0; i < NUM; i++)
        {
            if (_fdevent[i].fd == gdefaultfd)
                continue;
            std::cout << _fdevent[i].fd << " ";
        }
        std::cout << std::endl;
    }

    // 事件派发器 && 事件处理器
    void Dispatcher()
    {
        while (true)
        {
            int timeout = 2000; // 2s
            // For Test
            PrintFds();
            // 1.listen
            // 2.普通 fd
            int n = poll(_fdevent, NUM, timeout);
            switch (n)
            {
            case 0:
                LOG(LogLevel::DEBUG) << "time out...";
                break;
            case -1:
                LOG(LogLevel::DEBUG) << "poll error";
                break;
            default:
                LOG(LogLevel::DEBUG) << "事件就绪...: n:" << n; // 任意一个 fd 都可能时间就绪
                EventHandler();
                break;
            }
        }
    }
    ~PollServer()
    {
    }

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensockfd;

    struct pollfd _fdevent[NUM];
    // struct pollfd *_fdevent;
};