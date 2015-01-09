/*
 * $Header: /rd_2/project/Mozart/Linux_Libraries/Resize/Resize_Lib/Resize.h 10    13/01/15 3:56p Kensuke.chen $
 *
 * Copyright 2009 VN Inc. All rights reserved.
 *
 * Description:
 *
 * $History: Resize.h $
 * 
 * *****************  Version 10  *****************
 * User: Kensuke.chen Date: 13/01/15   Time: 3:56p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 9  *****************
 * User: Kensuke.chen Date: 12/04/24   Time: 2:02p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 8  *****************
 * User: Kensuke.chen Date: 11-06-22   Time: 11:12
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 7  *****************
 * User: Kensuke.chen Date: 11-04-21   Time: 9:31
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 6  *****************
 * User: Kensuke.chen Date: 11-04-14   Time: 11:09
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 5  *****************
 * User: Kensuke.chen Date: 11-03-08   Time: 14:26
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 4  *****************
 * User: Kensuke.chen Date: 10-12-08   Time: 16:22
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 3  *****************
 * User: Kensuke.chen Date: 10-11-12   Time: 14:12
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 2  *****************
 * User: Kensuke.chen Date: 10-07-09   Time: 16:42
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 1  *****************
 * User: Kensuke.chen Date: 10-02-11   Time: 10:28
 * Created in $/rd_2/project/Mozart/Linux_Libraries/RESIZE/Resize_Lib
 * 
 * *****************  Version 1  *****************
 * User: Kensuke.chen Date: 10-02-11   Time: 10:27
 * Created in $/rd_2/project/Mozart/Linux_Libraries/LINUX_LIBRARIES/Resize/Resize_Lib
 * 
 * *****************  Version 1  *****************
 * User: Kensuke.chen Date: 10-02-11   Time: 10:26
 * Created in $/rd_2/project/Mozart/Linux_Libraries/LINUX_LIBRARIES/Resize/Resize_Lib
 * 
 * *****************  Version 19  *****************
 * User: Ronald       Date: 09/06/10   Time: 10:13a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 18  *****************
 * User: Ronald       Date: 09/04/20   Time: 2:34p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 17  *****************
 * User: Ronald       Date: 09/01/12   Time: 10:45a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 16  *****************
 * User: Ronald       Date: 09/01/05   Time: 1:43p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 15  *****************
 * User: Ycchang      Date: 08/09/26   Time: 11:31a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 14  *****************
 * User: Ronald       Date: 08/09/26   Time: 10:12a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 13  *****************
 * User: Ronald       Date: 08/09/25   Time: 10:47p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 12  *****************
 * User: Ronald       Date: 08/09/25   Time: 1:54p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 11  *****************
 * User: Ronald       Date: 08/01/17   Time: 4:31p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 10  *****************
 * User: Ronald       Date: 08/01/16   Time: 5:29p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 9  *****************
 * User: Ycchang      Date: 08/01/10   Time: 1:07p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 8  *****************
 * User: Ronald       Date: 08/01/09   Time: 2:50p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 7  *****************
 * User: Ronald       Date: 07/09/07   Time: 4:38p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 6  *****************
 * User: Ronald       Date: 07/08/29   Time: 2:19p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 5  *****************
 * User: Ronald       Date: 07/08/21   Time: 4:39p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 4  *****************
 * User: Ronald       Date: 07/08/21   Time: 3:09p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 3  *****************
 * User: Fred         Date: 07/05/02   Time: 11:33a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 2  *****************
 * User: Fred         Date: 07/05/01   Time: 3:18p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 * *****************  Version 1  *****************
 * User: Fred         Date: 06/12/20   Time: 5:48p
 * Created in $/rd_2/project/SoC/Linux_Libraries/Resize/Resize_Lib
 * 
 */


/* =========================================================================================== */
#ifndef __RESIZE_H__
#define __RESIZE_H__

/* =========================================================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "errordef.h"
#include "global_codec.h"
#include "video_error.h"

/* =========================================================================================== */
#define RESIZE_VERSION MAKEFOURCC(5, 0, 0, 7)
/* Version 5.0.0.5 modification, 2011.06.22 */
#define RESIZE_ID_VERSION "5.0.0.7"
/* ======================================== */
//#define __USE_PROFILE__

#define VMA_IRE_FMT_YCBCR420	0x0
#define VMA_IRE_FMT_YCBCR422	0x1
#define VMA_IRE_FMT_YCBCR444	0x2

#ifdef __ASM_ARCH_PLATFORM_HAYDN_H__
// Bus1, DRAM1
#define RESIZE_IN_BUS_NUM	1
#define RESIZE_OUT_BUS_NUM	1
#endif //__ASM_ARCH_PLATFORM_HAYDN_H__

#ifdef __ASM_ARCH_PLATFORM_MOZART_H__
// Bus2, DRAM1
#define RESIZE_IN_BUS_NUM	1
#define RESIZE_OUT_BUS_NUM	1
#endif //__ASM_ARCH_PLATFORM_MOZART_H__

/* =========================================================================================== */
typedef struct resize_init_options
{
	DWORD dwVersion;
/* Version 5.0.0.0 modification, 2010.07.09 */
/* ======================================== */

/* Version 4.0.0.0 modification, 2008.09.25 */
/* ======================================== */
	void *pObjectMem;
} TResizeInitOptions;

typedef struct resize_state
{
/* Version 5.0.0.0 modification, 2010.07.09 */
	DWORD dwInWidth;
	DWORD dwInHeight;
	DWORD dwOutWidth;
	DWORD dwOutHeight;
	DWORD dwInStride;
	DWORD dwOutStride;
	DWORD dwImageMode;
	DWORD dwCmptNum;

	EPixelFormatFlags ePixelFormat;
/* ======================================== */

	BYTE *pbyYInFrame;				// Input Y frame buffer
	BYTE *pbyCbInFrame;				// Input Cb frame buffer
	BYTE *pbyCrInFrame;				// Input Cr frame buffer
	BYTE *pbyYOutFrame;				// Output Y frame buffer
	BYTE *pbyCbOutFrame;			// Output Cb frame buffer
	BYTE *pbyCrOutFrame;			// Output Cr frame buffer
} TResizeState;
/* ======================================== */

/* =========================================================================================== */
SCODE Resize_Initial(HANDLE *phObject, TResizeInitOptions *ptInitOptions);

SCODE Resize_Release(HANDLE *phObject);

SCODE Resize_ProcessOneFrame(HANDLE hObject, TResizeState *ptState);
/* Version 4.1.0.0 modification, 2009.06.10 */
SCODE Resize_StartOneFrame(HANDLE hObject, TResizeState *ptState);
SCODE Resize_WaitOneFrameComplete(HANDLE hObject, TResizeState *ptState);
/* ======================================== */

SCODE Resize_CheckParam(HANDLE hObject, TResizeState *ptState);

DWORD Resize_QueryMemSize(TResizeInitOptions *ptInitOptions);

SCODE Resize_GetVersionInfo(BYTE *pbyMajor, BYTE *pbyMinor, BYTE *pbyBuild, BYTE *pbyRevision);

/* =========================================================================================== */
#endif //__RESIZE_H__
