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
 * venc_motion.c
 *
 * \brief
 * Video motion process implement.
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

/* ========================================================================== */
SCODE VencMotion_Initial(HANDLE *phMotionObj, TMotionInitOpt tInitOpt)
{
    TMotionInfo *ptInfo = NULL;
    int         iRet    = -1;

    ptInfo = (TMotionInfo *)malloc( sizeof(TMotionInfo) );
    *phMotionObj = (HANDLE)(ptInfo);    

    if (phMotionObj == NULL)    return ERR_OUT_OF_MEMORY;

    memset(ptInfo, 0, sizeof(TMotionInfo));

    if (sem_init(&(ptInfo->smStart), 0, 0) != 0) return S_FAIL;
	if (sem_init(&(ptInfo->smEnd),   0, 0) != 0) return S_FAIL;
	if (sem_init(&(ptInfo->smOpt),   0, 1) != 0) return S_FAIL;

    ptInfo->dwParentPID         = getpid();
    ptInfo->hMemMgrObj          = tInitOpt.hMemMgrObj;
	ptInfo->dwSharedBufMinorNum = tInitOpt.dwSharedBufferMinorNum;
	memcpy( &ptInfo->_tInitInfo, 
            &tInitOpt.tInitNeedInfo, 
            sizeof(_TMotionInitNeedInfo) );
	
    if (!tInitOpt.szConfigFile) {
        ptInfo->szCfgFile = MOTION_DEFAULT_CONFIG_FILE;
    } 
    else {
        ptInfo->szCfgFile = tInitOpt.szConfigFile;
    }

    if (access(ptInfo->szCfgFile, F_OK) != 0) {
        fprintf(stderr, "%s Err: access \"%s\": (%d) %s\n", 
                MOT_PRE, ptInfo->szCfgFile, errno, strerror(errno));
        return S_FAIL;
    }
    MOT_DP("%s config file: \"%s\"\n", MOT_PRE, ptInfo->szCfgFile);

    //! Initial Motion VML instance
    if (VencMotion_InitVML(ptInfo) != S_OK) {
    	fprintf(stderr, "%s Err: VencMotion_InitVML\n", MOT_PRE);
        return S_FAIL;
    }

	VencMotion_InitSocket(ptInfo);

    {
#if 1
        pthread_attr_t      attr1;
        struct sched_param  param1;
        
        param1.sched_priority = 50;
        pthread_attr_init(&attr1);

    	//! An inherit value of PTHREAD_INHERIT_SCHED (the default) 
    	//! means that the scheduling policies defined in the creating 
    	//! thread are to be used, and any scheduling attributes defined 
    	//! in the pthread_create() call are to be ignored. 
    	//! If PTHREAD_EXPLICIT_SCHED is used, the attributes from the 
        //! pthread_create() call are to be used. 
        pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setschedpolicy (&attr1, SCHED_FIFO);
        pthread_attr_setschedparam  (&attr1, &param1);
#endif
    	//! Creat video encoder loop thread
    	iRet = pthread_create( &(ptInfo->ptThreadID), 
                               &attr1,
                               VencMotion_Loop,
                               (HANDLE)ptInfo );

    	pthread_attr_destroy(&attr1);
    }

    if (iRet != 0) {
        fprintf(stderr, "%s Err: pthread_create: %d\n", MOT_PRE, iRet);
        return S_FAIL;
    }

    return S_OK;
}

/* ========================================================================== */
SCODE VencMotion_Release(HANDLE *phMotionObj)
{
    TMotionInfo *ptInfo = (TMotionInfo *)(*phMotionObj);

    if (ptInfo != NULL) {
		ptInfo->bTerminateThread = TRUE;

        sem_post( &(ptInfo->smStart) );
        sem_wait( &(ptInfo->smEnd)   );
		//! it should be call after smEnd is post when thread exit.
		VencMotion_ReleaseVML(ptInfo);

		close(ptInfo->tRtspSckInfo.fdSock);
		close(ptInfo->tEvtSckInfo.fdSock);
        //! release semaphores
        sem_destroy( &(ptInfo->smStart) );
        sem_destroy( &(ptInfo->smEnd)   );
        sem_destroy( &(ptInfo->smOpt)   );
        if (ptInfo->tRtspSckInfo.szUSckPath != NULL) {
            free(ptInfo->tRtspSckInfo.szUSckPath);
            ptInfo->tRtspSckInfo.szUSckPath = NULL;
        }
        if (ptInfo->tEvtSckInfo.szUSckPath != NULL) {
            free(ptInfo->tEvtSckInfo.szUSckPath);
            ptInfo->tEvtSckInfo.szUSckPath = NULL;
        }
        free(ptInfo);
        *phMotionObj = NULL;
        MOT_DP("%s %s - done\n", MOT_PRE, __FUNCTION__);
    }
    return S_OK;	
}

/* ========================================================================== */
SCODE VencMotion_Start(HANDLE hMotionObj)
{
	TMotionInfo *ptInfo = (TMotionInfo *) hMotionObj;

    if (ptInfo->bRunning == TRUE)   return S_OK;

    ptInfo->bRunning = TRUE;
    MOT_DP("%s %s\n", MOT_PRE, __FUNCTION__);
    sem_post( &(ptInfo->smStart) );
	return S_OK;
}

/* ========================================================================== */
SCODE VencMotion_Stop(HANDLE hMotionObj)
{
    TMotionInfo *ptInfo = (TMotionInfo *) hMotionObj;

    if (ptInfo != NULL) {
	    if (ptInfo->bRunning == FALSE) return S_OK;
	    
	    ptInfo->bExitInnerLoop = FALSE;
        ptInfo->bRunning = FALSE;
    
        while (ptInfo->bExitInnerLoop == FALSE) {
            MOT_DP("%s %s - waiting\n", MOT_PRE, __FUNCTION__);
            usleep(100000);
        }
        MOT_DP("%s %s - done\n", MOT_PRE, __FUNCTION__);
    }
    return S_OK;
}

/* ========================================================================== */
void VencMotion_ResetConfigFlag(HANDLE hMotionObj)
{
    TMotionInfo *ptInfo = (TMotionInfo *) hMotionObj;
    if (ptInfo == NULL) {
        fprintf(stderr, "%s Err: %s: null instance\n", MOT_PRE, __FUNCTION__);
        return;        
    }

    sem_wait( &(ptInfo->smOpt) );
    ptInfo->bReCfg = TRUE;
    sem_post( &(ptInfo->smOpt) );
}

/* ========================================================================== */

