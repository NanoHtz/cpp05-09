#ifndef ITER_HPP
# define ITER_HPP

#include <cstddef>

template<typename T, typename F> void iter(T *ptr, const size_t len, F f)
{
    for (size_t i = 0; i < len; i++)
        f(ptr[i]);
}

#endif