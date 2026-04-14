#ifndef TASK_HPP
#define TASK_HPP

#include <iostream>
#include <string>

class Task
{
public:
    Task()
    {
    }
    Task(int a, int b) : _x(a), _y(b), _res(0)
    {
    }
    void operator()()
    {
        _res = _x + _y;
    }
    std::string tostring()
    {
        return std::to_string(_x) + "+" + std::to_string(_y) + "=";
    }
    int Result()
    {
        return _res;
    }

private:
    int _x;
    int _y;
    int _res;
};

#endif