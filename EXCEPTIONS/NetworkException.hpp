#include <exception>
#include <iostream>

class NetworkException : public std::exception
{
    std::string _message;
    public:
        NetworkException(std::string errorMsg);
        virtual ~NetworkException() throw() {};
        const char* what() const throw();
};