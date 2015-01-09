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
 * venc_motion.h
 *
 * \brief
 * Head file of video motion process implement.
 *
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_MOTION_H_
#define _VENC_MOTION_H_

#include "typedef.h"
#include "errordef.h"

#if defined(_MD_TRIGER_ROI_)
	#define MAX_ENCODER_NUM 10
#endif
typedef struct venc_motion_init_need_info
{
    //! sensor max support size, that match VIDEO_MAX_WIDTH in past definition
    DWORD   dwVideoMaxWidth;
    DWORD   dwVideoMaxHeight;
    //! capture size, that match VIDEO_ENC_WIDTH in past definition
    DWORD   dwVideoCapWidth;
    DWORD   dwVideoCapHeight;
    //! bit stream size
    DWORD   dwBitStreamSize;
    //! motion_snapshot_output
    DWORD   dwMotionSnapshotOutputDRAMLoc;    
    DWORD   dwVideoCodecInitMask;

} TMotionInitNeedInfo;

typedef struct venc_motion_initopt
{
    //! string pointer of config file name
    CHAR                *szConfigFile;
    //! record the man process
    DWORD               dwCallbackInstance;
    //! MemMgr pass from outside
    HANDLE              hMemMgrObj;
    //! SharedBuffer Minor Num between VideoIn and Motion
    DWORD               dwSharedBufferMinorNum;
    //! init need info from venc
    TMotionInitNeedInfo tInitNeedInfo;

} TMotionInitOpt;

SCODE VencMotion_Initial         (HANDLE *phMotionProcObj, 
                                  TMotionInitOpt tMotionInitOpt);
SCODE VencMotion_Release         (HANDLE *phMotionProcObj);
SCODE VencMotion_Start           (HANDLE hMotionProcObj);
SCODE VencMotion_Stop            (HANDLE hMotionProcObj);
void  VencMotion_ResetConfigFlag (HANDLE hMotionProcObj);

#endif // _VENC_MOTION_H_

