#ifndef ARRAY_HPP
# define ARRAY_HPP

#include <exception>

template<typename T> class Array
{
    private:

        T *_ptr;
        size_t _len;

    public:

        Array();
        ~Array();
        Array(const Array &);
		Array &operator=(const Array &);

        Array(unsigned int n);

        const T &operator[](const size_t n) const;
        T &operator[](size_t n);

        size_t size() const;

};

#include "Array.tpp"

#endif