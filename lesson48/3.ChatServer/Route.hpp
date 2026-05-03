#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "InetAddr.hpp"
#include "Mutex.hpp"

// class User
// {
// private:
//     std::string nickname;
//     std::string stauts;
//     InetAddr clientaddr;
// };

class Route
{
private:
    bool IsOnline(const InetAddr &who)
    {
        for(auto &user:_users)
            if(user == who)
                return true;

        return false;
    }
    void AddUser(const InetAddr &who)
    {
        _users.push_back(who);
    }
public:
    Route(){}
    void RouteMessage(std::string message,InetAddr who, int sockfd)
    {
        LOG(LogLevel::DEBUG) << "3.RouteMessage"; 
        {
            LockGuard lockguard(&_lock);
            // 1.判断用户是否在线：不在就插入表示上线，如果在，就什么不用管，直接转发
            if(!IsOnline(who)) 
                AddUser(who);

            // 2.转发给所有用户
            for(auto &user:_users)
            {
                std::string send_message = who.StringAddress();
                send_message += "# ";
                send_message += message;
                sendto(sockfd,send_message.c_str(),send_message.size(),0,user.Addr(),user.AddrLen());
            }
        }
    }
    ~Route(){}
private:
    std::vector<InetAddr> _users;
    Mutex _lock;
};