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
 * venc_master.h
 *
 * \brief
 * Head file of video master process implement.
 *
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_MASTER_H_
#define _VENC_MASTER_H_

#include "typedef.h"
#include "errordef.h"

typedef struct venc_master_init_need_info
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
	//! buffer number
	DWORD	dwBufNum;
	// string pointer of sensor config file
	CHAR	*szSensorConfigFile;
	//! vic output buffer DRAM parameter
	DWORD	dwVICOutputDRAMLoc;
} TMasterInitNeedInfo;

typedef struct venc_master_initopt
{
    //! device number of sensor input
    DWORD 	dwDevNum;
    //! string pointer of config file name
    CHAR 	*szConfigFile;
    //! record the man process
    DWORD	dwCallbackInstance;
    //! MemMgr pass from outside
    HANDLE	hMemMgrObj;
    //! init need info from venc
    TMasterInitNeedInfo	tInitNeedInfo;
} TMasterInitOpt;

typedef enum venc_master_msgoptiontype
{
    VENCMASTER_MSGOPTION_WDR_ON = 1,
    VENCMASTER_MSGOPTION_WDR_OFF,
    VENCMASTER_MSGOPTION_ENABLE_AF,
    VENCMASTER_MSGOPTION_DISABLE_AF,
    VENCMASTER_MSGOPTION_AF_ZOOM_IN,
    VENCMASTER_MSGOPTION_AF_ZOOM_OUT,
    VENCMASTER_MSGOPTION_AF_FOCUS_FAR,
    VENCMASTER_MSGOPTION_AF_FOCUS_NEAR,
    VENCMASTER_MSGOPTION_AUTOSCENE_IS_RUN,
    VENCMASTER_MSGOPTION_AUTOSCENE_IS_NOT_RUN
    
} EMasterMsgOptType;

typedef struct venc_master_msgoptions
{
    EMasterMsgOptType     	emasMsgOptType;
} TMasterMsgOptions;

SCODE VencMaster_Initial(HANDLE *phMasterProcObj, TMasterInitOpt tMasterInitOpt);
SCODE VencMaster_Release(HANDLE *phMasterProcObj);
SCODE VencMaster_Start(HANDLE hMasterProcObj);
SCODE VencMaster_Stop(HANDLE hMasterProcObj);
void VencMaster_ResetConfigFlag(HANDLE hMasterProcObj);
void VencMaster_MsgReceive(HANDLE hMasterProcObj, TMasterMsgOptions tMasterMsgOpt);
SCODE VencMaster_GetVideoCapHandle(HANDLE hMasterProcObj, HANDLE *hObj);
SCODE VencMaster_GetUARTIRCutFD(HANDLE hMasterProcObj, int *pFDIRCutUART);
SCODE VencMaster_GetIRCutFD(HANDLE hMasterProcObj, int *pFDIRCut);
#endif // _VENC_MASTER_H
