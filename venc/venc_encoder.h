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
 * venc_encoder.h
 *
 * \brief
 * Head file of video encoder process implement.
 *
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_ENCODER_H_
#define _VENC_ENCODER_H_

#include "typedef.h"
#include "errordef.h"
#define VIRTUAL_PTZ
typedef struct venc_encoder_init_need_info
{
	//! sensor max support size, that match VIDEO_MAX_WIDTH in past definition
	DWORD	dwVideoMaxWidth;
	DWORD	dwVideoMaxHeight;
	//! capture size, that match VIDEO_ENC_WIDTH in past definition
	DWORD	dwVideoCapWidth;
	DWORD	dwVideoCapHeight;
	//! bit stream size
	DWORD	dwBitStreamSize;
	DWORD  	dwIREOutput0DRAMLoc;		// ire_output0
	DWORD  	dwIREOutput1DRAMLoc;		// ire_output1
	DWORD  	dwBitStreamOutputDRAMLoc;	// bitstream_output
	DWORD  	dwMp4VBusNum;			// mp4v_bus_num
	DWORD  	dwH264BusNum;			// h264_bus_num
	DWORD	dwSVCBusNum;			//svc_bus_num
	BOOL	bInterlaceScan;         // interlace scan or progressive
	DWORD	dwMaxCapFrameRate;
	BOOLEAN	bCMOSSensor;
	DWORD dwVideoCodecInitMask;
} TEncoderInitNeedInfo;

typedef struct venc_encoder_initopt
{
	//! string pointer of config file name
	CHAR 	*szConfigFile;
	//! record the man process
	DWORD	dwCallbackInstance;
	//! MemMgr pass from outside
	HANDLE	hMemMgrObj;
	//! SharedBuffer Minor Num between VideoIn and Encoder
	DWORD	dwSharedBufferMinorNum;
	//! ID, to verify which stream is.
	DWORD	dwCurId;
	//! init need info from venc
	TEncoderInitNeedInfo tInitNeedInfo;
	//! The following field is for recording the resolution format of random cropping.
	DWORD				dwVideoResolutionFormatIndex;
} TEncoderInitOpt;

typedef enum venc_encoder_msgoptiontype
{
	VENCENCODER_MSGOPTION_SET_FORCEINTRAL = 1,
	VENCENCODER_MSGOPTION_SET_FORCECI,
	VENCENCODER_MSGOPTION_SET_REG_OUTPUT,
	VENCENCODER_MSGOPTION_SET_UNREG_OUTPUT,
	VENCENCODER_MSGOPTION_SET_VPTZ_ENABLE,
	VENCENCODER_MSGOPTION_SET_VPTZ_DISABLE,
	VENCENCODER_MSGOPTION_SET_VPTZ_PAN,
	VENCENCODER_MSGOPTION_SET_VPTZ_TILT,
	VENCENCODER_MSGOPTION_SET_VPTZ_ZOOM,
	VENCENCODER_MSGOPTION_SET_VPTZ_SPEED,
	VENCENCODER_MSGOPTION_ENABLE_FRAMERATE_CAL,
	VENCENCODER_MSGOPTION_DISABLE_FRAMERATE_CAL,
	VENCENCODER_MSGOPTION_SET_CONTROL_MODE,
	VENCENCODER_MSGOPTION_SET_QUANT,
	VENCENCODER_MSGOPTION_SET_BITRATE,
	VENCENCODER_MSGOPTION_SET_CODEC_TYPE,
	VENCENCODER_MSGOPTION_GET_QUALITY_PARAMETER,
	VENCENCODER_MSGOPTION_SET_ROI,
	VENCENCODER_MSGOPTION_GET_SNAPSHOT
} EEncoderMsgOptType;

typedef struct venc_encoder_msgoptions
{
	EEncoderMsgOptType     	eenMsgOptType;
	float					aflUserData[3];
} TEncoderMsgOptions;

SCODE VencEncoder_Initial(HANDLE *phEncoderProcObj, TEncoderInitOpt tEncoderInitOpt);
SCODE VencEncoder_Release(HANDLE *phEncoderProcObj);
SCODE VencEncoder_Start(HANDLE hEncoderProcObj);
SCODE VencEncoder_Stop(HANDLE hEncoderProcObj);
void VencEncoder_ResetConfigFlag(HANDLE hEncoderProcObj);
void VencEncoder_MsgReceive(HANDLE hEncoderProcObj, TEncoderMsgOptions tEncoderMsgOpt);

#endif // _VENC_ENCODER_H_
