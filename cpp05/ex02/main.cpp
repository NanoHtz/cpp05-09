#include "Bureaucrat.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "AForm.hpp"
#include <iostream>

int main()
{
    srand(time(0));
    try
    {
        Bureaucrat GoodBureaucrat = Bureaucrat("GoodBureaucrat", 1);
        RobotomyRequestForm Robot = RobotomyRequestForm("Robot");
        GoodBureaucrat.signForm(Robot);
        GoodBureaucrat.executeForm(Robot);
    }
    catch (std::exception & e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        Bureaucrat GoodBureaucrat01 = Bureaucrat("GoodBureaucrat01", 1);
        PresidentialPardonForm Presidential = PresidentialPardonForm("Presidential");
        GoodBureaucrat01.signForm(Presidential);
        GoodBureaucrat01.executeForm(Presidential);
    }
    catch (std::exception & e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        Bureaucrat GoodBureaucrat02 = Bureaucrat("GoodBureaucrat02", 1);
        ShrubberyCreationForm Shrubbery = ShrubberyCreationForm("Shrubbery");
        GoodBureaucrat02.signForm(Shrubbery);
        GoodBureaucrat02.executeForm(Shrubbery);
    }
    catch (std::exception & e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        Bureaucrat BadBureaucrat = Bureaucrat("BadBureaucrat", 150);
        RobotomyRequestForm Robot01 = RobotomyRequestForm("Robot01");
        PresidentialPardonForm Presidential01 = PresidentialPardonForm("Presidential01");
        ShrubberyCreationForm Shrubbery01 = ShrubberyCreationForm("Shrubbery01");

        BadBureaucrat.signForm(Robot01);
        BadBureaucrat.executeForm(Robot01);

        BadBureaucrat.signForm(Presidential01);
        BadBureaucrat.executeForm(Presidential01);

        BadBureaucrat.signForm(Shrubbery01);
        BadBureaucrat.executeForm(Shrubbery01);
    }
    catch (std::exception & e)
    {
        std::cout << e.what() << std::endl;
    }

    return (0);
}