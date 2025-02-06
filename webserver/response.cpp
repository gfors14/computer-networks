#include "response.h"

response::response() : version_("HTTP/1.1"), code_("200"), message_("OK") 
{
	
}

response::response(std::string version, std::string code, std::string message): version_(version), code_(code), message_(message) 
{
	
}

void response::set_version(std::string version)
{
	version_ = version;
}
void response::set_code(std::string code)
{
	code_ = code;
}
void response::set_message(std::string message)
{
	message_ = message;
}
void response::add_header(std::string name, std::string value)
{
	headers_[name] = value;
}
void response::set_data(std::string data)
{
	data_ = data;
}

std::string response::get_header(std::string name) 
{
    if (headers_.count(name))
        return headers_[name];
    return "";
}
std::string response::get_data()
{
	return data_;
}
std::string response::to_string()
{
	std::string res = version_ + " " + code_ + " " + message_ + "\r\n";
    
    for (const auto& h : headers_) 
	{
        res += h.first + ": " + h.second + "\r\n";
    }
    res += "\r\n";
    res += data_;
    return res;
}

    