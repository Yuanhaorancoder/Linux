#pragma once

#include <iostream>
#include <string>
#include <memory>
#include "HttpProtocol.hpp"
#include "TcpServer.hpp"
#include "Logger.hpp"

using namespace LogModule;

class HttpServer
{
public:
    HttpServer(uint16_t port)
    :_port(port),
     _tsvr(std::make_unique<TcpServer>(port))
    {}

    // 1.分析字节流是否有完整的HTTP报文
    // 2.http request -> http response
    std::string HandlerHttpRequest(std::string &streamstr)
    {
        // 1.报文完整性 - 略
        // 2.反序列化
        HttpRequest httpreq;
        httpreq.Deserialize(streamstr);
        // std::cout << "HandlerHttpRequest: " << httpreq["method"] << std::endl;
        // std::cout << "HandlerHttpRequest: " << httpreq["uri"] << std::endl;
        // std::cout << "HandlerHttpRequest: " << httpreq["http_version"] << std::endl;
        
        std::string target = httpreq["path"]; // /a/b/c.html
        std::cout << "http req: " << target << std::endl;

        // 3.处理请求，构建应答httpreq -> httpresp
        HttpResponse httpresp;

        // 4.应答序列化
        std::string httprespstr;
        httpresp.Serialize(&httprespstr);

        // 5.返回
        return httprespstr;
        
        

        // HttpRequest req;
        // Unpack(streamstr);
        // req.Deserialize(streamstr);

        // LOG(LogLevel::DEBUG) << "http request:\r\n";
        // LOG(LogLevel::DEBUG) << "###########################\r\n";
        // LOG(LogLevel::DEBUG) << streamstr;
        // LOG(LogLevel::DEBUG) << "###########################\r\n";

        // std::string resp_status = "HTTP/1.0 200 OK\r\n";
        // std::string resp_content = "hello http";
        // std::string contentlen = "Content-Length: " + std::to_string(resp_content.size()) + "\r\n";
        // std::string blankline = "\r\n";

        // return resp_status + contentlen + blankline + resp_content;
    }

    void Run()
    {
        _tsvr->Run([this](std::string &streamstr)->std::string{
            return this->HandlerHttpRequest(streamstr);
        });
    }

    ~HttpServer()
    {}
private:
    uint16_t _port;
    std::unique_ptr<TcpServer> _tsvr;
};