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
 * venc_motion_conf.c
 *
 * \brief
 * Video motion process setoption implement.
 * 
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#include "venc_motion_local.h"

//! ----------------CharDataHandler----------------
static void SetSendSckEnable  (HANDLE       hObject, 
                               const CHAR   *szEnable, 
                               SDWORD       len);
static void SetSendEventEnable(HANDLE       hObject, 
                               const CHAR   *szEnable, 
                               SDWORD       len);
#if defined(_MD_TRIGER_ROI_)
static void SetROIDuration(HANDLE hObject, const CHAR *szValue, SDWORD len);
static void SetROIDeltaQp(HANDLE hObject, const CHAR *szValue, SDWORD len);
static void SetROIEncodingInterval(HANDLE hObject, const CHAR *szValue, SDWORD len);
#endif

/* ========================================================================== */
TXmlWrapperTreeMap ptMotionTreeMap[] =
{
	{"root/venc_motion/send_sck",   NULL, &SetSendSckEnable,   NULL},
	{"root/venc_motion/send_event", NULL, &SetSendEventEnable, NULL},
#if defined(_MD_TRIGER_ROI_)
	{"root/venc_motion/roi/duration", NULL, &SetROIDuration, NULL},
	{"root/venc_motion/roi/deltaQp", NULL, &SetROIDeltaQp, NULL},
	{"root/venc_motion/roi/encodingInterval", NULL, &SetROIEncodingInterval, NULL},
#endif
    {NULL, NULL, NULL}
};

#if defined(_MD_TRIGER_ROI_)
static void SetROIDuration(HANDLE hObject, const CHAR *szValue, SDWORD len)
{
	TMotionInfo *ptInfo= (TMotionInfo *)hObject;
    CHAR        szTemp[len+1];
	DWORD dwValue = 0;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szValue);
	
	dwValue = strtoul(szTemp, (char**)NULL, 10);
	ptInfo->dwROIDuration = dwValue;
	
}
static void SetROIDeltaQp(HANDLE hObject, const CHAR *szValue, SDWORD len)
{
	TMotionInfo *ptInfo= (TMotionInfo *)hObject;
    CHAR        szTemp[len+1];
	SDWORD sdwValue = 0;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szValue);
	
	sdwValue  = atoi(szTemp);
	ptInfo->sdwROIDeltaQp = sdwValue;
}
static void SetROIEncodingInterval(HANDLE hObject, const CHAR *szValue, SDWORD len)
{
	TMotionInfo *ptInfo= (TMotionInfo *)hObject;
    CHAR        szTemp[len+1];
	DWORD dwValue = 0;
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szValue);
	
	dwValue = strtoul(szTemp, (char**)NULL, 10);
	ptInfo->dwROIEncodingInterval = dwValue;
}

#endif

/* ========================================================================== */
static void SetSendSckEnable(HANDLE hObject, const CHAR *szEnable, SDWORD len)
{
	TMotionInfo *ptInfo         = (TMotionInfo *)hObject;
    CHAR        szTemp[len+1];
    BOOL        bEnabled        = FALSE;
	
	memset(szTemp,0x0,sizeof(szTemp));
    snprintf(szTemp, len+1, "%s", szEnable);
    if (strcmp(szTemp, "on") == 0) bEnabled = TRUE;
	ptInfo->bSendSckEnable = bEnabled;
    return;
}

/* ========================================================================== */
static void SetSendEventEnable(HANDLE hObject, const CHAR *szEnable, SDWORD len)
{
	TMotionInfo *ptInfo         = (TMotionInfo *)hObject;
    CHAR        szTemp[len+1];
    BOOL        bEnabled        = FALSE;

	memset(szTemp,0x0,sizeof(szTemp));
    snprintf(szTemp, len+1, "%s", szEnable);
    if (strcmp(szTemp, "on") == 0)  bEnabled = TRUE;
	ptInfo->bSendEvtEnable = bEnabled;
    return;
}
/* ========================================================================== */

