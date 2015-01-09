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
 * venc_display_local.h
 *
 * \brief
 * Local header file of video display process implement.
 *
 * \date
 * 2009/09/11
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_DISPLAY_LOCAL_H_
#define _VENC_DISPLAY_LOCAL_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <syslog.h>
#include <errno.h>

#include "typedef.h"
#include "errordef.h"
#include "dbgdefs.h"
#include "videodef.h"
#include "videocomm.h"
#include "global_codec.h"
#include "ubuffer.h"
#include "venc_queue.h"
#include "venc_display.h"
#include "venc_display_conf.h"
#include "frameratectrl.h"
#include "xmlwrapper.h"
#include "MemMgr.h"
#include "MassMemAccess.h"
#ifdef _VMA_IRE
#include "Resize.h"
#endif
//#ifdef _VPL_VOC
#include "VideoDisp.h"
//#endif
#include "SharedBuffMgr.h"

#define DISPLAY_DEFAULT_CONFIG_FILE "/etc/conf.d/c0/vencdisplay_conf.xml"
#define DISPLAY_OUT_BUF_WIDTH       1920
#define DISPLAY_OUT_BUF_HEIGHT      1080 
#define DISP_PRE                    "[VENC_DISP]"   //! printf prefix
#define VIDEO_DISP_DUMMP_BUF_INDEX  31
#define VIDEO_DISP_DUMMP_BUF_INDEX2 30
#define VIDEO_DISP_DUMMP_BUF_INDEX3 29
#define VIDEO_DISP_DUMMP_BUF_INDEX4 28

#define VIDEO_DISP_DUMMP_BUF_HANDLE_INDEX 255
#define VIDEO_DISP_RESIZE_BUF_HANDLE_INDEX 254
#define READER_NUM                  3

//#define DISP_DEBUG  //! debug printf in display
#if (defined(_DEBUG)) && (!defined(DISP_DEBUG))
#define DISP_DEBUG
#endif
#define DISP_DEBUG
#ifdef DISP_DEBUG
#define DISP_DP(x...) printf(x)
#else
#define DISP_DP(...)
#endif

typedef enum venc_display_optiontype
{
	VENC_DISPLAY_OPTION_SET_OUTPUTFORMAT = 1,
} EDisplayOptType;

typedef struct venc_display_options
{
	EDisplayOptType edispOptType;
	DWORD           adwUserData[3];
} TDisplayOptions;

//! Display message overall Recorder
typedef struct venc_display_msg_record
{
	BOOL    bDumpVCapBuf;
	BOOL    bDumpVDispBuf;
	BOOL    bNeed2Display;
} TDisplayMsgOptRec;

typedef struct venc_display_init_private
{
	//! sensor max support size
	DWORD    dwVideoMaxWidth;
	DWORD    dwVideoMaxHeight;
	//! capture size
	DWORD    dwVideoCapWidth;
	DWORD    dwVideoCapHeight;
	//! sensor in size
	DWORD    dwVideoInWidth;
	DWORD    dwVideoInHeight;
	DWORD    dwVOCOutputDRAMLoc;    //! voc_output
} _TDisplayInitNeedInfo;

typedef struct venc_display_shared_buff_info
{
	//! handle of shared buffer
	HANDLE                  hSrdObj;
	int                     iSrdFD;
	int                     iShouldGetCounter;
	TSharedBuffMgrState     tSrdBufMgrState;
} TDisplaySrdBufInfo;

typedef struct venc_display_info
{
	//! device number of sensor input
	DWORD                   dwDevNum;
	//! string pointer of config file name
	CHAR                    *szCfgFile;
	//! recorde parent thread PID
	DWORD                   dwParentPID;

	//! start semaphore to protect loop contorl
	sem_t                   smStart;
	//! stop emaphore to protect loop control
	sem_t                   smEnd;
	//! options semaphore to protect option datas
	sem_t                   smOpt;
	//! message semaphore to protect share data (bReMsg_flag)
	sem_t                   smMsg;
	//! EnDisplay semaphore to turn on/off display
	sem_t                   smDisp;
	//! thread instance
	pthread_t               ptThreadID;

	//! handle of VMA memory manager
	HANDLE                  hMemMgrObj;

	TDisplaySrdBufInfo      tSrdBufInfo[READER_NUM];

	TVideoFrameBuffer       tResizeOutBuf[READER_NUM];

	BOOL                    bNeedResize;

	int                     dwSharedBufMinorNum;
	//! handle of MassMemAccess process
	HANDLE                  hMassMemAccObj;
	//! handle of VideoDisp process
	HANDLE                  hVDispObj;
	//! handle of I2C device controller
	HANDLE                  hIICCtrlObject;
	//! handle of TV Encoder object
	HANDLE                  hTVEncoderObject;
	//! handle of XMLWrapper
	HANDLE                  hXMLObj;
	//! handle of Resize
	HANDLE                  hResizeObj;

	//! Flag of loop running
	BOOL                    bRunning;
	//! ExitInnerLoop
	BOOL                    bExitInnerLoop;
	//! Terminate the out loop
	BOOL                    bTerminateThread;
	//! Reset config option used, it will change to TRUE
	BOOL                    bReConf_flag;
	//! if command fifo (Message interface) receive data, it will change to TRUE
	BOOL                    bReMsg_flag;
	//! if need dump after get buffer from VideoCap_GetBuf, it will set to TRUE
	BOOL                    bDumpVCapBuf;
	//! if need dump after get buffer from VideoDisp_Get_Buf, it will set to TRUE
	BOOL                    bDumpVDispBuf;
	//! if need to display to VOC
	BOOL                    bNeed2Display;

	//! pointer to Display input buffer
	BYTE                    *pbyDispBuf;
	BYTE                    *pbyDispBuf2;

	//! record output format, now support NTSC, PAL, HDMI, HDSDI is not ready
	EDispOutFmt             eDispOutFmt;
	//! I2C device path
	CHAR                    *szI2CDevice;
	//! output Width
	DWORD                   dwOutputWidth;
	//! output Height
	DWORD                   dwOutputHeight;

	DWORD                   dwFreq;
	//! record the VideoCap status
	TVideoFrameBuffer       tVideoFrameBuffer;
	//! record VideoCap current frame
	DWORD                   dwCurtVideoCapIndex;

	//! Message - option recoder
	TDisplayMsgOptRec       tMsgOptRec;

	//! private init infomation
	_TDisplayInitNeedInfo   _tInitInfo;
	//! check if the VOC has been started or not
	BOOL                    bDisplayStart;
	struct my_list *pDisplayInfoQueue;
	DWORD dwQueueElmtNum;
	
	HANDLE hFRCtrlObj;
} TDisplayInfo;

SCODE VencDisplay_InitVML(TDisplayInfo *ptDisplayInfo);
SCODE VencDisplay_ReleaseVML(TDisplayInfo *ptDisplayInfo);
void* VencDisplay_Loop(HANDLE hInstance);
SCODE VencDisplay_ParseInitNeedConfig(TDisplayInfo *ptDisplayInfo);
SCODE VencDisplay_InitSharedBuffer(TDisplayInfo *ptInfo, int iIndex,BOOL bSlowReader);

#endif // _VENC_DISPLAY_LOCAL_H_

