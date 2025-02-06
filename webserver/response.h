#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <unordered_map>

class response
{
	private:
	std::string version_;
	std::string code_;
	std::string message_;
	std::unordered_map<std::string, std::string> headers_;
    std::string data_;

	public:
	response();
    response(std::string version, std::string code, std::string message);
    void set_version(std::string version);
    void set_code(std::string code);
    void set_message(std::string message);
    void add_header(std::string name, std::string value);
    void set_data(std::string data);
    std::string get_header(std::string name);
    std::string get_data();
    std::string to_string();
};

#endif