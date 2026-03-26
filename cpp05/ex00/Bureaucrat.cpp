#include "Bureaucrat.hpp"

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
        throw (GradeTooLowException());
    else if (grade < 1)
        throw (GradeTooHighException());
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
    return ("Grade too high, grade cannot be higher than 1");
}

const char* Bureaucrat::GradeTooLowException::what() const throw()
{
    return ("Grade too low, grade cannot be lower than 150");
}

std::ostream &operator<<(std::ostream &fd, const Bureaucrat &other)
{
    fd << other.getName() << ", bureaucrat grade " << other.getGrade() << "." << std::endl;
    return (fd);
}