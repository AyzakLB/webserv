#pragma once

#include "Token.hpp"
#include "Scanner.hpp"
#include "Server.hpp"
// #include "Location.hpp"
#include "../EXCEPTIONS/ParsingException.hpp"
#include "../UTILITIES/Header.hpp"
#include <string>

class Parser {
    std::string _source;
    std::vector<Token> _tokens;
    size_t  _current;
    public:
        Parser(std::string source);
        std::vector<Server> parse();

        // main stuff
        void parseServerBlock(Server &server);
        void parseLocation(Server &server);
        void parseDefaultLocation(Server &server);
        void parseListen(Server &server);
        void parseErrorPage(Location &location);
        void parseClientMaxBodySize(Location &location);
        void parseAllowMethods(Location &location);
        void parseReturn(Location &location);
        void parseIndex(Location &location);
        void parseRoot(Location &location);
        void parseAutoIndex(Location &location);
        void parseUploadPath(Location &location);
        void parseCGI(Location &location);

        // Helpers
        
        Token peek();
        Token advance();
        bool isAtEnd();
        Token consume(TokenType type, std::string message);
        void syntaxCheck(TokenType expecteddToken, std::string error);
};