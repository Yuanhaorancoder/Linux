#include "Protocol.hpp"
#include "Socket.hpp"
#include <iostream>
#include <memory>

void Usage(std::string procname)
{
    std::cout << "Usage: " << procname << " server_ip server_port" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }

    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    std::unique_ptr<Socket> sockfd = std::make_unique<TcpSocket>();
    sockfd->BuildClientSocketMethod(serverip, serverport);

    std::unique_ptr<Protocol> protocol = std::make_unique<Protocol>();

    std::string inbuffer;
    while (true)
    {
        int cnt = 3;
        std::string sendstring;
        while (cnt--)
        {
            // 0.构建请求
            Request req;
            std::cout << "Enter x# ";
            std::cin >> req._x;
            std::cout << "Enter y# ";
            std::cin >> req._y;
            std::cout << "Enter oper# ";
            std::cin >> req._oper;

            // 1.序列化
            std::string reqjsonstr;
            req.Serialize(&reqjsonstr);

            // 2.添加报头
            sendstring += protocol->Package(reqjsonstr);
        }
        std::cout << sendstring << std::endl;

        // 3.发送
        sockfd->Send(sendstring);

        // 4.接受应答
        sockfd->Recv(&inbuffer); // 你能保证是完整的报文嘛

        // 5.解包并判断
        while (true)
        {
            std::string jsonrespstr;
            int n = protocol->UnPackage(inbuffer, &jsonrespstr);
            if (n == 0)
            {
                LOG(LogLevel::DEBUG) << "解包完成";
                break;
            }
            // 6.反序列化
            Response resp;
            resp.Deserialize(jsonrespstr);

            // 7.打印结果
            std::cout << resp._result << '[' << resp._exitcode << ']' << std::endl;
        }
    }

    return 0;
}