/*
 *******************************************************************************
 * $Header: /RD_1/Project/VVTK1000_Linux/warden/apps/webcam/video/videocomm/src/videocomm.h 1     05/08/02 15:03 Cchuang $
 *
 *  Copyright (c) 2000-2003 VN Inc. All rights reserved.
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
 * $History: videocomm.h $
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
 * *****************  Version 4  *****************
 * User: Yun          Date: 04/03/10   Time: 11:21a
 * Updated in $/rd_1/Project/TM1300_PSOS/FarSeer/video/videocomm/src
 * Merge with SOC project
 * 
 * *****************  Version 3  *****************
 * User: Joe          Date: 03/10/23   Time: 5:12p
 * Updated in $/rd_1/Project/TM1300_PSOS/FarSeer/video/videocomm/src
 * Add video color property
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
 *******************************************************************************
 */

/*!
 *******************************************************************************
 * Copyright 2000-2003 VN, Inc. All rights reserved.
 *
 * \file
 * videocomm.h
 *
 * \brief
 * header file of common global variables for video library group
 *
 * \date
 * 2003/03/27
 *
 * \author
 * May Hsu
 *
 *
 *******************************************************************************
 */

#include "typedef.h"
#include "errordef.h"
#include "global_codec.h"

#ifndef __VIDEOCOMM_H__
#define __VIDEOCOMM_H__

//! video standard enumerator
typedef enum {
    /*! Video standard is NTSC */
    vsNTSC  = 0,
    /*! Video standard is PAL */
    vsPAL   = 1
} EVideoStandard;

//! enumerator for video format, vfMxIF => motion detection size
typedef enum  {vfQSIF, vfSIF, vf4SIF, vfP4SIF, vfMSIF,      // NTSC
               vfQCIF, vfCIF, vf4CIF, vfP4CIF, vfMCIF,      // PAL
               vfQMGA, vfMGA, vfVGA, vfPVGA, vfMVGA,        // CMOS
               vfCst0, vfCst1, vfCst2, vfCst3, vfCst4,
               vfUXGA, vf5M, vf720p, vfxvga, vfwxga, vNTSC, vfHD1080, vftest, vftest2, vftest3, vftest4, vftest5,vPAL,vfTest6,vfTest7,vfTest8,vfTest9,vfTest10,vfUserDef1,vfUserDef2,vfUserDef3,vfUserDef4,vfUserDef5,vfUserDef6,vfUserDef7,vfUserDef8,vfUserDef9,vfUserDef10,vfUserDef11,vfUserDef12,
			   vfNum
              } EVideoFormat;

//! YUV format of video buffer enumerator
typedef enum {
    yuvf420,
    yuvf422_Cosited,
    yuvf422_Interspersed
} EVideoYUVFormat;

//! informations of video format structure
typedef struct {
    //! width of field buffer, must be multiple of 128
	WORD  wFieldWidth;
    //! height of field buffer
	WORD  wFieldHeight;
    //! width of video been processed
    WORD  wVideoWidth;
    //! height of video been processed
    WORD  wVideoHeight;
#ifdef _LINUX
    //! width of encoder been processed
    WORD  wEncWidth;
    //! height of encoder been processed
    WORD  wEncHeight;
    //! The offset of encoder at the X-axis
    WORD  wXOffset;
    //! The offset of encoder at the Y-axis
    WORD  wYOffset;
#endif // _LINUX
    //! field number of video
	WORD  wFieldNum;
    //! framerate of video
	WORD  wFrameRate;
    //! size of field in bytes
	DWORD dwFieldBufSize;
} TVideoFormatInfo;

//! video color vector definitions
typedef enum {
	vcBrightness,
	vcContrast,
	vcHue,
	vcSaturation
} TVideoColorVector;

//! video buffer status enumeration, filled with data or empty
typedef enum {
	//! finished, the buffer filled with data
	evsobsFinished = 1,
	//! empty, just return the buffer
	evsobsEmpty
} EVideoScalerBufStatus;

//! video frame buffer structure, output of VideoScaler and input of VideoEncoder
typedef struct {
    //! buffer size of frame
	DWORD				dwBufSize;
	//! point to start of Y frame buffer
	BYTE				*pbyBufY;	
	//! point to start of U frame buffer
	BYTE				*pbyBufU;
	//! point to start of V frame buffer
	BYTE				*pbyBufV;
	//! video format
    EVideoFormat    	vFormat;	
	//! Video in YUV format, can only be yuv420 in the present
	EVideoYUVFormat		yuvFormat;
	//! the status of output buffer, finished or empty
	EVideoScalerBufStatus eBufStatus;
    //! video frame stride
    DWORD				dwStride;   
    //! channel number of this buffer
    DWORD       		dwChNum;
	//! sec of current frame
	DWORD				dwSecond;
	//! millisecond of current frame
	DWORD				dwMilliSecond;
	//! microsecond of current frame
	DWORD		dwMicroSecond;
    //! frame count from start of current frame
	DWORD               dwFrameCount;
    //! dummy data, inorder to let structure 64 bytes aligned
    DWORD               adwDummyAlign[4];
	
	//! The following fields is for recording the timestamp of Mition Detection. 
	//! sec of MD
	DWORD				dwMDSecond;
	//! millisecond of MD
	DWORD				dwMDMilliSecond;
	//! microsecond of MD
	DWORD		dwMDMicroSecond;
} TVideoFrameBuffer;

//---------------------------------------------------------------------------
extern /*const*/ TVideoFormatInfo g_avfInfo[vfNum];
extern const EVideoSignalSize g_VideoFormat_2_VideoSignalSize[vfNum];
extern const EVideoFormat     g_VideoSignalSize_2_VideoFormat[13];
EVideoFormat VideoComm_GetFormatFromWH(DWORD dwWidth, DWORD dwHigth);
EVideoFormat VideoComm_WH2vFormat(DWORD dwWidth, DWORD dwHeight);
SCODE	VideoComm_ChkFormatByWH(EVideoFormat vFormat, DWORD dwWidth, DWORD dwHigth);


#endif //__VIDEOCOMM_H__


