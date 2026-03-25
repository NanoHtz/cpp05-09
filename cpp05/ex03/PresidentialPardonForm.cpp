#include "PresidentialPardonForm.hpp"
#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include <iostream>

PresidentialPardonForm::PresidentialPardonForm() : AForm("Pardon" ,25, 5), _target("Default")
{
}

PresidentialPardonForm::~PresidentialPardonForm()
{

}

PresidentialPardonForm::PresidentialPardonForm(const PresidentialPardonForm &other) : AForm("Pardon" ,25, 5)
{
    this->_target = other._target;
}

PresidentialPardonForm &PresidentialPardonForm::operator=(const PresidentialPardonForm &other)
{
    if (&other != this)
        this->_target = other._target;
    return (*this);
}

PresidentialPardonForm::PresidentialPardonForm(const std::string &target) : AForm("Pardon" ,25, 5) ,  _target(target)
{
}

void PresidentialPardonForm:: execute(const Bureaucrat &bureu) const
{

    if (this->getSigned() == false)
        throw (AForm::FormNotSigned());
    else if(bureu.getGrade() > this->getExec())
        throw (AForm::GradeTooLowException());
    else
        std::cout << "Informs that "<< this->_target <<" has been pardoned by Zaphod Beeblebrox." << std::endl;
}
