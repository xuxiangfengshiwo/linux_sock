#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <fcntl.h>

/***************define   ********/

#define MAX_CONN   5


/*********** typedef  ***********/

typedef struct  Sock_Channel_T{
/*  sock server fd */
int fd;
/*  sock appid  to identify unix domain sock path*/
pid_t app_id;


}Sock_Channel_T;

typedef int  SockRecvfd;
bool sock_create(Sock_Channel_T * channel);

/*  socket server  side  reply &&  recv   */
bool sock_sync_recv(Sock_Channel_T * server_channel, void * buf, size_t buf_len, SockRecvfd * client);

bool sock_sync_reply(SockRecvfd  client, const void * buf, size_t buf_len);

/*  socket client side send  &&  recv  */

bool sock_sync_send(Sock_Channel_T * server_channel, void * buf, size_t buf_len);
/***********  create unix socket server***********/
/*************
@para: channel
	channel structure contains sockfd created
@return:
	true if success
***********/
bool sock_create(Sock_Channel_T *  channel)
{
	bool status = false;
	pid_t  pid = getpid();
        int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
	channel->fd = fd;
	channel->app_id = pid;
        /* create unix sock by process id*/
	struct sockaddr_un addr;
	snprintf(addr.sun_path, sizeof(addr.sun_path),"/tmp/sock_%d",(int)(channel->app_id));
	addr.sun_family = AF_LOCAL;
	
        if(channel->fd>=0)
        {
        	unlink(addr.sun_path);
        }

	bind(channel->fd, (struct sockaddr *)&addr, SUN_LEN(&addr));
	chmod(addr.sun_path, 00777);
	listen(channel->fd, MAX_CONN);
	
	printf("create sock successfully at sock = %d,  path = %s\n", channel->fd, addr.sun_path);
        status = true;
	
	return status;
}



/************sock listen client connection and recv msg*****/
/**********
@para:
	server_channel, created by sock-create
	buf  , contain the recv msg
	buf_len
	client_channel, client connection fd
@return:
	true if success
**************/

bool sock_sync_recv(Sock_Channel_T * server_channel, void * buf, size_t buf_len, SockRecvfd * client)
{
	bool status = false;
	*client = accept(server_channel->fd, 0, 0);
	if(*client != -1)
	{
		read(*client, buf, buf_len);
		status = true;
	}
	return status;
}


/* 
@para[client]: client identify
@para[buf] : reply buf
@para[buf_len]: length of buf

note: msg direction server--> client
*/
bool sock_sync_reply(SockRecvfd  client,const void * buf, size_t buf_len)
{
	bool success = false;
	struct iovec send[1];
	send[0].iov_base  =(void *)buf;
	send[0].iov_len  = buf_len;
	
	if(writev(client, send, 1)==buf_len)
	{
		success = true;
	}
	close(client);
}

bool sock_sync_send(Sock_Channel_T * server_channel, void * buf, size_t buf_len)
{
	SockRecvfd client;
        struct sockaddr_un addr;
		
	client = socket(AF_UNIX, O_CLOEXEC|SOCK_STREAM, 0 );
	 /* connect unix sock by process id*/
      
        snprintf(addr.sun_path, sizeof(addr.sun_path),"/tmp/sock_%d",(int)(server_channel->app_id));
        addr.sun_family = AF_LOCAL;
	
	connect(client, (struct sockaddr *)(void *)&addr, sizeof(addr));
	
}

int main(int argc, char **argv)
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

  

}


