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
 *  | VN INC.                                                     |
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
 * LiveMediaSrc.h
 *
 * \brief
 * LiveMediaSrc header file.
 * LiveMediaSrc is an interface, you should implement it.
 * RtspSrvr uses LiveMediaSrc to get video/audio/mdata bitstreams for streaming.
 *
 * \date
 * 2010/11/16
 *
 * \author
 * djhow.tu@VN.com
 *
 *
 *******************************************************************************
 */

#ifndef __LIVE_MEDIA_SOURCE_H__
#define __LIVE_MEDIA_SOURCE_H__

#include "typedef.h"
#include "global_codec.h"
#include "dbgdefs.h"

/*! 
 *********************************************************************
 * Ubuffer information.
 *********************************************************************
*/
typedef struct lmsrc_ubuf_info
{
	//! Pointer to the ubuffer header
	BYTE	*pbyUBuffHdr;
	//! Size of ubuffer header
	int		iUBuffHdrLen;
	//! Pointer to the ubuffer payload
	BYTE	*pbyUBuffPayload;
	//! Size of ubuffer payload
	int		iUBuffPayloadLen;
} TLMSrcUBufInfo;

/*! 
 *********************************************************************
 * Enumeration of event type.
 *********************************************************************
*/
typedef enum lmsrc_event_type
{
	//! Need one Intra frame from this source
	letNeedIntra,
	//! Need one ubuffer conf from this source
	letNeedConf,
	//! Start needing bitstream from this source
	letBitstrmStart,
	//! Stop needing bitstream from this source
	letBitstrmStop
} ELMSrcEventType;

/*! 
 *********************************************************************
 *
 * \brief
 * Release the LiveMediaSrc object.
 *
 * \param[in,out] phObject
 * Pointer to the handle of LiveMediaSrc object.
 *
 * \retval S_OK
 * Release the LiveMediaSrc object ok.
 * \retval S_FAIL
 * Release the LiveMediaSrc object failed.
 *
 * \remarks
 * You should implement this API. 
 *
 ********************************************************************* 
*/
SCODE LiveMediaSrc_Release(HANDLE *phObject);

/*! 
 *********************************************************************
 *
 * \brief
 * Get the ubuffer.
 *
 * \param[in] hLMSrc
 * The handle of LiveMediaSrc object.
 *
 * \param[in] ptUBuffInfo
 * pointer to the ubuffer info.
 *
 * \retval S_OK
 * Get the ubuffer ok.
 * \retval S_FAIL
 * Get the ubuffer failed.
 *
 * \remarks
 * You should implement this API. 
 *
 * \see 
 * LiveMediaSrc_ReleaseUBuffer.
 *
 ********************************************************************* 
*/
SCODE LiveMediaSrc_GetUBuffer(HANDLE hLMSrc, TLMSrcUBufInfo *ptUBuffInfo);

/*! 
 *********************************************************************
 *
 * \brief
 * Release the gotten ubuffer.
 *
 * \param[in] hLMSrc
 * The handle of LiveMediaSrc object.
 *
 * \param[in] ptUBuffInfo
 * pointer to the ubuffer info.
 *
 * \retval S_OK
 * Release the gotten ubuffer ok.
 * \retval S_FAIL
 * Release the gotten ubuffer failed.
 *
 * \remarks
 * You should implement this API. 
 *
 * \see 
 * LiveMediaSrc_GetUBuffer.
 *
 ********************************************************************* 
*/
SCODE LiveMediaSrc_ReleaseUBuffer(HANDLE hLMSrc, TLMSrcUBufInfo *ptUBuffInfo);

/*! 
 *********************************************************************
 *
 * \brief
 * Event handling.
 *
 * \param[in] hLMSrc
 * The handle of LiveMediaSrc object.
 *
 * \param[in] eType
 * The type of event to be handling.
 *
 * \retval S_OK
 * Release the gotten ubuffer ok.
 * \retval S_FAIL
 * Release the gotten ubuffer failed.
 *
 * \remarks
 * You should implement this API. 
 *
 ********************************************************************* 
*/
SCODE LiveMediaSrc_EventHandler(HANDLE hLMSrc, ELMSrcEventType eType);

/*! 
 *********************************************************************
 *
 * \brief
 * Wake up and terminate source.
 *
 * \param[in] hLMSrc
 * The handle of LiveMediaSrc object.
 *
 * \retval S_OK
 * Wake up ok.
 * \retval S_FAIL
 * Wake up failed.
 *
 * \remarks
 * You should implement this API. 
 *
 ********************************************************************* 
*/
SCODE LiveMediaSrc_WakeUpToTerminate(HANDLE hLMSrc);

#endif // __LIVE_MEDIA_SOURCE_H__
