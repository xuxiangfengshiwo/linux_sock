#include  "x_assert.h"
#include  "x_data_types.h"

#include  <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>


bool Rpc_Create_ShaMem(int key, size_t size, Rpc_Open_ShaMem_Mode_T mode, Rpc_ShaMem_Handle_T *  handle)
{
	char file_path[32];
	int shm_fd;
	bool status = false;

	snprintf(file_path, sizeof(file_path), "x_shamem_%d", key);
	
	X_Require(key>=0, "invalid key");
	X_Require(size>=0, "invalid size");

	int open_mode = (mode == SHAMEM_READONLY)?(O_RDONLY | O_CREAT):(O_RDWR | O_CREAT);
	shm_fd = shm_open(file_path, open_mode, S_IRWXU | S_IRWXG | S_IRWXO);	
	if(shm_fd>= 0)/* valid memfd */
	{
		if(ftruncate(shm_fd, (off_t)size)==0)/*truncate shm_fd size */
		{
			int mmap_mode = (mode == SHAMEM_READONLY)?(PROT_READ):(PROT_READ|PROT_WRITE);
			void* shm = mmap(NULL, size, mmap_mode, MAP_SHARED, shm_fd, 0);/*  mmap shamem*/
			if(shm != MAP_FAILED)
			{
				handle->key = key;
				handle->data = shm;
				handle->size = size;
				handle->fd = shm_fd;
				
				status = true;
			}
			else
			{
			Log_Fault("failed to mmap");
			}

		}
		else
		{
			Log_Fault("failed to ftrucate shamem size");
		}
	}
	else
	{
		Log_Fault("can not open shamem");
	}	
	
	return status;
}
