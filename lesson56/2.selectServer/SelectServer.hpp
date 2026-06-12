#pragma once

#include <iostream>
#include <unistd.h>
#include <memory>
#include <sys/select.h>
#include "InetAddr.hpp"
#include "Logger.hpp"
#include "Mutex.hpp"
#include "Socket.hpp"

#define NUM (sizeof(fd_set)*8)
const int gdefaultfd =  -1;

using namespace LogModule;

class SelectServer
{
public:
    SelectServer(uint16_t port)
    :_port(port),
     _listensockfd(std::make_unique<TcpSocket>())
    {
        _listensockfd->BuildSocketMethod(_port);
        for(int i=0;i<NUM;i++)
            array_fds[i] = gdefaultfd;

        array_fds[0] = _listensockfd->Socketfd(); // 首先把_listensockfd放到辅助数组里
        
    }
    void HandlerEvent()
    {
        InetAddr clientaddr;
        int fd = _listensockfd->Accepter(&clientaddr); // 会不会被阻塞？不会
        LOG(LogLevel::INFO) << "get a new link: " << fd;
        // 得到了新连接，这个链接怎么处理？
        // recv(fd)？ 等+拷贝，不能 需要把fd托管给select，只有select才有 “等” 的能力 ——>只要把fd添加到辅助数组里即可
        if(fd >= 0)
        {
            int pos = 0;
            for(;pos < NUM;pos++)
            {
                if(array_fds[pos] == gdefaultfd)
                    break;
            }
            if(pos == NUM)
                close(fd);
            else
                array_fds[pos] = fd;
        }

    }
    void Start()
    {
        fd_set rfds; // read fd set
        while(true)
        {
            // rfds参数重置
            FD_ZERO(&rfds);
            int max_fd = gdefaultfd;
            for(int i=0;i<NUM;i++)
            {
                if(array_fds[i] == gdefaultfd) 
                    continue;
                FD_SET(array_fds[i],&rfds);
                if(max_fd < array_fds[i])
                    max_fd = array_fds[i];
            }

            // struct timeval timeout = {5,0}; // 每两秒等一次

            int n = select(max_fd+1,&rfds,nullptr,nullptr,nullptr);
            switch(n)
            {
                case 0:
                    LOG(LogLevel::DEBUG) << "time out...";
                    break;
                case -1:
                    LOG(LogLevel::DEBUG) << "select error";
                    break;
                default:
                    LOG(LogLevel::DEBUG) << "事件就绪...: n:" << n;
                    HandlerEvent();
                    break;
            }
        }
    }
    ~SelectServer()
    {}
private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensockfd;

    // select服务器，需要借助一个辅助数组
    int array_fds[NUM];
};