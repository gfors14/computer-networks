#include "Socket.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>


Socket::Socket() : socket_(-1), port_(0), host_("") {} 

Socket::Socket(std::string host, int port) : socket_(-1), port_(port), host_(host) {} 

Socket::Socket(int port) : port_(port), socket_(-1) 
{
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ == -1) {
        std::cerr << "Error: Could not create socket\n";
        return;
    }

    int opt = 1;
    if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) 
	{
        std::cerr << "Error: Setting SO_REUSEADDR socket option failed\n";
        return;
    }

    address_.sin_family = AF_INET;
    address_.sin_port = htons(port_);
    address_.sin_addr.s_addr = INADDR_ANY;
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

    // nslookup
    const char* ipAddr = "159.242.4.72";

    int inetResult = inet_pton(AF_INET, ipAddr, &address_.sin_addr);
    std::cout << "inet_pton result: " << inetResult << std::endl;

    if (inetResult <= 0) {
        std::cerr << "Error: Invalid address or address family\n";
        return;
    }

    // fix output
    char addrStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address_.sin_addr, addrStr, INET_ADDRSTRLEN);
    std::cout << "Address: " << addrStr << std::endl;

    if (::connect(socket_, (struct sockaddr *)&address_, sizeof(address_)) == -1) {
        std::cerr << "Error: Connection failed - " << strerror(errno) << "\n";
        return;
    }

    std::cout << "Connected to " << host_ << ":" << port_ << std::endl;
}




void Socket::bind() //specifies local address and binds to it
{
    socket_ = socket(AF_INET, SOCK_STREAM, 0); 
    if (socket_ == -1) {
        std::cerr << "Error: Could not create socket\n";
        return;
    }

    address_.sin_family = AF_INET;
    address_.sin_port = htons(port_);
    address_.sin_addr.s_addr = INADDR_ANY;

    if (::bind(socket_, (struct sockaddr *)&address_, sizeof(address_)) == -1) 
	{
        std::cerr << "Error: Bind failed - " << strerror(errno) << "\n";
        return;
    }

    std::cout << "Bound to port " << port_ << std::endl;
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

int Socket::getSocketDescriptor()
{
	return socket_;
}




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


void Socket::setSocket(int socket) 
{
    socket_ = socket;
}




Socket::~Socket() 
{
    if (socket_ != -1)
        ::close(socket_);
}

bool Socket::isValid() 
{
    return socket_ != -1;
}


