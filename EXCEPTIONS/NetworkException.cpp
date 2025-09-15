#include "NetworkException.hpp"

NetworkException::NetworkException(std::string errorMsg) : _message(errorMsg) 
{
    std::string result;
    result = "fatal error: " + _message;
    _message = result;
};

const char *NetworkException::what() const throw()
{
    return _message.c_str();
}