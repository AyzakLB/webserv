#include "ParsingException.hpp"

ParsingException::ParsingException(Token token, std::string error)
{
    std::ostringstream ss;
    std::string tokenString;
    if (token._type == EOF_TOKEN)
        tokenString = "EOF";
    else
        tokenString = token._lexeme;
    ss << "SYNTAX ERROR: "<< error << ", [" << tokenString << "] at line--> " << token._line << std::endl;
    _message = ss.str();
}

const char *ParsingException::what() const throw()
{
    return _message.c_str();
}