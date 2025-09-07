#pragma once

#include <iostream>
#include <vector>
#include "./Token.hpp"

class Scanner {

    const std::string _source;
    std::vector<Token> _tokens;
    size_t _count;
    size_t _tokenCount;
    size_t _start;
    size_t _current;
    size_t _line;
    public:
        Scanner(std::string source);
        void printTokens();
        void scanTokens();
        void scanToken();
        void getIdentifier();
        void getString();
        void addToken(TokenType type);
        void addToken(TokenType type, std::string lexeme);
        bool isAtEnd();
        char advance();
        char peek();
    // Helpers

    
};