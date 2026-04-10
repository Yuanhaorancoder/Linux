#ifndef __THREAD_HPP
#define __THREAD_HPP
#include <iostream>
#include <string>
#include <functional>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h> /* Definition of SYS_* constants */

using func_t = std::function<void()>;

// bug
static int gnum=1; 

class Thread
{
private:
    void getprocessid()
    {
        _pid=getpid();
    }
    void getlwp()
    {
        _lwpid = syscall(SYS_gettid);
    }
    static void *routine(void *args)
    {
        Thread *ts = static_cast<Thread*>(args);
        ts->getprocessid();
        ts->getlwp();
        ts->_func();

        return nullptr;
    }
public:
    Thread(func_t f):_func(f),_joinable(true)
    {
        _name="thread-" + std::to_string(gnum++); 
    }
    void start()
    {
        int n=pthread_create(&_tid,nullptr,routine,this);
        (void)n;
    }
    void stop()
    {
        int n=pthread_cancel(_tid);
        (void)n;
    }
    void join()
    {
        if(_joinable)
        {
            int n=pthread_join(_tid,nullptr);
            (void)n;
            printf("lwp: %d, name: %s, join sucess\n",_lwpid,_name.c_str());
        }
    }
    void detach()
    {   
        if(_joinable)
        {
            _joinable=false;
            int n=pthread_detach(_tid);
            (void)n;
        }
    }
    ~Thread()
    {}
private:
    pthread_t _tid;
    pid_t _pid;
    pid_t _lwpid;
    std::string _name;
    bool _joinable;

    func_t _func;
};

#endif  