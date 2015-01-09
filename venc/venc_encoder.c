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
 * venc_encoder.c
 *
 * \brief
 * Video encoder process implement.
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

/* ============================================================================================= */
SCODE VencEncoder_Initial(HANDLE *phEncoderProcObj, TEncoderInitOpt tEncoderInitOpt)
{
	TEncoderInfo *ptEncoderInfo;

	int iRet;
	int iROIWinCount = 0;
	ptEncoderInfo = (TEncoderInfo *)malloc(sizeof(TEncoderInfo));
	*phEncoderProcObj = (HANDLE)(ptEncoderInfo);    

	if (phEncoderProcObj == NULL)    return ERR_OUT_OF_MEMORY;

	memset(ptEncoderInfo, 0, sizeof(TEncoderInfo));
	//printf("[VENC_ENCODER](%d) sizeof(TEncoderInfo)=%x ptEncoderInfo=%p\n",__LINE__,sizeof(TEncoderInfo),ptEncoderInfo);
	if (sem_init(&(ptEncoderInfo->smStart), 0, 0) != 0) {
		return S_FAIL;
	}
	if (sem_init(&(ptEncoderInfo->smEnd), 0, 0) !=0) {
		return S_FAIL;
	}
	if (sem_init(&(ptEncoderInfo->smOptions), 0, 1) != 0) {
		return S_FAIL;
	}
	if (sem_init(&(ptEncoderInfo->smMsgOptions), 0, 1) != 0) {
		return S_FAIL;
	}

	ptEncoderInfo->dwParentPID = getpid();
	ptEncoderInfo->dwCallbackInstance = tEncoderInitOpt.dwCallbackInstance;
	ptEncoderInfo->hMemMgrObj = tEncoderInitOpt.hMemMgrObj;
	ptEncoderInfo->dwSharedBufferMinorNum = tEncoderInitOpt.dwSharedBufferMinorNum;
	ptEncoderInfo->dwCurId = tEncoderInitOpt.dwCurId;
	memcpy(&ptEncoderInfo->_tInitNeedInfo, &tEncoderInitOpt.tInitNeedInfo, sizeof(_TEncoderInitNeedInfo));
	ptEncoderInfo->eVPMode = vpmStreamEncJPEG;	// default set to MJPEG
#ifdef _VMA_MEBE
	ptEncoderInfo->eVPMode = vpmStreamEncMP4V;
#endif
#ifdef _VMA_H4EE
	ptEncoderInfo->eVPMode = vpmStreamEncH264;	
#endif
	
	ptEncoderInfo->dwVideoResolutionFormatIndex = tEncoderInitOpt.dwVideoResolutionFormatIndex;
	
	ptEncoderInfo->pfnRequestOutputBuffer = VencEncoder_RequestOutputBuffer;
	ptEncoderInfo->pfnReleaseOutputBuffer = VencEncoder_ReleaseOutputBuffer;
	ptEncoderInfo->pfnGetOutputBufferFD = VencEncoder_GetOutputBufferFD;
	ptEncoderInfo->bPreNeedResizeState = FALSE;
	ptEncoderInfo->dwResizePreFrameCapWidth = 0;
	ptEncoderInfo->dwResizePreFrameCapHeight = 0;
	ptEncoderInfo->dwResizePreFrameReqWidthPad = 0;
	ptEncoderInfo->dwResizePreFrameReqHeightPad = 0;
	ptEncoderInfo->dwJPEGSeqNo = 0;
	ptEncoderInfo->dwH264SeqNo = 0;
	ptEncoderInfo->dwMp4vSeqNo = 0;
	ptEncoderInfo->bPreNeedTextOverlayState = FALSE;
	
	ptEncoderInfo->dwH264MediaMemSize = 0;
	ptEncoderInfo->pbyH264InternalBuf = NULL;

	//Initialize the ROI staffs.
	memset(&ptEncoderInfo->tROICfg,0x0,sizeof(TROICfg));
	ptEncoderInfo->tROICfg.bROIEnabled = -1;
	ptEncoderInfo->tROICfg.dwCurWinId = -1;
	
	ptEncoderInfo->bUseOneDMATextoverlay = TRUE;
	ptEncoderInfo->bDoNotDMACopy = TRUE;
	
	for (iROIWinCount = 0; iROIWinCount < MAX_ROI_WIN_NUM; iROIWinCount++)
	{
		memset(&(ptEncoderInfo->tROICfg.atroiwCfg[iROIWinCount]),0x0,sizeof(TROIWinInfo));
		ptEncoderInfo->tROICfg.atroiwCfg[iROIWinCount].bEnabled = -1;
		memset(ptEncoderInfo->tROICfg.atroiwCfg[iROIWinCount].szName,0x0,sizeof(ptEncoderInfo->tROICfg.atroiwCfg[iROIWinCount].szName));
		ptEncoderInfo->tROICfg.atroiwCfg[iROIWinCount].dwLeft = -1;
		ptEncoderInfo->tROICfg.atroiwCfg[iROIWinCount].dwTop = -1;
		ptEncoderInfo->tROICfg.atroiwCfg[iROIWinCount].dwWidth =- 1;
		ptEncoderInfo->tROICfg.atroiwCfg[iROIWinCount].dwHeight = -1;
		ptEncoderInfo->tROICfg.atroiwCfg[iROIWinCount].dwSerialNum = ROI_NON_USED_WINDOW;
		ptEncoderInfo->tROICfg.atroiwCfg[iROIWinCount].bEnabledChange = FALSE;
		ptEncoderInfo->tROICfg.atroiwCfg[iROIWinCount].bPositionChange = FALSE;
		ptEncoderInfo->tROICfg.atroiwCfg[iROIWinCount].bWindowSizeChange = FALSE;
		ptEncoderInfo->tROICfg.atroiwCfg[iROIWinCount].bAnyFieldChange = FALSE;
	}
	
	ptEncoderInfo->tROICfg.bROIEnabledChange = -1;
	ptEncoderInfo->tROICfg.fXTranslate = 0;
	ptEncoderInfo->tROICfg.fYTranslate = 0;
	ptEncoderInfo->tROICfg.fXScale = 0;
	ptEncoderInfo->tROICfg.fYScale = 0;
	ptEncoderInfo->tROI2D.fXTranslate = -1.0;
	ptEncoderInfo->tROI2D.fYTranslate = 1.0;
	ptEncoderInfo->tROI2D.fXScale = 0.003125; 		// (2/640)
	ptEncoderInfo->tROI2D.fYScale = -0.00416667; 	// (2/480)
	
	if (!tEncoderInitOpt.szConfigFile) {
		ptEncoderInfo->szConfigFile = ENCODER_DEFAULT_CONFIG_FILE;
	} else {
		ptEncoderInfo->szConfigFile = tEncoderInitOpt.szConfigFile;
	}
	
	if (access(ptEncoderInfo->szConfigFile, F_OK) != 0) {
		DBPRINT1("[VENC_ENCODER] Configuration file %s not exist!!\n", ptEncoderInfo->szConfigFile);
		return S_FAIL;
	}
	DBPRINT1("[VENC_ENCODER] Configuration file %s access success ...\n", ptEncoderInfo->szConfigFile);

#ifdef _DUMP_VIDEOINDATA
	if ((ptEncoderInfo->pfVideoInData = fopen("/tmp/VencSlaveDumpVideoIn_1280x800_420.yuv", "wb")) == NULL) {
		fprintf(stderr, "[VENC_ENCODER] Open dump videoin output file fail !!\n");
		return S_FAIL;
	}
#endif // _DUMP_VIDEOINDATA

#ifdef _DUMP_RESIZE
	if ((ptEncoderInfo->pfResizeData = fopen("/tmp/DumpResizeData_1920x1088_420.yuv", "wb")) == NULL) {
		fprintf(stderr, "[VENC_ENCODER] Open dump resize output file fail !!\n");
		return S_FAIL;
	}
#endif // _DUMP_RESIZE

#ifdef _DUMP_ENCODER
	if ((ptEncoderInfo->pfMp4VOutput = fopen("/tmp/mp4v.m4v", "wb")) == NULL) {
		fprintf(stderr, "[VENC_ENCODER] Open m4v output file fail !!\n");
		return S_FAIL;
	}
	if ((ptEncoderInfo->pfH264Output = fopen("/tmp/H264Enc.264", "wb")) == NULL) {
		fprintf(stderr, "[VENC_ENCODER] Open H264Enc.out file fail !!\n");
		return S_FAIL;
	}
	ptEncoderInfo->bFirstIntra = FALSE;
#endif // _DUMP_ENCODER

	ptEncoderInfo->tResizeOutBuf0.eBufStatus = evsobsEmpty;
	ptEncoderInfo->tResizeOutBuf1.eBufStatus = evsobsEmpty;

	// Initial Encoder VML instance
	if (VencEncoder_InitVML(ptEncoderInfo) != S_OK) {
		fprintf(stderr, "[VENC_ENCODER] VencEncoder_InitVML error!!\n");
		return S_FAIL;
	}


	ptEncoderInfo->tCropInfo.dwStartX = 0;
	ptEncoderInfo->tCropInfo.dwStartY = 0;
	ptEncoderInfo->tCropInfo.dwWidth = 0;
	ptEncoderInfo->tCropInfo.dwHeight = 0;
	ptEncoderInfo->bRandomCropping = FALSE;

	VencEncoder_InitConfigParam(ptEncoderInfo);
#ifdef VIRTUAL_PTZ
	{
		TEptzInitOpts	tInitOpt;
		memset(&tInitOpt, 0, sizeof(TEptzInitOpts));

		tInitOpt.dwVersion = EPTZ_VERSION;
	   	tInitOpt.fP = 0;
	   	tInitOpt.fT = 0;
	   	tInitOpt.fZ = 1;
	   	tInitOpt.ePixelFormat = YUV420;
	   	tInitOpt.eAddrAlignType = ALIGN_TYPE_8_BYTE;
		tInitOpt.eSizeAlignType = ALIGN_TYPE_8_BYTE;

        if (eptz_init(&ptEncoderInfo->hEPTZObj, &tInitOpt) != S_OK) {
			printf("eptz_init error!\n");
			return S_FAIL;
		}
		ptEncoderInfo->tPTZValue.flPanValue = 0;
		ptEncoderInfo->tPTZValue.flTiltValue = 0;
		ptEncoderInfo->tPTZValue.flZoomValue = 1;
		ptEncoderInfo->tPTZValue.flSpeedValue = 1;
	}
#endif //VIRTUAL_PTZ


	VencSlave_TextOverlay_Initial(&ptEncoderInfo->tTextOverlayData, tEncoderInitOpt.tInitNeedInfo.dwVideoMaxWidth);

	// Creat video encoder loop thread
	iRet = pthread_create(&(ptEncoderInfo->ptThreadID), NULL,
			VencEncoder_Loop,
			(HANDLE)ptEncoderInfo);


	if (iRet != 0) {
		printf("create Encoder thread fail");
		return S_FAIL;
	}

	return S_OK;
}

/* ============================================================================================= */
SCODE VencEncoder_Release(HANDLE *phEncoderProcObj)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(*phEncoderProcObj);

	if (ptEncoderInfo != NULL) {
		ptEncoderInfo->bTerminateThread = TRUE;

		sem_post(&(ptEncoderInfo->smStart));

		sem_wait(&(ptEncoderInfo->smEnd));

		// it should be call after smEnd is post when thread exit.
		VencEncoder_ReleaseVML(ptEncoderInfo);

		// release semaphores
		sem_destroy(&(ptEncoderInfo->smStart));
		sem_destroy(&(ptEncoderInfo->smEnd));
		sem_destroy(&(ptEncoderInfo->smOptions));
		sem_destroy(&(ptEncoderInfo->smMsgOptions));

#ifdef _DUMP_VIDEOINDATA
		fclose(ptEncoderInfo->pfVideoInData);
#endif // _DUMP_VIDEOINDATA

#ifdef _DUMP_RESIZE
		fclose(ptEncoderInfo->pfResizeData);
#endif // _DUMP_RESIZE

#ifdef _DUMP_ENCODER
		fclose(ptEncoderInfo->pfMp4VOutput);
		fclose(ptEncoderInfo->pfH264Output);
#endif // _DUMP_ENCODER
#ifdef VIRTUAL_PTZ
		if (ptEncoderInfo->hEPTZObj != NULL)
			eptz_rls(&ptEncoderInfo->hEPTZObj);
#endif //VIRTUAL_PTZ
		VencSlave_TextOverlay_Release(&ptEncoderInfo->tTextOverlayData);

		free(ptEncoderInfo);
		*phEncoderProcObj = NULL;
		DBPRINT0("[VENC_ENCODER] Encoder Controler Release\n");
	}
	return S_OK;	
}

/* ============================================================================================= */
SCODE VencEncoder_Start(HANDLE hEncoderProcObj)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hEncoderProcObj;

	if (ptEncoderInfo->bRunning == TRUE)
		return S_OK;

	ptEncoderInfo->bRunning = TRUE;
	DBPRINT0("[VENC_ENCODER] Encoder Controler Start\n");
	sem_post(&(ptEncoderInfo->smStart));
	return S_OK;
}

/* ============================================================================================= */
SCODE VencEncoder_Stop(HANDLE hEncoderProcObj)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hEncoderProcObj;

	if (ptEncoderInfo != NULL) {
		if (ptEncoderInfo->bRunning == FALSE) return S_OK;

		ptEncoderInfo->bExitInnerLoop = FALSE;
		ptEncoderInfo->bRunning = FALSE;

		while (ptEncoderInfo->bExitInnerLoop == FALSE) {
			DBPRINT0("[VENC_ENCODER] Encoder Controler Stopping\n");
			usleep(100000);
		}
		DBPRINT0("[VENC_ENCODER] Encoder Controler Stop\n");
	}
	return S_OK;
}

/* ============================================================================================= */
void VencEncoder_ResetConfigFlag(HANDLE hEncoderProcObj)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hEncoderProcObj;
	if (hEncoderProcObj == NULL) {
		fprintf(stderr, "[VENC_ENCODER] Encoder setoptions fail: NULL Pointer!!\n");
		return;		
	}

	sem_wait(&(ptEncoderInfo->smOptions));
	ptEncoderInfo->bReConf_flag = TRUE;
	sem_post(&(ptEncoderInfo->smOptions));

}

/* ============================================================================================= */
void VencEncoder_MsgReceive(HANDLE hEncoderProcObj, TEncoderMsgOptions tEncoderMsgOpt)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hEncoderProcObj;
	
	
	if (hEncoderProcObj == NULL) {
		fprintf(stderr, "[VENC_ENCODER] VencEncoder_MsgReceive fail: NULL Pointer!!\n");
		return;		
	}
	switch (tEncoderMsgOpt.eenMsgOptType) {
		case VENCENCODER_MSGOPTION_SET_FORCEINTRAL:
			ptEncoderInfo->tMsgOptRec.bForceIntra = TRUE;
			break;
		case VENCENCODER_MSGOPTION_SET_FORCECI:
			ptEncoderInfo->tMsgOptRec.bForceCI = TRUE;
			break;
		case VENCENCODER_MSGOPTION_SET_REG_OUTPUT:
			++ptEncoderInfo->tMsgOptRec.dwVisitors;
			break;
		case VENCENCODER_MSGOPTION_SET_UNREG_OUTPUT:
			if (ptEncoderInfo->tMsgOptRec.dwVisitors > 0)
				--ptEncoderInfo->tMsgOptRec.dwVisitors;
			break;
#ifdef VIRTUAL_PTZ
		case VENCENCODER_MSGOPTION_SET_VPTZ_ENABLE:
            ptEncoderInfo->tMsgOptRec.bVPTZCmd = TRUE;
            ptEncoderInfo->tMsgOptRec.bVPTZEnableUpdate = TRUE;
            ptEncoderInfo->tMsgOptRec.bVPTZEnableValue = TRUE;
			break;
		case VENCENCODER_MSGOPTION_SET_VPTZ_DISABLE:
            ptEncoderInfo->tMsgOptRec.bVPTZCmd = TRUE;
            ptEncoderInfo->tMsgOptRec.bVPTZEnableUpdate = TRUE;
            ptEncoderInfo->tMsgOptRec.bVPTZEnableValue = FALSE;
			break;
		case VENCENCODER_MSGOPTION_SET_VPTZ_PAN:
            ptEncoderInfo->tMsgOptRec.bVPTZCmd = TRUE;
            ptEncoderInfo->tMsgOptRec.bVPTZPanUpdate = TRUE;
            ptEncoderInfo->tMsgOptRec.flPanValue = tEncoderMsgOpt.aflUserData[0];
            break;
		case VENCENCODER_MSGOPTION_SET_VPTZ_TILT:
            ptEncoderInfo->tMsgOptRec.bVPTZCmd = TRUE;
            ptEncoderInfo->tMsgOptRec.bVPTZTiltUpdate = TRUE;
            ptEncoderInfo->tMsgOptRec.flTiltValue = tEncoderMsgOpt.aflUserData[0];
            break;
		case VENCENCODER_MSGOPTION_SET_VPTZ_ZOOM:
            ptEncoderInfo->tMsgOptRec.bVPTZCmd = TRUE;
            ptEncoderInfo->tMsgOptRec.bVPTZZoomUpdate = TRUE;
            ptEncoderInfo->tMsgOptRec.flZoomValue = tEncoderMsgOpt.aflUserData[0];
            break;
		case VENCENCODER_MSGOPTION_SET_VPTZ_SPEED:
            ptEncoderInfo->tMsgOptRec.bVPTZCmd = TRUE;
            ptEncoderInfo->tMsgOptRec.bVPTZSpeedUpdate = TRUE;
            ptEncoderInfo->tMsgOptRec.flSpeedValue = tEncoderMsgOpt.aflUserData[0];
            break;
#endif //VIRTUAL_PTZ
		case VENCENCODER_MSGOPTION_ENABLE_FRAMERATE_CAL:
			ptEncoderInfo->tMsgOptRec.bTurnOnFRCalculate = TRUE;
			break;
		case VENCENCODER_MSGOPTION_DISABLE_FRAMERATE_CAL:
			ptEncoderInfo->tMsgOptRec.bTurnOffFRCalculate = TRUE;
			break;
#ifdef _JPEG_SNAPSHOT_				
		case VENCENCODER_MSGOPTION_GET_SNAPSHOT:
			ptEncoderInfo->tMsgOptRec.bGetSnapshot = TRUE;
			break;
#endif // _JPEG_SNAPSHOT_			
		case VENCENCODER_MSGOPTION_SET_CONTROL_MODE:
			ptEncoderInfo->tMsgOptRec.dwCtrlMode = (DWORD)tEncoderMsgOpt.aflUserData[0];
			ptEncoderInfo->tMsgOptRec.bQualityParameterChanged = TRUE;
			break;
		case VENCENCODER_MSGOPTION_SET_QUANT:
			ptEncoderInfo->tMsgOptRec.dwQuant = (DWORD)tEncoderMsgOpt.aflUserData[0];
			ptEncoderInfo->tMsgOptRec.bQualityParameterChanged = TRUE;
			break;
		case VENCENCODER_MSGOPTION_SET_BITRATE:
			ptEncoderInfo->tMsgOptRec.dwBitrate = (DWORD)tEncoderMsgOpt.aflUserData[0];
			ptEncoderInfo->tMsgOptRec.bQualityParameterChanged = TRUE;
			break;
		case VENCENCODER_MSGOPTION_SET_CODEC_TYPE:
			ptEncoderInfo->tMsgOptRec.dwCodecType = (DWORD)tEncoderMsgOpt.aflUserData[0];
			ptEncoderInfo->tMsgOptRec.bQualityParameterChanged = TRUE;
			break;
		case VENCENCODER_MSGOPTION_GET_QUALITY_PARAMETER:
			ptEncoderInfo->tMsgOptRec.bDumpQualityParameter = TRUE;
			break;
		case VENCENCODER_MSGOPTION_SET_ROI:
			ptEncoderInfo->tMsgOptRec.bSetROI = TRUE;
			break;
		default:
			DBPRINT0("[VENC_ENCODER] Unknown message option\n");
			break;
	}  

	sem_wait(&(ptEncoderInfo->smMsgOptions));
	ptEncoderInfo->bReMsg_flag = TRUE;
	sem_post(&(ptEncoderInfo->smMsgOptions));

}
