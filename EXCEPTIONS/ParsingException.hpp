#pragma once
#include <stdexcept>
#include "../CONFIGURATION_PARSER/Token.hpp"
#include <sstream>

class ParsingException : public std::exception
{
    std::string _message;
    public:
        ParsingException(std::string errorMessage);
        ParsingException(Token token, std::string errorMessage);
        virtual ~ParsingException() throw() {};
        const char *what() const throw();
};