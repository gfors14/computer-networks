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
		Socket(int port);
		void connect();
		void bind();
		void listen();
		Socket accept();
		static Socket fromDescriptor(int socketDescriptor);
		int getSocketDescriptor();
		void send(const std::string &message);
		void setSocket(int socket);
		std::string recv();
		void close();
		~Socket();
		bool isValid();
		

};

#endif
