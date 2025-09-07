#include "configurationParser/Scanner.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

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
    Scanner lexicalAnalygator(source);
    lexicalAnalygator.scanTokens();
    lexicalAnalygator.printTokens();
    file.close();

    return 0;
}

