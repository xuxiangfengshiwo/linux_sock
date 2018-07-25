#include "log_setting.h"
#ifdef  X_Require
#undef  X_Require
#define X_Require(expr,msg)  \
	if(expr){}  \
	else     \
	{       \
		Log_Fault(msg);\
	}

#endif
