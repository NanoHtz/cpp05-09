#include "Iter.hpp"

#include <string>
#include <ostream>
#include <iostream>

template<typename T> void printElement(const T &str)
{
    std::cout << str << std::endl;
}

template<typename T> void modify(T &str)
{
    str++;
}

int main( void )
{
    int nums[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    char letters[10] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

    std::cout << "--- Int array ---" << std::endl;
    iter(nums, 10, printElement<int>);

    std::cout << "--- Modify int array ---" << std::endl;
    iter(nums, 10, modify<int>);
    iter(nums, 10, printElement<int>);

    std::cout << "--- Char array ---" << std::endl;
    iter(letters, 10, printElement<char>);

    std::cout << "--- Modify char array ---" << std::endl;
    iter(letters, 10, modify<char>);
    iter(letters, 10, printElement<char>);

    return 0;
}