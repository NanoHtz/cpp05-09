#include "Bureaucrat.hpp"
#include "Form.hpp"

#include <iostream>

Bureaucrat::Bureaucrat() : _name("Default"), _grade(150)
{
}

Bureaucrat::~Bureaucrat()
{
    
}

Bureaucrat::Bureaucrat(const Bureaucrat &other) : _name(other._name), _grade(other._grade)
{
}

Bureaucrat &Bureaucrat::operator=(const Bureaucrat &other)
{
    if (&other != this)
        this->_grade = other._grade;
    return (*this);
}

Bureaucrat::Bureaucrat(const std::string &name, int grade) : _name(name)
{
    if (grade > 150)
        throw (GradeTooHighException());
    else if (grade < 1)
        throw (GradeTooLowException());
    else
        this->_grade = grade;
}

std::string Bureaucrat::getName() const
{
    return (this->_name);
}

int Bureaucrat::getGrade() const
{
    return (this->_grade);
}

void Bureaucrat::incrementGrade()
{
    if (this->_grade >= 2)
        this->_grade -= 1;
    else
        throw (GradeTooHighException());
}

void Bureaucrat::decrementGrade()
{
    if (this->_grade <= 149)
        this->_grade += 1;
    else
        throw (GradeTooLowException());
}

const char* Bureaucrat::GradeTooHighException::what() const throw()
{
    const char *c = "Grade too high, grade cannot be higher than 150";
    return (c);
}

const char* Bureaucrat::GradeTooLowException::what() const throw()
{
    const char *c = "Grade too low, grade cannot be lower than 1";
    return (c);
}

std::ostream &operator<<(std::ostream &fd, const Bureaucrat &other)
{
    fd << other.getName() << ", bureaucrat grade " << other.getGrade() << "." << std::endl;
    return (fd);
}

void Bureaucrat::signForm(Form &other)
{
    try
    {
        other.beSigned(*this);
        std::cout << this->getName() << " signed " << other.getName() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << this->getName() << "  couldn't sign  " << other.getName() << " because " << e.what() << std::endl;
    }
}