#pragma once

#include "Scanner.hpp"
#include <set>
#include <map>
#include <vector>
#include <string>

class Location {
    std::string                         _path; 
    std::map<std::string, std::string>  _error_page; // subject to change, https://nginx.org/en/docs/http/ngx_http_core_module.html#error_page
    size_t                              _client_max_body_size; // https://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size
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
    void setPath(const std::string &path);
    void addErrorPage(const std::string &error_code, const std::string &path);
    void setClientMaxBodySize(size_t size);
    void setMethod(TokenType method, bool state);
    void setReturn(std::string code, std::string url);
    void setRoot(const std::string &path);
    void addIndex(const std::string &path);
    void setAutoIndex(bool state);
    void setUploadPath(const std::string &path);
    void addCGI(std::string extension, std::string path);

    // GETTERS 
    const std::string &getPath() const;
    const std::map<std::string, std::string> &getErrorPages() const;
    const size_t &getClientMaxBodySize() const;
    bool getMethod(TokenType method) const;
    const std::vector<std::string> &getReturn() const;
    const std::string& getRoot() const;
    const std::vector<std::string> &getIndex() const;
    const bool &getAutoIndex() const;
    const std::string &getUploadPath() const;
    const std::map<std::string, std::string> &getCGI() const;
};