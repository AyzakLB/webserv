#include "Client.hpp"


Client::Client(SOCKET socket, const Server &server) : _socket(socket), _server(server),  _bytesSent(0) {};


// GETTERS
const SOCKET &Client::getSocket() const
{
    return _socket;
}

const Server &Client::getServer() const
{
    return _server;
}

std::string &Client::getRequest() 
{
    return _request;
}



const size_t &Client::getBytesSent() 
{
    return _bytesSent;
}

const std::string &Client::getResponse() const
{
    return _response;
}

// SETTERS

void Client::setRequest(const std::string &request)
{
    _request = request;
}



void Client::setBytesSent(size_t sent)
{
    _bytesSent = sent;
}