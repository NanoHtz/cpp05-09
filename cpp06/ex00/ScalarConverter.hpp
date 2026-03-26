#ifndef SCALARCONVERTER_HPP
# define SCALARCONVERTER_HPP

#include <string>
#include <iostream>
#include <climits>
#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <iomanip>

enum Type {
    CHAR,
    INT,
    FLOAT,
    DOUBLE,
    PSEUDO,
    ERROR
};

class ScalarConverter
{   
    private:

        ScalarConverter();
        ~ScalarConverter();
        ScalarConverter(const ScalarConverter &);
		ScalarConverter &operator=(const ScalarConverter &);

    public:

        static void convert(const std::string &str);
};

Type	str_type(const std::string &str, size_t len);
void	printSpecial(const std::string &str);
void	convertChar(const std::string &str);
void	convertInt(const std::string &str);
void	convertFloat(const std::string &str);
void	convertDouble(const std::string &str);

#endif