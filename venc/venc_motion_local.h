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
 * venc_motion_local.h
 *
 * \brief
 * Local header file of video motion process implement.
 *
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_MOTION_LOCAL_H_
#define _VENC_MOTION_LOCAL_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/time.h>
#include <syslog.h>
#include <assert.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

#include "typedef.h"
#include "errordef.h"
#include "dbgdefs.h"
#include "videodef.h"
#include "videocomm.h"

#include "venc_motion.h"

#include "ubuffer.h"

#include "MemMgr.h"
#include "SharedBuffMgr.h"
#include "xmlwrapper.h"
#ifdef _VMA_JEBE
    #include "JPEGEnc.h"
#endif

#if defined(_MD_TRIGER_ROI_)

#ifdef _H4EE
	#include "H264Enc.h"
#endif

#ifdef _SVC
	#include "SVCEnc.h"
#endif


#include "ROISelector.h"

#define MAX_ENCODER_NUM 10

#endif

#define MOTION_DEFAULT_CONFIG_FILE  "/etc/conf.d/c0/vencmotion_conf.0.xml"

//#define MOTION_DEBUG  //! debug print in motion
#if (defined(_DEBUG)) && (!defined(MOT_DEBUG))
    #define MOTION_DEBUG
#endif

//! printf prefix
#define MOT_PRE "[VENC_MOTION]"
//! motion debug print
#ifdef MOTION_DEBUG
    #define MOT_DP(x...) printf(x)  
#else
    #define MOT_DP(...)
#endif

//! unix domain socket
typedef struct socket_info
{
    //! Socket file descriptor
    int                 fdSock;
    //! Socket path 
    CHAR                *szUSckPath;
	//! Socket structure 
	struct sockaddr_un  sunx;
    //! need conf or not
    BOOL                bNeedConf;

} TSckInfo;

typedef struct motion_real_res_2_default_system
{
    FLOAT               fXTranslate;
    FLOAT               fYTranslate;
    FLOAT               fXScale;
    FLOAT               fYScale;

} TMotionReal2Default;

//! each motion result
typedef struct motion_result_info
{
    DWORD               dwIndex;
    BYTE                byPercent;
    BYTE                byThreshold;
    DWORD               dwLeft;
    DWORD               dwTop;
    DWORD               dwRight;
    DWORD               dwBottom;
#if defined(_MD_TRIGER_ROI_)	
	DWORD               dwCfgLeft;
	DWORD               dwCfgTop;
	DWORD               dwCfgWidth;
	DWORD               dwCfgHeight;
#endif
} TMotionResultInfo;

typedef struct user_data
{
    DWORD               dwUserDataLength;
    TMotionReal2Default tMtnR2D;
    DWORD               dwMotionNum;
	BOOL	bMotionEnable;
} TUserData;

#define VIDEO_CODEC_JPEG_INIT_BIT 0x00000001
#define VIDEO_CODEC_MP4V_INIT_BIT 0x00000002
#define VIDEO_CODEC_H264_INIT_BIT 0x00000004
#define VIDEO_CODEC_SVC_INIT_BIT  0x00000008

typedef struct venc_motion_init_private
{
    //! sensor max support size
    DWORD   dwVideoMaxWidth;
    DWORD   dwVideoMaxHeight;
    //! capture size
    DWORD   dwVideoCapWidth;
    DWORD   dwVideoCapHeight;
    //! bit stream size
    DWORD   dwBitStreamSize;
    //! motion_snapshot_output DRAM location
    DWORD   dwMotionSnapDRAMLoc;    
    DWORD   dwVideoCodecInitMask;

} _TMotionInitNeedInfo;

typedef struct venc_motion_info
{
    //! string pointer of config file name
    CHAR                    *szCfgFile;
    //! recorde parent thread PID
    DWORD                   dwParentPID;
    //! SharedBuffer MinorNum
    DWORD                   dwSharedBufMinorNum;

    //! start semaphore to protect loop contorl
    sem_t                   smStart;
    //! stop emaphore to protect loop control
    sem_t                   smEnd;
    //! options semaphore to protect share data (bReCfg)
    sem_t                   smOpt;
    //! thread instance
    pthread_t               ptThreadID;

    //! handle of VMA memory manager
    HANDLE                  hMemMgrObj;
    //! handle of share buffer
    HANDLE                  hVideoInSrdObj;
    //! handle of XMLWrapper
    HANDLE                  hXMLObj;

    //! when call Motion_Start, bRunning will be set TRUE
    //! when call Motion_Stop, bRunning will be set FALSE
    BOOL                    bRunning;
    //! when call Encoder_Stop, bExitInnerLoop will be set FALSE,
    //! and let it to wait thread to set this flag be TRUE    
    BOOL                    bExitInnerLoop;
    //! Terminate the out loop
    BOOL                    bTerminateThread;
    //! Reset config option used, it will change to TRUE
    BOOL                    bReCfg;    
    
    //! output socket info
    TSckInfo                tRtspSckInfo;
    BOOL                    bSendSckEnable;

    //! output event info
    TSckInfo                tEvtSckInfo;
    //! if true, if there is a motion, it will send to event socket
    BOOL                    bSendEvtEnable;

    //! shared buffer file descriptor for select used
    int                     iVideoInSrdFD;
    //! VideoIn sharedbuffer state
    TSharedBuffMgrState     tSrdBufMgrState;

    //! JPEG - handle of JPEG encoder
    HANDLE                  hJPEGEncObj;

    //! JPEG - output buffer
    BYTE                    *pbyEncOutBuf;

    //! Motion trigger snapshot - file path
    CHAR                    szSnapPath[128];

    //! private init infomation
    _TMotionInitNeedInfo    _tInitInfo;
    //! check if JPEG encoder is initialized or not
    DWORD                   dwVideoCodecInitMask;
	
#if defined(_MD_TRIGER_ROI_)
	DWORD dwROIDuration; // the unit is seconds
	SDWORD sdwROIDeltaQp;
	DWORD dwROIEncodingInterval;
	int iCmdFiFo;
#endif

} TMotionInfo;

SCODE VencMotion_InitVML    (TMotionInfo *ptInfo);
void  VencMotion_ReleaseVML (TMotionInfo *ptInfo);
SCODE VencMotion_InitSocket (TMotionInfo *ptInfo);
void* VencMotion_Loop       (HANDLE hInstance);
SCODE VencMotion_LoadConfig (HANDLE hObject);

#endif //_VENC_MOTION_LOCAL_H_

