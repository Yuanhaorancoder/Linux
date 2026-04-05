#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include "Task.hpp"

// const int gsize=64;

// // 任何线程异常，都会导致进程退出，join异常无法正确返回，没有意义！
// void *threadrun(void *args)
// {
//     std::string name=static_cast<const char *>(args);
//     int cnt=5;
//     while (true)
//     {
//         printf("我是一个新线程:tid: 0x%lx,pid: %d,name: %s,cnt: %d\n",
//                 pthread_self(), getpid(),name.c_str(),cnt);
//         cnt--;
//         sleep(1);
//         pthread_cancel(pthread_self());
//         // return nullptr;
//         // pthread_exit(nullptr);
//     }
//     // return (void*)10;
//     pthread_exit((void*)100);
//     // return nullptr;
//     // 新进程退出：
//     // 只要自己的线程函数跑完，线程就自然退出了 --调用return，表示线程退出
//     // return nullptr;
//     // pthread_exit(nullptr);
//     // exit(3); // 不能用来终止线程，它是用来终止进程的！多线程中，任意一个线程调用exit，都表示整个进程退出
// }

// int main()
// {
//     pthread_t tid;
//     char threadname[gsize];
//     snprintf(threadname,gsize,"thread-%d",1);

//     pthread_create(&tid, nullptr, threadrun, (void*)threadname);

//     // PTHREAD_CANCELED;
//     sleep(1);
//     // int n=pthread_cancel(tid);
//     // printf("cancel new thread done,n: %d\n",n);

//     void *ret=nullptr;
//     pthread_join(tid,&ret); // 如果新线程不退出，join：一直阻塞等待下去

//     printf("join %lx success,ret code: %lld\n",tid,(long long)ret);

//     return 0;
// }

const int gsize=64;

// void *threadrun(void *args)
// {
//     // Task *t = static_cast<Task *>(args);
//     // sleep(1);
//     // (*t)();
//     // sleep(1);

//     int cnt=5;
//     while (cnt--)
//     {
//         sleep(1);
//         // printf("我是一个新线程:tid: 0x%lx,pid: %d,name: %s\n", pthread_self(), getpid(),name.c_str());
//         // sleep(1);
//     }
//     return nullptr;
// }

void *threadrun(void *args)
{
    Task *t = static_cast<Task *>(args);
    t->Excute();
    return t;
}

// ./createThread num
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << argv[0] << " num" << std::endl;
        return 1;
    }

    int num = std::stoi(argv[1]); // 将字符串转化为数字
    std::vector<pthread_t> tids;
    for (int i = 0; i < num; i++)
    {
        sleep(1);
        // 如果我们要创建多线程呢？
        pthread_t tid;
        // char threadname[gsize];
        // char *threadname= new char[gsize]; // 开辟堆空间
        char threadname[gsize];
        snprintf(threadname,gsize,"thread-%d",i+1);

        Task *t=new Task(threadname,10+i,20*i);

        pthread_create(&tid, nullptr, threadrun, t);
        tids.push_back(tid);
        std::cout<<"create thread "<<threadname<<"done"<<std::endl;
        // sleep(1);
    }

    std::vector<Task*> result_list;
    for(auto &tid:tids)
    {
        Task *t=nullptr;
        pthread_join(tid,(void **)&t);
        result_list.push_back(t);
        std::cout<<"join sucess: "<<tid<<std::endl;
    }
    
    std::cout<<"处理结果清单: "<<std::endl;
    for(auto &res:result_list)
    {
        std::cout<<res->Result()<<std::endl;
    }

    // sleep(10);
    // for(auto &tid:tids)
    // {
    //     printf("创建新线程成功,new tid: 0x%lx,main tid: %lu,pid: %d\n", tid, pthread_self(), getpid());
    // }

    // // 主线程
    // while (true)
    // {
    //     std::cout<<"main thread running..."<<std::endl;
    //     sleep(1);
    // }

    return 0;
}