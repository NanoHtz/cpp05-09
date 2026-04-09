#include <iostream>
#include <vector>
#include <cstdlib>
#include "Span.hpp"

int main()
{
    std::cout << "=== Test del pdf ===" << std::endl;
    Span sp = Span(5);
    sp.addNumber(6);
    sp.addNumber(3);
    sp.addNumber(17);
    sp.addNumber(9);
    sp.addNumber(11);
    std::cout << "shortestSpan: " << sp.shortestSpan() << std::endl;
    std::cout << "longestSpan: " << sp.longestSpan() << std::endl;

    std::cout << "\n=== Test span lleno ===" << std::endl;
    try
    {
        sp.addNumber(42);
    }
    catch (std::exception &e)
    {
        std::cout << "Excepcion: span lleno" << std::endl;
    }

    std::cout << "\n=== Test menos de 2 elementos ===" << std::endl;
    try
    {
        Span sp2 = Span(1);
        sp2.addNumber(5);
        sp2.shortestSpan();
    }
    catch (std::exception &e)
    {
        std::cout << "Excepcion: menos de 2 elementos" << std::endl;
    }

    std::cout << "\n=== Test 10.000 numeros ===" << std::endl;
    Span sp3 = Span(10000);
    std::vector<int> vec;
    for (int i = 0; i < 10000; i++)
        vec.push_back(rand());
    sp3.addNumbers(vec.begin(), vec.end());
    std::cout << "shortestSpan: " << sp3.shortestSpan() << std::endl;
    std::cout << "longestSpan: " << sp3.longestSpan() << std::endl;

    return 0;
}