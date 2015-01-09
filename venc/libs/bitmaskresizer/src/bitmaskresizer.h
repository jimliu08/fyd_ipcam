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
 * bitmaskresizer.h
 *
 * \brief
 * Header file of bitmaskresizer library
 * 
 * \date
 * 2010/02/06
 * 
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */


/* ============================================================================================== */
#ifndef __BITMASKRESIZER_H__
#define __BITMASKRESIZER_H__

/* ============================================================================================== */
#include "typedef.h"
#include "errordef.h"
#include "global_codec.h"

/* ============================================================================================== */
#define BITMASKRESIZER_VERSION MAKEFOURCC(1, 0, 0, 1)

/* ============================================================================================== */
typedef struct bitmaskresizer_init_options
{
	DWORD dwVersion;

	void *pObjectMem;
} TBitMaskResizerInitOptions;

typedef struct bitmaskresizer_state
{
	DWORD dwInWidth;
	DWORD dwInHeight;
	DWORD dwInStride;

	DWORD dwOutWidth;
	DWORD dwOutHeight;
	DWORD dwOutStride;

	PBYTE pbyInBitMask;
	PBYTE pbyOutBitMask;
} TBitMaskResizerState;

typedef struct bitmaskresizer_options
{
	EVideoOptionFlags eOptionFlags;
	DWORD adwUserData[3];
} TBitMaskResizerOptions;

/* ============================================================================================== */
SCODE BitMaskResizer_Initial(HANDLE *phObject, TBitMaskResizerInitOptions *ptInitOptions);
SCODE BitMaskResizer_Release(HANDLE *phObject);

SCODE BitMaskResizer_ProcessOneFrame(HANDLE hObject, TBitMaskResizerState *ptState);

SCODE BitMaskResizer_SetOptions(HANDLE hObject, TBitMaskResizerOptions *ptOptions);
SCODE BitMaskResizer_CheckParam(HANDLE hObject, TBitMaskResizerState *ptState);
DWORD BitMaskResizer_QueryMemSize(TBitMaskResizerInitOptions *ptInitOptions);
SCODE BitMaskResizer_GetVersionInfo(PBYTE pbyMajor, PBYTE pbyMinor, PBYTE pbyBuild, PBYTE pbyRevision);

/* ============================================================================================== */
#endif //__BITMASKRESIZER_H__
