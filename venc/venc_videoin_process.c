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
 * venc_videoin_process.c
 *
 * \brief
 * Video in Core Processing function.
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

SCODE VencVideoIn_ApplyOption(TVideoInInfo *ptVideoInInfo);
#ifdef _VMA_IBPE

#ifdef _VMA_IBPE_BACK_DOOR
static SCODE ReadFile(HANDLE phMemMgrObj, FILE* pFile, void** pvDataBuffer)
{
	DWORD dwFileSize;
	TMemMgrState tMemMgrState;

	// Obtain file size:
	fseek(pFile, 0, SEEK_END);
	dwFileSize = ftell(pFile);
	rewind(pFile);
	
	if ((PVOID)(*pvDataBuffer) == NULL)
	{
		// allocate memory to contain the whole file:
		tMemMgrState.dwBusNum = 0;
		tMemMgrState.dwSize = dwFileSize;
		tMemMgrState.eAlignType = ALIGN_TYPE_256_BYTE;
		if ((*pvDataBuffer = (PVOID)MemMgr_GetMemory(phMemMgrObj, &tMemMgrState)) == NULL)
		{
			printf("[VENC_VIDEOIN] Allocate IBPE param File buffer fail !!\n");
			return S_FAIL;
		}
	}
	// copy the file into the buffer:
	if (fread(*pvDataBuffer, 1, dwFileSize, pFile) != dwFileSize)
	{		
		return S_FAIL;
	}

	return S_OK;
}
static SCODE Rest3DNRBackDoor(TVideoInInfo *ptVideoInInfo, BOOL bTWICEIBPE)
{
	SCODE sRet = S_OK;
	CHAR chNR3DMA1CurrWeightFileName[1024]; 
	HANDLE phIBPObject = ptVideoInInfo->hIBPObj;
#ifdef _PRE_NR_
	HANDLE hSencodPhaseIBPObject = ptVideoInInfo->hSencodPhaseIBPObj;
#endif
	HANDLE phMemMgrObj = ptVideoInInfo->hMemMgrObj;
	TImgBackEndProcessOptions tOptions;
	FILE *pParamFile;
	memset(chNR3DMA1CurrWeightFileName, 0x0,sizeof(chNR3DMA1CurrWeightFileName));
	if (bTWICEIBPE == TRUE)
	{
		strncpy(chNR3DMA1CurrWeightFileName, "/etc/conf.d/isptable/NR3DMA1CurrWeightPreNR.bin", sizeof(chNR3DMA1CurrWeightFileName));
	}
	else
	{
		strncpy(chNR3DMA1CurrWeightFileName, "/etc/conf.d/isptable/NR3DMA1CurrWeight.bin", sizeof(chNR3DMA1CurrWeightFileName));
	}
	printf("[venc_videoin_process](%d)The path of NR3DMA1CurrWeight is %s\n", __LINE__, chNR3DMA1CurrWeightFileName);
	if ((pParamFile = fopen(chNR3DMA1CurrWeightFileName, "r")) != NULL)
	{
		if (ReadFile(phMemMgrObj, pParamFile, &ptVideoInInfo->pvNR3DMA1CurrWeight) == S_OK)
		{
			memset(&tOptions, 0x0, sizeof(TImgBackEndProcessOptions));
			tOptions.eOptionFlags = 211;
			tOptions.apvUserData[0] = (PVOID)ptVideoInInfo->pvNR3DMA1CurrWeight;
			if (ImgBackEndProcess_SetOptions(phIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				printf("[venc_videoin_process](%d)Set %s Failed!!", __LINE__, chNR3DMA1CurrWeightFileName);
				sRet = S_FAIL;
			}
#ifdef _PRE_NR_	
			if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				printf("[venc_videoin_process](%d)Set %s Failed!!", __LINE__, chNR3DMA1CurrWeightFileName);
				sRet = S_FAIL;
			}
#endif
		 }
		 fclose(pParamFile);
	}
	else
	{
		printf("[venc_videoin_process](%d)Open %s Failed!!\n", __LINE__, chNR3DMA1CurrWeightFileName);
		sRet = S_FAIL;
	}
	return sRet;
}
#endif
#endif
/* ============================================================================================= */
void* VencVideoIn_Loop(HANDLE hInstance)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hInstance;

	syslog(LOG_INFO, "[VENC_VIDEOIN] venc_videoin thread pid: %d\n", getpid());

	// or using default (join)? think about it
	pthread_detach(pthread_self());

	VencVideoIn_Process(ptVideoInInfo);

	return 0;
}
#ifdef _VMA_IBPE
#if 0
void PrintWindowInfo(TMtnDtctWindow *ptMtnDtctWindow)
{
	printf("========================Motion Detection Info =================== \n");
	printf("dwWindowSerialNum: %u (App)\n", ptMtnDtctWindow->dwWindowSerialNum );
	printf("dwStartX         : %u (App)\n", ptMtnDtctWindow->dwStartX          );
	printf("dwStartY         : %u (App)\n", ptMtnDtctWindow->dwStartY          );
	printf("dwRangeX         : %u (App)\n", ptMtnDtctWindow->dwRangeX          );
	printf("dwRangeY         : %u (App)\n", ptMtnDtctWindow->dwRangeY          );
	printf("byPixDiffThr     : %d (App)\n",  ptMtnDtctWindow->byPixDiffThr      );
	printf("dwMotionPixNum   : %u (App)\n", ptMtnDtctWindow->dwMotionPixNum    );
	printf("dwMtnPercentThr  : %u (App)\n", ptMtnDtctWindow->dwMtnPercentThr   );
	printf("bIsMotion        : %s (App)\n",  ptMtnDtctWindow->bIsMotion ? "yes": "No");
	printf("========================Motion Detection Info End =================== \n");

}
#endif
#endif
/*===========================================================================*/
SCODE VencVideoIn_DumpAFStatus(TVideoInInfo *ptVideoInInfo)
{
	FILE *fp = NULL;

	fp = fopen(DUMP_AF_STATUS_FILE_PATH, "r");

	if (fp != NULL)
	{
		fclose(fp);
		if (unlink(DUMP_AF_STATUS_FILE_PATH) != 0)
		{
			printf("(%d)Call unlink() to remove file %s is failed. (%s)\n", __LINE__, DUMP_AF_STATUS_FILE_PATH, strerror(errno));
		}
	}
	
	fp = fopen(DUMP_AF_STATUS_FILE_PATH, "w");
	
	if (fp == NULL)
	{
		printf("(%d)Call fopen() to open file %s is failed. (%s)\n", __LINE__, DUMP_AF_STATUS_FILE_PATH, strerror(errno));
		goto exit;
	}

	if (fprintf(fp, "<zoomRatio>%d</zoomRatio>", ptVideoInInfo->tAFStatus.dwZoomRatio) == -1)
	{
		printf("(%d)Call fprint() to write dwZoomRatio=%d to file %s is failed. (%s)\n", __LINE__, ptVideoInInfo->tAFStatus.dwZoomRatio, DUMP_AF_STATUS_FILE_PATH, strerror(errno));
	}

	if (fp != NULL)
	{
		fclose(fp);
	}
exit:   
	return S_OK;
}

/*===========================================================================*/
SCODE VencVideoIn_KeepCurrentWB(TVideoInInfo *ptVideoInInfo)
{
	FILE *fp = NULL;

	fp = fopen(KEEP_CURRENT_WB_FILE_PATH, "r");
	if (fp != NULL)
	{
		fclose(fp);
		if (unlink(KEEP_CURRENT_WB_FILE_PATH) != 0)
		{
			printf("(%d)Call unlink() to remove file %s is failed. (%s)\n", __LINE__, KEEP_CURRENT_WB_FILE_PATH, strerror(errno));
		}
	}
	
	fp = fopen(KEEP_CURRENT_WB_FILE_PATH, "w");
	if (fp == NULL)
	{
		printf("(%d)Call fopen() to open file %s is failed. (%s)\n", __LINE__, KEEP_CURRENT_WB_FILE_PATH, strerror(errno));
	}
	if (fprintf(fp, "%d %d", ptVideoInInfo->tAWBRetGainValue.dwAWBRedGain, ptVideoInInfo->tAWBRetGainValue.dwAWBBlueGain) == -1)
	{
		printf("(%d)Call fprint() to write (RedGain,BlueGain)=(%d,%d) to file %s is failed. (%s)\n", __LINE__, ptVideoInInfo->tAWBRetGainValue.dwAWBRedGain, ptVideoInInfo->tAWBRetGainValue.dwAWBBlueGain, KEEP_CURRENT_WB_FILE_PATH, strerror(errno));
	}

	if (fp != NULL)
	{
		fclose(fp);
	}

	return S_OK;
}
/*===========================================================================*/

SCODE VencVideoIn_KeepCurrentWBFromFile(TVideoInInfo *ptVideoInInfo)
{
	TVideoSignalOptions tVideoSigOptions;
	FILE *fp = NULL;
	DWORD dwGreenGain = 64;//Use the default green gain 64. 
	DWORD dwGain = 0;
	DWORD dwAWBRedGain = 0;
	DWORD dwAWBBlueGain = 0;

	fp = fopen(KEEP_CURRENT_WB_FILE_PATH, "r");
	if (fp == NULL)
	{
		printf("(%d)Call fopen() to open file %s is failed. (%s)\n", __LINE__, KEEP_CURRENT_WB_FILE_PATH, strerror(errno));
		goto exit;
	}

	fscanf(fp, "%d %d", &dwAWBRedGain, &dwAWBBlueGain);

	dwGain = (dwGreenGain << 8) | dwAWBRedGain;

	memset(&tVideoSigOptions, 0x0, sizeof(TVideoSignalOptions));
	tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_AUTO_WHITE_BALANCE_CTRL;
	tVideoSigOptions.adwUserData[0] =  TRUE;
	tVideoSigOptions.adwUserData[1] =  dwAWBRedGain;//dwGain;
	tVideoSigOptions.adwUserData[2] =  dwAWBBlueGain;

	if (VideoCap_SetOptions(ptVideoInInfo->hVideoCapObj, &tVideoSigOptions) == S_OK) {
		//DBPRINT0("VencVideoIn_KeepCurrentWBFromFile()  successful \n");
		//printf("(%d) VencVideoIn_KeepCurrentWBFromFile() (WhiteBalance, RedGain, BlueGain, GreenGain) = (%s, %u, %u, %u) \n", __LINE__, (tVideoSigOptions.adwUserData[0] == 0)? "auto": "manual", ptVideoInInfo->tAWBRetGainValue.dwAWBRedGain, ptVideoInInfo->tAWBRetGainValue.dwAWBBlueGain,dwGreenGain);
		//return S_OK;
	} else {
		DBPRINT0("VencVideoIn_KeepCurrentWBFromFile() fail \n");
		return S_FAIL;
	}
exit:
	return S_OK;
}

/* ========================================================================= */
SCODE VencVideoIn_RetrieveVideoCapStatus(TVideoInInfo *ptVideoInInfo, TVideoCapState *ptVideoCapState)
{
	return S_OK;
}

/* ========================================================================= */
SCODE VencVideoIn_PhotoMtcDefTbl(TVideoInInfo *ptVideoInInfo, BYTE *pbyYFrame, EPhotoMtcMode ePhotoMtcMode)
{
	TVideoSignalOptions tVideoSigOptions;
	FILE  *pfPhotoLDCRef;
	CHAR	szTemp[128];

	// reset to default value
	//printf("[venc_videoin_process](%d)PhotoLDCTblSize:%d, LSCRation:%d\n", __LINE__, ptVideoInInfo->dwPhotoLDCTblSize, ptVideoInInfo->tPhotoMtcCfg.dwLSCRatio);
	memset(ptVideoInInfo->pwPhotoLDCRefFrame, 1024, ptVideoInInfo->dwPhotoLDCTblSize);
	tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_PHOTO_LDC_CALIBRATE;
	tVideoSigOptions.adwUserData[0] = (DWORD)ptVideoInInfo->pwPhotoLDCRefFrame;
	tVideoSigOptions.adwUserData[1] = (DWORD)pbyYFrame;
	tVideoSigOptions.adwUserData[2] = (ptVideoInInfo->tPhotoMtcCfg.dwLSCRatio << 16) | ptVideoInInfo->dwAllocatedMemForCapBufWidth;
	if (VideoCap_SetOptions(ptVideoInInfo->hVideoCapObj, &tVideoSigOptions) == S_OK) {
		printf("[VENC_VIDEOIN] VIDEO_SIGNAL_OPTION_PHOTO_LDC_CALIBRATE option success !!\n");
	} else {
		printf("[VENC_VIDEOIN] VIDEO_SIGNAL_OPTION_PHOTO_LDC_CALIBRATE option fail !!\n");
		return S_FAIL;
	}

	memset(szTemp, 0x0, sizeof(szTemp));
	if (ePhotoMtcMode == photomtcmUserDefine1) {
		strncpy(szTemp, PHOTOLDCREF_USERDEFINE1, strlen(PHOTOLDCREF_USERDEFINE1));
		syslog(LOG_INFO,"[VENC_VIDEOIN] Overwrite PhotLometric table %s under photomtcmUserDefine1 \n", szTemp);
	} else if (ePhotoMtcMode == photomtcmUserDefine2) {
		strncpy(szTemp, PHOTOLDCREF_USERDEFINE2, strlen(PHOTOLDCREF_USERDEFINE2));
		printf("[VENC_VIDEOIN] Overwrite Photometric table %s under photomtcmUserDefine2 \n", szTemp);
	} else {
		printf("[VENC_VIDEOIN] Photometric table can't be overwritten in mode %d. !!\n", ePhotoMtcMode);
		return S_FAIL;
	}

	if ((pfPhotoLDCRef = fopen(szTemp, "w")) == NULL) {
		printf("[VENC_VIDEOIN] Open Photo LDC reference file %s fail !!\n", szTemp);
		return S_FAIL;
	} else {
		fwrite(ptVideoInInfo->pwPhotoLDCRefFrame, sizeof(WORD), ptVideoInInfo->dwPhotoLDCTblSize, pfPhotoLDCRef);
		fclose(pfPhotoLDCRef);
		printf("[VENC_VIDEOIN] write new data to Photo LDC reference file %s !!\n", szTemp);
	}

	return S_OK;
}

/* ========================================================================= */
SCODE VencVideoIn_TryGetBuf(TVideoInInfo *ptVideoInInfo, TVideoCapState **ptVideoCapStateOld)
{
	static DWORD dwPrevFrameCount = 0xFFFFFFFF;
	static DWORD dwPreSecond = 0;
	static DWORD dwPreMilliSecond = 0;
	static DWORD dwPreMicroSecond = 0;
	SCODE  scRet;
#ifdef _DEFECT_PIXEL_ENABLE_
	TDefectPixelCorrectState tDPCState;
#endif
	static int dwCapIndex = 0;
	int i = 0;
#ifdef DEFECT_PIXEL_ENABLE
	TDefectPixelCorrectInitOptions tDPCInitOptions;
	TDefectPixelCorrectState tDPCState;
	HANDLE hDPCObject;
	FILE *pfDPCInput;
	DWORD dwFileSize, dwCheckSum;
	DWORD *DPInfo = NULL;
	CHAR acRootext[200] = "/etc/conf.d/dp_table.bin";
#endif  
	TVideoCapState *ptVideoCapState = &ptVideoInInfo->atVideoCapState[dwCapIndex];
	TVideoFrameBuffer  *ptVFB = &ptVideoInInfo->atVideoCapOutVFB[dwCapIndex];

	//Because videoin thread doens't take a lot of time, put sleep first to avoid call VideoCap_GetBuf twice.
	VideoCap_Sleep(ptVideoInInfo->hVideoCapObj);	
	scRet = VideoCap_GetBuf(ptVideoInInfo->hVideoCapObj, ptVideoCapState);

	if (scRet != S_OK) {
		
		VideoCap_Sleep(ptVideoInInfo->hVideoCapObj);
		scRet = VideoCap_GetBuf(ptVideoInInfo->hVideoCapObj, ptVideoCapState);
		if (scRet != S_OK)
		{
			if (ptVideoCapState->bNoSignal) {
				printf("[VENC_VIDEOIN] No signal !!\n");
			}
			if (ptVideoCapState->bFifoFull) {
				printf("%d [VENC_VIDEOIN] fifo full !!\n", __LINE__);
			}
		}
		
	} else if (ptVideoCapState->dwFrameCount == dwPrevFrameCount) {   
		if (VideoCap_ReleaseBuf(ptVideoInInfo->hVideoCapObj, ptVideoCapState) != S_OK) {
			fprintf(stderr, "[VENC_VIDEOIN] video_cap_releas_buf err!!\n");
		}
		VideoCap_Sleep(ptVideoInInfo->hVideoCapObj);
		scRet = VideoCap_GetBuf(ptVideoInInfo->hVideoCapObj, ptVideoCapState);
	}  

	if (scRet == S_OK)
	{
		if (ptVideoCapState->dwFrameCount != dwPrevFrameCount) 
		{
			// recorder framecount to avoid get same frame again 
			for (i = 0; i < BUFF_SIZE-1; i++) {
				ptVideoInInfo->adwVideoCapIndex[i] = ptVideoInInfo->adwVideoCapIndex[i+1];
			}
			ptVideoInInfo->adwVideoCapIndex[i] = dwCapIndex;
			ptVFB->dwSecond = ptVideoCapState->dwSecond;
			ptVFB->dwMilliSecond = ptVideoCapState->dwMilliSecond;
			ptVFB->dwMicroSecond = ptVideoCapState->dwMicroSecond;
			{
				DWORD dwTmpSecond = ptVFB->dwSecond;
				DWORD dwTmpMilliSecond = ptVFB->dwMilliSecond;
				DWORD dwTmpMicroSecond = ptVFB->dwMicroSecond;
				SDWORD sdwDiff = ((SDWORD)ptVFB->dwSecond - (SDWORD)dwPreSecond) * 1000000+ ((SDWORD)ptVFB->dwMilliSecond - (SDWORD)dwPreMilliSecond)*1000+((SDWORD)ptVFB->dwMicroSecond - (SDWORD)dwPreMicroSecond);

				if (sdwDiff < 0)
				{
					/*
					 * If MD (or IBPE ?) is ebabled, the timestamp of captured frame sometimes will be rollbacked (MN34041PL) !! 
					 * Now just do the following workaround.
					 */
					printf("[venc_videoin_process](%d)two frame time diff < 0 (shutter=%d)\n", __LINE__, ptVideoCapState->dwAEShutter);
					ptVFB->dwSecond = dwPreSecond + (DWORD)(ptVideoCapState->dwAEShutter / 100000000);
					ptVFB->dwMilliSecond = dwPreMilliSecond + (DWORD)(ptVideoCapState->dwAEShutter / 1000);
					ptVFB->dwMicroSecond = dwPreMicroSecond + (DWORD)(ptVideoCapState->dwAEShutter);
				}
				dwPreSecond = dwTmpSecond;
				dwPreMilliSecond = dwTmpMilliSecond;
				dwPreMicroSecond = dwTmpMicroSecond;
			}
			
			ptVFB->dwStride = ptVideoCapState->dwOutStride;
			ptVFB->vFormat = VideoComm_GetFormatFromWH(ptVideoCapState->dwOutWidth, ptVideoCapState->dwOutHeight);
			ptVFB->eBufStatus = evsobsFinished;
			ptVFB->pbyBufY = ptVideoCapState->pbyYFrame;
			ptVFB->pbyBufU = ptVideoCapState->pbyCbFrame;
			ptVFB->pbyBufV = ptVideoCapState->pbyCrFrame;
			ptVFB->dwFrameCount = ptVideoCapState->dwFrameCount;
			dwCapIndex++;
			dwCapIndex = (dwCapIndex % BUFF_SIZE);

			if (ptVideoInInfo->adwVideoCapIndex[0] == -1) {
				*ptVideoCapStateOld = NULL;
				return S_FAIL;
			}
			// recorder framecount to avoid get same frame again 

			*ptVideoCapStateOld = &ptVideoInInfo->atVideoCapState[ptVideoInInfo->adwVideoCapIndex[0]];

			dwPrevFrameCount = ptVideoCapState->dwFrameCount;
			ptVideoInInfo->tAFStatus.dwZoomRatio = ptVideoCapState->dwZoomRatio;
			ptVideoInInfo->tAWBRetGainValue.dwAWBRedGain = ptVideoCapState->dwAWBRedGain;
			ptVideoInInfo->tAWBRetGainValue.dwAWBBlueGain = ptVideoCapState->dwAWBBlueGain;

#ifdef _DEFECT_PIXEL_ENABLE_
			memset(&tDPCState, 0x0, sizeof(TDefectPixelCorrectState));
			tDPCState.pbyYFrame = ptVideoCapState->pbyYFrame;
			tDPCState.pbyCbFrame = ptVideoCapState->pbyCbFrame;
			tDPCState.pbyCrFrame = ptVideoCapState->pbyCrFrame;
			tDPCState.dwOutWidth = ptVideoCapState->dwOutWidth;
			tDPCState.dwOutHeight = ptVideoCapState->dwOutHeight;
			tDPCState.hMemMgrObj = ptVideoInInfo->hMemMgrObj;
			DefectPixelCorrect_ProcessOneFrame(ptVideoInInfo->hDPCObject, &tDPCState);
#endif
		} else {
			DBPRINT1("[VENC_VIDEOIN] Drop the second same frameCount :%03u\n", dwPrevFrameCount);
			if (VideoCap_ReleaseBuf(ptVideoInInfo->hVideoCapObj, ptVideoCapState) != S_OK) {
				fprintf(stderr, "[VENC_VIDEOIN](%d) video_cap_releas_buf err!!\n",__LINE__);                
			}
			scRet = S_FAIL;
		}
	} else {
		printf("[VENC_VIDEOIN](%d) bNoSignal=%d, bFifoFull=%d (Calling VideoCap_GetBuf() is failed!! (%d) )\n", __LINE__, ptVideoCapState->bNoSignal, ptVideoCapState->bFifoFull, scRet);

	}
	return scRet;
}

SCODE VencVideoIn_ReleaseInputBuffer(TVideoInInfo *ptVideoInInfo, DWORD dwIndex)
{
	SCODE scResult = S_OK;
	TVideoCapState tVideoCapState;
	// release video buffer to videocap   
	tVideoCapState.dwIndex = dwIndex;
	scResult = VideoCap_ReleaseBuf(ptVideoInInfo->hVideoCapObj, &tVideoCapState);
	//  	printf("[Release] Release tVideoCapState.dwIndex = %u \n", tVideoCapState.dwIndex);
	if (scResult != S_OK) {
		printf("[VENC_VIDEOIN] VideoProcessInputReleaseBuffer fail !!!\n");
	}
	return scResult;
}

#if defined(_AUTOSCENE_)
static void VencVideoIn_EnableIRCut(TVideoInInfo *ptVideoInInfo,BOOL bEnable)
{
	if (bEnable == TRUE)
	{	
		if (ioctl(ptVideoInInfo->fdIRCut, IRCUT_OPEN) == -1)
		{
			printf("[venc_videoin_process](%d)VencVideoIn_SetIRCut() is Failed!(%s)\n", __LINE__, strerror(errno));
			return;
		}
	}
	else
	{
		if (ioctl(ptVideoInInfo->fdIRCut, IRCUT_CLOSE) == -1)
		{
			printf("[venc_videoin_process](%d)VencVideoIn_SetIRCut() is Failed!(%s)\n", __LINE__, strerror(errno));
			return;
		}
	}
	
	ioctl(ptVideoInInfo->fdIRCut, IRCUT_STANDBY);
	
	printf("[venc_videoin_process](%d)VencVideoIn_SetIRCut() is successful!(%d)\n", __LINE__, bEnable);
	return;
}

#define SHARED_MEM_PATH_NAME "autoscene_shm"
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
		printf("[venc_videoin_process](%d)%s\n", __LINE__, strerror(errno));
		return S_FAIL;
	}
	fstat(fd, &stat);
	ptr = mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
	close(fd);
	if (iResultLen < stat.st_size)
	{
		printf("[venc_videoin_process](%d)Can not read the shm from %s. Because the len of reading buf  %d is smaller than shm size %d\n", __LINE__, SHARED_MEM_PATH_NAME, iResultLen, (int)stat.st_size);  
		return S_FAIL;
	}
	memset(szResult, 0x0, iResultLen);
	for (i = 0; i < stat.st_size; i++)
		szResult[i] = ptr[i];
	
	munmap(ptr,stat.st_size);

	return S_OK;
}
/*
   <?xml version="1.0" encoding="ISO-8859-1" ?>
   <root>
   <edge_enhancement>
   <mode>%d</mode>
   <edge_sensitivity>%d</edge_sensitivity>
   <noise_sensitivity>%d</noise_sensitivity>
   <strength>%d</strength>
   <clip>%d</clip>
   </edge_enhancement>
   <denoise>
   <enable>%d</enable>	
   <mode>%d</mode>
   <2d_mode_strength>%d</2d_mode_strength>
   <3d_mode_ref_strength>%d</3d_mode_ref_strength>
   <3d_mode_cur_weight>%d</3d_mode_cur_weight>
   <motion_adaptive_enable>%d</motion_adaptive_enable>
   </denoise>
   </root>

 */
static void SetDeNoiseEnable(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tYUVNRParam.bEnYUVNR = dwVal;
	}
	return;
}

static void SetDeNoiseMode(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tYUVNRParam.dwNRMode = dwVal;
	}
	return;
}

static void SetDeNoise2DModeStrength(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tYUVNRParam.dw2DNRStrength = dwVal;
	}
	return;
}

static void SetDeNoise3DModeRefStrength(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tYUVNRParam.dw3DNRRefStrength = dwVal;
	}
	return;
}


static void SetDeNoise3DModeCurWeight(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tYUVNRParam.dw3DNRCurrWeight = dwVal;
	}
	return;
}

static void SetDeNoiseMotionAdaptiveEnable(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tYUVNRParam.bNRMotionAdaptiveEn = dwVal;
	}
	return;
}

/////////////////////////////

static void SetPreDeNoiseEnable(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tYUVPreNRParam.bEnYUVNR = dwVal;
	}
	return;
}

static void SetPreDeNoiseMode(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tYUVPreNRParam.dwNRMode = dwVal;
	}
	return;
}

static void SetPreDeNoise2DModeStrength(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tYUVPreNRParam.dw2DNRStrength = dwVal;
	}
	return;
}

static void SetPreDeNoise3DModeRefStrength(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tYUVPreNRParam.dw3DNRRefStrength = dwVal;
	}
	return;
}


static void SetPreDeNoise3DModeCurWeight(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tYUVPreNRParam.dw3DNRCurrWeight = dwVal;
	}
	return;
}

static void SetPreDeNoiseMotionAdaptiveEnable(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tYUVPreNRParam.bNRMotionAdaptiveEn = dwVal;
	}
	return;
}
////////////////////////////

static void SetEEMode(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tEdgeEnhanceParam.bEnEdgeEnhance = dwVal;
	}
	return;
}

static void SetEENoiseSensitivity(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tEdgeEnhanceParam.dwNoiseSensitivity = dwVal;
	}
	return;
}

static void SetEEdgeSensitivity(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tEdgeEnhanceParam.dwEdgeSensitivity = dwVal;
	}
	return;
}

static void SetEEStrength(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tEdgeEnhanceParam.dwEEStrength = dwVal;
	}
	return;
}

static void SetEEClip(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tEdgeEnhanceParam.dwEEClip = dwVal;
	}
	return;
}

////////////////

static void SetPreEEMode(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tPreEdgeEnhanceParam.bEnEdgeEnhance = dwVal;
	}
	return;
}

static void SetPreEENoiseSensitivity(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tPreEdgeEnhanceParam.dwNoiseSensitivity = dwVal;
	}
	return;
}

static void SetPreEEdgeSensitivity(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tPreEdgeEnhanceParam.dwEdgeSensitivity = dwVal;
	}
	return;
}

static void SetPreEEStrength(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tPreEdgeEnhanceParam.dwEEStrength = dwVal;
	}
	return;
}

static void SetPreEEClip(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->tPreEdgeEnhanceParam.dwEEClip = dwVal;
	}
	return;
}

/////////////////////
static void SetIRCutEnable(void *userData, const char *s, int len)
{
	TAutoSceneState *ptAutoSceneState = (TAutoSceneState *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;
	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (ptAutoSceneState != NULL)
	{
		ptAutoSceneState->bEnIRCut = dwVal;
	}
	return;
}

static SCODE parse_isp_state(const char *szUpdatedState,TAutoSceneState *ptAutoSceneState)
{
	int sRet;
	HANDLE hXmlWrapObj;	
	TXmlWrapperTreeMap ptParsingMap[] = 
	{
		{"root/denoise/enable", NULL, &SetDeNoiseEnable, NULL},
		{"root/denoise/mode", NULL, &SetDeNoiseMode, NULL},
		{"root/denoise/two_d_mode_strength", NULL, &SetDeNoise2DModeStrength, NULL},
		{"root/denoise/three_d_mode_ref_strength", NULL, &SetDeNoise3DModeRefStrength, NULL},
		{"root/denoise/three_d_mode_cur_weight", NULL, &SetDeNoise3DModeCurWeight, NULL},
		{"root/denoise/motion_adaptive_enable", NULL, &SetDeNoiseMotionAdaptiveEnable, NULL},
		{"root/pre_denoise/enable", NULL, &SetPreDeNoiseEnable, NULL},
		{"root/pre_denoise/mode", NULL, &SetPreDeNoiseMode, NULL},
		{"root/pre_denoise/two_d_mode_strength", NULL, &SetPreDeNoise2DModeStrength, NULL},
		{"root/pre_denoise/three_d_mode_ref_strength", NULL, &SetPreDeNoise3DModeRefStrength, NULL},
		{"root/pre_denoise/three_d_mode_cur_weight", NULL, &SetPreDeNoise3DModeCurWeight, NULL},
		{"root/pre_denoise/motion_adaptive_enable", NULL, &SetPreDeNoiseMotionAdaptiveEnable, NULL},
		{"root/edge_enhancement/mode", NULL, &SetEEMode, NULL},
		{"root/edge_enhancement/edge_sensitivity", NULL, &SetEEdgeSensitivity, NULL},
		{"root/edge_enhancement/noise_sensitivity", NULL, &SetEENoiseSensitivity, NULL},
		{"root/edge_enhancement/strength", NULL, &SetEEStrength, NULL},
		{"root/edge_enhancement/clip", NULL, &SetEEClip, NULL},
		{"root/pre_edge_enhancement/mode", NULL, &SetPreEEMode, NULL},
		{"root/pre_edge_enhancement/edge_sensitivity", NULL, &SetPreEEdgeSensitivity, NULL},
		{"root/pre_edge_enhancement/noise_sensitivity", NULL, &SetPreEENoiseSensitivity, NULL},
		{"root/pre_edge_enhancement/strength", NULL, &SetPreEEStrength, NULL},
		{"root/pre_edge_enhancement/clip", NULL, &SetPreEEClip, NULL},
		{"root/ircut/enable", NULL, &SetIRCutEnable, NULL},
		{NULL, NULL, NULL}
	};
	TXmlWrapperInitOptions tInitOptions;
	memset(&tInitOptions, 0x0, sizeof(TXmlWrapperInitOptions));

	if (ptAutoSceneState == NULL)
	{
		printf("%s %d :ptAutoSceneState is NULL! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	if (szUpdatedState == NULL)
	{
		printf("%s %d :szUpdatedState is NULL! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	tInitOptions.dwVersion = XMLWRAPPER_VERSION;
	if (XmlWrapper_Initial(&hXmlWrapObj, &tInitOptions) != S_OK)
	{
		return S_FAIL;
	}
	if (XmlWrapper_SetHandler(hXmlWrapObj, ptParsingMap, NULL) != S_OK)
	{
		printf("%s %d : XmlWrapper_SetHandler Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	if ((sRet = XmlWrapper_ReadBuf_UsrDefFunc((BYTE *)szUpdatedState, hXmlWrapObj, ptAutoSceneState)) != S_OK)
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
SCODE VencVideoIn_ISPPROCESS_Handler(TVideoInInfo *ptVideoInInfo,DWORD dwAEShutter,DWORD dwAEGain)
{
	static BOOL bIRCutHaveEnabled = FALSE;
	BOOL bDeNoiseNoChanged = TRUE;
	BOOL bPreDeNoiseNoChanged = TRUE;
	BOOL bEENoChanged = TRUE;
	BOOL bPreEENoChanged = TRUE;
	static BOOL bTWICEIBPE = (BOOL)(-1);
	static BOOL bPrebTWICEIBPE = (BOOL)(-1);
	TAutoSceneState tAutoSceneState;
	TVideoSignalOptions tVideoSignalOptions;
	CHAR szUpdatedState[ SHARED_MEM_LEN];
#if defined(_PRE_NR_)
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
#endif
	memset(&tVideoSignalOptions, 0x0, sizeof(TVideoSignalOptions));
	memset(&tAutoSceneState , -1, sizeof(TAutoSceneState));
	
	if (ptVideoInInfo->bRestISPPROCESSStatus == TRUE)
	{
		bPrebTWICEIBPE = (BOOL)(-1);
		bTWICEIBPE = (BOOL)(-1);
		ptVideoInInfo->bRestISPPROCESSStatus = FALSE;
	}
	if (ptVideoInInfo->bISPUpdateState == TRUE)
	{
		//printf("[venc_videoin_process](%d)bISPUpdateState is TURE!!\n",__LINE__);
		memset(szUpdatedState, 0x0, sizeof(szUpdatedState));

		ptVideoInInfo->bISPUpdateState = FALSE;

		if (read_shm(szUpdatedState, sizeof(szUpdatedState)) == S_OK)
		{
#if 0
			printf("[venc_videoin_process](%d)szUpdatedState:\n",__LINE__);
			printf("[venc_videoin_process](%d)%s\n",__LINE__,szUpdatedState);
			printf("[venc_videoin_process](%d)---------------:\n",__LINE__);
#endif
			if (parse_isp_state(szUpdatedState, &tAutoSceneState) == S_OK)
			{
				bDeNoiseNoChanged = (tAutoSceneState.tYUVNRParam.bEnYUVNR == (BOOL)-1)&& \
								  	(tAutoSceneState.tYUVNRParam.dwNRMode == (DWORD)-1)&& \
								  	(tAutoSceneState.tYUVNRParam.dw2DNRStrength == (DWORD)-1)&& \
								  	(tAutoSceneState.tYUVNRParam.dw3DNRRefStrength == (DWORD)-1)&& \
								  	(tAutoSceneState.tYUVNRParam.bNRMotionAdaptiveEn == (BOOL)-1)&& \
								  	(tAutoSceneState.tYUVNRParam.dw3DNRCurrWeight == (DWORD)-1);
									
				bPreDeNoiseNoChanged = (tAutoSceneState.tYUVPreNRParam.bEnYUVNR == (BOOL)-1)&& \
								  	(tAutoSceneState.tYUVPreNRParam.dwNRMode == (DWORD)-1)&& \
								  	(tAutoSceneState.tYUVPreNRParam.dw2DNRStrength == (DWORD)-1)&& \
								  	(tAutoSceneState.tYUVPreNRParam.dw3DNRRefStrength == (DWORD)-1)&& \
								  	(tAutoSceneState.tYUVPreNRParam.bNRMotionAdaptiveEn == (BOOL)-1)&& \
								  	(tAutoSceneState.tYUVPreNRParam.dw3DNRCurrWeight == (DWORD)-1);					

				bEENoChanged = (tAutoSceneState.tEdgeEnhanceParam.bEnEdgeEnhance == (BOOL)-1)&& \
							   (tAutoSceneState.tEdgeEnhanceParam.dwEdgeSensitivity == (DWORD)-1)&& \
							   (tAutoSceneState.tEdgeEnhanceParam.dwNoiseSensitivity == (DWORD)-1)&& \
							   (tAutoSceneState.tEdgeEnhanceParam.dwEEStrength == (DWORD)-1)&& \
							   (tAutoSceneState.tEdgeEnhanceParam.dwEEClip == (DWORD)-1);
							   
				bPreEENoChanged = (tAutoSceneState.tPreEdgeEnhanceParam.bEnEdgeEnhance == (BOOL)-1)&& \
							   (tAutoSceneState.tPreEdgeEnhanceParam.dwEdgeSensitivity == (DWORD)-1)&& \
							   (tAutoSceneState.tPreEdgeEnhanceParam.dwNoiseSensitivity == (DWORD)-1)&& \
							   (tAutoSceneState.tPreEdgeEnhanceParam.dwEEStrength == (DWORD)-1)&& \
							   (tAutoSceneState.tPreEdgeEnhanceParam.dwEEClip == (DWORD)-1);			   
#if defined(_PRE_NR_)
				if((tAutoSceneState.tYUVPreNRParam.bEnYUVNR == FALSE) && (tAutoSceneState.tPreEdgeEnhanceParam.bEnEdgeEnhance == FALSE))
				{
					bTWICEIBPE = FALSE;
				}
				else
				{
					bTWICEIBPE = TRUE;
				}
#endif
				
#if defined(_PRE_NR_)				
				if (bTWICEIBPE == TRUE)
				{
					
					if (bEENoChanged == FALSE)
					{
#ifdef _AUTOSCENE__DEBUG_
						printf("[venc_videoin_process](%d)(%d)(EdgeEnhanceParam)(mode,edge_sensitivity,noise_sensitivity,strength,clip)=(%d,%d,%d,%d,%d)\n"
								,__LINE__
								,bTWICEIBPE
								,tAutoSceneState.tEdgeEnhanceParam.bEnEdgeEnhance
								,tAutoSceneState.tEdgeEnhanceParam.dwEdgeSensitivity
								,tAutoSceneState.tEdgeEnhanceParam.dwNoiseSensitivity
								,tAutoSceneState.tEdgeEnhanceParam.dwEEStrength
								,tAutoSceneState.tEdgeEnhanceParam.dwEEClip	
							);
#endif
						
						ptIBPOpt->tEdgeEnhancementCfg.bSecondPhaseEdgeEnhancementEnable = tAutoSceneState.tEdgeEnhanceParam.bEnEdgeEnhance;
						ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceClip = tAutoSceneState.tEdgeEnhanceParam.dwEEClip;
						ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceEdgeSensitivity = tAutoSceneState.tEdgeEnhanceParam.dwEdgeSensitivity;
						ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceNoiseSensitivity = tAutoSceneState.tEdgeEnhanceParam.dwNoiseSensitivity;
						ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceStrength = tAutoSceneState.tEdgeEnhanceParam.dwEEStrength;
//#if defined(_PRE_NR_)
						ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
//#endif
					}
					if (bDeNoiseNoChanged == FALSE)
					{
#ifdef _AUTOSCENE__DEBUG_						
						printf("[venc_videoin_process](%d)(%d)(NRParam)(enable,mode,2d_mode_strength,3d_mode_ref_strength,motion_adaptive_enable,3d_mode_cur_weight)=(%d,%d,%d,%d,%d,%d)\n"  \
								,__LINE__
								,bTWICEIBPE
								,tAutoSceneState.tYUVNRParam.bEnYUVNR
								,tAutoSceneState.tYUVNRParam.dwNRMode
								,tAutoSceneState.tYUVNRParam.dw2DNRStrength
								,tAutoSceneState.tYUVNRParam.dw3DNRRefStrength
								,tAutoSceneState.tYUVNRParam.bNRMotionAdaptiveEn
								,tAutoSceneState.tYUVNRParam.dw3DNRCurrWeight
							);
#endif
						ptIBPOpt->tDeNoiseCfg.bSecondPhaseDeNoiseEnable = tAutoSceneState.tYUVNRParam.bEnYUVNR;
						ptIBPOpt->tDeNoiseCfg.eSecondPhaseNoiseReductionMode = tAutoSceneState.tYUVNRParam.dwNRMode;
						ptIBPOpt->tDeNoiseCfg.dwSecondPhaseNoiseReductionNormalStrength = tAutoSceneState.tYUVNRParam.dw2DNRStrength;
						ptIBPOpt->tDeNoiseCfg. dwSecondPhaseNoiseReductionStrongRefStrength = tAutoSceneState.tYUVNRParam.dw3DNRRefStrength;
						ptIBPOpt->tDeNoiseCfg.bSecondPhaseNoiseReductionStrongMotionAdaptiveEn = tAutoSceneState.tYUVNRParam.bNRMotionAdaptiveEn;
						ptIBPOpt->tDeNoiseCfg.dwSecondPhaseNoiseReductionStrongCurrWeight = tAutoSceneState.tYUVNRParam.dw3DNRCurrWeight;
//#if defined(_PRE_NR_)						
						ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
//#endif
					}
					if (bPreEENoChanged == FALSE)
					{
#ifdef _AUTOSCENE__DEBUG_						
						printf("[venc_videoin_process](%d)(%d)(Pre EdgeEnhanceParam)(mode,edge_sensitivity,noise_sensitivity,strength,clip)=(%d,%d,%d,%d,%d)\n"
								,__LINE__
								,bTWICEIBPE
								,tAutoSceneState.tPreEdgeEnhanceParam.bEnEdgeEnhance
								,tAutoSceneState.tPreEdgeEnhanceParam.dwEdgeSensitivity
								,tAutoSceneState.tPreEdgeEnhanceParam.dwNoiseSensitivity
								,tAutoSceneState.tPreEdgeEnhanceParam.dwEEStrength
								,tAutoSceneState.tPreEdgeEnhanceParam.dwEEClip	
							);
#endif
						ptVideoInInfo->tIBPState.bEdgeEnhanceEn = tAutoSceneState.tPreEdgeEnhanceParam.bEnEdgeEnhance;
						ptVideoInInfo->tIBPState.dwEdgeEnhanceEdgeSensitivity = tAutoSceneState.tPreEdgeEnhanceParam.dwEdgeSensitivity;
						ptVideoInInfo->tIBPState.dwEdgeEnhanceNoiseSensitivity = tAutoSceneState.tPreEdgeEnhanceParam.dwNoiseSensitivity;
						ptVideoInInfo->tIBPState.dwEdgeEnhanceStrength = tAutoSceneState.tPreEdgeEnhanceParam.dwEEStrength;
						ptVideoInInfo->tIBPState.dwEdgeEnhanceClip = tAutoSceneState.tPreEdgeEnhanceParam.dwEEClip;
						ptVideoInInfo->tIBPState.bUpdateEn = TRUE;
					}
					if (bPreDeNoiseNoChanged == FALSE)
					{
#ifdef _AUTOSCENE__DEBUG_
						printf("[venc_videoin_process](%d)(%d)(Pre NRParam)(enable,mode,2d_mode_strength,3d_mode_ref_strength,motion_adaptive_enable,3d_mode_cur_weight)=(%d,%d,%d,%d,%d,%d)\n"  \
								,__LINE__
								,bTWICEIBPE
								,tAutoSceneState.tYUVPreNRParam.bEnYUVNR 
								,tAutoSceneState.tYUVPreNRParam.dwNRMode
								,tAutoSceneState.tYUVPreNRParam.dw2DNRStrength
								,tAutoSceneState.tYUVPreNRParam.dw3DNRRefStrength
								,tAutoSceneState.tYUVPreNRParam.bNRMotionAdaptiveEn
								,tAutoSceneState.tYUVPreNRParam.dw3DNRCurrWeight
							);
#endif	
						ptVideoInInfo->tIBPState.bNoiseReductionEn = tAutoSceneState.tYUVPreNRParam.bEnYUVNR;
						ptVideoInInfo->tIBPState.eNoiseReductionMode = tAutoSceneState.tYUVPreNRParam.dwNRMode;
						ptVideoInInfo->tIBPState.dwNoiseReductionNormalStrength = tAutoSceneState.tYUVPreNRParam.dw2DNRStrength;
						ptVideoInInfo->tIBPState.dwNoiseReductionStrongRefStrength = tAutoSceneState.tYUVPreNRParam.dw3DNRRefStrength;
						ptVideoInInfo->tIBPState.bNoiseReductionStrongMotionAdaptiveEn = tAutoSceneState.tYUVPreNRParam.bNRMotionAdaptiveEn;
						ptVideoInInfo->tIBPState.dwNoiseReductionStrongCurrWeight = tAutoSceneState.tYUVPreNRParam.dw3DNRCurrWeight;
						ptVideoInInfo->tIBPState.bUpdateEn = TRUE;
					}
					
				}
				else
#endif
				{
					if (bEENoChanged == FALSE)
					{
#ifdef _AUTOSCENE__DEBUG_					
						printf("[venc_videoin_process](%d)(%d)(EdgeEnhanceParam)(mode,edge_sensitivity,noise_sensitivity,strength,clip)=(%d,%d,%d,%d,%d)\n"
								,__LINE__
								,bTWICEIBPE
								,tAutoSceneState.tEdgeEnhanceParam.bEnEdgeEnhance
								,tAutoSceneState.tEdgeEnhanceParam.dwEdgeSensitivity
								,tAutoSceneState.tEdgeEnhanceParam.dwNoiseSensitivity
								,tAutoSceneState.tEdgeEnhanceParam.dwEEStrength
								,tAutoSceneState.tEdgeEnhanceParam.dwEEClip	
							);
#endif
						ptVideoInInfo->tIBPState.bEdgeEnhanceEn = tAutoSceneState.tEdgeEnhanceParam.bEnEdgeEnhance;
						ptVideoInInfo->tIBPState.dwEdgeEnhanceEdgeSensitivity = tAutoSceneState.tEdgeEnhanceParam.dwEdgeSensitivity;
						ptVideoInInfo->tIBPState.dwEdgeEnhanceNoiseSensitivity = tAutoSceneState.tEdgeEnhanceParam.dwNoiseSensitivity;
						ptVideoInInfo->tIBPState.dwEdgeEnhanceStrength = tAutoSceneState.tEdgeEnhanceParam.dwEEStrength;
						ptVideoInInfo->tIBPState.dwEdgeEnhanceClip = tAutoSceneState.tEdgeEnhanceParam.dwEEClip;
						ptVideoInInfo->tIBPState.bUpdateEn = TRUE;
					}
					if (bDeNoiseNoChanged == FALSE)
					{
#ifdef _AUTOSCENE__DEBUG_						
						printf("[venc_videoin_process](%d)(%d)(NRParam)(enable,mode,2d_mode_strength,3d_mode_ref_strength,motion_adaptive_enable,3d_mode_cur_weight)=(%d,%d,%d,%d,%d,%d)\n"  \
								,__LINE__
								,bTWICEIBPE
								,tAutoSceneState.tYUVNRParam.bEnYUVNR
								,tAutoSceneState.tYUVNRParam.dwNRMode
								,tAutoSceneState.tYUVNRParam.dw2DNRStrength
								,tAutoSceneState.tYUVNRParam.dw3DNRRefStrength
								,tAutoSceneState.tYUVNRParam.bNRMotionAdaptiveEn
								,tAutoSceneState.tYUVNRParam.dw3DNRCurrWeight
							);
#endif
						ptVideoInInfo->tIBPState.bNoiseReductionEn = tAutoSceneState.tYUVNRParam.bEnYUVNR;
						ptVideoInInfo->tIBPState.eNoiseReductionMode = tAutoSceneState.tYUVNRParam.dwNRMode;
						ptVideoInInfo->tIBPState.dwNoiseReductionNormalStrength = tAutoSceneState.tYUVNRParam.dw2DNRStrength;
						ptVideoInInfo->tIBPState.dwNoiseReductionStrongRefStrength = tAutoSceneState.tYUVNRParam.dw3DNRRefStrength;
						ptVideoInInfo->tIBPState.bNoiseReductionStrongMotionAdaptiveEn = tAutoSceneState.tYUVNRParam.bNRMotionAdaptiveEn;
						ptVideoInInfo->tIBPState.dwNoiseReductionStrongCurrWeight = tAutoSceneState.tYUVNRParam.dw3DNRCurrWeight;
						ptVideoInInfo->tIBPState.bUpdateEn = TRUE;
					}
				}
				
				if (tAutoSceneState.bEnIRCut == TRUE)
				{
					if (bIRCutHaveEnabled == FALSE)
					{
						VencVideoIn_EnableIRCut(ptVideoInInfo,TRUE);
						bIRCutHaveEnabled = TRUE;
					}
				}
				else
				{
					if (bIRCutHaveEnabled == TRUE)
					{
						VencVideoIn_EnableIRCut(ptVideoInInfo,FALSE);
						bIRCutHaveEnabled = FALSE;
					}
				}
			}  
		}
	}
#if defined(_PRE_NR_)	
#ifdef _VMA_IBPE	
#ifdef _VMA_IBPE_BACK_DOOR
	if (bPrebTWICEIBPE != bTWICEIBPE)
	{
		Rest3DNRBackDoor(ptVideoInInfo, bTWICEIBPE);
		
		bPrebTWICEIBPE = bTWICEIBPE;
		
	}
#endif
#endif
#endif
#ifdef _PRE_NR_	
	if (bTWICEIBPE == TRUE)
	{
		
		VencVideoIn_TWICEIBPE_Handler(ptVideoInInfo);
	}
	else
#endif		
	{
		VencVideoIn_IBPEONLY_Handler(ptVideoInInfo);
	}
	return S_OK;
}
#endif

/* ============================================================================================= */
#ifdef _VMA_IBPE
/* ============================================================================================= */
void VencVideoIn_MotionModification(TVideoInInfo *ptVideoInInfo)
{
	// change Motion Xscale, Yscale, for the real resolution, and setoption ( translate is the same)
	ptVideoInInfo->tMtnR2D.fXScale = (FLOAT)(2) / ptVideoInInfo->dwCurtCapWidth;
	ptVideoInInfo->tMtnR2D.fYScale = (FLOAT)(-2) / ptVideoInInfo->dwCurtCapHeight;
	printf("[VENC_VIDEOIN] change MtnR2D Xscale, YScale to (%.8f, %.8f) !!\n", ptVideoInInfo->tMtnR2D.fXScale, ptVideoInInfo->tMtnR2D.fYScale);
}

/* ============================================================================================= */
void VencVideoIn_ImgBackEndProcessModification(TVideoInInfo *ptVideoInInfo)
{
	// IBPE geometric parameter modify
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	DWORD	dwWidth, dwHeight;
	EGeoMtcMode	eGMSelectMode;
	EGeoMtcRes	eGMCurResId;

	dwWidth = ptVideoInInfo->dwCurtCapWidth;
	dwHeight = ptVideoInInfo->dwCurtCapHeight;

	ptVideoInInfo->tIBPState.dwWidth = dwWidth;
	ptVideoInInfo->tIBPState.dwHeight = dwHeight;
	printf("[VENC_VIDEOIN]ptVideoInInfo->tIBPState.dwCurrWidth=%d\n",ptVideoInInfo->tIBPState.dwWidth);
	printf("[VENC_VIDEOIN]ptVideoInInfo->tIBPState.dwCurrHeight=%d\n",ptVideoInInfo->tIBPState.dwHeight);
	eGMSelectMode = ptIBPOpt->tGeoMtcCfg.eGMSelectMode;
	if ((dwWidth == 640) && (dwHeight == 480)) {
		eGMCurResId = geomtcres640x480;
	} else if ((dwWidth == 1280) && (dwHeight == 720)) {
		eGMCurResId = geomtcres1280x720;
	} else if ((dwWidth == 1280) && (dwHeight == 800)) {
		eGMCurResId = geomtcres1280x800;
	} else if ((dwWidth == 1280) && (dwHeight == 1024)) {
		eGMCurResId = geomtcres1280x1024;
	} else if ((dwWidth == 1920) && (dwHeight == 1080)) {
		eGMCurResId = geomtcres1920x1080;
	} else if ((dwWidth == 2048) && (dwHeight == 1536)) {
		eGMCurResId = geomtcres2048x1536;	
	} else if ((dwWidth == 2560) && (dwHeight == 1920)) {
		eGMCurResId = geomtcres2560x1920;
	} else {
		eGMCurResId = geomtcres640x480;
	}

	if (ptIBPOpt->tGeoMtcCfg.bEnable == TRUE) 
	{	
		// if disable, no array entry can retrieve.
		// according eGMSelectMode and eGMCurResId to get the correct geometric parameter
		if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].bPosFwdMpgParam1Y == TRUE)
		{
			ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionYParam1 = (SDWORD) ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].dwFwdMpgParam1Y;
		}
		else
		{
			ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionYParam1 = (SDWORD)(0-(SDWORD)ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].dwFwdMpgParam1Y);
		}

		if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].bPosFwdMpgParam2Y == TRUE)
		{
			ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionYParam2 = (SDWORD) ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].dwFwdMpgParam2Y;
		}
		else
		{
			ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionYParam2 = (SDWORD)(0-(SDWORD)ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].dwFwdMpgParam2Y);
		}
		
		if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].bPosFwdMpgParam3Y == TRUE)
		{
			ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionYParam3 = (SDWORD) ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].dwFwdMpgParam3Y;
		}
		else
		{
			ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionYParam3 = (SDWORD)(0-(SDWORD)ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].dwFwdMpgParam3Y);
		}


		if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].bPosFwdMpgParam1CbCr == TRUE)
		{
			ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionCParam1 = (SDWORD)ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].dwFwdMpgParam1CbCr;
		}
		else
		{
			ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionCParam1 = (SDWORD)(0-(SDWORD)ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].dwFwdMpgParam1CbCr);
		}

		if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].bPosFwdMpgParam2CbCr == TRUE)
		{
			ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionCParam2 = (SDWORD)ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].dwFwdMpgParam2CbCr;
		}
		else
		{
			ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionCParam2 = (SDWORD)(0-(SDWORD)ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].dwFwdMpgParam2CbCr);
		}


		if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].bPosFwdMpgParam3CbCr == TRUE)
		{
			ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionCParam3 = (SDWORD)ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].dwFwdMpgParam3CbCr;
		}
		else
		{
			ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionCParam3 = (SDWORD)(0-(SDWORD)ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMSelectMode][eGMCurResId].dwFwdMpgParam3CbCr);
		}

	}
}
#endif
/* ============================================================================================= */
void VencVideoIn_ResolutionChange(TVideoInInfo *ptVideoInInfo)
{	
#ifdef _VMA_IBPE
	// modify motion relative
	VencVideoIn_MotionModification(ptVideoInInfo);
	// IBPE modify
	VencVideoIn_ImgBackEndProcessModification(ptVideoInInfo);
#endif
}

/* ========================================================================== */
void VencVideoIn_MsgApplyOpt(TVideoInInfo *ptVideoInInfo)
{
	TVideoInMsgOptRec *ptMsgOptRec = &(ptVideoInInfo->tMsgOptRec);

	if (ptMsgOptRec->bDumpSharedBuf == TRUE) {
		ptMsgOptRec->bDumpSharedBuf = FALSE;
		ptVideoInInfo->bDumpSharedBuf = TRUE;
	}

	if (ptMsgOptRec->bDumpVideoCapBuf == TRUE) {
		ptMsgOptRec->bDumpVideoCapBuf = FALSE;
		ptVideoInInfo->bDumpVideoCapBuf = TRUE;
	}

	if (ptMsgOptRec->bRetVideoCapStatus == TRUE) {
		ptMsgOptRec->bRetVideoCapStatus = FALSE;
		ptVideoInInfo->bRetVideoCapStatus = TRUE;
	}    

	if (ptMsgOptRec->bPhotoMtcDefTbl1 == TRUE) {
		ptMsgOptRec->bPhotoMtcDefTbl1 = FALSE;
		ptVideoInInfo->bPhotoMtcDefTbl1 = TRUE;
	}

	if (ptMsgOptRec->bPhotoMtcDefTbl2 == TRUE) {
		ptMsgOptRec->bPhotoMtcDefTbl2 = FALSE;
		ptVideoInInfo->bPhotoMtcDefTbl2 = TRUE;
	}
	if (ptMsgOptRec->bDumpAFStatus == TRUE){
		ptMsgOptRec->bDumpAFStatus = FALSE;
		ptVideoInInfo->bDumpAFStatus = TRUE;
	}
	if (ptMsgOptRec->bKeepCurrentWB == TRUE) {
		ptMsgOptRec->bKeepCurrentWB = FALSE;
		ptVideoInInfo->bKeepCurrentWB = TRUE;
	}

	if (ptMsgOptRec->bKeepCurrentWBFromFile == TRUE){
		ptMsgOptRec->bKeepCurrentWBFromFile = FALSE;
		ptVideoInInfo->bKeepCurrentWBFromFile = TRUE;
	}

	if (ptMsgOptRec->bTurnOnFRCalculate == TRUE) {
		ptMsgOptRec->bTurnOnFRCalculate = FALSE;
		ptVideoInInfo->bFRCalculate = TRUE;
		ptVideoInInfo->dwFRCalculateFrameCount = 0;
	}

	if (ptMsgOptRec->bTurnOffFRCalculate == TRUE) {
		ptMsgOptRec->bTurnOffFRCalculate = FALSE;
		ptVideoInInfo->bFRCalculate = FALSE;
		ptVideoInInfo->dwFRCalculateFrameCount = 0;
	}

	if (ptMsgOptRec->bClearIBPEInternalBuffer == TRUE) {
		ptMsgOptRec->bClearIBPEInternalBuffer = FALSE;
		ptVideoInInfo->bClearIBPEInternalBuffer = TRUE;
	}
#if defined(_AUTOSCENE_)
	if (ptMsgOptRec->bISPUpdateState == TRUE)
	{
		ptVideoInInfo->bISPUpdateState = TRUE;
		ptMsgOptRec->bISPUpdateState = FALSE;
		// ptVideoInInfo->bStatusChange = TRUE;
	}
	else{
		if (ptMsgOptRec->bISPEnable == TRUE)
		{	
			// printf("(%d)ptMsgOptRec->bISPEnable==TRUE\n",__LINE__);
			ptVideoInInfo->bISPEnable = TRUE;
			ptVideoInInfo->bStatusChange = TRUE;
			//ptMsgOptRec->bISPEnable = FALSE;
		}
		if (ptMsgOptRec->bISPEnable == FALSE)
		{	
			// printf("(%d)ptMsgOptRec->bISPEnable==FALSE\n",__LINE__);
			ptVideoInInfo->bISPEnable = FALSE;
			ptVideoInInfo->bStatusChange = TRUE;
			//ptMsgOptRec->bISPEnable = FALSE;
		}
	}
#endif
	//memset(ptMsgOptRec,0x0,sizeof( TVideoInMsgOptRec));
}

/* ============================================================================================= */
void VencVideoIn_Process(HANDLE hObject)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	SCODE	scResult;
	TVideoCapState *ptVideoCapState;
	//TVideoCapState tVideoCapState;
	TVideoFrameBuffer *ptOutVFB;
	static struct timeval pretv, tv;
	DWORD dwAEGain = 0;
	DWORD dwAEShutter = 0;
	DWORD dwSecond = 0;
	DWORD dwMilliSecond = 0;
	BOOL bFrameSkip = FALSE;
#if defined(_VMA_IBPE)	
	static EVideoInStatusType eviPreStatus = evsobsEmpty;
#endif
	int i = 0;
	
	//printf("[venc_videoin_process](%d)ptVideoInInfo->tIBPState.dwValidRefBuffNum=%d\n",__LINE__,ptVideoInInfo->tIBPState.dwValidRefBuffNum);
	for (i = 0; i < BUFF_SIZE; i++) {
		ptVideoInInfo->adwVideoCapIndex[i] = -1;
	}
	for (i = 0; i < BUFF_SIZE; i++) {
		memset(&ptVideoInInfo->atVideoCapState[i], 0, sizeof(TVideoCapState));
		memset(&ptVideoInInfo->atVideoCapOutVFB[i], 0, sizeof(TVideoFrameBuffer));
	}

	SharedBuffMgr_GetFileDescriptor(ptVideoInInfo->hSharedBuferObj, &(ptVideoInInfo->iShardBufferFD));
	while(1) {
		sem_wait(&(ptVideoInInfo->smStart));
		if (ptVideoInInfo->bTerminateThread == TRUE) break;

		DBPRINT0("[VENC_VIDEOIN] VideoIn Loop Start\n");

		while(ptVideoInInfo->bRunning == TRUE) {
			//bReConf_flag should set to true when first running
			if (ptVideoInInfo->bReConf_flag == TRUE) {
				sem_wait(&(ptVideoInInfo->smOptions));
#if defined(_AUTOSCENE_)
				VencVideoIn_InitConfigParam(ptVideoInInfo);
#endif
				VencVideoIn_LoadConfig(hObject);
				ptVideoInInfo->bReConf_flag = FALSE;
				sem_post(&(ptVideoInInfo->smOptions));
			}
			if (ptVideoInInfo->bReMsg_flag == TRUE) {
				sem_wait(&(ptVideoInInfo->smMsgOptions));
				VencVideoIn_MsgApplyOpt(ptVideoInInfo);
#if defined(_AUTOSCENE_)
				//When the initial eviStatus is NOPREPROCESS, and then switch to ISPPROCESS.
				//The parameters of IBPE state should be set from XML.
				if (ptVideoInInfo->bISPEnable == TRUE) {
					VencVideoIn_LoadConfig(hObject);
					VencVideoIn_ImgBackEndProcessSetoptions(ptVideoInInfo);
				}				
#endif
				ptVideoInInfo->bReMsg_flag = FALSE;
				sem_post(&(ptVideoInInfo->smMsgOptions));
			}

			if (ptVideoInInfo->bFRCalculate == TRUE) {
				if (ptVideoInInfo->dwFRCalculateFrameCount == 0) {
					gettimeofday(&pretv, NULL);
				}
			}
			if (ptVideoInInfo->bOptionsModified == TRUE) { // IBPE & DeInterlace will change it to TRUE
				// some thing to do .... ex: apply option
				VencVideoIn_ApplyOption(ptVideoInInfo);
				ptVideoInInfo->bOptionsModified = FALSE;
			}
#ifdef _VMA_IBPE
			if (ptVideoInInfo->bPMaskReconfig == TRUE) {
				VencVideoIn_PMaskSetoptions(ptVideoInInfo);
				ptVideoInInfo->bPMaskReconfig = FALSE;
			}
#endif
#ifdef _VMA_IBPE
			if (ptVideoInInfo->bIBPReconfig == TRUE) {
				VencVideoIn_ImgBackEndProcessSetoptions(ptVideoInInfo);
				ptVideoInInfo->bIBPReconfig = FALSE;
			}
#endif
			if (ptVideoInInfo->bPhotoMtcReconfig == TRUE) {
				VencVideoIn_PhotoMtcSetoptions(ptVideoInInfo);
				ptVideoInInfo->bPhotoMtcReconfig = FALSE;
				//printf("[VENC_VIDEOIN] Photometric reset\n");
			}
			// change status 
			if (ptVideoInInfo->bStatusChange) {
				ptVideoInInfo->eviStatus = NOPREPROCESS;
#if defined(_AUTOSCENE_)	
				if (ptVideoInInfo->bISPEnable == TRUE)
				{
					//The ISPPROCESS has the fist priority.
					ptVideoInInfo->eviStatus = ISPPROCESS;
					goto init_empty_buffer;
				}
#endif
#if defined(_VMA_IBPE)

#if defined(_PRE_NR_)
				if (ptVideoInInfo->tIBPOpt.bTWICEIBPEEnable == TRUE) { 
					ptVideoInInfo->eviStatus = TWICEIBPE;
					goto init_empty_buffer;
				}
#endif
				if (ptVideoInInfo->tIBPOpt.bIBPEEnable == TRUE) { 
					ptVideoInInfo->eviStatus = IBPEONLY;					
				}
				else { 
					ptVideoInInfo->eviStatus = NOPREPROCESS;
				}
#endif
#if defined(_AUTOSCENE_) || defined(_PRE_NR_)
init_empty_buffer:
#endif
				printf("[VENC_VIDEOIN](%d)ptVideoInInfo->eviStatus=%d\n",__LINE__,ptVideoInInfo->eviStatus);
				
				ptVideoInInfo->bStatusChange = FALSE;
			}
			// try to get buffer
			// release it
			if ((scResult = VencVideoIn_TryGetBuf(ptVideoInInfo,  &ptVideoCapState)) == S_OK) 
			{
#if 0
				printf("FrameCount = %u, %u, Time = %u:%u:%u, size=%ux%u\n",                
						ptVideoCapState->dwFrameCount,
						ptVideoCapState->dwIndex,
						ptVideoCapState->dwSecond,
						ptVideoCapState->dwMilliSecond,
						ptVideoCapState->dwMicroSecond,
						ptVideoCapState->dwOutWidth,
						ptVideoCapState->dwOutHeight);
#endif
			
			 if (ptVideoInInfo->bFrameRateCtrlEnable == TRUE)
			 {

					TFrameRateCtrlState tFRCtrlState;
					memset(&tFRCtrlState,0x0,sizeof(TFrameRateCtrlState));
					tFRCtrlState.dwSecond = ptVideoCapState->dwSecond;
					tFRCtrlState.dwMilliSecond = ptVideoCapState->dwMilliSecond;
					tFRCtrlState.dwMicroSecond = ptVideoCapState->dwMicroSecond;
					FrameRateCtrl_OneFrame(ptVideoInInfo->hFRCtrlObj, &tFRCtrlState);
					bFrameSkip = FALSE;
					if (tFRCtrlState.bAvailable == FALSE) {
						bFrameSkip = TRUE;
						goto release_pre_buffer;
					}
				}
				
				if ((ptVideoInInfo->dwCurtCapWidth != ptVideoCapState->dwOutWidth) 
						|| (ptVideoInInfo->dwCurtCapHeight != ptVideoCapState->dwOutHeight)) 
				{
#if 1
					printf("Previous Captured size: %4u x %4u \n",
							ptVideoInInfo->dwCurtCapWidth,
							ptVideoInInfo->dwCurtCapHeight);
					printf("Current Captured size: %4u x %4u \n",
							ptVideoCapState->dwOutWidth,
							ptVideoCapState->dwOutHeight);
#endif
					ptVideoInInfo->dwCurtCapWidth = ptVideoCapState->dwOutWidth;
					ptVideoInInfo->dwCurtCapHeight = ptVideoCapState->dwOutHeight;
					VencVideoIn_ResolutionChange(ptVideoInInfo);
				}

				// because motion setoption should know the current resolution, so, it should be called after change resolution.
				if (ptVideoInInfo->bMotionReconfig == TRUE) {
#ifdef _VMA_IBPE
					VencVideoIn_MotionSetoptions(ptVideoInInfo);
#endif               
					ptVideoInInfo->bMotionReconfig = FALSE;
				}

				ptOutVFB = &ptVideoInInfo->atVideoCapOutVFB[ptVideoInInfo->adwVideoCapIndex[0]]; 

				dwSecond = ptVideoCapState->dwSecond;
				dwMilliSecond = ptVideoCapState->dwMilliSecond;

				dwAEGain = ptVideoCapState->dwAEGain;
				dwAEShutter = ptVideoCapState->dwAEShutter;
				
				if (ptVideoInInfo->bRetVideoCapStatus == TRUE) {
					ptVideoInInfo->bRetVideoCapStatus = FALSE;
					VencVideoIn_RetrieveVideoCapStatus(ptVideoInInfo, ptVideoCapState);
				}

				if (ptVideoInInfo->bPhotoMtcDefTbl1 == TRUE) {
					ptVideoInInfo->bPhotoMtcDefTbl1 = FALSE;
					VencVideoIn_PhotoMtcDefTbl(ptVideoInInfo, ptOutVFB->pbyBufY, photomtcmUserDefine1);
				}

				if (ptVideoInInfo->bPhotoMtcDefTbl2 == TRUE) {
					ptVideoInInfo->bPhotoMtcDefTbl2 = FALSE;
					VencVideoIn_PhotoMtcDefTbl(ptVideoInInfo, ptOutVFB->pbyBufY, photomtcmUserDefine2);
				}
				if (ptVideoInInfo->bDumpAFStatus == TRUE)
				{
					ptVideoInInfo->bDumpAFStatus = FALSE;
					VencVideoIn_DumpAFStatus(ptVideoInInfo);
				}
				if (ptVideoInInfo->bKeepCurrentWB == TRUE)
				{
					ptVideoInInfo->bKeepCurrentWB  = FALSE;
					VencVideoIn_KeepCurrentWB(ptVideoInInfo);
				}
				if (ptVideoInInfo->bKeepCurrentWBFromFile == TRUE)
				{
					ptVideoInInfo->bKeepCurrentWBFromFile = FALSE;
					VencVideoIn_KeepCurrentWBFromFile(ptVideoInInfo);
				}

				if (ptVideoInInfo->bDumpVideoCapBuf == TRUE) {
					// dump file here.
					FILE *pfDumpYUV;
					CHAR szTemp[64];
					DWORD dwDumpSize;

					ptVideoInInfo->bDumpVideoCapBuf = FALSE;
#if defined (_YUV422_)			
					sprintf(szTemp, "/tmp/RawDatafromVideoIn%ux%u_%dx%d_422.yuv", ptOutVFB->dwSecond, ptOutVFB->dwMilliSecond, ptOutVFB->dwStride, g_avfInfo[ptOutVFB->vFormat].wEncHeight);
#else
					sprintf(szTemp, "/tmp/RawDatafromVideoIn%ux%u_%dx%d_420.yuv", ptOutVFB->dwSecond, ptOutVFB->dwMilliSecond, ptOutVFB->dwStride, g_avfInfo[ptOutVFB->vFormat].wEncHeight);
#endif
					if ((pfDumpYUV = fopen(szTemp, "wb")) == NULL) {
						printf("[VENC_VIDEOIN] open %s fail..... !!!\n", szTemp);
					} else {
						dwDumpSize = ptOutVFB->dwStride * g_avfInfo[ptOutVFB->vFormat].wEncHeight;
						fwrite(ptOutVFB->pbyBufY, 1, dwDumpSize, pfDumpYUV);
#if defined (_YUV422_)						
						fwrite(ptOutVFB->pbyBufU, 1, dwDumpSize >> 1, pfDumpYUV);
						fwrite(ptOutVFB->pbyBufV, 1, dwDumpSize >> 1, pfDumpYUV);
#else
						fwrite(ptOutVFB->pbyBufU, 1, dwDumpSize >> 2, pfDumpYUV);
						fwrite(ptOutVFB->pbyBufV, 1, dwDumpSize >> 2, pfDumpYUV);
#endif
						fclose(pfDumpYUV);
					}
				}
			}
			if (scResult == S_OK) {
				
				if (ptVideoInInfo->bClearIBPEInternalBuffer == TRUE)
				{
#ifdef _VMA_IBPE					
					TImgBackEndProcessOptions tOptions;
					memset(&tOptions,0x0,sizeof(TImgBackEndProcessOptions));
					tOptions.eOptionFlags = VIDEO_CLEAN_INTERNAL_BUFFER;
					tOptions.apvUserData[0] = (PVOID)( &(ptVideoInInfo->tIBPState));
					if (ImgBackEndProcess_SetOptions(ptVideoInInfo->hIBPObj, &tOptions) != S_OK)
					{
						printf("[venc_videoin_process](%d)Failed to clean internal reference frame buffer!\n", __LINE__);
					}
					else
					{
						printf("[venc_videoin_process](%d)Success to clean internal reference frame buffer!\n", __LINE__);
					}
											
#ifdef _PRE_NR_							
					memset(&tOptions,0x0,sizeof(TImgBackEndProcessOptions));
					tOptions.eOptionFlags = VIDEO_CLEAN_INTERNAL_BUFFER;
					tOptions.apvUserData[0] = (PVOID)( &(ptVideoInInfo->tSecondPhaseIBPState));
					if (ImgBackEndProcess_SetOptions(ptVideoInInfo->hSencodPhaseIBPObj, &tOptions) != S_OK)
					{
						printf("[venc_videoin_process](%d)Failed to clean internal reference frame buffer!\n", __LINE__);
					}
					else
					{
						printf("[venc_videoin_process](%d)Success to clean internal reference frame buffer!\n", __LINE__);
					}
#endif	
#endif
					ptVideoInInfo->bClearIBPEInternalBuffer = FALSE;
				}
#if defined(_VMA_IBPE)				
				if (ptVideoInInfo->tIBPState.bUpdateEn == TRUE)
				{
					//printf("[venc_videoin_process](%d)ptVideoInInfo->tIBPState.bUpdateEn==%d\n",__LINE__,ptVideoInInfo->tIBPState.bUpdateEn);
#ifdef _PRE_NR_						
					ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif					
				}
#endif
				if ( ptVideoInInfo->eviStatus == TWICEIBPE) {
#if defined(_VMA_IBPE)
#ifdef _PRE_NR_
					VencVideoIn_TWICEIBPE_Handler(ptVideoInInfo);
#endif
#endif					
				} else if (ptVideoInInfo->eviStatus == IBPEONLY) {
#if defined(_VMA_IBPE)					
					//printf("[VIDEOIN_PROCESS](%d)IBPEONLY\n",__LINE__);
					VencVideoIn_IBPEONLY_Handler(ptVideoInInfo);
#endif
				} else if (ptVideoInInfo->eviStatus == ISPPROCESS){
#if defined(_AUTOSCENE_)
					//printf("[VIDEOIN_PROCESS](%d)ISPPROCESS\n",__LINE__);
					//This handler is for servicing the autoscene staff.
					VencVideoIn_ISPPROCESS_Handler(ptVideoInInfo,dwAEShutter,dwAEGain);
#endif			
				} else {	
					//NOPREPROCESS
					//printf("[VIDEOIN_PROCESS](%d)NOPREPROCESS\n",__LINE__);
					//VencVideoIn_NOPREPROCESS_Handler(ptVideoInInfo);
#ifdef _YUV422_
					#if defined(_VMA_IBPE)	
						VencVideoIn_IBPEONLY_Handler(ptVideoInInfo);
					#else
						VencVideoIn_NOPREPROCESS_Handler(ptVideoInInfo);
					#endif
#else
					VencVideoIn_NOPREPROCESS_Handler(ptVideoInInfo);
#endif
				}
				
#if defined(_VMA_IBPE)				
				if (eviPreStatus != ptVideoInInfo->eviStatus)
				{
					//BOOL bDoClearBuffer = (eviPreStatus == NOPREPROCESS) && ((ptVideoInInfo->eviStatus == IBPEONLY) || (ptVideoInInfo->eviStatus == ISPPROCESS) || (ptVideoInInfo->eviStatus == TWICEIBPE));
					//if (bDoClearBuffer == TRUE)
					{
						TImgBackEndProcessOptions tOptions;
						memset(&tOptions,0x0,sizeof(TImgBackEndProcessOptions));
						tOptions.eOptionFlags = VIDEO_CLEAN_INTERNAL_BUFFER;
						tOptions.apvUserData[0] = (PVOID)( &(ptVideoInInfo->tIBPState));
						if (ImgBackEndProcess_SetOptions(ptVideoInInfo->hIBPObj, &tOptions) != S_OK)
						{
							printf("[venc_videoin_process](%d)Failed to clean internal reference frame buffer!\n", __LINE__);
						}
						else
						{
							printf("[venc_videoin_process](%d)Success to clean internal reference frame buffer!\n", __LINE__);
						}
						
#ifdef _PRE_NR_							
						
						memset(&tOptions,0x0,sizeof(TImgBackEndProcessOptions));
						tOptions.eOptionFlags = VIDEO_CLEAN_INTERNAL_BUFFER;
						tOptions.apvUserData[0] = (PVOID)( &(ptVideoInInfo->tSecondPhaseIBPState));
						if (ImgBackEndProcess_SetOptions(ptVideoInInfo->hSencodPhaseIBPObj, &tOptions) != S_OK)
						{
							printf("[venc_videoin_process](%d)Failed to clean internal reference frame buffer!\n", __LINE__);
						}
						else
						{
							printf("[venc_videoin_process](%d)Success to clean internal reference frame buffer!\n", __LINE__);
						}
#endif						
						
					}
					
					if (ptVideoInInfo->eviStatus == ISPPROCESS)
					{
#if defined(_AUTOSCENE_)						
						ptVideoInInfo->bRestISPPROCESSStatus = TRUE;
#endif
					}
					else
					{
						if (ptVideoInInfo->eviStatus == TWICEIBPE)
							Rest3DNRBackDoor(ptVideoInInfo, TRUE);
						else if (ptVideoInInfo->eviStatus == IBPEONLY)
							Rest3DNRBackDoor(ptVideoInInfo, FALSE);
					}

					eviPreStatus = ptVideoInInfo->eviStatus;
				}
				if (ptVideoInInfo->tIBPState.bUpdateEn == TRUE)
				{
					ptVideoInInfo->tIBPState.bUpdateEn = FALSE;
					//printf("[venc_videoin_process](%d)ptVideoInInfo->tIBPState.bUpdateEn==%d\n",__LINE__,ptVideoInInfo->tIBPState.bUpdateEn);
				}

#endif
				
release_pre_buffer:
				VencVideoIn_ReleaseInputBuffer(ptVideoInInfo, ptVideoInInfo->atVideoCapState[ptVideoInInfo->adwVideoCapIndex[0]].dwIndex);
			}
			if ((ptVideoInInfo->bFRCalculate == TRUE) && (bFrameSkip==FALSE)) {
				if (ptVideoInInfo->dwFRCalculateFrameCount == 99) {
					float fltotal_time = 0.0;
					gettimeofday(&tv, NULL);
					fltotal_time = ((tv.tv_sec - pretv.tv_sec)*1000000+tv.tv_usec)-pretv.tv_usec;
					printf("VideoIn (pid=%d): %u usec per 100fs (fps = %6f)\n",getpid(),(DWORD)fltotal_time, 100000000/fltotal_time);
					syslog(LOG_INFO, "VideoIn (pid=%d): %u usec per 100fs, (fps = %6f)\n",getpid(),(DWORD)fltotal_time, 100000000/fltotal_time);
					ptVideoInInfo->dwFRCalculateFrameCount = 0;
				} else {
					ptVideoInInfo->dwFRCalculateFrameCount++;
				}
			}			
		}
		// release the hold buffer 
		{
			int i = 0;
			for (i = 0; i < BUFF_SIZE-1; i++) {
				DWORD dwCurtVideoCapIndex = 0;
				dwCurtVideoCapIndex = ptVideoInInfo->atVideoCapState[ptVideoInInfo->adwVideoCapIndex[i+1]].dwIndex;
				VencVideoIn_ReleaseInputBuffer(ptVideoInInfo, dwCurtVideoCapIndex);                
			}
		}
		ptVideoInInfo->bExitInnerLoop = TRUE;
		DBPRINT0("[VENC_VIDEOIN] VideoIn Exit inner loop\n");
	}

	sem_post(&(ptVideoInInfo->smEnd));
	DBPRINT0("[VENC_VIDEOIN] VideoIn Exit thread\n");
}

/* ========================================================================= */
SCODE VencVideoIn_ApplyOption(TVideoInInfo *ptVideoInInfo)
{
	memset(&(ptVideoInInfo->tVideoInOptRec), 0, sizeof(TVideoInOptRec));
	return S_OK;
}

/* ========================================================================= */
void VencVideoIn_SetOptions(HANDLE hVideoInProcess, TVideoInOptions tVideoInOpt)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hVideoInProcess;
	if (ptVideoInInfo == NULL) {
		fprintf(stderr, "[VENC_VIDEOIN] VideoIn setoptions fail: NULL Pointer!!\n");
		return;
	}

	if (tVideoInOpt.eviOptType == VENCVIDEOIN_OPTION_SET_MOTIONTHRESHOLD) {
		ptVideoInInfo->tVideoInOptRec.bMotionThresholdChange = TRUE;
		ptVideoInInfo->tVideoInOptRec.dwMotionThreshold = tVideoInOpt.adwUserData[0];
	} else if (tVideoInOpt.eviOptType == VENCVIDEOIN_OPTION_SET_STATICPERIOD) {
		ptVideoInInfo->tVideoInOptRec.bStaticPeriodChange = TRUE;
		ptVideoInInfo->tVideoInOptRec.dwStaticPeriod = tVideoInOpt.adwUserData[0];
	} else if (tVideoInOpt.eviOptType == VENCVIDEOIN_OPTION_SET_RESETSTATISTICS) {
		ptVideoInInfo->tVideoInOptRec.bResetStatisticsChange = TRUE;
		ptVideoInInfo->tVideoInOptRec.bResetStatistics = tVideoInOpt.adwUserData[0];
	} else if (tVideoInOpt.eviOptType == VENCVIDEOIN_OPTION_SET_DEMO) {
		ptVideoInInfo->tVideoInOptRec.bDemoChange = TRUE;
		ptVideoInInfo->tVideoInOptRec.bDemo = tVideoInOpt.adwUserData[0];
	} else if (tVideoInOpt.eviOptType == VENCVIDEOIN_OPTION_SET_SUBTITLEDETECT) {
		ptVideoInInfo->tVideoInOptRec.bSubtitleDetectChange = TRUE;
		ptVideoInInfo->tVideoInOptRec.bSubtitleDetect = tVideoInOpt.adwUserData[0];
	} else if (tVideoInOpt.eviOptType == VENCVIDEOIN_OPTION_SET_TMPRLNR) {
		ptVideoInInfo->tVideoInOptRec.bTmprlNRChange = TRUE;
		ptVideoInInfo->tVideoInOptRec.bTmprlNR = tVideoInOpt.adwUserData[0];
	}

	ptVideoInInfo->bOptionsModified = TRUE;
	return;
}

/* ============================================================================================= */
SCODE VencVideoIn_LoadConfig(HANDLE hVideoInProcObj)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)(hVideoInProcObj);

	if (ptVideoInInfo == NULL)
		return S_FAIL;

	DBPRINT1("[VENC_VIDEOIN] XmlWrapper read config %s \n", ptVideoInInfo->szConfigFile);
	if (XmlWrapper_ReadFile_UsrDefFunc(ptVideoInInfo->szConfigFile, ptVideoInInfo->hXMLWrapperObj, ptVideoInInfo) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] XmlWrapper reload config fail! \n");
		return S_FAIL;
	}

	if (XmlWrapper_Reset(ptVideoInInfo->hXMLWrapperObj) != S_OK)  {
		DBPRINT0("[VENC_VIDEOIN] XmlWrapper read config fail! \n");
		return S_FAIL;
	}
	VencVideoIn_PhotoMtcReConfig(ptVideoInInfo);

#ifdef _VMA_IBPE
	VencVideoIn_SetImgBackEndProcessReConfig(ptVideoInInfo);
	VencVideoIn_SetMotionReConfig(ptVideoInInfo);
#endif
#ifdef _VMA_IBPE
	VencVideoIn_SetPMaskReConfig(ptVideoInInfo);
#endif

	return S_OK;
}
#ifdef _VMA_IBPE
/* =========================================================================================== */
SCODE VencVideoIn_SetImgBackEndProcessReConfig(HANDLE hVideoInProcObj)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)(hVideoInProcObj);
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	static DWORD dwTWICEIBPEEnable = -1;
	
	
	if ( ((DWORD)ptVideoInInfo->tIBPOpt.bTWICEIBPEEnable) !=  dwTWICEIBPEEnable)
	{
		ptVideoInInfo->bStatusChange = TRUE;
		dwTWICEIBPEEnable = ptVideoInInfo->tIBPOpt.bTWICEIBPEEnable;
		
	}
	
	if ((ptIBPOpt->tEdgeEnhancementCfg.bSecondPhaseEdgeEnhancementEnable == TRUE) || 
			(ptIBPOpt->tDeNoiseCfg.bSecondPhaseDeNoiseEnable == TRUE) || 
			(ptIBPOpt->tGeoMtcCfg.bEnable == TRUE) ||  
			(ptVideoInInfo->tPMaskCfg.bEnable==TRUE)||
			(ptIBPOpt->tDeiCfg.bDeInterlacerEnable==TRUE) ||
			(ptVideoInInfo->tMotionCfg.bMotionEnabled==TRUE)) 
	{
		if (ptVideoInInfo->tIBPOpt.bIBPEEnable == FALSE) {
			ptVideoInInfo->bStatusChange = TRUE;
		}
		ptVideoInInfo->tIBPOpt.bIBPEEnable = TRUE;
		ptVideoInInfo->bIBPReconfig = TRUE;
	} else {
		if (ptVideoInInfo->tIBPOpt.bIBPEEnable == TRUE) {
			ptVideoInInfo->bStatusChange = TRUE;
		}
		ptVideoInInfo->tIBPOpt.bIBPEEnable = FALSE;
		ptVideoInInfo->bIBPReconfig = FALSE;
	}
	if (ptVideoInInfo->tIBPOpt.bTWICEIBPEEnable == TRUE)
	{
			ptVideoInInfo->bIBPReconfig = TRUE;
	}
	//printf("bStatusChange = %d\n",ptVideoInInfo->bStatusChange);
	return S_OK;
}

/* =========================================================================================== */
SCODE VencVideoIn_SetMotionReConfig(HANDLE hVideoInProcObj)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)(hVideoInProcObj);
	// every time, it must set to TRUE 'cause of checking all status
	ptVideoInInfo->bMotionReconfig = TRUE;
	return S_OK;    
}

#endif
#ifdef _VMA_IBPE
/* =========================================================================================== */
SCODE VencVideoIn_SetPMaskReConfig(HANDLE hVideoInProcObj)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)(hVideoInProcObj);

	ptVideoInInfo->bPMaskReconfig = FALSE;

	if ((ptVideoInInfo->tPMaskCfg.bColorChange == TRUE) ||(ptVideoInInfo->tPMaskCfg.bSwitchChange == TRUE))
		ptVideoInInfo->bPMaskReconfig = TRUE;
	return S_OK;
}
#endif
/* =========================================================================================== */
SCODE VencVideoIn_PhotoMtcReConfig(HANDLE hVideoInProcObj)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)(hVideoInProcObj);

	if (ptVideoInInfo->tPhotoMtcCfg.bModified == TRUE) {
		ptVideoInInfo->bPhotoMtcReconfig = TRUE;
		ptVideoInInfo->tPhotoMtcCfg.bModified = FALSE;
	}	
	return S_OK;
}
