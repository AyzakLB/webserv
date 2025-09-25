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
    std::cout << "Client disconnected on socket: " << socket << std::endl;
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
    std::cout << "New client connected on socket: " << clientSocket << std::endl;
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

bool Webserv::parseRequest(Client &client) // parse the request and return true if the request is complete, you can implement it however you want, just make sure it returns the bool :3, its current body gha for testing purposes.
{
    // enjoy the spaghetti :3
    if (client.getRequest().empty())
        return false;
    else
        return true;
    if (client.getRequest().find("\r\n\r\n") == std::string::npos)
        return false;
    return true;
}
void generateResponse(Client &client) // Ayoub's function to generate a response based on the request, the request data structure might be part of the client? up to you to decide
{
    (void) client;
    return ;
} 
void Webserv::sendResponse(Client &client)
{
    const std::string &response = client.getRequest(); // should be getResponse() but let's use the request for now
    size_t responseSize = response.size();
    size_t bytesSent = client.getBytesSent();
    ssize_t result = send(client.getSocket(), response.c_str() + bytesSent, responseSize - bytesSent, 0);
    if (result > 0)
    {
        std::cout << "Sent " << result << " bytes to socket: " << client.getSocket() << std::endl;
        client.setBytesSent(bytesSent + result);
        if (client.getBytesSent() == responseSize)
        {
            deleteClient(client.getSocket());
            return ;
        }
    } else if (result == -1 && (errno != EAGAIN || errno != EWOULDBLOCK) )
    {
        std::cout << "Failed to send response to socket: " << client.getSocket() << std::endl;
        std::cerr << "send error: " << strerror(errno) << std::endl;
        deleteClient(client.getSocket());
        return ;
    }
    std::cout << "Partial response sent to socket: " << client.getSocket() << ", waiting for next write event." << std::endl;
    epoll_event event;
    event.data.fd = client.getSocket();
    event.events = EPOLLOUT;
    if (epoll_ctl(_epollFd, EPOLL_CTL_MOD, event.data.fd, &event) == -1)
    {
        std::cerr << "epoll_ctl failed: " << strerror(errno) << std::endl;
        deleteClient(client.getSocket());
        return ;
    }
}

void Webserv::handleReadEvent(Client *client)
{
    SOCKET socket = client->getSocket();
    char buffer[MAX_READ_SIZE];
    int bytesRead = recv(socket, buffer, sizeof(buffer) - 1, 0);
    buffer[bytesRead] = '\0';
    if (bytesRead > 0)
    {
        client->getRequest().append(buffer, bytesRead);
        std::cout << "Received " << bytesRead << " bytes: " << client->getRequest() << std::endl;
        bool requestReady = parseRequest(*client);
        generateResponse(*client); // Ayoub's function to generate a response based on the request, the request data structure might be part of the client? up to you to decide
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
                std::map<SOCKET, Client*>::iterator it = _socketToClient.find(eventSocket);
                if (it == _socketToClient.end())
                    continue; // normally this shouldn't happen
                Client *client = it->second;
                if (_events[n].events & EPOLLIN)
                    handleReadEvent(client);
                else if (_events[n].events & EPOLLOUT)
                    sendResponse(*client);
            }
        }
    }
}

void Webserv::start()
{
    getListeningSockets();
    eventLoop();
}
