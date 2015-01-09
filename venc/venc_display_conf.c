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
 * venc_display_conf.c
 *
 * \brief
 * Video display process setoption implement.
 * 
 * \date
 * 2009/09/11
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#include "venc_display_local.h"
#define MAX_VIDEO_RESOLUTION_LEN   11

static void SetDisplayFmt       (HANDLE hObj, const CHAR *szOutFmt, SDWORD len);
static void SetDisplayResolution(HANDLE hObj, const CHAR *szOutResolution, 
		SDWORD len);

static void SetDisplayEnable    (HANDLE hObj, const CHAR *szParam, SDWORD len);
static void SetDisplayFreq      (HANDLE hObj, const CHAR *szParam, SDWORD len);

/* ========================================================================== */
TXmlWrapperTreeMap ptDisplayTreeMap[] =
{   
	{"root/venc_display/output",            NULL, &SetDisplayEnable,     NULL},		
	{"root/venc_display/output_format",     NULL, &SetDisplayFmt,        NULL},
	{"root/venc_display/output_resolution", NULL, &SetDisplayResolution, NULL},
	{"root/venc_display/output_freq",       NULL, &SetDisplayFreq,       NULL},
	{NULL, NULL, NULL}
};

/* ========================================================================== */
static void SetDisplayEnable(HANDLE hObj, const CHAR *szParam, SDWORD len)
{
	TDisplayInfo    *ptInfo         = (TDisplayInfo *)(hObj);
	EDispOutEn      eCurrDispOutEn;
	CHAR            szTemp[len+1];
	static EDispOutEn  ePrDispOutEn = vDispUnknownState;

	memset( szTemp,0x0,sizeof(szTemp) );
	snprintf(szTemp, len+1, "%s", szParam);

	if (strcasecmp(szTemp, "on") == 0) {
		eCurrDispOutEn = vDispOutEnable;
	} 
	else { //if (strcasecmp(szTemp, "off") == 0) {
		eCurrDispOutEn = vDispOutDisable;
	}

	if (ePrDispOutEn != eCurrDispOutEn)
	{
		if (eCurrDispOutEn == vDispOutEnable) {
			ptInfo->bNeed2Display = TRUE;
			ptInfo->bDisplayStart = TRUE;
			//sem_post(&(ptInfo->smEnDisp));
		} 
		else 
			ptInfo->bNeed2Display = FALSE;
		
		ePrDispOutEn = eCurrDispOutEn;
		DISP_DP("%s Cfg: Output Enable = %s\n", DISP_PRE, szTemp);
	}
		return;
}
/* ========================================================================== */
static SCODE extract_resolution(const CHAR  *szResolution, 
			DWORD       *pdwWidth,
			DWORD       *pdwHeight)
{
	char    szRes[MAX_VIDEO_RESOLUTION_LEN + 1];
	char    *szToken    = NULL;
	
	strncpy(szRes, szResolution, MAX_VIDEO_RESOLUTION_LEN + 1);
	szToken = strchr(szRes, 'x');
	if (szToken == NULL) return S_FAIL;
	
	*szToken    = '\0';
	*pdwWidth   = strtol(szRes,       (char**)NULL, 10);
	*pdwHeight  = strtol(szToken + 1, (char**)NULL, 10);
	return S_OK;
}
/* ========================================================================== */
static void SetDisplayResolution(HANDLE hObj, const CHAR *szParam, SDWORD len)
{
	TDisplayInfo *ptInfo = (TDisplayInfo *)(hObj);
	DWORD dwWidth   = 0;
	DWORD dwHeight  = 0;
	CHAR  szTemp[len+1];
	
	memset( szTemp,0x0,sizeof(szTemp) );
	snprintf(szTemp, len+1, "%s", szParam);
	DISP_DP("%s Cfg: Output Resolution = %s\n", DISP_PRE, szTemp);
	
	if (extract_resolution(szTemp, &dwWidth, &dwHeight) != S_OK) return;
	
	ptInfo->dwOutputWidth   = dwWidth;
	ptInfo->dwOutputHeight  = dwHeight;
}
/* ========================================================================== */
static void SetDisplayFreq(HANDLE hObj, const CHAR *szParam, SDWORD len)
{
	TDisplayInfo    *ptInfo         = (TDisplayInfo *)(hObj);
	DWORD           dwFreq          = 0;
	CHAR            szTemp[len+1];
	
	memset( szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szParam);
	
	DISP_DP("%s Cfg: Output Frequency = %s\n", DISP_PRE, szTemp);
	dwFreq = strtol(szTemp, (char**) NULL, 10);
	ptInfo->dwFreq = dwFreq;
	return;
}
/* ========================================================================== */
static void SetDisplayFmt(HANDLE hObj, const CHAR *szParam, SDWORD len)
{
	TDisplayInfo    *ptInfo         = (TDisplayInfo *)(hObj);
	CHAR            szTemp[len+1];
	EDispOutFmt     eDispOutFmt;
	snprintf(szTemp, len+1, "%s", szParam);
	
	DISP_DP("%s Cfg: Output Format = %s\n", DISP_PRE, szTemp);
	
	if (strcasecmp(szTemp, "NTSC") == 0) {
		eDispOutFmt = vDispOutFmtNTSC;
	} else if (strcasecmp(szTemp, "PAL") == 0) {
		eDispOutFmt = vDispOutFmtPAL;
	} else if (strcasecmp(szTemp, "HDMI") == 0) {
		eDispOutFmt = vDispOutFmtHDMI;
	} else if (strcasecmp(szTemp, "HDSDI") == 0) {
		eDispOutFmt = vDispOutFmtHDSDI;
	} else {
		eDispOutFmt = vDispOutFmtUnknown;
		fprintf(stderr, "%s Err: Cfg Output Format %s is unsupported\n", 
					DISP_PRE, szTemp);
	}
	ptInfo->eDispOutFmt = eDispOutFmt;
	return;
}

	/* ========================================================================== */

