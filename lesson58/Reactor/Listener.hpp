// 获取新连接

#pragma once

#include <iostream>
#include <string>
#include <memory>
#include "Socket.hpp"
#include "Logger.hpp"
#include "Connection.hpp"

// 链接管理器（listensockfd） 先描述
class Listener : public Connection
{
public:
    Listener(uint16_t port)
    : _port(port),
      _listensock(std::make_unique<TcpSocket>())
    {
        _listensock->BuildSocketMethod(_port);
    }

    int Sockfd() override
    {
        return _listensock->Socketfd();
    }

    void Recver() override
    {
        // _listensock->Accepter()
    }
    void Sender() override
    {

    }
    // 异常处理
    void Excepter() override
    {

    }
private:    
    uint16_t _port;
    std::unique_ptr<Socket> _listensock;
};
