#ifndef __HTTP_PROTOCOL_HPP
#define __HTTP_PROTOCOL_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include "Logger.hpp"

using namespace LogModule;

const std::string linesep = "\r\n";
const std::string headersep = ": ";
const std::string webroot = "wwwroot";

class HttpRequest
{
private:
    int ReadOneLine(std::string &streamstr,std::string *line) // \r\n
    {
        auto pos = streamstr.find(linesep);
        if(pos == std::string::npos)
            return -1;
        *line = streamstr.substr(0,pos);
        streamstr.erase(0,pos+linesep.size());
        return line->size(); // >=
    }

    void ParseReqLine(std::string &status_line)
    {
        std::stringstream ss(status_line);
        ss >> _method >> _uri >> _http_version;

        // /a/b/c.html
        _path = webroot + _uri; // webroot/a/b/c.html
    }

    void PrintDebug()
    {
        std::cout << "reqline" << _method << "#" << _uri << "#" << _http_version << std::endl;
        for(auto &item:_headerkv)
        {
            std::cout << item.first << "->" << item.second << std::endl;
        }
        std::cout << "blankline: " << _blankline << std::endl; 
        std::cout << "body: " << _body << std::endl;
    }

    void SplitString(std::string &line,std::string *key,std::string *value,std::string sep = headersep)
    {
        auto pos =line.find(sep);
        if(pos == std::string::npos)
            return;
        *key = line.substr(0,pos);
        *value = line.substr(pos+sep.size());
    }
public:
    HttpRequest()
    {}
    void Deserialize(std::string &streamstr)
    {
        // 1.读取第一行
        // std::cout << "begin:\r\n" << streamstr << std::endl;
        std::string status_line;
        int n = ReadOneLine(streamstr,&status_line);
        (void)n;
        // LOG(LogLevel::DEBUG) << "status_line: " << status_line;
        // std::cout << "end:\r\n" << streamstr << std::endl;

        // 2.解析请求行
        ParseReqLine(status_line);
        // PrintDebug();

        // 3.提取请求报头
        n = 0;
        do{
            std::string line;
            n = ReadOneLine(status_line,&line);
            if(n > 0)
            {
                // key: value
                std::string key,value;
                SplitString(line,&key,&value,headersep);
                if(!key.empty() && !value.empty())
                {
                    _headerkv[key] = value;
                }
            }
            else if(n == 0)
            {
                _blankline = "\r\n";
                break;
            }
            else
            {
                LOG(LogLevel::DEBUG) << "bug??";
                break;
            }
        }while(n > 0);

        if(_headerkv.find("Content-Length") !=_headerkv.end()) // 请求正文不存在
        {
            int len = std::stoi(_headerkv["Content-Length"]);
            _body = streamstr.substr(0,len);
            // _body = streamstr;
        }
        PrintDebug();
    }
    std::string operator[](const std::string &key) const
    {
        if(key == "method")
            return _method;
        else if(key == "uri")
            return _uri;
        else if(key == "http_version")
            return _http_version;
        else if(key == "body")
            return _body;
        else if(key == "path")
            return _path;
        else
        {
            auto iter = _headerkv.find(key);
            if(iter != _headerkv.end())
                return iter->second;
        }
        return std::string();
    }
    ~HttpRequest()
    {}
private:
    std::string _method;
    std::string _uri; // /a/b/c.html  /(特殊情况)
    std::string _http_version;
    std::unordered_map<std::string,std::string> _headerkv;
    std::string _blankline;
    std::string _body;
private:
    std::string _path;
};

class HttpResponse
{
public:
    HttpResponse(){}
    ~HttpResponse(){}
    void Serialize(std::string *outstr)
    {

    }
private:
    // 结构化字段
    std::string _http_version;
    int _status_code;
    std::string _status_code_desc;
    std::unordered_map<std::string,std::string> _resp_headerkv;
    std::string _blankline;
    std::string _body;
};


#endif