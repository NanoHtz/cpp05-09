#ifndef SPAN_HPP
# define SPAN_HPP

# include <vector>
# include <algorithm>
# include <stdexcept>
#include <numeric>

class Span
{
    private:
        unsigned int        _n;
        std::vector<int>    _numbers;

    public:
        Span();
        ~Span();
        Span(const Span &src);
        Span &operator=(const Span &src);
        Span(unsigned int n);

        void    addNumber(int number);
        int     shortestSpan() const;
        int     longestSpan() const;

        template<typename InputIterator>
        void addNumbers(InputIterator start, InputIterator end)
        {
            size_t limit = std::distance(start, end);
            if (_numbers.size() + limit > _n)
                throw std::exception();
            else
                _numbers.insert(_numbers.end(), start, end);
        };
};

#endif