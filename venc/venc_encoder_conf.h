/*
 *******************************************************************************
 * $Header: $
 *
 *  Copyright (c) 2007-2010 VN Inc. All rights reserved.
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
 *  | VN INC.                                                     |
 *  +-----------------------------------------------------------------+
 *
 * $History: $
 * 
 *******************************************************************************
 */

/*!
 *******************************************************************************
 * Copyright 2007-2010 VN, Inc. All rights reserved.
 *
 * \file
 * venc_encoder_conf.h
 *
 * \brief
 * Header file of video encoder process setoption implement
 * 
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_ENCODER_CONF_H_
#define _VENC_ENCODER_CONF_H_

#define VIDEO_CODEC_MPEG4   0
#define VIDEO_CODEC_MJPEG   1
#define VIDEO_CODEC_H264	2
#define VIDEO_CODEC_SVC		3
#define MAX_VIDEO_CODEC     3

// define for quality type entry
#define VIDEO_QUALITY_FIX_BITRATE   0      //constant quality and constant bitrate
#define VIDEO_QUALITY_FIX_QUANT     1      //constant quality and variale bitrate
#define MAX_VIDEO_QUALITY           1

// // define for quant entry
#define	MIN_MPEG4_QUANT				1
#define	MAX_MPEG4_QUANT				31
#define	MIN_MJPEG_QUANT				0
#define	MAX_MJPEG_QUANT				255
#define MIN_H264_QUANT				0
#define MAX_H264_QUANT				51

#define MIN_SVC_QUANT				0
#define MAX_SVC_QUANT				51

#define MAX_VIDEO_RESOLUTION_LEN   11

#define MAX_ROI_WIN_NUM 	9
#define MAX_CAPTION_LEN 	14
#define ROI_NON_USED_WINDOW		0xFFFFFFFF
#define ROI_USED_WINDOW			0xEEEEEEEE
// motion content
typedef struct roi_windows_info
{
    BOOL            bEnabled;
    char            szName[MAX_CAPTION_LEN + 1];
    DWORD           dwLeft;
    DWORD           dwTop;
    DWORD           dwWidth;
    DWORD           dwHeight;
    DWORD			dwSerialNum;
    DWORD 		dwEncodingInterval;
    SDWORD		sdwDeltaQp;
    // new add item
    BOOL			bEnabledChange;
    BOOL			bPositionChange;
    BOOL			bWindowSizeChange;
    BOOL			bEncodingIntervalChange;
    BOOL			bDeltaQpChange;
    //! if anyone of bEnableChange, bPositionChange, bWindowSizeChange changed, bAnyFieldChange will set to TRUE,
    //! we used this field to check if any item change, it will speed up the search time.
    BOOL			bAnyFieldChange;
    DWORD	dwWinID;
} TROIWinInfo;

typedef struct roi_config
{
    BOOL            bROIEnabled;
    TROIWinInfo  atroiwCfg[MAX_ROI_WIN_NUM];
    DWORD           dwCurWinId;
    // new add item
    BOOL		bROIEnabledChange;
    // the four items should not changed after initialization.
    FLOAT			fXTranslate;
    FLOAT			fYTranslate;
    FLOAT			fXScale;
    FLOAT			fYScale;
} TROICfg;

typedef enum converting_method
{
	ecmCropping = 1,
	ecmSqueezing = 2,
	ecmRandomCropping=3
} EConvertingMethod;

typedef struct video_slave_config
{
    //! The encoder type of the stream
    DWORD   dwCodecType;
    //! The frame rate control
    DWORD   dwFrameRate;
    //! The video resolution of streams
    CHAR    szResolution[MAX_VIDEO_RESOLUTION_LEN + 1];
	//! The converting method
	EConvertingMethod ecMethod;
    //! The MPEG4 Interval between I frames
    DWORD   dwMPEG4KeyInterval;
    //! The MPEG4 quality control type
	ERateCtrlFlags	ercfMPEG4QualityType;
    //! The MPEG4 quant Value
    DWORD   dwMPEG4QuantValue;
    //! The MPEG4 maxmum Bitrate of stream
    DWORD   dwMPEG4Bitrate;
    //! The MJPEG quant Value
    DWORD   dwMJPEGQuantValue;
    //! The H264 Interval between I frames
    DWORD   dwH264KeyInterval;
    //! The H264 quality control type
    ERateCtrlFlags   ercfH264QualityType;
    //! The H264 quant Value
    DWORD   dwH264QuantValue;
    //! The H264 maxmum Bitrate of stream
    DWORD   dwH264Bitrate;
    //! The SVC Interval between I frames
    DWORD   dwSVCKeyInterval;
    //! The SVC quality control type
    ERateCtrlFlags   ercfSVCQualityType;
    //! The SVC quant Value
    DWORD   dwSVCQuantValue;
    //! The SVC maxmum Bitrate of stream
    DWORD   dwSVCBitrate;
    //! The SVC hdr enable flag 
    BOOL    bSVCHDREnable;
    //! The SVC layer number
    DWORD   dwSVCLayerNum;	
    //! The resolution format
    DWORD vfFormat;
	
	DWORD dwH264SearchRangeX;
	DWORD dwH264SearchRangeY;
	DWORD dwH264RefFrameNum;

	DWORD dwH264MaxFrameRate;
	DWORD dwH264MinFrameRate;

	DWORD dwH264MaxQuant;
	DWORD dwH264MinQuant;
	
	BOOL bDeblockEnable;
	
	DWORD dwH264Profile;
} TVideoSlaveCfg;

#endif //_VENC_ENCODER_CONF_H_
