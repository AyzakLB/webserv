#pragma once
#include "SocketHeader.hpp"
#include "../CONFIGURATION_PARSER/Parser.hpp"
#include "Client.hpp"
#include "../EXCEPTIONS/NetworkException.hpp"

class Webserv {
    const std::vector<Server> &_servers;
    std::map<SOCKET, Client*> _socketToClient; // map socket to client
    std::map<SOCKET,  size_t> _socketsToServer; // map socket to server
    epoll_event _events[MAX_EVENTS];
    int _epollFd;

    public:
        Webserv(const std::vector<Server> &servers);
        ~Webserv();
        void start();
        void getListeningSockets();
        SOCKET createSocket(const Connection &connection);
        void eventLoop();
        void deleteClient(SOCKET socket);
        void handleNewConnection(SOCKET socket);
        void handleReadEvent(Client *client);
        void handleWriteEvent(Client *client);
        void sendResponse(Client &client);
        bool parseRequest(Client &client);

};