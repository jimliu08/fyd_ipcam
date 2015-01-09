/*
 *******************************************************************************
 * $Header: $
 *
 *  Copyright (c) 2000-2006 VN Inc. All rights reserved.
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
 *  | VN INC.                                                    |
 *  +-----------------------------------------------------------------+
 *
 * $History: $
 */

/*!
 *******************************************************************************
 * Copyright 2000-2006 VN, Inc. All rights reserved.
 *
 * \file
 * xmlwrapper_app_local.h
 *
 * \brief
 * XmlWrapper local header file
 *
 * \date
 * 2006/12/11
 *
 * \author
 * Ming Jing Tsai
 *
 *******************************************************************************
 */
#ifndef _XMLWRAPPER_APP_LOCAL_H_
#define _XMLWRAPPER_APP_LOCAL_H_
#define MAX_DATA_LENGTH 80

typedef struct xmlwrapper_app_info
{
	CHAR szVer[MAX_DATA_LENGTH];
	CHAR szAuthor[MAX_DATA_LENGTH];
	CHAR szDate[MAX_DATA_LENGTH];
	CHAR szUsage[MAX_DATA_LENGTH];	
}TAPPInfo;

typedef struct xmlwrapper_app_flow
{
	CHAR szVer[MAX_DATA_LENGTH];
	CHAR szFirst[MAX_DATA_LENGTH];
	CHAR szNewConf[MAX_DATA_LENGTH];
	CHAR szReloading[MAX_DATA_LENGTH];
	CHAR szExit[MAX_DATA_LENGTH];
	struct xmlwrapper_app_flow* ptNext;
}TAPPFlow;

typedef struct xmlwrapper_app_handler
{
	TAPPFlow *ptafFlow;
	TAPPInfo *ptaiInfo;	
	HANDLE hTmp;
}TAPPHandler;

#endif //_XMLWRAPPER_APP_LOCAL_H_

