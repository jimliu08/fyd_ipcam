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
 * venc_videoin_handler.c
 *
 * \brief
 * Video in four handler function implementation
 *
 * \date
 * 2011/10/20
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#include "venc_videoin_local.h"

int CurWrittingBufIDX = 0;

/* ========================================================================= */
SCODE VencVideoIn_DMACHandler(TVideoInInfo *ptVideoInInfo, 
							BYTE *ptInBuf, 
							BYTE *ptOutBuf, 
							DWORD dwSrcWidth, 
							DWORD dwSrcHeight, 
							DWORD dwSrcStride,
							DWORD dwDstWidth,
							DWORD dwDstHeight, 
							DWORD dwDstStride, 
							DWORD dwSrcBus,
						   	DWORD dwDstBus
							)
{
	TMassMemAccessState *ptMassMemAccState = &(ptVideoInInfo->tMassMemAccState);
	BOOL  b1DMethod;

	b1DMethod = (dwSrcWidth == dwSrcStride)? TRUE: FALSE;
	//printf("[venc_videoin_handler](%d)dwSrcWidth=%u dwSrcStride=%u b1DMethod=%d\n",__LINE__,dwSrcWidth,dwSrcStride,b1DMethod);
	if (b1DMethod == TRUE) {
		ptMassMemAccState->b2DEnable = FALSE;
		ptMassMemAccState->bUsePhysAddr = FALSE;
		ptMassMemAccState->bFillingEnable = FALSE;
		ptMassMemAccState->dwSrcBusNum = dwSrcBus;
		ptMassMemAccState->dwDstBusNum = dwDstBus;
		ptMassMemAccState->dwMaxBurstLength = 128;
		ptMassMemAccState->bMaskEnable = FALSE;
		ptMassMemAccState->pbySrcAddr = ptInBuf;
		ptMassMemAccState->pbyDstAddr = ptOutBuf;
		ptMassMemAccState->dwTransSize = (dwSrcStride * dwSrcHeight);
		ptMassMemAccState->dwSrcStride = 0;
		ptMassMemAccState->dwDstStride = 0;
		ptMassMemAccState->dwDstWidth = 0;
		ptMassMemAccState->dwDstHeight = 0;
	} else {
		ptMassMemAccState->b2DEnable = TRUE;
		ptMassMemAccState->bUsePhysAddr = FALSE;
		ptMassMemAccState->bFillingEnable = FALSE;
		ptMassMemAccState->dwSrcBusNum = dwSrcBus;
		ptMassMemAccState->dwDstBusNum = dwDstBus;
		ptMassMemAccState->dwMaxBurstLength = 128;
		ptMassMemAccState->bMaskEnable = FALSE;
		ptMassMemAccState->pbySrcAddr = ptInBuf;
		ptMassMemAccState->pbyDstAddr = ptOutBuf;
		ptMassMemAccState->dwTransSize = 0;
		ptMassMemAccState->dwSrcStride = dwSrcStride;
		ptMassMemAccState->dwDstStride = dwDstStride;
		ptMassMemAccState->dwDstWidth = dwDstWidth;
		ptMassMemAccState->dwDstHeight = dwDstHeight;
	}

	if (MassMemAccess_ProcessOneFrame(ptVideoInInfo->hMassMemAccObj, ptMassMemAccState) != S_OK) {
		printf("[VENC_VIDEOIN] DMAC process frame error !!\n");
		return S_FAIL;
	}

	return S_OK;    
}
/* ============================================================================================= */
SCODE VencVideoIn_RequestOutputBuffer(TVideoInInfo *ptVideoInInfo, TSharedBuffMgrState *ptSrdBufMgrState)
{
	fd_set               	fdWrite;
	struct timeval 			timeout;
	int						fd = ptVideoInInfo->iShardBufferFD;
	int     				iResult;

	if (SharedBuffMgr_GetBuffer(ptVideoInInfo->hSharedBuferObj, ptSrdBufMgrState) == S_OK)
        return S_OK;

	FD_ZERO(&fdWrite);
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	FD_SET(fd, &fdWrite);

	iResult = select(fd+1, NULL, &fdWrite, NULL, &timeout);
	if (iResult <= 0) {
		DBPRINT0("[VENC_VIDEOIN] get videoin-encoder shared buffer select timeout.... \n");
	}
	if (FD_ISSET(fd, &fdWrite)) {
		if (SharedBuffMgr_GetBuffer(ptVideoInInfo->hSharedBuferObj, ptSrdBufMgrState) == S_OK) {
			return S_OK;
		}
	}

	return S_FAIL;
}

/* ========================================================================= */
void VencVideoIn_FillHdrData(TVideoFrameBuffer *ptSrcVFB, TVideoFrameBuffer *ptDstVFB)
{
	ptDstVFB->dwSecond = ptSrcVFB->dwSecond;
	ptDstVFB->dwMilliSecond = ptSrcVFB->dwMilliSecond;
	ptDstVFB->dwMicroSecond = ptSrcVFB->dwMicroSecond;
	ptDstVFB->dwStride = ptSrcVFB->dwStride;
	ptDstVFB->vFormat = ptSrcVFB->vFormat;
	ptDstVFB->eBufStatus = ptSrcVFB->eBufStatus;
	ptDstVFB->dwFrameCount	= ptSrcVFB->dwFrameCount;
	ptSrcVFB->eBufStatus = evsobsEmpty;
}

/* ========================================================================= */
void VencVideoIn_FillUserData(TVideoInInfo *ptVideoInInfo, BYTE *pbyData, TMotionResultPack *ptMtnResPack)
{
	TUserData *ptUserData = (TUserData *)(pbyData + sizeof(TVideoFrameBuffer));
	TMotionResultInfo *ptMtnData = (TMotionResultInfo *)((ptUserData + sizeof(TUserData)));
	ptUserData->dwMotionNum = 0;	// reset the value
	ptUserData->dwUserDataLength = 0;

	if (ptMtnResPack == NULL)
	{
		ptUserData->dwUserDataLength = 0; // default to zero.
		ptUserData->bMotionEnable = FALSE;
	}
	else
	{
		ptUserData->dwMotionNum = ptMtnResPack->dwMotionNum;
		ptUserData->bMotionEnable = TRUE;
		if (ptMtnResPack->dwMotionNum > 0)
		{  
			memcpy(ptMtnData, ptMtnResPack->atMtnRetInfo, sizeof(TMotionResultInfo) * ptUserData->dwMotionNum);
			if (ptUserData->dwMotionNum > 0) {
				memcpy(&(ptUserData->tMtnR2D), &(ptVideoInInfo->tMtnR2D), sizeof(ptUserData->tMtnR2D));
				// - sizeof(DWORD) means dwUserData field
				ptUserData->dwUserDataLength = sizeof(TUserData) - sizeof(DWORD) + (ptUserData->dwMotionNum * sizeof(TMotionResultInfo));
			}
			ptMtnResPack->dwMotionNum = 0; 
		}
	}
}
/* ========================================================================= */
#ifdef _VMA_IBPE
static BOOL GetMDWindowInfo(TVideoInInfo *ptVideoInInfo,TMotionDetectWindow *ptWindowInfo,DWORD dwSerialNum)
{	
	DWORD dwIndex = 0;
	BOOL bGot = FALSE;
	TImgBackEndProcessState *ptIBPState = &(ptVideoInInfo->tIBPState);
	TMotionDetectWindow *ptr = NULL;

	if (ptWindowInfo != NULL)
	{
		ptr = ptIBPState->tOutBuff.ptMotionDetectInfo;
		for (dwIndex = 0; dwIndex < MAX_MOTION_WIN_NUM; dwIndex++) 
		{
			if (ptr[dwIndex].dwSerialNum == dwSerialNum)
			{
				memcpy(ptWindowInfo, &ptr[dwIndex], sizeof(TMotionDetectWindow));
				bGot = TRUE;
			}
		}
	}
	return bGot;
}

/* ========================================================================= */
typedef struct md_result_info
{
	DWORD dwIndex;
	TMotionDetectWindow tMotionDetectWindow;	
} TMDResultInfo;

/* ========================================================================= */
void VencVideoIn_FillMtnData(TVideoInInfo *ptVideoInInfo, TMotionResultPack *ptMtnResPack)
{
	TMDResultInfo atMDResultInfo[MAX_MOTION_WIN_NUM];
	TMotionDetectWindow tMotionDetectWindow;

	TMotionWinInfo *ptMotionWinInfo = NULL;
	DWORD	dwIndex = 0;
	DWORD 	dwCount = 0;
	DWORD 	dwMotionPercent = 0;
	DWORD 	dwTotalMotionNum = 0;
	BOOL bIsAMotionWinEnable = FALSE;

	for (dwIndex = 0; dwIndex < MAX_MOTION_WIN_NUM; dwIndex++) 
	{
		atMDResultInfo[dwIndex].tMotionDetectWindow.bIsMotion = FALSE;
		atMDResultInfo[dwIndex].tMotionDetectWindow.dwSerialNum = -1;   
	}

	for (dwIndex = 0; dwIndex < MAX_MOTION_WIN_NUM; dwIndex++) 
	{
		ptMotionWinInfo = &(ptVideoInInfo->tMotionCfg.atmwCfg[dwIndex]);
		if (ptMotionWinInfo->bEnabled == TRUE)
		{
			if (GetMDWindowInfo(ptVideoInInfo, &tMotionDetectWindow, ptMotionWinInfo->dwSerialNum) == TRUE)
			{
				atMDResultInfo[dwCount].dwIndex = dwIndex;
				memcpy(&atMDResultInfo[dwCount].tMotionDetectWindow, &tMotionDetectWindow, sizeof(TMotionDetectWindow));
				dwCount++;
			}
			bIsAMotionWinEnable = TRUE;
		}
	}

	if (bIsAMotionWinEnable == FALSE)
	{
		ptMtnResPack->dwMotionNum = 0;
		return;
	}
	for (dwIndex = 0; dwIndex < dwCount; dwIndex++)
	{
		memcpy(&tMotionDetectWindow, &atMDResultInfo[dwIndex].tMotionDetectWindow, sizeof(TMotionDetectWindow));
		if (tMotionDetectWindow.bIsMotion == TRUE)
		{
			dwTotalMotionNum = tMotionDetectWindow.dwInMaskPixNum + tMotionDetectWindow.dwOutMaskPixNum;
			if ((tMotionDetectWindow.dwWidth != 0) && (tMotionDetectWindow.dwHeight != 0))
			{
				if ((tMotionDetectWindow.dwWidth * tMotionDetectWindow.dwHeight) != 0)
				{
					dwMotionPercent = (dwTotalMotionNum * 100) / (tMotionDetectWindow.dwWidth * tMotionDetectWindow.dwHeight);
				}
			}
			if (dwMotionPercent > 0)
			{
				ptMtnResPack->atMtnRetInfo[ptMtnResPack->dwMotionNum].dwIndex = atMDResultInfo[dwIndex].dwIndex;
				ptMtnResPack->atMtnRetInfo[ptMtnResPack->dwMotionNum].byPercent = dwMotionPercent;
				ptMtnResPack->atMtnRetInfo[ptMtnResPack->dwMotionNum].byThreshold = tMotionDetectWindow.dwMotionPercentThr;
				ptMtnResPack->atMtnRetInfo[ptMtnResPack->dwMotionNum].dwLeft = tMotionDetectWindow.dwHorzStart;
				ptMtnResPack->atMtnRetInfo[ptMtnResPack->dwMotionNum].dwTop = tMotionDetectWindow.dwVertStart;
				ptMtnResPack->atMtnRetInfo[ptMtnResPack->dwMotionNum].dwRight = tMotionDetectWindow.dwHorzStart + tMotionDetectWindow.dwWidth;
				ptMtnResPack->atMtnRetInfo[ptMtnResPack->dwMotionNum].dwBottom = tMotionDetectWindow.dwVertStart + tMotionDetectWindow.dwHeight;
#if defined(_MD_TRIGER_ROI_)	
				ptMtnResPack->atMtnRetInfo[ptMtnResPack->dwMotionNum].dwCfgLeft = ptMotionWinInfo->dwLeft;
				ptMtnResPack->atMtnRetInfo[ptMtnResPack->dwMotionNum].dwCfgTop = ptMotionWinInfo->dwTop;
				ptMtnResPack->atMtnRetInfo[ptMtnResPack->dwMotionNum].dwCfgWidth = ptMotionWinInfo->dwWidth;
				ptMtnResPack->atMtnRetInfo[ptMtnResPack->dwMotionNum].dwCfgHeight = ptMotionWinInfo->dwHeight;
#endif
				ptMtnResPack->dwMotionNum++;
				if (ptMtnResPack->dwMotionNum > MAX_MOTION_WIN_NUM)
				{
					ptMtnResPack->dwMotionNum = 0;
				}
			}
		}
	}
}

/* ========================================================================= */	
#if 0
static void VencVideoIn_RotateIBPEBuffInfoQueue(TVideoInInfo *ptVideoInInfo)
{
	TImgBackEndProcessBuffInfo tBuffTemp;
	int i = 0;
	
	if (ptVideoInInfo->tIBPState.bDeInterlaceEn == TRUE)
	{
		memcpy(&tBuffTemp,&ptVideoInInfo->atImgBackEndProcessBuffInfo[0],sizeof(TImgBackEndProcessBuffInfo));
		for (i=0; i< BUFF_SIZE-1; i++)
		{
			memcpy(&ptVideoInInfo->atImgBackEndProcessBuffInfo[i],&ptVideoInInfo->atImgBackEndProcessBuffInfo[i+1],sizeof(TImgBackEndProcessBuffInfo));;
		}
		memcpy(&ptVideoInInfo->atImgBackEndProcessBuffInfo[i],&tBuffTemp,sizeof(TImgBackEndProcessBuffInfo));
	}
	else
	{
		memcpy(&tBuffTemp, &ptVideoInInfo->atImgBackEndProcessBuffInfo[BUFF_SIZE-1], sizeof(TImgBackEndProcessBuffInfo));
		for (i = (BUFF_SIZE-1); i > 0; i--) 
		{
			memcpy(&ptVideoInInfo->atImgBackEndProcessBuffInfo[i-1], &ptVideoInInfo->atImgBackEndProcessBuffInfo[i], sizeof(TImgBackEndProcessBuffInfo));
		}
		memcpy(&ptVideoInInfo->atImgBackEndProcessBuffInfo[0], &tBuffTemp, sizeof(TImgBackEndProcessBuffInfo));
	}
	return;
}
#endif
/* ========================================================================= */
SCODE VencVideoIn_ImgBackEndProcessSetting(TVideoInInfo *ptVideoInInfo, 
										   TVideoFrameBuffer *ptOutBuf,
										   DWORD dwPhase,
										   BOOL bInBufFromVIC)
{
	int i = 0;
	static BOOL bFirst = TRUE;
	TImgBackEndProcessState *ptIBPState = &(ptVideoInInfo->tIBPState);
#ifdef _PRE_NR_	
	TImgBackEndProcessState *ptSecondPhaseIBPState = &(ptVideoInInfo->tSecondPhaseIBPState);
	memcpy(ptSecondPhaseIBPState, ptIBPState, sizeof(TImgBackEndProcessState));
#endif	
	if (bFirst == TRUE) 
	{	
		for (i = 0; i < BUFF_SIZE; i++)
		{
			memset(&ptVideoInInfo->atImgBackEndProcessBuffInfo[i], 0x0, sizeof(TImgBackEndProcessBuffInfo));
		}
		for (i = 0; i < BUFF_SIZE; i++)
		{
			ptVideoInInfo->aptMotionDetectResultInfo[i] = (TMotionDetectWindow*)calloc(MAX_MOTION_WIN_NUM, sizeof(TMotionDetectWindow));
		}
		ptIBPState->tOutBuff.tFrameTimeStamp.dwSecond = 0;
		ptIBPState->tOutBuff.tFrameTimeStamp.dwMilliSecond = 0;
		ptIBPState->tOutBuff.tFrameTimeStamp.dwMicroSecond = 0;
		ptIBPState->tOutBuff.tMotionDetectTimeStamp.dwSecond = 0;
		ptIBPState->tOutBuff.tMotionDetectTimeStamp.dwMilliSecond = 0;
		ptIBPState->tOutBuff.tMotionDetectTimeStamp.dwMicroSecond = 0;
		ptIBPState->tOutBuff.bIsMotionDetectResultValid = FALSE;
		ptIBPState->tOutBuff.ptMotionDetectInfo = ptVideoInInfo->aptMotionDetectResultInfo[0];
		
#ifdef _PRE_NR_		
		ptSecondPhaseIBPState->tOutBuff.tFrameTimeStamp.dwSecond = 0;
		ptSecondPhaseIBPState->tOutBuff.tFrameTimeStamp.dwMilliSecond = 0;
		ptSecondPhaseIBPState->tOutBuff.tFrameTimeStamp.dwMicroSecond = 0;
		ptSecondPhaseIBPState->tOutBuff.tMotionDetectTimeStamp.dwSecond = 0;
		ptSecondPhaseIBPState->tOutBuff.tMotionDetectTimeStamp.dwMilliSecond = 0;
		ptSecondPhaseIBPState->tOutBuff.tMotionDetectTimeStamp.dwMicroSecond = 0;
		ptSecondPhaseIBPState->tOutBuff.bIsMotionDetectResultValid = FALSE;
		ptSecondPhaseIBPState->tOutBuff.ptMotionDetectInfo = ptVideoInInfo->aptMotionDetectResultInfo[1];
#endif		
		
		bFirst = FALSE;
	}

	ptIBPState->tOutBuff.pbyYBuff = ptOutBuf->pbyBufY;
	ptIBPState->tOutBuff.pbyCbBuff = ptOutBuf->pbyBufU;
	ptIBPState->tOutBuff.pbyCrBuff = ptOutBuf->pbyBufV;

#ifdef _PRE_NR_	
	ptSecondPhaseIBPState->tOutBuff.pbyYBuff = ptOutBuf->pbyBufY;
	ptSecondPhaseIBPState->tOutBuff.pbyCbBuff = ptOutBuf->pbyBufU;
	ptSecondPhaseIBPState->tOutBuff.pbyCrBuff = ptOutBuf->pbyBufV;
#endif	
	
	for (i = 0; i < BUFF_SIZE; i++) 
	{
		if (ptVideoInInfo->tIBPState.bDeInterlaceEn == TRUE)
		{
#ifdef _PRE_NR_
			if (bInBufFromVIC == FALSE)
			{
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].pbyYBuff = ptVideoInInfo->atIBPEInVFB[i].pbyBufY;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].pbyCbBuff = ptVideoInInfo->atIBPEInVFB[i].pbyBufU;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].pbyCrBuff = ptVideoInInfo->atIBPEInVFB[i].pbyBufV;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].tFrameTimeStamp.dwSecond = ptVideoInInfo->atIBPEInVFB[i].dwSecond;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].tFrameTimeStamp.dwMilliSecond = ptVideoInInfo->atIBPEInVFB[i].dwMilliSecond;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].tFrameTimeStamp.dwMicroSecond = ptVideoInInfo->atIBPEInVFB[i].dwMicroSecond;
			}
			else
#endif
			{
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].pbyYBuff = ptVideoInInfo->atVideoCapOutVFB[ptVideoInInfo->adwVideoCapIndex[i]].pbyBufY;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].pbyCbBuff = ptVideoInInfo->atVideoCapOutVFB[ptVideoInInfo->adwVideoCapIndex[i]].pbyBufU;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].pbyCrBuff = ptVideoInInfo->atVideoCapOutVFB[ptVideoInInfo->adwVideoCapIndex[i]].pbyBufV;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].tFrameTimeStamp.dwSecond = ptVideoInInfo->atVideoCapOutVFB[ptVideoInInfo->adwVideoCapIndex[i]].dwSecond;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].tFrameTimeStamp.dwMilliSecond = ptVideoInInfo->atVideoCapOutVFB[ptVideoInInfo->adwVideoCapIndex[i]].dwMilliSecond;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].tFrameTimeStamp.dwMicroSecond = ptVideoInInfo->atVideoCapOutVFB[ptVideoInInfo->adwVideoCapIndex[i]].dwMicroSecond;
			}
		}
		else
		{
#ifdef _PRE_NR_			
			if (bInBufFromVIC == FALSE)
			{
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].pbyYBuff = ptVideoInInfo->atIBPEInVFB[BUFF_SIZE-1-i].pbyBufY;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].pbyCbBuff = ptVideoInInfo->atIBPEInVFB[BUFF_SIZE-1-i].pbyBufU;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].pbyCrBuff = ptVideoInInfo->atIBPEInVFB[BUFF_SIZE-1-i].pbyBufV;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].tFrameTimeStamp.dwSecond = ptVideoInInfo->atIBPEInVFB[BUFF_SIZE-1-i].dwSecond;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].tFrameTimeStamp.dwMilliSecond = ptVideoInInfo->atIBPEInVFB[BUFF_SIZE-1-i].dwMilliSecond;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].tFrameTimeStamp.dwMicroSecond = ptVideoInInfo->atIBPEInVFB[BUFF_SIZE-1-i].dwMicroSecond;
			}
			else
#endif			
			{
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].pbyYBuff = ptVideoInInfo->atVideoCapOutVFB[ptVideoInInfo->adwVideoCapIndex[BUFF_SIZE-1-i]].pbyBufY;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].pbyCbBuff = ptVideoInInfo->atVideoCapOutVFB[ptVideoInInfo->adwVideoCapIndex[BUFF_SIZE-1-i]].pbyBufU;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].pbyCrBuff = ptVideoInInfo->atVideoCapOutVFB[ptVideoInInfo->adwVideoCapIndex[BUFF_SIZE-1-i]].pbyBufV;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].tFrameTimeStamp.dwSecond = ptVideoInInfo->atVideoCapOutVFB[ptVideoInInfo->adwVideoCapIndex[BUFF_SIZE-1-i]].dwSecond;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].tFrameTimeStamp.dwMilliSecond = ptVideoInInfo->atVideoCapOutVFB[ptVideoInInfo->adwVideoCapIndex[BUFF_SIZE-1-i]].dwMilliSecond;
				ptVideoInInfo->atImgBackEndProcessBuffInfo[i].tFrameTimeStamp.dwMicroSecond = ptVideoInInfo->atVideoCapOutVFB[ptVideoInInfo->adwVideoCapIndex[BUFF_SIZE-1-i]].dwMicroSecond;
			}
		}
	}

	memcpy(&ptIBPState->tCurrInBuff, &ptVideoInInfo->atImgBackEndProcessBuffInfo[0], sizeof(TImgBackEndProcessBuffInfo));
	ptIBPState->ptRefInBuffQueue = &(ptVideoInInfo->atImgBackEndProcessBuffInfo[1]);
	ptIBPState->dwValidRefBuffNum = (BUFF_SIZE - 1);
#ifdef _PRE_NR_	
	memcpy(&ptSecondPhaseIBPState->tCurrInBuff, &ptVideoInInfo->atImgBackEndProcessBuffInfo[0], sizeof(TImgBackEndProcessBuffInfo));
	ptSecondPhaseIBPState->ptRefInBuffQueue = &(ptVideoInInfo->atImgBackEndProcessBuffInfo[1]);
	ptSecondPhaseIBPState->dwValidRefBuffNum = (BUFF_SIZE - 1);
#endif	
	
#if 0	
	for (i = 0; i < BUFF_SIZE; i++)
	{
			printf("(%d) (phase, sec, msec, usec)=(%d, %d, %d, %d)\n", i, dwPhase, 
														ptVideoInInfo->atImgBackEndProcessBuffInfo[i].tFrameTimeStamp.dwSecond, 
														ptVideoInInfo->atImgBackEndProcessBuffInfo[i].tFrameTimeStamp.dwMilliSecond,
														ptVideoInInfo->atImgBackEndProcessBuffInfo[i].tFrameTimeStamp.dwMicroSecond
  			);
	}
	printf("=========================================\n");
#endif
	/*if ((scRet = ImgBackEndProcess_CheckParam(ptVideoInInfo->hIBPObj, ptIBPState)) != S_OK) 
	{
		printf("[VENC_VIDEOIN] IBPE state parameters setting error !!\n");
		printf("Error code = 0x%08X\n", scRet);
		return S_FAIL;
	}*/
#ifdef _PRE_NR_	
	/*if ((scRet = ImgBackEndProcess_CheckParam(ptVideoInInfo->hSencodPhaseIBPObj, ptSecondPhaseIBPState)) != S_OK) 
	{
		printf("[VENC_VIDEOIN] IBPE state parameters setting error !!\n");
		printf("Error code = 0x%08X\n", scRet);
		return S_FAIL;
	}*/
#endif
	return S_OK;
}

/* ========================================================================= */
void VencVideoIn_IBPEONLY_Handler(TVideoInInfo *ptVideoInInfo)
{
	TVideoFrameBuffer *ptIBPEInVFB; 
	TVideoFrameBuffer *ptIBPEOutVFB;

	TSharedBuffMgrState tSrdBufMgrState;
	TImgBackEndProcessState *ptIBPState = &(ptVideoInInfo->tIBPState);

	// assign IBPE input buffer
	ptIBPEInVFB = &ptVideoInInfo->atVideoCapOutVFB[ptVideoInInfo->adwVideoCapIndex[0]];

	if (VencVideoIn_RequestOutputBuffer(ptVideoInInfo, &tSrdBufMgrState) == S_OK) 
	{
		BYTE *pbyDstYAddr;
		BYTE *pbyDstUAddr;
		BYTE *pbyDstVAddr;
		DWORD dwSrcHeight = g_avfInfo[ptIBPEInVFB->vFormat].wEncHeight;
		DWORD dwSrcStride = ptIBPEInVFB->dwStride;
		DWORD dwDstHeight = (dwSrcHeight + 15) & ~0x0f;
		DWORD dwDstStride = dwSrcStride;

		ptIBPEOutVFB = (TVideoFrameBuffer *)tSrdBufMgrState.pbyHdrAddr;
		pbyDstYAddr = tSrdBufMgrState.pbyDataAddr;
		pbyDstUAddr = pbyDstYAddr+(dwDstStride*dwDstHeight);
		pbyDstVAddr = pbyDstUAddr+(dwDstStride*dwDstHeight >> 2);

		ptIBPEOutVFB->pbyBufY = pbyDstYAddr;
		ptIBPEOutVFB->pbyBufU = pbyDstUAddr;
		ptIBPEOutVFB->pbyBufV = pbyDstVAddr;
		
		VencVideoIn_ImgBackEndProcessSetting(ptVideoInInfo, ptIBPEOutVFB, 0, TRUE);
		if (ptIBPEInVFB->eBufStatus == evsobsFinished) 
		{

			ptIBPState->dwInBusNum = ptVideoInInfo->_tInitNeedInfo.dwVICOutputDRAMLoc+1;
			ptIBPState->dwOutBusNum = ptVideoInInfo->_tInitNeedInfo.dwDMACOutputDRAMLoc+1;
				
#ifdef _YUV422_
			ptIBPState->eInPixelFormat = YUV422 ;
			ptIBPState->eOutPixelFormat = YUV420 ;
#else
			ptIBPState->eInPixelFormat = YUV420;
			ptIBPState->eOutPixelFormat = YUV420;
#endif
			
#ifdef _PRE_NR_
			if (ptVideoInInfo->bSencodPhaseInitNrStrongBlendBuff == FALSE)
			{
				if (ptIBPState->bNoiseReductionEn == TRUE)
				{
					if ((ptIBPState->eNoiseReductionMode == IMGBACKENDPROCESS_NR_STRONG) || (ptIBPState->eNoiseReductionMode == IMGBACKENDPROCESS_NR_BLEND))
					{
						printf("[venc_videoin_handler](%d) Because does not allocate buffer for doing strong/blend de-noise, disable de-noise!!\n",__LINE__);
						ptIBPState->bNoiseReductionEn = FALSE;
					}
				}
				
			}
			ImgBackEndProcess_OneFrame(ptVideoInInfo->hSencodPhaseIBPObj, ptIBPState);
#else
			if (ptVideoInInfo->bInitNrStrongBlendBuff == FALSE)
			{
				if (ptIBPState->bNoiseReductionEn == TRUE)
				{
					if ((ptIBPState->eNoiseReductionMode == IMGBACKENDPROCESS_NR_STRONG) || (ptIBPState->eNoiseReductionMode == IMGBACKENDPROCESS_NR_BLEND))
					{
						printf("[venc_videoin_handler](%d) Because does not allocate buffer for doing strong/blend de-noise, disable de-noise!!\n",__LINE__);
						ptIBPState->bNoiseReductionEn = FALSE;
					}
				}
				
			}
			ImgBackEndProcess_OneFrame(ptVideoInInfo->hIBPObj, ptIBPState);
#endif			
			VencVideoIn_FillHdrData(ptIBPEInVFB, ptIBPEOutVFB);
		
			ptIBPEOutVFB->dwSecond = ptIBPState->tOutBuff.tFrameTimeStamp.dwSecond;
			ptIBPEOutVFB->dwMilliSecond = ptIBPState->tOutBuff.tFrameTimeStamp.dwMilliSecond;
			ptIBPEOutVFB->dwMicroSecond = ptIBPState->tOutBuff.tFrameTimeStamp.dwMicroSecond;
			
			if ((ptIBPState->bMotionDetectEn == TRUE) && (ptIBPState->tOutBuff.bIsMotionDetectResultValid == TRUE) /*(ptVideoInInfo->tIBPState.tCurrInBuff.bIsMotionDetectResultValid == TRUE)*/)
			{
				ptIBPEOutVFB->dwMDSecond = ptIBPState->tOutBuff.tMotionDetectTimeStamp.dwSecond;
				ptIBPEOutVFB->dwMDMilliSecond = ptIBPState->tOutBuff.tMotionDetectTimeStamp.dwMilliSecond;
				ptIBPEOutVFB->dwMDMicroSecond = ptIBPState->tOutBuff.tMotionDetectTimeStamp.dwMicroSecond;
				
				VencVideoIn_FillMtnData(ptVideoInInfo, &ptVideoInInfo->tMotionResPack);
				VencVideoIn_FillUserData(ptVideoInInfo, tSrdBufMgrState.pbyHdrAddr, &ptVideoInInfo->tMotionResPack);
			}
			else
			{	
				ptIBPEOutVFB->dwMDSecond = ptIBPState->tOutBuff.tFrameTimeStamp.dwSecond;
				ptIBPEOutVFB->dwMDMilliSecond = ptIBPState->tOutBuff.tFrameTimeStamp.dwMilliSecond;
				ptIBPEOutVFB->dwMDMicroSecond = ptIBPState->tOutBuff.tFrameTimeStamp.dwMicroSecond;
				
				VencVideoIn_FillUserData(ptVideoInInfo, tSrdBufMgrState.pbyHdrAddr, NULL);
			}

			//VencVideoIn_RotateIBPEBuffInfoQueue(ptVideoInInfo);

			if (ptVideoInInfo->bDumpSharedBuf == TRUE)	// dump file here.
			{
				FILE *pfDumpYUV;
				CHAR szTemp[64];
				DWORD dwDumpSize;
				DWORD dwSrcHeight = g_avfInfo[ptIBPEInVFB->vFormat].wEncHeight;
				DWORD dwSrcStride = ptIBPEInVFB->dwStride;
				DWORD dwDstHeight = (((dwSrcHeight + 15) >> 4) << 4);
				DWORD dwDstStride = dwSrcStride;

				ptVideoInInfo->bDumpSharedBuf = FALSE;
				sprintf(szTemp, "/tmp/SharedBufOut%ux%u_%dx%d_420.yuv", ptIBPEOutVFB->dwSecond, ptIBPEOutVFB->dwMilliSecond, dwDstStride, dwDstHeight);
				if ((pfDumpYUV = fopen(szTemp, "wb")) == NULL) 
				{
					printf("[VENC_VIDEOIN] open %s fail..... !!!\n", szTemp);
				} 
				else 
				{
					dwDumpSize = dwDstStride * dwDstHeight;
					fwrite(ptIBPEOutVFB->pbyBufY, 1, dwDumpSize, pfDumpYUV);
					fwrite(ptIBPEOutVFB->pbyBufU, 1, dwDumpSize >> 2, pfDumpYUV);
					fwrite(ptIBPEOutVFB->pbyBufV, 1, dwDumpSize >> 2, pfDumpYUV);				
					fclose(pfDumpYUV);
				}
			} //end of dump file
			CurWrittingBufIDX = tSrdBufMgrState.dwIndex;
			SharedBuffMgr_ReleaseBuffer(ptVideoInInfo->hSharedBuferObj, &tSrdBufMgrState);
		}
	}
}
#ifdef _PRE_NR_

static DWORD GetMemory(HANDLE hMemObject, DWORD dwBusNum, DWORD dwSize, EAlignType eAlignType)
{
	TMemMgrState tMemMgrState;
	tMemMgrState.dwBusNum = dwBusNum;
	tMemMgrState.dwSize = dwSize;
	tMemMgrState.eAlignType = eAlignType;

	return (MemMgr_GetMemory(hMemObject, &tMemMgrState));
}
static void FirstPhaseProcess(TVideoFrameBuffer *ptIBPEOutVFB, TVideoInInfo *ptVideoInInfo)
{	
	TVideoFrameBuffer *ptIBPEInVFB; 
	TImgBackEndProcessState *ptIBPState = &(ptVideoInInfo->tIBPState);
	
	ptIBPState->dwInBusNum = ptVideoInInfo->_tInitNeedInfo.dwVICOutputDRAMLoc+1;
	ptIBPState->dwOutBusNum = ptVideoInInfo->_tInitNeedInfo.dwDMACOutputDRAMLoc+1;

#ifdef _YUV422_
	ptIBPState->eInPixelFormat = YUV422 ;
	ptIBPState->eOutPixelFormat = YUV420 ;
#else
	ptIBPState->eInPixelFormat = YUV420;
	ptIBPState->eOutPixelFormat = YUV420;
#endif		
	// assign IBPE input buffer
	ptIBPEInVFB = &ptVideoInInfo->atVideoCapOutVFB[ptVideoInInfo->adwVideoCapIndex[0]];
	VencVideoIn_ImgBackEndProcessSetting(ptVideoInInfo, ptIBPEOutVFB, 1, TRUE);
	if (ptIBPEInVFB->eBufStatus == evsobsFinished) 
	{
		//printf("EE first:%d\n",ptVideoInInfo->tIBPState.bEdgeEnhanceEn);
		//printf("NR first:%d\n",ptVideoInInfo->tIBPState.bNoiseReductionEn);
		if (ptVideoInInfo->bInitNrStrongBlendBuff == FALSE)
		{
			if (ptIBPState->bNoiseReductionEn == TRUE)
			{
				if ((ptIBPState->eNoiseReductionMode == IMGBACKENDPROCESS_NR_STRONG) || (ptIBPState->eNoiseReductionMode == IMGBACKENDPROCESS_NR_BLEND))
				{
					printf("[venc_videoin_handler](%d) Because does not allocate buffer for doing strong/blend de-noise, disable de-noise!!\n",__LINE__);
					ptIBPState->bNoiseReductionEn = FALSE;
				}
			}
				
		}
		ImgBackEndProcess_StartOneFrame(ptVideoInInfo->hIBPObj, ptIBPState);
		ImgBackEndProcess_WaitOneFrameComplete(ptVideoInInfo->hIBPObj, ptIBPState);
		VencVideoIn_FillHdrData(ptIBPEInVFB, ptIBPEOutVFB);
		ptIBPEOutVFB->dwSecond = ptIBPState->tOutBuff.tFrameTimeStamp.dwSecond;
		ptIBPEOutVFB->dwMilliSecond = ptIBPState->tOutBuff.tFrameTimeStamp.dwMilliSecond;
		ptIBPEOutVFB->dwMicroSecond = ptIBPState->tOutBuff.tFrameTimeStamp.dwMicroSecond;
		//VencVideoIn_RotateIBPEBuffInfoQueue(ptVideoInInfo);
	}
}
static void SecondPhaseProcess(TVideoInInfo *ptVideoInInfo)
{
	TVideoFrameBuffer *ptIBPEInVFB; 
	TVideoFrameBuffer *ptIBPEOutVFB;
	static BOOL bFirstRun = TRUE;
	TSharedBuffMgrState tSrdBufMgrState;
//	TImgBackEndProcessState *ptIBPState = &(ptVideoInInfo->tIBPState);

	// assign IBPE input buffer
	ptIBPEInVFB = &ptVideoInInfo->atIBPEInVFB[0];

	if (VencVideoIn_RequestOutputBuffer(ptVideoInInfo, &tSrdBufMgrState) == S_OK) 
	{
		BYTE *pbyDstYAddr;
		BYTE *pbyDstUAddr;
		BYTE *pbyDstVAddr;
		DWORD dwSrcHeight = g_avfInfo[ptIBPEInVFB->vFormat].wEncHeight;
		DWORD dwSrcStride = ptIBPEInVFB->dwStride;
		DWORD dwDstHeight = (dwSrcHeight + 15) & ~0x0f;
		DWORD dwDstStride = dwSrcStride;

		ptIBPEOutVFB = (TVideoFrameBuffer *)tSrdBufMgrState.pbyHdrAddr;
		pbyDstYAddr = tSrdBufMgrState.pbyDataAddr;
		pbyDstUAddr = pbyDstYAddr+(dwDstStride*dwDstHeight);
		pbyDstVAddr = pbyDstUAddr+(dwDstStride*dwDstHeight >> 2);

		ptIBPEOutVFB->pbyBufY = pbyDstYAddr;
		ptIBPEOutVFB->pbyBufU = pbyDstUAddr;
		ptIBPEOutVFB->pbyBufV = pbyDstVAddr;
		
		VencVideoIn_ImgBackEndProcessSetting(ptVideoInInfo, ptIBPEOutVFB, 2, FALSE);
		if (ptIBPEInVFB->eBufStatus == evsobsFinished) 
		{	
			TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
			//TImgBackEndProcessState tIBPState;
			//memcpy(&tIBPState, ptIBPState, sizeof(TImgBackEndProcessState));
		
			if(ptVideoInInfo->bSecondPhaseIBPEParameterUpdated == TRUE)
			{
				//printf("[venc_videoin_handler](%d)ptVideoInInfo->bSecondPhaseIBPEParameterUpdated==%d\n",__LINE__,ptVideoInInfo->bSecondPhaseIBPEParameterUpdated);
				ptVideoInInfo->tSecondPhaseIBPState.bUpdateEn = TRUE;
			}
			
			ptVideoInInfo->tSecondPhaseIBPState.eInPixelFormat = YUV420 ;
			ptVideoInInfo->tSecondPhaseIBPState.eOutPixelFormat = YUV420 ;
			
			ptVideoInInfo->tSecondPhaseIBPState.dwInBusNum = ptVideoInInfo->_tInitNeedInfo.dwDMACOutputDRAMLoc+1;
			ptVideoInInfo->tSecondPhaseIBPState.dwOutBusNum = ptVideoInInfo->_tInitNeedInfo.dwDMACOutputDRAMLoc+1;
				
			ptVideoInInfo->tSecondPhaseIBPState.bNoiseReductionEn = ptIBPOpt->tDeNoiseCfg.bSecondPhaseDeNoiseEnable;
			ptVideoInInfo->tSecondPhaseIBPState.eNoiseReductionMode = ptIBPOpt->tDeNoiseCfg.eSecondPhaseNoiseReductionMode;
			if (ptVideoInInfo->bSencodPhaseInitNrStrongBlendBuff == FALSE)
			{
				if (ptVideoInInfo->tSecondPhaseIBPState.bNoiseReductionEn == TRUE)
				{
					if ((ptVideoInInfo->tSecondPhaseIBPState.eNoiseReductionMode == IMGBACKENDPROCESS_NR_STRONG) || (ptVideoInInfo->tSecondPhaseIBPState.eNoiseReductionMode == IMGBACKENDPROCESS_NR_BLEND))
					{
						printf("[venc_videoin_handler](%d) Because does not allocate buffer for doing strong/blend de-noise, disable de-noise!!\n",__LINE__);
						ptVideoInInfo->tSecondPhaseIBPState.bNoiseReductionEn = FALSE;
					}
				}
				
			}	
			ptVideoInInfo->tSecondPhaseIBPState.dwNoiseReductionNormalStrength =   ptIBPOpt->tDeNoiseCfg.dwSecondPhaseNoiseReductionNormalStrength;
			ptVideoInInfo->tSecondPhaseIBPState.dwNoiseReductionStrongRefStrength =  ptIBPOpt->tDeNoiseCfg. dwSecondPhaseNoiseReductionStrongRefStrength;
			ptVideoInInfo->tSecondPhaseIBPState.dwNoiseReductionStrongCurrWeight =  ptIBPOpt->tDeNoiseCfg.dwSecondPhaseNoiseReductionStrongCurrWeight;
			ptVideoInInfo->tSecondPhaseIBPState.bNoiseReductionStrongMotionAdaptiveEn = ptIBPOpt->tDeNoiseCfg.bSecondPhaseNoiseReductionStrongMotionAdaptiveEn;
			
			ptVideoInInfo->tSecondPhaseIBPState.bEdgeEnhanceEn = ptIBPOpt->tEdgeEnhancementCfg.bSecondPhaseEdgeEnhancementEnable;
			ptVideoInInfo->tSecondPhaseIBPState.dwEdgeEnhanceClip = ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceClip;
			ptVideoInInfo->tSecondPhaseIBPState.dwEdgeEnhanceEdgeSensitivity = ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceEdgeSensitivity;
			ptVideoInInfo->tSecondPhaseIBPState.dwEdgeEnhanceNoiseSensitivity = ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceNoiseSensitivity;
			ptVideoInInfo->tSecondPhaseIBPState.dwEdgeEnhanceStrength = ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceStrength;
			
			if (bFirstRun == TRUE)
			{
				printf("[venc_videoin_handler](%d)bUpdateEn =%d\n",__LINE__,ptVideoInInfo->tSecondPhaseIBPState.bUpdateEn);
				printf("[venc_videoin_handler](%d)bNoiseReductionEn =%d\n",__LINE__,ptVideoInInfo->tSecondPhaseIBPState.bNoiseReductionEn);
				printf("[venc_videoin_handler](%d)eNoiseReductionMode =%d\n",__LINE__,ptVideoInInfo->tSecondPhaseIBPState.eNoiseReductionMode);
				bFirstRun = FALSE;
			}
			
			//printf("EE second:%d\n",ptVideoInInfo->tSecondPhaseIBPState.bEdgeEnhanceEn);
			//printf("NR second:%d\n",ptVideoInInfo->tSecondPhaseIBPState.bNoiseReductionEn);
			ImgBackEndProcess_StartOneFrame(ptVideoInInfo->hSencodPhaseIBPObj,&ptVideoInInfo->tSecondPhaseIBPState);
			ImgBackEndProcess_WaitOneFrameComplete(ptVideoInInfo->hSencodPhaseIBPObj, &ptVideoInInfo->tSecondPhaseIBPState);
						
			if (ptVideoInInfo->bSecondPhaseIBPEParameterUpdated == TRUE)
			{
					ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = FALSE;
					//printf("[venc_videoin_handler](%d)ptVideoInInfo->bSecondPhaseIBPEParameterUpdated==%d\n",__LINE__,ptVideoInInfo->bSecondPhaseIBPEParameterUpdated);
			}

			
			VencVideoIn_FillHdrData(ptIBPEInVFB, ptIBPEOutVFB);
		
			ptIBPEOutVFB->dwSecond = ptVideoInInfo->tSecondPhaseIBPState.tOutBuff.tFrameTimeStamp.dwSecond;
			ptIBPEOutVFB->dwMilliSecond = ptVideoInInfo->tSecondPhaseIBPState.tOutBuff.tFrameTimeStamp.dwMilliSecond;
			ptIBPEOutVFB->dwMicroSecond = ptVideoInInfo->tSecondPhaseIBPState.tOutBuff.tFrameTimeStamp.dwMicroSecond;
			
			if ((ptVideoInInfo->tSecondPhaseIBPState.bMotionDetectEn == TRUE) && (ptVideoInInfo->tSecondPhaseIBPState.tOutBuff.bIsMotionDetectResultValid == TRUE) /*(ptVideoInInfo->tIBPState.tCurrInBuff.bIsMotionDetectResultValid == TRUE)*/)
			{
				ptIBPEOutVFB->dwMDSecond = ptVideoInInfo->tSecondPhaseIBPState.tOutBuff.tMotionDetectTimeStamp.dwSecond;
				ptIBPEOutVFB->dwMDMilliSecond = ptVideoInInfo->tSecondPhaseIBPState.tOutBuff.tMotionDetectTimeStamp.dwMilliSecond;
				ptIBPEOutVFB->dwMDMicroSecond = ptVideoInInfo->tSecondPhaseIBPState.tOutBuff.tMotionDetectTimeStamp.dwMicroSecond;
				
				VencVideoIn_FillMtnData(ptVideoInInfo, &ptVideoInInfo->tMotionResPack);
				VencVideoIn_FillUserData(ptVideoInInfo, tSrdBufMgrState.pbyHdrAddr, &ptVideoInInfo->tMotionResPack);
			}
			else
			{	
				ptIBPEOutVFB->dwMDSecond = ptVideoInInfo->tSecondPhaseIBPState.tOutBuff.tFrameTimeStamp.dwSecond;
				ptIBPEOutVFB->dwMDMilliSecond = ptVideoInInfo->tSecondPhaseIBPState.tOutBuff.tFrameTimeStamp.dwMilliSecond;
				ptIBPEOutVFB->dwMDMicroSecond = ptVideoInInfo->tSecondPhaseIBPState.tOutBuff.tFrameTimeStamp.dwMicroSecond;
				
				VencVideoIn_FillUserData(ptVideoInInfo, tSrdBufMgrState.pbyHdrAddr, NULL);
			}
			//VencVideoIn_RotateIBPEBuffInfoQueue(ptVideoInInfo);
			if (ptVideoInInfo->bDumpSharedBuf == TRUE)	// dump file here.
			{
				FILE *pfDumpYUV;
				CHAR szTemp[64];
				DWORD dwDumpSize;
				DWORD dwSrcHeight = g_avfInfo[ptIBPEInVFB->vFormat].wEncHeight;
				DWORD dwSrcStride = ptIBPEInVFB->dwStride;
				DWORD dwDstHeight = (((dwSrcHeight + 15) >> 4) << 4);
				DWORD dwDstStride = dwSrcStride;

				ptVideoInInfo->bDumpSharedBuf = FALSE;
				sprintf(szTemp, "/tmp/SharedBufOut%ux%u_%dx%d_420.yuv", ptIBPEOutVFB->dwSecond, ptIBPEOutVFB->dwMilliSecond, dwDstStride, dwDstHeight);
				if ((pfDumpYUV = fopen(szTemp, "wb")) == NULL) 
				{
					printf("[VENC_VIDEOIN] open %s fail..... !!!\n", szTemp);
				} 
				else 
				{
					dwDumpSize = dwDstStride * dwDstHeight;
					fwrite(ptIBPEOutVFB->pbyBufY, 1, dwDumpSize, pfDumpYUV);
					fwrite(ptIBPEOutVFB->pbyBufU, 1, dwDumpSize >> 2, pfDumpYUV);
					fwrite(ptIBPEOutVFB->pbyBufV, 1, dwDumpSize >> 2, pfDumpYUV);				
					fclose(pfDumpYUV);
				}
			} //end of dump file
			CurWrittingBufIDX = tSrdBufMgrState.dwIndex;
			SharedBuffMgr_ReleaseBuffer(ptVideoInInfo->hSharedBuferObj, &tSrdBufMgrState);
		}
	}
}
static void FrameBufferArrayShift(TVideoFrameBuffer a[], size_t nmemb, int dst)
{
	int left = dst < 0;

	dst = abs(dst) % nmemb;

	if(dst)
	{
		TVideoFrameBuffer b[dst];

		if(left)
		{
			bcopy(a, b, dst * sizeof(TVideoFrameBuffer));
			bcopy(&a[dst], a, (nmemb - dst) * sizeof(TVideoFrameBuffer));
			bcopy(b, &a[nmemb - dst], dst * sizeof(TVideoFrameBuffer));
		}
		else
		{
			bcopy(&a[nmemb - dst], b, dst * sizeof(TVideoFrameBuffer));
			bcopy(a, &a[dst], (nmemb - dst) * sizeof(TVideoFrameBuffer));
			bcopy(b, a, dst * sizeof(TVideoFrameBuffer));
		}
	}
}
void VencVideoIn_TWICEIBPE_Handler(TVideoInInfo *ptVideoInInfo)
{
	static BOOL bFirstRun = TRUE;
	
	int i = 0;
	static DWORD dwOutIndex = 0;
	
	if (bFirstRun == TRUE)
	{	
		DWORD dwWidthPadding = (((ptVideoInInfo->_tInitNeedInfo.dwVideoMaxWidth + 15) >> 4) << 4);
		DWORD dwHeightPadding = (((ptVideoInInfo->_tInitNeedInfo.dwVideoMaxHeight + 15) >> 4) << 4);
		DWORD dwYFrameSize = dwWidthPadding * dwHeightPadding;
		DWORD dwCFrameSize = (dwYFrameSize >> 2);
		DWORD dwFrameSize = dwYFrameSize + (dwCFrameSize << 1);
		
		for (i = 0; i < BUFF_SIZE; i++) 
		{
			memset(&ptVideoInInfo->atIBPEInVFB[i], 0x0, sizeof(TVideoFrameBuffer));
			memset(&ptVideoInInfo->atIBPEOutVFB[i], 0x0, sizeof(TVideoFrameBuffer));
			ptVideoInInfo->atIBPEOutVFB[i].pbyBufY = (BYTE *)GetMemory(ptVideoInInfo->hMemMgrObj, ptVideoInInfo->_tInitNeedInfo.dwDMACOutputDRAMLoc, dwFrameSize, ALIGN_TYPE_128_BYTE);
			ptVideoInInfo->atIBPEOutVFB[i].pbyBufU = ptVideoInInfo->atIBPEOutVFB[i].pbyBufY + dwYFrameSize;
			ptVideoInInfo->atIBPEOutVFB[i].pbyBufV = ptVideoInInfo->atIBPEOutVFB[i].pbyBufY + dwYFrameSize + dwCFrameSize;	
			//printf("Allocated (%d)(Y,U,V)=(%p,%p,%p)\n", i, ptVideoInInfo->atIBPEOutVFB[i].pbyBufY, ptVideoInInfo->atIBPEOutVFB[i].pbyBufU, ptVideoInInfo->atIBPEOutVFB[i].pbyBufV);
		}
		bFirstRun = FALSE;
	}
	
	FirstPhaseProcess(&ptVideoInInfo->atIBPEOutVFB[dwOutIndex], ptVideoInInfo);
	
	memcpy(&ptVideoInInfo->atIBPEInVFB[BUFF_SIZE-1],&ptVideoInInfo->atIBPEOutVFB[dwOutIndex], sizeof(TVideoFrameBuffer));
#if 0
	for (i = 0; i < BUFF_SIZE; i++) 
	{	
		printf("(%d)(Y,U,V)=(%p,%p,%p)\n", i, ptVideoInInfo->atIBPEInVFB[i].pbyBufY, ptVideoInInfo->atIBPEInVFB[i].pbyBufU, ptVideoInInfo->atIBPEInVFB[i].pbyBufV);	
	}
	printf("===========================\n");
#endif	
	SecondPhaseProcess(ptVideoInInfo);
	
	FrameBufferArrayShift(ptVideoInInfo->atIBPEInVFB, sizeof(ptVideoInInfo->atIBPEInVFB)/sizeof(TVideoFrameBuffer), -1);
	
	dwOutIndex++;
	dwOutIndex = (dwOutIndex % BUFF_SIZE);
	return;
}
#endif
#endif

#ifdef _YUV422_
void yuv422yuv420(unsigned char *YBufferIn,
				  unsigned char *UBufferIn, 
				  unsigned char *VBufferIn,
				  unsigned char *YBufferOut, 
				  int width, int height)
{

	DWORD dwYFrameSize = height*width;
	DWORD dwCFrameSize = (dwYFrameSize >> 2);
	
	int line_in =0;
	int line_out =0;
	int x =0;
	unsigned char *UBufferOut =YBufferOut + dwYFrameSize; 
	unsigned char *VBufferOut = YBufferOut + dwYFrameSize+dwCFrameSize;
	int width2 = width/2;
    int height2 = height/2;
	
	{
		// top line
        for (x=0; x<width2; ++x ){
            UBufferOut[x] = (UBufferIn[x]+3*UBufferIn[x]+3*UBufferIn[width2+x]+
                             UBufferIn[2*width2+x]+4)>>3;
            VBufferOut[x] = (VBufferIn[x]+3*VBufferIn[x]+3*VBufferIn[width2+x]+
                             VBufferIn[2*width2+x]+4)>>3;
        }
        // middle lines        
        for (line_out=1,line_in=2; line_out<height2-1; line_out++,line_in+=2) {
            for (x=0; x<width2; ++x ){
                UBufferOut[line_out*width2+x] = (UBufferIn[(line_in-1)*width2+x]+
                                                 3*UBufferIn[line_in*width2+x]+
                                                 3*UBufferIn[(line_in+1)*width2+x]+
                                                 UBufferIn[(line_in+2)*width2+x]+4)>>3;
                VBufferOut[line_out*width2+x] = (VBufferIn[(line_in-1)*width2+x]+
                                                 3*VBufferIn[line_in*width2+x]+
                                                 3*VBufferIn[(line_in+1)*width2+x]+
                                                 VBufferIn[(line_in+2)*width2+x]+4)>>3;
            }
        }
 
        // bottom line 
        for (x=0; x<width2; ++x ){
            UBufferOut[(height2-1)*width2+x] = (UBufferIn[(height-3)*width2+x]+
                                                 3*UBufferIn[(height-2)*width2+x]+
                                                 3*UBufferIn[(height-1)*width2+x]+
                                                 UBufferIn[(height-1)*width2+x]+4)>>3;

            VBufferOut[(height2-1)*width2+x] = (VBufferIn[(height-3)*width2+x]+
                                                 3*VBufferIn[(height-2)*width2+x]+
                                                 3*VBufferIn[(height-1)*width2+x]+
                                                 VBufferIn[(height-1)*width2+x]+4)>>3;
        }
	}
}
#endif
/* ============================================================================================= */
void VencVideoIn_NOPREPROCESS_Handler(TVideoInInfo *ptVideoInInfo)
{
	TVideoFrameBuffer *ptCurrVFB;
	TSharedBuffMgrState tSrdBufMgrState;

	DWORD dwSrcWidth;
	DWORD dwSrcHeight;
	DWORD dwSrcStride;

	DWORD dwDstWidth;
	DWORD dwDstHeight;
	DWORD dwDstStride;

	BYTE *pbyDstYAddr;
	BYTE *pbyDstUAddr;
	BYTE *pbyDstVAddr;

	DWORD dwSrcBusNum = 0;
	DWORD dwDestBusNum = 0;
	
	TVideoFrameBuffer *ptTempVFB;
#ifdef _YUV422_
	DWORD dwYFrameSize = ptVideoInInfo->_tInitNeedInfo.dwVideoMaxWidth * ptVideoInInfo->_tInitNeedInfo.dwVideoMaxHeight;
	DWORD dwCFrameSize = (dwYFrameSize >> 2);
#endif
	
#ifdef _1DRAM_VERSION_	
	dwSrcBusNum = 1;
	dwDestBusNum = 1;
#else
	dwSrcBusNum = 1;
	dwDestBusNum = 2;
#endif
	ptCurrVFB = &ptVideoInInfo->atVideoCapOutVFB[ptVideoInInfo->adwVideoCapIndex[0]];
	dwSrcWidth = g_avfInfo[ptCurrVFB->vFormat].wEncWidth;
	dwSrcHeight = g_avfInfo[ptCurrVFB->vFormat].wEncHeight;
	dwSrcStride = ptCurrVFB->dwStride;

	dwDstWidth = (((dwSrcWidth + 15) >> 4) << 4);
	dwDstHeight = (((dwSrcHeight + 15) >> 4) << 4);
	dwDstStride = dwSrcStride;
	
	if (VencVideoIn_RequestOutputBuffer(ptVideoInInfo, &tSrdBufMgrState) == S_OK) 
	{
		pbyDstYAddr = tSrdBufMgrState.pbyDataAddr;
		pbyDstUAddr = pbyDstYAddr+(dwDstStride*dwDstHeight);
		pbyDstVAddr = pbyDstUAddr+(dwDstStride*dwDstHeight >> 2);	
#ifdef _YUV422_
		yuv422yuv420( ptCurrVFB->pbyBufY,
					 ptCurrVFB->pbyBufU, 
					 ptCurrVFB->pbyBufV ,
					 ptVideoInInfo->pbyYUV420FrameBuff, ptVideoInInfo->_tInitNeedInfo.dwVideoMaxWidth, ptVideoInInfo->_tInitNeedInfo.dwVideoMaxHeight);
		
		VencVideoIn_DMACHandler(ptVideoInInfo, ptCurrVFB->pbyBufY, pbyDstYAddr, dwSrcWidth, dwSrcHeight, dwSrcStride, dwDstWidth, dwDstHeight, dwDstStride, dwSrcBusNum, dwDestBusNum);
		VencVideoIn_DMACHandler(ptVideoInInfo, ptVideoInInfo->pbyYUV420FrameBuff+dwYFrameSize, pbyDstUAddr, (dwSrcWidth >> 1), (dwSrcHeight >> 1), (dwSrcStride >> 1), (dwDstWidth >> 1), (dwDstHeight >> 1), (dwDstStride >> 1), dwSrcBusNum, dwDestBusNum);
		VencVideoIn_DMACHandler(ptVideoInInfo, ptVideoInInfo->pbyYUV420FrameBuff+dwYFrameSize+dwCFrameSize, pbyDstVAddr, (dwSrcWidth >> 1), (dwSrcHeight >> 1), (dwSrcStride >> 1), (dwDstWidth >> 1), (dwDstHeight >> 1), (dwDstStride >> 1), dwSrcBusNum, dwDestBusNum);
		
#else
		VencVideoIn_DMACHandler(ptVideoInInfo, ptCurrVFB->pbyBufY, pbyDstYAddr, dwSrcWidth, dwSrcHeight, dwSrcStride, dwDstWidth, dwDstHeight, dwDstStride, dwSrcBusNum, dwDestBusNum);
		VencVideoIn_DMACHandler(ptVideoInInfo, ptCurrVFB->pbyBufU, pbyDstUAddr, (dwSrcWidth >> 1), (dwSrcHeight >> 1), (dwSrcStride >> 1), (dwDstWidth >> 1), (dwDstHeight >> 1), (dwDstStride >> 1), dwSrcBusNum, dwDestBusNum);
		VencVideoIn_DMACHandler(ptVideoInInfo, ptCurrVFB->pbyBufV, pbyDstVAddr, (dwSrcWidth >> 1), (dwSrcHeight >> 1), (dwSrcStride >> 1), (dwDstWidth >> 1), (dwDstHeight >> 1), (dwDstStride >> 1), dwSrcBusNum, dwDestBusNum);
#endif
		ptTempVFB = (TVideoFrameBuffer *)tSrdBufMgrState.pbyHdrAddr;
		ptTempVFB->pbyBufY = pbyDstYAddr;
		ptTempVFB->pbyBufU = pbyDstUAddr;
		ptTempVFB->pbyBufV = pbyDstVAddr;
	
		if (ptVideoInInfo->bDumpSharedBuf == TRUE)
		{
			FILE *pfDumpYUV;
			CHAR szTemp[64];
			DWORD dwDumpSize;
			
			ptVideoInInfo->bDumpSharedBuf = FALSE;
			sprintf(szTemp, "/tmp/SharedBufOut%ux%u_%dx%d_420.yuv", ptTempVFB->dwSecond, ptTempVFB->dwMilliSecond, dwDstStride, dwDstHeight);
			if ((pfDumpYUV = fopen(szTemp, "wb")) == NULL) 
			{
				printf("[VENC_VIDEOIN] open %s fail..... !!!\n", szTemp);
			} 
			else 
			{
				dwDumpSize = dwDstStride * dwDstHeight;
				fwrite(ptTempVFB->pbyBufY, 1, dwDumpSize, pfDumpYUV);
				fwrite(ptTempVFB->pbyBufU, 1, dwDumpSize >> 2, pfDumpYUV);
				fwrite(ptTempVFB->pbyBufV, 1, dwDumpSize >> 2, pfDumpYUV);
				fclose(pfDumpYUV);
			}
		}
		VencVideoIn_FillHdrData(ptCurrVFB, ptTempVFB);
		VencVideoIn_FillUserData(ptVideoInInfo, tSrdBufMgrState.pbyHdrAddr, NULL);
		CurWrittingBufIDX = tSrdBufMgrState.dwIndex;
		SharedBuffMgr_ReleaseBuffer(ptVideoInInfo->hSharedBuferObj, &tSrdBufMgrState);
	}
}

