#include "Server.hpp"
#include <iostream>
#include <stdexcept>
#include <stdlib.h>


//////////////////////////////////////////////////////////////////////// LOCATION //////////////////////////////////////////////////////////////////////

Location::Location()
{
    _index.push_back("index.html");
    _root = "html";
    _allow_methods[0] = true;
    _allow_methods[1] = true;
    _allow_methods[2] = true;
    _client_max_body_size = 1000000;
    _autoindex = false;
}

//////////////////////////////////////////////////////////////////////// SETTERS //////////////////////////////////////////////////////////////////////


void Location::setPath(const std::string &path)
{
     _path = path;
}
void Location::addErrorPage(const std::string &error_code, const std::string &path)
{
    _error_page[error_code] = path;
}

void Location::setClientMaxBodySize(size_t size)
{
    _client_max_body_size = size;
}

void Server::pushBackLocation(const std::string &path, Location location)
{
    _locations[path] = location;
}

void Location::addIndex(const std::string &path)
{
    _index.push_back(path);
}

void Location::setReturn(std::string code, std::string url)
{
    _return.push_back(code);
    _return.push_back(url);
}

void Location::addCGI(std::string extension, std::string path)
{
    _CGI[extension] = path;
}

void Location::setRoot(const std::string &path)
{
    _root  = path;
}
void Location::setUploadPath(const std::string &path)
{
    _upload_path  = path;
}

void Location::setMethod(TokenType method, bool state)
{
        _allow_methods[method - GET] = state;
}

void Location::setAutoIndex(bool state)
{
    _autoindex = state;
}

//////////////////////////////////////////////////////////////////////// GETTERS ////////////////////////////////////////////////////////////////////


const std::string &Location::getPath() const
{
    return _path;
}

const std::map<std::string, std::string> &Location::getErrorPages() const
{
    return _error_page;
}

const size_t &Location::getClientMaxBodySize() const
{
    return _client_max_body_size;
}

 bool Location::getMethod(TokenType method) const
{
    return _allow_methods[method - GET];
}

const std::vector<std::string> &Location::getReturn() const
{
    return _return;
}

const std::string &Location::getRoot() const
{
    return _root;
}

const std::vector<std::string> &Location::getIndex() const
{
    return _index;
}

const bool &Location::getAutoIndex() const
{
    return _autoindex;
}

const std::string &Location::getUploadPath() const
{
    return _upload_path;
}

const std::map<std::string, std::string> &Location::getCGI() const
{
    return _CGI;
}

////////////////////////////////////////////////////////////////////////// SERVER //////////////////////////////////////////////////////////////////////

Server::Server(){}

/////////////////////////////////////////////////////////////////////////// SETTERS ////////////////////////////////////////////////////////////////////
void Server::addLocation(const std::string &path, Location location)
{
    if (_locations.find(path) == _locations.end())
        _locations[path] = location;
    else
        throw std::runtime_error("a location block with the path '" + path + "' already exists");
}

void Server::addConnection(const std::string &address, const std::string &port, const Server &server)
{
   _listen.push_back(Connection(address, port, server));
}





/////////////////////////////////////////////////////////////////////////// GETTERS ////////////////////////////////////////////////////////////////////

const std::vector<Connection> &Server::getConnections() const
{
    return _listen;
}

const std::map<std::string, Location> &Server::getLocations() const
{
    return _locations;
}

const Location &Server::getLocation(const std::string &path) const
{
        std::string current_path = path;
        while (current_path.empty() == 0)
        {
            if (_locations.find(current_path) != _locations.end())
                return _locations.find(current_path)->second;
            current_path = current_path.substr(0, current_path.find_last_of('/'));
        }
        return _default_location;
}

Location &Server::getDefaultLocation()
{
    return _default_location;
}
