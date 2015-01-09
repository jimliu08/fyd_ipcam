/*
 *******************************************************************************
 * $Header: $
 *
 *  Copyright (c) 2006-2010 VN Inc. All rights reserved.
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
 * Copyright 2006-2010 VN, Inc. All rights reserved.
 *
 * \file
 * urecorder_limit.h
 *
 * \brief
 * Define the maximum ubuffer size of many kinds of media frames. If one media frame exceeds the maximum size, the frame should divided into several ubuffers.
 *
 * \date
 * 2007/02/01
 *
 * \author
 * Rey Cheng
 *
 *******************************************************************************
 */

#ifndef _UBUFFER_LIMIT_H_
#define _UBUFFER_LIMIT_H_

/*! The maximum size of a mpeg4 video ubuffer */
#define MAX_MP4V_UBUFFER	40960
/*! The maximum size of a aac4 ubuffer */
#define MAX_AAC4_UBUFFER	1024
/*! The maximum size of a gamr ubuffer */
#define MAX_GAMR_UBUFFER	512
/*! The maximum size of a g711 ubuffer */
#define MAX_G711_UBUFFER	4096

#endif //_UBUFFER_LIMIT_H_
