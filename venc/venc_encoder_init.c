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
 * venc_encoder_init.c
 *
 * \brief
 * Initial each required component
 *
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#include "venc_encoder_local.h"

extern TXmlWrapperTreeMap ptEncoderTreeMap[];

/* ========================================================================== */
#ifdef _JPEG_SNAPSHOT_


void VencEncoder_SetSnapshotOutputSharedBufferPath(HANDLE hObject, 
                                           const CHAR *szSharedBufferPath, 
                                           SDWORD len)
{   
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
    CHAR szSrdBufPath[len+1];
    CHAR *byTemp;
    
    snprintf(szSrdBufPath, len+1, "%s", szSharedBufferPath);
    byTemp = strrchr(strdup(szSrdBufPath), 'r');
    byTemp++;

    ptEncoderInfo->dwSnapshotSharedBufferMinorNum = 
        (DWORD)strtol(byTemp, NULL, 10);
    printf("[VENC_ENCODER] Encoder snapshot shared buffer output device node = %u !!\n", 
           ptEncoderInfo->dwSnapshotSharedBufferMinorNum);
}

/* ========================================================================== */
void VencEncoder_SetSnapshotBufNum(HANDLE hObject, 
                                           const CHAR *szTemp, 
                                           SDWORD len)
{   
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
    CHAR szValue[len+1];
    
    memset(szValue, 0x0, sizeof(szValue));
    snprintf(szValue, len+1, "%s", szTemp);
    ptEncoderInfo->dwSnapshotBufNum = strtoul(szValue, NULL, 10);
}

SCODE VencEncoder_InitSnapshotOutputSharedBuffer(TEncoderInfo *ptEncoderInfo, 
                                         const _TEncoderInitNeedInfo *const _ptInitNeedInfo)
{
    TSharedBuffMgrInitOptions tInitOptions;
    
    HANDLE hXmlWrapperObj;
    TXmlWrapperInitOptions tXmlWrapperInitOptions;
    
    //! Initial XMLWrapper to get Sharedbuffer MinorNum 
    TXmlWrapperTreeMap ptShardBufferMap[] = 
    {    
        {"root/venc_encoder/snapshot_sharebuffer/path", NULL, &VencEncoder_SetSnapshotOutputSharedBufferPath, NULL},
        {"root/venc_encoder/snapshot_sharebuffer/buff_num", NULL, &VencEncoder_SetSnapshotBufNum, NULL},
        {NULL, NULL, NULL}
    };
    tXmlWrapperInitOptions.dwVersion = XMLWRAPPER_VERSION;
    if (XmlWrapper_Initial(&hXmlWrapperObj, &tXmlWrapperInitOptions) != S_OK) {
        return S_FAIL;
    }
    if (XmlWrapper_SetHandler(hXmlWrapperObj, ptShardBufferMap,NULL) != S_OK) {
        printf("%s %d : XmlWrapper_SetHandler Fail! \n", __FILE__, __LINE__);
        return S_FAIL;
    }
    if (XmlWrapper_ReadFile_UsrDefFunc(ptEncoderInfo->szConfigFile, 
                                       hXmlWrapperObj, 
                                       ptEncoderInfo) 
        != S_OK) {

        printf("%s %d : XmlWrapper_ReadFile_UsrDefFunc %s Fail! \n", 
               __FILE__, __LINE__, ptEncoderInfo->szConfigFile);
        return S_FAIL;
    }
    if (XmlWrapper_Release(&hXmlWrapperObj) != S_OK) {
        printf("Release XmlWrapper Fail! \n");
        return S_FAIL;
    }
    
    // Initial Encoder output sharedbuffergr
    tInitOptions.dwVersion      = SHAREDBUFFMGR_VERSION;
    tInitOptions.dwBufNum       = ptEncoderInfo->dwSnapshotBufNum;
    tInitOptions.dwHeaderSize   = 16;
    tInitOptions.dwDataSize     = _ptInitNeedInfo->dwBitStreamSize;
    tInitOptions.eAlignType     = ALIGN_TYPE_8_BYTE;
    tInitOptions.pObjectMem     = NULL;
    tInitOptions.dwBusIndex     = _ptInitNeedInfo->dwBitStreamOutputDRAMLoc;
    tInitOptions.dwMinorNum     = ptEncoderInfo->dwSnapshotSharedBufferMinorNum;
    tInitOptions.eProcessRole   = esbmprWriter;
    tInitOptions.byInitialVal   = 0x0;
    if (SharedBuffMgr_Initial(&(ptEncoderInfo->hSnapshotSrdObj), &tInitOptions) 
        != S_OK) {

        printf("[VENC_ENCODER] Initialize Encoder Snapshot output shared buffer fail "
               "!!\n");
        return S_FAIL;
    }    
    printf("[VENC_ENCODER] Initialize Encoder output shared buffer success, "
           "the Minor number is %u, size = %u !!\n", 
           tInitOptions.dwMinorNum, tInitOptions.dwDataSize);
    return S_OK;
}
#endif // _JPEG_SNAPSHOT_


SCODE VencEncoder_InitMassMemAccess(HANDLE *phMassMemAccObject, DWORD dwVideoMaxWidth, DWORD dwVideoMaxHeight)
{
	TMassMemAccessInitOptions tMassMemAccessInitOpt;
	TMassMemAccessOptions tOptions;
	
	memset(&(tMassMemAccessInitOpt), 0, sizeof(TMassMemAccessInitOptions));
	tMassMemAccessInitOpt.dwVersion = MASSMEMACCESS_VERSION;
	tMassMemAccessInitOpt.dwMaxFrameWidth = dwVideoMaxWidth;
	tMassMemAccessInitOpt.dwMaxFrameHeight = dwVideoMaxHeight;
	tMassMemAccessInitOpt.dwMaskBusNum = 0;
	tMassMemAccessInitOpt.bMaskInit = FALSE;
	tMassMemAccessInitOpt.pObjectMem = NULL;
	if (MassMemAccess_Initial(phMassMemAccObject, &tMassMemAccessInitOpt) != S_OK) {
		printf("[VENC_ENCODER] Mass memory access initial err !!\n");
		return S_FAIL;
	}
	tOptions.adwUserData[0] = FALSE;
	tOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_CACHE_COHERENCE;
	if (MassMemAccess_SetOptions(*phMassMemAccObject, &tOptions) != S_OK)
	{
		printf("[VENC_ENCODER]Fail to set up cache operation !!\n");
		return S_FAIL;
	}
	return S_OK;
	
}
/* ========================================================================== */
DWORD VencEncoder_GetMemory(HANDLE hMemObject,
		DWORD dwBusNum, 
		DWORD dwSize, 
		EAlignType eAlignType)
{
	TMemMgrState tMemMgrState;
	tMemMgrState.dwBusNum = dwBusNum;
	tMemMgrState.dwSize = dwSize;
	tMemMgrState.eAlignType = eAlignType;

	return (MemMgr_GetMemory(hMemObject, &tMemMgrState));
}

/* ========================================================================== */
void VencEncoder_SetOutputSharedBufferPath(HANDLE hObject, 
		const CHAR *szSharedBufferPath, 
		SDWORD len)
{   
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	CHAR szSrdBufPath[len+1];
	CHAR *byTemp;

	snprintf(szSrdBufPath, len+1, "%s", szSharedBufferPath);
	byTemp = strrchr(strdup(szSrdBufPath), 'r');
	byTemp++;

	ptEncoderInfo->dwOutputSharedBufferMinorNum = 
		(DWORD)strtol(byTemp, NULL, 10);
	//printf("[VENC_ENCODER] Encoder shared buffer output device node = %u !!\n", 
	//       ptEncoderInfo->dwOutputSharedBufferMinorNum);
}

/* ========================================================================== */
SCODE VencEncoder_InitXMLWrapper(HANDLE *phXMLWrapperObject)
{
	TXmlWrapperInitOptions txpInitOptions;

	txpInitOptions.dwVersion = XMLWRAPPER_VERSION;
	if (XmlWrapper_Initial(phXMLWrapperObject, &txpInitOptions) != S_OK) {
		fprintf(stderr, "[VENC_ENCODER] Initial XMLWrapper object fail !! \n");
		return S_FAIL;
	}
	if (XmlWrapper_SetHandler((*phXMLWrapperObject), ptEncoderTreeMap, NULL) 
			!= S_OK) {

		fprintf(stderr, "[VENC_ENCODER] Set XMLWrapper handler fail !! \n");
		return S_FAIL;
	}

	return S_OK;
}

/* ========================================================================== */
SCODE VencEncoder_InitFrameRateCtrl(HANDLE *phFRCtrlObj)
{
	TFrameRateInitOptions tFRCtrlInitOptions;

	tFRCtrlInitOptions.dwVersion    = FRAMERATECTRL_VERSION;
	tFRCtrlInitOptions.dwFrameRate  = 0;   //! initial disable framerate control

	if (FrameRateCtrl_Initial(phFRCtrlObj, &tFRCtrlInitOptions) != S_OK) {
		fprintf(stderr, 
				"[VENC_ENCODER] Initial Frame rate control object fail !! \n");
		return S_FAIL;
	}

	return S_OK;
}
#ifdef _VMA_MEBE
/* ========================================================================== */
SCODE VencEncoder_InitMp4VEnc(TEncoderInfo *ptEncoderInfo, 
		const _TEncoderInitNeedInfo *const _ptInitNeedInfo)
{
	TMp4VEncInitOptions tMp4VEncInitOpt;  
	memset(&(tMp4VEncInitOpt), 0, sizeof(TMp4VEncInitOptions)); 

	tMp4VEncInitOpt.dwVersion           = MP4VENC_VERSION;
	tMp4VEncInitOpt.dwMaxFrameWidth     = _ptInitNeedInfo->dwVideoMaxWidth;
	tMp4VEncInitOpt.dwMaxFrameHeight    = _ptInitNeedInfo->dwVideoMaxHeight;
	tMp4VEncInitOpt.dwEncWidth          = _ptInitNeedInfo->dwVideoCapWidth;
	tMp4VEncInitOpt.dwEncHeight         = _ptInitNeedInfo->dwVideoCapHeight;
	tMp4VEncInitOpt.dwInWidth           = _ptInitNeedInfo->dwVideoMaxWidth;
	tMp4VEncInitOpt.dwInHeight          = _ptInitNeedInfo->dwVideoMaxHeight;
	tMp4VEncInitOpt.dwInHorzOffset      = 0;
	tMp4VEncInitOpt.dwInVertOffset      = 0;    
	tMp4VEncInitOpt.dwTimeResolution    = 30;   //1000;
	tMp4VEncInitOpt.dwPacketSize        = 700;
	tMp4VEncInitOpt.dwTicksPerTR        = 1;    //001;
	tMp4VEncInitOpt.dwIntraVOPInterval  = 7;

	tMp4VEncInitOpt.dwQuant             = 7; 
	//! g_tVideoSlaveCfg.dwMPEG4QuantValue; default value

	//tMp4VEncInitOpt.dwFcodeForward      = 1; obsolete from Mp4VEnc V8.0.0.0 
	tMp4VEncInitOpt.dwBitRate           = 384000; 
	//! g_tVideoSlaveCfg.dwMPEG4Bitrate; default value
	tMp4VEncInitOpt.dwIntraMBRefreshNum = 0;

	tMp4VEncInitOpt.bEnableShortHeader  = FALSE;
	tMp4VEncInitOpt.bUseGVOP            = FALSE;
	tMp4VEncInitOpt.bUseCI              = TRUE;

	if ((tMp4VEncInitOpt.dwInWidth * tMp4VEncInitOpt.dwEncHeight) >= (320 * 240)) 
	{
		tMp4VEncInitOpt.bDisableResyncMarker = TRUE;
	} 
	else
		tMp4VEncInitOpt.bDisableResyncMarker = FALSE;

	// syslog(LOG_INFO,
	//        "[venc_encoder_init]tMp4VEncInitOpt.bDisableResyncMarker = %d "
	//        "tMp4VEncInitOpt.dwInWidth=%d tMp4VEncInitOpt.dwEncHeight=%d\n",
	//        tMp4VEncInitOpt.bDisableResyncMarker, tMp4VEncInitOpt.dwInWidth,
	//        tMp4VEncInitOpt.dwEncHeight);


	tMp4VEncInitOpt.bEnableDataPartitioned  = FALSE;
	tMp4VEncInitOpt.bEnableRVLC             = FALSE;
	tMp4VEncInitOpt.bEnableIntraVOPRefresh  = TRUE; 
	//! rename from Mp4VEnc V8.0.0.0
	tMp4VEncInitOpt.bUseDetailedVPHeader    = FALSE;
	tMp4VEncInitOpt.eRCType                 = RATE_CTRL_NONE;
	//! (g_tVideoSlaveCfg.dwMPEG4QualityType == VIDEO_QUALITY_FIX_BITRATE) ? 
	//!                                      RATE_CTRL_VQCB : RATE_CTRL_NONE;
	tMp4VEncInitOpt.dwBSBufSize             = 0;//ptEncoderInfo->_tInitNeedInfo.dwBitStreamSize;
	tMp4VEncInitOpt.dwTimeOut               = 1;
	tMp4VEncInitOpt.dwSemaphore             = (DWORD)NULL;
	tMp4VEncInitOpt.dwUserData              = 0;
	tMp4VEncInitOpt.eBufTypeFlags           = MPEGNONCIRCULAR;
	tMp4VEncInitOpt.pfnSend                 = NULL;
	tMp4VEncInitOpt.pfnEnter                = NULL;
	tMp4VEncInitOpt.pfnLeave                = NULL;
	tMp4VEncInitOpt.pObjectMem              = NULL;   
	tMp4VEncInitOpt.pbyYFrame               = NULL;
	tMp4VEncInitOpt.pbyCbFrame              = NULL;
	tMp4VEncInitOpt.pbyCrFrame              = NULL;
	tMp4VEncInitOpt.pbyEncUserData          = NULL;

	//! two init option for v10.0.0.0 new library interface
	//tMp4VEncInitOpt.dwStreamBusNum = 0;
	tMp4VEncInitOpt.dwStreamBusNum  = _ptInitNeedInfo->dwBitStreamOutputDRAMLoc;
	tMp4VEncInitOpt.dwFrameBusNum   = _ptInitNeedInfo->dwMp4VBusNum;
	//printf("[venc_encoder_init](%d)tMp4VEncInitOpt.dwFrameBusNum=%d\n",
	//       __LINE__,tMp4VEncInitOpt.dwFrameBusNum);

	if (ptEncoderInfo->dwH264MediaMemSize == 0) {
    	ptEncoderInfo->dwH264MediaMemSize = Mp4VEnc_QueryMediaMemSize(&tMp4VEncInitOpt);
	}
	if (ptEncoderInfo->pbyH264InternalBuf == NULL) {
		DWORD dwDramNum = -1;
#ifdef _1DRAM_VERSION_
		dwDramNum = 0;
#else
		dwDramNum = 1;
#endif		
		ptEncoderInfo->pbyH264InternalBuf = (BYTE *)VencEncoder_GetMemory(ptEncoderInfo->hMemMgrObj,
					                                                      dwDramNum,
            	        		            	                       	  ptEncoderInfo->dwH264MediaMemSize,
	        			                                        	      ALIGN_TYPE_1_BYTE);
	}
	tMp4VEncInitOpt.dwMediaBuffSize = ptEncoderInfo->dwH264MediaMemSize;
    tMp4VEncInitOpt.pbyMediaBuff = ptEncoderInfo->pbyH264InternalBuf;
	if (Mp4VEnc_Initial(&(ptEncoderInfo->hMp4VEncObj), &tMp4VEncInitOpt) 
			!= S_OK) {

		printf("Initialize MPEG-4 encoder object fail !!\n");
		return S_FAIL;
	}

	if (ptEncoderInfo->pdwPacketCountBuf == NULL) {
		ptEncoderInfo->pdwPacketCountBuf = 
			(DWORD *)calloc(tMp4VEncInitOpt.dwMaxPacketCount, sizeof(DWORD));
		if (ptEncoderInfo->pdwPacketCountBuf == NULL) {
			printf("[Video Process] Allocating pdwPacketCountBuf "
					"memory fail !!\n");
			return S_FAIL;
		}
		memset( ptEncoderInfo->pdwPacketCountBuf, 
				0, 
				tMp4VEncInitOpt.dwMaxPacketCount * sizeof(DWORD) );
	}
	ptEncoderInfo->dwMp4VTimeResolution = tMp4VEncInitOpt.dwTimeResolution;
	return S_OK;
}
#endif

/* ========================================================================== */
#ifdef _VMA_JEBE
SCODE VencEncoder_InitJPEGEnc(TEncoderInfo *ptEncoderInfo, 
		const _TEncoderInitNeedInfo *const _ptInitNeedInfo)
{
	TJPEGEncInitOptions tJPEGEncInitOpt;
	memset(&(tJPEGEncInitOpt), 0, sizeof(tJPEGEncInitOpt));

	tJPEGEncInitOpt.dwVersion           = JPEGENC_VERSION;
	tJPEGEncInitOpt.dwInWidth           = _ptInitNeedInfo->dwVideoMaxWidth;
	tJPEGEncInitOpt.dwInHeight          = _ptInitNeedInfo->dwVideoMaxHeight;
	tJPEGEncInitOpt.dwEncWidth          = _ptInitNeedInfo->dwVideoCapWidth;
	tJPEGEncInitOpt.dwEncHeight         = _ptInitNeedInfo->dwVideoCapHeight;
	tJPEGEncInitOpt.dwInHorzOffset      = 0;
	tJPEGEncInitOpt.dwInVertOffset      = 0;
	tJPEGEncInitOpt.dwQualityFactor     = 70;
	tJPEGEncInitOpt.dwRstIntNum         = 0;
	tJPEGEncInitOpt.bJFIFHdr            = FALSE;
	tJPEGEncInitOpt.bHighCompression    = FALSE;
	tJPEGEncInitOpt.eQTableType         = JPEGENC_Q_TABLE_TYPE_STANDARD;
	tJPEGEncInitOpt.eColorFlags         = COLOR;
	tJPEGEncInitOpt.ePixelFormatFlags   = YUV420;
	tJPEGEncInitOpt.dwBSBufSize         = 0;
	tJPEGEncInitOpt.dwTimeOut           = 1;
	tJPEGEncInitOpt.dwSemaphore         = (DWORD)NULL;
	tJPEGEncInitOpt.dwUserData          = 0;
	tJPEGEncInitOpt.eBufTypeFlags       = JPEGNONCIRCULAR;
	tJPEGEncInitOpt.pfnSend             = NULL;
	tJPEGEncInitOpt.pfnEnter            = NULL;
	tJPEGEncInitOpt.pfnLeave            = NULL;
	tJPEGEncInitOpt.hAppSendObject      = NULL;

	tJPEGEncInitOpt.pbyYFrame           = NULL;
	tJPEGEncInitOpt.pbyCbFrame          = NULL;
	tJPEGEncInitOpt.pbyCrFrame          = NULL;
	tJPEGEncInitOpt.pbyUserData         = NULL;
	tJPEGEncInitOpt.pObjectMem          = NULL;

	if (JPEGEnc_Initial(&(ptEncoderInfo->hJPEGEncObj), &tJPEGEncInitOpt) != S_OK ) 
	{
		printf("Initialize JPEG encoder object fail !!\n");
		return S_FAIL;
	}
#ifdef _JPEG_SNAPSHOT_
    if (JPEGEnc_Initial(&(ptEncoderInfo->hJPEGEncSnapShot), &tJPEGEncInitOpt) != S_OK) 
	{
		printf("Initialize JPEG encoder object for snapshot fail !!\n");
        return S_FAIL;
    }
  #if 0
    {
        TJPEGEncOptions tJPEGEncOpt;
        SCODE scRet;
        tJPEGEncOpt.eOptionFlags = VIDEO_CHANGE_QUANT;
        tJPEGEncOpt.adwUserData[0] = 40;
    	if (ptEncoderInfo->hJPEGEncSnapShot != NULL) {
	        scRet = JPEGEnc_SetOptions(ptEncoderInfo->hJPEGEncSnapShot, &tJPEGEncOpt);
	        assert(scRet == S_OK);
	        DBPRINT1("[VENC_ENCODER] JPEG Quant %u\n", tJPEGEncOpt.adwUserData[0]);
	    }        
    }
  #endif    
#endif // _JPEG_SNAPSHOT_
	return S_OK;
}
#endif // _VMA_JEBE

/* ========================================================================== */
#ifdef _H4EE
SCODE VencEncoder_InitH264Enc(TEncoderInfo *ptEncoderInfo, 
		const _TEncoderInitNeedInfo *const _ptInitNeedInfo)
{
	TH264EncInitOptions tH264EncInitOpt;
	memset(&(tH264EncInitOpt), 0, sizeof(tH264EncInitOpt));

	//! set initial option
	tH264EncInitOpt.dwVersion           = H264ENC_VERSION;
	tH264EncInitOpt.dwMaxFrameWidth     = _ptInitNeedInfo->dwVideoMaxWidth;
	tH264EncInitOpt.dwMaxFrameHeight    = _ptInitNeedInfo->dwVideoMaxHeight;
	tH264EncInitOpt.dwEncWidth          = _ptInitNeedInfo->dwVideoCapWidth;
	tH264EncInitOpt.dwEncHeight         = _ptInitNeedInfo->dwVideoCapHeight;
	tH264EncInitOpt.dwInWidth           = _ptInitNeedInfo->dwVideoMaxWidth;
	tH264EncInitOpt.dwInHeight          = _ptInitNeedInfo->dwVideoMaxHeight;
	tH264EncInitOpt.dwInHorzOffset      = 0;
	tH264EncInitOpt.dwInVertOffset      = 0;
#if 0
	if (_ptInitNeedInfo->bCMOSSensor == FALSE) {  
		tH264EncInitOpt.dwTicksPerSecond = 60000;//30 FPS
		tH264EncInitOpt.dwTicksPerFrame  = 1001;
	}
	else {
		/*
		tH264EncInitOpt.dwTicksPerSecond = 
			_ptInitNeedInfo->dwMaxCapFrameRate * 2;
		tH264EncInitOpt.dwTicksPerFrame  = 1;
		*/
		tH264EncInitOpt.dwTicksPerSecond = 60000*2;//60 FPS
		tH264EncInitOpt.dwTicksPerFrame  = 1001;
	}
#endif

	tH264EncInitOpt.dwTicksPerSecond = _ptInitNeedInfo->dwMaxCapFrameRate * 2000;
	tH264EncInitOpt.dwTicksPerFrame  = 1000;
	//printf("[venc_encoder_init](%d)dwTicksPerSecond=%d dwTicksPerFrame=%d\n", __LINE__, tH264EncInitOpt.dwTicksPerSecond, tH264EncInitOpt.dwTicksPerFrame);
	tH264EncInitOpt.dwQuant                 = 20;
	//! g_tVideoSlaveCfg.dwH264QuantValue;
	tH264EncInitOpt.dwIntraSliceInterval    = 30;       //! I frame interval
	tH264EncInitOpt.bIntraSliceRefreshEn    = TRUE;
	tH264EncInitOpt.bAdvancedSkipModeEn     = FALSE;    //! default
	tH264EncInitOpt.bConstrainedIntraPredEn = FALSE;
	tH264EncInitOpt.bIntra16x16PredEn       = FALSE;    //! default
	tH264EncInitOpt.bInterChmaSADEn         = FALSE;
	tH264EncInitOpt.dwProfile               = 0;        //! high profile
	tH264EncInitOpt.dwBitRate               = 3840000; 
	//! g_tVideoSlaveCfg.dwH264Bitrate; default value
	tH264EncInitOpt.eRCType                 = RATE_CTRL_NONE;
	//! (g_tVideoSlaveCfg.dwMPEG4QualityType == VIDEO_QUALITY_FIX_BITRATE) ? 
	//!                                     RATE_CTRL_VQCB : RATE_CTRL_NONE;
	tH264EncInitOpt.dwIntraRefreshMBNum     = 0;        //! default
	tH264EncInitOpt.bEncByteStreamHdr       = TRUE;
	//! two init option for v2.0.0.0 new library interface
	//tH264EncInitOpt.dwStreamBusNum          = 0;
	tH264EncInitOpt.dwStreamBusNum  = _ptInitNeedInfo->dwBitStreamOutputDRAMLoc;
	tH264EncInitOpt.dwFrameBusNum   = _ptInitNeedInfo->dwH264BusNum;

	tH264EncInitOpt.dwBSBufSize     = 0;           // PB_BUFFER_SIZE (1024*1024)
	tH264EncInitOpt.dwTimeOut       = 1;                // 500
	tH264EncInitOpt.dwSemaphore     = (DWORD)NULL;      // (DWORD)&mutexEnc
	tH264EncInitOpt.dwUserData      = 0;                // 1
	tH264EncInitOpt.eBufTypeFlags   = H264NONCIRCULAR;  // H264CIRCULAR
	tH264EncInitOpt.pfnSend         = NULL;             // SendData
	tH264EncInitOpt.pfnEnter        = NULL;             // EnterCritical
	tH264EncInitOpt.pfnLeave        = NULL;             // LeaveCritical

	tH264EncInitOpt.pObjectMem      = NULL;
	tH264EncInitOpt.pbyYFrame       = NULL; // pbyFrame;
	tH264EncInitOpt.pbyCbFrame      = NULL; // pbyFrame + dwInWidth*dwInHeight;
	tH264EncInitOpt.pbyCrFrame      = NULL; 
	// pbyFrame + dwInWidth*dwInHeight + ((dwInWidth*dwInHeight)>>2);
	tH264EncInitOpt.pbyEncUserData  = NULL; // pbyEncUserData;
	//! for ROI
	tH264EncInitOpt.sdwObjectQp     = 0;    //-26~+25
	tH264EncInitOpt.pbyObjectMask   = NULL;
	//! 0~15 : 0 means 30 fps, 15 means 45 fps
	tH264EncInitOpt.dwSpeedNum      = 0;

	tH264EncInitOpt.dwRefFrameNum   = 2;
	tH264EncInitOpt.dwSearchRangeX  = 4;     //! 1 => +-16, 2 => +-32, 4 => +-60
	tH264EncInitOpt.dwSearchRangeY  = 2;     //! 1 => +-16, 2 => +-32, 4 => +-60

	//printf("[VENC_ENCODER_INIT](%d)H.264 Fast Mode: %s\n", 
	//       __LINE__, tH264EncInitOpt.bFastModeEnable ? "Enable" : "Disable");
	printf("[VENC_ENCODER_INIT](%d)H.264 Speed Num: %u\n",
			__LINE__, tH264EncInitOpt.dwSpeedNum); 
	ptEncoderInfo->dwH264TimeResolution = 
		( (tH264EncInitOpt.dwTicksPerSecond / tH264EncInitOpt.dwTicksPerFrame) 
		  >> 1 );
		
	if (ptEncoderInfo->dwH264MediaMemSize == 0) {
    	ptEncoderInfo->dwH264MediaMemSize = H264Enc_QueryMediaMemSize(&tH264EncInitOpt);
	}
    tH264EncInitOpt.dwMediaBuffSize = ptEncoderInfo->dwH264MediaMemSize;

	if (ptEncoderInfo->pbyH264InternalBuf == NULL)
	{
		DWORD dwDramNum = -1;
#ifdef _1DRAM_VERSION_
		dwDramNum = 0;
#else
		dwDramNum = 1;
#endif		
		ptEncoderInfo->pbyH264InternalBuf = (BYTE *)VencEncoder_GetMemory(ptEncoderInfo->hMemMgrObj,
				                                                      dwDramNum,
                    		            	                       	  tH264EncInitOpt.dwMediaBuffSize,
																	  ALIGN_TYPE_1_BYTE);
		printf("[VENC_ENCODER_INIT](%d)Allocate Internal buffer for H264: %d bytes!!\n", __LINE__, tH264EncInitOpt.dwMediaBuffSize); 
	}
    
    tH264EncInitOpt.pbyMediaBuff = ptEncoderInfo->pbyH264InternalBuf;
	
	if (H264Enc_Initial(&(ptEncoderInfo->hH264EncObj), &tH264EncInitOpt) != S_OK) 
	{
		printf("Initialize H264 encoder object fail !!\n");
		return S_FAIL;
	}
	return S_OK;
}
#endif // _H4EE
/* ========================================================================== */
#ifdef _SVC
static void GetMaxLayerNum(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	DWORD *pdwMaxLayerNum = (DWORD *)hObject;
	CHAR szTemp[len+1];
	memset(szTemp, 0x0, sizeof(szTemp));

	snprintf(szTemp, len+1, "%s", szElement);
	*pdwMaxLayerNum = strtoul(szTemp, NULL, 10);
}
static SCODE  QuerySVCMaxLayerNum(char *szConfigFile,DWORD *pdwMaxLayerNum)
{
	HANDLE hXmlWrapperObj;
	TXmlWrapperInitOptions tXmlWrapperInitOptions;

	
	TXmlWrapperTreeMap ptMap[] = 
	{
		{"root/venc_encoder/svc_max_layer_num", NULL, &GetMaxLayerNum, NULL},
		{NULL, NULL, NULL}
	};
		
	memset(&tXmlWrapperInitOptions, 0, sizeof(TXmlWrapperInitOptions));
		
	tXmlWrapperInitOptions.dwVersion = XMLWRAPPER_VERSION;
		
	if (XmlWrapper_Initial(&hXmlWrapperObj, &tXmlWrapperInitOptions) != S_OK)
	{
			return S_FAIL;
	}
		
	if (XmlWrapper_SetHandler(hXmlWrapperObj, ptMap, NULL) != S_OK)
	{
			printf("%s %d : XmlWrapper_SetHandler Fail! \n", __FILE__, __LINE__);
			return S_FAIL;
	}

	if (XmlWrapper_ReadFile_UsrDefFunc(szConfigFile, hXmlWrapperObj, pdwMaxLayerNum) != S_OK)
	{
			printf(__FILE__":%s:%d [IBPE] XmlWrapper_ReadBuf_UsrDefFunc fail\n", __func__, __LINE__);
			return S_FAIL;
	}

	if (XmlWrapper_Release(&hXmlWrapperObj) != S_OK)
	{
			printf("Release XmlWrapper Fail! \n");
			return S_FAIL;
	}

	return S_OK;
}
SCODE VencEncoder_InitSVCEnc(TEncoderInfo *ptEncoderInfo, 
		const _TEncoderInitNeedInfo *const _ptInitNeedInfo)
{
	TSVCEncInitOptions tSVCEncInitOpt;
	memset(&(tSVCEncInitOpt), 0, sizeof(tSVCEncInitOpt));

	// set initial option
	tSVCEncInitOpt.dwVersion            = SVCENC_VERSION;
	tSVCEncInitOpt.dwMaxFrameWidth      = _ptInitNeedInfo->dwVideoMaxWidth;
	tSVCEncInitOpt.dwMaxFrameHeight     = _ptInitNeedInfo->dwVideoMaxHeight;
	tSVCEncInitOpt.dwEncWidth           = _ptInitNeedInfo->dwVideoCapWidth;
	tSVCEncInitOpt.dwEncHeight          = _ptInitNeedInfo->dwVideoCapHeight;
	tSVCEncInitOpt.dwInWidth            = _ptInitNeedInfo->dwVideoMaxWidth;
	tSVCEncInitOpt.dwInHeight           = _ptInitNeedInfo->dwVideoMaxHeight;
	tSVCEncInitOpt.dwInHorzOffset       = 0;
	tSVCEncInitOpt.dwInVertOffset       = 0;
#if 0	
	if (_ptInitNeedInfo->bCMOSSensor == FALSE) {  
		tSVCEncInitOpt.dwTicksPerSecond   = 60000;
		tSVCEncInitOpt.dwTicksPerFrame    = 1001;
	}
	else {
		/*
		tSVCEncInitOpt.dwTicksPerSecond = _ptInitNeedInfo->dwMaxCapFrameRate * 2;
		tSVCEncInitOpt.dwTicksPerFrame  = 1;
		*/
		tSVCEncInitOpt.dwTicksPerSecond   = 60000*2;
		tSVCEncInitOpt.dwTicksPerFrame    = 1001;
	}
#endif
	tSVCEncInitOpt.dwTicksPerSecond = _ptInitNeedInfo->dwMaxCapFrameRate * 2000;
	tSVCEncInitOpt.dwTicksPerFrame  = 1000;
	printf("[venc_encoder_init](%d)dwTicksPerSecond=%d dwTicksPerFrame=%d\n", __LINE__, tSVCEncInitOpt.dwTicksPerSecond, tSVCEncInitOpt.dwTicksPerFrame);
	tSVCEncInitOpt.dwQuant                  = 20;
	//! g_tVideoSlaveCfg.dwH264QuantValue;
	tSVCEncInitOpt.dwIntraSliceInterval     = 30;       //! I frame interval
	tSVCEncInitOpt.bIntraSliceRefreshEn     = TRUE;
	tSVCEncInitOpt.bAdvancedSkipModeEn      = FALSE;    //! default
	tSVCEncInitOpt.bConstrainedIntraPredEn  = FALSE;
	tSVCEncInitOpt.bIntra16x16PredEn        = FALSE;    //! default
	tSVCEncInitOpt.bInterChmaSADEn          = FALSE;
	tSVCEncInitOpt.dwProfile                = 2;        //! high profile
	tSVCEncInitOpt.dwBitRate                = 3840000; 
	//! g_tVideoSlaveCfg.dwH264Bitrate; default value
	tSVCEncInitOpt.eRCType                  = RATE_CTRL_NONE;
	//! (g_tVideoSlaveCfg.dwMPEG4QualityType == VIDEO_QUALITY_FIX_BITRATE) ? 
	//!                                     RATE_CTRL_VQCB : RATE_CTRL_NONE;
	tSVCEncInitOpt.dwIntraRefreshMBNum      = 0;        //! default
	tSVCEncInitOpt.bEncByteStreamHdr        = TRUE;
	//! two init option for v2.0.0.0 new library interface
	//tH264EncInitOpt.dwStreamBusNum = 0;
	tSVCEncInitOpt.dwStreamBusNum   = _ptInitNeedInfo->dwBitStreamOutputDRAMLoc;
	tSVCEncInitOpt.dwFrameBusNum    = _ptInitNeedInfo->dwSVCBusNum;

	tSVCEncInitOpt.dwBSBufSize      = 0;           // PB_BUFFER_SIZE (1024*1024)
	tSVCEncInitOpt.dwTimeOut        = 1;                // 500
	tSVCEncInitOpt.dwSemaphore      = (DWORD)NULL;      // (DWORD)&mutexEnc
	tSVCEncInitOpt.dwUserData       = 0;                // 1
	tSVCEncInitOpt.eBufTypeFlags    = H264NONCIRCULAR;  // H264CIRCULAR
	tSVCEncInitOpt.pfnSend          = NULL;             // SendData
	tSVCEncInitOpt.pfnEnter         = NULL;             // EnterCritical
	tSVCEncInitOpt.pfnLeave         = NULL;             // LeaveCritical

	tSVCEncInitOpt.pObjectMem       = NULL;
	tSVCEncInitOpt.pbyYFrame        = NULL; // pbyFrame;
	tSVCEncInitOpt.pbyCbFrame       = NULL; // pbyFrame + dwInWidth*dwInHeight;
	tSVCEncInitOpt.pbyCrFrame       = NULL; 
	// pbyFrame + dwInWidth*dwInHeight + ((dwInWidth*dwInHeight)>>2);
	tSVCEncInitOpt.pbyEncUserData   = NULL; // pbyEncUserData;
	//! for ROI
	tSVCEncInitOpt.sdwObjectQp      = 0;    //-26~+25
	tSVCEncInitOpt.pbyObjectMask    = NULL;

	//tSVCEncInitOpt.bFastModeEnable = FALSE;
	tSVCEncInitOpt.dwSpeedNum      = 0;
	//! 0~15 : 0 means 30 fps, 15 means 45 fps
	tSVCEncInitOpt.dwRefFrameNum   = 1;
	tSVCEncInitOpt.dwSearchRangeX  = 4;     //! 1 => +-16, 2 => +-32, 4 => +-60
	tSVCEncInitOpt.dwSearchRangeY  = 2;     //! 1 => +-16, 2 => +-32, 4 => +-60

	//    printf("[VENC_ENCODER_INIT](%d)SVC Fast Mode: %s\n",
	//           __LINE__, tSVCEncInitOpt.bFastModeEnable?"Enable":"Disable"); 
	printf("[VENC_ENCODER_INIT](%d)SVC Speed Num: %u\n",
			__LINE__, tSVCEncInitOpt.dwSpeedNum); 

	//! for SVC
	tSVCEncInitOpt.bSVCSEIEn            = FALSE;
	tSVCEncInitOpt.bKeyFrameOnlyMode    = TRUE;
	tSVCEncInitOpt.dwLayerNum           = 1;
	QuerySVCMaxLayerNum(ptEncoderInfo->szConfigFile ,&tSVCEncInitOpt.dwLayerNum);
	printf("[venc_encoder_init](%d)MaxSVCMaxLayerNum=[%d](%s)\n",__LINE__,tSVCEncInitOpt.dwLayerNum,ptEncoderInfo->szConfigFile);
	
	ptEncoderInfo->dwSVCTimeResolution = 
		( (tSVCEncInitOpt.dwTicksPerSecond / tSVCEncInitOpt.dwTicksPerFrame) 
		  >> 1);
		
	if (ptEncoderInfo->dwH264MediaMemSize == 0) {
		printf("[venc_encoder_init](%d)\n",__LINE__);
    	ptEncoderInfo->dwH264MediaMemSize = SVCEnc_QueryMediaMemSize(&tSVCEncInitOpt);
		printf("[venc_encoder_init](%d)dwH264MediaMemSize=%d\n",__LINE__,ptEncoderInfo->dwH264MediaMemSize );
	}
    tSVCEncInitOpt.dwMediaBuffSize = ptEncoderInfo->dwH264MediaMemSize;

	if (ptEncoderInfo->pbyH264InternalBuf == NULL)
	{
		DWORD dwDramNum = -1;
#ifdef _1DRAM_VERSION_
		dwDramNum = 0;
#else
		dwDramNum = 1;
#endif		
		ptEncoderInfo->pbyH264InternalBuf = (BYTE *)VencEncoder_GetMemory(ptEncoderInfo->hMemMgrObj,
				                                                      dwDramNum,
                    		            	                       	  tSVCEncInitOpt.dwMediaBuffSize,
																	  ALIGN_TYPE_1_BYTE);
		printf("[VENC_ENCODER_INIT](%d)Allocate Internal buffer for SVC: %d bytes!!\n", __LINE__,tSVCEncInitOpt.dwMediaBuffSize); 
	}
    
	tSVCEncInitOpt.pbyMediaBuff = ptEncoderInfo->pbyH264InternalBuf;	
		
#if 0
	printf("tSVCEncInitOpt.dwStreamBusNum=%d\n", tSVCEncInitOpt.dwStreamBusNum);
	printf("tSVCEncInitOpt.dwFrameBusNum=%d\n", tSVCEncInitOpt.dwFrameBusNum);

	printf("tSVCEncInitOpt.dwMaxFrameWidth=%d\n", 
			tSVCEncInitOpt.dwMaxFrameWidth); 
	printf("tSVCEncInitOpt.dwMaxFrameHeight=%d\n",
			tSVCEncInitOpt.dwMaxFrameHeight); 
	printf("tSVCEncInitOpt.dwEncWidth=%d\n", tSVCEncInitOpt.dwEncWidth);
	printf("tSVCEncInitOpt.dwEncHeight=%d\n", tSVCEncInitOpt.dwEncHeight);
	printf("tSVCEncInitOpt.dwInWidth=%d\n", tSVCEncInitOpt.dwInWidth);
	printf("tSVCEncInitOpt.dwInHeight=%d\n", tSVCEncInitOpt.dwInHeight); 
	printf("tSVCEncInitOpt.dwTicksPerSecond=%d\n",
			tSVCEncInitOpt.dwTicksPerSecond);
#endif
	if (SVCEnc_Initial(&(ptEncoderInfo->hSVCEncObj), &tSVCEncInitOpt) != S_OK) {
		printf("Initialize SVC encoder object fail !!\n");
		return S_FAIL;
	}
	return S_OK;
}
#endif // _SVC
/* ========================================================================== */
SCODE VencEncoder_InitVideoInSharedBuffer(TEncoderInfo *ptEncoderInfo)
{
	TSharedBuffMgrInitOptions tInitOptions;

	// Initial sharedbuffergr
	tInitOptions.dwVersion      = SHAREDBUFFMGR_VERSION;
	tInitOptions.pObjectMem     = NULL;
	tInitOptions.dwMinorNum     = ptEncoderInfo->dwSharedBufferMinorNum;
	tInitOptions.eProcessRole   = esbmprReader;

	DBPRINT1("[VENC_ENCODER] dwMinorNum = %u ..\n", tInitOptions.dwMinorNum);

	if (SharedBuffMgr_Initial(&(ptEncoderInfo->hVideoInSrdObj), &tInitOptions) != S_OK) 
	{
		fprintf(stderr, "[VENC_ENCODER] Initialize shared buffer manager "
				"object fail !!\n");
		return S_FAIL;
	}    
	return S_OK;
}

/* ========================================================================== */
SCODE VencEncoder_InitOutputSharedBuffer(TEncoderInfo *ptEncoderInfo, 
		const _TEncoderInitNeedInfo *const _ptInitNeedInfo)
{
	TSharedBuffMgrInitOptions tInitOptions;

	HANDLE hXmlWrapperObj;
	TXmlWrapperInitOptions tXmlWrapperInitOptions;

	//! Initial XMLWrapper to get Sharedbuffer MinorNum 
	TXmlWrapperTreeMap ptShardBufferMap[] = 
	{    
		{"root/venc_encoder/output_sharebuffer/path", NULL, 
			&VencEncoder_SetOutputSharedBufferPath, NULL},
		{NULL, NULL, NULL}
	};
	tXmlWrapperInitOptions.dwVersion = XMLWRAPPER_VERSION;
	if (XmlWrapper_Initial(&hXmlWrapperObj, &tXmlWrapperInitOptions) != S_OK) {
		return S_FAIL;
	}
	if (XmlWrapper_SetHandler(hXmlWrapperObj, ptShardBufferMap,NULL) != S_OK) {
		printf("%s %d : XmlWrapper_SetHandler Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	if (XmlWrapper_ReadFile_UsrDefFunc(ptEncoderInfo->szConfigFile, 
				hXmlWrapperObj, 
				ptEncoderInfo) 
			!= S_OK) {

		printf("%s %d : XmlWrapper_ReadFile_UsrDefFunc %s Fail! \n", 
				__FILE__, __LINE__, ptEncoderInfo->szConfigFile);
		return S_FAIL;
	}
	if (XmlWrapper_Release(&hXmlWrapperObj) != S_OK) {
		printf("Release XmlWrapper Fail! \n");
		return S_FAIL;
	}

	// Initial Encoder output sharedbuffergr
	tInitOptions.dwVersion      = SHAREDBUFFMGR_VERSION;
	tInitOptions.dwBufNum       = 4;
	tInitOptions.dwHeaderSize   = SHAREDBUFFER_HEADERSIZE;
	tInitOptions.dwDataSize     = _ptInitNeedInfo->dwBitStreamSize;
	tInitOptions.eAlignType     = ALIGN_TYPE_8_BYTE;
	tInitOptions.pObjectMem     = NULL;
	tInitOptions.dwBusIndex     = _ptInitNeedInfo->dwBitStreamOutputDRAMLoc;
	tInitOptions.dwMinorNum     = ptEncoderInfo->dwOutputSharedBufferMinorNum;
	tInitOptions.eProcessRole   = esbmprWriter;
	tInitOptions.byInitialVal   = 0x0;
	if (SharedBuffMgr_Initial(&(ptEncoderInfo->hOutputSrdObj), &tInitOptions) 
			!= S_OK) {

		printf("[VENC_ENCODER] Initialize Encoder output shared buffer fail "
				"!!\n");
		return S_FAIL;
	}    
	printf("[VENC_ENCODER] Initialize Encoder output shared buffer success, "
			"the Minor number is %u, size = %u !!\n", 
			tInitOptions.dwMinorNum, tInitOptions.dwDataSize);
	//syslog(LOG_INFO, "[VENC_MEM] SharedBuffer for encoder output buffer : %d bytes\n",tInitOptions.dwBufNum*(tInitOptions.dwDataSize+tInitOptions.dwHeaderSize));

	return S_OK;
}

/* ========================================================================== */
SCODE VencEncoder_InitConfigParam(TEncoderInfo *ptEncoderInfo)
{
	//! initial tVideoSlaveCfg value
	ptEncoderInfo->tVideoSlaveCfg.dwCodecType           = -1;
	ptEncoderInfo->tVideoSlaveCfg.dwFrameRate           = -1;
	*(ptEncoderInfo->tVideoSlaveCfg.szResolution)       = '\0';
	ptEncoderInfo->tVideoSlaveCfg.ecMethod              = -1;
	ptEncoderInfo->tVideoSlaveCfg.dwMPEG4KeyInterval    = -1;
	ptEncoderInfo->tVideoSlaveCfg.ercfMPEG4QualityType  = -1;
	ptEncoderInfo->tVideoSlaveCfg.dwMPEG4QuantValue     = -1;
	ptEncoderInfo->tVideoSlaveCfg.dwMPEG4Bitrate        = -1;
	ptEncoderInfo->tVideoSlaveCfg.dwMJPEGQuantValue     = -1;
	ptEncoderInfo->tVideoSlaveCfg.dwH264KeyInterval     = -1;
	ptEncoderInfo->tVideoSlaveCfg.ercfH264QualityType   = -1;
	ptEncoderInfo->tVideoSlaveCfg.dwH264QuantValue      = -1;
	ptEncoderInfo->tVideoSlaveCfg.dwH264Bitrate         = -1;
	ptEncoderInfo->tVideoSlaveCfg.vfFormat              = -1;
	ptEncoderInfo->tVideoSlaveCfg.dwSVCKeyInterval      = -1;
	ptEncoderInfo->tVideoSlaveCfg.ercfSVCQualityType    = -1;
	ptEncoderInfo->tVideoSlaveCfg.dwSVCQuantValue       = -1;
	ptEncoderInfo->tVideoSlaveCfg.dwSVCBitrate          = -1;
	ptEncoderInfo->tVideoSlaveCfg.bSVCHDREnable         = -1;
	ptEncoderInfo->tVideoSlaveCfg.dwSVCLayerNum         = -1;
	ptEncoderInfo->tVideoSlaveCfg.dwH264MaxFrameRate = -1;
	return S_OK;
}



/* ========================================================================== */
SCODE VencEncoder_InitVML(TEncoderInfo *ptEncoderInfo)
{
	DWORD dwDataSize;
	DWORD dwWidthPadding;
	DWORD dwHeightPadding;
	DWORD dwVideoCodecMaskRet = 0;
#ifdef _VMA_IRE    
	TResizeInitOptions tResizeInitOptions; 
#endif    
	//! multiple of 16
	dwWidthPadding = 
		(((ptEncoderInfo->_tInitNeedInfo.dwVideoMaxWidth + 15) >> 4) << 4);
	dwHeightPadding = 
		(((ptEncoderInfo->_tInitNeedInfo.dwVideoMaxHeight + 15) >> 4) << 4);

	dwDataSize = (((dwWidthPadding * dwHeightPadding) * 3) >> 1); 
	//! YUV420 format

	//! get output bitstream buffer, maybe it needn't, 
	//! it is work around for H264Enc used.
	/* 
	   ptEncoderInfo->pbyEncOutputBuf =
	   (BYTE *)VencEncoder_GetMemory( ptEncoderInfo->hMemMgrObj,
	   0,  //! bus 0
	   MAX_BITSTREAM_SIZE,
	   ALIGN_TYPE_64_BYTE );
	   if (ptEncoderInfo->pbyEncOutputBuf == NULL) {
	   fprintf(stderr,
	   "[VENC_ENCODER] Allocate bitstream output framebuffer fail"
	   "!!\n");
	   return ERR_OUT_OF_MEMORY;
	   }
	   DBPRINT0("[VENC_ENCODER] Allocate bitstream output framebuffer"
	   "Success !!\n");
	 */
	ptEncoderInfo->tResizeOutBuf0.pbyBufY = 
		(BYTE *)VencEncoder_GetMemory(ptEncoderInfo->hMemMgrObj,
				ptEncoderInfo->_tInitNeedInfo.dwIREOutput0DRAMLoc,
				dwDataSize,
				ALIGN_TYPE_128_BYTE);

	if (ptEncoderInfo->tResizeOutBuf0.pbyBufY == NULL) {
		fprintf(stderr, "[VENC_ENCODER] Allocate ResizeOutBuf fail !!\n");
		return ERR_OUT_OF_MEMORY;
	}
	//syslog(LOG_INFO, "[VENC_MEM] tResizeOutBuf0.pbyBufY  : %d bytes\n", dwDataSize);
	DBPRINT0("[VENC_ENCODER] Allocate ResizeOutBuf Success !!\n");
#ifndef _SINGLE_BUFFER_
	ptEncoderInfo->tResizeOutBuf1.pbyBufY = 
		(BYTE *)VencEncoder_GetMemory(ptEncoderInfo->hMemMgrObj,
				ptEncoderInfo->_tInitNeedInfo.dwIREOutput1DRAMLoc,
				dwDataSize,
				ALIGN_TYPE_128_BYTE);
	if (ptEncoderInfo->tResizeOutBuf1.pbyBufY == NULL) {
		fprintf(stderr, "[VENC_ENCODER] Allocate ResizeOutBuf fail !!\n");
		return ERR_OUT_OF_MEMORY;
	}

	//syslog(LOG_INFO, "[VENC_MEM] tResizeOutBuf1.pbyBufY  : %d bytes\n", dwDataSize);
	DBPRINT0("[VENC_ENCODER] Allocate ResizeOutBuf Success !!\n");
#else
	ptEncoderInfo->tResizeOutBuf1.pbyBufY = ptEncoderInfo->tResizeOutBuf0.pbyBufY;
#endif // _SINGLE_BUFFER_	


	//! ===== Initial instance =====
	//! Initial VideoIn Shared Buffer
	if (VencEncoder_InitVideoInSharedBuffer(ptEncoderInfo) != S_OK) {
		fprintf(stderr, "[VENC_ENCODER] Initialize Shared buffer object fail "
				"!!\n");
		return S_FAIL;
	}
	DBPRINT0("[VENC_ENCODER] Initialize Shared buffer object Success !!\n");

	//! Initial Encoder output Shared Buffer
	if (VencEncoder_InitOutputSharedBuffer(ptEncoderInfo, 
				&(ptEncoderInfo->_tInitNeedInfo)) 
			!= S_OK) {
		fprintf(stderr, "[VENC_ENCODER] Initialize Shared buffer object fail "
				"!!\n");
		return S_FAIL;
	}
	DBPRINT0("[VENC_ENCODER] Initialize Shared buffer object Success !!\n");

	//! Initial XMLWrapper instance
	if (VencEncoder_InitXMLWrapper(&(ptEncoderInfo->hXMLWrapperObj)) != S_OK) {
		fprintf(stderr, "[VENC_ENCODER] Initial XMLWrapper object fail !!\n");
		return S_FAIL;    
	}
	DBPRINT0("[VENC_ENCODER] Initial XMLWrapper Success !!\n");

	//! Initial Frame rate control instance
	if (VencEncoder_InitFrameRateCtrl(&(ptEncoderInfo->hFRCtrlObj)) != S_OK) {
		fprintf(stderr, "[VENC_ENCODER] Initial Frame rate control object fail "
				"!!\n");
		return S_FAIL;
	}
	DBPRINT0("[VENC_ENCODER] Initial Frame rate control Success !!\n");

	printf("[venc_encoder_init](%d)VideoCodecInitMask:%08x\n", 
			__LINE__, ptEncoderInfo->_tInitNeedInfo.dwVideoCodecInitMask);
	
#ifdef _SVC
	//! Initial SVC encoder
	dwVideoCodecMaskRet = ptEncoderInfo->_tInitNeedInfo.dwVideoCodecInitMask & VIDEO_CODEC_SVC_INIT_BIT;
	if (dwVideoCodecMaskRet != 0) {
		if (VencEncoder_InitSVCEnc(ptEncoderInfo, &(ptEncoderInfo->_tInitNeedInfo)) != S_OK) {
			fprintf(stderr, "[VENC_ENCODER] Initialize SVC encoder object fail "
					"!!\n");
			return S_FAIL;
		}
		printf("[venc_encoder_init](%d) Initialize SVC encoder object Success "
				"!!\n",__LINE__);
	}
#endif // _SVC	
#ifdef _H4EE
	//! Initial H264 encoder
	dwVideoCodecMaskRet = ptEncoderInfo->_tInitNeedInfo.dwVideoCodecInitMask & VIDEO_CODEC_H264_INIT_BIT;
	if (dwVideoCodecMaskRet != 0) {
		if (VencEncoder_InitH264Enc(ptEncoderInfo, &(ptEncoderInfo->_tInitNeedInfo)) != S_OK) {

			fprintf(stderr, "[VENC_ENCODER] Initialize H264 encoder object fail"
					" !!\n");
			return S_FAIL;
		}
		printf("[venc_encoder_init](%d) Initialize H264 encoder object Success"
				" !!\n",__LINE__);
	}
#endif // _H4EE
	//! Initial MPEG-4 encoder
#ifdef _VMA_MEBE
	dwVideoCodecMaskRet = ptEncoderInfo->_tInitNeedInfo.dwVideoCodecInitMask & VIDEO_CODEC_MP4V_INIT_BIT;
	if (dwVideoCodecMaskRet != 0) {
		if (VencEncoder_InitMp4VEnc(ptEncoderInfo, &(ptEncoderInfo->_tInitNeedInfo)) != S_OK) {
			fprintf(stderr, "[VENC_ENCODER] Initialize MPEG-4 encoder object fail"
					" !!\n");
			return S_FAIL;
		}
		printf("[venc_encoder_init](%d) Initialize MPEG-4 encoder object "
				"Success !!\n",__LINE__);
	}
#endif // _VMA_MEBE
	//! Initial JPEG encoder
#ifdef _VMA_JEBE
	dwVideoCodecMaskRet = ptEncoderInfo->_tInitNeedInfo.dwVideoCodecInitMask & VIDEO_CODEC_JPEG_INIT_BIT;
	if (dwVideoCodecMaskRet != 0) {
		if (VencEncoder_InitJPEGEnc(ptEncoderInfo, &(ptEncoderInfo->_tInitNeedInfo)) != S_OK) {
			fprintf(stderr, "[VENC_ENCODER] Initialize JPEG encoder object fail"
					" !!\n");
			return S_FAIL;
		}
		printf("[venc_encoder_init](%d) Initialize JPEG encoder object Success"
				" !!\n",__LINE__);
	}
#endif // _VMA_JEBE


#ifdef _VMA_IRE
	//! Initial Resize handle
	memset(&(tResizeInitOptions), 0, sizeof(TResizeInitOptions));
	tResizeInitOptions.dwVersion  = RESIZE_VERSION;
	tResizeInitOptions.pObjectMem = NULL;
	if (Resize_Initial(&(ptEncoderInfo->hResizeObj), &tResizeInitOptions) != S_OK) {
		fprintf(stderr,"[VENC_ENCODER] Initial Resize object fail !!\n");
		return S_FAIL;
	}
	DBPRINT0("[VENC_ENCODER] Initialize Resize object Success !!\n");
#endif
	if (VencEncoder_InitMassMemAccess(&ptEncoderInfo->hMassMemAccObjY, ptEncoderInfo->_tInitNeedInfo.dwVideoMaxWidth, ptEncoderInfo->_tInitNeedInfo.dwVideoMaxHeight) != S_OK) {
		fprintf(stderr, "[VENC_ENCODER] Initialize MassMemAccess handle fail !!\n");
		return S_FAIL;
	}
	if (VencEncoder_InitMassMemAccess(&ptEncoderInfo->hMassMemAccObjCb, ptEncoderInfo->_tInitNeedInfo.dwVideoMaxWidth, ptEncoderInfo->_tInitNeedInfo.dwVideoMaxHeight) != S_OK) {
		fprintf(stderr, "[VENC_ENCODER] Initialize MassMemAccess handle fail !!\n");
		return S_FAIL;
	}
	if (VencEncoder_InitMassMemAccess(&ptEncoderInfo->hMassMemAccObjCr, ptEncoderInfo->_tInitNeedInfo.dwVideoMaxWidth, ptEncoderInfo->_tInitNeedInfo.dwVideoMaxHeight) != S_OK) {
		fprintf(stderr, "[VENC_ENCODER] Initialize MassMemAccess handle fail !!\n");
		return S_FAIL;
	}
	
	if (VencEncoder_InitMassMemAccess(&ptEncoderInfo->hMassMemAccObj, ptEncoderInfo->_tInitNeedInfo.dwVideoMaxWidth, ptEncoderInfo->_tInitNeedInfo.dwVideoMaxHeight) != S_OK) {
		fprintf(stderr, "[VENC_ENCODER] Initialize MassMemAccess handle fail !!\n");
		return S_FAIL;
	}
	
#ifdef _JPEG_SNAPSHOT_
    //! Initial snapshot output Shared Buffer
    if (VencEncoder_InitSnapshotOutputSharedBuffer(ptEncoderInfo, &(ptEncoderInfo->_tInitNeedInfo)) != S_OK) {
        fprintf(stderr, "[VENC_ENCODER] Initialize Shared buffer for snapshot fail "
                        "!!\n");
        return S_FAIL;
    }
    DBPRINT0("[VENC_ENCODER] Initialize Shared buffer for snapshot Success !!\n");
#endif // _JPEG_SNAPSHOT_
	return S_OK;
}

/* ========================================================================== */
void VencEncoder_ReleaseVML(TEncoderInfo *ptEncoderInfo)
{
	//! Free every buffer we allocate in EncoderInfo
	/*
	   if (MemMgr_FreeMemory(ptEncoderInfo->hMemMgrObj, 
	   (DWORD)ptEncoderInfo->pbyEncOutputBuf) 
	   != S_OK) {

	   fprintf(stderr,
	   "[VENC_ENCODER] Release bitstream output framebuffer buffer "
	   "fail !!\n");
	   }
	 */
	
	if (ptEncoderInfo->pbyH264InternalBuf != NULL) {
		if (MemMgr_FreeMemory(ptEncoderInfo->hMemMgrObj, (DWORD)ptEncoderInfo->pbyH264InternalBuf) != S_OK) {
			fprintf(stderr, 
					"[VENC_ENCODER] Release H264 Codec internal buffer fail !!\n");
		}
	}
	
	if (MemMgr_FreeMemory(ptEncoderInfo->hMemMgrObj, (DWORD)ptEncoderInfo->tResizeOutBuf0.pbyBufY) != S_OK) {
		fprintf(stderr, 
				"[VENC_ENCODER] Release resize output buffer 0 fail !!\n");
	}
#ifndef _SINGLE_BUFFER_
	if (MemMgr_FreeMemory(ptEncoderInfo->hMemMgrObj, (DWORD)ptEncoderInfo->tResizeOutBuf1.pbyBufY) != S_OK) {
		fprintf(stderr, 
				"[VENC_ENCODER] Release resize output buffer 1 fail !!\n");
	}
#endif // _SINGLE_BUFFER_
	if (ptEncoderInfo->pdwPacketCountBuf != NULL) {
		free(ptEncoderInfo->pdwPacketCountBuf);
	}

	//! ===== Release Instance =====
#ifdef _VMA_MEBE  
	//! Release MPEG-4 encoder
	if (ptEncoderInfo->hMp4VEncObj != NULL) {
		if (Mp4VEnc_Release(&(ptEncoderInfo->hMp4VEncObj)) != S_OK) {
			fprintf(stderr, 
					"[VENC_ENCODER] Release MPEG-4 encoder object fail !!\n");
		}
	}
#endif
#ifdef _VMA_JEBE
	//! Release JPEG encoder
	if (ptEncoderInfo->hJPEGEncObj != NULL) {
		if (JPEGEnc_Release(&(ptEncoderInfo->hJPEGEncObj)) != S_OK) {
			fprintf(stderr, 
					"[VENC_ENCODER] Release JPEG encoder object fail !!\n");
		}
	}
#endif // _VMA_JEBE
#ifdef _H4EE
	//! Release H264 encoder
	if (ptEncoderInfo->hH264EncObj != NULL) {
		if (H264Enc_Release(&(ptEncoderInfo->hH264EncObj)) != S_OK) {
			fprintf(stderr, 
					"[VENC_ENCODER] Release H264 encoder object fail !!\n");
		}
	}

#ifdef _JPEG_SNAPSHOT_
    //! Release JPEG encoder
    if (ptEncoderInfo->hJPEGEncSnapShot != NULL) {
        if (JPEGEnc_Release(&(ptEncoderInfo->hJPEGEncSnapShot)) != S_OK) {
            fprintf(stderr, 
                    "[VENC_ENCODER] Release JPEG encoder for snapshot fail !!\n");
        }
    }
#endif // _JPEG_SNAPSHOT_

#endif // _H4EE
#ifdef _SVC
	//! Release SVC encoder
	if (ptEncoderInfo->hSVCEncObj != NULL) {
		if (SVCEnc_Release(&(ptEncoderInfo->hSVCEncObj)) != S_OK) {
			fprintf(stderr, 
					"[VENC_ENCODER] Release SVC encoder object fail !!\n");
		}
	}
#endif // _SVC
#ifdef _VMA_IRE
	//! Release Resize handle
	if (ptEncoderInfo->hResizeObj != NULL) {        
		if (Resize_Release(&(ptEncoderInfo->hResizeObj)) != S_OK) {
			fprintf(stderr, "[VENC_ENCODER] Release Resize object fail !!\n");
		}
	}
#endif
   // Release MassMemAccess instance
   if (ptEncoderInfo->hMassMemAccObjY != NULL) {
	   if (MassMemAccess_Release(&(ptEncoderInfo->hMassMemAccObjY)) != S_OK) {
 	  	                 fprintf(stderr, "[VENC_ENCODER] Release MassMemAccess object fail !!\n");
		   
		}
	   
	}
	if (ptEncoderInfo->hMassMemAccObjCb != NULL) {
		
		if (MassMemAccess_Release(&(ptEncoderInfo->hMassMemAccObjCb)) != S_OK) {
			fprintf(stderr, "[VENC_ENCODER] Release MassMemAccess object fail !!\n");
			
		}
		
	}
	if (ptEncoderInfo->hMassMemAccObjCr != NULL) {
		if (MassMemAccess_Release(&(ptEncoderInfo->hMassMemAccObjCr)) != S_OK) {
			fprintf(stderr, "[VENC_ENCODER] Release MassMemAccess object fail !!\n");
			
		}
		
	}
	//! Release shared buffer instance
	if (SharedBuffMgr_Release(&(ptEncoderInfo->hVideoInSrdObj)) != S_OK) {
		fprintf(stderr, 
				"[VENC_ENCODER] Release shared buffer manager object fail !!"
				"\n");
	}    

	if (SharedBuffMgr_Release(&(ptEncoderInfo->hOutputSrdObj)) != S_OK) {
		fprintf(stderr, 
				"[VENC_ENCODER] Release shared buffer manager object fail !!"
				"\n");
	}
#ifdef _JPEG_SNAPSHOT_
    if (SharedBuffMgr_Release(&(ptEncoderInfo->hSnapshotSrdObj)) != S_OK) {
        fprintf(stderr, 
                "[VENC_ENCODER] Release shared buffer manager object for snapshot fail !!"
                "\n");
    }
#endif // _JPEG_SNAPSHOT_
	if (XmlWrapper_Release(&(ptEncoderInfo->hXMLWrapperObj)) != S_OK) {
		fprintf(stderr, "[VENC_ENCODER] Release xmlwrapper object fail !!\n");
	}

	if (FrameRateCtrl_Release(&(ptEncoderInfo->hFRCtrlObj)) != S_OK) {
		fprintf(stderr, 
				"[VENC_ENCODER] Release Frame rate control object fail !!\n");
	}
	return;
}

/* ========================================================================= */

