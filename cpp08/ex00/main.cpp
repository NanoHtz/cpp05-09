#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include "easyfind.hpp"

int main()
{
    std::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);

    try
    {
        std::vector<int>::const_iterator it = easyfind(vec, 1);
        std::cout << "Found in vector: " << *it << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "Not found in vector" << std::endl;
    }

    try
    {
        easyfind(vec, 42);
    }
    catch (std::exception &e)
    {
        std::cout << "42 not found in vector" << std::endl;
    }

    std::list<int> lst;
    lst.push_back(10);
    lst.push_back(20);
    lst.push_back(30);

    try
    {
        std::list<int>::const_iterator it = easyfind(lst, 20);
        std::cout << "Found in list: " << *it << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "Not found in list" << std::endl;
    }

    try
    {
        std::list<int>::const_iterator it = easyfind(lst, 50);
        std::cout << "Found in list: " << *it << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "Not found in list" << std::endl;
    }

    std::deque<int> dq;
    dq.push_back(10);
    dq.push_back(20);
    dq.push_back(30);

    try
    {
        std::deque<int>::const_iterator it = easyfind(dq, 20);
        std::cout << "Found in deque: " << *it << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "Not found in deque" << std::endl;
    }

    try
    {
        std::deque<int>::const_iterator it = easyfind(dq, 50);
        std::cout << "Found in deque: " << *it << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "Not found in deque" << std::endl;
    }

    return 0;
}