#pragma once

#include "Scanner.hpp"
#include <set>
#include <map>
#include <vector>
#include <string>
#include "Location.hpp"
class Server;
class Connection {
    std::string _address;
    std::string  _port;
    // const Server &server;
    public:
        Connection(const std::string &address, const std::string &port) : _address(address), _port(port){};
        const std::string &getAddress() const { return _address;}
        const std::string &getPort() const { return _port;} 
};



class Server {
    std::vector<Connection>             _listen; // https://nginx.org/en/docs/http/ngx_http_core_module.html#listen
    std::map<std::string, Location>     _locations;
    Location                           _default_location; // gotta
    
    public:
        bool                               _foundListen;
        Server();

        // SETTERS
        void addLocation(const std::string &path, Location location);
        void addConnection(const std::string &address, const std::string &port);
        void pushBackLocation(const std::string &path, Location location);
        
        // GETTERS
        const std::vector<Connection> &getConnections() const;
        const std::map<std::string, Location> &getLocations() const;
        const Location &getLocation(const std::string &path) const;
        Location &getDefaultLocation();
};