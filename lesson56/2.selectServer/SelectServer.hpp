#pragma once

#include <iostream>
#include <unistd.h>
#include <memory>
#include <sys/select.h>
#include <sys/socket.h>
#include "InetAddr.hpp"
#include "Logger.hpp"
#include "Mutex.hpp"
#include "Socket.hpp"

#define NUM (sizeof(fd_set) * 8)
const int gdefaultfd = -1;

using namespace LogModule;

class SelectServer
{
public:
    SelectServer(uint16_t port)
        : _port(port),
          _listensockfd(std::make_unique<TcpSocket>())
    {
        _listensockfd->BuildSocketMethod(_port);
        for (int i = 0; i < NUM; i++)
            array_fds[i] = gdefaultfd;

        array_fds[0] = _listensockfd->Socketfd(); // 首先把_listensockfd放到辅助数组里
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
                if (array_fds[pos] == gdefaultfd)
                    break;
            }
            if (pos >= NUM)
            {
                LOG(LogLevel::WARNING) << "server is full!";
                close(fd);
            }
            else
                array_fds[pos] = fd;
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
        ssize_t n = recv(array_fds[i], buffer, sizeof(buffer) - 1, 0); // 会不会被阻塞? 不会！
        if (n > 0)
        {
            buffer[n] = 0;
            std::cout << "client say@ " << buffer << std::endl;
            ;

            // 直接写？还是交给 select 帮我关心写事件就绪
            std::string sendstring = "echo# ";
            sendstring += buffer;
            send(array_fds[i], sendstring.c_str(), sendstring.size(), 0);
        }
        else if (n == 0)
        {
            LOG(LogLevel::INFO) << "sockfd is close: " << array_fds[i];
            close(array_fds[i]); // 1.关闭 fd
            array_fds[i] = gdefaultfd;
        }
        else
        {
            LOG(LogLevel::ERROR) << "recv error!" << array_fds[i];
            close(array_fds[i]);       // 1.关闭 fd
            array_fds[i] = gdefaultfd; // 2.取消 select 关心
        }
    }

    void EventHandler(fd_set &rfds)
    {
        // 就绪事件的fd,不止一个了
        for (int i = 0; i < NUM; i++)
        {
            if (array_fds[i] == gdefaultfd)
                continue;
            // 合法的fd && 就绪
            if (FD_ISSET(array_fds[i], &rfds))
            {
                if (array_fds[i] == _listensockfd->Socketfd())
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
        std::cout << "Select Server fds list: ";
        for (int i = 0; i < NUM; i++)
        {
            if (array_fds[i] == gdefaultfd)
                continue;
            std::cout << array_fds[i] << " ";
        }
        std::cout << std::endl;
    }

    // 事件派发器 && 事件处理器
    void Dispatcher()
    {
        fd_set rfds; // read fd set
        while (true)
        {
            // rfds参数重置
            FD_ZERO(&rfds);
            int max_fd = gdefaultfd;
            for (int i = 0; i < NUM; i++)
            {
                if (array_fds[i] == gdefaultfd)
                    continue;
                FD_SET(array_fds[i], &rfds);
                if (max_fd < array_fds[i])
                    max_fd = array_fds[i];
            }

            // struct timeval timeout = {5,0}; // 每两秒等一次
            PrintFds();
            // 1.listen
            // 2.普通 fd
            int n = select(max_fd + 1, &rfds, nullptr, nullptr, nullptr);
            switch (n)
            {
            case 0:
                LOG(LogLevel::DEBUG) << "time out...";
                break;
            case -1:
                LOG(LogLevel::DEBUG) << "select error";
                break;
            default:
                LOG(LogLevel::DEBUG) << "事件就绪...: n:" << n; // 任意一个 fd 都可能时间就绪
                EventHandler(rfds);
                break;
            }
        }
    }
    ~SelectServer()
    {
    }

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensockfd;

    // select服务器，需要借助一个辅助数组
    int array_fds[NUM];
};