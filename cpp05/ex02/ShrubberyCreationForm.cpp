#include "ShrubberyCreationForm.hpp"
#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include <iostream>

ShrubberyCreationForm::ShrubberyCreationForm() : AForm("Shrubbery", 137, 145), _target("Default")
{
}

ShrubberyCreationForm::~ShrubberyCreationForm()
{

}

ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm &other) : AForm("Shrubbery", 137, 145)
{
    this->_target = other._target;
}

ShrubberyCreationForm &ShrubberyCreationForm::operator=(const ShrubberyCreationForm &other)
{
    if (&other != this)
        this->_target = other._target;
    return (*this);
}

ShrubberyCreationForm::ShrubberyCreationForm(const std::string &target) : AForm("Shrubbery", 137, 145),  _target(target)
{
}

void ShrubberyCreationForm:: execute(const Bureaucrat &bureu) const
{

    if (this->getSigned() == false)
        throw (AForm::FormNotSigned());
    else if(bureu.getGrade() > this->getExec())
        throw (AForm::GradeTooLowException());
    else
    {
        std::string _archive = this->_target + "_shrubbery";
        std::ofstream archive(_archive.c_str());
        if (archive.is_open())
        {
            archive << "  *  \n";
            archive << " *** \n";
            archive << "*****\n";
            archive << " *** \n";
            archive << "  *  \n";
            archive.close();
        }
        else
            std::cerr << "Error creating archive.\n" << std::endl;
    }
}
