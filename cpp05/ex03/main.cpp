#include "Bureaucrat.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "AForm.hpp"
#include "Intern.hpp"
#include <iostream>

int main()
{
    srand(time(0));
    try
    {
        Intern inter00 = Intern();
        AForm *Intern01 = inter00.makeForm("presidential pardon", "GoodBureaucrat");
        delete Intern01;
        AForm *Intern02 = inter00.makeForm("robotomy request", "GoodBureaucrat");
        delete Intern02;
        AForm *Intern03 = inter00.makeForm("shrubbery creation", "GoodBureaucrat");
        delete Intern03;
        AForm *Intern04 = inter00.makeForm("ERROR", "GoodBureaucrat");
        delete Intern04;
    }
    catch (std::exception & e)
    {
        std::cout << e.what() << std::endl;
    }

    Bureaucrat boss("Boss", 1);
    Intern someIntern;
    AForm *form = someIntern.makeForm("presidential pardon", "Alice");
    boss.signForm(*form);
    boss.executeForm(*form);
    delete form;

    Bureaucrat boss2("Boss2", 1);
    Intern someIntern2;
    AForm *form2 = someIntern2.makeForm("robotomy request", "Bob");
    boss2.executeForm(*form2);
    delete form2;

    Bureaucrat lowGuy("LowGuy", 150);
    Intern someIntern3;
    AForm *form3 = someIntern3.makeForm("shrubbery creation", "Charlie");
    lowGuy.signForm(*form3);
    lowGuy.executeForm(*form3);
    delete form3;

    return (0);
}