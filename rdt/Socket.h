#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <netinet/in.h>

class Socket 
{
	private:
		int sockfd;
		int socket_;
		int port_;
		std::string host_;
		struct sockaddr_in address_;
		bool isUDP_;


	public:
		Socket();
		Socket(std::string host, int port, bool isUDP);
		Socket(int port, bool isUDP);
		void connect();
		void bind();
		void listen();
		Socket accept();
		static Socket fromDescriptor(int socketDescriptor);
		void send(const std::string &message);
		void sendTo(const std::string& message, const std::string& host, int port);
		std::string recv();
		std::string recvFrom(sockaddr_in& source, int& sourcePort);
		void close();
		~Socket();
		bool isValid();
};

#endif
