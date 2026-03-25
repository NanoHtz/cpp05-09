#include "RobotomyRequestForm.hpp"
#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include <iostream>

RobotomyRequestForm::RobotomyRequestForm() : AForm("Robotomy", 45, 72), _target("Default")
{
}

RobotomyRequestForm::~RobotomyRequestForm()
{

}

RobotomyRequestForm::RobotomyRequestForm(const RobotomyRequestForm &other) : AForm("Robotomy", 45, 72)
{
    this->_target = other._target;
}

RobotomyRequestForm &RobotomyRequestForm::operator=(const RobotomyRequestForm &other)
{
    if (&other != this)
        this->_target = other._target;
    return (*this);
}

RobotomyRequestForm::RobotomyRequestForm(const std::string &target) : AForm("Robotomy", 45, 72),  _target(target)
{
}

void RobotomyRequestForm:: execute(const Bureaucrat &bureu) const
{

    if (this->getSigned() == false)
        throw (AForm::FormNotSigned());
    else if(bureu.getGrade() > this->getExec())
        throw (AForm::GradeTooLowException());
    else
    {
        std::cout << "Dr......" << std::endl;
        srand(time(0));
        int random = rand() % 2;
        if (random == 0)
            std::cout << "informs that " << this->_target << " has been robotomized successfully.\n" << std::endl;
        else
            std::cout << "informs that " << this->_target << " robotomized failed.\n" << std::endl;
    }
}
