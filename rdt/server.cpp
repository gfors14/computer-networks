#include "Socket.h"
#include <iostream>
#include <netinet/in.h>
#include <string>

int main() {
   
    Socket server(43539, true); // True for UDP
    std::cout << "Server is running and waiting for messages..." << std::endl;

    while (true) 
	{
		//std::cout << "Inside while True loop" << std::endl;
        sockaddr_in source;
		int sourcePort = -1;
        std::string message = server.recvFrom(source, sourcePort);
		
        if (!message.empty()) {
            std::cout << "Received: " << message << " from port " << sourcePort << std::endl;
        }
    }

    return 0;
}
