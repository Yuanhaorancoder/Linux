#include "Connection.hpp"
#include "Listener.hpp"
#include "Reactor.hpp"
#include <memory>

static const int gport = 8080;

int main()
{
    // 1.listener 
    std::shared_ptr<Connection> conn = std::make_shared<Listener>(gport);
    conn->SetEvents(EPOLLIN | EPOLLET); // ET模式

    // 2.Reactor
    std::unique_ptr<Reactor> R = std::make_unique<Reactor>();

    // 3.listener->Reactor
    R->AddConnection(conn);

    // 4.开始事件派发
    R->Dispatcher();

    return 0;
}