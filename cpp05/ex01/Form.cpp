#include "Form.hpp"
#include "Bureaucrat.hpp"
#include <iostream>

Form::Form() : _signed(false), _name("Default"), _exec(1), _sig(1)
{
}

Form::~Form()
{
    
}

Form::Form(const Form &other) : _signed(other._signed), _name(other._name), _exec(other._exec), _sig(other._sig)
{
}

Form &Form::operator=(const Form &other)
{
    if (&other != this)
        this->_signed = other._signed;
    return (*this);
}

Form::Form(const std::string &name, int exec, int sig) : _signed(false), _name(name), _exec(exec), _sig(sig)
{
    if (_exec > 150 || _sig > 150)
        throw (GradeTooHighException());
    else if (_exec < 1 || _sig < 1)
        throw (GradeTooLowException());
}

std::string Form::getName() const
{
    return (this->_name);
}

bool Form::getSigned() const
{
    return (this->_signed);
}

int Form::getExec() const
{
    return (this->_exec);
}

int Form::getSig() const
{
    return (this->_sig);
}

const char* Form::GradeTooHighException::what() const throw()
{
    const char *c = "Grade too high, grade cannot be higher than 150";
    return (c);
}

const char* Form::GradeTooLowException::what() const throw()
{
    const char *c = "Grade too low, grade cannot be lower than 1";
    return (c);
}

void Form::beSigned(const Bureaucrat &bureu)
{
    if (this->_sig >= bureu.getGrade())
        this->_signed = true;
    else
        throw (GradeTooLowException());
}

std::ostream &operator<<(std::ostream &fd, const Form &other)
{
    fd << "Name: " << other.getName() << std::endl;
    fd << "Signed: " << other.getSigned() << std::endl;
    fd << "Grade for signed: " << other.getSig() << std::endl;
    fd << "Grade for execute: " << other.getExec() << std::endl;
    return (fd);
}