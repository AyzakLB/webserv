#include "Parser.hpp"
#include <cstdlib>

Parser::Parser(std::string source) : _source(source), _current(0) {}
/////////////////////////////////////////////// MAIN PARSING LOGIC ////////////////////////////////////////////////////////

std::vector<Server> Parser::parse()
{
    std::vector<Server> servers;
    Scanner scanner(_source);
    scanner.scanTokens();
    // scanner.printTokens();
    _tokens = scanner.getTokens();
    while (_tokens[_current]._type != EOF_TOKEN ) // && _current < _tokens.size() - 1 check if you actually need this condition, you probably don't...
    {
        if (peek()._type == SERVER)
        {
            Server server;
            parseServerBlock(server);
            if (server._foundListen == false)
                throw ParsingException("at least ONE listen directive is required in your configuration file. Can't go to a house without an address now can we?");
            servers.push_back(server);
        }
        else
            throw ParsingException(peek(), "expected a server block");
    }
    return servers;
}

void Parser::parseServerBlock(Server &server)
{
    consume(SERVER, "expected a server");
    consume(BRACKET_LEFT, "expected a '{' before server block");
    while (peek()._type != BRACKET_RIGHT && peek()._type != EOF_TOKEN)
    {
        if (peek()._type == LISTEN)
            parseListen(server);
        else if (peek()._type == LOCATION)
            parseLocation(server);
        else
            parseDefaultLocation(server);
    }
    consume(BRACKET_RIGHT, "expected a '}' to close server block");
}
 
void Parser::parseDefaultLocation(Server &server)
{
        if (peek()._type == ERROR_PAGE)
            parseErrorPage(server.getDefaultLocation());
        else if (peek()._type == CLIENT_MAX_BODY_SIZE)
            parseClientMaxBodySize(server.getDefaultLocation());
        else if (peek()._type == ALLOW_METHODS)
            parseAllowMethods(server.getDefaultLocation());
        else if (peek()._type == RETURN)
            parseReturn(server.getDefaultLocation());
        else if (peek()._type == INDEX)
            parseIndex(server.getDefaultLocation());
        else if (peek()._type == ROOT)
            parseRoot(server.getDefaultLocation());
        else if (peek()._type == AUTOINDEX)
            parseAutoIndex(server.getDefaultLocation());
        else if (peek()._type == UPLOAD_PATH)
            parseUploadPath(server.getDefaultLocation());
        else if (peek()._type == CGI)
            parseCGI(server.getDefaultLocation());
        else
            throw ParsingException(peek(), "unexpected token in server blockkk");
}
void Parser::parseLocation(Server &server)
{
    Location location;
    consume(LOCATION, "expected a location");
    Token path = consume(VALUE_PATH, "expected a path after location");
    location.setPath(path._lexeme);
    consume(BRACKET_LEFT, "expected a '{' before location block");
    while (peek()._type != BRACKET_RIGHT && peek()._type != EOF_TOKEN)
    {
        if (peek()._type == ERROR_PAGE)
            parseErrorPage(location);
        else if (peek()._type == CLIENT_MAX_BODY_SIZE)
            parseClientMaxBodySize(location);
        else if (peek()._type == CLIENT_MAX_BODY_SIZE)
            parseClientMaxBodySize(location);
        else if (peek()._type == ALLOW_METHODS)
            parseAllowMethods(location);
        else if (peek()._type == RETURN)
            parseReturn(location);
        else if (peek()._type == INDEX)
            parseIndex(location);
        else if (peek()._type == ROOT)
            parseRoot(location);
        else if (peek()._type == AUTOINDEX)
            parseAutoIndex(location);
        else if (peek()._type == UPLOAD_PATH)
            parseUploadPath(location);
        else if (peek()._type == CGI)
            parseCGI(location);
        else
            throw ParsingException(peek(), "unexpected token in location block");
    }
    consume(BRACKET_RIGHT, "expected a '}' to close location block");
    server.addLocation(path._lexeme, location);
}

///////////////////////////////////////////////////// SERVER DIRECTIVE PARSERS   ////////////////////////////////////////////////////////
void Parser::parseListen(Server &server) 
{       
        if (server._foundListen == false)
            server._foundListen = true;
        consume(LISTEN, "expected a 'listen' directive");
        syntaxCheck(VALUE_STRING, "unexpected value for the address in listen");
        std::string address = advance()._lexeme;
        syntaxCheck(VALUE_NUMBER, "unexpected value for the port in listen");
        std::string port = advance()._lexeme;

        server.addConnection(address, port);
        consume(SEMICOLON, "expected a ';' after listen directive");
}


void Parser::parseErrorPage(Location &location)
{
    consume(ERROR_PAGE, "expected an 'error_page' directive");
    char *end;
    size_t code;
    std::string path;
    std::vector<std::string> error_codes;
    while (peek()._type == VALUE_NUMBER)
        error_codes.push_back(advance()._lexeme);
    if (peek()._type != VALUE_PATH)
        throw ParsingException(peek(), "expected a path after error codes");
    path = advance()._lexeme;
    for (size_t i = 0; i < error_codes.size(); i++)
    {
        code = strtoul(error_codes[i].c_str(), &end,10);
        if ( code < 300 || code > 599)
            throw ParsingException(peek(), "error code must be between 300 and 599");
        location.addErrorPage(error_codes[i], path);
    }
    consume(SEMICOLON, "expected a ';' after error_page directive");
}

size_t parseBodySize(std::string size, Token token);
void Parser::parseClientMaxBodySize(Location &location)
{
    consume(CLIENT_MAX_BODY_SIZE, "expected a 'client_max_body_size' directive");
    if (peek()._type != VALUE_NUMBER && peek()._type != VALUE_STRING)
        throw ParsingException(peek(), "invalid value for client_max_body_size");
    location.setClientMaxBodySize(parseBodySize(peek()._lexeme, peek()));
    advance();
    consume(SEMICOLON, "expected a ';' after client_max_body_size directive");
}
///////////////////////////////////////////////////// LOCATION DIRECTIVE PARSERS   ////////////////////////////////////////////////////////

void Parser::parseAllowMethods(Location &location)
{
    consume(ALLOW_METHODS, "expected an 'allow_methods' directive");
    if (peek()._type != GET && peek()._type != POST && peek()._type != DELETE)
        throw ParsingException(peek(), "invalid method in allow_methods directive");
    location.setMethod(GET, false);
    location.setMethod(POST, false);
    location.setMethod(DELETE, false);
    while (peek()._type == GET || peek()._type == POST || peek()._type == DELETE)
        location.setMethod(advance()._type, true);
    consume(SEMICOLON, "expected a ';' after allow_methods directive");
}
void Parser::parseReturn(Location &location)
{
    consume(RETURN, "expected a 'return' directive");
    if (peek()._type != VALUE_NUMBER && peek()._type != VALUE_PATH)
        throw ParsingException(peek(), "invalid return value 1");
    if (peek()._type == VALUE_NUMBER)
    {
        std::string code = advance()._lexeme;
        if (peek()._type != VALUE_PATH && peek()._type != VALUE_STRING)
            throw ParsingException(peek(), "invalid return value 2 ");
        location.setReturn(code, advance()._lexeme);
    } else
        location.setReturn("", advance()._lexeme);
    consume(SEMICOLON, "expected a ';' after return directive");
}

void Parser::parseRoot(Location &location)
{
    consume(ROOT, "expected a 'root' directive");
    if (peek()._type != VALUE_PATH && peek()._type != VALUE_STRING)
        throw ParsingException(peek(), "invalid root value");
    location.setRoot(advance()._lexeme);
    consume(SEMICOLON, "expected a ';' after root directive");
}

void Parser::parseIndex(Location &location)
{
    consume(INDEX, "expected an 'index' directive");
    while (peek()._type == VALUE_PATH || peek()._type == VALUE_STRING)
    {
        if (peek()._type == VALUE_PATH)
        {
            location.addIndex(advance()._lexeme);
            break ;
        }
        location.addIndex(advance()._lexeme);
    }
    consume(SEMICOLON, "expected a ';' after index directive");
}

void Parser::parseAutoIndex(Location &location)
{
    consume(AUTOINDEX, "expected an 'autoindex' directive");
    if (peek()._lexeme == "on")
        location.setAutoIndex(true);
    else if (peek()._lexeme == "off")
        location.setAutoIndex(false);
    else
        throw ParsingException(peek(), "invalid autoindex value");
    advance();
    consume(SEMICOLON, "expected a ';' after autoindex directive");
}

void Parser::parseUploadPath(Location &location)
{
    consume(UPLOAD_PATH, "expected an 'upload_path' directive");
    if (peek()._type != VALUE_PATH)
        throw ParsingException(peek(), "invalid upload_path value");
    location.setUploadPath(advance()._lexeme);
    consume(SEMICOLON, "expected a ';' after upload_path directive");
}

void Parser::parseCGI(Location &location)
{
    consume(CGI, "expected a 'CGI' directive");
    if (peek()._type != VALUE_STRING || peek()._lexeme[0] != '.')
        throw ParsingException(peek(), "invalid CGI extension");
    std::string extension = advance()._lexeme;
    if (peek()._type != VALUE_PATH)
        throw ParsingException(peek(), "invalid CGI path");
    std::string path = advance()._lexeme;
    location.addCGI(extension, path);
    consume(SEMICOLON, "expected a ';' after CGI directive");
}

///////////////////////////////////////////////////// HELPERS ////////////////////////////////////////////////////////

Token Parser::peek()
{
    return _tokens[_current];
}

Token Parser::advance()
{
   _current++;
   return _tokens[_current - 1]; 
}

void Parser::syntaxCheck(TokenType expecteddToken, std::string error)
{
   if (expecteddToken != peek()._type)
        throw ParsingException(peek(), error);
}

Token Parser::consume(TokenType token, std::string error)
{
    syntaxCheck(token, error);
    advance();
    return _tokens[_current - 1];

}

size_t parseBodySize(std::string size, Token token)
{
    char *endptr = NULL;
    if (token._type == VALUE_NUMBER)
        return strtoul(size.c_str(), &endptr, 10);
    size_t multiplier = 1;
    char unit = size[size.length() - 1];
    if (unit == 'K' || unit == 'k')
        multiplier = 1000;
    else if (unit == 'M' || unit == 'm')
        multiplier = 1000000;
    else if (unit == 'G' || unit == 'g')
        multiplier = 1000000000;
    else
        throw ParsingException(token, "invalid body size format");
    size = size.substr(0, size.length() - 1);
    for (size_t i = 0; i < size.length(); i++)
    {
        if (size[i] < '0' || size[i] > '9')
            throw ParsingException(token, "invalid body size format");
    }
    return strtoul(size.c_str(), &endptr, 10) * multiplier;
}


bool Parser::isAtEnd()
{
    return _current >= _tokens.size();
}