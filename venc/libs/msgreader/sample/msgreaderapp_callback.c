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
 */

/*!
 *******************************************************************************
 * Copyright 2007-2010 VN, Inc. All rights reserved.
 *
 * \file
 * xmlwrapper_app_callback.c
 *
 * \brief
 * Xmlwrapper app callback function
 *
 * \date
 * 2006/12/11
 *
 * \author
 * Ming Jing Tsai
 *
 *
 *******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xmlwrapper.h"
#include "msgreader_app.h"

TMessageMember g_tMsgMember;

void MsgReader_GetHost(HANDLE hObject, const CHAR *szVisitor, int len);
void MsgReader_GetContent(HANDLE hObject, const CHAR *szVisitor, int len);

TXmlWrapperTreeMap MsgReaderTreeHdl[] =
{
	{"control/request/host", NULL, &MsgReader_GetHost, NULL},
	{"control/request/content", NULL, &MsgReader_GetContent, NULL},
	{NULL, NULL, NULL}
};

/* ========================================================================== */
void MsgReader_GetHost(HANDLE hObject, const CHAR *szVisitor, int len)
{
    CHAR szTemp[len+1];
    snprintf(szTemp, len+1, "%s", szVisitor);

    if (strcasecmp(szTemp, "master") == 0) {
        printf("[VENC_MESSAGE] host - master !!\n");
		g_tMsgMember.eHost = msgHMaster;
    } else if (strcasecmp(szTemp, "videoin") == 0) {
        printf("[VENC_MESSAGE] host - videoin !!\n");
		g_tMsgMember.eHost = msgHVideoIn;
    } else if (strcasecmp(szTemp, "encoder0") == 0) {
        printf("[VENC_MESSAGE] host - encoder0 !!\n");
		g_tMsgMember.eHost = msgHEncoder0;
    } else if (strcasecmp(szTemp, "encoder1") == 0) {
        printf("[VENC_MESSAGE] host - encoder1 !!\n");
		g_tMsgMember.eHost = msgHEncoder1;
    } else if (strcasecmp(szTemp, "encoder2") == 0) {
        printf("[VENC_MESSAGE] host - encoder2 !!\n");
		g_tMsgMember.eHost = msgHEncoder2;
    } else if (strcasecmp(szTemp, "display") == 0) {
        printf("[VENC_MESSAGE] host - display !!\n");
		g_tMsgMember.eHost = msgHDisplay;
    } else {
		printf("[VENC_MESSAGE] unsupport host !!\n");
		g_tMsgMember.eHost = msgHUnKnown;
    }
}

/* ========================================================================== */
void MsgReader_GetContent(HANDLE hObject, const CHAR *szVisitor, int len)
{
    CHAR szTemp[len+1];
    snprintf(szTemp, len+1, "%s", szVisitor);

    if (strcasecmp(szTemp, "start") == 0) {
        printf("[VENC_MESSAGE] content - start !!\n");
		g_tMsgMember.eContent = msgCStart;
    } else if (strcasecmp(szTemp, "stop") == 0) {
        printf("[VENC_MESSAGE] content - stop !!\n");
		g_tMsgMember.eContent = msgCStop;
    } else if (strcasecmp(szTemp, "forceCI") == 0) {
        printf("[VENC_MESSAGE] content - forceCI !!\n");
		g_tMsgMember.eContent = msgCForceCI;
    } else if (strcasecmp(szTemp, "forceIntra") == 0) {
        printf("[VENC_MESSAGE] content - forceIntra !!\n");
		g_tMsgMember.eContent = msgCForceIntra;
    } else if (strcasecmp(szTemp, "dumpVideoCapBuf") == 0) {
        printf("[VENC_MESSAGE] content - dumpVideoCapBuf !!\n");
		g_tMsgMember.eContent = msgCDumpVideoCapBuf;
    } else if (strcasecmp(szTemp, "dumpVideoDispBuf") == 0) {
        printf("[VENC_MESSAGE] content - dumpVideoDispBuf !!\n");
		g_tMsgMember.eContent = msgCDumpVideoDispBuf;
    } else {
		printf("[VENC_MESSAGE] unsupport content !!\n");
		g_tMsgMember.eContent = msgCUnKnown;
    }
}
