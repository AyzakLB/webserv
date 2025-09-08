#pragma once

#include <vector>
#include <iostream>
#include <map>

// static size_t _tokenToStringSize = sizeof(_tokenToString) / sizeof(_tokenToString[0]);

enum TokenType {
    ////////////////////////// BLOCKS //////////////////////////
    SERVER,
    LOCATION,
    
    ////////////////////////// DIRECTIVES //////////////////////////
    
    // Network Configuration
    LISTEN,
    
    // Error handling
    ERROR_PAGE,
    
    // Request Body Limits
    CLIENT_MAX_BODY_SIZE,
    
    // File Serving
    ROOT,
    INDEX,
    AUTOINDEX,
    
    //HTTP Method Control
    ALLOW_METHODS,
    
    // Redirections
    RETURN,
    
    //Redirections
    UPLOAD_PATH,
    
    // CGI configurations
    CGI,
    
    // MISS
    BRACKET_LEFT,
    BRACKET_RIGHT,
    SEMICOLON,
    VALUE_NUMBER,
    VALUE_STRING,
    VALUE_PATH,
    EOF_TOKEN,
};

std::map<std::string, int> createStringToTokenMap();

std::map<int, std::string> createTokenToStringMap();

extern std::map<int, std::string> tokenToString;
extern std::map<std::string, int> stringToToken;

class Token {
    public:
    const TokenType _type;
    const std::string _lexeme;
    const int _line;
    Token(TokenType type, std::string lexeme, size_t line) : _type(type), _lexeme(lexeme), _line(line){};
    void printToken();

};
