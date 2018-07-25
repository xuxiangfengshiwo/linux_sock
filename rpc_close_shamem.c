#include "log_setting.h"
#include "x_assert.h"
#include "x_data_types.h"

#include "stdio.h"
#include "stdbool.h"

#include "rpc_shamem.h"

bool Rpc_Close_ShaMem(Rpc_ShaMem_Handle_T *  handle)
{
	bool  status = true;
	char path[32];
	X_Require(handle!= NULL, "null handle" );
	X_Require(handle->data!= NULL,"null data");
	X_Require(handle->size>= 0,"non pos  size");


	/*unmap shamem */
	if(munmap(handle->data, handle->size)!= 0)
	{
		Log_Fault("munmap shamem failed at key = %d, fd = %d",handle->key, handle->fd);
		status = false;
	}
	/*close shamem*/
	if(close(handle->fd)!=0)
	{
		Log_Fault("close shamem failed at key = %d , fd = %d", handle->key, handle->fd);		
	}
	/*unlink shamem path*/
	snprintf(path,sizeof(path),"x_shamem_%d", handle->key);
	
	if(shm_unlink(path)!= 0)
	{
		status = false;
		Log_Fault("unlink shamem failed");
	}
	
	return status;
		
}
