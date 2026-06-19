#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include "Connection.hpp"
#include "Logger.hpp"
#include "Poller.hpp"

static const int gnum = 128;

class Reactor
{
private:
    bool IsConnectionExists(int sockfd)
    {
        return _connections.find(sockfd) != _connections.end();
    }

public:
    Reactor()
        : _epoller(std::make_unique<Poller>())
    {
    }
    // 新增Connection接口
    void AddConnection(std::shared_ptr<Connection> &conn)
    {
        // 1.conn->sockfd && event 写透到内核
        int sockfd = conn->Sockfd();
        uint32_t events = conn->Events();
        _epoller->AddEvents(sockfd, events);
        // 2.conn托管给_connections
        _connections[sockfd] = conn;
        // 3.conn回指Reactor;
        conn->_R = this; // 指向当前对象

        LOG(LogLevel::INFO) << "insert " << conn->Sockfd() << " into Reactor!";
    }

    void EnableReadWrite(int sockfd,bool isread,bool iswrite)
    {
        if(!IsConnectionExists(sockfd)) return;
        
    }

    void LoopOnce(int timeout)
    {
        int n = _epoller->WaitEvents(revs, gnum, timeout);
        for (int i = 0; i < n; i++)
        {
            // 哪一个fd
            int sockfd = revs[i].data.fd;
            // 哪些事件就绪了
            uint32_t revents = revs[i].events;

            if ((revents & EPOLLERR) || (revents & EPOLLHUP))
            {
                // if(IsConnectionExists(sockfd))
                //     _connections[sockfd]->Excepter();
                revents = (EPOLLIN | EPOLLOUT); // 把错误转化成为读写
            }

            // 事件就绪
            if ((revents & EPOLLIN) && IsConnectionExists(sockfd)) // 读事件就绪 && sockfd存在
            {
                // 我们还用区分是listenfd？还是普通的fd吗？ 不用了！
                _connections[sockfd]->Recver();
            }
            if ((revents & EPOLLOUT) && IsConnectionExists(sockfd)) // 写事件就绪 && sockfd存在
            {
                _connections[sockfd]->Sender();
            }
        }
    }

    void DebugPrint()
    {
        std::cout << "Reactor dsockfd list: ";
        for(auto &conn : _connections)
        {
            std::cout << conn.second->Sockfd() << " ";
        }
        std::cout << std::endl;
    }

    void Dispatcher()
    {
        int timeout = -1;
        while (true)
        {
            DebugPrint();
            LoopOnce(timeout);
        }
    }

    ~Reactor()
    {
    }

private:
    // 1.epoller 模型
    std::unique_ptr<Poller> _epoller; // 我们从epoll这里拿到的之后，哪一个fd的哪些时间就绪！
    // 2.组织connection
    std::unordered_map<int, std::shared_ptr<Connection>> _connections; // 服务器内所有的connection
    // 3.已经就绪的事件清单
    struct epoll_event revs[gnum];
};
