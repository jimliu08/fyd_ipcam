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
 * venc_master_local.h
 *
 * \brief
 * Local header file of video master process implement.
 *
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_MASTER_LOCAL_H_
#define _VENC_MASTER_LOCAL_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <syslog.h>


//For IRCut
#include <errno.h>
#include <fcntl.h> 
#include <sys/ioctl.h>


//=========================================
// For IRCut driver
#include "ircut.h"
#define IRCUT_DEVICE "/dev/ircut"
//=========================================


#include "typedef.h"
#include "errordef.h"
#include "dbgdefs.h"
#include "videodef.h"
#include "videocomm.h"

#include "venc_master.h"
#include "venc_master_conf.h"

#include "xmlwrapper.h"
#include "MemMgr.h"
#include "VideoCap.h"

#define MASTER_DEFAULT_CONFIG_FILE    "/etc/conf.d/c0/vencmaster_conf.xml"
#define KEEP_CURRENT_WB_FILE_PATH  "/etc/conf.d/keep_current_wb"


extern TVideoMasterCfg g_tVideoMasterCfg;
extern TWBCfg g_tWBCfg;
extern TColorCorrectionCfg g_tColorCorrctionCfg;
extern TGammaTableCfg g_tGmaTblCfg;
extern TResPLCfg g_tResPLCfg;
extern TAECfg g_tAECfg;
extern TAFCfg g_tAFCfg;
extern TTMCfg g_tTMCfg;
extern TCECfg g_tCECfg;
extern TWDRCfg g_tWDRCfg;
extern TImgAdjCfg g_tImgAdjCfg;
extern TBlackClampCfg g_tBlackClampCfg;
extern TDeImpulseCfg g_tDeImpulseCfg;

typedef struct autoexposure_real_res_2_default_system
{
	FLOAT			fXTranslate;
	FLOAT			fYTranslate;
	FLOAT			fXScale;
	FLOAT			fYScale;
} TAEReal2Default;

typedef struct wdr_status
{
	BOOL	bModified;
	BOOL	bEnable;
} TWDRStatus;


typedef struct af_status
{
    BOOL	bModified;
    BOOL	bEnable;

} TAFStatus;


typedef enum af_zoom_val
{
	
  AFZoomIn = 0,
  AFZoomOut = 1,  
  AFZoomUnknow = 2

} EZoomVal;

typedef enum af_focus_val
{
	
  AFFocusNear = 0,
  AFFocusFar = 1,  
  AFFocusUnknow = 2

} EFocusVal;

typedef struct af_zoom
{
  BOOL	bModified;
  EZoomVal eZoomVal;
  
}TAFZoom;

typedef struct af_focus
{
  BOOL	bModified;
  EFocusVal  eFocusVal;
  
}TAFFocus;

typedef struct auto_scene
{
    BOOL	bModified;
   BOOL	bISRun;
}TAutoScene;
//! Master message overall Recorder
typedef struct venc_master_msg_record
{
	TWDRStatus	tWDRStatus;
	TAFStatus 	tAFStatus;
	TAFZoom   	tAFZoom;
	TAFFocus	tAFFocus;
	TAutoScene	tAutoScene;
} TMasterMsgOptRec;

//! TAWBRetGainValue definition
typedef struct auto_white_balance_retrieve_gain_value
{
	DWORD	dwAWBRedGain;
	DWORD	dwAWBBlueGain;
} TAWBRetGainValue;

typedef struct venc_master_init_private
{
	//! sensor max support size
	DWORD	dwVideoMaxWidth;
	DWORD	dwVideoMaxHeight;
	//! capture size
	DWORD	dwVideoCapWidth;
	DWORD	dwVideoCapHeight;
	//! sensor in size
	DWORD	dwVideoInWidth;
	DWORD	dwVideoInHeight;
	//! buffer number
	DWORD	dwBufNum;
	// string pointer of sensor config file
	CHAR	*szSensorConfigFile;
	//! vic output buffer DRAM parameter
	DWORD	dwVICOutputDRAMLoc;
} _TMasterInitNeedInfo;

typedef struct venc_master_info
{
    //! device number of sensor input
    DWORD      			dwDevNum;
    //! string pointer of config file name
    CHAR       			*szConfigFile;
    //! recorde parent thread PID
    DWORD               dwParentPID;
    //! a callback parameter instance
    DWORD               dwCallbackInstance;

    //! start semaphore to protect loop contorl
    sem_t				smStart;
    //! stop emaphore to protect loop control
    sem_t              	smEnd;
    //! wakeup semaphore to protect loop control
	sem_t				smWakeup;
    //! options semaphore to protect option datas
    sem_t              	smOptions;
    //! message semaphore to protect share data (bReMsg_flag)
    sem_t              	smMsgOptions;
    //! thread instance
    pthread_t			ptThreadID;
    
    //! handle of I2C device controller
    HANDLE     hIICCtrlObject;

    //! handle of image sensor (MT9V111, SAA7113H...)
    HANDLE     hSensorObject;

    //! handle of VMA memory manager
    HANDLE     hMemMgrObj;

    //! handle of harware video capture
    HANDLE     hVideoCapObject;

    //! handle of xmlwrapper
    HANDLE     hXMLWrapperObj;
   

	//! handle of AutoFocus
//	HANDLE hAutoFocus;

	//! handle of AutoExposure
	HANDLE		hAEObject;

    //! VideoCap output buffer
    TVideoFrameBuffer tVideoFrameBuffer;

    //! handle the video capture index
    DWORD      dwVideoCapIndex;

    //! Flag of loop running
    BOOL       	bRunning;
    //! ExitInnerLoop
    BOOL       	bExitInnerLoop;
    //! Terminate the out loop
	BOOL		bTerminateThread;
    //! Reset config option used, it will change to TRUE
    BOOL      	bReConf_flag;
	//! if command fifo (Message interface) receive data, it will change to TRUE
	BOOL		bReMsg_flag;

	//! Resolution/Power Line reconfig
	BOOL  	   	bResPLReconfig;

	//! Color correction reconfig
 	BOOL	   	bCCReconfig;

	//! Tone Mapping reconfig
	BOOL		bTMReconfig;

	//! Black Clamp reconfig
	BOOL bBlackClampReconfig;
	
	//! De Impulse reconfig
	BOOL bDeImpulseReconfig;
	
	//! Contrast Enhancement reconfig
	BOOL		bCEReconfig;

	//! White balance reconfig
	BOOL  	   	bWBReconfig;

	//! Gamma table reconfig
	BOOL	   	bGammaTableReconfig;

	//! Image Adjustment reconfig
	BOOL		bImgAdjReconfig;
	
	//! WDR status record
	TWDRStatus	tWDRStatus;
	
	//! AF status record
	TAFStatus tAFStatus;

	//! AF Zoom 
	TAFZoom   	tAFZoom;

	//! AF Focus 
	TAFFocus	tAFFocus;
	
	//! retrieve gain value record
	TAWBRetGainValue tAWBRetGainValue;
	
	//! auto exposure reconfig
	BOOL		bAEReconfig;
	
	//! auto focus reconfig
	BOOL 		bAFReconfig;
	//! wdr reconfig
	BOOL 		bWDRReconfig;
	
	//! record about software AE Enable, to decide AE_OneFrame or not
	BOOL		bSWAEEnable;
	//! AutoExposure info
	TAEInfo		tAEInfo;
	//! AutoExposure real resolution 2 default coordinate system
	TAEReal2Default tAER2D;
	
	//! Message - option recoder
	TMasterMsgOptRec	tMsgOptRec;
	
	// private init infomation
	_TMasterInitNeedInfo _tInitNeedInfo;
	
	
	int  fdIRCut;
	int  fdIRCutUART;
	BYTE *pbyAFAdjustBuf;
	BYTE *pbyCapBuf;
	
	BOOL bAutoSceneIsRun;
	
	BYTE *pbyStatAEWBBufUsrBaseAddr;
	BYTE *pbyStatHistoBufUsrBaseAddr;
	BYTE *pbyStatFocusBufUsrBaseAddr;
} TMasterInfo;

SCODE VencMaster_IRCut_Initial(TMasterInfo *ptMasterInfo);
SCODE VencMaster_IRCut_Release(TMasterInfo *ptMasterInfo);

SCODE VencMaster_InitVML(TMasterInfo *ptMasterInfo);
SCODE VencMaster_ReleaseVML(TMasterInfo *ptMasterInfo);
SCODE VencMaster_InitConfigParam(TMasterInfo *ptMasterInfo);
void* VencMaster_Loop(HANDLE hInstance);
SCODE VencMaster_LoadConfig(HANDLE hVencMObject);
SCODE VencMaster_WBReConfig(HANDLE hVencMObject);
SCODE VencMaster_CCReConfig(HANDLE hVencMObject);
SCODE VencMaster_TMReConfig(HANDLE hVencMObject);
SCODE VencMaster_BlackClampReConfig(HANDLE hVencMObject);
SCODE VencMaster_DeImpulseReConfig(HANDLE hVencMObject);
SCODE VencMaster_CEReConfig(HANDLE hVencMObject);
SCODE VencMaster_ResPLReConfig(HANDLE hVencMObject);
SCODE VencMaster_AEReConfig(HANDLE hVencMObject);
SCODE VencMaster_AFReConfig(HANDLE hVencMObject);
SCODE VencMaster_WDRReConfig(HANDLE hVencMObject);
SCODE VencMaster_GammaTableReConfig(HANDLE hVencMObject);
SCODE VencMaster_ImgAdjReConfig(HANDLE hVencMObject);
SCODE VencMaster_PhotoMtcReConfig(HANDLE hVencMObject);
SDWORD GetParam(FILE *pfInput);
#endif // _VENC_MASTER_LOCAL_H
