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

std::map<std::string, int> createStringToTokenMap() {
    std::map<std::string, int> m;
    m["SERVER"] = SERVER;
    m["LOCATION"] = LOCATION;
    m["LISTEN"] = LISTEN;
    m["ERROR_PAGE"] = ERROR_PAGE;
    m["CLIENT_MAX_BODY_SIZE"] = CLIENT_MAX_BODY_SIZE;
    m["ROOT"] = ROOT;
    m["INDEX"] = INDEX;
    m["AUTOINDEX"] = AUTOINDEX;
    m["ALLOW_METHODS"] = ALLOW_METHODS;
    m["RETURN"] = RETURN;
    m["UPLOAD_PATH"] = UPLOAD_PATH;
    m["CGI"] = CGI;
    m["BRACKET_LEFT"] = BRACKET_LEFT;
    m["BRACKET_RIGHT"] = BRACKET_RIGHT;
    m["SEMICOLON"] = SEMICOLON;
    m["VALUE_NUMBER"] = VALUE_NUMBER;
    m["VALUE_STRING"] = VALUE_STRING;
    m["VALUE_PATH"] = VALUE_PATH;
    m["EOF_TOKEN"] = EOF_TOKEN;
    return m;
}

std::map<int, std::string> createTokenToStringMap() {
    std::map<int, std::string> m;
    m[SERVER] = "SERVER";
    m[LOCATION] = "LOCATION";
    m[LISTEN] = "LISTEN";
    m[ERROR_PAGE] = "ERROR_PAGE";
    m[CLIENT_MAX_BODY_SIZE] = "CLIENT_MAX_BODY_SIZE";
    m[ROOT] = "ROOT";
    m[INDEX] = "INDEX";
    m[AUTOINDEX] = "AUTOINDEX";
    m[ALLOW_METHODS] = "ALLOW_METHODS";
    m[RETURN] = "RETURN";
    m[UPLOAD_PATH] = "UPLOAD_PATH";
    m[CGI] = "CGI";
    m[BRACKET_LEFT] = "BRACKET_LEFT";
    m[BRACKET_RIGHT] = "BRACKET_RIGHT";
    m[SEMICOLON] = "SEMICOLON";
    m[VALUE_NUMBER] = "VALUE_NUMBER";
    m[VALUE_STRING] = "VALUE_STRING";
    m[VALUE_PATH] = "VALUE_PATH";
    m[EOF_TOKEN] = "EOF_TOKEN";
    return m;
}

std::map<int, std::string> tokenToString = createTokenToStringMap();
std::map<std::string, int> stringToToken = createStringToTokenMap();

class Token {
    public:
    const TokenType _type;
    const std::string _lexeme;
    const int _line;
    Token(TokenType type, std::string lexeme, size_t line) : _type(type), _lexeme(lexeme), _line(line){};
    void printToken();

};

void Token::printToken()
{
    std:: cout << "--> " << _line << " " << tokenToString.find(_type)->second << " [" << _lexeme << "]" << std::endl;
}