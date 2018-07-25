#include  "x_assert.h"
#include  "x_data_types.h"

#include  <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "rpc_shamem.h"

bool Rpc_Open_ShaMem(int key, Rpc_Open_ShaMem_Mode_T mode, Rpc_ShaMem_Handle_T *  handle)
{
	int shm_fd;
        bool status = false;
	char file_path[32];

        snprintf(file_path, sizeof(file_path), "x_shamem_%d", key);

        X_Require(key>=0, "invalid key");
        X_Require(handle!= NULL, "invalid handle");

        int open_mode = (mode == SHAMEM_READONLY)?(O_RDONLY | O_CREAT):(O_RDWR | O_CREAT);
        shm_fd = shm_open(file_path, open_mode, S_IRWXU | S_IRWXG | S_IRWXO);
        if(shm_fd>= 0)/* valid memfd */
        {
		 struct stat file_stat;
     		 if (fstat(shm_fd, &file_stat) == 0)
		{
                        int mmap_mode = (mode == SHAMEM_READONLY)?(PROT_READ):(PROT_READ|PROT_WRITE);
                   	 void* shm = mmap(
            			NULL, 
           			 (size_t)file_stat.st_size, 
           			 mmap_mode, 
           			 MAP_SHARED, 
            			shm_fd, 
        			    0);       
                        if(shm != MAP_FAILED)
                        {
                                handle->key = key;
                                handle->data = shm;
                                handle->size = (size_t)file_stat.st_size;
                                handle->fd = shm_fd;

                                status = true;
                        }
                        else
                        {
                        Log_Fault("failed to mmap");
			close(shm_fd);
                        }

                }
                else
                {
                        Log_Fault("failed to ftrucate shamem size");
			close(shm_fd);
                }
        }
        else
        {
                Log_Fault("can not open shamem");
        }

        return status;

	
}
