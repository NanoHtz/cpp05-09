#ifndef BUREAUCRAT_HPP
# define BUREAUCRAT_HPP

# include <string>
# include <exception>
# include <ostream>

class AForm;

class Bureaucrat
{
    private:

        const std::string _name;
        int         _grade;

    public:
        //Canonical
        Bureaucrat();
		~Bureaucrat();
		Bureaucrat(const Bureaucrat &other);
		Bureaucrat &operator=(const Bureaucrat &other);
        //Other methods
        Bureaucrat(const std::string &name, int grade);
        std::string getName() const;
        int getGrade() const;
        void incrementGrade();
        void decrementGrade();
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

        void signForm(AForm &other);
        void executeForm(AForm const & form) const;

};

std::ostream &operator<<(std::ostream &fd, const Bureaucrat &other);

#endif