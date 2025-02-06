#include "request.h"


		request::request()
		{
			method_ = "GET";
			path_ = "/";
			proto_ = "HTTP/1.1";
		}
		request::request(std::string method, std::string path)
		{
			method_ = method;
			path_ = path;
			proto_ = "HTTP/1.1";
		}
		void request::set_method(std::string method)
		{
			method_ = method;
		}
		void request::set_path(std::string path)
		{
			path_ = path;
		}
		void request::set_proto(std::string proto)
		{
			proto_ = proto;
		}
		void request::add_header(std::string name, std::string value)
		{
			headers_[name] = value;
		}
		//void request::set_data(std::vector<std::byte> file){}
		std::string request::to_string(void)
		{
			std::string req =method_ + " " + path_ + " " + proto_ + "\r\n";
			
			for(const auto& h: headers_)
			{
				req += h.first + ": " + h.second + "\r\n";
			}
			req += "\r\n";
			return req;
		}