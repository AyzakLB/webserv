#include <fstream>
#include <sstream>
#include <iostream>
#include "CONFIGURATION_PARSER/Parser.hpp"
#include "CONFIGURATION_PARSER/Scanner.hpp"
#include "SERVER/Webserv.hpp"

#define DEFAULT_CONFIG_PATH "./TESTS/default_config_file.conf"

std::string readConfigFile(const std::string& filename)
{
    
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    std::string filename;
    if (argc == 1)
        filename = DEFAULT_CONFIG_PATH;
    else if (argc == 2)
        filename = argv[1];
    else
    {
        std::cerr << "either provide a single configuration file, or run the program without any argument!" << std::endl;
        return 1;
    }

    // Get the content from the stringstream
    std::string source = readConfigFile(filename);
    try {
        Parser parser(source);
        std::vector<Server> servers  = parser.parse();
        // printServers(servers); 
        Webserv webserv(servers);
        webserv.start();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    // file.close();

    return 0;
}