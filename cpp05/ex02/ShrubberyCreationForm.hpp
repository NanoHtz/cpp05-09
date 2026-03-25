#ifndef SHRUBBERY_HPP
# define SHRUBBERY_HPP

#include "AForm.hpp"
#include <fstream>

class ShrubberyCreationForm : public AForm
{
    private:

        std::string _target;

    public:

        ShrubberyCreationForm();
        ~ShrubberyCreationForm();
        ShrubberyCreationForm(const ShrubberyCreationForm &other);
		ShrubberyCreationForm &operator=(const ShrubberyCreationForm &other);


        ShrubberyCreationForm(const std::string &target);

        virtual void execute(const Bureaucrat &bureu) const;
};


#endif