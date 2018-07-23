
all:socket_server

socket_server: socktest_server.c
	gcc -o socket_server socktest_server.c socket_server_1.c 
clean:	
	rm -rf socket_server


