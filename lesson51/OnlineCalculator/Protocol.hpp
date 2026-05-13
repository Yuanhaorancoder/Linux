#ifndef __PROTOCOL_HPP
#define __PROTOCOL_HPP

#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <functional>
#include <jsoncpp/json/json.h>
#include "Logger.hpp"

using namespace LogModule;

// 请求报文 client->server
class Request
{
public:
    Request() {}
    Request(int x, int y, char oper) : _x(x), _y(y), _oper(oper)
    {
    }

    void Serialize(std::string *outstr)
    {
        // 序列化,结构化属性 多变一
        Json::Value root;
        root["datax"] = _x; // 转字符串
        root["datay"] = _y;
        root["oper"] = _oper;

        Json::FastWriter writer;
        // Json::StyledWriter writer;
        *outstr = writer.write(root);
    }

    void Deserialize(std::string &instr)
    {
        // 反序列化，字节流信息 一变多
        Json::Value root;
        Json::Reader reader;
        if (reader.parse(instr, root))
        {
            _x = root["datax"].asInt();
            _y = root["datay"].asInt();
            _oper = root["oper"].asInt();
        }
        else
        {
            std::cout << "bug!!!" << std::endl;
        }
    }

    void Print()
    {
        std::cout << "_x: " << _x << std::endl;
        std::cout << "_y: " << _y << std::endl;
        std::cout << "_oper: " << _oper << std::endl;
    }

    ~Request() {}

public:
    // _x _oper _y
    int _x;
    int _y;
    char _oper; // + - * / %
};

// 应答报文 server->client
class Response
{
public:
    Response() : _result(0), _exitcode(0)
    {
    }
    // Response()
    // {}
    void Serialize(std::string *outstr)
    {
        // 序列化
        Json::Value root;
        root["result"] = _result;
        root["exitcode"] = _exitcode;

        Json::FastWriter writer;
        *outstr = writer.write(root);
    }

    void Deserialize(std::string &instr)
    {
        // 反序列化
        Json::Value root;
        Json::Reader reader;
        if (reader.parse(instr, root))
        {
            _result = root["result"].asInt();
            _exitcode = root["exitcode"].asInt();
        }
        else
        {
            std::cout << "bug!!!" << std::endl;
        }
    }
    ~Response() {}

public:
    int _result;
    int _exitcode; // 0:结果可信 1/2/3/4...: 结果不可信，错误原因，状态码
};

const std::string gsep = "\r\n";

using callback_t = std::function<Response(const Request &)>;

class Protocol
{
public:
    Protocol() {}
    Protocol(callback_t cb)
        : _cb(cb)
    {
    }
    ~Protocol() {}

    // 封包 {"datax":10,"datay":20,"oper":43} -> "40"\r\n{"datax":10,"datay":20,"oper":43}\r\n
    std::string Package(const std::string &jsonstr)
    {
        uint32_t len = jsonstr.size();
        return std::to_string(len) + gsep + jsonstr + gsep;
    }

    // 解包
    // int > 0:  提取了一个完整的json报文
    // int ==0 : 不完整，什么都不做
    // int <0 :  UnPackage出错了
    // "LE
    // "LEN"
    // "LEN"\r\
    // "LEN"\r\n
    // "LEN"\r\n{"datax"
    // "LEN"\r\n{"datax":10,"dat
    // "LEN"\r\n{"datax":10,"datay":20,"oper":
    // "LEN"\r\n{"datax":10,"datay":20,"oper":43}\r\n"LEN"\r\n{"datax":10,"datay":20,"oper":43}\r\n"LEN"\r\n{"datax":10,"datay":20,"oper":43}\r\n
    // "LEN"\r\n{"datax":10,"datay":20,"oper":43}\r\n"LEN"\r\n{"datax"
    int UnPackage(std::string &streamstr, std::string *jsonstr)
    {
        // a.完整的报文:处理，提取 b.不完整：什么都不做
        auto pos = streamstr.find(gsep);
        if (pos == std::string::npos)
            return 0; // 报文不完整
        std::string packlenstr = streamstr.substr(0, pos);
        // packlenstr -> '0'->'9'
        for (auto &c : packlenstr)
        {
            if (c < '0' || c > '9')
            {
                return -1;
            }
        }

        uint32_t packlenint = std::stoi(packlenstr); // "40"->40

        // 如果一个报文是完整的,应该是多长
        uint32_t targetlen = packlenstr.size() + packlenint + 2 * gsep.size(); // "LEN" + \r\n + json + \r\n

        if (streamstr.size() < targetlen)
            return 0; // 报文不完整

        // 提取这个完整的jsonstr
        *jsonstr = streamstr.substr(pos + gsep.size(), packlenint);
        streamstr.erase(0, targetlen); // 报文出队列
        return targetlen;
    }

public:
    std::string HandlerRequest(std::string &streamstr)
    {
        LOG(LogLevel::DEBUG) << "Enter HandlerRequest";

        std::string resp_package;
        // 1.检测报文完整性
        while (true)
        {
            std::string jsonstring;
            int n = UnPackage(streamstr, &jsonstring);
            if (n == 0)
            {
                LOG(LogLevel::DEBUG) << "解析完毕";
                return resp_package;
            }
            else if (n == -1)
            {
                LOG(LogLevel::DEBUG) << "协议解析失败";
                exit(1);
            }
            // 我就可以保证streamstr一定至少有一个报文
            LOG(LogLevel::DEBUG) << "request : jsonstring: " << jsonstring;

            // 2.{"datax":10,"datay":20,"oper":43}->反序列化
            Request req;
            req.Deserialize(jsonstring);

            // 3.Request -> Response ---计算
            Response resp = _cb(req);

            // 4.序列化
            std::string respjsonstr;
            resp.Serialize(&respjsonstr);

            LOG(LogLevel::DEBUG) << "response : jsonstring: " << jsonstring;

            // 5.封包
            resp_package += Package(respjsonstr);
            LOG(LogLevel::DEBUG) << "Package: \r\n" << resp_package;
        }
        return resp_package;
    }

private:
    callback_t _cb;
};

// 问题：client->request->{"datax":10,"datay":20,"oper":43}->write->OS->网络->收到
// 问题:粘包问题
// {"datax"
// {"datax":10,"dat
// {"datax":10,"datay":20,"oper":
// {"datax":10,"datay":20,"oper":43}
// {"datax":10,"datay":20,"oper":43}{"datax"
// {"datax":10,"datay":20,"oper":43}{"datax":10,"datay":20,"oper":43}{"datax"s

// 报文长度 + 特殊分隔符 + jsonstring
// "LEN"\r\njsonstring -> 你怎么保证报文是完整的?
// "LE
// "LEN"
// "LEN"\r\
// "LEN"\r\n
// "LEN"\r\n{"datax"
// "LEN"\r\n{"datax":10,"dat
// "LEN"\r\n{"datax":10,"datay":20,"oper":
// "LEN"\r\n{"datax":10,"datay":20,"oper":43}
// "LEN"\r\n{"datax":10,"datay":20,"oper":43}"LEN"\r\n{"datax"
// "LEN"\r\n{"datax":10,"datay":20,"oper":43}"LEN"\r\n{"datax":10,"datay":20,"oper":43}"LEN"\r\n{"datax"s

// 报文长度 + 特殊分隔符 + jsonstring + 特殊分隔符
// "LEN"\r\njsonstring\r\n

// 如何序列化？反序列化？
// Request为例，自己做序列和反序列化
// 方案一：
// "_x _oper _y" --- 序列化
// 如何解决粘包问题？应用层自己解决
// len -> "len"\r\n "_x _oper _y"\r\n
// 解包
// 得到完整报文
// 反序列化
// 方案二：Jsoncpp

#endif