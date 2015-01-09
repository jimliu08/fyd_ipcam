/*
 *******************************************************************************
 * $Header: /RD_1/Project/VVTK1000_Linux/warden/apps/webcam/video/videocomm/src/videocomm.c 1     05/08/02 15:03 Cchuang $
 *
 *  Copyright (c) 2000-2002 VN Inc. All rights reserved.
 *
 *  +-----------------------------------------------------------------+
 *  | THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY ONLY BE USED |
 *  | AND COPIED IN ACCORDANCE WITH THE TERMS AND CONDITIONS OF SUCH  |
 *  | A LICENSE AND WITH THE INCLUSION OF THE THIS COPY RIGHT NOTICE. |
 *  | THIS SOFTWARE OR ANY OTHER COPIES OF THIS SOFTWARE MAY NOT BE   |
 *  | PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY OTHER PERSON. THE   |
 *  | OWNERSHIP AND TITLE OF THIS SOFTWARE IS NOT TRANSFERRED.        |
 *  |                                                                 |
 *  | THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT   |
 *  | ANY PRIOR NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY |
 *  | VN INC.                                                    |
 *  +-----------------------------------------------------------------+
 *
 * $History: videocomm.c $
 * 
 * *****************  Version 1  *****************
 * User: Cchuang      Date: 05/08/02   Time: 15:03
 * Created in $/RD_1/Project/VVTK1000_Linux/warden/apps/webcam/video/videocomm/src
 * 
 * *****************  Version 2  *****************
 * User: Cchuang      Date: 05/06/30   Time: 16:45
 * Updated in $/RD_1/Project/VVTK1000/Warden/apps/webcam/video/videocomm/src
 * 
 * *****************  Version 1  *****************
 * User: Cchuang      Date: 05/02/03   Time: 18:15
 * Created in $/RD_1/Project/VVTK1000/Warden/apps/webcam/VIDEO/videocomm/src
 * Warden Project
 * 
 * *****************  Version 2  *****************
 * User: Yun          Date: 04/04/19   Time: 12:51p
 * Updated in $/RD_1/Project/PNX1300_PSOS/Farseer/video/videocomm/src
 * Modify the field width of VGA and PVGA
 * 
 * *****************  Version 2  *****************
 * User: Joe          Date: 03/08/08   Time: 11:11a
 * Updated in $/rd_1/Project/TM1300_PSOS/FarSeer/video/videocomm/src
 * add progressive fromat
 * 
 * *****************  Version 1  *****************
 * User: Yun          Date: 03/04/16   Time: 2:27p
 * Created in $/rd_1/Project/TM1300_PSOS/FarSeer/video/videocomm/src
 * 
 * *****************  Version 2  *****************
 * User: Joe          Date: 03/03/18   Time: 5:07p
 * Updated in $/rd_1/Project/TM1300_PSOS/FarSeer/video/videoio/src
 * 1. Add bInvertField option in videoin module
 * 2. Add bMergeFieldToYUV420 in videoin module
 * 3. Change version to 1.0.0.3
 * 
 * *****************  Version 1  *****************
 * User: Joe          Date: 03/02/27   Time: 5:57p
 * Created in $/rd_1/System/FIRMWARE/VIDEOIO/src
 * First check in VideoIO module
 *
 *******************************************************************************
 */

/*!
 *******************************************************************************
 * Copyright 2000-2002 VN, Inc. All rights reserved.
 *
 * \file
 * videocomm.c
 *
 * \brief
 * Common global variables for video library group
 *
 * \date
 * 2002/11/29
 *
 * \author
 * Joe Wu
 *
 *
 *******************************************************************************
 */

#include "typedef.h"
#include "errordef.h"
#include "videocomm.h"
#include <stdio.h>
//! video information for each format
#ifndef _LINUX
/*const*/ TVideoFormatInfo g_avfInfo[vfNum] = {
					 {256, 120, 176, 120, 1, 30, 176 * 120},	// QSIF
					 {384, 240, 352, 240, 1, 30, 352 * 240},	// SIF
					 {768, 240, 704, 240, 2, 30, 704 * 240},	// 4SIF
					 {768, 480, 704, 480, 1, 30, 704 * 480},	// P4SIF
					 {128,  60,  88,  60, 1, 30,  88 *  60},	// MSIF

				 	 {256, 144, 176, 144, 1, 24, 176 * 144},	// QCIF
				 	 {384, 288, 352, 288, 1, 24, 352 * 288},	// CIF
					 {768, 288, 704, 288, 2, 24, 704 * 288},	// 4CIF
					 {768, 576, 704, 576, 1, 30, 704 * 576},	// P4CIF
					 {128,  60,  88,  72, 1, 24,  88 *  72},	// MCIF

				 	 {256, 120, 160, 120, 1, 30, 160 * 120},	// QMGA
				 	 {384, 240, 320, 240, 1, 30, 320 * 240},	// MGA
					 {768, 240, 640, 240, 2, 30, 640 * 240},	// VGA
					 {768, 480, 640, 480, 1, 30, 640 * 480},	// PVGA
					 {128,  60,  80,  60, 1, 30,  80 *  60},	// MVGA
				 	 };
#else // _LINUX
/*const*/ TVideoFormatInfo g_avfInfo[vfNum] = {
	{256, 120, 176, 120, 176, 120,   0,   0, 1, 30, 176 * 120},	// QSIF
	{384, 240, 352, 240, 352, 240,   0,   0, 1, 30, 352 * 240},	// SIF
	{768, 240, 704, 240, 704, 240,   0,   0, 2, 30, 704 * 240},	// 4SIF
	{768, 480, 704, 480, 704, 480,   0,   0, 1, 30, 704 * 480},	// P4SIF
	{128,  60,  88,  60,  88,  60,   0,   0, 1, 30,  88 *  60},	// MSIF

	{256, 144, 176, 144, 176, 144,   0,   0, 1, 24, 176 * 144},	// QCIF
	{384, 288, 352, 288, 352, 288,   0,   0, 1, 24, 352 * 288},	// CIF
	{768, 288, 704, 288, 704, 288,   0,   0, 2, 24, 704 * 288},	// 4CIF
	{768, 576, 704, 576, 704, 576,   0,   0, 1, 30, 704 * 576},	// P4CIF
	{128,  60,  88,  72,  88,  72,   0,   0, 1, 24,  88 *  72},	// MCIF

	{256, 120, 160, 120, 160, 120,   0,   0, 1, 30, 160 * 120},	// QMGA
	{384, 240, 320, 240, 320, 240,   0,   0, 1, 30, 320 * 240},	// MGA
	{768, 240, 640, 240, 640, 240,   0,   0, 2, 30, 640 * 240},	// VGA
	{768, 480, 640, 480, 640, 480,   0,   0, 1, 30, 640 * 480},	// PVGA
	{128,  60,  80,  60,  80,  60,   0,   0, 1, 30,  80 *  60},	// MVGA

	{  0,   0, 800, 600, 800, 600,   0,   0, 1, 30, 800 * 600},	// SVGA
	{  0,   0, 960, 720, 960, 720,   0,   0, 1, 30, 960 * 720},	
	{  0,   0,1024, 768,1024, 768,   0,   0, 1, 30,1024 * 768},	
	{  0,   0,1008,1008,1008,1008,   0,   0, 1, 30,1008 *1008},	
	{  0,   0,1280,1024,1280,1024,   0,   0, 1, 30,1280 *1024},	// SXGA

	{  0,   0,1600,1200,1600,1200,   0,   0, 1, 30,1600 *1200},	// UXGA
	{  0,   0,2560,1920,2560,1920,   0,   0, 1, 30,2560 *1920},	// UXGA
	{  0,   0,1280, 720,1280, 720,   0,   0, 1, 30,1280 * 720},	// vf720p
	{  0,   0,1280, 960,1280, 960,   0,   0, 1, 30,1280 * 960},	// vfxvga
	{  0,   0,1280, 800,1280, 800,   0,   0, 1, 30,1280 * 800},	// wxga
	{  0,   0, 720, 480, 720, 480,   0,   0, 1, 30, 720 * 480},	// NTSC
	{  0,   0,1920,1080,1920,1080,   0,   0, 1, 30,1920 *1080},	// HD1080
	{  0,   0, 480, 480, 480, 480,   0,   0, 1, 30, 480 * 480},	// vfTest
	{  0,   0, 432, 480, 432, 480,   0,   0, 1, 30, 432 * 480},	// vfTest2
	{  0,   0, 240, 160, 240, 160,   0,   0, 1, 30, 240 * 160},	// vfTest3
	{  0,   0, 432, 240, 432, 240,   0,   0, 1, 30, 432 * 240},	// vfTest4
	{  0,   0, 360, 480, 360, 480,   0,   0, 1, 30, 360 * 480},	// vfTest5
	{  0,   0, 720, 576, 720, 576,   0,   0, 1, 30, 720 * 576},	//PAL
	{  0,   0, 638, 480, 638, 480,   0,   0, 1, 30, 638 * 480},    //vfTest6
	{  0,   0, 720, 405, 720, 405,   0,   0, 1, 30, 720 * 405}, 	//vfTest7
	{  0,   0, 180, 120, 180, 120,   0,   0, 1, 30, 180 * 120}, 	//vfTest8
	{  0,   0, 360, 240, 360, 240,   0,   0, 1, 30, 360 * 240}, 	//vfTest9
	{  0,   0,2048,1536,2048,1536,   0,   0, 1, 30,2048 *1536},	//vfTest10
	{  0,   0,   0,   0,   0,   0,   0,   0, 1, 30,       0*0},     //vfUserDef1
	{  0,   0,   0,   0,   0,   0,   0,   0, 1, 30,       0*0},     //vfUserDef2
	{  0,   0,   0,   0,   0,   0,   0,   0, 1, 30,       0*0},     //vfUserDef3
	{  0,   0,   0,   0,   0,   0,   0,   0, 1, 30,       0*0},     //vfUserDef4
	{  0,   0,   0,   0,   0,   0,   0,   0, 1, 30,       0*0},     //vfUserDef5
	{  0,   0,   0,   0,   0,   0,   0,   0, 1, 30,       0*0},     //vfUserDef6
	{  0,   0,   0,   0,   0,   0,   0,   0, 1, 30,       0*0},     //vfUserDef7
	{  0,   0,   0,   0,   0,   0,   0,   0, 1, 30,       0*0},     //vfUserDef8
	{  0,   0,   0,   0,   0,   0,   0,   0, 1, 30,       0*0},     //vfUserDef9
	{  0,   0,   0,   0,   0,   0,   0,   0, 1, 30,       0*0},     //vfUserDef10
	{  0,   0,   0,   0,   0,   0,   0,   0, 1, 30,       0*0},     //vfUserDef11
	{  0,   0,   0,   0,   0,   0,   0,   0, 1, 30,       0*0},     //vfUserDef12
	};
#endif //_LINUX

    

//---------------------------------------------------------------------------

EVideoFormat VideoComm_GetFormatFromWH(DWORD dwWidth, DWORD dwHeight)
{
	int	i;
	static int	iLast = 0;
	DWORD	dwEntrys;
	
	if ((g_avfInfo[iLast].wVideoWidth == dwWidth) 
	 && (g_avfInfo[iLast].wVideoHeight == dwHeight))
	{
		return iLast;
	}
	
	//printf("%s:%d: Find (%u, %u), ", __FILE__, __LINE__, dwWidth, dwHeight);
	dwEntrys = sizeof(g_avfInfo) / sizeof(TVideoFormatInfo);
	
	for (i = 0; i < dwEntrys; i ++)
	{
		if ((g_avfInfo[i].wVideoWidth == dwWidth) 
		 && (g_avfInfo[i].wVideoHeight == dwHeight))
		{
			break;
		}
	}
	iLast = i;
	
	//printf("( %d, %d)\n", g_avfInfo[i].wVideoWidth, g_avfInfo[i].wVideoHeight);
	return i;
}

SCODE	VideoComm_ChkFormatByWH(EVideoFormat vFormat, DWORD dwWidth, DWORD dwHeigth)
{
	if (g_avfInfo[vFormat].wVideoWidth != dwWidth) return S_FAIL;
	if (g_avfInfo[vFormat].wVideoHeight != dwHeigth) return S_FAIL;
	return S_OK;
}

EVideoFormat VideoComm_WH2vFormat(DWORD dwWidth, DWORD dwHeight)
{
	int	i;
	for (i = 0; i < vfNum; i ++)
	{
		if ((g_avfInfo[i].wEncWidth == dwWidth) 
		 && (g_avfInfo[i].wEncHeight == dwHeight))
		{
			break;
		}
	}
	return i;
}

