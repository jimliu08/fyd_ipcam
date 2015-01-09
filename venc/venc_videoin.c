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
 * venc_videoin.c
 *
 * \brief
 * Video in process implement.
 *
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#include "venc_videoin_local.h"

/* ============================================================================================= */
SCODE VencVideoIn_Initial(HANDLE *phVideoInProcObj, TVideoInInitOpt tVideoInInitOpt)
{
	TVideoInInfo *ptVideoInInfo;
	int iRet;    
	DWORD	dwIndex;

	ptVideoInInfo = (TVideoInInfo *)malloc(sizeof(TVideoInInfo));
	*phVideoInProcObj = (HANDLE)(ptVideoInInfo);

	if (phVideoInProcObj == NULL)	return ERR_OUT_OF_MEMORY;

	memset(ptVideoInInfo, 0, sizeof(TVideoInInfo));

	if (sem_init(&(ptVideoInInfo->smStart), 0, 0) != 0) {
		return S_FAIL;
	}
	if (sem_init(&(ptVideoInInfo->smEnd), 0, 0) != 0) {
		return S_FAIL;
	}
	if (sem_init(&(ptVideoInInfo->smOptions), 0, 1) != 0) {
		return S_FAIL;
	}
	if (sem_init(&(ptVideoInInfo->smMsgOptions), 0, 1) != 0) {
		return S_FAIL;
	}
	ptVideoInInfo->dwParentPID = getpid();
	ptVideoInInfo->dwCallbackInstance = tVideoInInitOpt.dwCallbackInstance;
	ptVideoInInfo->dwDevNum = tVideoInInitOpt.dwDevNum;	
	ptVideoInInfo->hMemMgrObj = tVideoInInitOpt.hMemMgrObj;
	ptVideoInInfo->dwSharedBufferMinorNum = tVideoInInitOpt.dwSharedBufferMinorNum;
	ptVideoInInfo->hVideoCapObj = tVideoInInitOpt.hVideoCapObj;
	memcpy(&ptVideoInInfo->_tInitNeedInfo, &tVideoInInitOpt.tInitNeedInfo, sizeof(_TVideoInInitNeedInfo));

	ptVideoInInfo->dwOutputSharedBufferNum = tVideoInInitOpt.dwOutputSharedBufferNum;

	ptVideoInInfo->dwAllocatedMemForCapBufWidth = tVideoInInitOpt.tInitNeedInfo.dwVideoMaxWidth;

	ptVideoInInfo->fdIRCut = tVideoInInitOpt.fdIRCut;
	ptVideoInInfo->fdIRCutUART = tVideoInInitOpt.fdIRCutUART;
	printf("[venc_videoin](%d)(fdIRCutUART,fdIRCut)=(%d,%d)\n", __LINE__, ptVideoInInfo->fdIRCutUART, ptVideoInInfo->fdIRCut);

	ptVideoInInfo->eviStatus = NOPREPROCESS;

	// if config doesn't setting, the default value is for 320x240 config resolution.
	ptVideoInInfo->tMotionCfg.fXTranslate = -1.0;
	ptVideoInInfo->tMotionCfg.fYTranslate = 1.0;
	ptVideoInInfo->tMotionCfg.fXScale = 0.00625;
	ptVideoInInfo->tMotionCfg.fYScale = -0.00833333;

	// if config doesn't setting, the default value is for 640x480 resolution.
	ptVideoInInfo->tMtnR2D.fXTranslate = -1.0;
	ptVideoInInfo->tMtnR2D.fYTranslate = 1.0;
	ptVideoInInfo->tMtnR2D.fXScale = 0.003125; 		// (2/640)
	ptVideoInInfo->tMtnR2D.fYScale = -0.00416667; 	// (2/480)

	for (dwIndex = 0; dwIndex < MAX_MOTION_WIN_NUM; dwIndex++) { // MAX_MOTION_WIN_NUM = 3
		ptVideoInInfo->tMotionCfg.atmwCfg[dwIndex].dwSerialNum = NON_USED_WINDOW;  // NON_USED_WINDOW = 0xFFFFFFFF
	}

	if (!tVideoInInitOpt.szConfigFile) {
		ptVideoInInfo->szConfigFile = VIDEOIN_DEFAULT_CONFIG_FILE;
	} else {
		ptVideoInInfo->szConfigFile = tVideoInInitOpt.szConfigFile;
	}

	if (access(ptVideoInInfo->szConfigFile, F_OK) != 0) {
		DBPRINT1("[VENC_VIDEOIN] Configuration file %s not exist!!\n", ptVideoInInfo->szConfigFile);
		return S_FAIL;
	}
	DBPRINT1("[VENC_VIDEOIN] Configuration file %s access success ...\n", ptVideoInInfo->szConfigFile);

#ifdef _DUMP_VIDEOIN
	if ((ptVideoInInfo->pfCapOutput = fopen("/home/acer.tsai/dump/VideoIn_1280x1024_420.yuv", "wb")) == NULL) {
		fprintf(stderr, "[VENC_VIDEOIN] Open videoin file fail !!\n");
		return S_FAIL;
	}
#endif // _DUMP_VIDEOIN 
#ifdef _DUMP_SHAREDBUFFERDATA
	if ((ptVideoInInfo->pfSharedBufferDataOut = fopen("/home/acer.tsai/dump/SharedBufferData_640x480_420.yuv", "wb")) == NULL) {
		fprintf(stderr, "[VENC_VIDEOIN] Open sharedbuffer ouput file fail !!\n");
		return S_FAIL;
	}
#endif // _DUMP_SHAREDBUFFERDATA



	if (VencVideoIn_InitVML(ptVideoInInfo) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] VencVideoIn_InitVML error!!\n");
		return S_FAIL;
	}
	// Initial VideoIn VML states structure with default values
	if (VencVideoIn_InitStates(ptVideoInInfo) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] VencVideoIn_InitStates error!!\n");
		return S_FAIL;
	}

	// initial some config parameter
	if (VencVideoIn_InitConfigParam(ptVideoInInfo) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] VencVideoIn_InitConfigParam error!!\n");
		return S_FAIL;
	}

	// Creat video in loop thread
	iRet = pthread_create(&(ptVideoInInfo->ptThreadID), NULL,
						  VencVideoIn_Loop,
						  (HANDLE)ptVideoInInfo);


	if (iRet != 0) {
		printf("[VENC_VIDEOIN] create videoin thread fail");
		return S_FAIL;
	}

	return S_OK;
}

/* ============================================================================================= */
SCODE VencVideoIn_Release(HANDLE *phVideoInProcObj)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)(*phVideoInProcObj);

	if (ptVideoInInfo != NULL) {
		ptVideoInInfo->bTerminateThread = TRUE;

		sem_post(&(ptVideoInInfo->smStart));

		sem_wait(&(ptVideoInInfo->smEnd));
		// it should be call after smEnd is post when thread exit.
		VencVideoIn_ReleaseVML(ptVideoInInfo);

		// release semaphores
		sem_destroy(&(ptVideoInInfo->smStart));
		sem_destroy(&(ptVideoInInfo->smEnd));
		sem_destroy(&(ptVideoInInfo->smOptions));
		sem_destroy(&(ptVideoInInfo->smMsgOptions));

#ifdef _DUMP_VIDEOIN
		fclose(ptVideoInInfo->pfCapOutput);
#endif // _DUMP_VIDEOIN

#ifdef _DUMP_SHAREDBUFFERDATA
		fclose(ptVideoInInfo->pfSharedBufferDataOut);
#endif // _DUMP_SHAREDBUFFERDATA
#if defined(_VMA_IBPE)		
		{
			int i = 0;

			for (i = 0; i < BUFF_SIZE; i++)
			{
				if (ptVideoInInfo->aptMotionDetectResultInfo[i] != NULL)
				{
					free(ptVideoInInfo->aptMotionDetectResultInfo[i]);
					ptVideoInInfo->aptMotionDetectResultInfo[i] = NULL;
				}
			} 
		}
#endif
		free(ptVideoInInfo);
		*phVideoInProcObj = NULL;
		DBPRINT0("[VENC_VIDEOIN] VideoIn Controler Release\n");
	}
	return S_OK;	
}

/* ============================================================================================= */
SCODE VencVideoIn_Start(HANDLE hVideoInProcObj)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)(hVideoInProcObj);

	if (ptVideoInInfo->bRunning == TRUE)
		return S_OK;

	ptVideoInInfo->bRunning = TRUE;
	DBPRINT0("[VENC_VIDEOIN] VideoIn Controler Start\n");
	sem_post(&(ptVideoInInfo->smStart));

	return S_OK;
}

/* ============================================================================================= */
SCODE VencVideoIn_Stop(HANDLE hVideoInProcObj)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)(hVideoInProcObj);

	if (ptVideoInInfo->bRunning == FALSE) return S_OK;

	ptVideoInInfo->bExitInnerLoop = FALSE;
	ptVideoInInfo->bRunning = FALSE;

	while (ptVideoInInfo->bExitInnerLoop == FALSE) {
		DBPRINT0("[VENC_VIDEOIN] VideoIn Controler Stopping\n");
		usleep(100000);
	}
	DBPRINT0("[VENC_VIDEOIN] VideoIn Controler Stop\n");
	return S_OK;
}

/* ============================================================================================= */
void VencVideoIn_ResetConfigFlag(HANDLE hVideoInProcObj)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)(hVideoInProcObj);
	if (ptVideoInInfo == NULL) {
		fprintf(stderr, "[VENC_VIDEOIN] VideoIn setoptions fail: NULL Pointer!!\n");
		return;		
	}

	sem_wait(&(ptVideoInInfo->smOptions));
	ptVideoInInfo->bReConf_flag = TRUE;
	sem_post(&(ptVideoInInfo->smOptions));
}

/* ============================================================================================= */
void VencVideoin_MsgReceive(HANDLE hVideoInProcObj, TVideoinMsgOptions tVideoinMsgOpt)
{
	TVideoInInfo  *ptVideoInInfo = (TVideoInInfo *)hVideoInProcObj;
	if (hVideoInProcObj == NULL) {
		fprintf(stderr, "[VENC_MASTER] VencVideoin_MsgReceive fail: NULL Pointer!!\n");
		return;		
	}

	memset(&(ptVideoInInfo->tMsgOptRec), 0x0, sizeof(TVideoInMsgOptRec));

	switch(tVideoinMsgOpt.emasMsgOptType) {
		case VENCVIDEOIN_MSGOPTION_DUMP_VIDEOCAPBUF:
			ptVideoInInfo->tMsgOptRec.bDumpVideoCapBuf = TRUE;
			break;
		case VENCVIDEOIN_MSGOPTION_DUMP_SHAREDBUF:
			ptVideoInInfo->tMsgOptRec.bDumpSharedBuf = TRUE;
			break;
		case VENCVIDEOIN_MSGOPTION_RETRIEVE_VIDEOCAPSTATUS:
			ptVideoInInfo->tMsgOptRec.bRetVideoCapStatus = TRUE;
			break;
		case VENCVIDEOIN_MSGOPTION_PHOTOMTC_DEFINETBL1:
			ptVideoInInfo->tMsgOptRec.bPhotoMtcDefTbl1 = TRUE;
			break;
		case VENCVIDEOIN_MSGOPTION_PHOTOMTC_DEFINETBL2:
			ptVideoInInfo->tMsgOptRec.bPhotoMtcDefTbl2 = TRUE;
			break;	
		case VENCVIDEOIN_MSGOPTION_KEEP_CURRENT_WB:
			ptVideoInInfo->tMsgOptRec.bKeepCurrentWB = TRUE;
			break;
		case VENCVIDEOIN_MSGOPTION_KEEP_CURRENT_WB_FROM_FILE:
			ptVideoInInfo->tMsgOptRec.bKeepCurrentWBFromFile = TRUE;
			break;
		case VENCVIDEOIN_MSGOPTION_ENABLE_FRAMERATE_CAL:
			ptVideoInInfo->tMsgOptRec.bTurnOnFRCalculate = TRUE;
			break;
		case VENCVIDEOIN_MSGOPTION_DISABLE_FRAMERATE_CAL:
			ptVideoInInfo->tMsgOptRec.bTurnOffFRCalculate = TRUE;
			break;
		case VENCVIDEOIN_MSGOPTION_DUMP_AF_STATUS:
			ptVideoInInfo->tMsgOptRec.bDumpAFStatus = TRUE;
			break;
		case VENCVIDEOIN_MSGOPTION_ISP_ENABLE:
			ptVideoInInfo->tMsgOptRec.bISPEnable = TRUE;
			break;
		case VENCVIDEOIN_MSGOPTION_ISP_DISABLE:
			ptVideoInInfo->tMsgOptRec.bISPEnable = FALSE;
			break;
		case VENCVIDEOIN_MSGOPTION_ISP_UPDATE_STATE:
			ptVideoInInfo->tMsgOptRec.bISPUpdateState = TRUE;
			break;
		case VENCVIDEOIN_MSGOPTION_CLEAR_IBPE_INTERNAL_BUFFER:
			ptVideoInInfo->tMsgOptRec.bClearIBPEInternalBuffer = TRUE;
			break;
		default:
			DBPRINT0("[VENC_VIDEOIN] Unknown message option\n");
			break;
	}  

	sem_wait(&(ptVideoInInfo->smMsgOptions));
	ptVideoInInfo->bReMsg_flag = TRUE;
	sem_post(&(ptVideoInInfo->smMsgOptions));

}
