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
 * venc_videoin.h
 *
 * \brief
 * Head file of video in process implement.
 *
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_VIDEOIN_H_
#define _VENC_VIDEOIN_H_

#include "typedef.h"
#include "errordef.h"

typedef struct venc_videoin_init_need_info
{
	//! sensor max support size, that match VIDEO_MAX_WIDTH in past definition
	DWORD	dwVideoMaxWidth;
	DWORD	dwVideoMaxHeight;
	//! capture size, that match VIDEO_ENC_WIDTH in past definition
	DWORD	dwVideoCapWidth;
	DWORD	dwVideoCapHeight;
	//! sensor in size, that match VIDEO_IN_WIDTH in past definition
	DWORD	dwVideoInWidth;
	DWORD	dwVideoInHeight;
	DWORD  	dwLDCRefFrameDRAMLoc; 	// ldc_ref_frame
	DWORD 	dwDMACOutputDRAMLoc;	// dmac_output and ibpe output, it means sharedbuffer dram location
	DWORD 	dwVICOutputDRAMLoc;		// vic_output, it means dmac_input and ibpe_input dram location
} TVideoInInitNeedInfo;

typedef struct venc_videoin_initopt
{
    //! device number of sensor input
    DWORD 	dwDevNum;
    //! string pointer of config file name
    CHAR 	*szConfigFile;
    //! record the man process
    DWORD	dwCallbackInstance;
    //! MemMgr pass from outside
    HANDLE	hMemMgrObj;
    //! SharedBuffer Minor Num
    DWORD	dwSharedBufferMinorNum;
	//! init need info from venc
	TVideoInInitNeedInfo tInitNeedInfo;
	//! VideoCap handle
	HANDLE		hVideoCapObj;
	int  fdIRCut;
	int  fdIRCutUART;
	DWORD dwOutputSharedBufferNum;
} TVideoInInitOpt;

typedef enum venc_videoin_msgoptiontype
{
	VENCVIDEOIN_MSGOPTION_DUMP_VIDEOCAPBUF = 1,
	VENCVIDEOIN_MSGOPTION_DUMP_SHAREDBUF,
	VENCVIDEOIN_MSGOPTION_RETRIEVE_VIDEOCAPSTATUS,
	VENCVIDEOIN_MSGOPTION_PHOTOMTC_DEFINETBL1,
	VENCVIDEOIN_MSGOPTION_PHOTOMTC_DEFINETBL2,
	VENCVIDEOIN_MSGOPTION_KEEP_CURRENT_WB,
	VENCVIDEOIN_MSGOPTION_KEEP_CURRENT_WB_FROM_FILE,
	VENCVIDEOIN_MSGOPTION_DUMP_AF_STATUS,
	VENCVIDEOIN_MSGOPTION_ENABLE_FRAMERATE_CAL,
	VENCVIDEOIN_MSGOPTION_DISABLE_FRAMERATE_CAL,
	VENCVIDEOIN_MSGOPTION_ISP_ENABLE,
	VENCVIDEOIN_MSGOPTION_ISP_DISABLE,
	VENCVIDEOIN_MSGOPTION_ISP_UPDATE_STATE,
	VENCVIDEOIN_MSGOPTION_CLEAR_IBPE_INTERNAL_BUFFER
} EVideoinMsgOptType;

typedef struct venc_videoin_msgoptions
{
    EVideoinMsgOptType     	emasMsgOptType;
} TVideoinMsgOptions;

SCODE VencVideoIn_Initial(HANDLE *phVideoInProcObj, TVideoInInitOpt tVideoInInitOpt);
SCODE VencVideoIn_Release(HANDLE *phVideoInProcObj);
SCODE VencVideoIn_Start(HANDLE hVideoInProcObj);
SCODE VencVideoIn_Stop(HANDLE hVideoInProcObj);
void VencVideoIn_ResetConfigFlag(HANDLE hVideoInProcObj);
void VencVideoin_MsgReceive(HANDLE hMasterProcObj, TVideoinMsgOptions tVideoinMsgOpt);

#endif //_VENC_VIDEOIN_H_
