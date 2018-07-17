
all:socket_server_1

socket_server_1: socket_server_1.c
	gcc -o socket_server_1 socket_server_1.c 
clean:	
	rm -rf socket_server_1


