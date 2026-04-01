#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void Schdule()
{
    printf("调度...\n");
}

void IDT(int signum)
{
    switch (signum)
    {
    case 1:
        break;
    case 2:
        Schdule();
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    // ...
    }
}

int main()
{
    for (int i = 1; i <= 31; i++)
        signal(i, IDT);

    while(1)
        pause();
    return 0;
}

// // void WaitChld(int signo)
// // {
// //     printf("pid: %d get a signal:%d\n", getpid(), signo);

// //     sleep(5);

// //     while (1)
// //     {
// //         // 第一个参数设置为-1，表示等待任意一个子进程退出
// //         // pid_t rid = waitpid(-1, NULL, 0);
// //         pid_t rid = waitpid(-1, NULL, WNOHANG); // 非阻塞等待
// //         if (rid > 0)
// //         {
// //             printf("wait child: %d,success\n", rid);
// //         }
// //         else
// //             break;
// //     }
// // }

// int main()
// {
//     // signal(SIGCHLD, WaitChld);
//     signal(SIGCHLD, SIG_IGN);

//     for (int i = 0; i < 10; i++)
//     {
//         pid_t id = fork();
//         if (id == 0)
//         {
//             // child
//             int cnt = 5;
//             while (cnt)
//             {
//                 printf("我是子进程,pid: %d,ppid: %d\n", getpid(), getppid());
//                 sleep(1);
//                 cnt--;
//             }
//             printf("子进程退出！\n");
//             exit(0);
//         }
//     }

//     // 父进程
//     while (1)
//     {
//         printf("父进程完成任务!\n");
//         sleep(1);
//     }

//     // 父进程
//     // while(1)
//     //     pause();
// }

// volatile int flag=0;

// void handler(int signum)
// {
//     (void)signum;
//     flag=1;
//     printf("修改flag 0->1\n");
// }

// int main()
// {
//     signal(2,handler);

//     while(!flag);
//     printf("进程正常退出\n");

//     return 0;
// }