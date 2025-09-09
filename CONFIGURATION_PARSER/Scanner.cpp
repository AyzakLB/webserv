#include <iostream>
#include <vector>
#include "Scanner.hpp"


TokenType getStringTokenType(std::string string);
bool isInCharset(char c, const char *charset);
bool isLowerCase(std::string string);
std::string toUpperCase(std::string string);

Scanner::Scanner(std::string source) : _source(source) 
{
    _count = 0;
    _tokenCount = 0;
    _start = 0;
    _current = 0;
    _line = 1;
}

void Scanner::printTokens()
{
    for (size_t i = 0; i < _tokenCount; i++)
        _tokens[i].printToken();
}

void Scanner::scanTokens() 
{
    while (!isAtEnd()) {
            _start = _current;
            scanToken();
            }
        _tokens.push_back(Token(EOF_TOKEN, "", _line));
        _tokenCount++;
}

    void Scanner::scanToken()
    {
        char c = advance();
        switch (c) {
            case '{': addToken(BRACKET_LEFT); break;
            case '}': addToken(BRACKET_RIGHT); break;
            case ';': addToken(SEMICOLON); break;
            case '"': getString(); break;
            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n': _line++; break;
            case '#': {
                while (peek() != '\n' && !isAtEnd()) 
                    advance();
                    }
                    break;
            default:
                    getIdentifier();
                break;
        }
    }

    void Scanner::getIdentifier()
    {
        while (isInCharset(peek(), "{};\" \r\t\n#") == 0)
            advance();
        std::string lexeme = _source.substr(_start, _current - _start);
        if (isLowerCase(lexeme))
        {
            std::string upperLexeme = toUpperCase(lexeme);
                if (stringToToken.find(upperLexeme) != stringToToken.end())
                {
                    addToken(static_cast<TokenType>(stringToToken.find(upperLexeme)->second));
                    return ;
                }
        } else if (stringToToken.find(lexeme) != stringToToken.end())
        {
            addToken(static_cast<TokenType>(stringToToken.find(lexeme)->second));
            return ;
        }
        addToken(getStringTokenType(lexeme), lexeme);
    }

    void Scanner::getString()
    {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') 
                _line++;
            advance();
        }
        if (isAtEnd()) {
            std::cerr << "Unterminated string at line " << _line << std::endl;
            return;
        }
        advance();
        std::string lexeme = _source.substr(_start + 1, _current - _start - 2);
        addToken(getStringTokenType(lexeme), lexeme);
    }

bool Scanner::isAtEnd() 
{
    return _current >= _source.length();
}
    
char Scanner::advance()
{
    _current++;
    return _source[_current - 1];
}

    void Scanner::addToken(TokenType type)
    {
        addToken(type, "");
    }

    void Scanner::addToken(TokenType type, std::string lexeme)
    {
        if (lexeme.empty())
           lexeme = _source.substr(_start, _current - _start);
        _tokens.push_back(Token(type, lexeme, _line));
        _tokenCount++;
    }

    char Scanner::peek() {
        if (isAtEnd())
            return '\0';
        return _source[_current];
    }

/////////////////////////////////////// getters ////////////////////////////////////
const std::vector<Token> &Scanner::getTokens() const 
{ 
    return _tokens; 
};

////////////////////////////////////// HELPERS ////////////////////////////////////
bool isInCharset(char c, const char *charset)
{
    for (int i = 0; charset[i] != '\0'; i++) {
        if (c == charset[i])
            return true;
    }
    return false;
}
bool isLowerCase(std::string string)
{
    for (size_t i = 0; i < string.length(); i++) {
        char c = string[i];
        if ((c < 'a' || c > 'z') && (c != '_')) {
            return false;
        }
    }
    return true;
}

std::string toUpperCase(std::string string)
{
    for (size_t i = 0; i < string.length(); i++) {
        char c = string[i];
        if (c >= 'a' && c <= 'z') {
            string[i] = c - 32;
        }
    }
    return string;
}

bool isNumber(std::string string)
{
    for (size_t i = 0; i < string.length(); i++) {
        char c = string[i];
        if (c < '0' || c > '9') {
            return false;
        }
    }
    return true;
}

TokenType getStringTokenType(std::string string)
{
    if (string[0] == '/')
        return VALUE_PATH;
    else if (isNumber(string))
        return VALUE_NUMBER;
    return VALUE_STRING;
}