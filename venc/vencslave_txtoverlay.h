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
 * vencslave_txtoverlay.c
 *
 * \brief
 * Text overlay of venc
 *
 * \date
 * 2008/04/09
 *
 * \author
 * Cheng-Ching Huang
 *
 *******************************************************************************
 */

#ifndef _VENCSLAVE_TXTOVERLAY_H_
#define _VENCSLAVE_TXTOVERLAY_H_

#include "typedef.h"
#include "errordef.h"
#include "dbgdefs.h"
#include "videodef.h"
#include "videocomm.h"

/* ========================================================================= */
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H
#include FT_GLYPH_H

#define TEXT_OVERLAY

typedef struct bitmap_record {
    FT_Bitmap         bitmap;
    FT_Int            bitmap_left;
    FT_Int            bitmap_top;
    FT_Pos            horiAdvance;    
} BITMAP_REC;

#define MAX_CHAR_NUM    128     //total ASCII code
#define MAX_FONT_PATH_LEN   127
#define MAX_CUSTOM_TEXT_LEN	127
#define MAX_OVERLAYED_TEXT_LEN 255
#define MAX_OVERLAYED_TEXT_HEIGHT 48

typedef struct vencslave_text_overlay_options
{
	/* Configured externally */
	BOOL	bTextOverlayEnable;
	CHAR	szFontPath[MAX_FONT_PATH_LEN + 1];
	CHAR	szCustomText[MAX_CUSTOM_TEXT_LEN + 1];
	BOOL	bMilliSecEnable;
	/* Internal members */
	FT_Library	ftLib;
	FT_Face		ftFace;
	SDWORD		sdwNominalHeight;
	/* the following 4 variables (sdwLineHeight, sdwOrigin, sdwLeftMargin, 
	   sdwRightMargin) control the margin around the overlayed text string. 
	   you may customize them in function VencSlave_TextOverlay_SetOptions. */
	SDWORD		sdwLineHeight; /*! the height of the overlayed buffer */
	SDWORD		sdwOrigin;     /*! the origin of a glyph (horizontal text layout)  */
	SDWORD		sdwLeftMargin; /*! the left margin */
	SDWORD		sdwRightMargin; /*! the right margin */
	//CHAR		szText[MAX_OVERLAYED_TEXT_LEN + 1]; /*! the string represented in pbyTextBitMap */
	DWORD		dwBitMapStride;      /*! this variable also serves as an option-has-modified indicator. */
	DWORD		dwBitMapWidth;
	DWORD		dwCurrentSecond;
	BYTE		*pbyTextBitMap;
	BITMAP_REC  BitmapRec[MAX_CHAR_NUM];
} TTextOverlayOptions;

SCODE VencSlave_TextOverlay_Initial(TTextOverlayOptions *pData, const DWORD dwVideoMaxWidth);
void VencSlave_TextOverlay_Release(TTextOverlayOptions *pData);
SCODE VencSlave_TextOverlay_SetOptions(TTextOverlayOptions *pData, TTextOverlayOptions *pOptions);
void VencSlave_TextOverlay(TVideoFrameBuffer *ptTempVFB, TTextOverlayOptions *pOptions, const DWORD dwVideoMaxWidth);

#endif //_VENCSLAVE_TXTOVERLAY_H_


