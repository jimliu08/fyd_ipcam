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
 * venc_app_local.h
 *
 * \brief
 * Local header file of video process implement.
 *
 * \date
 * 2009/08/04
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_APP_LOCAL_H_
#define _VENC_APP_LOCAL_H_

/* ============================================================================================= */
#include <syslog.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
//common used
#include "videodef.h"
#include "dbgdefs.h"
#include "ubuffer.h"
//file relative
#include "venc_app.h"

#include "venc_master.h"
#include "venc_videoin.h"
#include "venc_encoder.h"
#include "venc_motion.h"
#include "venc_display.h"
#include "frameratectrl.h"
// library relative
#include "xmlwrapper.h"
#include "MemMgr.h"

#define DEFAULT_CONFIG_FILE    "/etc/conf.d/venc_conf.0.xml"
#define DEFAULT_COMMAND_FIFO   "/tmp/venc/venc.0.fifo"
//#define MAX_SLAVE_NUM				1024
#define MAX_HW_CHANNELNUM			8   /* for Mozart, Vivaldi: 1, Bach 4, Haydn 1, Mozart 8 */
extern TXmlWrapperTreeMap ptTreeMap[];
extern BOOL g_bTerminate;

typedef struct venc_conf_dram_location
{
    DWORD  dwVICOutputDRAMLoc; 			// vic_output
    DWORD  dwLDCRefFrameDRAMLoc;    		// ldc_ref_frame
	DWORD  dwDMACOutputDRAMLoc;			// dmac_output, it means sharedbuffer dram location
    DWORD  dwIREOutput0DRAMLoc;			// ire_output0
    DWORD  dwIREOutput1DRAMLoc;			// ire_output1
    DWORD  dwBitStreamOutputDRAMLoc;		// bitstream_output
    DWORD  dwVOCOutputDRAMLoc;			// voc_output
    DWORD  dwMotionSnapshotOutputDRAMLoc;	// motion_snapshot_output
    DWORD  dwMp4VBusNum;			// mp4v_bus_num
    DWORD  dwH264BusNum;			// h264_bus_num
    DWORD  dwSVCBusNum;				// svc_bus_num
    DWORD  dwEncROIBusNum;			//bus number of H.264 Enc ROI Mask Buf
} TVencDRAMLoc;
// all thread config path record
typedef struct venc_conf_path
{
	CHAR	*szSensorConfigFile;
	CHAR	*szCapabilityPath;
	BOOL	bMasterEnable;
	CHAR	*szMasterConfigFile;
	DWORD	dwSharedBufferMinorNum;
	BOOL	bVideoInEnable;
	CHAR	*szVideoInConfigFile;
	DWORD	dwEncoderTotalNum;
	DWORD	dwCurId;
	//CHAR	*szEncoderConfigFile[MAX_SLAVE_NUM];
	CHAR **pszEncoderConfigFile;	
	BOOL	bMotionEnable;
	CHAR	*szMotionConfigFile;
	BOOL	bDisplayEnable;
	CHAR	*szDisplayConfigFile;
	TVencDRAMLoc	tDRAMParam;
} TVencConfPath;

#define VIDEO_CODEC_JPEG_INIT_BIT 0x00000001
#define VIDEO_CODEC_MP4V_INIT_BIT 0x00000002
#define VIDEO_CODEC_H264_INIT_BIT 0x00000004
#define VIDEO_CODEC_SVC_INIT_BIT 0x00000008

#define JPEG_STR "mjpeg"
#define MP4V_STR "mpeg4"
#define H264_STR "h264"
#define SVC_STR  "svc"
#define SUPPORT_CODEC_STR_DELIM ","




// all Capability record
typedef struct venc_capability
{
	CHAR	*szSensorName;
	//! videocap initial buf number
	DWORD	dwBufNum;
	//! sensor max support size, that match VIDEO_MAX_WIDTH in past definition
	DWORD	dwVideoMaxWidth;
	DWORD	dwVideoMaxHeight;
	//! capture size, that match VIDEO_ENC_WIDTH in past definition
	DWORD	dwVideoCapWidth;
	DWORD	dwVideoCapHeight;
	//! sensor in size, that match VIDEO_IN_WIDTH in past definition
	DWORD	dwVideoInWidth;
	DWORD	dwVideoInHeight;
	DWORD	bInterlaceScan;
	DWORD	dwMaxCapFrameRate;
	BOOLEAN	bCMOSSensor;
	DWORD	dwVideoCodecInitMask;
	
} TVencCapabilityRec;

typedef struct venc_info
{
    //! string pointer of config file name
    CHAR				*szConfigFile;
    //! control reload config file
    sem_t				smOptions;

    //! handle of xmlwrapper, to reload config used
    HANDLE				hXMLWrapperObj;
    //! handle of MemMgr
    HANDLE              hMemMgrObj;
    //! handle of MsgDove
    HANDLE              hMsgReaderObj;
    //! handle of Master process
    HANDLE              hMasterProcObj;
    //! handle of VideoIn process
    HANDLE              hVideoInProcObj;
    //! handle of Encoding process
  //  HANDLE              hEncoderProcObj[MAX_SLAVE_NUM];
    HANDLE  		*phEncoderProcObj;
    //! handle of Motion sending process
    HANDLE              hMotionProcObj;
	//! handle of Display process
	HANDLE				hDisplayProcObj;
	
	//! all thread config path record
	TVencConfPath		tVencCfgPath;
	//! bitwise of Encoding process
	//DWORD 				dwCurrSwitch[MAX_SLAVE_NUM];
	DWORD		*pdwCurrSwitch;	
	//! capability record
	TVencCapabilityRec 	tCapabilityRec;
} TVencInfo;

SCODE VencApp_InitMemMgr(HANDLE *phMemMgrObj);
SCODE VencApp_InitXmlWrapper(HANDLE *phXMLWrapperObj);
SCODE VencApp_ParseVencConfig(TVencInfo *ptVencInfo);
SCODE VencApp_GetInitNeedInfo(TVencInfo *ptVencInfo);

SCODE VencMessage_Initial(TVencInfo *ptVencInfo, CHAR* szMsgFifo);
SCODE VencMessage_Start(HANDLE hMessageProcObj);
SCODE VencMessage_Stop(HANDLE hMessageProcObj);
SCODE VencMessage_Release(HANDLE *phMessageProcObj);

#endif //_VENC_APP_LOCAL_H_
