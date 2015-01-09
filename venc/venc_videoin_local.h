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
 * venc_videoina_local.h
 *
 * \brief
 * Local header file of video in process implement.
 *
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_VIDEOIN_LOCAL_H_
#define _VENC_VIDEOIN_LOCAL_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <syslog.h>
#include <sched.h>
#include <errno.h>
#include <fcntl.h>		/* for nonblocking */
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

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

#include "venc_videoin.h"
#include "venc_videoin_conf.h"
#include "frameratectrl.h"
#include "videocomm.h"
#include "xmlwrapper.h"
#include "MemMgr.h"
#include "VideoCap.h"
#if defined(_DEFECT_PIXEL_ENABLE_)  
#include "DefectPixelCorrect.h"
#endif
#if defined(_AUTOSCENE_)  
#include "AutoScene.h"
#endif

#if defined(_VMA_IBPE)
#include "ImgBackEndProcess.h"
#include "png.h"
#include "bitmaskresizer.h"
#endif

#include "MassMemAccess.h"
#include "SharedBuffMgr.h"

#define PHOTOLDCREF_M1 "/etc/conf.d/c0/photo_ldc_tbl_m1.dat"
#define PHOTOLDCREF_M2 "/etc/conf.d/c0/photo_ldc_tbl_m2.dat"
#define PHOTOLDCREF_USERDEFINE1 "/etc/conf.d/c0/photo_ldc_tbl_userdefine1.dat"
#define PHOTOLDCREF_USERDEFINE2 "/etc/conf.d/c0/photo_ldc_tbl_userdefine2.dat"
#define VIDEOIN_DEFAULT_CONFIG_FILE    "/etc/conf.d/vencvideoin_conf.xml"
#define MAX_MOTION_WIN_NUM    16  // it should be remove when include _conf.h
#define KEEP_CURRENT_WB_FILE_PATH  "/etc/conf.d/keep_current_wb"
#define DUMP_AF_STATUS_FILE_PATH "/tmp/af_status.log"

//#define  _VMA_IBPE_BACK_DOOR
#define IBPE_GET2MULTIPLE(S)	((((S)&0x00000007)==0)?(S):((((S)>>1)+1)<<1))
#define IBPE_GET8MULTIPLE(S)	((((S)&0x00000007)==0)?(S):((((S)>>3)+1)<<3))
#define IBPE_GET16MULTIPLE(S)	((((S)&0x0000000F)==0)?(S):((((S)>>4)+1)<<4))
#define IBPE_GET64MULTIPLE(S)	((((S)&0x0000003F)==0)?(S):((((S)>>6)+1)<<6))
#define MAX_FILE_PATH_LEN (1024)

//! VideoIn setoption Recorder
typedef struct venc_videoin_option_record
{
	//    BOOL            bResolution;
	//    EVideoFormat    vfFormat;

	//! record whether Motion Threshold change or not
	BOOL			bMotionThresholdChange;
	DWORD			dwMotionThreshold;
	//! record whether Static Period change or not
	BOOL			bStaticPeriodChange;
	DWORD			dwStaticPeriod;

	//! record whether Reset Statistics change or not
	BOOL 	bResetStatisticsChange;
	BOOL	bResetStatistics;
	//! record whether Demo change or not
	BOOL 	bDemoChange;
	BOOL 	bDemo;
	//! record whether SubtitleDetect change or not
	BOOL 	bSubtitleDetectChange;
	BOOL 	bSubtitleDetect;
	//! record whether TmprlNR change or not
	BOOL 	bTmprlNRChange;
	BOOL 	bTmprlNR;
} TVideoInOptRec;

typedef enum venc_videoin_optiontype
{
	VENCVIDEOIN_OPTION_SET_DEINTERLACER_ENABLE = 1,
	VENCVIDEOIN_OPTION_SET_MOTIONTHRESHOLD,
	VENCVIDEOIN_OPTION_SET_STATICPERIOD,
	VENCVIDEOIN_OPTION_SET_RESETSTATISTICS,
	VENCVIDEOIN_OPTION_SET_DEMO,
	VENCVIDEOIN_OPTION_SET_SUBTITLEDETECT,
	VENCVIDEOIN_OPTION_SET_TMPRLNR,
} EVideoInOptType;

typedef struct venc_videoin_options
{
	EVideoInOptType     eviOptType;
	DWORD               adwUserData[3];
} TVideoInOptions;

//! Status type
typedef enum venc_videoin_statustype
{	
	IBPEONLY = 1,
	ISPPROCESS = 2,
	NOPREPROCESS = 3,	
	TWICEIBPE = 4,
} EVideoInStatusType;


//! Master message overall Recorder
typedef struct venc_videoin_msg_record
{
	BOOL    bDumpVideoCapBuf;
	BOOL    bDumpSharedBuf;
	BOOL    bRetVideoCapStatus;
	BOOL    bPhotoMtcDefTbl1;
	BOOL    bPhotoMtcDefTbl2;
	BOOL    bKeepCurrentWB;
	BOOL    bKeepCurrentWBFromFile;
	BOOL    bTurnOnFRCalculate;
	BOOL    bTurnOffFRCalculate;
	BOOL    bDumpAFStatus;
	BOOL    bISPEnable;
	BOOL    bISPDisable;
	BOOL    bISPUpdateState;
	BOOL	bClearIBPEInternalBuffer;
} TVideoInMsgOptRec;

typedef struct auto_white_balance_retrieve_gain_value
{
	DWORD	dwAWBRedGain;
	DWORD	dwAWBBlueGain;
} TAWBRetGainValue;

typedef struct auto_focus_status
{
	DWORD dwZoomRatio;

} TAFStatus;	

typedef struct motion_real_res_2_default_system
{
	FLOAT			fXTranslate;
	FLOAT			fYTranslate;
	FLOAT			fXScale;
	FLOAT			fYScale;
} TMotionReal2Default;

// each motion result
typedef struct motion_result_info
{
	DWORD			dwIndex;
	BYTE			byPercent;
	BYTE			byThreshold;
	DWORD           dwLeft;
	DWORD           dwTop;
	DWORD           dwRight;
	DWORD           dwBottom;
#if defined(_MD_TRIGER_ROI_)
	DWORD           dwCfgLeft;
	DWORD           dwCfgTop;
	DWORD           dwCfgWidth;
	DWORD           dwCfgHeight;
#endif	
} TMotionResultInfo;

typedef struct motion_result_package
{
	DWORD	dwMotionNum;
	TMotionResultInfo	atMtnRetInfo[MAX_MOTION_WIN_NUM];
} TMotionResultPack;

typedef struct user_data
{
	DWORD 	dwUserDataLength;
	TMotionReal2Default tMtnR2D;
	DWORD	dwMotionNum;
	BOOL	bMotionEnable;
} TUserData;

typedef struct venc_videoin_init_private
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
	DWORD  	dwLDCRefFrameDRAMLoc; 	// ldc_ref_frame

	DWORD 	dwDMACOutputDRAMLoc;	// dmac_output and ibpe_output, it means sharedbuffer dram location
	DWORD   dwVICOutputDRAMLoc;		// vic_output, it means dmac_input and ibpe_input dram location
	
} _TVideoInInitNeedInfo;

#if defined(_VMA_IBPE)
typedef struct png_format
{
	png_structp		png_ptr;
	png_infop 		info_ptr;
	png_bytep 		*row_pointers;
	BYTE	*pbyPNGBuffer;
} TPNGStruct;
typedef struct ibpe_privacy_mask
{
	DWORD dwPMaskWidth;
	DWORD dwPMaskHeight;
	DWORD dwPMaskStride;
	DWORD dwPMaskSz;
	BYTE *pbyPMask;
	DWORD dwWidth;
	DWORD dwHeight;
	BYTE *pbyBitMask;
	HANDLE hBMaskObj;
	TPNGStruct *ptPNGStruct;
	HANDLE     hMemMgrObj;
}TIBPEPMask;
#endif

typedef struct venc_videoin_info
{
	//! device number of sensor input
	DWORD      			dwDevNum;
	//! string pointer of config file name
	CHAR       			*szConfigFile;
	//! recorde parent thread PID
	DWORD               dwParentPID;
	//! a callback parameter instance
	DWORD               dwCallbackInstance;
	//! SharedBuffer MinorNum
	DWORD               dwSharedBufferMinorNum;
	//! start semaphore to protect loop contorl
	sem_t				smStart;
	//! stop emaphore to protect loop control
	sem_t              	smEnd;
	//! message semaphore to protect share data (bReMsg_flag)
	sem_t              	smMsgOptions;
	//! options semaphore to protect option datas
	sem_t              	smOptions;
	//! thread instance
	pthread_t			ptThreadID;
	//! handle of VMA memory manager
	HANDLE     hMemMgrObj;
	//! handle of harware video capture
	HANDLE     	hVideoCapObj;
	//! handle of Image back-end process
	HANDLE     	hIBPObj;
	BOOL		bInitNrStrongBlendBuff;
	//! handle of MassMemAccess process
	HANDLE     hMassMemAccObj;
	//! handle of DeInterlacer process
	HANDLE     hDeInterlacer;
	//! handle of Shared buffer
	HANDLE		hSharedBuferObj;
	//! handle of XMLWrapper
	HANDLE		hXMLWrapperObj;
	//! handle of Frame rate control
	HANDLE 				hFRCtrlObj;
	BOOLEAN bFrameRateCtrlEnable;
	//! handle of PrivacyMask
	HANDLE		hPMaskObj;
	//! handle of DPC
	HANDLE  hDPCObject;
	//! string pointer of shared buffer path
	CHAR                *szSharedBufferPath;
	//! Image Back End Process state parameter
#ifdef _VMA_IBPE
	TImgBackEndProcessState tIBPState;
#endif	
	//! IBPE enable or not by config file
	//	BOOL 		bIBPEEnable;
	//! MassMemAccess state parameter -- using to do DAMC
	TMassMemAccessState		tMassMemAccState;
	//! file descriptor of Shared buffer
	int			iShardBufferFD;
	//! Flag of inner loop running
	BOOL       bRunning;
	//! ExitInnerLoop
	BOOL       	bExitInnerLoop;
	//! Terminate the out loop
	BOOL       bTerminateThread;    
	//! Reset config option used, it will change to TRUE
	BOOL		bReConf_flag;
	//! some option be modified by signal
	BOOL       bOptionsModified;
	//! if need to retrieve VideoCap Status, it will set to TRUE, either AWB or AE
	BOOL		bRetVideoCapStatus;
	//! option recoder
	TVideoInOptRec      tVideoInOptRec;
	//! if privacy mask configurations
	BOOL       	bPMaskReconfig;
#ifdef _VMA_IBPE
	//! privacy mask infomation
	TPMaskCfg 	tPMaskCfg;	
#endif	
	//! Motion detect reconfiguration
	BOOL       bMotionReconfig;
	//! motion informationBUFF_SIZE 
	TMotionCfg tMotionCfg;
	//! Photometric reconfig
	BOOL 			bPhotoMtcReconfig;
	//! photometric information
	TPhotoMtcCfg 	tPhotoMtcCfg;
	//! if need to rewrite Photometric user define table 1, it will set to TRUE
	BOOL    	bPhotoMtcDefTbl1;
	//! if need to rewrite Photometric user define table 2, it will set to TRUE
	BOOL    	bPhotoMtcDefTbl2;
	//! if need to keep the current WB, it will be set to TRUE.
	BOOL     	bKeepCurrentWB;
	//!if need to keep the current WB from file, it will be set to TRUE. 
	BOOL		bKeepCurrentWBFromFile;
	//!the current WB gain value
	TAWBRetGainValue tAWBRetGainValue;
	//!the cuurent AF status
	TAFStatus tAFStatus;
	//!if need to get the AF status, it will be set to TRUE.
	BOOL bDumpAFStatus;
	//! IBPE reconfiguration
	BOOL       bIBPReconfig;
#ifdef _VMA_IBPE
	//! IBPE config information
	TVideoInIBPOptions tIBPOpt;
#ifdef _VMA_IBPE_BACK_DOOR
	PVOID pvEESensitivity;
	PVOID pvEEStrength;
	PVOID pvEE3x3Sensitivity;
	PVOID pvEE3x3Alpha;
	PVOID pvEE3x3Strength;
	PVOID pvDI;
	PVOID pvNR2D;
	PVOID pvNR2D1Ref;
	PVOID pvNR2D2Ref;
	PVOID pvNR3DMA0CurrWeight;
	PVOID pvNR3DMA0RefStrength;
	PVOID pvNR3DMA1CurrWeight;
	PVOID pvNR3DMA1RefStrength;
#endif
#endif
	//! if command fifo (Message interface) receive data, it will change to TRUE
	BOOL		bReMsg_flag;		
	//! record current status
	EVideoInStatusType eviStatus;
	//!	status change
	BOOL		bStatusChange;
	
	TMotionResultPack	tMotionResPack;
	
	DWORD				dwVideoCapOutIndex;
	
	DWORD               dwCurtCapWidth;
	DWORD               dwCurtCapHeight;
	
	TMotionReal2Default tMtnR2D;

#ifdef _DUMP_VIDEOIN
	//! Video Capture output file
	FILE                *pfCapOutput;
#endif // _DUMP_VIDEOIN

#ifdef _DUMP_SHAREDBUFFERDATA
	//! Sharedbuffer data output file 
	FILE				*pfSharedBufferDataOut;
#endif //	_DUMP_SHAREDBUFFERDATA	

	// private init infomation
	_TVideoInInitNeedInfo _tInitNeedInfo;

	//! Message - option recoder
	TVideoInMsgOptRec	tMsgOptRec;

	//! if need dump after VideoCap_GetBuf, it will set to TRUE
	BOOL				bDumpVideoCapBuf;	
	//! if need dump from SharedBuffer, it will set to TRUE
	BOOL				bDumpSharedBuf;

	//! if need show framerate calculate, it will set to TRUE
	BOOL                bFRCalculate;
	DWORD               dwFRCalculateFrameCount;
	
	DWORD				dwPhotoLDCTblSize;
	WORD				*pwPhotoLDCRefFrame;
	
	BOOL bISPEnable;
	BOOL bISPUpdateState;
	
	int  fdIRCut;
	int  fdIRCutUART;

	TVideoCapState      atVideoCapState[BUFF_SIZE];  
	DWORD               adwVideoCapIndex[BUFF_SIZE];  // get buffer and record the VideoCap index
	TVideoFrameBuffer   atVideoCapOutVFB[BUFF_SIZE];

	/*Privacy mask relatvies*/
	//! string pointer of bitmap path
	CHAR          szBitmapPath[MAX_FILE_PATH_LEN];
	//! the field means the width of allocated memory for captured buffer
	DWORD 	dwAllocatedMemForCapBufWidth;
#ifdef _VMA_IBPE
	TIBPEPMask tIBPEPMask;
	TMotionDetectWindow  *aptMotionDetectResultInfo[BUFF_SIZE];
	TImgBackEndProcessBuffInfo atImgBackEndProcessBuffInfo[BUFF_SIZE];
#endif	
#ifdef _PRE_NR_
	TVideoFrameBuffer   atIBPEOutVFB[BUFF_SIZE];
	TVideoFrameBuffer   atIBPEInVFB[BUFF_SIZE];
	BOOL bTwiceIBPEEnable;
	BOOL bSecondPhaseIBPEParameterUpdated;
	HANDLE     	hSencodPhaseIBPObj;
	BOOL		bSencodPhaseInitNrStrongBlendBuff;
#ifdef _VMA_IBPE
	TImgBackEndProcessState tSecondPhaseIBPState;
#endif		
#endif
	DWORD dwOutputSharedBufferNum;
	BOOL	bClearIBPEInternalBuffer;
#ifdef _AUTOSCENE_	
	BOOL  bRestISPPROCESSStatus;
#endif

#ifdef _YUV422_
	BYTE *pbyYUV420FrameBuff;
#endif
} TVideoInInfo;

SCODE VencVideoIn_InitIBPEPMask(HANDLE hIBPEPMask, char *szConfigFile);
SCODE VencVideoIn_IBPEReadBitMask(HANDLE hIBPEPMask, CHAR *szFilePath);
SCODE VencVideoIn_ReleaseIBPEPMask(HANDLE hIBPEPMask);

SCODE VencVideoIn_InitVML(TVideoInInfo *ptVideoInInfo);
SCODE VencVideoIn_InitStates(TVideoInInfo *ptVideoInInfo);
SCODE VencVideoIn_InitConfigParam(TVideoInInfo *ptVideoInInfo);
void VencVideoIn_SetOptions(HANDLE hVideoInProcess, TVideoInOptions tVideoInOpt);
void VencVideoIn_ReleaseVML(TVideoInInfo *ptVideoInInfo);
SCODE VencVideoIn_LoadConfig(HANDLE hObject);
void* VencVideoIn_Loop(HANDLE hInstance);
void VencVideoIn_Process(HANDLE hObject);


#ifdef _VMA_IBPE
void VencVideoIn_IBPEONLY_Handler(TVideoInInfo *ptVideoInInfo);
#ifdef _PRE_NR_
void VencVideoIn_TWICEIBPE_Handler(TVideoInInfo *ptVideoInInfo);
#endif
#endif
void VencVideoIn_NOPREPROCESS_Handler(TVideoInInfo *ptVideoInInfo);

#endif //_VENC_VIDEOIN_LOCAL_H_
