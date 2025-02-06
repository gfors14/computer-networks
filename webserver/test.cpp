#include "WebServer.h"

	
int main()
{
    int port = 1072;
    std::string documentRoot = "/home/gfors/CS445/websiteserver/html/blue.jpg";

    WebServer server(port, documentRoot);
    server.start();

    return 0;
}
