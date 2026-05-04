#ifndef __TCPECHOSERVER_HPP
#define __TCPECHOSERVER_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Logger.hpp"
#include "InetAddr.hpp"
#include "ThreadPool.hpp"

static const uint16_t gdefaultport = 8080;
static const int gbacklog = 32;

using namespace LogModule;

using task_t = std::function<void()>;

class TcpEchoServer
{
private:
    // sockfd:既可以支持读，又可以支持写，TCP socket也是全双工的 ---同时读写，并且不影响
    void Service(int sockfd,InetAddr client)
    {
        // 长连接服务
        while(true)
        {
            char inbuffer[1024];
            // 1.读取数据
            ssize_t n = read(sockfd,inbuffer,sizeof(inbuffer)-1); // 默认字符串
            if(n > 0)
            {
                inbuffer[n] = 0;
                LOG(LogLevel::INFO) << client.StringAddress() << "say# " << inbuffer;
            }
            else if(n == 0) // 客户端退出
            {
                LOG(LogLevel::INFO) << client.StringAddress() << " close sockfd: " << sockfd << ",me too!";
                break;
            }
            else
            {
                LOG(LogLevel::ERROR) << "read sockfd error" << sockfd;
                break;
            }
            // 2.加工处理数据
            std::string echo_string = "server echo# ";
            echo_string += inbuffer;

            // 3.写回数据
            n = write(sockfd,echo_string.c_str(),sizeof(echo_string));
            if(n <= 0)
            {
                LOG(LogLevel::ERROR) << "write sockfd error" << sockfd;
                break;
            }
        }
        // 关闭文件描述符
        close(sockfd);
    }
public:
    TcpEchoServer(uint16_t port = gdefaultport)
        :_port(port),_listensockfd(-1)
    {}
    void Init()
    {
        // 1.创建socket
        _listensockfd = socket(AF_INET,SOCK_STREAM,0);
        if(_listensockfd < 0)
        {
            LOG(LogLevel::FATAL) << "socket error" << _listensockfd;
            exit(2);
        }
        LOG(LogLevel::INFO) << "socket success: " << _listensockfd; // 3

        // 2.bind : 设置服务器的socket信息
        struct sockaddr_in local;
        memset(&local,0,sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = htonl(INADDR_ANY);
        // local.sin_addr = inet_addr(_ip);

        int n = bind(_listensockfd,(struct sockaddr*)&local,sizeof(local));
        // int n = bind(_listensockfd,(const sockaddr*)&local,sizeof(local));
        if(n < 0)
        {
            LOG(LogLevel::FATAL) << "bind error: " << _listensockfd;
            exit(3);
        }
        LOG(LogLevel::DEBUG) << "bind success: " << _listensockfd;

        // 3.将socket设置为监听状态
        n = listen(_listensockfd,gbacklog);
        if(n < 0)
        {
            LOG(LogLevel::FATAL) << "listen error: " << _listensockfd;
            exit(4);
        }
        LOG(LogLevel::DEBUG) << "listen success: " << _listensockfd;
    }

    class ThreadData
    {
    public:
        ThreadData(int sockfd,InetAddr addr,TcpEchoServer *owner)
            :_sockfd(sockfd),_address(addr),_owner(owner)
        {}
    public:
        int _sockfd;
        InetAddr _address;
        TcpEchoServer *_owner;
    };

    static void *threadrun(void *args)
    {
        pthread_detach(pthread_self()); //线程分离
        ThreadData *td = static_cast<ThreadData *>(args);
        td->_owner->Service(td->_sockfd,td->_address);
        delete td;
        return nullptr;
    }
    void Start()
    {
        // signal(SIGCHLD,SIG_IGN); // 最佳实践
        while(true)
        {
            struct sockaddr_in clientaddr;
            socklen_t len = sizeof(clientaddr);
            int sockfd = accept(_listensockfd,(struct sockaddr*)&clientaddr,&len);
            if(sockfd < 0)
            {
                LOG(LogLevel::WARNING) << "accept error";
                continue;
            }
            InetAddr clientaddress(clientaddr);
            LOG(LogLevel::INFO) << "get a new link: " << clientaddress.StringAddress() << "sockfd: " << sockfd;

            //Version3 --- 线程池 --- 目前代码样例 --- bug
            ThreadPool<task_t>::GetInstance()->Enqueue([sockfd,clientaddress,this](){
                Service(sockfd,clientaddress);
            });


            //Version2 --- 多线程版本
            // pthread_t tid;
            // ThreadData *td = new ThreadData(sockfd,clientaddress,this);
            // pthread_create(&tid,nullptr,threadrun,(void*)td);  
            

            // 处理链接，进行IO通信          
            // Version0 --- 不会被使用                                       
            // Service(sockfd,clientaddress);
        

            // Version1 --- 多进程
            // pid_t id = fork();
            // if(id < 0)
            // {
            //     LOG(LogLevel::ERROR) << "fork error";
            //     close(sockfd);
            // }
            // else if(id == 0) // 子进程
            // {
            //     // 关闭自己并不需要的sockfd
            //     close(_listensockfd);

            //     // 子进程拷贝父进程的文件描述符表，从而和父进程看到同一批文件
            //     if(fork() > 0) // 自己退出
            //         exit(0);
            //     // 孙子进程 -- 变孤儿进程，系统回收资源
            //     Service(sockfd,clientaddress);
            //     exit(0);
            // }
            // else
            // {}
            // //父进程 -- 回收子进程
            // close(sockfd);
            // pid_t rid = waitpid(id,nullptr,0);
            // (void)rid;
        }
    }
    ~TcpEchoServer(){}
private:
    uint16_t _port;
    int _listensockfd;
};

#endif