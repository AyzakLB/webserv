#include "Parser.hpp"

std::vector<Server> Parser::parse()
{
    std::vector<Server> servers;

    while (isAtEnd() == false)
    {
        if (peek()._type == SERVER)
        {
            Server server = parseServerBlock();
            servers.push_back(server);
        }
        else
        {
            // Handle syntax error later
        }
    }
    return servers;
}

const Server &Parser::parseServerBlock()
{
    Server server;
    consume(SERVER, "SYNTAX ERROR: expected a server");
    consume(BRACKET_LEFT, "SYNTAX ERROR: expected a '{' before server block");
    while (peek()._type != BRACKET_RIGHT && peek()._type != EOF_TOKEN)
    {
        if (peek()._type == LISTEN)
            parseListen(server);
        else if (peek()._type == ERROR_PAGE)
            parseErrorPage(server);
        else if (peek()._type == CLIENT_MAX_BODY_SIZE)
            parseClientMaxBodySize(server);
        else if (peek()._type == LOCATION)
            parseLocation(server);
        else
        {
            // handle syntax error later;
        }
    }
    consume(BRACKET_RIGHT, "SYNTAX ERROR: expected a '}' to close server block");
    return server;
}

const Location &Parser::parseLocation(Server server)
{
    Location location;
    consume(LOCATION, "SYNTAX ERROR: expected a location");
    if (peek()._lexeme[0] != '/')
        
    consume(BRACKET_LEFT, "SYNTAX ERROR: expected a '{' before location block");   
    while (peek()._type != BRACKET_RIGHT && peek()._type != EOF_TOKEN)
    {
        if (peek()._type == ALLOW_METHODS)
            parseAllowMethods(location);
        if (peek()._type == RETURN)
            parseReturn(location);
        if (peek()._type == INDEX)
            parseIndex(location);
        if (peek()._type == ROOT)
            parseRoot(location);
        if (peek()._type == AUTOINDEX)
            parseAutoIndex(location);
        if (peek()._type == UPLOAD_PATH)
            parseUploadPath(location);
        if (peek()._type == CGI)
            parseCGI(location);
        else
        {
            // handle syntax error later;
        }
    }
    consume(BRACKET_RIGHT, "SYNTAX ERROR: expected a '}' to close location block");
    return location;
}


///////////////////////////////////////////////////// HELPRES ////////////////////////////////////////////////////////

Token Parser::peek()
{

    return _tokens[_current];
}

Token Parser::advance()
{
    
}