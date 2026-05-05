#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <deque>
#include <string>
#include <utility>
#include <vector>

class PmergeMe
{
    private:
        std::vector<int> _vectorInput;
        std::deque<int> _dequeInput;
        std::vector<int> _vectorSorted;
        std::deque<int> _dequeSorted;
        double _vectorTime;
        double _dequeTime;

        static bool parsePositiveInt(const std::string &token, int &value);
        static std::vector<std::size_t> buildJacobsthalOrder(std::size_t size);

        std::vector<int> sortVector(const std::vector<int> &input) const;
        std::deque<int> sortDeque(const std::deque<int> &input) const;

        std::size_t findUpperBoundVector(
            const std::vector< std::pair<int, std::pair<std::size_t, bool> > > &chain,
            std::size_t groupId) const;
        std::size_t findUpperBoundDeque(
            const std::deque< std::pair<int, std::pair<std::size_t, bool> > > &chain,
            std::size_t groupId) const;
        std::size_t lowerBoundVector(
            const std::vector< std::pair<int, std::pair<std::size_t, bool> > > &chain,
            int value, std::size_t end) const;
        std::size_t lowerBoundDeque(
            const std::deque< std::pair<int, std::pair<std::size_t, bool> > > &chain,
            int value, std::size_t end) const;

    public:
        PmergeMe();
        PmergeMe(int ac, char **av);
        PmergeMe(const PmergeMe &other);
        PmergeMe &operator=(const PmergeMe &other);
        ~PmergeMe();

        void sort();

        const std::vector<int> &getVectorInput(void) const;
        const std::vector<int> &getVectorSorted(void) const;
        const std::deque<int> &getDequeSorted(void) const;
        double getVectorTime(void) const;
        double getDequeTime(void) const;
};

#endif
