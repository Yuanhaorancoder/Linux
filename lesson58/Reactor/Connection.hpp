#pragma once

// 每一个fd，后续都对应一个connection链接
#include <iostream>
#include <string>
#include "InetAddr.hpp"

// 基类！！ 先描述
class Connection
{
public:
    Connection()
    : _events(0)
    {

    }
    uint32_t Events() {return _events;}
    void SetEvents(uint32_t events) { _events = events;}
    virtual int Sockfd() = 0;
    virtual void Recver() = 0;
    virtual void Sender() = 0;
    virtual void Excepter() = 0; // 异常处理

    ~Connection()
    {

    }
protected:
    std::string _inbuffer;  // 接受缓冲区
    std::string _outbuffer; // 输出缓冲区

    InetAddr _clientaddr;   // 客户端地址（谁连接的我服务器）

    uint32_t _events;       // Connection 关心什么事件
};