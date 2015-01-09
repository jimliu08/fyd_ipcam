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
 * venc_encoder_jpeg.h
 *
 * \brief
 * header file of jpeg relative function declaration.
 *
 * \date
 * 2012/09/14
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_ENCODER_JPEG_H_
#define _VENC_ENCODER_JPEG_H_

#ifdef _VMA_JEBE
#include "JPEGEnc.h"
void VencEncoder_JPEGApplyOpt(HANDLE hObject);
void VencEncoder_JPEGForceCI(HANDLE hObject, EVideoFormat vFormat);
void VencEncoder_JPEGEncode(HANDLE hObject, TVideoFrameBuffer *ptVFB);
void VencEncoder_JPEGChangeFrameSize(HANDLE hObject);
void VencEncoder_JPEGChangeInFrameSize(HANDLE hObject, DWORD dwStride);
#else

#define VencEncoder_JPEGApplyOpt(...)
#define VencEncoder_JPEGForceCI(...)
#define VencEncoder_JPEGEncode(...)
#define VencEncoder_JPEGChangeFrameSize(...)
#define VencEncoder_JPEGChangeInFrameSize(...)
#endif
#endif //_VENC_ENCODER_JPEG_H_
