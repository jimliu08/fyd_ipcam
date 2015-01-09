#ifndef __SRC_COMMON_H__
#define __SRC_COMMON_H__

#include "ubuffer.h"
#include "ubuffer_limit.h"
//#include "avicontainer.h"
//#if 0
typedef struct src_ubuf_info
{
	BYTE	*pbyUBuffHdr;
	int		iUBuffHdrLen;
	BYTE	*pbyUBuffPayload;
	int		iUBuffPayloadLen;
} TSrcUBufInfo;
//#endif
typedef enum src_event_type
{
	LetNeedIntra,
	LetNeedConf,
	LetBitstrmStart,
	LetBitstrmStop
} ESrcEventType;

#endif

