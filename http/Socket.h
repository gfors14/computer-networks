#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <netinet/in.h>

class Socket 
{
	private:
		int socket_;
		int port_;
		std::string host_;
		struct sockaddr_in address_;

	public:
		Socket();
		Socket(std::string host, int port);
		void connect();
		void bind();
		void listen();
		int accept();
		void send(const std::string &message);
		void setSocket(int socket);
		std::string recv();
		void close();
		~Socket();
};

#endif
