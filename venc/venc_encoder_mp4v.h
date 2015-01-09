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
 * venc_encoder_mp4v.h
 *
 * \brief
 * header file of mp4v relative function declaration.
 *
 * \date
 * 2012/09/14
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_ENCODER_MP4V_H_
#define _VENC_ENCODER_MP4V_H_

#ifdef _VMA_MEBE
#include "Mp4VEnc.h"

void VencEncoder_Mp4VApplyOpt(HANDLE hObject);
void VencEncoder_Mp4VForceCI(HANDLE hObject, EVideoFormat vFormat);
void VencEncoder_Mp4VEncode(HANDLE hObject, TVideoFrameBuffer *ptVFB);
void VencEncoder_Mp4VRestart(HANDLE hObject);
void VencEncoder_Mp4VChangeFrameSize(HANDLE hObject);
void VencEncoder_Mp4VChangeInFrameSize(HANDLE hObject, DWORD dwStride);
void VencEncoder_Mp4VChangeEncodeMode(HANDLE hObject, DWORD dwUserData0, DWORD dwUserData1, DWORD dwUserData2);
#else

#define VencEncoder_Mp4VApplyOpt(...)
#define VencEncoder_Mp4VForceCI(...)
#define VencEncoder_Mp4VEncode(...)
#define VencEncoder_Mp4VRestart(...)
#define VencEncoder_Mp4VChangeFrameSize(...)
#define VencEncoder_Mp4VChangeInFrameSize(...)
#define VencEncoder_Mp4VChangeEncodeMode(...)
#endif

#endif //_VENC_ENCODER_MP4V_H_
