#include <iostream>
#include <cstdio> 
#include <unistd.h>
#include <signal.h>

void PrintPending(sigset_t &pending)
{
    // 0000 0000 .... 0000 -> 0000 0000 0000 0010
    for(int signum=31;signum>=1;signum--)
    {
        // 判断当前信号在信号集，打印 "1" 否则 "0"
        if(sigismember(&pending,signum))
            printf("1");
        else
            printf("0");
    } 
    printf("\n");
}

void handler(int signum)
{
    std::cout<<"get a signal: "<<signum<<std::endl;

    // 在处理2号信号期间，2号信号被自动block了！
    while(true)
    {
        sigset_t pending;
        sigpending(&pending);

        PrintPending(pending);
        sleep(1);
    }

    // exit(0);
}

int main()
{
    struct sigaction act,old_act;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&(act.sa_mask)); // ???,如果进程收到了大量的重复信号？？
    sigaddset(&(act.sa_mask),1);
    sigaddset(&(act.sa_mask),3);
    sigaddset(&(act.sa_mask),4);
    sigaddset(&(act.sa_mask),5);

    sigaction(2,&act,&old_act); // signal

    while(true)
    {
        pause();
    }
}


// void PrintPending(sigset_t &pending)
// {
//     // 0000 0000 .... 0000 -> 0000 0000 0000 0010
//     for(int signum=31;signum>=1;signum--)
//     {
//         // 判断当前信号在信号集，打印 "1" 否则 "0"
//         if(sigismember(&pending,signum))
//             printf("1");
//         else
//             printf("0");
//     } 
//     printf("\n");
// }

// void handler(int signo)
// {
//     std::cout<<"处理了："<<signo<<" 信号"<<std::endl;
//     std::cout<<"#####################"<<std::endl;

//     sigset_t pending;
//     // sigemptyset(&pending);
//     sigfillset(&pending); 
//     int m=sigpending(&pending);
//     (void)m;

//     //打印
//     PrintPending(pending);

//     std::cout<<"#####################"<<std::endl;
// }

// int main()
// {
//     // 0.自定义捕捉2号信号
//     signal(SIGINT,handler);

//     std::cout<<"pid: "<<getpid()<<std::endl;
//     // 1.定义&&初始化信号集
//     sigset_t block,old_block;
//     sigemptyset(&block);
//     sigemptyset(&old_block); 

//     // 把所有信号添加到block信号集里面
//     for(int signum=1;signum<=31;signum++)
//         sigaddset(&block,signum);

//     // 2.向block信号集添加信号
//     // sigaddset(&block,SIGINT); // 有没有设置到内核中，有没有设置到当前进程内部？没有！！！定义到栈上了

//     // 3.屏蔽二号信号
//     int n = sigprocmask(SIG_SETMASK,&block,&old_block);
//     (void)n;

//     // 4.不断获取panding信号集&&打印
//     int cnt=0;
//     sigset_t pending;
//     while(true)
//     {
//         sigemptyset(&pending);
//         // 获取pending信号集
//         int m=sigpending(&pending);
//         (void)m;

//         // 打印
//         PrintPending(pending); 

//         sleep(1);

//         // cnt++;

//         // // 尝试恢复
//         // if(cnt==20)
//         // {
//         //     std::cout<<"恢复2号信号，解除屏蔽"<<std::endl;
//         //     sigprocmask(SIG_SETMASK,&old_block,nullptr);
//         // }
//     }
//     return 0;
// }