#include <iostream>
#include <sys/select.h>

int main()
{
    std::cout << sizeof(fd_set)*8 << std::endl;
    return 0;
}

// #include <iostream>
// #include <unistd.h>
// #include <fcntl.h>

// void SetNonBlock(int fd)
// {
//     int flags = fcntl(fd,F_GETFL);
//     if(flags < 0)
//     {
//         perror("fcntl");
//         return;
//     }
//     fcntl(fd,F_SETFL,flags | O_NONBLOCK);
// }

// void DoOtherThing()
// {
//     std::cout << "0 not ready,do other thing!" << std::endl;
// }

// int main()
// {
//     SetNonBlock(0);

//     char inbuffer[128];
//     while(true)
//     {
//         inbuffer[0] = 0; // O(1) 初始化字符串
//         ssize_t n = read(0,inbuffer,sizeof(inbuffer));
//         if(n > 0)
//         {
//             inbuffer[n] = 0;
//             std::cout << "inbuffer: " << inbuffer << std::endl;
//         }
//         else if(n == 0)
//         {
//             std::cout << "enter done,break" << std::endl;
//             break;
//         }
//         else
//         {
//             // 1.非阻塞模式下，只能以出错形式返回
//             // 2.底层没有数据，read返回，算不算出错？不算！
//             // 3.如果返回值<0，应该关心什么？因为什么原因出错了，错误码 errno == 11
//             // 4.错误码 errno == 11 EAGAIN || EWOULDBLOCK 不是真的出错，知识表明fd没有就绪
//             // std::cout << "read error: n = " << n << ",errno=" << errno << std::endl;
//             if(errno == EAGAIN || errno == EWOULDBLOCK)
//             {
//                 DoOtherThing();
//                 sleep(1);
//                 std::cout << "read 0,0 fd data not ready!" << std::endl;
//                 continue;
//             }
//                                     //如果当前的系统调用因为收到信号而被操作系统强行中断了（errno == EINTR）
//             else if(errno == EINTR) //不要报错退出，直接跳过本次循环后面的代码，重新发起下一次调用（continue;）
//             {
//                 continue;
//             }
//             else
//             {
//                 std::cout << "read error: n = " << n << ",errno=" << errno << std::endl;
//                 break;
//             }
//         }
//         sleep(1);
//     }
//     return 0;
// }