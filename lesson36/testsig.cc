#include <iostream>
#include <unistd.h>
#include <signal.h>

int cnt = 0;

void handlersig(int sig)
{
    // switch(sig)
    // {
    //     case 1:
    //     break;
    //     case 2:
    //     break;
    //     case 3:
    //     break;
    //     ....
    // }
    std::cout << "哈哈，我正在处理一个信号,pid: " << getpid() << " sig number: " << sig << " cnt: " << cnt << std::endl;
    alarm(2);
    // exit(1); // 没有这个exit就会死循环

    fork && exec("shutdown ..");
}

int main()
{
    alarm(2); // 一次性闹钟 && 只对这个进程，只会有一个闹钟
    // int n=alarm(0); // alarm返回值是上一个闹钟的剩余时间,alarm(0)表示取消闹钟
    // printf("remain: %d\n",n);

    signal(SIGALRM, handlersig);

    while (true)
    {
        std::cout<<"counter: "<<cnt<<std::endl;
        cnt++;
        sleep(1);
    }














    // signal(SIGFPE,handlersig);
    // for(int signo=1;signo<=31;signo++)
    //     signal(signo,handlersig);

    // signal(2, handlersig);
    // signal(2, SIG_DFL); // 默认动作
    // signal(2, SIG_IGN); // 忽略
    // signal(3, handlersig);
    // signal(4, handlersig);

    // int a;
    // while (true)
    // {
    //     // scanf("%d",&a); // 后台不允许获取数据！
    //     // std::cout << "我是一个进程,pid: " << getpid() << std::endl;
    //     // sleep(1);
    //     // // int *p=nullptr;
    //     // // *p=100; // 野指针报错

    //     // int a=10;
    //     // a/=0;

    //     // 自己给自己发信号 raise();
    //     // raise(SIGINT); // 2号信号
    //     // abort();
    // }

    return 0;
}