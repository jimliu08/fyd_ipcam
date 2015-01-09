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
 * venc_display.h
 *
 * \brief
 * Head file of video display process implement.
 *
 * \date
 * 2009/09/11
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_DISPLAY_H_
#define _VENC_DISPLAY_H_

#include "typedef.h"
#include "errordef.h"

typedef struct venc_display_init_need_info
{
    //! sensor max support size, that match VIDEO_MAX_WIDTH in past definition
    DWORD   dwVideoMaxWidth;
    DWORD   dwVideoMaxHeight;
    //! capture size, that match VIDEO_ENC_WIDTH in past definition
    DWORD   dwVideoCapWidth;
    DWORD   dwVideoCapHeight;
    //! sensor in size, that match VIDEO_IN_WIDTH in past definition
    DWORD   dwVideoInWidth;
    DWORD   dwVideoInHeight;
    //! voc_output
    DWORD   dwVOCOutputDRAMLoc;    
} TDisplayInitNeedInfo;

typedef struct venc_display_initopt
{
    //! device number of sensor input
    DWORD                   dwDevNum;
    //! string pointer of config file name
    CHAR                    *szConfigFile;
    //! record the man process
    DWORD                   dwCallbackInstance;
    //! MemMgr pass from outside
    HANDLE                  hMemMgrObj;
    //! SharedBuffer Minor Num between VideoIn and VideoDisp
    DWORD                   dwSharedBufferMinorNum;
    //! init need info from venc
    TDisplayInitNeedInfo    tInitNeedInfo;
} TDisplayInitOpt;

typedef enum venc_display_msgoptiontype
{
    VENCDISPLAY_MSGOPTION_DUMP_VIDEOCAPBUF = 1,
    VENCDISPLAY_MSGOPTION_DUMP_VIDEODISPBUF,
    VENCDISPLAY_MSGOPTION_START_DISPLAY,
    VENCDISPLAY_MSGOPTION_STOP_DISPLAY
} EDisplayMsgOptType;

typedef struct venc_display_msgoptions
{
    EDisplayMsgOptType  edispMsgOptType;
} TDisplayMsgOptions;

SCODE VencDisplay_Initial(HANDLE *phDisplayProcObj, 
                          TDisplayInitOpt tDisplayInitOpt);
SCODE VencDisplay_Release(HANDLE *phDisplayProcObj);
SCODE VencDisplay_Start(HANDLE hDisplayProcObj);
SCODE VencDisplay_Stop(HANDLE hDisplayProcObj);
void VencDisplay_ResetConfigFlag(HANDLE hDisplayProcObj);
void VencDisplay_MsgReceive(HANDLE hDisplayProcObj, 
                            TDisplayMsgOptions tDisplayMsgOpt);

#endif //_VENC_DISPLAY_H_

