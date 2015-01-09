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
 * venc_message.h
 *
 * \brief
 * Head file of video message member definition.
 *
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_MESSAGE_H_
#define _VENC_MESSAGE_H_

#include "msgreader.h"

// Host Type
typedef enum venc_message_host_type
{
    msgHMaster = 0,
    msgHVideoIn,
    msgHEncoder0,
    msgHEncoder1,
    msgHEncoder2,
    msgHDisplay,
    msgHUnKnown
} EHostType;

// Content
typedef enum venc_message_content_type
{
	msgCStart = 0,
	msgCStop,
	msgCForceCI,
	msgCForceIntra,
	msgCDumpVideoCapBuf,
	msgCDumpSharedBuf,
	msgCDumpVideoDispBuf,
	msgCRetVideoCapStatus,
	msgCPhotoMtcDefTbl1,
	msgCPhotoMtcDefTbl2,
	msgCEnableDisp,
	msgCDisableDisp,
	msgCWideDynamicRangeOn,
	msgCWideDynamicRangeOff,
	msgCKeepCurrentWB,
	msgCKeepCurrentWBFromFile,
	msgCEnableAF,
	msgCDisableAF,
	msgCEnableFrameRateCal,
	msgCDisableFrameRateCal,
	msgCDumpAFStatus,
	msgCAFZoomIn,
	msgCAFZoomOut,
	msgCAFFocusFar,
	msgCAFFocusNear,
	msgCISPEnable,
	msgCISPDisable,
	msgCISPUpdateState,
	msgAutoSceneIsRun,
	msgAutoSceneIsNotRun,
	msgSetRateCtrlMode,
	msgSetQuant,
	msgSetBitRate,
	msgGetQualityParameter,
	msgSetROI,
#ifdef _JPEG_SNAPSHOT_	
	msgCGetSnapshot,
#endif // _JPEG_SNAPSHOT_
	msgCClearInternalBuffer,
	msgCUnKnown
} EContentType;

typedef struct venc_message_member
{
	EHostType		eHost;
	EContentType	eContent;
	CHAR szHostName[64];
} TMessageMember;

#endif // _VENC_MESSAGE_H_
