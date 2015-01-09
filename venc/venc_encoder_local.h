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
 * venc_encoder_local.h
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

#ifndef _VENC_ENCODER_LOCAL_H_
#define _VENC_ENCODER_LOCAL_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <syslog.h>
#include <assert.h>

#include "typedef.h"
#include "errordef.h"
#include "dbgdefs.h"
#include "videodef.h"
#include "videocomm.h"
#include "global_codec.h"
#include "ubuffer.h"

#include "venc_encoder.h"
#include "venc_encoder_conf.h"
#include "venc_encoder_h264.h"
#include "venc_encoder_jpeg.h"
#include "venc_encoder_mp4v.h"
#include "venc_encoder_svc.h"

#include "MemMgr.h"
#include "SharedBuffMgr.h"
#ifdef _VMA_IRE 
#include "Resize.h"
#endif

#include "xmlwrapper.h"
#include "frameratectrl.h"
#include "ROISelector.h"
#include "eptz.h"
#include "MassMemAccess.h"
#include "vencslave_txtoverlay.h"

#ifdef _1DRAM_VERSION_
	#define _SINGLE_BUFFER_
#endif

#define SHAREDBUFFER_HEADERSIZE 256
#define DIFF_SEC_INTERVAL 2
#define DIFF_MSEC_INTERVAL 2000

#define MIN_H264_ROI_ENC_INTERVAL 1
#define MAX_H264_ROI_ENC_INTERVAL 300

#define H264_MACRO_BLK_WIDTH 16
#define H264_MACRO_BLK_HEIGHT  16
#define H264_MACRO_BLK_SIZE (H264_MACRO_BLK_WIDTH*H264_MACRO_BLK_HEIGHT)

#define SVC_MACRO_BLK_WIDTH 16
#define SVC_MACRO_BLK_HEIGHT  16
#define SVC_MACRO_BLK_SIZE (SVC_MACRO_BLK_WIDTH*SVC_MACRO_BLK_HEIGHT)

#define ROI_MASK_BUF_BUS_NUM 1

#define QUALITY_PARAMETER_DUMP_PATH "/tmp/.qualty_parameter.dump"
#define ENCODER_DEFAULT_CONFIG_FILE    "/etc/conf.d/c0/vencencoder_conf.0.xml"

typedef SCODE (* FEncoder_RequestOutputBuffer)(HANDLE dwIntance, TSharedBuffMgrState *ptSharedBuffMgrState);
typedef SCODE (* FEncoder_ReleaseOutputBuffer)(HANDLE dwIntance, TSharedBuffMgrState* ptSharedbuffMgrState);
typedef SCODE (* FEncoder_GetOutputBufferFD)(HANDLE dwIntance, int *fd);

//! encoding mode
typedef enum video_process_mode
{
    vpmStreamEncJPEG    = 0x00000001,
    vpmStreamEncMP4V    = 0x00000002,
	vpmStreamEncH264	= 0x00000004,
	vpmStreamEncSVC = 0x00000008,
	vpmStreamEncode 	= 0x0000000f,
	#if 0
#ifdef _H4EE
	vpmStreamEncode 	= 0x00000007,
#else    
    vpmStreamEncode     = 0x00000003,
#endif //_H4EE
#endif 
    vpmMotionDetect     = 0x00000100,
    vpmSnapshot         = 0x00000200,
} EVideoProcessMode;

typedef enum vencslave_encoder_optiontype
{
    VENCSLAVE_ENCODER_OPTION_SET_CODEC = 1,
    VENCSLAVE_ENCODER_OPTION_SET_RESOLUTION,
    VENCSLAVE_ENCODER_OPTION_SET_FRAMERATE,
    VENCSLAVE_ENCODER_OPTION_SET_CONVERTINGMETHOD,
    VENCSLAVE_ENCODER_OPTION_SET_MPEG4_INTRAINTERVAL,
    VENCSLAVE_ENCODER_OPTION_SET_MPEG4_RATECTRL,
    VENCSLAVE_ENCODER_OPTION_SET_MPEG4_BITRATE,
    VENCSLAVE_ENCODER_OPTION_SET_MPEG4_QUANT,
    VENCSLAVE_ENCODER_OPTION_SET_MJPEG_QUANT,
    VENCSLAVE_ENCODER_OPTION_SET_H264_INTRAINTERVAL,
    VENCSLAVE_ENCODER_OPTION_SET_H264_RATECTRL,
    VENCSLAVE_ENCODER_OPTION_SET_H264_QUANT,
    VENCSLAVE_ENCODER_OPTION_SET_H264_BITRATE,
    VENCSLAVE_ENCODER_OPTION_SET_H264_MAXIMUM_FRAME_RATE,
    VENCSLAVE_ENCODER_OPTION_SET_SVC_INTRAINTERVAL,
    VENCSLAVE_ENCODER_OPTION_SET_SVC_RATECTRL,
    VENCSLAVE_ENCODER_OPTION_SET_SVC_QUANT,
    VENCSLAVE_ENCODER_OPTION_SET_SVC_BITRATE,
    VENCSLAVE_ENCODER_OPTION_SET_SVC_HDRENABLE,
    VENCSLAVE_ENCODER_OPTION_SET_ROI,
    VENCSLAVE_ENCODER_OPTION_SET_SVC_LAYERNUM,
	VENCSLAVE_ENCODER_OPTION_SET_H264_REFERENCE_FRAME_NUM,
	VENCSLAVE_ENCODER_OPTION_SET_H264_SEARCH_RANGE_X,
	VENCSLAVE_ENCODER_OPTION_SET_H264_SEARCH_RANGE_Y,
	VENCSLAVE_ENCODER_OPTION_SET_H264_MAX_FRAME_RATE,
	VENCSLAVE_ENCODER_OPTION_SET_H264_MIN_FRAME_RATE,
	VENCSLAVE_ENCODER_OPTION_SET_H264_MAX_QUANT,
	VENCSLAVE_ENCODER_OPTION_SET_H264_MIN_QUANT,
	VENCSLAVE_ENCODER_OPTION_SET_H264_DEBLOCKENABLE,
	VENCSLAVE_ENCODER_OPTION_SET_H264_PRFOFILE,
	VENCSLAVE_ENCODER_OPTION_SET_UNKNOWN
} EEncoderOptType;

/* ========================================================================== */
typedef struct vencslave_encoder_options
{
    EEncoderOptType     eenOptType;
    DWORD               adwUserData[3];
} TEncoderOptions;


//! Encoder setoption mp4v Recorder
typedef struct mp4v_option_record
{
    BOOL    bIntraInterval;
    DWORD   dwIntraInterval;
    
    BOOL    bSetRateControl;
    ERateCtrlFlags   ercfMPEG4QualityType;
    
    BOOL    bQuant;
    DWORD   dwQuant;
    
    BOOL    bBitrate;
    DWORD   dwBitrate;
    
    BOOL    bForceIntra;

	BOOL	bQualityGroup;
} TMp4VOptRec;

//! Encoder setoption jpeg Recorder
typedef struct jpeg_option_record
{
    BOOL    bQuant;
    DWORD   dwQuant;
} TJPEGOptRec;


//! Encoder setoption H264 Recorder
typedef struct h264_option_record
{
    BOOL    bIntraInterval;
    DWORD   dwIntraInterval;

    BOOL    bSetRateControl;
    ERateCtrlFlags   ercfH264QualityType;

    BOOL    bQuant;
    DWORD  	dwQuant;
	DWORD 	dwMaxQuant;
	DWORD 	dwMinQuant;

    BOOL    bBitrate;
    DWORD   dwBitrate;

    BOOL    bQualityGroup;
    BOOL    bForceIntra;
	
	DWORD dwSearchRangeX;
	DWORD dwSearchRangeY;
	BOOL bSearchRangeGroup;
	
	DWORD dwRefFrameNum;
	BOOL bRefFrameNum;
	
	DWORD dwMaxFrameRate;
	DWORD dwMinFrameRate;
	BOOL  bFrameRateGroup;
	
	BOOL bDeblock; 
	BOOL bDeblockEnable;
	
	BOOL  bChangeProfile;
	DWORD dwProfile;//0: baseline, 1: main profile, 2: high profile.
} TH264OptRec;

//! Encoder setoption SVC Recorder
typedef struct svc_option_record
{
    BOOL    bIntraInterval;
    DWORD   dwIntraInterval;

    BOOL    bSetRateControl;
    ERateCtrlFlags   ercfSVCQualityType;

    BOOL    bQuant;
    DWORD   dwQuant;

    BOOL    bBitrate;
    DWORD   dwBitrate;

    BOOL    bForceIntra;
    BOOL    bQualityGroup;
    
    BOOL bHDREnable;
    BOOL bSetHDREnable;
    
    DWORD dwLayerNum;
    BOOL bSetLayerNum;
    
    
} TSVCOptRec;


//! Encoder setoption overall Recorder
typedef struct prestting_option_record
{
	//! setting codec type
    BOOL    bCodec;
    DWORD   dwCodecType;
	//! cropping/squeezing
	BOOL	bConvertingMethod;
	EConvertingMethod ecMethod;
    //! setting framerate
    BOOL 	bFrameRate;
    DWORD	dwFrameRate;
    //! setting resolution
    BOOL    bResolution;
    EVideoFormat    vfFormat;
    //! setting text overlay
    BOOL			bTextOverlay;
    TTextOverlayOptions tTextOverlayOptions;   
  
} TPreSetOptRec;

//! Encoder message overall Recorder
typedef struct venc_encoder_msg_record
{
	BOOL	bForceCI;
	BOOL	bForceIntra;
#ifdef VIRTUAL_PTZ
	BOOL	bVPTZCmd;
	BOOL	bVPTZPanUpdate;
    float   flPanValue;
	BOOL	bVPTZTiltUpdate;
    float   flTiltValue;
	BOOL	bVPTZZoomUpdate;
    float   flZoomValue;
	BOOL	bVPTZSpeedUpdate;
    float   flSpeedValue;
	BOOL	bVPTZEnableUpdate;
	BOOL	bVPTZEnableValue;
#endif //VIRTUAL_PTZ
    BOOL    bTurnOnFRCalculate;
    BOOL    bTurnOffFRCalculate;

	DWORD dwVisitors;	
	DWORD dwCodecType;
	DWORD dwCtrlMode;
	DWORD dwQuant;
	DWORD dwBitrate;
	BOOL bQualityParameterChanged;
	BOOL bDumpQualityParameter;
	BOOL bSetROI;
#ifdef _JPEG_SNAPSHOT_	
	 BOOL    bGetSnapshot;
#endif // _JPEG_SNAPSHOT_
		
} TEncoderMsgOptRec;
typedef struct ptz_value_record
{
    float   flPanValue;
    float   flTiltValue;
    float   flZoomValue;
    float   flSpeedValue;
} TPTZValue;


typedef struct roi_real_res_2_default_system
{
	FLOAT			fXTranslate;
	FLOAT			fYTranslate;
	FLOAT			fXScale;
	FLOAT			fYScale;
} TROIReal2Default;

#define VIDEO_CODEC_JPEG_INIT_BIT 0x00000001
#define VIDEO_CODEC_MP4V_INIT_BIT 0x00000002
#define VIDEO_CODEC_H264_INIT_BIT 0x00000004
#define VIDEO_CODEC_SVC_INIT_BIT 0x00000008
typedef struct venc_encoder_init_private
{
	//! sensor max support size
	DWORD	dwVideoMaxWidth;
	DWORD	dwVideoMaxHeight;
	//! capture size
	DWORD	dwVideoCapWidth;
	DWORD	dwVideoCapHeight;
	//! bit stream size
	DWORD	dwBitStreamSize;
   	DWORD  	dwIREOutput0DRAMLoc;		// ire_output0
   	DWORD  	dwIREOutput1DRAMLoc;		// ire_output1
   	DWORD  	dwBitStreamOutputDRAMLoc;	// bitstream_output
   	DWORD  	dwMp4VBusNum;			// mp4v_bus_num
   	DWORD  	dwH264BusNum;			// h264_bus_num
	DWORD	dwSVCBusNum;
    BOOL	bInterlaceScan;         // interlace scan or progressive
    DWORD	dwMaxCapFrameRate;
    BOOLEAN	bCMOSSensor;
    DWORD	dwVideoCodecInitMask;
} _TEncoderInitNeedInfo;

typedef struct crop_info
{

  DWORD dwStartX;
  DWORD dwStartY;
  DWORD dwWidth;
  DWORD dwHeight;
  
}TCropInfo;
typedef struct roi_rectangle_win
{
	TROIRectangle tROIRectangle;
	BOOL bActive;

}TROIRectangleWin;


typedef struct venc_encoder_info
{
    //! string pointer of config file name
    CHAR       			*szConfigFile;
    //! recorde parent thread PID
    DWORD               dwParentPID;
    //! a callback parameter instance
    DWORD               dwCallbackInstance;
    //! VideoIn SharedBuffer MinorNum
    DWORD               dwSharedBufferMinorNum;
	//! ID, to verify which stream is.
	DWORD				dwCurId;
    //! start semaphore to protect loop contorl
    sem_t				smStart;
    //! stop emaphore to protect loop control
    sem_t              	smEnd;
    //! options semaphore to protect share data (bReConf_flag)
    sem_t              	smOptions;
    //! message semaphore to protect share data (bReMsg_flag)
    sem_t              	smMsgOptions;
    //! thread instance
    pthread_t			ptThreadID;

    //! encoder process mode
    EVideoProcessMode   eVPMode;
	//! handle of VMA memory manager
	HANDLE				hMemMgrObj;
    //! ShareBuff object that get output buffer
    HANDLE              hOutputSrdObj;	// original name hJPEGSrdObj
	//! handle of share buffer
	HANDLE				hVideoInSrdObj;	// original name hSharedBuferObj
    //! JPEG - handle of JPEG encoder
    HANDLE              hJPEGEncObj;
    //! H264 - handle of H264 encoder
    HANDLE              hH264EncObj;
    //! MPEG4 - handle of MPEG4 encoder
    HANDLE              hMp4VEncObj;
    //! SVC-handle of SVC encoder
    HANDLE		hSVCEncObj;
	//! handle of VMA Resize instance
	HANDLE				hResizeObj;
	//! handle of XMLWrapper
	HANDLE				hXMLWrapperObj;
	//! handle of Frame rate control
	HANDLE 				hFRCtrlObj;
#ifdef _VMA_IRE	
	//! resize state
	TResizeState tResizeState;
#endif
    //! H264 - TimeResolution
    DWORD               dwH264TimeResolution;
    //! MPEG4 - TimeResolution
    DWORD               dwMp4VTimeResolution;
    //! SVC -TimeResolution
    DWORD		dwSVCTimeResolution;

	//! Encoder-StreamServer share buffer get buffer function pointer
	FEncoder_RequestOutputBuffer pfnRequestOutputBuffer;
	//! Encoder-StreamServer share buffer release buffer function pointer
	FEncoder_ReleaseOutputBuffer pfnReleaseOutputBuffer;
	//! Encoder-StreamServer share buffer get file descriptor function pointer
	FEncoder_GetOutputBufferFD pfnGetOutputBufferFD;

    //! Resize output buffer
	TVideoFrameBuffer   tResizeOutBuf0;
	TVideoFrameBuffer   tResizeOutBuf1;

    TTextOverlayOptions tTextOverlayData;
    TTextOverlayOptions	tTextOverlayOptForConfig;
	TVideoSlaveCfg		tVideoSlaveCfg;

	//! shared buffer file descriptor for select used
	int					iVideoInSrdFD;
	//! VideoIn sharedbuffer state
	TSharedBuffMgrState tSrdBufMgrState;

	//! record previous status about resize
	BOOL				bPreNeedResizeState;
    //! Codec Change
    BOOL				bCodecChange;
	//! Current vFormat recode
    EVideoFormat        vfCurtFormat;
	//! H264 - output buffer, maybe it needn't
//    BYTE                *pbyEncOutputBuf;
	//! MPEG4 - packet mode packet size info buff
    DWORD               *pdwPacketCountBuf;
	// when call Encoder_Start, bRunning will be set TRUE
    // when call Encoder_Stop, bRunning will be set FALSE
    BOOL                bRunning;
    // when call Encoder_Stop, bExitInnerLoop will be set FALSE,
    // and let it to wait thread to set this flag be TRUE    
    BOOL                bExitInnerLoop;
    //! Terminate the out loop
    BOOL				bTerminateThread;    
    //! Reset config option used, it will change to TRUE
    BOOL				bReConf_flag;	
	//! if command fifo (Message interface) receive data, it will change to TRUE
	BOOL				bReMsg_flag;
	//! To decide it should encode or not
	BOOL				bDoEncode;
	//! operation flag
    BOOL                bOptionsModified;
    //! MPEG4 - operation flag
    BOOL                bMp4VOptionsModified;
    //! JPEG - operation flag
    BOOL                bJPEGOptionsModified;
	//! H264 - operation flag
	BOOL				bH264OptionsModified;
	//! SVC - operation flag
	BOOL				bSVCOptionsModified;
	//! JPEG - Sequence No
	DWORD				dwJPEGSeqNo;
	//! MPEG4 - Sequence No
	DWORD				dwMp4vSeqNo;
	//! H264 - Sequence No
	DWORD				dwH264SeqNo;
	//! SVC - Sequence No
	DWORD				dwSVCSeqNo;
	//! PreSet recoder
    TPreSetOptRec    	tPreSetOptRec;
    //! MPEG4 - option recoder
    TMp4VOptRec      	tMp4VOptRec;
    //! JPEG - option recoder
    TJPEGOptRec      	tJPEGOptRec;
	//! H264 - option recoder
    TH264OptRec			tH264OptRec;
    //! SVC - option recoder
    TSVCOptRec			tSVCOptRec;
	//! Message - option recoder
	TEncoderMsgOptRec	tMsgOptRec;
	//! need Conf?
    BOOL			    bForceCI;
	//! 1 or 0, to decide which buffer to use
	DWORD				dwSwitchIndex;
	//! CalculateSize function refer and setting
    DWORD               dwCurtCapWidth;
    DWORD               dwCurtCapHeight;
    DWORD               dwReqWidth;
    DWORD               dwReqHeight;
    DWORD               dwReqWidthPad;
    DWORD               dwReqHeightPad;
	BOOL 				bNeedResize;
	BOOL				bNeedResizeChange;
	DWORD				dwCropWidth;
	DWORD				dwCropHeight;
	//! record previous frame Capture size info
	DWORD				dwResizePreFrameCapWidth;
	DWORD				dwResizePreFrameCapHeight;
	DWORD				dwResizePreFrameReqWidthPad;
	DWORD				dwResizePreFrameReqHeightPad;
	//! Output SharedBuffer MinorNum
	DWORD				dwOutputSharedBufferMinorNum;
	EConvertingMethod   ecMethod;

// file pointer to open file to dump
#ifdef _DUMP_ENCODER
    //! MPEG4 - bitstream output file
    FILE                *pfMp4VOutput;
    //! JPEG - bitstream output file
    FILE                *pfJPEGOutput;
    DWORD				dwFrameCount;
    //! H264 - bitstream output file
    FILE                *pfH264Output;
    BOOL bFirstIntra;//For dumping H.264 bitstream
    FILE		*pfSVCOutput;
#endif // _DUMP_ENCODER

#ifdef _DUMP_VIDEOINDATA
	FILE				*pfVideoInData;
#endif // _DUMP_VIDEOINDATA

#ifdef _DUMP_RESIZE
	FILE				*pfResizeData;
#endif // _DUMP_RESIZE

#ifdef VIRTUAL_PTZ
	//! PTZ change?
	BOOL				bVPTZCmd;
	BOOL				bVPTZSpeedUpdate;    
	// new ptz handle
	HANDLE				hEPTZObj;
    TPTZValue           tPTZValue;
    BOOL                bVPTZReset;
    DWORD               dwPTZFullViewWidth;
    DWORD               dwPTZFullViewHeight;
#endif //VIRTUAL_PTZ
	// private init infomation
	_TEncoderInitNeedInfo _tInitNeedInfo;
	
	// record the previous timestamp for Codec_RESTART used
	DWORD dwLastSecond;
	
	DWORD   dwH264EncFrameCount;
	DWORD   dwH264TotalBytes;
	
	DWORD   dwJPEGEncFrameCount;
	DWORD   dwJPEGTotalBytes;
	
	DWORD   dwMP4VEncFrameCount;
	DWORD   dwMP4VTotalBytes;
	
	DWORD   dwSVCEncFrameCount;
	DWORD   dwSVCTotalBytes;
	
	TROICfg tROICfg;
	TROIReal2Default tROI2D;
	BOOL bROIReconfig;


    //! if need show framerate calculate, it will set to TRUE
    BOOL      bFRCalculate;
    DWORD     dwFRCalculateFrameCount;
    
    TCropInfo tCropInfo;
    BOOL bRandomCropping;
    BOOL bChangeRandomCropping;
	BOOL bNeed2Recalculate;
	BOOL bDoNotDMACopy;

	TROIRectangleWin atROIRectangleWin[MAX_ROI_WIN_NUM];
	
	HANDLE                  hMassMemAccObjY;
    HANDLE                  hMassMemAccObjCb;
    HANDLE                  hMassMemAccObjCr;
    TMassMemAccessState     tMassMemAccStateY;
    TMassMemAccessState     tMassMemAccStateCb;
    TMassMemAccessState     tMassMemAccStateCr;
	
	HANDLE                  hMassMemAccObj;
	TMassMemAccessState     tMassMemAccState;
    
    BOOL                    bNeedTextOverlayChange;
    //! record previous status about text overlay enable/disable state
    BOOL                    bPreNeedTextOverlayState; 
#ifdef _JPEG_SNAPSHOT_	
	BOOL                    bGetSnapshot;
	HANDLE                  hJPEGEncSnapShot;
	//! Snapshot SharedBuffer MinorNum
	DWORD                               dwSnapshotSharedBufferMinorNum;
	//! ShareBuff object that for snapshot output
	HANDLE                  hSnapshotSrdObj;    // original name hJPEGSrdObj
	DWORD                   dwSnapshotBufNum;
	DWORD                   dwSnapshotCountdown;
#ifdef _DUMP_JPEG_FILE
	FILE                *pfJPEGOutput4Snapshot;
	DWORD				dwFrameCount4Snapshot;
#endif // _DUMP_JPEG_FILE
#endif // _JPEG_SNAPSHOT_
	DWORD				dwH264MediaMemSize;
	BYTE 				*pbyH264InternalBuf;
	
	BOOL 				bUseOneDMATextoverlay;
	//! The following field is for recording the resolution format of random cropping.
	DWORD				dwVideoResolutionFormatIndex;
     
} TEncoderInfo;

SCODE VencEncoder_RequestOutputBuffer(HANDLE hObject, TSharedBuffMgrState* ptSharedbuffMgrState);
SCODE VencEncoder_ReleaseOutputBuffer(HANDLE hObject, TSharedBuffMgrState* ptSharedbuffMgrState);
SCODE VencEncoder_GetOutputBufferFD(HANDLE hObject, int *fd);

SCODE VencEncoder_InitVML(TEncoderInfo *ptEncoderInfo);
void VencEncoder_ReleaseVML(TEncoderInfo *ptEncoderInfo);
void* VencEncoder_Loop(HANDLE hInstance);
SCODE VencEncoder_LoadConfig(HANDLE hObject);
void VencEncoder_SetOptions(HANDLE hEncoderProcess, TEncoderOptions tEncoderOpt);
void VencEncoder_SetTextOverlayOptions(HANDLE hEncoderProcess, TTextOverlayOptions *ptTextOverlayOpt);
SCODE VencEncoder_InitConfigParam(TEncoderInfo *ptEncoderInfo);

SCODE VencEncoder_ROISetoptions(TEncoderInfo *ptEncoderInfo);

#endif //_VENC_ENCODER_LOCAL_H_
