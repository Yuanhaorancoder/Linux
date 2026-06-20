#pragma once

// 帮我们监听所有的fd是否就绪

#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/epoll.h>
#include "Logger.hpp"
#include "Common.hpp"

static const int gsize = 128;
// #define IN EPOLLIN
// #define OUT EPOLLOUT

using namespace LogModule;

class Poller
{
private:
    int EpollCtlHelper(int sockfd,uint32_t events,int oper)
    {
        if(oper == EPOLL_CTL_DEL)
        {
            return epoll_ctl(_epfd,oper,sockfd,nullptr);    
        }

        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = sockfd;
        return epoll_ctl(_epfd,oper,sockfd,&ev);
    }
public:
    Poller()
    {
        _epfd = epoll_create(gsize);
        if(_epfd < 0)
        {
            LOG(LogLevel::FATAL) << "epoll_create error!";
            exit(EPOLL_ERR);
        }
        LOG(LogLevel::INFO) << "epoll_create success: " << _epfd;
    }
    void DelEvents(int sockfd)
    {
        EpollCtlHelper(sockfd,0,EPOLL_CTL_DEL);
    }
    void AddEvents(int sockfd,uint32_t events)
    {
        int n = EpollCtlHelper(sockfd,events,EPOLL_CTL_ADD);
        if(n < 0)
        {
            LOG(LogLevel::FATAL) << "AddEvents error!";
        }
    }
    void ModEvents(int sockfd,uint32_t events)
    {
        int n = EpollCtlHelper(sockfd,events,EPOLL_CTL_MOD);
        if(n < 0)
        {
            LOG(LogLevel::FATAL) << "ModEvents error!";
        }
    }
    int WaitEvents(struct epoll_event revs[],int num,int timeout)
    {
        int n = epoll_wait(_epfd,revs,num,timeout);
        if(n < 0)
        {
            LOG(LogLevel::FATAL) << "epoll_wait error!";
        }
        else if(n == 0)
        {
            LOG(LogLevel::INFO) << "epoll_wait timeout!";
        }
        return n;
    }

    ~Poller()
    {

    }
private:
    int _epfd;
};

// class Poller
// {
// public:
//     virtual bool create() = 0;
//     virtual bool Destory() = 0;
//     virtual void GetEvents() = 0;
//     virtual void SetFdEvent() = 0;
// private:
// };

// class SleectPoller : Poller
// {

// };

// class PollPoller : Poller
// {

// };

// class EpollPoller : Poller
// {

// };