#include "ThreadPool.hpp"
#include <memory>

int main()
{
    ENABLE_CONSOLE_LOG_STRATEGY();

    std::unique_ptr<ThreadPool> tp = std::make_unique<ThreadPool>();
    tp->Start();

    while(true)
    {
        tp->Enqueue(task);
    }

    tp->Stop();

    tp->Wait();
    
    return 0;
}