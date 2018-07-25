
#include <sys/types.h>
typedef enum  Rpc_Open_ShaMem_Mode_T
{
	SHAMEM_READONLY,
	SHAMEM_RDWR
}Rpc_Open_ShaMem_Mode_T;


typedef struct Rpc_ShaMem_Handle_T
{
	int key;
	void * data;
	int fd;
	size_t size;
}Rpc_ShaMem_Handle_T;
