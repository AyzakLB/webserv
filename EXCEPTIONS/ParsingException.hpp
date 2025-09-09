#pragma once
#include <stdexcept>
#include "../CONFIGURATION_PARSER/Token.hpp"
#include <sstream>

class ParsingException : public std::exception
{
    std::string _message;
    public:
        ParsingException(Token token, std::string string);
        virtual ~ParsingException() throw() {};
        const char *what() const throw();
};