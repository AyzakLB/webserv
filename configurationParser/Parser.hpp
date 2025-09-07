#include "Scanner.hpp"
#include <set>

enum Method {
    GET,
    POST,
    DELETE,
};

class Connection {
    std::string _address;
    std::string  _port;
    public:
        Connection(const std::string &address, const std::string &port) : _address(address), _port(port){};
        const std::string &getAddress() const { return _address;}
        const std::string &getPort() const { return _port;} 
};

class Server {
    std::map<std::string, Server>     _locations; 
    std::vector<Connection>             _listen;
    std::map<std::string, std::string>  _error_page; // subject to change
    std::vector<std::string>            _index;
    std::vector<std::string>            _CGI; // to be determined
    std::string                         _root;
    std::string                         _upload_path;
    size_t                              _client_max_body_size;
    bool                                _allow_methods[3];
    bool                                _autoindex;
    public:
        Server();

        // SETTERS
        void addLocation(const std::string &path, Server location);
        void addConnection(const std::string &addport);
        void addErrorPage(const std::string &error_code, const std::string &path);
        void addIndex(const std::string &path);
        void addCGI(std::string);
        void setRoot(const std::string &path);
        void setUploadPath(const std::string &path);
        void setClientMaxBodySize(size_t size);
        void setMethod(Method method, bool state);
        void setAutoIndex(bool state);

        // GETTES
        const Server &getLocation(const std::string &path);
        
};