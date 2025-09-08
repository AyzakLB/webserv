#pragma once

#include "Token.hpp"
#include "Scanner.hpp"
#include "Server.hpp"


class Parser {
    size_t  _current;
    public:
        Parser() : _current(0){};
        std::vector<Server> parse();

        // main stuff
        const Server &parseServerBlock();
        const Location &Parser::parseLocation(Server server);
        void parseListen(Server &server);
        void parseErrorPage(Server &server);
        void parseClientMaxBodySize(Server &server);
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
        void consume(TokenType type, std::string message);
}