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
 * venc_display_init.c
 *
 * \brief
 * Initial each required component.
 *
 * \date
 * 2009/09/11
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#if defined(_VPL_VOC)

#include "venc_display_local.h"

extern TXmlWrapperTreeMap   ptDisplayTreeMap[];

static void SetOutputFormat       ( HANDLE      hObject, 
		const CHAR  *szOutputFormat, 
		SDWORD      len);
static void SetOutputResolution   ( HANDLE      hObject, 
		const CHAR  *szOutputResolution, 
		SDWORD      len);
static void SetI2CDevice          ( HANDLE      hObject, 
		const CHAR  *szI2CDevice, 
		SDWORD      len);
static void SetOutputEnable       ( HANDLE      hObject,
		const CHAR  *szParam, 
		SDWORD      len);
static void SetOutputFreq         ( HANDLE      hObject, 
		const CHAR  *szParam, 
		SDWORD      len);

/* ============================================================================================= */
SCODE VencDisplay_ParseInitNeedConfig(TDisplayInfo *ptInfo)
{
	HANDLE                  hXmlWrapperObj;
	TXmlWrapperInitOptions  tXmlWrapperInitOptions;
	SCODE                   scRet                   = S_FAIL;

	//! Initial XMLWrapper to get all relative config
	TXmlWrapperTreeMap ptDispInitNeedTreeMap[] = 
	{
		{"root/venc_display/output",            NULL, &SetOutputEnable,     NULL},        
		{"root/venc_display/output_format",     NULL, &SetOutputFormat,     NULL},    
		{"root/venc_display/output_resolution", NULL, &SetOutputResolution, NULL},        
		{"root/venc_display/output_freq",       NULL, &SetOutputFreq,       NULL},
		{"root/venc_display/i2c_device",        NULL, &SetI2CDevice,        NULL},
		{NULL, NULL, NULL}
	};

	tXmlWrapperInitOptions.dwVersion = XMLWRAPPER_VERSION;
	scRet = XmlWrapper_Initial(&hXmlWrapperObj, &tXmlWrapperInitOptions);
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: XmlWrapper_Initial - 0x%08x\n", 
				DISP_PRE, scRet);
		return S_FAIL;
	}    
	scRet = XmlWrapper_SetHandler(hXmlWrapperObj, ptDispInitNeedTreeMap, NULL);
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: XmlWrapper_SetHandler -0x%08x\n", 
				DISP_PRE, scRet);
		return S_FAIL;
	}
	scRet = XmlWrapper_ReadFile_UsrDefFunc(ptInfo->szCfgFile, 
			hXmlWrapperObj, 
			ptInfo);
	if (scRet != S_OK) {
		fprintf(stderr, 
				"%s Err: XmlWrapper_ReadFile_UsrDefFunc \"%s\" - 0x%08x\n",
				DISP_PRE, ptInfo->szCfgFile, scRet);
		return S_FAIL;
	}
	scRet = XmlWrapper_Release(&hXmlWrapperObj);
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: Release XmlWrapper - 0x%08x\n", 
				DISP_PRE, scRet);
		return S_FAIL;
	}
	return S_OK;
}

/* ============================================================================================= */
static void SetOutputEnable(HANDLE hObject, const CHAR *szParam, SDWORD len)
{

	TDisplayInfo    *ptInfo         = (TDisplayInfo *)(hObject);
	EDispOutEn      eCurrDispOutEn;
	CHAR            szTemp[len+1];
	//static EDispOutEn  ePrDispOutEn = vDispUnknownState;

	memset( szTemp,0x0,sizeof(szTemp) );
	snprintf(szTemp, len+1, "%s", szParam);

	if (strcasecmp(szTemp, "on") == 0) {
		eCurrDispOutEn = vDispOutEnable;
	} 
	else { //if (strcasecmp(szTemp, "off") == 0) {
		eCurrDispOutEn = vDispOutDisable;
	}

	if (eCurrDispOutEn == vDispOutEnable) {
		ptInfo->bNeed2Display = TRUE;
		ptInfo->bDisplayStart = TRUE;
		//sem_post(&(ptInfo->smEnDisp));
	} 
	else 
	{ //if (eDispOutEn == vdispOutDisable) {
			ptInfo->bNeed2Display = FALSE;
	}
	DISP_DP("%s Cfg: Output Enable = %s\n", DISP_PRE, szTemp);

	return;
}

/* ============================================================================================= */
static void SetOutputFormat(HANDLE      hObject, 
			const CHAR  *szOutputFormat, 
			SDWORD      len)
{
	TDisplayInfo    *ptInfo = (TDisplayInfo *)(hObject);
	CHAR            szTemp[len+1];

	snprintf(szTemp, len+1, "%s", szOutputFormat);

	if (strcasecmp(szTemp, "NTSC") == 0) {
		ptInfo->eDispOutFmt = vDispOutFmtNTSC;
	} 
	else if (strcasecmp(szTemp, "PAL") == 0) {
		ptInfo->eDispOutFmt = vDispOutFmtPAL;
	} 
	else if (strcasecmp(szTemp, "HDMI") == 0) {
		ptInfo->eDispOutFmt = vDispOutFmtHDMI;
	} 
	else if (strcasecmp(szTemp, "HDSDI") == 0) {
			ptInfo->eDispOutFmt = vDispOutFmtHDSDI;
	} 
	else {
		ptInfo->eDispOutFmt = vDispOutFmtUnknown;
	}
	printf("%s cfg: Output format: %d !!\n", 
				DISP_PRE ,ptInfo->eDispOutFmt);
}

/* ============================================================================================= */
static void SetOutputResolution(HANDLE      hObject, 
			const CHAR  *szOutputResolution, 
			SDWORD      len)
{
	TDisplayInfo    *ptInfo  = (TDisplayInfo *)(hObject);
	CHAR            *szRes          = NULL;
	char            *szToken        = NULL;

	szRes = malloc(len+1);
	memset( szRes, 0, (len+1) );
	strncpy(szRes, szOutputResolution, len);
	printf("%s cfg: Output Resolution: %s\n", DISP_PRE, szRes);

	ptInfo->dwOutputWidth  = strtol(szRes, &szToken, 10);
	ptInfo->dwOutputHeight = strtol(szToken+1, (char**)NULL, 10);
}

/* ============================================================================================= */
static void SetOutputFreq(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TDisplayInfo    *ptInfo = (TDisplayInfo *)(hObject);
	CHAR            szTemp[len+1];
	
	snprintf(szTemp, (len+1), "%s",szParam);
	printf("%s cfg: Output Freq: %s\n", DISP_PRE, szTemp);  
	ptInfo->dwFreq = strtol(szTemp, (char**)NULL, 10);
}

/* ============================================================================================= */
static void SetI2CDevice(HANDLE hObject, const CHAR *szI2CDevice, SDWORD len)
{
	TDisplayInfo    *ptInfo = (TDisplayInfo *)(hObject);
	CHAR            szTemp[len+1];

	snprintf(szTemp, (len+1), "%s", szI2CDevice);
	ptInfo->szI2CDevice = strdup(szTemp);
	printf("%s I2C device path = %s !!\n", DISP_PRE, ptInfo->szI2CDevice);
}

/* ============================================================================================= */
SCODE VencDisplay_InitResize(HANDLE *phResizeObj)
{
	SCODE               scRet = S_OK;
#ifdef _VMA_IRE	
	//! Initial Resize handle
	TResizeInitOptions  tResizeInitOptions;
	
	memset( &(tResizeInitOptions), 0, sizeof(TResizeInitOptions) );
	tResizeInitOptions.dwVersion    = RESIZE_VERSION;
	tResizeInitOptions.pObjectMem   = NULL;
	scRet = Resize_Initial(phResizeObj, &tResizeInitOptions);
	if (scRet != S_OK) {
		fprintf(stderr,"%s Err: Resize_Initial - 0x%08x\n", DISP_PRE, scRet);
	}
	else DISP_DP("%s Resize_Initial done\n", DISP_PRE);
#endif
	return scRet;
}

/* ========================================================================== */
SCODE VencDisplay_InitSharedBuffer(TDisplayInfo *ptInfo, int iIndex,BOOL bSlowReader)
{
	TSharedBuffMgrInitOptions   tInitOpt;
	SCODE                       scRet       = S_FAIL;
	//! Initial sharedbuffergr
	tInitOpt.dwVersion      = SHAREDBUFFMGR_VERSION;
	tInitOpt.pObjectMem     = NULL;
	tInitOpt.dwMinorNum     = ptInfo->dwSharedBufMinorNum;
	if (bSlowReader == TRUE)
	{
		tInitOpt.eProcessRole   = esbmprSlowReader;
	}
	else
	{
		tInitOpt.eProcessRole   = esbmprReader;
	}
	DISP_DP("%s dwMinorNum = %u ..\n", DISP_PRE, tInitOpt.dwMinorNum);

	scRet = SharedBuffMgr_Initial(&(ptInfo->tSrdBufInfo[iIndex].hSrdObj), &tInitOpt);
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: SharedBuffMgr_Initial - 0x%08x\n", 
					DISP_PRE, scRet);
		return scRet;
	}    
	SharedBuffMgr_GetFileDescriptor(ptInfo->tSrdBufInfo[iIndex].hSrdObj, &(ptInfo->tSrdBufInfo[iIndex].iSrdFD));
	return S_OK;
}

/* ========================================================================== */
SCODE VencDisplay_InitMassMemAccess(HANDLE *phMassMemAccObj, 
			const _TDisplayInitNeedInfo *const _ptInfo)
{
	TMassMemAccessInitOptions   tMassMemAccessInitOpt;
	TMassMemAccessOptions tOptions;
	SCODE scRet = S_FAIL;
	memset( &(tMassMemAccessInitOpt), 0, sizeof(TMassMemAccessInitOptions) );

	tMassMemAccessInitOpt.dwVersion         = MASSMEMACCESS_VERSION;
	tMassMemAccessInitOpt.dwMaxFrameWidth   = _ptInfo->dwVideoMaxWidth;
	tMassMemAccessInitOpt.dwMaxFrameHeight  = _ptInfo->dwVideoMaxHeight;
	tMassMemAccessInitOpt.dwMaskBusNum      = 0;
	tMassMemAccessInitOpt.bMaskInit         = FALSE;
	tMassMemAccessInitOpt.pObjectMem        = NULL;
	scRet = MassMemAccess_Initial(phMassMemAccObj, &tMassMemAccessInitOpt);
	if (scRet != S_OK) {
			fprintf(stderr, "%s Err: MassMemAccess_Initial - 0x%08x\n",
					DISP_PRE, scRet);
	}
	tOptions.adwUserData[0] = FALSE;
	tOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_CACHE_COHERENCE;
	if (MassMemAccess_SetOptions(*phMassMemAccObj, &tOptions) != S_OK)
	{
		printf("[VENC_DISPLAY]Fail to set up cache operation !!\n");
		return S_FAIL;
	}
	return scRet;
}
/* ========================================================================== */
SCODE VencDisplay_InitFrameRateCtrl(HANDLE *phFRCtrlObj)
{
    TFrameRateInitOptions tFRCtrlInitOptions;

    tFRCtrlInitOptions.dwVersion    = FRAMERATECTRL_VERSION;
    tFRCtrlInitOptions.dwFrameRate  = 0;   //! initial disable framerate control

    if (FrameRateCtrl_Initial(phFRCtrlObj, &tFRCtrlInitOptions) != S_OK) {
        fprintf(stderr, 
                "[VENC_DISP] Initial Frame rate control object fail !! \n");
        return S_FAIL;
    }

    return S_OK;
}
/* ========================================================================== */
DWORD VencDisplay_GetMemory(HANDLE      hMemObj,
			DWORD       dwBusNum, 
			DWORD       dwSize, 
			EAlignType  eAlignType)
{
	TMemMgrState tMemMgrState;
	memset( &(tMemMgrState), 0, sizeof(TMemMgrState) );

	tMemMgrState.dwBusNum   = dwBusNum;
	tMemMgrState.dwSize     = dwSize;
	tMemMgrState.eAlignType = eAlignType;

	return ( MemMgr_GetMemory(hMemObj, &tMemMgrState) );
}

/* ========================================================================== */
SCODE VencDisplay_InitVideoDisp(TDisplayInfo *ptInfo)
{
	TVideoDispInitOptions   tVDispInitOpt;
	DWORD                   dwFrameSize     = 0;
	DWORD                   dwYSize         = 0;
	DWORD                   dwCbCrSize      = 0;
	SCODE                   scRet           = S_FAIL;

	memset( &(tVDispInitOpt), 0, sizeof(TVideoDispInitOptions) );
	tVDispInitOpt.dwVersion         = VIDEODISP_VERSION;
	tVDispInitOpt.pObjectMem        = NULL;
	tVDispInitOpt.epixelInFormat    = YUV420;

	switch (ptInfo->eDispOutFmt) {
		case vDispOutFmtHDMI:
			tVDispInitOpt.eOutFormat = VIDEO_SIGNAL_FORMAT_RGB24;
			break;
		case vDispOutFmtHDSDI:
			tVDispInitOpt.eOutFormat = VIDEO_SIGNAL_FORMAT_PROGRESSIVE_BT1120;
			break;
		default:
			tVDispInitOpt.eOutFormat = VIDEO_SIGNAL_FORMAT_INTERLACE_CCIR656;
			break;
	}

	if (ptInfo->dwOutputWidth == 720 && ptInfo->dwOutputHeight == 480) {
		tVDispInitOpt.eOutputSize           = VIDEO_SIGNAL_SIZE_720x480;
		tVDispInitOpt.eOutputRefreshRate    = VIDEO_SIGNAL_FREQUENCY_60HZ;
	} else if (ptInfo->dwOutputWidth == 1280 && ptInfo->dwOutputHeight == 720) {
		tVDispInitOpt.eOutputSize           = VIDEO_SIGNAL_SIZE_1280x720;
		tVDispInitOpt.eOutputRefreshRate    = VIDEO_SIGNAL_FREQUENCY_60HZ;
	} else if (ptInfo->dwOutputWidth == 1920 && ptInfo->dwOutputHeight == 1080) {
		tVDispInitOpt.eOutputSize           = VIDEO_SIGNAL_SIZE_1920x1080;
		tVDispInitOpt.eOutputRefreshRate    = VIDEO_SIGNAL_FREQUENCY_24HZ;
	} else if (ptInfo->dwOutputWidth == 720 && ptInfo->dwOutputHeight == 576) {
		tVDispInitOpt.eOutputSize           = VIDEO_SIGNAL_SIZE_720x576;
		tVDispInitOpt.eOutputRefreshRate    = VIDEO_SIGNAL_FREQUENCY_60HZ;
	} else if (ptInfo->dwOutputWidth == 1280 && ptInfo->dwOutputHeight == 1024) {
		tVDispInitOpt.eOutputSize           = VIDEO_SIGNAL_SIZE_1280x1024;
		tVDispInitOpt.eOutputRefreshRate    = VIDEO_SIGNAL_FREQUENCY_60HZ;
	} else {
		fprintf(stderr, "%s unsupported resolution %dx%d\n", DISP_PRE, ptInfo->dwOutputWidth, ptInfo->dwOutputHeight);
		return S_FAIL;
	}

#if defined(_IMX035)
	tVDispInitOpt.dwMaxInWidth  = 1280;//DISPLAY_OUT_BUF_WIDTH;
	tVDispInitOpt.dwMaxInHeight = 1024;//DISPLAY_OUT_BUF_HEIGHT;
#else
	tVDispInitOpt.dwMaxInWidth  = DISPLAY_OUT_BUF_WIDTH;
	tVDispInitOpt.dwMaxInHeight = DISPLAY_OUT_BUF_HEIGHT;
#endif    
	tVDispInitOpt.dwInWidth     = DISPLAY_OUT_BUF_WIDTH;
	tVDispInitOpt.dwInHeight    = DISPLAY_OUT_BUF_HEIGHT;
	tVDispInitOpt.bPALEn = (ptInfo->eDispOutFmt == vDispOutFmtPAL)? 1 : 0;
	//! 0->NTSC 1->PAL
	tVDispInitOpt.sdwBrightness = 0;         //! brightness -128~+128 0: disable
	tVDispInitOpt.sdwContrast   = 0;         //! contrast -128~+128   0: disable
	tVDispInitOpt.dwSaturation  = 128;       //! saturation 0~511

	dwYSize = DISPLAY_OUT_BUF_WIDTH * DISPLAY_OUT_BUF_HEIGHT;
	dwCbCrSize = (tVDispInitOpt.epixelInFormat == YUV420) ? (dwYSize >> 2) : (dwYSize >> 1);
	dwFrameSize = dwYSize + (dwCbCrSize << 1);

	ptInfo->pbyDispBuf = (BYTE *)VencDisplay_GetMemory(ptInfo->hMemMgrObj,
					ptInfo->_tInitInfo.dwVOCOutputDRAMLoc,
					dwFrameSize,
					ALIGN_TYPE_8_BYTE );
	if (ptInfo->pbyDispBuf == NULL ) {
	  fprintf(stderr, "%s Err: VencDisplay_GetMemory - null\n", DISP_PRE);
	  return S_FAIL;
	} 
	else DISP_DP("%s VencDisplay_GetMemory done - %u\n", 
				DISP_PRE, dwFrameSize);

	memset(ptInfo->pbyDispBuf, 0, dwFrameSize);
	//syslog(LOG_INFO, "[VENC_MEM] pbyDispBuf   : %d bytes\n", dwFrameSize);
	
	
	ptInfo->pbyDispBuf2 = (BYTE *)VencDisplay_GetMemory(ptInfo->hMemMgrObj,
					ptInfo->_tInitInfo.dwVOCOutputDRAMLoc,
					dwFrameSize,
					ALIGN_TYPE_8_BYTE );
	if (ptInfo->pbyDispBuf2 == NULL) {
	  fprintf(stderr, "%s Err: VencDisplay_GetMemory - null\n", DISP_PRE);
	  return S_FAIL;
	} 
	else DISP_DP("%s VencDisplay_GetMemory done - %u\n", 
				DISP_PRE, dwFrameSize);

	memset(ptInfo->pbyDispBuf2, 0, dwFrameSize);
	//syslog(LOG_INFO, "[VENC_MEM] pbyDispBuf2   : %d bytes\n", dwFrameSize);

	ptInfo->pDisplayInfoQueue = list_new();
	ptInfo->dwQueueElmtNum = 0;

	if (VencDisplay_InitFrameRateCtrl(&ptInfo->hFRCtrlObj) == S_FAIL)
	{
	  fprintf(stderr, "%s Err: VencDisplay_InitFrameRateCtrl() failed\n", DISP_PRE);
	  return S_FAIL;
	}
	
	// allocate resize output memory
	{
		int i;
		for (i = 0; i < READER_NUM; i++) {
			ptInfo->tResizeOutBuf[i].pbyBufY = (BYTE *)VencDisplay_GetMemory(ptInfo->hMemMgrObj,
							ptInfo->_tInitInfo.dwVOCOutputDRAMLoc,
							dwFrameSize,
							ALIGN_TYPE_128_BYTE);
			if (ptInfo->tResizeOutBuf[i].pbyBufY == NULL) {
				fprintf(stderr, "%s Err: VencDisplay_GetMemory - null\n", DISP_PRE);
				return S_FAIL;
			} else {
				DISP_DP("%s VencDisplay_GetMemory done - %u\n", 
							DISP_PRE, dwFrameSize);
				ptInfo->tResizeOutBuf[i].pbyBufU = ptInfo->tResizeOutBuf[i].pbyBufY + dwYSize;
				ptInfo->tResizeOutBuf[i].pbyBufV = ptInfo->tResizeOutBuf[i].pbyBufU + dwCbCrSize;
			}
		}
		
		//syslog(LOG_INFO, "[VENC_MEM] tResizeOutBuf: %d bytes\n", dwFrameSize*READER_NUM);
	}

	tVDispInitOpt.pbyInBuffer = NULL;
	scRet = VideoDisp_Initial(&(ptInfo->hVDispObj), &tVDispInitOpt); 
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: VideoDisp_Initial - 0x%08x\n",
					DISP_PRE, scRet);
		return S_FAIL;
	}
#if 1    
	{
		TVideoDispState tState;
		memset(&tState, 0, sizeof(TVideoDispState));
		tState.dwIndex = VIDEO_DISP_DUMMP_BUF_INDEX;
		tState.pbyYFrame = ptInfo->pbyDispBuf;
		tState.pbyCbFrame = ptInfo->pbyDispBuf + dwYSize;
		tState.pbyCrFrame = tState.pbyCbFrame + dwCbCrSize;

		scRet = VideoDisp_QueueBuf(ptInfo->hVDispObj, &tState); 
		if (scRet != S_OK) {
			fprintf(stderr, "%s Err: Initial stage: VideoDisp_QueueBuf failed - 0x%08x\n",
						DISP_PRE, scRet);
			return S_FAIL;
		}
		list_add_element(ptInfo->pDisplayInfoQueue, VIDEO_DISP_DUMMP_BUF_INDEX,VIDEO_DISP_DUMMP_BUF_HANDLE_INDEX);
		ptInfo->dwQueueElmtNum++;

		tState.dwIndex = VIDEO_DISP_DUMMP_BUF_INDEX2;
		tState.pbyYFrame = ptInfo->pbyDispBuf2;
		tState.pbyCbFrame = ptInfo->pbyDispBuf2 + dwYSize;
		tState.pbyCrFrame = tState.pbyCbFrame + dwCbCrSize;

		scRet = VideoDisp_QueueBuf(ptInfo->hVDispObj, &tState); 
		if (scRet != S_OK) {
			fprintf(stderr, "%s Err: Initial stage: VideoDisp_QueueBuf failed - 0x%08x\n",
						DISP_PRE, scRet);
			return S_FAIL;
		}
		list_add_element(ptInfo->pDisplayInfoQueue, VIDEO_DISP_DUMMP_BUF_INDEX2,VIDEO_DISP_DUMMP_BUF_HANDLE_INDEX);
		ptInfo->dwQueueElmtNum++;
	}
#endif
	return scRet;
}

/* ========================================================================== */
SCODE VencDisplay_InitXMLWrapper(HANDLE *phXMLObj)
{
	TXmlWrapperInitOptions  tXMLInitOpt;
	SCODE                   scRet = S_FAIL;
	tXMLInitOpt.dwVersion = XMLWRAPPER_VERSION;
	scRet = XmlWrapper_Initial(phXMLObj, &tXMLInitOpt);
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: XmlWrapper_Initial - 0x%08x\n",
					DISP_PRE, scRet);
		return scRet;
	}
	scRet = XmlWrapper_SetHandler(*phXMLObj, ptDisplayTreeMap, NULL); 
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: XmlWrapper_SetHandler - 0x%08x\n",
					DISP_PRE, scRet);
	}
	return scRet;
}

/* ========================================================================== */
SCODE VencDisplay_InitVML(TDisplayInfo *ptInfo)
{
	SCODE scRet = S_FAIL;
	//! Initial MassMemAccess instance 
	scRet = VencDisplay_InitMassMemAccess( &(ptInfo->hMassMemAccObj), 
				&(ptInfo->_tInitInfo) );
	if (scRet != S_OK) {
			fprintf(stderr, "%s Err: VencDisplay_InitMassMemAccess - 0x%08x\n",
					DISP_PRE, scRet);
			return scRet;   
	}
	DISP_DP("%s VencDisplay_InitMassMemAccess done \n", DISP_PRE);

	//! Initial VideoDisp instance
	scRet = VencDisplay_InitVideoDisp(ptInfo);
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: VencDisplay_InitVideoDisp - 0x%08x\n",
					DISP_PRE, scRet);
		return scRet;
	}
	DISP_DP("%s VencDisplay_InitVideoDisp done\n", DISP_PRE);

	//! Initial XMLWrapper instance 
	scRet = VencDisplay_InitXMLWrapper(&(ptInfo->hXMLObj));
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: VencDisplay_InitXMLWrapper - 0x%08x\n",
					DISP_PRE, scRet);
		return scRet;
	}
	DISP_DP("%s VencDisplay_InitXMLWrapper done\n", DISP_PRE);


#ifdef _VMA_IRE     
	scRet = VencDisplay_InitResize(&(ptInfo->hResizeObj));
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: VencDisplay_InitResize - 0x%08x\n",
					DISP_PRE, scRet);
		return scRet;
	}
	DISP_DP("%s VencDisplay_InitResize done\n", DISP_PRE);
#endif
	return scRet;
}

/* ========================================================================== */
SCODE VencDisplay_ReleaseVML(TDisplayInfo *ptInfo)
{
	SCODE scRet = S_FAIL;
	int i;

	if (ptInfo->pDisplayInfoQueue != NULL)
	{
		list_free(ptInfo->pDisplayInfoQueue);   
		free(ptInfo->pDisplayInfoQueue);        
		ptInfo->pDisplayInfoQueue = NULL;      
	}
	if (ptInfo->hFRCtrlObj != NULL)
	{
	  if (FrameRateCtrl_Release(&(ptInfo->hFRCtrlObj)) != S_OK) {
	  
	    fprintf(stderr, "[VENC_DISP] Release Frame rate control object fail !!\n");
	  }
	}
	//! Release buffers
	for (i = 0; i < READER_NUM; i++) {
		scRet = MemMgr_FreeMemory(ptInfo->hMemMgrObj, (DWORD)ptInfo->tResizeOutBuf[i].pbyBufY);
		if (scRet != S_OK) {
			fprintf(stderr, "%s Err: MemMgr_FreeMemory - 0x%08x\n", 
						DISP_PRE, scRet);
		}
	}

	scRet = MemMgr_FreeMemory(ptInfo->hMemMgrObj, (DWORD)ptInfo->pbyDispBuf);
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: MemMgr_FreeMemory - 0x%08x\n", 
					DISP_PRE, scRet);
	}

	scRet = MemMgr_FreeMemory(ptInfo->hMemMgrObj, (DWORD)ptInfo->pbyDispBuf2);
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: MemMgr_FreeMemory - 0x%08x\n", 
					DISP_PRE, scRet);
	}

	//! Release instance
	if (ptInfo->hVDispObj != NULL) {
		VideoDisp_Stop(ptInfo->hVDispObj);
	} 
	else DISP_DP("%s hVDispObj is NULL\n", DISP_PRE);

	//! Release shared buffer instance
	for (i = 0; i < READER_NUM; i++) {
		if (ptInfo->tSrdBufInfo[i].hSrdObj != NULL){
		    scRet = SharedBuffMgr_Release(&(ptInfo->tSrdBufInfo[i].hSrdObj));
		    if (scRet != S_OK) {
			fprintf(stderr, "%s Err: SharedBuffMgr_Release - 0x%08x\n",
							DISP_PRE, scRet);
		    }
		}
	}

	//! Release MassMemAccess instance 
	scRet = MassMemAccess_Release( &(ptInfo->hMassMemAccObj) );
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: MassMemAccess_Release - 0x%08x\n",
					DISP_PRE, scRet);
	}
	//! Release VideoDisp instance 
	scRet = VideoDisp_Release( &(ptInfo->hVDispObj) );
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: VideoDisp_Release - 0x%08x\n", 
					DISP_PRE, scRet);
	}
	//! Release xmlwrapper instance 
	scRet = XmlWrapper_Release( &(ptInfo->hXMLObj) );
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: XmlWrapper_Release - 0x%08x\n",
					DISP_PRE, scRet);
	}
#ifdef _VMA_IRE
	//! Release Resize handle
	if (ptInfo->hResizeObj != NULL) {        
		scRet = Resize_Release( &(ptInfo->hResizeObj) );
		if (scRet != S_OK) {
			fprintf(stderr, "%s Err: Resize_Release - 0x%08x\n", 
						DISP_PRE, scRet);
		}
	}
#endif
	return S_OK;
}

#endif

/* ========================================================================== */

