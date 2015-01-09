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
 * venc_master_conf.c
 *
 * \brief
 * Video master process setoption implement.
 *
 * \date
 * 2009/08/04
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#include "venc_master_local.h"
#include <syslog.h>
#include <ctype.h>
#include <sys/stat.h>


/* ============================================================================================= */

//-------------StartElementHandler-------------

//-------------CharDataHandler-------------
//setting
void VencMaster_SetCapResolution(HANDLE hObject, const CHAR *szResolution, SDWORD len);
void VencMaster_SetVideoColor(HANDLE hObject, const CHAR *szColor, SDWORD len);
void VencMaster_SetVideoCMOSMode(HANDLE hObject, const CHAR *szFreq, SDWORD len);
void VencMaster_SetVideoFlip(HANDLE hObject, const CHAR *szFlip, SDWORD len);
void VencMaster_SetVideoMirror(HANDLE hObject, const CHAR *szMirror, SDWORD len);
void VencMaster_SetAutoIRIS(HANDLE hObject, const CHAR *szAutoIRIS, SDWORD len);
void VencMaster_SetIRCut(HANDLE hObject, const CHAR *szAutoIRIS, SDWORD len);
//WDR setting
void VencMaster_SetWDREnable(HANDLE hObject, const CHAR *szValue, SDWORD len);
void VencMaster_SetWDRLevel(HANDLE hObject, const CHAR *szValue, SDWORD len);
void VencMaster_SetWDRLevelValue(HANDLE hObject, const CHAR *szValue, SDWORD len);
//AutoFocus setting
void VencMaster_SetAutoCurrentCmd(HANDLE hObject, const CHAR *szValue, SDWORD len);
void VencMaster_SetAutoFocusValue(HANDLE hObject, const CHAR *szValue, SDWORD len);
void VencMaster_SetAutoFocusZoom(HANDLE hObject, const CHAR *szValue, SDWORD len);
void VencMaster_SetAutoFocusSpeed(HANDLE hObject, const CHAR *szSpeed, SDWORD len);
void VencMaster_SetAutoFocusStartX(HANDLE hObject, const CHAR *szStartX, SDWORD len);
void VencMaster_SetAutoFocusStartY(HANDLE hObject, const CHAR *szStartY, SDWORD len);
void VencMaster_SetAutoFocusWidth(HANDLE hObject, const CHAR *szWidth, SDWORD len);
void VencMaster_SetAutoFocusHeight(HANDLE hObject, const CHAR *szHeight, SDWORD len);
void VencMaster_SetAutoFocusViewResolution(HANDLE hObject, const CHAR *szResolution, SDWORD len);
void VencMaster_SetAutoFocusZoomSpeed(HANDLE hObject, const CHAR *szSpeed, SDWORD len);
void VencMaster_SetAutoFocusNoiseThresHold(HANDLE hObject, const CHAR *szthreshold, SDWORD len);
void VencMaster_SetAutoFocusReFocus(HANDLE hObject, const CHAR *szReFocus, SDWORD len);
// color correction
void VencMaster_SetCCCoeff_RR(HANDLE hObject, const char *szCoeffRR, SDWORD len);
void VencMaster_SetCCCoeff_GR(HANDLE hObject, const char *szCoeffGR, SDWORD len);
void VencMaster_SetCCCoeff_BR(HANDLE hObject, const char *szCoeffBR, SDWORD len);
void VencMaster_SetCCCoeff_RG(HANDLE hObject, const char *szCoeffRG, SDWORD len);
void VencMaster_SetCCCoeff_GG(HANDLE hObject, const char *szCoeffGG, SDWORD len);
void VencMaster_SetCCCoeff_BG(HANDLE hObject, const char *szCoeffBG, SDWORD len);
void VencMaster_SetCCCoeff_RB(HANDLE hObject, const char *szCoeffRB, SDWORD len);
void VencMaster_SetCCCoeff_GB(HANDLE hObject, const char *szCoeffGB, SDWORD len);
void VencMaster_SetCCCoeff_BB(HANDLE hObject, const char *szCoeffBB, SDWORD len);
void VencMaster_SetCCOffset_R(HANDLE hObject, const char *szOffsetR, SDWORD len);
void VencMaster_SetCCOffset_G(HANDLE hObject, const char *szOffsetG, SDWORD len);
void VencMaster_SetCCOffset_B(HANDLE hObject, const char *szOffsetB, SDWORD len);
// gamma table
SCODE VencMaster_SetGammaTableTag(HANDLE hObject, const CHAR *szGammaTableMode, const CHAR** atts);
void VencMaster_SetGammaTableCollection(HANDLE hObject, const char *szTable, SDWORD len);
void VencMaster_SetGammaTable(HANDLE hObject, const CHAR *szElement);
// white balance
void VencMaster_SetWhiteBalanceSelectMode(HANDLE hObject, const CHAR *szWBSelectMode, SDWORD len);
SCODE VencMaster_SetWhiteBalanceModeTag(HANDLE hObject, const CHAR *szWhiteBalanceMode, const CHAR** atts);
void VencMaster_SetWhiteBalanceRedGain(HANDLE hObject, const CHAR *szRedGain, SDWORD len);
void VencMaster_SetWhiteBalanceBlueGain(HANDLE hObject, const CHAR *szBlueGain, SDWORD len);
void VencMaster_SetWhiteBalanceGreenGain(HANDLE hObject, const CHAR *szGreenGain, SDWORD len);
void VencMaster_SetWhiteBalanceFullFrameGrid(HANDLE hObject, const CHAR *seElement, SDWORD len);
SCODE  VencMaster_SetWBGridTag(HANDLE hObject, const CHAR *szElement, const CHAR** atts);
void VencMaster_SetWBGridSelected(HANDLE hObject, const CHAR *seElement, SDWORD len);
void VencMaster_SetWBGridWeight(HANDLE hObject, const CHAR *seElement, SDWORD len);
// auto exposure
void VencMaster_SetAEMode(HANDLE hObject, const CHAR *szMode, SDWORD len);
void VencMaster_SetAEXTranslate(HANDLE hObject, const CHAR *szAEXTranslate, SDWORD len);
void VencMaster_SetAEYTranslate(HANDLE hObject, const CHAR *szAEYTranslate, SDWORD len);
void VencMaster_SetAEXScale(HANDLE hObject, const CHAR *szAEXScale, SDWORD len);
void VencMaster_SetAEYScale(HANDLE hObject, const CHAR *szAEYScale, SDWORD len);
void VencMaster_SetAEMaxShutter(HANDLE hObject, const CHAR *szMaxShutter, SDWORD len);
void VencMaster_SetAEMaxGain(HANDLE hObject, const CHAR *szMaxGain, SDWORD len);
void VencMaster_SetAEMinShutter(HANDLE hObject, const CHAR *szMinShutter, SDWORD len);
void VencMaster_SetAEMinGain(HANDLE hObject, const CHAR *szMinGain, SDWORD len);
void VencMaster_SetAETargetLuminance(HANDLE hObject, const CHAR *szTargetLuminance, SDWORD len);
void VencMaster_SetAETargetOffset(HANDLE hObject, const CHAR *szTargetOffset, SDWORD len);
SCODE VencMaster_SetAEWinTag(HANDLE hObject, const CHAR *szAEWin, const CHAR** atts);
void VencMaster_SetAEWinStartXPos(HANDLE hObject, const CHAR *szStartX, SDWORD len);
void VencMaster_SetAEWinStartYPos(HANDLE hObject, const CHAR *szStartY, SDWORD len);
void VencMaster_SetAEWinWidth(HANDLE hObject, const CHAR *szWidth, SDWORD len);
void VencMaster_SetAEWinHeight(HANDLE hObject, const CHAR *szHeight, SDWORD len);
void VencMaster_SetAEWinWeight(HANDLE hObject, const CHAR *szName, SDWORD len);
void VencMaster_SetAEGridRangeStartXPos(HANDLE hObject, const CHAR *szStartX, SDWORD len);
void VencMaster_SetAEGridRangeStartYPos(HANDLE hObject, const CHAR *szStartY, SDWORD len);
void VencMaster_SetAEGridRangeWidth(HANDLE hObject, const CHAR *szWidth, SDWORD len);
void VencMaster_SetAEGridRangeHeight(HANDLE hObject, const CHAR *szHeight, SDWORD len);
SCODE VencMaster_SetAEGridTag(HANDLE hObject, const CHAR *szAEWin, const CHAR** atts);
void VencMaster_SetAEGridSelected(HANDLE hObject, const CHAR *szSelected, SDWORD len);
void VencMaster_SetAEGridWeight(HANDLE hObject, const CHAR *szWeight, SDWORD len);
//tone mapping
void VencMaster_SetToneMappingEnable(HANDLE hObject, const CHAR *szTMEnable, SDWORD len);
void VencMaster_SetToneMappingLevel(HANDLE hObject, const CHAR *szTMLevel, SDWORD len);
//black clamp
void VencMaster_SetBlackClampGr(HANDLE hObject, const CHAR *szElement, SDWORD len);
void VencMaster_SetBlackClampR(HANDLE hObject, const CHAR *szElement, SDWORD len);
void VencMaster_SetBlackClampGb(HANDLE hObject, const CHAR *szElement, SDWORD len);
void VencMaster_SetBlackClampB(HANDLE hObject, const CHAR *szElement, SDWORD len);
//de-impulse
void VencMaster_SetDeImpulseEnable(HANDLE hObject, const CHAR *szElement, SDWORD len);
void VencMaster_SetDeImpulseDcBotThrd(HANDLE hObject, const CHAR *szElement, SDWORD len);
void VencMaster_SetDeImpulseDcTopThrd(HANDLE hObject, const CHAR *szElement, SDWORD len);
void VencMaster_SetDeImpulseMaxTopLevel(HANDLE hObject, const CHAR *szElement, SDWORD len);
void VencMaster_SetDeImpulseMaxBotLevel(HANDLE hObject, const CHAR *szElement, SDWORD len);
void VencMaster_SetDeImpulseMinTopLevel(HANDLE hObject, const CHAR *szElement, SDWORD len);
void VencMaster_SetDeImpulseMinBotLevel(HANDLE hObject, const CHAR *szElement, SDWORD len);
//contrast enhancement
void VencMaster_SetCEEnable(HANDLE hObject, const CHAR *szCEEnable, SDWORD len);
void VencMaster_SetCEMode(HANDLE hObject, const CHAR *szCEMode, SDWORD len);
void VencMaster_SetCEBirghtness(HANDLE hObject, const CHAR *szCEBrightness, SDWORD len);
void VencMaster_SetCEDefaultMethod(HANDLE hObject, const CHAR *szCEDefaultMethod, SDWORD len);
SCODE VencMaster_SetCEUserTableTag(HANDLE hObject, const CHAR *szCEUserTableMode, const CHAR** atts);
void VencMaster_SetCEUserTableCollection(HANDLE hObject, const CHAR *szTable, SDWORD len);
void VencMaster_SetCEUserTable(HANDLE hObject, const CHAR *szElement);
//Image mode
void VencMaster_SetImageAdjustmentSelectMode(HANDLE hObject, const CHAR *szImgAdjSelectMode, SDWORD len);
void VencMaster_SetImageAdjustmentBright(HANDLE hObject, const CHAR *szBright, SDWORD len);
void VencMaster_SetImageAdjustmentContrast(HANDLE hObject, const CHAR *szContrast, SDWORD len);
void VencMaster_SetImageAdjustmentSaturation(HANDLE hObject, const CHAR *szSaturation, SDWORD len);
void VencMaster_SetImageAdjustmentStartLum(HANDLE hObject, const CHAR *szStartLum, SDWORD len);
void VencMaster_SetImageAdjustmentEndLum(HANDLE hObject, const CHAR *szEndLum, SDWORD len);
void VencMaster_SetImageAdjustmentTolerance(HANDLE hObject, const CHAR *szTolerance, SDWORD len);

TXmlWrapperTreeMap ptMasterTreeMap[] =
{
	//setting
	{"root/venc_master/setting/ircut",NULL,&VencMaster_SetIRCut, NULL},
	//{"root/venc_master/setting/autoiris", NULL, &VencMaster_SetAutoIRIS, NULL},
	{"root/venc_master/setting/resolution", NULL, &VencMaster_SetCapResolution, NULL},
	{"root/venc_master/setting/pline_freq", NULL, &VencMaster_SetVideoCMOSMode, NULL},
	{"root/venc_master/setting/flip_enable", NULL, &VencMaster_SetVideoFlip, NULL},
	{"root/venc_master/setting/mirror_enable", NULL, &VencMaster_SetVideoMirror, NULL},
	{"root/venc_master/setting/color_mode", NULL, &VencMaster_SetVideoColor, NULL},
	//AutoFocus setting
	{"root/venc_master/setting/auto_focus/current_cmd", NULL, &VencMaster_SetAutoCurrentCmd, NULL},
	{"root/venc_master/setting/auto_focus/value", NULL, &VencMaster_SetAutoFocusValue, NULL},
	{"root/venc_master/setting/auto_focus/zoom",NULL,&VencMaster_SetAutoFocusZoom,NULL},
	{"root/venc_master/setting/auto_focus/speed", NULL, &VencMaster_SetAutoFocusSpeed, NULL},
	{"root/venc_master/setting/auto_focus/startX", NULL, &VencMaster_SetAutoFocusStartX, NULL},
	{"root/venc_master/setting/auto_focus/startY", NULL, &VencMaster_SetAutoFocusStartY, NULL},
	{"root/venc_master/setting/auto_focus/width", NULL, &VencMaster_SetAutoFocusWidth, NULL},
	{"root/venc_master/setting/auto_focus/height", NULL, &VencMaster_SetAutoFocusHeight, NULL},
	{"root/venc_master/setting/auto_focus/view_resolution", NULL, &VencMaster_SetAutoFocusViewResolution, NULL},
	{"root/venc_master/setting/auto_focus/zoomspeed", NULL, &VencMaster_SetAutoFocusZoomSpeed, NULL},
	{"root/venc_master/setting/auto_focus/noisethreshold", NULL, &VencMaster_SetAutoFocusNoiseThresHold, NULL},
	{"root/venc_master/setting/auto_focus/refocus", NULL, &VencMaster_SetAutoFocusReFocus, NULL},
	//WDR setting
	{"root/venc_master/isp/wdr/enable", NULL, &VencMaster_SetWDREnable, NULL},
	{"root/venc_master/isp/wdr/level", NULL, &VencMaster_SetWDRLevel, NULL},
	{"root/venc_master/isp/wdr/levelvalue", NULL, &VencMaster_SetWDRLevelValue, NULL},
	//color correction setting
	{"root/venc_master/isp/color_correction/coeff_RR", NULL, &VencMaster_SetCCCoeff_RR, NULL},
	{"root/venc_master/isp/color_correction/coeff_GR", NULL, &VencMaster_SetCCCoeff_GR, NULL},
	{"root/venc_master/isp/color_correction/coeff_BR", NULL, &VencMaster_SetCCCoeff_BR, NULL},
	{"root/venc_master/isp/color_correction/coeff_RG", NULL, &VencMaster_SetCCCoeff_RG, NULL},
	{"root/venc_master/isp/color_correction/coeff_GG", NULL, &VencMaster_SetCCCoeff_GG, NULL},
	{"root/venc_master/isp/color_correction/coeff_BG", NULL, &VencMaster_SetCCCoeff_BG, NULL},
	{"root/venc_master/isp/color_correction/coeff_RB", NULL, &VencMaster_SetCCCoeff_RB, NULL},
	{"root/venc_master/isp/color_correction/coeff_GB", NULL, &VencMaster_SetCCCoeff_GB, NULL},
	{"root/venc_master/isp/color_correction/coeff_BB", NULL, &VencMaster_SetCCCoeff_BB, NULL},
	{"root/venc_master/isp/color_correction/offset_R", NULL, &VencMaster_SetCCOffset_R, NULL},
	{"root/venc_master/isp/color_correction/offset_G", NULL, &VencMaster_SetCCOffset_G, NULL},
	{"root/venc_master/isp/color_correction/offset_B", NULL, &VencMaster_SetCCOffset_B, NULL},
	//gamma table
	{"root/venc_master/isp/gamma_table", &VencMaster_SetGammaTableTag, &VencMaster_SetGammaTableCollection, &VencMaster_SetGammaTable},
	//while balance
	{"root/venc_master/isp/white_balance/select_mode", NULL, &VencMaster_SetWhiteBalanceSelectMode, NULL},
	{"root/venc_master/isp/white_balance/mode", &VencMaster_SetWhiteBalanceModeTag, NULL, NULL},
	{"root/venc_master/isp/white_balance/mode/Red_Gain", NULL, &VencMaster_SetWhiteBalanceRedGain, NULL},
	{"root/venc_master/isp/white_balance/mode/Blue_Gain", NULL, &VencMaster_SetWhiteBalanceBlueGain, NULL},
	{"root/venc_master/isp/white_balance/mode/Green_Gain", NULL, &VencMaster_SetWhiteBalanceGreenGain, NULL},
	{"root/venc_master/isp/white_balance/full_frame_grid", NULL, &VencMaster_SetWhiteBalanceFullFrameGrid, NULL},
	{"root/venc_master/isp/white_balance/grid", &VencMaster_SetWBGridTag, NULL, NULL},
	{"root/venc_master/isp/white_balance/grid/selected", NULL, &VencMaster_SetWBGridSelected, NULL},
	{"root/venc_master/isp/white_balance/grid/weight", NULL, &VencMaster_SetWBGridWeight, NULL},
	//AE
	{"root/venc_master/isp/auto_exposure/Transformation/Translate/x", NULL, &VencMaster_SetAEXTranslate, NULL},
	{"root/venc_master/isp/auto_exposure/Transformation/Translate/y", NULL, &VencMaster_SetAEYTranslate, NULL},
	{"root/venc_master/isp/auto_exposure/Transformation/scale/x", NULL, &VencMaster_SetAEXScale, NULL},
	{"root/venc_master/isp/auto_exposure/Transformation/scale/y", NULL, &VencMaster_SetAEYScale, NULL},
	{"root/venc_master/isp/auto_exposure/mode", NULL, &VencMaster_SetAEMode, NULL},
	{"root/venc_master/isp/auto_exposure/autoiris", NULL, &VencMaster_SetAutoIRIS, NULL},
	{"root/venc_master/isp/auto_exposure/max_shutter", NULL, &VencMaster_SetAEMaxShutter, NULL},
	{"root/venc_master/isp/auto_exposure/max_gain", NULL, &VencMaster_SetAEMaxGain, NULL},
	{"root/venc_master/isp/auto_exposure/min_shutter", NULL, &VencMaster_SetAEMinShutter, NULL},
	{"root/venc_master/isp/auto_exposure/min_gain", NULL, &VencMaster_SetAEMinGain, NULL},
	{"root/venc_master/isp/auto_exposure/target_luminance", NULL, &VencMaster_SetAETargetLuminance, NULL},
	{"root/venc_master/isp/auto_exposure/target_offset", NULL, &VencMaster_SetAETargetOffset, NULL},
	{"root/venc_master/isp/auto_exposure/win", &VencMaster_SetAEWinTag, NULL, NULL},
	{"root/venc_master/isp/auto_exposure/win/startX", NULL, &VencMaster_SetAEWinStartXPos, NULL},
	{"root/venc_master/isp/auto_exposure/win/startY", NULL, &VencMaster_SetAEWinStartYPos, NULL},
	{"root/venc_master/isp/auto_exposure/win/width", NULL, &VencMaster_SetAEWinWidth, NULL},
	{"root/venc_master/isp/auto_exposure/win/height", NULL, &VencMaster_SetAEWinHeight, NULL},
	{"root/venc_master/isp/auto_exposure/win/weight", NULL, &VencMaster_SetAEWinWeight, NULL},
	{"root/venc_master/isp/auto_exposure/grid_range/startX", NULL, &VencMaster_SetAEGridRangeStartXPos, NULL},
	{"root/venc_master/isp/auto_exposure/grid_range/startY", NULL, &VencMaster_SetAEGridRangeStartYPos, NULL},
	{"root/venc_master/isp/auto_exposure/grid_range/width", NULL,  &VencMaster_SetAEGridRangeWidth, NULL},
	{"root/venc_master/isp/auto_exposure/grid_range/height", NULL, &VencMaster_SetAEGridRangeHeight, NULL},
	{"root/venc_master/isp/auto_exposure/grid", &VencMaster_SetAEGridTag, NULL, NULL},
	{"root/venc_master/isp/auto_exposure/grid/selected", NULL, &VencMaster_SetAEGridSelected, NULL},
	{"root/venc_master/isp/auto_exposure/grid/weight", NULL, &VencMaster_SetAEGridWeight, NULL},
	//tone mapping
	{"root/venc_master/isp/tone_mapping/enable", NULL, &VencMaster_SetToneMappingEnable, NULL},
	{"root/venc_master/isp/tone_mapping/level", NULL, &VencMaster_SetToneMappingLevel, NULL},
	//black clamp
	{"root/venc_master/isp/black_clamp/gr", NULL, &VencMaster_SetBlackClampGr, NULL},
	{"root/venc_master/isp/black_clamp/r", NULL, &VencMaster_SetBlackClampR, NULL},
	{"root/venc_master/isp/black_clamp/gb", NULL, &VencMaster_SetBlackClampGb, NULL},
	{"root/venc_master/isp/black_clamp/b", NULL, &VencMaster_SetBlackClampB, NULL},
	//de-impulse
	{"root/venc_master/isp/deimpulse/enable", NULL, &VencMaster_SetDeImpulseEnable, NULL},
	{"root/venc_master/isp/deimpulse/dc_bot_threshold", NULL, &VencMaster_SetDeImpulseDcBotThrd, NULL},
	{"root/venc_master/isp/deimpulse/dc_top_threshold", NULL, &VencMaster_SetDeImpulseDcTopThrd, NULL},
	{"root/venc_master/isp/deimpulse/max_top_level", NULL, &VencMaster_SetDeImpulseMaxTopLevel, NULL},
	{"root/venc_master/isp/deimpulse/max_bot_level", NULL, &VencMaster_SetDeImpulseMaxBotLevel, NULL},
	{"root/venc_master/isp/deimpulse/min_top_level", NULL, &VencMaster_SetDeImpulseMinTopLevel, NULL},
	{"root/venc_master/isp/deimpulse/min_bot_level", NULL, &VencMaster_SetDeImpulseMinBotLevel, NULL},
	//contrast enhancement
	{"root/venc_master/isp/contrast_enhancement/enable", NULL, &VencMaster_SetCEEnable, NULL},
	{"root/venc_master/isp/contrast_enhancement/mode", NULL, &VencMaster_SetCEMode, NULL},
	{"root/venc_master/isp/contrast_enhancement/brightness", NULL, &VencMaster_SetCEBirghtness, NULL},
	{"root/venc_master/isp/contrast_enhancement/default_method", NULL, &VencMaster_SetCEDefaultMethod, NULL},
	{"root/venc_master/isp/contrast_enhancement/user_table", &VencMaster_SetCEUserTableTag, &VencMaster_SetCEUserTableCollection, &VencMaster_SetCEUserTable},
	//ImageMode
	{"root/venc_master/isp/image_adjustment/select_mode", NULL, &VencMaster_SetImageAdjustmentSelectMode, NULL},
	{"root/venc_master/isp/image_adjustment/bright", NULL, &VencMaster_SetImageAdjustmentBright, NULL},
	{"root/venc_master/isp/image_adjustment/contrast", NULL, &VencMaster_SetImageAdjustmentContrast, NULL},
	{"root/venc_master/isp/image_adjustment/saturation", NULL, &VencMaster_SetImageAdjustmentSaturation, NULL},
	{"root/venc_master/isp/image_adjustment/start_lum", NULL, &VencMaster_SetImageAdjustmentStartLum, NULL},
	{"root/venc_master/isp/image_adjustment/end_lum", NULL, &VencMaster_SetImageAdjustmentEndLum, NULL},
	{"root/venc_master/isp/image_adjustment/tolerance", NULL, &VencMaster_SetImageAdjustmentTolerance, NULL},
	{NULL, NULL, NULL}
};

TVideoMasterCfg g_tVideoMasterCfg = {
	"\0",   //resolution
	-1,     //color
	-1,     //if flip
	-1,     //if mirror
	-1,		//en autoirs 		
	-1		//en ircut
};

TWBCfg g_tWBCfg;

TColorCorrectionCfg g_tColorCorrctionCfg = {
	FALSE, // bModified
	-100.000, // RR 
	-100.000, // GR
	-100.000, // BR
	-100.000, // RG
	-100.000, // GG
	-100.000, // BG
	-100.000, // RB
	-100.000, // GB
	-100.000, // BB
	-100.000, // Offset_R
	-100.000, // Offset_G
	-100.000  // Offset_B
};
TTMCfg g_tTMCfg = {
	FALSE,	// bModified
	FALSE,	// bEnable
	-1		// dwTMLevel
};
TBlackClampCfg g_tBlackClampCfg={
	FALSE,
	-1, 
	-1,
	-1,
	-1
};

TDeImpulseCfg g_tDeImpulseCfg={
	FALSE,
	FALSE,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1
};
TCECfg g_tCECfg;					
TGammaTableCfg g_tGmaTblCfg;

TResPLCfg g_tResPLCfg = {
	FALSE,	// bModified
	FALSE,	// bResolutionModified
	FALSE,	// bFrequencyModified
	-1		// Frequency
};

TAFCfg g_tAFCfg;

TWDRCfg g_tWDRCfg;

TAECfg g_tAECfg;
TImgAdjCfg g_tImgAdjCfg = {
	FALSE, 	// bModified
	FALSE,	// bSelectModeModified
	-1,		// eImgAdjSelectMode
	FALSE,	// bBrightModified
	-1,		// dwBrightness
	FALSE,	// bContrastModified
	-1,		// dwContrast
	FALSE,	// bSaturationModified
	-1,		// dwSaturation
	-1,		// dwStartLum
	-1,		// dwEndLum
	-1		// dwTolerance
};

#define SLEEPT 5000

#if defined(_AUTOSCENE_)

#define AUTOSCENE_PROCESS_NAME "autoscene"
typedef  struct process_status
{
	BOOL isExist;
	int pid;
}TProcessStatus;

static SCODE create_tmpfile(char *szTmpfileName)
{

  int fd;
  int len = -1;
  
  if(szTmpfileName==NULL)
  {
      return S_FAIL;
  }
  
  strcpy(szTmpfileName,"/tmp/checkRTSPS-XXXXXX"); 

  umask(0077);
  fd = mkstemp(szTmpfileName);
  
  len = strlen(szTmpfileName);
  szTmpfileName[len+1]='\0';
  
  close(fd);
  
  return S_OK;
 
}

static void checkProcessStaus(TProcessStatus *ptStatus, char *szProcessName)
{
   char  szCommand[128];
   char szTmpFileName[64];
   struct stat stFile;
   FILE *fp = NULL;
   int pid = -1;
   int ret = -1;
   
   ptStatus->isExist = FALSE;
   ptStatus->pid = -1;
   
   if (create_tmpfile(szTmpFileName) != S_OK) return;
   
   ret =snprintf(szCommand,sizeof(szCommand),"/bin/pidof  %s  > %s",szProcessName,szTmpFileName);
   if (ret < 0)
   {
	 goto exit;
   }
   szCommand[ret+1]='\0';
   //printf("szTmpFileName=(%s),szCommand=(%s)\n",szTmpFileName,szCommand);
   system(szCommand);
   stat(szTmpFileName,&stFile);
   if (stFile.st_size !=0)
   {
		fp=fopen(szTmpFileName,"r");
		if(fp != NULL)
		{
			 fscanf(fp,"%d",&pid);
			 ptStatus->isExist = TRUE;
			 ptStatus->pid = pid;
		}
   }
   
exit:   
   if(fp!=NULL)
   {
     fclose(fp);
   }
   
   unlink(szTmpFileName);
}

BOOL isAutoSceneExist(void)
{
	BOOL isExist = FALSE;
	TProcessStatus tStatus;
	
	checkProcessStaus(&tStatus, AUTOSCENE_PROCESS_NAME);
	isExist = tStatus.isExist;
	
	return isExist;
}

#endif
/* ============================================================================================= */
SCODE VencMaster_InitConfigParam(TMasterInfo *ptMasterInfo)
{
	// AutoExposure relative
	DWORD	dwIndex;
	g_tAECfg.fXTranslate = -1.0;
	g_tAECfg.fYTranslate = 1.0;
	g_tAECfg.fXScale = 0.00625;
	g_tAECfg.fYScale = -0.00833333;
	g_tAECfg.bModified = TRUE;
	g_tAECfg.bGroupModified = FALSE;
	g_tAECfg.bMaxShutterModified = FALSE;
	g_tAECfg.dwMaxShutter = -1;
	g_tAECfg.bMaxGainModified = FALSE;
	g_tAECfg.dwMaxGain = -1;
	g_tAECfg.bMinShutterModified = FALSE;
	g_tAECfg.dwMinShutter = -1;
	g_tAECfg.bMinGainModified = FALSE;
	g_tAECfg.dwMinGain = -1;
	g_tAECfg.bTargetLuminanceModified = FALSE;
	g_tAECfg.dwTargetLuminance = -1;
	g_tAECfg.dwTargetOffset = -1;
	g_tAECfg.bMeasureWindowModified = FALSE;
	g_tAECfg.dwMode = -1; //0:auto,1:backlight,2:customized, default mode is customized.
	g_tAECfg.bModeModified = FALSE;

	for (dwIndex = 0; dwIndex < APP_AE_WINDOW_NUM; dwIndex++) {
		g_tAECfg.tAEWindow[dwIndex].dwStartX = -1;
		g_tAECfg.tAEWindow[dwIndex].dwStartY = -1;
		g_tAECfg.tAEWindow[dwIndex].dwWidth = -1;
		g_tAECfg.tAEWindow[dwIndex].dwHeight = -1;
		g_tAECfg.tAEWindow[dwIndex].dwWeight = -1;
	}

	g_tAECfg.bGridRangeModified = FALSE;
	g_tAECfg.tAEGridRange.dwStartX = -1;
	g_tAECfg.tAEGridRange.dwStartY = -1;
	g_tAECfg.tAEGridRange.dwWidth = -1;
	g_tAECfg.tAEGridRange.dwHeight = -1;
	g_tAECfg.bGridModified = FALSE;
	g_tAECfg.dwCurGridID = -1;

	for (dwIndex = 0; dwIndex < MAX_AE_GRID_NUM; dwIndex++) {
		g_tAECfg.atAEGrids[dwIndex].bSelected = FALSE;
		g_tAECfg.atAEGrids[dwIndex].dwWeight = -1 ;

	}
	//	White Balance relative
	g_tWBCfg.bModified = TRUE;
	g_tWBCfg.eWBSelectMode = wbmNum;
	g_tWBCfg.eWBCurModeId = wbmNum;
	g_tWBCfg.bGridModified = TRUE;
	g_tWBCfg.dwCurGridID = -1;
	g_tWBCfg.bFullFrameGridSwitch = FALSE;
	g_tWBCfg.bFullFrameGrid = FALSE;
	for (dwIndex = 0; dwIndex < wbmNum; dwIndex++) {
		g_tWBCfg.aWBTypeValue[dwIndex].dwRedGain = -1;
		g_tWBCfg.aWBTypeValue[dwIndex].dwBlueGain = -1;
		g_tWBCfg.aWBTypeValue[dwIndex].dwGreenGain = -1;
	}

	for (dwIndex = 0; dwIndex < MAX_WB_GRID_NUM ; dwIndex++) {
		g_tWBCfg.atWBGrids[dwIndex].bSelected = FALSE;
		g_tWBCfg.atWBGrids[dwIndex].dwWeight = -1 ;
	}

	// contrast enhancement relative
	g_tCECfg.bModified = TRUE;
	g_tCECfg.bEnable = FALSE;
	g_tCECfg.dwCEMode = -1;
	g_tCECfg.dwCEBrightness = -1;
	g_tCECfg.dwDefaultMethod = -1;
	for (dwIndex = 0; dwIndex < 256; dwIndex++) {
		g_tCECfg.abyCEUserDefineTbl[dwIndex] = 0;
	}
	g_tCECfg.dwPosition = -1;

	// gamma table relative
	for (dwIndex = 0; dwIndex < GAMMA_TABLE_SIZE ; dwIndex++) {
		g_tGmaTblCfg.abyGammaTbl[dwIndex] = 0;
	}

	g_tAFCfg.dwValue = -1;
	g_tAFCfg.dwSpeed = -1;
	g_tAFCfg.dwStartX = -1;
	g_tAFCfg.dwStartY = -1;
	g_tAFCfg.dwWidth = -1;
	g_tAFCfg.dwHeight = -1;
	g_tAFCfg.bModified = TRUE;
	g_tAFCfg.bValueModified = FALSE;
	g_tAFCfg.bSpeedModified = FALSE;
	g_tAFCfg.bWindowModified = FALSE;
	g_tAFCfg.dwZoomValue = -1;
	g_tAFCfg.bZoomValueModified = FALSE;
	g_tAFCfg.dwZoomSpeed = -1;
	g_tAFCfg.bZoomSpeedModified = FALSE;
	g_tAFCfg.dwNoiseThreshold = -1;
	g_tAFCfg.bNoiseThresholdModified = FALSE;
	g_tAFCfg.bReFocus = FALSE;
	g_tAFCfg.bReFocusModified = FALSE;
	g_tAFCfg.fViewXTranslate = 0.0;
	g_tAFCfg.fViewYTranslate = 0.0;
	g_tAFCfg.fViewXScale = 0.0;
	g_tAFCfg.fViewYScale = 0.0;
	g_tAFCfg.dwViewWidth = -1;
	g_tAFCfg.dwViewHeight = -1;
	g_tAFCfg.bViewResolutionModified = FALSE;

	memset(g_tAFCfg.szCurrentCmd, 0x0, AF_CURRENT_CMD_SIZE);

	g_tWDRCfg.bModified = TRUE;
	g_tWDRCfg.bEnable = FALSE;
	g_tWDRCfg.bEnableModified = FALSE;
	g_tWDRCfg.eLevel = wdrLevelUnkown;
	g_tWDRCfg.bLevelModified = FALSE;
	g_tWDRCfg.dwLevelValue = -1;
	g_tWDRCfg.bLevelValueModified = FALSE;

	memset(g_tVideoMasterCfg.szCapResolution, 0x0, sizeof(g_tVideoMasterCfg.szCapResolution));
	g_tVideoMasterCfg.bColor = -1;
	g_tVideoMasterCfg.bFlip = -1;
	g_tVideoMasterCfg.bMirror = -1;
	g_tVideoMasterCfg.bEnAutoIRIS = -1;
	g_tVideoMasterCfg.bEnIRCut = -1;

	//color correction
	g_tColorCorrctionCfg.bModified = TRUE;
	g_tColorCorrctionCfg.flCoeff_RR = -100.0;
	g_tColorCorrctionCfg.flCoeff_GR = -100.0;
	g_tColorCorrctionCfg.flCoeff_BR = -100.0;
	g_tColorCorrctionCfg.flCoeff_RG = -100.0;
	g_tColorCorrctionCfg.flCoeff_GG = -100.0;
    g_tColorCorrctionCfg.flCoeff_BG = -100.0;
    g_tColorCorrctionCfg.flCoeff_RB = -100.0;
	g_tColorCorrctionCfg.flCoeff_GB = -100.0;
    g_tColorCorrctionCfg.flCoeff_BB = -100.0;
    g_tColorCorrctionCfg.flOffset_R = -100.0;
    g_tColorCorrctionCfg.flOffset_G = -100.0;
	g_tColorCorrctionCfg.flOffset_B = -100.0;

	//tone mapping
	g_tTMCfg.bModified = TRUE;
	g_tTMCfg.bEnable = FALSE;
	g_tTMCfg.dwTMLevel = -1;
	
	//power line frequency
	g_tResPLCfg.bModified = TRUE;
	g_tResPLCfg.bResolutionModified = TRUE;
	g_tResPLCfg.bFrequencyModified = TRUE;
	g_tResPLCfg.dwMode = -1;
	
	//Image ajustment
	g_tImgAdjCfg.bModified = TRUE;
	g_tImgAdjCfg.bSelectModeModified = TRUE;
	g_tImgAdjCfg.eImgAdjSelectMode = imgadjmNum;
	g_tImgAdjCfg.bBrightModified = TRUE;
	g_tImgAdjCfg.dwBrightness = -1;
	g_tImgAdjCfg.bContrastModified = TRUE;
    g_tImgAdjCfg.dwContrast = -1;
	g_tImgAdjCfg.bSaturationModified = TRUE;
	g_tImgAdjCfg.dwSaturation = -1;
	g_tImgAdjCfg.dwStartLum = -1;
	g_tImgAdjCfg.dwEndLum = -1;
	g_tImgAdjCfg.dwTolerance = -1;

	//black clamp
	g_tBlackClampCfg.bModified = TRUE;
	g_tBlackClampCfg.dwBlackClampGr = -1;
	g_tBlackClampCfg.dwBlackClampR = -1;
	g_tBlackClampCfg.dwBlackClampGb = -1;
	g_tBlackClampCfg.dwBlackClampB  = -1;

	//de-impulse
	g_tDeImpulseCfg.bModified = TRUE;
	g_tDeImpulseCfg.bDeImpulseEn = FALSE;
	g_tDeImpulseCfg.dwDcBotThrd = -1;
	g_tDeImpulseCfg.dwDcTopThrd = -1;
	g_tDeImpulseCfg.dwMaxTopLevel = -1;
	g_tDeImpulseCfg.dwMaxBotLevel = -1;
	g_tDeImpulseCfg.dwMinTopLevel = -1;
	g_tDeImpulseCfg.dwMinBotLevel = -1;

	return S_OK;
}
//WDR setting
void VencMaster_SetWDREnable(HANDLE hObject, const CHAR *szValue, SDWORD len)
{
	CHAR szTemp[len+1];
	BOOL	bEnable = FALSE;
	memset(szTemp,0x0,len+1);
	snprintf(szTemp, len+1, "%s", szValue);
	if (strcmp(szTemp, "on") == 0)
		bEnable = TRUE;

	if (g_tWDRCfg.bEnable != bEnable) {
		g_tWDRCfg.bModified = TRUE;
		g_tWDRCfg.bEnable = bEnable;
		g_tWDRCfg.bEnableModified = TRUE;
	}
	return;
}
void VencMaster_SetWDRLevel(HANDLE hObject, const CHAR *szValue, SDWORD len)
{

	CHAR szTemp[len+1];
	EWDRLevel eLevel;
	memset(szTemp,0x0,len+1);
	snprintf(szTemp, len+1, "%s", szValue);
	if (strcasecmp(szTemp, "1") == 0) {
		eLevel = wdrLevel1;
	} else if (strcasecmp(szTemp, "2") == 0) {
		eLevel = wdrLevel2;
	} else if (strcasecmp(szTemp, "3") == 0) {
		eLevel = wdrLevel3;
	} else if (strcasecmp(szTemp, "customized") == 0) {
		eLevel = wdrLevelCustomized;
	} else {
		eLevel = wdrLevelUnkown;
	}

	if (g_tWDRCfg.eLevel != eLevel) {
		g_tWDRCfg.bModified = TRUE;
		g_tWDRCfg.eLevel = eLevel;
		g_tWDRCfg.bLevelModified = TRUE;
	}

	return;

}
void VencMaster_SetWDRLevelValue(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwLevelValue = 0;
	CHAR szValue[len+1];

	memset(szValue,0x0,sizeof(szValue));
	snprintf(szValue, len+1, "%s", szTemp);
	dwLevelValue = strtoul(szValue, NULL, 10);

	if (dwLevelValue != g_tWDRCfg.dwLevelValue)
	{
		g_tWDRCfg.bModified = TRUE;
		g_tWDRCfg.dwLevelValue = dwLevelValue;
		g_tWDRCfg.bLevelValueModified = TRUE;
	}

	return;
}
void VencMaster_SetAutoFocusViewResolution(HANDLE hObject, const CHAR *szResolution, SDWORD len)
{
	CHAR    *szRes;
	char    *szToken = NULL;
	DWORD   dwWidth, dwHeight;
	// TMasterInfo *ptMasterInfo = (TMasterInfo *)hObject;
	szRes = malloc(len+1);
	memset(szRes, 0, len+1);
	strncpy(szRes, szResolution, len);
	dwWidth = strtol(szRes, &szToken, 10);
	dwHeight = strtol(szToken+1, (char**) NULL, 10);

	g_tAFCfg.fViewXScale =(FLOAT)(2) / dwWidth;
	g_tAFCfg.fViewYScale = (FLOAT)(-2) / dwHeight;
	g_tAFCfg.fViewXTranslate = -1.0;
	g_tAFCfg.fViewYTranslate = 1.0;
	g_tAFCfg.dwViewWidth = dwWidth;
	g_tAFCfg.dwViewHeight = dwHeight;
	g_tAFCfg.bViewResolutionModified = TRUE;
	g_tAFCfg.bModified = TRUE;

}
void VencMaster_SetAutoCurrentCmd(HANDLE hObject, const CHAR *szCmd, SDWORD len)
{
	memset(g_tAFCfg.szCurrentCmd,0x0,AF_CURRENT_CMD_SIZE);
	strncpy(g_tAFCfg.szCurrentCmd,szCmd,len);

	return;
}
void  VencMaster_SetAutoFocusValue(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwValue = 0;
	CHAR szValue[len+1];

	memset(szValue,0x0,sizeof(szValue));
	snprintf(szValue, len+1, "%s", szTemp);
	dwValue = strtoul(szValue, NULL, 10);

	if (g_tAFCfg.dwValue != dwValue) {
		g_tAFCfg.bModified = TRUE;
		g_tAFCfg.bValueModified = TRUE;
		g_tAFCfg.dwValue = dwValue;
	}
	return;

}
void  VencMaster_SetAutoFocusZoom(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwZoomValue = 0;
	CHAR szValue[len+1];

	memset(szValue,0x0,sizeof(szValue));
	snprintf(szValue, len+1, "%s", szTemp);
	dwZoomValue = strtoul(szValue, NULL, 10);

	if (g_tAFCfg.dwZoomValue != dwZoomValue) {
		g_tAFCfg.bModified = TRUE;
		g_tAFCfg.bZoomValueModified = TRUE;
		g_tAFCfg.dwZoomValue = dwZoomValue;
	}

	return;

}
/* ============================================================================================= */
void  VencMaster_SetAutoFocusSpeed(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwSpeed = 0;

	CHAR szSpeed[len+1];

	memset(szSpeed,0x0,sizeof(szSpeed));
	snprintf(szSpeed, len+1, "%s", szTemp);
	dwSpeed = strtoul(szSpeed, NULL, 10);

	if (g_tAFCfg.dwSpeed != dwSpeed) {
		g_tAFCfg.bModified = TRUE;
		g_tAFCfg.bSpeedModified = TRUE;
		g_tAFCfg.dwSpeed = dwSpeed;
	}

	return;
}
/* ============================================================================================= */
void VencMaster_SetAutoFocusZoomSpeed(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwZoomSpeed = 0;
	CHAR szZoomSpeed[len+1];

	memset(szZoomSpeed,0x0,sizeof(szZoomSpeed));
	snprintf(szZoomSpeed, len+1, "%s", szTemp);
	dwZoomSpeed = strtoul(szZoomSpeed, NULL, 10);

	if (g_tAFCfg.dwZoomSpeed != dwZoomSpeed)
	{
		g_tAFCfg.bModified = TRUE;
		g_tAFCfg.bZoomSpeedModified = TRUE;
		g_tAFCfg.dwZoomSpeed = dwZoomSpeed;
	}
	return;
}
/* ============================================================================================= */

void VencMaster_SetAutoFocusNoiseThresHold(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwNoiseThreshold = 0;

	CHAR szthreshold[len+1];

	memset(szthreshold,0x0,sizeof(szthreshold));
	snprintf(szthreshold, len+1, "%s", szTemp);
	dwNoiseThreshold = strtoul(szthreshold, NULL, 10);
	if (g_tAFCfg.dwNoiseThreshold != dwNoiseThreshold)
	{
		g_tAFCfg.bModified = TRUE;
		g_tAFCfg. bNoiseThresholdModified = TRUE;
		g_tAFCfg.dwNoiseThreshold = dwNoiseThreshold;
	}
	return;
}
/* ============================================================================================= */
void VencMaster_SetAutoFocusReFocus(HANDLE hObject, const CHAR *szReFocus, SDWORD len)
{    
	CHAR szTemp[len+1];
	BOOL bReFocus = FALSE;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szReFocus);

	if (strcasecmp(szTemp, "on") == 0) 
	{
		bReFocus = TRUE;
	}

	if (g_tAFCfg.bReFocus != bReFocus)
	{
		g_tAFCfg.bModified = TRUE;
		g_tAFCfg.bReFocusModified = TRUE;
		g_tAFCfg.bReFocus = bReFocus;
	}
	return;
}
/* ============================================================================================= */
void  VencMaster_SetAutoFocusStartX(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwStartX = 0;
	CHAR szStartX[len+1];
	memset(szStartX,0x0,sizeof(szStartX));
	snprintf(szStartX, len+1, "%s", szTemp);
	dwStartX = strtoul(szStartX, NULL, 10);

	if (g_tAFCfg.dwStartX != dwStartX)
	{
		g_tAFCfg.bModified = TRUE;
		g_tAFCfg.bWindowModified = TRUE;
		g_tAFCfg.dwStartX = dwStartX;
	}
	return;

}
/* ============================================================================================= */
void  VencMaster_SetAutoFocusStartY(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwStartY = 0;
	CHAR szStartY[len+1];

	memset(szStartY,0x0,sizeof(szStartY));
	snprintf(szStartY, len+1, "%s", szTemp);
	dwStartY = strtoul(szStartY, NULL, 10);
	if (g_tAFCfg.dwStartY != dwStartY)
	{
		g_tAFCfg.bModified = TRUE;
		g_tAFCfg.bWindowModified = TRUE;
		g_tAFCfg.dwStartY = dwStartY;
	}
	return;

}
/* ============================================================================================= */
void  VencMaster_SetAutoFocusWidth(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{

	DWORD dwWidth = 0;
	CHAR szWidth[len+1];
	memset(szWidth,0x0,sizeof(szWidth));
	snprintf(szWidth, len+1, "%s", szTemp);
	dwWidth = strtoul(szWidth, NULL, 10);
	if (g_tAFCfg.dwWidth != dwWidth) {
		g_tAFCfg.bModified = TRUE;
		g_tAFCfg.bWindowModified = TRUE;
		g_tAFCfg.dwWidth = dwWidth;
	}

	return;
}
/* ============================================================================================= */
void  VencMaster_SetAutoFocusHeight(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwHeight = 0;
	CHAR szHeight[len+1];
	memset(szHeight,0x0,sizeof(szHeight));
	snprintf(szHeight, len+1, "%s", szTemp);
	dwHeight = strtoul(szHeight, NULL, 10);
	if (g_tAFCfg.dwHeight != dwHeight) {
		g_tAFCfg.bModified = TRUE;
		g_tAFCfg.bWindowModified = TRUE;
		g_tAFCfg.dwHeight = dwHeight;
	}
	return;

}
/* ============================================================================================= */
void VencMaster_SetCapResolution(HANDLE hObject, const CHAR *szResolution, SDWORD len)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *)hObject;
	CHAR    *szRes;
	char    *szToken = NULL;
	DWORD   dwWidth, dwHeight;

	szRes = malloc(len+1);
	memset(szRes, 0, len+1);
	strncpy(szRes, szResolution, len);
	dwWidth = strtol(szRes, &szToken, 10);
	dwHeight = strtol(szToken+1, (char**) NULL, 10);

	if (strcmp(szRes, g_tVideoMasterCfg.szCapResolution) != 0) {

		if (szToken == NULL) return;

		// it should change to dwVideoMaxWidth/dwVideoMaxHeight if it can support change resolution dyanmically.
		if (dwWidth > ptMasterInfo->_tInitNeedInfo.dwVideoCapWidth) {
			DBPRINT0("[VENC_MASTER] Capture width range is out of product spec !!\n");
			dwWidth = ptMasterInfo->_tInitNeedInfo.dwVideoCapWidth;
		}

		if (dwHeight > ptMasterInfo->_tInitNeedInfo.dwVideoCapHeight) {
			DBPRINT0("[VENC_MASTER] Capture height range is out of product spec !!\n");
			dwHeight = ptMasterInfo->_tInitNeedInfo.dwVideoCapHeight;
		}

		g_tResPLCfg.bModified = TRUE;
		g_tResPLCfg.bResolutionModified = TRUE;

		// change the normalization scale
		ptMasterInfo->tAER2D.fXScale = (FLOAT)(2) / dwWidth;
		ptMasterInfo->tAER2D.fYScale = (FLOAT)(-2) / dwHeight;
		// because the Window info should reset, set it to TRUE
		g_tAECfg.bMeasureWindowModified = TRUE;
		g_tAECfg.bModified = TRUE;

		strncpy(g_tVideoMasterCfg.szCapResolution, szRes, MAX_VIDEO_RESOLUTION_LEN+1);
		DBPRINT2("[VENC_MASTER] Capture resolution W x H: %u x %u\n", dwWidth, dwHeight);

		//The coordinate system of AE on the  view site is the same as the one on the server site.
		g_tAECfg.fXScale = (FLOAT)(2) / dwWidth;
		g_tAECfg.fYScale = (FLOAT)(-2) / dwHeight;
		g_tAECfg.fXTranslate = -1.0;
		g_tAECfg.fYTranslate = 1.0;

	}

}

/* ============================================================================================= */
void VencMaster_SetVideoCMOSMode(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwFreq = 0;
	CHAR szFreq[len+1];
#if 0
#if defined(_AUTOSCENE_)	
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	if (isAutoSceneExist() && ptMasterInfo->bAutoSceneIsRun)
	{
		printf("[venc_master_conf](%d)Because autoscene is running, does not call %s!!\n",__LINE__,__func__);
		return;
	}
#endif
#endif
	memset(szFreq,0x0,sizeof(szFreq));
	snprintf(szFreq, len+1, "%s", szTemp);
	dwFreq = strtoul(szFreq, NULL, 10);

	if (g_tResPLCfg.dwMode != dwFreq) {
		if ((dwFreq != 50) && (dwFreq != 60)) {
			return;
		}
		g_tResPLCfg.bModified = TRUE;
		g_tResPLCfg.bFrequencyModified = TRUE;
		g_tResPLCfg.dwMode = dwFreq;
	}
}

/* ============================================================================================= */
SCODE VencMaster_ResPLSetoptions(HANDLE hObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;

	char    *szToken = NULL;
	DWORD   dwWidth = 0, dwHeight = 0;

	printf("[venc_master_conf](%d)Enter VencMaster_ResPLSetoptions() \n",__LINE__);
	// 50/60Hz change
	if (g_tResPLCfg.bFrequencyModified == TRUE) {
		printf("[venc_master_conf](%d) Call VIDEO_SIGNAL_OPTION_SET_FREQUENCY SetOptions()\n",__LINE__);
		memset(&tVideoSigOptions,0x0,sizeof(TVideoSignalOptions));
		tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_FREQUENCY;
		tVideoSigOptions.adwUserData[0] = (g_tResPLCfg.dwMode == 50)? VIDEO_SIGNAL_FREQUENCY_50HZ : VIDEO_SIGNAL_FREQUENCY_60HZ;
		VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions);
		g_tResPLCfg.bFrequencyModified = FALSE;
	}
	// resolution change
	if (g_tResPLCfg.bResolutionModified == TRUE) {

		g_tResPLCfg.bResolutionModified = FALSE;
		dwWidth = strtol(g_tVideoMasterCfg.szCapResolution, &szToken, 10);
		dwHeight = strtol(szToken+1, (char**) NULL, 10);
		//1920x1080 <-> 2560x1920 OV5653?
#if defined(_MT9P031) ||  defined(_OV5653) || defined(_AR0331)
		if((dwWidth==2560 && dwHeight==1920) || (dwWidth==1920 && dwHeight==1080) || (dwWidth==2048 && dwHeight==1536))
		{    
			memset(&tVideoSigOptions,0x0,sizeof(TVideoSignalOptions));
			//printf("[venc_master_conf](%d) Call VIDEO_SIGNAL_OPTION_SET_SIZE SetOptions() ResStr=%s dwWidth=%d, dwHeight=%d  stride=%d\n",__LINE__,g_tVideoMasterCfg.szCapResolution,dwWidth, dwHeight,ptMasterInfo->_tInitNeedInfo.dwVideoMaxWidth);
			tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_SIZE;
			tVideoSigOptions.adwUserData[0] = (dwHeight << 16) | dwWidth;
			tVideoSigOptions.adwUserData[1] = ptMasterInfo->_tInitNeedInfo.dwVideoMaxWidth; // Max means stride
			VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions);
		}
#endif
	}
	return S_OK;
}

/* ============================================================================================= */
void VencMaster_SetWhiteBalanceSelectMode(HANDLE hObject, const CHAR *szWBSelectMode, SDWORD len)
{
	CHAR szTemp[len+1];
	EWBMode	eWBSelectMode;
	
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szWBSelectMode);
	
	if (strcasecmp(szTemp, "Manual") == 0) {
		eWBSelectMode = wbmManual;
	}else if (strcasecmp(szTemp, "Simple") == 0) {
		eWBSelectMode =  wbmSimple;
	} else if (strcasecmp(szTemp, "IncandescentLight") == 0) {
		eWBSelectMode = wbmIncandescentLight;
	} else if (strcasecmp(szTemp, "CoolWhiteFluorescentLight") == 0) {
		eWBSelectMode = wbmCoolWhiteFluorescentLight;
	} else if (strcasecmp(szTemp, "SunLight") == 0) {
		eWBSelectMode = wbmSunLight;
	} else if (strcasecmp(szTemp, "Cloudy") == 0) {
		eWBSelectMode = wbmCloudy;		
	} else if (strcasecmp(szTemp, "SunShade") == 0) {
		eWBSelectMode = wbmSunShade;		
	} else if (strcasecmp(szTemp, "UserDefine1") == 0) {
		eWBSelectMode = wbmUserDefine1;
	} else if (strcasecmp(szTemp, "UserDefine2") == 0) {
		eWBSelectMode = wbmUserDefine2;
	} else {
		eWBSelectMode = wbmAuto;
	}

	if (g_tWBCfg.eWBSelectMode != eWBSelectMode) {
		g_tWBCfg.eWBSelectMode = eWBSelectMode;
		g_tWBCfg.bModified = TRUE;
	}
}

/* ============================================================================================= */
SCODE VencMaster_SetWhiteBalanceModeTag(HANDLE hObject, const CHAR *szWhiteBalanceMode, const CHAR** atts)
{
	EWBMode	eWBCurModeId;
	CHAR szTemp[64];

	if (atts) {
		if (strcmp(atts[0], "id") != 0) {
			DBPRINT0("[VENC_MASTER] No id attribute \n");
			return IGNORE_CHILD_CONFIG;            
		}
		memset(szTemp,0x0,sizeof(szTemp));
		strncpy(szTemp, atts[1], strlen(atts[1]));
		if (strcasecmp(szTemp, "Manual") == 0) {
			eWBCurModeId = wbmManual;
		} else if (strcasecmp(szTemp, "Simple") == 0) {
			eWBCurModeId = wbmSimple;
		} else if (strcasecmp(szTemp, "IncandescentLight") == 0) {
			eWBCurModeId = wbmIncandescentLight;
		} else if (strcasecmp(szTemp, "CoolWhiteFluorescentLight") == 0) {
			eWBCurModeId = wbmCoolWhiteFluorescentLight;
		} else if (strcasecmp(szTemp, "SunLight") == 0) {
			eWBCurModeId = wbmSunLight;
		} else if (strcasecmp(szTemp, "Cloudy") == 0) {
			eWBCurModeId = wbmCloudy;		
		} else if (strcasecmp(szTemp, "SunShade") == 0) {
			eWBCurModeId = wbmSunShade;		
		} else if (strcasecmp(szTemp, "UserDefine1") == 0) {
			eWBCurModeId = wbmUserDefine1;
		} else if (strcasecmp(szTemp, "UserDefine2") == 0) {
			eWBCurModeId = wbmUserDefine2;
		} else {
			eWBCurModeId = wbmAuto;
		}

		if (eWBCurModeId < wbmAuto || eWBCurModeId >= wbmNum)
			return IGNORE_CHILD_CONFIG;
		g_tWBCfg.eWBCurModeId = eWBCurModeId;
		return S_OK;
	}
	DBPRINT0("[VENC_MASTER] No attribute \n");
	return IGNORE_CHILD_CONFIG;
}

/* ============================================================================================= */
void VencMaster_SetWhiteBalanceRedGain(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwRedGain = 0;

	CHAR szRedGain[len+1];
	EWBMode	eWBCurModeId = g_tWBCfg.eWBCurModeId;
	memset(szRedGain,0x0,sizeof(szRedGain));
	snprintf(szRedGain, len+1, "%s", szTemp);
	dwRedGain = strtoul(szRedGain, NULL, 10);
	if (eWBCurModeId == wbmAuto)	// auto needn't set R/G gain
		return;

	if (dwRedGain > WB_MAX_REDGAIN)
		dwRedGain = WB_MAX_REDGAIN;
	else if (dwRedGain < WB_MIN_REDGAIN)
		dwRedGain = WB_MIN_REDGAIN;

	if (g_tWBCfg.aWBTypeValue[eWBCurModeId].dwRedGain != dwRedGain) {
		g_tWBCfg.aWBTypeValue[eWBCurModeId].dwRedGain = dwRedGain;
		g_tWBCfg.bModified = TRUE;
		DBPRINT2("[VENC_MASTER] WhiteBalance %d dwRedGain Change to %u \n", eWBCurModeId, g_tWBCfg.aWBTypeValue[eWBCurModeId].dwRedGain);
	}
}

/* ============================================================================================= */
void VencMaster_SetWhiteBalanceBlueGain(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwBlueGain = 0;

	CHAR szBlueGain[len+1];
	EWBMode	eWBCurModeId = g_tWBCfg.eWBCurModeId;
	memset(szBlueGain,0x0,sizeof(szBlueGain));
	snprintf(szBlueGain, len+1, "%s", szTemp);
	dwBlueGain = strtoul(szBlueGain, NULL, 10);
	if (eWBCurModeId == wbmAuto)	// auto needn't set R/G gain
		return;

	if (dwBlueGain > WB_MAX_BLUEGAIN)
		dwBlueGain = WB_MAX_BLUEGAIN;
	else if (dwBlueGain < WB_MIN_BLUEGAIN)
		dwBlueGain = WB_MIN_BLUEGAIN;

	if (g_tWBCfg.aWBTypeValue[eWBCurModeId].dwBlueGain != dwBlueGain) {
		g_tWBCfg.aWBTypeValue[eWBCurModeId].dwBlueGain = dwBlueGain;
		g_tWBCfg.bModified = TRUE;
		DBPRINT2("[VENC_MASTER] WhiteBalance %d dwBlueGain Change to %u \n", eWBCurModeId, g_tWBCfg.aWBTypeValue[eWBCurModeId].dwBlueGain);
	}
}

/* ============================================================================================= */
void VencMaster_SetWhiteBalanceGreenGain(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwGreenGain = 0;

	CHAR szGreenGain[len+1];

	EWBMode	eWBCurModeId = g_tWBCfg.eWBCurModeId;
	memset(szGreenGain,0x0,sizeof(szGreenGain));
	snprintf(szGreenGain, len+1, "%s", szTemp);
	dwGreenGain = strtoul(szGreenGain, NULL, 10);
	
	if (eWBCurModeId == wbmAuto)	// auto needn't set R/G gain
		return;

	if (dwGreenGain > WB_MAX_GREENGAIN)
		dwGreenGain = WB_MAX_GREENGAIN;
	else if (dwGreenGain < WB_MIN_GREENGAIN)
		dwGreenGain = WB_MIN_GREENGAIN;

	if (g_tWBCfg.aWBTypeValue[eWBCurModeId].dwGreenGain != dwGreenGain) {
		g_tWBCfg.aWBTypeValue[eWBCurModeId].dwGreenGain = dwGreenGain;
		g_tWBCfg.bModified = TRUE;
		DBPRINT2("[VENC_MASTER] WhiteBalance %d dwGreenGain Change to %u \n", eWBCurModeId, g_tWBCfg.aWBTypeValue[eWBCurModeId].dwGreenGain);
	}
}


SCODE  VencMaster_SetWBGridTag(HANDLE hObject, const CHAR *szElement, const CHAR** atts)
{
	DWORD  dwCurWinId;

	if (atts) {
		if (strcmp(atts[0], "id") != 0) {
			printf("[VENC_MASTER](%d) No grid id attribute \n",__LINE__);
			return IGNORE_CHILD_CONFIG;            
		}
		dwCurWinId = strtoul(atts[1], (char**)NULL, 10);
		if (dwCurWinId < 0 || dwCurWinId >= MAX_WB_GRID_NUM)
			return IGNORE_CHILD_CONFIG;
		g_tWBCfg.dwCurGridID = dwCurWinId;
		return S_OK;
	}
	printf("[VENC_MASTER](%d) No attributes\n",__LINE__);
	return IGNORE_CHILD_CONFIG;
}
void VencMaster_SetWBGridSelected(HANDLE hObject, const CHAR *seElement, SDWORD len)
{
	DWORD dwCurWinId =  g_tWBCfg.dwCurGridID;
	CHAR szSelected[len+1];
	BOOL bSelected = FALSE;
	memset(szSelected,0x0,sizeof(szSelected));
	snprintf(szSelected, len+1, "%s", seElement);

	if (strncmp(szSelected,"true",strlen(szSelected)) ==0)
	{
		bSelected = TRUE;
	}
	if (g_tWBCfg.atWBGrids[dwCurWinId].bSelected != bSelected) {
		g_tWBCfg.atWBGrids[dwCurWinId].bSelected = bSelected;
		g_tWBCfg.bGridModified = TRUE;
		g_tWBCfg.bModified = TRUE;
	}
	return;
}
void VencMaster_SetWBGridWeight(HANDLE hObject, const CHAR *seElement, SDWORD len)
{
	DWORD dwCurWinId =  g_tWBCfg.dwCurGridID;
	CHAR szWeight[len+1];
	DWORD dwWeight = 0;
	memset(szWeight,0x0,sizeof(szWeight));
	snprintf(szWeight, len+1, "%s", seElement);
	dwWeight = strtoul(szWeight, NULL, 10);
	
	if (g_tWBCfg.atWBGrids[dwCurWinId].dwWeight != dwWeight) {
		g_tWBCfg.atWBGrids[dwCurWinId].dwWeight = dwWeight;
		g_tWBCfg.bGridModified = TRUE;
		g_tWBCfg.bModified = TRUE;
	}
	return;
}
void VencMaster_SetWhiteBalanceFullFrameGrid(HANDLE hObject, const CHAR *seElement, SDWORD len)
{

	CHAR szSelected[len+1];
	BOOL bSelected = FALSE;
	memset(szSelected,0x0,sizeof(szSelected));
	snprintf(szSelected, len+1, "%s", seElement);
	if (strncmp(szSelected,"true",strlen(szSelected)) ==0)
	{
		bSelected = TRUE;
	}
	if (g_tWBCfg.bFullFrameGrid != bSelected) {
		g_tWBCfg.bFullFrameGrid  =bSelected;
		g_tWBCfg.bModified = TRUE;
		g_tWBCfg.bFullFrameGridSwitch = TRUE;
	}
	printf("[venc_master_conf](%d)g_tWBCfg.bFullFrameGrid=%d szSelected=%s\n",__LINE__,g_tWBCfg.bFullFrameGrid,  szSelected);

	return;
}
/* ============================================================================================= */
SCODE VencMaster_WBSetoptions(HANDLE hObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;
	EWBMode	eWBSelectMode = g_tWBCfg.eWBSelectMode;
	DWORD dwGain = 0;
	DWORD dwAWBRedGain = 0;
	DWORD dwAWBBlueGain = 0;
	DWORD dwAWBGreenGain = 0;

	DWORD adwAWBWeight[MAX_WB_GRID_NUM];
	DWORD dwIndex = 0;
	DWORD dwModeValue = 0;
	
	dwAWBRedGain = g_tWBCfg.aWBTypeValue[eWBSelectMode].dwRedGain;
	dwAWBBlueGain = g_tWBCfg.aWBTypeValue[eWBSelectMode].dwBlueGain;
	dwAWBGreenGain = g_tWBCfg.aWBTypeValue[eWBSelectMode].dwGreenGain;	    
	
	memset(&tVideoSigOptions,0x0,sizeof(TVideoSignalOptions));
	dwGain=(dwAWBGreenGain << 8)|dwAWBRedGain;
	tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_AUTO_WHITE_BALANCE_CTRL;
	//auto:0 simple:1 manual:2
	if (eWBSelectMode == wbmManual)
	{  
		dwModeValue = 2;
	}
	else if (eWBSelectMode == wbmAuto)
	{ 
		dwModeValue = 0;
	}
	else if (eWBSelectMode ==  wbmSimple)  
	{ 
		dwModeValue = 1;
	}
	else
	{
		dwModeValue = 0;
	}
	tVideoSigOptions.adwUserData[0] = dwModeValue;
	tVideoSigOptions.adwUserData[1] = (eWBSelectMode == wbmAuto)? 0 :dwAWBRedGain;// dwGain;
	tVideoSigOptions.adwUserData[2] = (eWBSelectMode == wbmAuto)? 0 :dwAWBBlueGain;

	if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) == S_OK) {
		//printf("VencMaster_WBSetoptions successful \n");
		//printf("[venc_master_conf](%d)(WhiteBalance, RedGain, BlueGain ,GreenGain) = (%d, %u, %u,%u) \n",__LINE__,dwModeValue, dwAWBRedGain, dwAWBBlueGain,dwAWBGreenGain);

	} else {
		printf("VencMaster_WBSetoptions fail(%d) \n",__LINE__);

	}

	if ((g_tWBCfg.bFullFrameGridSwitch==TRUE) || (g_tWBCfg.bGridModified==TRUE))
	{
		if (g_tWBCfg.bFullFrameGridSwitch == TRUE)
			g_tWBCfg.bFullFrameGridSwitch = FALSE;
		else if (g_tWBCfg.bGridModified == TRUE)
			g_tWBCfg.bGridModified = FALSE;

		memset(&tVideoSigOptions,0x0,sizeof(TVideoSignalOptions));
		tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_AUTO_WHITE_BALANCE_WINDOW_PRIORITY;

		memset(adwAWBWeight,0x0,sizeof(DWORD)*MAX_WB_GRID_NUM );

		if (g_tWBCfg.bFullFrameGrid == TRUE)
		{
			for(dwIndex = 0; dwIndex < MAX_WB_GRID_NUM; dwIndex++)
			{
				adwAWBWeight[dwIndex]=1;
			}
		}
		else
		{
			for (dwIndex = 0; dwIndex < MAX_WB_GRID_NUM; dwIndex++) 
			{
				if (g_tWBCfg.atWBGrids[dwIndex].bSelected == TRUE)
				{
					adwAWBWeight[dwIndex]=g_tWBCfg.atWBGrids[dwIndex].dwWeight;
				}
				else
				{
					adwAWBWeight[dwIndex]=0;
				}
			}
		}
		
		tVideoSigOptions.adwUserData[0] = (DWORD)(&(adwAWBWeight[0]));
		
		if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) == S_OK) {
			//printf("[%d]VencMaster_WBSetoptions successful \n",__LINE__);
		} else {
			printf("VencMaster_WBSetoptions fail(%d) \n",__LINE__);
		}
#if 0
		printf("[VENC_MASTER_CONF](%d)WB Grid weight array (size:%d):\n",__LINE__,MAX_WB_GRID_NUM);
		debug_printf("======================\n");
		for(dwIndex = 0; dwIndex < MAX_WB_GRID_NUM; dwIndex++)
		{
			debug_printf("(%d,%u) ",dwIndex,adwAWBWeight[dwIndex]);
		}
		debug_printf("\n====================\n");
#endif
	}
	return S_OK;
}

/* ============================================================================================= */
void VencMaster_SetAEXTranslate(HANDLE hObject, const CHAR *szAEXTranslate, SDWORD len)
{
	CHAR szTemp[len+1];
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szAEXTranslate);
	//g_tAECfg.fXTranslate = atof(szTemp);
	DBPRINT1("[VENC_MASTER] XTranslate %.8f \n", g_tAECfg.fXTranslate);
}

/* ============================================================================================= */
void VencMaster_SetAEYTranslate(HANDLE hObject, const CHAR *szAEYTranslate, SDWORD len)
{
	CHAR szTemp[len+1];
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szAEYTranslate);
	// g_tAECfg.fYTranslate = atof(szTemp);
	DBPRINT1("[VENC_MASTER] YTranslate %.8f \n", g_tAECfg.fYTranslate);	
}

/* ============================================================================================= */
void VencMaster_SetAEXScale(HANDLE hObject, const CHAR *szAEXScale, SDWORD len)
{
	CHAR szTemp[len+1];
	snprintf(szTemp, len+1, "%s", szAEXScale);
	// g_tAECfg.fXScale = atof(szTemp);
	DBPRINT1("[VENC_MASTER] XScale %.8f \n", g_tAECfg.fXScale);
}

/* ============================================================================================= */
void VencMaster_SetAEYScale(HANDLE hObject, const CHAR *szAEYScale, SDWORD len)
{
	CHAR szTemp[len+1];
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szAEYScale);
	//g_tAECfg.fYScale = atof(szTemp);
	DBPRINT1("[VENC_MASTER] YScale %.8f \n", g_tAECfg.fYScale);
}
/*===============================================================================================*/
void VencMaster_SetAEMode(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwMode = 0;
	CHAR szMode[len+1];

	memset(szMode,0x0,sizeof(szMode));
	snprintf(szMode, len+1, "%s", szTemp);
	dwMode = strtoul(szMode, NULL, 10);
	//printf("[venc_master_conf](%d)szMode=%s\n",__LINE__,szMode);
	//printf("[venc_master_conf](%d)g_tAECfg.dwMode=%d  dwMode=%d\n",__LINE__,g_tAECfg.dwMode,dwMode);
	if (dwMode > 2)//0:auto,1:backlight,2:customized
	{
		printf("[venc_master_conf](%d)AE Mode: %ul is not supported!\n",__LINE__,dwMode);
		return;
	}
	if (g_tAECfg.dwMode != dwMode) {
		g_tAECfg.dwMode = dwMode;
		g_tAECfg.bModeModified = TRUE;
		g_tAECfg.bGroupModified = TRUE;
		g_tAECfg.bModified = TRUE;
		//printf("[venc_master_conf](%d)AE Mode=%u g_tAECfg.dwMode=%d\n",__LINE__,dwMode,g_tAECfg.dwMode);
	}

	return;
}
/* ============================================================================================= */
void VencMaster_SetAEMaxShutter(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwMaxShutter = 0;
	CHAR szMaxShutter[len+1];

	memset(szMaxShutter,0x0,sizeof(szMaxShutter));
	snprintf(szMaxShutter, len+1, "%s", szTemp);
	dwMaxShutter = strtoul(szMaxShutter, NULL, 10);
	if (g_tAECfg.dwMaxShutter != dwMaxShutter) {
		g_tAECfg.dwMaxShutter = dwMaxShutter;
		g_tAECfg.bMaxShutterModified = TRUE;
		g_tAECfg.bGroupModified = TRUE;
		g_tAECfg.bModified = TRUE;
		//printf("[venc_master_conf](%d)MaxShutter has been changed as: %u \n",__LINE__,g_tAECfg.dwMaxShutter);
	}
}

/* ============================================================================================= */
void VencMaster_SetAEMaxGain(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwMaxGain = 0;

	CHAR szMaxGain[len+1];
	memset(szMaxGain,0x0,sizeof(szMaxGain));
	snprintf(szMaxGain, len+1, "%s", szTemp);
	dwMaxGain = strtoul(szMaxGain, NULL, 10);

	if (g_tAECfg.dwMaxGain != dwMaxGain) {
		g_tAECfg.dwMaxGain = dwMaxGain;
		g_tAECfg.bMaxGainModified = TRUE;
		g_tAECfg.bGroupModified = TRUE;
		g_tAECfg.bModified = TRUE;
		//printf("[venc_master_conf](%d)MaxGain has been changed as: %u \n",__LINE__,g_tAECfg.dwMaxGain);
	}
}

/* ============================================================================================= */
void VencMaster_SetAEMinShutter(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwMinShutter = 0;
	CHAR szMinShutter[len+1];

	memset( szMinShutter,0x0,sizeof( szMinShutter));
	snprintf(szMinShutter, len+1, "%s", szTemp);
	dwMinShutter = strtoul(szMinShutter, NULL, 10);
	
	if (g_tAECfg.dwMinShutter != dwMinShutter) {
		g_tAECfg.dwMinShutter = dwMinShutter;
		g_tAECfg.bMinShutterModified = TRUE;
		g_tAECfg.bGroupModified = TRUE;
		g_tAECfg.bModified = TRUE;
		//printf("[venc_master_conf](%d)MinShutter has been changed as: %u \n",__LINE__,g_tAECfg.dwMinShutter);
	}
}

/* ============================================================================================= */
void VencMaster_SetAEMinGain(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwMinGain = 0;

	CHAR szMinGain[len+1];
	memset(szMinGain,0x0,sizeof(szMinGain));
	snprintf(szMinGain, len+1, "%s", szTemp);
	dwMinGain = strtoul(szMinGain, NULL, 10);

	if (g_tAECfg.dwMinGain != dwMinGain) {
		g_tAECfg.dwMinGain = dwMinGain;
		g_tAECfg.bMinGainModified = TRUE;
		g_tAECfg.bGroupModified = TRUE;
		g_tAECfg.bModified = TRUE;
		//printf("[venc_master_conf](%d)MinGain has been changed as: %u \n",__LINE__,g_tAECfg.dwMinGain);
	}
}

/* ============================================================================================= */
void VencMaster_SetAETargetLuminance(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{

	DWORD dwTargetLuminance = 0;

	CHAR szTargetLuminance[len+1];
	memset(szTargetLuminance,0x0,sizeof(szTargetLuminance));
	snprintf(szTargetLuminance, len+1, "%s", szTemp);
	dwTargetLuminance = strtoul(szTargetLuminance, NULL, 10);

	if (g_tAECfg.dwTargetLuminance != dwTargetLuminance) {
		g_tAECfg.dwTargetLuminance = dwTargetLuminance;
		g_tAECfg.bTargetLuminanceModified = TRUE;
		g_tAECfg.bGroupModified = TRUE;
		g_tAECfg.bModified = TRUE;
		//printf("[venc_master_conf](%d)TargetLuminance  has been changed as: %u \n",__LINE__,g_tAECfg.dwTargetLuminance);
	}
}

/* ============================================================================================= */
void VencMaster_SetAETargetOffset(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwTargetOffset = 0;
	CHAR szTargetOffset[len+1];
	memset(szTargetOffset,0x0,sizeof(szTargetOffset));
	snprintf(szTargetOffset, len+1, "%s", szTemp);
	dwTargetOffset = strtoul(szTargetOffset, NULL, 10);
	if (g_tAECfg.dwTargetOffset != dwTargetOffset) {
		g_tAECfg.dwTargetOffset = dwTargetOffset;
		g_tAECfg.bTargetLuminanceModified = TRUE;
		g_tAECfg.bGroupModified = TRUE;
		g_tAECfg.bModified = TRUE;
	}
}

/* ============================================================================================= */
SCODE VencMaster_SetAEWinTag(HANDLE hObject, const CHAR *szAEWin, const CHAR** atts)
{
	DWORD  dwCurWinId;

	if (atts) {
		if (strcmp(atts[0], "id") != 0) {
			DBPRINT0("[VENC_MASTER] No id attribute \n");
			return IGNORE_CHILD_CONFIG;            
		}
		dwCurWinId = strtoul(atts[1], (char**)NULL, 10);
		if (dwCurWinId < 0 || dwCurWinId >= APP_AE_WINDOW_NUM)
			return IGNORE_CHILD_CONFIG;
		g_tAECfg.dwCurWinId = dwCurWinId;
		return S_OK;
	}
	DBPRINT0("[VENC_MASTER] No attribute \n");
	return IGNORE_CHILD_CONFIG;
}

/* ============================================================================================= */
void VencMaster_SetAEWinStartXPos(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwStartX  = 0;
	DWORD dwCurWinId = g_tAECfg.dwCurWinId;
	CHAR szStartX[len+1];
	memset(szStartX,0x0,sizeof(szStartX));
	snprintf(szStartX, len+1, "%s", szTemp);
	dwStartX  = strtoul(szStartX, NULL, 10);
	
	if (g_tAECfg.tAEWindow[dwCurWinId].dwStartX != dwStartX) {
		g_tAECfg.tAEWindow[dwCurWinId].dwStartX = dwStartX;
		g_tAECfg.bMeasureWindowModified = TRUE;
		g_tAECfg.bModified = TRUE;
		//printf("[venc_master_conf](%d)AE widow %u StartX Change to %u\n",__LINE__,dwCurWinId, g_tAECfg.tAEWindow[dwCurWinId].dwStartX);
		//DBPRINT2("[VENC_MASTER] AE widow %u StartX Change to %u \n", dwCurWinId, g_tAECfg.tAEWindow[dwCurWinId].dwStartX);
	}
}

/* ============================================================================================= */
void VencMaster_SetAEWinStartYPos(HANDLE hObject,const CHAR *szTemp, SDWORD len)
{

	DWORD dwStartY  = 0;
	DWORD dwCurWinId = g_tAECfg.dwCurWinId;
	CHAR szStartY[len+1];
	memset(szStartY,0x0,sizeof(szStartY));
	snprintf(szStartY, len+1, "%s", szTemp);
	dwStartY  = strtoul(szStartY, NULL, 10);
	
	if (g_tAECfg.tAEWindow[dwCurWinId].dwStartY != dwStartY) {
		g_tAECfg.tAEWindow[dwCurWinId].dwStartY = dwStartY;
		g_tAECfg.bMeasureWindowModified = TRUE;
		g_tAECfg.bModified = TRUE;
		//printf("[venc_master_conf](%d)AE widow %u StartY Change to %u\n",__LINE__,dwCurWinId, g_tAECfg.tAEWindow[dwCurWinId].dwStartY);
		//DBPRINT2("[VENC_MASTER] AE widow %u StartY Change to %u \n", dwCurWinId, g_tAECfg.tAEWindow[dwCurWinId].dwStartY);
	}
}

/* ============================================================================================= */
void VencMaster_SetAEWinWidth(HANDLE hObject, const CHAR * szTemp, SDWORD len)
{  
	DWORD dwWidth  = 0;
	DWORD dwCurWinId = g_tAECfg.dwCurWinId;
	CHAR szWidth[len+1];
	memset(szWidth,0x0,sizeof(szWidth));
	snprintf(szWidth, len+1, "%s", szTemp);
	dwWidth  = strtoul(szWidth, NULL, 10);

	if (g_tAECfg.tAEWindow[dwCurWinId].dwWidth != dwWidth) {
		g_tAECfg.tAEWindow[dwCurWinId].dwWidth = dwWidth;
		g_tAECfg.bMeasureWindowModified = TRUE;
		g_tAECfg.bModified = TRUE;
		//printf("[venc_master_conf](%d)AE widow %u dwWidth Change to %u \n",__LINE__,dwCurWinId, g_tAECfg.tAEWindow[dwCurWinId].dwWidth);
		// DBPRINT2("[VENC_MASTER] AE widow %u dwWidth Change to %u \n", dwCurWinId, g_tAECfg.tAEWindow[dwCurWinId].dwWidth);
	}
}

/* ============================================================================================= */
void VencMaster_SetAEWinHeight(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwHeight  = 0;
	DWORD dwCurWinId = g_tAECfg.dwCurWinId;
	CHAR szHeight[len+1];
	memset(szHeight,0x0,sizeof(szHeight));
	snprintf(szHeight, len+1, "%s", szTemp);
	dwHeight = strtoul(szHeight, NULL, 10);

	if (g_tAECfg.tAEWindow[dwCurWinId].dwHeight != dwHeight) {
		g_tAECfg.tAEWindow[dwCurWinId].dwHeight = dwHeight;
		g_tAECfg.bMeasureWindowModified = TRUE;
		g_tAECfg.bModified = TRUE;
		//printf("[venc_master_conf](%d) AE widow %u dwHeight Change to %u \n",__LINE__ ,dwCurWinId, g_tAECfg.tAEWindow[dwCurWinId].dwHeight);
		// DBPRINT2("[VENC_MASTER] AE widow %u dwHeight Change to %u \n", dwCurWinId, g_tAECfg.tAEWindow[dwCurWinId].dwHeight);
	}
}

/* ============================================================================================= */
void VencMaster_SetAEWinWeight(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwWeight  = 0;
	DWORD dwCurWinId = g_tAECfg.dwCurWinId;
	CHAR szWeight[len+1];
	memset(szWeight,0x0,sizeof(szWeight));
	snprintf(szWeight, len+1, "%s", szTemp);
	dwWeight = strtoul(szWeight, NULL, 10);

	if (g_tAECfg.tAEWindow[dwCurWinId].dwWeight != dwWeight) {
		g_tAECfg.tAEWindow[dwCurWinId].dwWeight = dwWeight;
		g_tAECfg.bMeasureWindowModified = TRUE;
		g_tAECfg.bModified = TRUE;
		//printf("[venc_master_conf](%d) AE widow %u dwWeight Change to %u \n",__LINE__ ,dwCurWinId, g_tAECfg.tAEWindow[dwCurWinId].dwWeight);
		//DBPRINT2("[VENC_MASTER] AE widow %u dwWeight Change to %u \n", dwCurWinId, g_tAECfg.tAEWindow[dwCurWinId].dwWeight);
	}
}
/* ============================================================================================= */
void VencMaster_SetAEGridRangeStartXPos(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{   

	DWORD dwStartX = 0;
	CHAR szStartX[len+1];
	memset(szStartX,0x0,sizeof(szStartX));
	snprintf(szStartX, len+1, "%s", szTemp);
	dwStartX = strtoul(szStartX, NULL, 10);
	if (g_tAECfg.tAEGridRange.dwStartX != dwStartX) {
		g_tAECfg.tAEGridRange.dwStartX = dwStartX;
		g_tAECfg.bGridRangeModified = TRUE;
		g_tAECfg.bModified = TRUE;
	}
	return;
}
void VencMaster_SetAEGridRangeStartYPos(HANDLE hObject, const CHAR * szTemp, SDWORD len)
{
	DWORD dwStartY = 0;
	CHAR szStartY[len+1];
	memset(szStartY,0x0,sizeof(szStartY));
	snprintf(szStartY, len+1, "%s", szTemp);
	dwStartY = strtoul(szStartY, NULL, 10);
	if (g_tAECfg.tAEGridRange.dwStartY != dwStartY) {
		g_tAECfg.tAEGridRange.dwStartY = dwStartY;
		g_tAECfg.bGridRangeModified = TRUE;
		g_tAECfg.bModified = TRUE;
	}
	return;
}
void VencMaster_SetAEGridRangeWidth(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwWidth = 0;
	CHAR szWidth[len+1];
	memset(szWidth,0x0,sizeof(szWidth));
	snprintf(szWidth, len+1, "%s", szTemp);
	dwWidth = strtoul(szWidth, NULL, 10);
	if (g_tAECfg.tAEGridRange.dwWidth != dwWidth) {
		g_tAECfg.tAEGridRange.dwWidth = dwWidth;
		g_tAECfg.bGridRangeModified = TRUE;
		g_tAECfg.bModified = TRUE;
	}
	return;
}
void VencMaster_SetAEGridRangeHeight(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwHeight = 0;
	CHAR szHeight[len+1];
	memset(szHeight,0x0,sizeof(szHeight));
	snprintf(szHeight, len+1, "%s", szTemp);
	dwHeight = strtoul(szHeight, NULL, 10);
	if (g_tAECfg.tAEGridRange.dwHeight != dwHeight) {
		g_tAECfg.tAEGridRange.dwHeight = dwHeight;
		g_tAECfg.bGridRangeModified = TRUE;
		g_tAECfg.bModified = TRUE;
	}
	return;
}
SCODE VencMaster_SetAEGridTag(HANDLE hObject, const CHAR *szAEWin, const CHAR** atts)
{

	DWORD  dwCurWinId;

	if (atts) {
		if (strcmp(atts[0], "id") != 0) {
			printf("[VENC_MASTER](%d) No grid id attribute \n",__LINE__);
			return IGNORE_CHILD_CONFIG;            
		}
		dwCurWinId = strtoul(atts[1], (char**)NULL, 10);
		if (dwCurWinId < 0 || dwCurWinId >= MAX_AE_GRID_NUM)
			return IGNORE_CHILD_CONFIG;
		g_tAECfg.dwCurGridID = dwCurWinId;
		return S_OK;
	}
	printf("[VENC_MASTER](%d) No attributes\n",__LINE__);
	return IGNORE_CHILD_CONFIG;

}
void VencMaster_SetAEGridSelected(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwCurWinId =  g_tAECfg.dwCurGridID;
	CHAR szSelected[len+1];
	BOOL bSelected = FALSE;
	memset(szSelected,0x0,sizeof(szSelected));
	snprintf(szSelected, len+1, "%s", szTemp);
	
	if (strncmp(szSelected,"true",strlen(szSelected)) ==0)
	{
		bSelected = TRUE;
	}
	
	if (g_tAECfg.atAEGrids[dwCurWinId].bSelected != bSelected) {
		g_tAECfg.atAEGrids[dwCurWinId].bSelected = bSelected;
		g_tAECfg.bGridModified = TRUE;
		g_tAECfg.bModified = TRUE;
	}
	return;
}
void VencMaster_SetAEGridWeight(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwCurWinId =  g_tAECfg.dwCurGridID;
	CHAR szWeight[len+1];
	DWORD dwWeight = 0;
	memset(szWeight,0x0,sizeof(szWeight));
	snprintf(szWeight, len+1, "%s", szTemp);
	dwWeight = strtoul(szWeight, NULL, 10);

	if (g_tAECfg.atAEGrids[dwCurWinId].dwWeight != dwWeight) {
		g_tAECfg.atAEGrids[dwCurWinId].dwWeight = dwWeight;
		g_tAECfg.bGridModified = TRUE;
		g_tAECfg.bModified = TRUE;
	}
	return;
}

/* =============================================================================================*/
SCODE VencMaster_AFSetoptions(HANDLE hObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;
	FLOAT	fXTconf2def = g_tAFCfg.fViewXTranslate;
	FLOAT	fYTconf2def = g_tAFCfg.fViewYTranslate;
	FLOAT	fXSconf2def = g_tAFCfg.fViewXScale;
	FLOAT	fYSconf2def = g_tAFCfg.fViewYScale;

	FLOAT	fXTreal2def = ptMasterInfo->tAER2D.fXTranslate;
	FLOAT	fYTreal2def = ptMasterInfo->tAER2D.fYTranslate;
	FLOAT	fXSreal2def = ptMasterInfo->tAER2D.fXScale;
	FLOAT	fYSreal2def = ptMasterInfo->tAER2D.fYScale;

	DWORD  dwStartX = 0;
	DWORD  dwStartY = 0;
	DWORD  dwWidth = 0;
	DWORD  dwHeight = 0;
	DWORD adwAFWinTbl[2];
	
//	printf("[venc_master_conf](%d)Current AF command: %s\n",__LINE__,g_tAFCfg.szCurrentCmd);
	
	// #if 0					
	if (strncmp(g_tAFCfg.szCurrentCmd,AF_CHANGE_VALUE,AF_CURRENT_CMD_SIZE) == 0)
	{

		if (g_tAFCfg.bValueModified == TRUE)
		{
//			printf("[venc_master_conf](%d)AF focus position: %s\n",__LINE__,g_tAFCfg.dwValue?"far":"near");
			memset(&tVideoSigOptions,0x0,sizeof(TVideoSignalOptions));
			tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_FOCUS_POSITION ;
			if (g_tAFCfg.dwValue == 0)
			{
				tVideoSigOptions.adwUserData[0]=0;
			}
			else if (g_tAFCfg.dwValue == 1)
			{
				tVideoSigOptions.adwUserData[0]=1; 
			}
			else
			{
				printf("[venc_master_conf](%d) %d is illegal focus value!\n",__LINE__,g_tAFCfg.dwValue);
			}
#ifdef _AF
			//syslog(LOG_INFO,"[venc_master_conf](%d)tVideoSigOptions.adwUserData[0]=%d\n",__LINE__,tVideoSigOptions.adwUserData[0]);  
			if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
				printf("[venc_master_conf](%d) set VIDEO_SIGNAL_OPTION_SET_FOCUS_POSITION fail!!\n",__LINE__);
			}else{
				printf("[venc_master_conf](%d) set VIDEO_SIGNAL_OPTION_SET_FOCUS_POSITION success!!\n",__LINE__);

			}
#endif
			g_tAFCfg.bValueModified = FALSE;
		}
	}
	//#endif 
	//#if 0
	if (strncmp(g_tAFCfg.szCurrentCmd,AF_CHANGE_ZOOM,AF_CURRENT_CMD_SIZE) == 0)
	{

		if (g_tAFCfg.bZoomValueModified == TRUE)
		{
			printf("[venc_master_conf](%d)AF zoom position: %s\n",__LINE__,g_tAFCfg.dwZoomValue?"zoom-out":"zoom-in");

			memset(&tVideoSigOptions,0x0,sizeof(TVideoSignalOptions));
			//printf("[venc_master_conf](%d)focus zoom=%d\n",__LINE__,g_tAFCfg.dwZoomValue);
			// g_tAFCfg.bZoomValueModified = FALSE;
			tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_ZOOM_POSITION;
			if (g_tAFCfg.dwZoomValue == 0)
			{
				tVideoSigOptions.adwUserData[0]=0;
			}
			else if (g_tAFCfg.dwZoomValue == 1)
			{
				tVideoSigOptions.adwUserData[0]=1; 
			}
			else
			{
				printf("[venc_master_conf](%d) %d is illegal zoom value!\n",__LINE__,g_tAFCfg.dwZoomValue);
			}

#ifdef _AF
			//printf("[venc_master_conf](%d)tVideoSigOptions.adwUserData[0]=%d\n",__LINE__,tVideoSigOptions.adwUserData[0]); 
			if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
				printf("[venc_master_conf](%d) call VIDEO_SIGNAL_OPTION_SET_ZOOM_POSITION fail!!\n",__LINE__);
			}else{
				printf("[venc_master_conf](%d) call VIDEO_SIGNAL_OPTION_SET_ZOOM_POSITION success!!\n",__LINE__);

			}
#endif
			g_tAFCfg.bZoomValueModified = FALSE;
		}
	}
	// #endif     
	if (strncmp(g_tAFCfg.szCurrentCmd,AF_CHANGE_SPEED,AF_CURRENT_CMD_SIZE) == 0)
	{
		if (g_tAFCfg.bSpeedModified == TRUE)
		{
			printf("[venc_master_conf](%d)AF focs speed: %u\n",__LINE__,g_tAFCfg.dwSpeed);
			memset(&tVideoSigOptions,0x0,sizeof(TVideoSignalOptions));
			//printf("[venc_master_conf](%d)focus speed=%d\n",__LINE__,g_tAFCfg.dwSpeed);
			//g_tAFCfg.bSpeedModified = FALSE;
			tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_FOCUS_SPEED ;
			if ((g_tAFCfg.dwSpeed >= 0) && (g_tAFCfg.dwSpeed <= 6))
			{
				tVideoSigOptions.adwUserData[0]=g_tAFCfg.dwSpeed;
			}
			else
			{
				printf("[venc_master_conf](%d) %d is illegal focus speed!\n",__LINE__,g_tAFCfg.dwSpeed);
			}
#ifdef _AF
			//syslog(LOG_INFO,"[venc_master_conf](%d)tVideoSigOptions.adwUserData[0]=%d\n",__LINE__,tVideoSigOptions.adwUserData[0]);  
			if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
				printf("[venc_master_conf](%d) call VIDEO_SIGNAL_OPTION_SET_FOCUS_SPEED fail!!\n",__LINE__);
			}else{
				printf("[venc_master_conf](%d) call VIDEO_SIGNAL_OPTION_SET_FOCUS_SPEED success!!\n",__LINE__);

			}
#endif
			g_tAFCfg.bSpeedModified = FALSE;
		}

	}

	if (strncmp(g_tAFCfg.szCurrentCmd,AF_CHANGE_WIN,AF_CURRENT_CMD_SIZE) ==0 )
	{
		if (g_tAFCfg.bWindowModified == TRUE) 
		{
			printf("[venc_master_conf](%d)AF window:(x,y,width,height)=(%u,%u,%u,%u)\n",__LINE__,g_tAFCfg.dwStartX,g_tAFCfg.dwStartY,g_tAFCfg.dwWidth,g_tAFCfg.dwHeight);

			memset(&tVideoSigOptions,0x0,sizeof(TVideoSignalOptions));
			// printf("[venc_master_conf](%d)focus window (view)=(x:%d,y:%d,w:%d,h:%d)\n",__LINE__,g_tAFCfg.dwStartX,g_tAFCfg.dwStartY,g_tAFCfg.dwWidth,g_tAFCfg.dwHeight);
			//g_tAFCfg.bWindowModified = FALSE;
			tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_AUTO_FOCUS_WINDOW ;
			if (((g_tAFCfg.dwStartY+g_tAFCfg.dwHeight) >=  g_tAFCfg.dwViewHeight) || ((g_tAFCfg.dwStartX+g_tAFCfg.dwWidth) >= g_tAFCfg.dwViewWidth))
			{
				printf("[venc_master_conf](%d)focus window (view) (x:%d,y:%d,w:%d,h:%d) is out of the boundary (%d x %d)",__LINE__,g_tAFCfg.dwStartX,g_tAFCfg.dwStartY,g_tAFCfg.dwWidth,g_tAFCfg.dwHeight,g_tAFCfg.dwViewWidth,g_tAFCfg.dwViewHeight);
				goto exit;
			}
			adwAFWinTbl[0]=(DWORD)0x0;
			adwAFWinTbl[1]=(DWORD)0x0;
			dwStartX = (DWORD) (((((((FLOAT)g_tAFCfg.dwStartX) * fXSconf2def)+fXTconf2def) - fXTreal2def) / fXSreal2def) + 0.5);
			dwStartY = (DWORD) (((((((FLOAT)g_tAFCfg.dwStartY) * fYSconf2def)+fYTconf2def) - fYTreal2def) / fYSreal2def) + 0.5);
			dwWidth = (DWORD) (((((((FLOAT)g_tAFCfg.dwWidth) * fXSconf2def)+fXTconf2def) - fXTreal2def) / fXSreal2def) + 0.5);
			dwHeight = (DWORD) (((((((FLOAT)g_tAFCfg.dwHeight)* fYSconf2def)+fYTconf2def) - fYTreal2def) / fYSreal2def) + 0.5);
			//printf("[venc_master_conf](%d)focus window (real)=(x:%d,y:%d,w:%d,h:%d)\n",__LINE__,dwStartX,dwStartY,dwWidth,dwHeight);
			adwAFWinTbl[0] =  (adwAFWinTbl[0] | dwStartY) ;
			adwAFWinTbl[0] =  (adwAFWinTbl[0] | (dwStartX << 16 ));
			adwAFWinTbl[1] =  (adwAFWinTbl[1] | dwHeight) ;
			adwAFWinTbl[1] =  (adwAFWinTbl[1] | (dwWidth << 16 ));
			// printf("[venc_master_conf](%d)adwAFWinTbl[0]=%08x adwAFWinTbl[1]=%08x",__LINE__,adwAFWinTbl[0],adwAFWinTbl[1]);
			tVideoSigOptions.adwUserData[0]=(DWORD)adwAFWinTbl;
			//syslog(LOG_INFO,"[venc_master_conf](%d)tVideoSigOptions.adwUserData[0]=%d\n",__LINE__,tVideoSigOptions.adwUserData[0]);  
#ifdef _AF
			if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
				printf("[venc_master_conf](%d) call VIDEO_SIGNAL_OPTION_SET_AUTO_FOCUS_WINDOW  fail!!\n",__LINE__);
			}else{
				printf("[venc_master_conf](%d) call VIDEO_SIGNAL_OPTION_SET_AUTO_FOCUS_WINDOW  success!!\n",__LINE__);

			}
#endif
			g_tAFCfg.bWindowModified = FALSE;
		}
	}

	if (strncmp(g_tAFCfg.szCurrentCmd,AF_CHANGE_ZOOM_SPEED,AF_CURRENT_CMD_SIZE) == 0)
	{
		if (g_tAFCfg.bZoomSpeedModified == TRUE)
		{

			printf("[venc_master_conf](%d)AF zoom speed: %u\n",__LINE__,g_tAFCfg.dwZoomSpeed);

			memset(&tVideoSigOptions,0x0,sizeof(TVideoSignalOptions));
			tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_ZOOM_SPEED ;
			if ((g_tAFCfg.dwZoomSpeed >= 0) && (g_tAFCfg.dwZoomSpeed <= 6))
			{
				tVideoSigOptions.adwUserData[0]=g_tAFCfg.dwZoomSpeed;
			}
			else
			{
				printf("[venc_master_conf](%d) %d is illegal focus speed!\n",__LINE__,g_tAFCfg.dwZoomSpeed);
			}
#ifdef _AF
			//syslog(LOG_INFO,"[venc_master_conf](%d)tVideoSigOptions.adwUserData[0]=%d\n",__LINE__,tVideoSigOptions.adwUserData[0]);  
			if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
				printf("[venc_master_conf](%d) call VIDEO_SIGNAL_OPTION_SET_ZOOM_SPEED fail!!\n",__LINE__);
			}else{

				printf("[venc_master_conf](%d) call VIDEO_SIGNAL_OPTION_SET_ZOOM_SPEED success!!\n",__LINE__);
			}
#endif
			g_tAFCfg.bZoomSpeedModified = FALSE;
		}
	}
	if (strncmp(g_tAFCfg.szCurrentCmd,AF_CHANGE_NOISE_THRESHOLD,AF_CURRENT_CMD_SIZE) == 0)
	{
		if (g_tAFCfg.bNoiseThresholdModified == TRUE)
		{
			printf("[venc_master_conf](%d)AF noise threshold: %u\n",__LINE__,g_tAFCfg.dwNoiseThreshold);

			memset(&tVideoSigOptions,0x0,sizeof(TVideoSignalOptions));
			tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_FOCUS_NOISE_THRES ;
			if ((g_tAFCfg.dwNoiseThreshold >= 0) && (g_tAFCfg.dwNoiseThreshold <= 7))
			{
				tVideoSigOptions.adwUserData[0]=g_tAFCfg.dwNoiseThreshold;
			}
			else
			{
				printf("[venc_master_conf](%d) %d is illegal focus noise threshold!\n",__LINE__,g_tAFCfg.dwNoiseThreshold);
			}
#ifdef _AF
			//syslog(LOG_INFO,"[venc_master_conf](%d)tVideoSigOptions.adwUserData[0]=%d\n",__LINE__,tVideoSigOptions.adwUserData[0]);  
			if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
				printf("[venc_master_conf](%d) call VIDEO_SIGNAL_OPTION_SET_FOCUS_NOISE_THRES fail!!\n",__LINE__);
			}else{
				printf("[venc_master_conf](%d) call VIDEO_SIGNAL_OPTION_SET_FOCUS_NOISE_THRES success!!\n",__LINE__);
			} 
#endif
			g_tAFCfg.bNoiseThresholdModified = FALSE;
		}

	}
	if (strncmp(g_tAFCfg.szCurrentCmd,AF_CHANGE_REFOCUS,AF_CURRENT_CMD_SIZE) == 0)
	{
		if (g_tAFCfg.bReFocusModified == TRUE)
		{
			printf("[venc_master_conf](%d)Re-focus: %s\n",__LINE__,g_tAFCfg.bReFocus?"TRUE":"FALSE");
			memset(&tVideoSigOptions,0x0,sizeof(TVideoSignalOptions));
			if (g_tAFCfg.bReFocus == TRUE)
			{
				tVideoSigOptions.adwUserData[0]=1;
			}
			else
			{
				tVideoSigOptions.adwUserData[0]=0;
			}
			tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_ZOOMTRACKING_FOCUS_EN;
#ifdef _AF
			//syslog(LOG_INFO,"[venc_master_conf](%d)tVideoSigOptions.adwUserData[0]=%d\n",__LINE__,tVideoSigOptions.adwUserData[0]);  
			if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
				printf("[venc_master_conf](%d) call VIDEO_SIGNAL_OPTION_SET_ZOOMTRACKING_FOCUS_EN fail!!\n",__LINE__);
			}else{
				printf("[venc_master_conf](%d) call VIDEO_SIGNAL_OPTION_SET_ZOOMTRACKING_FOCUS_EN success!!\n",__LINE__);

			}

#endif
			g_tAFCfg.bReFocusModified = FALSE;
		}
	}
exit:
	return S_OK;

}
/*===============================================================================================*/
SCODE VencMaster_AFSetoptionsFromMsgCtrl(HANDLE hObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;

	printf("[venc_master_conf](%d)Set Zoom Position? %s\n",__LINE__,ptMasterInfo->tAFZoom.bModified?"TRUE":"FALSE");
	printf("[venc_master_conf](%d)Set Focus Position? %s\n",__LINE__,ptMasterInfo->tAFFocus.bModified?"TRUE":"FALSE");

	if (ptMasterInfo->tAFZoom.bModified == TRUE)
	{	
		printf("[venc_master_conf](%d)Set Zoom Position as %s\n",__LINE__,(ptMasterInfo->tAFZoom.eZoomVal== AFZoomIn)?"ZoomIn":"ZoomOut");
		memset(&tVideoSigOptions,0x0,sizeof(TVideoSignalOptions));
		tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_ZOOM_POSITION;
		if (ptMasterInfo->tAFZoom.eZoomVal == AFZoomIn)
		{

			tVideoSigOptions.adwUserData[0]=0;
		}
		else if (ptMasterInfo->tAFZoom.eZoomVal == AFZoomOut)
		{
			tVideoSigOptions.adwUserData[0]=1; 
		}
		else
		{
			printf("[venc_master_conf](%d) %d is illegal zoom value!\n",__LINE__,ptMasterInfo->tAFZoom.eZoomVal);
			goto exit;
		}
#ifdef _AF
		//printf("[venc_master_conf](%d)tVideoSigOptions.adwUserData[0]=%d\n",__LINE__,tVideoSigOptions.adwUserData[0]); 
		if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
			printf("[venc_master_conf](%d) call VIDEO_SIGNAL_OPTION_SET_ZOOM_POSITION fail!!\n",__LINE__);
		}else{
			printf("[venc_master_conf](%d) call VIDEO_SIGNAL_OPTION_SET_ZOOM_POSITION success!!\n",__LINE__);
		}
#endif
	}
	if (ptMasterInfo->tAFFocus.bModified == TRUE)
	{
		printf("[venc_master_conf](%d)Set Focus Position as %s\n",__LINE__,(ptMasterInfo->tAFFocus.eFocusVal==AFFocusNear)?"FocusNear":"FocusFar");
		memset(&tVideoSigOptions,0x0,sizeof(TVideoSignalOptions));
		tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_FOCUS_POSITION ;
		if (ptMasterInfo->tAFFocus.eFocusVal == AFFocusNear)
		{
			tVideoSigOptions.adwUserData[0]=0;
		}
		else if (ptMasterInfo->tAFFocus.eFocusVal == AFFocusFar)
		{
			tVideoSigOptions.adwUserData[0]=1; 
		}
		else
		{
			printf("[venc_master_conf](%d) %d is illegal focus value!\n",__LINE__,ptMasterInfo->tAFFocus.eFocusVal);
		}
#ifdef _AF
		//syslog(LOG_INFO,"[venc_master_conf](%d)tVideoSigOptions.adwUserData[0]=%d\n",__LINE__,tVideoSigOptions.adwUserData[0]);  
		if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
			printf("[venc_master_conf](%d) call VIDEO_SIGNAL_OPTION_SET_FOCUS_POSITION fail!!\n",__LINE__);
		}else{
			printf("[venc_master_conf](%d) call VIDEO_SIGNAL_OPTION_SET_FOCUS_POSITION success!!\n",__LINE__);
		}
#endif
	}

exit:  
	return S_OK;
}

/* ============================================================================================= */
SCODE VencMaster_AESetoptions(HANDLE hObject)
{	
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;
	DWORD	dwIndex = 0;
#if defined(_AUTOSCENE_)
	if (isAutoSceneExist() && ptMasterInfo->bAutoSceneIsRun)
	{
		printf("[venc_master_conf](%d)Because autoscene is running, does not call %s!!\n",__LINE__,__func__);
		return S_OK;
	}
#endif
	if (g_tAECfg.bGroupModified == TRUE)
	{
		g_tAECfg.bGroupModified = FALSE;

		if (g_tAECfg.bModeModified == TRUE)
		{
			g_tAECfg.bModeModified = FALSE;
			tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MODE;
			if (g_tAECfg.dwMode == 0)
			{  
				tVideoSigOptions.adwUserData[0] = (DWORD)(AEMODE_AUTO);
			}
			else if (g_tAECfg.dwMode == 1)
			{
				tVideoSigOptions.adwUserData[0] = (DWORD)(AEMODE_BACKLIGHT);
			}
			else if (g_tAECfg.dwMode == 2)
			{
				tVideoSigOptions.adwUserData[0] = (DWORD)(AEMODE_CUSTOMIZED);
			}
			else
			{
				goto next;
			}
			//syslog(LOG_INFO,"[venc_master_conf](%d)tVideoSigOptions.adwUserData[0]=%d\n",__LINE__,tVideoSigOptions.adwUserData[0]);  
			if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
				printf("[venc_master_conf](%d) set VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MODE fail!!\n",__LINE__);
			}
		}
next:		
		//		AutoExposure_EnterCriticalSec(ptMasterInfo->hAEObject);  	-- what's the new interface
		// check MaxShutter
		if (g_tAECfg.bMaxShutterModified == TRUE) {
			g_tAECfg.bMaxShutterModified = FALSE;
			tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MAX_SHUTTER;
			tVideoSigOptions.adwUserData[0] = g_tAECfg.dwMaxShutter;
			if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
				DBPRINT0("[VENC_MASTER] set VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MAX_SHUTTER fail!!\n");
			} else {
				DBPRINT0("[VENC_MASTER] set VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MAX_SHUTTER success!!\n");
			}
		}
		// check MaxGain
		if (g_tAECfg.bMaxGainModified == TRUE) {
			g_tAECfg.bMaxGainModified = FALSE;
			tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MAX_GAIN;
			tVideoSigOptions.adwUserData[0] = g_tAECfg.dwMaxGain * 1000;
			if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
				DBPRINT0("[VENC_MASTER] set VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MAX_GAIN fail!!\n");
			} else {
				DBPRINT0("[VENC_MASTER] set VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MAX_GAIN success!!\n");
			}
		}
		// check MinShutter
		if (g_tAECfg.bMinShutterModified == TRUE) {
			g_tAECfg.bMinShutterModified = FALSE;
			tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MIN_SHUTTER;
			tVideoSigOptions.adwUserData[0] = g_tAECfg.dwMinShutter;
			if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
				DBPRINT0("[VENC_MASTER] set VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MIN_SHUTTER fail!!\n");
			} else {
				DBPRINT0("[VENC_MASTER] set VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MIN_SHUTTER success!!\n");
			}
		}
		// check MinGain
		if (g_tAECfg.bMinGainModified == TRUE) {
			g_tAECfg.bMinGainModified = FALSE;
			tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MIN_GAIN;
			tVideoSigOptions.adwUserData[0] = g_tAECfg.dwMinGain * 1000;
			if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
				DBPRINT0("[VENC_MASTER] set VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MIN_GAIN fail!!\n");
			} else {
				DBPRINT0("[VENC_MASTER] set VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_MIN_GAIN success!!\n");
			}
		}
		// check target luminance
		if (g_tAECfg.bTargetLuminanceModified == TRUE) {
			g_tAECfg.bTargetLuminanceModified = FALSE;
			tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_TARGET_LUMINANCE;
			tVideoSigOptions.adwUserData[0] = g_tAECfg.dwTargetLuminance;
			tVideoSigOptions.adwUserData[1] = g_tAECfg.dwTargetOffset;		
			if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
				DBPRINT0("[VENC_MASTER] set VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_TARGET_LUMINANCE fail!!\n");
			} else {
				DBPRINT0("[VENC_MASTER] set VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_TARGET_LUMINANCE success!!\n");
			}
		}
	//		AutoExposure_LeaveCriticalSec(ptMasterInfo->hAEObject);		-- what's the new interface
	}

	if (g_tAECfg.bGridRangeModified == TRUE) 
	{ 
		DWORD adwAEWinTbl[2];

		if (g_tAECfg.bGridRangeModified == TRUE)
		{
			g_tAECfg.bGridRangeModified = FALSE;
		}
#if 0
		printf("[VENC_MASTER_CONF](%d)AE Grid Range (X,Y,W,H)=(%d,%d,%d,%d)\n",
				__LINE__,
				g_tAECfg.tAEGridRange.dwStartX,
				g_tAECfg.tAEGridRange.dwStartY,
				g_tAECfg.tAEGridRange.dwWidth,
				g_tAECfg.tAEGridRange.dwHeight
			  );
#endif
		memset(adwAEWinTbl,0x0,sizeof(adwAEWinTbl)*sizeof(DWORD));

		adwAEWinTbl[0] = ((g_tAECfg.tAEGridRange.dwStartY << 16) | g_tAECfg.tAEGridRange.dwStartX); // start Y, start X
		adwAEWinTbl[1] = ((g_tAECfg.tAEGridRange.dwHeight << 16) | g_tAECfg.tAEGridRange.dwWidth);  // height, width

		// For VideoCap AE Window position
		tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_WINDOWS;
		tVideoSigOptions.adwUserData[0] = (DWORD)adwAEWinTbl;
		//printf("[VENC_MASTER](%d) adwAEWinTbl[0]=%08X\n",__LINE__,adwAEWinTbl[0]);
		//printf("[VENC_MASTER](%d) adwAEWinTbl[1]=%08X\n",__LINE__,adwAEWinTbl[1]);

		if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
			printf("[venc_master_conf](%d) set VideoCap VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_WINDOWS fail!!\n",__LINE__);
			//			return S_FAIL;
		}
	}

	if ((g_tAECfg.bGridModified == TRUE) && (g_tAECfg.dwMode == 2)) 
	{
		DWORD adwAEGridWeight[MAX_AE_GRID_NUM];
//		int i = 0;
		memset(adwAEGridWeight,0x0,sizeof(adwAEGridWeight));
		if (g_tAECfg.bGridModified == TRUE)
		{
			g_tAECfg.bGridModified = FALSE;
		}
		for (dwIndex = 0; dwIndex < MAX_AE_GRID_NUM; dwIndex++) {

			if (g_tAECfg.atAEGrids[dwIndex].bSelected == TRUE)
			{
				adwAEGridWeight[dwIndex]=g_tAECfg.atAEGrids[dwIndex].dwWeight;
			}
			else
			{
				adwAEGridWeight[dwIndex]=0;
			}
			//printf("[VENC_MASTER_CONF](%d) Index: %u, (selected,weight) = (%u, %u) !!\n",__LINE__ ,dwIndex, g_tAECfg.atAEGrids[dwIndex].bSelected,g_tAECfg.atAEGrids[dwIndex].dwWeight);
		}

		tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_WINDOW_PRIORITY;
		tVideoSigOptions.adwUserData[0] = (DWORD)(&(adwAEGridWeight[0]));
		if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {
			printf("[venc_master_conf](%d) set VideoCap VIDEO_SIGNAL_OPTION_SET_AUTO_EXPOSURE_WINDOW_PRIORITY fail!!\n",__LINE__);
			//			return S_FAIL;
		} 
#if 0
		printf("[VENC_MASTER_CONF](%d)AE Grid weight array (size:%d):\n",__LINE__,MAX_AE_GRID_NUM);
		printf("======================\n");
		for (i = 0; i < MAX_AE_GRID_NUM; i++)
		{
			printf("(%d,%u) ",i,adwAEGridWeight[i]);
		}
		printf("\n====================\n");
#endif
	}
	return S_OK;
}

/* ============================================================================================= */
void VencMaster_SetVideoColor(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;
	BOOL bColor = FALSE;
	CHAR szColor[len+1];
	BOOL bIsGray = FALSE;
#if defined(_AUTOSCENE_)
	if (isAutoSceneExist() && ptMasterInfo->bAutoSceneIsRun)
	{
		printf("[venc_master_conf](%d)Because autoscene is running, does not call %s!!\n",__LINE__,__func__);
		return;
	}
#endif
	memset(szColor,0x0,sizeof(szColor));
	snprintf(szColor, len+1, "%s", szTemp);
	bColor = (BOOL)strtoul(szColor, NULL, 10);

	if (g_tVideoMasterCfg.bColor != bColor) {
		bIsGray = (bColor <= 0)?TRUE:FALSE;
		tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_MONO;
		tVideoSigOptions.adwUserData[0] = bIsGray;
		if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) == S_OK) {
			g_tVideoMasterCfg.bColor =  bColor;
			DBPRINT1("VencMaster_SetVideoColor %s\n", (bIsGray)?"Gray":"Color");
		} else {
			DBPRINT1("VencMaster_SetVideoColor  %s Fail\n", (bIsGray)?"Gray":"Color");
			//        usleep(SLEEPT);
		}
	}
}

/* ============================================================================================= */
void VencMaster_SetVideoFlip(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;
	BOOL bFlip = FALSE;
	CHAR szFlip[len+1];
	BOOL bIsFlip = FALSE;

	memset(szFlip ,0x0,sizeof(szFlip));
	snprintf(szFlip, len+1, "%s", szTemp);
	bFlip = (BOOL)strtoul(szFlip, NULL, 10);

	if (g_tVideoMasterCfg.bFlip != bFlip) {
		bIsFlip = (bFlip > 0);
		tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_FLIP;
		tVideoSigOptions.adwUserData[0] = bIsFlip;
		if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) == S_OK) {
			g_tVideoMasterCfg.bFlip = bFlip;
			DBPRINT1("VencMaster_SetVideoFlip %d\n", bFlip);
		} else {
			DBPRINT1("VencMaster_SetVideoFlip %d Fail\n", bFlip);
		}
		//        usleep(SLEEPT);
	}
}
/*=================================================================================*/
void VencMaster_SetIRCut(HANDLE hObject, const CHAR *szAutoIRIS, SDWORD len)
{
	CHAR szTemp[len+1];
	BOOLEAN bEnable = FALSE;
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;

#if defined(_AUTOSCENE_)
	if (isAutoSceneExist() && ptMasterInfo->bAutoSceneIsRun)
	{
		printf("[venc_master_conf](%d)Because autoscene is running, does not call %s!!\n",__LINE__,__func__);
		return;
	}
#endif
	if (ptMasterInfo->fdIRCut == -1)
	{
		printf("[venc_master_conf](%d)VencMaster_SetIRCut() is  Failed!(ptMasterInfo->fdIRCut == -1)\n",__LINE__);
		return;
	}
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szAutoIRIS);
	if (strcmp(szTemp, "on") == 0)
	{
		bEnable = TRUE;
	}

	if (bEnable != g_tVideoMasterCfg.bEnIRCut)
	{	
		if (bEnable == TRUE)
		{
			if (ioctl(ptMasterInfo->fdIRCut, IRCUT_CLOSE) == -1)
			{
				printf("[venc_master_conf](%d)VencMaster_SetIRCut() is  Failed!(%s)\n",__LINE__,strerror(errno));
				return;
			}
		}
		else
		{
			if (ioctl(ptMasterInfo->fdIRCut, IRCUT_OPEN) == -1)
			{
				printf("[venc_master_conf](%d)VencMaster_SetIRCut() is  Failed!(%s)\n",__LINE__,strerror(errno));
				return;
			}
		}
		ioctl(ptMasterInfo->fdIRCut, IRCUT_STANDBY);
		g_tVideoMasterCfg.bEnIRCut = bEnable;
		//printf("[venc_master_conf](%d)VencMaster_SetIRCut() is  successful!!\n",__LINE__);
	}
}

/*=================================================================================*/

void VencMaster_SetAutoIRIS(HANDLE hObject, const CHAR *szAutoIRIS, SDWORD len)
{

#ifdef _AUTOIRIS
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;
	CHAR szTemp[len+1];
	BOOLEAN bEnable = FALSE;

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szAutoIRIS);
	
	if (strcmp(szTemp, "on") == 0)
	{
		bEnable = TRUE;
	}
	if (bEnable != g_tVideoMasterCfg.bEnAutoIRIS)
	{
		memset(&tVideoSigOptions,0x0,sizeof( TVideoSignalOptions ));
		tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_AUTO_IRIS_EN;
		if (bEnable == TRUE)
		{
			tVideoSigOptions.adwUserData[0] =AUTOIRIS_ENABLE; 
		}
		else
		{
			tVideoSigOptions.adwUserData[0] = AUTOIRIS_DISABLE;  
		}
		if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) == S_OK) {
			g_tVideoMasterCfg.bEnAutoIRIS = bEnable;
			//printf("[venc_master_conf](%d)VencMaster_SetAutoIRIS to %d successful!\n",__LINE__,bEnable);
		} else {
			printf("[venc_master_conf](%d)VencMaster_SetAutoIRIS to %d Fail!\n",__LINE__,bEnable);

		}
	}
#else
//	printf("[venc_master_conf](%d)The firmware does not support  AutoIRIS...\n",__LINE__);

#endif

}
/* ============================================================================================= */
void VencMaster_SetVideoMirror(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;
	BOOL bMirror = FALSE;
	CHAR szMirror[len+1];
	BOOL bIsMirror = FALSE;

	memset(szMirror ,0x0,sizeof(szMirror));
	snprintf(szMirror, len+1, "%s", szTemp);
	bMirror = (BOOL)strtoul(szMirror, NULL, 10);

	if (g_tVideoMasterCfg.bMirror != bMirror) {
		bIsMirror = (bMirror > 0);
		tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_MIRROR;
		tVideoSigOptions.adwUserData[0] = bIsMirror;
		if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) == S_OK) {
			g_tVideoMasterCfg.bMirror = bMirror;
			DBPRINT1("VencMaster_SetVideoMirror %d\n", bMirror);
		} else {
			DBPRINT1("VencMaster_SetVideoMirror %d Fail\n", bMirror);
			//        usleep(SLEEPT);
		}
	}
}

/* ============================================================================================= */
void VencMaster_SetCCCoeff_RR(HANDLE hObject, const CHAR *szCoeffRR, SDWORD len)
{
	FLOAT flCoeff_RR = 0.0;
	CHAR szTemp[len+1];

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szCoeffRR);
	flCoeff_RR = atof(szTemp);

	if (g_tColorCorrctionCfg.flCoeff_RR != flCoeff_RR) {
		g_tColorCorrctionCfg.flCoeff_RR = flCoeff_RR;
		g_tColorCorrctionCfg.bModified = TRUE;
		DBPRINT1("VencMaster_SetCCCoeff_RR set %.3f\n", g_tColorCorrctionCfg.flCoeff_RR);
	}
}

/* ============================================================================================= */
void VencMaster_SetCCCoeff_GR(HANDLE hObject, const CHAR *szCoeffGR, SDWORD len)
{
	FLOAT flCoeff_GR = 0.0;
	CHAR szTemp[len+1];

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szCoeffGR);
	flCoeff_GR = atof(szTemp);

	if (g_tColorCorrctionCfg.flCoeff_GR != flCoeff_GR) {
		g_tColorCorrctionCfg.flCoeff_GR = flCoeff_GR;
		g_tColorCorrctionCfg.bModified = TRUE;
		DBPRINT1("VencMaster_SetCCCoeff_GR set %.3f\n", g_tColorCorrctionCfg.flCoeff_GR);
	}
}

/* ============================================================================================= */
void VencMaster_SetCCCoeff_BR(HANDLE hObject, const CHAR *szCoeffBR, SDWORD len)
{
	FLOAT flCoeff_BR = 0.0;
	CHAR szTemp[len+1];

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szCoeffBR);
	flCoeff_BR = atof(szTemp);

	if (g_tColorCorrctionCfg.flCoeff_BR != flCoeff_BR) {
		g_tColorCorrctionCfg.flCoeff_BR = flCoeff_BR;
		g_tColorCorrctionCfg.bModified = TRUE;
		DBPRINT1("VencMaster_SetCCCoeff_BR set %.3f\n", g_tColorCorrctionCfg.flCoeff_BR);
	}
}

/* ============================================================================================= */
void VencMaster_SetCCCoeff_RG(HANDLE hObject, const CHAR *szCoeffRG, SDWORD len)
{
	FLOAT flCoeff_RG = 0.0;
	CHAR szTemp[len+1];

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szCoeffRG);
	flCoeff_RG = atof(szTemp);

	if (g_tColorCorrctionCfg.flCoeff_RG != flCoeff_RG) {
		g_tColorCorrctionCfg.flCoeff_RG = flCoeff_RG;
		g_tColorCorrctionCfg.bModified = TRUE;
		DBPRINT1("VencMaster_SetCCCoeff_RG set %.3f\n", g_tColorCorrctionCfg.flCoeff_RG);
	}
}

/* ============================================================================================= */
void VencMaster_SetCCCoeff_GG(HANDLE hObject, const CHAR *szCoeffGG, SDWORD len)
{
	FLOAT flCoeff_GG = 0.0;
	CHAR szTemp[len+1];

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szCoeffGG);
	flCoeff_GG = atof(szTemp);

	if (g_tColorCorrctionCfg.flCoeff_GG != flCoeff_GG) {
		g_tColorCorrctionCfg.flCoeff_GG = flCoeff_GG;
		g_tColorCorrctionCfg.bModified = TRUE;
		DBPRINT1("VencMaster_SetCCCoeff_GG set %.3f\n", g_tColorCorrctionCfg.flCoeff_GG);
	}
}

/* ============================================================================================= */
void VencMaster_SetCCCoeff_BG(HANDLE hObject, const CHAR *szCoeffBG, SDWORD len)
{
	FLOAT flCoeff_BG = 0.0;
	CHAR szTemp[len+1];

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szCoeffBG);
	flCoeff_BG = atof(szTemp);

	if (g_tColorCorrctionCfg.flCoeff_BG != flCoeff_BG) {
		g_tColorCorrctionCfg.flCoeff_BG = flCoeff_BG;
		g_tColorCorrctionCfg.bModified = TRUE;
		DBPRINT1("VencMaster_SetCCCoeff_BG set %.3f\n", g_tColorCorrctionCfg.flCoeff_BG);
	}
}

/* ============================================================================================= */
void VencMaster_SetCCCoeff_RB(HANDLE hObject, const CHAR *szCoeffRB, SDWORD len)
{
	FLOAT flCoeff_RB = 0.0;
	CHAR szTemp[len+1];

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szCoeffRB);
	flCoeff_RB = atof(szTemp);

	if (g_tColorCorrctionCfg.flCoeff_RB != flCoeff_RB) {
		g_tColorCorrctionCfg.flCoeff_RB = flCoeff_RB;
		g_tColorCorrctionCfg.bModified = TRUE;
		DBPRINT1("VencMaster_SetCCCoeff_RB set %.3f\n", g_tColorCorrctionCfg.flCoeff_RB);
	}
}

/* ============================================================================================= */
void VencMaster_SetCCCoeff_GB(HANDLE hObject, const CHAR *szCoeffGB, SDWORD len)
{
	FLOAT flCoeff_GB = 0.0;
	CHAR szTemp[len+1];

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szCoeffGB);
	flCoeff_GB = atof(szTemp);

	if (g_tColorCorrctionCfg.flCoeff_GB != flCoeff_GB) {
		g_tColorCorrctionCfg.flCoeff_GB = flCoeff_GB;
		g_tColorCorrctionCfg.bModified = TRUE;
		DBPRINT1("VencMaster_SetCCCoeff_GB set %.3f\n", g_tColorCorrctionCfg.flCoeff_GB);
	}
}

/* ============================================================================================= */
void VencMaster_SetCCCoeff_BB(HANDLE hObject, const CHAR *szCoeffBB, SDWORD len)
{
	FLOAT flCoeff_BB = 0.0;
	CHAR szTemp[len+1];

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szCoeffBB);
	flCoeff_BB = atof(szTemp);

	if (g_tColorCorrctionCfg.flCoeff_BB != flCoeff_BB) {
		g_tColorCorrctionCfg.flCoeff_BB = flCoeff_BB;
		g_tColorCorrctionCfg.bModified = TRUE;
		DBPRINT1("VencMaster_SetCCCoeff_BB set %.3f\n", g_tColorCorrctionCfg.flCoeff_BB);
	}
}

/* ============================================================================================= */
void VencMaster_SetCCOffset_R(HANDLE hObject, const char *szOffsetR, SDWORD len)
{
	FLOAT flOffset_R = 0.0;
	CHAR szTemp[len+1];

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szOffsetR);
	flOffset_R = atof(szTemp);

	if (g_tColorCorrctionCfg.flOffset_R != flOffset_R) {
		g_tColorCorrctionCfg.flOffset_R = flOffset_R;
		g_tColorCorrctionCfg.bModified = TRUE;
		DBPRINT1("VencMaster_SetCCOffset_R set %.3f\n", g_tColorCorrctionCfg.flOffset_R);
	}
}

/* ============================================================================================= */
void VencMaster_SetCCOffset_G(HANDLE hObject, const char *szOffsetG, SDWORD len)
{
	FLOAT flOffset_G = 0.0;
	CHAR szTemp[len+1];

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szOffsetG);
	flOffset_G = atof(szTemp);

	if (g_tColorCorrctionCfg.flOffset_G != flOffset_G) {
		g_tColorCorrctionCfg.flOffset_G = flOffset_G;
		g_tColorCorrctionCfg.bModified = TRUE;
		DBPRINT1("VencMaster_SetCCOffset_G set %.3f\n", g_tColorCorrctionCfg.flOffset_G);
	}
}

/* ============================================================================================= */
void VencMaster_SetCCOffset_B(HANDLE hObject, const char *szOffsetB, SDWORD len)
{
	FLOAT flOffset_B = 0.0;
	CHAR szTemp[len+1];

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szOffsetB);
	flOffset_B = atof(szTemp);

	if (g_tColorCorrctionCfg.flOffset_B != flOffset_B) {
		g_tColorCorrctionCfg.flOffset_B = flOffset_B;
		g_tColorCorrctionCfg.bModified = TRUE;
		DBPRINT1("VencMaster_SetCCOffset_B set %.3f\n", g_tColorCorrctionCfg.flOffset_B);
	}
}

/* ============================================================================================= */
SCODE VencMaster_CCSetoptions(HANDLE hObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;
	BOOL bOffsetSetFailed = FALSE;

	FLOAT aflUserData[12] = {0.0};

#if defined(_AUTOSCENE_)
	if (isAutoSceneExist() && ptMasterInfo->bAutoSceneIsRun)
	{
		printf("[venc_master_conf](%d)Because autoscene is running, does not call %s!!\n",__LINE__,__func__);
		return S_OK;
	}
#endif
	
	aflUserData[0] = g_tColorCorrctionCfg.flCoeff_RR;
	aflUserData[1] = g_tColorCorrctionCfg.flCoeff_GR;
	aflUserData[2] = g_tColorCorrctionCfg.flCoeff_BR;
	aflUserData[3] = g_tColorCorrctionCfg.flCoeff_RG;
	aflUserData[4] = g_tColorCorrctionCfg.flCoeff_GG;
	aflUserData[5] = g_tColorCorrctionCfg.flCoeff_BG;
	aflUserData[6] = g_tColorCorrctionCfg.flCoeff_RB;
	aflUserData[7] = g_tColorCorrctionCfg.flCoeff_GB;
	aflUserData[8] = g_tColorCorrctionCfg.flCoeff_BB;
	aflUserData[9] = g_tColorCorrctionCfg.flOffset_R;//-7~+7 for V3
	aflUserData[10] = g_tColorCorrctionCfg.flOffset_G;//-7~+7 for V3
	aflUserData[11] = g_tColorCorrctionCfg.flOffset_B;//-7~+7 for V3

	if ((aflUserData[9] < -7 )|| (aflUserData[9] > 7))
		bOffsetSetFailed = TRUE;

	if ((aflUserData[10] < -7 )|| (aflUserData[10] > 7))
		bOffsetSetFailed = TRUE;

	if ((aflUserData[11] < -7 )|| (aflUserData[11] > 7))
		bOffsetSetFailed = TRUE;

	if (bOffsetSetFailed == TRUE)
	{
		printf("VencMaster_CCSetoptions fail!! Because the range of color offset (R,G,B)=(%.3f, %.3f, %.3f) is -7~+7.\n",aflUserData[9], aflUserData[10], aflUserData[11]);
		return S_FAIL;
	}

	tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_COLOR_CORRECTION;
	tVideoSigOptions.adwUserData[0] = (DWORD)aflUserData;
	if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) == S_OK) {
#if 0
		printf("VencMaster_CCSetoptions successful \n");
		printf(" (RR, GR, BR) = (%.3f, %.3f, %.3f) \n", aflUserData[0], aflUserData[1], aflUserData[2]);
		printf(" (RG, GG, BG) = (%.3f, %.3f, %.3f) \n", aflUserData[3], aflUserData[4], aflUserData[5]);
		printf(" (RB, GB, BB) = (%.3f, %.3f, %.3f) \n", aflUserData[6], aflUserData[7], aflUserData[8]);
		printf(" (Ro, Go, Bo) = (%.3f, %.3f, %.3f) \n", aflUserData[9], aflUserData[10], aflUserData[11]);
#endif
		return S_OK;
	} else {
		printf("VencMaster_CCSetoptions fail \n");
		return S_FAIL;
	}
}

/* ============================================================================================= */
void VencMaster_SetToneMappingEnable(HANDLE hObject, const CHAR *szTMEnable, SDWORD len)
{
	CHAR szTemp[len+1];
	BOOL	bEnable = FALSE;

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szTMEnable);
	if (strcmp(szTemp, "on") == 0)
		bEnable = TRUE;

	if (g_tTMCfg.bEnable != bEnable) {
		g_tTMCfg.bEnable = bEnable;
		g_tTMCfg.bModified = TRUE;
	}
}

/* ============================================================================================= */
void VencMaster_SetToneMappingLevel(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwTMLevel = 0;
	CHAR szTMLevel[len+1];

	memset(szTMLevel ,0x0,sizeof(szTMLevel));
	snprintf(szTMLevel, len+1, "%s", szTemp);
	dwTMLevel = strtoul(szTMLevel, NULL, 10);

	if (g_tTMCfg.dwTMLevel != dwTMLevel) {
		g_tTMCfg.dwTMLevel = dwTMLevel;
		g_tTMCfg.bModified = TRUE;
	}
}

/* ============================================================================================= */
SCODE VencMaster_TMSetoptions(HANDLE hObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;

	tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_TONE_MAPPING;

	tVideoSigOptions.adwUserData[0] = (g_tTMCfg.bEnable == TRUE)? TRUE : FALSE;
	tVideoSigOptions.adwUserData[1] = g_tTMCfg.dwTMLevel;

	if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) == S_OK) {
		DBPRINT0("VencMaster_TMSetoptions successful \n");
		//printf("(Tone Mapping, Level) = (%s, %u) \n", (g_tTMCfg.bEnable == TRUE)? "TRUE" : "FALSE", tVideoSigOptions.adwUserData[1]);
		return S_OK;
	} else {
		DBPRINT0("VencMaster_TMSetoptions fail \n");
		return S_FAIL;
	}
}

/* ============================================================================================= */

void VencMaster_SetBlackClampGr(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	DWORD dwVlaue = 0;
	CHAR szValue[len+1];

	memset(szValue ,0x0,sizeof(szValue));
	snprintf(szValue, len+1, "%s", szElement);
	dwVlaue  = strtoul(szValue, NULL, 10);

	if (g_tBlackClampCfg.dwBlackClampGr != dwVlaue) {
		g_tBlackClampCfg.dwBlackClampGr=  dwVlaue ;
		g_tBlackClampCfg.bModified = TRUE;
	}
}
void VencMaster_SetBlackClampR(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	DWORD dwVlaue = 0;
	CHAR szValue[len+1];

	memset(szValue ,0x0,sizeof(szValue));
	snprintf(szValue, len+1, "%s", szElement);
	dwVlaue  = strtoul(szValue, NULL, 10);

	if (g_tBlackClampCfg.dwBlackClampR != dwVlaue) {
		g_tBlackClampCfg.dwBlackClampR=  dwVlaue ;
		g_tBlackClampCfg.bModified = TRUE;
	}

}
void VencMaster_SetBlackClampGb(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	DWORD dwVlaue = 0;
	CHAR szValue[len+1];

	memset(szValue ,0x0,sizeof(szValue));
	snprintf(szValue, len+1, "%s", szElement);
	dwVlaue  = strtoul(szValue, NULL, 10);

	if (g_tBlackClampCfg.dwBlackClampGb != dwVlaue) {
		g_tBlackClampCfg.dwBlackClampGb=  dwVlaue ;
		g_tBlackClampCfg.bModified = TRUE;
	}
}
void VencMaster_SetBlackClampB(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	DWORD dwVlaue = 0;
	CHAR szValue[len+1];

	memset(szValue ,0x0,sizeof(szValue));
	snprintf(szValue, len+1, "%s", szElement);
	dwVlaue  = strtoul(szValue, NULL, 10);

	if (g_tBlackClampCfg.dwBlackClampB != dwVlaue) {
		g_tBlackClampCfg.dwBlackClampB=  dwVlaue ;
		g_tBlackClampCfg.bModified = TRUE;
	}
}

SCODE VencMaster_BlackCalmpSetoptions(HANDLE hObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;
#if defined(_AUTOSCENE_)
	if (isAutoSceneExist() && ptMasterInfo->bAutoSceneIsRun)
	{
		printf("[venc_master_conf](%d)Because autoscene is running, does not call %s!!\n",__LINE__,__func__);
		return S_OK;
	}
#endif
	
	memset(&tVideoSigOptions, 0x0,sizeof(TVideoSignalOptions));
	tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_BLACK_CLAMP;
	tVideoSigOptions.adwUserData[0] =  (((g_tBlackClampCfg.dwBlackClampB&0xFF)<<24) | ((g_tBlackClampCfg.dwBlackClampGb&0xFF)<<16) | 
			((g_tBlackClampCfg.dwBlackClampR&0xFF)<<8) |(g_tBlackClampCfg.dwBlackClampGr&0xFF));
	
	if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) == S_OK) {
		//printf("VencMaster_TBlackCalmpSetoptions successful!! (Gr,R,Gb,B) = (%u,%u,%u,%u)\n",g_tBlackClampCfg.dwBlackClampGr,g_tBlackClampCfg.dwBlackClampR,g_tBlackClampCfg.dwBlackClampGb, g_tBlackClampCfg.dwBlackClampB);
		return S_OK;
	} else {
		printf(" VencMaster_TBlackCalmpSetoptions fail \n");
		return S_FAIL;
	}
}
/* ============================================================================================= */

void VencMaster_SetDeImpulseEnable(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	CHAR szTemp[len+1];
	BOOL	bEnable = FALSE;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szElement);
	if (strcmp(szTemp, "on") == 0)
		bEnable = TRUE;

	if (g_tDeImpulseCfg.bDeImpulseEn != bEnable) {
		g_tDeImpulseCfg.bDeImpulseEn = bEnable;
		g_tDeImpulseCfg.bModified = TRUE;
	}

}
void VencMaster_SetDeImpulseDcBotThrd(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	DWORD dwVlaue = 0;
	CHAR szValue[len+1];
	memset(szValue ,0x0,sizeof(szValue));
	snprintf(szValue, len+1, "%s", szElement);
	dwVlaue  = strtoul(szValue, NULL, 10);

	if (g_tDeImpulseCfg.dwDcBotThrd != dwVlaue ) {
		g_tDeImpulseCfg.dwDcBotThrd=  dwVlaue ;
		g_tDeImpulseCfg.bModified = TRUE;
	}

}
void VencMaster_SetDeImpulseDcTopThrd(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	DWORD dwVlaue = 0;
	CHAR szValue[len+1];
	memset(szValue ,0x0,sizeof(szValue));
	snprintf(szValue, len+1, "%s", szElement);
	dwVlaue  = strtoul(szValue, NULL, 10);
	
	if (g_tDeImpulseCfg.dwDcTopThrd != dwVlaue ) {
		g_tDeImpulseCfg.dwDcTopThrd=  dwVlaue ;
		g_tDeImpulseCfg.bModified = TRUE;
	}
}
void VencMaster_SetDeImpulseMaxTopLevel(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	DWORD dwVlaue = 0;
	CHAR szValue[len+1];
	memset(szValue ,0x0,sizeof(szValue));
	snprintf(szValue, len+1, "%s", szElement);
	dwVlaue  = strtoul(szValue, NULL, 10);

	if (g_tDeImpulseCfg.dwMaxTopLevel != dwVlaue ) {
		g_tDeImpulseCfg.dwMaxTopLevel=  dwVlaue ;
		g_tDeImpulseCfg.bModified = TRUE;
	}

}
void VencMaster_SetDeImpulseMaxBotLevel(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	DWORD dwVlaue = 0;
	CHAR szValue[len+1];
	memset(szValue ,0x0,sizeof(szValue));
	snprintf(szValue, len+1, "%s", szElement);
	dwVlaue  = strtoul(szValue, NULL, 10);
	
	if (g_tDeImpulseCfg.dwMaxBotLevel != dwVlaue ) {
		g_tDeImpulseCfg.dwMaxBotLevel=  dwVlaue ;
		g_tDeImpulseCfg.bModified = TRUE;
	}
}
void VencMaster_SetDeImpulseMinTopLevel(HANDLE hObject, const CHAR *szElement, SDWORD len)
{
	DWORD dwVlaue = 0;
	CHAR szValue[len+1];
	memset(szValue ,0x0,sizeof(szValue));
	snprintf(szValue, len+1, "%s", szElement);
	dwVlaue  = strtoul(szValue, NULL, 10);
	
	if (g_tDeImpulseCfg.dwMinTopLevel != dwVlaue ) {
		g_tDeImpulseCfg.dwMinTopLevel=  dwVlaue ;
		g_tDeImpulseCfg.bModified = TRUE;
	}

}
void VencMaster_SetDeImpulseMinBotLevel(HANDLE hObject, const CHAR *szElement, SDWORD len)
{ 
	DWORD dwVlaue = 0;
	CHAR szValue[len+1];
	memset(szValue ,0x0,sizeof(szValue));
	snprintf(szValue, len+1, "%s", szElement);
	dwVlaue  = strtoul(szValue, NULL, 10);
	
	if (g_tDeImpulseCfg.dwMinBotLevel != dwVlaue ) {
		g_tDeImpulseCfg.dwMinBotLevel=  dwVlaue ;
		g_tDeImpulseCfg.bModified = TRUE;
	}

}
SCODE VencMaster_DeImpulseSetoptions(HANDLE hObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;
	DWORD adwDeImpulseParam[12];
	
	BOOL bDeImpulseEn = FALSE;
	DWORD	dwDcBotThrd = -1 ; 
	DWORD	dwDcTopThrd = -1 ;
	DWORD	dwMaxTopLevel = -1; 
	DWORD	dwMaxBotLevel = -1; 
	DWORD	dwMinTopLevel = -1; 
	DWORD	dwMinBotLevel = -1;
#if defined(_AUTOSCENE_)
	if (isAutoSceneExist() && ptMasterInfo->bAutoSceneIsRun)
	{
		printf("[venc_master_conf](%d)Because autoscene is running, does not call %s!!\n",__LINE__,__func__);
		return S_OK;
	}
#endif
	memset(adwDeImpulseParam,0x0,sizeof(adwDeImpulseParam));
	memset(&tVideoSigOptions, 0x0,sizeof(TVideoSignalOptions));
	
	bDeImpulseEn = g_tDeImpulseCfg.bDeImpulseEn; // 0~65535 get from UI
	dwDcBotThrd = g_tDeImpulseCfg.dwDcBotThrd; // 0~65535 get from UI
	dwDcTopThrd = g_tDeImpulseCfg.dwDcTopThrd; // 0~65535 Fixed in xml
	dwMaxTopLevel = g_tDeImpulseCfg.dwMaxTopLevel; // 0~65535  Fixed in xml
	dwMaxBotLevel = g_tDeImpulseCfg.dwMaxBotLevel; // 0~65535  Fixed in xml
	dwMinTopLevel = g_tDeImpulseCfg.dwMinTopLevel; // 0~65535  Fixed in xml
	dwMinBotLevel = g_tDeImpulseCfg.dwMinBotLevel; // 0~65535  Fixed in xml

	adwDeImpulseParam[0] = ((dwDcTopThrd<<16) | dwDcBotThrd);
	adwDeImpulseParam[1] = ((dwMaxTopLevel<<16) | dwMaxBotLevel);
	adwDeImpulseParam[2] = ((dwMinTopLevel<<16) | dwMinBotLevel);
	adwDeImpulseParam[3] = adwDeImpulseParam[0];
	adwDeImpulseParam[4] = adwDeImpulseParam[1];
	adwDeImpulseParam[5] = adwDeImpulseParam[2];
	adwDeImpulseParam[6] = adwDeImpulseParam[0];
	adwDeImpulseParam[7] = adwDeImpulseParam[1];
	adwDeImpulseParam[8] = adwDeImpulseParam[2];
	adwDeImpulseParam[9] = adwDeImpulseParam[0];
	adwDeImpulseParam[10] = adwDeImpulseParam[1];
	adwDeImpulseParam[11] = adwDeImpulseParam[2];

	tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_IMPULSE_NOISE_REMOVAL ;
	tVideoSigOptions.adwUserData[0] =(DWORD)bDeImpulseEn;
	tVideoSigOptions.adwUserData[1] =(DWORD)adwDeImpulseParam;

	if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) == S_OK) {
		//printf("VencMaster_DeImpulseSetoptions successful!! (Enable,DcBotThrd,DcTopThrd,MaxTopLevel,MaxBotLevel,MinTopLevel,MinBotLevel) = (%u,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x)\n",bDeImpulseEn,dwDcBotThrd,dwDcTopThrd,dwMaxTopLevel,dwMaxBotLevel,dwMinTopLevel,dwMinBotLevel);
		return S_OK;
	} else {
		printf(" VencMaster_DeImpulseSetoptions fail \n");
		return S_FAIL;
	}


}

/* ============================================================================================= */
void VencMaster_SetCEEnable(HANDLE hObject, const CHAR *szCEEnable, SDWORD len)
{
	CHAR szTemp[len+1];
	BOOL	bEnable = FALSE;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szCEEnable);
	if (strcmp(szTemp, "on") == 0)
		bEnable = TRUE;

	if (g_tCECfg.bEnable != bEnable) {
		g_tCECfg.bEnable = bEnable;
		g_tCECfg.bModified = TRUE;
	}
}

/* ============================================================================================= */
void VencMaster_SetCEMode(HANDLE hObject, const CHAR *szCEMode, SDWORD len)
{
	CHAR szTemp[len+1];
	DWORD	dwCEMode;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szCEMode);    
	if (strcasecmp(szTemp, "User") == 0) {
		dwCEMode = CE_MODE_USER;
	} else if (strcasecmp(szTemp, "Default") == 0) {
		dwCEMode = CE_MODE_DEFAULT;
	} else {
		dwCEMode = CE_MODE_AUTO;
	}

	if (g_tCECfg.dwCEMode != dwCEMode) {
		g_tCECfg.dwCEMode = dwCEMode;
		g_tCECfg.bModified = TRUE;
	}
}

/* ============================================================================================= */
void VencMaster_SetCEBirghtness(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwCEBrightness = 0;
	CHAR szCEBrightness[len+1];	


	memset(szCEBrightness ,0x0,sizeof(szCEBrightness));
	snprintf(szCEBrightness, len+1, "%s", szTemp);
	dwCEBrightness = strtoul(szCEBrightness, NULL, 10);
	if (g_tCECfg.dwCEBrightness != dwCEBrightness) {
		g_tCECfg.dwCEBrightness = dwCEBrightness;
		g_tCECfg.bModified = TRUE;
	}
}

/* ============================================================================================= */
void VencMaster_SetCEDefaultMethod(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwDefaultMethod = 0;
	CHAR szCEDefaultMethod[len+1];	

	memset(szCEDefaultMethod ,0x0,sizeof(szCEDefaultMethod));
	snprintf(szCEDefaultMethod, len+1, "%s", szTemp);
	dwDefaultMethod  = strtoul(szCEDefaultMethod, NULL, 10);

	if (g_tCECfg.dwDefaultMethod != dwDefaultMethod) {
		g_tCECfg.dwDefaultMethod = dwDefaultMethod;
		g_tCECfg.bModified = TRUE;
	}
}

/* ============================================================================================= */
SCODE VencMaster_SetCEUserTableTag(HANDLE hObject, const CHAR *szCEUserTableMode, const CHAR** atts)
{
	g_tCECfg.dwCounter = 0;
	g_tCECfg.pbyCollect = (BYTE *)malloc(MAX_TABLE_SIZE);
	memset(g_tCECfg.pbyCollect, 0, MAX_TABLE_SIZE);
	g_tCECfg.dwPosition = 0;
	return S_OK;
}

/* ============================================================================================= */
void VencMaster_SetCEUserTableCollection(HANDLE hObject, const CHAR *szTable, SDWORD len)
{
	strncpy((g_tCECfg.pbyCollect + g_tCECfg.dwPosition), szTable, len);
	g_tCECfg.dwPosition += len;
}

/* ============================================================================================= */
void VencMaster_SetCEUserTable(HANDLE hObject, const CHAR *szElement)
{
	char *szToken;
	BYTE *pbyTemp;

	pbyTemp = g_tCECfg.pbyCollect;
	szToken = strtok(pbyTemp, ",");
	while (szToken != NULL) {
		g_tCECfg.abyCEUserDefineTbl[g_tCECfg.dwCounter] = (BYTE)atoi(szToken);
		szToken = strtok(NULL, ",");
		g_tCECfg.dwCounter++;
	}
	free(g_tCECfg.pbyCollect);
	g_tCECfg.bModified = TRUE;
}

/* ============================================================================================= */
SCODE VencMaster_CESetoptions(HANDLE hObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;

	BOOL  bCEEnable = g_tCECfg.bEnable;	
	DWORD dwCEMode = g_tCECfg.dwCEMode;
	DWORD dwDefaultMethod = g_tCECfg.dwDefaultMethod;
	DWORD dwCEBrightness = g_tCECfg.dwCEBrightness;
	DWORD dwUserData = 0;
#if defined(_AUTOSCENE_)
	if (isAutoSceneExist() && ptMasterInfo->bAutoSceneIsRun)
	{
		printf("[venc_master_conf](%d)Because autoscene is running, does not call %s!!\n",__LINE__,__func__);
		return S_OK;
	}
#endif
	if (bCEEnable == TRUE)
		dwUserData = 1;

	if (dwCEMode == CE_MODE_AUTO) {
		dwUserData |= (1 << 1);
	} else if (dwCEMode == CE_MODE_USER) {
		dwUserData |= (1 << 2);
	}

	dwUserData |= (dwDefaultMethod << 3);
	dwUserData |= (dwCEBrightness << 5);

	//printf("[Contrast Enhance] bCEEnable = %s, dwCEMode = %u, dwDefaultMethod = %u, dwCEBrightness = %u !!\n", (bCEEnable==TRUE)?"TRUE":"FALSE", dwCEMode, dwDefaultMethod, dwCEBrightness);
	tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_CONTRAST_ENHANCEMENT;
	tVideoSigOptions.adwUserData[0] = dwUserData;	
	if (dwCEMode == CE_MODE_USER) {
		tVideoSigOptions.adwUserData[1] = (DWORD)(g_tCECfg.abyCEUserDefineTbl);
	}
	if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) == S_OK) {
		DBPRINT0("[VENC_MASTER] VencMaster_CESetoptions successful \n");
		debug_printf("[VENC_MASTER] tVideoSigOptions.adwUserData[0] = %u \n", tVideoSigOptions.adwUserData[0]);
		return S_OK;
	} else {
		DBPRINT0("[VENC_MASTER] VencMaster_CESetoptions fail \n");
		return S_FAIL;
	}
}

/* ============================================================================================= */
SCODE VencMaster_SetGammaTableTag(HANDLE hObject, const CHAR *szGammaTableMode, const CHAR** atts)
{
	return S_OK;
}

/* ============================================================================================= */
static void trim(char * s) {
	char * p = s;
	int l = strlen(p);

	while(isspace(p[l - 1])||(p[l - 1]=='\n')||(p[l - 1]=='\r')) p[--l] = 0;
	while(* p && isspace(* p)) ++p, --l;

	memmove(s, p, l + 1);
}


void VencMaster_SetGammaTableCollection(HANDLE hObject, const CHAR *szTable, SDWORD len)
{

	FILE *fp = NULL;
	char szTmp[len+1];
	char str[128];
	int count = 0;

	memset(str,0x0,sizeof(str));
	memset(szTmp,0x0,sizeof(szTmp));
	snprintf(szTmp, len+1, "%s", szTable);

	//printf("[venc_master_conf](%d)gamma_table:%s\n",__LINE__,szTmp);

	fp = fopen(szTmp,"r");
	if (fp != NULL)
	{
		while(!feof(fp))
		{
			memset(str,0x0,sizeof(str));
			if (fgets(str,sizeof(str),fp) != NULL)
			{
				trim(str);
				if (count >= GAMMA_TABLE_SIZE)
				{
					goto exit;
				}
				g_tGmaTblCfg.abyGammaTbl[count] = (BYTE)atoi(str);
				count++;
			}
		}
	}
	else
	{
		printf("[venc_master_conf](%d)cannot open gamma_table:%s\n",__LINE__,szTmp);
	}
exit:
	//printf("[venc_master_conf](%d)gamma_table element count:%d\n",__LINE__,count);
	if (fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}

}

/* ============================================================================================= */
void VencMaster_SetGammaTable(HANDLE hObject, const CHAR *szElement)
{
	return;
}

/* ============================================================================================= */
SCODE VencMaster_GammaTableSetoptions(HANDLE hObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;
	
#if defined(_AUTOSCENE_)
	if (isAutoSceneExist() && ptMasterInfo->bAutoSceneIsRun)
	{
		printf("[venc_master_conf](%d)Because autoscene is running, does not call %s!!\n",__LINE__,__func__);
		return S_OK;
	}
#endif
	
	tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_GAMMA_TABLE;
	tVideoSigOptions.adwUserData[0] = (DWORD)(g_tGmaTblCfg.abyGammaTbl);
	if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) == S_OK) {
		DBPRINT0("VencMaster_GammaTableSetoptions successful \n");
	} else {
		DBPRINT0("VencMaster_GammaTableSetoptions fail \n");
	}
	return S_OK;
}

/* ============================================================================================= */
void VencMaster_SetImageAdjustmentSelectMode(HANDLE hObject, const CHAR *szImgAdjSelectMode, SDWORD len)
{
	CHAR szTemp[len+1];
	EImgAdjMode	eImgAdjSelectMode;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szImgAdjSelectMode);
	
	if (strcasecmp(szTemp, "dynamic") == 0) {
		eImgAdjSelectMode = imgadjmDynamic;
	} else if (strcasecmp(szTemp, "day") == 0){
		eImgAdjSelectMode = imgadjmFixed;
	} else if (strcasecmp(szTemp, "night") == 0){
		eImgAdjSelectMode = imgadjmNight;
	} else if (strcasecmp(szTemp, "auto") == 0){
		eImgAdjSelectMode = imgadjmAuto;
	}else {
		eImgAdjSelectMode = imgadjmFixed;
	}
	
	if (g_tImgAdjCfg.eImgAdjSelectMode != eImgAdjSelectMode) {
		g_tImgAdjCfg.eImgAdjSelectMode = eImgAdjSelectMode;
		g_tImgAdjCfg.bSelectModeModified = TRUE;
		g_tImgAdjCfg.bModified = TRUE;
	}
}

/* ============================================================================================= */
void VencMaster_SetImageAdjustmentBright(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwBright = 0;
	CHAR szBright[len+1];
	
	memset(szBright, 0x0, sizeof(szBright));
	snprintf(szBright, len+1, "%s", szTemp);
	dwBright  = strtoul(szBright, NULL, 10);
	dwBright = (dwBright&0x000000FF);

	if (g_tImgAdjCfg.dwBrightness != dwBright) {
		g_tImgAdjCfg.dwBrightness = dwBright;
		g_tImgAdjCfg.bBrightModified = TRUE;
		g_tImgAdjCfg.bModified = TRUE;
	}
}

/* ============================================================================================= */
void VencMaster_SetImageAdjustmentContrast(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwContrast = 0;

	CHAR szContrast[len+1];

	memset(szContrast, 0x0, sizeof(szContrast));
	snprintf(szContrast, len+1, "%s", szTemp);
	dwContrast   = strtoul(szContrast, NULL, 10);
	dwContrast = (dwContrast&0x000000FF);

	if (g_tImgAdjCfg.dwContrast != dwContrast) {
		g_tImgAdjCfg.dwContrast = dwContrast;
		g_tImgAdjCfg.bContrastModified = TRUE;
		g_tImgAdjCfg.bModified = TRUE;
	}
}

/* ============================================================================================= */
void VencMaster_SetImageAdjustmentSaturation(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwSaturation = 0;
	CHAR szSaturation[len+1];	

	memset(szSaturation ,0x0,sizeof(szSaturation));
	snprintf(szSaturation, len+1, "%s", szTemp);
	dwSaturation   = strtoul(szSaturation, NULL, 10);

	if (g_tImgAdjCfg.dwSaturation != dwSaturation) {
		g_tImgAdjCfg.dwSaturation = dwSaturation;
		g_tImgAdjCfg.bSaturationModified = TRUE;
		g_tImgAdjCfg.bModified = TRUE;
	}
}

/* ============================================================================================= */
void VencMaster_SetImageAdjustmentStartLum(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwStartLum = 0;
	CHAR szStartLum[len+1];
	memset(szStartLum ,0x0,sizeof(szStartLum));
	snprintf(szStartLum, len+1, "%s", szTemp);
	dwStartLum   = strtoul(szStartLum, NULL, 10);

	if (g_tImgAdjCfg.dwStartLum != dwStartLum) {
		g_tImgAdjCfg.dwStartLum = dwStartLum;
		g_tImgAdjCfg.bModified = TRUE;
	}
}

/* ============================================================================================= */
void VencMaster_SetImageAdjustmentEndLum(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwEndLum = 0;
	CHAR szEndLum[len+1];
	memset(szEndLum ,0x0,sizeof(szEndLum));
	snprintf(szEndLum, len+1, "%s", szTemp);
	dwEndLum    = strtoul(szEndLum, NULL, 10);

	if (g_tImgAdjCfg.dwEndLum != dwEndLum) {
		g_tImgAdjCfg.dwEndLum = dwEndLum;
		g_tImgAdjCfg.bModified = TRUE;
	}
}

/* ============================================================================================= */
void VencMaster_SetImageAdjustmentTolerance(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	DWORD dwTolerance = 0;
	CHAR szTolerance[len+1];
	memset(szTolerance ,0x0,sizeof(szTolerance));
	snprintf(szTolerance, len+1, "%s", szTemp);
	dwTolerance   = strtoul(szTolerance, NULL, 10);

	if (g_tImgAdjCfg.dwTolerance != dwTolerance) {
		g_tImgAdjCfg.dwTolerance = dwTolerance;
		g_tImgAdjCfg.bModified = TRUE;
	}
}

/* ============================================================================================= */
SCODE VencMaster_ImgAdjSetoptions(HANDLE hObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;
	EImgAdjMode	eImgAdjSelectMode = g_tImgAdjCfg.eImgAdjSelectMode;
#if defined(_AUTOSCENE_)
	if (isAutoSceneExist() && ptMasterInfo->bAutoSceneIsRun)
	{
		printf("[venc_master_conf](%d)Because autoscene is running, does not call %s!!\n",__LINE__,__func__);
		return S_OK;
	}
#endif
	if (g_tImgAdjCfg.bBrightModified == TRUE) {
		tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_BRIGHTNESS;
		tVideoSigOptions.adwUserData[0] = g_tImgAdjCfg.dwBrightness;
		if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) == S_OK) {
			DBPRINT1("[VENC_MASTER] SetBright %u\n", g_tImgAdjCfg.dwBrightness);
		} else {
			DBPRINT1("[VENC_MASTER] SetBright %u Fail\n", g_tImgAdjCfg.dwBrightness);
		}
		g_tImgAdjCfg.bBrightModified = FALSE;
	}
	if (g_tImgAdjCfg.bContrastModified == TRUE) {
		tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_CONTRAST;
		tVideoSigOptions.adwUserData[0] = g_tImgAdjCfg.dwContrast;
		if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) == S_OK) {
			DBPRINT1("[VENC_MASTER] SetContrast %u\n", g_tImgAdjCfg.dwContrast);
		} else {
			DBPRINT1("[VENC_MASTER] SetContrast %u Fail\n", g_tImgAdjCfg.dwContrast);
		}
		g_tImgAdjCfg.bContrastModified = FALSE;
	}
	if (g_tImgAdjCfg.bSaturationModified == TRUE) {
		tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_SATURATION;
		tVideoSigOptions.adwUserData[0] = g_tImgAdjCfg.dwSaturation;
		if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) == S_OK) {
			DBPRINT1("[VENC_MASTER] SetSaturation %u\n", g_tImgAdjCfg.dwSaturation);
		} else {
			DBPRINT1("[VENC_MASTER] SetSaturation %u Fail\n", g_tImgAdjCfg.dwSaturation);
		}
		g_tImgAdjCfg.bSaturationModified = FALSE;
#ifdef _VMA_IBPE
		{
			CHAR szCmd[1024];
			memset(szCmd, 0x0, sizeof(szCmd));
			snprintf(szCmd, sizeof(szCmd), "/usr/bin/msgctrl -f /tmp/venc/c%d/command.fifo  -h videoin -c ClearIBPEInternalBuffer", ptMasterInfo->dwDevNum);
			//printf("[venc_master_conf](%d)Execute szCmd:%s for clearing the IBPE internal buffer!!\n", __LINE__, szCmd);
			system(szCmd);
		}
#endif
	}

	if (g_tImgAdjCfg.bSelectModeModified == TRUE) {
		//If adding new three modes:(1)day,(2)night,(3)auto, must provide the new setoption().
		tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_AUTO_COLOR_SUPPRESSION;
		tVideoSigOptions.adwUserData[0] = (eImgAdjSelectMode == imgadjmDynamic)? TRUE : FALSE;
		tVideoSigOptions.adwUserData[1] = g_tImgAdjCfg.dwStartLum;
		tVideoSigOptions.adwUserData[2] = g_tImgAdjCfg.dwEndLum;
		// how to control dwTolerance ?
		if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) == S_OK) {
			DBPRINT3("[VENC_MASTER] Set Color Suppression (T/F, StartLum, EndLum) = (%s, %u, %u) \n", (tVideoSigOptions.adwUserData[0])? "TRUE" : "FALSE", tVideoSigOptions.adwUserData[1], tVideoSigOptions.adwUserData[2]);
		} else {
			DBPRINT0("[VENC_MASTER] Set Color Suppression Fail");
		}
		g_tImgAdjCfg.bSelectModeModified = FALSE;
	}	
	return S_OK;
}

/* ============================================================================================= */

#define WRDDB_NUM 9

typedef struct wdr_db_pair{
	DWORD  dwLevel;
	DWORD	 dwDBValue;
}TWDRdbPair;

const TWDRdbPair g_tWDRdbPair[WRDDB_NUM]=
{
	{0,0},
	{1,6},
	{2,12},
	{3,18},
	{4,24},
	{5,30},
	{6,36},
	{7,42},
	{8,48}
};

SCODE VencMaster_WDRSetoptions(HANDLE hObject)
{
#ifdef _WDR
	TMasterInfo *ptMasterInfo = (TMasterInfo *) hObject;
	TVideoSignalOptions tVideoSigOptions;

	BOOL bSetEnableOption = FALSE;

	memset(&tVideoSigOptions,0x0,sizeof(TVideoSignalOptions));
	if (ptMasterInfo->tWDRStatus.bModified == TRUE)//from msgctrl
	{
		tVideoSigOptions.adwUserData[0] = ptMasterInfo->tWDRStatus.bEnable;
		bSetEnableOption = TRUE;
	}
	else if (g_tWDRCfg.bEnableModified == TRUE)//from configuration files
	{
		tVideoSigOptions.adwUserData[0] =g_tWDRCfg.bEnable;
		bSetEnableOption = TRUE;
	}

	if (bSetEnableOption == TRUE)
	{
		tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_WDR;
		//printf("[venc_master_conf](%d)WDR Enable? %s\n", __LINE__,(tVideoSigOptions.adwUserData[0])? "TRUE": "FALSE");
		if (g_tWDRCfg.bEnable == TRUE)
		{
			goto check_level;
		}

		if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) {

			printf("[venc_master_conf](%d)VencMaster_WDRSetoptions (VIDEO_SIGNAL_OPTION_SET_WDR) fail \n",__LINE__);
			return S_FAIL;
		}
	}
check_level:    
	if (g_tWDRCfg.bEnable == TRUE)
	{
		memset(&tVideoSigOptions,0x0,sizeof(TVideoSignalOptions));
		if (g_tWDRCfg.bLevelValueModified == TRUE)
		{
			//Call setoption to set the level value.
			tVideoSigOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_WDR;
			tVideoSigOptions.adwUserData[0]=0;
			{
				int i = 0;
				for (i = 0; i < WRDDB_NUM; i++)
				{
					if (g_tWDRCfg.dwLevelValue == g_tWDRdbPair[i].dwDBValue)
					{
						tVideoSigOptions.adwUserData[0]=g_tWDRdbPair[i].dwLevel;

					}  
				}
			}
			//printf("[venc_master_conf](%d)The modifed level vlaue is %d (adwUserData[0] == %u).\n",__LINE__,g_tWDRCfg.dwLevelValue,tVideoSigOptions.adwUserData[0]);
			if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSigOptions) != S_OK) 
			{
				printf("[venc_master_conf](%d)VencMaster_WDRSetoptions (VIDEO_SIGNAL_OPTION_SET_WDR) fail \n",__LINE__);
				return S_FAIL;
			}

		}
	}
#else
//	printf("[venc_master_conf](%d)The firmware does not support WDR! \n",__LINE__);
#endif
	return S_OK;
}
