#include "Intern.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include <iostream>

struct FormCreator
{
    std::string _name;
    AForm* (*create)(const std::string &);
};

Intern::Intern()
{

}
Intern::~Intern()
{

}
Intern::Intern(const Intern &)
{

}
Intern &Intern::operator=(const Intern &)
{
    return (*this);
}

static AForm* createPresidential(const std::string &target)
{
    return new PresidentialPardonForm(target);
}

static AForm* createRobotomy(const std::string &target)
{
    return new RobotomyRequestForm(target);
}

static AForm* createShrubbery(const std::string &target)
{
    return new ShrubberyCreationForm(target);
}

AForm *Intern::makeForm(const std::string &name, const std::string &target)
{
    FormCreator forms[3] = {
    {"presidential pardon", &createPresidential},
    {"robotomy request", &createRobotomy},
    {"shrubbery creation", &createShrubbery}
    };
    for (int i = 0; i < 3; i++)
    {
        if (forms[i]._name == name)
        {
            std::cout << "Intern creates " << name << std::endl;
            return forms[i].create(target);
        }
    }
    std::cout << "Intern can't create " << name << std::endl;
    return NULL;
}