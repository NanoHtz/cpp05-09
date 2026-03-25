#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include "Easyfind.hpp"

int main()
{
    std::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);

    try {
        std::vector<int>::iterator it = easyfind(vec, 3);
        std::cout << "Found in vector: " << *it << std::endl;
    } catch (std::exception &e) {
        std::cout << "Not found in vector" << std::endl;
    }

    try {
        easyfind(vec, 42);
    } catch (std::exception &e) {
        std::cout << "42 not found in vector" << std::endl;
    }

    std::list<int> lst;
    lst.push_back(10);
    lst.push_back(20);
    lst.push_back(30);

    try {
        std::list<int>::iterator it = easyfind(lst, 20);
        std::cout << "Found in list: " << *it << std::endl;
    } catch (std::exception &e) {
        std::cout << "Not found in list" << std::endl;
    }

    return 0;
}