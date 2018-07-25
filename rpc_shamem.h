#ifndef __RPC_SHAMEM_H
#define __RPC_SHAMEM_H
#endif

bool Rpc_Create_ShaMem(int key, size_t size, Rpc_Open_ShaMem_Mode_T mode, Rpc_ShaMem_Handle_T *  handle);

bool Rpc_Open_ShaMem(int key, Rpc_Open_ShaMem_Mode_T mode, Rpc_ShaMem_Handle_T *  handle);

bool Rpc_Close_ShaMem(Rpc_ShaMem_Handle_T *  handle);



