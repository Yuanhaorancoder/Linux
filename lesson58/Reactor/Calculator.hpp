#ifndef __CALCULATOR_HPP
#define __CALCULATOR_HPP

#include <iostream>
#include <string>
#include "Protocol.hpp"

class Request;
class Response;

class Calculator
{
public:
    Calculator()
    {
    }
    ~Calculator()
    {
    }

public:
    Response Exec(const Request &req)
    {
        LOG(LogLevel::DEBUG) << "Enter Calculator Exec";
        
        Response resp;
        switch (req._oper)
        {
        case '+':
            resp._result = req._x + req._y;
            break;
        case '-':
            resp._result = req._x - req._y;
            break;
        case '*':
            resp._result = req._x * req._y;
            break;
        case '/':
            {
                if(req._y == 0)
                    resp._exitcode = 1; // div 0
                else
                    resp._result = req._x / req._y;
            }
            break;
        case '%':
            {
                if(req._y == 0)
                    resp._exitcode = 2; // % 0
                else
                    resp._result = req._x % req._y;
            }
            break;
        default: 
            resp._exitcode = 3; // oper code error
            break;
        }

        return resp;
    }
};

#endif