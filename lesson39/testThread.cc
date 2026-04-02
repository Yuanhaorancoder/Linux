#include <iostream>
#include <thread>
#include <unistd.h>

void hello()
{
    while(true)
    {
        std::cout<<"我是新线程...,pid: "<<getpid()<<std::endl;
        sleep(1);
    }
}

int main()
{
    std::thread t(hello);

    while(true)
    {
        std::cout<<"我是主线程...,pid: "<<getpid()<<std::endl;
        sleep(1);
    }

    t.join();
    return 0;
}








// #include <iostream>
// #include <pthread.h>
// #include <unistd.h>

// void* hello(void* args)
// {
//     const char *name=(const char*)args; 
//     while(true)
//     {
//         std::cout<<"我是新线程...,pid: "<<getpid()<<" name is: "<<name<<std::endl;
//         sleep(1);
//     }
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid,nullptr,hello,(void*)"new-thread");
    
//     while(true)
//     {
//         std::cout<<"我是主线程...,pid: "<<getpid()<<std::endl;
//         sleep(1);
//     }
//     return 0;
// }