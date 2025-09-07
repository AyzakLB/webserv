#include "Server.hpp"
#include <sstream>
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
    _autoindex = false;
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

void Location::addCGI(std::string)
{
    // to be determined;
}

void Location::setRoot(const std::string &path)
{
    _root  = path;
}
void Location::setUploadPath(const std::string &path)
{
    _upload_path  = path;
}

void Location::setMethod(Method method, bool state)
{
    _allow_methods[method] = state;
}

void Location::setAutoIndex(bool state)
{
    _autoindex = state;
}


////////////////////////////////////////////////////////////////////////// SERVER //////////////////////////////////////////////////////////////////////

Server::Server()
{
    _client_max_body_size = 1000000;
}


/////////////////////////////////////////////////////////////////////////// SETTERS ////////////////////////////////////////////////////////////////////
void Server::addLocation(const std::string &path, Location location)
{
    try 
    {
        if (_locations.find(path) == _locations.end())
            _locations[path] = location;
        else
            throw std::runtime_error("location block already exists");
    } catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}


std::vector<std::string> split(std::string string, char delimiter)
{
    std::vector<std::string> result;
    std::string temp;
    std::stringstream ss(string);
    while (std::getline(ss, temp, delimiter))
    {
        result.push_back(temp);
    }
    return result;
}
void Server::addConnection(const std::string &addport)
{
    try
    {
        std::vector<std::string> parts = split(addport, ':');
        if (parts.size() != 2)
            throw std::runtime_error("invalid address:port pair!");
        std::string hostname = parts[0];
        std::string port = parts[1];
        _listen.push_back(Connection(hostname, port));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}


void Server::addErrorPage(const std::string &error_code, const std::string &path)
{
    _error_page[error_code] = path;
}

void Server::setClientMaxBodySize(size_t size)
{
    _client_max_body_size = size;
}


/////////////////////////////////////////////////////////////////////////// GETTERS ////////////////////////////////////////////////////////////////////

const Location &Server::getLocation(const std::string &path)
{
    try
    {
        std::string current_path = path;
        while (current_path.empty() == 0)
        {
            if (_locations.find(current_path) != _locations.end())
                return _locations.find(current_path)->second;
            current_path = current_path.substr(0, current_path.find_last_of('/'));
        }
        throw std::runtime_error("no matching block for the requested path");
    } catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        // This is problematic. What should be returned on error?
        // For now, I'll re-throw, but this needs to be handled by the caller.
        throw;
    }
}


