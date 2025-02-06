#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Socket.h" 
#include "request.h" 
#include "response.h" 
#include <string>
#include <unordered_map>
#include <filesystem>
#include "Buffer.h"

class WebServer 
{
	public:
		WebServer(int port, const std::string& documentRoot);
		void start();

	private:
		int port_;
		std::string documentRoot_;
		std::unordered_map<std::string, std::string> mimeTypes_;

		void initializeMimeTypeMap();
		void handleConnection(Socket& clientSocket);
		std::string resolveFilePath(const std::string& path);
		std::string getMimeType(const std::string& extension);
		response generateResponse(const request& req);
		response serveFile(const std::string& filePath, const std::string& mimeType);
};

#endif 
