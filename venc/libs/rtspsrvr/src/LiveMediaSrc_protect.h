/*
 *******************************************************************************
 * $Header: $
 *
 *  Copyright (c) 2007-2012 VN Inc. All rights reserved.
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
 * 
 *******************************************************************************
 */

/*!
 *******************************************************************************
 * Copyright 2007-2012 VN, Inc. All rights reserved.
 *
 * \file
 * LiveMediaSrc_protect.h
 *
 * \brief
 * LiveMediaSrc protect header file.
 * Only the class which implements LiveMediaSrc needs to include this header file
 *
 * \date
 * 2008/10/08
 *
 * \author
 * djhow.tu@VN.com
 *
 *******************************************************************************
 */

#ifndef __LIVE_MEDIA_SOURCE_PROTECT_H__
#define __LIVE_MEDIA_SOURCE_PROTECT_H__

#include "LiveMediaSrc.h"
#include <stdio.h>

typedef SCODE (*PFLMSrcRelease)	(HANDLE*);
typedef SCODE (*PFLMSrcUBufCB)	(HANDLE, TLMSrcUBufInfo*);
typedef SCODE (*PFLMSrcEvntCB)	(HANDLE, ELMSrcEventType);
typedef SCODE (*PFLMSrcWakeUpToTerminate)  (HANDLE);

typedef struct live_media_src_callback_map
{
	PFLMSrcRelease		pfRelease;
	PFLMSrcUBufCB		pfGetUBuf;
	PFLMSrcUBufCB		pfReleaseUBuf;
	PFLMSrcEvntCB		pfEvntHandler;
	PFLMSrcWakeUpToTerminate		pfWakeUpToTerminate;
} TLMSrcCBMap;

/*!
 *******************************************************************************
 * LiveMediaSrc base struct.
 * \note When you implement LiveMediaSrc, the first element of your class must
 * be this base struct.
 *******************************************************************************
 */
typedef struct live_media_src
{
	//! Pointer to the callback map, aka vtable
	TLMSrcCBMap			*ptCBMap;
} TLiveMediaSrc;
#endif // __LIVE_MEDIA_SOURCE_PROTECT_H__
