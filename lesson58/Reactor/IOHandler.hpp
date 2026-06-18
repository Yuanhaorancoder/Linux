// 处理普通fd
#pragma once

#include <iostream>
#include <string>
#include "Logger.hpp"
#include "Connection.hpp"

// IO处理器（普通fd） 先描述
class IOHandler : public Connection
{
public:
    IOHandler()
    {}
    int Sockfd() override
    {
        return _sockfd;
    }

    void Recver() override
    {}
    void Sender() override
    {}
    // 异常处理
    void Excepter() override
    {}
private:
    int _sockfd;
};