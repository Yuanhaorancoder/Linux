#pragma once 

#include <iostream>
#include <vector>
#include "Thread.hpp"
#include "Logger.hpp"

static const int gnum = 5;

using namespace LogModule;

void DefaultRun()
{
    char name[64];
    pthread_getname_np(pthread_self(),name,sizeof(name));
    while(true)
    {
        LOG(LogLevel::DEBUG) << name << "线程执行默认方法";
        sleep(1);
    }
}

class ThreadPool
{
public:
    ThreadPool(int num = gnum):_num(num)
    {
        for(int i=0;i<_num;i++)
        {
            _treads.emplace_back(DefaultRun);
        }
    }
    void Start()
    {

    }
    void Enqueue()
    {

    }
    void Stop()
    {

    }
    void Wait()
    {
        
    }
    ~ThreadPool(){}
private:
    std::vector<Thread> _treads; // 所有线程
    int _num;
};