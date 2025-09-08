#include "./Token.hpp"

std::map<std::string, int> createStringToTokenMap() {
    std::map<std::string, int> m;
    m["SERVER"] = SERVER;
    m["LOCATION"] = LOCATION;
    m["LISTEN"] = LISTEN;
    m["ERROR_PAGE"] = ERROR_PAGE;
    m["CLIENT_MAX_BODY_SIZE"] = CLIENT_MAX_BODY_SIZE;
    m["ROOT"] = ROOT;
    m["INDEX"] = INDEX;
    m["AUTOINDEX"] = AUTOINDEX;
    m["ALLOW_METHODS"] = ALLOW_METHODS;
    m["RETURN"] = RETURN;
    m["UPLOAD_PATH"] = UPLOAD_PATH;
    m["CGI"] = CGI;
    m["BRACKET_LEFT"] = BRACKET_LEFT;
    m["BRACKET_RIGHT"] = BRACKET_RIGHT;
    m["SEMICOLON"] = SEMICOLON;
    m["VALUE_NUMBER"] = VALUE_NUMBER;
    m["VALUE_STRING"] = VALUE_STRING;
    m["VALUE_PATH"] = VALUE_PATH;
    m["EOF_TOKEN"] = EOF_TOKEN;
    return m;
}

std::map<int, std::string> createTokenToStringMap() {
    std::map<int, std::string> m;
    m[SERVER] = "SERVER";
    m[LOCATION] = "LOCATION";
    m[LISTEN] = "LISTEN";
    m[ERROR_PAGE] = "ERROR_PAGE";
    m[CLIENT_MAX_BODY_SIZE] = "CLIENT_MAX_BODY_SIZE";
    m[ROOT] = "ROOT";
    m[INDEX] = "INDEX";
    m[AUTOINDEX] = "AUTOINDEX";
    m[ALLOW_METHODS] = "ALLOW_METHODS";
    m[RETURN] = "RETURN";
    m[UPLOAD_PATH] = "UPLOAD_PATH";
    m[CGI] = "CGI";
    m[BRACKET_LEFT] = "BRACKET_LEFT";
    m[BRACKET_RIGHT] = "BRACKET_RIGHT";
    m[SEMICOLON] = "SEMICOLON";
    m[VALUE_NUMBER] = "VALUE_NUMBER";
    m[VALUE_STRING] = "VALUE_STRING";
    m[VALUE_PATH] = "VALUE_PATH";
    m[EOF_TOKEN] = "EOF_TOKEN";
    return m;
}

std::map<int, std::string> tokenToString = createTokenToStringMap();
std::map<std::string, int> stringToToken = createStringToTokenMap();

void Token::printToken()
{
    std:: cout << "--> " << _line << " " << tokenToString.find(_type)->second << " [" << _lexeme << "]" << std::endl;
}
