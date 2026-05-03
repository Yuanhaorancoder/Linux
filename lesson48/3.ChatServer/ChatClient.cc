#include <iostream>
#include <string>
#include <cstring>
#include "Thread.hpp"
#include "InetAddr.hpp"

int sockfd = -1;
std::string serverip;
uint16_t serverport = 0;

void Usage(const std::string &name)
{
    std::cerr << "Usage: " << name << " server_ip server_port" << std::endl;
}

void InitClient()
{
    // 1. 创建udp socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "socket error" << std::endl;
    }
}

void RecvMessage()
{
    while (true)
    {
        // 2. 接受数据
        struct sockaddr_in temp;
        socklen_t len = sizeof(temp);
        char buffer[1024];
        ssize_t m = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&temp, &len);
        if (m > 0)
            buffer[m] = 0;
        (void)m;

        std::cerr << buffer << std::endl; // 2 打印
    }
}
void SendMessage()
{
    // 2. 构建目标服务器socket信息
    InetAddr serveraddress(serverport, serverip);

    while (true)
    {
        std::string line;
        std::cout << "Please Enter# ";
        std::getline(std::cin, line);

        // 1. 发送数据
        ssize_t n = sendto(sockfd, line.c_str(), line.size(), 0,\
            serveraddress.Addr(), serveraddress.AddrLen());
        (void)n;
    }
}

// 作为client端，它怎么知道服务器端的IP和port？
// ./chat_client 192.1.1.1 8080
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(0);
    }
    serverip = argv[1];
    serverport = std::stoi(argv[2]);
    InitClient();

    Thread recver(RecvMessage);
    Thread sender(SendMessage);

    recver.start();
    sender.start();

    recver.join();
    sender.join();

    return 0;
}