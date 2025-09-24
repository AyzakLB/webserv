
#include <fstream>
#include <sstream>
#include <iostream>
#include "../CONFIGURATION_PARSER/Parser.hpp"
#include "../CONFIGURATION_PARSER/Scanner.hpp"
#include "../SERVER/Webserv.hpp"
void printLocation(const Location& location, bool isDefault)
{
    if (isDefault == false)
        std::cout << "location " << location.getPath() << " {" << std::endl;
    std::cout << "  root: " << location.getRoot() << std::endl;
    std::cout << "  index: ";
    const std::vector<std::string>& indexes = location.getIndex();
    for (size_t i = 0; i < indexes.size(); i++) {
        std::cout << indexes[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "  autoindex: " << (location.getAutoIndex() ? "on" : "off") << std::endl;
    std::cout << "  upload_path: " << location.getUploadPath() << std::endl;
    std::cout << "  client_max_body_size: " << location.getClientMaxBodySize() << std::endl;
    std::cout << "  error_page: ";
    const std::map<std::string, std::string>& error_pages = location.getErrorPages();
    for (std::map<std::string, std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); ++it) {
        std::cout << it->first << " -> " << it->second << " ";
    }
    std::cout << std::endl;
    std::cout << "  allow_methods: ";
    if (location.getMethod(GET)) std::cout << "GET ";
    if (location.getMethod(POST)) std::cout << "POST ";
    if (location.getMethod(DELETE)) std::cout << "DELETE ";
    std::cout << std::endl;
    std::cout << "  return: ";
    const std::vector<std::string>& returns = location.getReturn();
    for (size_t i = 0; i < returns.size(); i++) {
        std::cout << returns[i] << " ";
    }
    if (isDefault == false)
        std::cout <<  "}" << std::endl;
}

void printServer(Server& server)
{
    std::cout << "server {" << std::endl;
    const std::vector<Connection>& connections = server.getConnections();
    for (size_t i = 0; i < connections.size(); i++) {
        std::cout << "  listen: ";
        std::cout << connections[i].getAddress() << ":" << connections[i].getPort() << " ";
        std::cout << std::endl;
    }
    printLocation(server.getDefaultLocation(), true);
    const std::map<std::string, Location>& locations = server.getLocations();
    for (std::map<std::string, Location>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
        printLocation(it->second, false);
    }
    std::cout << "}" << std::endl;
}
void printServers(std::vector<Server>& servers) 
{
    for (size_t i = 0; i < servers.size(); i++) {
        std::cout << "/////////////////////////////////////////// Server " << i + 1 << " :" << "///////////////////////////////////////////////" << std::endl;
        printServer(servers[i]);
    }
}