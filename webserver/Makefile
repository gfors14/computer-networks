server.exe: test.o Socket.o request.o response.o Buffer.o WebServer.o
	g++ test.o Socket.o request.o response.o Buffer.o WebServer.o  -o server.exe

WebServer.o: WebServer.cpp
	g++ -c WebServer.cpp -o WebServer.o

test.o: test.cpp
	g++ -c test.cpp -o test.o
	
Socket.o: Socket.cpp
	g++ -c Socket.cpp -o Socket.o
	
request.o: request.cpp
	g++ -c request.cpp -o request.o
	
response.o: response.cpp
	g++ -c response.cpp -o response.o
	
Buffer.o: Buffer.cpp
	g++ -c Buffer.cpp -o Buffer.o
	

clean:
	rm *.o server.exe