/*
 * $Header: /rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib/VideoDisp.h 29    13/01/15 6:52p Vincent $
 *
 * Copyright 2008 VN Inc. All rights reserved.
 *
 * Description:
 *
 * $History: VideoDisp.h $
 * 
 * *****************  Version 29  *****************
 * User: Vincent      Date: 13/01/15   Time: 6:52p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 * FEATURE: Replace S_FAIL with error codes and refine the error
 * description for what-if analysis in the application - DONE.
 *
 * *****************  Version 28  *****************
 * User: Vincent      Date: 12/10/15   Time: 4:18p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 * 1. FEATURE: Synchronize with VPL_VOC device driver version 6.0.0.0
 * 2. FEATURE: Add VideoDisp_QueueBuf() and VideoDisp_DeQueueBuf() to
 * support dynamically feed display buffer address
 *
 * *****************  Version 27  *****************
 * User: Vincent      Date: 12/07/10   Time: 8:26p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 26  *****************
 * User: Alan         Date: 12/06/26   Time: 1:43p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 * MODIFICATION: Change version from 4.1.0.0 to 4.0.0.3 - DONE.
 *
 * *****************  Version 25  *****************
 * User: Bernard      Date: 12/02/07   Time: 7:33p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 * Update version from 4.0.0.2 to version 4.1.0.0 - Done.
 *
 * *****************  Version 24  *****************
 * User: Evelyn       Date: 11/11/29   Time: 4:00p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 23  *****************
 * User: Evelyn       Date: 11/09/22   Time: 5:51p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 22  *****************
 * User: Evelyn       Date: 11/09/14   Time: 6:22p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 21  *****************
 * User: Vincent      Date: 11/08/31   Time: 2:58p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 * 1. FEATURE: Synchronize with VPL_VOC device driver version 4.3.0.0.
 * 2. MODIFICATION: Support 16-bit YUV with embedded SYNC - DONE.
 *
 * *****************  Version 20  *****************
 * User: Alan         Date: 11/07/14   Time: 3:01p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 * UPDATE: Update version number from 3.3.0.0 to 3.4.0.0 - DONE.
 *
 * *****************  Version 19  *****************
 * User: Alan         Date: 11/07/06   Time: 11:59a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 * UPDATE: Update version number from 3.2.0.0 to 3.3.0.0 - DONE.
 *
 * *****************  Version 18  *****************
 * User: Alan         Date: 11/07/01   Time: 11:15a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 17  *****************
 * User: Albert.shen  Date: 11/03/08   Time: 11:40a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 16  *****************
 * User: Albert.shen  Date: 11/03/07   Time: 9:04p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 15  *****************
 * User: Albert.shen  Date: 11/01/26   Time: 1:52p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 14  *****************
 * User: Albert.shen  Date: 11/01/14   Time: 6:17p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 13  *****************
 * User: Albert.shen  Date: 11/01/12   Time: 5:39p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 12  *****************
 * User: Albert.shen  Date: 11/01/02   Time: 10:58p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 11  *****************
 * User: Albert.shen  Date: 10/09/17   Time: 4:05p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 9  *****************
 * User: Albert.shen  Date: 10/06/07   Time: 3:33p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 8  *****************
 * User: Albert.shen  Date: 10/04/29   Time: 12:31p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 7  *****************
 * User: Albert.shen  Date: 10/04/26   Time: 11:35a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 6  *****************
 * User: Albert.shen  Date: 10/03/10   Time: 4:52p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 5  *****************
 * User: Albert.shen  Date: 10/03/10   Time: 3:50p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 4  *****************
 * User: Albert.shen  Date: 10/02/26   Time: 2:43p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 3  *****************
 * User: Albert.shen  Date: 09/08/11   Time: 9:23a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 * *****************  Version 2  *****************
 * User: Albert.shen  Date: 08/12/05   Time: 9:28p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 *
 *
 *
 */

/* ============================================================================================== */
#ifndef __VIDEODISP_H__
#define __VIDEODISP_H__

/* =========================================================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "global_codec.h"
#include "typedef.h"

/* =========================================================================================== */
#define VIDEODISP_VERSION MAKEFOURCC(5, 0, 0, 1)
#define VIDEODISP_ID_VERSION "5.0.0.1"

/* =========================================================================================== */
#define VIDEODISP_IN_BUS_NUM    1

/* =========================================================================================== */
typedef struct video_display_init_options
{
    DWORD dwVersion;
    void *pObjectMem;
    EPixelFormatFlags epixelInFormat;
    EVideoSignalFormat eOutFormat;
    EVideoSignalSize eOutputSize;
/* Version 3.0.0.0 modification, 2010.04.28 */
    EVideoSignalFrequency eOutputRefreshRate;
/* ======================================== */
    DWORD dwMaxInWidth;
    DWORD dwMaxInHeight;
    DWORD dwInWidth;
    DWORD dwInHeight;
/* Version 3.0.0.0 modification, 2010.04.28 */
    BOOL  bPALEn;
    BYTE *pbyInBuffer;
/* ======================================== */

    SDWORD sdwBrightness;
    SDWORD sdwContrast;
    DWORD dwSaturation;

} TVideoDispInitOptions;

typedef struct video_display_options
{
    EVideoDispOptionFlags eOptionFlags;
    DWORD adwUserData[6];
} TVideoDispOptions;

typedef struct video_display_state
{
    DWORD dwIndex;
    DWORD dwFrameCount;
    BYTE *pbyYFrame;
    BYTE *pbyCbFrame;
    BYTE *pbyCrFrame;

} TVideoDispState;

/* Version 3.0.0.0 modification, 2010.04.28 */
typedef struct video_display_output_timing_control
{
    DWORD dwHsyncTotalLength;
    DWORD dwVsyncTotalLength;
    DWORD dwVsyncFrontPorch;
    DWORD dwVsyncBackPorch;
    DWORD dwHsyncFrontPorch;
    DWORD dwHsyncBackPorch;
    DWORD dwRatio;
    DWORD dwDivider;

} TVideoDispOutCtrl;
/* ======================================== */

/* =========================================================================================== */
SCODE VideoDisp_Initial(HANDLE *phObject, TVideoDispInitOptions *ptInitOptions);
SCODE VideoDisp_Release(HANDLE *phObject);
void VideoDisp_Start(HANDLE hObject);
/* Version 3.3.0.0 modification, 2011.07.05 */
void VideoDisp_WaitComplete(HANDLE hObject);
/* ======================================== */
void VideoDisp_Stop(HANDLE hObject);
SCODE VideoDisp_SetOptions(HANDLE hObject, TVideoDispOptions *);

SCODE VideoDisp_Get_Buf(HANDLE hObject, TVideoDispState *);
SCODE VideoDisp_Release_Buf(HANDLE hObject, TVideoDispState *);
DWORD VideoDisp_QueryMemSize(TVideoDispInitOptions *);
SCODE VideoDisp_GetVersionInfo(BYTE *pbyMajor, BYTE *pbyMinor, BYTE *pbyBuild, BYTE *pbyRevision);
/* Version 5.0.0.0 modification, 2012.10.15 */
SCODE VideoDisp_QueueBuf(HANDLE hObject, TVideoDispState *ptState);
SCODE VideoDisp_DeQueueBuf(HANDLE hObject, TVideoDispState *ptState);
/* ======================================== */
/* =========================================================================================== */
#endif //__VIDEODISP_H__
