http.exe: main.o Socket.o request.o response.o
	g++ main.o Socket.o request.o response.o -o http.exe
	
main.o: main.cpp
	g++ -c main.cpp -o main.o
	
Socket.o: Socket.cpp
	g++ -c Socket.cpp -o Socket.o
	
request.o: request.cpp
	g++ -c request.cpp -o request.o
	
response.o: response.cpp
	g++ -c response.cpp -o response.o
	
clean:
	rm *.o http.exe