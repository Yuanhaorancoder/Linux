#pragma once

#include <iostream>
#include <queue>
#include <pthread.h>

static const int gdefaultcap=5;

template<class T>
class BlockQueue
{
public:
    BlockQueue(int cap = gdefaultcap):_cap(cap),_productor_sleeper_cnt(0),_consumer_sleeper_cnt(0),_low_water(cap/3),_high_water(cap/3*2)
    {
        pthread_mutex_init(&_mutex,nullptr);
        pthread_cond_init(&_cond_productor,nullptr);
        pthread_cond_init(&_cond_consumer,nullptr);
    }
    void Push(T in)
    {
        // 生产
        pthread_mutex_lock(&_mutex);
        // 1.判断是否满，本身页也是临界区 --- 
        // 2.那么，判断逻辑，也必然会在加锁和解锁之间
        // 3.那么判断结果的处理，也必然在临界区内部，
        while(_queue.size()==_cap) // bug
        {
            // 4.等待的核心操作，就是把线程挂起到cond的等待队列中！
            // 结论:pthread_cond_wait 释放第二个参数对应的锁
            _productor_sleeper_cnt++;
            // a.函数调用失败，导致休眠失败 b.伪唤醒问题
            pthread_cond_wait(&_cond_productor,&_mutex); // 锁参数的理解
            _productor_sleeper_cnt--;
            // 5.当线程被唤醒，也必然是在临界区内部被唤醒
            // 结论:pthread_cond_wait 对应的线程在被唤醒的时候，自动重新申请锁，锁申请成功，才会返回
        }
        _queue.push(in);

        // 生产一个，就可以唤醒消费者
        // if(_queue.size() > _high_water && _consumer_sleeper_cnt > 0)
        //     pthread_cond_signal(&_cond_consumer); //？
        pthread_cond_signal(&_cond_consumer);
        pthread_mutex_unlock(&_mutex);
    }
    void Pop(T *out)
    {
        pthread_mutex_lock(&_mutex);

        while(_queue.empty())
        {
            _consumer_sleeper_cnt++;
            pthread_cond_wait(&_cond_consumer,&_mutex);
            _consumer_sleeper_cnt--;
        }

        *out=_queue.front();
        _queue.pop();

        // 消费一个，就可以唤醒生产者了
        // if(_queue.size() < _low_water && _productor_sleeper_cnt > 0)
        //     pthread_cond_signal(&_cond_productor); // 放在加锁之前，之后都是可以的
        pthread_cond_signal(&_cond_productor);
        pthread_mutex_unlock(&_mutex);
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond_productor);
        pthread_cond_destroy(&_cond_consumer);
    }
private:
    
    unsigned int _cap;
    int _productor_sleeper_cnt;
    int _consumer_sleeper_cnt;

    unsigned int _low_water; // 有效数据太少了
    unsigned int _high_water; // 有效数据太多了

    std::queue<T> _queue; // 临界资源
    pthread_mutex_t _mutex;
    pthread_cond_t _cond_productor; // 阻塞队列满了，生产者等待
    pthread_cond_t _cond_consumer; // 阻塞队列空了，消费者等待
};