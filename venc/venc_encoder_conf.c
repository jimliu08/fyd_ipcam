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
 * venc_encoder_conf.c
 *
 * \brief
 * Video encoder process setoption implement.
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


//----------------CharDataHandler----------------
void VencEncoder_SetVideoCodecType(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetVideoResolution(HANDLE hObject, const CHAR* szParam, SDWORD len);
void VencEncoder_SetConvertingMethod(HANDLE hObject, const CHAR* szParam, SDWORD len);
void VencEncoder_SetVideoFrameRate(HANDLE hObject, const CHAR* szParam, SDWORD len);
void VencEncoder_SetMPEG4KeyInterval(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetMPEG4QualityType(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetMPEG4Quant(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetMPEG4Bitrate(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetMJPEGQuant(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetH264KeyInterval(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetH264QualityType(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetH264Quant(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetH264MaxQuant(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetH264MinQuant(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetH264Bitrate(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetH264SearchRangeX(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetH264SearchRangeY(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetH264RefFrameNum(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetH264MaxFrameRate(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetH264MinFrameRate(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetH264Deblock(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetH264Profile(HANDLE hObject, const CHAR *szParam, SDWORD len);

void VencEncoder_SetTextOverlayEnable(HANDLE hObject, const CHAR* szTextOverlayEnable, SDWORD len);
void VencEncoder_SetTextOverlayFontPath(HANDLE hObject, const CHAR* szTextOverlayFontPath, SDWORD len);
SCODE VencEncoder_ClearTextOverlayCustomText(HANDLE hObject, const CHAR *szParam, const CHAR **atts);
void VencEncoder_SetTextOverlayCustomText(HANDLE hObject, const CHAR* szTextOverlayCustomText, SDWORD len);
void VencEncoder_SetTextOverlayEnableMilliSec(HANDLE hObject, const CHAR* szTextOverlayEnableTimeStamp, SDWORD len);
void VencEncoder_SetSVCKeyInterval(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetSVCQualityType(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetSVCQuant(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetSVCBitrate(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetSVCHDREnable(HANDLE hObject, const CHAR *szParam, SDWORD len);
void VencEncoder_SetSVCLayerNum(HANDLE hObject, const CHAR *szParam, SDWORD len);
/*ROI XML conf callback functions*/
void VencEncoder_SetROIEnable(HANDLE hObject, const CHAR *szEnable, SDWORD len);
void VencEncoder_SetROIXTranslate(HANDLE hObject, const CHAR *szXTranslate, SDWORD len);
void VencEncoder_SetROIYTranslate(HANDLE hObject, const CHAR *szYTranslate, SDWORD len);
void VencEncoder_SetROIXScale(HANDLE hObject, const CHAR *szXScale, SDWORD len);
void VencEncoder_SetROIYScale(HANDLE hObject, const CHAR *szYScale, SDWORD len);
SCODE VencEncoder_SetROIWinTag(HANDLE hObject, const CHAR *szID, const CHAR** atts);
void VencEncoder_SetROIWinEnable(HANDLE hObject, const CHAR *szEnable, SDWORD len);
void VencEncoder_SetROIWinName(HANDLE hObject, const CHAR *szName, SDWORD len);
void VencEncoder_SetROIWinLeft(HANDLE hObject, const CHAR *szLeft, SDWORD len);
void VencEncoder_SetROIWinTop(HANDLE hObject, const CHAR *szTop, SDWORD len);
void VencEncoder_SetROIWinWidth(HANDLE hObject, const CHAR *szWidth, SDWORD len);
void VencEncoder_SetROIWinHeight(HANDLE hObject, const CHAR *szHeight, SDWORD len);
void VencEncoder_SetROIDeltaQp(HANDLE hObject, const CHAR *szDeltaQp, SDWORD len);
void VencEncoder_SetROIEncodingInterval(HANDLE hObject, const CHAR *szEncodingInterval, SDWORD len);
/*Randomly Cropping XML conf callback functions*/
void VencEncoder_SetCropTop(HANDLE hObject, const CHAR *szTemp, SDWORD len);
void VencEncoder_SetCropLeft(HANDLE hObject, const CHAR *szTemp, SDWORD len);
void VencEncoder_SetCropWidth(HANDLE hObject, const CHAR *szTemp, SDWORD len);
void VencEncoder_SetCropHeight(HANDLE hObject, const CHAR *szTemp, SDWORD len);

/* =========================================================================================== */
TXmlWrapperTreeMap ptEncoderTreeMap[] =
{
	{"root/venc_encoder/codec", NULL, &VencEncoder_SetVideoCodecType, NULL},
	{"root/venc_encoder/resolution", NULL, &VencEncoder_SetVideoResolution, NULL},
	{"root/venc_encoder/converting_method", NULL, &VencEncoder_SetConvertingMethod, NULL},
	{"root/venc_encoder/crop/top", NULL, &VencEncoder_SetCropTop, NULL},
	{"root/venc_encoder/crop/left", NULL, &VencEncoder_SetCropLeft, NULL},
	{"root/venc_encoder/crop/width", NULL, &VencEncoder_SetCropWidth, NULL},
	{"root/venc_encoder/crop/height", NULL, &VencEncoder_SetCropHeight, NULL},
	{"root/venc_encoder/frame_rate", NULL, &VencEncoder_SetVideoFrameRate, NULL},
	{"root/venc_encoder/mpeg4_key_interval", NULL, &VencEncoder_SetMPEG4KeyInterval, NULL},
	{"root/venc_encoder/mpeg4_quality_type", NULL, &VencEncoder_SetMPEG4QualityType, NULL},
	{"root/venc_encoder/mpeg4_quant_value", NULL, &VencEncoder_SetMPEG4Quant, NULL},
	{"root/venc_encoder/mpeg4_bitrate", NULL, &VencEncoder_SetMPEG4Bitrate, NULL},
	{"root/venc_encoder/mjpeg_quant_value", NULL, &VencEncoder_SetMJPEGQuant, NULL},
	{"root/venc_encoder/h264_key_interval", NULL, &VencEncoder_SetH264KeyInterval, NULL},
	{"root/venc_encoder/h264_quality_type", NULL, &VencEncoder_SetH264QualityType, NULL},
	{"root/venc_encoder/h264_quant_value", NULL, &VencEncoder_SetH264Quant, NULL},
	{"root/venc_encoder/h264_max_quant_value",NULL, &VencEncoder_SetH264MaxQuant, NULL},
	{"root/venc_encoder/h264_min_quant_value",NULL, &VencEncoder_SetH264MinQuant, NULL},
	{"root/venc_encoder/h264_bitrate", NULL, &VencEncoder_SetH264Bitrate, NULL},
	{"root/venc_encoder/h264_search_range_x",NULL, &VencEncoder_SetH264SearchRangeX, NULL},
	{"root/venc_encoder/h264_search_range_y",NULL, &VencEncoder_SetH264SearchRangeY, NULL},
	{"root/venc_encoder/h264_ref_frame_num",NULL, &VencEncoder_SetH264RefFrameNum, NULL},
	{"root/venc_encoder/h264_max_frame_rate",NULL, &VencEncoder_SetH264MaxFrameRate, NULL},
	{"root/venc_encoder/h264_min_frame_rate",NULL, &VencEncoder_SetH264MinFrameRate, NULL},
	{"root/venc_encoder/h264_deblock",NULL, &VencEncoder_SetH264Deblock, NULL},
	{"root/venc_encoder/h264_profile",NULL, &VencEncoder_SetH264Profile, NULL},
	{"root/venc_encoder/svc_key_interval", NULL, &VencEncoder_SetSVCKeyInterval, NULL},
	{"root/venc_encoder/svc_quality_type", NULL, &VencEncoder_SetSVCQualityType, NULL},
	{"root/venc_encoder/svc_quant_value", NULL, &VencEncoder_SetSVCQuant, NULL},
	{"root/venc_encoder/svc_bitrate", NULL, &VencEncoder_SetSVCBitrate, NULL},
	{"root/venc_encoder/svc_hdr_enable", NULL, &VencEncoder_SetSVCHDREnable, NULL},
	{"root/venc_encoder/svc_layer_num", NULL, &VencEncoder_SetSVCLayerNum, NULL},
	//ROI
	{"root/venc_encoder/roi/enable", NULL, &VencEncoder_SetROIEnable, NULL},
	{"root/venc_encoder/roi/Transformation/Translate/x", NULL, &VencEncoder_SetROIXTranslate, NULL},
	{"root/venc_encoder/roi/Transformation/Translate/y", NULL, &VencEncoder_SetROIYTranslate, NULL},
	{"root/venc_encoder/roi/Transformation/scale/x", NULL, &VencEncoder_SetROIXScale, NULL},
	{"root/venc_encoder/roi/Transformation/scale/y", NULL, &VencEncoder_SetROIYScale, NULL},
	{"root/venc_encoder/roi/win", &VencEncoder_SetROIWinTag, NULL, NULL},
	{"root/venc_encoder/roi/win/active", NULL, &VencEncoder_SetROIWinEnable, NULL},
	{"root/venc_encoder/roi/win/name", NULL, &VencEncoder_SetROIWinName, NULL},
	{"root/venc_encoder/roi/win/left", NULL, &VencEncoder_SetROIWinLeft, NULL},
	{"root/venc_encoder/roi/win/top", NULL, &VencEncoder_SetROIWinTop, NULL},
	{"root/venc_encoder/roi/win/width", NULL, &VencEncoder_SetROIWinWidth, NULL},
	{"root/venc_encoder/roi/win/height", NULL, &VencEncoder_SetROIWinHeight, NULL},
	{"root/venc_encoder/roi/win/QP", NULL, &VencEncoder_SetROIDeltaQp, NULL},
	{"root/venc_encoder/roi/win/interval", NULL, &VencEncoder_SetROIEncodingInterval, NULL},
	// text overlay
	{"root/venc_encoder/text_overlay/enable", NULL, &VencEncoder_SetTextOverlayEnable, NULL},
	{"root/venc_encoder/text_overlay/font", NULL, &VencEncoder_SetTextOverlayFontPath, NULL},
	{"root/venc_encoder/text_overlay/format/custom_text", &VencEncoder_ClearTextOverlayCustomText, &VencEncoder_SetTextOverlayCustomText, NULL},
	{"root/venc_encoder/text_overlay/format/enable_millisec", NULL, &VencEncoder_SetTextOverlayEnableMilliSec, NULL},
	{NULL, NULL, NULL}
};
/* =========================================================================================== */
void VencEncoder_SetCropTop(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	DWORD dwTemp = strtoul(szTemp, NULL, 10);

	if (dwTemp != ptEncoderInfo->tCropInfo.dwStartY)
	{
		ptEncoderInfo->tCropInfo.dwStartY = dwTemp;
		 ptEncoderInfo->tCropInfo.dwStartY = ((ptEncoderInfo->tCropInfo.dwStartY+ 15) >> 4) << 4;
		ptEncoderInfo->bChangeRandomCropping = TRUE;
	}
	return;
}
/* =========================================================================================== */
void VencEncoder_SetCropLeft(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	DWORD dwTemp = strtoul(szTemp, NULL, 10);

	if (dwTemp != ptEncoderInfo->tCropInfo.dwStartX)
	{
		ptEncoderInfo->tCropInfo.dwStartX = dwTemp;
		ptEncoderInfo->tCropInfo.dwStartX = ((ptEncoderInfo->tCropInfo.dwStartX+ 15) >> 4) << 4;
		ptEncoderInfo->bChangeRandomCropping = TRUE;
	}
	return;
}
/* =========================================================================================== */
void VencEncoder_SetCropWidth(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{	
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	DWORD dwTemp = strtoul(szTemp, NULL, 10);

	if (dwTemp != ptEncoderInfo->tCropInfo.dwWidth)
	{
		ptEncoderInfo->tCropInfo.dwWidth = dwTemp;
		ptEncoderInfo->bChangeRandomCropping = TRUE;
	}
	return;
}
/* =========================================================================================== */
void VencEncoder_SetCropHeight(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	DWORD dwTemp = strtoul(szTemp, NULL, 10);

	if (dwTemp != ptEncoderInfo->tCropInfo.dwHeight)
	{
		ptEncoderInfo->tCropInfo.dwHeight = dwTemp;
		ptEncoderInfo->bChangeRandomCropping = TRUE;
	}
	return;
}
/* =========================================================================================== */
void VencEncoder_SetVideoCodecType(HANDLE hObject, const CHAR *szParam, SDWORD len)
{   
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR *szCodec = NULL;
	DWORD dwCodecType = -1;
	szCodec = (CHAR *)malloc(len+1);
	memset(szCodec,0x0,len+1);
	// Codec type
	snprintf(szCodec, len+1, "%s", szParam);


	if (strcasecmp(szCodec, "mpeg4") == 0) {
		dwCodecType = VIDEO_CODEC_MPEG4;
	} 
	else if (strcasecmp(szCodec, "mjpeg") == 0) {
		dwCodecType = VIDEO_CODEC_MJPEG;
	}
	else if (strcasecmp(szCodec, "h264") == 0) {
		dwCodecType = VIDEO_CODEC_H264;
	}
	else if (strcasecmp(szCodec, "svc") == 0) {
		dwCodecType = VIDEO_CODEC_SVC;
	}
	else {
		//===== type is not correct!!
		fprintf(stderr, "%s:%d CODEC TYPE is not recognized!\n", __FILE__, __LINE__);
		return;
	}

	// set CodecType
	if (ptEncoderInfo->tVideoSlaveCfg.dwCodecType != dwCodecType) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_CODEC;

		tEncoderOpt.adwUserData[0] =vpmStreamEncJPEG;
#ifdef _VMA_MEBE	
		if (dwCodecType == VIDEO_CODEC_MPEG4)
			tEncoderOpt.adwUserData[0] = vpmStreamEncMP4V;
#endif
#ifdef _VMA_JEBE 
		if (dwCodecType == VIDEO_CODEC_MJPEG)
			tEncoderOpt.adwUserData[0] = vpmStreamEncJPEG;
#endif	
#ifdef _H4EE
		if (dwCodecType == VIDEO_CODEC_H264)
			tEncoderOpt.adwUserData[0] = vpmStreamEncH264;
#endif
#ifdef _SVC
		if (dwCodecType == VIDEO_CODEC_SVC)
			tEncoderOpt.adwUserData[0] = vpmStreamEncSVC;
#endif
		VencEncoder_SetOptions(hObject, tEncoderOpt);
		ptEncoderInfo->tVideoSlaveCfg.dwCodecType = dwCodecType;

#ifdef _VMA_MEBE
		if (dwCodecType == VIDEO_CODEC_MPEG4) {
			printf("[VENC_ENCODER_CONF] Codec %s\n", "mpeg4");
		} 
#endif
#ifdef _VMA_JEBE 
		if (dwCodecType == VIDEO_CODEC_MJPEG) {
			printf("[VENC_ENCODER_CONF] Codec %s\n", "mjpeg");
		} 
#endif
#ifdef _H4EE
		if (dwCodecType == VIDEO_CODEC_H264) {
			printf("[VENC_ENCODER_CONF] Codec %s\n", "H264");
		}
#endif	
#ifdef _SVC
		if (dwCodecType == VIDEO_CODEC_SVC) {
			printf("[VENC_ENCODER_CONF] Codec %s\n", "SVC");
		}
#endif
	}  
	if (szCodec != NULL)
	{
		free(szCodec);
		szCodec = NULL;
	}
}

/* =========================================================================================== */
void VencEncoder_SetVideoFrameRate(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	DWORD dwFrameRate;
	CHAR szTemp[len+1];
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwFrameRate = strtoul(szTemp, NULL, 10);
	if (ptEncoderInfo->tVideoSlaveCfg.dwFrameRate != dwFrameRate) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_FRAMERATE;
		tEncoderOpt.adwUserData[0] = dwFrameRate;
		VencEncoder_SetOptions(hObject, tEncoderOpt);

		ptEncoderInfo->tVideoSlaveCfg.dwFrameRate = dwFrameRate;
		DBPRINT1("[VENC_ENCODER] FrameRate %u\n", ptEncoderInfo->tVideoSlaveCfg.dwFrameRate);
	}
}
DWORD vfCurFormat = -1;
/* =========================================================================================== */
EVideoFormat VencEncoder_Res2vFormat(const CHAR* szResolution, DWORD dwVideoResolutionFormatIndex)
{
	char    szRes[MAX_VIDEO_RESOLUTION_LEN + 1];
	char    *szToken = NULL;
	DWORD   dwWidth, dwHeight, dwFormat;

	strncpy(szRes, szResolution, MAX_VIDEO_RESOLUTION_LEN + 1);
	szToken = strchr(szRes, 'x');
	if (szToken == NULL) return vfNum;
	*szToken = '\0';
	dwWidth = strtol(szRes, (char**) NULL, 10);
	dwHeight = strtol(szToken + 1, (char**) NULL, 10);

	dwFormat = VideoComm_WH2vFormat(dwWidth, dwHeight);

	if (dwFormat == vfNum) {

		
			g_avfInfo[dwVideoResolutionFormatIndex].wFieldWidth = 0;      
			g_avfInfo[dwVideoResolutionFormatIndex].wFieldHeight = 0;
			g_avfInfo[dwVideoResolutionFormatIndex].wVideoWidth = dwWidth;
			g_avfInfo[dwVideoResolutionFormatIndex].wVideoHeight = dwHeight;
			g_avfInfo[dwVideoResolutionFormatIndex].wEncWidth = dwWidth;
			g_avfInfo[dwVideoResolutionFormatIndex].wEncHeight = dwHeight;
			g_avfInfo[dwVideoResolutionFormatIndex].wXOffset = 0;
			g_avfInfo[dwVideoResolutionFormatIndex].wYOffset = 0;
			g_avfInfo[dwVideoResolutionFormatIndex].wFieldNum = 1;
			g_avfInfo[dwVideoResolutionFormatIndex].wFrameRate = 30;
			g_avfInfo[dwVideoResolutionFormatIndex].dwFieldBufSize = dwWidth*dwHeight;
			dwFormat = dwVideoResolutionFormatIndex;
		
	}
	printf("dwFormat = %d\n",dwFormat);
	return dwFormat;
}
/* =========================================================================================== */
void VencEncoder_SetVideoResolution(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	CHAR szTemp[len+1];
	DWORD vfFormat;
	TEncoderOptions tEncoderOpt;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	vfFormat = VencEncoder_Res2vFormat(szTemp, ptEncoderInfo->dwVideoResolutionFormatIndex);

	if (vfFormat == vfNum 
			|| (g_avfInfo[vfFormat].wEncWidth > ptEncoderInfo->_tInitNeedInfo.dwVideoMaxWidth)
			|| (g_avfInfo[vfFormat].wEncHeight > ptEncoderInfo->_tInitNeedInfo.dwVideoMaxHeight)) {
		vfFormat = vfPVGA;
		fprintf(stderr, "[VENC_ENCODER] Not regular resolution, set to VGA (640x480) !!\n");
	}

	tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_RESOLUTION;
	tEncoderOpt.adwUserData[0] = vfFormat;

	VencEncoder_SetOptions(hObject, tEncoderOpt);

	strncpy(ptEncoderInfo->tVideoSlaveCfg.szResolution, szTemp, len+1);
	ptEncoderInfo->tVideoSlaveCfg.vfFormat = vfFormat;
	ptEncoderInfo->tROI2D.fXScale = (FLOAT) (2)/g_avfInfo[vfFormat].wEncWidth; 		
	ptEncoderInfo->tROI2D.fYScale = (FLOAT) (-2)/g_avfInfo[vfFormat].wEncHeight; 

	printf("[VENC_ENCODER_CONF](%d) Resolution changed as %u x %u \n",__LINE__,g_avfInfo[vfFormat].wEncWidth,g_avfInfo[vfFormat].wEncHeight);

}
/* =========================================================================================== */
void VencEncoder_SetConvertingMethod(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)hObject;
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	EConvertingMethod ecMethod;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);

	if (strcmp(szTemp, "cropping") == 0) {
		ecMethod = ecmCropping;
	}else if (strcmp(szTemp, "randomcropping") == 0) {
		ecMethod = ecmRandomCropping;
	} else {
		ecMethod = ecmSqueezing;
	}

	if (ptEncoderInfo->tVideoSlaveCfg.ecMethod != ecMethod) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_CONVERTINGMETHOD;
		//printf(__FILE__":%d: cropping \n", __LINE__);
		tEncoderOpt.adwUserData[0] = ecMethod;
		VencEncoder_SetOptions(hObject, tEncoderOpt);

		ptEncoderInfo->tVideoSlaveCfg.ecMethod = ecMethod;
	}
}
/* =========================================================================================== */
void VencEncoder_SetMPEG4KeyInterval(HANDLE hObject, const CHAR *szParam, SDWORD len)
{   
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwKeyInterval;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwKeyInterval = strtoul(szTemp, NULL, 10);

	if (ptEncoderInfo->tVideoSlaveCfg.dwMPEG4KeyInterval != dwKeyInterval) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_MPEG4_INTRAINTERVAL;
		tEncoderOpt.adwUserData[0] = dwKeyInterval;
		VencEncoder_SetOptions(hObject, tEncoderOpt);

		ptEncoderInfo->tVideoSlaveCfg.dwMPEG4KeyInterval = dwKeyInterval;
		DBPRINT1("[VENC_ENCODER] MPEG4 KeyInterval %u\n", tEncoderOpt.adwUserData[0]);
	}
}
/* =========================================================================================== */
void VencEncoder_SetMPEG4QualityType(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)hObject;
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	ERateCtrlFlags ercfType;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	if (strcmp(szTemp, "VQCB") == 0) {
		ercfType = RATE_CTRL_VQCB;
	} else if (strcmp(szTemp, "CQCB") == 0) {
		ercfType = RATE_CTRL_CQCB;
	} else if (strcmp(szTemp, "CVBR") == 0) {
		ercfType = RATE_CTRL_CVBR;
	} else {
		ercfType = RATE_CTRL_NONE;
	}
#if 0
	if (ptEncoderInfo->tVideoSlaveCfg.ercfMPEG4QualityType != ercfType) {
#endif
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_MPEG4_RATECTRL;
		tEncoderOpt.adwUserData[0] = ercfType;
		VencEncoder_SetOptions(hObject, tEncoderOpt);

		ptEncoderInfo->tVideoSlaveCfg.ercfMPEG4QualityType = ercfType;
		if (tEncoderOpt.adwUserData[0] == RATE_CTRL_NONE) {
			DBPRINT0("[VENC_ENCODER] MPEG4 RateCtrl method : NONE\n");
		} else if (tEncoderOpt.adwUserData[0] == RATE_CTRL_VQCB) {
			DBPRINT0("[VENC_ENCODER] MPEG4 RateCtrl method : VQCB\n");
		} else if (tEncoderOpt.adwUserData[0] == RATE_CTRL_CQCB) {
			DBPRINT0("[VENC_ENCODER] MPEG4 RateCtrl method : CQCB\n");
		} else if (tEncoderOpt.adwUserData[0] == RATE_CTRL_CVBR) {
			DBPRINT0("[VENC_ENCODER] MPEG4 RateCtrl method : CVBR\n");
		} else {
			DBPRINT0("[VENC_ENCODER] MPEG4 RateCtrl method : unsupported method\n");
		}
#if 0      
	}
#endif
}
/* =========================================================================================== */
void VencEncoder_SetMPEG4Quant(HANDLE hObject, const CHAR *szParam, SDWORD len)
{       
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwQuantValue;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwQuantValue = strtoul(szTemp, NULL, 10);
#if 0	
	if (ptEncoderInfo->tVideoSlaveCfg.dwMPEG4QuantValue != dwQuantValue) {
#endif
		if (dwQuantValue > MAX_MPEG4_QUANT) {
			dwQuantValue = MAX_MPEG4_QUANT;
		} else if (dwQuantValue < MIN_MPEG4_QUANT) {
			dwQuantValue = MIN_MPEG4_QUANT;
		}

		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_MPEG4_QUANT;
		tEncoderOpt.adwUserData[0] = dwQuantValue;
		VencEncoder_SetOptions(hObject, tEncoderOpt);

		ptEncoderInfo->tVideoSlaveCfg.dwMPEG4QuantValue = dwQuantValue;         
		DBPRINT1("[VENC_ENCODER] MPEG4 Quant Value %u\n", ptEncoderInfo->tVideoSlaveCfg.dwMPEG4QuantValue);
#if 0		
	}
#endif
}
/* =========================================================================================== */
void VencEncoder_SetMPEG4Bitrate(HANDLE hObject, const CHAR *szParam, SDWORD len)
{       
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwBitrate;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwBitrate = strtoul(szTemp, NULL, 10);
#if 0    
	if (ptEncoderInfo->tVideoSlaveCfg.dwMPEG4Bitrate != dwBitrate) {
#endif
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_MPEG4_BITRATE;
		tEncoderOpt.adwUserData[0] = dwBitrate;
		VencEncoder_SetOptions(hObject, tEncoderOpt);
		ptEncoderInfo->tVideoSlaveCfg.dwMPEG4Bitrate = dwBitrate;
		DBPRINT1("[VENC_ENCODER] MPEG4 Bitrate %u\n", ptEncoderInfo->tVideoSlaveCfg.dwMPEG4Bitrate);
#if 0		
	}
#endif
}
/* =========================================================================================== */
void VencEncoder_SetMJPEGQuant(HANDLE hObject, const CHAR *szParam, SDWORD len)
{       
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwQuantValue;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwQuantValue = strtoul(szTemp, NULL, 10);
#if 0
	if (ptEncoderInfo->tVideoSlaveCfg.dwMJPEGQuantValue != dwQuantValue) {
#endif		
		if (dwQuantValue > MAX_MJPEG_QUANT) {
			dwQuantValue = MAX_MJPEG_QUANT;
		} else if (dwQuantValue < MIN_MJPEG_QUANT) {
			dwQuantValue = MIN_MJPEG_QUANT;
		}

		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_MJPEG_QUANT;
		tEncoderOpt.adwUserData[0] = dwQuantValue;
		VencEncoder_SetOptions(hObject, tEncoderOpt);

		ptEncoderInfo->tVideoSlaveCfg.dwMJPEGQuantValue = dwQuantValue;         
		DBPRINT1("[VENC_ENCODER] MJPEG Quant Value %u\n", ptEncoderInfo->tVideoSlaveCfg.dwMJPEGQuantValue);
#if 0		
	}
#endif    
}
/* =========================================================================================== */
void VencEncoder_SetH264KeyInterval(HANDLE hObject, const CHAR *szParam, SDWORD len)
{   
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwKeyInterval;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwKeyInterval = strtoul(szTemp, NULL, 10);

	if (ptEncoderInfo->tVideoSlaveCfg.dwH264KeyInterval != dwKeyInterval) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_H264_INTRAINTERVAL;
		tEncoderOpt.adwUserData[0] = dwKeyInterval;
		VencEncoder_SetOptions(hObject, tEncoderOpt);

		ptEncoderInfo->tVideoSlaveCfg.dwH264KeyInterval = dwKeyInterval;
		DBPRINT1("[VENC_ENCODER] H264 KeyInterval %u\n", tEncoderOpt.adwUserData[0]);
	}
}
/* =========================================================================================== */
void VencEncoder_SetH264QualityType(HANDLE hObject, const CHAR *szParam, SDWORD len)
{    
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)hObject;
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	ERateCtrlFlags ercfType;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	if (strcmp(szTemp, "VQCB") == 0) {
		ercfType = RATE_CTRL_VQCB;
	} else if (strcmp(szTemp, "CQCB") == 0) {
		ercfType = RATE_CTRL_CQCB;
	} else if (strcmp(szTemp, "CVBR") == 0) {
		ercfType = RATE_CTRL_CVBR;
	}else if (strcmp(szTemp, "ADPT") == 0) {
		ercfType = RATE_CTRL_ADPT; 
	}else {
		ercfType = RATE_CTRL_NONE;
	}
#if 0   
	if (ptEncoderInfo->tVideoSlaveCfg.ercfH264QualityType != ercfType) {
#endif
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_H264_RATECTRL;
		tEncoderOpt.adwUserData[0] = ercfType;
		VencEncoder_SetOptions(hObject, tEncoderOpt);

		ptEncoderInfo->tVideoSlaveCfg.ercfH264QualityType = ercfType;
		if (tEncoderOpt.adwUserData[0] == RATE_CTRL_NONE) {
			DBPRINT0("[VENC_ENCODER] H264 RateCtrl method : NONE\n");
		} else if (tEncoderOpt.adwUserData[0] == RATE_CTRL_VQCB) {
			DBPRINT0("[VENC_ENCODER] H264 RateCtrl method : VQCB\n");
		} else if (tEncoderOpt.adwUserData[0] == RATE_CTRL_CQCB) {
			DBPRINT0("[VENC_ENCODER] H264 RateCtrl method : CQCB\n");
		} else if (tEncoderOpt.adwUserData[0] == RATE_CTRL_CVBR) {
			DBPRINT0("[VENC_ENCODER] H264 RateCtrl method : CVBR\n");
		}else if (tEncoderOpt.adwUserData[0] == RATE_CTRL_ADPT) {
			DBPRINT0("[VENC_ENCODER] H264 RateCtrl method : ADPT\n");
		} else {
			DBPRINT0("[VENC_ENCODER] H264 RateCtrl method : unsupported method\n");
		}
#if 0        
	}
#endif    
}
/* =========================================================================================== */
void VencEncoder_SetH264Quant(HANDLE hObject, const CHAR *szParam, SDWORD len)
{       
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwQuantValue;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwQuantValue = strtoul(szTemp, NULL, 10);
#if 0	
	if (ptEncoderInfo->tVideoSlaveCfg.dwH264QuantValue != dwQuantValue) {
#endif		
		if (dwQuantValue > MAX_H264_QUANT) {
			dwQuantValue = MAX_H264_QUANT;
		} else if (dwQuantValue < MIN_H264_QUANT) {
			dwQuantValue = MIN_H264_QUANT;
		}

		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_H264_QUANT;
		tEncoderOpt.adwUserData[0] = dwQuantValue;
		VencEncoder_SetOptions(hObject, tEncoderOpt);

		ptEncoderInfo->tVideoSlaveCfg.dwH264QuantValue = dwQuantValue;         
		DBPRINT1("[VENC_ENCODER] H264 Quant Value %u\n", ptEncoderInfo->tVideoSlaveCfg.dwH264QuantValue);

#if 0	
	}
#endif	
}
/* =========================================================================================== */
void VencEncoder_SetH264Bitrate(HANDLE hObject, const CHAR *szParam, SDWORD len)
{       
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwBitrate;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwBitrate = strtoul(szTemp, NULL, 10);
#if 0  
	if (ptEncoderInfo->tVideoSlaveCfg.dwH264Bitrate != dwBitrate) {
#endif
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_H264_BITRATE;
		tEncoderOpt.adwUserData[0] = dwBitrate;
		VencEncoder_SetOptions(hObject, tEncoderOpt);
		ptEncoderInfo->tVideoSlaveCfg.dwH264Bitrate = dwBitrate;
		DBPRINT1("[VENC_ENCODER] H264 Bitrate %u\n", ptEncoderInfo->tVideoSlaveCfg.dwH264Bitrate);
#if 0		
	}
#endif    
}
/* =========================================================================================== */
void VencEncoder_SetH264SearchRangeX(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwSearchRangeX = 0;
	
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwSearchRangeX = strtoul(szTemp, NULL, 10);
	
	if (ptEncoderInfo->tVideoSlaveCfg.dwH264SearchRangeX != dwSearchRangeX) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_H264_SEARCH_RANGE_X;
		tEncoderOpt.adwUserData[0] = dwSearchRangeX;
		VencEncoder_SetOptions(hObject, tEncoderOpt);
		ptEncoderInfo->tVideoSlaveCfg.dwH264SearchRangeX = dwSearchRangeX;
		debug_printf("[VENC_ENCODER_CONF](%d) H264 Search Range X %u\n",__LINE__, ptEncoderInfo->tVideoSlaveCfg.dwH264SearchRangeX);
	}
}
void VencEncoder_SetH264SearchRangeY(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwSearchRangeY = 0;

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwSearchRangeY = strtoul(szTemp, NULL, 10);

	if (ptEncoderInfo->tVideoSlaveCfg.dwH264SearchRangeY != dwSearchRangeY) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_H264_SEARCH_RANGE_Y;
		tEncoderOpt.adwUserData[0] = dwSearchRangeY;
		VencEncoder_SetOptions(hObject, tEncoderOpt);
		ptEncoderInfo->tVideoSlaveCfg.dwH264SearchRangeY = dwSearchRangeY;
		debug_printf("[VENC_ENCODER_CONF](%d) H264 Search Range Y %u\n",__LINE__, ptEncoderInfo->tVideoSlaveCfg.dwH264SearchRangeY);
	}
	
}
void VencEncoder_SetH264RefFrameNum(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwRefFrameNum = 0;

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwRefFrameNum = strtoul(szTemp, NULL, 10);

	if (ptEncoderInfo->tVideoSlaveCfg.dwH264RefFrameNum != dwRefFrameNum) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_H264_REFERENCE_FRAME_NUM;
		tEncoderOpt.adwUserData[0] = dwRefFrameNum;
		VencEncoder_SetOptions(hObject, tEncoderOpt);
		ptEncoderInfo->tVideoSlaveCfg.dwH264RefFrameNum = dwRefFrameNum;
		debug_printf("[VENC_ENCODER_CONF](%d) H264 Refernce Frame Num %u\n",__LINE__, ptEncoderInfo->tVideoSlaveCfg.dwH264RefFrameNum);
	}
}	
void VencEncoder_SetH264MaxFrameRate(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwMaxFrameRate = 0;

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwMaxFrameRate = strtoul(szTemp, NULL, 10);

	if (ptEncoderInfo->tVideoSlaveCfg.dwH264MaxFrameRate != dwMaxFrameRate) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_H264_MAX_FRAME_RATE;
		tEncoderOpt.adwUserData[0] = dwMaxFrameRate;
		VencEncoder_SetOptions(hObject, tEncoderOpt);
		ptEncoderInfo->tVideoSlaveCfg.dwH264MaxFrameRate = dwMaxFrameRate;
		debug_printf("[VENC_ENCODER_CONF](%d) H264 Max Frame Rate %u\n",__LINE__, ptEncoderInfo->tVideoSlaveCfg.dwH264MaxFrameRate);
	}
}
void VencEncoder_SetH264MinFrameRate(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwMinFrameRate = 0;

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwMinFrameRate = strtoul(szTemp, NULL, 10);

	if (ptEncoderInfo->tVideoSlaveCfg.dwH264MinFrameRate != dwMinFrameRate) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_H264_MIN_FRAME_RATE;
		tEncoderOpt.adwUserData[0] = dwMinFrameRate;
		VencEncoder_SetOptions(hObject, tEncoderOpt);
		ptEncoderInfo->tVideoSlaveCfg.dwH264MinFrameRate = dwMinFrameRate;
		printf("[VENC_ENCODER_CONF](%d) H264 Min Frame Rate %u\n",__LINE__, ptEncoderInfo->tVideoSlaveCfg.dwH264MinFrameRate);
	}
	
}

void VencEncoder_SetH264Deblock(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	BOOL bDeblockEnable = FALSE;
	
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	
	if (strcasecmp(szTemp, "on") == 0) 
	{
		bDeblockEnable = TRUE;
	}
	if (ptEncoderInfo->tVideoSlaveCfg.bDeblockEnable != bDeblockEnable) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_H264_DEBLOCKENABLE;
		tEncoderOpt.adwUserData[0] = bDeblockEnable;
		VencEncoder_SetOptions(hObject, tEncoderOpt);
		ptEncoderInfo->tVideoSlaveCfg.bDeblockEnable = bDeblockEnable;
		printf("[VENC_ENCODER_CONF] H264 Deblock enable: %s\n",ptEncoderInfo->tVideoSlaveCfg.bDeblockEnable?"TRUE":"FALSE");
	}
}

void VencEncoder_SetH264Profile(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwProfile = 0;

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwProfile = strtoul(szTemp, NULL, 10);

	if (ptEncoderInfo->tVideoSlaveCfg.dwH264Profile != dwProfile) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_H264_PRFOFILE;
		tEncoderOpt.adwUserData[0] = dwProfile;
		VencEncoder_SetOptions(hObject, tEncoderOpt);
		ptEncoderInfo->tVideoSlaveCfg.dwH264Profile = dwProfile;
		printf("[VENC_ENCODER_CONF](%d) H264 Profile %u\n",__LINE__, ptEncoderInfo->tVideoSlaveCfg.dwH264Profile);
	}
	
}
	
void VencEncoder_SetH264MaxQuant(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwMaxQuant = 0;

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwMaxQuant = strtoul(szTemp, NULL, 10);

	if (ptEncoderInfo->tVideoSlaveCfg.dwH264MaxQuant != dwMaxQuant) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_H264_MAX_QUANT;
		tEncoderOpt.adwUserData[0] = dwMaxQuant;
		VencEncoder_SetOptions(hObject, tEncoderOpt);
		ptEncoderInfo->tVideoSlaveCfg.dwH264MaxQuant = dwMaxQuant;
		debug_printf("[VENC_ENCODER_CONF](%d) H264 Max Quant %u\n",__LINE__, ptEncoderInfo->tVideoSlaveCfg.dwH264MaxQuant);
	}
	
}
void VencEncoder_SetH264MinQuant(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwMinQuant = 0;

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwMinQuant = strtoul(szTemp, NULL, 10);

	if (ptEncoderInfo->tVideoSlaveCfg.dwH264MinQuant != dwMinQuant) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_H264_MIN_QUANT;
		tEncoderOpt.adwUserData[0] = dwMinQuant;
		VencEncoder_SetOptions(hObject, tEncoderOpt);
		ptEncoderInfo->tVideoSlaveCfg.dwH264MinQuant = dwMinQuant;
		printf("[VENC_ENCODER_CONF](%d) H264 Min Quant %u\n",__LINE__, ptEncoderInfo->tVideoSlaveCfg.dwH264MinQuant);
	}
}

/* =========================================================================================== */
void VencEncoder_SetSVCKeyInterval(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwKeyInterval;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwKeyInterval = strtoul(szTemp, NULL, 10);

	if (ptEncoderInfo->tVideoSlaveCfg.dwSVCKeyInterval != dwKeyInterval) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_SVC_INTRAINTERVAL;
		tEncoderOpt.adwUserData[0] = dwKeyInterval;
		VencEncoder_SetOptions(hObject, tEncoderOpt);

		ptEncoderInfo->tVideoSlaveCfg.dwSVCKeyInterval = dwKeyInterval;
		//printf("[VENC_ENCODER_CONF] SVC KeyInterval %u\n", tEncoderOpt.adwUserData[0]);
	}

	return;
}
/* =========================================================================================== */
void VencEncoder_SetSVCQualityType(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)hObject;
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	ERateCtrlFlags ercfType;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	if (strcmp(szTemp, "VQCB") == 0) {
		ercfType = RATE_CTRL_VQCB;
	} else if (strcmp(szTemp, "CQCB") == 0) {
		ercfType = RATE_CTRL_CQCB;
	} else if (strcmp(szTemp, "CVBR") == 0) {
		ercfType = RATE_CTRL_CVBR;
	} else {
		ercfType = RATE_CTRL_NONE;
	}
#if 0    
	if (ptEncoderInfo->tVideoSlaveCfg.ercfSVCQualityType != ercfType) {
#endif		
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_SVC_RATECTRL;
		tEncoderOpt.adwUserData[0] = ercfType;
		VencEncoder_SetOptions(hObject, tEncoderOpt);

		ptEncoderInfo->tVideoSlaveCfg.ercfSVCQualityType = ercfType;
#if 0
		if (tEncoderOpt.adwUserData[0] == RATE_CTRL_NONE) {
			printf("[VENC_ENCODER_CONF] SVC RateCtrl method : NONE\n");
		} else if (tEncoderOpt.adwUserData[0] == RATE_CTRL_VQCB) {
			printf("[VENC_ENCODER_CONF] SVC RateCtrl method : VQCB\n");
		} else if (tEncoderOpt.adwUserData[0] == RATE_CTRL_CQCB) {
			printf("[VENC_ENCODER_CONF] SVC RateCtrl method : CQCB\n");
		} else if (tEncoderOpt.adwUserData[0] == RATE_CTRL_CVBR) {
			printf("[VENC_ENCODER_CONF] SVC RateCtrl method : CVBR\n");
		} else {
			printf("[VENC_ENCODER_CONF] SVC RateCtrl method : unsupported method\n");
		}
#endif
#if 0	
	}
#endif

	return;
}
/* =========================================================================================== */
void VencEncoder_SetSVCQuant(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwQuantValue;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwQuantValue = strtoul(szTemp, NULL, 10);
#if 0	
	if (ptEncoderInfo->tVideoSlaveCfg.dwSVCQuantValue != dwQuantValue) {
#endif
		if (dwQuantValue > MAX_SVC_QUANT) {
			dwQuantValue = MAX_SVC_QUANT;
		} else if (dwQuantValue < MIN_SVC_QUANT) {
			dwQuantValue = MIN_SVC_QUANT;
		}

		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_SVC_QUANT;
		tEncoderOpt.adwUserData[0] = dwQuantValue;
		VencEncoder_SetOptions(hObject, tEncoderOpt);

		ptEncoderInfo->tVideoSlaveCfg.dwSVCQuantValue = dwQuantValue;         
		// printf("[VENC_ENCODER_CONF] SVC Quant Value %u\n", ptEncoderInfo->tVideoSlaveCfg.dwSVCQuantValue);
#if 0
	}
#endif
	return;
}
/* =========================================================================================== */
void VencEncoder_SetSVCBitrate(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwBitrate;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwBitrate = strtoul(szTemp, NULL, 10);
#if 0    
	if (ptEncoderInfo->tVideoSlaveCfg.dwSVCBitrate != dwBitrate) {
#endif		
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_SVC_BITRATE;
		tEncoderOpt.adwUserData[0] = dwBitrate;
		VencEncoder_SetOptions(hObject, tEncoderOpt);
		ptEncoderInfo->tVideoSlaveCfg.dwSVCBitrate = dwBitrate;
		// printf("[VENC_ENCODER_CONF] SVC Bitrate %u\n", ptEncoderInfo->tVideoSlaveCfg.dwSVCBitrate);
#if 0		
	}
#endif
	return;
}
/* =========================================================================================== */
void VencEncoder_SetSVCHDREnable(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	BOOL bSVCHDREnable = FALSE;
	
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	
	if (strcasecmp(szTemp, "on") == 0) 
	{
		bSVCHDREnable = TRUE;
	}
	if (ptEncoderInfo->tVideoSlaveCfg.bSVCHDREnable != bSVCHDREnable) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_SVC_HDRENABLE;
		tEncoderOpt.adwUserData[0] = bSVCHDREnable;
		VencEncoder_SetOptions(hObject, tEncoderOpt);
		ptEncoderInfo->tVideoSlaveCfg.bSVCHDREnable = bSVCHDREnable;
		// printf("[VENC_ENCODER_CONF] SVC HDR Enable: %s\n",ptEncoderInfo->tVideoSlaveCfg.bSVCHDREnable?"TRUE":"FALSE");
	}
	return;
}

/* =========================================================================================== */
void VencEncoder_SetSVCLayerNum(HANDLE hObject, const CHAR *szParam, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TEncoderOptions tEncoderOpt;
	CHAR szTemp[len+1];
	DWORD dwLayerNum = 0;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	dwLayerNum = strtoul(szTemp, NULL, 10);

	if (ptEncoderInfo->tVideoSlaveCfg.dwSVCLayerNum !=  dwLayerNum) {
		tEncoderOpt.eenOptType = VENCSLAVE_ENCODER_OPTION_SET_SVC_LAYERNUM;
		tEncoderOpt.adwUserData[0] =dwLayerNum;
		VencEncoder_SetOptions(hObject, tEncoderOpt);
		ptEncoderInfo->tVideoSlaveCfg.dwSVCLayerNum = dwLayerNum;
		//printf("[VENC_ENCODER_CONF] SVC Layer Num: %u\n",ptEncoderInfo->tVideoSlaveCfg.dwSVCLayerNum  );
	}
	return;
}
/* =========================================================================================== */
void VencEncoder_SetROIEnable(HANDLE hObject, const CHAR *szEnable, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	CHAR szTemp[len+1];
	BOOL bEnabled = FALSE;
	
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szEnable);
	
	ptEncoderInfo->tROICfg.bROIEnabledChange = FALSE;
	
	if (strcmp(szTemp, "on") == 0)
	{
		bEnabled = TRUE;
	}
	if (bEnabled != ptEncoderInfo->tROICfg.bROIEnabled) {
		ptEncoderInfo->tROICfg.bROIEnabledChange = TRUE;
		ptEncoderInfo->tROICfg.bROIEnabled = bEnabled;
		//printf("[VENC_ENCODER_CONF](%d) ROI enable change to %s (%s) \n",__LINE__,(ptEncoderInfo->tROICfg.bROIEnabled)?"enable":"disable",ptEncoderInfo->tROICfg.bROIEnabledChange?"EnableChanged":"EnableNotChanged");
	}
	return;
}
/* =========================================================================================== */
void VencEncoder_SetROIXTranslate(HANDLE hObject, const CHAR *szXTranslate, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	CHAR szTemp[len+1];
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szXTranslate);
	ptEncoderInfo->tROICfg.fXTranslate = atof(szTemp);
	//printf("[VENC_ENCODER_CONF](%d) ROI XTranslate %.8f \n",__LINE__ , ptEncoderInfo->tROICfg.fXTranslate);
	return;
}
/* =========================================================================================== */
void VencEncoder_SetROIYTranslate(HANDLE hObject, const CHAR *szYTranslate, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	CHAR szTemp[len+1];
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szYTranslate);
	ptEncoderInfo->tROICfg.fYTranslate = atof(szTemp);
	//printf("[VENC_ENCODER_CONF](%d) ROI YTranslate %.8f \n",__LINE__ , ptEncoderInfo->tROICfg.fYTranslate);
	return;
}
/* =========================================================================================== */
void VencEncoder_SetROIXScale(HANDLE hObject, const CHAR *szXScale, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	CHAR szTemp[len+1];
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szXScale);
	ptEncoderInfo->tROICfg.fXScale = atof(szTemp);
	//printf("[VENC_ENCODER_CONF](%d) ROI XScale %.8f \n",__LINE__ , ptEncoderInfo->tROICfg.fXScale);
	return;
}
/* =========================================================================================== */
void VencEncoder_SetROIYScale(HANDLE hObject, const CHAR *szYScale, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	CHAR szTemp[len+1];
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szYScale);
	ptEncoderInfo->tROICfg.fYScale = atof(szTemp);
	//  printf("[VENC_ENCODER_CONF](%d) ROI YScale %.8f \n",__LINE__ , ptEncoderInfo->tROICfg.fYScale);
	return;
}
/* =========================================================================================== */
SCODE VencEncoder_SetROIWinTag(HANDLE hObject, const CHAR *szID, const CHAR** atts)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	DWORD  dwCurWinId;
	TROIWinInfo *ptROIWinInfo = NULL;

	if (atts) {
		if (strcmp(atts[0], "id") != 0) {
			printf("[VENC_ENCODER_CONF](%d) No id attribute \n",__LINE__);
			return IGNORE_CHILD_CONFIG;            
		}
		dwCurWinId = strtoul(atts[1], (char**)NULL, 10);
		if (dwCurWinId < 0 || dwCurWinId > MAX_ROI_WIN_NUM)
			return IGNORE_CHILD_CONFIG;
		ptEncoderInfo->tROICfg.dwCurWinId = dwCurWinId;
		ptROIWinInfo = &(ptEncoderInfo->tROICfg.atroiwCfg[ptEncoderInfo->tROICfg.dwCurWinId]);
		ptROIWinInfo->dwWinID = dwCurWinId;
		//printf("[VENC_ENCODER_CONF](%d) ROI Current Window ID %u \n",__LINE__, ptEncoderInfo->tROICfg.dwCurWinId);
		return S_OK;
	}
	printf("[VENC_ENCODER_CONF](%d) No id attribute \n",__LINE__);
	return IGNORE_CHILD_CONFIG;

}
/* =========================================================================================== */
void VencEncoder_SetROIWinEnable(HANDLE hObject, const CHAR *szEnable, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	CHAR szTemp[len+1];
	DWORD dwCurWinId = -1;
	BOOL bEnabled = FALSE;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szEnable);
	dwCurWinId = ptEncoderInfo->tROICfg.dwCurWinId;

	if (strcmp(szTemp, "on") == 0)
		bEnabled = TRUE;
	ptEncoderInfo->tROICfg.atroiwCfg[dwCurWinId].bEnabledChange = FALSE;
	if (bEnabled != ptEncoderInfo->tROICfg.atroiwCfg[dwCurWinId].bEnabled) {
		ptEncoderInfo->tROICfg.atroiwCfg[dwCurWinId].bEnabledChange = TRUE;
		ptEncoderInfo->tROICfg.atroiwCfg[dwCurWinId].bEnabled = bEnabled;
		ptEncoderInfo->tROICfg.atroiwCfg[dwCurWinId].bAnyFieldChange = TRUE;
		// printf("[VENC_ENCODER_CONF](%d) widows %u Enabled Change to %s \n",__LINE__ ,dwCurWinId, (ptEncoderInfo->tROICfg.atroiwCfg[dwCurWinId].bEnabled)?"Enable":"Disable");
	}
	return;
}
/* =========================================================================================== */
void VencEncoder_SetROIWinName(HANDLE hObject, const CHAR *szName, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TROIWinInfo *ptROIWinInfo;

	ptROIWinInfo = &(ptEncoderInfo->tROICfg.atroiwCfg[ptEncoderInfo->tROICfg.dwCurWinId]);
	memset(ptROIWinInfo->szName,0x0,sizeof(ptROIWinInfo->szName));
	strncpy(ptROIWinInfo->szName, szName, (len<(MAX_CAPTION_LEN+1)?len:MAX_CAPTION_LEN));
	ptROIWinInfo->szName[MAX_CAPTION_LEN] = '\0';
	//printf("[VENC_ENCODER_CONF](%d) ROI window name %s\n",__LINE__, ptROIWinInfo->szName);

	return;
}
/* =========================================================================================== */
void VencEncoder_SetROIWinLeft(HANDLE hObject, const CHAR *szLeft, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TROIWinInfo *ptROIWinInfo;
	DWORD dwLeft;

	ptROIWinInfo = &(ptEncoderInfo->tROICfg.atroiwCfg[ptEncoderInfo->tROICfg.dwCurWinId]);
	dwLeft = strtoul(szLeft, (char**)NULL, 10);
	//ptROIWinInfo->bPositionChange = FALSE;
	if (dwLeft != ptROIWinInfo->dwLeft) {
		ptROIWinInfo->bPositionChange = TRUE;
		ptROIWinInfo->dwLeft = dwLeft;
		ptROIWinInfo->bAnyFieldChange = TRUE;
		//printf("[VENC_ENCODER_CONF](%d) widows %u Left Position Change to %u \n",__LINE__,ptEncoderInfo->tROICfg.dwCurWinId, ptROIWinInfo->dwLeft);
	}

	return;
}
/* =========================================================================================== */
void VencEncoder_SetROIWinTop(HANDLE hObject, const CHAR *szTop, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TROIWinInfo *ptROIWinInfo;
	DWORD dwTop;

	ptROIWinInfo = &(ptEncoderInfo->tROICfg.atroiwCfg[ptEncoderInfo->tROICfg.dwCurWinId]);
	dwTop = strtoul(szTop, (char**)NULL, 10);
	//ptROIWinInfo->bPositionChange = FALSE;
	if (dwTop != ptROIWinInfo->dwTop) {
		ptROIWinInfo->bPositionChange = TRUE;
		ptROIWinInfo->dwTop = dwTop;
		ptROIWinInfo->bAnyFieldChange = TRUE;
		//printf("[VENC_ENCODER_CONF](%d) widows %u Top Position Change to %u \n",__LINE__,ptEncoderInfo->tROICfg.dwCurWinId, ptROIWinInfo->dwTop);
	}
	return;
}
/* =========================================================================================== */
void VencEncoder_SetROIWinWidth(HANDLE hObject, const CHAR *szWidth, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TROIWinInfo *ptROIWinInfo;
	DWORD dwWidth;

	ptROIWinInfo = &(ptEncoderInfo->tROICfg.atroiwCfg[ptEncoderInfo->tROICfg.dwCurWinId]);
	dwWidth = strtoul(szWidth, (char**)NULL, 10);
	//ptROIWinInfo->bWindowSizeChange = FALSE;
	if (dwWidth != ptROIWinInfo->dwWidth) {
		ptROIWinInfo->bWindowSizeChange = TRUE;
		ptROIWinInfo->dwWidth = dwWidth;
		ptROIWinInfo->bAnyFieldChange = TRUE;
		//printf("[VENC_ENCODER_CONF](%d) widows %u Width Change to %u \n",__LINE__, ptEncoderInfo->tROICfg.dwCurWinId, ptROIWinInfo->dwWidth);	
	}
	return;
}
/* =========================================================================================== */
void VencEncoder_SetROIWinHeight(HANDLE hObject, const CHAR *szHeight, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TROIWinInfo *ptROIWinInfo;
	DWORD dwHeight;

	ptROIWinInfo = &(ptEncoderInfo->tROICfg.atroiwCfg[ptEncoderInfo->tROICfg.dwCurWinId]);
	dwHeight = strtoul(szHeight, (char**)NULL, 10);
	//ptROIWinInfo->bWindowSizeChange = FALSE;
	if (dwHeight != ptROIWinInfo->dwHeight) {
		ptROIWinInfo->bWindowSizeChange = TRUE;
		ptROIWinInfo->dwHeight = dwHeight;
		ptROIWinInfo->bAnyFieldChange = TRUE;
		//printf("[VENC_ENCODER_CONF](%d) widows %u Height Change to %u \n",__LINE__, ptEncoderInfo->tROICfg.dwCurWinId, ptROIWinInfo->dwHeight);	
	}
	return;
}
/* =========================================================================================== */
void VencEncoder_SetROIDeltaQp(HANDLE hObject, const CHAR *szDeltaQp, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TROIWinInfo *ptROIWinInfo;
	SDWORD sdwDeltaQp = 0;
	CHAR szTemp[len+1];

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szDeltaQp);
	sdwDeltaQp = atoi(szTemp);

	ptROIWinInfo = &(ptEncoderInfo->tROICfg.atroiwCfg[ptEncoderInfo->tROICfg.dwCurWinId]);
	ptROIWinInfo->bDeltaQpChange = FALSE;
	if (sdwDeltaQp != ptROIWinInfo->sdwDeltaQp) {
		ptROIWinInfo->bDeltaQpChange = TRUE;
		ptROIWinInfo->sdwDeltaQp = sdwDeltaQp;
		ptROIWinInfo->bAnyFieldChange = TRUE;
		//printf("[VENC_ENCODER_CONF](%d) widows %u deltaQp Change to %d \n",__LINE__, ptEncoderInfo->tROICfg.dwCurWinId, ptROIWinInfo->sdwDeltaQp);
	}
	return;
}
/* =========================================================================================== */
void VencEncoder_SetROIEncodingInterval(HANDLE hObject, const CHAR *szEncodingInterval, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)(hObject);
	TROIWinInfo *ptROIWinInfo;
	DWORD dwEncodingInterval = 0;
	CHAR szTemp[len+1];

	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szEncodingInterval);
	dwEncodingInterval = strtoul(szTemp, (char**)NULL, 10);
	if (dwEncodingInterval < MIN_H264_ROI_ENC_INTERVAL)
	{
		dwEncodingInterval = MIN_H264_ROI_ENC_INTERVAL;
	}
	
	if (dwEncodingInterval > MAX_H264_ROI_ENC_INTERVAL)
	{
		dwEncodingInterval = MAX_H264_ROI_ENC_INTERVAL;
	}
	ptROIWinInfo = &(ptEncoderInfo->tROICfg.atroiwCfg[ptEncoderInfo->tROICfg.dwCurWinId]);
	ptROIWinInfo->bEncodingIntervalChange = FALSE;
	if (dwEncodingInterval != ptROIWinInfo->dwEncodingInterval) {
		ptROIWinInfo->bEncodingIntervalChange = TRUE;
		ptROIWinInfo->dwEncodingInterval = dwEncodingInterval;
		ptROIWinInfo->bAnyFieldChange = TRUE;
		debug_printf("[VENC_ENCODER_CONF](%d) widows %u EncodingInterval Change to %d \n",__LINE__, ptEncoderInfo->tROICfg.dwCurWinId, ptROIWinInfo->dwEncodingInterval);
	}
	return;
}

/* ========================================================================= */
SCODE VencEncoder_ROI_AddWindow( TEncoderInfo *ptEncoderInfo , TROIWinInfo *ptROIWinInfo)
{
	FLOAT	fXTconf2def = ptEncoderInfo->tROICfg.fXTranslate;
	FLOAT	fYTconf2def = ptEncoderInfo->tROICfg.fYTranslate;
	FLOAT	fXSconf2def = ptEncoderInfo->tROICfg.fXScale;
	FLOAT	fYSconf2def = ptEncoderInfo->tROICfg.fYScale;
	FLOAT	fXTreal2def = ptEncoderInfo->tROI2D.fXTranslate;
	FLOAT	fYTreal2def = ptEncoderInfo->tROI2D.fYTranslate;
	FLOAT	fXSreal2def = ptEncoderInfo->tROI2D.fXScale;
	FLOAT	fYSreal2def = ptEncoderInfo->tROI2D.fYScale;
	
	memset(&ptEncoderInfo->atROIRectangleWin[ptROIWinInfo->dwWinID],0x0,sizeof(TROIRectangleWin));
	ptEncoderInfo->atROIRectangleWin[ptROIWinInfo->dwWinID].tROIRectangle.dwX = (DWORD) (((((((FLOAT)ptROIWinInfo->dwLeft)* fXSconf2def)+fXTconf2def) - fXTreal2def) / fXSreal2def) + 0.5);
	ptEncoderInfo->atROIRectangleWin[ptROIWinInfo->dwWinID].tROIRectangle.dwY = (DWORD) (((((((FLOAT)ptROIWinInfo->dwTop)* fYSconf2def)+fYTconf2def) - fYTreal2def) / fYSreal2def) + 0.5);
	ptEncoderInfo->atROIRectangleWin[ptROIWinInfo->dwWinID].tROIRectangle.dwWidth= (DWORD) (((((((FLOAT)ptROIWinInfo->dwWidth) * fXSconf2def)+fXTconf2def) - fXTreal2def) / fXSreal2def) + 0.5);
	ptEncoderInfo->atROIRectangleWin[ptROIWinInfo->dwWinID].tROIRectangle.dwHeight = (DWORD) (((((((FLOAT)ptROIWinInfo->dwHeight)* fYSconf2def)+fYTconf2def) - fYTreal2def) / fYSreal2def) + 0.5);
	ptEncoderInfo->atROIRectangleWin[ptROIWinInfo->dwWinID].bActive = TRUE;
	ptROIWinInfo->dwSerialNum = ROI_USED_WINDOW;
	
	return S_OK;
}

/* ========================================================================= */
SCODE VencEncoder_ROI_DeleteWindow(TEncoderInfo *ptEncoderInfo , TROIWinInfo *ptROIWinInfo)
{
	memset(&ptEncoderInfo->atROIRectangleWin[ptROIWinInfo->dwWinID],0x0,sizeof(TROIRectangleWin));
	ptEncoderInfo->atROIRectangleWin[ptROIWinInfo->dwWinID].bActive = FALSE;
	// printf("[VENC_ENCODER_CONF](%d) Deleting window S/N: %u , Total retangle num: %u window ID: %u\n",__LINE__ ,ptROIWinInfo->dwSerialNum,g_dwRectToalNum,ptROIWinInfo->dwWinID);
	ptROIWinInfo->dwSerialNum = ROI_NON_USED_WINDOW;
	
	return S_OK;
}

/* ========================================================================= */
SCODE VencEncoder_ROI_SetPosition(TEncoderInfo *ptEncoderInfo , TROIWinInfo *ptROIWinInfo)
{
	FLOAT	fXTconf2def = ptEncoderInfo->tROICfg.fXTranslate;
	FLOAT	fYTconf2def = ptEncoderInfo->tROICfg.fYTranslate;
	FLOAT	fXSconf2def = ptEncoderInfo->tROICfg.fXScale;
	FLOAT	fYSconf2def = ptEncoderInfo->tROICfg.fYScale;
	FLOAT	fXTreal2def = ptEncoderInfo->tROI2D.fXTranslate;
	FLOAT	fYTreal2def = ptEncoderInfo->tROI2D.fYTranslate;
	FLOAT	fXSreal2def = ptEncoderInfo->tROI2D.fXScale;
	FLOAT	fYSreal2def = ptEncoderInfo->tROI2D.fYScale;
	
	ptEncoderInfo->atROIRectangleWin[ptROIWinInfo->dwWinID].tROIRectangle.dwX = (DWORD) (((((((FLOAT)ptROIWinInfo->dwLeft)  * fXSconf2def)+fXTconf2def) - fXTreal2def) / fXSreal2def) + 0.5);
	ptEncoderInfo->atROIRectangleWin[ptROIWinInfo->dwWinID].tROIRectangle.dwY = (DWORD) (((((((FLOAT)ptROIWinInfo->dwTop)   * fYSconf2def)+fYTconf2def) - fYTreal2def) / fYSreal2def) + 0.5);
	//printf("[VENC_ENCODER_CONF](%d)Window S/N:%u ,  Set (x,y) = (%u, %u) widnow id: %u\n",__LINE__ ,g_dwWindowSerialNum,g_atROIRectangle[g_dwWindowSerialNum].dwX, g_atROIRectangle[g_dwWindowSerialNum].dwY,ptROIWinInfo->dwWinID);
	return S_OK;
}

/* ========================================================================= */
SCODE VencEncoder_ROI_SetWindowSize(TEncoderInfo *ptEncoderInfo , TROIWinInfo *ptROIWinInfo)
{
	FLOAT	fXTconf2def = ptEncoderInfo->tROICfg.fXTranslate;
	FLOAT	fYTconf2def = ptEncoderInfo->tROICfg.fYTranslate;
	FLOAT	fXSconf2def = ptEncoderInfo->tROICfg.fXScale;
	FLOAT	fYSconf2def = ptEncoderInfo->tROICfg.fYScale;
	FLOAT	fXTreal2def = ptEncoderInfo->tROI2D.fXTranslate;
	FLOAT	fYTreal2def = ptEncoderInfo->tROI2D.fYTranslate;
	FLOAT	fXSreal2def = ptEncoderInfo->tROI2D.fXScale;
	FLOAT	fYSreal2def = ptEncoderInfo->tROI2D.fYScale;
	
	ptEncoderInfo->atROIRectangleWin[ptROIWinInfo->dwWinID].tROIRectangle.dwWidth  = (DWORD) (((((((FLOAT)ptROIWinInfo->dwWidth) * fXSconf2def)+fXTconf2def) - fXTreal2def) / fXSreal2def) + 0.5);
	ptEncoderInfo->atROIRectangleWin[ptROIWinInfo->dwWinID].tROIRectangle.dwHeight = (DWORD) (((((((FLOAT)ptROIWinInfo->dwHeight)* fYSconf2def)+fYTconf2def) - fYTreal2def) / fYSreal2def) + 0.5);
	//printf("[VENC_ENCODER_CONF](%d)Window S/N:%u ,  Set (width,height) = (%u, %u) window id: %u \n",__LINE__ ,g_dwWindowSerialNum,g_atROIRectangle[g_dwWindowSerialNum].dwWidth,g_atROIRectangle[g_dwWindowSerialNum].dwHeight,ptROIWinInfo->dwWinID);
	return S_OK;
}

/* ========================================================================= */
#if defined(_H4EE) || defined(_SVC)
static void VencEncoder_Calibrate_ROI(DWORD *pdwEndMBXNum, DWORD *pdwEndMBYNum, DWORD dwMBXNum, DWORD dwMBYNum)
{
	
	if(*pdwEndMBXNum >= (dwMBXNum-1))
		(*pdwEndMBXNum)--;
	
	if(*pdwEndMBYNum >= (dwMBYNum-1))
		(*pdwEndMBYNum)--;
	
	return;
}
#endif
/* ========================================================================= */

#ifdef _H4EE
typedef struct h264_roi_info
{
	TH264EncROIWindowsInfos tH264EncROIWindowsInfos;
	int iROIRegionIndex;
}TH264ROIInfo;
static SCODE DoH264ROISetoptions(TEncoderInfo *ptEncoderInfo, BOOL bEnROI)
{
	TROISelectorInitOptions tInitOptions;
	TROIWinInfo *ptROIWinInfo = NULL;
	TROISelectorState tState;
	HANDLE hROIObject = NULL;
	DWORD	dwEncWidth = g_avfInfo[ptEncoderInfo->tVideoSlaveCfg.vfFormat].wEncWidth;
	DWORD   dwEncHeight = g_avfInfo[ptEncoderInfo->tVideoSlaveCfg.vfFormat].wEncHeight;
	DWORD   dwWidthPad = ((dwEncWidth + 15) >> 4) << 4;
	DWORD   dwHeightPad = ((dwEncHeight+ 15) >> 4) << 4;
	SCODE scRet = S_OK;
	DWORD dwIndex = 0;
	DWORD dwROIRegionIndex = 0;
	DWORD dwROIMacroBlkNumForEachRegion = (dwWidthPad*dwHeightPad)/(H264_MACRO_BLK_SIZE);
	
	TH264ROIInfo atH264ROIInfo[MAX_ROI_WIN_NUM];
	TH264EncOptions tEncOptions;
#if 0
	printf("[venc_encoder_conf](%d)dwWidthPad=%d,dwHeightPad=%d\n",__LINE__,dwWidthPad,dwHeightPad);
#endif
	memset(&tInitOptions, 0x0, sizeof(TROISelectorInitOptions));
	memset(&tState, 0x0, sizeof(TROISelectorState));
	
	for (dwIndex = 0; dwIndex < MAX_ROI_WIN_NUM; dwIndex++)
	{ 
		memset(&atH264ROIInfo[dwIndex], 0x0, sizeof(TH264ROIInfo));
		atH264ROIInfo[dwIndex].iROIRegionIndex = -1;
		atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.bROIEnable = FALSE;
	}
	
	if (bEnROI == TRUE)
	{
		tInitOptions.dwVersion = ROISELECTOR_VERSION;
		tInitOptions.dwFrameWidth = dwWidthPad;
		tInitOptions.dwFrameHeight = dwHeightPad;
		tInitOptions.dwMacroBlockWidth = H264_MACRO_BLK_WIDTH;
		tInitOptions.dwMacroBlockHeight = H264_MACRO_BLK_HEIGHT;
		printf("[VENC_ENCODER_CONF](%d)(width,height,blkwidth,blkheight)=(%d,%d,%d,%d)\n",
			   __LINE__,
				tInitOptions.dwFrameWidth,
				tInitOptions.dwFrameHeight,
				tInitOptions.dwMacroBlockWidth,
				tInitOptions.dwMacroBlockHeight
  			);
		if (ROISelector_Initial(&hROIObject, &tInitOptions) != S_OK)
		{
			printf("[VENC_ENCODER_CONF](%d)Initialize ROI Selector object fail !!\n",__LINE__);
			scRet = S_FAIL;
			goto exit;
		}

		for (dwIndex = 0; dwIndex < MAX_ROI_WIN_NUM; dwIndex++)
		{
			DWORD dwX = 0;
			DWORD dwY = 0;
			DWORD dwWidth = 0;
			DWORD dwHeight = 0;
			BOOL bOutOfBound = FALSE;
			
			ptROIWinInfo = &(ptEncoderInfo->tROICfg.atroiwCfg[dwIndex]);
			atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.dwROIWindowNum = dwIndex;
			atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.sdwDeltaQp = ptROIWinInfo->sdwDeltaQp;
			atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.dwEncodingInterval = ptROIWinInfo->dwEncodingInterval;
			
			if (ptEncoderInfo->atROIRectangleWin[dwIndex].bActive == TRUE)
			{
				dwX = ptEncoderInfo->atROIRectangleWin[dwIndex].tROIRectangle.dwX;
				dwY = ptEncoderInfo->atROIRectangleWin[dwIndex].tROIRectangle.dwY;
				dwWidth = ptEncoderInfo->atROIRectangleWin[dwIndex].tROIRectangle.dwWidth;
				dwHeight = ptEncoderInfo->atROIRectangleWin[dwIndex].tROIRectangle.dwHeight;
				
				//If the index of the window is (MAX_ROI_WIN_NUM - 1), the window is backgound window.
				//Do not care the  position and size when setting backgound window.
				if (dwIndex == (MAX_ROI_WIN_NUM - 1))
				{
					dwX = 0;
					dwY = 0;
					dwWidth = dwWidthPad;
					dwHeight = dwHeightPad;
				}
				
				bOutOfBound = (((dwX + dwWidth) > dwWidthPad) || ((dwY + dwHeight) > dwHeightPad));
				if (bOutOfBound == TRUE)
				{
					printf("[VENC_ENCODER_CONF](%d)(x,y,width,height)=(%d,%d,%d,%d) is out of bound!!\n",
					   __LINE__,
					   dwX,
					   dwY,
					   dwWidth,
					   dwHeight
					);	
					continue;
				}
				tState.atROIRectangle[dwROIRegionIndex].dwX = dwX;
				tState.atROIRectangle[dwROIRegionIndex].dwY = dwY;
				tState.atROIRectangle[dwROIRegionIndex].dwWidth = dwWidth;
				tState.atROIRectangle[dwROIRegionIndex].dwHeight = dwHeight;
				tState.apdwBlockIndex[dwROIRegionIndex] = (DWORD *)malloc(sizeof(DWORD)*dwROIMacroBlkNumForEachRegion);
				atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.bROIEnable = TRUE;
				atH264ROIInfo[dwIndex].iROIRegionIndex = dwROIRegionIndex;
				printf("[VENC_ENCODER_CONF](%d)(index,x,y,width,height,ptr)=(%d,%d,%d,%d,%d,%p)\n",
					   __LINE__,
					   dwROIRegionIndex,
					   tState.atROIRectangle[dwROIRegionIndex].dwX,
					   tState.atROIRectangle[dwROIRegionIndex].dwY,
					   tState.atROIRectangle[dwROIRegionIndex].dwWidth,
		               tState.atROIRectangle[dwROIRegionIndex].dwHeight,
						tState.apdwBlockIndex[dwROIRegionIndex]
  					);
				dwROIRegionIndex++;
			}
		}
		
		tState.dwRecNum = dwROIRegionIndex;
		
		printf("[VENC_ENCODER_CONF](%d) Total Rectangle num is %u \n",__LINE__,tState.dwRecNum);
		if (hROIObject != NULL)
		{	//printf("%d\n",__LINE__);
			ROISelector_OneFrame(hROIObject, &tState);
			//printf("%d\n",__LINE__);
		}
		else
		{	printf("[VENC_ENCODER_CONF](%d)Initialize ROI HANDLE is NULL !!\n",__LINE__);
			scRet = S_FAIL;
			goto exit;
		}

		for (dwIndex = 0; dwIndex < MAX_ROI_WIN_NUM; dwIndex++)
		{
			if (atH264ROIInfo[dwIndex].iROIRegionIndex != -1)
			{
				int i = atH264ROIInfo[dwIndex].iROIRegionIndex;
				
				DWORD dwEndMBIndex = (DWORD)(*(tState.apdwBlockIndex[i]+tState.adwTotalIndexNum[i]-1));
				DWORD dwStartMBIndex = (DWORD)(*tState.apdwBlockIndex[i]);
				
				atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.dwStartMBXNum = (DWORD)(dwStartMBIndex % (dwWidthPad/H264_MACRO_BLK_WIDTH));
				atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.dwStartMBYNum = (DWORD)(dwStartMBIndex / (dwWidthPad/H264_MACRO_BLK_WIDTH));
				atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.dwEndMBXNum = (DWORD)(dwEndMBIndex % (dwWidthPad/H264_MACRO_BLK_WIDTH));
				atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.dwEndMBYNum = (DWORD)(dwEndMBIndex / (dwWidthPad/H264_MACRO_BLK_WIDTH));
				//For removing the Warning: righest MB did not support bound box from the setoption VIDEO_CHANGE_REGION_OF_INTEREST_WINDOWS_INFO.
				VencEncoder_Calibrate_ROI(&atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.dwEndMBXNum, 
										  &atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.dwEndMBYNum,
										  (dwWidthPad / H264_MACRO_BLK_WIDTH),
										  (dwHeightPad / H264_MACRO_BLK_HEIGHT)
 											);
				
				
			}
		
			memset(&tEncOptions,0x0,sizeof(TH264EncOptions));
			tEncOptions.eOptionFlags = VIDEO_CHANGE_REGION_OF_INTEREST_WINDOWS_INFO; 
			tEncOptions.adwUserData[0] = (DWORD)&(atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos);
			if (ptEncoderInfo->hH264EncObj != NULL)
			{
				if (H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tEncOptions) != S_OK)                
				{                                                                        
					printf("[VENC_ENCODER_CONF](%d)Call H264Enc_SetOptions():VIDEO_CHANGE_REGION_OF_INTEREST_WINDOWS_INFO fail !!\n",__LINE__);
					scRet = S_FAIL;
					goto exit;                                                     
				}
			}
		}
	}
	else
	{
		for (dwIndex = 0; dwIndex < MAX_ROI_WIN_NUM; dwIndex++)
		{
			ptROIWinInfo = &(ptEncoderInfo->tROICfg.atroiwCfg[dwIndex]);
			atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.dwROIWindowNum = dwIndex;
			atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.sdwDeltaQp = ptROIWinInfo->sdwDeltaQp;
			atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.dwEncodingInterval = ptROIWinInfo->dwEncodingInterval;
			memset(&tEncOptions,0x0,sizeof(TH264EncOptions));
			tEncOptions.eOptionFlags = VIDEO_CHANGE_REGION_OF_INTEREST_WINDOWS_INFO; 
			tEncOptions.adwUserData[0] = (DWORD)&(atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos);
			if (ptEncoderInfo->hH264EncObj != NULL)
			{
				if (H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tEncOptions) != S_OK)                
				{                                                                        
					printf("[VENC_ENCODER_CONF](%d)Call H264Enc_SetOptions():VIDEO_CHANGE_REGION_OF_INTEREST_WINDOWS_INFO fail !!\n",__LINE__);
					scRet = S_FAIL;
					goto exit;                                                     
				}
			}	
		}
	}
	for (dwIndex = 0; dwIndex < MAX_ROI_WIN_NUM; dwIndex++)
	{
		debug_printf("[VENC_ENCODER_CONF](%d)[roi](win,en,sMBX,sMBY,eMBX,eMBY,dQp,encInter)=(%d,%d,%d,%d,%d,%d,%d,%d)\n",
			    __LINE__,
				atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.dwROIWindowNum,
				atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.bROIEnable,
				atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.dwStartMBXNum,
				atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.dwStartMBYNum,
				atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.dwEndMBXNum,
				atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.dwEndMBYNum,
				atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.sdwDeltaQp,
				atH264ROIInfo[dwIndex].tH264EncROIWindowsInfos.dwEncodingInterval
			  );
	}
exit:
	if (hROIObject != NULL)
	{
		if (ROISelector_Release(&hROIObject) != S_OK)
		{
			printf("[VENC_ENCODER_CONF](%d)Release ROI Selector object fail !!\n",__LINE__);
			scRet = S_FAIL;
		}
	}

	for (dwIndex = 0; dwIndex < tState.dwRecNum; dwIndex++)
	{
		if (tState.apdwBlockIndex[dwIndex] != NULL)
		{
			free(tState.apdwBlockIndex[dwIndex]);
			tState.apdwBlockIndex[dwIndex] = NULL;
		}
	}
	
	return scRet;
}
#endif

#ifdef _SVC
typedef struct svc_roi_info
{
	TSVCEncROIWindowsInfos tSVCEncROIWindowsInfos;
	int iROIRegionIndex;
}TSVCROIInfo;

static SCODE DoSVCROISetoptions(TEncoderInfo *ptEncoderInfo, BOOL bEnROI)
{
	TROISelectorInitOptions tInitOptions;
	TROIWinInfo *ptROIWinInfo = NULL;
	TROISelectorState tState;
	HANDLE hROIObject = NULL;
	DWORD	dwEncWidth = g_avfInfo[ptEncoderInfo->tVideoSlaveCfg.vfFormat].wEncWidth;
	DWORD   dwEncHeight = g_avfInfo[ptEncoderInfo->tVideoSlaveCfg.vfFormat].wEncHeight;
	DWORD   dwWidthPad = ((dwEncWidth + 15) >> 4) << 4;
	DWORD   dwHeightPad = ((dwEncHeight+ 15) >> 4) << 4;
	SCODE scRet = S_OK;
	DWORD dwIndex = 0;
	DWORD dwROIRegionIndex = 0;
	DWORD dwROIMacroBlkNumForEachRegion = (dwWidthPad*dwHeightPad)/(SVC_MACRO_BLK_SIZE);
	
	TSVCROIInfo atSVCROIInfo[MAX_ROI_WIN_NUM];
	TSVCEncOptions tEncOptions;
#if 0
	printf("[venc_encoder_conf](%d)dwWidthPad=%d,dwHeightPad=%d\n",__LINE__,dwWidthPad,dwHeightPad);
#endif
	memset(&tInitOptions, 0x0, sizeof(TROISelectorInitOptions));
	memset(&tState, 0x0, sizeof(TROISelectorState));
	
	for (dwIndex = 0; dwIndex < MAX_ROI_WIN_NUM; dwIndex++)
	{
		memset(&atSVCROIInfo[dwIndex],0x0,sizeof(TSVCROIInfo));
		atSVCROIInfo[dwIndex].iROIRegionIndex = -1;
		atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.bROIEnable = FALSE;
	}
	
	if (bEnROI == TRUE)
	{
		tInitOptions.dwVersion = ROISELECTOR_VERSION;
		tInitOptions.dwFrameWidth = dwWidthPad;
		tInitOptions.dwFrameHeight = dwHeightPad;
		tInitOptions.dwMacroBlockWidth = SVC_MACRO_BLK_WIDTH;
		tInitOptions.dwMacroBlockHeight = SVC_MACRO_BLK_HEIGHT;

		if (ROISelector_Initial(&hROIObject, &tInitOptions) != S_OK)
		{
			printf("[VENC_ENCODER_CONF](%d)Initialize ROI Selector object fail !!\n",__LINE__);
			scRet = S_FAIL;
			goto exit;
		}

		for (dwIndex = 0; dwIndex < MAX_ROI_WIN_NUM; dwIndex++)
		{
			DWORD dwX = 0;
			DWORD dwY = 0;
			DWORD dwWidth = 0;
			DWORD dwHeight = 0;
			BOOL bOutOfBound = FALSE;
			
			ptROIWinInfo = &(ptEncoderInfo->tROICfg.atroiwCfg[dwIndex]);
			atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.dwROIWindowNum = dwIndex;
			atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.sdwDeltaQp = ptROIWinInfo->sdwDeltaQp;
			atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.dwEncodingInterval = ptROIWinInfo->dwEncodingInterval;
			if (ptEncoderInfo->atROIRectangleWin[dwIndex].bActive == TRUE)
			{
				dwX = ptEncoderInfo->atROIRectangleWin[dwIndex].tROIRectangle.dwX;
				dwY = ptEncoderInfo->atROIRectangleWin[dwIndex].tROIRectangle.dwY;
				dwWidth = ptEncoderInfo->atROIRectangleWin[dwIndex].tROIRectangle.dwWidth;
				dwHeight = ptEncoderInfo->atROIRectangleWin[dwIndex].tROIRectangle.dwHeight;
				
				//If the index of the window is (MAX_ROI_WIN_NUM - 1), the window is backgound window.
				//Do not care the  position and size when setting backgound window.
				if (dwIndex == (MAX_ROI_WIN_NUM - 1))
				{
					dwX = 0;
					dwY = 0;
					dwWidth = dwWidthPad;
					dwHeight = dwHeightPad;
				}
				
				bOutOfBound = (((dwX + dwWidth) > dwWidthPad) || ((dwY + dwHeight) > dwHeightPad));
				if (bOutOfBound == TRUE)
				{
					printf("[VENC_ENCODER_CONF](%d)(x,y,width,height)=(%d,%d,%d,%d) is out of bound!!\n",
					   __LINE__,
					   dwX,
					   dwY,
					   dwWidth,
					   dwHeight
					);	
					continue;
				}
				tState.atROIRectangle[dwROIRegionIndex].dwX = dwX;
				tState.atROIRectangle[dwROIRegionIndex].dwY = dwY;
				tState.atROIRectangle[dwROIRegionIndex].dwWidth = dwWidth;
				tState.atROIRectangle[dwROIRegionIndex].dwHeight = dwHeight;
				tState.apdwBlockIndex[dwROIRegionIndex] = (DWORD *)malloc(sizeof(DWORD)*dwROIMacroBlkNumForEachRegion);
				atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.bROIEnable = TRUE;
				atSVCROIInfo[dwIndex].iROIRegionIndex = dwROIRegionIndex;
				dwROIRegionIndex++;
			}
		}
		
		tState.dwRecNum = dwROIRegionIndex;
		
		printf("[VENC_ENCODER_CONF](%d) Total Rectangle num is %u \n",__LINE__,tState.dwRecNum);
		if (hROIObject != NULL)
		{
			ROISelector_OneFrame(hROIObject, &tState);
		}
		else
		{	printf("[VENC_ENCODER_CONF](%d)Initialize ROI HANDLE is NULL !!\n",__LINE__);
			scRet = S_FAIL;
			goto exit;
		}

		for (dwIndex = 0; dwIndex < MAX_ROI_WIN_NUM; dwIndex++)
		{
			if (atSVCROIInfo[dwIndex].iROIRegionIndex != -1)
			{
				int i = atSVCROIInfo[dwIndex].iROIRegionIndex;
				
				DWORD dwEndMBIndex = (DWORD)(*(tState.apdwBlockIndex[i]+tState.adwTotalIndexNum[i]-1));
				DWORD dwStartMBIndex = (DWORD)(*tState.apdwBlockIndex[i]);
				
				atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.dwStartMBXNum = (DWORD)(dwStartMBIndex % (dwWidthPad/SVC_MACRO_BLK_WIDTH));
				atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.dwStartMBYNum = (DWORD)(dwStartMBIndex / (dwWidthPad/SVC_MACRO_BLK_WIDTH));
				atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.dwEndMBXNum = (DWORD)(dwEndMBIndex % (dwWidthPad/SVC_MACRO_BLK_WIDTH));
				atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.dwEndMBYNum = (DWORD)(dwEndMBIndex / (dwWidthPad/SVC_MACRO_BLK_WIDTH));
			
				//For removing the Warning: righest MB did not support bound box from the setoption VIDEO_CHANGE_REGION_OF_INTEREST_WINDOWS_INFO.
				VencEncoder_Calibrate_ROI(&atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.dwEndMBXNum, 
										  &atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.dwEndMBYNum,
										  (dwWidthPad / SVC_MACRO_BLK_WIDTH),
										  (dwHeightPad / SVC_MACRO_BLK_HEIGHT)
 											);
			}
		
			memset(&tEncOptions,0x0,sizeof(TSVCEncOptions));
			tEncOptions.eOptionFlags = VIDEO_CHANGE_REGION_OF_INTEREST_WINDOWS_INFO; 
			tEncOptions.adwUserData[0] = (DWORD)&(atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos);
			if (ptEncoderInfo->hSVCEncObj != NULL)
			{
				if (SVCEnc_SetOptions(ptEncoderInfo->hSVCEncObj, &tEncOptions) != S_OK)                
				{                                                                        
					printf("[VENC_ENCODER_CONF](%d)Call SVCEnc_SetOptions():VIDEO_CHANGE_REGION_OF_INTEREST_WINDOWS_INFO fail !!\n",__LINE__);
					scRet = S_FAIL;
					goto exit;                                                     
				}
			}
		}
	}
	else
	{
		for (dwIndex = 0; dwIndex < MAX_ROI_WIN_NUM; dwIndex++)
		{
			ptROIWinInfo = &(ptEncoderInfo->tROICfg.atroiwCfg[dwIndex]);
			atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.dwROIWindowNum = dwIndex;
			atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.sdwDeltaQp = ptROIWinInfo->sdwDeltaQp;
			atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.dwEncodingInterval = ptROIWinInfo->dwEncodingInterval;
			memset(&tEncOptions,0x0,sizeof(TSVCEncOptions));
			tEncOptions.eOptionFlags = VIDEO_CHANGE_REGION_OF_INTEREST_WINDOWS_INFO; 
			tEncOptions.adwUserData[0] = (DWORD)&(atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos);
			if (ptEncoderInfo->hSVCEncObj != NULL)
			{
				if (SVCEnc_SetOptions(ptEncoderInfo->hSVCEncObj, &tEncOptions) != S_OK)                
				{                                                                        
					printf("[VENC_ENCODER_CONF](%d)Call SVCEnc_SetOptions():VIDEO_CHANGE_REGION_OF_INTEREST_WINDOWS_INFO fail !!\n",__LINE__);
					scRet = S_FAIL;
					goto exit;                                                     
				}
			}	
		}
	}
#if 0
	for (dwIndex = 0; dwIndex < MAX_ROI_WIN_NUM; dwIndex++)
	{
		printf("[VENC_ENCODER_CONF](%d)[roi](win,en,sMBX,sMBY,eMBX,eMBY,dQp,encInter)=(%d,%d,%d,%d,%d,%d,%d,%d)\n",
			    __LINE__,
				atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.dwROIWindowNum,
				atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.bROIEnable,
				atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.dwStartMBXNum,
				atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.dwStartMBYNum,
				atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.dwEndMBXNum,
				atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.dwEndMBYNum,
				atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.sdwDeltaQp,
				atSVCROIInfo[dwIndex].tSVCEncROIWindowsInfos.dwEncodingInterval
			  );
	}
#endif
exit:
	if (hROIObject != NULL)
	{
		if (ROISelector_Release(&hROIObject) != S_OK)
		{
			printf("[VENC_ENCODER_CONF](%d)Release ROI Selector object fail !!\n",__LINE__);
			scRet = S_FAIL;
		}
	}

	for (dwIndex = 0; dwIndex < tState.dwRecNum; dwIndex++)
	{
		if (tState.apdwBlockIndex[dwIndex] !=  NULL)
		{
			free(tState.apdwBlockIndex[dwIndex]);
			tState.apdwBlockIndex[dwIndex] = NULL;
		}
	}
	
	return scRet;
}
#endif

SCODE VencEncoder_ROISetoptions(TEncoderInfo *ptEncoderInfo)
{
	TROIWinInfo *ptROIWinInfo = NULL;
	BOOL bROIMaskBufChanged = FALSE;

	DWORD dwIndex = 0;
	DWORD dwCodecType = ptEncoderInfo->tVideoSlaveCfg.dwCodecType;
	static DWORD	vfPreFormat = -1;
	
	debug_printf("[VENC_ENCODER_CONF](%d)Check codec type before doing ROI.\n",__LINE__);
	if (dwCodecType == VIDEO_CODEC_MPEG4) {
		printf("[VENC_ENCODER_CONF](%d) Codec %s\n",__LINE__ ,"mpeg4");
	} 
	else if (dwCodecType == VIDEO_CODEC_MJPEG) {
		printf("[VENC_ENCODER_CONF](%d) Codec %s\n",__LINE__ ,"mjpeg");
	} 
	else if (dwCodecType == VIDEO_CODEC_H264) {
		printf("[VENC_ENCODER_CONF](%d) Codec %s\n",__LINE__ ,"H264");
	}
	else if (dwCodecType == VIDEO_CODEC_SVC) {
		printf("[VENC_ENCODER_CONF](%d) Codec %s\n",__LINE__ ,"SVC");
	}
#if 0
	if (dwCodecType != VIDEO_CODEC_H264 && dwCodecType != VIDEO_CODEC_SVC)
	{
		return S_OK;
	}
#endif
	printf("[VENC_ENCODER_CONF](%d)ROI: %s (%s)\n",__LINE__,ptEncoderInfo->tROICfg.bROIEnabled?"Enable":"Disable",ptEncoderInfo->tROICfg.bROIEnabledChange?"EnableChanged":"EnableNotChanged");
	
	// According to bMotionEnabledChange status, do things respectively.
	if (ptEncoderInfo->tROICfg.bROIEnabledChange == TRUE) {
		// <enable> off -> on 
		if (ptEncoderInfo->tROICfg.bROIEnabled == TRUE) {
			// if <active> is on, it should ADD_WINDOW, and record the serial number
			for (dwIndex = 0; dwIndex < MAX_ROI_WIN_NUM; dwIndex++) {
				ptROIWinInfo = &(ptEncoderInfo->tROICfg.atroiwCfg[dwIndex]);
				if (ptROIWinInfo->bEnabled == TRUE) {
					VencEncoder_ROI_AddWindow(ptEncoderInfo, ptROIWinInfo);
				}
			}
		} else { // <enable> on -> off // done
			// search for SN, if SN != NON_USED_WINDOW, it should delete, and set it to NON_USED_WINDOW
			for (dwIndex = 0; dwIndex < MAX_ROI_WIN_NUM; dwIndex++) {
				ptROIWinInfo = &(ptEncoderInfo->tROICfg.atroiwCfg[dwIndex]);
				if (ptROIWinInfo->dwSerialNum != ROI_NON_USED_WINDOW) {
					//delete window
					VencEncoder_ROI_DeleteWindow(ptEncoderInfo, ptROIWinInfo);
				}
			}			
		}
		bROIMaskBufChanged = TRUE;
		
	} else { // bMotionEnabledChange == FALSE
		// <enable> on -> on
		if (ptEncoderInfo->tROICfg.bROIEnabled == TRUE) {
			// each window, if bEnabled change, it should ADD_WINDOW or DELETE_WINDOW
			//              else, other field to setoption
			for (dwIndex = 0; dwIndex < MAX_ROI_WIN_NUM; dwIndex++) {
				ptROIWinInfo = &(ptEncoderInfo->tROICfg.atroiwCfg[dwIndex]);

				if (ptROIWinInfo->bEnabledChange == TRUE) {
					
					if (ptROIWinInfo->bEnabled == TRUE) { // <active> off -> on
						VencEncoder_ROI_AddWindow(ptEncoderInfo, ptROIWinInfo);
					}else { // <active> on -> off
						VencEncoder_ROI_DeleteWindow(ptEncoderInfo, ptROIWinInfo);
					}
					bROIMaskBufChanged = TRUE;
					continue;
				}
				if (ptROIWinInfo->bPositionChange == TRUE) {
					VencEncoder_ROI_SetPosition(ptEncoderInfo, ptROIWinInfo);
					//printf("ROI(%d) Position changed...\n", dwIndex);
					ptROIWinInfo->bPositionChange = FALSE;
					bROIMaskBufChanged = TRUE;
				}
				if (ptROIWinInfo->bWindowSizeChange == TRUE) {
					VencEncoder_ROI_SetWindowSize(ptEncoderInfo, ptROIWinInfo);
					//printf("ROI(%d) Window size changed...\n", dwIndex);
					ptROIWinInfo->bWindowSizeChange = FALSE;
					bROIMaskBufChanged = TRUE;
				}
				if (ptROIWinInfo->bDeltaQpChange == TRUE){
					bROIMaskBufChanged = TRUE;
				}
				if (ptROIWinInfo->bEncodingIntervalChange == TRUE){
					bROIMaskBufChanged = TRUE;
				}
			}			
		}
				
	}
	if (ptEncoderInfo->tVideoSlaveCfg.vfFormat != vfPreFormat)
	{
		for (dwIndex = 0; dwIndex < MAX_ROI_WIN_NUM; dwIndex++) 
		{
			ptROIWinInfo = &(ptEncoderInfo->tROICfg.atroiwCfg[dwIndex]);
			VencEncoder_ROI_SetPosition(ptEncoderInfo, ptROIWinInfo);
			VencEncoder_ROI_SetWindowSize(ptEncoderInfo, ptROIWinInfo);
		}
		vfPreFormat = ptEncoderInfo->tVideoSlaveCfg.vfFormat;
		bROIMaskBufChanged = TRUE;
	}
	if (bROIMaskBufChanged == TRUE)
	{
		if (ptEncoderInfo->tROICfg.bROIEnabled == TRUE)
		{
			printf("[VENC_ENCODER_CONF](%d)Because ROI is enabled. Call setoption to set ROI!!\n",__LINE__);
#ifdef _H4EE			
			DoH264ROISetoptions(ptEncoderInfo,TRUE);
#endif
#ifdef _SVC			
			DoSVCROISetoptions(ptEncoderInfo,TRUE);
#endif
		}
		else
		{
			if (ptEncoderInfo->tROICfg.bROIEnabledChange == TRUE)
			{
				printf("[VENC_ENCODER_CONF](%d)Because ROI is diaabled. Call setoption to disable all ROI windows!!\n",__LINE__);
				#ifdef _H4EE		
					DoH264ROISetoptions(ptEncoderInfo,FALSE);
				#endif
				#ifdef _SVC				
					DoSVCROISetoptions(ptEncoderInfo,FALSE);
				#endif
			}
		}
		bROIMaskBufChanged = FALSE;
	}
	
	return S_OK;
}

/* =========================================================================================== */
void VencEncoder_SetTextOverlayEnable(HANDLE hObject, const CHAR* szTextOverlayEnable, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)hObject;
	CHAR szTemp[len+1];
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szTextOverlayEnable);

	if (strcmp(szTemp, "on") == 0) {
		ptEncoderInfo->tTextOverlayOptForConfig.bTextOverlayEnable = TRUE;
	} else {
		ptEncoderInfo->tTextOverlayOptForConfig.bTextOverlayEnable = FALSE;
	}
}
/* =========================================================================================== */
void VencEncoder_SetTextOverlayFontPath(HANDLE hObject, const CHAR* szTextOverlayFontPath, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)hObject;
	CHAR szTemp[len+1];
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szTextOverlayFontPath);
	strncpy(ptEncoderInfo->tTextOverlayOptForConfig.szFontPath, szTemp, len+1);    
	//printf("ptEncoderInfo->tTextOverlayOptForConfig.szFontPath = %s, len = %d\n", ptEncoderInfo->tTextOverlayOptForConfig.szFontPath, strlen(ptEncoderInfo->tTextOverlayOptForConfig.szFontPath));
}

/* =========================================================================================== */
SCODE VencEncoder_ClearTextOverlayCustomText(HANDLE hObject, const CHAR *szParam, const CHAR **atts)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)hObject;
	//ptEncoderInfo->tTextOverlayOptForConfig.szCustomText[0] = '\0';
	memset(ptEncoderInfo->tTextOverlayOptForConfig.szCustomText, 0x0, sizeof(ptEncoderInfo->tTextOverlayOptForConfig.szCustomText));
	return S_OK;
}
/* =========================================================================================== */
void VencEncoder_SetTextOverlayCustomText(HANDLE hObject, const CHAR* szTextOverlayCustomText, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)hObject;
	CHAR szTemp[len+1];
	memset(szTemp,0x0,sizeof(szTemp));
	
	snprintf(szTemp, len+1, "%s", szTextOverlayCustomText);
	
	strncat(ptEncoderInfo->tTextOverlayOptForConfig.szCustomText, szTemp, sizeof(ptEncoderInfo->tTextOverlayOptForConfig.szCustomText) - strlen(ptEncoderInfo->tTextOverlayOptForConfig.szCustomText) - 1);
	//strncpy(ptEncoderInfo->tTextOverlayOptForConfig.szCustomText, szTemp, len+1);    
	//printf("ptEncoderInfo->tTextOverlayOptForConfig.szCustomText = %s, len= %d\n", ptEncoderInfo->tTextOverlayOptForConfig.szCustomText, strlen(ptEncoderInfo->tTextOverlayOptForConfig.szCustomText));
}
/* =========================================================================================== */
void VencEncoder_SetTextOverlayEnableMilliSec(HANDLE hObject, const CHAR* szTextOverlayEnableTimeStamp, SDWORD len)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *)hObject;
	CHAR szTemp[len+1];
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szTextOverlayEnableTimeStamp);

	if (strcmp(szTemp, "on") == 0) {
		ptEncoderInfo->tTextOverlayOptForConfig.bMilliSecEnable = TRUE;
	} else {
		ptEncoderInfo->tTextOverlayOptForConfig.bMilliSecEnable = FALSE;
	}
	VencEncoder_SetTextOverlayOptions(hObject, &(ptEncoderInfo->tTextOverlayOptForConfig));
}

