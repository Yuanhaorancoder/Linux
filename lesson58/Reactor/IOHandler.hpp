// 处理普通fd
#pragma once

#include <iostream>
#include <string>
#include "Logger.hpp"
#include "Connection.hpp"

using namespace LogModule;

static const int gbuffersize = 1024;

using OnMessage_t = std::function<std::string (std::string &,int *code)>;

// IO处理器（普通fd） 先描述
// 只负责数据读取和发送
class IOHandler : public Connection
{
public:
    IOHandler(int sockfd, OnMessage_t on_message)
    :_sockfd(sockfd),
     _on_message(on_message)
    {}
    int Sockfd() override
    {
        return _sockfd;
    }
    void Close() override
    {
        close(_sockfd);
    }
    void Recver() override
    {
        Updata();
        LOG(LogLevel::INFO) << "IOHandler event ready, sockfd is : " << _sockfd;
        // 你必须循环式的把本次数据全部读取完毕
        while(true)
        {
            char buffer[gbuffersize];
            ssize_t n = recv(_sockfd,buffer,gbuffersize-1,0);
            if(n > 0)
            {
                buffer[n] = 0;
                _inbuffer += buffer;
            }
            else if(n == 0)
            {
                LOG(LogLevel::INFO)<<"client quit,address is: "<< _clientaddr.StringAddress()<<" sockfd: "<< _sockfd;
                Excepter();
                return; // 注意
            }
            else
            {
                // 不一定是出错了！
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    break;
                }
                else if(errno == EINTR)
                {
                    continue;
                }
                else
                {
                    // 真正读取错误
                    LOG(LogLevel::ERROR)<<"recv error,address is: "<< _clientaddr.StringAddress()<<" sockfd: "<< _sockfd;
                    Excepter();
                    return;
                }
            }
        }
        // 尝试检查，(收到的数据，是否有至少一个完整的报文? 如果有，你要提取，然后交给上层进行反序列化处理)这些工作，
        // 不应该让IOHandler来做，而应该结合协议来做
        // 如果没有，什么都不做

        int code = 0;
        std::string result = _on_message(_inbuffer,&code);
        if(code == 0)
        {
            _outbuffer += result; // result:string(),result:response
        }
        else
        {
            Excepter();
            return;
        }

        // 发送
        // version1
        // if(!_outbuffer.empty())
        //     Sender();
        // version2
        if(!_outbuffer.empty())
            _R->EnableReadWrite(_sockfd,true,true); // 使能写事件关心
    }
    void Sender() override
    {
        Updata();
        while(true)
        {
            ssize_t n = send(_sockfd,_outbuffer.c_str(),_outbuffer.size(),0);
            if(n >= 0)
            {
                _outbuffer.erase(0,n);
                if(_outbuffer.empty())
                {
                    break;
                }
            }
            else
            {
                // 是真的报错了吗? -- 不一定
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    // 本轮数据发送完毕了
                    break;
                }
                else if(errno == EINTR)
                {
                    continue;
                }
                else
                {
                    LOG(LogLevel::ERROR)<<"send error,address is: "<< _clientaddr.StringAddress()<<" sockfd: "<< _sockfd;
                    Excepter();
                    return;
                }
            }
        }

        // 一定没有错误
        // 1.刚好发完 2.没发完
        if(_outbuffer.empty())
            _R->EnableReadWrite(_sockfd,true,false);
        else // 没法玩？发送条件不满足了，对conn，修改sockfd关心的事件，epoll->EPOLLOUT
            _R->EnableReadWrite(_sockfd,true,true);
    }
    // 异常处理
    void Excepter() override
    {
        LOG(LogLevel::ERROR) << "Excepter, address is: " << _clientaddr.StringAddress() << " sockfd: " << _sockfd;
        _R->DelConnection(_sockfd);
    }
private:
    int _sockfd;
    OnMessage_t _on_message;
    std::string _inbuffer;  // 接受缓冲区
    std::string _outbuffer; // 输出缓冲区
};