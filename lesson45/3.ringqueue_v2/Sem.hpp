#pragma once

#include <iostream>
#include <pthread.h>
#include <semaphore.h>

class Semaphore
{
public:
    Semaphore(int num):_init(num)
    {
        sem_init(&_sem,0,_init);
    }
    void P()
    {
        sem_wait(&_sem);
    }
    void V()
    {
        sem_post(&_sem);
    }
    ~Semaphore()
    {
        sem_destroy(&_sem);
    }
private:
    sem_t _sem;
    int _init;
};