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
 * venc_encoder_dispatch.c
 *
 * \brief
 * Write encoded bitstream to shared buffer
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

/* =========================================================================================== */
SCODE VencEncoder_RequestOutputBuffer(HANDLE hSrdObj, TSharedBuffMgrState *ptSharedBuffMgrState)
{	
	if (SharedBuffMgr_GetBuffer(hSrdObj, ptSharedBuffMgrState) == S_OK) {
		return S_OK;
	} else {
		return S_FAIL;
	}
}

/* =========================================================================================== */
SCODE VencEncoder_ReleaseOutputBuffer(HANDLE hSrdObj, TSharedBuffMgrState *ptSharedBuffMgrState)
{
	SharedBuffMgr_ReleaseBuffer(hSrdObj, ptSharedBuffMgrState);
	return S_OK;
}

/* =========================================================================================== */
SCODE VencEncoder_GetOutputBufferFD(HANDLE hSrdObj, int* fd)
{
	SharedBuffMgr_GetFileDescriptor(hSrdObj, fd);
	return S_OK;
}
