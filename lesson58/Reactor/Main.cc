#include "Calculator.hpp"
#include "Protocol.hpp"
#include "Connection.hpp"
#include "Reactor.hpp"
#include "Listener.hpp"
#include <memory>

static const int gport = 8080;

int main()
{
    ENABLE_CONSOLE_LOG_STRATEGY();
    // 1.定义计算器对象
    std::unique_ptr<Calculator> cal = std::make_unique<Calculator>();

    // 2.定义协议对象
    std::unique_ptr<Protocol> protocol = std::make_unique<Protocol>(
        [&cal](const Request &req) -> Response {
            return cal->Exec(req);
        }
    );

    // 3.listener 
    std::shared_ptr<Connection> conn = std::make_shared<Listener>(gport,
        [&protocol](std::string &inbuffer,int *code) -> std::string{
            return protocol->HandlerRequest(inbuffer,code);
        }
    );
    conn->SetEvents(EPOLLIN | EPOLLET); // 读 && ET模式

    // 2.Reactor
    std::unique_ptr<Reactor> R = std::make_unique<Reactor>();

    // 3.listener -> Reactor
    R->AddConnection(conn);

    // 4.开始事件派发
    R->Dispatcher();

    return 0;
}