#pragma once

#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <fcntl.h>

// 将进程变成守护进程的函数
void Daemon()
{
    // 1.忽略相关信号
    signal(SIGPIPE,SIG_IGN);
    signal(SIGCHLD,SIG_IGN);

    // 2.让自己不要成为组长
    if(fork() > 0)
    {
        // 父进程
        exit(0);
    }

    // 子进程
    // 3.形成会话，自己是话首进程
    setsid();

    // 4.处理标准输入输出
    int fd = open("/dev/null",O_RDWR);
    if(fd >= 0)
    {
        dup2(fd,0);
        dup2(fd,1);
        dup2(fd,2);
    }
}