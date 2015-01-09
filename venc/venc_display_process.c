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
 * venc_display_process.c
 *
 * \brief
 * Video Display Core Processing function.
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
//#define     DISP_PRINT_FPS
//#define     DISP_PRINT_INPUT_FPS

static SCODE VencDisplay_LoadConfig(HANDLE hObject);

/* ========================================================================== */
static TVideoFrameBuffer* VencDisplay_RequestInBuf(TDisplayInfo *ptInfo, HANDLE hSrdObj, int iSrdFD, TSharedBuffMgrState *ptSrdBufMgrState)
{
	TVideoFrameBuffer   *ptVFrameBuf = NULL;
	fd_set              fdRead;
	struct timeval      timeout;
	int                 fd                  = iSrdFD;
	int                 iResult;
	SCODE               scRet               = S_FAIL;

	FD_ZERO(&fdRead);
	timeout.tv_sec  = 1;
	timeout.tv_usec = 0;

	FD_SET(fd, &fdRead);

	iResult = select(fd+1, &fdRead, NULL, NULL,NULL /*&timeout*/);
	if (iResult <= 0) {
		DISP_DP("%s %s() select() timeout\n", DISP_PRE, __FUNCTION__);
		ptVFrameBuf = NULL;
	}
	else if (FD_ISSET(fd, &fdRead)) {
		scRet = SharedBuffMgr_GetBuffer(hSrdObj, 
				ptSrdBufMgrState);
		if (scRet == S_OK) {
			ptVFrameBuf = 
				(TVideoFrameBuffer *)(ptSrdBufMgrState->pbyHdrAddr);
			//            printf("ptSrdBufMgrState->dwIndex = %d\n", ptSrdBufMgrState->dwIndex);
		} 
		else ptVFrameBuf = NULL;
	}
	else ptVFrameBuf = NULL;
	return ptVFrameBuf;
}
/* ========================================================================== */
static SCODE VencDisplay_SetInputFrameRate(HANDLE hFRCtrlObj,DWORD dwFrameRate)
{
	TFRCtrlOptions tFRCtrlOptions;
	tFRCtrlOptions.eOptionFlags = FRCTRL_CHANGE_FRAMERATE;
	tFRCtrlOptions.adwUserData[0] = dwFrameRate;

	return FrameRateCtrl_SetOption(hFRCtrlObj, &tFRCtrlOptions);
}

/* ========================================================================== */
static void VencDisplay_CheckInputFrameRate(TDisplayInfo *ptInfo)
{
	if (ptInfo->bNeedResize == TRUE)
	{
		if (ptInfo->eDispOutFmt == vDispOutFmtPAL ) {
			//The VOC framerate of PAL is 25. But when setting 25 to framerate ctrl lib, we will got unsatble frame rate. Whe setting 24, we will get the stable framte rate . 
			VencDisplay_SetInputFrameRate(ptInfo->hFRCtrlObj,24);
		} else if ( ptInfo->eDispOutFmt == vDispOutFmtNTSC) {
			//The VOC framerate of NTSC is 30. But when setting 30 to framerate ctrl lib, we will got unsatble frame rate. Whe setting 28, we will get the stable framte rate. 
			VencDisplay_SetInputFrameRate(ptInfo->hFRCtrlObj,28);
		} else if (ptInfo->eDispOutFmt == vDispOutFmtHDMI) {
			if ((ptInfo->dwFreq == 60) && (ptInfo->dwOutputWidth == 1280) && (ptInfo->dwOutputHeight==720)) {
				VencDisplay_SetInputFrameRate(ptInfo->hFRCtrlObj,60);
			}
		} else if (ptInfo->eDispOutFmt == vDispOutFmtHDSDI) {
			if ((ptInfo->dwFreq == 60) && (ptInfo->dwOutputWidth == 1280) && (ptInfo->dwOutputHeight==720)) {
				VencDisplay_SetInputFrameRate(ptInfo->hFRCtrlObj,60);
			}
		}
	}
}
/* ========================================================================== */
static SCODE VencDisplay_InitialSharedBuffer(TDisplayInfo *ptInfo ,DWORD dwReaderNum ,BOOL bSlowReader)
{
	int i;
	SCODE scRet = S_FAIL;

	for (i = 0; i < dwReaderNum; i++) {
		if ((ptInfo->tSrdBufInfo[i].hSrdObj == NULL)) {
			//! Initial sharedbuffer instance
			scRet = VencDisplay_InitSharedBuffer(ptInfo, i,bSlowReader);
			if (scRet != S_OK) {
				fprintf(stderr, "%s Err: VencDisplay_InitSharedBuffer-0x%08x\n",DISP_PRE, scRet);
			}
			DISP_DP("%s VencDisplay_InitSharedBuffer done\n", DISP_PRE);
		} else DISP_DP("%s hSrdObj is not NULL\n", DISP_PRE);
	}

	return scRet;

}

static SCODE VencDisplay_SetInputSize(TDisplayInfo *ptInfo,DWORD dwInWidth,DWORD dwInHeight,DWORD dwStride)
{
	TVideoDispOptions   tOptions;
	SCODE scRet = S_FAIL;
	memset( &tOptions,0x0,sizeof(TVideoDispOptions) );
	tOptions.eOptionFlags = VIDEO_DISP_OPTION_SET_INPUT;
	tOptions. adwUserData[0] = dwInWidth;
	tOptions. adwUserData[1] = dwInHeight;
	tOptions. adwUserData[2] = dwStride;
	tOptions. adwUserData[3] = YUV420;
	
	scRet = VideoDisp_SetOptions(ptInfo->hVDispObj, &tOptions);
					
	if (scRet != S_OK)
	{
		printf("(%d)Call VIDEO_DISP_OPTION_SET_INPUT failed!!\n",__LINE__);
	}
	return scRet;
	
}
/* ========================================================================== */
static SCODE VencDisplay_CheckNeedResize(TDisplayInfo *ptInfo)
{
	DWORD	dwSrdBufIndex = 0;
	int                 iResult = -1;
	TVideoFrameBuffer   *ptVFrameBuf = NULL;
	fd_set              fdRead;
	int                 fd = -1;
	SCODE scRet = S_FAIL;

	FD_ZERO(&fdRead);

	ptInfo->bNeedResize = FALSE;

	VencDisplay_InitialSharedBuffer(ptInfo ,1 ,FALSE);

	fd = ptInfo->tSrdBufInfo[dwSrdBufIndex].iSrdFD;

	FD_SET(fd, &fdRead);

	iResult = select(fd+1, &fdRead, NULL, NULL, NULL);
	if (iResult <= 0) {
		DISP_DP("%s %s() select() timeout\n", DISP_PRE, __FUNCTION__);
	} else if (FD_ISSET(fd, &fdRead)) {
		scRet = SharedBuffMgr_GetBuffer(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj,&ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
		if (scRet == S_OK) {
			ptVFrameBuf = (TVideoFrameBuffer *)(ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState.pbyHdrAddr);
			if ( (g_avfInfo[ptVFrameBuf->vFormat].wEncWidth != ptInfo->dwOutputWidth) ||(g_avfInfo[ptVFrameBuf->vFormat].wEncHeight != ptInfo->dwOutputHeight) ) {	
				ptInfo->bNeedResize = TRUE;
				VencDisplay_SetInputSize(ptInfo,ptInfo->dwOutputWidth,ptInfo->dwOutputHeight,DISPLAY_OUT_BUF_WIDTH);
			} else {
				ptInfo->bNeedResize = FALSE;
				VencDisplay_SetInputSize(ptInfo,g_avfInfo[ptVFrameBuf->vFormat].wEncWidth,g_avfInfo[ptVFrameBuf->vFormat].wEncHeight,ptVFrameBuf->dwStride);
			}
			SharedBuffMgr_ReleaseBuffer(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, &ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
		} 
	}
	scRet = SharedBuffMgr_Release( &(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj) );
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: SharedBuffMgr_Release 0x%08x\n", DISP_PRE, scRet);
	}
	else
	{
		printf("(%d)SharedBuffMgr_Release handleIndex=%d\n",__LINE__,dwSrdBufIndex);
	}
	ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj = NULL;

	return scRet;

}
/* ========================================================================== */
static void VencDisplay_SyncIndex(TDisplayInfo *ptInfo)
{
	TVideoFrameBuffer   *ptVFrameBuf = NULL;
	fd_set              fdRead;
	struct timeval      timeout;
	int                 fd;
	int iResult = -1;
	SCODE               scRet               = S_FAIL;

	DWORD               dwBaseIndex = -1;
	DWORD               dwSrdBufIndex;

	printf("(%d)Enter VencDisplay_SyncIndex!\n",__LINE__); 
	{
		dwSrdBufIndex = 0;
		FD_ZERO(&fdRead);
		timeout.tv_sec  = 1;
		timeout.tv_usec = 0;

		fd = ptInfo->tSrdBufInfo[dwSrdBufIndex].iSrdFD;
		FD_SET(fd, &fdRead);

		iResult = select(fd+1, &fdRead, NULL, NULL, NULL /*&timeout*/);
		if (iResult <= 0) {
			DISP_DP("%s %s() select() timeout\n", DISP_PRE, __FUNCTION__);
		} else if (FD_ISSET(fd, &fdRead)) {
			scRet = SharedBuffMgr_GetBuffer(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, 
					&ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
			if (scRet == S_OK) {
				ptVFrameBuf = (TVideoFrameBuffer *)(ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState.pbyHdrAddr);
				SharedBuffMgr_ReleaseBuffer(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, &ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
			} 
		}
	}

	if (ptInfo->bNeedResize == FALSE) {
		{
			dwSrdBufIndex = 1;
			FD_ZERO(&fdRead);
			timeout.tv_sec  = 1;
			timeout.tv_usec = 0;

			fd = ptInfo->tSrdBufInfo[dwSrdBufIndex].iSrdFD;
			FD_SET(fd, &fdRead);

			iResult = select(fd+1, &fdRead, NULL, NULL,NULL /*&timeout*/);
			if (iResult <= 0) {
				DISP_DP("%s %s() select() timeout\n", DISP_PRE, __FUNCTION__);
			} else if (FD_ISSET(fd, &fdRead)) {
				scRet = SharedBuffMgr_GetBuffer(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, 
						&ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
				if (scRet == S_OK) {
					//                dwIndex1 = ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState.dwIndex;
					SharedBuffMgr_ReleaseBuffer(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, &ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
				} 
			}
		}
		//    printf("dwIndex1 = %d\n", dwIndex1);

		{
			dwSrdBufIndex = 2;
			FD_ZERO(&fdRead);
			timeout.tv_sec  = 1;
			timeout.tv_usec = 0;

			fd = ptInfo->tSrdBufInfo[dwSrdBufIndex].iSrdFD;
			FD_SET(fd, &fdRead);

			iResult = select(fd+1, &fdRead, NULL, NULL,NULL /*&timeout*/);
			if (iResult <= 0) {
				DISP_DP("%s %s() select() timeout\n", DISP_PRE, __FUNCTION__);
			} else if (FD_ISSET(fd, &fdRead)) {
				scRet = SharedBuffMgr_GetBuffer(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, 
						&ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
				if (scRet == S_OK) {
					dwBaseIndex = ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState.dwIndex;
					SharedBuffMgr_ReleaseBuffer(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, &ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
				} 
			}
		}
		printf("dwBaseIndex = %d\n", dwBaseIndex);

		while (1) {
			dwSrdBufIndex = 0;
			FD_ZERO(&fdRead);
			timeout.tv_sec  = 1;
			timeout.tv_usec = 0;

			fd = ptInfo->tSrdBufInfo[dwSrdBufIndex].iSrdFD;
			FD_SET(fd, &fdRead);

			iResult = select(fd+1, &fdRead, NULL, NULL, NULL/*&timeout*/);
			if (iResult <= 0) {
				DISP_DP("%s %s() select() timeout\n", DISP_PRE, __FUNCTION__);
			} else if (FD_ISSET(fd, &fdRead)) {
				scRet = SharedBuffMgr_GetBuffer(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, 
						&ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
				if (scRet == S_OK) {
					DWORD   dwTempIndex;
					dwTempIndex = ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState.dwIndex;
					SharedBuffMgr_ReleaseBuffer(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, &ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
					if (dwTempIndex == dwBaseIndex) break;
				} 
			}
		}

		while (1) {
			dwSrdBufIndex = 1;
			FD_ZERO(&fdRead);
			timeout.tv_sec  = 1;
			timeout.tv_usec = 0;

			fd = ptInfo->tSrdBufInfo[dwSrdBufIndex].iSrdFD;
			FD_SET(fd, &fdRead);

			iResult = select(fd+1, &fdRead, NULL, NULL,NULL /*&timeout*/);
			if (iResult <= 0) {
				DISP_DP("%s %s() select() timeout\n", DISP_PRE, __FUNCTION__);
			} else if (FD_ISSET(fd, &fdRead)) {
				scRet = SharedBuffMgr_GetBuffer(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, 
						&ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
				if (scRet == S_OK) {
					DWORD   dwTempIndex;
					dwTempIndex = ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState.dwIndex;
					SharedBuffMgr_ReleaseBuffer(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, &ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
					if (dwTempIndex == dwBaseIndex) break;
				} 
			}
		}

		ptInfo->tSrdBufInfo[0].iShouldGetCounter = 0;
		ptInfo->tSrdBufInfo[1].iShouldGetCounter = 2;
		ptInfo->tSrdBufInfo[2].iShouldGetCounter = 1;
	}
	printf("(%d)Exit VencDisplay_SyncIndex!\n",__LINE__); 

}

/* ========================================================================== */
SCODE VencDisplay_ReleaseInBuf(HANDLE hSrdObj, 
		TSharedBuffMgrState *ptSharedBuffMgrState)
{
	return SharedBuffMgr_ReleaseBuffer(hSrdObj, ptSharedBuffMgrState);
}

/* ========================================================================== */
#ifdef _VMA_IRE	
SCODE VencDisplay_Resize(HANDLE             hResizeObj,
		TVideoFrameBuffer  *ptVInFB, 
		TVideoFrameBuffer  *ptVOutFB, 
		DWORD              dwInW, 
		DWORD              dwOutW, 
		DWORD              dwInH, 
		DWORD              dwOutH,
		DWORD              dwInS,
		DWORD              dwOutS,
		DWORD              dwResizedW,
		DWORD              dwResizedH)
{
	TResizeState tState;
	SCODE scRet         = S_FAIL;
	DWORD dwOutYOffset  = 0;
	DWORD dwOutUOffset  = 0;
	DWORD dwUVOutH      = 0;
	DWORD dwMethod      = ((dwInW * dwInH) >= (dwOutW * dwOutH))? 0: 1;
	//! 0:large to small, 1:small to large

	memset( &tState,0x0,sizeof(TResizeState) );
	tState.dwInStride   = dwInS;
	tState.dwOutStride  = dwOutS;
	tState.dwInWidth    = dwInW;
	tState.dwInHeight   = dwInH;
	tState.dwOutWidth   = dwResizedW;
	tState.dwOutHeight  = dwResizedH;
	tState.dwImageMode  = 0;
	tState.ePixelFormat = YUV420;   //! input format;
	tState.dwCmptNum    = 3;
	tState.pbyYInFrame  = ptVInFB->pbyBufY;
	tState.pbyCbInFrame = ptVInFB->pbyBufU;
	tState.pbyCrInFrame = ptVInFB->pbyBufV;

	dwUVOutH = dwOutH >> 1;
	if (dwMethod == 0) {
		dwOutYOffset = ((dwOutH   - dwResizedH       ) >> 1) * dwOutS;
		dwOutUOffset = ((dwUVOutH - (dwResizedH >> 1)) >> 1) * (dwOutS >> 1);
	} 
	else {
		dwOutYOffset = 0;
		dwOutUOffset = 0;
	}
	//    tState.pbyYOutFrame  = ptVOutFB->pbyBufY + dwOutYOffset;
	//    tState.pbyCbOutFrame = ptVOutFB->pbyBufU + dwOutUOffset;
	//    tState.pbyCrOutFrame = ptVOutFB->pbyBufV + dwOutUOffset;
	tState.pbyYOutFrame  = ptVOutFB->pbyBufY;
	tState.pbyCbOutFrame = ptVOutFB->pbyBufU;
	tState.pbyCrOutFrame = ptVOutFB->pbyBufV;


	/*if ((scRet = Resize_CheckParam(hResizeObj, &tState)) != S_OK) {
		fprintf(stderr, "%s Err Resize_CheckParam - 0x%08x\n", 
				DISP_PRE, scRet);
		return scRet;
	}*/
	if ((scRet = Resize_ProcessOneFrame(hResizeObj, &tState)) != S_OK) {
		fprintf(stderr, "%s Resize_ProcessOneFrame - 0x%08x\n", 
				DISP_PRE, scRet);
		return scRet;
	}
	return S_OK;
}
#endif
/* ========================================================================== */
static SCODE VencDisplay_Handler(TDisplayInfo       *ptInfo, 
		TVideoFrameBuffer  *ptVInFB, 
		DWORD              dwBufIndex,
		TVideoDispState    *ptVDispState,
		DWORD dwHandleIndex)
{
	//! source in size is at ptVInFB->vFormat
	//! source out format is at ptInfo->eDispOutFmt
	//! source out addr is at ptVDispState->pbyY(Cb)(Cr)Frame

	//! if current isn't equal to previous, it should re-compute the start 
	//! address offset.

	static DWORD s_dwPrevInW    = 0;
	static DWORD s_dwPrevInH    = 0;
	static DWORD s_dwPrevOutW   = 0;
	static DWORD s_dwPrevOutH   = 0;
	//! it shouldn't re-compute except input/output resolution change, so use 
	//! static variable to record.
	static DWORD dwDstYOffset  = 0;
	static DWORD dwDstUVOffset = 0;

	static DWORD dwSrcYOffset   = 0;
	static DWORD dwSrcUVOffset  = 0;

	DWORD dwUVHeightAbs         = 0;
	DWORD dwUVWidthAbs          = 0;
	DWORD dwYStride             = DISPLAY_OUT_BUF_WIDTH;
	DWORD dwUVStride            = dwYStride >> 1;
#ifdef _VMA_IRE	
	DWORD dwInStride			= ptVInFB->dwStride;
#endif
	DWORD dwYInW                = g_avfInfo[ptVInFB->vFormat].wEncWidth;
	DWORD dwYInH                = g_avfInfo[ptVInFB->vFormat].wEncHeight;
	DWORD dwYOutW               = ptInfo->dwOutputWidth;
	DWORD dwYOutH               = ptInfo->dwOutputHeight;
	DWORD dwUVInW               = dwYInW  >> 1;
	DWORD dwUVInH               = dwYInH  >> 1;
	DWORD dwUVOutW              = dwYOutW >> 1;
	DWORD dwUVOutH              = dwYOutH >> 1;

	//! recompute the start address offset if needed, the value is at dwOffset.
	if ( (s_dwPrevInW  != dwYInW)  || (s_dwPrevInH  != dwYInH)  ||
			(s_dwPrevOutW != dwYOutW) || (s_dwPrevOutH != dwYOutH) ) {

		//! compute Y component
		DWORD dwYHeightAbs = (dwYOutH < dwYInH) ? 
			(dwYInH - dwYOutH): (dwYOutH - dwYInH);
		DWORD dwYWidthAbs  = (dwYOutW < dwYInW) ? 
			(dwYInW - dwYOutW): (dwYOutW - dwYInW);

		dwDstYOffset = ((dwYHeightAbs >> 1) * dwYStride) + (dwYWidthAbs >> 1);
		dwDstYOffset = ((dwDstYOffset + 7) >> 3) << 3;
		//! 8 byte alignment, needed?
		dwSrcYOffset  = ((dwYHeightAbs >> 1) * dwYInW) + (dwYWidthAbs >> 1);
		dwSrcYOffset  = ((dwSrcYOffset + 7) >> 3) << 3;
		//! 8 byte alignment, needed?
		//! compute UV component
		dwUVHeightAbs = 
			(dwUVOutH < dwUVInH) ? (dwUVInH - dwUVOutH): (dwUVOutH - dwUVInH);
		dwUVWidthAbs  = 
			(dwUVOutW < dwUVInW) ? (dwUVInW - dwUVOutW): (dwUVOutW - dwUVInW);
		dwDstUVOffset =
			((dwUVHeightAbs >> 1) * dwUVStride) + (dwUVWidthAbs >> 1);
		dwDstUVOffset = ((dwDstUVOffset + 7) >> 3) << 3;
		//! 8 byte alignment, needed?
		dwSrcUVOffset = ((dwUVHeightAbs >> 1) * dwUVInW) + (dwUVWidthAbs >> 1);
		dwSrcUVOffset = ((dwSrcUVOffset + 7) >> 3) << 3;
		//! 8 byte alignment, needed?
		s_dwPrevInW  = dwYInW;
		s_dwPrevInH  = dwYInH;
		s_dwPrevOutW = dwYOutW;
		s_dwPrevOutH = dwYOutH;
	}
	/*if ((dwYInW != dwYOutW) || (dwYInH != dwYOutH))*/ 
	if (ptInfo->bNeedResize == TRUE) {

		static DWORD    dwSwitch = 0;
		struct my_struct search;
		memset(&search,0x0,sizeof(struct my_struct ));
		search.bufferIndex = dwSwitch;
		search.handleIndex = VIDEO_DISP_RESIZE_BUF_HANDLE_INDEX;
		if (is_exist_elem(ptInfo->pDisplayInfoQueue, &search) == 0) {
			TVideoDispState     tVDispState;    // for inqueue used.
#ifdef _VMA_IRE	
			VencDisplay_Resize(ptInfo->hResizeObj,
					ptVInFB,
					&ptInfo->tResizeOutBuf[dwSwitch],
					dwYInW, 
					dwYOutW, 
					dwYInH, 
					dwYOutH,
					dwInStride,
					dwYStride,
					dwYOutW,
					dwYOutH);
#endif		
			tVDispState.dwIndex = dwSwitch;
			tVDispState.pbyYFrame = ptInfo->tResizeOutBuf[dwSwitch].pbyBufY;
			tVDispState.pbyCbFrame = ptInfo->tResizeOutBuf[dwSwitch].pbyBufU;
			tVDispState.pbyCrFrame = ptInfo->tResizeOutBuf[dwSwitch].pbyBufV;
			if (VideoDisp_QueueBuf(ptInfo->hVDispObj, &tVDispState) != S_OK) {
				printf("Queue buffer failed !\n");
				exit(1);
			}
			list_add_element(ptInfo->pDisplayInfoQueue, dwSwitch,VIDEO_DISP_RESIZE_BUF_HANDLE_INDEX);
			ptInfo->dwQueueElmtNum++;
			if (VideoDisp_DeQueueBuf(ptInfo->hVDispObj, ptVDispState) != S_OK) {
				printf("Release buffer failed !\n");
				exit(1);

			}
			list_remove_element(ptInfo->pDisplayInfoQueue);
			ptInfo->dwQueueElmtNum--;

		}
		dwSwitch++;
		if (dwSwitch == READER_NUM)
			dwSwitch = 0;

	} else {

		TVideoDispState     tVDispState;    // for inqueue used.
		tVDispState.dwIndex = dwBufIndex;
		tVDispState.pbyYFrame = ptVInFB->pbyBufY;
		tVDispState.pbyCbFrame = ptVInFB->pbyBufU;
		tVDispState.pbyCrFrame = ptVInFB->pbyBufV;

		if (VideoDisp_QueueBuf(ptInfo->hVDispObj, &tVDispState) != S_OK) {
			printf("Queue buffer failed !\n");
			exit(1);
		}
		list_add_element(ptInfo->pDisplayInfoQueue, dwBufIndex,dwHandleIndex);
		ptInfo->dwQueueElmtNum++;
		if (VideoDisp_DeQueueBuf(ptInfo->hVDispObj, ptVDispState) != S_OK) {
			printf("Release buffer failed !\n");
			exit(1);

		}
		list_remove_element(ptInfo->pDisplayInfoQueue);
		ptInfo->dwQueueElmtNum--;

	}
	return S_OK;
}

/* ========================================================================== */
static void VencDisplay_MsgApplyOpt(TDisplayInfo *ptInfo)
{
	TDisplayMsgOptRec *ptMsgOptRec = &(ptInfo->tMsgOptRec);

	DISP_DP("%s ptMsgOptRec->bDumpVDispBuf=%d",
			DISP_PRE, ptMsgOptRec->bDumpVDispBuf);

	if (ptMsgOptRec->bDumpVCapBuf == TRUE) {
		ptInfo->bDumpVCapBuf = TRUE;
	} else if (ptMsgOptRec->bDumpVDispBuf == TRUE) {
		ptInfo->bDumpVDispBuf = TRUE;
	} else if (ptMsgOptRec->bNeed2Display == TRUE) {
		ptInfo->bNeed2Display = TRUE;
	} else if (ptMsgOptRec->bNeed2Display == FALSE) {
		ptInfo->bNeed2Display = FALSE;
	}
	printf("[venc_display_process](%d)ptInfo->bNeed2Display=%d\n",__LINE__,ptInfo->bNeed2Display);

	memset(ptMsgOptRec, 0, sizeof(TDisplayMsgOptRec));
	return;
}

static BOOL is_all_dummy_in_display_queue(TDisplayInfo *ptInfo)
{

	struct my_struct* p = NULL;
	DWORD dwCount = 0;
	BOOL isAllDummyInDispQueue = FALSE;

	if (ptInfo->pDisplayInfoQueue)
	{
		for (p = ptInfo->pDisplayInfoQueue->head; p; p = p->next)
		{
			if (((p->bufferIndex == VIDEO_DISP_DUMMP_BUF_INDEX) && (p->handleIndex == VIDEO_DISP_DUMMP_BUF_HANDLE_INDEX  )  )|| ((p->bufferIndex== VIDEO_DISP_DUMMP_BUF_INDEX2) && (p->handleIndex == VIDEO_DISP_DUMMP_BUF_HANDLE_INDEX )))
			{
				dwCount++;
			} 

		}

	}
	if (dwCount == 2)
	{
		isAllDummyInDispQueue = TRUE;	     
	} 

	return isAllDummyInDispQueue;	    


}

static SCODE remove_frame_from_display_queue(TDisplayInfo *ptInfo, TVideoDispState *ptRemovedVDispState, struct my_struct *pt)
{

	SCODE sRet = S_OK;
	memset(ptRemovedVDispState,0x0,sizeof(TVideoDispState));

	if (VideoDisp_DeQueueBuf(ptInfo->hVDispObj, ptRemovedVDispState) != S_OK)
	{
		printf("(%d)Call VideoDisp_DeQueueBuf() failed!!\n",__LINE__);
		sRet = S_FAIL;
		goto exit;
	}
	else
	{
		printf("Remove buffer index:%d\n",ptRemovedVDispState->dwIndex);
		memset(pt,0x0,sizeof( struct my_struct));
		list_remove_element_and_get_removed_elem(ptInfo->pDisplayInfoQueue, pt);
		ptInfo->dwQueueElmtNum--;


	}
exit:
	return sRet;

}

static SCODE add_frame_into_display_queue(TDisplayInfo *ptInfo,DWORD dwBufIndex,DWORD dwHandleIndex,BYTE *pbyYFrame,BYTE *pbyCbFrame,BYTE *pbyCrFrame)
{
	SCODE sRet = S_OK;
	SCODE scRet = S_OK;

	TVideoDispState     tVDispState; 

	memset(&tVDispState,0x0,sizeof(TVideoDispState));

	tVDispState.dwIndex = dwBufIndex;
	tVDispState.pbyYFrame = pbyYFrame;
	tVDispState.pbyCbFrame = pbyCbFrame ;
	tVDispState.pbyCrFrame = pbyCrFrame ;

	scRet = VideoDisp_QueueBuf(ptInfo->hVDispObj, &tVDispState); 
	if (scRet != S_OK) {

		fprintf(stderr, "%s (%d) Err: Initial stage: VideoDisp_QueueBuf failed - 0x%08x\n",DISP_PRE,__LINE__ ,scRet);
		sRet = S_FAIL;
		goto exit;
	}

	if (ptInfo->pDisplayInfoQueue != NULL)
	{
		list_add_element(ptInfo->pDisplayInfoQueue, dwBufIndex,dwHandleIndex);
		ptInfo->dwQueueElmtNum++;
	}
	else
	{
		sRet = S_FAIL;
	}
exit:
	return sRet;
}

static void VencDisplay_DumpVideoCapBuf(TVideoFrameBuffer   *ptVFrameBuf)
{
	//! dump file here.
	FILE    *pfDumpYUV;
	CHAR    szTemp[64];
	DWORD   dwDumpSize = 0;
	DWORD   dwWidth = 0;
	DWORD   dwHeight = 0;
	dwWidth  = ptVFrameBuf->dwStride;
	dwHeight = g_avfInfo[ptVFrameBuf->vFormat].wEncHeight;
	
	memset(szTemp,0x0,sizeof(szTemp));
	sprintf(szTemp, "/tmp/VideoDispGetVideoCapBuf_%ux%u_420.yuv", dwWidth, dwHeight);
	
	if ((pfDumpYUV = fopen(szTemp, "wb")) == NULL) {
		fprintf(stderr, "%s Err: fopen \"%s\": (%d) %s\n", DISP_PRE, szTemp, errno, strerror(errno));
	} else {
		dwDumpSize = dwWidth * dwHeight;
		fwrite(ptVFrameBuf->pbyBufY, 1, dwDumpSize, pfDumpYUV);
		fwrite(ptVFrameBuf->pbyBufU, 1, (dwDumpSize >> 2), pfDumpYUV);
		fwrite(ptVFrameBuf->pbyBufV, 1, (dwDumpSize >> 2), pfDumpYUV);
		fclose(pfDumpYUV);
	}
}

/* ========================================================================== */
void* VencDisplay_Loop(HANDLE hInstance)
{
	TDisplayInfo        *ptInfo      = (TDisplayInfo *)(hInstance);
	TVideoFrameBuffer   *ptVFrameBuf = NULL;
	SCODE               scRet        = S_FAIL;
	DWORD               dwSrdBufIndex   = 0;
#ifdef  DISP_PRINT_FPS
	struct timeval pre_display_handler_tv, cur_display_handler_tv;
	FLOAT fDisplayHanlderTime = 0.0;
#endif	

#ifdef	DISP_PRINT_INPUT_FPS
	DWORD dwCurFrameTime = 0.0;
	DWORD dwPreFrameTime = 0.0;
#endif	
	static DWORD dwShowCount = 0;

	static BOOL bPreNeed2Display = FALSE;
	//static BOOL bPreDisplayStart = FALSE;

	static DWORD  dwPreOutputWidth = -1;
	static DWORD  dwPreOutputHeight = -1;
	static DWORD  dwPreFreq = -1;
	static EDispOutFmt  ePreDispOutFmt = vDispOutFmtUnknown;

	//static BOOL bNeedStartVOC = FALSE;

	static BOOL bDisplayModeChanged = FALSE;

	syslog(LOG_INFO, "%s venc_display thread pid: %d\n", DISP_PRE, getpid());

	pthread_detach(pthread_self());

	while (1) {
		sem_wait(&(ptInfo->smStart));
		if (ptInfo->bTerminateThread == TRUE) break;

		DISP_DP("%s Display Loop Start\n", DISP_PRE);

		while (ptInfo->bRunning == TRUE) {
			//! bReConf_flag should set to true when first running
			if (ptInfo->bReConf_flag == TRUE) {
				DISP_DP("%s Reload configuration\n", DISP_PRE);
				sem_wait( &(ptInfo->smOpt) );
				VencDisplay_LoadConfig(hInstance);
				ptInfo->bReConf_flag = FALSE;
				sem_post( &(ptInfo->smOpt) );
			}

			if (ptInfo->bReMsg_flag == TRUE) {
				sem_wait( &(ptInfo->smMsg) );
				VencDisplay_MsgApplyOpt(ptInfo);
				ptInfo->bReMsg_flag = FALSE;
				sem_post( &(ptInfo->smMsg) );
			}

			if ((bPreNeed2Display != ptInfo->bNeed2Display)||
					(dwPreOutputWidth != ptInfo->dwOutputWidth)||
					(dwPreOutputHeight != ptInfo->dwOutputHeight)||
					(dwPreFreq != ptInfo->dwFreq)||
					(ePreDispOutFmt != ptInfo->eDispOutFmt)
			  )
			{
				bDisplayModeChanged = TRUE;

				bPreNeed2Display = ptInfo->bNeed2Display;
				/*bPreDisplayStart = ptInfo->bDisplayStart;*/
				dwPreOutputWidth = ptInfo->dwOutputWidth;
				dwPreOutputHeight = ptInfo->dwOutputHeight;
				dwPreFreq = ptInfo->dwFreq;
				ePreDispOutFmt = ptInfo->eDispOutFmt;


			}

			if ((ptInfo->bNeed2Display == FALSE) || (ptInfo->bDisplayStart == TRUE) || (bDisplayModeChanged == TRUE))
			{

				printf("[venc_display_process](%d)(bNeed2Display,bDisplayStart,bDisplayModeChanged)=(%d,%d,%d)\n",__LINE__,ptInfo->bNeed2Display,ptInfo->bDisplayStart,bDisplayModeChanged);

				//when display mode has been chaneged, the index of sharedbuffer info should be reset.
				dwSrdBufIndex = 0;

				dwShowCount = 0;

				/*
				 * When output wanto to be disabled/enabled or the display mode has been chaned, do the following things:
				 * (1) Call VideoDisp_DeQueueBuf() to dequeue a buffer from display queue.
				 * (2) Call VideoDisp_QueueBuf() to addquque a dummy buffer (VIDEO_DISP_DUMMP_BUF_INDEX) to display queue.
				 * (3) Call VideoDisp_DeQueueBuf() to dequeue a buffer from display queue.
				 * (4) Call VideoDisp_QueueBuf() to addquque a dummy buffer (VIDEO_DISP_DUMMP_BUF_INDEX2) to display queue.
				 * (5) Release all sharedbuffer handles of buffer readers. (release the locks of readers)
				 * NOTE:
				 * (1) For Calling VideoDisp_DeQueueBuf(),we need to call  VideoDisp_Start() to start VOC.
				 * (2) When calling  VideoDisp_Start(), there will be  a interrupt happening. 
				 * (3) The interrupt will cause sem_wait() to wakeup. Should be the condition detected? (http://stackoverflow.com/questions/3661872/sem-wait-not-working-as-expected).
				 * 
				 */  

				printf("(%d)=========================================\n",__LINE__);

				if (is_all_dummy_in_display_queue(ptInfo) == TRUE)
				{
					goto ReleaseShrdBufferHnadle;
				}

				if (ptInfo->hVDispObj != NULL) 
				{
					VideoDisp_Start(ptInfo->hVDispObj);
					//bNeedStartVOC = TRUE;
				} 
				else
				{
					fprintf(stderr, "(%d)%shVDispObj is NULL\n",__LINE__ ,DISP_PRE); 
				} 

				{
					struct my_struct t;
					DWORD dwYSize = DISPLAY_OUT_BUF_WIDTH * DISPLAY_OUT_BUF_HEIGHT;
					DWORD dwCbCrSize = (dwYSize >> 2);
					TVideoDispState     tVDispState;  
					BYTE *pbyYFrame = NULL;
					BYTE *pbyCbFrame = NULL;
					BYTE *pbyCrFrame = NULL;

					memset(&t,0x0,sizeof( struct my_struct));
					memset(&tVDispState,0x0,sizeof(TVideoDispState));

					printf("(%d)Before Removing, ptInfo->dwQueueElmtNum=%d\n",__LINE__,ptInfo->dwQueueElmtNum);
					list_print(ptInfo->pDisplayInfoQueue);

					if (remove_frame_from_display_queue(ptInfo,&tVDispState,&t) == S_FAIL)
					{
						goto exit;
					}
					if (t.handleIndex != VIDEO_DISP_RESIZE_BUF_HANDLE_INDEX)
					{ 
						if (ptInfo->tSrdBufInfo[t.handleIndex].hSrdObj != NULL)
						{
							
							VencDisplay_ReleaseInBuf(ptInfo->tSrdBufInfo[t.handleIndex].hSrdObj, &ptInfo->tSrdBufInfo[t.handleIndex].tSrdBufMgrState);
							SharedBuffMgr_Release( &(ptInfo->tSrdBufInfo[t.handleIndex].hSrdObj) );
							ptInfo->tSrdBufInfo[t.handleIndex].hSrdObj = NULL;
						}
					}
					pbyYFrame = ptInfo->pbyDispBuf;
					pbyCbFrame = ptInfo->pbyDispBuf + dwYSize;
					pbyCrFrame = pbyCbFrame+dwCbCrSize;

					if (add_frame_into_display_queue(ptInfo,VIDEO_DISP_DUMMP_BUF_INDEX,
								VIDEO_DISP_DUMMP_BUF_HANDLE_INDEX,
								pbyYFrame,
								pbyCbFrame,
								pbyCrFrame) == S_FAIL)
					{
						goto exit;
					}

					if (remove_frame_from_display_queue(ptInfo,&tVDispState,&t) == S_FAIL)
					{
						goto exit;
					}
					if (t.handleIndex != VIDEO_DISP_RESIZE_BUF_HANDLE_INDEX)
					{
						if (ptInfo->tSrdBufInfo[t.handleIndex].hSrdObj != NULL)
						{
							VencDisplay_ReleaseInBuf(ptInfo->tSrdBufInfo[t.handleIndex].hSrdObj, &ptInfo->tSrdBufInfo[t.handleIndex].tSrdBufMgrState);
							SharedBuffMgr_Release(&(ptInfo->tSrdBufInfo[t.handleIndex].hSrdObj));
							ptInfo->tSrdBufInfo[t.handleIndex].hSrdObj = NULL; 
						}
					}

					pbyYFrame = ptInfo->pbyDispBuf2;
					pbyCbFrame = ptInfo->pbyDispBuf2 + dwYSize;
					pbyCrFrame = pbyCbFrame+dwCbCrSize;


					if (add_frame_into_display_queue(ptInfo,VIDEO_DISP_DUMMP_BUF_INDEX2,
								VIDEO_DISP_DUMMP_BUF_HANDLE_INDEX,
								pbyYFrame,
								pbyCbFrame,
								pbyCrFrame) == S_FAIL)
					{
						goto exit;
					}

					printf("(%d)After Removing, ptInfo->dwQueueElmtNum=%d\n",__LINE__,ptInfo->dwQueueElmtNum);
					list_print(ptInfo->pDisplayInfoQueue);


				}

ReleaseShrdBufferHnadle:
				{
					int i = 0;
					for (i = 0; i < READER_NUM; i++){
						if (ptInfo->tSrdBufInfo[i].hSrdObj != NULL) {
							//! Release shared buffer instance
							printf("%d\n",__LINE__);
							scRet = SharedBuffMgr_Release( &(ptInfo->tSrdBufInfo[i].hSrdObj) );
							if (scRet != S_OK) {
								fprintf(stderr, 
										"%s Err: SharedBuffMgr_Release 0x%08x\n", 
										DISP_PRE, scRet);
							}
							else
							{
								printf("(%d)SharedBuffMgr_Release handleIndex=%d\n",__LINE__,i);
							} 
							printf("%d\n",__LINE__);
							ptInfo->tSrdBufInfo[i].hSrdObj = NULL;
						} 
						else 
							fprintf(stderr, "%s hSrdObj is NULL\n", DISP_PRE);
					}
				}
				printf("(%d)=========================================\n",__LINE__);

				printf("%s Display state changed=%d\n", DISP_PRE,bDisplayModeChanged);

				/*
				 * The condition "(bDisplayModeChanged==TRUE) && (ptInfo->bNeed2Display == TRUE)" means the following contion:
				 * Output has been eanbled. And the output mode has been changed. So the sharedbuffer handles need to be inited.
				 */
				if ((bDisplayModeChanged == TRUE) && (ptInfo->bNeed2Display == TRUE))
				{

					printf("%s Re-init sharedbuffer status! Display status changed!\n", DISP_PRE); 

					if (VencDisplay_CheckNeedResize(ptInfo) == S_OK)
					{	

						if (ptInfo->bNeedResize == TRUE)
						{
							VencDisplay_InitialSharedBuffer(ptInfo ,1 ,FALSE);
							VencDisplay_CheckInputFrameRate(ptInfo);
						}
						else
						{
							if (ptInfo->dwFreq == 60)
							{	
								VencDisplay_InitialSharedBuffer(ptInfo ,READER_NUM ,FALSE);
								VencDisplay_SyncIndex(ptInfo);
							}
							else//dwFreq==24,30
							{
								VencDisplay_InitialSharedBuffer(ptInfo ,1 ,TRUE);	
							}
						}
					}
					else
					{
						goto StopDsipaly;
					}
					bDisplayModeChanged = FALSE;

				}

				/*
				 * The condition "(ptInfo->bNeed2Display == TRUE) && (ptInfo->bDisplayStart==TRUE)" means "want to enable output".
				 */    
				if ((ptInfo->bDisplayStart==TRUE) && (ptInfo->bNeed2Display == TRUE))
				{
					printf("%s Start displaying\n", DISP_PRE);

					if (ptInfo->hVDispObj != NULL) 
					{
						VideoDisp_Start(ptInfo->hVDispObj);
					} 
					else
					{
						fprintf(stderr, "%s hVDispObj is NULL\n", DISP_PRE);
					}
					ptInfo->bDisplayStart = FALSE;
				}

				/*
				 * The condition "(ptInfo->bNeed2Display == FALSE)" means "want to disable output". 
				 */ 
				if (ptInfo->bNeed2Display == FALSE)
				{
StopDsipaly:			
					printf("%s Stop displaying!\n", DISP_PRE);  

					if (ptInfo->hVDispObj != NULL) {
						printf("%s Begin Calling VideoDisp_Stop()!\n", DISP_PRE); 
						VideoDisp_Stop(ptInfo->hVDispObj);
						printf("%s Finish Calling VideoDisp_Stop()!\n", DISP_PRE); 
					} 
					else fprintf(stderr, "%s hVDispObj is NULL\n", DISP_PRE);

					//continue_wait:	
					if (sem_wait(&(ptInfo->smDisp)) != 0)
					{
						printf("(%d)Call sem_wait failed!! (%s)\n",__LINE__,strerror(errno));
					}
#if 1
					if (errno == EINTR)
					{  
						printf("(%d)%s\n",__LINE__,strerror(errno));
						//bNeedStartVOC = FALSE;
						//goto continue_wait;
					}
#endif					
					//printf("(%d)\n",__LINE__);
					continue;
				}
			}

#ifndef DISP_PRINT_FPS
			if (dwShowCount < 10)
			{
				list_print(ptInfo->pDisplayInfoQueue);
			}
			dwShowCount++;
#endif			
			if (ptInfo->bNeedResize == FALSE) {

				if (ptInfo->dwFreq == 60)
				{
					//! try to get source in buffer
					ptVFrameBuf = VencDisplay_RequestInBuf(ptInfo, ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, ptInfo->tSrdBufInfo[dwSrdBufIndex].iSrdFD, &ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
					if (ptVFrameBuf == NULL) {
						sched_yield();
						continue;
					}
					if ((ptInfo->bDumpVCapBuf == TRUE) && (ptVFrameBuf != NULL))
					{
							VencDisplay_DumpVideoCapBuf(ptVFrameBuf);
							ptInfo->bDumpVCapBuf = FALSE;
					}
					if (ptInfo->tSrdBufInfo[dwSrdBufIndex].iShouldGetCounter != 0) {
						VencDisplay_ReleaseInBuf(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, &ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
						ptInfo->tSrdBufInfo[dwSrdBufIndex].iShouldGetCounter++;
						if (ptInfo->tSrdBufInfo[dwSrdBufIndex].iShouldGetCounter == READER_NUM) {
							ptInfo->tSrdBufInfo[dwSrdBufIndex].iShouldGetCounter = 0;
						}
					} else {  // this frame is needed, feed into VOC
						TVideoDispState     tVDispState;    // for dequeue used.

#ifdef  DISP_PRINT_FPS
						gettimeofday(&pre_display_handler_tv,NULL);
#endif
						VencDisplay_Handler(ptInfo, 
								ptVFrameBuf,
								ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState.dwIndex,
								&tVDispState,
								dwSrdBufIndex
								);

#ifdef  DISP_PRINT_FPS					
						gettimeofday(&cur_display_handler_tv,NULL);
						fDisplayHanlderTime = ((cur_display_handler_tv.tv_sec-pre_display_handler_tv.tv_sec)*1000000+cur_display_handler_tv.tv_usec)-pre_display_handler_tv.tv_usec;
						printf("[venc_display_process](%d)[%d]fps=%6f\n",__LINE__,dwSrdBufIndex,1000000.0/fDisplayHanlderTime);
#endif
						ptInfo->tSrdBufInfo[dwSrdBufIndex].iShouldGetCounter++;
						if ((tVDispState.dwIndex != VIDEO_DISP_DUMMP_BUF_INDEX) && (tVDispState.dwIndex != VIDEO_DISP_DUMMP_BUF_INDEX2)) {
							if (dwSrdBufIndex == 0) {
								VencDisplay_ReleaseInBuf(ptInfo->tSrdBufInfo[1].hSrdObj, &ptInfo->tSrdBufInfo[1].tSrdBufMgrState);
							} else if (dwSrdBufIndex == 1) {
								VencDisplay_ReleaseInBuf(ptInfo->tSrdBufInfo[2].hSrdObj, &ptInfo->tSrdBufInfo[2].tSrdBufMgrState);
							} else if (dwSrdBufIndex == 2) {
								VencDisplay_ReleaseInBuf(ptInfo->tSrdBufInfo[0].hSrdObj, &ptInfo->tSrdBufInfo[0].tSrdBufMgrState);
							}
						}
					
						ptVFrameBuf = NULL;
						
						dwSrdBufIndex++;
						if (dwSrdBufIndex == READER_NUM) {
							dwSrdBufIndex = 0;
						}
					}
				}
				else
				{

					TVideoDispState     tVDispState;    // for dequeue used.
					dwSrdBufIndex = 0;
					ptVFrameBuf = VencDisplay_RequestInBuf(ptInfo, ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, ptInfo->tSrdBufInfo[dwSrdBufIndex].iSrdFD, &ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
					if (ptVFrameBuf == NULL) {
						sched_yield();
						continue;
					}
					if ((ptInfo->bDumpVCapBuf == TRUE) && (ptVFrameBuf != NULL))
					{
							VencDisplay_DumpVideoCapBuf(ptVFrameBuf);
							ptInfo->bDumpVCapBuf = FALSE;
					}
#ifdef  DISP_PRINT_FPS
					gettimeofday(&pre_display_handler_tv,NULL);
#endif
					VencDisplay_Handler(ptInfo, 
							ptVFrameBuf,
							ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState.dwIndex,
							&tVDispState,
							dwSrdBufIndex
							);
#ifdef  DISP_PRINT_FPS					
					gettimeofday(&cur_display_handler_tv,NULL);
					fDisplayHanlderTime = ((cur_display_handler_tv.tv_sec-pre_display_handler_tv.tv_sec)*1000000+cur_display_handler_tv.tv_usec)-pre_display_handler_tv.tv_usec;
					printf("[venc_display_process](%d)[%d]fps=%6f\n",__LINE__,dwSrdBufIndex,1000000.0/fDisplayHanlderTime);
#endif
					if ((tVDispState.dwIndex != VIDEO_DISP_DUMMP_BUF_INDEX) && (tVDispState.dwIndex != VIDEO_DISP_DUMMP_BUF_INDEX2)) {
						VencDisplay_ReleaseInBuf(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, &ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
					}
					
					ptVFrameBuf = NULL;

				}

			} else { // it need resize
				TVideoDispState     tVDispState;    // for dequeue used.
				dwSrdBufIndex = 0;
				ptVFrameBuf = VencDisplay_RequestInBuf(ptInfo, ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, ptInfo->tSrdBufInfo[dwSrdBufIndex].iSrdFD, &ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
				if (ptVFrameBuf == NULL) {
					sched_yield();
					continue;
				}
				if ((ptInfo->bDumpVCapBuf == TRUE) && (ptVFrameBuf != NULL))
				{
							VencDisplay_DumpVideoCapBuf(ptVFrameBuf);
							ptInfo->bDumpVCapBuf = FALSE;
				}
#ifdef  DISP_PRINT_FPS
				gettimeofday(&pre_display_handler_tv,NULL);
#endif
				if (ptInfo->hFRCtrlObj != NULL)
				{
					TFrameRateCtrlState tFRCtrlState;
					memset(&tFRCtrlState,0x0,sizeof(TFrameRateCtrlState));
					tFRCtrlState.dwSecond = ptVFrameBuf->dwSecond;
					tFRCtrlState.dwMilliSecond = ptVFrameBuf->dwMilliSecond;

					FrameRateCtrl_OneFrame(ptInfo->hFRCtrlObj, &tFRCtrlState);

					if (tFRCtrlState.bAvailable == FALSE) {

						VencDisplay_ReleaseInBuf(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, &ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
						ptVFrameBuf = NULL;
						continue;
					}				  
				}
#ifdef	DISP_PRINT_INPUT_FPS
				dwCurFrameTime = ptVFrameBuf->dwSecond*1000*1000+ptVFrameBuf->dwMilliSecond*1000;
				printf("Time diff =%u fps= %6f\n",(dwCurFrameTime-dwPreFrameTime),1000000.0/(dwCurFrameTime-dwPreFrameTime));
				dwPreFrameTime = dwCurFrameTime;				
#endif	    
				VencDisplay_Handler(ptInfo, 
						ptVFrameBuf,
						ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState.dwIndex,
						&tVDispState,
						dwSrdBufIndex
						);
#ifdef  DISP_PRINT_FPS
				gettimeofday(&cur_display_handler_tv,NULL);
				fDisplayHanlderTime = ((cur_display_handler_tv.tv_sec-pre_display_handler_tv.tv_sec)*1000000+cur_display_handler_tv.tv_usec)-pre_display_handler_tv.tv_usec;
				printf("[venc_display_process](%d)[%d]fps=%6f\n",__LINE__,dwSrdBufIndex,1000000.0/fDisplayHanlderTime);
#endif				

				VencDisplay_ReleaseInBuf(ptInfo->tSrdBufInfo[dwSrdBufIndex].hSrdObj, &ptInfo->tSrdBufInfo[dwSrdBufIndex].tSrdBufMgrState);
				ptVFrameBuf = NULL;
			}


		} //! while (ptInfo->bRunning == TRUE)

		// release again? dequeue again?
		//        if (ptVFrameBuf != NULL) {
		//            VencDisplay_ReleaseInBuf(ptInfo->tSrdBufInfo[0].hSrdObj, &ptInfo->tSrdBufInfo[0].tSrdBufMgrState);
		//            ptVFrameBuf = NULL;
		//        }

		ptInfo->bExitInnerLoop = TRUE;

		DISP_DP("%s ExitInnerLoop\n", DISP_PRE);

	} //! while (1)

exit:  
	sem_post(&(ptInfo->smEnd));
	DISP_DP("%s Exit thread\n", DISP_PRE);

	return 0;
}

/* ========================================================================== */
static SCODE VencDisplay_SetOptions(TDisplayInfo *ptInfo)
{
	TVideoDispOptions   tOptions;
	DWORD               dwWidth         = 0xFFFFFFFF;
	DWORD               dwHeight        = 0xFFFFFFFF;
	DWORD               dwFreq          = 0xFFFFFFFF;
	EDispOutFmt         eDispOutFmt;
	SCODE               scRet           = S_FAIL;

	if (ptInfo == NULL) {
		fprintf(stderr, "%s Err: %s - NULL Pointer.\n", DISP_PRE, __FUNCTION__);
		return S_FAIL;
	}
	dwWidth     = ptInfo->dwOutputWidth;
	dwHeight    = ptInfo->dwOutputHeight;
	dwFreq      = ptInfo->dwFreq;
	eDispOutFmt = ptInfo->eDispOutFmt;
	
	//! display pixel format
	memset( &tOptions, 0x0, sizeof(TVideoDispOptions) );
	tOptions.eOptionFlags = VIDEO_DISP_OPTION_SET_PIXEL_FORMAT;
	if ((eDispOutFmt == vDispOutFmtNTSC) ||(eDispOutFmt == vDispOutFmtPAL)) {
		tOptions. adwUserData[0] = VIDEO_SIGNAL_FORMAT_INTERLACE_CCIR656; 
		DISP_DP("%s %s CCIR656 \n", DISP_PRE, __FUNCTION__);
	} else if (eDispOutFmt == vDispOutFmtHDMI) {
		tOptions. adwUserData[0] = VIDEO_SIGNAL_FORMAT_RGB24; 
		DISP_DP("%s %s RGB24 \n", DISP_PRE, __FUNCTION__);
	} else if (eDispOutFmt ==  vDispOutFmtHDSDI) {
		tOptions. adwUserData[0] = VIDEO_SIGNAL_FORMAT_PROGRESSIVE_BT1120;
		DISP_DP("%s %s BT1120 \n", DISP_PRE, __FUNCTION__);
	} else {
		fprintf(stderr, "%s Err: EDispOutFmt %d not supported!\n", 
				DISP_PRE, eDispOutFmt);
		return S_FAIL;
	}

	scRet = VideoDisp_SetOptions(ptInfo->hVDispObj, &tOptions);
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: VideoDisp_SetOptions 0x%08x\n", 
				DISP_PRE, scRet);
	}
	printf("99999999999999999999999999999999999999999999999999999999\n");
	//! set resolution & frequency
	memset( &tOptions,0x0,sizeof(TVideoDispOptions) );
	tOptions.eOptionFlags = VIDEO_DISP_OPTION_SET_OUTPUT_SIZE;
	if (dwWidth == 720  && dwHeight == 480) {
		tOptions. adwUserData[0] = VIDEO_SIGNAL_SIZE_720x480;
		DISP_DP("%s %s 720x480 ", DISP_PRE, __FUNCTION__);
	} 
	else if (dwWidth == 720  && dwHeight == 576) {
		tOptions. adwUserData[0] = VIDEO_SIGNAL_SIZE_720x576;
		DISP_DP("%s %s 720x576 ", DISP_PRE, __FUNCTION__);
	} 
	else if (dwWidth == 768  && dwHeight == 576) {
		tOptions. adwUserData[0] = VIDEO_SIGNAL_SIZE_768x576;
		DISP_DP("%s %s 768x576 ", DISP_PRE, __FUNCTION__);
	} 
	else if (dwWidth == 1280 && dwHeight == 720) {
		tOptions. adwUserData[0] = VIDEO_SIGNAL_SIZE_1280x720;
		DISP_DP("%s %s 1280x720 ", DISP_PRE, __FUNCTION__);
	} 
	else if (dwWidth == 1280 && dwHeight == 1024) {
		tOptions. adwUserData[0] = VIDEO_SIGNAL_SIZE_1280x1024;
		DISP_DP("%s %s 1280x720 ", DISP_PRE, __FUNCTION__);
	} 
	else if (dwWidth == 1920 && dwHeight == 1080) {
		tOptions. adwUserData[0] = VIDEO_SIGNAL_SIZE_1920x1080;
		DISP_DP("%s %s 1920x1080 ", DISP_PRE, __FUNCTION__);
	} 
	else {
		fprintf(stderr, "%s Err: %dx%d is not supported!\n", 
				DISP_PRE, dwWidth, dwHeight);
		return S_FAIL;
	}

	switch (dwFreq) {
		case 60:
			tOptions. adwUserData[1] = VIDEO_SIGNAL_FREQUENCY_60HZ;
			break;
		case 50:
			tOptions. adwUserData[1] = VIDEO_SIGNAL_FREQUENCY_50HZ;
			break;
		case 30:
			tOptions. adwUserData[1] = VIDEO_SIGNAL_FREQUENCY_30HZ;
			break;
		case 24:
			tOptions. adwUserData[1] = VIDEO_SIGNAL_FREQUENCY_24HZ;
			break;
		default:
			fprintf(stderr, "%s Err: %d Hz is not supported!\n", DISP_PRE, dwFreq);
			return S_FAIL;
	}
	DISP_DP("%dHZ \n", dwFreq);


	scRet = VideoDisp_SetOptions(ptInfo->hVDispObj, &tOptions);
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: VideoDisp_SetOptions 0x%08x\n", 
				DISP_PRE, scRet);
		return scRet;
	}

	return S_OK;
}

/* ========================================================================== */
static SCODE VencDisplay_LoadConfig(HANDLE hObject)
{
	TDisplayInfo    *ptInfo = (TDisplayInfo *)hObject;
	SCODE           scRet   = S_FAIL;

	static DWORD  dwPreOutputWidth = -1;
	static DWORD  dwPreOutputHeight = -1;
	static DWORD  dwPreFreq = -1;
	static EDispOutFmt  ePreDispOutFmt = vDispOutFmtUnknown;
	BOOL bDisplayModeChanged = FALSE;

	if (ptInfo == NULL) return S_FAIL;

	DISP_DP("%s XmlWrapper read config %s \n", 
			DISP_PRE, ptInfo->szCfgFile);
	scRet = XmlWrapper_ReadFile_UsrDefFunc(ptInfo->szCfgFile, 
			ptInfo->hXMLObj, 
			ptInfo);
	if (scRet != S_OK) {
		fprintf(stderr, "%s XmlWrapper reload config fail \"%s\" - 0x%08x \n", 
				DISP_PRE, ptInfo->szCfgFile, scRet);
		return scRet;
	}
	scRet = XmlWrapper_Reset(ptInfo->hXMLObj);
	if (scRet != S_OK) {
		fprintf(stderr, "%s XmlWrapper reset fail - 0x%08x \n", 
				DISP_PRE, scRet);
		return scRet;
	}

	if ((dwPreOutputWidth != ptInfo->dwOutputWidth)||
			(dwPreOutputHeight != ptInfo->dwOutputHeight)||
			(dwPreFreq != ptInfo->dwFreq)||
			(ePreDispOutFmt != ptInfo->eDispOutFmt)
	  )
	{
		bDisplayModeChanged = TRUE;


		dwPreOutputWidth = ptInfo->dwOutputWidth;
		dwPreOutputHeight = ptInfo->dwOutputHeight;
		dwPreFreq = ptInfo->dwFreq;
		ePreDispOutFmt = ptInfo->eDispOutFmt;

	}


	if (bDisplayModeChanged == TRUE)
	{
		return VencDisplay_SetOptions(ptInfo);
	}
	else
	{
		return S_OK;
	}
}
#endif

/* ========================================================================== */

