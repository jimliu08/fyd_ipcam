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
 * venc_videoin_init.c
 *
 * \brief
 * Initial each required component.
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

int iWrittingBufNum = 0;

extern TXmlWrapperTreeMap ptVideoInTreeMap[];

/* =========================================================================================== */
SCODE VencVideoIn_InitFrameRateCtrl(HANDLE *phFRCtrlObj)
{
	TFrameRateInitOptions tFRCtrlInitOptions;

	tFRCtrlInitOptions.dwVersion = FRAMERATECTRL_VERSION;
	tFRCtrlInitOptions.dwFrameRate = 0;	// initial disable framerate control

	if (FrameRateCtrl_Initial(phFRCtrlObj, &tFRCtrlInitOptions) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Initial Frame rate control object fail !! \n");
		return S_FAIL;
	}

	return S_OK;
}

/* ========================================================================== */
DWORD VencVideoIn_GetMemory(HANDLE hMemObject,DWORD dwBusNum, DWORD dwSize, EAlignType eAlignType)
{
	TMemMgrState tMemMgrState;
	tMemMgrState.dwBusNum = dwBusNum;
	tMemMgrState.dwSize = dwSize;
	tMemMgrState.eAlignType = eAlignType;

	return (MemMgr_GetMemory(hMemObject, &tMemMgrState));
}

#ifdef _VMA_IBPE
/* ========================================================================== */
void IBPE_Init_bInitNrStrongBuff(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	TImgBackEndProcessInitOptions *ptInit = (TImgBackEndProcessInitOptions *)hObject;
	CHAR szTemp[len+1];
	memset(szTemp, 0x0, sizeof(szTemp));

	snprintf(szTemp, len+1, "%s", szElement);
	if (strcasecmp(szTemp, "true") == 0) {
		ptInit->bInitNrStrongBlendBuff = TRUE;
	} else {
		ptInit->bInitNrStrongBlendBuff = FALSE;
	}
}

/* ========================================================================== */
void IBPE_Init_bInitStaticMapBuff(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	TImgBackEndProcessInitOptions *ptInit = (TImgBackEndProcessInitOptions *)hObject;
#ifdef _VMA_DIE
	CHAR szTemp[len+1];
	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szElement);
	if (strcasecmp(szTemp, "true") == 0) {		
		ptInit->bInitStaticMapBuff = TRUE;
	} else {
		ptInit->bInitStaticMapBuff = FALSE;
	}
#else
	ptInit->bInitStaticMapBuff = FALSE;
#endif

}
/* ========================================================================== */
void IBPE_Init_MapMemRankNum(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	TImgBackEndProcessInitOptions *ptInit = (TImgBackEndProcessInitOptions *)hObject;
	CHAR szTemp[len+1];
	memset(szTemp, 0x0, sizeof(szTemp));

	snprintf(szTemp, len+1, "%s", szElement);
	ptInit->dwMapMemRankNum = strtoul(szTemp, NULL, 10);
}

/* ========================================================================== */
void IBPE_Init_RefMemRankNum(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	TImgBackEndProcessInitOptions *ptInit = (TImgBackEndProcessInitOptions *)hObject;
	CHAR szTemp[len+1];
	memset(szTemp, 0x0, sizeof(szTemp));

	snprintf(szTemp, len+1, "%s", szElement);
	ptInit->dwRefMemRankNum = strtoul(szTemp, NULL, 10);
}

/* ========================================================================== */
SCODE VencVideoIn_InitImgBackEndProcess(HANDLE *phIBPObject, const _TVideoInInitNeedInfo *const _ptInitNeedInfo,char *szConfigFile, BOOL bSecondPhase, BOOL *pbInitNrStrongBlendBuff)
{ 
	TImgBackEndProcessInitOptions tIBPInitOpt;
	memset(&(tIBPInitOpt), 0, sizeof(TImgBackEndProcessInitOptions));
	{	
		HANDLE hXmlWrapperObj;
		TXmlWrapperInitOptions tXmlWrapperInitOptions;
		
		TXmlWrapperTreeMap ptMap[] = 
		{	
			{"root/venc_videoin/ibpe_initial/bInitStaticMapBuff", NULL, &IBPE_Init_bInitStaticMapBuff, NULL},
			{"root/venc_videoin/ibpe_initial/bInitNrStrongBuff", NULL, &IBPE_Init_bInitNrStrongBuff, NULL},
			{"root/venc_videoin/ibpe_initial/map_mem_rank_num", NULL, &IBPE_Init_MapMemRankNum, NULL},
			{"root/venc_videoin/ibpe_initial/ref_mem_rank_num", NULL, &IBPE_Init_RefMemRankNum, NULL},
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

		if (XmlWrapper_ReadFile_UsrDefFunc(szConfigFile, hXmlWrapperObj, &tIBPInitOpt) != S_OK)
		{
			printf(__FILE__":%s:%d [IBPE] XmlWrapper_ReadBuf_UsrDefFunc fail\n", __func__, __LINE__);
			return S_FAIL;
		}

		if (XmlWrapper_Release(&hXmlWrapperObj) != S_OK)
		{
			printf("Release XmlWrapper Fail! \n");
			return S_FAIL;
		}
	}

	tIBPInitOpt.dwVersion = IMGBACKENDPROCESS_VERSION;
	tIBPInitOpt.pObjectMem = NULL;
	tIBPInitOpt.dwMaxWindowNum = MAX_MOTION_WIN_NUM;
	tIBPInitOpt.dwMaxWidth = _ptInitNeedInfo->dwVideoMaxWidth;
	tIBPInitOpt.dwMaxHeight = _ptInitNeedInfo->dwVideoMaxHeight;
	tIBPInitOpt.dwInStride = _ptInitNeedInfo->dwVideoMaxWidth;
	tIBPInitOpt.dwOutStride = _ptInitNeedInfo->dwVideoMaxWidth;
#ifdef _PRE_NR_	
	if (bSecondPhase == TRUE)
	{
		//The IBPE handle of sencod phase should always be enabled the flag "bInitNrStrongBlendBuff" .
		tIBPInitOpt.bInitNrStrongBlendBuff = TRUE;
	}
#endif	
	{
		DWORD dwMaskBuffWidth = 0;
		DWORD dwMaskBuffStride = 0;
		dwMaskBuffWidth = (_ptInitNeedInfo->dwVideoMaxWidth+15) >> 4;
		dwMaskBuffStride = ((dwMaskBuffWidth+7)>>3) << 3;
		tIBPInitOpt.dwPrivacyMaskMapStride = dwMaskBuffStride;
	}
#if defined (_YUV422_)	
	tIBPInitOpt.bIfNrStrongBlendBuffFormat420 = FALSE;	
#else
	tIBPInitOpt.bIfNrStrongBlendBuffFormat420 = TRUE;	
#endif
	if (ImgBackEndProcess_Initial(phIBPObject, &tIBPInitOpt) != S_OK) {
		printf("[VENC_VIDEOIN] Image backend processing initial err !!\n");
		return S_FAIL;
	}
	
	*pbInitNrStrongBlendBuff = tIBPInitOpt.bInitNrStrongBlendBuff;
	
	return S_OK;
}

/* ========================================================================== */
#ifdef _VMA_IBPE_BACK_DOOR
SCODE VencVideoIn_ReleaseIBPParam(TVideoInInfo *ptVideoInInfo)
{
	SCODE scRet = S_OK;
	HANDLE phMemMgrObj = ptVideoInInfo->hMemMgrObj;

	printf("[venc_videoin_init](%d)Enter VencVideoIn_ReleaseIBPParam()...\n",__LINE__);

	if (ptVideoInInfo->pvEESensitivity != NULL)
	{
		if (MemMgr_FreeMemory(phMemMgrObj, (DWORD)ptVideoInInfo->pvEESensitivity) != S_OK) {
			fprintf(stderr, "[VENC_VIDEOIN](%d) Release EESensitivity Buffer fail !!\n",__LINE__);
			scRet = S_FAIL;
		}
		ptVideoInInfo->pvEESensitivity = NULL;
	}	
	if (ptVideoInInfo->pvEEStrength != NULL)
	{
		if (MemMgr_FreeMemory(phMemMgrObj, (DWORD)ptVideoInInfo->pvEEStrength) != S_OK) {
			fprintf(stderr, "[VENC_VIDEOIN](%d) Release EEStrength Buffer fail !!\n",__LINE__);
			scRet = S_FAIL;
		}
		ptVideoInInfo->pvEEStrength = NULL;
	}	
	if (ptVideoInInfo->pvEE3x3Sensitivity != NULL)
	{
		if (MemMgr_FreeMemory(phMemMgrObj, (DWORD)ptVideoInInfo->pvEE3x3Sensitivity) != S_OK) {
			fprintf(stderr, "[VENC_VIDEOIN](%d) Release EE3x3Sensitivity Buffer fail !!\n",__LINE__);
			scRet = S_FAIL;
		}
		ptVideoInInfo->pvEE3x3Sensitivity = NULL;
	}	
	if (ptVideoInInfo->pvEE3x3Alpha != NULL)
	{
		if (MemMgr_FreeMemory(phMemMgrObj, (DWORD)ptVideoInInfo->pvEE3x3Alpha) != S_OK) {
			fprintf(stderr, "[VENC_VIDEOIN](%d) Release EE3x3Alpha Buffer fail !!\n",__LINE__);
			scRet = S_FAIL;
		}
		ptVideoInInfo->pvEE3x3Alpha = NULL;
	}	
	if (ptVideoInInfo->pvEE3x3Strength != NULL)
	{
		if (MemMgr_FreeMemory(phMemMgrObj, (DWORD)ptVideoInInfo->pvEE3x3Strength) != S_OK) {
			fprintf(stderr, "[VENC_VIDEOIN](%d) Release EE3x3Strength Buffer fail !!\n",__LINE__);
			scRet = S_FAIL;
		}
		ptVideoInInfo->pvEE3x3Strength = NULL;
	}	
	if (ptVideoInInfo->pvDI != NULL)
	{
		if (MemMgr_FreeMemory(phMemMgrObj, (DWORD)ptVideoInInfo->pvDI) != S_OK) {
			fprintf(stderr, "[VENC_VIDEOIN](%d) Release DI Buffer fail !!\n",__LINE__);
			scRet = S_FAIL;
		}
		ptVideoInInfo->pvDI = NULL;
	}	
	if (ptVideoInInfo->pvNR2D != NULL)
	{
		if (MemMgr_FreeMemory(phMemMgrObj, (DWORD)ptVideoInInfo->pvNR2D) != S_OK) {
			fprintf(stderr, "[VENC_VIDEOIN](%d) Release NR2D Buffer fail !!\n",__LINE__);
			scRet = S_FAIL;
		}
		ptVideoInInfo->pvNR2D = NULL;
	}	
	if (ptVideoInInfo->pvNR2D1Ref != NULL)
	{
		if (MemMgr_FreeMemory(phMemMgrObj, (DWORD)ptVideoInInfo->pvNR2D1Ref) != S_OK) {
			fprintf(stderr, "[VENC_VIDEOIN](%d) Release NR3D1Ref Buffer fail !!\n",__LINE__);
			scRet = S_FAIL;
		}
		ptVideoInInfo->pvNR2D1Ref = NULL;
	}	
	if (ptVideoInInfo->pvNR2D2Ref != NULL)
	{
		if (MemMgr_FreeMemory(phMemMgrObj, (DWORD)ptVideoInInfo->pvNR2D2Ref) != S_OK) {
			fprintf(stderr, "[VENC_VIDEOIN](%d) Release NR3D2Ref Buffer fail !!\n",__LINE__);
			scRet = S_FAIL;
		}
		ptVideoInInfo->pvNR2D2Ref = NULL;
	}	
	if (ptVideoInInfo->pvNR3DMA0CurrWeight != NULL)
	{
		if (MemMgr_FreeMemory(phMemMgrObj, (DWORD)ptVideoInInfo->pvNR3DMA0CurrWeight) != S_OK) {
			fprintf(stderr, "[VENC_VIDEOIN](%d) Release NR3DMA0CurrWeight Buffer fail !!\n",__LINE__);
			scRet = S_FAIL;
		}
		ptVideoInInfo->pvNR3DMA0CurrWeight = NULL;
	}	
	if (ptVideoInInfo->pvNR3DMA0RefStrength != NULL)
	{
		if (MemMgr_FreeMemory(phMemMgrObj, (DWORD)ptVideoInInfo->pvNR3DMA0RefStrength) != S_OK) {
			fprintf(stderr, "[VENC_VIDEOIN](%d) Release NR3DMA0RefStrength Buffer fail !!\n",__LINE__);
			scRet = S_FAIL;
		}
		ptVideoInInfo->pvNR3DMA0RefStrength = NULL;
	}	
	if (ptVideoInInfo->pvNR3DMA1CurrWeight != NULL)
	{
		if (MemMgr_FreeMemory(phMemMgrObj, (DWORD)ptVideoInInfo->pvNR3DMA1CurrWeight) != S_OK) {
			fprintf(stderr, "[VENC_VIDEOIN](%d) Release NR3DMA1CurrWeight Buffer fail !!\n",__LINE__);
			scRet = S_FAIL;
		}
		ptVideoInInfo->pvNR3DMA1CurrWeight = NULL;
	}	
	if (ptVideoInInfo->pvNR3DMA1RefStrength != NULL)
	{
		if (MemMgr_FreeMemory(phMemMgrObj, (DWORD)ptVideoInInfo->pvNR3DMA1RefStrength) != S_OK) {
			fprintf(stderr, "[VENC_VIDEOIN](%d) Release NR3DMA1RefStrength Buffer fail !!\n",__LINE__);
			scRet = S_FAIL;
		}
		ptVideoInInfo->pvNR3DMA1RefStrength = NULL;
	}	

	printf("[venc_videoin_init](%d)Enter VencVideoIn_ReleaseIBPParam()...\n",__LINE__);

	return scRet;
}

/* ========================================================================== */
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

/* ========================================================================== */
SCODE VencVideoIn_LoadIBPParam(TVideoInInfo *ptVideoInInfo)
{
	SCODE scRet = S_OK;
	FILE *pParamFile;
	HANDLE phIBPObject = ptVideoInInfo->hIBPObj;
#ifdef _PRE_NR_		
	HANDLE hSencodPhaseIBPObject = ptVideoInInfo->hSencodPhaseIBPObj;
#endif
	HANDLE phMemMgrObj = ptVideoInInfo->hMemMgrObj;
	CHAR chEESensitivityFileName[200] = "/etc/conf.d/isptable/EESensitivity.bin";
	CHAR chEEStrengthFileName[200] = "/etc/conf.d/isptable/EEStrength.bin";
	CHAR chEE3x3SensitivityFileName[200] = "/etc/conf.d/isptable/EE3x3Sensitivity.bin";
	CHAR chEE3x3AlphaFileName[200] = "/etc/conf.d/isptable/EE3x3Alpha.bin";
	CHAR chEE3x3StrengthFileName[200] = "/etc/conf.d/isptable/EE3x3Strength.bin";
	CHAR chDIFileName[200] = "/etc/conf.d/isptable/DI.bin";
	CHAR chNR2DFileName[200] = "/etc/conf.d/isptable/NR2D.bin";
	CHAR chNR2D1RefFileName[200] = "/etc/conf.d/isptable/NR2D1Ref.bin";
	CHAR chNR2D2RefFileName[200] = "/etc/conf.d/isptable/NR2D2Ref.bin";
	CHAR chNR3DMA0CurrWeightFileName[200] = "/etc/conf.d/isptable/NR3DMA0CurrWeight.bin";
	CHAR chNR3DMA0RefStrengthFileName[200] = "/etc/conf.d/isptable/NR3DMA0RefStrength.bin";
	CHAR chNR3DMA1CurrWeightFileName[200] = "/etc/conf.d/isptable/NR3DMA1CurrWeight.bin";
	CHAR chNR3DMA1RefStrengthFileName[200] = "/etc/conf.d/isptable/NR3DMA1RefStrength.bin";
	TImgBackEndProcessOptions tOptions;

	ptVideoInInfo->pvEESensitivity = NULL;
	ptVideoInInfo->pvEEStrength = NULL;
	ptVideoInInfo->pvEE3x3Sensitivity = NULL;
	ptVideoInInfo->pvEE3x3Alpha = NULL;
	ptVideoInInfo->pvEE3x3Strength = NULL;
	ptVideoInInfo->pvDI = NULL;
	ptVideoInInfo->pvNR2D = NULL;
	ptVideoInInfo->pvNR2D1Ref = NULL;
	ptVideoInInfo->pvNR2D2Ref = NULL;
	ptVideoInInfo->pvNR3DMA0CurrWeight = NULL;	
	ptVideoInInfo->pvNR3DMA0RefStrength = NULL;
	ptVideoInInfo->pvNR3DMA1CurrWeight = NULL;
	ptVideoInInfo->pvNR3DMA1RefStrength = NULL;

	printf("[venc_videoin_init](%d)Enter VencVideoIn_LoadIBPParam()...\n",__LINE__);
	// 0. Read EE Sensitivity param into buffer:
	if ((pParamFile = fopen(chEESensitivityFileName, "r")) != NULL)
	{
		if (ReadFile(phMemMgrObj, pParamFile, &ptVideoInInfo->pvEESensitivity) == S_OK)
		{
			tOptions.eOptionFlags = 200;
			tOptions.apvUserData[0] = (PVOID)ptVideoInInfo->pvEESensitivity;
			if (ImgBackEndProcess_SetOptions(phIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#ifdef _PRE_NR_				
			if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#endif
			
		}
		fclose(pParamFile);
	}

	// 1. Read EE Strength param into buffer:
	if ((pParamFile = fopen(chEEStrengthFileName, "r")) != NULL)
	{
		if (ReadFile(phMemMgrObj, pParamFile, &ptVideoInInfo->pvEEStrength) == S_OK)
		{
			tOptions.eOptionFlags = 201;
			tOptions.apvUserData[0] = (PVOID)ptVideoInInfo->pvEEStrength;
			if (ImgBackEndProcess_SetOptions(phIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#ifdef _PRE_NR_				
			if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#endif
		}
		fclose(pParamFile);
	}	

	// 2. Read EE 3x3 Sensitivity param into buffer:
	if ((pParamFile = fopen(chEE3x3SensitivityFileName, "r")) != NULL)
	{
		if (ReadFile(phMemMgrObj, pParamFile, &ptVideoInInfo->pvEE3x3Sensitivity) == S_OK)
		{
			tOptions.eOptionFlags = 202;
			tOptions.apvUserData[0] = (PVOID)ptVideoInInfo->pvEE3x3Sensitivity;
			if (ImgBackEndProcess_SetOptions(phIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#ifdef _PRE_NR_				
			if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#endif
		}
		fclose(pParamFile);
	}

	// 3. Read EE 3x3 Alpha param into buffer:
	if ((pParamFile = fopen(chEE3x3AlphaFileName, "r")) != NULL)
	{
		if (ReadFile(phMemMgrObj, pParamFile, &ptVideoInInfo->pvEE3x3Alpha) == S_OK)
		{
			tOptions.eOptionFlags = 203;
			tOptions.apvUserData[0] = (PVOID)ptVideoInInfo->pvEE3x3Alpha;
			if (ImgBackEndProcess_SetOptions(phIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#ifdef _PRE_NR_				
			if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#endif
		}
		fclose(pParamFile);
	}

	// 4. Read EE 3x3 Strength param into buffer:
	if ((pParamFile = fopen(chEE3x3StrengthFileName, "r")) != NULL)
	{
		if (ReadFile(phMemMgrObj, pParamFile, &ptVideoInInfo->pvEE3x3Strength) == S_OK)
		{
			tOptions.eOptionFlags = 204;
			tOptions.apvUserData[0] = (PVOID)ptVideoInInfo->pvEE3x3Strength;
			if (ImgBackEndProcess_SetOptions(phIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#ifdef _PRE_NR_			
			if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#endif
		}
		fclose(pParamFile);
	}

	// 5. Read DI param into buffer:
	if ((pParamFile = fopen(chDIFileName, "r")) != NULL)
	{
		if (ReadFile(phMemMgrObj, pParamFile, &ptVideoInInfo->pvDI) == S_OK)
		{
			tOptions.eOptionFlags = 205;
			tOptions.apvUserData[0] = (PVOID)ptVideoInInfo->pvDI;
			if (ImgBackEndProcess_SetOptions(phIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#ifdef _PRE_NR_				
			if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#endif
		}
		fclose(pParamFile);
	}

	// 6. Read 2D NR param into buffer:
	if ((pParamFile = fopen(chNR2DFileName, "r")) != NULL)
	{
		if (ReadFile(phMemMgrObj, pParamFile, &ptVideoInInfo->pvNR2D) == S_OK)
		{
			tOptions.eOptionFlags = 206;
			tOptions.apvUserData[0] = (PVOID)ptVideoInInfo->pvNR2D;
			if (ImgBackEndProcess_SetOptions(phIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#ifdef _PRE_NR_				
			if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#endif
		}
		fclose(pParamFile);
	}

	// 7. Read 2D NR 1Ref param into buffer:
	if ((pParamFile = fopen(chNR2D1RefFileName, "r")) != NULL)
	{
		if (ReadFile(phMemMgrObj, pParamFile, &ptVideoInInfo->pvNR2D1Ref) == S_OK)
		{
			tOptions.eOptionFlags = 207;
			tOptions.apvUserData[0] = (PVOID)ptVideoInInfo->pvNR2D1Ref;
			if (ImgBackEndProcess_SetOptions(phIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#ifdef _PRE_NR_				
			if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#endif
		}
		fclose(pParamFile);
	}

	// 8. Read 2D NR 2Ref param into buffer:
	if ((pParamFile = fopen(chNR2D2RefFileName, "r")) != NULL)
	{
		if (ReadFile(phMemMgrObj, pParamFile, &ptVideoInInfo->pvNR2D2Ref) == S_OK)
		{
			tOptions.eOptionFlags = 208;
			tOptions.apvUserData[0] = (PVOID)ptVideoInInfo->pvNR2D2Ref;
			if (ImgBackEndProcess_SetOptions(phIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#ifdef _PRE_NR_				
			if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#endif
		}
		fclose(pParamFile);
	}

	// 9. Read 3D NR curr. frame weight in disable Motion Adaptive mode param into buffer:
	if ((pParamFile = fopen(chNR3DMA0CurrWeightFileName, "r")) != NULL)
	{
		if (ReadFile(phMemMgrObj, pParamFile, &ptVideoInInfo->pvNR3DMA0CurrWeight) == S_OK)
		{
			tOptions.eOptionFlags = 209;
			tOptions.apvUserData[0] = (PVOID)ptVideoInInfo->pvNR3DMA0CurrWeight;
			if (ImgBackEndProcess_SetOptions(phIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#ifdef _PRE_NR_			
			if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#endif
		}
		fclose(pParamFile);
	}

	// 10. Read 3D NR ref. frame strength in disable Motion Adaptive mode param into buffer:
	if ((pParamFile = fopen(chNR3DMA0RefStrengthFileName, "r")) != NULL)
	{
		if (ReadFile(phMemMgrObj, pParamFile, &ptVideoInInfo->pvNR3DMA0RefStrength) == S_OK)
		{
			tOptions.eOptionFlags = 210;
			tOptions.apvUserData[0] = (PVOID)ptVideoInInfo->pvNR3DMA0RefStrength;
			if (ImgBackEndProcess_SetOptions(phIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#ifdef _PRE_NR_				
			if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#endif
		}
		fclose(pParamFile);
	}

	// 11. Read 3D NR curr. frame weight in enable Motion Adaptive mode param into buffer:
	if ((pParamFile = fopen(chNR3DMA1CurrWeightFileName, "r")) != NULL)
	{
		if (ReadFile(phMemMgrObj, pParamFile, &ptVideoInInfo->pvNR3DMA1CurrWeight) == S_OK)
		{
			tOptions.eOptionFlags = 211;
			tOptions.apvUserData[0] = (PVOID)ptVideoInInfo->pvNR3DMA1CurrWeight;
			if (ImgBackEndProcess_SetOptions(phIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#ifdef _PRE_NR_				
			if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#endif
		}
		fclose(pParamFile);
	}

	// 12. Read 3D NR ref. frame strength in enable Motion Adaptive mode param into buffer:
	if ((pParamFile = fopen(chNR3DMA1RefStrengthFileName, "r")) != NULL)
	{
		if (ReadFile(phMemMgrObj, pParamFile, &ptVideoInInfo->pvNR3DMA1RefStrength) == S_OK)
		{
			tOptions.eOptionFlags = 212;
			tOptions.apvUserData[0] = (PVOID)ptVideoInInfo->pvNR3DMA1RefStrength;
			if (ImgBackEndProcess_SetOptions(phIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#ifdef _PRE_NR_				
			if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObject, &tOptions) != S_OK)
			{
				fclose(pParamFile);
				scRet = S_FAIL;
				goto exit;
			}
#endif
		}
		fclose(pParamFile);
	}
exit:
	printf("[venc_videoin_init](%d)Leave VencVideoIn_LoadIBPParam()...\n",__LINE__);
	return scRet; 
}
#endif
#endif

/* ========================================================================== */
SCODE VencVideoIn_InitMassMemAccess(HANDLE *phMassMemAccObject, const _TVideoInInitNeedInfo *const _ptInitNeedInfo)
{
	TMassMemAccessInitOptions tMassMemAccessInitOpt;
	TMassMemAccessOptions tOptions;
	memset(&(tMassMemAccessInitOpt), 0, sizeof(TMassMemAccessInitOptions));

	tMassMemAccessInitOpt.dwVersion = MASSMEMACCESS_VERSION;
	tMassMemAccessInitOpt.dwMaxFrameWidth = _ptInitNeedInfo->dwVideoMaxWidth;
	tMassMemAccessInitOpt.dwMaxFrameHeight = _ptInitNeedInfo->dwVideoMaxHeight;
	tMassMemAccessInitOpt.dwMaskBusNum = 0;
	tMassMemAccessInitOpt.bMaskInit = FALSE;
	tMassMemAccessInitOpt.pObjectMem = NULL;
	if (MassMemAccess_Initial(phMassMemAccObject, &tMassMemAccessInitOpt) != S_OK) {
		printf("[VENC_VIDEOIN] Mass memory access initial err !!\n");
		return S_FAIL;
	}
	tOptions.adwUserData[0] = FALSE;
	tOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_CACHE_COHERENCE;
	if (MassMemAccess_SetOptions(*phMassMemAccObject, &tOptions) != S_OK)
	{
		printf("[VENC_VIDEOIN]Fail to set up cache operation !!\n");
		return S_FAIL;
	}
	return S_OK;
}

/* ========================================================================== */
#ifdef _DEFECT_PIXEL_ENABLE_
static int DP_VerifyCheckSum(DWORD CheckSum, DWORD DPNum, DWORD *DPInfo)
{
	unsigned long i;
	unsigned long VerifyCheckSum = 0;

	if (DPInfo == NULL)	return 0;

	VerifyCheckSum += DPNum;

	for (i = 2; i < (DPNum*3+2); i++)
	{
		VerifyCheckSum += DPInfo[i];
	}

	if (VerifyCheckSum != CheckSum)
	{
		return 0;
	}
	else
	{
		return 1;
	}

}
SCODE VencVideoIn_ReleaseDPC(HANDLE *phDPCObject)
{
	return DefectPixelCorrect_Release(phDPCObject);

}
SCODE VencVideoIn_InitDPC(HANDLE *phDPCObject,HANDLE hMemObject)
{
	TDefectPixelCorrectInitOptions tDPCInitOptions;
	FILE *pfDPCInput;
	DWORD dwFileSize, dwCheckSum;
	DWORD *DPInfo = NULL;
	CHAR acRootext[200] = "/etc/conf.d/dp_table.bin";
	TMemMgrState tMemMgrState;
	int i = 0;
	SCODE scRet = S_OK;

	memset(&tMemMgrState, 0x0, sizeof(TMemMgrState));
	memset(&tDPCInitOptions, 0x0, sizeof(TDefectPixelCorrectInitOptions));
	tDPCInitOptions.ptDPInfo = NULL;
	if ((pfDPCInput = fopen(acRootext, "rb")) != NULL)
	{
		// obtain file size:
		fseek (pfDPCInput , 0 , SEEK_END);
		dwFileSize = ftell(pfDPCInput);
		rewind(pfDPCInput);

		// allocate memory to contain the whole file:
		tMemMgrState.dwBusNum = 0;
		tMemMgrState.dwSize = dwFileSize;
		tMemMgrState.eAlignType = ALIGN_TYPE_256_BYTE;

		if ((DPInfo = (DWORD*)MemMgr_GetMemory(hMemObject, &tMemMgrState)) == NULL)
		{
			printf("[VENC_VIDEOIN] Allocate DefectPixelInfo File buffer fail !!\n");
			scRet = S_FAIL;
			goto exit;
		}
		else
		{
			printf("[VENC_VIDEOIN] Request %d bytes DefectPixelInfo File buffer succeed !!\n", (int)dwFileSize);
		}

		// copy the file into the buffer:
		if (fread(DPInfo, 1, dwFileSize, pfDPCInput) != dwFileSize)
		{		
			scRet = S_FAIL;
			goto exit;
		}
		fclose(pfDPCInput);

		dwCheckSum = DPInfo[0];
		// Check Sum
		if (DP_VerifyCheckSum(dwCheckSum, DPInfo[1], DPInfo))
		{
			printf("[VENC_VIDEOIN] DP_VerifyCheckSum OK !! \n");
			tDPCInitOptions.dwDefectPixelNum = DPInfo[1];

			// allocate memory to contain DP position:
			tMemMgrState.dwBusNum = VIDEOCAP_IN_BUS_NUM;
			tMemMgrState.dwSize = tDPCInitOptions.dwDefectPixelNum * sizeof(TDefectPixelInfo);
			tMemMgrState.eAlignType = ALIGN_TYPE_256_BYTE;

			if ((tDPCInitOptions.ptDPInfo = (TDefectPixelInfo*)MemMgr_GetMemory(hMemObject, &tMemMgrState)) == NULL)
			{
				printf("[VENC_VIDEOIN] Allocate DefectPixelInfo buffer fail !!\n");
				scRet = S_FAIL;
				goto exit;
			}
			else
			{
				printf("[VENC_VIDEOIN]  Request %d bytes DefectPixelInfo buffer succeed !!\n", (int)(tDPCInitOptions.dwDefectPixelNum * sizeof(TDefectPixelInfo)));
			}

			for(i = 0; i < tDPCInitOptions.dwDefectPixelNum; i++)
			{
				tDPCInitOptions.ptDPInfo[i].dwPixelNum = DPInfo[3*i+2];
				tDPCInitOptions.ptDPInfo[i].dwLineNum = DPInfo[3*i+3];
				tDPCInitOptions.ptDPInfo[i].dwType = DPInfo[3*i+4];
			}

			tDPCInitOptions.dwVersion = DEFECTPIXELCORRECT_VERSION;
			tDPCInitOptions.pObjectMem = NULL;
			if (DefectPixelCorrect_Initial(phDPCObject, &tDPCInitOptions) != S_OK) {
				printf("[VENC_VIDEOIN] DPC Object Initialization failed !!\n");
				scRet = S_FAIL;
				goto exit;
			}
			else
			{
				printf("[VENC_VIDEOIN] DPC object initialization succeeded !!\n");
			}
		}
		else
		{
			printf("[VENC_VIDEOIN] DP_VerifyCheckSum NG !! \n");
			scRet = S_FAIL;
			goto exit;
		}
	}
	else
	{
		printf("[VENC_VIDEOIN] Open %s fail !!\n", acRootext);
	}
exit:
	if (MemMgr_FreeMemory(hMemObject, (DWORD)DPInfo) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN](%d) Release DPC Info Buffer fail !!\n",__LINE__);
		scRet = S_FAIL;
	}
	if (MemMgr_FreeMemory(hMemObject, (DWORD)tDPCInitOptions.ptDPInfo) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN](%d) Release DPC Info Buffer fail !!\n",__LINE__);
		scRet = S_FAIL;
	}
	return scRet;

}
#endif
/* ========================================================================== */


/* ============================================================================================= */
SCODE VencVideoIn_InitXMLWrapper(HANDLE *phXMLWrapperObject)
{
	TXmlWrapperInitOptions txpInitOptions;

	txpInitOptions.dwVersion = XMLWRAPPER_VERSION;
	if (XmlWrapper_Initial(phXMLWrapperObject, &txpInitOptions) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Initial XMLWrapper object fail !! \n");
		return S_FAIL;
	}
	if (XmlWrapper_SetHandler((*phXMLWrapperObject), ptVideoInTreeMap,NULL) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Set XMLWrapper handler fail !! \n");
		return S_FAIL;
	}

	return S_OK;
}

/* ============================================================================================= */
SCODE VencVideoIn_InitSharedBuffer(TVideoInInfo *ptVideoInInfo, const _TVideoInInitNeedInfo *const _ptInitNeedInfo)
{	
	TSharedBuffMgrInitOptions tInitOptions;
	DWORD dwDataSize;
	DWORD dwWidthPadding;
	DWORD dwHeightPadding;

	// multiple of 16
	//dwWidthPadding = (((_ptInitNeedInfo->dwVideoCapWidth + 15) >> 4) << 4);
	//dwHeightPadding = (((_ptInitNeedInfo->dwVideoCapHeight + 15) >> 4) << 4);
	dwWidthPadding = (((_ptInitNeedInfo->dwVideoMaxWidth + 15) >> 4) << 4);
	dwHeightPadding = (((_ptInitNeedInfo->dwVideoMaxHeight+ 15) >> 4) << 4);
	dwDataSize = (((dwWidthPadding * dwHeightPadding) * 3) >> 1); // YUV420 format
	
	iWrittingBufNum = (int)ptVideoInInfo->dwOutputSharedBufferNum;
	// Initial sharedbuffergr
	tInitOptions.dwVersion = SHAREDBUFFMGR_VERSION;
	tInitOptions.dwBufNum = ptVideoInInfo->dwOutputSharedBufferNum;
	tInitOptions.dwHeaderSize = 1024; // enough ?
	tInitOptions.dwDataSize = dwDataSize;
	tInitOptions.eAlignType = ALIGN_TYPE_128_BYTE;
	tInitOptions.pObjectMem = NULL;
	tInitOptions.dwBusIndex = _ptInitNeedInfo->dwDMACOutputDRAMLoc;
	tInitOptions.dwMinorNum = ptVideoInInfo->dwSharedBufferMinorNum;
	tInitOptions.eProcessRole = esbmprWriter;
	tInitOptions.byInitialVal = 0x80;
	printf("[VENC_VIDEOIN] Shared Buffer MinorNum = %u ..\n", tInitOptions.dwMinorNum);

	printf("[VENC_MEM] SharedBuffer for videoin output buffer : %d bytes (buffer num=%d)\n", tInitOptions.dwBufNum * (tInitOptions.dwDataSize + tInitOptions.dwHeaderSize), ptVideoInInfo->dwOutputSharedBufferNum);

	if (SharedBuffMgr_Initial(&(ptVideoInInfo->hSharedBuferObj), &tInitOptions) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Initialize shared buffer manager object fail !!\n");
		return S_FAIL;
	}

	return S_OK;
}

#ifdef _VMA_IBPE
/* ========================================================================== */
SCODE VencVideoIn_InitPrivacyMask(TVideoInInfo *ptVideoInInfo, const _TVideoInInitNeedInfo *const _ptInitNeedInfo, HANDLE hMemMgrObj)
{
	memset(&ptVideoInInfo->tIBPEPMask, 0x0, sizeof(TIBPEPMask));
	ptVideoInInfo->tIBPEPMask.hMemMgrObj = hMemMgrObj;
	ptVideoInInfo->tIBPEPMask.dwWidth = _ptInitNeedInfo->dwVideoMaxWidth;
	ptVideoInInfo->tIBPEPMask.dwHeight = _ptInitNeedInfo->dwVideoMaxHeight;
	VencVideoIn_InitIBPEPMask(&ptVideoInInfo->tIBPEPMask, ptVideoInInfo->szConfigFile);
	return S_OK;
}

SCODE VencVideoIn_ReleasePrivacyMask(TVideoInInfo *ptVideoInInfo)
{
	VencVideoIn_ReleaseIBPEPMask(&ptVideoInInfo->tIBPEPMask);
	return S_OK;
}
#endif
/* ========================================================================== */
SCODE VencVideoIn_InitVML(TVideoInInfo *ptVideoInInfo)
{
	DWORD dwFrameSize = 0, dwYFrameSize = 0, dwCFrameSize = 0;
	DWORD dwVideoSize = 0;
	// only support YUV420
	dwYFrameSize = ptVideoInInfo->_tInitNeedInfo.dwVideoMaxWidth * ptVideoInInfo->_tInitNeedInfo.dwVideoMaxHeight;
	dwCFrameSize = (dwYFrameSize >> 2);
	dwFrameSize = dwYFrameSize + (dwCFrameSize << 1);
	// ===== Initial buffers ===== 

	// allocate photometric lens distortion buffer
	//ptVideoInInfo->dwPhotoLDCTblSize = (512 * ptVideoInInfo->_tInitNeedInfo.dwVideoMaxHeight);
	//ptVideoInInfo->dwPhotoLDCTblSize = (512 * ptVideoInInfo->_tInitNeedInfo.dwVideoInHeight);
#ifdef _YUV422_
	ptVideoInInfo->pbyYUV420FrameBuff = (BYTE *)VencVideoIn_GetMemory(ptVideoInInfo->hMemMgrObj,
												0,
												dwFrameSize ,
												ALIGN_TYPE_1024_BYTE
											  );
	
	if (ptVideoInInfo->pbyYUV420FrameBuff == NULL)
	{
		printf("[VENC_VIDEOIN] Allocate pbyYUV420FrameBuff fail !!\n");
		return S_FAIL;
	}
#endif	
	
	
	dwVideoSize = ptVideoInInfo->_tInitNeedInfo.dwVideoInWidth * ptVideoInInfo->_tInitNeedInfo.dwVideoInHeight;
	if (ptVideoInInfo->_tInitNeedInfo.dwVideoInWidth <= 512)
	{
		ptVideoInInfo->dwPhotoLDCTblSize = dwVideoSize; // size of WORD
	}
	else if (ptVideoInInfo->_tInitNeedInfo.dwVideoInWidth <= 1024)
	{
		ptVideoInInfo->dwPhotoLDCTblSize = dwVideoSize >> 1; // size of WORD
	}
	else if (ptVideoInInfo->_tInitNeedInfo.dwVideoInWidth <= 2048)
	{
		ptVideoInInfo->dwPhotoLDCTblSize = dwVideoSize >> 2; // size of WORD
	}
	else
	{
		ptVideoInInfo->dwPhotoLDCTblSize = dwVideoSize >> 3; // size of WORD
	}

	if ((ptVideoInInfo->pwPhotoLDCRefFrame = (WORD *)VencVideoIn_GetMemory(ptVideoInInfo->hMemMgrObj,
																		   ptVideoInInfo->_tInitNeedInfo.dwLDCRefFrameDRAMLoc,
																		   (ptVideoInInfo->dwPhotoLDCTblSize << 1),
																		   ALIGN_TYPE_1024_BYTE
																		   )) == NULL) {
		printf("[VENC_VIDEOIN] Allocate photometric lens distortion correction reference frame buffer fail !!\n");
		return S_FAIL;
	} else {
		printf("[VENC_VIDEOIN] Request %u bytes non-cached memory succeed !!\n", (ptVideoInInfo->dwPhotoLDCTblSize << 1));
		//syslog(LOG_INFO, "[VENC_MEM]  pwPhotoLDCRefFrame : %d bytes\n", (ptVideoInInfo->dwPhotoLDCTblSize << 1));
	}
	
	memset(ptVideoInInfo->pwPhotoLDCRefFrame, 1024, ptVideoInInfo->dwPhotoLDCTblSize);
	
	// ===== Initial other instance =====
	// Initial video capture instance
#ifdef _VMA_IBPE
	// Initial IBP instance
	if (VencVideoIn_InitImgBackEndProcess(&(ptVideoInInfo->hIBPObj), &(ptVideoInInfo->_tInitNeedInfo), ptVideoInInfo->szConfigFile, FALSE, &(ptVideoInInfo->bInitNrStrongBlendBuff)) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Initial image back-end object fail !!\n");
		return S_FAIL;    
	}
	DBPRINT0("[VENC_VIDEOIN] Initial image back-end object Success !!\n");
#ifdef _PRE_NR_	
	if (VencVideoIn_InitImgBackEndProcess(&(ptVideoInInfo->hSencodPhaseIBPObj), &(ptVideoInInfo->_tInitNeedInfo), ptVideoInInfo->szConfigFile, TRUE, &(ptVideoInInfo->bSencodPhaseInitNrStrongBlendBuff)) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Initial image back-end object fail !!\n");
		return S_FAIL;    
	}
#endif
	

#ifdef _VMA_IBPE_BACK_DOOR
	if (VencVideoIn_LoadIBPParam(ptVideoInInfo) != S_OK)
	{
		fprintf(stderr, "[VENC_VIDEOIN] Load IBPE param fail !!\n");
		return S_FAIL;
	}
#endif	

#endif

	// Initial MassMemAccess instance
	if (VencVideoIn_InitMassMemAccess(&(ptVideoInInfo->hMassMemAccObj), &(ptVideoInInfo->_tInitNeedInfo)) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Initial MassMemAccess object fail !!\n");
		return S_FAIL;    
	}
	DBPRINT0("[VENC_VIDEOIN] Initial MassMemAccess Success !!\n");

	// Initial XMLWrapper instance
	if (VencVideoIn_InitXMLWrapper(&(ptVideoInInfo->hXMLWrapperObj)) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Initial XMLWrapper object fail !!\n");
		return S_FAIL;    
	}
	DBPRINT0("[VENC_VIDEOIN] Initial XMLWrapper Success !!\n");


	// Initial SharedBuffer instance
	if (VencVideoIn_InitSharedBuffer(ptVideoInInfo, &(ptVideoInInfo->_tInitNeedInfo)) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Initial SharedBuffer object fail !!\n");
		return S_FAIL;    
	}
	DBPRINT0("[VENC_VIDEOIN] Initial SharedBuffer Success !!\n");

#ifdef _VMA_IBPE
	// Initial PrivacyMask instance
	if (VencVideoIn_InitPrivacyMask(ptVideoInInfo, &(ptVideoInInfo->_tInitNeedInfo), ptVideoInInfo->hMemMgrObj) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Initial PrivacyMask object fail !!\n");
		return S_FAIL;    
	}
	DBPRINT0("[VENC_VIDEOIN] Initial PrivacyMask Success !!\n");
#endif

	// Initial Frame rate control instance
	if (VencVideoIn_InitFrameRateCtrl(&(ptVideoInInfo->hFRCtrlObj)) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Initial Frame rate control object fail !!\n");
		return S_FAIL;
	}

#ifdef _DEFECT_PIXEL_ENABLE_  
	if (VencVideoIn_InitDPC(&(ptVideoInInfo->hDPCObject), ptVideoInInfo->hMemMgrObj) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Initial DPC object fail !!\n");
		return S_FAIL;
	}
#endif
	return S_OK;   
}

/* ========================================================================== */
void VencVideoIn_ReleaseVML(TVideoInInfo *ptVideoInInfo)
{
	// ===== Release buffers =====
#ifdef _YUV422_
	if (ptVideoInInfo->pbyYUV420FrameBuff != NULL)
	{
		if (MemMgr_FreeMemory(ptVideoInInfo->hMemMgrObj, (DWORD)ptVideoInInfo->pbyYUV420FrameBuff) != S_OK) {
			fprintf(stderr, "[VENC_VIDEOIN] Release pbyYUV420FrameBuff fail !!\n");	
		}
		ptVideoInInfo->pbyYUV420FrameBuff= NULL;
		
	}
#endif
	
	if (MemMgr_FreeMemory(ptVideoInInfo->hMemMgrObj, (DWORD)ptVideoInInfo->pwPhotoLDCRefFrame) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Release Photometric buffer fail !!\n");	
	}
#ifdef _PRE_NR_
	{
		int i=0;
		for (i = 0; i < BUFF_SIZE; i++)
		{
			if (ptVideoInInfo->atIBPEOutVFB[i].pbyBufY != NULL)
			{
				if (ptVideoInInfo->hMemMgrObj != NULL)
				{
					MemMgr_FreeMemory(ptVideoInInfo->hMemMgrObj, (DWORD)ptVideoInInfo->atIBPEOutVFB[i].pbyBufY);
				}
			}		
		}
	}
#endif
	// ===== Release instance =====
#ifdef _VMA_IBPE
	if (VencVideoIn_ReleasePrivacyMask(ptVideoInInfo) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Release PrivacyMask  fail !!\n");
	}

	if (ImgBackEndProcess_Release(&(ptVideoInInfo->hIBPObj)) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Release image back-end object fail !!\n");
	}

#ifdef _PRE_NR_	
	if (ImgBackEndProcess_Release(&(ptVideoInInfo->hSencodPhaseIBPObj)) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Release image back-end object fail !!\n");
	}
#endif	
	
	
#ifdef _VMA_IBPE_BACK_DOOR
	VencVideoIn_ReleaseIBPParam(ptVideoInInfo);
#endif

#endif
	
	// Release MassMemAccess instance
	if (MassMemAccess_Release(&(ptVideoInInfo->hMassMemAccObj)) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Release MassMemAccess object fail !!\n");
	}

	// Release xmlwrapper instance
	if (XmlWrapper_Release(&(ptVideoInInfo->hXMLWrapperObj)) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Release XMLWrapper ojbect fail !!\n");
	}

	// Release shared buffer instance
	if (SharedBuffMgr_Release(&(ptVideoInInfo->hSharedBuferObj)) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Release shared buffer manager object fail !!\n");
	} 

	//Release framectrl instatnce
	if (FrameRateCtrl_Release(&(ptVideoInInfo->hFRCtrlObj)) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Release frame rate control object fail!!\n");
	}

#ifdef _DEFECT_PIXEL_ENABLE_
	if (VencVideoIn_ReleaseDPC(&(ptVideoInInfo->hDPCObject)) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Release DPC object fail !!\n");
	}
#endif
}

#ifdef _VMA_IBPE
SCODE VencVideoIn_InitStatus_ImgBackEndProcess(TImgBackEndProcessState *ptIBPState,TIBPEPMask *ptIBPEPMask)
{
	ptIBPState->pbyPrivacyMaskBuff = ptIBPEPMask->pbyPMask;
	ptIBPState->bUpdateEn = TRUE;
	return S_OK;
}	
#endif	
/* ========================================================================== */
SCODE VencVideoIn_InitStatus_MassMemAccess(TMassMemAccessState *ptMassMemAccState)
{
	//MassMemAccess state
	memset(ptMassMemAccState, 0, sizeof(TMassMemAccessState));
	ptMassMemAccState->b2DEnable = FALSE;
	ptMassMemAccState->bUsePhysAddr = FALSE;
	ptMassMemAccState->bFillingEnable = FALSE;
	ptMassMemAccState->dwSrcBusNum = 1;
	ptMassMemAccState->dwDstBusNum = 2;
	ptMassMemAccState->dwMaxBurstLength = 128; 
	ptMassMemAccState->dwTransSize = 0;  // should be setting in process
	ptMassMemAccState->pbySrcAddr = NULL; // should be setting in process
	ptMassMemAccState->pbyDstAddr = NULL; // should be setting in process
	return S_OK;
}

/* ========================================================================== */
SCODE VencVideoIn_InitStates(TVideoInInfo *ptVideoInInfo)
{
	// initial state parameter, some state will not change after initial 
	// assign here to avoid reassign each time.
	TMassMemAccessState *ptMassMemAccState = &(ptVideoInInfo->tMassMemAccState);
#ifdef _VMA_IBPE
	TImgBackEndProcessState *ptIBPState = &(ptVideoInInfo->tIBPState);

	VencVideoIn_InitStatus_ImgBackEndProcess(ptIBPState, &ptVideoInInfo->tIBPEPMask);
#endif // _VMA_IBPE	
	VencVideoIn_InitStatus_MassMemAccess(ptMassMemAccState);
	ptVideoInInfo->bISPEnable = FALSE;
	return S_OK;
}
