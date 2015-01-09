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
 * venc_encoder_h264.h
 *
 * \brief
 * header file of h264 relative function declaration.
 *
 * \date
 * 2012/09/14
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_ENCODER_H264_H_
#define _VENC_ENCODER_H264_H_

#ifdef _H4EE
#include "H264Enc.h"

void VencEncoder_H264ApplyOpt(HANDLE hObject);
void VencEncoder_H264ForceCI(HANDLE hObject, EVideoFormat vFormat);
void VencEncoder_H264Encode(HANDLE hObject, TVideoFrameBuffer* ptVFB);
void VencEncoder_H264Restart(HANDLE hObject);
void VencEncoder_H264ChangeROI(HANDLE hObject);
void VencEncoder_H264ChangeFrameSize(HANDLE hObject);
void VencEncoder_H264ChangeInFrameSize(HANDLE hObject, DWORD dwStride);
void VencEncoder_H264ChangeFrameTimeTick(HANDLE hObject, DWORD dwFrameRate);

#else

#define VencEncoder_H264ApplyOpt(...)
#define VencEncoder_H264ForceCI(...)
#define VencEncoder_H264Encode(...)
#define VencEncoder_H264Restart(...)
#define VencEncoder_H264ChangeROI(...)
#define VencEncoder_H264ChangeFrameSize(...)
#define VencEncoder_H264ChangeInFrameSize(...)
#define VencEncoder_H264ChangeFrameTimeTick(...)
#endif

#endif //_VENC_ENCODER_H264_H_
