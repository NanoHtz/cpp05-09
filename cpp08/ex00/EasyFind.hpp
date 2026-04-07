#ifndef EASYFIND_HPP
# define EASYFIND_HPP

# include <algorithm>

template<typename T>
typename T::const_iterator easyfind(const T &data, int look_for)
{
    typename T::const_iterator it = std::find(data.begin(), data.end(), look_for);
    if (it == data.end())
        throw std::exception();
    return it;
}

#endif