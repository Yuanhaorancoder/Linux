#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#include "Mutex.hpp"

// 全局票量
int tickets = 1000;
// Mutex lock;
// std::mutex lock;
pthread_mutex_t glock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gcond=PTHREAD_COND_INITIALIZER;

void buyTicket(int id) 
{
    while (true) 
    {
        // 临时变量，进入到该循环，实现构造方法即加锁，该锁生命周期结束，实现析构即解锁
        // LockGuard lockGuard(&lock);  // RAII风格的枷锁逻辑
        // C++的
        // std::lock_guard<std::mutex> lock(&lock);
        // 加锁：保护共享资源的原子操作
        // lock.Lock();
        pthread_mutex_lock(&glock);
        pthread_cond_wait(&gcond,&glock);

        if (tickets > 0) 
        {
            usleep(1000);
            // 购票并减少余票（--tickets 是先减后输出，符合剩余票逻辑）
            std::cout << "Thread " << id << " bought ticket, remaining: " << --tickets << std::endl;
            // 解锁：操作完成后释放锁
            // lock.Unlock();
            pthread_mutex_unlock(&glock);
        } 
        else 
        {
            // 余票为0时，解锁并退出循环
            // lock.Unlock();
            pthread_mutex_unlock(&glock);
            break;
        }
    }
}

int main() {
    // 定义4个售票线程
    std::thread threads[4];
    
    // 启动4个线程
    for (int i = 0; i < 4; ++i) 
    {
        threads[i] = std::thread(buyTicket, i + 1);
    }
    
    sleep(3);

    while(true)
    {
        std::cout<<"wake up ..."<<std::endl;
        // 唤醒一个
        // pthread_cond_signal(&gcond);

        // 全部唤醒
        pthread_cond_broadcast(&gcond);
        sleep(1);
    }

    // 等待所有线程执行完毕
    for (auto& t : threads) 
    {
        t.join();
    }
    
    std::cout << "All tickets sold out!" << std::endl;
    
    return 0;
}