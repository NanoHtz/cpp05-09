#ifndef ROBOTOMY_HPP
# define ROBOTOMY_HPP

#include "AForm.hpp"
#include <cstdlib>
#include <ctime>

class RobotomyRequestForm : public AForm
{
    private:

        std::string _target;

    public:

        RobotomyRequestForm();
        ~RobotomyRequestForm();
        RobotomyRequestForm(const RobotomyRequestForm &other);
		RobotomyRequestForm &operator=(const RobotomyRequestForm &other);


        RobotomyRequestForm(const std::string &target);

        virtual void execute(const Bureaucrat &bureu) const;
};


#endif