#include "Location.hpp"
#define CLIENT_MAX_BODY_SIZE 1000000
Location::Location()
{
    _index.push_back("index.html");
    _root = "html";
    _allow_methods[0] = true;
    _allow_methods[1] = true;
    _allow_methods[2] = true;
    _client_max_body_size = CLIENT_MAX_BODY_SIZE;
    _autoindex = false;
}


/////////////////////////////////////////////////// SETTERS ////////////////////////////////////////

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

/////////////////////////////////////////////////// GETTERS ////////////////////////////////////////

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