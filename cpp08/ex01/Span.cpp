#include "Span.hpp"

Span::Span() : _n(0), _numbers()
{
}

Span::~Span()
{
    
}

Span::Span(const Span &other) : _n(other._n), _numbers(other._numbers)
{
}

Span &Span::operator=(const Span &other)
{
    if (&other != this)
    {
        this->_n = other._n;
        this->_numbers = other._numbers;
    }
    return (*this);
}

Span::Span(unsigned int n) : _n(n)
{

}

void Span::addNumber(int number)
{
    if (this->_numbers.size() < this->_n)
        _numbers.push_back(number); 
    else
        throw std::exception();
}

int Span::longestSpan() const
{
    if (this->_numbers.size() < 2)
        throw std::exception();
    return (*std::max_element(_numbers.begin(), _numbers.end()) - *std::min_element(_numbers.begin(), _numbers.end()));
}

int Span::shortestSpan() const
{
    if (this->_numbers.size() < 2)
        throw std::exception();

    std::vector<int> sorted = _numbers;
    std::sort(sorted.begin(), sorted.end());
    
    std::vector<int> diffs(sorted.size());
    std::adjacent_difference(sorted.begin(), sorted.end(), diffs.begin());
    return *std::min_element(diffs.begin() + 1, diffs.end());
}