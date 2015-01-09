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
 * venc_display.c
 *
 * \brief
 * Video display process implement.
 *
 * \date
 * 2009/09/11
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#if defined(_VPL_VOC)

#include "venc_display_local.h"

/* ========================================================================== */
SCODE VencDisplay_Initial(HANDLE *phDispObj, TDisplayInitOpt tInitOpt)
{
	TDisplayInfo    *ptInfo;
    int             iRet    = -1;

    ptInfo = (TDisplayInfo *)malloc(sizeof(TDisplayInfo));
    *phDispObj = (HANDLE)(ptInfo);

	if (phDispObj == NULL)	return ERR_OUT_OF_MEMORY;

    memset( ptInfo, 0, sizeof(TDisplayInfo) );

    if (sem_init(&(ptInfo->smStart),0,0)  != 0) {
    	return S_FAIL;
    }
	if (sem_init(&(ptInfo->smEnd),0,0) != 0) {
    	return S_FAIL;
	}
	if (sem_init(&(ptInfo->smOpt),0,1) != 0) {
    	return S_FAIL;
	}
	if (sem_init(&(ptInfo->smMsg),0,1) != 0) {
    	return S_FAIL;
	}
    if (sem_init(&(ptInfo->smDisp),0,0) != 0) {
    	return S_FAIL;
    }

    ptInfo->dwParentPID         = getpid();
    ptInfo->dwDevNum            = tInitOpt.dwDevNum;
    ptInfo->hMemMgrObj          = tInitOpt.hMemMgrObj;
	ptInfo->dwSharedBufMinorNum = tInitOpt.dwSharedBufferMinorNum;
	ptInfo->bNeed2Display       = FALSE;

    memcpy(&ptInfo->_tInitInfo, 
           &tInitOpt.tInitNeedInfo, 
           sizeof(_TDisplayInitNeedInfo));

    //!device has checked at main thread
	if (!tInitOpt.szConfigFile) {
        ptInfo->szCfgFile  = DISPLAY_DEFAULT_CONFIG_FILE;
    } 
    else ptInfo->szCfgFile = tInitOpt.szConfigFile;

    if (access(ptInfo->szCfgFile, F_OK) != 0) {
        fprintf(stderr, "%s Err: access cfg \"%s\": (%d) %s\n", 
                DISP_PRE, ptInfo->szCfgFile, errno, strerror(errno));
        return S_FAIL;
    }
    DISP_DP("%s Configuration file \"%s\"\n", DISP_PRE, ptInfo->szCfgFile);

	//! read config setting about some initial needed infomation
	if (VencDisplay_ParseInitNeedConfig(ptInfo) != S_OK) {
        fprintf(stderr, "%s Err: ParseInitNeedConfig\n", DISP_PRE);
		return S_FAIL;
	}

    //! Initial Display VML instance
    if (VencDisplay_InitVML(ptInfo) != S_OK) {
    	fprintf(stderr, "%s Err: InitVML\n", DISP_PRE);
        return S_FAIL;
    }

    //! Creat Display loop thread
    iRet = pthread_create(&(ptInfo->ptThreadID), 
                          NULL,
                          VencDisplay_Loop,
                          (HANDLE)ptInfo);
    if (iRet != 0) {
    	fprintf(stderr, "%s Err: pthread_create %d\n", DISP_PRE, iRet);
        return S_FAIL;
    }
    return S_OK;
}

/* ========================================================================== */
SCODE VencDisplay_Release(HANDLE *phDispObj)
{
    TDisplayInfo *ptInfo = (TDisplayInfo *)(*phDispObj);

    if (ptInfo != NULL) {
		ptInfo->bTerminateThread = TRUE;

        sem_post(&(ptInfo->smStart));

        sem_wait(&(ptInfo->smEnd));
		//! it should be call after smEnd is post when thread exit.
		VencDisplay_ReleaseVML(ptInfo);

        //! release semaphores
        sem_destroy(&(ptInfo->smStart));
        sem_destroy(&(ptInfo->smEnd));
        sem_destroy(&(ptInfo->smOpt));
        sem_destroy(&(ptInfo->smMsg));
        sem_destroy(&(ptInfo->smDisp));
	
	    if (ptInfo->szI2CDevice != NULL) {
    	    free(ptInfo->szI2CDevice);
    	    ptInfo->szI2CDevice = NULL;
	    }
        free(ptInfo);
        *phDispObj = NULL;
        DISP_DP("%s VencDisplay_Release done\n", DISP_PRE);
    }
    return S_OK;	
}

/* ========================================================================== */
SCODE VencDisplay_Start(HANDLE hDispObj)
{
    TDisplayInfo *ptInfo = (TDisplayInfo *)(hDispObj);
    
    if (ptInfo->bRunning == TRUE) return S_OK;
    
    ptInfo->bRunning = TRUE;
    DISP_DP("%s VencDisplay_Start\n", DISP_PRE);
    sem_post(&(ptInfo->smStart));

    return S_OK;
}

/* ========================================================================== */
SCODE VencDisplay_Stop(HANDLE hDispObj)
{
    TDisplayInfo *ptInfo = (TDisplayInfo *)(hDispObj);

    if (ptInfo->bRunning == FALSE) return S_OK;

    ptInfo->bExitInnerLoop = FALSE;
    ptInfo->bRunning       = FALSE;

	sem_post(&(ptInfo->smDisp));

    while (ptInfo->bExitInnerLoop == FALSE) {
        DISP_DP("%s VencDisplay_Stop: waiting\n", DISP_PRE);
        usleep(100000);
    }
    DISP_DP("%s VencDisplay_Stop\n", DISP_PRE);

    return S_OK;
}

/* ========================================================================== */
void VencDisplay_ResetConfigFlag(HANDLE hDispObj)
{
    TDisplayInfo *ptInfo = (TDisplayInfo *)(hDispObj);
	if (ptInfo == NULL) {
		fprintf(stderr, "%s Err: %s: null instance\n", DISP_PRE, __FUNCTION__);
		return;		
	}
    
    sem_wait(&(ptInfo->smOpt));
    ptInfo->bReConf_flag = TRUE;
    sem_post(&(ptInfo->smDisp));
    sem_post(&(ptInfo->smOpt));
}

/* ========================================================================== */
void VencDisplay_MsgReceive(HANDLE hDispObj, TDisplayMsgOptions tDispMsgOpt)
{
    TDisplayInfo *ptInfo = (TDisplayInfo *)(hDispObj);
	if (hDispObj == NULL) {
		fprintf(stderr, "%s Err: %s: null instance\n", DISP_PRE, __FUNCTION__);
		return;		
	}

    switch(tDispMsgOpt.edispMsgOptType) {
    case VENCDISPLAY_MSGOPTION_DUMP_VIDEOCAPBUF:
        ptInfo->tMsgOptRec.bDumpVCapBuf = TRUE;
        break;
    case VENCDISPLAY_MSGOPTION_DUMP_VIDEODISPBUF:
        DISP_DP("%s VENCDISPLAY_MSGOPTION_DUMP_VIDEODISPBUF\n", DISP_PRE);
        ptInfo->tMsgOptRec.bDumpVDispBuf = TRUE;
        break;
    case VENCDISPLAY_MSGOPTION_START_DISPLAY:
        DISP_DP("%s VENCDISPLAY_MSGOPTION_START_DISPLAY\n", DISP_PRE);
        ptInfo->tMsgOptRec.bNeed2Display = TRUE;
        sem_post(&(ptInfo->smDisp));
        break;
    case VENCDISPLAY_MSGOPTION_STOP_DISPLAY:
        DISP_DP("%s VENCDISPLAY_MSGOPTION_STOP_DISPLAY\n", DISP_PRE);
        ptInfo->tMsgOptRec.bNeed2Display = FALSE;
        break;
    default:
        fprintf(stderr, "%s Err: Unknown message option\n", DISP_PRE);
        break;
    }  

	sem_wait(&(ptInfo->smMsg));
	ptInfo->bReMsg_flag = TRUE;
	sem_post(&(ptInfo->smMsg));
}
#endif

/* ========================================================================== */
