#include "WebServer.h"
#include "Buffer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <filesystem>

WebServer::WebServer(int port, const std::string& documentRoot) : port_(port), documentRoot_(documentRoot) 
{
    initializeMimeTypeMap();
}

void WebServer::initializeMimeTypeMap() 
{
    mimeTypes_[".html"] = "text/html";
    mimeTypes_[".css"] = "text/css";
    mimeTypes_[".jpg"] = "image/jpeg";
    mimeTypes_[".jpeg"] = "image/jpeg";
    mimeTypes_[".png"] = "image/png";

}

void WebServer::start() 
{
    Socket serverSocket(port_); 
    serverSocket.bind();        
    serverSocket.listen();      

    std::cout << "Server listening on port " << port_ << std::endl;

    while (true) 
	{ 
        std::cout << "Waiting for new connection..." << std::endl;

        struct sockaddr_in clientAddr; // Client address
        socklen_t clientAddrSize = sizeof(clientAddr);
        // Accept a new connection
        int clientSockDescriptor = ::accept(serverSocket.getSocketDescriptor(), (struct sockaddr*)&clientAddr, &clientAddrSize);

        if (clientSockDescriptor == -1) {
            // Error if accept fails
            std::cerr << "Error accepting connection: " << strerror(errno) << std::endl;
            continue;
        }

        Socket clientSocket = Socket::fromDescriptor(clientSockDescriptor);
        std::cout << "Accepted new connection" << std::endl;

        // Handle the connection with the client
        handleConnection(clientSocket);

    }
}


void WebServer::handleConnection(Socket& clientSocket) 
{
    std::cout << "Handling new connection..." << std::endl; 
    
    Buffer requestBuffer;
    std::string dataPart = clientSocket.recv();

    if (dataPart.empty()) {
        std::cout << "Received empty data part, closing connection." << std::endl;
        return;
    }

    requestBuffer.append(reinterpret_cast<const std::byte*>(dataPart.data()), dataPart.size());
    std::string requestStr = requestBuffer.toString();

    std::cout << "Received request: " << std::endl << requestStr << std::endl; 

    if (requestStr.find("\r\n\r\n") == std::string::npos) 
	{
        std::cout << "Request headers not complete or missing, closing connection." << std::endl;
        return;
    }

    request req;
    // Parse the request
    std::cout << "Request parsed successfully." << std::endl; 
    
    response resp = generateResponse(req);
    std::cout << "Sending response..." << std::endl; 
    clientSocket.send(resp.to_string());
    std::cout << "Response sent successfully." << std::endl; 
}


response WebServer::generateResponse(const request& req) 
{
    // For testing,
    std::string filePath = "/home/gfors/public_html/CS445/websiteserver/html/blue.jpg";
    std::cout << "Using hardcoded file path: " << filePath << std::endl;

    // Check for file existence as before
    if (!std::filesystem::exists(filePath)) 
	{
        std::cout << "Hardcoded file not found, sending 404 response." << std::endl;
        response resp("HTTP/1.1", "404", "Not Found");
        resp.set_data("<html><body><h1>404 Not Found</h1></body></html>");
        return resp;
    } 
	else 
	{
        std::cout << "Hardcoded file exists, proceeding to serve." << std::endl;
    }

    std::string mimeType = getMimeType(std::filesystem::path(filePath).extension());
    std::cout << "MIME type: " << mimeType << std::endl;

    return serveFile(filePath, mimeType);
}


response WebServer::serveFile(const std::string& filePath, const std::string& mimeType) 
{
	
	 // Check if file exists and can be read
    if (!std::filesystem::exists(filePath)) 
	{
        std::cerr << "Debug: File does not exist: " << filePath << std::endl;
    } 
	else if (!std::filesystem::is_regular_file(filePath)) 
	{
        std::cerr << "Debug: Path is not a file: " << filePath << std::endl;
    } 
	else if (std::filesystem::is_empty(filePath)) 
	{
        std::cerr << "Debug: File is empty: " << filePath << std::endl;
    } 
	else 
	{
        std::cerr << "Debug: File exists and is not empty: " << filePath << std::endl;
        auto file_size = std::filesystem::file_size(filePath);
        std::cerr << "Debug: File size: " << file_size << " bytes" << std::endl;
    }

    std::ifstream file;
    try 
	{
        file.open(filePath, std::ios::binary);
    } 
	catch (const std::ifstream::failure& e) 
	{
        std::cerr << "Exception opening/reading file: " << e.what() << std::endl;
    }

    if (!file) 
	{
        std::cerr << "Failed to open file even after checks: " << filePath << std::endl;
        response resp("HTTP/1.1", "404", "Not Found");
        resp.set_data("<html><body><h1>404 Not Found</h1></body></html>");
        return resp;
   
   }


    // read from file
    Buffer fileBuffer;
    std::vector<std::byte> tempBuffer(1024);
    
    while (file.read(reinterpret_cast<char*>(tempBuffer.data()), tempBuffer.size()) || file.gcount() > 0) 
	{
        fileBuffer.append(tempBuffer.data(), file.gcount());
    }

    std::string fileContent = fileBuffer.toString();
    response resp("HTTP/1.1", "200", "OK");
    resp.add_header("Content-Type", mimeType);
    resp.set_data(fileContent);

    return resp;
}


std::string WebServer::resolveFilePath(const std::string& requestPath) 
{
    std::cout << "Original request path: " << requestPath << std::endl;

    std::string fullPath = documentRoot_;

    std::cout << "Initial document root: " << documentRoot_ << std::endl;

    // default name if the path is the root
    std::string normalizedPath = requestPath;
    if (normalizedPath == "/" || normalizedPath.empty()) 
	{
        normalizedPath = "/index.html"; // Default file
    }

    std::cout << "Normalized path: " << normalizedPath << std::endl;

    if (fullPath.back() != '/') 
	{
        fullPath += "/";
    }

    // Append requestPath ensuring the first character is not a slash
    if (normalizedPath.front() == '/') 
	{
        fullPath += normalizedPath.substr(1);
    } 
	else 
	{
        fullPath += normalizedPath;
    }

    std::cout << "Resolved file path: " << fullPath << std::endl;
    return fullPath;
}



std::string WebServer::getMimeType(const std::string& extension) 
{
    std::cout << "Querying MIME type for extension: " << extension << std::endl; // Debug 
    if (mimeTypes_.find(extension) != mimeTypes_.end()) 
	{
        std::string mimeType = mimeTypes_[extension];
        std::cout << "Found MIME type: " << mimeType << std::endl; // Debug
        return mimeType;
    }
    std::cout << "MIME type not found, defaulting to application/octet-stream" << std::endl; // Debug 
    return "application/octet-stream";
}
