#pragma once 

#include <iostream>
#include <vector>
#include <queue>
#include "Thread.hpp"
#include "Logger.hpp"
#include "Mutex.hpp"
#include "Cond.hpp"

static const int gnum = 5;

using namespace LogModule;

#if 0
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
#endif

template<typename T>
class ThreadPool
{
private:
    bool IsTaskQueueEmpty()
    {
        return _queue.empty();
    }

    T PopHelper()
    {
        T t = _queue.front();
        _queue.pop();
        return t;
    }
    
    void ThreadRoutine() // 线程执行
    {
        char name[64];
        pthread_getname_np(pthread_self(),name,sizeof(name));
        
        while(true)
        {
            T task;
            {
                // 临界区
                LockGuard lockguard(&_lock);
                // 1.没有任务 && 线程池运行不退出 -> 允许休眠
                while(IsTaskQueueEmpty() && _isrunning)
                {
                    _sleeper_cnt++;
                    LOG(LogLevel::DEBUG) << "没有任务，线程休眠: |" << name << "|";
                    _cond.Wait(_lock); 
                    LOG(LogLevel::DEBUG) << "有任务，线程唤醒: |" << name << "|";
                    _sleeper_cnt--;
                }
                // 2.没有任务 && 线程池不运行退出 -> 线程结束
                if(IsTaskQueueEmpty() && !_isrunning)
                {
                    LOG(LogLevel::INFO) << "Thread: " << name << " quit";
                    break;
                }
                // 3.有任务 && 线程池退出 ，不关心线程有没有退出
                // 4.有任务 && 线程池不退出 ，不关心线程有没有退出
                // 目前：一定是有任务的
                task = PopHelper();
            }
            task(); // 处理任务不应该在临界区内部处理，获取任务之后，任务已经从公有变成私有了
        }
    }
public:
    ThreadPool(int num = gnum):_num(num),_isrunning(false),_sleeper_cnt(0)
    {
        for(int i=0;i<_num;i++)
        {
            _threads.emplace_back([this](){
                this->ThreadRoutine();
            });
        }
    }
    void Start()
    {
        LockGuard lockguard(&_lock);
        if(_isrunning) 
            return;
        _isrunning = true;
        for(auto &thread:_threads)
            thread.start();
    }
    void Enqueue(const T &task)
    {
        LockGuard lockguard(&_lock);
        if(!_isrunning) // 当线程池没有运行，禁止push任务
            return;
        _queue.push(task);
        if(_sleeper_cnt > 0) 
            _cond.NotifyOne();
    }
    void Stop()
    {
        LockGuard lockguard(&_lock);
        if(_isrunning)
        {
            LOG(LogLevel::DEBUG) << "关闭线程池";
            _isrunning = false;
            if(_sleeper_cnt > 0)
                _cond.NotifyAll();
            // for(auto &thread:_threads)
            //     thread.stop();
        }
    }
    void Wait()
    {
        for(auto &thread:_threads)
            thread.join();
    }
    ~ThreadPool(){}
private:
    std::vector<Thread> _threads; // 所有线程
    int _num;
    bool _isrunning;
    // int _status; // running, stop, quit
    int _sleeper_cnt; // 休眠的线程的个数

    std::queue<T> _queue;
    Mutex _lock;
    Cond _cond;

}; 