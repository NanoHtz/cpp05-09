#ifndef AForm_HPP
# define AForm_HPP

#include "Bureaucrat.hpp"

class AForm
{
    private:

        bool                _signed;
        const std::string   _name;
        const int           _exec;
        const int           _sig;

    public:
        //canonical
        AForm();
        virtual ~AForm();
        AForm(const AForm &other);
		AForm &operator=(const AForm &other);
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
        class FormNotSigned : public std::exception
        {
            public:
                const char* what() const throw();
        };
        //Others
        AForm(const std::string &name, int exec, int sig);
        void beSigned(const Bureaucrat &bureu);
        virtual void execute(const Bureaucrat &bureu) const = 0;
        
};

std::ostream &operator<<(std::ostream &fd, const AForm &other);

#endif