#include "Bureaucrat.hpp"
#include "Form.hpp"
#include <iostream>

int main()
{
    try
    {
        Bureaucrat GoodBureaucrat = Bureaucrat("GoodBureaucrat", 1);
        Form Form00 = Form("20a", 1, 1);

        GoodBureaucrat.signForm(Form00);
    }
    catch (std::exception & e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        Bureaucrat BadBureaucrat00 = Bureaucrat("BadBureaucrat00", 150);
        Form Form00 = Form("20a", 1, 1);
        BadBureaucrat00.signForm(Form00);
    }
    catch (std::exception & e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        Form        Form01 = Form("20b", 170, 170);
    }
    catch (std::exception & e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        Bureaucrat b("Incremented", 1);
        std::cout << b;
        b.incrementGrade();
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        Form f("FormA", 50, 25);
        std::cout << f;
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        Bureaucrat original("Original", 10);
        Bureaucrat copy(original);
        Bureaucrat assigned("Placeholder", 50);
        assigned = original;
        std::cout << original;
        std::cout << copy;
        std::cout << assigned;
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        Form original("OriginalForm", 30, 20);
        Form copy(original);
        Form assigned("Placeholder", 80, 80);
        assigned = original;
        std::cout << original;
        std::cout << copy;
        std::cout << assigned;
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    return (0);
}