#include "PmergeMe.hpp"
#include <deque>
#include <exception>
#include <iostream>
#include <vector>

template <typename Container>
static void printSequence(const Container &container)
{
    typename Container::const_iterator it = container.begin();

    while (it != container.end())
    {
        std::cout << *it;
        ++it;
        if (it != container.end())
            std::cout << " ";
    }
    std::cout << std::endl;
}

int main(int ac, char **av)
{
    try
    {
        PmergeMe sorter(ac, av);

        sorter.sort();
        std::cout << "Before: ";
        printSequence(sorter.getVectorInput());
        std::cout << "After:  ";
        printSequence(sorter.getVectorSorted());
        std::cout << "Time to process a range of "
            << sorter.getVectorInput().size()
            << " elements with std::vector : "
            << sorter.getVectorTime() << " us" << std::endl;
        std::cout << "Time to process a range of "
            << sorter.getDequeSorted().size()
            << " elements with std::deque : "
            << sorter.getDequeTime() << " us" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0);
}
