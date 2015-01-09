/*
 * $Header: /rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib/PBEngine.h 29    13/05/20 11:22a Jaja $
 *
 * Copyright 2008 VN, Inc.  All rights reserved.
 *
 * Description:
 *
 * $History: PBEngine.h $
 * 
 * *****************  Version 29  *****************
 * User: Jaja         Date: 13/05/20   Time: 11:22a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 3.1.0.0 to 3.1.0.1 - DONE.
* 
 * *****************  Version 28  *****************
 * User: Jaja         Date: 13/05/03   Time: 8:38a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 3.0.0.12 to 3.1.0.0 - DONE.
 *
 * *****************  Version 27  *****************
 * User: Jaja         Date: 13/01/13   Time: 3:09a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 3.0.0.11 to 3.0.0.12 - DONE.
 *
 * *****************  Version 26  *****************
 * User: Jaja         Date: 12/04/13   Time: 12:05a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 3.0.0.10 to 3.0.0.11 - DONE.
 *
 * *****************  Version 25  *****************
 * User: Jaja         Date: 11/12/02   Time: 2:02p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 3.0.0.9 to 3.0.0.10 - DONE.
 *
 * *****************  Version 24  *****************
 * User: Alan         Date: 10/09/13   Time: 3:59p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 3.0.0.8 to 3.0.0.9 - DONE.
 *
 * *****************  Version 23  *****************
 * User: Jaja         Date: 10/08/25   Time: 8:53p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 3.0.0.7 to 3.0.0.8 - DONE.
 *
 * *****************  Version 22  *****************
 * User: Bernard      Date: 10/05/20   Time: 2:23p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * 1. MODIFICATION: Remove DLLAPI for PB_SpliceAlignOneBits() - DONE.
 * 2. MODIFICATION: Add PB_SpliceAlignOneBits() in PBEngine.h - DONE.
 *
 * 3. MODIFICATION: Add PB_SetZeroCount() in PBEngine.h - DONE.
 * UPDATE: Update version number from 3.0.0.6 to 3.0.0.7 - DONE.
 *
 * *****************  Version 21  *****************
 * User: Jaja         Date: 10/04/02   Time: 7:26p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 3.0.0.5 to 3.0.0.6 - DONE.
 *
 * *****************  Version 20  *****************
 * User: Lawrence.hu  Date: 10/01/05   Time: 4:41p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * FEATURE: Synchronous with MassMemAccess_Lib version 8.0.0.0 - DONE.
 * MODIFICATION: Add pbySrcAddrY and pbyDstAddrY for MassMemAccess_Lib on
 * Beethoven Platform - DONE.
 *
 * *****************  Version 19  *****************
 * User: Alan         Date: 09/06/09   Time: 9:10p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 3.0.0.3 to 3.0.0.4 - DONE.
 *
 * *****************  Version 18  *****************
 * User: Jaja         Date: 09/04/01   Time: 10:06a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 3.0.0.2 to 3.0.0.3 - DONE.
 *
 * *****************  Version 17  *****************
 * User: Alan         Date: 09/01/08   Time: 4:32p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * MODIFICATION: Add ID string declaration - DONE.
 * UPDATE: Update version number from 3.0.0.1 to 3.0.0.2 - DONE.
 *
 * *****************  Version 16  *****************
 * User: Sor          Date: 08/01/28   Time: 11:24a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 3.0.0.0 to 3.0.0.1 - DONE.
 *
 * *****************  Version 15  *****************
 * User: Sor          Date: 08/01/26   Time: 10:37p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 *
 * *****************  Version 14  *****************
 * User: Sor          Date: 08/01/26   Time: 10:19p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * FEATURE: Add DMA support when using Bach architecture in PB_PutBytes()
 * - DONE.
 *
 * FEATURE: Change all UnAlign to Unalign - DONE.
 *
 * BUG: The initial value of phObject should be assigned before any error
 * check procedure in MassMemAccess_Initial() - FIXED.
 *
 * *****************  Version 13  *****************
 * User: Jaja         Date: 08/01/21   Time: 10:52p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 *
 * *****************  Version 12  *****************
 * User: Jaja         Date: 07/12/10   Time: 2:32p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 2.2.0.0 to 2.3.0.0 - DONE.
 *
 * *****************  Version 11  *****************
 * User: Bernard      Date: 07/11/22   Time: 9:42a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 2.1.0.0 to 2.2.0.0 - DONE.
 *
 * *****************  Version 10  *****************
 * User: Bernard      Date: 07/11/22   Time: 9:41a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * FEATURE: Add PB_GetUnalignBytes(), PB_GetCurrAddr() - DONE.
 *
 * *****************  Version 9  *****************
 * User: Bernard      Date: 07/09/14   Time: 4:30p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 2.0.0.2 to 2.1.0.0 - DONE.
 *
 * *****************  Version 8  *****************
 * User: Bernard      Date: 07/09/14   Time: 4:29p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 *
 * *****************  Version 7  *****************
 * User: Bernard      Date: 07/02/02   Time: 12:01p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 2.0.0.1 to 2.0.0.2 - DONE.
 *
 * *****************  Version 6  *****************
 * User: Jaja         Date: 06/10/17   Time: 8:03p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 *
 * *****************  Version 4  *****************
 * User: Jaja         Date: 06/06/21   Time: 10:47p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 2.0.0.0 to 2.0.0.1 - DONE.
 * *****************  Version 3  *****************
 * User: Jaja         Date: 06/01/26   Time: 2:29p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 * UPDATE: Update version number from 1.0.0.0 to 2.0.0.0 - DONE.
 *
 * *****************  Version 2  *****************
 * User: Jaja         Date: 06/01/26   Time: 2:28p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 *
 * *****************  Version 1  *****************
 * User: Sor          Date: 05/07/22   Time: 5:54p
 * Created in $/rd_2/project/SoC/Linux_Libraries/Sample/PBEngine_Lib
 *
 */

/* =========================================================================================== */
#ifndef __PBENGINE_H__
#define __PBENGINE_H__

/* =========================================================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "errordef.h"
#include "global_codec.h"

/* =========================================================================================== */
#define PBENGINE_VERSION MAKEFOURCC(3, 1, 0, 1)
/* Version 3.0.0.2 modification, 2009.01.08 */
#define PBENGINE_ID_VERSION "3.1.0.1"
/* ======================================== */

/* =========================================================================================== */
typedef struct pb_engine_init_options
{
	DWORD dwVersion;
	DWORD dwBufSize;		/* PB engine buffer size */
	DWORD dwTimeOut;		/* Loop checking time */
	DWORD dwSemaphore;
	DWORD dwUserData;

/* Version 3.0.0.0 modification, 2008.01.26 */
	BOOL bEnableDMA;
/* ======================================== */

	EBufTypeFlags		eBufTypeFlags;
	FOnSendData			pfnSend;
	FOnEnterCriticalSec pfnEnter;
	FOnLeaveCriticalSec pfnLeave;

	void	*pObjectMem;
} TPBEngineInitOptions;

/* =========================================================================================== */
#ifdef __cplusplus
extern "C" {
#endif

/* PB_Initial initializes the PB engine. */
SCODE PB_Initial(HANDLE *phObject, TPBEngineInitOptions *pPBInitOptions);

/* PB_Release releases the PB engine. */
SCODE PB_Release(HANDLE *phObject);

/* PB_SetBufAddr sets the new buffer to the PB engine. */
SCODE PB_SetNewBuf(HANDLE hObject, BYTE *pbyBuf, DWORD dwBufSize);

/* PB_UpdateByteCount updates the emptiness value of the PB engine
	 buffer since the previous calling of PB_UpdateByteCount function. */
SCODE PB_UpdateByteCount(HANDLE hObject);

/* PB_SpliceAlign makes the PB engine buffer move to the byte-aligned
	 position with splicing the unaligned bits. */
SCODE PB_SpliceAlign(HANDLE hObject);

/* Version 3.0.0.7 modification, 2010.05.20 */
/* PB_SpliceAlignOneBits makes the PB engine buffer padding several bits
	 of one to the byte-aligned position with splicing the unaligned bits. */
SCODE PB_SpliceAlignOneBits(HANDLE hObject);
/* ======================================== */

/* PB_SpliceBytes splice dwNumBytes from the dwsymbol to the PB engine. */
SCODE PB_SpliceBytes(HANDLE hObject, DWORD dwNumBytes, DWORD dwSymbol);

/* PB_SpliceBits splices dwNumBits from the dwSymbol to the PB engine
	 buffer. */
SCODE PB_SpliceBits(HANDLE hObject, DWORD dwNumBits, DWORD dwSymbol);

/* PB_PutBytes puts dwNumBytes from pbyBuf to the PB engine buffer and
   splices the same bytes from PB engine buffer. */
SCODE PB_PutBytes(HANDLE hObject, BYTE *pbyBuf, DWORD dwNumBytes);

/* PB_BackwardAlignBytes shift used bytes forward according specified offset */
SCODE PB_ShiftUsedBytes(HANDLE hObject, DWORD dwOffset);

/* PB_GetBytes gets dwNumBytes from the PB engine buffer to the pbyBuf. */
SCODE PB_GetBytes(HANDLE hObject, BYTE *pbyBuf, DWORD dwNumBytes);

/* PB_FlushPool flushes the PB engine buffer to empty. */
SCODE PB_FlushPool(HANDLE hObject);

/* PB_GetFullness gets the fullness of the PB engine buffer. */
DWORD PB_GetFullness(HANDLE hObject);

/* PB_Stop forces the PB engine function leaving the waiting loop
	 immediately and return S_FAIL to the calling function. */
SCODE PB_Stop(HANDLE hObject);

/* PB_Continue must be called before any PB engine function when the
	 PB_Stop has been called. */
SCODE PB_Continue(HANDLE hObject);

/* For MPEG-4 video encoder */
SCODE PB_SpliceStuffing(HANDLE hObject);

/* Get the encoded byte counts. */
DWORD PB_GetEncBytes(HANDLE hObject);

/* Get the encoded bits counts. */
DWORD PB_GetEncBits(HANDLE hObject);

SCODE DLLAPI PB_SetBookmark(HANDLE hObject);

SCODE DLLAPI PB_GotoBookmark(HANDLE hObject);

DWORD PB_QueryMemSize(TPBEngineInitOptions *pPBInitOptions);

DWORD PB_GetBitPosition(HANDLE hObject);

DWORD PB_GetSymbol(HANDLE hObject);

SCODE PB_SetBitPosition(HANDLE hObject, DWORD);

/* Version 2.1.0.0 modification, 2007.09.12 */
SCODE PB_UpdateBytePosition(HANDLE hObject, DWORD);

DWORD PB_GetBaseAddr(HANDLE hObject);

DWORD PB_GetBufSize(HANDLE hObject);
/* ======================================== */

/* Version 2.2.0.0 modification, 2007.11.21 */
/* Version 3.0.0.0 modification, 2008.01.26 */
QWORD PB_GetUnalignBytes(HANDLE hObject);
/* ======================================== */

DWORD PB_GetCurrAddr(HANDLE hObject);

DWORD PB_GetZeroCount(HANDLE hObject);
/* ======================================== */

SCODE PB_SetSymbol(HANDLE hObject, DWORD);

/* Version 3.0.0.7 modification, 2010.05.20 */
SCODE PB_SetZeroCount(HANDLE hObject, DWORD dwZeroCount);
/* ======================================== */

SCODE PB_GetVersionInfo(BYTE *pbyMajor, BYTE *pbyMinor, BYTE *pbyBuild, BYTE *pbyRevision);

#ifdef __cplusplus
}
#endif

/* =========================================================================================== */
#endif //__PBENGINE_H__
