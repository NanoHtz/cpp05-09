#include "Array.hpp"

template<typename T>
Array<T>::Array() : _ptr(NULL), _len(0)
{

}

template<typename T>
Array<T>::Array(unsigned int n)
{
    this->_ptr = new T[n]();
    this->_len = n;
}

template<typename T>
Array<T>::Array(const Array &src)
{
    this->_len = src.size();
    this->_ptr = new T[this->_len];
    for (size_t i = 0; i < this->_len; i++)
        this->_ptr[i] = src[i];
}

template<typename T>
Array<T> &Array<T>::operator=(const Array &src)
{
    if (this == &src)
        return (*this);
    delete [] this->_ptr;
    this->_len = src.size();
    this->_ptr = new T[this->_len];
    for (size_t i = 0; i < this->_len; i++)
        this->_ptr[i] = src[i];
    return (*this);
}

template<typename T>
Array<T>::~Array()
{
    delete [] this->_ptr;
}

template<typename T>
const T &Array<T>::operator[](const size_t n) const
{
    if (n >= this->_len)
        throw std::exception();
    else
        return (this->_ptr[n]);
}

template<typename T>
T &Array<T>::operator[](size_t n)
{
    if (n >= this->_len)
        throw std::exception();
    else
        return (this->_ptr[n]);
}

template<typename T>
size_t Array<T>::size() const
{
    return (this->_len);
}