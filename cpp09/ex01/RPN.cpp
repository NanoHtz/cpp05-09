#include "RPN.hpp"
#include <cctype>
#include <sstream>
#include <stdexcept>

RPN::RPN()
{
}

RPN::RPN(const RPN &other) : _stack(other._stack)
{
}

RPN &RPN::operator=(const RPN &other)
{
    if (this != &other)
        _stack = other._stack;
    return (*this);
}

RPN::~RPN()
{
}

int RPN::evaluate(const std::string &expression)
{
    for (; !_stack.empty(); _stack.pop())
    {
    }

    std::istringstream iss(expression);
    std::string token;

    while (iss >> token)
    {
        if (token.length() != 1)
            throw std::runtime_error("Error");

        char c = token[0];

        if (std::isdigit(static_cast<unsigned char>(c)))
            _stack.push(c - '0');
        else if (c == '+' || c == '-' || c == '*' || c == '/')
        {
            if (_stack.size() < 2)
                throw std::runtime_error("Error");

            int right = _stack.top();
            _stack.pop();
            int left = _stack.top();
            _stack.pop();

            int result;
            if (c == '+')
                result = left + right;
            else if (c == '-')
                result = left - right;
            else if (c == '*')
                result = left * right;
            else
            {
                if (right == 0)
                    throw std::runtime_error("Error");
                result = left / right;
            }
            _stack.push(result);
        }
        else
            throw std::runtime_error("Error");
    }
    if (_stack.size() != 1)
        throw std::runtime_error("Error");
    return (_stack.top());
}
