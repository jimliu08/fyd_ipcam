#ifndef __LIVE_MEDIA_SOURCE_H__
#define __LIVE_MEDIA_SOURCE_H__

#include "common.h"
#include "typedef.h"
#include "global_codec.h"
#include "dbgdefs.h"

typedef struct lmsrc_ubuf_info
{
	BYTE	*pbyUBuffHdr;
	int		iUBuffHdrLen;
	BYTE	*pbyUBuffPayload;
	int		iUBuffPayloadLen;
} TLMSrcUBufInfo;

typedef enum lmsrc_event_type
{
	letNeedIntra,
	letNeedConf,
	letBitstrmStart,
	letBitstrmStop
} ELMSrcEventType;

SCODE LiveMediaSrc_Release(HANDLE *phObject);
SCODE LiveMediaSrc_GetUBuffer(HANDLE hLMSrc, TLMSrcUBufInfo *ptUBuffInfo);
SCODE LiveMediaSrc_ReleaseUBuffer(HANDLE hLMSrc, TLMSrcUBufInfo *ptUBuffInfo);
SCODE LiveMediaSrc_EventHandler(HANDLE hLMSrc, ELMSrcEventType eType);

#endif // __LIVE_MEDIA_SOURCE_H__
