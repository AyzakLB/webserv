#include <vector>
#include <string>
#include <iostream>
#include <sstream>

std::vector<std::string> split(std::string string, char delimiter)
{
    std::vector<std::string> result;
    std::string temp;
    std::stringstream ss(string);
    while (std::getline(ss, temp, delimiter))
    {
        result.push_back(temp);
    }
    return result;
}