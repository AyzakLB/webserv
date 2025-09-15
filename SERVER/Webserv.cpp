#include "Webserv.hpp"

Webserv::Webserv(const std::vector<Server> &servers) : _servers(servers) 
{
    _epollFd = epoll_create(1);
    if (_epollFd == -1)
        throw NetworkException(strerror(errno));
}

Webserv::~Webserv()
{
    std::map<SOCKET, Client*>::iterator it;
    for (it = _socketToClient.begin(); it != _socketToClient.end(); ++it)
        deleteClient(it->first);
    close(_epollFd);
}

SOCKET Webserv::createSocket(const Connection &connection)
{
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    const char *address = connection.getAddress().c_str();
    const char *port = connection.getPort().c_str();
    addrinfo *result;
    int error;
    error = getaddrinfo(address, port, &hints, &result);
    if (error != 0)
        throw NetworkException(gai_strerror(error));
    SOCKET listenSocket = -1;
    addrinfo *temp = result;
    while (temp)
    {
        listenSocket = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);
        int opt = 1;
        if (ISVALIDSOCKET(listenSocket) == false || setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
        {
            temp = temp->ai_next;
            close(listenSocket);
            listenSocket = -1;
            continue;
        }
        if (bind(listenSocket, temp->ai_addr, temp->ai_addrlen) == 0)
            break;
        close(listenSocket);
        listenSocket = -1;
        temp = temp->ai_next;
    }
    freeaddrinfo(result);
    if (listenSocket == -1)
        throw NetworkException(strerror(errno));
    if (fcntl(listenSocket, F_SETFL, O_NONBLOCK) == -1)
        throw NetworkException(strerror(errno));
    if (listen(listenSocket, SOMAXCONN) == -1)
        throw NetworkException(strerror(errno));
    return listenSocket;
}

void Webserv::getListeningSockets()
{
    for (size_t i = 0; i < _servers.size(); ++i)
    {
        const std::vector<Connection> &connections = _servers[i].getConnections();
        for (size_t j = 0; j < connections.size(); ++j)
        {
            SOCKET sock = createSocket(connections[j]);
            epoll_event event;
            event.data.fd = sock;
            event.events = EPOLLIN;
            if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, sock, &event) == -1)
                throw NetworkException(strerror(errno));
            _socketsToServer[sock] = i;
        }
    }
}

void Webserv::deleteClient(SOCKET socket)
{
    std::map<SOCKET, Client*>::iterator it = _socketToClient.find(socket);
    if (it != _socketToClient.end())
    {
        epoll_ctl(_epollFd, EPOLL_CTL_DEL, socket, NULL);
        close(it->first);
        delete it->second;
        _socketToClient.erase(it);
    }
}

void Webserv::handleNewConnection(SOCKET socket)
{
    SOCKET clientSocket;
    clientSocket = accept(socket, NULL, NULL);
    if (clientSocket == -1)
        return ;
    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1)
    {
        close(clientSocket);
        return;
    }
    Client *client = new Client(clientSocket, _servers[_socketsToServer[socket]]);
    if (!client)
    {
        close(clientSocket);
        return;
    }
    _socketToClient[clientSocket] = client;
    epoll_event event;
    event.data.fd = clientSocket;
    event.events = EPOLLIN;
    if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientSocket, &event) == -1)
    {
        deleteClient(clientSocket);
        return;
    }
}


bool Webserv::parseRequest(Client &client)
{
    // enjoy the spaghetti :3
    if (client.getRequest().find("\r\n\r\n") == std::string::npos)
        return false;
    return true;
}

void Webserv::sendResponse(Client &client)
{
    const std::string &response = client.getResponse();
    size_t responseSize = response.size();
    size_t bytesSent = client.getBytesSent();
    ssize_t result = send(client.getSocket(), response.c_str() + bytesSent, responseSize - bytesSent, 0);
    if (result > 0)
    {
        client.setBytesSent(bytesSent + result);
        if (client.getBytesSent() == responseSize)
        {
            deleteClient(client.getSocket());
            return ;
        }
    } else if (result == -1 && errno != EAGAIN && errno != EWOULDBLOCK )
    {
        std::cerr << "send error: " << strerror(errno) << std::endl;
        deleteClient(client.getSocket());
        return ;
    }
    epoll_event event;
    event.data.fd = client.getSocket();
    event.events = EPOLLIN | EPOLLOUT;
    if (epoll_ctl(_epollFd, EPOLL_CTL_MOD, event.data.fd, &event) == -1)
    {
        std::cerr << "epoll_ctl failed: " << strerror(errno) << std::endl;
        deleteClient(client.getSocket());
        return ;
    }
}

void Webserv::handleReadEvent(SOCKET socket)
{
    std::map<SOCKET, Client*>::iterator it = _socketToClient.find(socket);
    if (it == _socketToClient.end())
    {
        std::cerr << "Client not found for socket: " << socket << std::endl;
        close(socket);
        return;
    }
    Client *client = _socketToClient[socket];
    char buffer[MAX_READ_SIZE];
    int bytesRead = recv(socket, buffer, sizeof(buffer) - 1, 0);
    buffer[bytesRead] = '\0';
    if (bytesRead > 0)
    {
        client->getRequest().append(buffer, bytesRead);
        bool requestReady = parseRequest(*client);
        if (requestReady)
            sendResponse(*client);
    } else if (bytesRead == 0)
    {
        std::cout << "bye bye client!" << std::endl;
        deleteClient(socket);
    }
    else if (bytesRead == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
    {
        std::cerr << "Error reading from socket: " << strerror(errno) << std::endl;
        deleteClient(socket);
    }
}

void Webserv::handleWriteEvent(SOCKET socket)
{
    std::map<SOCKET, Client*>::iterator it = _socketToClient.find(socket);
    if (it == _socketToClient.end())
    {
        std::cerr << "Client not found for socket: " << socket << std::endl;
        close(socket);
        return;
    }
    Client *client = _socketToClient[socket];
    sendResponse(*client);
}
void Webserv::eventLoop()
{
    while (true)
    {
        int nfds = epoll_wait(_epollFd, _events, MAX_EVENTS, -1);
        if (nfds == -1)
            throw NetworkException(strerror(errno));
        for (int n = 0; n < nfds; ++n)
        {
            SOCKET eventSocket = _events[n].data.fd;
            if (_socketsToServer.find(eventSocket) != _socketsToServer.end())
                handleNewConnection(eventSocket);
            else
            {
                if (_events[n].events & EPOLLIN)
                    handleReadEvent(eventSocket);
                else if (_events[n].events & EPOLLOUT)
                    handleWriteEvent(eventSocket);
            }
        }
    }
}

void Webserv::start()
{
    getListeningSockets();
    eventLoop();
}
