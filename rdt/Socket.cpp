#include "Socket.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>


Socket::Socket() : socket_(-1), port_(0), host_(""), isUDP_(false) {}

Socket::Socket(std::string host, int port, bool isUDP) : host_(host), port_(port), isUDP_(isUDP) 
{
    sockfd = socket(AF_INET, isUDP ? SOCK_DGRAM : SOCK_STREAM, 0);
    if (sockfd == -1) 
	{
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
	{
        std::cerr << "Failed to set SO_REUSEADDR: " << strerror(errno) << std::endl;
        ::close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (isUDP) 
	{
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) 
		{
            std::cerr << "Failed to set SO_REUSEPORT: " << strerror(errno) << std::endl;
            ::close(sockfd);
            exit(EXIT_FAILURE);
        }
    }

    memset(&address_, 0, sizeof(address_));
    address_.sin_family = AF_INET;
    address_.sin_port = htons(port);
    address_.sin_addr.s_addr = inet_addr(host.c_str());

    if (::bind(sockfd, (struct sockaddr *)&address_, sizeof(address_)) == -1) {
        std::cerr << "Failed to bind UDP socket: " << strerror(errno) << std::endl;
        ::close(sockfd);
        exit(EXIT_FAILURE);
    }
}


Socket::Socket(int port, bool isUDP) : isUDP_(isUDP) 
{
    sockfd = socket(AF_INET, isUDP ? SOCK_DGRAM : SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

		int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
	{
		std::cerr << "Error setting SO_REUSEADDR: " << strerror(errno) << std::endl;
		close();
		exit(EXIT_FAILURE);
	}
	if (isUDP && setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) 
	{
		std::cerr << "Error setting SO_REUSEPORT: " << strerror(errno) << std::endl;
		close();
		exit(EXIT_FAILURE);
	}


    memset(&address_, 0, sizeof(address_));
    address_.sin_family = AF_INET;
    address_.sin_port = htons(port);
    address_.sin_addr.s_addr = INADDR_ANY;

    if (::bind(sockfd, (struct sockaddr *)&address_, sizeof(address_)) == -1) 
	{
        std::cerr << "Failed to bind socket: " << strerror(errno) << std::endl;
        ::close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (!isUDP && ::listen(sockfd, SOMAXCONN) == -1) 
	{
        std::cerr << "Failed to listen on TCP socket: " << strerror(errno) << std::endl;
        ::close(sockfd);
        exit(EXIT_FAILURE);
    }
}





void Socket::sendTo(const std::string &message, const std::string &host, int port) 
{
    if (sockfd < 0) {
        std::cerr << "Socket is not initialized. Cannot send data." << std::endl;
        return;
    }

    sockaddr_in destAddr;
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &destAddr.sin_addr);

    int sentBytes = sendto(sockfd, message.c_str(), message.size(), 0, (struct sockaddr *)&destAddr, sizeof(destAddr));
    if (sentBytes < 0) {
        std::cerr << "Failed to send data: " << strerror(errno) << std::endl;
    } else {
        std::cout << "Successfully sent " << sentBytes << " bytes to " << host << ":" << port << std::endl;
    }
}



std::string Socket::recvFrom(sockaddr_in& source, int& sourcePort) 
{
    char buffer[1024];
    socklen_t len = sizeof(source);  // Ensure this is set correctly before the call

    std::cout << "Waiting to receive data..." << std::endl;
    ssize_t recvd = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&source, &len);

    if (recvd == -1) 
	{
        std::cerr << "Receive error: " << strerror(errno) << std::endl;
        return "";
    } 
	else if (recvd == 0) 
	{
        std::cout << "No data received, possibly connection closed." << std::endl;
        return "";
    }

    sourcePort = ntohs(source.sin_port);  // This should now reflect the sender's port
    std::cout << "Received " << recvd << " bytes from port " << sourcePort << std::endl;
    return std::string(buffer, recvd);
}


void Socket::connect() 
{
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ == -1) 
	{
        std::cerr << "Error: Could not create socket\n";
        return;
    }

    address_.sin_family = AF_INET;
    address_.sin_port = htons(port_);

    // Use one of the resolved IP addresses Use nslookup (website) to find IPS you need
    const char* ipAddr = "159.242.4.72"; // IP address of httpbin.org

    int inetResult = inet_pton(AF_INET, ipAddr, &address_.sin_addr);
    std::cout << "inet_pton result: " << inetResult << std::endl;

    if (inetResult <= 0) {
        std::cerr << "Error: Invalid address or address family\n";
        return;
    }

    // Debugging output
    char addrStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address_.sin_addr, addrStr, INET_ADDRSTRLEN);
    std::cout << "Address: " << addrStr << std::endl;

    if (::connect(socket_, (struct sockaddr *)&address_, sizeof(address_)) == -1) {
        std::cerr << "Error: Connection failed - " << strerror(errno) << "\n";
        return;
    }

    std::cout << "Connected to " << host_ << ":" << port_ << std::endl;
}




void Socket::bind() {
	
    // Check if the socket is already created, if not, create based on the isUDP_ flag
    if (socket_ == -1) {
        socket_ = ::socket(AF_INET, isUDP_ ? SOCK_DGRAM : SOCK_STREAM, 0);
        if (socket_ == -1) {
            std::cerr << "Error: Could not create socket\n";
            return;
        }

        // Set socket options, applicable to both TCP and UDP
        int opt = 1;
        if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
            std::cerr << "Error: Setting SO_REUSEADDR socket option failed\n";
            return;
        }
    }

    // Set up the address structure
    address_.sin_family = AF_INET;
    address_.sin_port = htons(port_);
    address_.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (::bind(socket_, (struct sockaddr *)&address_, sizeof(address_)) == -1) {
        std::cerr << "Error: Bind failed - " << strerror(errno) << "\n";
        return;
    }

    std::cout << "Bound to port " << port_ << std::endl;

    // If TCP, the socket may need to be set to listen
    if (!isUDP_) 
	{
        if (::listen(socket_, SOMAXCONN) == -1) 
		{
            std::cerr << "Error: Could not set socket to listen\n";
            return;
        }
        std::cout << "Listening on port " << port_ << std::endl;
    }
}

void Socket::listen() 
{
    if (::listen(socket_, SOMAXCONN) == -1) 
	{
        std::cerr << "Error: Could not listen\n";
        return;
    }

    std::cout << "Listening on port " << port_ << std::endl;
}


Socket Socket::accept() 
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    std::cout << "Attempting to accept a connection..." << std::endl;
    int clientSockDescriptor = ::accept(socket_, (struct sockaddr *)&clientAddr, &clientAddrSize);
    if (clientSockDescriptor == -1) 
	{
        std::cerr << "Error accepting connection: " << strerror(errno) << std::endl;
        return Socket(); 
    }
    std::cout << "Accepted connection from " << inet_ntoa(clientAddr.sin_addr) << std::endl;
    return Socket::fromDescriptor(clientSockDescriptor);
}

Socket Socket::fromDescriptor(int socketDescriptor) 
{
    Socket sock;
    sock.socket_ = socketDescriptor;
    return sock;
}

/* int Socket::getSocketDescriptor()
{
	return socket_;
}*/




	void Socket::send(const std::string &message) 
	{
		::send(socket_, message.c_str(), message.size(), 0);
	}

	std::string Socket::recv() 
	{
		char buf[16384];
		memset(buf, 0, sizeof(buf));
		ssize_t received = ::recv(socket_, buf, sizeof(buf), 0);
		if (received == -1) 
		{
			std::cerr << "Error on receiving data: " << strerror(errno) << std::endl;
			return "";
		} 
		else if (received == 0) 
		{
			std::cout << "Connection closed by peer." << std::endl;
			return "";
		}
		return std::string(buf, received);
	}

	/*
	void Socket::setSocket(int socket) 
	{
		socket_ = socket;
	} */

Socket::~Socket() 
{
    if (sockfd != -1) ::close(sockfd);
}


bool Socket::isValid() 
{
    return socket_ != -1;
}

void Socket::close() 
{
    if (sockfd != -1) 
	{
        ::close(sockfd);  
        sockfd = -1; 
        std::cout << "Socket closed successfully." << std::endl;
    }
}


