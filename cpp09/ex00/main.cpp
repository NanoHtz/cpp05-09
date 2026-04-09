#include "BitcoinExchange.hpp"
#include <iostream>
#include <ostream>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Error: invalid number of arguments." << std::endl;
        return 1;
    }

    BitcoinExchange btc;
    btc.load_csv("data.csv");
    btc.process_input(argv[1]);

    return 0;
}