#include "PmergeMe.hpp"
#include <climits>
#include <cctype>
#include <ctime>
#include <stdexcept>
#include <string>
#include <utility>

typedef std::pair<int, int> t_intPair;
typedef std::pair<int, std::pair<std::size_t, bool> > t_vectorNode;
typedef std::pair<int, std::pair<std::size_t, bool> > t_dequeNode;

PmergeMe::PmergeMe() : _vectorTime(0.0), _dequeTime(0.0)
{
}

PmergeMe::PmergeMe(int ac, char **av) : _vectorTime(0.0), _dequeTime(0.0)
{
    int value;

    if (ac < 2)
        throw std::runtime_error("Error");
    for (int i = 1; i < ac; i++)
    {
        if (!parsePositiveInt(av[i], value))
            throw std::runtime_error("Error");
        _vectorInput.push_back(value);
        _dequeInput.push_back(value);
    }
}

PmergeMe::PmergeMe(const PmergeMe &other) : _vectorInput(other._vectorInput),
    _dequeInput(other._dequeInput), _vectorSorted(other._vectorSorted),
    _dequeSorted(other._dequeSorted), _vectorTime(other._vectorTime),
    _dequeTime(other._dequeTime)
{
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    if (this != &other)
    {
        _vectorInput = other._vectorInput;
        _dequeInput = other._dequeInput;
        _vectorSorted = other._vectorSorted;
        _dequeSorted = other._dequeSorted;
        _vectorTime = other._vectorTime;
        _dequeTime = other._dequeTime;
    }
    return (*this);
}

PmergeMe::~PmergeMe()
{
}

bool PmergeMe::parsePositiveInt(const std::string &token, int &value)
{
    int result = 0;

    if (token.empty())
        return (false);
    for (std::string::size_type i = 0; i < token.length(); i++)
    {
        if (!std::isdigit(static_cast<unsigned char>(token[i])))
            return (false);
        if (result > (INT_MAX - (token[i] - '0')) / 10)
            return (false);
        result = (result * 10) + (token[i] - '0');
    }
    if (result <= 0)
        return (false);
    value = result;
    return (true);
}

std::vector<std::size_t> PmergeMe::buildJacobsthalOrder(std::size_t size)
{
    std::vector<std::size_t> order;
    std::size_t previous = 0;
    std::size_t last = 1;
    std::size_t beforeLast = 0;
    std::size_t current;

    while (previous < size)
    {
        current = last;
        if (current > size)
            current = size;
        while (current > previous)
        {
            order.push_back(current - 1);
            current--;
        }
        previous = last;
        if (previous > size)
            previous = size;
        current = last + (2 * beforeLast);
        beforeLast = last;
        last = current;
    }
    return (order);
}

std::size_t PmergeMe::findUpperBoundVector(
    const std::vector<t_vectorNode> &chain, std::size_t groupId) const
{
    for (std::size_t i = 0; i < chain.size(); i++)
    {
        if (chain[i].second.second && chain[i].second.first == groupId)
            return (i);
    }
    return (chain.size());
}

std::size_t PmergeMe::findUpperBoundDeque(
    const std::deque<t_dequeNode> &chain, std::size_t groupId) const
{
    for (std::size_t i = 0; i < chain.size(); i++)
    {
        if (chain[i].second.second && chain[i].second.first == groupId)
            return (i);
    }
    return (chain.size());
}

std::size_t PmergeMe::lowerBoundVector(const std::vector<t_vectorNode> &chain,
    int value, std::size_t end) const
{
    std::size_t left = 0;
    std::size_t right = end;
    std::size_t middle;

    while (left < right)
    {
        middle = left + ((right - left) / 2);
        if (chain[middle].first < value)
            left = middle + 1;
        else
            right = middle;
    }
    return (left);
}

std::size_t PmergeMe::lowerBoundDeque(const std::deque<t_dequeNode> &chain,
    int value, std::size_t end) const
{
    std::size_t left = 0;
    std::size_t right = end;
    std::size_t middle;

    while (left < right)
    {
        middle = left + ((right - left) / 2);
        if (chain[middle].first < value)
            left = middle + 1;
        else
            right = middle;
    }
    return (left);
}

std::vector<int> PmergeMe::sortVector(const std::vector<int> &input) const
{
    std::vector<t_intPair> pairs;
    std::vector<t_intPair> sortedPairs;
    std::vector<t_vectorNode> chain;
    std::vector<t_vectorNode> pending;
    std::vector<std::size_t> order;
    std::vector<int> result;
    bool hasStraggler = (input.size() % 2 != 0);
    int straggler = 0;

    if (input.size() <= 1)
        return (input);
    for (std::size_t i = 0; i + 1 < input.size(); i += 2)
    {
        if (input[i] <= input[i + 1])
            pairs.push_back(std::make_pair(input[i], input[i + 1]));
        else
            pairs.push_back(std::make_pair(input[i + 1], input[i]));
    }
    if (hasStraggler)
        straggler = input[input.size() - 1];
    if (!pairs.empty())
    {
        std::vector<int> largerValues;
        std::vector<int> sortedLargeValues;

        for (std::size_t i = 0; i < pairs.size(); i++)
            largerValues.push_back(pairs[i].second);
        sortedLargeValues = sortVector(largerValues);
        for (std::size_t i = 0; i < sortedLargeValues.size(); i++)
        {
            for (std::size_t j = 0; j < pairs.size(); j++)
            {
                if (pairs[j].second == sortedLargeValues[i])
                {
                    sortedPairs.push_back(pairs[j]);
                    pairs.erase(pairs.begin() + j);
                    break ;
                }
            }
        }
    }
    if (!sortedPairs.empty())
        chain.push_back(std::make_pair(sortedPairs[0].first,
            std::make_pair(0, false)));
    for (std::size_t i = 0; i < sortedPairs.size(); i++)
    {
        chain.push_back(std::make_pair(sortedPairs[i].second,
            std::make_pair(i, true)));
        if (i > 0)
            pending.push_back(std::make_pair(sortedPairs[i].first,
                std::make_pair(i, false)));
    }
    order = buildJacobsthalOrder(pending.size());
    for (std::size_t i = 0; i < order.size(); i++)
    {
        t_vectorNode current = pending[order[i]];
        std::size_t upperBound = findUpperBoundVector(chain,
                current.second.first);
        std::size_t insertAt = lowerBoundVector(chain, current.first, upperBound);
        chain.insert(chain.begin() + insertAt, current);
    }
    if (hasStraggler)
    {
        std::size_t insertAt = lowerBoundVector(chain, straggler, chain.size());
        chain.insert(chain.begin() + insertAt,
            std::make_pair(straggler, std::make_pair(chain.size(), false)));
    }
    for (std::size_t i = 0; i < chain.size(); i++)
        result.push_back(chain[i].first);
    return (result);
}

std::deque<int> PmergeMe::sortDeque(const std::deque<int> &input) const
{
    std::deque<t_intPair> pairs;
    std::deque<t_intPair> sortedPairs;
    std::deque<t_dequeNode> chain;
    std::deque<t_dequeNode> pending;
    std::vector<std::size_t> order;
    std::deque<int> result;
    bool hasStraggler = (input.size() % 2 != 0);
    int straggler = 0;

    if (input.size() <= 1)
        return (input);
    for (std::size_t i = 0; i + 1 < input.size(); i += 2)
    {
        if (input[i] <= input[i + 1])
            pairs.push_back(std::make_pair(input[i], input[i + 1]));
        else
            pairs.push_back(std::make_pair(input[i + 1], input[i]));
    }
    if (hasStraggler)
        straggler = input[input.size() - 1];
    if (!pairs.empty())
    {
        std::deque<int> largerValues;
        std::deque<int> sortedLargeValues;

        for (std::size_t i = 0; i < pairs.size(); i++)
            largerValues.push_back(pairs[i].second);
        sortedLargeValues = sortDeque(largerValues);
        for (std::size_t i = 0; i < sortedLargeValues.size(); i++)
        {
            for (std::size_t j = 0; j < pairs.size(); j++)
            {
                if (pairs[j].second == sortedLargeValues[i])
                {
                    sortedPairs.push_back(pairs[j]);
                    pairs.erase(pairs.begin() + j);
                    break ;
                }
            }
        }
    }
    if (!sortedPairs.empty())
        chain.push_back(std::make_pair(sortedPairs[0].first,
            std::make_pair(0, false)));
    for (std::size_t i = 0; i < sortedPairs.size(); i++)
    {
        chain.push_back(std::make_pair(sortedPairs[i].second,
            std::make_pair(i, true)));
        if (i > 0)
            pending.push_back(std::make_pair(sortedPairs[i].first,
                std::make_pair(i, false)));
    }
    order = buildJacobsthalOrder(pending.size());
    for (std::size_t i = 0; i < order.size(); i++)
    {
        t_dequeNode current = pending[order[i]];
        std::size_t upperBound = findUpperBoundDeque(chain,
                current.second.first);
        std::size_t insertAt = lowerBoundDeque(chain, current.first, upperBound);
        chain.insert(chain.begin() + insertAt, current);
    }
    if (hasStraggler)
    {
        std::size_t insertAt = lowerBoundDeque(chain, straggler, chain.size());
        chain.insert(chain.begin() + insertAt,
            std::make_pair(straggler, std::make_pair(chain.size(), false)));
    }
    for (std::size_t i = 0; i < chain.size(); i++)
        result.push_back(chain[i].first);
    return (result);
}

void PmergeMe::sort()
{
    clock_t start;
    clock_t end;

    start = std::clock();
    _vectorSorted = sortVector(_vectorInput);
    end = std::clock();
    _vectorTime = (static_cast<double>(end - start) * 1000000.0)
        / CLOCKS_PER_SEC;
    start = std::clock();
    _dequeSorted = sortDeque(_dequeInput);
    end = std::clock();
    _dequeTime = (static_cast<double>(end - start) * 1000000.0)
        / CLOCKS_PER_SEC;
}

const std::vector<int> &PmergeMe::getVectorInput(void) const
{
    return (_vectorInput);
}

const std::vector<int> &PmergeMe::getVectorSorted(void) const
{
    return (_vectorSorted);
}

const std::deque<int> &PmergeMe::getDequeSorted(void) const
{
    return (_dequeSorted);
}

double PmergeMe::getVectorTime(void) const
{
    return (_vectorTime);
}

double PmergeMe::getDequeTime(void) const
{
    return (_dequeTime);
}
