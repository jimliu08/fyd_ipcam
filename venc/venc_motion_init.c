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
 * venc_motion_init.c
 *
 * \brief
 * Initial each required component
 *
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#include "venc_motion_local.h"

extern TXmlWrapperTreeMap ptMotionTreeMap[];

/* ========================================================================== */
void VencMotion_GetSckPath(HANDLE hObject, const CHAR *szParam, SDWORD len)
{   
    TMotionInfo *ptInfo             = (TMotionInfo *)(hObject);
    CHAR        szSckPath[len+1];

    snprintf(szSckPath, len+1, "%s", szParam);
    ptInfo->tRtspSckInfo.szUSckPath = strdup(szSckPath);
    MOT_DP( "%s RTSP socket path \"%s\"\n", 
            MOT_PRE, ptInfo->tRtspSckInfo.szUSckPath );
    return;
}

/* ========================================================================== */
void VencMotion_GetEventPath(HANDLE hObject, const CHAR *szParam, SDWORD len)
{   
    TMotionInfo *ptInfo             = (TMotionInfo *)(hObject);
    CHAR        szSckPath[len+1];

    snprintf(szSckPath, len+1, "%s", szParam);
    ptInfo->tEvtSckInfo.szUSckPath = strdup(szSckPath);
    MOT_DP("%s event socket path \"%s\"\n", 
           MOT_PRE, ptInfo->tEvtSckInfo.szUSckPath );
    return;
}

/* ========================================================================== */
DWORD VencMotion_GetMemory(HANDLE       hMemObject,   
                           DWORD        dwBusNum, 
                           DWORD        dwSize,        
                           EAlignType   eAlignType)
{
    TMemMgrState tState;

    memset( &(tState), 0, sizeof(TMemMgrState) );
    tState.dwBusNum   = dwBusNum;
    tState.dwSize     = dwSize;
    tState.eAlignType = eAlignType;
    MOT_DP("%s MemMgr_GetMemory, bus %d, size %d, aligntype %d\n", 
           MOT_PRE, dwBusNum, dwSize, eAlignType);
    return ( MemMgr_GetMemory(hMemObject, &tState) );
}

/* ========================================================================== */
void initSocket(TSckInfo *ptSckInfo, const char *szSckName)
{
    int iRet = -1;
    int iLen = 0;

    //! Create the unix socket
    ptSckInfo->fdSock = socket(AF_UNIX, SOCK_DGRAM, 0);
    memset( &(ptSckInfo->sunx), 0, sizeof(ptSckInfo->sunx) );
    ptSckInfo->sunx.sun_family = AF_UNIX;
    (void) strncpy(ptSckInfo->sunx.sun_path, szSckName, strlen(szSckName));
    iLen = 
        sizeof(ptSckInfo->sunx.sun_family) + strlen(ptSckInfo->sunx.sun_path);
    iRet = connect( ptSckInfo->fdSock, 
                    (struct sockaddr *)&ptSckInfo->sunx, 
                    iLen);
    if (iRet < 0) {
        fprintf(stderr, "%s Err: connect \"%s\": (%d) %s\n", 
                MOT_PRE, szSckName, errno, strerror(errno));
    }
    ptSckInfo->bNeedConf = TRUE;
    return;
}

/* ========================================================================== */
SCODE VencMotion_InitSocket(TMotionInfo *ptInfo)
{
    HANDLE                  hXMLObj;
    TXmlWrapperInitOptions  tInitOpt;
    SCODE                   scRet       = S_FAIL;
    
    //! Initial XMLWrapper to get socket path
    TXmlWrapperTreeMap ptSckPathMap[] = 
    {    
        {"root/venc_motion/socket_path", NULL, &VencMotion_GetSckPath,   NULL},
        {"root/venc_motion/event_path",  NULL, &VencMotion_GetEventPath, NULL},
        {NULL, NULL, NULL}
    };

    memset( &(tInitOpt), 0, sizeof(TXmlWrapperInitOptions) );
    tInitOpt.dwVersion = XMLWRAPPER_VERSION;
    if (XmlWrapper_Initial(&hXMLObj, &tInitOpt) != S_OK) {
        return S_FAIL;
    }
    if (XmlWrapper_SetHandler(hXMLObj, ptSckPathMap, NULL) != S_OK) {
        fprintf(stderr, "%s Err: XmlWrapper_SetHandler\n", MOT_PRE);
        return S_FAIL;
    }
    scRet = XmlWrapper_ReadFile_UsrDefFunc(ptInfo->szCfgFile, 
                                           hXMLObj, 
                                           ptInfo);
    if (scRet != S_OK) {
        fprintf(stderr, 
                "%s Err: XmlWrapper_ReadFile_UsrDefFunc \"%s\": 0x%08x\n", 
                MOT_PRE, ptInfo->szCfgFile, scRet);
        return S_FAIL;
    }
    if (XmlWrapper_Release(&hXMLObj) != S_OK) {
        fprintf(stderr, "%s Err: XmlWrapper_Release\n", MOT_PRE);
        return S_FAIL;
    }

    initSocket( &(ptInfo->tRtspSckInfo), ptInfo->tRtspSckInfo.szUSckPath );
    ptInfo->tRtspSckInfo.bNeedConf = TRUE;
    initSocket( &(ptInfo->tEvtSckInfo), ptInfo->tEvtSckInfo.szUSckPath );

    //! set Event socket to non-blocking mode
    {
        int flags;
        flags = fcntl(ptInfo->tEvtSckInfo.fdSock, F_GETFL, 0);
        if ( flags < 0 ) {
            fprintf(stderr, "%s Err: fcntl F_GETFL: (%d) %s\n", 
                    MOT_PRE, errno, strerror(errno));
        }
        flags |= O_NONBLOCK;
        if (fcntl(ptInfo->tEvtSckInfo.fdSock, F_SETFL, flags) < 0) {
            fprintf(stderr, "%s Err: fcntl F_SETFL: (%d) %s\n", 
                    MOT_PRE, errno, strerror(errno));
        }
    }
    return S_OK;
}

/* ========================================================================== */
SCODE VencMotion_InitXMLWrapper(HANDLE *phXMLWrapperObject)
{
    TXmlWrapperInitOptions  tInitOpt;
    SCODE                   scRet       = S_FAIL;

    memset( &(tInitOpt), 0, sizeof(TXmlWrapperInitOptions) );
        
    tInitOpt.dwVersion = XMLWRAPPER_VERSION;
    if ( XmlWrapper_Initial(phXMLWrapperObject, &tInitOpt) != S_OK ) {
        fprintf(stderr, "%s Err: XmlWrapper_Initial\n", MOT_PRE);
        return S_FAIL;
    }
    scRet = XmlWrapper_SetHandler((*phXMLWrapperObject), ptMotionTreeMap, NULL);
    if (scRet  != S_OK ) {
        fprintf(stderr, "%s Err: XmlWrapper_SetHandler\n", MOT_PRE);
        return S_FAIL;
    }
    return S_OK;
}
#ifdef _VMA_JEBE
/* ========================================================================== */
SCODE VencMotion_InitJPEGEnc(TMotionInfo                *ptInfo, 
                             const _TMotionInitNeedInfo *const _ptInitInfo)
{
    TJPEGEncInitOptions tInitOpt;
    memset( &(tInitOpt), 0, sizeof(TJPEGEncInitOptions) );
    
    tInitOpt.dwVersion           = JPEGENC_VERSION;
    tInitOpt.dwInWidth           = _ptInitInfo->dwVideoMaxWidth;
    tInitOpt.dwInHeight          = _ptInitInfo->dwVideoMaxHeight;
    tInitOpt.dwEncWidth          = _ptInitInfo->dwVideoCapWidth;
    tInitOpt.dwEncHeight         = _ptInitInfo->dwVideoCapHeight;
    tInitOpt.dwInHorzOffset      = 0;
    tInitOpt.dwInVertOffset      = 0;
    tInitOpt.dwQualityFactor     = 50;
    tInitOpt.dwRstIntNum         = 0;
    tInitOpt.bJFIFHdr            = FALSE;
    tInitOpt.bHighCompression    = FALSE;
    tInitOpt.eQTableType         = JPEGENC_Q_TABLE_TYPE_STANDARD;
    tInitOpt.eColorFlags         = COLOR;
    tInitOpt.ePixelFormatFlags   = YUV420;
    tInitOpt.dwBSBufSize         = 0;
    tInitOpt.dwTimeOut           = 1;
    tInitOpt.dwSemaphore         = (DWORD)NULL;
    tInitOpt.dwUserData          = 0;
    tInitOpt.eBufTypeFlags       = JPEGNONCIRCULAR;
    tInitOpt.pfnSend             = NULL;
    tInitOpt.pfnEnter            = NULL;
    tInitOpt.pfnLeave            = NULL;
    tInitOpt.hAppSendObject      = NULL;

    tInitOpt.pbyYFrame           = NULL;
    tInitOpt.pbyCbFrame          = NULL;
    tInitOpt.pbyCrFrame          = NULL;
    tInitOpt.pbyUserData         = NULL;
    tInitOpt.pObjectMem          = NULL;

    if ( JPEGEnc_Initial(&(ptInfo->hJPEGEncObj), &tInitOpt) != S_OK ) {
        fprintf(stderr, "%s Err: JPEGEnc_Initial\n", MOT_PRE);
        return S_FAIL;
    }
    return S_OK;
}
#endif
/* ========================================================================== */
SCODE VencMotion_InitVideoInSharedBuffer(TMotionInfo *ptInfo)
{
    TSharedBuffMgrInitOptions tInitOpt;
    memset( &(tInitOpt), 0, sizeof(TSharedBuffMgrInitOptions) );
    
    //! Initial sharedbuffergr
    tInitOpt.dwVersion = SHAREDBUFFMGR_VERSION;
    tInitOpt.pObjectMem = NULL;
    tInitOpt.dwMinorNum = ptInfo->dwSharedBufMinorNum;
    tInitOpt.eProcessRole = esbmprReader;

    MOT_DP("%s %s dwMinorNum = %u \n", 
           MOT_PRE, __FUNCTION__, tInitOpt.dwMinorNum);
    
    if (SharedBuffMgr_Initial(&(ptInfo->hVideoInSrdObj), &tInitOpt) != S_OK) {
        fprintf(stderr, "%s Err: SharedBuffMgr_Initial\n", MOT_PRE);
        return S_FAIL;
    }    
    return S_OK;
}

/* ========================================================================== */
SCODE VencMotion_InitVML(TMotionInfo *ptInfo)
{
#ifdef _VMA_JEBE
	DWORD dwVideoCodecMaskRet = 0;
#endif    
    //! get output bitstream buffer, for encode JPEG used
    ptInfo->pbyEncOutBuf = 
        (BYTE *)VencMotion_GetMemory(ptInfo->hMemMgrObj,
                                     ptInfo->_tInitInfo.dwMotionSnapDRAMLoc,
                                     ptInfo->_tInitInfo.dwBitStreamSize,
                                     ALIGN_TYPE_64_BYTE);
    if (ptInfo->pbyEncOutBuf == NULL) {
        fprintf(stderr, "%s Err: get encoder output buffer\n", MOT_PRE);
        return ERR_OUT_OF_MEMORY;
    }
    MOT_DP("%s get encoder output buffer done\n", MOT_PRE);

    //! ===== Initial instance =====
    //! Initial VideoIn Shared Buffer
    if (VencMotion_InitVideoInSharedBuffer(ptInfo) != S_OK) {
        fprintf(stderr, "%s Err: VencMotion_InitVideoInSharedBuffer\n", 
                MOT_PRE);
        return S_FAIL;
    }
    MOT_DP("%s VencMotion_InitVideoInSharedBuffer done\n", MOT_PRE);

    //! Initial XMLWrapper instance
    if (VencMotion_InitXMLWrapper(&(ptInfo->hXMLObj)) != S_OK) {
        fprintf(stderr, "%s Err: VencMotion_InitXMLWrapper\n", MOT_PRE);
        return S_FAIL;    
    }
    MOT_DP("%s VencMotion_InitXMLWrapper done\n", MOT_PRE);
#ifdef _VMA_JEBE
    //! Initial JPEG
    dwVideoCodecMaskRet = 
        (ptInfo->_tInitInfo.dwVideoCodecInitMask & VIDEO_CODEC_JPEG_INIT_BIT);

    if (dwVideoCodecMaskRet != 0) {
        if (VencMotion_InitJPEGEnc(ptInfo, &(ptInfo->_tInitInfo)) != S_OK) {
            fprintf(stderr, "%s Err: VencMotion_InitJPEGEnc\n", MOT_PRE);
            return S_FAIL;
        }
        MOT_DP("%s VencMotion_InitJPEGEnc done\n", MOT_PRE);
    }
#endif    
    return S_OK;
}

/* ========================================================================== */
void VencMotion_ReleaseVML(TMotionInfo *ptInfo)
{  
    SCODE scRet = S_FAIL;
    //! Free every buffer we allocate in EncoderInfo
    scRet = MemMgr_FreeMemory(ptInfo->hMemMgrObj, (DWORD)ptInfo->pbyEncOutBuf);
    if (scRet != S_OK) {
        fprintf(stderr, "%s Err: MemMgr_FreeMemory EncOut %p - 0x%08x\n", 
                MOT_PRE, ptInfo->pbyEncOutBuf, scRet);
    }

    //! ===== Release Instance =====  
    //! Release shared buffer instance
    if (SharedBuffMgr_Release(&(ptInfo->hVideoInSrdObj)) != S_OK) {
        fprintf(stderr, "%s Err: SharedBuffMgr_Release\n", MOT_PRE);
    }    

    //! Release xmlwrapper instance
    if (XmlWrapper_Release(&(ptInfo->hXMLObj)) != S_OK) {
        fprintf(stderr, "%s Err: XmlWrapper_Release\n", MOT_PRE);
    }
#ifdef _VMA_JEBE
    //! Release JPEG encoder
    if (ptInfo->hJPEGEncObj!=NULL) {
        if (JPEGEnc_Release(&(ptInfo->hJPEGEncObj)) != S_OK) {
            fprintf(stderr, "%s Err: JPEGEnc_Release\n", MOT_PRE);
        }
    }
#endif
    return;
}

/* ========================================================================== */

