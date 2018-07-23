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

/*  socket client side send  without  recv  */

bool sock_sync_sendN(Sock_Channel_T * server_channel, void * buf, size_t buf_len);


