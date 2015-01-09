/*
 * $Header: /rd_2/project/Mozart/Linux_Libraries/JPEGEnc/JPEGEnc_Lib/JPEGEnc.h 40    13/01/17 10:45a Andy.tsao $
 *
 * Copyright 2008 VN Inc. All rights reserved.
 *
 * Description:
 *
 * $History: JPEGEnc.h $
 * 
 * *****************  Version 40  *****************
 * User: Andy.tsao    Date: 13/01/17   Time: 10:45a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * 
 * *****************  Version 39  *****************
 * User: Andy.tsao    Date: 12/04/27   Time: 6:30p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * 
 * *****************  Version 38  *****************
 * User: Bernard      Date: 11/07/13   Time: 5:13p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * Update version number from 4.4.0.0 to 4.4.0.1 - Done.
 * 
 * *****************  Version 37  *****************
 * User: Bernard      Date: 10/06/07   Time: 3:58p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * FEATURE: Add new optios for VIDEO_CHANGE_QUANT_TABLE. - DONE.
 * 
 * *****************  Version 36  *****************
 * User: Bernard      Date: 10/05/17   Time: 6:33p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * 
 * *****************  Version 35  *****************
 * User: Bernard      Date: 10/05/17   Time: 4:42p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * Update version number from 4.3.0.3 to 4.3.0.4 - Done.
 * 
 * *****************  Version 34  *****************
 * User: Bernard      Date: 10/02/05   Time: 12:17p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * pdate version number from 4.3.0.2 to 4.3.0.3 - Done.
 * 
 * *****************  Version 33  *****************
 * User: Bernard      Date: 10/01/29   Time: 3:25p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * Update version number from 4.3.0.1 to 4.3.0.2 - Done.
 * 
 * *****************  Version 32  *****************
 * User: Bernard      Date: 09/12/07   Time: 6:13p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * Update version number from 4.3.0.0 to 4.3.0.1 - Done.
 * 
 * *****************  Version 30  *****************
 * User: Bernard      Date: 09/06/11   Time: 5:56p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * FEATURE: Add two API functions, JPEGEnc_StartOneFrame and
 * JPEGEnc_WaitOneFrameComplete, for noncircular mode. - DONE.
 * 
 * *****************  Version 29  *****************
 * User: Bernard      Date: 09/06/11   Time: 10:55a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * Update version number from 4.2.0.3 to 4.3.0.0 - Done.
 * 
 * *****************  Version 28  *****************
 * User: Bernard      Date: 09/06/10   Time: 9:57a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * FEATURE: Modify the setting of bus number for Mozart. - DONE.
 * 
 * *****************  Version 27  *****************
 * User: Bernard      Date: 09/02/27   Time: 3:54p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * Update version number from 4.2.0.2 to 4.2.0.3 - Done.
 * 
 * *****************  Version 26  *****************
 * User: Bernard      Date: 09/02/27   Time: 3:53p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * FEATURE: Remove video_error.h - DONE.
 * 
 * *****************  Version 25  *****************
 * User: Bernard      Date: 09/01/08   Time: 12:34p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * Update version number from 4.2.0.1 to 4.2.0.2 - Done.
 * 
 * *****************  Version 24  *****************
 * User: Bernard      Date: 09/01/08   Time: 12:33p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * 
 * *****************  Version 23  *****************
 * User: Bernard      Date: 08/09/18   Time: 1:35p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * Update version number from 4.2.0.0 to 4.2.0.1 - Done.
 * 
 * *****************  Version 22  *****************
 * User: Bernard      Date: 08/09/18   Time: 1:35p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * FEATURE: Remove JPEGENC_SINGLE_DRAM_EN in JEPGEnc.h to enable single
 * DRAM usage instead of dual DRAM usage. - DONE.
 * 
 * *****************  Version 21  *****************
 * User: Bernard      Date: 08/09/18   Time: 11:29a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * Update version number from 4.1.0.0 to 4.2.0.0 - Done.
 * 
 * *****************  Version 20  *****************
 * User: Bernard      Date: 08/09/18   Time: 11:26a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * FEATURE: Add macro JPEGENC_SINGLE_DRAM_EN in JEPGEnc.h to enable single
 * DRAM usage instead of dual DRAM usage. - DONE.
 * 
 * *****************  Version 19  *****************
 * User: Bernard      Date: 08/09/09   Time: 8:42p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * Update version number from 4.0.0.1 to 4.1.0.0 - Done.
 * 
 * *****************  Version 18  *****************
 * User: Bernard      Date: 08/08/22   Time: 2:09p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * Update version number from 4.0.0.0 to 4.0.0.1 - Done.
 * 
 * *****************  Version 17  *****************
 * User: Bernard      Date: 08/07/15   Time: 2:41p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * Update version number from 3.0.0.1 to 4.0.0.0 - Done.
 * 
 * *****************  Version 16  *****************
 * User: Bernard      Date: 08/07/15   Time: 2:40p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * Synchronize with JEBE Devices driver 5.0.0.0 - Done.
 * 
 * *****************  Version 15  *****************
 * User: Bernard      Date: 08/07/02   Time: 6:02p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * UPDATE: Update version number from 3.0.0.0 to 3.0.0.1 - DONE.
 * 
 * *****************  Version 14  *****************
 * User: Bernard      Date: 08/03/18   Time: 12:15p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * UPDATE: Update version number from 2.2.0.2 to 3.0.0.0 - DONE.
 * 
 * *****************  Version 13  *****************
 * User: Bernard      Date: 08/03/18   Time: 12:13p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * MODIFICATION: Update header description for SourceSafe - DONE.
 * 
 * *****************  Version 12  *****************
 * User: Bernard      Date: 08/03/18   Time: 11:59a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * INTERFACE: Rename JPEGENC_RAW_BUS_NUM to JPEGENC_IN_BUS_NUM and
 * JPEGENC_BS_BUS_NUM to JPEGENC_OUT_BUS_NUM. - DONE.
 * 
 * *****************  Version 11  *****************
 * User: Bernard      Date: 08/01/10   Time: 7:52p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * UPDATE: Update version number from 2.2.0.1 to 2.2.0.2 - DONE.
 * 
 * *****************  Version 10  *****************
 * User: Bernard      Date: 07/12/10   Time: 9:56p
 * Updated in $/rd_2/Training/SoC_Development/Bernard/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * UPDATE: Update version number from 2.2.0.0 to 2.2.0.1 - DONE.
 * 
 * *****************  Version 9  *****************
 * User: Bernard      Date: 07/11/22   Time: 9:45a
 * Updated in $/rd_2/Training/SoC_Development/Bernard/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * UPDATE: Update version number from 2.1.0.3 to 2.2.0.0 - DONE.
 * 
 * *****************  Version 8  *****************
 * User: Bernard      Date: 07/11/08   Time: 12:16p
 * Updated in $/rd_2/Training/SoC_Development/Bernard/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * UPDATE: Update version number from 2.1.0.2 to 2.1.0.3 - DONE.
 * 
 * *****************  Version 7  *****************
 * User: Bernard      Date: 07/11/08   Time: 9:58a
 * Updated in $/rd_2/Training/SoC_Development/Bernard/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * UPDATE: Update version number from 2.1.0.1 to 2.1.0.2 - DONE.
 * 
 * *****************  Version 6  *****************
 * User: Bernard      Date: 07/10/19   Time: 5:35p
 * Updated in $/rd_2/Training/SoC_Development/Bernard/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * UPDATE: Update version number from 2.1.0.0 to 2.1.0.1 - DONE.
 * 
 * *****************  Version 5  *****************
 * User: Bernard      Date: 07/10/17   Time: 11:20p
 * Updated in $/rd_2/Training/SoC_Development/Bernard/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * UPDATE: Update version number from 2.0.0.0 to 2.1.0.0 - DONE.
 * 
 * *****************  Version 4  *****************
 * User: Bernard      Date: 07/10/12   Time: 1:34p
 * Updated in $/rd_2/Training/SoC_Development/Bernard/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * UPDATE: Update version number from 1.0.0.0 to 2.0.0.0 - DONE.
 * 
 * *****************  Version 3  *****************
 * User: Bernard      Date: 07/09/20   Time: 3:01p
 * Updated in $/rd_2/Training/SoC_Development/Bernard/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * FEATURE: Support slice based circular mode encoding process - Done.
 * 
 * *****************  Version 2  *****************
 * User: Bernard      Date: 07/06/06   Time: 7:58p
 * Updated in $/rd_2/Training/SoC_Development/Bernard/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * 
 * *****************  Version 1  *****************
 * User: Bernard      Date: 06/12/11   Time: 9:00p
 * Created in $/rd_2/Training/SoC_Development/Bernard/SoC/Linux_Libraries/JPEGEnc/JPEGEnc_Lib
 * 
 */

/* ============================================================================================== */
#ifndef __JPEGENC_H__
#define __JPEGENC_H__

/* ============================================================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "errordef.h"
/* Version 4.2.0.2 modification, 2009.01.08 */
/* ======================================== */
#include "global_codec.h"

/* ============================================================================================== */
#define JPEGENC_VERSION MAKEFOURCC(4, 4, 0, 3)
/* Version 4.2.0.3 modification, 2009.02.27 */
#define JPEGENC_ID_VERSION "4.4.0.3" 
/* ======================================== */

/* ============================================================================================== */
/* Version 4.2.0.1 modification, 2008.09.18 */
/* ======================================== */

/* ============================================================================================== */
/* Version 3.0.0.0 modification, 2008.03.17 */
/* Version 4.2.0.1 modification, 2008.09.18 */
#define JPEGENC_IN_BUS_NUM	1
#define JPEGENC_OUT_BUS_NUM 0
/* ======================================== */
/* ======================================== */
 
/* ============================================================================================== */
/* Version 4.4.0.0 modification, 2010.06.07 */
typedef enum jpeg_enc_q_table_type
{
    JPEGENC_Q_TABLE_TYPE_STANDARD = 1,
    JPEGENC_Q_TABLE_TYPE_PELI = 2,
    JPEGENC_Q_TABLE_TYPE_LEE = 3,
	JPEGENC_Q_TABLE_TYPE_CUSTOM_YCBCR = 4,
	JPEGENC_Q_TABLE_TYPE_CUSTOM_Y = 5,
	JPEGENC_Q_TABLE_TYPE_CUSTOM_CBCR = 6
} EJPEGEncQTableType;
/* ======================================== */

/* ============================================================================================== */
typedef struct jpeg_enc_init_options
{
    DWORD dwVersion;
    DWORD dwInWidth;
    DWORD dwInHeight;
    DWORD dwEncWidth;
    DWORD dwEncHeight;
    DWORD dwInHorzOffset;
    DWORD dwInVertOffset;
/* Version 2.0.0.0 modification, 2007.09.12 */
/* ======================================== */
    DWORD dwQualityFactor;
    DWORD dwRstIntNum; /* Block number in one image packet, 0 means disable. */

    BOOL bJFIFHdr;
    BOOL bHighCompression; /* Enable adaptive huffman coding. */

    EJPEGEncQTableType eQTableType;
    EColorFlags eColorFlags; 
    EPixelFormatFlags ePixelFormatFlags;
    
    /* For PB engine initialization */
    DWORD dwBSBufSize;  /* PB engine buffer size */
    DWORD dwTimeOut;        /* Loop checking time */
    DWORD dwSemaphore;
    DWORD dwUserData;

    EBufTypeFlags eBufTypeFlags;
    FOnSendData pfnSend;
    FOnEnterCriticalSec pfnEnter;
    FOnLeaveCriticalSec pfnLeave;
/* Version 2.0.0.0 modification, 2007.09.12 */
	HANDLE hAppSendObject;
/* ======================================== */

    BYTE *pbyYFrame;
    BYTE *pbyCbFrame;
    BYTE *pbyCrFrame;
    BYTE *pbyUserData;       // system must guarantee the size of the decoded user data buffer
    void *pObjectMem;
} TJPEGEncInitOptions;

typedef struct jpeg_enc_state
{
    DWORD dwUserDataLength;
    DWORD dwEncSize;
} TJPEGEncState;

typedef struct jpeg_enc_options
{
    EVideoOptionFlags eOptionFlags;
    DWORD adwUserData[3];
} TJPEGEncOptions;

/* ============================================================================================== */
#ifdef __cplusplus
extern "C" {
#endif

/* Main encoding process functions */
SCODE JPEGEnc_Initial(HANDLE *phObject, TJPEGEncInitOptions *ptInitOptions);
SCODE JPEGEnc_OneFrame(HANDLE hObject, TJPEGEncState *ptState);
/* Version 4.3.0.0 modification, 2009.06.11 */
SCODE JPEGEnc_StartOneFrame(HANDLE hObject, TJPEGEncState *ptState);
SCODE JPEGEnc_WaitOneFrameComplete(HANDLE hObject, TJPEGEncState *ptState);
/* ======================================== */
SCODE JPEGEnc_Release(HANDLE *phObject);

/* This function can't be called simultaneously with the main encoding process functions. */
SCODE JPEGEnc_SetOptions(HANDLE hObject, TJPEGEncOptions *ptEncOptions);

/* These functions can be called simultaneously with the main encoding process functions. */
SCODE JPEGEnc_TakeData(HANDLE hObject, BYTE *pbyBuf, DWORD dwNumBytes);
SCODE JPEGEnc_Quit(HANDLE hObject);
DWORD JPEGEnc_QueryFilledBufferSpace(HANDLE hObject);
DWORD JPEGEnc_QueryMemSize(TJPEGEncInitOptions *ptInitOptions);

SCODE JPEGEnc_GetVersionInfo(BYTE *pbyMajor, BYTE *pbyMinor, BYTE *pbyBuild, BYTE *pbyRevision);

#ifdef __cplusplus
}
#endif

/* ============================================================================================== */
#endif  //__JPEGENC_H__
