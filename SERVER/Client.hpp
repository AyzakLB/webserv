#pragma once

#include "SocketHeader.hpp"
// #include "../CONFIGURATION_PARSER/Server.hpp"
class Server;
class Location;
class Client {
    SOCKET    _socket;
    const Server   &_server;
    std::string _request;
    std::string _response;
    size_t   _bytesSent;
    public:
        Client(SOCKET socket, const Server &server);

        // GETTERS
        const SOCKET &getSocket() const;
        const Server &getServer() const;
        std::string &getRequest();
        const size_t &getBytesSent() const;
        const std::string &getResponse() const;
        const Location &getConfig(std::string path) const; // to be implemented later
        // SETTERS
        void setRequest(const std::string &request);
        void setBytesSent(size_t sent);
        void setResponse(const std::string &response);
};