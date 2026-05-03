#include "UdpServer.hpp"
#include "Route.hpp"
#include "ThreadPool.hpp"

#include <memory>

void Usage(const std::string &name)
{
    std::cerr << "Usge: " << name << " port" << std::endl;
}

using task_t = std::function<void()>;

// ./chatServer 8080
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }

    ENABLE_CONSOLE_LOG_STRATEGY();

    // std::string server_ip = argv[1];
    uint16_t server_port = std::stoi(argv[1]);

    // 法2
    // std::unique_ptr<Route> r = std::make_unique<Route>();
    // std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(
    //     [&r](std::string message, InetAddr who, int sockfd) {
    //         auto task = std::bind(&Route::RouteMessage,r.get(),message,who,sockfd);
    //         ThreadPool<task_t>::GetInstance()->Enqueue(task);
    //     },
    //     server_port);

    // 法1
    std::unique_ptr<Route> r = std::make_unique<Route>();
    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(
        [&r](std::string message, InetAddr who, int sockfd)
        {
            // 网络系列数据
            LOG(LogLevel::INFO) << "1. get a message: " << message
                                << ",client addr: " << who.IP() << ":" << who.Port();

            auto task = [&message, &who, &sockfd, &r]()
            {
                r->RouteMessage(message, who, sockfd);
            };
            ThreadPool<task_t>::GetInstance()->Enqueue(task);
        },
        server_port);

    usvr->Init();
    usvr->Start();

    return 0;
}