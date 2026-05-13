#ifndef __TCPSERVER_HPP
#define __TCPSERVER_HPP

#include <iostream>
#include <string>
#include <memory>
#include <signal.h>
#include <functional>
#include "Socket.hpp"

using handler_t = std::function<std::string(std::string &)>;

class TcpServer
{
public:
    TcpServer(uint16_t port,handler_t handler)
        :_port(port),
         _listensockfd(std::make_unique<TcpSocket>()),
         _isrunning(false),
         _handlerstream(handler)
    {
        _listensockfd->BuildSocketMethod(_port); // 模板方法模式
    }

    void Run()
    {
        _isrunning = true;
        signal(SIGCHLD,SIG_IGN);
        while(_isrunning)
        {
            InetAddr clientaddr;
            auto sockfd = _listensockfd->Accepter(&clientaddr); // 1.clientaddredd 2.socket
            if(!sockfd)
            {
                LOG(LogLevel::WARNING) << "accept error";
                continue;
            }
            LOG(LogLevel::DEBUG) << "client addr: " << clientaddr.StringAddress() << " sockfd:" << sockfd->Socketfd();
        
            // 处理链接的问题
            if(fork() == 0)
            {
                // 子进程
                _listensockfd->Close();
                HanderIO(sockfd,clientaddr);
                exit(0);
            }
            // 父进程
            sockfd->Close();
        }
    }

    void HanderIO(std::shared_ptr<Socket> sockfd,InetAddr clientaddr)
    {
        // 长服务
        std::string inbuffer;
        while(true)
        {
            int n = sockfd->Recv(&inbuffer); // +=
            if(n < 0)
            {
                LOG(LogLevel::WARNING) << "recv error";
                break;
            }
            if(n == 0)
            {
                LOG(LogLevel::INFO) << "client quit: " << clientaddr.StringAddress() << " sockfd:" << sockfd->Socketfd();
                break; 
            }

            LOG(LogLevel::DEBUG) << inbuffer;

            // 1.分析inbudder报文完整性
            // 2. a.完整的报文:处理，提取 b.不完整：什么都不做
            // 3.这个工作谁做？--协议 Tcpserver不做
            std::string outbuffer;
            if(_handlerstream)
                outbuffer = _handlerstream(inbuffer); // 回调函数会调出去，也会再回来
            if(!outbuffer.empty())
                sockfd->Send(outbuffer);
        }
        sockfd->Close();
    }
    ~TcpServer()
    {}
private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensockfd;
    bool _isrunning;
    handler_t _handlerstream;
};

#endif