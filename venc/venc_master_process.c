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
 * venc_master_process.c
 *
 * \brief
 * Video Master Core Processing function.
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
#include <syslog.h>

/* ========================================================================== */
void VencMaster_MsgApplyOpt(TMasterInfo *ptMasterInfo)
{
	TMasterMsgOptRec *ptMsgOptRec = &(ptMasterInfo->tMsgOptRec);

	if (ptMsgOptRec->tWDRStatus.bModified == TRUE) 
	{
		ptMasterInfo->tWDRStatus.bModified = TRUE;
		ptMasterInfo->tWDRStatus.bEnable = ptMsgOptRec->tWDRStatus.bEnable;
	}

	if (ptMsgOptRec->tAFStatus.bModified == TRUE)
	{
		ptMasterInfo->tAFStatus.bModified = TRUE;
		ptMasterInfo->tAFStatus.bEnable = ptMsgOptRec->tAFStatus.bEnable;
	}

	if (ptMsgOptRec->tAFZoom.bModified == TRUE)
	{
		ptMasterInfo->tAFZoom.bModified = TRUE;
		ptMasterInfo->tAFZoom.eZoomVal = ptMsgOptRec->tAFZoom.eZoomVal;
	}

	if (ptMsgOptRec->tAFFocus.bModified == TRUE)
	{
		ptMasterInfo->tAFFocus.bModified = TRUE;
		ptMasterInfo->tAFFocus.eFocusVal = ptMsgOptRec->tAFFocus.eFocusVal;
	}	

	if (ptMsgOptRec->tAutoScene.bModified==TRUE)
	{	
		ptMasterInfo->bAutoSceneIsRun = ptMsgOptRec->tAutoScene.bISRun;
	}  
	memset(ptMsgOptRec, 0, sizeof(TMasterMsgOptRec));
}

/* ============================================================================================= */
void* VencMaster_Loop(HANDLE hInstance)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *)hInstance;
#if defined(_AUTOSCENE_)
	static BOOL bPreAutoSceneIsRun = (BOOL)(-1);
#endif
	syslog(LOG_INFO, "[VENC_MASTER] venc_master thread pid: %d\n", getpid());

	// or using default (join)? think about it
	pthread_detach(pthread_self());

	while(1) {
		sem_wait(&(ptMasterInfo->smStart));
		if (ptMasterInfo->bTerminateThread == TRUE)     break;

		DBPRINT0("[VENC_MASTER] VideoMaster Loop Start\n");

		while (ptMasterInfo->bRunning == TRUE) {
			// think about it
			//bReConf_flag should set to true when first running

			if (ptMasterInfo->bReMsg_flag == TRUE) {
				sem_wait(&(ptMasterInfo->smMsgOptions));
				VencMaster_MsgApplyOpt(ptMasterInfo);
				ptMasterInfo->bReMsg_flag = FALSE;
				sem_post(&(ptMasterInfo->smMsgOptions));
			}
#if defined(_AUTOSCENE_)
			if (bPreAutoSceneIsRun != ptMasterInfo->bAutoSceneIsRun)
			{
					if (ptMasterInfo->bAutoSceneIsRun == FALSE)
					{
						VencMaster_InitConfigParam(ptMasterInfo);
					}
					
					bPreAutoSceneIsRun = ptMasterInfo->bAutoSceneIsRun;
					
					printf("[venc_master_process](%d)AutoScene is run? %s\n",__LINE__,ptMasterInfo->bAutoSceneIsRun?"TRUE":"FALSE");
			}	
#endif
			
			if (ptMasterInfo->bReConf_flag == TRUE) {
				sem_wait(&(ptMasterInfo->smOptions));
				VencMaster_LoadConfig(hInstance);
				ptMasterInfo->bReConf_flag = FALSE;
				sem_post(&(ptMasterInfo->smOptions));
			}

			if (ptMasterInfo->bResPLReconfig == TRUE) {
				VencMaster_ResPLSetoptions(ptMasterInfo);
				ptMasterInfo->bResPLReconfig = FALSE;
				//printf("[VENC_MASTER] Resolution and Power Line reset\n");
			}
#if 1
			if (ptMasterInfo->bWBReconfig == TRUE) {
				VencMaster_WBSetoptions(ptMasterInfo);
				ptMasterInfo->bWBReconfig = FALSE;
				//printf("[VENC_MASTER] White Balance reset\n");
			}
#endif

			if (ptMasterInfo->bCCReconfig == TRUE) {
				VencMaster_CCSetoptions(ptMasterInfo);
				ptMasterInfo->bCCReconfig = FALSE;
				//printf("[VENC_MASTER] Color correction reset\n");
			}

			if (ptMasterInfo->bTMReconfig == TRUE) {
				VencMaster_TMSetoptions(ptMasterInfo);
				ptMasterInfo->bTMReconfig = FALSE;
				//printf("[VENC_MASTER] Tone Mapping reset\n");
			}

			if (ptMasterInfo->bBlackClampReconfig == TRUE){

				VencMaster_BlackCalmpSetoptions(ptMasterInfo);
				ptMasterInfo->bBlackClampReconfig = FALSE;
				//printf("[VENC_MASTER] Black Clamp reset!!\n");
			}

			if (ptMasterInfo->bDeImpulseReconfig == TRUE){

				VencMaster_DeImpulseSetoptions(ptMasterInfo);
				ptMasterInfo->bDeImpulseReconfig = FALSE;
				//printf("[VENC_MASTER] De-Impulse reset!!\n");
			}
#if defined(_AUTOSCENE_)            
			if (ptMasterInfo->bAutoSceneIsRun == FALSE)
			{
#endif
				if (ptMasterInfo->bCEReconfig == TRUE) {
					VencMaster_CESetoptions(ptMasterInfo);
					ptMasterInfo->bCEReconfig = FALSE;
					//printf("[VENC_MASTER] Contrast Enhancement reset\n");
				}

#if defined(_AUTOSCENE_)  
			}
#endif 
			if (ptMasterInfo->bImgAdjReconfig == TRUE) {
				VencMaster_ImgAdjSetoptions(ptMasterInfo);
				ptMasterInfo->bImgAdjReconfig = FALSE;
				//printf("[VENC_MASTER] Image Adjustment reset\n");
			}
#if 1             
#if defined(_AUTOSCENE_)            
			if (ptMasterInfo->bAutoSceneIsRun == FALSE)
			{
#endif
				if (ptMasterInfo->bGammaTableReconfig == TRUE) {
					VencMaster_GammaTableSetoptions(ptMasterInfo);
					ptMasterInfo->bGammaTableReconfig = FALSE;
					//printf("[VENC_MASTER] Gamma Table reset\n");
				}
#if defined(_AUTOSCENE_)  
			}
#endif
			if (ptMasterInfo->bAEReconfig == TRUE) {
				VencMaster_AESetoptions(ptMasterInfo);
				ptMasterInfo->bAEReconfig = FALSE;
				//printf("[VENC_MASTER] AE reconfig\n");
			}
			
			if (ptMasterInfo->bAFReconfig == TRUE) {
				VencMaster_AFSetoptions(ptMasterInfo);
				ptMasterInfo->bAFReconfig = FALSE;
				//printf("[VENC_MASTER] AF reconfig\n");
			}
			
			if ((ptMasterInfo->tWDRStatus.bModified == TRUE) || (ptMasterInfo->bWDRReconfig == TRUE)) {
				VencMaster_WDRSetoptions(ptMasterInfo);
				if (ptMasterInfo->tWDRStatus.bModified == TRUE) {
					ptMasterInfo->tWDRStatus.bModified = FALSE;
				}
				if (ptMasterInfo->bWDRReconfig == TRUE) {
					ptMasterInfo->bWDRReconfig = FALSE;
				}
				//printf("[VENC_MASTER] WDR reconfig\n");
			}

			if (ptMasterInfo->tAFStatus.bModified == TRUE) {
				//printf("[VENC_MASTER]AF control now is %s .\n",ptMasterInfo->tAFStatus.bEnable ? "enabled" : "diabled");
				ptMasterInfo->tAFStatus.bModified = FALSE;
			}

			if (ptMasterInfo->tAFZoom.bModified || ptMasterInfo->tAFFocus.bModified) {
				//printf("[VENC_MASTER]AF control now from msgctrl.\n");
				VencMaster_AFSetoptionsFromMsgCtrl(ptMasterInfo);
				if (ptMasterInfo->tAFZoom.bModified == TRUE) {
					ptMasterInfo->tAFZoom.bModified = FALSE;
				}

				if (ptMasterInfo->tAFFocus.bModified == TRUE) {
					ptMasterInfo->tAFFocus.bModified = FALSE;
				}
			}
#endif			
			sem_wait(&(ptMasterInfo->smWakeup));
		}

		ptMasterInfo->bExitInnerLoop = TRUE;

		DBPRINT0("[VENC_MASTER] Video Master Exit inner loop\n");
	}

	sem_post(&(ptMasterInfo->smEnd));
	DBPRINT0("[VENC_MASTER] Master Exit thread\n");

	return 0;
}

/* ============================================================================================= */
SCODE VencMaster_LoadConfig(HANDLE hVencMObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *)(hVencMObject);

	if (ptMasterInfo == NULL)
		return S_FAIL;

	DBPRINT1("[VENC_MASTER] XmlWrapper read config %s \n", ptMasterInfo->szConfigFile);
	if (XmlWrapper_ReadFile_UsrDefFunc(ptMasterInfo->szConfigFile, ptMasterInfo->hXMLWrapperObj, ptMasterInfo) != S_OK) {
		fprintf(stderr, "[VENC_MASTER] XmlWrapper reload config fail! \n");
		return S_FAIL;
	}

	if (XmlWrapper_Reset(ptMasterInfo->hXMLWrapperObj) != S_OK)  {
		DBPRINT0("[VENC_MASTER] XmlWrapper read config fail! \n");
		return S_FAIL;
	}

	VencMaster_ResPLReConfig(ptMasterInfo);
	VencMaster_WBReConfig(ptMasterInfo);
	VencMaster_CCReConfig(ptMasterInfo);
	VencMaster_TMReConfig(ptMasterInfo);
	VencMaster_BlackClampReConfig(ptMasterInfo);
	VencMaster_DeImpulseReConfig(ptMasterInfo);
	VencMaster_CEReConfig(ptMasterInfo);
	VencMaster_GammaTableReConfig(ptMasterInfo);
	VencMaster_AEReConfig(ptMasterInfo);
	VencMaster_AFReConfig(ptMasterInfo);
	VencMaster_ImgAdjReConfig(ptMasterInfo);
	VencMaster_WDRReConfig(ptMasterInfo);
	return S_OK;
}

/* =========================================================================================== */
SCODE VencMaster_ResPLReConfig(HANDLE hVencMObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *)(hVencMObject);

	if (g_tResPLCfg.bModified == TRUE) {
		ptMasterInfo->bResPLReconfig = TRUE;
		g_tResPLCfg.bModified = FALSE;
	}	
	return S_OK;
}

/* =========================================================================================== */
SCODE VencMaster_WBReConfig(HANDLE hVencMObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *)(hVencMObject);

	if (g_tWBCfg.bModified == TRUE) {
		ptMasterInfo->bWBReconfig = TRUE;
		g_tWBCfg.bModified = FALSE;
	}
	return S_OK;
}

/* =========================================================================================== */
SCODE VencMaster_CCReConfig(HANDLE hVencMObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *)(hVencMObject);

	if (g_tColorCorrctionCfg.bModified == TRUE) {
		ptMasterInfo->bCCReconfig = TRUE;
		g_tColorCorrctionCfg.bModified = FALSE;
	}

	return S_OK;
}

/* =========================================================================================== */
SCODE VencMaster_TMReConfig(HANDLE hVencMObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *)(hVencMObject);

	if (g_tTMCfg.bModified == TRUE) {
		ptMasterInfo->bTMReconfig = TRUE;
		g_tTMCfg.bModified = FALSE;
	}

	return S_OK;
}

/* =========================================================================================== */
SCODE VencMaster_BlackClampReConfig(HANDLE hVencMObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *)(hVencMObject);

	if (g_tBlackClampCfg.bModified == TRUE) {
		ptMasterInfo->bBlackClampReconfig = TRUE;
		g_tBlackClampCfg.bModified = FALSE;
	}

	return S_OK;
}

/* =========================================================================================== */
SCODE VencMaster_DeImpulseReConfig(HANDLE hVencMObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *)(hVencMObject);

	if (g_tDeImpulseCfg.bModified == TRUE) {
		ptMasterInfo->bDeImpulseReconfig = TRUE;
		g_tDeImpulseCfg.bModified = FALSE;
	}

	return S_OK;

}

/* =========================================================================================== */
SCODE VencMaster_CEReConfig(HANDLE hVencMObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *)(hVencMObject);

	if (g_tCECfg.bModified == TRUE) {
		ptMasterInfo->bCEReconfig = TRUE;
		g_tCECfg.bModified = FALSE;
	}

	return S_OK;
}

/* =========================================================================================== */
SCODE VencMaster_GammaTableReConfig(HANDLE hVencMObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *)(hVencMObject);

	ptMasterInfo->bGammaTableReconfig = TRUE;

	return S_OK;
}

/* =========================================================================================== */
SCODE VencMaster_AEReConfig(HANDLE hVencMObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *)(hVencMObject);

	if (g_tAECfg.bModified == TRUE) {
		ptMasterInfo->bAEReconfig = TRUE;
		g_tAECfg.bModified = FALSE;
	}

	return S_OK;
}

/*===============================================================================================*/
SCODE VencMaster_WDRReConfig(HANDLE hVencMObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *)(hVencMObject);

	if (g_tWDRCfg.bModified == TRUE) {
		ptMasterInfo->bWDRReconfig = TRUE;
		g_tWDRCfg.bModified = FALSE;
	}

	return S_OK;
}

/* =========================================================================================== */
SCODE VencMaster_AFReConfig(HANDLE hVencMObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *)(hVencMObject);

	if (g_tAFCfg.bModified == TRUE) {
		ptMasterInfo->bAFReconfig = TRUE;
		//g_tAFCfg.bModified = FALSE;
	}

	return S_OK;
}

/* =========================================================================================== */
SCODE VencMaster_ImgAdjReConfig(HANDLE hVencMObject)
{
	TMasterInfo *ptMasterInfo = (TMasterInfo *)(hVencMObject);

	if (g_tImgAdjCfg.bModified == TRUE) {
		ptMasterInfo->bImgAdjReconfig = TRUE;
		g_tImgAdjCfg.bModified = FALSE;
	}

	return S_OK;
}
