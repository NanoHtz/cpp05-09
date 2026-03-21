#ifndef FORM_HPP
# define FORM_HPP

#include "Bureaucrat.hpp"

class Form
{
    private:

        bool                _signed;
        const std::string   _name;
        const int           _exec;
        const int           _sig;

    public:
        //canonical
        Form();
        ~Form();
        Form(const Form &other);
		Form &operator=(const Form &other);
        //getters
        bool getSigned() const;
        std::string getName() const;
        int getExec() const;
        int getSig() const;
        //Exceptions
        class GradeTooHighException : public std::exception
        {
            public:
                const char* what() const throw();
        };
        class GradeTooLowException : public std::exception
        {
            public:
                const char* what() const throw();
        };
        //Others
        Form(const std::string &name, int exec, int sig);
        void beSigned(const Bureaucrat &bureu);
        
};

std::ostream &operator<<(std::ostream &fd, const Form &other);

#endif