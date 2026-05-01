#include "UdpEchoServer.hpp"

void Usage(const std::string &name)
{
    std::cerr<< "Usge: " << name << " ip port" << std::endl;
}

// ./UdpEchoServer 192.168.1.1 8080 ---v1
// ./UdpEchoServer 8080 ---v2
int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        Usage(argv[0]);
        exit(0);
    }

    // std::string server_ip = argv[1];
    // uint16_t server_port = std::stoi(argv[2]);
    uint16_t server_port = std::stoi(argv[1]);

    // UdpEchoServer usvr(server_ip, server_port);
    UdpEchoServer usvr(server_port);
    usvr.Init();
    usvr.Start();
    return 0;
}