#include "Task.hpp"
#include "ThreadPool.hpp"
#include <memory>

int main()
{
    ENABLE_CONSOLE_LOG_STRATEGY();

    LOG(LogLevel::DEBUG) << "进程已经跑了很久了";
    sleep(3);

    // std::unique_ptr<ThreadPool<task_t> > tp = std::make_unique<ThreadPool<task_t> >();
    ThreadPool<task_t>::GetInstance()->Start();

    int cnt=10;
    while(cnt--)
    {
        LOG(LogLevel::DEBUG) << "-----------------------" << cnt;
        sleep(1); 
        ThreadPool<task_t>::GetInstance()->Enqueue(task1);

        sleep(1);
        ThreadPool<task_t>::GetInstance()->Enqueue(task2);
    }

    ThreadPool<task_t>::GetInstance()->Stop();

    // sleep(3);

    ThreadPool<task_t>::GetInstance()->Wait();
    
    return 0;
}