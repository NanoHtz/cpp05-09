#include "AForm.hpp"
#include "Bureaucrat.hpp"
#include <iostream>

AForm::AForm() : _signed(false), _name("Default"), _exec(1), _sig(1)
{
}

AForm::~AForm()
{
    
}

AForm::AForm(const AForm &other) : _signed(other._signed), _name(other._name), _exec(other._exec), _sig(other._sig)
{
}

AForm &AForm::operator=(const AForm &other)
{
    if (&other != this)
        this->_signed = other._signed;
    return (*this);
}

AForm::AForm(const std::string &name, int exec, int sig) : _signed(false), _name(name), _exec(exec), _sig(sig)
{
    if (_exec > 150 || _sig > 150)
        throw (GradeTooLowException());
    else if (_exec < 1 || _sig < 1)
        throw (GradeTooHighException());
}

std::string AForm::getName() const
{
    return (this->_name);
}

bool AForm::getSigned() const
{
    return (this->_signed);
}

int AForm::getExec() const
{
    return (this->_exec);
}

int AForm::getSig() const
{
    return (this->_sig);
}

const char* AForm::GradeTooHighException::what() const throw()
{
    const char *c = "Grade too high, grade cannot be higher than 150";
    return (c);
}

const char* AForm::GradeTooLowException::what() const throw()
{
    const char *c = "Grade too low, grade cannot be lower than 1";
    return (c);
}

void AForm::beSigned(const Bureaucrat &bureu)
{
    if (this->_sig >= bureu.getGrade())
        this->_signed = true;
    else
        throw (GradeTooLowException());
}

std::ostream &operator<<(std::ostream &fd, const AForm &other)
{
    fd << "Name: " << other.getName() << std::endl;
    fd << "Signed: " << other.getSigned() << std::endl;
    fd << "Grade for signed: " << other.getSig() << std::endl;
    fd << "Grade for execute: " << other.getExec() << std::endl;
    return (fd);
}

const char* AForm::FormNotSigned::what() const throw()
{
    const char *c = "Form not signed";
    return (c);
}