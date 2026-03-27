#include "ScalarConverter.hpp"

int main(int argc, char **argv)
{
    if (argc == 2)
    {             
		ScalarConverter::convert(argv[1]);                                           
    }                                                                                                                                        
	else if (argc == 1)
	{
		std::cout << "=== CHAR ===" << std::endl;
		ScalarConverter::convert("a");
		std::cout << std::endl;
		ScalarConverter::convert("*");
		std::cout << std::endl;

		std::cout << "=== INT ===" << std::endl;
		ScalarConverter::convert("0");
		std::cout << std::endl;
		ScalarConverter::convert("-42");
		std::cout << std::endl;
		ScalarConverter::convert("42");
		std::cout << std::endl;

		std::cout << "=== FLOAT ===" << std::endl;
		ScalarConverter::convert("42.0f");
		std::cout << std::endl;
		ScalarConverter::convert("-4.2f");
		std::cout << std::endl;

		std::cout << "=== DOUBLE ===" << std::endl;
		ScalarConverter::convert("42.0");
		std::cout << std::endl;
		ScalarConverter::convert("-4.2");
		std::cout << std::endl;

		std::cout << "=== PSEUDO ===" << std::endl;
		ScalarConverter::convert("nan");
		std::cout << std::endl;
		ScalarConverter::convert("nanf");
		std::cout << std::endl;
		ScalarConverter::convert("+inf");
		std::cout << std::endl;
		ScalarConverter::convert("-inff");
		std::cout << std::endl;

		std::cout << "=== ERROR ===" << std::endl;
		ScalarConverter::convert("hello");
		std::cout << std::endl;
		ScalarConverter::convert("");
		std::cout << std::endl;
	}
	else
	{
		std::cerr << "Usage: ./ScalarConverter <literal>" << std::endl
		return 1;
	}
	return (0);
}