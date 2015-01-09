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
 * venc_master.c
 *
 * \brief
 * Video master process implement.
 *
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#include "venc_master_local.h"


/* ============================================================================================= */
SCODE VencMaster_Initial(HANDLE *phMasterProcObj, TMasterInitOpt tMasterInitOpt)
{
    TMasterInfo *ptMasterInfo;
    int iRet;
    
    ptMasterInfo = (TMasterInfo *)malloc(sizeof(TMasterInfo));
    *phMasterProcObj = (HANDLE)(ptMasterInfo);

    if (phMasterProcObj == NULL)    return ERR_OUT_OF_MEMORY;

    memset(ptMasterInfo, 0, sizeof(TMasterInfo));

    ptMasterInfo->fdIRCut = -1;
    
    if (sem_init(&(ptMasterInfo->smStart), 0, 0) != 0) {
    	return S_FAIL;
    }
	if (sem_init(&(ptMasterInfo->smEnd), 0, 0) !=0) {
    	return S_FAIL;
	}
    if (sem_init(&(ptMasterInfo->smWakeup), 0, 0) != 0) {
    	return S_FAIL;
    }
	if (sem_init(&(ptMasterInfo->smOptions), 0, 1) != 0) {
    	return S_FAIL;
	}
	if (sem_init(&(ptMasterInfo->smMsgOptions), 0, 1) != 0) {
    	return S_FAIL;
	}

    ptMasterInfo->dwParentPID = getpid();
    ptMasterInfo->dwDevNum = tMasterInitOpt.dwDevNum;	
    ptMasterInfo->dwCallbackInstance = tMasterInitOpt.dwCallbackInstance;
    ptMasterInfo->hMemMgrObj = tMasterInitOpt.hMemMgrObj;
    memcpy(&ptMasterInfo->_tInitNeedInfo, &tMasterInitOpt.tInitNeedInfo, sizeof(_TMasterInitNeedInfo));

	//device has checked at main thread
	if (!tMasterInitOpt.szConfigFile) {
        ptMasterInfo->szConfigFile = MASTER_DEFAULT_CONFIG_FILE;
    } else {
        ptMasterInfo->szConfigFile = tMasterInitOpt.szConfigFile;
    }

    if (access(ptMasterInfo->szConfigFile, F_OK) != 0) {
        DBPRINT1("[VENC_MASTER] Configuration file %s not exist!!\n", ptMasterInfo->szConfigFile);
        return S_FAIL;
    }
    DBPRINT1("[VENC_MASTER] Configuration file %s\n", ptMasterInfo->szConfigFile);

	// if config doesn't setting, the default value is for 640x480 resolution.
	ptMasterInfo->tAER2D.fXTranslate = -1.0;
	ptMasterInfo->tAER2D.fYTranslate = 1.0;
	ptMasterInfo->tAER2D.fXScale = 0.003125; 		// (2/640)
	ptMasterInfo->tAER2D.fYScale = -0.00416667; 	// (2/480)

    // Initial Master VML instance
    if (VencMaster_InitVML(ptMasterInfo) != S_OK) {
    	fprintf(stderr, "[VENC_MASTER] VencMaster_InitVML error!!\n");
        return S_FAIL;
    }
	// initial some config parameter
	if (VencMaster_InitConfigParam(ptMasterInfo) != S_OK) {
    	fprintf(stderr, "[VENC_MASTER] VencMaster_InitConfigParam error!!\n");
    	return S_FAIL;
	}

    if (VencMaster_IRCut_Initial(ptMasterInfo) != S_OK)
    {
      fprintf(stderr, "[VENC_MASTER] IRCut_Initial error!! But venc_master can be continued to  work!!\n");
    }
    else
    {
      printf("[VENC_MASTER](%d) IRCut_Initial is successful!!\n",__LINE__);
    }

    // Creat master loop thread
    iRet = pthread_create(&(ptMasterInfo->ptThreadID), NULL,
                          VencMaster_Loop,
                          (HANDLE)ptMasterInfo);
    
    if (iRet != 0) {
    	printf("[VENC_MASTER] create master thread fail");
        return S_FAIL;
    }

    return S_OK;
}

/* ============================================================================================= */
SCODE VencMaster_Release(HANDLE *phMasterProcObj)
{
    TMasterInfo *ptMasterInfo = (TMasterInfo *)(*phMasterProcObj);
	
	if (ptMasterInfo != NULL) {
		ptMasterInfo->bTerminateThread = TRUE;

        sem_post(&(ptMasterInfo->smStart));
		sem_wait(&(ptMasterInfo->smEnd));
		
		// it should be call after smEnd is post when thread exit.
		VencMaster_ReleaseVML(ptMasterInfo);
        
        // release semaphores
        sem_destroy(&(ptMasterInfo->smStart));
        sem_destroy(&(ptMasterInfo->smEnd));
        sem_destroy(&(ptMasterInfo->smWakeup));
        sem_destroy(&(ptMasterInfo->smOptions));
        sem_destroy(&(ptMasterInfo->smMsgOptions));
        
		VencMaster_IRCut_Release(ptMasterInfo);
	    free(ptMasterInfo);	
	    *phMasterProcObj = NULL;
		DBPRINT0("[VENC_MASTER] Master Controler Release\n");
	}	
    return S_OK;
}

/* ============================================================================================= */
SCODE VencMaster_Start(HANDLE hMasterProcObj)
{
    TMasterInfo *ptMasterInfo = (TMasterInfo *)(hMasterProcObj);
    
    if (ptMasterInfo->bRunning == TRUE)
        return S_OK;
        
    ptMasterInfo->bRunning = TRUE;
    DBPRINT0("[VENC_MASTER] Master Controler Start\n");
    sem_post(&(ptMasterInfo->smStart));

    return S_OK;
}

/* ============================================================================================= */
SCODE VencMaster_Stop(HANDLE hMasterProcObj)
{
    TMasterInfo *ptMasterInfo = (TMasterInfo *)(hMasterProcObj);

    if (ptMasterInfo->bRunning == FALSE) return S_OK;

    ptMasterInfo->bExitInnerLoop = FALSE;
    ptMasterInfo->bRunning = FALSE;
    
    sem_post(&(ptMasterInfo->smWakeup));
    while (ptMasterInfo->bExitInnerLoop == FALSE) {
        DBPRINT0("[VENC_MASTER] Master Controler Stopping\n");
        usleep(100000);
    }
    DBPRINT0("[VENC_MASTER] Master Controler Stop\n");
    return S_OK;
}

/* ============================================================================================= */
void VencMaster_ResetConfigFlag(HANDLE hMasterProcObj)
{
    TMasterInfo *ptMasterInfo = (TMasterInfo *)hMasterProcObj;
	if (ptMasterInfo == NULL) {
        fprintf(stderr, "[VENC_MASTER] Master setoptions fail: NULL Pointer!!\n");
        return;		
	}
    sem_wait(&(ptMasterInfo->smOptions));
	ptMasterInfo->bReConf_flag = TRUE;
    sem_post(&(ptMasterInfo->smOptions));
    sem_post(&(ptMasterInfo->smWakeup));
}

/* ============================================================================================= */
void VencMaster_MsgReceive(HANDLE hMasterProcObj, TMasterMsgOptions tMasterMsgOpt)
{
    TMasterInfo *ptMasterInfo = (TMasterInfo *)hMasterProcObj;
	if (hMasterProcObj == NULL) {
		fprintf(stderr, "[VENC_MASTER] VencMaster_MsgReceive fail: NULL Pointer!!\n");
		return;		
	}

    switch(tMasterMsgOpt.emasMsgOptType) {
		case VENCMASTER_MSGOPTION_WDR_ON:
            ptMasterInfo->tMsgOptRec.tWDRStatus.bModified = TRUE;
            ptMasterInfo->tMsgOptRec.tWDRStatus.bEnable = TRUE;
            break;
        case VENCMASTER_MSGOPTION_WDR_OFF:
            ptMasterInfo->tMsgOptRec.tWDRStatus.bModified = TRUE;
            ptMasterInfo->tMsgOptRec.tWDRStatus.bEnable = FALSE;
            break;
		case VENCMASTER_MSGOPTION_ENABLE_AF:
	    	ptMasterInfo->tMsgOptRec.tAFStatus.bModified = TRUE;
            ptMasterInfo->tMsgOptRec.tAFStatus.bEnable = TRUE;
		   	break;
		case VENCMASTER_MSGOPTION_DISABLE_AF:
	    	ptMasterInfo->tMsgOptRec.tAFStatus.bModified = TRUE;
            ptMasterInfo->tMsgOptRec.tAFStatus.bEnable = FALSE;
	    	break;
		case VENCMASTER_MSGOPTION_AF_ZOOM_IN:
		ptMasterInfo->tMsgOptRec.tAFZoom.bModified = TRUE;
            ptMasterInfo->tMsgOptRec.tAFZoom.eZoomVal=  AFZoomIn;
	    	break;
		case VENCMASTER_MSGOPTION_AF_ZOOM_OUT:
	    	ptMasterInfo->tMsgOptRec.tAFZoom.bModified = TRUE;
            ptMasterInfo->tMsgOptRec.tAFZoom.eZoomVal = AFZoomOut;
	    	break;
		case VENCMASTER_MSGOPTION_AF_FOCUS_NEAR:
			ptMasterInfo->tMsgOptRec.tAFFocus.bModified = TRUE;
            ptMasterInfo->tMsgOptRec.tAFFocus.eFocusVal = AFFocusNear;
			break;
		case VENCMASTER_MSGOPTION_AF_FOCUS_FAR:
	    	ptMasterInfo->tMsgOptRec.tAFFocus.bModified = TRUE;
            ptMasterInfo->tMsgOptRec.tAFFocus.eFocusVal = AFFocusFar;
	    	break;   
		case VENCMASTER_MSGOPTION_AUTOSCENE_IS_RUN:
			ptMasterInfo->tMsgOptRec.tAutoScene.bModified = TRUE;
	      	ptMasterInfo->tMsgOptRec.tAutoScene.bISRun = TRUE;
			break;
		case VENCMASTER_MSGOPTION_AUTOSCENE_IS_NOT_RUN:
			ptMasterInfo->tMsgOptRec.tAutoScene.bModified = TRUE;
			ptMasterInfo->tMsgOptRec.tAutoScene.bISRun = FALSE;
			break;
        default:
            DBPRINT0("[VENC_MASTER] Unknown message option\n");
            break;
    }  

	sem_wait(&(ptMasterInfo->smMsgOptions));
	ptMasterInfo->bReMsg_flag = TRUE;
	sem_post(&(ptMasterInfo->smMsgOptions));
    sem_post(&(ptMasterInfo->smWakeup));
}

/* ============================================================================================= */
SCODE VencMaster_GetVideoCapHandle(HANDLE hMasterProcObj, HANDLE *hObj)
{
    TMasterInfo *ptMasterInfo = (TMasterInfo *)hMasterProcObj;
    *hObj = ptMasterInfo->hVideoCapObject;
	return S_OK;
}
/* ============================================================================================= */
SCODE VencMaster_GetUARTIRCutFD(HANDLE hMasterProcObj, int *pFDIRCutUART)
{
     *pFDIRCutUART = -1;
     return S_OK;
}

SCODE VencMaster_GetIRCutFD(HANDLE hMasterProcObj, int *pFDIRCut)
{
     TMasterInfo *ptMasterInfo = (TMasterInfo *)hMasterProcObj;
     *pFDIRCut = ptMasterInfo->fdIRCut;
     return S_OK;
}
