#include "Scanner.hpp"
#include <set>
#include <map>
#include <vector>
#include <string>

// enum Method {
//     GET,
//     POST,
//     DELETE,
// };

class Connection {
    std::string _address;
    std::string  _port;
    public:
        Connection(const std::string &address, const std::string &port) : _address(address), _port(port){};
        const std::string &getAddress() const { return _address;}
        const std::string &getPort() const { return _port;} 
};

class Location {
    bool                                _allow_methods[3]; 
    std::vector<std::string>            _return; // https://nginx.org/en/docs/http/ngx_http_rewrite_module.html#return
    std::string                         _root; // https://nginx.org/en/docs/http/ngx_http_core_module.html#root
    std::vector<std::string>            _index; // https://nginx.org/en/docs/http/ngx_http_index_module.html#index
    bool                                _autoindex; // https://nginx.org/en/docs/http/ngx_http_autoindex_module.html#autoindex
    std::string                         _upload_path; 
    std::map<std::string, std::string>            _CGI; // to be determined

public:
    Location();

    // SETTERS
    void setMethod(TokenType method, bool state);
    void setReturn(std::string code, std::string url);
    void setRoot(const std::string &path);
    void addIndex(const std::string &path);
    void setAutoIndex(bool state);
    void setUploadPath(const std::string &path);
    void addCGI(std::string extension, std::string path);

    // GETTERS 
    const std::string& getRoot() const { return _root; }
};

class Server {
    std::vector<Connection>             _listen; // https://nginx.org/en/docs/http/ngx_http_core_module.html#listen
    std::map<std::string, std::string>  _error_page; // subject to change, https://nginx.org/en/docs/http/ngx_http_core_module.html#error_page
    size_t                              _client_max_body_size; // https://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size
    std::map<std::string, Location>     _locations; 

    public:
        Server();

        // SETTERS
        void addLocation(const std::string &path, Location location);
        void addConnection(const std::string &address, const std::string &port);
        void addErrorPage(const std::string &error_code, const std::string &path);
        void setClientMaxBodySize(size_t size);
        void pushBackLocation(const std::string &path, Location location);
        // GETTERS
        const Location &getLocation(const std::string &path);
        
};