#ifndef PRESIDENTIAL_HPP
# define PRESIDENTIAL_HPP

#include "AForm.hpp"

class PresidentialPardonForm : public AForm
{
    private:

        std::string _target;

    public:

        PresidentialPardonForm();
        ~PresidentialPardonForm();
        PresidentialPardonForm(const PresidentialPardonForm &other);
		PresidentialPardonForm &operator=(const PresidentialPardonForm &other);


        PresidentialPardonForm(const std::string &target);

        virtual void execute(const Bureaucrat &bureu) const;
};


#endif