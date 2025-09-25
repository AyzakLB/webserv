#include "Server.hpp"


Server::Server(){}

void Server::addLocation(const std::string &path, Location location)
{
    if (_locations.find(path) == _locations.end())
        _locations[path] = location;
    else
        throw std::runtime_error("a location block with the path '" + path + "' already exists");
}

void Server::addConnection(const std::string &address, const std::string &port)
{
   _listen.push_back(Connection(address, port));
}

/////////////////////////////////////////////////////////////////////////// SETTERS ////////////////////////////////////////////////////////////////////

void Server::pushBackLocation(const std::string &path, Location location)
{
    _locations[path] = location;
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