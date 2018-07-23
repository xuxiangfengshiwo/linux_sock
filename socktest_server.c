#include "socket_server_1.h"

int main()
{
  

        char read_buf[1024];
        char send_buf[2] = "OK";
        Sock_Channel_T sock;
        SockRecvfd  client_sock;
        sock_create(&sock);

        while(1)
        {
                sock_sync_recv(&sock, read_buf, sizeof(read_buf), &client_sock);
                printf("recv msg %s\n", read_buf);
                sock_sync_reply(client_sock, send_buf, sizeof(send_buf));
        }

	return 0;

}
