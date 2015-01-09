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
 * venc_encoder_process.c
 *
 * \brief
 * Encoder Core Processing function.
 * 
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#include <syslog.h>

#include <sys/syscall.h>
#include <unistd.h>
#include "venc_encoder_local.h"

#define gettid()    syscall(__NR_gettid)

#if defined(_MD_TRIGER_ROI_)
#include <unistd.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h> 
#include <fcntl.h>		/* for nonblocking */
#include <errno.h>
#endif

extern int CurWrittingBufIDX;

extern int iWrittingBufNum;

void VencEncoder_PreSetApplyOpt(TEncoderInfo *ptEncoderInfo);

void VencEncoder_Encode(TEncoderInfo *ptEncoderInfo, EVideoProcessMode eVPMode, TVideoFrameBuffer *ptVFB);

/* ========================================================================== */
#ifdef _VMA_IRE
SCODE VencEncoder_ResizeSwitchBuffer(TEncoderInfo *ptEncoderInfo, TVideoFrameBuffer *ptInVFB, TVideoFrameBuffer *ptOutVFB)
{
	TResizeState *ptResizeState = &(ptEncoderInfo->tResizeState);
	DWORD dwDstYSize;
	DWORD dwDstUSize;

	dwDstYSize = ptEncoderInfo->dwReqWidthPad * ptEncoderInfo->dwReqHeightPad;
	dwDstUSize = (dwDstYSize >> 2);
	ptOutVFB->pbyBufU = ptOutVFB->pbyBufY + dwDstYSize;
	ptOutVFB->pbyBufV = ptOutVFB->pbyBufY + dwDstYSize + dwDstUSize;

	// if the signal is interlace scan, and the encoding height <= capture height,
	// no matter the deinterlace is on or off, we should set the dwInHeight to half of input height and dwInStride to doulbe of input stride
	// to get better quality.
	if ((ptEncoderInfo->_tInitNeedInfo.bInterlaceScan == TRUE) && (ptEncoderInfo->dwReqHeightPad <= (ptEncoderInfo->dwCropHeight >> 1))) {
		ptResizeState->dwInWidth = ptEncoderInfo->dwCropWidth;
		ptResizeState->dwInHeight = (ptEncoderInfo->dwCropHeight >> 1);
		ptResizeState->dwOutWidth =  ptEncoderInfo->dwReqWidthPad;
		ptResizeState->dwOutHeight = ptEncoderInfo->dwReqHeightPad;
		ptResizeState->dwInStride = (ptInVFB->dwStride << 1);
		ptResizeState->dwOutStride = ptEncoderInfo->dwReqWidthPad;
	} else {
		ptResizeState->dwInWidth = ptEncoderInfo->dwCropWidth;
		ptResizeState->dwInHeight = ptEncoderInfo->dwCropHeight;
		ptResizeState->dwOutWidth =  ptEncoderInfo->dwReqWidthPad;
		ptResizeState->dwOutHeight = ptEncoderInfo->dwReqHeightPad;
		ptResizeState->dwInStride = ptInVFB->dwStride;
		ptResizeState->dwOutStride = ptEncoderInfo->dwReqWidthPad;
	}

	ptResizeState->dwImageMode = 0; //Mozart support frame mode
	ptResizeState->dwCmptNum = 3; //Y, Cb, Cr
	ptResizeState->ePixelFormat = YUV420; // input format
	ptResizeState->pbyYInFrame = ptInVFB->pbyBufY;
	ptResizeState->pbyCbInFrame = ptInVFB->pbyBufU;
	ptResizeState->pbyCrInFrame = ptInVFB->pbyBufV;
	ptResizeState->pbyYOutFrame = ptOutVFB->pbyBufY;
	ptResizeState->pbyCbOutFrame = ptOutVFB->pbyBufU;
	ptResizeState->pbyCrOutFrame = ptOutVFB->pbyBufV;

#if 0
	printf("(InWid, InHei, OutWid, OutHei, InS, OutS) = (%d, %d, %d, %d, %d, %d)\n",
			ptResizeState->dwInWidth,
			ptResizeState->dwInHeight,
			ptResizeState->dwOutWidth,
			ptResizeState->dwOutHeight,
			ptResizeState->dwInStride,
			ptResizeState->dwOutStride);
#endif      
	/*if (Resize_CheckParam(ptEncoderInfo->hResizeObj, ptResizeState) != S_OK) {
		printf("[VENC_ENCODER] Resize State parameters setting error !!\n");
		printf("Error code = 0x%08X\n", Resize_CheckParam(ptEncoderInfo->hResizeObj, ptResizeState));
		return S_FAIL;
	}*/

	ptEncoderInfo->dwResizePreFrameCapWidth = ptEncoderInfo->dwCropWidth;
	ptEncoderInfo->dwResizePreFrameCapHeight = ptEncoderInfo->dwCropHeight;
	ptEncoderInfo->dwResizePreFrameReqWidthPad = ptEncoderInfo->dwReqWidthPad;
	ptEncoderInfo->dwResizePreFrameReqHeightPad = ptEncoderInfo->dwReqHeightPad;

	ptOutVFB->dwStride = ptEncoderInfo->dwReqWidthPad;
	return S_OK;
}
#endif // _VMA_IRE

/* ========================================================================= */
SCODE VencEncoder_CalculateSize(TEncoderInfo *ptEncoderInfo)
{
	EVideoFormat vfFormat = ptEncoderInfo->vfCurtFormat;

	DWORD dwReqWidth = g_avfInfo[vfFormat].wEncWidth;
	DWORD dwReqHeight =  g_avfInfo[vfFormat].wEncHeight;
	// At first Load, we don't know the capture size,
	// and the dwCurtCapHeight and dwCurtCapHeight are zeros.
	// Set the values to request resolution.
	if (ptEncoderInfo->dwCurtCapWidth == 0 || ptEncoderInfo->dwCurtCapHeight == 0) {
		ptEncoderInfo->dwCurtCapWidth = dwReqWidth;
		ptEncoderInfo->dwCurtCapHeight = dwReqHeight;
	}

	ptEncoderInfo->dwReqWidth = dwReqWidth;
	ptEncoderInfo->dwReqHeight = dwReqHeight;
	ptEncoderInfo->dwReqWidthPad = ptEncoderInfo->_tInitNeedInfo.dwVideoMaxWidth;
	ptEncoderInfo->dwReqHeightPad = ptEncoderInfo->_tInitNeedInfo.dwVideoMaxHeight;

	ptEncoderInfo->dwCropWidth  = ptEncoderInfo->dwCurtCapWidth;
	ptEncoderInfo->dwCropHeight = ptEncoderInfo->dwCurtCapHeight;


	// To avoid the output image being squeezed by the resizer. 
	// We crop the input image by Virtual PTZ engine first. 
	// This functionality is temporarily sealed.

	if (ptEncoderInfo->ecMethod == ecmCropping) {
		FLOAT   fRw, fRh;
		DWORD   dwTmpWidth, dwTmpHeight;
		fRw = (FLOAT)dwReqWidth / (FLOAT)ptEncoderInfo->dwCurtCapWidth;
		fRh = (FLOAT)dwReqHeight / (FLOAT)ptEncoderInfo->dwCurtCapHeight;

		if (fRh >= fRw) {
			dwTmpWidth = ((DWORD)((FLOAT)dwReqWidth / fRh) >> 4) << 4;
			dwTmpHeight = ptEncoderInfo->dwCurtCapHeight;
		} else {
			dwTmpWidth = ptEncoderInfo->dwCurtCapWidth;
			dwTmpHeight = ((DWORD)((FLOAT)dwReqHeight / fRw) >> 4) << 4;
		}
		//printf("(dwTmpWidth, dwTmpHeight) = (%d, %d) fRw=%f fRh=%f\n", dwTmpWidth, dwTmpHeight, fRw, fRh);
		ptEncoderInfo->dwCropWidth = dwTmpWidth;
		ptEncoderInfo->dwCropHeight = dwTmpHeight;
	} else if (ptEncoderInfo->ecMethod == ecmRandomCropping) {
		printf("at random cropping mode, (X,Y,W,H) = (%d,%d,%d,%d)\n", ptEncoderInfo->tCropInfo.dwStartX, ptEncoderInfo->tCropInfo.dwStartY, ptEncoderInfo->tCropInfo.dwWidth, ptEncoderInfo->tCropInfo.dwHeight);
		ptEncoderInfo->dwCropWidth = ptEncoderInfo->tCropInfo.dwWidth;
		ptEncoderInfo->dwCropHeight = ptEncoderInfo->tCropInfo.dwHeight;	  
	} else {
		//printf(__FILE__ ":%d: do nothing\n", __LINE__);
	}

	ptEncoderInfo->dwPTZFullViewWidth = ptEncoderInfo->dwCropWidth;
	ptEncoderInfo->dwPTZFullViewHeight = ptEncoderInfo->dwCropHeight;

	printf("======== Resize infos ========\n");
	printf("[VENC_ENCODER] Captured size W x H: %u x %u \n",
			ptEncoderInfo->dwCurtCapWidth,
			ptEncoderInfo->dwCurtCapHeight);
	printf("[VENC_ENCODER] Request size  W x H: %u x %u \n",
			dwReqWidth,
			dwReqHeight);
	printf("[VENC_ENCODER] Request sizepad W x H: %u x %u\n",
			ptEncoderInfo->dwReqWidthPad,
			ptEncoderInfo->dwReqHeightPad);
	printf("[VENC_ENCODER]ptEncoderInfo->ecMethod=%d\n",ptEncoderInfo->ecMethod);
	printf("[VENC_ENCODER]ptEncoderInfo->dwCropWidth=%d ptEncoderInfo->dwCropHeight=%d\n",ptEncoderInfo->dwCropWidth,ptEncoderInfo->dwCropHeight);
	printf("==============================\n");
	return S_OK;
}

/* ========================================================================== */
SCODE VencEncoder_ReleaseInputBuf(HANDLE hVideoInSrdObj, TSharedBuffMgrState *ptSharedBuffMgrState)
{
	SharedBuffMgr_ReleaseBuffer(hVideoInSrdObj, ptSharedBuffMgrState);
	return S_OK;
}

/* ========================================================================== */
TSharedBuffMgrState* VencEncoder_RequestInputBuf(TEncoderInfo *ptEncoderInfo)
{
	TSharedBuffMgrState *ptSrdBufMgrState = NULL;
	TVideoFrameBuffer *ptVFB = NULL;
	BYTE *pbyDataAddr = NULL;
	DWORD	dwWidth;
	DWORD	dwHeight;
	TFrameRateCtrlState tFRCtrlState;

	fd_set               	fdRead;
	struct timeval 			timeout;
	int						fd = ptEncoderInfo->iVideoInSrdFD;
	int     				iResult;
	//static TVideoFrameBuffer tPreVideoFrameBuffer;  
	FD_ZERO(&fdRead);
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	FD_SET(fd, &fdRead);

	iResult = select(fd+1, &fdRead, NULL, NULL, &timeout);
	if (iResult <= 0) {
		DBPRINT2("%s %d [VENC_ENCODER] get shared buffer select timeout.... \n", __func__, __LINE__);
		return ptSrdBufMgrState;
	}
	if (FD_ISSET(fd, &fdRead)) {
		if (SharedBuffMgr_GetBuffer(ptEncoderInfo->hVideoInSrdObj, &(ptEncoderInfo->tSrdBufMgrState)) == S_OK) {
			ptSrdBufMgrState = &(ptEncoderInfo->tSrdBufMgrState);
			ptVFB = (TVideoFrameBuffer *)(ptSrdBufMgrState->pbyHdrAddr);
			pbyDataAddr = ptSrdBufMgrState->pbyDataAddr;
			dwWidth = g_avfInfo[ptVFB->vFormat].wEncWidth;
			dwHeight = g_avfInfo[ptVFB->vFormat].wEncHeight;
			tFRCtrlState.dwSecond = ptVFB->dwSecond;
			tFRCtrlState.dwMilliSecond = ptVFB->dwMilliSecond;
			tFRCtrlState.dwMicroSecond = ptVFB->dwMicroSecond;
			FrameRateCtrl_OneFrame(ptEncoderInfo->hFRCtrlObj, &tFRCtrlState);
			if (tFRCtrlState.bAvailable == FALSE) {
				VencEncoder_ReleaseInputBuf(ptEncoderInfo->hVideoInSrdObj, ptSrdBufMgrState);	
				ptSrdBufMgrState = NULL;
			}
			else
			{	//Do no thing. Just debug the frame count.
				;
				/*
				printf("dwFrameCount = %d (%s)\n",ptVFB->dwFrameCount,((ptVFB->dwFrameCount%2)==0)?"even":"odd");
				if ((tPreVideoFrameBuffer.dwFrameCount % 2) != (ptVFB->dwFrameCount % 2))
				{
					printf("pre:(%d,%d,%d,%d) -> cur:(%d,%d,%d,%d)\n",
						   tPreVideoFrameBuffer.dwFrameCount ,
						   tPreVideoFrameBuffer.dwSecond,
						   tPreVideoFrameBuffer.dwMilliSecond,
						   tPreVideoFrameBuffer.dwMicroSecond,
						   ptVFB->dwFrameCount ,
						   ptVFB->dwSecond,
						   ptVFB->dwMilliSecond,
						   ptVFB->dwMicroSecond
  						);
				}
				tPreVideoFrameBuffer.dwFrameCount = ptVFB->dwFrameCount;
				tPreVideoFrameBuffer.dwSecond = ptVFB->dwSecond;
				tPreVideoFrameBuffer.dwMilliSecond = ptVFB->dwMilliSecond;
				tPreVideoFrameBuffer.dwMicroSecond = ptVFB->dwMicroSecond;
				*/
				
			}
#ifdef _DUMP_VIDEOINDATA
			fwrite(pbyDataAddr, 1, ((dwWidth * dwHeight)*3)>>1, ptEncoderInfo->pfVideoInData);	
#endif // _DUMP_VIDEOINDATA

#if 0
			printf("[Cap Result] Time = %u:%03u, Width, Height = %ux%u, dwStride = %u\n",                
					ptVFB->dwSecond,
					ptVFB->dwMilliSecond,
					dwWidth, 
					dwHeight,
					ptVFB->dwStride);

#endif // 1
			// take this picture and APP should release this buffer

			if ((ptEncoderInfo->dwCurtCapWidth != dwWidth) || (ptEncoderInfo->dwCurtCapHeight != dwHeight)) {
#if 0
				printf("Previous Captured size: %4u x %4u \n",
						ptEncoderInfo->dwCurtCapWidth,
						ptEncoderInfo->dwCurtCapHeight);
				printf("Current Captured size: %4u x %4u \n",
						dwWidth, 
						dwHeight);
#endif	
				ptEncoderInfo->dwCurtCapWidth = dwWidth;
				ptEncoderInfo->dwCurtCapHeight = dwHeight;
				ptEncoderInfo->bNeed2Recalculate = TRUE;
			}
		} else {
			ptSrdBufMgrState = NULL;
		}
	}
	return ptSrdBufMgrState;
}

/* ========================================================================== */
void VencEncoder_PreSetApplyOpt(TEncoderInfo *ptEncoderInfo)
{
	TPreSetOptRec *ptPreSetOptRec = &(ptEncoderInfo->tPreSetOptRec);

	if (ptPreSetOptRec->bConvertingMethod == TRUE) {
		ptEncoderInfo->ecMethod = ptPreSetOptRec->ecMethod;
		ptEncoderInfo->bNeed2Recalculate = TRUE;
	}

	if (ptPreSetOptRec->bCodec == TRUE) {
		ptEncoderInfo->eVPMode = ptPreSetOptRec->dwCodecType;
		ptEncoderInfo->bCodecChange = TRUE;
	}

	if (ptPreSetOptRec->bResolution == TRUE) {
		ptEncoderInfo->vfCurtFormat = (EVideoFormat)ptPreSetOptRec->vfFormat;
		ptEncoderInfo->bForceCI = TRUE;
		ptEncoderInfo->bNeed2Recalculate = TRUE;
	}

	if (ptPreSetOptRec->bFrameRate == TRUE) {
		TFRCtrlOptions tFRCtrlOptions;
		tFRCtrlOptions.eOptionFlags = FRCTRL_CHANGE_FRAMERATE;
		tFRCtrlOptions.adwUserData[0] = ptPreSetOptRec->dwFrameRate;
		FrameRateCtrl_SetOption(ptEncoderInfo->hFRCtrlObj, &tFRCtrlOptions);

        VencEncoder_H264ChangeFrameTimeTick(ptEncoderInfo, ptPreSetOptRec->dwFrameRate);
	}

	if (ptPreSetOptRec->bTextOverlay == TRUE) {
		VencSlave_TextOverlay_SetOptions(&ptEncoderInfo->tTextOverlayData, 
				&ptEncoderInfo->tPreSetOptRec.tTextOverlayOptions);
	}

	memset(ptPreSetOptRec, 0, sizeof(TPreSetOptRec));	
}

/* ========================================================================= */
void VencEncoder_SetTextOverlayOptions(HANDLE hEncoderProcess, TTextOverlayOptions *ptTextOverlayOpt)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)hEncoderProcess;
	if (ptEncoderInfo == NULL) {
		fprintf(stderr, "[VENC_ENCODER] VencSlave setoptions fail: NULL Pointer!!\n");
		return;
	}
	//    sem_wait(&(ptEncoderInfo->smOptions));   
	ptEncoderInfo->tPreSetOptRec.bTextOverlay = TRUE;
	memcpy(&(ptEncoderInfo->tPreSetOptRec.tTextOverlayOptions), 
			ptTextOverlayOpt, sizeof(TTextOverlayOptions));

	ptEncoderInfo->bOptionsModified = TRUE;    
	//    sem_post(&(ptEncoderInfo->smOptions));
	return;
}
/* ========================================================================== */

#if defined(_MD_TRIGER_ROI_)

#define SHARED_MEM_PATH_NAME "md_trigger_roi_shm"
#define SHARED_MEM_LEN 8192
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
static SCODE read_shm(char *szResult,int iResultLen)
{
	int		i, fd;
	struct stat	stat;
	char	*ptr;
	fd = shm_open(SHARED_MEM_PATH_NAME, O_RDONLY, FILE_MODE);
	if (fd == -1)
	{
		printf("[venc_videoin_process](%d)%s\n",__LINE__,strerror(errno));
		return S_FAIL;
	}
	fstat(fd, &stat);
	ptr = mmap(NULL, stat.st_size, PROT_READ,
			MAP_SHARED, fd, 0);
	close(fd);
	if (iResultLen < stat.st_size)
	{
		printf("[venc_videoin_process](%d)Can not read the shm from %s. Because the len of reading buf  %d is smaller than shm size %d\n",__LINE__,SHARED_MEM_PATH_NAME,iResultLen,(int)stat.st_size);  
		return S_FAIL;
	}
	memset(szResult,0x0,iResultLen);
	for (i = 0; i < stat.st_size; i++)
		szResult[i] = ptr[i] ;
	
	munmap(ptr,stat.st_size);
	
	return S_OK;
}
/*
   <?xml version="1.0" encoding="ISO-8859-1" ?>
   <root>
   <enable>0</enable>
   <id>0</id>
   <startMBX>3</startMBX>
   <startMBY>20</startMBY>
   <endMBX>9</endMBX>
   <endMBY>33</endMBY>
   <deltaQ>-40</deltaQ>
   <interval>1</interval>
   </root>

 */

void SetROIWinEnable(HANDLE hObject, const CHAR *szValue, SDWORD len)
{
	TH264EncROIWindowsInfos *ptH264EncROIWindowsInfos = (TH264EncROIWindowsInfos *)(hObject);
	CHAR szTemp[len+1];
	DWORD	dwValue = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szValue);

	dwValue = strtoul(szTemp, (char**)NULL, 10);
	ptH264EncROIWindowsInfos->bROIEnable = dwValue;
	return;

}
void SetROIWinID(HANDLE hObject, const CHAR *szValue, SDWORD len)
{
	TH264EncROIWindowsInfos *ptH264EncROIWindowsInfos = (TH264EncROIWindowsInfos *)(hObject);
	CHAR szTemp[len+1];
	DWORD	dwValue = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szValue);

	dwValue = strtoul(szTemp, (char**)NULL, 10);
	ptH264EncROIWindowsInfos->dwROIWindowNum = dwValue;
	return;
}
void SetROIWinStartMBX(HANDLE hObject, const CHAR *szValue, SDWORD len)
{
	TH264EncROIWindowsInfos *ptH264EncROIWindowsInfos = (TH264EncROIWindowsInfos *)(hObject);
	CHAR szTemp[len+1];
	DWORD	dwValue = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szValue);

	dwValue = strtoul(szTemp, (char**)NULL, 10);
	ptH264EncROIWindowsInfos->dwStartMBXNum = dwValue;
	return;
}
void SetROIWinStartMBY(HANDLE hObject, const CHAR *szValue, SDWORD len)
{
	TH264EncROIWindowsInfos *ptH264EncROIWindowsInfos = (TH264EncROIWindowsInfos *)(hObject);
	CHAR szTemp[len+1];
	DWORD	dwValue = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szValue);

	dwValue = strtoul(szTemp, (char**)NULL, 10);
	ptH264EncROIWindowsInfos->dwStartMBYNum = dwValue;
	return;

}
void SetROIWinEndMBX(HANDLE hObject, const CHAR *szValue, SDWORD len)
{
	TH264EncROIWindowsInfos *ptH264EncROIWindowsInfos = (TH264EncROIWindowsInfos *)(hObject);
	CHAR szTemp[len+1];
	DWORD	dwValue = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szValue);

	dwValue = strtoul(szTemp, (char**)NULL, 10);
	ptH264EncROIWindowsInfos->dwEndMBXNum = dwValue;
	return;

}
void SetROIWinEndMBY(HANDLE hObject, const CHAR *szValue, SDWORD len)
{
	TH264EncROIWindowsInfos *ptH264EncROIWindowsInfos = (TH264EncROIWindowsInfos *)(hObject);
	CHAR szTemp[len+1];
	DWORD	dwValue = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szValue);

	dwValue = strtoul(szTemp, (char**)NULL, 10);
	ptH264EncROIWindowsInfos->dwEndMBYNum = dwValue;
	return;

}
void SetROIWinDeltaQ(HANDLE hObject, const CHAR *szValue, SDWORD len)
{
	TH264EncROIWindowsInfos *ptH264EncROIWindowsInfos = (TH264EncROIWindowsInfos *)(hObject);

	CHAR szTemp[len+1];

	SDWORD sdwValue  = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szValue);

	sdwValue  = atoi(szTemp);

	ptH264EncROIWindowsInfos->sdwDeltaQp = sdwValue;
	return;

}
void SetROIWinInterval(HANDLE hObject, const CHAR *szValue, SDWORD len)
{
	TH264EncROIWindowsInfos *ptH264EncROIWindowsInfos = (TH264EncROIWindowsInfos *)(hObject);

	CHAR szTemp[len+1];
	DWORD	dwValue = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szValue);

	dwValue = strtoul(szTemp, (char**)NULL, 10);

	//printf("[venc_encoder_process](%d)szTemp=%s dwValue=%d\n",__LINE__,szTemp,dwValue);
	ptH264EncROIWindowsInfos->dwEncodingInterval = dwValue;

}

static SCODE parse_roi_win_info(const char *szROIResult,TH264EncROIWindowsInfos *ptH264EncROIWindowsInfos)
{
	int sRet;
	HANDLE hXmlWrapObj;	
	TXmlWrapperTreeMap ptParsingMap[] = 
	{
		{"root/enable", NULL, &SetROIWinEnable, NULL},
		{"root/id", NULL, &SetROIWinID, NULL},
		{"root/startMBX", NULL, &SetROIWinStartMBX, NULL},
		{"root/startMBY", NULL,&SetROIWinStartMBY, NULL},
		{"root/endMBX",NULL,&SetROIWinEndMBX,NULL},
		{"root/endMBY",NULL,&SetROIWinEndMBY,NULL},
		{"root/deltaQ",NULL,&SetROIWinDeltaQ,NULL},
		{"root/interval",NULL,&SetROIWinInterval,NULL},
		{NULL, NULL, NULL}
	};
	TXmlWrapperInitOptions tInitOptions;
	memset(&tInitOptions, 0x0, sizeof(TXmlWrapperInitOptions));

	if (ptH264EncROIWindowsInfos == NULL)
	{
		printf("%s %d :ptH264EncROIWindowsInfos is NULL! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	if (szROIResult == NULL)
	{
		printf("%s %d :szROIResult is NULL! \n", __FILE__, __LINE__);
		return S_FAIL;
	}

	tInitOptions.dwVersion = XMLWRAPPER_VERSION;
	if (XmlWrapper_Initial(&hXmlWrapObj, &tInitOptions) != S_OK)
	{
		return S_FAIL;
	}
	if (XmlWrapper_SetHandler(hXmlWrapObj, ptParsingMap,NULL) != S_OK)
	{
		printf("%s %d : XmlWrapper_SetHandler Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	if ((sRet = XmlWrapper_ReadBuf_UsrDefFunc((BYTE *)szROIResult, hXmlWrapObj,ptH264EncROIWindowsInfos)) != S_OK)
	{
		printf("%s %d : XmlWrapper_ReadBuf_UsrDefFunc Fail! %x\n", __FILE__, __LINE__, sRet);
		return S_FAIL;
	}
	if (XmlWrapper_Release(&hXmlWrapObj) != S_OK)
	{
		printf("Release XmlWrapper Fail! \n");
		return S_FAIL;
	}
	return S_OK;

}
#endif

void VencEncoder_MsgApplyOpt(TEncoderInfo *ptEncoderInfo)
{
	TEncoderMsgOptRec *ptMsgOptRec = &(ptEncoderInfo->tMsgOptRec);
	EVideoProcessMode eVPMode = ptEncoderInfo->eVPMode;
	TEncoderOptions tEncoderOpt;

	if (ptMsgOptRec->bForceCI == TRUE) {
		ptEncoderInfo->bForceCI = TRUE;
		ptMsgOptRec->bForceCI = FALSE;
	}

	if (ptMsgOptRec->bForceIntra == TRUE) {
		if ((eVPMode & vpmStreamEncode) == vpmStreamEncMP4V) {
#ifdef _VMA_MEBE
			ptEncoderInfo->tMp4VOptRec.bForceIntra = TRUE;
			ptEncoderInfo->bMp4VOptionsModified = TRUE;
#endif	
		} else if ((eVPMode & vpmStreamEncode) == vpmStreamEncH264) {
#ifdef _H4EE
			ptEncoderInfo->tH264OptRec.bForceIntra = TRUE;
			ptEncoderInfo->bH264OptionsModified = TRUE;
#endif	
		} else if ((eVPMode & vpmStreamEncode) == vpmStreamEncSVC) {
#ifdef _SVC		
			ptEncoderInfo->tSVCOptRec.bForceIntra = TRUE;
			ptEncoderInfo->bSVCOptionsModified = TRUE;
#endif	
		}

		ptMsgOptRec->bForceIntra = FALSE;
		ptEncoderInfo->bOptionsModified = TRUE;
	}

#ifdef VIRTUAL_PTZ
	if (ptMsgOptRec->bVPTZCmd == TRUE) {
		ptEncoderInfo->bVPTZCmd = TRUE;

		if (ptMsgOptRec->bVPTZPanUpdate == TRUE) {
			ptEncoderInfo->tPTZValue.flPanValue = ptMsgOptRec->flPanValue;
		}
		if (ptMsgOptRec->bVPTZTiltUpdate == TRUE) {
			ptEncoderInfo->tPTZValue.flTiltValue = ptMsgOptRec->flTiltValue;
		}
		if (ptMsgOptRec->bVPTZZoomUpdate == TRUE) {
			ptEncoderInfo->tPTZValue.flZoomValue = ptMsgOptRec->flZoomValue;
		}
		if (ptMsgOptRec->bVPTZSpeedUpdate == TRUE) {
			ptEncoderInfo->tPTZValue.flSpeedValue = ptMsgOptRec->flSpeedValue;
			ptEncoderInfo->bVPTZSpeedUpdate = TRUE;        
		}
		ptMsgOptRec->bVPTZCmd = FALSE;
	}
#endif //VIRTUAL_PTZ

	if (ptMsgOptRec->bTurnOnFRCalculate == TRUE) {
		ptEncoderInfo->bFRCalculate = TRUE;
		ptEncoderInfo->dwFRCalculateFrameCount = 0;
		ptMsgOptRec->bTurnOnFRCalculate = FALSE;
	}

	if (ptMsgOptRec->bTurnOffFRCalculate == TRUE) {
		ptEncoderInfo->bFRCalculate = FALSE;
		ptEncoderInfo->dwFRCalculateFrameCount = 0;
		ptMsgOptRec->bTurnOffFRCalculate = FALSE;
	}
	if (ptMsgOptRec->bQualityParameterChanged == TRUE)
	{
		/* 0:H264,1:SVC,2:MPEG4,3:MJPEG */
		ERateCtrlFlags ercfType;

		ptMsgOptRec->bQualityParameterChanged = FALSE;

		if (ptMsgOptRec->dwQuant > MAX_H264_QUANT) 
		{
			ptMsgOptRec->dwQuant = MAX_H264_QUANT;
		} 
		else if (ptMsgOptRec->dwQuant < MIN_H264_QUANT) 
		{
			ptMsgOptRec->dwQuant = MIN_H264_QUANT;
		}

		memset(&tEncoderOpt,0x0,sizeof(TEncoderOptions));
		if (ptMsgOptRec->dwCodecType == 0)//H264
		{
			tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_H264_QUANT;
		}
		else if (ptMsgOptRec->dwCodecType == 1)//SVC
		{
			tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_SVC_QUANT;
		}
		else if (ptMsgOptRec->dwCodecType == 2)//MPEG4
		{
			tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_MPEG4_QUANT;
		}
		else if (ptMsgOptRec->dwCodecType == 3)//MJPEG
		{
			tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_MJPEG_QUANT;
		}
		else
		{
			tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_UNKNOWN;
		}
		tEncoderOpt.adwUserData[0] = ptMsgOptRec->dwQuant;
		VencEncoder_SetOptions((HANDLE)ptEncoderInfo, tEncoderOpt);

		//There is no bitrate and rate control mode for MJPEG.
		if (ptMsgOptRec->dwCodecType == 3)//MJPEG
		{
			goto next_check;
		}

		if (ptMsgOptRec->dwCtrlMode == 1) 
		{
			ercfType = RATE_CTRL_VQCB;
		} 
		else if (ptMsgOptRec->dwCtrlMode == 2) 
		{
			ercfType = RATE_CTRL_CQCB;
		} 
		else if (ptMsgOptRec->dwCtrlMode == 3) 
		{
			ercfType =  RATE_CTRL_STRICT_VQCB;
		}
		else if (ptMsgOptRec->dwCtrlMode == 4) 
		{
			ercfType = RATE_CTRL_CVBR;
		} 
		else {
			ercfType = RATE_CTRL_NONE;
		}  

		memset(&tEncoderOpt,0x0,sizeof(TEncoderOptions));
		if (ptMsgOptRec->dwCodecType == 0)//H264
		{ 	    
			tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_H264_RATECTRL;
		} 
		else if (ptMsgOptRec->dwCodecType == 1)//SVC
		{
			tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_SVC_RATECTRL;
		}
		else if (ptMsgOptRec->dwCodecType == 2)//MPEG4
		{
			tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_MPEG4_RATECTRL;
		}
		else
		{
			tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_UNKNOWN;
		}
		tEncoderOpt.adwUserData[0] = ercfType;
		VencEncoder_SetOptions((HANDLE)ptEncoderInfo, tEncoderOpt);


		memset(&tEncoderOpt,0x0,sizeof(TEncoderOptions));
		if (ptMsgOptRec->dwCodecType == 0)//H264
		{
			tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_H264_BITRATE;
		}
		else if (ptMsgOptRec->dwCodecType == 1)//SVC
		{
			tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_SVC_BITRATE;
		}
		else if (ptMsgOptRec->dwCodecType == 2)//MPEG4
		{
			tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_MPEG4_BITRATE;
		}
		else
		{
			tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_UNKNOWN;
		}
		tEncoderOpt.adwUserData[0] = ptMsgOptRec->dwBitrate;
		VencEncoder_SetOptions((HANDLE)ptEncoderInfo, tEncoderOpt);

	}
	
next_check:
	if (ptMsgOptRec->bDumpQualityParameter == TRUE)
	{

		FILE *fpDump = NULL;
		CHAR szH264QualityType[16];
		CHAR szSVCQualityType[16];
		CHAR szMp4QualityType[16];
		CHAR szCurrentCodec[16];

		printf("[venc_encoder_process](%d)H264 [mode,bitrate,quant]=(%d,%d,%d)\n",__LINE__,ptEncoderInfo->tH264OptRec.ercfH264QualityType,ptEncoderInfo->tH264OptRec.dwBitrate,ptEncoderInfo->tH264OptRec.dwQuant);
		printf("[venc_encoder_process](%d)SVC [mode,bitrate,quant]=(%d,%d,%d)\n",__LINE__,ptEncoderInfo->tSVCOptRec.ercfSVCQualityType,ptEncoderInfo->tSVCOptRec.dwBitrate,ptEncoderInfo->tSVCOptRec.dwQuant);
		printf("[venc_encoder_process](%d)MPEG4 [mode,bitrate,quant]=(%d,%d,%d)\n",__LINE__,ptEncoderInfo->tMp4VOptRec.ercfMPEG4QualityType,ptEncoderInfo->tMp4VOptRec.dwBitrate,ptEncoderInfo->tMp4VOptRec.dwQuant);
		printf("[venc_encoder_process](%d)JPEG [quant]=(%d)\n",__LINE__,ptEncoderInfo->tJPEGOptRec.dwQuant);

		fpDump = fopen(QUALITY_PARAMETER_DUMP_PATH,"w");
		if (fpDump != NULL)
		{	
			fclose(fpDump);
			fpDump = NULL;
			unlink(QUALITY_PARAMETER_DUMP_PATH);
		}

		fpDump = fopen(QUALITY_PARAMETER_DUMP_PATH,"w");


		if (fpDump == NULL)
			goto exit;

		memset(szH264QualityType,0x0,sizeof(szH264QualityType));
		memset(szSVCQualityType,0x0,sizeof(szSVCQualityType));
		memset(szMp4QualityType,0x0,sizeof(szMp4QualityType));
		memset(szCurrentCodec,0x0,sizeof(szCurrentCodec));

		if (ptEncoderInfo->tH264OptRec.ercfH264QualityType == RATE_CTRL_NONE)
		{
			strncpy(szH264QualityType,"NONE",strlen("NONE"));
		}
		else if (ptEncoderInfo->tH264OptRec.ercfH264QualityType == RATE_CTRL_VQCB)
		{
			strncpy(szH264QualityType,"VQCB",strlen("VQCB"));
		}
		else if (ptEncoderInfo->tH264OptRec.ercfH264QualityType == RATE_CTRL_CQCB)
		{
			strncpy(szH264QualityType,"CQCB",strlen("CQCB"));
		}
		else if (ptEncoderInfo->tH264OptRec.ercfH264QualityType == RATE_CTRL_CVBR)
		{
			strncpy(szH264QualityType,"CVBR",strlen("CVBR"));
		}
		else if (ptEncoderInfo->tH264OptRec.ercfH264QualityType == RATE_CTRL_STRICT_VQCB)
		{
			strncpy(szH264QualityType,"STRICT_VQCB",strlen("STRICT_VQCB"));
		}
		else
		{
			strncpy(szH264QualityType,"NONE",strlen("NONE"));
		}


		if (ptEncoderInfo->tSVCOptRec.ercfSVCQualityType == RATE_CTRL_NONE)
		{
			strncpy(szSVCQualityType,"NONE",strlen("NONE"));
		}
		else if (ptEncoderInfo->tSVCOptRec.ercfSVCQualityType == RATE_CTRL_VQCB)
		{
			strncpy(szSVCQualityType,"VQCB",strlen("VQCB"));
		}
		else if (ptEncoderInfo->tSVCOptRec.ercfSVCQualityType == RATE_CTRL_CQCB)
		{
			strncpy(szSVCQualityType,"CQCB",strlen("CQCB"));
		}
		else if (ptEncoderInfo->tSVCOptRec.ercfSVCQualityType == RATE_CTRL_CVBR)
		{
			strncpy(szSVCQualityType,"CVBR",strlen("CVBR"));
		}
		else if (ptEncoderInfo->tSVCOptRec.ercfSVCQualityType == RATE_CTRL_STRICT_VQCB)
		{
			strncpy(szSVCQualityType,"STRICT_VQCB",strlen("STRICT_VQCB"));
		}
		else
		{
			strncpy(szSVCQualityType,"NONE",strlen("NONE"));
		}

		if (ptEncoderInfo->tMp4VOptRec.ercfMPEG4QualityType == RATE_CTRL_NONE)
		{
			strncpy(szMp4QualityType,"NONE",strlen("NONE"));
		}
		else if (ptEncoderInfo->tMp4VOptRec.ercfMPEG4QualityType == RATE_CTRL_VQCB)
		{
			strncpy(szMp4QualityType,"VQCB",strlen("VQCB"));
		}
		else if (ptEncoderInfo->tMp4VOptRec.ercfMPEG4QualityType == RATE_CTRL_CQCB)
		{
			strncpy(szMp4QualityType,"CQCB",strlen("CQCB"));
		}
		else if (ptEncoderInfo->tMp4VOptRec.ercfMPEG4QualityType == RATE_CTRL_CVBR)
		{
			strncpy(szMp4QualityType,"CVBR",strlen("CVBR"));
		}
		else if (ptEncoderInfo->tMp4VOptRec.ercfMPEG4QualityType == RATE_CTRL_STRICT_VQCB)
		{
			strncpy(szMp4QualityType,"STRICT_VQCB",strlen("STRICT_VQCB"));
		}
		else
		{
			strncpy(szMp4QualityType,"NONE",strlen("NONE"));
		}

		if ((ptEncoderInfo->eVPMode & vpmStreamEncode) == vpmStreamEncH264)
		{
			strncpy(szCurrentCodec,"h264",strlen("h264"));
		}
		else if ((ptEncoderInfo->eVPMode & vpmStreamEncode) == vpmStreamEncSVC)
		{
			strncpy(szCurrentCodec,"svc",strlen("svc"));
		}
		else if ((ptEncoderInfo->eVPMode & vpmStreamEncode) == vpmStreamEncMP4V)
		{
			strncpy(szCurrentCodec,"mpeg4",strlen("mpeg4"));
		} 
		else if ((ptEncoderInfo->eVPMode & vpmStreamEncode) == vpmStreamEncJPEG)
		{
			strncpy(szCurrentCodec,"mjpeg",strlen("mjpeg"));
		}
		else
		{
			strncpy(szCurrentCodec,"unknown",strlen("unknown"));
		}

		fprintf(fpDump,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
				"<root>\n"
				"<quality>\n"
				"<current_codec>%s</current_codec>\n" 
				"<h264_quality_type>%s</h264_quality_type>\n"
				"<h264_quant_value>%d</h264_quant_value>\n"
				"<h264_bitrate>%d</h264_bitrate>\n"
				"<svc_quality_type>%s</svc_quality_type>\n"
				"<svc_quant_value>%d</svc_quant_value>\n"
				"<svc_bitrate>%d</svc_bitrate>\n"
				"<mpeg4_quality_type>%s</mpeg4_quality_type>\n"
				"<mpeg4_quant_value>%d</mpeg4_quant_value>\n"
				"<mpeg4_bitrate>%d</mpeg4_bitrate>\n"
				"<mjpeg_quant_value>%d</mjpeg_quant_value>\n"
				"</quality>\n"
				"</root>",
				szCurrentCodec,
				szH264QualityType,ptEncoderInfo->tH264OptRec.dwQuant,ptEncoderInfo->tH264OptRec.dwBitrate,
				szSVCQualityType,ptEncoderInfo->tSVCOptRec.dwQuant,ptEncoderInfo->tSVCOptRec.dwBitrate,
				szMp4QualityType,ptEncoderInfo->tMp4VOptRec.dwQuant,ptEncoderInfo->tMp4VOptRec.dwBitrate,
				ptEncoderInfo->tJPEGOptRec.dwQuant
					);

		if (fpDump != NULL)
		{
			fclose(fpDump);
			fpDump = NULL;
		}

		ptMsgOptRec->bDumpQualityParameter = FALSE;
	}
#if defined(_MD_TRIGER_ROI_)
	//printf("[venc_encoder_process](%d)ptMsgOptRec->bSetROI=%d ptEncoderInfo->dwVisitorCounter=%d\n",__LINE__,ptMsgOptRec->bSetROI,ptEncoderInfo->dwVisitorCounter);
	if (ptMsgOptRec->bSetROI == TRUE)
	{
		//[VENC_ENCODER_CONF](1596)[roi](win,en,sMBX,sMBY,eMBX,eMBY,dQp,encInter)=(0,1,0,0,29,16,-40,1)
		CHAR szReadBuf[SHARED_MEM_LEN];
		memset(szReadBuf,0x0,sizeof(szReadBuf));

		if (read_shm(szReadBuf,sizeof(szReadBuf)) == S_OK)
		{	
#if 0
			printf("[venc_encoder_process](%d)szReadBuf:\n",__LINE__);
			printf("%s\n",szReadBuf);
			printf("[venc_encoder_process](%d)End of szReadBuf!\n",__LINE__);
#endif	
			if ((ptEncoderInfo->eVPMode & vpmStreamEncode) == vpmStreamEncSVC) 
			{
				printf("[venc_encoder_process](%d)Do SVC ROI!!\n",__LINE__);

				{
#ifdef _SVC
					TSVCEncROIWindowsInfos tSVCEncROIWindowsInfos;
					TSVCEncOptions tEncOptions;
					memset(&tSVCEncROIWindowsInfos,0x0,sizeof(TSVCEncROIWindowsInfos));
					memset(&tEncOptions,0x0,sizeof(TSVCEncOptions));
					if (parse_roi_win_info(szReadBuf, &tSVCEncROIWindowsInfos) == S_OK)
					{
						tEncOptions.eOptionFlags = VIDEO_CHANGE_REGION_OF_INTEREST_WINDOWS_INFO; 
						tEncOptions.adwUserData[0] = (DWORD)&(tSVCEncROIWindowsInfos);
						if (ptEncoderInfo->hSVCEncObj != NULL)
						{
							if (SVCEnc_SetOptions(ptEncoderInfo->hSVCEncObj, &tEncOptions) != S_OK)
							{ 
								printf("[venc_encoder_process](%d)Call SVCEnc_SetOptions():VIDEO_CHANGE_REGION_OF_INTEREST_WINDOWS_INFO fail !!\n",__LINE__);
							}
						}
						printf("[venc_encoder_process](%d)[roi](win,en,sMBX,sMBY,eMBX,eMBY,dQp,encInter)=(%d,%d,%d,%d,%d,%d,%d,%d)\n",
								__LINE__,
								tSVCEncROIWindowsInfos.dwROIWindowNum,
								tSVCEncROIWindowsInfos.bROIEnable,
								tSVCEncROIWindowsInfos.dwStartMBXNum,
								tSVCEncROIWindowsInfos.dwStartMBYNum,
								tSVCEncROIWindowsInfos.dwEndMBXNum,
								tSVCEncROIWindowsInfos.dwEndMBYNum,
								tSVCEncROIWindowsInfos.sdwDeltaQp,
								tSVCEncROIWindowsInfos.dwEncodingInterval);
					}
#endif
				}
			} 
			else  if ((ptEncoderInfo->eVPMode & vpmStreamEncode) == vpmStreamEncH264)
			{
				printf("[venc_encoder_process](%d)Do H264 ROI!!\n",__LINE__);

				{
#ifdef _H4EE
					TH264EncROIWindowsInfos tH264EncROIWindowsInfos;
					TH264EncOptions tEncOptions;

					memset(&tH264EncROIWindowsInfos,0x0,sizeof(TH264EncROIWindowsInfos));
					memset(&tEncOptions,0x0,sizeof(TH264EncOptions));

					if (parse_roi_win_info(szReadBuf,&tH264EncROIWindowsInfos) == S_OK)
					{
						tEncOptions.eOptionFlags = VIDEO_CHANGE_REGION_OF_INTEREST_WINDOWS_INFO;
						tEncOptions.adwUserData[0] = (DWORD)&(tH264EncROIWindowsInfos);
						if (ptEncoderInfo->hH264EncObj != NULL)
						{
							if (H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tEncOptions) != S_OK)
							{
								printf("[venc_encoder_process](%d)Call H264Enc_SetOptions():VIDEO_CHANGE_REGION_OF_INTEREST_WINDOWS_INFO fail !!\n",__LINE__);
							}
						}
						printf("[venc_encoder_process](%d)[roi](win,en,sMBX,sMBY,eMBX,eMBY,dQp,encInter)=(%d,%d,%d,%d,%d,%d,%d,%d)\n",
								__LINE__,
								tH264EncROIWindowsInfos.dwROIWindowNum,
								tH264EncROIWindowsInfos.bROIEnable,
								tH264EncROIWindowsInfos.dwStartMBXNum,
								tH264EncROIWindowsInfos.dwStartMBYNum,
								tH264EncROIWindowsInfos.dwEndMBXNum,
								tH264EncROIWindowsInfos.dwEndMBYNum,
								tH264EncROIWindowsInfos.sdwDeltaQp,
								tH264EncROIWindowsInfos.dwEncodingInterval);
					}
#endif
				}
			}
			else
				printf("[venc_encoder_process](%d)Because running codec is not H264 or SVC, does not do ROI!!\n",__LINE__);

		}
		ptMsgOptRec->bSetROI = FALSE;
	}
#endif

#ifdef _JPEG_SNAPSHOT_
	if (ptMsgOptRec->bGetSnapshot == TRUE) {
		ptEncoderInfo->bGetSnapshot = TRUE;
		ptEncoderInfo->bDoEncode = TRUE;
		ptEncoderInfo->dwSnapshotCountdown = 2;
	}
	else
#endif // _JPEG_SNAPSHOT_
	{
		ptEncoderInfo->bDoEncode = (ptMsgOptRec->dwVisitors > 0);
	}

exit:	
	//memset(ptMsgOptRec, 0, sizeof(TEncoderMsgOptRec));
	return;	

}

/* =========================================================================================== */
void VencEncoder_ReSetEncoder(TEncoderInfo *ptEncoderInfo, DWORD dwStride)
{
	//    if ((ptEncoderInfo->eVPMode & vpmStreamEncode) == vpmStreamEncMP4V)
	{
        VencEncoder_Mp4VChangeFrameSize(ptEncoderInfo);
        VencEncoder_Mp4VChangeInFrameSize(ptEncoderInfo, dwStride);
	}
	//    else if ((ptEncoderInfo->eVPMode & vpmStreamEncode) == vpmStreamEncJPEG)
	{
		VencEncoder_JPEGChangeFrameSize(ptEncoderInfo);
		VencEncoder_JPEGChangeInFrameSize(ptEncoderInfo, dwStride);
	}

	{
#ifdef _VMA_JEBE
#ifdef _JPEG_SNAPSHOT_
		SCODE scRet;
		TJPEGEncOptions tJPEGEncOpt;

		memset(&tJPEGEncOpt, 0x0, sizeof(TJPEGEncOptions));
		tJPEGEncOpt.eOptionFlags = VIDEO_CHANGE_FRAME_SIZE;
		tJPEGEncOpt.adwUserData[0] = g_avfInfo[ptEncoderInfo->vfCurtFormat].wEncWidth;
		tJPEGEncOpt.adwUserData[1] = g_avfInfo[ptEncoderInfo->vfCurtFormat].wEncHeight;
		if (ptEncoderInfo->hJPEGEncSnapShot != NULL) {
			scRet = JPEGEnc_SetOptions(ptEncoderInfo->hJPEGEncSnapShot, &tJPEGEncOpt);
			assert(scRet == S_OK);
			printf("Change JPEG Encode size %u,%u\n", tJPEGEncOpt.adwUserData[0], tJPEGEncOpt.adwUserData[1]);
		}
		memset(&tJPEGEncOpt,0x0,sizeof(TJPEGEncOptions));
		tJPEGEncOpt.eOptionFlags = VIDEO_CHANGE_IN_FRAME_SIZE;
		tJPEGEncOpt.adwUserData[0] = dwStride;
		tJPEGEncOpt.adwUserData[1] = g_avfInfo[ptEncoderInfo->vfCurtFormat].wEncHeight;
		if (ptEncoderInfo->hJPEGEncSnapShot != NULL) {
			scRet = JPEGEnc_SetOptions(ptEncoderInfo->hJPEGEncSnapShot, &tJPEGEncOpt);
			assert(scRet == S_OK);
			printf("Change JPEG Input size %u,%u\n", tJPEGEncOpt.adwUserData[0], tJPEGEncOpt.adwUserData[1]);        
		}  
#endif // _JPEG_SNAPSHOT_
#endif // _VMA_JEBE
	}

	//	else if ((ptEncoderInfo->eVPMode & vpmStreamEncode) == vpmStreamEncH264)
	{
		VencEncoder_H264ChangeFrameSize(ptEncoderInfo);
		VencEncoder_H264ChangeInFrameSize(ptEncoderInfo, dwStride);
	}

	{
		VencEncoder_SVCChangeFrameSize(ptEncoderInfo);
		VencEncoder_SVCChangeInFrameSize(ptEncoderInfo, dwStride);
    }

}

/* ========================================================================== */
SCODE VencEncoder_MMASwitchBuffer(TEncoderInfo *ptEncoderInfo, TVideoFrameBuffer *ptInVFB, DWORD dwSrcStride, DWORD dwSrcHeight, TVideoFrameBuffer *ptOutVFB)
{
	TMassMemAccessState *ptMassMemAccState = NULL;

	DWORD dwDstYSize;
	DWORD dwDstUSize;
	
	static const DWORD dwMaxBurstLength = 256; // original one is 128.
#ifdef _1DRAM_VERSION_
	static const DWORD dwDMABus = 0;
#else
	static const DWORD dwDMABus = 2;
#endif
	DWORD dwPaddingShiftOffset = 0;
	

	if (ptEncoderInfo->bUseOneDMATextoverlay == FALSE)
	{
		dwPaddingShiftOffset = dwSrcStride * ((((dwSrcHeight + 15) >> 4) << 4) - dwSrcHeight);
	}

	
	if (ptEncoderInfo->bUseOneDMATextoverlay == TRUE)
	{
		//Because the width and height  of ptInVFB (the videoin output buffer) and ptOutVFB (the resize output buffer) is padding to 16.
		dwSrcStride = (((dwSrcStride + 15) >> 4) << 4);
		dwSrcHeight = (((dwSrcHeight + 15) >> 4) << 4);
	}
	
	dwDstYSize = dwSrcStride * dwSrcHeight;
	dwDstUSize = (dwDstYSize >> 2);
	
	if (ptEncoderInfo->bUseOneDMATextoverlay == TRUE)
	{
		ptOutVFB->pbyBufU = ptOutVFB->pbyBufY + dwDstYSize;
		ptOutVFB->pbyBufV = ptOutVFB->pbyBufY + dwDstYSize + dwDstUSize;

	}
	else 
	{
		ptOutVFB->pbyBufU = ptOutVFB->pbyBufY + dwDstYSize + dwPaddingShiftOffset;
		ptOutVFB->pbyBufV = ptOutVFB->pbyBufY + dwDstYSize + dwDstUSize + dwPaddingShiftOffset + (dwPaddingShiftOffset >> 2);
	}	

	// use 1D method
	// Y setting
	ptMassMemAccState = &(ptEncoderInfo->tMassMemAccStateY);	    
	ptMassMemAccState->b2DEnable = FALSE;
	ptMassMemAccState->bUsePhysAddr = FALSE;
	ptMassMemAccState->bFillingEnable = FALSE;
	ptMassMemAccState->dwSrcBusNum = dwDMABus;
	ptMassMemAccState->dwDstBusNum = dwDMABus;
	ptMassMemAccState->dwMaxBurstLength = dwMaxBurstLength;
	ptMassMemAccState->bMaskEnable = FALSE;
	ptMassMemAccState->pbySrcAddr = ptInVFB->pbyBufY;
	ptMassMemAccState->pbyDstAddr = ptOutVFB->pbyBufY;

	if (ptEncoderInfo->bUseOneDMATextoverlay == TRUE) {
	#ifdef _SINGLE_BUFFER_
		ptMassMemAccState->dwTransSize = (dwSrcStride*dwSrcHeight);  // if single buffer, it can transfer Y only.
	#else
		ptMassMemAccState->dwTransSize = ((dwDstYSize * 3) >> 1);
	#endif //_SINGLE_BUFFER_
	} else {
		ptMassMemAccState->dwTransSize = (dwSrcStride*dwSrcHeight);
	}
	
	ptMassMemAccState->dwSrcStride = 0;
	ptMassMemAccState->dwDstStride = 0;
	ptMassMemAccState->dwDstWidth =  0;
	ptMassMemAccState->dwDstHeight = 0;	
	
	if (ptEncoderInfo->bUseOneDMATextoverlay == FALSE) 
	{
		// Cb setting
		ptMassMemAccState = &(ptEncoderInfo->tMassMemAccStateCb);
		ptMassMemAccState->b2DEnable = FALSE;
		ptMassMemAccState->bUsePhysAddr = FALSE;
		ptMassMemAccState->bFillingEnable = FALSE;
		ptMassMemAccState->dwSrcBusNum = dwDMABus;
		ptMassMemAccState->dwDstBusNum = dwDMABus;
		ptMassMemAccState->dwMaxBurstLength = dwMaxBurstLength;
		ptMassMemAccState->bMaskEnable = FALSE;
		ptMassMemAccState->pbySrcAddr = ptInVFB->pbyBufU;
		ptMassMemAccState->pbyDstAddr = ptOutVFB->pbyBufU;
		ptMassMemAccState->dwTransSize = ((dwSrcStride >> 1)*(dwSrcHeight >> 1));
		ptMassMemAccState->dwSrcStride = 0;
		ptMassMemAccState->dwDstStride = 0;
		ptMassMemAccState->dwDstWidth = 0;
		ptMassMemAccState->dwDstHeight = 0;	

		// Cr setting
		ptMassMemAccState = &(ptEncoderInfo->tMassMemAccStateCr);
		ptMassMemAccState->b2DEnable = FALSE;
		ptMassMemAccState->bUsePhysAddr = FALSE;
		ptMassMemAccState->bFillingEnable = FALSE;
		ptMassMemAccState->dwSrcBusNum = dwDMABus;
		ptMassMemAccState->dwDstBusNum = dwDMABus;
		ptMassMemAccState->dwMaxBurstLength = dwMaxBurstLength;
		ptMassMemAccState->bMaskEnable = FALSE;
		ptMassMemAccState->pbySrcAddr = ptInVFB->pbyBufV;
		ptMassMemAccState->pbyDstAddr = ptOutVFB->pbyBufV;
		ptMassMemAccState->dwTransSize = ((dwSrcStride >> 1)*(dwSrcHeight >> 1));
		ptMassMemAccState->dwSrcStride = 0;
		ptMassMemAccState->dwDstStride = 0;
		ptMassMemAccState->dwDstWidth = 0;
		ptMassMemAccState->dwDstHeight = 0;	        
	}
	ptOutVFB->dwStride = dwSrcStride;
	return S_OK;
}

/* ========================================================================== */
#define MAX_STREAM_MUM 5
static pthread_mutex_t TrackStateMutex = PTHREAD_MUTEX_INITIALIZER;
static BOOL* TrackDoNotDMACopy[MAX_STREAM_MUM] = { NULL };
static int TotalNoResizeStream = 0;
void* VencEncoder_Loop(HANDLE hInstance)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)hInstance;
	TSharedBuffMgrState *ptSharedBuffMgrState = NULL;
	TVideoFrameBuffer tVFB;
	TVideoFrameBuffer tCachedVFB;
	TVideoFrameBuffer *ptVFB = NULL;
	TVideoFrameBuffer *ptCachedVFB = NULL;
	TVideoFrameBuffer *ptEncodeVFB = NULL;
	EVideoProcessMode eVPMode;
	EVideoFormat vFormat;       // recorder the enc format to check if changed
	EVideoFormat vCapFormat;    // recorder the cap format to check if changed
	struct timeval pretv, tv;
	static DWORD dwThreadCount = 0;
	BOOL bCheckDMACopy = TRUE;
	BOOL bDoNotDMACopy = TRUE;

	DWORD       dwReqWidth = 0;
	DWORD       dwReqHeight = 0;
	vFormat = vfNum;
	vCapFormat = vfNum;

	tCachedVFB.eBufStatus = evsobsEmpty;

	syslog(LOG_INFO, "[VENC_ENCODER] venc_encoder(%d)jeff thread pid: %d\n",(dwThreadCount++), getpid());
	// or using default (join)? think about it
	pthread_detach(pthread_self());

	SharedBuffMgr_GetFileDescriptor(ptEncoderInfo->hVideoInSrdObj, &(ptEncoderInfo->iVideoInSrdFD));

	while (1) {
		sem_wait(&(ptEncoderInfo->smStart));
		if (ptEncoderInfo->bTerminateThread == TRUE)     break;

		DBPRINT0("[VENC_ENCODER] Encoder Loop Start\n");

		while (ptEncoderInfo->bRunning == TRUE) {
			eVPMode = ptEncoderInfo->eVPMode;

			//bReConf_flag should set to true when first running
			if (ptEncoderInfo->bReConf_flag == TRUE) {
				sem_wait(&(ptEncoderInfo->smOptions));
				VencEncoder_LoadConfig(hInstance);
				ptEncoderInfo->bReConf_flag = FALSE;
				sem_post(&(ptEncoderInfo->smOptions));
			}

			if (ptEncoderInfo->bReMsg_flag == TRUE) {
				sem_wait(&(ptEncoderInfo->smMsgOptions));
				VencEncoder_MsgApplyOpt(ptEncoderInfo);
				ptEncoderInfo->bReMsg_flag = FALSE;
				sem_post(&(ptEncoderInfo->smMsgOptions));
			}

			if (ptEncoderInfo->bFRCalculate == TRUE) {
				if (ptEncoderInfo->dwFRCalculateFrameCount == 0) {
					gettimeofday(&pretv, NULL);
				}
			}

			if (ptEncoderInfo->bVPTZCmd == TRUE) {
				TEptzOpts tOpts;

				ptEncoderInfo->bVPTZCmd = FALSE;
				ptEncoderInfo->bVPTZReset = TRUE;

				tOpts.eEptzType = eoSetPTZ;
				tOpts.adwUserData[0] = (DWORD)&ptEncoderInfo->tPTZValue.flPanValue;
				tOpts.adwUserData[1] = (DWORD)&ptEncoderInfo->tPTZValue.flTiltValue;
				tOpts.adwUserData[2] = (DWORD)&ptEncoderInfo->tPTZValue.flZoomValue;
				eptz_options(ptEncoderInfo->hEPTZObj, &tOpts);
				if (ptEncoderInfo->bVPTZSpeedUpdate == TRUE) {
					ptEncoderInfo->bVPTZSpeedUpdate = FALSE;
					tOpts.eEptzType = eoSetSpeed;
					tOpts.adwUserData[0] = esSetSmooth;
					tOpts.adwUserData[1] = (DWORD)&ptEncoderInfo->tPTZValue.flSpeedValue;
					eptz_options(ptEncoderInfo->hEPTZObj, &tOpts);
				}
			}

			if (ptEncoderInfo->bOptionsModified == TRUE) {
				//                sem_wait(&(ptEncoderInfo->smOptions));
				ptEncoderInfo->bOptionsModified = FALSE;
				VencEncoder_PreSetApplyOpt(ptEncoderInfo);
				eVPMode = ptEncoderInfo->eVPMode;

				if (ptEncoderInfo->bPreNeedTextOverlayState != ptEncoderInfo->tTextOverlayData.bTextOverlayEnable) {
					ptEncoderInfo->bNeedTextOverlayChange = TRUE;
					ptEncoderInfo->bPreNeedTextOverlayState = ptEncoderInfo->tTextOverlayData.bTextOverlayEnable;
				}

				// check mpeg4 options
				if ((eVPMode & vpmStreamEncode) == vpmStreamEncMP4V) {
					if (ptEncoderInfo->bMp4VOptionsModified == TRUE) {
						ptEncoderInfo->bMp4VOptionsModified = FALSE;
						VencEncoder_Mp4VApplyOpt(ptEncoderInfo);
					}
				}
                // check mjpeg options
				else if ((eVPMode & vpmStreamEncode) == vpmStreamEncJPEG) {
					if (ptEncoderInfo->bJPEGOptionsModified == TRUE) {
						ptEncoderInfo->bJPEGOptionsModified = FALSE;
						VencEncoder_JPEGApplyOpt(ptEncoderInfo);
						VencEncoder_JPEGForceCI(ptEncoderInfo, vFormat);
					}
				}
				// check H264 options
				else if ((eVPMode & vpmStreamEncode) == vpmStreamEncH264) {
					if (ptEncoderInfo->bH264OptionsModified == TRUE) {
						ptEncoderInfo->bH264OptionsModified = FALSE;
						VencEncoder_H264ApplyOpt(ptEncoderInfo);
					}
				}
				// check SVC options
				else if ((eVPMode & vpmStreamEncode) == vpmStreamEncSVC) {
					if (ptEncoderInfo->bSVCOptionsModified == TRUE) {
						ptEncoderInfo->bSVCOptionsModified = FALSE;
						VencEncoder_SVCApplyOpt(ptEncoderInfo);
					}
				}
			}
			// request input you frame from share buffer
			ptSharedBuffMgrState = VencEncoder_RequestInputBuf(ptEncoderInfo);
			if (!ptSharedBuffMgrState) {
				sched_yield();
				continue;
			}

			// no client, just release sharedbuffer, but if it need conf, it shouldn't release buffer directly.
			if (!ptEncoderInfo->bDoEncode) {	
				if (!bCheckDMACopy)
				{
					pthread_mutex_lock(&TrackStateMutex);
					TrackDoNotDMACopy[ptEncoderInfo->dwCurId] = NULL;
					--TotalNoResizeStream;
					if (TotalNoResizeStream <= 1)
					{
						int idx = 0;
						for (idx = 0; idx < MAX_STREAM_MUM; ++idx)
						{
							if(TrackDoNotDMACopy[idx] != NULL)
								*(TrackDoNotDMACopy[idx]) = TRUE;
						}
					}
					bDoNotDMACopy = ptEncoderInfo->bDoNotDMACopy = TRUE;
					pthread_mutex_unlock(&TrackStateMutex);
					bCheckDMACopy = TRUE;
				}

				if ((ptEncoderInfo->bForceCI == FALSE) && (ptEncoderInfo->bCodecChange == FALSE)) {
					VencEncoder_ReleaseInputBuf(ptEncoderInfo->hVideoInSrdObj, ptSharedBuffMgrState);
					ptSharedBuffMgrState = NULL;
					continue;
				}
			}

			//if (((CurWrittingBufIDX - ((int)ptSharedBuffMgrState->dwIndex)) > 1) && (CurWrittingBufIDX != 0))
			{
				int iDiff = (int)(CurWrittingBufIDX - ((int)ptSharedBuffMgrState->dwIndex));
				if ((abs(iDiff) > 1) && (abs(iDiff) != (iWrittingBufNum -1)))	
				{
					VencEncoder_ReleaseInputBuf(ptEncoderInfo->hVideoInSrdObj, ptSharedBuffMgrState);
					ptSharedBuffMgrState = NULL;
					tCachedVFB.eBufStatus = evsobsEmpty;
					continue;
				}
			}

			// it must copy 'cause of if setting cropping, pbyBufY(U)(V) will be changed, but it's shared data.
			memcpy(&tVFB, (TVideoFrameBuffer *)(ptSharedBuffMgrState->pbyHdrAddr), sizeof(TVideoFrameBuffer));
			ptVFB = &tVFB;

#ifdef VIRTUAL_PTZ
			if (ptEncoderInfo->bVPTZReset == TRUE) {
				ptEncoderInfo->bNeed2Recalculate = TRUE;
				if (ptEncoderInfo->bChangeRandomCropping == TRUE) {
					ptEncoderInfo->bNeed2Recalculate = TRUE;
					ptEncoderInfo->bChangeRandomCropping = FALSE;
				}
			}		
#endif //VIRTUAL_PTZ

			if (ptEncoderInfo->bNeed2Recalculate == TRUE) {
				VencEncoder_CalculateSize(ptEncoderInfo);
				ptEncoderInfo->bNeed2Recalculate = FALSE;
			}

			// before apply eptz, we need to check if need to add offset by cropping
			// 1. multiple dwStride or dwCurtCapWidth?
			// 2. Is U/V offset the half of Y offset?
			// if the result isn't correct, maybe it's the root cause.
			if (ptEncoderInfo->ecMethod == ecmCropping) {
				DWORD   dwYOffset, dwUOffset;
				dwYOffset = (((ptEncoderInfo->dwCurtCapHeight - ptEncoderInfo->dwPTZFullViewHeight) >> 1) * ptVFB->dwStride) + 
					((ptEncoderInfo->dwCurtCapWidth - ptEncoderInfo->dwPTZFullViewWidth) >> 1);
				dwUOffset = ((((ptEncoderInfo->dwCurtCapHeight >> 1) - (ptEncoderInfo->dwPTZFullViewHeight >> 1)) >> 1) * (ptVFB->dwStride >> 1)) +
					(((ptEncoderInfo->dwCurtCapWidth >> 1) - (ptEncoderInfo->dwPTZFullViewWidth >> 1)) >> 1);
				{
					DWORD dwXForYBuf = 0;
					DWORD dwYForYBuf = 0;
					DWORD dwXForUBuf = 0;
					DWORD dwYForUBuf = 0;
					DWORD dwXForVBuf = 0;
					DWORD dwYForVBuf = 0;
					//dwXForYBuf = dwYOffset % ptVFB->dwStride;
					//dwYForYBuf = dwYOffset / ptVFB->dwStride;
					dwXForUBuf = dwUOffset % (ptVFB->dwStride/2);
					dwYForUBuf = dwUOffset / (ptVFB->dwStride/2);
					dwXForVBuf = dwUOffset % (ptVFB->dwStride/2);
					dwYForVBuf = dwUOffset / (ptVFB->dwStride/2);
					//printf("================================\n");
					//printf(" location = Y:(%d, %d) U:(%d, %d) V:(%d, %d)\n",dwXForYBuf, dwYForYBuf, dwXForUBuf, dwYForUBuf,dwXForVBuf, dwYForVBuf);
					//align 8
					dwXForUBuf =  (((dwXForUBuf+ 7)) >> 3) << 3; 
					dwYForUBuf =  (((dwYForUBuf+ 7)) >> 3) << 3;
					//align 8
					dwXForVBuf =  (((dwXForVBuf+ 7)) >> 3) << 3;
					dwYForVBuf =  (((dwYForVBuf+ 7)) >> 3) << 3;
					//The (x,y) in Y  plane is twice as many as (x,y) in U, V plane
					dwXForYBuf = dwXForUBuf *2;
					dwYForYBuf = dwYForUBuf *2;
					//printf("  location = Y:(%d, %d) U:(%d, %d) V:(%d, %d)\n",dwXForYBuf, dwYForYBuf, dwXForUBuf, dwYForUBuf,dwXForVBuf, dwYForVBuf);
					ptVFB->pbyBufY = ptVFB->pbyBufY + dwYForYBuf*ptVFB->dwStride + dwXForYBuf;
					ptVFB->pbyBufU = ptVFB->pbyBufU + dwYForUBuf*(ptVFB->dwStride/2) + dwXForUBuf;
					ptVFB->pbyBufV = ptVFB->pbyBufV  +  dwYForVBuf*(ptVFB->dwStride/2) + dwXForVBuf;
				}
				//printf("(dwYOffset, dwUOffset) = (%d, %d)\n", dwYOffset, dwUOffset);
				if ((dwYOffset != 0 ) || (dwUOffset != 0 ))
				{
					ptEncoderInfo->bUseOneDMATextoverlay = FALSE;
				}
				else
				{
					ptEncoderInfo->bUseOneDMATextoverlay = TRUE; 
				}
				
				ptVFB->pbyBufY = (BYTE *) ((( (DWORD)ptVFB->pbyBufY + 7) >>3) <<3);
				ptVFB->pbyBufU = (BYTE *) ((( (DWORD)ptVFB->pbyBufU + 7) >>3) <<3);
				ptVFB->pbyBufV = (BYTE *) ((( (DWORD)ptVFB->pbyBufV + 7) >>3) <<3);
				
			} else if (ptEncoderInfo->ecMethod == ecmRandomCropping) {
				DWORD   dwYOffset, dwUOffset;
				dwYOffset = (ptEncoderInfo->tCropInfo.dwStartY * ptVFB->dwStride) +  ptEncoderInfo->tCropInfo.dwStartX;
				dwUOffset = ((ptEncoderInfo->tCropInfo.dwStartY >> 1) * (ptVFB->dwStride >> 1)) +  (ptEncoderInfo->tCropInfo.dwStartX >> 1);
				
				{
					DWORD dwXForYBuf = 0;
					DWORD dwYForYBuf = 0;
					DWORD dwXForUBuf = 0;
					DWORD dwYForUBuf = 0;
					DWORD dwXForVBuf = 0;
					DWORD dwYForVBuf = 0;
					//dwXForYBuf = dwYOffset % ptVFB->dwStride;
					//dwYForYBuf = dwYOffset / ptVFB->dwStride;
					dwXForUBuf = dwUOffset % (ptVFB->dwStride/2);
					dwYForUBuf = dwUOffset / (ptVFB->dwStride/2);
					dwXForVBuf = dwUOffset % (ptVFB->dwStride/2);
					dwYForVBuf = dwUOffset / (ptVFB->dwStride/2);
					//printf("================================\n");
					//printf(" location = Y:(%d, %d) U:(%d, %d) V:(%d, %d)\n",dwXForYBuf, dwYForYBuf, dwXForUBuf, dwYForUBuf,dwXForVBuf, dwYForVBuf);
					//align 8
					dwXForUBuf =  (((dwXForUBuf+ 7)) >> 3) << 3; 
					dwYForUBuf =  (((dwYForUBuf+ 7)) >> 3) << 3;
					//align 8
					dwXForVBuf =  (((dwXForVBuf+ 7)) >> 3) << 3;
					dwYForVBuf =  (((dwYForVBuf+ 7)) >> 3) << 3;
					//The (x,y) in Y  plane is twice as many as (x,y) in U, V plane
					dwXForYBuf = dwXForUBuf *2;
					dwYForYBuf = dwYForUBuf *2;
					//printf("  location = Y:(%d, %d) U:(%d, %d) V:(%d, %d)\n",dwXForYBuf, dwYForYBuf, dwXForUBuf, dwYForUBuf,dwXForVBuf, dwYForVBuf);
					ptVFB->pbyBufY = ptVFB->pbyBufY + dwYForYBuf*ptVFB->dwStride + dwXForYBuf;
					ptVFB->pbyBufU = ptVFB->pbyBufU + dwYForUBuf*(ptVFB->dwStride/2) + dwXForUBuf;
					ptVFB->pbyBufV = ptVFB->pbyBufV  +  dwYForVBuf*(ptVFB->dwStride/2) + dwXForVBuf;
				}
				
				if ((dwYOffset != 0 ) || (dwUOffset != 0 ))
				{
					ptEncoderInfo->bUseOneDMATextoverlay = FALSE;
				}
				else
				{
					ptEncoderInfo->bUseOneDMATextoverlay = TRUE; 
				}
				
				ptVFB->pbyBufY = (BYTE *) ((( (DWORD)ptVFB->pbyBufY + 7) >>3) <<3);
				ptVFB->pbyBufU = (BYTE *) ((( (DWORD)ptVFB->pbyBufU + 7) >>3) <<3);
				ptVFB->pbyBufV = (BYTE *) ((( (DWORD)ptVFB->pbyBufV + 7) >>3) <<3);
				
			} // else: squeezing mode.

			// apply eptz
			{
				TEptzState  tEptzState;
				memset(&tEptzState, 0, sizeof(TEptzState));
				tEptzState.dwVideoBufStride     = ptVFB->dwStride;
				tEptzState.dwFullViewWidth      = ptEncoderInfo->dwPTZFullViewWidth;
				tEptzState.dwFullViewHeight     = ptEncoderInfo->dwPTZFullViewHeight;
				tEptzState.pbyFullViewCmpt1     = ptVFB->pbyBufY;
				tEptzState.pbyFullViewCmpt2     = ptVFB->pbyBufU;
				tEptzState.pbyFullViewCmpt3     = ptVFB->pbyBufV;

				eptz_oneframe(ptEncoderInfo->hEPTZObj, &tEptzState);
#if 1
				tEptzState.dwCloseUpViewWidth  = ((( (DWORD)tEptzState.dwCloseUpViewWidth + 7) >>3) <<3);
				tEptzState.dwCloseUpViewHeight = ((( (DWORD)tEptzState.dwCloseUpViewHeight + 7) >>3) <<3);			
#endif			
				ptEncoderInfo->dwCropWidth       = tEptzState.dwCloseUpViewWidth;
				ptEncoderInfo->dwCropHeight      = tEptzState.dwCloseUpViewHeight;
				
				ptVFB->pbyBufY = tEptzState.pbyCloseUpViewCmpt1;
				ptVFB->pbyBufU = tEptzState.pbyCloseUpViewCmpt2;
				ptVFB->pbyBufV = tEptzState.pbyCloseUpViewCmpt3;
#if 0
				printf("=======================================\n");
				printf("FullView = (%d, %d)\n", ptEncoderInfo->dwPTZFullViewWidth, ptEncoderInfo->dwPTZFullViewHeight);
				printf("ClosView = (%d, %d)\n", tEptzState.dwCloseUpViewWidth, tEptzState.dwCloseUpViewHeight);
				printf("    Full = (%p, %p, %p)\n", tEptzState.pbyFullViewCmpt1, tEptzState.pbyFullViewCmpt2, tEptzState.pbyFullViewCmpt3);
				printf("   Full alignment = (%d, %d, %d)\n", ((DWORD)tEptzState.pbyFullViewCmpt1 % 16), ((DWORD)tEptzState.pbyFullViewCmpt2 % 16), ((DWORD)tEptzState.pbyFullViewCmpt3 % 16));
				printf("   Close = (%p, %p, %p)\n", ptVFB->pbyBufY, ptVFB->pbyBufU, ptVFB->pbyBufV);
				printf("   Close alignment = (%d, %d, %d)\n", ((DWORD)ptVFB->pbyBufY % 8), ((DWORD)ptVFB->pbyBufU % 8), ((DWORD)ptVFB->pbyBufV % 8));
				printf("	PTZ = (p:%f, t:%f, z:%f, s:%f)\n",ptEncoderInfo->tPTZValue.flPanValue,ptEncoderInfo->tPTZValue.flTiltValue, ptEncoderInfo->tPTZValue.flZoomValue, ptEncoderInfo->tPTZValue.flSpeedValue);
				//ptVFB->pbyBufY = (BYTE *) ((( (DWORD)tEptzState.pbyCloseUpViewCmpt1 + 7) >>3) <<3);
				//ptVFB->pbyBufU = (BYTE *) ((( (DWORD)tEptzState.pbyCloseUpViewCmpt2 + 7) >>3) <<3);
				//ptVFB->pbyBufV = (BYTE *) ((( (DWORD)tEptzState.pbyCloseUpViewCmpt3 + 7) >>3) <<3);
#endif
		
				
#if 1
				{
					DWORD dwXForYBuf = 0;
					DWORD dwYForYBuf = 0;
					DWORD dwXForUBuf = 0;
					DWORD dwYForUBuf = 0;
					DWORD dwXForVBuf = 0;
					DWORD dwYForVBuf = 0;
					//dwXForYBuf = ((DWORD)((DWORD)tEptzState.pbyCloseUpViewCmpt1- (DWORD)tEptzState.pbyFullViewCmpt1)) % ptVFB->dwStride;
					//dwYForYBuf = ((DWORD)((DWORD)tEptzState.pbyCloseUpViewCmpt1- (DWORD)tEptzState.pbyFullViewCmpt1)) / ptVFB->dwStride;
					dwXForUBuf = ((DWORD)((DWORD)tEptzState.pbyCloseUpViewCmpt2- (DWORD)tEptzState.pbyFullViewCmpt2)) % (ptVFB->dwStride/2);
					dwYForUBuf = ((DWORD)((DWORD)tEptzState.pbyCloseUpViewCmpt2- (DWORD)tEptzState.pbyFullViewCmpt2)) / (ptVFB->dwStride/2);
					dwXForVBuf = ((DWORD)((DWORD)tEptzState.pbyCloseUpViewCmpt3- (DWORD)tEptzState.pbyFullViewCmpt3)) % (ptVFB->dwStride/2);
					dwYForVBuf = ((DWORD)((DWORD)tEptzState.pbyCloseUpViewCmpt3- (DWORD)tEptzState.pbyFullViewCmpt3)) / (ptVFB->dwStride/2);
					//printf(" location = Y:(%d, %d) U:(%d, %d) V:(%d, %d)\n",dwXForYBuf, dwYForYBuf, dwXForUBuf, dwYForUBuf,dwXForVBuf, dwYForVBuf);
					//dwXForYBuf =  (((dwXForYBuf+ 7)) >> 3) << 3;
					//dwYForYBuf =  (((dwYForYBuf+ 7)) >> 3) << 3;
					//align 8
					dwXForUBuf =  (((dwXForUBuf+ 7)) >> 3) << 3; 
					dwYForUBuf =  (((dwYForUBuf+ 7)) >> 3) << 3;
					//align 8
					dwXForVBuf =  (((dwXForVBuf+ 7)) >> 3) << 3;
					dwYForVBuf =  (((dwYForVBuf+ 7)) >> 3) << 3;
					//The (x,y) in Y  plane is twice as many as (x,y) in U, V plane
					dwXForYBuf = dwXForUBuf *2;
					dwYForYBuf = dwYForUBuf *2;
					//printf("  location = Y:(%d, %d) U:(%d, %d) V:(%d, %d)\n",dwXForYBuf, dwYForYBuf, dwXForUBuf, dwYForUBuf,dwXForVBuf, dwYForVBuf);
					ptVFB->pbyBufY = tEptzState.pbyFullViewCmpt1 + dwYForYBuf*ptVFB->dwStride + dwXForYBuf;
					ptVFB->pbyBufU = tEptzState.pbyFullViewCmpt2 + dwYForUBuf*(ptVFB->dwStride/2) + dwXForUBuf;
					ptVFB->pbyBufV = tEptzState.pbyFullViewCmpt3 +  dwYForVBuf*(ptVFB->dwStride/2) + dwXForVBuf;
				}
#endif
				if ((ptEncoderInfo->dwCropWidth != ptEncoderInfo->dwReqWidth) ||
						(ptEncoderInfo->dwCropHeight != ptEncoderInfo->dwReqHeight)) {
					ptEncoderInfo->bNeedResize = TRUE;

					ptEncoderInfo->dwReqWidthPad = ((ptEncoderInfo->dwReqWidth + 15) >> 4) << 4;
					ptEncoderInfo->dwReqHeightPad = ((ptEncoderInfo->dwReqHeight + 15) >> 4) << 4;

					// if ReqWidthPad is N*128+16 or N*128+32, it should set to N*128+48 - hardware limitation
					// add at 2010/03/04
					if (((ptEncoderInfo->dwReqWidthPad - 16) % 128) == 0) {
						ptEncoderInfo->dwReqWidthPad += 32;
					} else if (((ptEncoderInfo->dwReqWidthPad - 32) % 128) == 0) {
						ptEncoderInfo->dwReqWidthPad += 16;
					}
				} else {
					ptEncoderInfo->bNeedResize = FALSE;
				}

				if (ptEncoderInfo->bPreNeedResizeState != ptEncoderInfo->bNeedResize) {
					ptEncoderInfo->bNeedResizeChange = TRUE;
					ptEncoderInfo->bPreNeedResizeState = ptEncoderInfo->bNeedResize;
				}

#if 0
				printf("Crop = (%d, %d)\n", ptEncoderInfo->dwCropWidth, ptEncoderInfo->dwCropHeight);
				printf("ReqWidth = (%d, %d)\n", ptEncoderInfo->dwReqWidth, ptEncoderInfo->dwReqHeight);
				printf("bNeedResize = %s\n", (ptEncoderInfo->bNeedResize == TRUE)? "TRUE": "FALSE");
#endif
			}
			
#ifdef  _VMA_IRE		  
			// buffer switch
			if (ptEncoderInfo->bNeedResize == TRUE) {
				ptCachedVFB = (ptEncoderInfo->dwSwitchIndex == 0x0)? &(ptEncoderInfo->tResizeOutBuf0) : &(ptEncoderInfo->tResizeOutBuf1);
				VencEncoder_ResizeSwitchBuffer(ptEncoderInfo, ptVFB, ptCachedVFB);
#ifdef _SINGLE_BUFFER_
				Resize_ProcessOneFrame(ptEncoderInfo->hResizeObj, &(ptEncoderInfo->tResizeState));
				ptCachedVFB->eBufStatus = evsobsFinished;
				ptCachedVFB->dwSecond = ptVFB->dwSecond;
				ptCachedVFB->dwMilliSecond = ptVFB->dwMilliSecond;
				ptCachedVFB->dwMicroSecond = ptVFB->dwMicroSecond;
#else
				Resize_StartOneFrame(ptEncoderInfo->hResizeObj, &(ptEncoderInfo->tResizeState));
				ptCachedVFB->eBufStatus = evsobsEmpty;
#endif // _SINGLE_BUFFER_
			}
			else if (ptEncoderInfo->tTextOverlayData.bTextOverlayEnable)
#else
			if (!ptEncoderInfo->bNeedResize && ptEncoderInfo->tTextOverlayData.bTextOverlayEnable)
#endif
			{

				if (bCheckDMACopy)
				{
					pthread_mutex_lock(&TrackStateMutex);
					TrackDoNotDMACopy[ptEncoderInfo->dwCurId] = &(ptEncoderInfo->bDoNotDMACopy);
					++TotalNoResizeStream;
					if (TotalNoResizeStream > 1)
					{
						int idx = 0;
						for (idx = 0; idx < MAX_STREAM_MUM; ++idx)
						{
							if(TrackDoNotDMACopy[idx] != NULL)
								*(TrackDoNotDMACopy[idx]) = FALSE;
						}
					}
					pthread_mutex_unlock(&TrackStateMutex);
					bCheckDMACopy = FALSE;
				}

				//copy value to prevent concurrent issue.
				bDoNotDMACopy = ptEncoderInfo->bDoNotDMACopy;

				if (bDoNotDMACopy)
				{
					ptCachedVFB = &tCachedVFB;
				}
				else
				{
					ptCachedVFB = (ptEncoderInfo->dwSwitchIndex == 0x0)? &(ptEncoderInfo->tResizeOutBuf0) : &(ptEncoderInfo->tResizeOutBuf1);
					VencEncoder_MMASwitchBuffer(ptEncoderInfo,
							ptVFB, 
							ptEncoderInfo->dwCurtCapWidth,
							ptEncoderInfo->dwCurtCapHeight,
							ptCachedVFB);
#ifdef _SINGLE_BUFFER_
					MassMemAccess_ProcessOneFrame(ptEncoderInfo->hMassMemAccObjY, &ptEncoderInfo->tMassMemAccStateY);
					if (ptEncoderInfo->bUseOneDMATextoverlay == FALSE) {
						MassMemAccess_ProcessOneFrame(ptEncoderInfo->hMassMemAccObjCb, &ptEncoderInfo->tMassMemAccStateCb);
						MassMemAccess_ProcessOneFrame(ptEncoderInfo->hMassMemAccObjCr, &ptEncoderInfo->tMassMemAccStateCr);
					}
					ptCachedVFB->eBufStatus = evsobsFinished;
					ptCachedVFB->dwSecond = ptVFB->dwSecond;
					ptCachedVFB->dwMilliSecond = ptVFB->dwMilliSecond;
					ptCachedVFB->dwMicroSecond = ptVFB->dwMicroSecond;
#else
					MassMemAccess_StartOneFrame(ptEncoderInfo->hMassMemAccObjY, &ptEncoderInfo->tMassMemAccStateY);
					if (ptEncoderInfo->bUseOneDMATextoverlay == FALSE) {
						MassMemAccess_StartOneFrame(ptEncoderInfo->hMassMemAccObjCb, &ptEncoderInfo->tMassMemAccStateCb);
						MassMemAccess_StartOneFrame(ptEncoderInfo->hMassMemAccObjCr, &ptEncoderInfo->tMassMemAccStateCr);
					}
					ptCachedVFB->eBufStatus = evsobsEmpty;
#endif // _SINGLE_BUFFER_
				}
			}
			else
				ptCachedVFB = NULL;

			{	// according to the new status, call the relative setoption function.
				// a-1. resolution change, so, it should reset Codec FRAME_SIZE
				if ((vFormat != ptEncoderInfo->vfCurtFormat) || 
						(vCapFormat != ptVFB->vFormat) || 
						(ptEncoderInfo->dwReqWidth != dwReqWidth) || 
						(ptEncoderInfo->dwReqHeight != dwReqHeight) ||
						(ptEncoderInfo->bNeedResizeChange == TRUE) ||
						(ptEncoderInfo->bNeedTextOverlayChange == TRUE)) {
					ptEncoderInfo->bNeedResizeChange = FALSE;
					ptEncoderInfo->bNeedTextOverlayChange = FALSE;
					if (ptEncoderInfo->bNeedResize) {
						VencEncoder_ReSetEncoder(ptEncoderInfo, ptCachedVFB->dwStride);
					} else {
						VencEncoder_ReSetEncoder(ptEncoderInfo, ptVFB->dwStride);
					}

					dwReqHeight = ptEncoderInfo->dwReqHeight;
					dwReqWidth = ptEncoderInfo->dwReqWidth;
					vFormat = ptEncoderInfo->vfCurtFormat;
					vCapFormat = ptVFB->vFormat;

					if ((eVPMode & vpmStreamEncode) == vpmStreamEncMP4V) {   
						if ((g_avfInfo[vFormat].wEncWidth * g_avfInfo[vFormat].wEncHeight) >= (320 * 240)) {               
                            VencEncoder_Mp4VChangeEncodeMode(ptEncoderInfo, 1, 0, 0);
						} else {
                            VencEncoder_Mp4VChangeEncodeMode(ptEncoderInfo, 0, 0, 0);
						}
						VencEncoder_Mp4VForceCI(ptEncoderInfo, vFormat);
					}
					else if ((eVPMode & vpmStreamEncode) == vpmStreamEncJPEG) {
						VencEncoder_JPEGForceCI(ptEncoderInfo, vFormat);
					}
                    else if ((eVPMode & vpmStreamEncode) == vpmStreamEncH264) {
						VencEncoder_H264ForceCI(ptEncoderInfo, vFormat);
					}
                    else if ((eVPMode & vpmStreamEncode) == vpmStreamEncSVC) {
						VencEncoder_SVCForceCI(ptEncoderInfo, vFormat);
					}
					printf("Video Format changed\n");  
				}
#ifdef VIRTUAL_PTZ
				// a-2. VPTZ reset
				if (ptEncoderInfo->bVPTZReset == TRUE) {
					if (ptEncoderInfo->bNeedResize) {
						VencEncoder_ReSetEncoder(ptEncoderInfo, ptCachedVFB->dwStride);
					} else {
						VencEncoder_ReSetEncoder(ptEncoderInfo, ptVFB->dwStride);
					}
					ptEncoderInfo->bVPTZReset = FALSE;
				}
#endif //VIRTUAL_PTZ
				// a-3. it need Conf
				// a-4. Codec change, it should send the new conf.
				if (ptEncoderInfo->bForceCI == TRUE ||
						ptEncoderInfo->bCodecChange == TRUE) {
					if ((eVPMode & vpmStreamEncode) == vpmStreamEncJPEG) {
						VencEncoder_JPEGForceCI(ptEncoderInfo, vFormat);
						VencEncoder_JPEGForceCI(ptEncoderInfo, vFormat);
					}
					else if ((eVPMode & vpmStreamEncode) == vpmStreamEncMP4V) {
						VencEncoder_Mp4VForceCI(ptEncoderInfo, vFormat);
						VencEncoder_Mp4VForceCI(ptEncoderInfo, vFormat);
					}
					else if ((eVPMode & vpmStreamEncode) == vpmStreamEncH264) {
						VencEncoder_H264ForceCI(ptEncoderInfo, vFormat);
						VencEncoder_H264ForceCI(ptEncoderInfo, vFormat);
					}
					else if ((eVPMode & vpmStreamEncode) == vpmStreamEncSVC) {
						VencEncoder_SVCForceCI(ptEncoderInfo, vFormat);
						VencEncoder_SVCForceCI(ptEncoderInfo, vFormat);
					}
					ptEncoderInfo->bForceCI = FALSE;
					ptEncoderInfo->bCodecChange = FALSE;
				}
			}

			// one frame
			{
				// encode
				if ((eVPMode & vpmStreamEncode) != 0) {
					if (ptCachedVFB)
					{
#ifndef _SINGLE_BUFFER_
						if (ptEncoderInfo->bNeedResize || !bDoNotDMACopy)
							ptEncodeVFB = (ptEncoderInfo->dwSwitchIndex == 0x0)? &(ptEncoderInfo->tResizeOutBuf1) : &(ptEncoderInfo->tResizeOutBuf0);
						else
#endif
							ptEncodeVFB = ptCachedVFB;

						if (ptEncodeVFB->eBufStatus != evsobsEmpty)
						{
#ifdef _SINGLE_BUFFER_
							if (!bDoNotDMACopy)
							{
								ptEncodeVFB->pbyBufU = ptVFB->pbyBufU;
								ptEncodeVFB->pbyBufV = ptVFB->pbyBufV;
							}
#endif

							if (ptEncoderInfo->tTextOverlayData.bTextOverlayEnable)
								VencSlave_TextOverlay(ptEncodeVFB, &ptEncoderInfo->tTextOverlayData, ptEncoderInfo->_tInitNeedInfo.dwVideoMaxWidth);

							VencEncoder_Encode(ptEncoderInfo, eVPMode, ptEncodeVFB);
						}
					}
					else
					{
						if (ptEncoderInfo->tTextOverlayData.bTextOverlayEnable)
							VencSlave_TextOverlay(ptVFB, &ptEncoderInfo->tTextOverlayData, ptEncoderInfo->_tInitNeedInfo.dwVideoMaxWidth);
						VencEncoder_Encode(ptEncoderInfo, eVPMode, ptVFB);
					}
				}
				
#ifndef _SINGLE_BUFFER_
#ifdef _VMA_IRE		
				if (ptEncoderInfo->bNeedResize) {
					Resize_WaitOneFrameComplete(ptEncoderInfo->hResizeObj, &(ptEncoderInfo->tResizeState));
				}
				else if (ptEncoderInfo->tTextOverlayData.bTextOverlayEnable)
#else
				if (!ptEncoderInfo->bNeedResize && ptEncoderInfo->tTextOverlayData.bTextOverlayEnable)
#endif
				{
					if (bDoNotDMACopy)
						memcpy(ptCachedVFB, &tVFB, sizeof(TVideoFrameBuffer));
					else
					{
						MassMemAccess_WaitOneFrameComplete(ptEncoderInfo->hMassMemAccObjY, &ptEncoderInfo->tMassMemAccStateY);
						if (!ptEncoderInfo->bUseOneDMATextoverlay) {
							MassMemAccess_WaitOneFrameComplete(ptEncoderInfo->hMassMemAccObjCb, &ptEncoderInfo->tMassMemAccStateCb);
							MassMemAccess_WaitOneFrameComplete(ptEncoderInfo->hMassMemAccObjCr, &ptEncoderInfo->tMassMemAccStateCr);
						}
					}
				}

				if (ptCachedVFB) {
					ptCachedVFB->eBufStatus = evsobsFinished;
					ptCachedVFB->dwSecond = ptVFB->dwSecond;
					ptCachedVFB->dwMilliSecond = ptVFB->dwMilliSecond;
					ptCachedVFB->dwMicroSecond = ptVFB->dwMicroSecond;
				}
#else
				if (ptCachedVFB && bDoNotDMACopy && !ptEncoderInfo->bNeedResize)
				{
					memcpy(ptCachedVFB, &tVFB, sizeof(TVideoFrameBuffer));
					ptCachedVFB->eBufStatus = evsobsFinished;
					ptCachedVFB->dwSecond = ptVFB->dwSecond;
					ptCachedVFB->dwMilliSecond = ptVFB->dwMilliSecond;
					ptCachedVFB->dwMicroSecond = ptVFB->dwMicroSecond;
				}
				
#endif // ifndef _SINGLE_BUFFER_				
				ptEncoderInfo->dwSwitchIndex = ~(ptEncoderInfo->dwSwitchIndex);
			}
			//release_shared_buffer:
			// 3. release shared buffer
			VencEncoder_ReleaseInputBuf(ptEncoderInfo->hVideoInSrdObj, ptSharedBuffMgrState);
			ptSharedBuffMgrState = NULL;

			if (ptEncoderInfo->bFRCalculate == TRUE) {
				if (ptEncoderInfo->dwFRCalculateFrameCount == 99) {
					float fltotal_time = 0.0;
					gettimeofday(&tv, NULL);
					fltotal_time = ((tv.tv_sec - pretv.tv_sec)*1000000+tv.tv_usec)-pretv.tv_usec;
					printf("Encoder (pid:%d): %u usec per 100fs (fps = %6f)\n", getpid(), (DWORD)fltotal_time, 100000000/fltotal_time);
					syslog(LOG_INFO, "Encoder (pid:%d): %u usec per 100fs, (fps = %6f)\n", getpid(), (DWORD)fltotal_time, 100000000/fltotal_time);
					ptEncoderInfo->dwFRCalculateFrameCount = 0;
				} else {
					ptEncoderInfo->dwFRCalculateFrameCount++;
				}
			}
		}

		if (ptSharedBuffMgrState != NULL) {
			VencEncoder_ReleaseInputBuf(ptEncoderInfo->hVideoInSrdObj, ptSharedBuffMgrState);
			ptSharedBuffMgrState = NULL;
		}

		ptEncoderInfo->bExitInnerLoop = TRUE;

		DBPRINT0("[VENC_ENCODER] Encoder Exit inner loop\n");
	}

	sem_post(&(ptEncoderInfo->smEnd));
	DBPRINT0("[VENC_ENCODER] Encoder Exit thread\n");

	return 0;
}
/* ========================================================================== */
void VencEncoder_SetOptions(HANDLE hEncoderProcess, TEncoderOptions tEncoderOpt)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)hEncoderProcess;
	if (ptEncoderInfo == NULL) {
		fprintf(stderr, "[VENC_ENCODER] Encoder setoptions fail: NULL Pointer!!\n");
		return;
	}

	//    sem_wait(&(ptEncoderInfo->smOptions));
	switch(tEncoderOpt.eenOptType) {
		case VENCSLAVE_ENCODER_OPTION_SET_CODEC:
			ptEncoderInfo->tPreSetOptRec.bCodec = TRUE;
			ptEncoderInfo->tPreSetOptRec.dwCodecType = (EVideoProcessMode)tEncoderOpt.adwUserData[0];
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_FRAMERATE:
			ptEncoderInfo->tPreSetOptRec.bFrameRate = TRUE;
			ptEncoderInfo->tPreSetOptRec.dwFrameRate = tEncoderOpt.adwUserData[0];
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_RESOLUTION:
			ptEncoderInfo->tPreSetOptRec.bResolution = TRUE;
			ptEncoderInfo->tPreSetOptRec.vfFormat = tEncoderOpt.adwUserData[0];
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_CONVERTINGMETHOD:
			ptEncoderInfo->tPreSetOptRec.bConvertingMethod = TRUE;
			ptEncoderInfo->tPreSetOptRec.ecMethod = tEncoderOpt.adwUserData[0];
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_MPEG4_INTRAINTERVAL:
			ptEncoderInfo->tMp4VOptRec.bIntraInterval = TRUE;
			ptEncoderInfo->tMp4VOptRec.dwIntraInterval = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->bMp4VOptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_MPEG4_RATECTRL:
			ptEncoderInfo->tMp4VOptRec.bSetRateControl = TRUE;
			ptEncoderInfo->tMp4VOptRec.ercfMPEG4QualityType = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->tMp4VOptRec.bQualityGroup = TRUE;
			ptEncoderInfo->bMp4VOptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_MPEG4_QUANT:
			ptEncoderInfo->tMp4VOptRec.bQuant = TRUE;
			ptEncoderInfo->tMp4VOptRec.dwQuant = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->tMp4VOptRec.bQualityGroup = TRUE;
			ptEncoderInfo->bMp4VOptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_MPEG4_BITRATE:
			ptEncoderInfo->tMp4VOptRec.bBitrate = TRUE;
			ptEncoderInfo->tMp4VOptRec.dwBitrate = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->tMp4VOptRec.bQualityGroup = TRUE;
			ptEncoderInfo->bMp4VOptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_MJPEG_QUANT:                
			ptEncoderInfo->tJPEGOptRec.bQuant = TRUE;
			ptEncoderInfo->tJPEGOptRec.dwQuant = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->bJPEGOptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_H264_INTRAINTERVAL:
			ptEncoderInfo->tH264OptRec.bIntraInterval = TRUE;
			ptEncoderInfo->tH264OptRec.dwIntraInterval = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->bH264OptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_H264_RATECTRL:
			ptEncoderInfo->tH264OptRec.bSetRateControl = TRUE;
			ptEncoderInfo->tH264OptRec.ercfH264QualityType = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->tH264OptRec.bQualityGroup = TRUE;
			ptEncoderInfo->bH264OptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_H264_QUANT:
			ptEncoderInfo->tH264OptRec.bQuant = TRUE;
			ptEncoderInfo->tH264OptRec.dwQuant = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->tH264OptRec.bQualityGroup = TRUE;
			ptEncoderInfo->bH264OptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_H264_BITRATE:
			ptEncoderInfo->tH264OptRec.bBitrate = TRUE;
			ptEncoderInfo->tH264OptRec.dwBitrate = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->tH264OptRec.bQualityGroup = TRUE;
			ptEncoderInfo->bH264OptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_SVC_INTRAINTERVAL:
			ptEncoderInfo->tSVCOptRec.bIntraInterval = TRUE;
			ptEncoderInfo->tSVCOptRec.dwIntraInterval = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->bSVCOptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_SVC_RATECTRL:
			ptEncoderInfo->tSVCOptRec.bSetRateControl = TRUE;
			ptEncoderInfo->tSVCOptRec.ercfSVCQualityType = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->tSVCOptRec.bQualityGroup = TRUE;
			ptEncoderInfo->bSVCOptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_SVC_QUANT:
			ptEncoderInfo->tSVCOptRec.bQuant = TRUE;
			ptEncoderInfo->tSVCOptRec.dwQuant = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->tSVCOptRec.bQualityGroup = TRUE;
			ptEncoderInfo->bSVCOptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_SVC_BITRATE:
			ptEncoderInfo->tSVCOptRec.bBitrate = TRUE;
			ptEncoderInfo->tSVCOptRec.dwBitrate = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->tSVCOptRec.bQualityGroup = TRUE;
			ptEncoderInfo->bSVCOptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_SVC_HDRENABLE:
			ptEncoderInfo->tSVCOptRec.bHDREnable = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->tSVCOptRec.bSetHDREnable = TRUE;
			ptEncoderInfo->bSVCOptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_SVC_LAYERNUM:
			ptEncoderInfo->tSVCOptRec.dwLayerNum = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->tSVCOptRec.bSetLayerNum = TRUE;
			ptEncoderInfo->bSVCOptionsModified = TRUE;
			break; 
		case VENCSLAVE_ENCODER_OPTION_SET_H264_REFERENCE_FRAME_NUM:
			ptEncoderInfo->tH264OptRec.bRefFrameNum = TRUE;
			ptEncoderInfo->tH264OptRec.dwRefFrameNum = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->bH264OptionsModified = TRUE;
			break; 
		case VENCSLAVE_ENCODER_OPTION_SET_H264_SEARCH_RANGE_X:
			ptEncoderInfo->tH264OptRec.bSearchRangeGroup = TRUE;
			ptEncoderInfo->tH264OptRec.dwSearchRangeX = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->bH264OptionsModified = TRUE;
			break; 
		case VENCSLAVE_ENCODER_OPTION_SET_H264_SEARCH_RANGE_Y:
			ptEncoderInfo->tH264OptRec.bSearchRangeGroup = TRUE;
			ptEncoderInfo->tH264OptRec.dwSearchRangeY = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->bH264OptionsModified = TRUE;
			break; 
		case VENCSLAVE_ENCODER_OPTION_SET_H264_MAX_FRAME_RATE:
			ptEncoderInfo->tH264OptRec.bFrameRateGroup = TRUE;
			ptEncoderInfo->tH264OptRec.dwMaxFrameRate = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->bH264OptionsModified = TRUE;
			break; 
		case VENCSLAVE_ENCODER_OPTION_SET_H264_MIN_FRAME_RATE:
			ptEncoderInfo->tH264OptRec.bFrameRateGroup = TRUE;
			ptEncoderInfo->tH264OptRec.dwMinFrameRate = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->bH264OptionsModified = TRUE;	
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_H264_MAX_QUANT:
			ptEncoderInfo->tH264OptRec.bQuant = TRUE;
			ptEncoderInfo->tH264OptRec.dwMaxQuant = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->tH264OptRec.bQualityGroup = TRUE;
			ptEncoderInfo->bH264OptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_H264_MIN_QUANT:
			ptEncoderInfo->tH264OptRec.bQuant = TRUE;
			ptEncoderInfo->tH264OptRec.dwMinQuant = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->tH264OptRec.bQualityGroup = TRUE;
			ptEncoderInfo->bH264OptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_H264_DEBLOCKENABLE:
			ptEncoderInfo->tH264OptRec.bDeblock = TRUE;
			ptEncoderInfo->tH264OptRec.bDeblockEnable = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->bH264OptionsModified = TRUE;
			break;
		case VENCSLAVE_ENCODER_OPTION_SET_H264_PRFOFILE:
			ptEncoderInfo->tH264OptRec.bChangeProfile = TRUE;
			ptEncoderInfo->tH264OptRec.dwProfile = tEncoderOpt.adwUserData[0];
			ptEncoderInfo->bH264OptionsModified = TRUE;
			break;	
		default:
			DBPRINT0("[VENC_ENCODER] Unknown option\n");
			break;
	}  

	ptEncoderInfo->bOptionsModified = TRUE;
	//    sem_post(&(ptEncoderInfo->smOptions));
	return;
}

/* =========================================================================================== */
SCODE VencEncoder_LoadConfig(HANDLE hObject)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)hObject;

	if (ptEncoderInfo == NULL)
		return S_FAIL;

	if (XmlWrapper_ReadFile_UsrDefFunc(ptEncoderInfo->szConfigFile, ptEncoderInfo->hXMLWrapperObj, ptEncoderInfo) != S_OK) {
		fprintf(stderr, "[VENC_ENCODER] XmlWrapper reload config fail! \n");
		return S_FAIL;
	}

	if (XmlWrapper_Reset(ptEncoderInfo->hXMLWrapperObj) != S_OK)  {
		DBPRINT0("[VENC_ENCODER] XmlWrapper read config fail! \n");
		return S_FAIL;
	}
	//#ifdef _ENC_ROI_
	VencEncoder_ROISetoptions(ptEncoderInfo);
	//#endif    
	return S_OK;
}


/* =========================================================================================== */

#ifdef _JPEG_SNAPSHOT_
void VencEncoder_JPEGSnapshot(TEncoderInfo *ptEncoderInfo, DWORD dwTimeStampSec, DWORD dwTimeStampMilliSec)
{
	int						iEncCnt;
	struct timeval 			timeout;
	int						fdShardBuffer;
	int     				iResult;
	fd_set              	fdsWrite;

	// for snapshot
	TSharedBuffMgrState    tSharedBuffMgrState;

	// JPEG Encoder stuff
	HANDLE                 hJPEGEncObj = ptEncoderInfo->hJPEGEncSnapShot;
	TJPEGEncOptions        tJPEGEncOpt;
	TJPEGEncState          tJPEGEncState;
	SCODE                  scResult;
	//    BYTE                   *pbyBuffer;
	DWORD                  dwFrameBytes = 0;

	// DWORD dwSecond = dwTimeStampSec;
	//DWORD dwMilliSecond = dwTimeStampMilliSec;

#ifdef _DUMP_JPEG_FILE
	char aszJPEGFilePath[32];
	sprintf(aszJPEGFilePath, "/tmp/snapshot%02d.jpg", ptEncoderInfo->dwFrameCount4Snapshot);
	ptEncoderInfo->pfJPEGOutput4Snapshot = fopen(aszJPEGFilePath, "wb");
#endif // _DUMP_JPEG_FILE

	iEncCnt = 0;
	if (!hJPEGEncObj)
		return;

	ptEncoderInfo->pfnGetOutputBufferFD(ptEncoderInfo->hSnapshotSrdObj, &fdShardBuffer);

	if (ptEncoderInfo->pfnRequestOutputBuffer(ptEncoderInfo->hSnapshotSrdObj, &tSharedBuffMgrState) != S_OK)
	{
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		FD_ZERO(&fdsWrite);
		FD_SET(fdShardBuffer, &fdsWrite);

		iResult = select(fdShardBuffer + 1, NULL, &fdsWrite, NULL, &timeout);
		if (iResult <= 0) {
			DBPRINT0("MJPEG select timeout.... \n");
		}
		if (!FD_ISSET(fdShardBuffer, &fdsWrite))
			return;
		if (ptEncoderInfo->pfnRequestOutputBuffer(ptEncoderInfo->hSnapshotSrdObj, &tSharedBuffMgrState) != S_OK)
			return;
	}
	while(iEncCnt < 3) {
		// set output buffer to JPEG encoder
		tJPEGEncOpt.eOptionFlags = VIDEO_CHANGE_BITSTREAM_BUFFER;
		tJPEGEncOpt.adwUserData[0] = (DWORD)(tSharedBuffMgrState.pbyDataAddr);
		tJPEGEncOpt.adwUserData[1] = ptEncoderInfo->_tInitNeedInfo.dwBitStreamSize;

		scResult = JPEGEnc_SetOptions(hJPEGEncObj, &tJPEGEncOpt);
		assert(scResult == S_OK);

		memset(&tJPEGEncState, 0, sizeof(TJPEGEncState));
		scResult = JPEGEnc_OneFrame(hJPEGEncObj, &tJPEGEncState);
		if (scResult == S_OK) {
			dwFrameBytes = tJPEGEncState.dwEncSize;
			*(DWORD *)tSharedBuffMgrState.pbyHdrAddr = tJPEGEncState.dwEncSize;                    
#ifdef _DUMP_JPEG_FILE
			if (ptEncoderInfo->pfJPEGOutput4Snapshot == NULL) {
				printf("can't open jpeg output\n");
				break;
			}					
			fwrite(tSharedBuffMgrState.pbyDataAddr, sizeof(BYTE), dwFrameBytes, ptEncoderInfo->pfJPEGOutput4Snapshot);
			fclose(ptEncoderInfo->pfJPEGOutput4Snapshot);
			(ptEncoderInfo->dwFrameCount4Snapshot < 15)?ptEncoderInfo->dwFrameCount4Snapshot++:(ptEncoderInfo->dwFrameCount4Snapshot = 0);
#endif // _DUMP_JPEG_FILE       
			break;
		} else {
			printf("[VENC_ENCODER] Error encoder JPEG image %d, #%d\n", scResult, iEncCnt);
			iEncCnt++;
			dwFrameBytes = 0;
			// change quant value
			tJPEGEncOpt.eOptionFlags  = VIDEO_CHANGE_QUANT;
			tJPEGEncOpt.adwUserData[0] = 40 + iEncCnt*50;
			scResult = JPEGEnc_SetOptions(hJPEGEncObj, &tJPEGEncOpt);
			assert(scResult == S_OK);
		}
	}//end while(iEncCnt < 3)
	ptEncoderInfo->pfnReleaseOutputBuffer(ptEncoderInfo->hSnapshotSrdObj, &tSharedBuffMgrState);		                 
	// restore the original quant if necessary
	if (iEncCnt != 0) {
		tJPEGEncOpt.eOptionFlags  = VIDEO_CHANGE_QUANT;
		tJPEGEncOpt.adwUserData[0] = 40;
		scResult = JPEGEnc_SetOptions(hJPEGEncObj, &tJPEGEncOpt);
		assert(scResult == S_OK);
	}        	
}
#endif // _JPEG_SNAPSHOT_
/* =========================================================================================== */
void VencEncoder_Encode(TEncoderInfo *ptEncoderInfo, EVideoProcessMode eVPMode, TVideoFrameBuffer *ptVFB)
{
	// check if it need to call Codec_RESTART
	// if time different longer than 2 second, reset video encoder
	if ((ptVFB->dwSecond - ptEncoderInfo->dwLastSecond) > DIFF_SEC_INTERVAL)
	{
		if ((eVPMode & vpmStreamEncode) == vpmStreamEncH264) {
			VencEncoder_H264Restart(ptEncoderInfo);
		}
		else if ((eVPMode & vpmStreamEncode) == vpmStreamEncMP4V) {
			VencEncoder_Mp4VRestart(ptEncoderInfo);
		}
		else if ((eVPMode & vpmStreamEncode) == vpmStreamEncSVC) {
			VencEncoder_SVCRestart(ptEncoderInfo);
		}
	}

	ptEncoderInfo->dwLastSecond = ptVFB->dwSecond;

	if ((eVPMode & vpmStreamEncH264) == vpmStreamEncH264) {
		VencEncoder_H264Encode(ptEncoderInfo, ptVFB);
	}
	else if ((eVPMode & vpmStreamEncJPEG) == vpmStreamEncJPEG) {
		VencEncoder_JPEGEncode(ptEncoderInfo, ptVFB);
	}
	else if ((eVPMode & vpmStreamEncMP4V) == vpmStreamEncMP4V) {
		VencEncoder_Mp4VEncode(ptEncoderInfo, ptVFB);
	}
	else if ((eVPMode & vpmStreamEncode) == vpmStreamEncSVC) {
		VencEncoder_SVCEncode(ptEncoderInfo, ptVFB);
	}

#ifdef _VMA_JEBE
#ifdef _JPEG_SNAPSHOT_
	if (ptEncoderInfo->bGetSnapshot == TRUE) {
		TJPEGEncOptions        tJPEGEncOpt;
		memset(&tJPEGEncOpt, 0x0, sizeof(TJPEGEncOptions));
		tJPEGEncOpt.eOptionFlags    = VIDEO_CHANGE_FRAME_BUFFER;
		tJPEGEncOpt.adwUserData[0]  = (DWORD)ptVFB->pbyBufY;
		tJPEGEncOpt.adwUserData[1]  = (DWORD)ptVFB->pbyBufU;
		tJPEGEncOpt.adwUserData[2]  = (DWORD)ptVFB->pbyBufV;
		if (ptEncoderInfo->hJPEGEncSnapShot != NULL) {
			if (JPEGEnc_SetOptions(ptEncoderInfo->hJPEGEncSnapShot, &tJPEGEncOpt) != S_OK)
				return;
		}

		if ((--ptEncoderInfo->dwSnapshotCountdown) == 0) {
			VencEncoder_JPEGSnapshot(ptEncoderInfo, ptVFB->dwSecond, ptVFB->dwMilliSecond);
			ptEncoderInfo->bGetSnapshot = FALSE;
			ptEncoderInfo->bDoEncode = (ptEncoderInfo->tMsgOptRec.dwVisitors > 0);
		}
	}
#endif // _JPEG_SNAPSHOT_   
#endif // _VMA_JEBE
}
