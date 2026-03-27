#include "ScalarConverter.hpp"

ScalarConverter::ScalarConverter()
{

}

ScalarConverter::~ScalarConverter()
{
    
}

ScalarConverter::ScalarConverter(const ScalarConverter &)
{
    
}

ScalarConverter &ScalarConverter::operator=(const ScalarConverter &)
{
    return (*this);
}

void ScalarConverter::convert(const std::string &str)
{
	size_t size = str.length();
	Type type = str_type(str, size);

	switch (type)
	{
		case PSEUDO:
			printSpecial(str);
			break;
		case CHAR:
			convertChar(str);
			break;
		case INT:
			convertInt(str);
			break;
		case FLOAT:
			convertFloat(str);
			break;
		case DOUBLE:
			convertDouble(str);
			break;
		case ERROR:
			std::cout << "Error: invalid input" << std::endl;
			break;
	}
}

static bool	isPseudo(const std::string& str)
{
	if (str == "nan" || str == "nanf" || str == "+inf" || str == "+inff" || str == "-inf" || str == "-inff")
		return (true);
	return (false);
}

static bool isChar(const std::string &str, size_t len)
{
    if (len == 1 && !std::isdigit((unsigned char)str[0]))
        return (true);
    return (false);
}

static bool isInt(const std::string &str, size_t len)
{
    if (str.empty())
        return (false);
    size_t i = 0;
    if ((str[0] == '+' || str[0] == '-') && len > 1)
        i++;
    for (; i < len; i++)
    {
        if (!std::isdigit(str[i]))
            return (false);
    }
    return (true);
}

static bool isFloat(const std::string &str, size_t len, size_t f)
{
    if (len - 1 != f)
        return (false);

    size_t i = 0;
    int dots = 0;
    int digits = 0;
    if ((str[0] == '+' || str[0] == '-') && len > 1)
        i++;
    for (; i < f; i++)
    {
        if (!std::isdigit(str[i]) && str[i] != '.')
            return (false);
        if (str[i] == '.')
            dots++;
        else
            digits++;
    }
    if (dots != 1 || digits < 1)
        return (false);
    return (true);
}

static bool isDouble(const std::string &str, size_t len)
{
    size_t i = 0;
    int dots = 0;
    int digits = 0;
    if ((str[0] == '+' || str[0] == '-') && len > 1)
        i++;
    for (; i < len; i++)
    {
        if (!std::isdigit(str[i]) && str[i] != '.')
            return (false);
        if (str[i] == '.')
            dots++;
        else
            digits++;
    }
    if (dots != 1 || digits < 1)
        return (false);
    return (true);
}

Type str_type(const std::string &str, size_t len)
{
    size_t  dot = str.find('.');

    if (dot == std::string::npos)
    {
        if (isPseudo(str))
            return PSEUDO;
        if (isChar(str, len))
            return CHAR;
        if (isInt(str, len))
            return INT;
    }
    else
    {
        size_t  f = str.find('f');
        if (f != std::string::npos)
        {
            if (isFloat(str, len, f))
                return FLOAT;
        }
        else
        {
            if (isDouble(str, len))
                return DOUBLE;
        }
    }   
    return (ERROR);
}