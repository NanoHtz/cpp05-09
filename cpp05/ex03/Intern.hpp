#ifndef INTERN_HPP
# define INTERN_HPP

#include "AForm.hpp"

class Intern
{
    private:

    public:

        Intern();
        ~Intern();
        Intern(const Intern &other);
		Intern &operator=(const Intern &other);

        AForm *makeForm(const std::string &name, const std::string &target);
};


#endif