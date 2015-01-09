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
 * venc_app.h
 *
 * \brief
 * Head file of video process implement.
 *
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_APP_H_
#define _VENC_APP_H_
#include "typedef.h"
#include "errordef.h"

typedef struct venc_app_initopt
{
    //! device number of sensor input
    DWORD dwDevNum;
    //! string pointer of config file name
    CHAR* szConfigFile;
    //! record the man process
    CHAR* szCommandFIFO;
} TVencInitOpt;


SCODE Venc_Initial(HANDLE *phVencObject, TVencInitOpt tVencInitOpt);
SCODE Venc_Release(HANDLE *phVencObject);
SCODE Venc_Start(HANDLE hVencObject);
SCODE Venc_Stop(HANDLE hVencObject);
SCODE Venc_LoadConfig(HANDLE hVencObject);
#endif // _VENC_APP_H
