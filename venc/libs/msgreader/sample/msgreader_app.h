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
 * msgreader.h
 *
 * \brief
 * Goldberg - msgreader header file
 *
 * \date
 * 2007/08/03
 *
 * \author
 * Erin Tsai
 *
 *
 *******************************************************************************
 */

#include "msgreader.h"

typedef enum venc_message_host_type
{
	msgHMaster = 0,
    msgHVideoIn,
    msgHEncoder0,
    msgHEncoder1,
    msgHEncoder2,
	msgHDisplay,
    msgHUnKnown
} EHostType;

// Content
typedef enum venc_message_content_type
{
	msgCStart = 0,
	msgCStop,
	msgCForceCI,
	msgCForceIntra,
	msgCDumpVideoCapBuf,
	msgCDumpVideoDispBuf,
	msgCUnKnown
} EContentType;

typedef struct venc_message_member
{
	EHostType		eHost;
	EContentType	eContent;
} TMessageMember;

typedef struct msgreader_app_usrobj
{
	CHAR szMsg[20];
}TMsgReaderAppObj;
