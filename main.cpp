#include <fstream>
#include <sstream>
#include <iostream>
#include "CONFIGURATION_PARSER/Parser.hpp"
#include "CONFIGURATION_PARSER/Scanner.hpp"


// void printServers(const std::vector<Server>& servers) {
//     std::cout << "--- Configuration ---" << std::endl;
//     for (size_t i = 0; i < servers.size(); ++i) {
//         const Server& server = servers[i];
//         std::cout << "Server " << i + 1 << ":" << std::endl;

//         // Print Listen connections
//         const std::vector<Connection>& connections = server.getListen();
//         for (size_t j = 0; j < connections.size(); ++j) {
//             std::cout << "  Listen: " << connections[j].getAddress() << ":" << connections[j].getPort() << std::endl;
//         }

//         // Print Server Names
//         const std::vector<std::string>& serverNames = server.getServerName();
//         if (!serverNames.empty()) {
//             std::cout << "  Server Names: ";
//             for (size_t j = 0; j < serverNames.size(); ++j) {
//                 std::cout << serverNames[j] << " ";
//             }
//             std::cout << std::endl;
//         }

//         // Print Error Pages
//         const std::map<int, std::string>& errorPages = server.getErrorPages();
//         for (std::map<int, std::string>::const_iterator it = errorPages.begin(); it != errorPages.end(); ++it) {
//             std::cout << "  Error Page: " << it->first << " -> " << it->second << std::endl;
//         }

//         // Print Client Max Body Size
//         std::cout << "  Client Max Body Size: " << server.getClientMaxBodySize() << std::endl;

//         // Print Locations
//         const std::map<std::string, Location>& locations = server.getLocations();
//         for (std::map<std::string, Location>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
//             const std::string& path = it->first;
//             const Location& loc = it->second;
//             std::cout << "  Location " << path << ":" << std::endl;
//             std::cout << "    Root: " << loc.getRoot() << std::endl;
//             std::cout << "    Autoindex: " << (loc.getAutoIndex() ? "on" : "off") << std::endl;

//             // Print Allowed Methods
//             const std::vector<std::string>& methods = loc.getAllowMethods();
//             std::cout << "    Allowed Methods: ";
//             for (size_t k = 0; k < methods.size(); ++k) {
//                 std::cout << methods[k] << " ";
//             }
//             std::cout << std::endl;

//             // Print Index files
//             const std::vector<std::string>& indexFiles = loc.getIndex();
//             if (!indexFiles.empty()) {
//                 std::cout << "    Index: ";
//                 for (size_t k = 0; k < indexFiles.size(); ++k) {
//                     std::cout << indexFiles[k] << " ";
//                 }
//                 std::cout << std::endl;
//             }

//             // Print Return
//             const std::pair<int, std::string>& returnPair = loc.getReturn();
//             if (returnPair.first != 0) {
//                 std::cout << "    Return: " << returnPair.first << " " << returnPair.second << std::endl;
//             }

//             // Print Upload Path
//             if (!loc.getUploadPath().empty()) {
//                 std::cout << "    Upload Path: " << loc.getUploadPath() << std::endl;
//             }

//             // Print CGI
//             const std::map<std::string, std::string>& cgi = loc.getCGI();
//             for (std::map<std::string, std::string>::const_iterator cgi_it = cgi.begin(); cgi_it != cgi.end(); ++cgi_it) {
//                 std::cout << "    CGI: " << cgi_it->first << " -> " << cgi_it->second << std::endl;
//             }
//         }
//         std::cout << std::endl;
//     }
//     std::cout << "---------------------" << std::endl;
// }

int main(int argc, char* argv[]) {
    
    if (argc != 2)
    {
        std::cout << "config files" << std::endl;
        return 1;
    }
    // Open the file
    std::ifstream file(argv[1]);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << argv[1] << "'" << std::endl;
        return 1;
    }
    // Use a stringstream to read the file's content
    std::stringstream buffer;
    buffer << file.rdbuf();

    // Get the content from the stringstream
    std::string source = buffer.str();
    try {
        Parser parser(source);
        std::vector<Server> servers  = parser.parse();
        // printServers(servers);

    } catch (const ParsingException& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    file.close();

    return 0;
}

