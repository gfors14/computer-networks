#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <unordered_map>

class request {
private:
    std::string method_;
    std::string path_;
    std::string proto_;
    std::unordered_map<std::string, std::string> headers_;

public:
    request();
    request(std::string method, std::string path);
    void set_method(std::string method);
    void set_path(std::string path);
    void set_proto(std::string proto);
    void add_header(std::string name, std::string value);
    std::string to_string();
};

#endif
