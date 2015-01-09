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
 * venc_encoder_svc.h
 *
 * \brief
 * header file of svc relative function declaration.
 *
 * \date
 * 2012/09/17
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_ENCODER_SVC_H_
#define _VENC_ENCODER_SVC_H_

#ifdef _SVC
#include "SVCEnc.h"
typedef TUBufferConfH264  TUBufferConfSVC;

void VencEncoder_SVCApplyOpt(HANDLE hObject);
void VencEncoder_SVCForceCI(HANDLE hObject, EVideoFormat vFormat);
void VencEncoder_SVCEncode(HANDLE hObject, TVideoFrameBuffer *ptVFB);
void VencEncoder_SVCRestart(HANDLE hObject);
void VencEncoder_SVCChangeROI(HANDLE hObject);
void VencEncoder_SVCChangeFrameSize(HANDLE hObject);
void VencEncoder_SVCChangeInFrameSize(HANDLE hObject, DWORD dwStride);
#else

#define VencEncoder_SVCApplyOpt(...)
#define VencEncoder_SVCForceCI(...)
#define VencEncoder_SVCEncode(...)
#define VencEncoder_SVCRestart(...)
#define VencEncoder_SVCChangeROI(...)
#define VencEncoder_SVCChangeFrameSize(...)
#define VencEncoder_SVCChangeInFrameSize(...)
#endif

#endif //_VENC_ENCODER_SVC_H_
