/*
 * $Header: /rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib/ImgBackEndProcess.h 26    13/08/26 12:34a Linus.huang $
 *
 * Copyright 2010-2011 VN INC.. All rights reserved.
 *
 * Description:
 *
 * $History: ImgBackEndProcess.h $
 * 
 * *****************  Version 26  *****************
 * User: Linus.huang  Date: 13/08/26   Time: 12:34a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 25  *****************
 * User: Linus.huang  Date: 13/08/19   Time: 5:42p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 24  *****************
 * User: Linus.huang  Date: 13/08/07   Time: 9:21p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 23  *****************
 * User: Linus.huang  Date: 13/05/28   Time: 12:51a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 22  *****************
 * User: Linus.huang  Date: 13/04/01   Time: 10:01p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 21  *****************
 * User: Linus.huang  Date: 13/03/13   Time: 4:10p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 20  *****************
 * User: Linus.huang  Date: 13/01/18   Time: 1:58a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 19  *****************
 * User: Linus.huang  Date: 13/01/05   Time: 8:14p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 18  *****************
 * User: Linus.huang  Date: 13/01/03   Time: 1:16p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 17  *****************
 * User: Linus.huang  Date: 12/12/03   Time: 5:22p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 16  *****************
 * User: Linus.huang  Date: 12/11/08   Time: 5:20p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 15  *****************
 * User: Linus.huang  Date: 12/11/08   Time: 3:08p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 13  *****************
 * User: Linus.huang  Date: 12/10/16   Time: 2:16a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 12  *****************
 * User: Linus.huang  Date: 12/10/15   Time: 8:02p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 11  *****************
 * User: Linus.huang  Date: 12/10/12   Time: 5:36p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 10  *****************
 * User: Linus.huang  Date: 12/10/09   Time: 12:06p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 9  *****************
 * User: Linus.huang  Date: 12/10/02   Time: 2:52p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 8  *****************
 * User: Linus.huang  Date: 12/09/17   Time: 8:32p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 7  *****************
 * User: Linus.huang  Date: 12/08/16   Time: 4:03p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 6  *****************
 * User: Linus.huang  Date: 12/08/09   Time: 2:07p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 5  *****************
 * User: Linus.huang  Date: 12/08/09   Time: 10:59a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 4  *****************
 * User: Linus.huang  Date: 12/08/09   Time: 10:37a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 3  *****************
 * User: Linus.huang  Date: 12/08/09   Time: 8:43a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 2  *****************
 * User: Linus.huang  Date: 12/07/30   Time: 4:36p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 1  *****************
 * User: Linus.huang  Date: 12/07/02   Time: 7:49p
 * Created in $/rd_2/project/Mozart/Linux_Libraries/ImgBackEndProcess_3.0.0.0/ImgBackEndProcess_Lib
 * 
 * *****************  Version 41  *****************
 * User: Linus.huang  Date: 11/04/12   Time: 1:39p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/ImgBackEndProcess/ImgBackEndProcess_Lib
 * 
 * *****************  Version 40  *****************
 * User: Linus.huang  Date: 11/04/12   Time: 1:24p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/ImgBackEndProcess/ImgBackEndProcess_Lib
 * 
 * *****************  Version 39  *****************
 * User: Linus.huang  Date: 11/04/12   Time: 1:20p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/ImgBackEndProcess/ImgBackEndProcess_Lib
 * 
 * *****************  Version 38  *****************
 * User: Linus.huang  Date: 11/03/31   Time: 1:20p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/ImgBackEndProcess/ImgBackEndProcess_Lib
 * 
 * *****************  Version 37  *****************
 * User: Linus.huang  Date: 11/02/16   Time: 11:58a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/ImgBackEndProcess/ImgBackEndProcess_Lib
 * 
 * *****************  Version 36  *****************
 * User: Linus.huang  Date: 11/02/09   Time: 5:11p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/ImgBackEndProcess/ImgBackEndProcess_Lib
 * 
 */

/* ============================================================================================== */
#ifndef __IMGBACKENDPROCESS_H__
#define __IMGBACKENDPROCESS_H__

/* ============================================================================================= */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typedef.h"
#include "errordef.h"
#include "global_codec.h"

//#define _MD_DEBUG_
//#define _DRAW_WINDOW_
//#define _LOOP_TEST_

/* ============================================================================================= */
#define IMGBACKENDPROCESS_VERSION MAKEFOURCC(11, 0, 0, 1)

#define IMGBACKENDPROCESS_ID_VERSION "11.0.0.1"

/* ============================================================================================= */

// IBPE support 2 bus transfer: 1 or 2
#define IMGBACKENDPROCESS_IN_BUS_NUM	1
#define IMGBACKENDPROCESS_OUT_BUS_NUM	1
#define IMGBACKENDPROCESS_MAP_BUS_NUM	1
#define IMGBACKENDPROCESS_REF_BUS_NUM	1

// define the external DRAM number: 0 ~ 3
#define IMGBACKENDPROCESS_IN_MEM_RANK_NUM		0
#define IMGBACKENDPROCESS_OUT_MEM_RANK_NUM		0
#define IMGBACKENDPROCESS_MAP_MEM_RANK_NUM		0
#define IMGBACKENDPROCESS_REF_MEM_RANK_NUM		0

// Maximum motion detection window number
#define MOTION_DTCT_MAX_WIN_NUM 16
/* ============================================================================================= */

/* Version 8.0.0.0 modification, 2012.08.06 */
typedef enum imgbackendprocess_deinterlace_mode
{
	IMGBACKENDPROCESS_DEINTERLACER_WEAVE = 0,
	IMGBACKENDPROCESS_DEINTERLACER_BLEND = 1,
	IMGBACKENDPROCESS_DEINTERLACER_MOTION_ADAPTIVE_BASIC = 2,
	IMGBACKENDPROCESS_DEINTERLACER_MOTION_ADAPTIVE_ADVANCED = 3
} EImgBackEndProcessDeInterlaceMode;

/* Version 9.0.0.0 modification, 2012.09.17 */
typedef enum imgbackendprocess_interlace_pic_format
{
	IMGBACKENDPROCESS_INTERLACE_FORMAT_INTERLEAVE = 0,	
	IMGBACKENDPROCESS_INTERLACE_FORMAT_PLANAR = 1
} EImgBackEndProcessInterlacePicFormat;
/* ======================================== */

typedef enum imgbackendprocess_noise_reduction_mode
{
	IMGBACKENDPROCESS_NR_NORMAL_2D = 0,	
	IMGBACKENDPROCESS_NR_NORMAL_3D_1_REF = 1,
	IMGBACKENDPROCESS_NR_NORMAL_3D_2_REF = 2,
	IMGBACKENDPROCESS_NR_STRONG = 3,
/* Version 9.0.0.0 modification, 2012.10.01 */
	IMGBACKENDPROCESS_NR_BLEND = 4
/* ======================================== */
} EImgBackEndProcessNoiseReductionMode;

/* Version 10.0.0.1 modification, 2013.01.17 */
/* ========================================= */

typedef struct imgbackendprocess_init_options
{
	DWORD dwVersion;
	void *pObjectMem;
	DWORD dwMaxWindowNum;
	
	/* IBPE_IN_SIZE */
/* Version 9.0.0.0 modification, 2012.09.14 */
	DWORD dwMaxWidth;
	DWORD dwMaxHeight;
/* ======================================== */

	/* IBPE_STRIDE */
	DWORD dwInStride;
	DWORD dwOutStride;
/* Version 8.0.0.0 modification, 2012.08.06 */
	DWORD dwPrivacyMaskMapStride;
/* ======================================== */

	BOOL bInitStaticMapBuff;
/* Version 9.0.0.0 modification, 2012.09.21 */
	BOOL bInitNrStrongBlendBuff;
/* Version 11.0.0.0 modification, 2013.08.23 */
	BOOL bIfNrStrongBlendBuffFormat420;
/* ========================================= */
/* Version 11.0.0.0 modification, 2013.08.20 */
/* ========================================= */

	DWORD dwMapMemRankNum;
	DWORD dwRefMemRankNum;

} TImgBackEndProcessInitOptions;

typedef struct motion_detect_window
{
	DWORD dwSerialNum;
	DWORD dwHorzStart;				// 16-bit: Window start position X
	DWORD dwVertStart;				// 16-bit: Window start position Y
	DWORD dwWidth;					// 16-bit: Window width
	DWORD dwHeight;					// 16-bit: Window height
	BYTE byPixDiffThr;				// 8-bit: Absolute pixel difference threshold for a motion pixel, 0~255
	DWORD dwInMaskPixNum;			// 23-bit: Motion pixels in one window inside privacy mask
	DWORD dwOutMaskPixNum;			// 23-bit: Motion pixels in one window outside privacy mask
	DWORD dwMotionPercentThr;		// 0~100
	BOOL bIsMotion;					// 1-bit: If whole window detects motion
	BOOL bIsOutMaskMotion;			// 1-bit: If the outside-mask region within window detects motion
} TMotionDetectWindow;
/* Version 10.0.0.0 modification, 2012.12.25 */
typedef struct time_stamp
{
	DWORD dwSecond;
	DWORD dwMilliSecond;
	DWORD dwMicroSecond;
} TTimeStamp;
/* ========================================= */
typedef struct imgbackendprocess_buff_info
{
	BYTE *pbyYBuff;
	BYTE *pbyCbBuff;
	BYTE *pbyCrBuff;
	BOOL bIsMotionDetectResultValid;
	TMotionDetectWindow *ptMotionDetectInfo;		// Need size = 16 * sizeof(TMotionDetectWindow)
/* Version 10.0.0.0 modification, 2012.12.26 */
	TTimeStamp tFrameTimeStamp;
	TTimeStamp tMotionDetectTimeStamp;
/* ========================================= */
} TImgBackEndProcessBuffInfo;
/* ======================================== */
/* ============================================================================================== */

typedef struct imgbackendprocesss_state
{	
/* Version 9.0.0.0 modification, 2012.09.17 */
	BOOL bUpdateEn;			// If State config changed or initialization, set TRUE.
	DWORD dwWidth;
	DWORD dwHeight;
/* Version 11.0.0.0 modification, 2013.08.20 */
	EPixelFormatFlags eInPixelFormat;
	EPixelFormatFlags eOutPixelFormat;
/* ========================================= */
/* Version 11.0.0.0 modification, 2013.08.20 */
	DWORD dwInBusNum;
	DWORD dwOutBusNum;
	DWORD dwMapBusNum;
	DWORD dwRefBusNum;
/* ========================================= */
	/* Input & output buffer */
/* Version 9.0.0.0 modification, 2012.09.17 */
	TImgBackEndProcessBuffInfo tCurrInBuff;
	TImgBackEndProcessBuffInfo *ptRefInBuffQueue;
	DWORD dwValidRefBuffNum;

/* Version 10.0.0.0 modification, 2012.12.27 */
	TImgBackEndProcessBuffInfo tOutBuff;
/* ========================================= */
/* ======================================== */

	BOOL bYComponentEn;
	BOOL bCComponentEn;
	BOOL bYOutEn;
	BOOL bCOutEn;

	/* Motion Detection */
	BOOL bMotionDetectEn;
	BOOL bMotionDetectPrivacyMaskEn;

	/* Noise Reduction */
	BOOL bNoiseReductionEn;
	EImgBackEndProcessNoiseReductionMode eNoiseReductionMode;
	DWORD dwNoiseReductionNormalStrength;				//0~200;  15
/* Version 10.0.0.0 modification, 2012.12.26 */
	//DWORD dwNoiseReductionStrongStrength;				//Strong mode:0~30, blend mode: 0~6 
	DWORD dwNoiseReductionStrongRefStrength;
	DWORD dwNoiseReductionStrongCurrWeight;
	//BOOL bNoiseReductionMotionAdaptiveEn;
	BOOL bNoiseReductionStrongMotionAdaptiveEn;
	//DWORD dwNoiseReductionMotionAdaptiveStrength;		//0~2; 2
/* ======================================== */

	/* Edge Enhancement */
	BOOL bEdgeEnhanceEn;
/* Version 10.0.0.0 modification, 2012.12.26 */
	//DWORD dwEdgeEnhanceSensitivity;		//0~4
	DWORD dwEdgeEnhanceStrength;
	DWORD dwEdgeEnhanceClip;
	DWORD dwEdgeEnhanceEdgeSensitivity;		//
	DWORD dwEdgeEnhanceNoiseSensitivity;	//0~63
/* ========================================= */

	/* Giometric Lens Distortion Correction */
	BOOL bGeometricLensDistortionCorrectionEn;

/* Version 9.0.0.0 modification, 2012.09.17 */
	SDWORD sdwGeometricLensDistortionCorrectionYParam1;
	SDWORD sdwGeometricLensDistortionCorrectionYParam2;
	SDWORD sdwGeometricLensDistortionCorrectionYParam3;
	SDWORD sdwGeometricLensDistortionCorrectionCParam1;
	SDWORD sdwGeometricLensDistortionCorrectionCParam2;
	SDWORD sdwGeometricLensDistortionCorrectionCParam3;
/* ======================================== */

	/* De-Interlacing */
	BOOL bDeInterlaceEn;
	EImgBackEndProcessDeInterlaceMode eDeInterlaceMode;
	EImgBackEndProcessInterlacePicFormat eDeInterlacePicFormat;

	/* privacy mask */
/* Version 9.0.0.0 modification, 2012.09.17 */
	BOOL bOutFramePrivacyMaskEn;
	BYTE *pbyPrivacyMaskBuff;
	DWORD dwPrivacyMaskYConstant;
	DWORD dwPrivacyMaskCbConstant;
	DWORD dwPrivacyMaskCrConstant;
/* ======================================== */
/* ======================================== */
} TImgBackEndProcessState;


typedef struct imgbackendprocesss_options
{
	EVideoOptionFlags eOptionFlags;
	PVOID apvUserData[3];
} TImgBackEndProcessOptions;

/* ============================================================================================= */
SCODE ImgBackEndProcess_Initial(HANDLE *phObject, TImgBackEndProcessInitOptions *ptInitOptions);
SCODE ImgBackEndProcess_Release(HANDLE *phObject);

SCODE ImgBackEndProcess_OneFrame(HANDLE hObject, TImgBackEndProcessState *ptState);
SCODE ImgBackEndProcess_StartOneFrame(HANDLE hObject, TImgBackEndProcessState *ptState);
SCODE ImgBackEndProcess_WaitOneFrameComplete(HANDLE hObject, TImgBackEndProcessState *ptState);

/* Version 9.0.0.0 modification, 2011.04.20 */
/* Version 10.0.0.1 modification, 2013.01.17 */
SCODE ImgBackEndProcess_CheckParam(HANDLE hObject, TImgBackEndProcessState *ptState);
/* ========================================= */
/* ======================================== */
DWORD ImgBackEndProcess_QueryMemSize(TImgBackEndProcessInitOptions *ptInitOptions);
SCODE ImgBackEndProcess_GetVersionInfo(BYTE *pbyMajor, BYTE *pbyMinor, BYTE *pbyBuild, BYTE *pbyRevision);

SCODE ImgBackEndProcess_SetOptions(HANDLE hObject, TImgBackEndProcessOptions *ptOptions);
//SCODE ImgBackEndProcess_GetWindowInfo(HANDLE hObject, TMotionDetectWindow *ptUserWindowInfo);

/* ============================================================================================= */
#endif //__ImgBackEndProcess_H__

/* =========================================================================================== */
