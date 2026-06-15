#include "SelectServer.hpp"

#include <memory>

const uint16_t gport = 8080;

int main()
{
    std::unique_ptr<SelectServer> select_svr = std::make_unique<SelectServer>(gport);
    select_svr->Dispatcher();

    return 0;
}