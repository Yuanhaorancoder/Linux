#pragma once

#include <iostream>
#include <string>

class Task
{
public:
    Task(const std::string &who, int x, int y) : _x(x), _y(y),_who(who)
    {}
    Task()
    {}
    void Excute()
    {
        _result=_x+_y;
    }
    std::string Result()
    {
        return std::to_string(_x)+"+"+std::to_string(_y)+"="+std::to_string(_result);
    }
    ~Task()
    {}

private:
    int _x;
    int _y;
    int _result;
    std::string _who;
};