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
 * venc_videoin_conf.c
 *
 * \brief
 * Videoin process setoption implement.
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

//-------------StartElementHandler-------------
SCODE VencVideoIn_SetMotionWinTag(HANDLE hObject, const CHAR *szMotion, const CHAR** atts);

//----------------CharDataHandler----------------
void VencVideoIn_SetFrameRate(HANDLE hObject, const CHAR *szFrameRate, SDWORD len);
void VencVideoIn_SetFrameRateCtrlEnable(HANDLE hObject, const CHAR *szEnable, SDWORD len);
#ifdef _VMA_IBPE
//ibpe ctrl
void VencVideoIn_SetIBPECtrlYCmpntEn(HANDLE hObject, const CHAR *szElement, SDWORD len);
void VencVideoIn_SetIBPECtrlCCmpntEn(HANDLE hObject, const CHAR *szElement, SDWORD len);
void VencVideoIn_SetIBPECtrlYOutEn(HANDLE hObject, const CHAR *szElement, SDWORD len);
void VencVideoIn_SetIBPECtrlCOutEn(HANDLE hObject, const CHAR *szElement, SDWORD len);
void VencVideoIn_SetIBPECtrlPixelFormat(HANDLE hObject, const CHAR *szElement, SDWORD len);
//twice ibpe
void VencVideoIn_SetTwiceIBPEEnable(HANDLE hObject, const CHAR *szElement, SDWORD len);
//deinterlace
void VencVideoIn_SetDEIEnable(HANDLE hObject, const CHAR* szElement, SDWORD len);
void VencVideoIn_SetDEIMode(HANDLE hObject, const CHAR* szElement, SDWORD len);
void VencVideoIn_SetDEIPicFormat(HANDLE hObject, const CHAR* szElement, SDWORD len);
// edge enhancement
void VencVideoIn_SetEdgeEnhancementEnable(HANDLE hObject, const CHAR *szEEEnable, SDWORD len);
void VencVideoIn_SetEdgeEnhancementStrength(HANDLE hObject, const CHAR *szEEStrength, SDWORD len);
void VencVideoIn_SetEdgeEnhancementEdgeSensitivity(HANDLE hObject, const CHAR *szEESensitivity, SDWORD len);
void VencVideoIn_SetEdgeEnhancementNoiseSensitivity(HANDLE hObject, const CHAR *szEESensitivity, SDWORD len);
void VencVideoIn_SetEdgeEnhancementClip(HANDLE hObject, const CHAR *szEdgeEnhanceClip, SDWORD len);
void VencVideoIn_SetSecondPhaseEdgeEnhancementEnable(HANDLE hObject, const CHAR *szEdgeEnhanceClip, SDWORD len);
void VencVideoIn_SetSecondPhaseEdgeEnhancementStrength(HANDLE hObject, const CHAR *szEdgeEnhanceClip, SDWORD len);
void VencVideoIn_SetSecondPhaseEdgeEnhancementEdgeSensitivity(HANDLE hObject, const CHAR *szEdgeEnhanceClip, SDWORD len);
void VencVideoIn_SetSecondPhaseEdgeEnhancementNoiseSensitivity(HANDLE hObject, const CHAR *szEdgeEnhanceClip, SDWORD len);
void VencVideoIn_SetSecondPhaseEdgeEnhancementClip(HANDLE hObject, const CHAR *szEdgeEnhanceClip, SDWORD len);
//denoise
void VencVideoIn_SetDeNoiseEnable(HANDLE hObject, const CHAR *szDNEnable, SDWORD len);
void VencVideoIn_SetDeNoiseMode(HANDLE hObject, const CHAR* szElement, SDWORD len);
void VencVideoIn_SetDeNoiseNormalModeStrength(HANDLE hObject, const CHAR* szElement, SDWORD len);
void VencVideoIn_SetDeNoiseStrongRefStrength(HANDLE hObject, const CHAR* szElement, SDWORD len);
void VencVideoIn_SetDeNoiseStrongCurrWeight(HANDLE hObject, const CHAR* szElement, SDWORD len);
void VencVideoIn_SetDeNoiseStrongMotionAdaptiveEn(HANDLE hObject, const CHAR* szElement, SDWORD len);
void VencVideoIn_SetSecondPhaseDeNoiseEnable(HANDLE hObject, const CHAR* szElement, SDWORD len);
void VencVideoIn_SetSecondPhaseDeNoiseMode(HANDLE hObject, const CHAR* szElement, SDWORD len);
void VencVideoIn_SetSecondPhaseDeNoiseNormalModeStrength(HANDLE hObject, const CHAR* szElement, SDWORD len);
void VencVideoIn_SetSecondPhaseDeNoiseStrongRefStrength(HANDLE hObject, const CHAR* szElement, SDWORD len); 
void VencVideoIn_SetSecondPhaseDeNoiseStrongCurrWeight(HANDLE hObject, const CHAR* szElement, SDWORD len);
void VencVideoIn_SetSecondPhaseDeNoiseStrongMotionAdaptiveEn(HANDLE hObject, const CHAR* szElement, SDWORD len);
//geometric
void VencVideoIn_SetGeometricSelectMode(HANDLE hObject, const CHAR *szGeoMtcSelectMode, SDWORD len);
SCODE VencVideoIn_SetGeometricModeTag(HANDLE hObject, const CHAR *szGeoMtcMode, const CHAR** atts);
void VencVideoIn_SetGeometricFwdMpgParam1Y(HANDLE hObject, const CHAR *szFwdMpgParam1Y, SDWORD len);
void VencVideoIn_SetGeometricFwdMpgParam2Y(HANDLE hObject, const CHAR *szFwdMpgParam2Y, SDWORD len);
void VencVideoIn_SetGeometricFwdMpgParam3Y(HANDLE hObject, const CHAR *szFwdMpgParam3Y, SDWORD len);
void VencVideoIn_SetGeometricPosFwdMpgParam1Y(HANDLE hObject, const CHAR *szPosFwdMpgParam1Y, SDWORD len);
void VencVideoIn_SetGeometricPosFwdMpgParam2Y(HANDLE hObject, const CHAR *szPosFwdMpgParam2Y, SDWORD len);
void VencVideoIn_SetGeometricPosFwdMpgParam3Y(HANDLE hObject, const CHAR *szPosFwdMpgParam3Y, SDWORD len);
void VencVideoIn_SetGeometricFwdMpgParam1CbCr(HANDLE hObject, const CHAR *szFwdMpgParam1CbCr, SDWORD len);
void VencVideoIn_SetGeometricFwdMpgParam2CbCr(HANDLE hObject, const CHAR *szFwdMpgParam2CbCr, SDWORD len);
void VencVideoIn_SetGeometricFwdMpgParam3CbCr(HANDLE hObject, const CHAR *szFwdMpgParam3CbCr, SDWORD len);
void VencVideoIn_SetGeometricPosFwdMpgParam1CbCr(HANDLE hObject, const CHAR *szPosFwdMpgParam1CbCr, SDWORD len);
void VencVideoIn_SetGeometricPosFwdMpgParam2CbCr(HANDLE hObject, const CHAR *szPosFwdMpgParam2CbCr, SDWORD len);
void VencVideoIn_SetGeometricPosFwdMpgParam3CbCr(HANDLE hObject, const CHAR *szPosFwdMpgParam3CbCr, SDWORD len);
//PMASK
void VencVideoIn_SetPMask(HANDLE hObject, const CHAR *szPmask, SDWORD len);
void VencVideoIn_SetPMaskSwitch(HANDLE hObject, const CHAR *szSwitch, SDWORD len);
void VencVideoIn_SetPMaskAllColor(HANDLE hObject, const CHAR *szColor, SDWORD len);
// Motion Detection
void VencVideoIn_SetMotion(HANDLE hObject, const CHAR *szMotion, SDWORD len);
void VencVideoIn_SetMotionMDPMaskEn(HANDLE hObject, const CHAR *szElement, SDWORD len);
void VencVideoIn_SetMotionXTranslate(HANDLE hObject, const CHAR *szXTranslate, SDWORD len);
void VencVideoIn_SetMotionYTranslate(HANDLE hObject, const CHAR *szYTranslate, SDWORD len);
void VencVideoIn_SetMotionXScale(HANDLE hObject, const CHAR *szXScale, SDWORD len);
void VencVideoIn_SetMotionYScale(HANDLE hObject, const CHAR *szYScale, SDWORD len);
void VencVideoIn_SetMotionWinEnable(HANDLE hObject, const CHAR *szEnable, SDWORD len);
void VencVideoIn_SetMotionWinName(HANDLE hObject, const CHAR *szName, SDWORD len);
void VencVideoIn_SetMotionWinLeft(HANDLE hObject, const CHAR *szLeft, SDWORD len);
void VencVideoIn_SetMotionWinTop(HANDLE hObject, const CHAR *szTop, SDWORD len);
void VencVideoIn_SetMotionWinWidth(HANDLE hObject, const CHAR *szWidth, SDWORD len);
void VencVideoIn_SetMotionWinHeight(HANDLE hObject, const CHAR *szHeight, SDWORD len);
void VencVideoIn_SetMotionWinSens(HANDLE hObject, const CHAR *szSensitivity, SDWORD len);
void VencVideoIn_SetMotionWinObjsize(HANDLE hObject, const CHAR *szObjesize, SDWORD len);
//photometric
void VencVideoIn_SetPhotometricMode(HANDLE hObject, const CHAR *szPhotometricMode, SDWORD len);
void VencVideoIn_SetPhotometricLSCRatio(HANDLE hObject, const CHAR *szPhotometricLSCRatio, SDWORD len);
#endif
/* =========================================================================================== */
TXmlWrapperTreeMap ptVideoInTreeMap[] =
{

	//{"root/venc_videoin/resolution", , },
	{"root/venc_videoin/frame_ctrl/enable", NULL, &VencVideoIn_SetFrameRateCtrlEnable, NULL},
	{"root/venc_videoin/frame_ctrl/frame_rate", NULL, &VencVideoIn_SetFrameRate, NULL},
	//motion setting
#ifdef _VMA_IBPE
	{"root/venc_videoin/motion/enable", NULL, &VencVideoIn_SetMotion, NULL},
	{"root/venc_videoin/motion/MDPMaskEn", NULL, &VencVideoIn_SetMotionMDPMaskEn, NULL},
	{"root/venc_videoin/motion/Transformation/Translate/x", NULL, &VencVideoIn_SetMotionXTranslate, NULL},
	{"root/venc_videoin/motion/Transformation/Translate/y", NULL, &VencVideoIn_SetMotionYTranslate, NULL},
	{"root/venc_videoin/motion/Transformation/scale/x", NULL, &VencVideoIn_SetMotionXScale, NULL},
	{"root/venc_videoin/motion/Transformation/scale/y", NULL, &VencVideoIn_SetMotionYScale, NULL},
	{"root/venc_videoin/motion/win", &VencVideoIn_SetMotionWinTag, NULL, NULL},
	{"root/venc_videoin/motion/win/active", NULL, &VencVideoIn_SetMotionWinEnable, NULL},
	{"root/venc_videoin/motion/win/name", NULL, &VencVideoIn_SetMotionWinName, NULL},
	{"root/venc_videoin/motion/win/left", NULL, &VencVideoIn_SetMotionWinLeft, NULL},
	{"root/venc_videoin/motion/win/top", NULL, &VencVideoIn_SetMotionWinTop, NULL},
	{"root/venc_videoin/motion/win/width", NULL, &VencVideoIn_SetMotionWinWidth, NULL},
	{"root/venc_videoin/motion/win/height", NULL, &VencVideoIn_SetMotionWinHeight, NULL},
	{"root/venc_videoin/motion/win/sensitivity", NULL, &VencVideoIn_SetMotionWinSens, NULL},
	{"root/venc_videoin/motion/win/objsize", NULL, &VencVideoIn_SetMotionWinObjsize, NULL},
	//twice ibpe
	{"root/venc_videoin/isp/pre_nr/enable", NULL, &VencVideoIn_SetTwiceIBPEEnable, NULL},
	//deinterlacer category
	{"root/venc_videoin/isp/deinterlacer/enable", NULL, &VencVideoIn_SetDEIEnable, NULL}, 
	{"root/venc_videoin/isp/deinterlacer/picformat", NULL, &VencVideoIn_SetDEIPicFormat, NULL},
	{"root/venc_videoin/isp/deinterlacer/mode", NULL, &VencVideoIn_SetDEIMode, NULL},
	// edge enhancement
	{"root/venc_videoin/isp/edge_enhancement/pre/enable", NULL, &VencVideoIn_SetEdgeEnhancementEnable, NULL},
	{"root/venc_videoin/isp/edge_enhancement/pre/strength", NULL, &VencVideoIn_SetEdgeEnhancementStrength, NULL},
	{"root/venc_videoin/isp/edge_enhancement/pre/edge_sensitivity", NULL, &VencVideoIn_SetEdgeEnhancementEdgeSensitivity, NULL},
	{"root/venc_videoin/isp/edge_enhancement/pre/noise_sensitivity", NULL, &VencVideoIn_SetEdgeEnhancementNoiseSensitivity, NULL},
	{"root/venc_videoin/isp/edge_enhancement/pre/clip", NULL, &VencVideoIn_SetEdgeEnhancementClip, NULL},
	{"root/venc_videoin/isp/edge_enhancement/post/enable", NULL, &VencVideoIn_SetSecondPhaseEdgeEnhancementEnable, NULL},
	{"root/venc_videoin/isp/edge_enhancement/post/strength", NULL, &VencVideoIn_SetSecondPhaseEdgeEnhancementStrength, NULL},
	{"root/venc_videoin/isp/edge_enhancement/post/edge_sensitivity", NULL, &VencVideoIn_SetSecondPhaseEdgeEnhancementEdgeSensitivity, NULL},
	{"root/venc_videoin/isp/edge_enhancement/post/noise_sensitivity", NULL, &VencVideoIn_SetSecondPhaseEdgeEnhancementNoiseSensitivity, NULL},
	{"root/venc_videoin/isp/edge_enhancement/post/clip", NULL, &VencVideoIn_SetSecondPhaseEdgeEnhancementClip, NULL},
	// de-noise
	{"root/venc_videoin/isp/denoise/pre/enable", NULL, &VencVideoIn_SetDeNoiseEnable, NULL},
	{"root/venc_videoin/isp/denoise/pre/mode", NULL, &VencVideoIn_SetDeNoiseMode, NULL},
	{"root/venc_videoin/isp/denoise/pre/normal_mode_strength", NULL, &VencVideoIn_SetDeNoiseNormalModeStrength, NULL},
	{"root/venc_videoin/isp/denoise/pre/strong_ref_strength", NULL, &VencVideoIn_SetDeNoiseStrongRefStrength, NULL}, 
	{"root/venc_videoin/isp/denoise/pre/strong_curr_weight", NULL, &VencVideoIn_SetDeNoiseStrongCurrWeight, NULL},
	{"root/venc_videoin/isp/denoise/pre/strong_motion_adaptive_enable", NULL, &VencVideoIn_SetDeNoiseStrongMotionAdaptiveEn, NULL},
	{"root/venc_videoin/isp/denoise/post/enable", NULL, &VencVideoIn_SetSecondPhaseDeNoiseEnable, NULL},
	{"root/venc_videoin/isp/denoise/post/mode", NULL, &VencVideoIn_SetSecondPhaseDeNoiseMode, NULL},
	{"root/venc_videoin/isp/denoise/post/normal_mode_strength", NULL, &VencVideoIn_SetSecondPhaseDeNoiseNormalModeStrength, NULL},
	{"root/venc_videoin/isp/denoise/post/strong_ref_strength", NULL, &VencVideoIn_SetSecondPhaseDeNoiseStrongRefStrength, NULL}, 
	{"root/venc_videoin/isp/denoise/post/strong_curr_weight", NULL, &VencVideoIn_SetSecondPhaseDeNoiseStrongCurrWeight, NULL},
	{"root/venc_videoin/isp/denoise/post/strong_motion_adaptive_enable", NULL, &VencVideoIn_SetSecondPhaseDeNoiseStrongMotionAdaptiveEn, NULL},
	//geometric
	{"root/venc_videoin/isp/geometric/select_mode", NULL, &VencVideoIn_SetGeometricSelectMode, NULL},
	{"root/venc_videoin/isp/geometric/mode", &VencVideoIn_SetGeometricModeTag, NULL, NULL},
	{"root/venc_videoin/isp/geometric/mode/FwdMpgParam1Y", NULL, &VencVideoIn_SetGeometricFwdMpgParam1Y, NULL},
	{"root/venc_videoin/isp/geometric/mode/FwdMpgParam2Y", NULL, &VencVideoIn_SetGeometricFwdMpgParam2Y, NULL},
	{"root/venc_videoin/isp/geometric/mode/FwdMpgParam3Y", NULL, &VencVideoIn_SetGeometricFwdMpgParam3Y, NULL},
	{"root/venc_videoin/isp/geometric/mode/PosFwdMpgParam1Y", NULL, &VencVideoIn_SetGeometricPosFwdMpgParam1Y, NULL},
	{"root/venc_videoin/isp/geometric/mode/PosFwdMpgParam2Y", NULL, &VencVideoIn_SetGeometricPosFwdMpgParam2Y, NULL},
	{"root/venc_videoin/isp/geometric/mode/PosFwdMpgParam3Y", NULL, &VencVideoIn_SetGeometricPosFwdMpgParam3Y, NULL},
	{"root/venc_videoin/isp/geometric/mode/FwdMpgParam1CbCr", NULL, &VencVideoIn_SetGeometricFwdMpgParam1CbCr, NULL},
	{"root/venc_videoin/isp/geometric/mode/FwdMpgParam2CbCr", NULL, &VencVideoIn_SetGeometricFwdMpgParam2CbCr, NULL},
	{"root/venc_videoin/isp/geometric/mode/FwdMpgParam3CbCr", NULL, &VencVideoIn_SetGeometricFwdMpgParam3CbCr, NULL},	
	{"root/venc_videoin/isp/geometric/mode/PosFwdMpgParam1CbCr", NULL, &VencVideoIn_SetGeometricPosFwdMpgParam1CbCr, NULL},
	{"root/venc_videoin/isp/geometric/mode/PosFwdMpgParam2CbCr", NULL, &VencVideoIn_SetGeometricPosFwdMpgParam2CbCr, NULL},
	{"root/venc_videoin/isp/geometric/mode/PosFwdMpgParam3CbCr", NULL, &VencVideoIn_SetGeometricPosFwdMpgParam3CbCr, NULL},
	//ibpe ctrl
	{"root/venc_videoin/ibpe_ctrl/YCmpntEn", NULL, &VencVideoIn_SetIBPECtrlYCmpntEn, NULL},
	{"root/venc_videoin/ibpe_ctrl/CCmpntEn", NULL, &VencVideoIn_SetIBPECtrlCCmpntEn, NULL},
	{"root/venc_videoin/ibpe_ctrl/YOutEn", NULL, &VencVideoIn_SetIBPECtrlYOutEn, NULL},
	{"root/venc_videoin/ibpe_ctrl/COutEn", NULL, &VencVideoIn_SetIBPECtrlCOutEn, NULL},
	{"root/venc_videoin/ibpe_ctrl/PixelFormat", NULL, &VencVideoIn_SetIBPECtrlPixelFormat, NULL},
	//photometric
	{"root/venc_videoin/isp/photometric/mode", NULL, &VencVideoIn_SetPhotometricMode, NULL},
	{"root/venc_videoin/isp/photometric/lsc_ratio", NULL, &VencVideoIn_SetPhotometricLSCRatio, NULL},
	//privacy mask setting
	{"root/venc_videoin/pmask/enable", NULL, &VencVideoIn_SetPMask, NULL},
	{"root/venc_videoin/pmask/switch", NULL, &VencVideoIn_SetPMaskSwitch, NULL},
	{"root/venc_videoin/pmask/color", NULL, &VencVideoIn_SetPMaskAllColor, NULL},
#endif    
	{NULL, NULL, NULL}
};

TVideoInCfg g_tVideoInCfg = {   
	FALSE,				//bDeinterlace
	-1,					//dwMotionThreshold
	-1,					//dwStaticPeriod
	FALSE,				//bResetStatistics
	FALSE,				//bDemo
	FALSE,				//bSubtitleDetect
	FALSE,				//bTmprlNR
};

/* ============================================================================================= */
SCODE VencVideoIn_InitConfigParam(TVideoInInfo *ptVideoInInfo)
{
#ifdef _VMA_IBPE
	DWORD dwGeoMtcModeIndex, dwGeoMtcResIndex;

	//Reset all values of IBPE state
	memset(&ptVideoInInfo->tIBPOpt,0x0,sizeof(TVideoInIBPOptions));
	// IBPE relative
	//ptVideoInInfo->tIBPOpt.bIBPEEnable = FALSE;
	// edge enhancement relative
	ptVideoInInfo->tIBPOpt.tEdgeEnhancementCfg.bEdgeEnhancementEnable = FALSE;
	//	ptVideoInInfo->tIBPOpt.tEdgeEnhancementCfg.dwEdgeEnhanceSensitivity = -1;
	ptVideoInInfo->tIBPOpt.tEdgeEnhancementCfg.dwEdgeEnhanceStrength = -1;
	// denoise relative
	ptVideoInInfo->tIBPOpt.tDeNoiseCfg.bDeNoiseEnable = FALSE;
	// geometric relative
	ptVideoInInfo->tIBPOpt.tGeoMtcCfg.bEnable = FALSE;
	ptVideoInInfo->tIBPOpt.tGeoMtcCfg.eGMSelectMode = geomtcmNum;
	for (dwGeoMtcModeIndex = 0; dwGeoMtcModeIndex < geomtcmNum; dwGeoMtcModeIndex++) {
		for (dwGeoMtcResIndex = 0; dwGeoMtcResIndex < geomtcresNum; dwGeoMtcResIndex++) {
			ptVideoInInfo->tIBPOpt.tGeoMtcCfg.aaGMTypeValue[dwGeoMtcModeIndex][dwGeoMtcResIndex].dwFwdMpgParam1Y = -1;
			ptVideoInInfo->tIBPOpt.tGeoMtcCfg.aaGMTypeValue[dwGeoMtcModeIndex][dwGeoMtcResIndex].dwFwdMpgParam2Y = -1;
			ptVideoInInfo->tIBPOpt.tGeoMtcCfg.aaGMTypeValue[dwGeoMtcModeIndex][dwGeoMtcResIndex].dwFwdMpgParam3Y = -1;
			ptVideoInInfo->tIBPOpt.tGeoMtcCfg.aaGMTypeValue[dwGeoMtcModeIndex][dwGeoMtcResIndex].bPosFwdMpgParam1Y = FALSE;
			ptVideoInInfo->tIBPOpt.tGeoMtcCfg.aaGMTypeValue[dwGeoMtcModeIndex][dwGeoMtcResIndex].bPosFwdMpgParam2Y = FALSE;
			ptVideoInInfo->tIBPOpt.tGeoMtcCfg.aaGMTypeValue[dwGeoMtcModeIndex][dwGeoMtcResIndex].bPosFwdMpgParam3Y = FALSE;
			ptVideoInInfo->tIBPOpt.tGeoMtcCfg.aaGMTypeValue[dwGeoMtcModeIndex][dwGeoMtcResIndex].dwFwdMpgParam1CbCr = -1;
			ptVideoInInfo->tIBPOpt.tGeoMtcCfg.aaGMTypeValue[dwGeoMtcModeIndex][dwGeoMtcResIndex].dwFwdMpgParam2CbCr = -1;
			ptVideoInInfo->tIBPOpt.tGeoMtcCfg.aaGMTypeValue[dwGeoMtcModeIndex][dwGeoMtcResIndex].dwFwdMpgParam3CbCr = -1;
			ptVideoInInfo->tIBPOpt.tGeoMtcCfg.aaGMTypeValue[dwGeoMtcModeIndex][dwGeoMtcResIndex].bPosFwdMpgParam1CbCr = FALSE;
			ptVideoInInfo->tIBPOpt.tGeoMtcCfg.aaGMTypeValue[dwGeoMtcModeIndex][dwGeoMtcResIndex].bPosFwdMpgParam2CbCr = FALSE;
			ptVideoInInfo->tIBPOpt.tGeoMtcCfg.aaGMTypeValue[dwGeoMtcModeIndex][dwGeoMtcResIndex].bPosFwdMpgParam3CbCr = FALSE;
		}
	}

	//PrivacyMask relative 
	ptVideoInInfo->tPMaskCfg.bEnable = FALSE;
	ptVideoInInfo->tPMaskCfg.bColorChange = FALSE;
	ptVideoInInfo->tPMaskCfg.dwColor = -1;
	ptVideoInInfo->tPMaskCfg.bSwitchChange = FALSE;
	ptVideoInInfo->tPMaskCfg.dwSwitch = -1;
#endif
	//photometric relative
	ptVideoInInfo->tPhotoMtcCfg.bModified = FALSE;
	ptVideoInInfo->tPhotoMtcCfg.ePhotoMtcMode = photomtcmNum;

	g_tVideoInCfg.dwMotionThreshold = -1;
	g_tVideoInCfg.dwStaticPeriod = -1;
	g_tVideoInCfg.bResetStatistics = FALSE;
	g_tVideoInCfg.bDemo = FALSE;
	g_tVideoInCfg.bSubtitleDetect = FALSE;
	g_tVideoInCfg.bTmprlNR = FALSE;

	return S_OK;
}
#ifdef _VMA_IBPE
//twice ibpe
void VencVideoIn_SetTwiceIBPEEnable(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	BOOL	bEnable = FALSE;
	
	snprintf(szTemp, len+1, "%s", szElement);
	if (strcmp(szTemp, "on") == 0)
		bEnable = TRUE;

	if (ptIBPOpt->bTWICEIBPEEnable != bEnable) {
		ptIBPOpt->bTWICEIBPEEnable = bEnable;		
#ifdef _PRE_NR_		
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif
	}
	
}
//IBPE ctrl
void VencVideoIn_SetIBPECtrlYCmpntEn(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	//TImgBackEndProcessState *ptIBPState = (TImgBackEndProcessState *)hObject;
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	memset(szTemp, 0x0, sizeof(szTemp));

	snprintf(szTemp, len+1, "%s", szElement);
	if (strcasecmp(szTemp, "true") == 0) {
		ptIBPOpt->tIBPECtrl.bYCmpntEn = TRUE;
	} else {
		ptIBPOpt->tIBPECtrl.bYCmpntEn = FALSE;
	}
}

void VencVideoIn_SetIBPECtrlCCmpntEn(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	//TImgBackEndProcessState *ptIBPState = (TImgBackEndProcessState *)hObject;
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	memset(szTemp, 0x0, sizeof(szTemp));

	snprintf(szTemp, len+1, "%s", szElement);
	if (strcasecmp(szTemp, "true") == 0) {
		ptIBPOpt->tIBPECtrl.bCCmpntEn = TRUE;
	} else {
		ptIBPOpt->tIBPECtrl.bCCmpntEn = FALSE;
	}
}

void VencVideoIn_SetIBPECtrlYOutEn(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	//TImgBackEndProcessState *ptIBPState = (TImgBackEndProcessState *)hObject;
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	memset(szTemp, 0x0, sizeof(szTemp));

	snprintf(szTemp, len+1, "%s", szElement);
	if (strcasecmp(szTemp, "true") == 0) {
		ptIBPOpt->tIBPECtrl.bYOutEn = TRUE;
	} else {
		ptIBPOpt->tIBPECtrl.bYOutEn = FALSE;
	}
}

void VencVideoIn_SetIBPECtrlCOutEn(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	//TImgBackEndProcessState *ptIBPState = (TImgBackEndProcessState *)hObject;
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	memset(szTemp, 0x0, sizeof(szTemp));

	snprintf(szTemp, len+1, "%s", szElement);
	if (strcasecmp(szTemp, "true") == 0) {
		ptIBPOpt->tIBPECtrl.bCOutEn = TRUE;
	} else {
		ptIBPOpt->tIBPECtrl.bCOutEn = FALSE;
	}
}

void VencVideoIn_SetIBPECtrlPixelFormat(HANDLE hObject, const CHAR *szElement, SDWORD len)
{  
	//TImgBackEndProcessState *ptIBPState = (TImgBackEndProcessState *)hObject;
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	memset(szTemp, 0x0, sizeof(szTemp));

	snprintf(szTemp, len+1, "%s", szElement);
	ptIBPOpt->tIBPECtrl.ePixelFormat = strtoul(szTemp, NULL, 10);
}
/* ============================================================================================= */
#endif //_VMA_IBPE

void VencVideoIn_SetFrameRate(HANDLE hObject, const CHAR *szFrameRate, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	DWORD dwFrameRate = strtoul(szFrameRate, NULL, 10);

	TFRCtrlOptions tFRCtrlOptions;

	memset(&tFRCtrlOptions,0x0,sizeof(TFRCtrlOptions));
	tFRCtrlOptions.eOptionFlags = FRCTRL_CHANGE_FRAMERATE;
	if (ptVideoInInfo->bFrameRateCtrlEnable == TRUE)
	{
		tFRCtrlOptions.adwUserData[0] = dwFrameRate;
	}
	else
	{
		tFRCtrlOptions.adwUserData[0] = 0;
	}
	//printf("[VENC_VIDEOIN] VencVideoIn_SetFrameRate (%d)  ... \n",tFRCtrlOptions.adwUserData[0]);
	FrameRateCtrl_SetOption(ptVideoInInfo->hFRCtrlObj, &tFRCtrlOptions);
}
/* ============================================================================================= */

void VencVideoIn_SetFrameRateCtrlEnable(HANDLE hObject, const CHAR *szEnable, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	CHAR szTemp[len+1];
	snprintf(szTemp, len+1, "%s", szEnable);

	DBPRINT0("[VENC_VIDEOIN] VencVideoIn_SetFrameRateCtrlEnable ... \n");

	if (strcmp(szTemp, "on") == 0) {           
		ptVideoInInfo->bFrameRateCtrlEnable = TRUE;
	}
	else {
		ptVideoInInfo->bFrameRateCtrlEnable = FALSE;
	}
}

#ifdef _VMA_IBPE
/* ============================================================================================= */
void VencVideoIn_SetPMask(HANDLE hObject, const CHAR *szPmask, SDWORD len)
{   
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	CHAR szTemp[len+1];
	snprintf(szTemp, len+1, "%s", szPmask);

	DBPRINT0("[VENC_VIDEOIN] VencVideoIn_SetPMask ... \n");

	if (strcmp(szTemp, "on") == 0) {           
		ptVideoInInfo->tPMaskCfg.bEnable = TRUE;
		ptVideoInInfo->tPMaskCfg.bOutPMaskEn = TRUE;	    
	}
	else {
		ptVideoInInfo->tPMaskCfg.bEnable = FALSE;
		ptVideoInInfo->tPMaskCfg.bOutPMaskEn = FALSE;	
	}
}

/* ============================================================================================= */
void VencVideoIn_SetPMaskAllColor(HANDLE hObject, const CHAR *szColor, SDWORD len)
{   
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	DWORD dwColor = strtoul(szColor, (char**)NULL, 10);

	if (ptVideoInInfo->tPMaskCfg.dwColor != dwColor) {
		ptVideoInInfo->tPMaskCfg.dwColor = dwColor;
		ptVideoInInfo->tPMaskCfg.bColorChange = TRUE;
	}
}

/* ============================================================================================= */
void VencVideoIn_SetPMaskSwitch(HANDLE hObject, const CHAR *szSwitch, SDWORD len)
{   
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	DWORD dwSwitch = strtoul(szSwitch, (char**)NULL, 10);

	if (ptVideoInInfo->tPMaskCfg.dwSwitch != dwSwitch) {
		ptVideoInInfo->tPMaskCfg.dwSwitch = dwSwitch;
		ptVideoInInfo->tPMaskCfg.bSwitchChange = TRUE;
	}
}

/* =========================================================================================== */
void VencVideoIn_SetBitmakPath(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	CHAR szBitmapPath[len+1];
	memset(szBitmapPath, 0x0, sizeof(szBitmapPath));

	snprintf(szBitmapPath, len+1, "%s", szParam);
	memset(ptVideoInInfo->szBitmapPath, 0x0, sizeof(ptVideoInInfo->szBitmapPath));
	strncpy(ptVideoInInfo->szBitmapPath, szBitmapPath, strlen(szBitmapPath));
	//printf("[venc_videoin_conf](%d)ptVideoInInfo->szBitmapPath=%s\n", __LINE__, ptVideoInInfo->szBitmapPath);
}

static SCODE SetBitmapPath(TVideoInInfo *ptVideoInInfo )
{
	HANDLE hXmlWrapperObj;
	TXmlWrapperInitOptions tXmlWrapperInitOptions;
	
	// Initial XMLWrapper to get PrivacyMask bitmap path 
	TXmlWrapperTreeMap ptShardBufferMap[] = 
	{	
		{"root/venc_videoin/pmask/file_path", NULL, &VencVideoIn_SetBitmakPath, NULL},
		{NULL, NULL, NULL}
	};
	tXmlWrapperInitOptions.dwVersion = XMLWRAPPER_VERSION;
	if (XmlWrapper_Initial(&hXmlWrapperObj, &tXmlWrapperInitOptions) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Initial XMLWrapper object fail !! \n");
		return S_FAIL;
	}
	if (XmlWrapper_SetHandler(hXmlWrapperObj, ptShardBufferMap, NULL) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] Set XMLWrapper handler fail !! \n");
		return S_FAIL;
	}
	//printf("[venc_videoin_conf](%d)ptVideoInInfo->szConfigFile=%s\n", __LINE__, ptVideoInInfo->szConfigFile);
	if (XmlWrapper_ReadFile_UsrDefFunc(ptVideoInInfo->szConfigFile, hXmlWrapperObj, ptVideoInInfo) != S_OK) {
		fprintf(stderr, "[VENC_VIDEOIN] XmlWrapper_ReadFile_UsrDefFunc %s Fail! \n", ptVideoInInfo->szConfigFile);
		return S_FAIL;
	}
	if (XmlWrapper_Release(&hXmlWrapperObj) != S_OK) {
		printf("Release XmlWrapper Fail! \n");
		return S_FAIL;
	}
	return S_OK;
}

SCODE VencVideoIn_PMaskSetoptions(HANDLE hObject)
{			
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	SetBitmapPath(ptVideoInInfo);
	
	if (ptVideoInInfo->tPMaskCfg.bSwitchChange == TRUE) {
		//printf("[VENC_VIDEOIN_CONF](%d)Mask png file=[%s]\n", __LINE__, ptVideoInInfo->szBitmapPath);
		if (VencVideoIn_IBPEReadBitMask(&ptVideoInInfo->tIBPEPMask, ptVideoInInfo->szBitmapPath) == S_OK)
		{
			//printf("[venc_videoin_conf]Call VencVideoIn_IBPEPNG2BitMask() successfully!!(%d)\n", __LINE__); 
		}
		else
		{
			//printf("[venc_videoin_conf]Call VencVideoIn_IBPEPNG2BitMask() failed!!(%d)\n", __LINE__); 
		}
		ptVideoInInfo->tPMaskCfg.bSwitchChange = FALSE;
	}

	if (ptVideoInInfo->tPMaskCfg.bColorChange == TRUE) {
		//<!-- 0:black, 1:white, 2:Red, 3:Green, 4:Blue -->
		//About color of YUV, please refer to http://multimedia.cx/eggs/yuv-and-rgb/ .
		if (ptVideoInInfo->tPMaskCfg.dwColor == 0)	//black
		{
			ptVideoInInfo->tPMaskCfg.dwPMaskFillY = 0x0;
			ptVideoInInfo->tPMaskCfg.dwPMaskFillCb = 0x80;
			ptVideoInInfo->tPMaskCfg.dwPMaskFillCr = 0x80;
		}
		else if (ptVideoInInfo->tPMaskCfg.dwColor == 1)	//while
		{
			ptVideoInInfo->tPMaskCfg.dwPMaskFillY = 0xff;
			ptVideoInInfo->tPMaskCfg.dwPMaskFillCb = 0x80;
			ptVideoInInfo->tPMaskCfg.dwPMaskFillCr = 0x80;
		} 
		else if (ptVideoInInfo->tPMaskCfg.dwColor == 2)	//red
		{
			ptVideoInInfo->tPMaskCfg.dwPMaskFillY = 0x0;
			ptVideoInInfo->tPMaskCfg.dwPMaskFillCb = 0x0;
			ptVideoInInfo->tPMaskCfg.dwPMaskFillCr = 0xff;
		}
		else if (ptVideoInInfo->tPMaskCfg.dwColor == 3)	//green
		{
			ptVideoInInfo->tPMaskCfg.dwPMaskFillY = 0x0;
			ptVideoInInfo->tPMaskCfg.dwPMaskFillCb = 0x0;
			ptVideoInInfo->tPMaskCfg.dwPMaskFillCr = 0x0;
		}
		else if (ptVideoInInfo->tPMaskCfg.dwColor == 4)	//blue
		{
			ptVideoInInfo->tPMaskCfg.dwPMaskFillY = 0x0;
			ptVideoInInfo->tPMaskCfg.dwPMaskFillCb = 0xff;
			ptVideoInInfo->tPMaskCfg.dwPMaskFillCr = 0x0;
		}
		else
		{ 
			ptVideoInInfo->tPMaskCfg.dwPMaskFillY = 0x0;
			ptVideoInInfo->tPMaskCfg.dwPMaskFillCb = 0x80;
			ptVideoInInfo->tPMaskCfg.dwPMaskFillCr = 0x80;

		}
		ptVideoInInfo->tPMaskCfg.bColorChange = FALSE;
	}
	return S_OK;
}
#endif
#ifdef _VMA_IBPE
/* ============================================================================================= */
void VencVideoIn_SetEdgeEnhancementEnable(HANDLE hObject, const CHAR *szEEEnable, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	BOOL	bEnable = FALSE;

	snprintf(szTemp, len+1, "%s", szEEEnable);
	if (strcmp(szTemp, "on") == 0)
		bEnable = TRUE;

	if (ptIBPOpt->tEdgeEnhancementCfg.bEdgeEnhancementEnable != bEnable) {
		ptIBPOpt->tEdgeEnhancementCfg.bEdgeEnhancementEnable = bEnable;
#ifdef _PRE_NR_		
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif
	}
}

/* ============================================================================================= */
void VencVideoIn_SetEdgeEnhancementEdgeSensitivity(HANDLE hObject, const CHAR *szEESensitivity, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	DWORD dwEESensitivity = strtoul(szEESensitivity, NULL, 10);

	if (ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceEdgeSensitivity != dwEESensitivity) {
		ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceEdgeSensitivity = dwEESensitivity;
#ifdef _PRE_NR_		
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif		
		//printf("[venc_videoin_conf](%d)Edge sensitivity of EE is %d\n", __LINE__, ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceEdgeSensitivity);
	}
}

void VencVideoIn_SetEdgeEnhancementNoiseSensitivity(HANDLE hObject, const CHAR *szEESensitivity, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	DWORD dwEESensitivity = strtoul(szEESensitivity, NULL, 10);

	if (ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceNoiseSensitivity != dwEESensitivity) {
		ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceNoiseSensitivity = dwEESensitivity;
#ifdef _PRE_NR_		
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif		
		//printf("[venc_videoin_conf](%d)Noise sensitivity of EE is %d\n", __LINE__, ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceNoiseSensitivity);
	}
}

void VencVideoIn_SetEdgeEnhancementClip(HANDLE hObject, const CHAR *szEdgeEnhanceClip, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	DWORD dwEdgeEnhanceClip = strtoul(szEdgeEnhanceClip, NULL, 10);

	if (ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceClip != dwEdgeEnhanceClip) {
		ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceClip = dwEdgeEnhanceClip;
#ifdef _PRE_NR_		
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif		
		//printf("[venc_videoin_conf](%d)Clip of EE is %d\n", __LINE__, ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceClip);
	}
}

/* ============================================================================================= */
void VencVideoIn_SetEdgeEnhancementStrength(HANDLE hObject, const CHAR *szEEStrength, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	DWORD dwEEStrength = strtoul(szEEStrength, NULL, 10);

	if (ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceStrength != dwEEStrength) {
		ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceStrength = dwEEStrength;
#ifdef _PRE_NR_		
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif		
		//printf("[venc_videoin_conf](%d)Edge strengthof EE is %d\n", __LINE__, ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceStrength);
	}
}
/* ============================================================================================= */
void VencVideoIn_SetSecondPhaseEdgeEnhancementEnable(HANDLE hObject, const CHAR *szEEEnable, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	BOOL	bEnable = FALSE;

	snprintf(szTemp, len+1, "%s", szEEEnable);
	if (strcmp(szTemp, "on") == 0)
		bEnable = TRUE;

	if (ptIBPOpt->tEdgeEnhancementCfg.bSecondPhaseEdgeEnhancementEnable != bEnable) {
		ptIBPOpt->tEdgeEnhancementCfg.bSecondPhaseEdgeEnhancementEnable = bEnable;
#ifdef _PRE_NR_		
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif
	}
}

/* ============================================================================================= */
void VencVideoIn_SetSecondPhaseEdgeEnhancementEdgeSensitivity(HANDLE hObject, const CHAR *szEESensitivity, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	DWORD dwEESensitivity = strtoul(szEESensitivity, NULL, 10);

	if (ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceEdgeSensitivity != dwEESensitivity) {
		ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceEdgeSensitivity = dwEESensitivity;
#ifdef _PRE_NR_		
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif		
		//printf("[venc_videoin_conf](%d)Edge sensitivity of EE is %d\n", __LINE__, ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceEdgeSensitivity);
	}
}

void VencVideoIn_SetSecondPhaseEdgeEnhancementNoiseSensitivity(HANDLE hObject, const CHAR *szEESensitivity, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	DWORD dwEESensitivity = strtoul(szEESensitivity, NULL, 10);

	if (ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceNoiseSensitivity != dwEESensitivity) {
		ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceNoiseSensitivity = dwEESensitivity;
#ifdef _PRE_NR_		
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif		
		//printf("[venc_videoin_conf](%d)Noise sensitivity of EE is %d\n", __LINE__, ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceNoiseSensitivity);
	}
}

void VencVideoIn_SetSecondPhaseEdgeEnhancementClip(HANDLE hObject, const CHAR *szEdgeEnhanceClip, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	DWORD dwEdgeEnhanceClip = strtoul(szEdgeEnhanceClip, NULL, 10);

	if (ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceClip != dwEdgeEnhanceClip) {
		ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceClip = dwEdgeEnhanceClip;
#ifdef _PRE_NR_		
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif	
		//printf("[venc_videoin_conf](%d)Clip of EE is %d\n", __LINE__, ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceClip);
	}
}

/* ============================================================================================= */
void VencVideoIn_SetSecondPhaseEdgeEnhancementStrength(HANDLE hObject, const CHAR *szEEStrength, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	DWORD dwEEStrength = strtoul(szEEStrength, NULL, 10);

	if (ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceStrength != dwEEStrength) {
		ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceStrength = dwEEStrength;
#ifdef _PRE_NR_			
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif
		//printf("[venc_videoin_conf](%d)Edge strengthof EE is %d\n", __LINE__, ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceStrength);
	}
}
/* ============================================================================================= */
void VencVideoIn_SetDeNoiseEnable(HANDLE hObject, const CHAR *szDNEnable, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	BOOL	bEnable = FALSE;

	snprintf(szTemp, len+1, "%s", szDNEnable);
	if (strcmp(szTemp, "on") == 0)
		bEnable = TRUE;

	if (ptIBPOpt->tDeNoiseCfg.bDeNoiseEnable != bEnable) {
		ptIBPOpt->tDeNoiseCfg.bDeNoiseEnable = bEnable;
#ifdef _PRE_NR_		
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif
	}
}
/* =========================================================================================== */
void VencVideoIn_SetDeNoiseMode(HANDLE hObject, const CHAR* szElement, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;	
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	DWORD dwValue = 0;
	static DWORD  dwPreValue = -1 ;
	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szElement);
	dwValue = strtoul(szTemp, NULL, 10);
	if (dwPreValue != dwValue)
	{
		dwPreValue = dwValue;
#ifdef _PRE_NR_			
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif
	}
	switch(dwValue)
	{
		case 0:
			ptIBPOpt->tDeNoiseCfg.eNoiseReductionMode = inrmNormal2D;
			break;
		case 1:
			ptIBPOpt->tDeNoiseCfg.eNoiseReductionMode = inrmNormal3D1Ref;
			break;
		case 2:
			ptIBPOpt->tDeNoiseCfg.eNoiseReductionMode = inrmNormal3D2Ref;
			break;
		case 3:
			ptIBPOpt->tDeNoiseCfg.eNoiseReductionMode = inrmStrong;
			break;
		case 4:
			ptIBPOpt->tDeNoiseCfg.eNoiseReductionMode = inrmBlend;
			break;
		default:
			ptIBPOpt->tDeNoiseCfg.eNoiseReductionMode = inrmNormal3D2Ref;
	}
}
/* =========================================================================================== */
void VencVideoIn_SetDeNoiseNormalModeStrength(HANDLE hObject, const CHAR* szElement, SDWORD len)
{ 
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;	
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	DWORD dwValue = 0;
	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szElement);
	dwValue = strtoul(szTemp, NULL, 10);
	if (ptIBPOpt->tDeNoiseCfg.dwNoiseReductionNormalStrength != dwValue) {
		ptIBPOpt->tDeNoiseCfg.dwNoiseReductionNormalStrength = dwValue;
#ifdef _PRE_NR_		
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif
	}
}
/* =========================================================================================== */
void VencVideoIn_SetDeNoiseStrongRefStrength(HANDLE hObject, const CHAR* szElement, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;	
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	DWORD dwValue = 0;
	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szElement);
	dwValue = strtoul(szTemp, NULL, 10);
	if (ptIBPOpt->tDeNoiseCfg.dwNoiseReductionStrongRefStrength != dwValue) 
	{
		ptIBPOpt->tDeNoiseCfg.dwNoiseReductionStrongRefStrength = dwValue;
#ifdef _PRE_NR_		
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif	
	}
}
/* =========================================================================================== */
void VencVideoIn_SetDeNoiseStrongCurrWeight(HANDLE hObject, const CHAR* szElement, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;	
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	DWORD dwValue = 0;
	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szElement);
	dwValue = strtoul(szTemp, NULL, 10);
	if (ptIBPOpt->tDeNoiseCfg.dwNoiseReductionStrongCurrWeight != dwValue) 
	{
		ptIBPOpt->tDeNoiseCfg.dwNoiseReductionStrongCurrWeight = dwValue;
#ifdef _PRE_NR_		
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif	
	}
}
/* =========================================================================================== */
void VencVideoIn_SetDeNoiseStrongMotionAdaptiveEn(HANDLE hObject, const CHAR* szElement, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	BOOL	bEnable = FALSE;
	snprintf(szTemp, len+1, "%s", szElement);
	if (strcmp(szTemp, "true") == 0)
		bEnable = TRUE;

	if (ptIBPOpt->tDeNoiseCfg.bNoiseReductionStrongMotionAdaptiveEn != bEnable) {
		ptIBPOpt->tDeNoiseCfg.bNoiseReductionStrongMotionAdaptiveEn = bEnable;
#ifdef _PRE_NR_		
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif	
	}
}
/* =========================================================================================== */
void VencVideoIn_SetSecondPhaseDeNoiseEnable(HANDLE hObject, const CHAR *szDNEnable, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	BOOL	bEnable = FALSE;

	snprintf(szTemp, len+1, "%s", szDNEnable);
	if (strcmp(szTemp, "on") == 0)
		bEnable = TRUE;

	if (ptIBPOpt->tDeNoiseCfg.bSecondPhaseDeNoiseEnable != bEnable) {
#ifdef _PRE_NR_			
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif
		ptIBPOpt->tDeNoiseCfg.bSecondPhaseDeNoiseEnable = bEnable;
	}
}
/* =========================================================================================== */
void VencVideoIn_SetSecondPhaseDeNoiseMode(HANDLE hObject, const CHAR* szElement, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;	
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	DWORD dwValue = 0;
	static DWORD  dwPreValue = -1 ;
	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szElement);
	dwValue = strtoul(szTemp, NULL, 10);
	
	if (dwPreValue != dwValue)
	{
		dwPreValue = dwValue;
#ifdef _PRE_NR_			
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif
	}
	
	switch(dwValue)
	{
		case 0:
			ptIBPOpt->tDeNoiseCfg.eSecondPhaseNoiseReductionMode = inrmNormal2D;
			break;
		case 1:
			ptIBPOpt->tDeNoiseCfg.eSecondPhaseNoiseReductionMode = inrmNormal3D1Ref;
			break;
		case 2:
			ptIBPOpt->tDeNoiseCfg.eSecondPhaseNoiseReductionMode = inrmNormal3D2Ref;
			break;
		case 3:
			ptIBPOpt->tDeNoiseCfg.eSecondPhaseNoiseReductionMode = inrmStrong;
			break;
		case 4:
			ptIBPOpt->tDeNoiseCfg.eSecondPhaseNoiseReductionMode = inrmBlend;
			break;
		default:
			ptIBPOpt->tDeNoiseCfg.eSecondPhaseNoiseReductionMode = inrmNormal3D2Ref;
	}
}
/* =========================================================================================== */
void VencVideoIn_SetSecondPhaseDeNoiseNormalModeStrength(HANDLE hObject, const CHAR* szElement, SDWORD len)
{ 
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;	
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	DWORD dwValue = 0;
	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szElement);
	dwValue = strtoul(szTemp, NULL, 10);
	if (ptIBPOpt->tDeNoiseCfg.dwSecondPhaseNoiseReductionNormalStrength != dwValue) {
		ptIBPOpt->tDeNoiseCfg.dwSecondPhaseNoiseReductionNormalStrength = dwValue;
#ifdef _PRE_NR_	
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif
	}
}
/* =========================================================================================== */
void VencVideoIn_SetSecondPhaseDeNoiseStrongRefStrength(HANDLE hObject, const CHAR* szElement, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;	
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	DWORD dwValue = 0;
	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szElement);
	dwValue = strtoul(szTemp, NULL, 10);
	if (ptIBPOpt->tDeNoiseCfg.dwSecondPhaseNoiseReductionStrongRefStrength != dwValue) 
	{
#ifdef _PRE_NR_			
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif
		ptIBPOpt->tDeNoiseCfg.dwSecondPhaseNoiseReductionStrongRefStrength = dwValue;
	}
}
/* =========================================================================================== */
void VencVideoIn_SetSecondPhaseDeNoiseStrongCurrWeight(HANDLE hObject, const CHAR* szElement, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;	
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	DWORD dwValue = 0;
	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szElement);
	dwValue = strtoul(szTemp, NULL, 10);
	if (ptIBPOpt->tDeNoiseCfg.dwSecondPhaseNoiseReductionStrongCurrWeight != dwValue) 
	{
#ifdef _PRE_NR_			
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif
		ptIBPOpt->tDeNoiseCfg.dwSecondPhaseNoiseReductionStrongCurrWeight = dwValue;
	}
}
/* =========================================================================================== */
void VencVideoIn_SetSecondPhaseDeNoiseStrongMotionAdaptiveEn(HANDLE hObject, const CHAR* szElement, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	BOOL	bEnable = FALSE;
	snprintf(szTemp, len+1, "%s", szElement);
	if (strcmp(szTemp, "true") == 0)
		bEnable = TRUE;

	if (ptIBPOpt->tDeNoiseCfg.bSecondPhaseNoiseReductionStrongMotionAdaptiveEn != bEnable) {
#ifdef _PRE_NR_			
		ptVideoInInfo->bSecondPhaseIBPEParameterUpdated = TRUE;
#endif
		ptIBPOpt->tDeNoiseCfg.bSecondPhaseNoiseReductionStrongMotionAdaptiveEn = bEnable;
	}
}
/* ============================================================================================= */
void VencVideoIn_SetGeometricSelectMode(HANDLE hObject, const CHAR *szGeoMtcSelectMode, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	EGeoMtcMode	eGMSelectMode;
	BOOL	bEnable = TRUE;
	snprintf(szTemp, len+1, "%s", szGeoMtcSelectMode);
	if (strcasecmp(szTemp, "barrel1") == 0) {
		eGMSelectMode = geomtcmBarrel1;
	} else if (strcasecmp(szTemp, "barrel2") == 0) {
		eGMSelectMode = geomtcmBarrel2;
	} else if (strcasecmp(szTemp, "pincushion1") == 0) {
		eGMSelectMode = geomtcmPincushion1;
	} else if (strcasecmp(szTemp, "pincushion2") == 0) {
		eGMSelectMode = geomtcmPincushion2;
	} else if (strcasecmp(szTemp, "userdefine") == 0) {
		eGMSelectMode = geomtcmUserDefine;
	} else {	// disable
		eGMSelectMode = geomtcmNum;
		bEnable = FALSE;
	}

	ptIBPOpt->tGeoMtcCfg.bEnable = bEnable;
	ptIBPOpt->tGeoMtcCfg.eGMSelectMode = eGMSelectMode;	// if eGMselectMode = geomtcmNum, it means disable
}

/* ============================================================================================= */
SCODE VencVideoIn_SetGeometricModeTag(HANDLE hObject, const CHAR *szGeoMtcMode, const CHAR** atts)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	EGeoMtcMode	eGMCurModeId = geomtcmNum;
	EGeoMtcRes	eGMCurResId = geomtcresNum;

	if (atts) {
		if (strcmp(atts[0], "id") != 0) {
			DBPRINT0("[VENC_VIDEOIN] No id attribute \n");
			return IGNORE_CHILD_CONFIG;
		}
		if (strcasecmp(atts[1], "barrel1") == 0) {
			eGMCurModeId = geomtcmBarrel1;
		} else if (strcasecmp(atts[1], "barrel2") == 0) {
			eGMCurModeId = geomtcmBarrel2;
		} else if (strcasecmp(atts[1], "pincushion1") == 0) {
			eGMCurModeId = geomtcmPincushion1;
		} else if (strcasecmp(atts[1], "pincushion2") == 0) {
			eGMCurModeId = geomtcmPincushion2;
		} else if (strcasecmp(atts[1], "userdefine") == 0) {
			eGMCurModeId = geomtcmUserDefine;
		} else {
			printf("[VENC_VIDEOIN] Non support Geometric mode !!\n");
			return IGNORE_CHILD_CONFIG;
		}

		if (strcmp(atts[2], "resolution") != 0) {
			DBPRINT0("[VENC_VIDEOIN] No resolution attribute \n");
			return IGNORE_CHILD_CONFIG;
		}
		if (strcmp(atts[3], "640x480") == 0) {
			eGMCurResId = geomtcres640x480;
		} else if (strcmp(atts[3], "1280x720") == 0) {
			eGMCurResId = geomtcres1280x720;
		} else if (strcmp(atts[3], "1280x800") == 0) {
			eGMCurResId = geomtcres1280x800;
		} else if (strcmp(atts[3], "1280x1024") == 0) {
			eGMCurResId = geomtcres1280x1024;
		} else if (strcmp(atts[3], "1920x1080") == 0) {
			eGMCurResId = geomtcres1920x1080;
		} else if (strcmp(atts[3], "2048x1536") == 0) {
			eGMCurResId = geomtcres2048x1536;
		} else if (strcmp(atts[3], "2560x1920") == 0) {
			eGMCurResId = geomtcres2560x1920;
		} else {
			printf("[VENC_VIDEOIN] Non support Geometric resolution %s!!\n", atts[3]);
			return IGNORE_CHILD_CONFIG;
		}

		ptIBPOpt->tGeoMtcCfg.eGMCurModeId = eGMCurModeId;
		ptIBPOpt->tGeoMtcCfg.eGMCurResId = eGMCurResId;

		return S_OK;
	}

	DBPRINT0("[VENC_VIDEOIN] No attribute \n");
	return IGNORE_CHILD_CONFIG;
}

/* ============================================================================================= */
void VencVideoIn_SetGeometricFwdMpgParam1Y(HANDLE hObject, const CHAR *szFwdMpgParam1Y, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	EGeoMtcMode	eGMCurModeId = ptIBPOpt->tGeoMtcCfg.eGMCurModeId;
	EGeoMtcRes	eGMCurResId = ptIBPOpt->tGeoMtcCfg.eGMCurResId;

	DWORD dwFwdMpgParam1Y = strtoul(szFwdMpgParam1Y, NULL, 10);

	if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].dwFwdMpgParam1Y != dwFwdMpgParam1Y) {
		ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].dwFwdMpgParam1Y = dwFwdMpgParam1Y;
		//DBPRINT3("[VENC_VIDEOIN] Geometric (%d,%d) dwFwdMpgParam1Y Change to %u \n", eGMCurModeId, eGMCurResId, dwFwdMpgParam1Y);
	}
}

/* ============================================================================================= */
void VencVideoIn_SetGeometricFwdMpgParam2Y(HANDLE hObject, const CHAR *szFwdMpgParam2Y, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	EGeoMtcMode	eGMCurModeId = ptIBPOpt->tGeoMtcCfg.eGMCurModeId;
	EGeoMtcRes	eGMCurResId = ptIBPOpt->tGeoMtcCfg.eGMCurResId;

	DWORD dwFwdMpgParam2Y = strtoul(szFwdMpgParam2Y, NULL, 10);

	if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].dwFwdMpgParam2Y != dwFwdMpgParam2Y) {
		ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].dwFwdMpgParam2Y = dwFwdMpgParam2Y;
		//DBPRINT3("[VENC_VIDEOIN] Geometric (%d,%d) dwFwdMpgParam2Y Change to %u \n", eGMCurModeId, eGMCurResId, dwFwdMpgParam2Y);
	}
}

/* ============================================================================================= */
void VencVideoIn_SetGeometricFwdMpgParam3Y(HANDLE hObject, const CHAR *szFwdMpgParam3Y, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	EGeoMtcMode	eGMCurModeId = ptIBPOpt->tGeoMtcCfg.eGMCurModeId;
	EGeoMtcRes	eGMCurResId = ptIBPOpt->tGeoMtcCfg.eGMCurResId;

	DWORD dwFwdMpgParam3Y = strtoul(szFwdMpgParam3Y, NULL, 10);

	if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].dwFwdMpgParam3Y != dwFwdMpgParam3Y) {
		ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].dwFwdMpgParam3Y = dwFwdMpgParam3Y;
		//DBPRINT3("[VENC_VIDEOIN] Geometric (%d,%d) dwFwdMpgParam3Y Change to %u \n", eGMCurModeId, eGMCurResId, dwFwdMpgParam3Y);
	}
}

/* ============================================================================================= */
void VencVideoIn_SetGeometricPosFwdMpgParam1Y(HANDLE hObject, const CHAR *szPosFwdMpgParam1Y, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	EGeoMtcMode	eGMCurModeId = ptIBPOpt->tGeoMtcCfg.eGMCurModeId;
	EGeoMtcRes	eGMCurResId = ptIBPOpt->tGeoMtcCfg.eGMCurResId;

	BOOL bPosFwdMpgParam1Y = strtoul(szPosFwdMpgParam1Y, NULL, 10);

	if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].bPosFwdMpgParam1Y != bPosFwdMpgParam1Y) {
		ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].bPosFwdMpgParam1Y = bPosFwdMpgParam1Y;
		//DBPRINT3("[VENC_VIDEOIN] Geometric (%d,%d) bPosFwdMpgParam1Y Change to %d \n", eGMCurModeId, eGMCurResId, bPosFwdMpgParam1Y);
	}
}

/* ============================================================================================= */
void VencVideoIn_SetGeometricPosFwdMpgParam2Y(HANDLE hObject, const CHAR *szPosFwdMpgParam2Y, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	EGeoMtcMode	eGMCurModeId = ptIBPOpt->tGeoMtcCfg.eGMCurModeId;
	EGeoMtcRes	eGMCurResId = ptIBPOpt->tGeoMtcCfg.eGMCurResId;

	BOOL bPosFwdMpgParam2Y = strtoul(szPosFwdMpgParam2Y, NULL, 10);

	if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].bPosFwdMpgParam2Y != bPosFwdMpgParam2Y) {
		ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].bPosFwdMpgParam2Y = bPosFwdMpgParam2Y;
		//DBPRINT3("[VENC_VIDEOIN] Geometric (%d,%d) bPosFwdMpgParam2Y Change to %d \n", eGMCurModeId, eGMCurResId, bPosFwdMpgParam2Y);
	}
}

/* ============================================================================================= */
void VencVideoIn_SetGeometricPosFwdMpgParam3Y(HANDLE hObject, const CHAR *szPosFwdMpgParam3Y, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	EGeoMtcMode	eGMCurModeId = ptIBPOpt->tGeoMtcCfg.eGMCurModeId;
	EGeoMtcRes	eGMCurResId = ptIBPOpt->tGeoMtcCfg.eGMCurResId;

	BOOL bPosFwdMpgParam3Y = strtoul(szPosFwdMpgParam3Y, NULL, 10);

	if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].bPosFwdMpgParam3Y != bPosFwdMpgParam3Y) {
		ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].bPosFwdMpgParam3Y = bPosFwdMpgParam3Y;
		//DBPRINT3("[VENC_VIDEOIN] Geometric (%d,%d) bPosFwdMpgParam3Y Change to %d \n", eGMCurModeId, eGMCurResId, bPosFwdMpgParam3Y);
	}
}

/* ============================================================================================= */
void VencVideoIn_SetGeometricFwdMpgParam1CbCr(HANDLE hObject, const CHAR *szFwdMpgParam1CbCr, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	EGeoMtcMode	eGMCurModeId = ptIBPOpt->tGeoMtcCfg.eGMCurModeId;
	EGeoMtcRes	eGMCurResId = ptIBPOpt->tGeoMtcCfg.eGMCurResId;

	DWORD dwFwdMpgParam1CbCr = strtoul(szFwdMpgParam1CbCr, NULL, 10);

	if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].dwFwdMpgParam1CbCr != dwFwdMpgParam1CbCr) {
		ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].dwFwdMpgParam1CbCr = dwFwdMpgParam1CbCr;
		//DBPRINT3("[VENC_VIDEOIN] Geometric (%d,%d) dwFwdMpgParam1CbCr Change to %u \n", eGMCurModeId, eGMCurResId, dwFwdMpgParam1CbCr);
	}
}

/* ============================================================================================= */
void VencVideoIn_SetGeometricFwdMpgParam2CbCr(HANDLE hObject, const CHAR *szFwdMpgParam2CbCr, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	EGeoMtcMode	eGMCurModeId = ptIBPOpt->tGeoMtcCfg.eGMCurModeId;
	EGeoMtcRes	eGMCurResId = ptIBPOpt->tGeoMtcCfg.eGMCurResId;

	DWORD dwFwdMpgParam2CbCr = strtoul(szFwdMpgParam2CbCr, NULL, 10);

	if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].dwFwdMpgParam2CbCr != dwFwdMpgParam2CbCr) {
		ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].dwFwdMpgParam2CbCr = dwFwdMpgParam2CbCr;
		//DBPRINT3("[VENC_VIDEOIN] Geometric (%d,%d) dwFwdMpgParam2CbCr Change to %u \n", eGMCurModeId, eGMCurResId, dwFwdMpgParam2CbCr);
	}
}

/* ============================================================================================= */
void VencVideoIn_SetGeometricFwdMpgParam3CbCr(HANDLE hObject, const CHAR *szFwdMpgParam3CbCr, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	EGeoMtcMode	eGMCurModeId = ptIBPOpt->tGeoMtcCfg.eGMCurModeId;
	EGeoMtcRes	eGMCurResId = ptIBPOpt->tGeoMtcCfg.eGMCurResId;

	DWORD dwFwdMpgParam3CbCr = strtoul(szFwdMpgParam3CbCr, NULL, 10);

	if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].dwFwdMpgParam3CbCr != dwFwdMpgParam3CbCr) {
		ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].dwFwdMpgParam3CbCr = dwFwdMpgParam3CbCr;
		//DBPRINT3("[VENC_VIDEOIN] Geometric (%d,%d) dwFwdMpgParam3CbCr Change to %u \n", eGMCurModeId, eGMCurResId, dwFwdMpgParam3CbCr);
	}
}

/* ============================================================================================= */
void VencVideoIn_SetGeometricPosFwdMpgParam1CbCr(HANDLE hObject, const CHAR *szPosFwdMpgParam1CbCr, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	EGeoMtcMode	eGMCurModeId = ptIBPOpt->tGeoMtcCfg.eGMCurModeId;
	EGeoMtcRes	eGMCurResId = ptIBPOpt->tGeoMtcCfg.eGMCurResId;

	BOOL bPosFwdMpgParam1CbCr = strtoul(szPosFwdMpgParam1CbCr, NULL, 10);

	if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].bPosFwdMpgParam1CbCr != bPosFwdMpgParam1CbCr) {
		ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].bPosFwdMpgParam1CbCr = bPosFwdMpgParam1CbCr;
		//DBPRINT3("[VENC_VIDEOIN] Geometric (%d,%d) bPosFwdMpgParam1CbCr Change to %d \n", eGMCurModeId, eGMCurResId, bPosFwdMpgParam1CbCr);
	}
}

/* ============================================================================================= */
void VencVideoIn_SetGeometricPosFwdMpgParam2CbCr(HANDLE hObject, const CHAR *szPosFwdMpgParam2CbCr, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	EGeoMtcMode	eGMCurModeId = ptIBPOpt->tGeoMtcCfg.eGMCurModeId;
	EGeoMtcRes	eGMCurResId = ptIBPOpt->tGeoMtcCfg.eGMCurResId;

	BOOL bPosFwdMpgParam2CbCr = strtoul(szPosFwdMpgParam2CbCr, NULL, 10);

	if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].bPosFwdMpgParam2CbCr != bPosFwdMpgParam2CbCr) {
		ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].bPosFwdMpgParam2CbCr = bPosFwdMpgParam2CbCr;
		//DBPRINT3("[VENC_VIDEOIN] Geometric (%d,%d) bPosFwdMpgParam2CbCr Change to %d \n", eGMCurModeId, eGMCurResId, bPosFwdMpgParam2CbCr);
	}
}

/* ============================================================================================= */
void VencVideoIn_SetGeometricPosFwdMpgParam3CbCr(HANDLE hObject, const CHAR *szPosFwdMpgParam3CbCr, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	EGeoMtcMode	eGMCurModeId = ptIBPOpt->tGeoMtcCfg.eGMCurModeId;
	EGeoMtcRes	eGMCurResId = ptIBPOpt->tGeoMtcCfg.eGMCurResId;

	BOOL bPosFwdMpgParam3CbCr = strtoul(szPosFwdMpgParam3CbCr, NULL, 10);

	if (ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].bPosFwdMpgParam3CbCr != bPosFwdMpgParam3CbCr) {
		ptIBPOpt->tGeoMtcCfg.aaGMTypeValue[eGMCurModeId][eGMCurResId].bPosFwdMpgParam3CbCr = bPosFwdMpgParam3CbCr;
		//DBPRINT3("[VENC_VIDEOIN] Geometric (%d,%d) bPosFwdMpgParam3CbCr Change to %d \n", eGMCurModeId, eGMCurResId, bPosFwdMpgParam3CbCr);
	}
}
/* =========================================================================================== */
SCODE VencVideoIn_ImgBackEndProcessSetoptions(HANDLE hObject)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	DWORD	dwWidth = 0, dwHeight = 0;

	EGeoMtcMode	eGMSelectMode;
	EGeoMtcRes	eGMCurResId;

	dwWidth = ptVideoInInfo->dwCurtCapWidth;
	dwHeight = ptVideoInInfo->dwCurtCapHeight;
	//printf("ptVideoInInfo->dwCurtCapWidth=%d,ptVideoInInfo->dwCurtCapHeight=%d\n",ptVideoInInfo->dwCurtCapWidth,ptVideoInInfo->dwCurtCapHeight);
	ptVideoInInfo->tIBPState.dwMapBusNum = 1;
	ptVideoInInfo->tIBPState.dwRefBusNum = 1;
	ptVideoInInfo->tIBPState.bUpdateEn = TRUE;
	ptVideoInInfo->tIBPState.dwValidRefBuffNum = (BUFF_SIZE - 1);
	//ptVideoInInfo->tIBPState.ePixelFormat = ptIBPOpt->tIBPECtrl.ePixelFormat;
	
	ptVideoInInfo->tIBPState.bYComponentEn = ptIBPOpt->tIBPECtrl.bYCmpntEn;
	ptVideoInInfo->tIBPState.bCComponentEn = ptIBPOpt->tIBPECtrl.bCCmpntEn;
	ptVideoInInfo->tIBPState.bYOutEn = ptIBPOpt->tIBPECtrl.bYOutEn;
	ptVideoInInfo->tIBPState.bCOutEn = ptIBPOpt->tIBPECtrl.bCOutEn;
	ptVideoInInfo->tIBPState.bMotionDetectEn = ptVideoInInfo->tMotionCfg.bMotionEnabled;
	ptVideoInInfo->tIBPState.bMotionDetectPrivacyMaskEn = ptVideoInInfo->tMotionCfg.bMDPMaskEn;
	
	if (ptVideoInInfo->tIBPOpt.bTWICEIBPEEnable == FALSE)
	{
		ptVideoInInfo->tIBPState.bEdgeEnhanceEn = ptIBPOpt->tEdgeEnhancementCfg.bSecondPhaseEdgeEnhancementEnable;
		ptVideoInInfo->tIBPState.dwEdgeEnhanceStrength = ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceStrength;
		ptVideoInInfo->tIBPState.dwEdgeEnhanceClip = ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceClip;
		ptVideoInInfo->tIBPState.dwEdgeEnhanceEdgeSensitivity = ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceEdgeSensitivity;
		ptVideoInInfo->tIBPState.dwEdgeEnhanceNoiseSensitivity = ptIBPOpt->tEdgeEnhancementCfg.dwSecondPhaseEdgeEnhanceNoiseSensitivity;
		
	}
	else
	{
		ptVideoInInfo->tIBPState.bEdgeEnhanceEn = ptIBPOpt->tEdgeEnhancementCfg.bEdgeEnhancementEnable;
		ptVideoInInfo->tIBPState.dwEdgeEnhanceStrength = ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceStrength;
		ptVideoInInfo->tIBPState.dwEdgeEnhanceClip = ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceClip;
		ptVideoInInfo->tIBPState.dwEdgeEnhanceEdgeSensitivity = ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceEdgeSensitivity;
		ptVideoInInfo->tIBPState.dwEdgeEnhanceNoiseSensitivity = ptIBPOpt->tEdgeEnhancementCfg.dwEdgeEnhanceNoiseSensitivity;
	}
	
	
	if (ptVideoInInfo->tIBPOpt.bTWICEIBPEEnable == FALSE)
	{	
		ptVideoInInfo->tIBPState.bNoiseReductionEn = ptIBPOpt->tDeNoiseCfg.bSecondPhaseDeNoiseEnable;
		ptVideoInInfo->tIBPState.eNoiseReductionMode = ptIBPOpt->tDeNoiseCfg.eSecondPhaseNoiseReductionMode;
		ptVideoInInfo->tIBPState.dwNoiseReductionNormalStrength = ptIBPOpt->tDeNoiseCfg.dwSecondPhaseNoiseReductionNormalStrength;			//0~200;  default:15
		ptVideoInInfo->tIBPState.dwNoiseReductionStrongRefStrength = ptIBPOpt->tDeNoiseCfg.dwSecondPhaseNoiseReductionStrongRefStrength;
		ptVideoInInfo->tIBPState.dwNoiseReductionStrongCurrWeight = ptIBPOpt->tDeNoiseCfg.dwSecondPhaseNoiseReductionStrongCurrWeight;
		ptVideoInInfo->tIBPState.bNoiseReductionStrongMotionAdaptiveEn = ptIBPOpt->tDeNoiseCfg.bSecondPhaseNoiseReductionStrongMotionAdaptiveEn;
	}
	else
	{
		ptVideoInInfo->tIBPState.bNoiseReductionEn = ptIBPOpt->tDeNoiseCfg.bDeNoiseEnable;
		ptVideoInInfo->tIBPState.eNoiseReductionMode = ptIBPOpt->tDeNoiseCfg.eNoiseReductionMode;
		ptVideoInInfo->tIBPState.dwNoiseReductionNormalStrength = ptIBPOpt->tDeNoiseCfg.dwNoiseReductionNormalStrength;			//0~200;  default:15
		ptVideoInInfo->tIBPState.dwNoiseReductionStrongRefStrength = ptIBPOpt->tDeNoiseCfg.dwNoiseReductionStrongRefStrength;
		ptVideoInInfo->tIBPState.dwNoiseReductionStrongCurrWeight = ptIBPOpt->tDeNoiseCfg.dwNoiseReductionStrongCurrWeight;
		ptVideoInInfo->tIBPState.bNoiseReductionStrongMotionAdaptiveEn = ptIBPOpt->tDeNoiseCfg.bNoiseReductionStrongMotionAdaptiveEn;
	}
#ifdef _VMA_DIE
	ptVideoInInfo->tIBPState.bDeInterlaceEn = ptIBPOpt->tDeiCfg.bDeInterlacerEnable;
#else
	ptVideoInInfo->tIBPState.bDeInterlaceEn = FALSE;
#endif
	ptVideoInInfo->tIBPState.eDeInterlaceMode = ptIBPOpt->tDeiCfg.eDeInterlaceMode;
	ptVideoInInfo->tIBPState.eDeInterlacePicFormat = ptIBPOpt->tDeiCfg.eDeInterlacePicFormat;
	ptVideoInInfo->tIBPState.bOutFramePrivacyMaskEn = ptVideoInInfo->tPMaskCfg.bOutPMaskEn;
	ptVideoInInfo->tIBPState.dwPrivacyMaskYConstant = ptVideoInInfo->tPMaskCfg.dwPMaskFillY;
	ptVideoInInfo->tIBPState.dwPrivacyMaskCbConstant = ptVideoInInfo->tPMaskCfg.dwPMaskFillCb;
	ptVideoInInfo->tIBPState.dwPrivacyMaskCrConstant = ptVideoInInfo->tPMaskCfg.dwPMaskFillCr;
	ptVideoInInfo->tIBPState.bGeometricLensDistortionCorrectionEn = ptIBPOpt->tGeoMtcCfg.bEnable;
	
	// when first load, not get buffer yet, so dwCurtCapWidth/dwCurtCapHeight will be 0, so it will set to geomtcres640x480
	// but when get buffer, call VencVideoIn_ResolutionChange function, it should reset again.
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

	if (ptIBPOpt->tGeoMtcCfg.bEnable == TRUE) {	// if disable, no array entry can retrieve.
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
#if 0
	printf("=======================================================\n");
	printf("[VENC_VIDEOIN]IBPE Ctrl:\n");
	printf("[VENC_VIDEOIN]ePixelFormat=%d\n",ptVideoInInfo->tIBPState.ePixelFormat);
	printf("[VENC_VIDEOIN]bYCmpntEn=%d\n",ptVideoInInfo->tIBPState.bYComponentEn);
	printf("[VENC_VIDEOIN]bCCmpntEn=%d\n",ptVideoInInfo->tIBPState.bCComponentEn);
	printf("[VENC_VIDEOIN]bYOutEn=%d\n",ptVideoInInfo->tIBPState.bYOutEn);
	printf("[VENC_VIDEOIN]bCOutEn=%d\n",ptVideoInInfo->tIBPState.bCOutEn);
	printf("[VENC_VIDEOIN]bUpdateEn=%d\n",ptVideoInInfo->tIBPState.bUpdateEn);
	printf("[VENC_VIDEOIN]bTwiceIBPE=%d\n",ptVideoInInfo->tIBPOpt.bTWICEIBPEEnable);
	printf("=======================================================\n");
	printf("[VENC_VIDEOIN]Motion Detection:\n");
	printf("[VENC_VIDEOIN]bMotionDtctEn=%d\n",ptVideoInInfo->tIBPState.bMotionDetectEn);
	printf("[VENC_VIDEOIN]bMDPMaskEn=%d\n",ptVideoInInfo->tIBPState.bMotionDetectPrivacyMaskEn);
	printf("=======================================================\n");
	printf("[VENC_VIDEOIN]Noise Reduction:\n");
	printf("[VENC_VIDEOIN]bDenoiseEnable=%d\n",ptVideoInInfo->tIBPState.bNoiseReductionEn);
	printf("[VENC_VIDEOIN]eNoiseReductionMode=%d\n",ptVideoInInfo->tIBPState.eNoiseReductionMode);
	printf("[VENC_VIDEOIN]dwNoiseReductionNormalStrength=%d\n",ptVideoInInfo->tIBPState.dwNoiseReductionNormalStrength);
	printf("[VENC_VIDEOIN]dwNoiseReductionStrongRefStrength=%d\n",ptVideoInInfo->tIBPState.dwNoiseReductionStrongRefStrength);
	printf("[VENC_VIDEOIN]dwNoiseReductionStrongCurrWeight=%d\n",ptVideoInInfo->tIBPState.dwNoiseReductionStrongCurrWeight);
	printf("[VENC_VIDEOIN]bNoiseReductionStrongMotionAdaptiveEn=%d\n",ptVideoInInfo->tIBPState.bNoiseReductionStrongMotionAdaptiveEn);
	printf("=======================================================\n");
	printf("[VENC_VIDEOIN]Edge Enhancement:\n");
	printf("[VENC_VIDEOIN]bEdgeEnhanceEn=%d\n",ptVideoInInfo->tIBPState.bEdgeEnhanceEn);
	printf("[VENC_VIDEOIN]dwEdgeEnhanceClip=%d\n",ptVideoInInfo->tIBPState.dwEdgeEnhanceClip);
	printf("[VENC_VIDEOIN]dwEdgeEnhanceEdgeSensitivity=%d\n",ptVideoInInfo->tIBPState.dwEdgeEnhanceEdgeSensitivity);
	printf("[VENC_VIDEOIN]dwEdgeEnhanceNoiseSensitivity=%d\n",ptVideoInInfo->tIBPState.dwEdgeEnhanceNoiseSensitivity);
	printf("[VENC_VIDEOIN]dwEdgeEnhanceStrength=%d\n",ptVideoInInfo->tIBPState.dwEdgeEnhanceStrength);
	printf("=======================================================\n");
	printf("[VENC_VIDEOIN]De-Interlacing:\n");
	printf("[VENC_VIDEOIN]bDeInterlacerEn=%d\n",ptVideoInInfo->tIBPState.bDeInterlaceEn);
	printf("[VENC_VIDEOIN]eDeInterlacerMode=%d\n",ptVideoInInfo->tIBPState.eDeInterlaceMode);
	printf("[VENC_VIDEOIN]eDeInterlacePicFormat=%d\n",ptVideoInInfo->tIBPState.eDeInterlacePicFormat);
	printf("=======================================================\n");
	printf("[VENC_VIDEOIN]Privacy Mask:\n");
	printf("[VENC_VIDEOIN]bOutPMaskEn=%d\n",ptVideoInInfo->tIBPState.bOutFramePrivacyMaskEn);
	printf("[VENC_VIDEOIN]dwPMaskFillY = 0x%04x\n",ptVideoInInfo->tIBPState.dwPrivacyMaskYConstant);
	printf("[VENC_VIDEOIN]dwPMaskFillCb = 0x%04x\n",ptVideoInInfo->tIBPState.dwPrivacyMaskCbConstant);
	printf("[VENC_VIDEOIN]dwPMaskFillCr = 0x%04x\n",ptVideoInInfo->tIBPState.dwPrivacyMaskCrConstant);
	printf("=======================================================\n");
	printf("[VENC_VIDEOIN]Giometric Lens Distortion Correction:\n");
	printf("[VENC_VIDEOIN]bGeoLDCEn=%d\n",ptVideoInInfo->tIBPState.bGeometricLensDistortionCorrectionEn);
	printf("[VENC_VIDEOIN]sdwGeometricLensDistortionCorrectionYParam1 = %d\n", ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionYParam1);
	printf("[VENC_VIDEOIN]sdwGeometricLensDistortionCorrectionYParam2 = %d\n", ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionYParam2);
	printf("[VENC_VIDEOIN]sdwGeometricLensDistortionCorrectionYParam3 = %d\n", ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionYParam3);
	printf("[VENC_VIDEOIN]sdwGeometricLensDistortionCorrectionCParam1 = %d\n", ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionCParam1);
	printf("[VENC_VIDEOIN]sdwGeometricLensDistortionCorrectionCParam2 = %d\n", ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionCParam2);
	printf("[VENC_VIDEOIN]sdwGeometricLensDistortionCorrectionCParam3 = %d\n", ptVideoInInfo->tIBPState.sdwGeometricLensDistortionCorrectionCParam3);
	printf("=======================================================\n");
#endif
	return S_OK;	
}

/* ========================================================================= */
void VencVideoIn_SetDEIEnable(HANDLE hObject, const CHAR* szElement, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
#ifdef _VMA_DIE	
	CHAR szTemp[len+1];
	BOOL	bEnable = FALSE;
	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szElement);
	if (strcmp(szTemp, "on") == 0)
		bEnable = TRUE;

	if (ptIBPOpt->tDeiCfg.bDeInterlacerEnable != bEnable) {
		ptIBPOpt->tDeiCfg.bDeInterlacerEnable = bEnable;
	}
#else
	ptIBPOpt->tDeiCfg.bDeInterlacerEnable  = FALSE;
#endif
}

void VencVideoIn_SetDEIPicFormat(HANDLE hObject, const CHAR* szElement, SDWORD len)
{
	//TImgBackEndProcessState *ptIBPState = (TImgBackEndProcessState *)hObject;
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	DWORD dwValue = 0;
	CHAR szTemp[len+1];

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szElement);
	dwValue = strtoul(szTemp, NULL, 10);

	switch(dwValue)
	{
		case 0:
			ptIBPOpt->tDeiCfg.eDeInterlacePicFormat = idePicINTERLEAVE;
			break;
		case 1:
			ptIBPOpt->tDeiCfg.eDeInterlacePicFormat = idePicPLANAR;
			break;
		default:
			ptIBPOpt->tDeiCfg.eDeInterlacePicFormat = idePicINTERLEAVE;
	}
}

void VencVideoIn_SetDEIMode(HANDLE hObject, const CHAR* szElement, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;	
	TVideoInIBPOptions *ptIBPOpt = &(ptVideoInInfo->tIBPOpt);
	CHAR szTemp[len+1];
	DWORD dwValue = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szElement);
	dwValue = strtoul(szTemp, NULL, 10);

	switch(dwValue)
	{
		case 0:
			ptIBPOpt->tDeiCfg.eDeInterlaceMode = idemWeave;
			break;
		case 1:
			ptIBPOpt->tDeiCfg.eDeInterlaceMode = idemBlend;
			break;
		case 2:
			ptIBPOpt->tDeiCfg.eDeInterlaceMode = idemMotionAdaptiveBasic;
			break;
		case 3:
			ptIBPOpt->tDeiCfg.eDeInterlaceMode = idemMotionAdaptiveAdvanced;
			break;
		default:
			ptIBPOpt->tDeiCfg.eDeInterlaceMode = idemMotionAdaptiveBasic;
	}
}

/* ========================================================================= */
void VencVideoIn_SetMotion(HANDLE hObject, const CHAR *szMotion, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	CHAR szTemp[len+1];
	BOOL bEnabled = FALSE;
	DWORD dwIndex;
	snprintf(szTemp, len+1, "%s", szMotion);

	// reset bAnyFieldChange, or it will reserve the previous value, but it shouldn't
	ptVideoInInfo->tMotionCfg.bMotionEnabledChange = FALSE;
	for (dwIndex = 0; dwIndex < MAX_MOTION_WIN_NUM; dwIndex++) {
		ptVideoInInfo->tMotionCfg.atmwCfg[dwIndex].bEnabledChange = FALSE;
		ptVideoInInfo->tMotionCfg.atmwCfg[dwIndex].bPositionChange = FALSE;
		ptVideoInInfo->tMotionCfg.atmwCfg[dwIndex].bWindowSizeChange = FALSE;
		ptVideoInInfo->tMotionCfg.atmwCfg[dwIndex].bDetectThresholdChange = FALSE;
		ptVideoInInfo->tMotionCfg.atmwCfg[dwIndex].bAnyFieldChange = FALSE;
	}

	if (strcmp(szTemp, "on") == 0)
		bEnabled = TRUE;

	if (bEnabled != ptVideoInInfo->tMotionCfg.bMotionEnabled) {
		ptVideoInInfo->tMotionCfg.bMotionEnabledChange = TRUE;
		ptVideoInInfo->tMotionCfg.bMotionEnabled = bEnabled;

		ptVideoInInfo->bStatusChange = TRUE;
		DBPRINT1("[VENC_MOTION] motion enable change to %s \n", (ptVideoInInfo->tMotionCfg.bMotionEnabled)?"enable":"disable");
	}
}

void VencVideoIn_SetMotionMDPMaskEn(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	CHAR szTemp[len+1];
	BOOL bEnabled = FALSE;

	snprintf(szTemp, len+1, "%s", szElement);

	if (strcmp(szTemp, "true") == 0)
		bEnabled = TRUE;

	if (bEnabled == TRUE)
	{
		ptVideoInInfo->tMotionCfg.bMDPMaskEn = TRUE;
	}
	else
	{
		ptVideoInInfo->tMotionCfg.bMDPMaskEn = FALSE;
	}
}

/* ============================================================================================= */
void VencVideoIn_SetMotionXTranslate(HANDLE hObject, const CHAR *szXTranslate, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	CHAR szTemp[len+1];
	snprintf(szTemp, len+1, "%s", szXTranslate);
	ptVideoInInfo->tMotionCfg.fXTranslate = atof(szTemp);
	DBPRINT1("[VENC_MOTION] XTranslate %.8f \n", ptVideoInInfo->tMotionCfg.fXTranslate);	
}

/* ============================================================================================= */
void VencVideoIn_SetMotionYTranslate(HANDLE hObject, const CHAR *szYTranslate, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	CHAR szTemp[len+1];
	snprintf(szTemp, len+1, "%s", szYTranslate);
	ptVideoInInfo->tMotionCfg.fYTranslate = atof(szTemp);
	DBPRINT1("[VENC_MOTION] YTranslate %.8f \n", ptVideoInInfo->tMotionCfg.fYTranslate);	
}

/* ============================================================================================= */
void VencVideoIn_SetMotionXScale(HANDLE hObject, const CHAR *szXScale, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	CHAR szTemp[len+1];
	snprintf(szTemp, len+1, "%s", szXScale);
	ptVideoInInfo->tMotionCfg.fXScale = atof(szTemp);
	DBPRINT1("[VENC_MOTION] XScale %.8f \n", ptVideoInInfo->tMotionCfg.fXScale);	
}

/* ============================================================================================= */
void VencVideoIn_SetMotionYScale(HANDLE hObject, const CHAR *szYScale, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	CHAR szTemp[len+1];
	snprintf(szTemp, len+1, "%s", szYScale);
	ptVideoInInfo->tMotionCfg.fYScale = atof(szTemp);
	DBPRINT1("[VENC_MOTION] YScale %.8f \n", ptVideoInInfo->tMotionCfg.fYScale);
}

/* ========================================================================= */
SCODE VencVideoIn_SetMotionWinTag(HANDLE hObject, const CHAR *szWinTag, const CHAR** atts)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	DWORD  dwCurWinId;

	if (atts) {
		if (strcmp(atts[0], "id") != 0) {
			DBPRINT0("[VENC_MOTION] No id attribute \n");
			return IGNORE_CHILD_CONFIG;            
		}
		dwCurWinId = strtoul(atts[1], (char**)NULL, 10);
		if (dwCurWinId < 0 || dwCurWinId > MAX_MOTION_WIN_NUM)
			return IGNORE_CHILD_CONFIG;
		//printf("[venc_videoin_conf](%d)dwCurWinId=%d\n",__LINE__,dwCurWinId);
		ptVideoInInfo->tMotionCfg.dwCurWinId = dwCurWinId;
		return S_OK;
	}
	DBPRINT0("[VENC_MOTION] No attribute \n");
	return IGNORE_CHILD_CONFIG;
}

/* ========================================================================= */
void VencVideoIn_SetMotionWinEnable(HANDLE hObject, const CHAR *szEnable, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	CHAR szTemp[len+1];
	DWORD dwCurWinId;
	BOOL bEnabled = FALSE;

	snprintf(szTemp, len+1, "%s", szEnable);
	dwCurWinId = ptVideoInInfo->tMotionCfg.dwCurWinId;

	if (strcmp(szTemp, "on") == 0)
		bEnabled = TRUE;

	if (bEnabled != ptVideoInInfo->tMotionCfg.atmwCfg[dwCurWinId].bEnabled) {
		ptVideoInInfo->tMotionCfg.atmwCfg[dwCurWinId].bEnabledChange = TRUE;
		ptVideoInInfo->tMotionCfg.atmwCfg[dwCurWinId].bEnabled = bEnabled;
		ptVideoInInfo->tMotionCfg.atmwCfg[dwCurWinId].bAnyFieldChange = TRUE;
		printf("[VENC_MOTION] widows %u Enabled Change to %s \n", dwCurWinId, (ptVideoInInfo->tMotionCfg.atmwCfg[dwCurWinId].bEnabled)?"Enable":"Disable");
	}
}

/* ========================================================================= */
void VencVideoIn_SetMotionWinName(HANDLE hObject, const CHAR *szName, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TMotionWinInfo *ptMotionWinInfo;

	ptMotionWinInfo = &(ptVideoInInfo->tMotionCfg.atmwCfg[ptVideoInInfo->tMotionCfg.dwCurWinId]);
	strncpy(ptMotionWinInfo->szName, szName, (len<(MAX_CAPTION_LEN+1)? len: MAX_CAPTION_LEN));
	ptMotionWinInfo->szName[MAX_CAPTION_LEN] = '\0';
	//printf("[VENC_MOTION] VencVideoIn_SetMotionWinName %s\n", ptMotionWinInfo->szName);
}

/* ========================================================================= */
void VencVideoIn_SetMotionWinLeft(HANDLE hObject, const CHAR *szLeft, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TMotionWinInfo *ptMotionWinInfo;
	DWORD dwLeft;

	ptMotionWinInfo = &(ptVideoInInfo->tMotionCfg.atmwCfg[ptVideoInInfo->tMotionCfg.dwCurWinId]);
	dwLeft = strtoul(szLeft, (char**)NULL, 10);

	if (dwLeft != ptMotionWinInfo->dwLeft) {
		ptMotionWinInfo->bPositionChange = TRUE;
		ptMotionWinInfo->dwLeft = dwLeft;
		ptMotionWinInfo->bAnyFieldChange = TRUE;
		//printf("[VENC_MOTION] widows %u Left Position Change to %u \n", ptVideoInInfo->tMotionCfg.dwCurWinId, ptMotionWinInfo->dwLeft);
	}
}

/* ========================================================================= */
void VencVideoIn_SetMotionWinTop(HANDLE hObject, const CHAR *szTop, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TMotionWinInfo *ptMotionWinInfo;
	DWORD dwTop;

	ptMotionWinInfo = &(ptVideoInInfo->tMotionCfg.atmwCfg[ptVideoInInfo->tMotionCfg.dwCurWinId]);
	dwTop = strtoul(szTop, (char**)NULL, 10);

	if (dwTop != ptMotionWinInfo->dwTop) {
		ptMotionWinInfo->bPositionChange = TRUE;
		ptMotionWinInfo->dwTop = dwTop;
		ptMotionWinInfo->bAnyFieldChange = TRUE;
		//printf("[VENC_MOTION] widows %u Top Position Change to %u \n", ptVideoInInfo->tMotionCfg.dwCurWinId, ptMotionWinInfo->dwTop);	
	}
}

/* ========================================================================= */
void VencVideoIn_SetMotionWinWidth(HANDLE hObject, const CHAR *szWidth, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TMotionWinInfo *ptMotionWinInfo;
	DWORD dwWidth;

	ptMotionWinInfo = &(ptVideoInInfo->tMotionCfg.atmwCfg[ptVideoInInfo->tMotionCfg.dwCurWinId]);
	dwWidth = strtoul(szWidth, (char**)NULL, 10);

	if (dwWidth != ptMotionWinInfo->dwWidth) {
		ptMotionWinInfo->bWindowSizeChange = TRUE;
		ptMotionWinInfo->dwWidth = dwWidth;
		ptMotionWinInfo->bAnyFieldChange = TRUE;
		//printf("[VENC_MOTION] widows %u Width Change to %u \n", ptVideoInInfo->tMotionCfg.dwCurWinId, ptMotionWinInfo->dwWidth);	
	}
}

/* ========================================================================= */
void VencVideoIn_SetMotionWinHeight(HANDLE hObject, const CHAR *szHeight, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TMotionWinInfo *ptMotionWinInfo;
	DWORD dwHeight;

	ptMotionWinInfo = &(ptVideoInInfo->tMotionCfg.atmwCfg[ptVideoInInfo->tMotionCfg.dwCurWinId]);
	dwHeight = strtoul(szHeight, (char**)NULL, 10);

	if (dwHeight != ptMotionWinInfo->dwHeight) {
		ptMotionWinInfo->bWindowSizeChange = TRUE;
		ptMotionWinInfo->dwHeight = dwHeight;
		ptMotionWinInfo->bAnyFieldChange = TRUE;
		//printf("[VENC_MOTION] widows %u Height Change to %u \n", ptVideoInInfo->tMotionCfg.dwCurWinId, ptMotionWinInfo->dwHeight);
	}
}

/* ========================================================================= */
void VencVideoIn_SetMotionWinSens(HANDLE hObject, const CHAR *szSensitivity, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TMotionWinInfo *ptMotionWinInfo;
	DWORD dwSensitivity;

	ptMotionWinInfo = &(ptVideoInInfo->tMotionCfg.atmwCfg[ptVideoInInfo->tMotionCfg.dwCurWinId]);
	dwSensitivity = strtoul(szSensitivity, (char**)NULL, 10);
	if (dwSensitivity > 100) {
		dwSensitivity = 100;
	} else if (dwSensitivity < 0) {
		dwSensitivity = 0;
	}

	if (dwSensitivity != ptMotionWinInfo->dwSensitivity) {
		ptMotionWinInfo->bDetectThresholdChange = TRUE;
		ptMotionWinInfo->dwSensitivity = dwSensitivity;
		ptMotionWinInfo->bAnyFieldChange = TRUE;
		//printf("[VENC_MOTION] widows %u Sensitivity Change to %u \n", ptVideoInInfo->tMotionCfg.dwCurWinId, ptMotionWinInfo->dwSensitivity);
	}
}

/* ========================================================================= */
void VencVideoIn_SetMotionWinObjsize(HANDLE hObject, const CHAR *szObjSize, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TMotionWinInfo *ptMotionWinInfo;
	DWORD dwObjSize;

	ptMotionWinInfo = &(ptVideoInInfo->tMotionCfg.atmwCfg[ptVideoInInfo->tMotionCfg.dwCurWinId]);
	dwObjSize = strtoul(szObjSize, (char**)NULL, 10);
	if ( dwObjSize > 100) {
		dwObjSize = 100;
	} else if (dwObjSize < 0) {
		dwObjSize = 0;
	}

	if (dwObjSize != ptMotionWinInfo->dwObjSize) {
		ptMotionWinInfo->bDetectThresholdChange = TRUE;
		ptMotionWinInfo->dwObjSize = dwObjSize;
		ptMotionWinInfo->bAnyFieldChange = TRUE;
		//printf("[VENC_MOTION] widows %u ObjSize Change to %u \n", ptVideoInInfo->tMotionCfg.dwCurWinId, ptMotionWinInfo->dwObjSize);
	}
}

/* ========================================================================= */
SCODE VencVideoIn_Motion_AddWindow(TVideoInInfo *ptVideoInInfo, TMotionWinInfo *ptMotionWinInfo)
{
	TImgBackEndProcessOptions tOptions;
	TMotionDetectWindow tMotionWin;
	HANDLE	hIBPObj = ptVideoInInfo->hIBPObj;
#ifdef _PRE_NR_	
	HANDLE hSencodPhaseIBPObj = ptVideoInInfo->hSencodPhaseIBPObj;
#endif
	FLOAT	fXTconf2def = ptVideoInInfo->tMotionCfg.fXTranslate;
	FLOAT	fYTconf2def = ptVideoInInfo->tMotionCfg.fYTranslate;
	FLOAT	fXSconf2def = ptVideoInInfo->tMotionCfg.fXScale;
	FLOAT	fYSconf2def = ptVideoInInfo->tMotionCfg.fYScale;

	FLOAT	fXTreal2def = ptVideoInInfo->tMtnR2D.fXTranslate;
	FLOAT	fYTreal2def = ptVideoInInfo->tMtnR2D.fYTranslate;
	FLOAT	fXSreal2def = ptVideoInInfo->tMtnR2D.fXScale;
	FLOAT	fYSreal2def = ptVideoInInfo->tMtnR2D.fYScale;
	
	memset(&tOptions, 0, sizeof(TImgBackEndProcessOptions));
	memset(&tMotionWin, 0, sizeof(TMotionDetectWindow));
	
	ptVideoInInfo->tIBPState.tCurrInBuff.bIsMotionDetectResultValid = FALSE;
	tMotionWin.dwHorzStart = (DWORD) (((((((FLOAT)ptMotionWinInfo->dwLeft)  * fXSconf2def)+fXTconf2def) - fXTreal2def) / fXSreal2def) + 0.5);
	tMotionWin.dwVertStart = (DWORD) (((((((FLOAT)ptMotionWinInfo->dwTop)   * fYSconf2def)+fYTconf2def) - fYTreal2def) / fYSreal2def) + 0.5);
	tMotionWin.dwWidth = (DWORD) (((((((FLOAT)ptMotionWinInfo->dwWidth) * fXSconf2def)+fXTconf2def) - fXTreal2def) / fXSreal2def) + 0.5);
	tMotionWin.dwHeight = (DWORD) (((((((FLOAT)ptMotionWinInfo->dwHeight)* fYSconf2def)+fYTconf2def) - fYTreal2def) / fYSreal2def) + 0.5);

	tMotionWin.dwMotionPercentThr = ptMotionWinInfo->dwObjSize;
	tMotionWin.byPixDiffThr = 255 * (100 - ptMotionWinInfo->dwSensitivity) / 100;

	tOptions.eOptionFlags = VIDEO_ADD_WINDOW;
	tOptions.apvUserData[0] = (PVOID)(&tMotionWin);
	if (ImgBackEndProcess_SetOptions(hIBPObj, &tOptions) != S_OK) {
		printf("[VENC_MOTION] Set VIDEO_ADD_WINDOW option fail !!\n");
		return S_FAIL;
	}
#ifdef _PRE_NR_	
	if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObj, &tOptions) != S_OK) {
		printf("[VENC_MOTION] Set VIDEO_ADD_WINDOW option fail !!\n");
		return S_FAIL;
	}
#endif
	// keep window serial number to get information when there is a motion
	ptMotionWinInfo->dwSerialNum = tMotionWin.dwSerialNum;					
	printf("[venc_videoin_conf]Add window S/N: %u\n", ptMotionWinInfo->dwSerialNum);

	//printf("[venc_videoin_conf](%d)motio window:(serialNum,HorzStart,VertStart,Width,Height,dwMotionPercentThr,byPixDiffThr)="
		//	"(%d,%d,%d,%d,%d,%d,%d)\n",__LINE__,tMotionWin.dwSerialNum,tMotionWin.dwHorzStart,tMotionWin.dwVertStart,tMotionWin.dwWidth,tMotionWin.dwHeight,tMotionWin.dwMotionPercentThr,tMotionWin.byPixDiffThr);

	return S_OK;
}

/* ========================================================================= */
SCODE VencVideoIn_Motion_DeleteWindow(TVideoInInfo *ptVideoInInfo, TMotionWinInfo *ptMotionWinInfo)
{
	TImgBackEndProcessOptions tOptions;
	ptVideoInInfo->tIBPState.tCurrInBuff.bIsMotionDetectResultValid = FALSE;

	memset(&tOptions, 0, sizeof(TImgBackEndProcessOptions));
	tOptions.eOptionFlags = VIDEO_DELETE_WINDOW;
	tOptions.apvUserData[0] = (PVOID)ptMotionWinInfo->dwSerialNum;
	if (ImgBackEndProcess_SetOptions(ptVideoInInfo->hIBPObj, &tOptions) != S_OK) {
		printf("[VENC_MOTION] Set VIDEO_DELETE_WINDOW option fail !!\n");
		return S_FAIL;
	}
#ifdef _PRE_NR_
	if (ImgBackEndProcess_SetOptions(ptVideoInInfo->hSencodPhaseIBPObj, &tOptions) != S_OK) {
		printf("[VENC_MOTION] Set VIDEO_DELETE_WINDOW option fail !!\n");
		return S_FAIL;
	}
#endif
	printf("[venc_videoin_conf]Deleting window S/N: %u\n", ptMotionWinInfo->dwSerialNum);
	return S_OK;
}

/* ========================================================================= */
SCODE VencVideoIn_Motion_SetPosition(TVideoInInfo *ptVideoInInfo, TMotionWinInfo *ptMotionWinInfo)
{
	TImgBackEndProcessOptions tOptions;
	HANDLE	hIBPObj = ptVideoInInfo->hIBPObj;
#ifdef _PRE_NR_	
	HANDLE hSencodPhaseIBPObj = ptVideoInInfo->hSencodPhaseIBPObj;
#endif
	FLOAT	fXTconf2def = ptVideoInInfo->tMotionCfg.fXTranslate;
	FLOAT	fYTconf2def = ptVideoInInfo->tMotionCfg.fYTranslate;
	FLOAT	fXSconf2def = ptVideoInInfo->tMotionCfg.fXScale;
	FLOAT	fYSconf2def = ptVideoInInfo->tMotionCfg.fYScale;

	FLOAT	fXTreal2def = ptVideoInInfo->tMtnR2D.fXTranslate;
	FLOAT	fYTreal2def = ptVideoInInfo->tMtnR2D.fYTranslate;
	FLOAT	fXSreal2def = ptVideoInInfo->tMtnR2D.fXScale;
	FLOAT	fYSreal2def = ptVideoInInfo->tMtnR2D.fYScale;
	
	ptVideoInInfo->tIBPState.tCurrInBuff.bIsMotionDetectResultValid = FALSE;
	
	memset(&tOptions, 0, sizeof(TImgBackEndProcessOptions));
	tOptions.eOptionFlags = VIDEO_CHANGE_WINDOW_POSITION;
	tOptions.apvUserData[0] = (PVOID)ptMotionWinInfo->dwSerialNum;
	tOptions.apvUserData[1] = (PVOID)((DWORD) (((((((FLOAT)ptMotionWinInfo->dwLeft)  * fXSconf2def)+fXTconf2def) - fXTreal2def) / fXSreal2def) + 0.5));
	tOptions.apvUserData[2] = (PVOID)((DWORD)(((((((FLOAT)ptMotionWinInfo->dwTop)   * fYSconf2def)+fYTconf2def) - fYTreal2def) / fYSreal2def) + 0.5));

	if (ImgBackEndProcess_SetOptions(hIBPObj, &tOptions) != S_OK) {
		printf("[VENC_MOTION] Set VIDEO_CHANGE_WINDOW_POSITION option fail !!\n");
		return S_FAIL;
	}
#ifdef _PRE_NR_	
	if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObj, &tOptions) != S_OK) {
		printf("[VENC_MOTION] Set VIDEO_CHANGE_WINDOW_POSITION option fail !!\n");
		return S_FAIL;
	}
#endif
	printf("[VENC_MOTION] Set (x,y) = (%u, %u) \n", (DWORD)tOptions.apvUserData[1], (DWORD)tOptions.apvUserData[2]);

	return S_OK;
}

/* ========================================================================= */
SCODE VencVideoIn_Motion_SetWindowSize(TVideoInInfo *ptVideoInInfo, TMotionWinInfo *ptMotionWinInfo)
{
	TImgBackEndProcessOptions tOptions;	

	HANDLE	hIBPObj = ptVideoInInfo->hIBPObj;
#ifdef _PRE_NR_	
	HANDLE hSencodPhaseIBPObj = ptVideoInInfo->hSencodPhaseIBPObj;
#endif
	FLOAT	fXTconf2def = ptVideoInInfo->tMotionCfg.fXTranslate;
	FLOAT	fYTconf2def = ptVideoInInfo->tMotionCfg.fYTranslate;
	FLOAT	fXSconf2def = ptVideoInInfo->tMotionCfg.fXScale;
	FLOAT	fYSconf2def = ptVideoInInfo->tMotionCfg.fYScale;
	
	FLOAT	fXTreal2def = ptVideoInInfo->tMtnR2D.fXTranslate;
	FLOAT	fYTreal2def = ptVideoInInfo->tMtnR2D.fYTranslate;
	FLOAT	fXSreal2def = ptVideoInInfo->tMtnR2D.fXScale;
	FLOAT	fYSreal2def = ptVideoInInfo->tMtnR2D.fYScale;
	
	ptVideoInInfo->tIBPState.tCurrInBuff.bIsMotionDetectResultValid = FALSE;
	
	memset(&tOptions, 0, sizeof(TImgBackEndProcessOptions));
	tOptions.eOptionFlags = VIDEO_CHANGE_WINDOW_SIZE;
	tOptions.apvUserData[0] = (PVOID)ptMotionWinInfo->dwSerialNum;
	tOptions.apvUserData[1] = (PVOID) ((DWORD)(((((((FLOAT)ptMotionWinInfo->dwWidth) * fXSconf2def)+fXTconf2def) - fXTreal2def) / fXSreal2def) + 0.5));
	tOptions.apvUserData[2] = (PVOID) ((DWORD)(((((((FLOAT)ptMotionWinInfo->dwHeight)* fYSconf2def)+fYTconf2def) - fYTreal2def) / fYSreal2def) + 0.5));

	if (ImgBackEndProcess_SetOptions(hIBPObj, &tOptions) != S_OK) {
		printf("[VENC_MOTION] Set VIDEO_CHANGE_WINDOW_POSITION option fail !!\n");
		return S_FAIL;
	}
#ifdef _PRE_NR_	
	if (ImgBackEndProcess_SetOptions(hSencodPhaseIBPObj, &tOptions) != S_OK) {
		printf("[VENC_MOTION] Set VIDEO_CHANGE_WINDOW_POSITION option fail !!\n");
		return S_FAIL;
	}
#endif
	return S_OK;
}

/* ========================================================================= */
//SCODE VencVideoIn_Motion_SetDetectThreshold(HANDLE hDeInterlacer, TMotionWinInfo *ptMotionWinInfo)
SCODE VencVideoIn_Motion_SetDetectThreshold(TVideoInInfo *ptVideoInInfo, TMotionWinInfo *ptMotionWinInfo)
{
	TImgBackEndProcessOptions tOptions;	
	ptVideoInInfo->tIBPState.tCurrInBuff.bIsMotionDetectResultValid = FALSE;

	memset(&tOptions, 0, sizeof(TImgBackEndProcessOptions));
	tOptions.eOptionFlags = VIDEO_CHANGE_MOTION_DETECTION_THRESHOLD;
	tOptions.apvUserData[0] = (PVOID)ptMotionWinInfo->dwSerialNum;
	tOptions.apvUserData[1] = (PVOID)((DWORD)(255 * (100 - ptMotionWinInfo->dwSensitivity) / 100)); // pixel difference threshold
	tOptions.apvUserData[2] = (PVOID)ptMotionWinInfo->dwObjSize; // motion percentage

	if (ImgBackEndProcess_SetOptions(ptVideoInInfo->hIBPObj, &tOptions) != S_OK) {
		printf("[VENC_MOTION] Set VIDEO_CHANGE_WINDOW_POSITION option fail !!\n");
		return S_FAIL;
	}
#ifdef _PRE_NR_	
	if (ImgBackEndProcess_SetOptions(ptVideoInInfo->hSencodPhaseIBPObj, &tOptions) != S_OK) {
		printf("[VENC_MOTION] Set VIDEO_CHANGE_WINDOW_POSITION option fail !!\n");
		return S_FAIL;
	}
#endif
	return S_OK;
}


SCODE VencVideoIn_MotionSetoptions(HANDLE hObject)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TMotionWinInfo *ptMotionWinInfo;
	DWORD dwIndex;

	// According to bMotionEnabledChange status, do things respectively.
	if (ptVideoInInfo->tMotionCfg.bMotionEnabledChange == TRUE) {
		// <enable> off -> on 
		if (ptVideoInInfo->tMotionCfg.bMotionEnabled == TRUE) {
			// if <active> is on, it should ADD_WINDOW, and record the serial number
			for (dwIndex = 0; dwIndex < MAX_MOTION_WIN_NUM; dwIndex++) {
				ptMotionWinInfo = &(ptVideoInInfo->tMotionCfg.atmwCfg[dwIndex]);
				if (ptMotionWinInfo->bEnabled == TRUE) {
					VencVideoIn_Motion_AddWindow(ptVideoInInfo, ptMotionWinInfo);
				}
			}
		} else { // <enable> on -> off // done
			// search for SN, if SN != NON_USED_WINDOW, it should delete, and set it to NON_USED_WINDOW
			for (dwIndex = 0; dwIndex < MAX_MOTION_WIN_NUM; dwIndex++) {
				ptMotionWinInfo = &(ptVideoInInfo->tMotionCfg.atmwCfg[dwIndex]);
				if (ptMotionWinInfo->dwSerialNum != NON_USED_WINDOW) {
					//1. delete window
					//2. set to NON_USED_WINDOW
#if 1
					VencVideoIn_Motion_DeleteWindow(ptVideoInInfo, ptMotionWinInfo);
#endif
					ptMotionWinInfo->dwSerialNum = NON_USED_WINDOW;
				}
			}			
		}
	} else { // bMotionEnabledChange == FALSE
		// <enable> on -> on
		if (ptVideoInInfo->tMotionCfg.bMotionEnabled == TRUE) {
			// each window, if bEnabled change, it should ADD_WINDOW or DELETE_WINDOW
			//              else, other field to setoption
			for (dwIndex = 0; dwIndex < MAX_MOTION_WIN_NUM; dwIndex++) {
				ptMotionWinInfo = &(ptVideoInInfo->tMotionCfg.atmwCfg[dwIndex]);
				if (ptMotionWinInfo->bAnyFieldChange == TRUE) {
					if (ptMotionWinInfo->bEnabledChange == TRUE) {
						if (ptMotionWinInfo->bEnabled == TRUE) { // <active> off -> on
							VencVideoIn_Motion_AddWindow(ptVideoInInfo, ptMotionWinInfo);
						} else { // <active> on -> off
#if 1
							VencVideoIn_Motion_DeleteWindow(ptVideoInInfo, ptMotionWinInfo);
#endif
							ptMotionWinInfo->dwSerialNum = NON_USED_WINDOW;
						}
						continue; // 'cause of add or delete, it shouldn't set again.
					}
					if (ptMotionWinInfo->bPositionChange == TRUE) {
						VencVideoIn_Motion_SetPosition(ptVideoInInfo, ptMotionWinInfo);
					}
					if (ptMotionWinInfo->bWindowSizeChange == TRUE) {
						VencVideoIn_Motion_SetWindowSize(ptVideoInInfo, ptMotionWinInfo);
					}
					if (ptMotionWinInfo->bDetectThresholdChange == TRUE) {
#if 1
						VencVideoIn_Motion_SetDetectThreshold(ptVideoInInfo, ptMotionWinInfo);
#endif
					}
				}
			}			
		}	
	}
	return S_OK;
}
#endif // _VMA_IBPE

/* ============================================================================================= */
void VencVideoIn_SetPhotometricMode(HANDLE hObject, const CHAR *szPhotoMtcMode, SDWORD len)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;

	CHAR szTemp[len+1];
	EPhotoMtcMode	ePhotoMtcMode;

	snprintf(szTemp, len+1, "%s", szPhotoMtcMode);
	if (strcasecmp(szTemp, "photometric1") == 0) {
		ePhotoMtcMode = photomtcm1;
	} else if (strcasecmp(szTemp, "photometric2") == 0) {
		ePhotoMtcMode = photomtcm2;
	} else if (strcasecmp(szTemp, "userdefine1") == 0) {
		ePhotoMtcMode = photomtcmUserDefine1;
	} else if (strcasecmp(szTemp, "userdefine2") == 0) {
		ePhotoMtcMode = photomtcmUserDefine2;
	} else {
		ePhotoMtcMode = photomtcdisable;
	}

	if (ptVideoInInfo->tPhotoMtcCfg.ePhotoMtcMode != ePhotoMtcMode) {
		ptVideoInInfo->tPhotoMtcCfg.ePhotoMtcMode = ePhotoMtcMode;
		ptVideoInInfo->tPhotoMtcCfg.bModified = TRUE;
	}
	//printf("[VENC_VIDEOIN] VencVideoIn_SetPhotometricMode  ePhotoMtcMode = %d !!\n", ePhotoMtcMode);
}
void VencVideoIn_SetPhotometricLSCRatio(HANDLE hObject, const CHAR *szPhotoMtcLSCRatio, SDWORD len)
{
	
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	
	DWORD dwValue = 0;
	CHAR szTemp[len+1];

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szPhotoMtcLSCRatio);
	dwValue = strtoul(szTemp, NULL, 10);
	
	if (ptVideoInInfo->tPhotoMtcCfg.dwLSCRatio != dwValue)
	{
		ptVideoInInfo->tPhotoMtcCfg.dwLSCRatio = dwValue;
		ptVideoInInfo->tPhotoMtcCfg.bModified = TRUE;
	}
}
/* ========================================================================= */
SCODE VencVideoIn_PhotoMtcSetoptions(HANDLE hObject)
{
	TVideoInInfo *ptVideoInInfo = (TVideoInInfo *)hObject;
	TVideoSignalOptions tVideoSigOptions;
	EPhotoMtcMode ePhotoMtcMode = ptVideoInInfo->tPhotoMtcCfg.ePhotoMtcMode;
	FILE  *pfPhotoLDCRef;
	CHAR	szTemp[128];

	tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_PHOTO_LDC_EN;
	tVideoSigOptions.adwUserData[0] = (ePhotoMtcMode == photomtcdisable)? FALSE : TRUE;
	if (VideoCap_SetOptions(ptVideoInInfo->hVideoCapObj, &tVideoSigOptions) == S_OK) {
//		printf("[VENC_VIDEOIN] Set Photometric enable %s success !!\n", (ePhotoMtcMode == photomtcdisable)? "FALSE" : "true");
	} else {
		printf("[VENC_VIDEOIN] Set Photometric enable %s fail !!\n", (ePhotoMtcMode == photomtcdisable)? "FALSE" : "true");
	}

	if (ePhotoMtcMode != photomtcdisable) {
		if (ePhotoMtcMode == photomtcm1) {
			strcpy(szTemp, PHOTOLDCREF_M1);
		} else if (ePhotoMtcMode == photomtcm2) {
			strcpy(szTemp, PHOTOLDCREF_M2);
		} else if (ePhotoMtcMode == photomtcmUserDefine1) {
			strcpy(szTemp, PHOTOLDCREF_USERDEFINE1);
		} else {
			strcpy(szTemp, PHOTOLDCREF_USERDEFINE2);
		}
		if ((pfPhotoLDCRef = fopen(szTemp, "r")) == NULL) {
			printf("[VENC_VIDEOIN] Open Photo LDC reference file %s fail !!\n", szTemp);
			return S_FAIL;
		} else {
			printf("[VENC_VIDEOIN] Open Photo LDC reference file %s successful !!\n", szTemp);
			memset(ptVideoInInfo->pwPhotoLDCRefFrame, 1024, ptVideoInInfo->dwPhotoLDCTblSize);
			if (fread(ptVideoInInfo->pwPhotoLDCRefFrame, sizeof(WORD), ptVideoInInfo->dwPhotoLDCTblSize, pfPhotoLDCRef) != ptVideoInInfo->dwPhotoLDCTblSize) {
				printf("[VENC_VIDEOIN] Insufficient photometric lens distortion correction reference frame data !!\n");
				fclose(pfPhotoLDCRef);
				return S_FAIL;
			} else {
				fclose(pfPhotoLDCRef);
				tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_PHOTO_LDC_TABLE;
				tVideoSigOptions.adwUserData[0] = (DWORD)ptVideoInInfo->pwPhotoLDCRefFrame;
				
				if (VideoCap_SetOptions(ptVideoInInfo->hVideoCapObj, &tVideoSigOptions) == S_OK) {
					printf("[VENC_VIDEOIN] Set Photometric mode %d success !!\n", ePhotoMtcMode);
				} else {
					printf("[VENC_VIDEOIN] Set Photometric mode %d Fail !!\n", ePhotoMtcMode);
				}
			}
		}
	}
	return S_OK;
}
