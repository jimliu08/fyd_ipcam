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
 * venc_app.c
 *
 * \brief
 * Video process implement.
 *
 * \date
 * 2009/08/04
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#include "venc_app_local.h"
#include "videocomm.h"
/* ============================================================================================= */
SCODE Venc_Initial(HANDLE *phVencObject, TVencInitOpt tVencInitOpt)
{	
    TVencInfo *ptVencInfo = (TVencInfo *)malloc(sizeof(TVencInfo));
	TMasterInitOpt	tMasterInitOpt;
    TVideoInInitOpt tVideoInInitOpt;
    TEncoderInitOpt tEncoderInitOpt;
#if 0    
    TFRCtrlOptions tFRCtrlOptions;
#endif
#ifdef	_VMA_IBPE
	TMotionInitOpt	tMotionInitOpt;
#endif
#ifdef _VPL_VOC
	TDisplayInitOpt tDisplayInitOpt;
#endif	
	DWORD	dwBitStreamSize;
    DWORD	dwCurId;

    if (ptVencInfo == NULL) {
        return ERR_OUT_OF_MEMORY;
    }
    memset(ptVencInfo, 0, sizeof(TVencInfo));
    *phVencObject = (HANDLE)(ptVencInfo);

    if (tVencInitOpt.dwDevNum >= MAX_HW_CHANNELNUM) {
        DBPRINT1("[VENC] Error device number %u !!\n", tVencInitOpt.dwDevNum);
        DBPRINT1("[VENC] The Mozart only supports device 0~%d !!\n", (MAX_HW_CHANNELNUM-1));
        return S_FAIL;
    }
    DBPRINT1("[VENC] Device number %u !!\n", tVencInitOpt.dwDevNum);

    if (!tVencInitOpt.szConfigFile) {
        ptVencInfo->szConfigFile = DEFAULT_CONFIG_FILE;
    }
    else {
        ptVencInfo->szConfigFile = tVencInitOpt.szConfigFile;
    }

    if (access(ptVencInfo->szConfigFile, F_OK) != 0) {
        DBPRINT1("[VENC] Configuration file %s not exist!!\n", ptVencInfo->szConfigFile);
        return S_FAIL;
    }
    DBPRINT1("[VENC] Configuration file %s\n", ptVencInfo->szConfigFile);

    // Create semaphore for thread communicate with critical section variables
    if (sem_init(&(ptVencInfo->smOptions), 0, 1) != 0) {
    	return S_FAIL;
    }

	g_bTerminate = FALSE;

    // Initial memmgr
    if (VencApp_InitMemMgr(&(ptVencInfo->hMemMgrObj)) != S_OK) {
        fprintf(stderr, "[VENC] Initial MemMgr object fail !!\n");
        return S_FAIL;
    }

    // Initial XmlWrapper Instance 
    if (VencApp_InitXmlWrapper(&(ptVencInfo->hXMLWrapperObj)) != S_OK) {
        fprintf(stderr, "[VENC] Initial XMLWrapper object fail !!\n");
        return S_FAIL;
    }

	// read all process config setting about thread create
	if (VencApp_ParseVencConfig(ptVencInfo) != S_OK) {
        DBPRINT0("[VENC] parse venc config fail !!\n");
		return S_FAIL;
	}

	// parse all needed information like MaxFrameSize, CaptureSize ...
	if (VencApp_GetInitNeedInfo(ptVencInfo) != S_OK) {
        DBPRINT0("[VENC] get init need info fail !!\n");
		return S_FAIL;
	}
#if 0	
	#if defined(_MT9P031) ||  defined( _OV5653) || defined(_AR0331)
		//printf("[venc_app](%d)Hello!!\n",__LINE__);
		ptVencInfo->tCapabilityRec.dwVideoMaxWidth = ptVencInfo->tCapabilityRec.dwVideoCapWidth;
		ptVencInfo->tCapabilityRec.dwVideoMaxHeight = ptVencInfo->tCapabilityRec.dwVideoCapHeight;
	#endif
		
	#if defined(_5M2MSWITCH)
	   ptVencInfo->tCapabilityRec.dwVideoMaxWidth = 2560;
	   ptVencInfo->tCapabilityRec.dwVideoMaxHeight = 1920;
	#elif defined(_3M2MSWITCH)
	   ptVencInfo->tCapabilityRec.dwVideoMaxWidth = 2048;
	   ptVencInfo->tCapabilityRec.dwVideoMaxHeight = 1536;
	#endif		
#endif
#if !defined(_5M2MSWITCH) && !defined(_3M2MSWITCH)	   
	if (ptVencInfo->tCapabilityRec.dwVideoMaxWidth > ptVencInfo->tCapabilityRec.dwVideoCapWidth)
	{
		ptVencInfo->tCapabilityRec.dwVideoMaxWidth = ptVencInfo->tCapabilityRec.dwVideoCapWidth; 
	}
	
	if (ptVencInfo->tCapabilityRec.dwVideoMaxHeight > ptVencInfo->tCapabilityRec.dwVideoCapHeight)
	{
		ptVencInfo->tCapabilityRec.dwVideoMaxHeight = ptVencInfo->tCapabilityRec.dwVideoCapHeight;
	}
#endif
	
#if 1
	printf("[VENC] Capability - dwBufNum = %u\n", ptVencInfo->tCapabilityRec.dwBufNum);
	printf("[VENC] Capability - dwVideoMaxWidth = %u\n", ptVencInfo->tCapabilityRec.dwVideoMaxWidth);
	printf("[VENC] Capability - dwVideoMaxHeight = %u\n", ptVencInfo->tCapabilityRec.dwVideoMaxHeight);
	printf("[VENC] Capability - capture resolution W x H: %u x %u\n", ptVencInfo->tCapabilityRec.dwVideoCapWidth, ptVencInfo->tCapabilityRec.dwVideoCapHeight);
	printf("[VENC] Capability - sensor in resolution W x H: %u x %u\n", ptVencInfo->tCapabilityRec.dwVideoInWidth, ptVencInfo->tCapabilityRec.dwVideoInHeight);
	printf("[VENC] Capability - Interlace or Progressive? %s\n", (ptVencInfo->tCapabilityRec.bInterlaceScan)? "Interlace": "Progressive");
#endif


	// decide bitstream size here, match the past defined case
	if (ptVencInfo->tCapabilityRec.dwVideoMaxWidth >= 1280) {
		dwBitStreamSize = 1024 * 1024;
	} else {
		dwBitStreamSize = 720 * 480;
	}
    // Initial Video Master process
    if (ptVencInfo->tVencCfgPath.bMasterEnable == TRUE) {
	    tMasterInitOpt.dwDevNum = tVencInitOpt.dwDevNum;
	    tMasterInitOpt.dwCallbackInstance = (DWORD)ptVencInfo;
	    tMasterInitOpt.szConfigFile = ptVencInfo->tVencCfgPath.szMasterConfigFile;
	    tMasterInitOpt.hMemMgrObj = ptVencInfo->hMemMgrObj;
	    tMasterInitOpt.tInitNeedInfo.dwVideoMaxWidth = ptVencInfo->tCapabilityRec.dwVideoMaxWidth;
	    tMasterInitOpt.tInitNeedInfo.dwVideoMaxHeight = ptVencInfo->tCapabilityRec.dwVideoMaxHeight;
	    tMasterInitOpt.tInitNeedInfo.dwVideoCapWidth = ptVencInfo->tCapabilityRec.dwVideoCapWidth;
	    tMasterInitOpt.tInitNeedInfo.dwVideoCapHeight = ptVencInfo->tCapabilityRec.dwVideoCapHeight;
	    tMasterInitOpt.tInitNeedInfo.dwVideoInWidth = ptVencInfo->tCapabilityRec.dwVideoInWidth;
	    tMasterInitOpt.tInitNeedInfo.dwVideoInHeight = ptVencInfo->tCapabilityRec.dwVideoInHeight;
	    tMasterInitOpt.tInitNeedInfo.dwBufNum = ptVencInfo->tCapabilityRec.dwBufNum;
	    tMasterInitOpt.tInitNeedInfo.szSensorConfigFile = ptVencInfo->tVencCfgPath.szSensorConfigFile;
	    tMasterInitOpt.tInitNeedInfo.dwVICOutputDRAMLoc = ptVencInfo->tVencCfgPath.tDRAMParam.dwVICOutputDRAMLoc;
	    if (VencMaster_Initial(&(ptVencInfo->hMasterProcObj), tMasterInitOpt) != S_OK) {
	    	printf("[VENC] VencMaster_Initial fail !!\n");
	    	exit(0);
	    }
	   	printf("[VENC] VencMaster_Initial successful !!\n");
    }

	// Initial Video Input process
    if (ptVencInfo->tVencCfgPath.bVideoInEnable == TRUE) {
	    tVideoInInitOpt.dwDevNum = tVencInitOpt.dwDevNum;
	    tVideoInInitOpt.dwCallbackInstance = (DWORD)ptVencInfo;
	    tVideoInInitOpt.szConfigFile = ptVencInfo->tVencCfgPath.szVideoInConfigFile;
	    tVideoInInitOpt.hMemMgrObj = ptVencInfo->hMemMgrObj;
	    VencMaster_GetVideoCapHandle(ptVencInfo->hMasterProcObj, &tVideoInInitOpt.hVideoCapObj);
	    VencMaster_GetUARTIRCutFD(ptVencInfo->hMasterProcObj, &tVideoInInitOpt.fdIRCutUART);
	    VencMaster_GetIRCutFD(ptVencInfo->hMasterProcObj, &tVideoInInitOpt.fdIRCut);
	    tVideoInInitOpt.dwSharedBufferMinorNum = ptVencInfo->tVencCfgPath.dwSharedBufferMinorNum;
	    tVideoInInitOpt.tInitNeedInfo.dwVideoMaxWidth = ptVencInfo->tCapabilityRec.dwVideoMaxWidth;
	    tVideoInInitOpt.tInitNeedInfo.dwVideoMaxHeight = ptVencInfo->tCapabilityRec.dwVideoMaxHeight;
	    tVideoInInitOpt.tInitNeedInfo.dwVideoCapWidth = ptVencInfo->tCapabilityRec.dwVideoCapWidth;
	    tVideoInInitOpt.tInitNeedInfo.dwVideoCapHeight = ptVencInfo->tCapabilityRec.dwVideoCapHeight;
	    tVideoInInitOpt.tInitNeedInfo.dwVideoInWidth = ptVencInfo->tCapabilityRec.dwVideoInWidth;
	    tVideoInInitOpt.tInitNeedInfo.dwVideoInHeight = ptVencInfo->tCapabilityRec.dwVideoInHeight;
	    tVideoInInitOpt.tInitNeedInfo.dwLDCRefFrameDRAMLoc = ptVencInfo->tVencCfgPath.tDRAMParam.dwLDCRefFrameDRAMLoc;
	    tVideoInInitOpt.tInitNeedInfo.dwDMACOutputDRAMLoc = ptVencInfo->tVencCfgPath.tDRAMParam.dwDMACOutputDRAMLoc;
		tVideoInInitOpt.tInitNeedInfo.dwVICOutputDRAMLoc = ptVencInfo->tVencCfgPath.tDRAMParam.dwVICOutputDRAMLoc;
		if (ptVencInfo->tVencCfgPath.bDisplayEnable == TRUE)
		{
			tVideoInInitOpt.dwOutputSharedBufferNum = 9;
		}
		else
		{
			tVideoInInitOpt.dwOutputSharedBufferNum = 4;
		}
		if (VencVideoIn_Initial(&(ptVencInfo->hVideoInProcObj), tVideoInInitOpt) != S_OK) {
	    	printf("[VENC] VencVideoIn_Initial fail !!\n");
	    	exit(0);
	    }
	  
	    printf("[VENC] VencVideoIn_Initial successful !!\n");
	}

    ptVencInfo->phEncoderProcObj = (HANDLE *)malloc(sizeof(HANDLE)*ptVencInfo->tVencCfgPath.dwEncoderTotalNum);
    for (dwCurId = 0; dwCurId < ptVencInfo->tVencCfgPath.dwEncoderTotalNum ; dwCurId++)
    {
		ptVencInfo->phEncoderProcObj[dwCurId]=NULL;
    }
    // Initial Video Encoder process
    for (dwCurId = 0; dwCurId < ptVencInfo->tVencCfgPath.dwEncoderTotalNum; dwCurId++) {
	    tEncoderInitOpt.hMemMgrObj = ptVencInfo->hMemMgrObj;
	    tEncoderInitOpt.dwCallbackInstance = (DWORD)ptVencInfo;
	    tEncoderInitOpt.dwSharedBufferMinorNum = ptVencInfo->tVencCfgPath.dwSharedBufferMinorNum;
	    tEncoderInitOpt.szConfigFile = ptVencInfo->tVencCfgPath.pszEncoderConfigFile[dwCurId];
	    tEncoderInitOpt.dwCurId = dwCurId;
		tEncoderInitOpt.dwVideoResolutionFormatIndex = vfUserDef1 + dwCurId;
		if (tEncoderInitOpt.dwVideoResolutionFormatIndex == vfNum)
		{
			tEncoderInitOpt.dwVideoResolutionFormatIndex = vfUserDef1;
			printf("[VENC][Warnning][encoder %d] The video resolution format index is upper bound!! Reset as %d\n", dwCurId, vfUserDef1);
		}
	    tEncoderInitOpt.tInitNeedInfo.dwVideoMaxWidth = ptVencInfo->tCapabilityRec.dwVideoMaxWidth;
	    tEncoderInitOpt.tInitNeedInfo.dwVideoMaxHeight = ptVencInfo->tCapabilityRec.dwVideoMaxHeight;
	 
	    tEncoderInitOpt.tInitNeedInfo.dwVideoCapWidth = ptVencInfo->tCapabilityRec.dwVideoCapWidth;
	    tEncoderInitOpt.tInitNeedInfo.dwVideoCapHeight = ptVencInfo->tCapabilityRec.dwVideoCapHeight;
	    tEncoderInitOpt.tInitNeedInfo.dwBitStreamSize = dwBitStreamSize;
	    tEncoderInitOpt.tInitNeedInfo.dwIREOutput0DRAMLoc = ptVencInfo->tVencCfgPath.tDRAMParam.dwIREOutput0DRAMLoc;
	    tEncoderInitOpt.tInitNeedInfo.dwIREOutput1DRAMLoc = ptVencInfo->tVencCfgPath.tDRAMParam.dwIREOutput1DRAMLoc;
	    tEncoderInitOpt.tInitNeedInfo.dwBitStreamOutputDRAMLoc = ptVencInfo->tVencCfgPath.tDRAMParam.dwBitStreamOutputDRAMLoc;
	    tEncoderInitOpt.tInitNeedInfo.dwMp4VBusNum = ptVencInfo->tVencCfgPath.tDRAMParam.dwMp4VBusNum;
	    tEncoderInitOpt.tInitNeedInfo.dwH264BusNum = ptVencInfo->tVencCfgPath.tDRAMParam.dwH264BusNum;
	    tEncoderInitOpt.tInitNeedInfo.dwSVCBusNum = ptVencInfo->tVencCfgPath.tDRAMParam.dwSVCBusNum;
	    tEncoderInitOpt.tInitNeedInfo.bInterlaceScan = ptVencInfo->tCapabilityRec.bInterlaceScan;
	    tEncoderInitOpt.tInitNeedInfo.dwMaxCapFrameRate = ptVencInfo->tCapabilityRec.dwMaxCapFrameRate;
	    tEncoderInitOpt.tInitNeedInfo.bCMOSSensor = ptVencInfo->tCapabilityRec.bCMOSSensor;
	    tEncoderInitOpt.tInitNeedInfo.dwVideoCodecInitMask = ptVencInfo->tCapabilityRec.dwVideoCodecInitMask;
		#if 0
	    if (VencEncoder_Initial(&(ptVencInfo->hEncoderProcObj[dwCurId]), tEncoderInitOpt) != S_OK) {
	    	printf("[VENC] VencEncoder_Initial fail !!\n");
	    	exit(0);
	    }
		#endif	
		if (VencEncoder_Initial(&(ptVencInfo->phEncoderProcObj[dwCurId]), tEncoderInitOpt) != S_OK) {
	    	printf("[VENC] VencEncoder_Initial fail !!\n");
	    	exit(0);
	    }
		//printf("[VENC_APP](%d)ptVencInfo->phEncoderProcObj[%d]=%p\n",__LINE__,dwCurId,ptVencInfo->phEncoderProcObj[dwCurId]);
		
	   	printf("[VENC] VencEncoder_Initial successful !!\n");
		
    }
    #if 0
	for (dwCurId = 0; dwCurId < ptVencInfo->tVencCfgPath.dwEncoderTotalNum; dwCurId++) {
		printf("[VENC_APP](%d) ptVencInfo->phEncoderProcObj[%d]=%p\n",__LINE__,dwCurId,ptVencInfo->phEncoderProcObj[dwCurId]);
	}
#endif


#if defined(_VMA_IBPE)
	// Initial Motion Sending process
    if (ptVencInfo->tVencCfgPath.bMotionEnable == TRUE) {
	    tMotionInitOpt.dwCallbackInstance = (DWORD)ptVencInfo;
	    tMotionInitOpt.szConfigFile = ptVencInfo->tVencCfgPath.szMotionConfigFile;
	    tMotionInitOpt.hMemMgrObj = ptVencInfo->hMemMgrObj;
	    tMotionInitOpt.dwSharedBufferMinorNum = ptVencInfo->tVencCfgPath.dwSharedBufferMinorNum;
	    tMotionInitOpt.tInitNeedInfo.dwVideoMaxWidth = ptVencInfo->tCapabilityRec.dwVideoMaxWidth;
	    tMotionInitOpt.tInitNeedInfo.dwVideoMaxHeight = ptVencInfo->tCapabilityRec.dwVideoMaxHeight;
	    tMotionInitOpt.tInitNeedInfo.dwVideoCapWidth = ptVencInfo->tCapabilityRec.dwVideoCapWidth;
	    tMotionInitOpt.tInitNeedInfo.dwVideoCapHeight = ptVencInfo->tCapabilityRec.dwVideoCapHeight;
	    tMotionInitOpt.tInitNeedInfo.dwBitStreamSize = dwBitStreamSize;
	    tMotionInitOpt.tInitNeedInfo.dwMotionSnapshotOutputDRAMLoc = ptVencInfo->tVencCfgPath.tDRAMParam.dwMotionSnapshotOutputDRAMLoc;
	    tMotionInitOpt.tInitNeedInfo.dwVideoCodecInitMask = ptVencInfo->tCapabilityRec.dwVideoCodecInitMask;	
		if (VencMotion_Initial(&(ptVencInfo->hMotionProcObj), tMotionInitOpt) != S_OK) {
	    	printf("[VENC] VencMotion_Initial fail !!\n");
	    	exit(0);
	    }
	   	printf("[VENC] VencMotion_Initial successful !!\n");
	}
#endif

#if defined(_VPL_VOC)
	// Initial Video Display process
    if (ptVencInfo->tVencCfgPath.bDisplayEnable == TRUE) {
	    tDisplayInitOpt.dwDevNum = tVencInitOpt.dwDevNum;
	    tDisplayInitOpt.dwCallbackInstance = (DWORD)ptVencInfo;
	    tDisplayInitOpt.dwSharedBufferMinorNum = ptVencInfo->tVencCfgPath.dwSharedBufferMinorNum;
	    tDisplayInitOpt.szConfigFile = ptVencInfo->tVencCfgPath.szDisplayConfigFile;
	    tDisplayInitOpt.hMemMgrObj = ptVencInfo->hMemMgrObj;
	    tDisplayInitOpt.tInitNeedInfo.dwVideoMaxWidth = ptVencInfo->tCapabilityRec.dwVideoMaxWidth;
	    tDisplayInitOpt.tInitNeedInfo.dwVideoMaxHeight = ptVencInfo->tCapabilityRec.dwVideoMaxHeight;
	    tDisplayInitOpt.tInitNeedInfo.dwVideoCapWidth = ptVencInfo->tCapabilityRec.dwVideoCapWidth;
	    tDisplayInitOpt.tInitNeedInfo.dwVideoCapHeight = ptVencInfo->tCapabilityRec.dwVideoCapHeight;
	    tDisplayInitOpt.tInitNeedInfo.dwVideoInWidth = ptVencInfo->tCapabilityRec.dwVideoInWidth;
	    tDisplayInitOpt.tInitNeedInfo.dwVideoInHeight = ptVencInfo->tCapabilityRec.dwVideoInHeight;
	    tDisplayInitOpt.tInitNeedInfo.dwVOCOutputDRAMLoc = ptVencInfo->tVencCfgPath.tDRAMParam.dwVOCOutputDRAMLoc;
	    if (VencDisplay_Initial(&(ptVencInfo->hDisplayProcObj), tDisplayInitOpt) != S_OK) {
	    	printf("[VENC] VencDisplay_Initial fail !!\n");
	    	exit(0);
	    }
	   	printf("[VENC] VencDisplay_Initial successful !!\n");
	}
#endif
	
    Venc_LoadConfig(ptVencInfo);

    if (tVencInitOpt.szCommandFIFO == NULL) {
        tVencInitOpt.szCommandFIFO = DEFAULT_COMMAND_FIFO;
    }
	DBPRINT1("[VENC] CommandFIFO path = %s\n", tVencInitOpt.szCommandFIFO);

	// Initial Message process
    VencMessage_Initial(ptVencInfo, tVencInitOpt.szCommandFIFO);

    return S_OK;
}

/* ============================================================================================= */
SCODE Venc_Release(HANDLE *phVencObject)
{
    TVencInfo *ptVencInfo = (TVencInfo *)(*phVencObject);
    DWORD	dwCurId;
	
	if (ptVencInfo->hMsgReaderObj != NULL) 
	{
		VencMessage_Release(&ptVencInfo->hMsgReaderObj);
	}
	
	for (dwCurId = 0; dwCurId < ptVencInfo->tVencCfgPath.dwEncoderTotalNum; dwCurId++) 
	{
		if (ptVencInfo->phEncoderProcObj[dwCurId] != NULL) {
		    VencEncoder_Release(&ptVencInfo->phEncoderProcObj[dwCurId]);
		    DBPRINT1("[VENC] Encoder process %u release\n", dwCurId);
		}
	}

	for (dwCurId = 0; dwCurId < ptVencInfo->tVencCfgPath.dwEncoderTotalNum; dwCurId++) 
	{
		if (ptVencInfo->tVencCfgPath.pszEncoderConfigFile[dwCurId] != NULL)
		{
			free(ptVencInfo->tVencCfgPath.pszEncoderConfigFile[dwCurId]);		
			ptVencInfo->tVencCfgPath.pszEncoderConfigFile[dwCurId]=NULL;
		}
	}
	if (ptVencInfo->tVencCfgPath.pszEncoderConfigFile != NULL)
	{
		free(ptVencInfo->tVencCfgPath.pszEncoderConfigFile);
		ptVencInfo->tVencCfgPath.pszEncoderConfigFile = NULL;
	}

	if (ptVencInfo->pdwCurrSwitch != NULL)
	{
		free(ptVencInfo->pdwCurrSwitch);
		ptVencInfo->pdwCurrSwitch = NULL;
	}
	if (ptVencInfo->tVencCfgPath.szCapabilityPath != NULL)
	{
	    free(ptVencInfo->tVencCfgPath.szCapabilityPath);
	    ptVencInfo->tVencCfgPath.szCapabilityPath = NULL;
	}
	if (ptVencInfo->tVencCfgPath.szSensorConfigFile != NULL)
	{
	    free(ptVencInfo->tVencCfgPath.szSensorConfigFile);
	    ptVencInfo->tVencCfgPath.szSensorConfigFile = NULL;
	}
	if (ptVencInfo->tVencCfgPath.szMasterConfigFile != NULL)
	{
	    free(ptVencInfo->tVencCfgPath.szMasterConfigFile);
	    ptVencInfo->tVencCfgPath.szMasterConfigFile = NULL;
	}
	
	if (ptVencInfo->tVencCfgPath.szMotionConfigFile != NULL)
	{
	    free(ptVencInfo->tVencCfgPath.szMotionConfigFile);
	    ptVencInfo->tVencCfgPath.szMotionConfigFile = NULL;
	}
	
	if (ptVencInfo->tVencCfgPath.szDisplayConfigFile != NULL)
	{
		free( ptVencInfo->tVencCfgPath.szDisplayConfigFile);
		ptVencInfo->tVencCfgPath.szDisplayConfigFile = NULL;
	}
	
	if (ptVencInfo->tVencCfgPath.szVideoInConfigFile != NULL)
	{
		free(ptVencInfo->tVencCfgPath.szVideoInConfigFile);
		ptVencInfo->tVencCfgPath.szVideoInConfigFile = NULL;
	}
	
	
#if defined(_VMA_IBPE)
	if (ptVencInfo->hMotionProcObj != NULL) {
    	VencMotion_Release(&ptVencInfo->hMotionProcObj);
    	DBPRINT0("[VENC] Motion sending process release\n");	
	}
#endif
#if defined(_VPL_VOC)
	if (ptVencInfo->hDisplayProcObj != NULL) {
	    VencDisplay_Release(&ptVencInfo->hDisplayProcObj);
	    DBPRINT0("[VENC] Display process release\n");
	}
#endif
	if (ptVencInfo->tCapabilityRec.szSensorName != NULL )
	{
		free(ptVencInfo->tCapabilityRec.szSensorName);
		ptVencInfo->tCapabilityRec.szSensorName = NULL;
		DBPRINT0("[VENC] ptVencInfo->tCapabilityRec.szSensorName release\n");
	}
	if (ptVencInfo->hVideoInProcObj != NULL) {
	    VencVideoIn_Release(&ptVencInfo->hVideoInProcObj);
	    DBPRINT0("[VENC] VideoIn process release\n");
	}

	if (ptVencInfo->hMasterProcObj != NULL) {
		VencMaster_Release(&ptVencInfo->hMasterProcObj);
	    DBPRINT0("[VENC] Master process release\n");
	}
    
	// release Venc initial parts, it should be release last
	if (ptVencInfo->hXMLWrapperObj != NULL)
		XmlWrapper_Release(&(ptVencInfo->hXMLWrapperObj));

	if (ptVencInfo->hMemMgrObj != NULL)
		MemMgr_Release(&ptVencInfo->hMemMgrObj);
	
	sem_destroy(&(ptVencInfo->smOptions));
	
	if (ptVencInfo) free(ptVencInfo);
	
	*phVencObject = NULL;
  
    return S_OK;
}

/* ============================================================================================= */
SCODE Venc_Start(HANDLE hVencObject)
{
    TVencInfo *ptVencInfo = (TVencInfo *)(hVencObject);
    DWORD	dwCurId;

	if (ptVencInfo->hMasterProcObj != NULL) {
		VencMaster_Start(ptVencInfo->hMasterProcObj);
	    DBPRINT0("[VENC] Master process start\n");
	}
#if defined(_VPL_VOC)
	if (ptVencInfo->hDisplayProcObj != NULL) {
		VencDisplay_Start(ptVencInfo->hDisplayProcObj);
	    DBPRINT0("[VENC] Display process start\n");
	}
#endif


	if (ptVencInfo->hVideoInProcObj != NULL) {
	    VencVideoIn_Start(ptVencInfo->hVideoInProcObj);
	    DBPRINT0("[VENC] VideoIn process start\n");
	}

    for (dwCurId = 0; dwCurId < ptVencInfo->tVencCfgPath.dwEncoderTotalNum; dwCurId++) {
    	if (ptVencInfo->phEncoderProcObj[dwCurId] != NULL) {
			VencEncoder_Start(ptVencInfo->phEncoderProcObj[dwCurId]);	
		    DBPRINT1("[VENC] Encoder process %u start\n", dwCurId);
    	}
	}
#if defined(_VMA_IBPE)
	if (ptVencInfo->hMotionProcObj != NULL) {
	    VencMotion_Start(ptVencInfo->hMotionProcObj);
	    DBPRINT0("[VENC] Motion sending process start\n");
	}
#endif
	if (ptVencInfo->hMsgReaderObj != NULL) {
	    VencMessage_Start(ptVencInfo->hMsgReaderObj);
	}

    return S_OK;
}

/* ============================================================================================= */
SCODE Venc_Stop(HANDLE hVencObject)
{
    TVencInfo *ptVencInfo = (TVencInfo *)(hVencObject);
    DWORD	dwCurId;

	if (ptVencInfo->hMsgReaderObj != NULL) {
	    VencMessage_Stop(ptVencInfo->hMsgReaderObj);
		DBPRINT0("[VENC] Message handler stop\n");
	}
#if defined(_VMA_IBPE)
	if (ptVencInfo->hMotionProcObj != NULL) {
	    VencMotion_Stop(ptVencInfo->hMotionProcObj);
	    DBPRINT0("[VENC] Motion sending process stop\n");
	}
#endif
    for (dwCurId = 0; dwCurId < ptVencInfo->tVencCfgPath.dwEncoderTotalNum; dwCurId++) {
    	if (ptVencInfo->phEncoderProcObj[dwCurId] != NULL) {
	#if 0
		    VencEncoder_Stop(ptVencInfo->hEncoderProcObj[dwCurId]);
	#endif
		VencEncoder_Stop(ptVencInfo->phEncoderProcObj[dwCurId]);	
		    DBPRINT1("[VENC] Encoder process %u stop\n", dwCurId);
    	}
	}

#if defined(_VPL_VOC)
	if (ptVencInfo->hDisplayProcObj != NULL) {
	    VencDisplay_Stop(ptVencInfo->hDisplayProcObj);
	    DBPRINT0("[VENC] Display process stop\n");
	}
#endif

	if (ptVencInfo->hVideoInProcObj != NULL) {
	    VencVideoIn_Stop(ptVencInfo->hVideoInProcObj);
	    DBPRINT0("[VENC] VideoIn process stop\n");
	}

	if (ptVencInfo->hMasterProcObj != NULL) {
		VencMaster_Stop(ptVencInfo->hMasterProcObj);
		DBPRINT0("[VENC] Master process stop\n");
	}

    return S_OK;
}

/* ============================================================================================= */
SCODE Venc_LoadConfig(HANDLE hVencObject)
{
    TVencInfo *ptVencInfo = (TVencInfo *)(hVencObject);

    // Load configurations and compare with current configurations to see if them are changed
    sem_wait(&(ptVencInfo->smOptions));
	if (XmlWrapper_ReadFile_UsrDefFunc(ptVencInfo->szConfigFile, ptVencInfo->hXMLWrapperObj, ptVencInfo) != S_OK) {
        fprintf(stderr, "[VENC] XmlWrapper reload config fail! \n");
        return S_FAIL;
    }

	if (XmlWrapper_Reset(ptVencInfo->hXMLWrapperObj) != S_OK)  {
		DBPRINT0("[VENC] XmlWrapper read config fail! \n");
		return S_FAIL;
	}
    sem_post(&(ptVencInfo->smOptions));
    return S_OK;
}
