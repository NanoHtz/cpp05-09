#include "Bureaucrat.hpp"
#include <iostream>

int main()
{
    try
    {
        Bureaucrat GoodBureaucrat = Bureaucrat("GoodBureaucrat", 75);

        std::cout << GoodBureaucrat;
        std::cout << GoodBureaucrat.getGrade() << std::endl;
        GoodBureaucrat.incrementGrade();
        std::cout << "GoodBureaucrat grade +1:" << std::endl;
        std::cout << GoodBureaucrat.getGrade() << std::endl;
    }
    catch (std::exception & e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        Bureaucrat BadBureaucrat00 = Bureaucrat("BadBureaucrat00", 1);

        std::cout << BadBureaucrat00;
        std::cout << BadBureaucrat00.getGrade() << std::endl;
        std::cout << "BadBureaucrat00 grade +1:" << std::endl;
        BadBureaucrat00.incrementGrade();
        std::cout << BadBureaucrat00.getGrade() << std::endl;
    }
    catch (std::exception & e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        std::cout << std::endl <<"BadBureaucrat01:" << std::endl;
        Bureaucrat BadBureaucrat01 = Bureaucrat("BadBureaucrat01", -10);
    }
    catch (std::exception & e)
    {
        std::cout << e.what() << std::endl;
    }

    return (0);
}