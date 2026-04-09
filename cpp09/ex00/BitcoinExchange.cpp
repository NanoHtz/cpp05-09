#include "BitcoinExchange.hpp"

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cctype>

BitcoinExchange::BitcoinExchange()
{

}

BitcoinExchange::~BitcoinExchange()
{

}
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
{
    this->_map = other._map;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
    if (&other != this)
        this->_map = other._map;
    return (*this);
}

static bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static bool isValidDate(const std::string &date)
{
    if (date.size() != 10)
        return false;
    if (date[4] != '-' || date[7] != '-')
        return false;
    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(date[i]))
            return false;
    }
    int year  = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day   = std::atoi(date.substr(8, 2).c_str());
    if (month < 1 || month > 12)
        return false;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year))
        daysInMonth[1] = 29;
    if (day < 1 || day > daysInMonth[month - 1])
        return false;
    return true;
}

void BitcoinExchange::load_csv(std::string csv)
{
    std::ifstream file(csv.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line))
    {
        size_t pos = line.find(',');
        std::string date = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        float f = atof(value.c_str());
        _map[date] = f;
    }
}

void BitcoinExchange::process_input(std::string input)
{
    std::ifstream file(input.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }
    if (_map.empty())
    {
        std::cerr << "Error: database is empty." << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);

        size_t pos = line.find(" | ");
        if (pos == std::string::npos)
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        std::string date = line.substr(0, pos);
        std::string value = line.substr(pos + 3);

        if (!isValidDate(date))
        {
            std::cerr << "Error: bad input => " << date << std::endl;
            continue;
        }

        char *endptr;
        double d = std::strtod(value.c_str(), &endptr);
        if (endptr == value.c_str() || *endptr != '\0')
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        float f = static_cast<float>(d);

        if (f < 0)
        {
            std::cerr << "Error: not a positive number." << std::endl;
            continue;
        }
        if (f > 1000)
        {
            std::cerr << "Error: too large a number." << std::endl;
            continue;
        }

        std::map<std::string, float>::iterator it = _map.lower_bound(date);
        if (it == _map.end())
            --it;
        else if (it->first != date)
        {
            if (it == _map.begin())
            {
                std::cerr << "Error: bad input => " << date << std::endl;
                continue;
            }
            --it;
        }

        std::cout << date << " => " << f << " = " << it->second * f << std::endl;
    }
}