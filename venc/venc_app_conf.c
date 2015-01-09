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
 * venc_app_conf.c
 *
 * \brief
 * Video process setoption implement.
 *
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#include "venc_app_local.h"

/* ============================================================================================= */
void VencApp_SetMasterSwitch(HANDLE hObject, const CHAR *szSwitch, SDWORD len);
void VencApp_SetVideoInSwitch(HANDLE hObject, const CHAR *szSwitch, SDWORD len);
void VencApp_SetEncoderSwitch(HANDLE hObject, const CHAR *szSwitch, SDWORD len);
extern SCODE VencApp_SetEncoderTag(HANDLE hObject, const CHAR* szIDTag, const CHAR** atts);
void VencApp_SetMotionSwitch(HANDLE hObject, const CHAR *szSwitch, SDWORD len);
#ifdef _VPL_VOC
void VencApp_SetDisplaySwitch(HANDLE hObject, const CHAR *szSwitch, SDWORD len);
#endif
TXmlWrapperTreeMap ptTreeMap[] =
{
    //setting
	{"root/venc_master/switch", NULL, &VencApp_SetMasterSwitch, NULL},
	{"root/venc_videoin/switch", NULL, &VencApp_SetVideoInSwitch, NULL},
    {"root/venc_encoder/encoder", &VencApp_SetEncoderTag, NULL, NULL},
    {"root/venc_encoder/encoder/switch", NULL, &VencApp_SetEncoderSwitch, NULL},
	{"root/venc_motion/switch", NULL, &VencApp_SetMotionSwitch, NULL},
#ifdef _VPL_VOC
	{"root/venc_display/switch", NULL, &VencApp_SetDisplaySwitch, NULL},
#endif	
    {NULL, NULL, NULL}
};

/* ============================================================================================= */
void VencApp_SetMasterSwitch(HANDLE hObject, const CHAR *szSwitch, SDWORD len)
{   
    TVencInfo *ptVencInfo = (TVencInfo *)hObject;
    static DWORD dwCurrSwitch = -1;
	DWORD dwSwitch = strtoul(szSwitch, (char**)NULL, 10);
	
	if (dwCurrSwitch != dwSwitch) {
		VencMaster_ResetConfigFlag(ptVencInfo->hMasterProcObj);
		dwCurrSwitch = dwSwitch;
	}
}

/* ============================================================================================= */
void VencApp_SetVideoInSwitch(HANDLE hObject, const CHAR *szSwitch, SDWORD len)
{   
    TVencInfo *ptVencInfo = (TVencInfo *)hObject;
    static DWORD dwCurrSwitch = -1;
	DWORD dwSwitch = strtoul(szSwitch, (char**)NULL, 10);
	
	if (dwCurrSwitch != dwSwitch) {
		VencVideoIn_ResetConfigFlag(ptVencInfo->hVideoInProcObj);
		dwCurrSwitch = dwSwitch;
	}
}

/* ============================================================================================= */
void VencApp_SetEncoderSwitch(HANDLE hObject, const CHAR *szSwitch, SDWORD len)
{   
    TVencInfo *ptVencInfo = (TVencInfo *)hObject;
    DWORD dwCurId = ptVencInfo->tVencCfgPath.dwCurId;    
	DWORD dwSwitch = strtoul(szSwitch, (char**)NULL, 10);
	
	if (ptVencInfo->pdwCurrSwitch[dwCurId] != dwSwitch) {
#if 0
		VencEncoder_ResetConfigFlag(ptVencInfo->hEncoderProcObj[dwCurId]);
#endif	
		VencEncoder_ResetConfigFlag(ptVencInfo->phEncoderProcObj[dwCurId]);
		ptVencInfo->pdwCurrSwitch[dwCurId] = dwSwitch;
	}
}
#ifdef _VPL_VOC
/* ============================================================================================= */
void VencApp_SetDisplaySwitch(HANDLE hObject, const CHAR *szSwitch, SDWORD len)
{   
    TVencInfo *ptVencInfo = (TVencInfo *)hObject;
    static DWORD dwCurrSwitch = -1;
	DWORD dwSwitch = strtoul(szSwitch, (char**)NULL, 10);
	
	if (dwCurrSwitch != dwSwitch) {
		VencDisplay_ResetConfigFlag(ptVencInfo->hDisplayProcObj);
		dwCurrSwitch = dwSwitch;
	}
}
#endif
/* ============================================================================================= */
void VencApp_SetMotionSwitch(HANDLE hObject, const CHAR *szSwitch, SDWORD len)
{   
    TVencInfo *ptVencInfo = (TVencInfo *)hObject;
    static DWORD dwCurrSwitch = -1;
	DWORD dwSwitch = strtoul(szSwitch, (char**)NULL, 10);
	
	if (dwCurrSwitch != dwSwitch) {
		VencMotion_ResetConfigFlag(ptVencInfo->hMotionProcObj);
		dwCurrSwitch = dwSwitch;
	}
}
