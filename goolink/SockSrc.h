#ifndef __SOCKET_SOURCE_H__
#define __SOCKET_SOURCE_H__

#include "SrcCommon.h"

typedef struct socksrc_init_options
{
	CHAR	*szRecvSockPath;
	CHAR	*szCmdFiFoPath;
} TSockSrcInitOpts;

int   SockSrc_GetFd(HANDLE hSockSrc);
SCODE SockSrc_Initial(HANDLE *phSockSrc, TSockSrcInitOpts *ptInitOpts);
SCODE SockSrc_Release(HANDLE *phSockSrc);

SCODE SockSrc_GetUBuffer(HANDLE hSockSrc, TSrcUBufInfo *ptUBuffInfo);
SCODE SockSrc_ReleaseUBuffer(HANDLE hSockSrc, TSrcUBufInfo *ptUBuffInfo);
SCODE SockSrc_EventHandler(HANDLE hSockSrc, ESrcEventType eType);

#endif // __SOCKET_SOURCE_H__
