#include "Socket.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


Socket::Socket() : socket_(-1), port_(0), host_("") {} //initizalize default constructor

Socket::Socket(std::string host, int port) : socket_(-1), port_(port), host_(host) {} //parameterized constructor which initializes values

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
    const char* ipAddr = "34.198.0.55"; // IP address of httpbin.org

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

    if (::bind(socket_, (struct sockaddr *)&address_, sizeof(address_)) == -1) {
        std::cerr << "Error: Bind failed\n";
        return;
    }

    std::cout << "Bound to port " << port_ << std::endl;
}

void Socket::listen() 
{
    if (::listen(socket_, SOMAXCONN) == -1) {
        std::cerr << "Error: Could not listen\n";
        return;
    }

    std::cout << "Listening on port " << port_ << std::endl;
}

int Socket::accept() 
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int clientSocket = ::accept(socket_, (struct sockaddr *)&clientAddr, &clientAddrSize);
    if (clientSocket == -1) {
        std::cerr << "Error: Could not accept call\n";
        return -1;
    }

    std::cout << "Accepted connection from " << inet_ntoa(clientAddr.sin_addr) << std::endl;
    return clientSocket;
}

void Socket::send(const std::string &message) 
{
    ::send(socket_, message.c_str(), message.size(), 0);
}

std::string Socket::recv() 
{
    char buf[4096];
    memset(buf, 0, sizeof(buf));
    ::recv(socket_, buf, sizeof(buf), 0);
    return std::string(buf);
}

void Socket::setSocket(int socket) {
    socket_ = socket;
}



Socket::~Socket() 
{
    if (socket_ != -1)
        ::close(socket_);
}

