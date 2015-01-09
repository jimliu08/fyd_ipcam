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
 * venc_message.c
 *
 * \brief
 * venc message initial, receive and handler.
 * 
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#include "venc_app_local.h"
#include "venc_message.h"


/* it is a mimic of strtof since we don't have strtof on uClibc today. */
#if 1
static FLOAT mystrtof(const CHAR *szTmp, CHAR **endptr) 
{
	SDWORD	sdwTmp;
	SDWORD	sdwInt;
	BOOL	bIsFrac = FALSE;
	FLOAT	fAtom = 0.1;
	FLOAT	fOut = 0.0;
	BOOL	bIsMinus = FALSE;

	if (*szTmp == '-') {
		bIsMinus = TRUE; 
		szTmp ++;
	}

	sdwInt = 0;
	while (1) {
		sdwTmp = (SDWORD) (*szTmp - '0');

		if (*szTmp == '.') {
			bIsFrac = TRUE;
			fOut = (FLOAT)sdwInt;
		} else if ((sdwTmp >= 0) && (sdwTmp < 10)) {
			if (bIsFrac) {
				fOut += fAtom * (FLOAT)sdwTmp;
				fAtom *= 0.1;
			} else {
				sdwInt = sdwInt * 10 + sdwTmp;
			}
		} else {
			if (!bIsFrac) {
				fOut = (FLOAT)sdwInt;
			}
			break;
		}
		szTmp ++;
	}

	if (bIsMinus) fOut = 0.0 - fOut; 

	return fOut;
}
#else
#define mystrtof	strtof
#endif

TMessageMember g_tMsgMember;

/* ========================================================================== */
void VencMessage_VerifyMasterMsg(HANDLE hMasterProcObj, EContentType eContent)
{
	TMasterMsgOptions tMasterMsgOpt;
	memset(&(tMasterMsgOpt), 0, sizeof(TMasterMsgOptions)); 

	switch(eContent) {
		case msgCWideDynamicRangeOn:
			tMasterMsgOpt.emasMsgOptType = VENCMASTER_MSGOPTION_WDR_ON;
			break;
		case msgCWideDynamicRangeOff:
			tMasterMsgOpt.emasMsgOptType = VENCMASTER_MSGOPTION_WDR_OFF;
			break;
		case msgCEnableAF:
			tMasterMsgOpt.emasMsgOptType = VENCMASTER_MSGOPTION_ENABLE_AF;
			break;
		case msgCDisableAF:
			tMasterMsgOpt.emasMsgOptType =  VENCMASTER_MSGOPTION_DISABLE_AF;
			break;
		case msgCAFZoomIn:
			tMasterMsgOpt.emasMsgOptType =  VENCMASTER_MSGOPTION_AF_ZOOM_IN;
			break;
		case msgCAFZoomOut:
			tMasterMsgOpt.emasMsgOptType =  VENCMASTER_MSGOPTION_AF_ZOOM_OUT;
			break;
		case msgCAFFocusFar:
			tMasterMsgOpt.emasMsgOptType =  VENCMASTER_MSGOPTION_AF_FOCUS_FAR;
			break;
		case msgCAFFocusNear:		
			tMasterMsgOpt.emasMsgOptType =  VENCMASTER_MSGOPTION_AF_FOCUS_NEAR;
			break;
		case msgAutoSceneIsRun:
			tMasterMsgOpt.emasMsgOptType =  VENCMASTER_MSGOPTION_AUTOSCENE_IS_RUN;
			break;
		case msgAutoSceneIsNotRun:
			tMasterMsgOpt.emasMsgOptType =   VENCMASTER_MSGOPTION_AUTOSCENE_IS_NOT_RUN;
			break;
		default:
			DBPRINT0("[VENC_MESSAGE] unsupport content type\n");
			break;
	}
	VencMaster_MsgReceive(hMasterProcObj, tMasterMsgOpt);
}

/* ========================================================================== */
void VencMessage_VerifyVideoInMsg(HANDLE hVideoInProcObj,EContentType eContent)
{
	TVideoinMsgOptions tVideoinMsgOpt;
	memset(&(tVideoinMsgOpt), 0, sizeof(TVideoinMsgOptions)); 

	switch(eContent) {
		case msgCDumpVideoCapBuf:
			//printf("[venc_message](%d)\n",__LINE__);
			tVideoinMsgOpt.emasMsgOptType = VENCVIDEOIN_MSGOPTION_DUMP_VIDEOCAPBUF;
			break;
		case msgCDumpSharedBuf:
			//printf("[venc_message](%d)\n",__LINE__);
			tVideoinMsgOpt.emasMsgOptType = VENCVIDEOIN_MSGOPTION_DUMP_SHAREDBUF;
			break;
		case msgCRetVideoCapStatus:
			//printf("[venc_message](%d)\n",__LINE__);
			tVideoinMsgOpt.emasMsgOptType = VENCVIDEOIN_MSGOPTION_RETRIEVE_VIDEOCAPSTATUS;
			break;      
		case msgCPhotoMtcDefTbl1:
			//printf("[venc_message](%d)\n",__LINE__);
			tVideoinMsgOpt.emasMsgOptType = VENCVIDEOIN_MSGOPTION_PHOTOMTC_DEFINETBL1;
			break;
		case msgCPhotoMtcDefTbl2:
			//printf("[venc_message](%d)\n",__LINE__);
			tVideoinMsgOpt.emasMsgOptType = VENCVIDEOIN_MSGOPTION_PHOTOMTC_DEFINETBL2;
			break;
		case msgCKeepCurrentWB:
			//printf("[venc_message](%d)\n",__LINE__);
			tVideoinMsgOpt.emasMsgOptType = VENCVIDEOIN_MSGOPTION_KEEP_CURRENT_WB;
			break;
		case msgCKeepCurrentWBFromFile:
			//printf("[venc_message](%d)\n",__LINE__);
			tVideoinMsgOpt.emasMsgOptType = VENCVIDEOIN_MSGOPTION_KEEP_CURRENT_WB_FROM_FILE;
			break;
		case msgCEnableFrameRateCal:
			//printf("[venc_message](%d)\n",__LINE__);
			tVideoinMsgOpt.emasMsgOptType = VENCVIDEOIN_MSGOPTION_ENABLE_FRAMERATE_CAL;
			break;
		case msgCDisableFrameRateCal:
			//printf("[venc_message](%d)\n",__LINE__);
			tVideoinMsgOpt.emasMsgOptType = VENCVIDEOIN_MSGOPTION_DISABLE_FRAMERATE_CAL;
			break;
		case msgCDumpAFStatus:
			//printf("[venc_message](%d)\n",__LINE__);
			tVideoinMsgOpt.emasMsgOptType = VENCVIDEOIN_MSGOPTION_DUMP_AF_STATUS;
			break;
		case msgCISPEnable:
			//printf("[venc_message](%d)\n",__LINE__);
			tVideoinMsgOpt.emasMsgOptType = VENCVIDEOIN_MSGOPTION_ISP_ENABLE;
			break;	
		case msgCISPDisable:
			//printf("[venc_message](%d)\n",__LINE__);
			tVideoinMsgOpt.emasMsgOptType = VENCVIDEOIN_MSGOPTION_ISP_DISABLE;
			break;
		case msgCISPUpdateState:
		  //printf("[venc_message](%d)\n",__LINE__);
			tVideoinMsgOpt.emasMsgOptType = VENCVIDEOIN_MSGOPTION_ISP_UPDATE_STATE;
			break;
		case msgCClearInternalBuffer:
			tVideoinMsgOpt.emasMsgOptType = VENCVIDEOIN_MSGOPTION_CLEAR_IBPE_INTERNAL_BUFFER;
			break;
		default:
			DBPRINT0("[VENC_MESSAGE] unsupport content type\n");
			break;
	}
	VencVideoin_MsgReceive(hVideoInProcObj, tVideoinMsgOpt);

}

/* ========================================================================== */
void VencMessage_VerifyEncoderMsg(HANDLE hEncoderProcObj, EContentType eContent)
{
	TEncoderMsgOptions tEncoderMsgOpt;
	memset(&(tEncoderMsgOpt), 0, sizeof(TEncoderMsgOptions)); 

	switch(eContent) {
		case msgCStart:
			tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_SET_REG_OUTPUT;
			break;
		case msgCStop:
			tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_SET_UNREG_OUTPUT;
			break;
		case msgCForceCI:
			tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_SET_FORCECI;
			break;
		case msgCForceIntra:
			tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_SET_FORCEINTRAL;
			break;
		case msgCEnableFrameRateCal:
			tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_ENABLE_FRAMERATE_CAL;
			break;
		case msgCDisableFrameRateCal:
			tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_DISABLE_FRAMERATE_CAL;
			break;
		case msgGetQualityParameter:
			tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_GET_QUALITY_PARAMETER;
			break;
		case msgSetROI:
			tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_SET_ROI;
			break;
#ifdef _JPEG_SNAPSHOT_	
		case msgCGetSnapshot:
			tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_GET_SNAPSHOT;
			break;
#endif
		default:
			DBPRINT0("[VENC_MESSAGE] unsupport content type\n");
			break;
	}
	VencEncoder_MsgReceive(hEncoderProcObj, tEncoderMsgOpt);    
}
#ifdef _VPL_VOC
/* ========================================================================== */
void VencMessage_VerifyDisplayMsg(HANDLE hDisplayProcObj, EContentType eContent)
{
	TDisplayMsgOptions tDisplayMsgOpt;
	memset(&(tDisplayMsgOpt), 0, sizeof(TDisplayMsgOptions));

	switch(eContent) {
		case msgCDumpVideoCapBuf:
			tDisplayMsgOpt.edispMsgOptType = VENCDISPLAY_MSGOPTION_DUMP_VIDEOCAPBUF;
			break;
		case msgCDumpVideoDispBuf:
			tDisplayMsgOpt.edispMsgOptType = VENCDISPLAY_MSGOPTION_DUMP_VIDEODISPBUF;
			break;
		case msgCEnableDisp:
			tDisplayMsgOpt.edispMsgOptType = VENCDISPLAY_MSGOPTION_START_DISPLAY;
			break;
		case msgCDisableDisp:
			tDisplayMsgOpt.edispMsgOptType = VENCDISPLAY_MSGOPTION_STOP_DISPLAY;
			break;
		default:
			DBPRINT0("[VENC_MESSAGE] unsupport content type\n");
			break;
	}
	VencDisplay_MsgReceive(hDisplayProcObj, tDisplayMsgOpt);
}
#endif
/* ========================================================================== */
void VencMessge_VerifyMsg(TVencInfo *ptVencInfo)
{
	if (strncmp(g_tMsgMember.szHostName, "encoder", strlen("encoder")) == 0) 
	{
        DWORD dwIndex = 0;
        dwIndex = strtol(strtok(g_tMsgMember.szHostName, "encoder"), NULL, 10);
        if (dwIndex >= ptVencInfo->tVencCfgPath.dwEncoderTotalNum) {
            printf("[VENC_MESSAGE] unsupport encoder%d, Total encoder num is %d\n", dwIndex, ptVencInfo->tVencCfgPath.dwEncoderTotalNum);
        } else {
            VencMessage_VerifyEncoderMsg(ptVencInfo->phEncoderProcObj[dwIndex], g_tMsgMember.eContent);	
        }
    }
    else
	{
		switch(g_tMsgMember.eHost) {
			case msgHMaster:
				//printf("[venc_message]%d\n",__LINE__);
				VencMessage_VerifyMasterMsg(ptVencInfo->hMasterProcObj, g_tMsgMember.eContent);
				break;
			case msgHVideoIn:
				//printf("[venc_message]%d\n",__LINE__);
				VencMessage_VerifyVideoInMsg(ptVencInfo->hVideoInProcObj,g_tMsgMember.eContent);
				break;
				/*			
							case msgHEncoder0:
							VencMessage_VerifyEncoderMsg(ptVencInfo->hEncoderProcObj[0], g_tMsgMember.eContent);
							break;
							case msgHEncoder1:
							VencMessage_VerifyEncoderMsg(ptVencInfo->hEncoderProcObj[1], g_tMsgMember.eContent);
							break;
							case msgHEncoder2:
							VencMessage_VerifyEncoderMsg(ptVencInfo->hEncoderProcObj[2], g_tMsgMember.eContent);
							break;
				*/
	#ifdef _VPL_VOC
			case msgHDisplay:
			//printf("[venc_message]%d\n",__LINE__);
				VencMessage_VerifyDisplayMsg(ptVencInfo->hDisplayProcObj, g_tMsgMember.eContent);
				break;
	#endif      
			default:
			//printf("[venc_message]%d\n",__LINE__);
				// DBPRINT0("[VENC_MESSAGE] unsupport host type\n");
				break;
		}
	}


}

/* ========================================================================== */
void VencMessage_GetHost(HANDLE hObject, const CHAR *szVisitor, SDWORD len)
{
	CHAR szTemp[len+1];
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szVisitor);
	memset(g_tMsgMember.szHostName,0x0,sizeof(g_tMsgMember.szHostName));
	
	if (strcasecmp(szTemp, "master") == 0) {
		printf("[VENC_MESSAGE] host - master !!\n");
		g_tMsgMember.eHost = msgHMaster;
	} else if (strcasecmp(szTemp, "videoin") == 0) {
#ifdef _AUTOSCENE__DEBUG_
		printf("[VENC_MESSAGE] host - videoin !!\n");
#endif
		g_tMsgMember.eHost = msgHVideoIn;
		/*    
		      } else if (strcasecmp(szTemp, "encoder0") == 0) {
		      printf("[VENC_MESSAGE] host - encoder0 !!\n");
		      g_tMsgMember.eHost = msgHEncoder0;
		      } else if (strcasecmp(szTemp, "encoder1") == 0) {
		      printf("[VENC_MESSAGE] host - encoder1 !!\n");
		      g_tMsgMember.eHost = msgHEncoder1;
		      } else if (strcasecmp(szTemp, "encoder2") == 0) {
		      printf("[VENC_MESSAGE] host - encoder2 !!\n");
		      g_tMsgMember.eHost = msgHEncoder2;
		 */		
#ifdef _VPL_VOC
	} else if (strcasecmp(szTemp, "display") == 0) {
		printf("[VENC_MESSAGE] host - display !!\n");
		g_tMsgMember.eHost = msgHDisplay;
#endif
	} else if (strncmp(szTemp,"encoder",strlen("encoder"))==0){
		printf("[VENC_MESSAGE] host - encoder !!\n");
		strncpy(g_tMsgMember.szHostName,szTemp,len+1);	
	} else {
		printf("[VENC_MESSAGE] unsupport host !!\n");
		g_tMsgMember.eHost = msgHUnKnown;
	}
}

/* ========================================================================== */
void VencMessage_GetContent(HANDLE hObject, const CHAR *szVisitor, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *)(hObject);
	CHAR szTemp[len+1];
	memset(szTemp,0x0,sizeof(szTemp));
	snprintf(szTemp, len+1, "%s", szVisitor);

	if (strcasecmp(szTemp, "start") == 0) {
		printf("[VENC_MESSAGE] content - start !!\n");
		g_tMsgMember.eContent = msgCStart;
	} else if (strcasecmp(szTemp, "stop") == 0) {
		printf("[VENC_MESSAGE] content - stop !!\n");
		g_tMsgMember.eContent = msgCStop;
	} else if (strcasecmp(szTemp, "forceCI") == 0) {
		printf("[VENC_MESSAGE] content - forceCI !!\n");
		g_tMsgMember.eContent = msgCForceCI;
	} else if (strcasecmp(szTemp, "forceIntra") == 0) {
		printf("[VENC_MESSAGE] content - forceIntra !!\n");
		g_tMsgMember.eContent = msgCForceIntra;
	} else if (strcasecmp(szTemp, "WDR_ON") == 0) {
		printf("[VENC_MESSAGE] content - WDR_ON !!\n");
		g_tMsgMember.eContent = msgCWideDynamicRangeOn;
	} else if (strcasecmp(szTemp, "WDR_OFF") == 0) {
		printf("[VENC_MESSAGE] content - WDR_OFF !!\n");
		g_tMsgMember.eContent = msgCWideDynamicRangeOff;
	} else if (strcasecmp(szTemp, "dumpVideoCapBuf") == 0) {
		printf("[VENC_MESSAGE] content - dumpVideoCapBuf !!\n");
		g_tMsgMember.eContent = msgCDumpVideoCapBuf;
	} else if (strcasecmp(szTemp, "dumpSharedBuf") == 0) {
		printf("[VENC_MESSAGE] content - dumpSharedBuf !!\n");
		g_tMsgMember.eContent = msgCDumpSharedBuf;
	} else if (strcasecmp(szTemp, "dumpVideoDispBuf") == 0) {
		printf("[VENC_MESSAGE] content - dumpVideoDispBuf !!\n");
		g_tMsgMember.eContent = msgCDumpVideoDispBuf;
	} else if (strcasecmp(szTemp, "RetVideoCapStatus") == 0) {
		printf("[VENC_MESSAGE] content - RetVideoCapStatus !!\n");
		g_tMsgMember.eContent = msgCRetVideoCapStatus;
	} else if (strcasecmp(szTemp, "photomtcdeftbl1") == 0) {
		printf("[VENC_MESSAGE] content - photomtcdeftbl1 !!\n");
		g_tMsgMember.eContent = msgCPhotoMtcDefTbl1;
	}else if (strcasecmp(szTemp, "photomtcdeftbl2") == 0) {
		printf("[VENC_MESSAGE] content - photomtcdeftbl2 !!\n");
		g_tMsgMember.eContent = msgCPhotoMtcDefTbl2;		
#ifdef _VPL_VOC
	} else if (strcasecmp(szTemp, "EnableDisplay") == 0) {
		printf("[VENC_MESSAGE] content - EnableDisplay !!\n");
		g_tMsgMember.eContent = msgCEnableDisp;
	} else if (strcasecmp(szTemp, "DisableDisplay") == 0) {
		printf("[VENC_MESSAGE] content - DisableDisplay !!\n");
		g_tMsgMember.eContent = msgCDisableDisp;
#endif
	}else if (strcasecmp(szTemp, "keepCurrentWB") == 0){
		printf("[VENC_MESSAGE] content - keepCurrentWB !!\n");
		g_tMsgMember.eContent = msgCKeepCurrentWB;
	}else if (strcasecmp(szTemp, "keepCurrentWBFromFile") == 0){
		printf("[VENC_MESSAGE] content - keepCurrentWBFromFile !!\n");
		g_tMsgMember.eContent = msgCKeepCurrentWBFromFile;
	}else if (strcasecmp(szTemp, "dumpAFStatus") == 0){
		printf("[VENC_MESSAGE] content - dumpAFStatus  !!\n");
		g_tMsgMember.eContent = msgCDumpAFStatus;
	}else if (strcasecmp(szTemp, "AF_ON") == 0){
		printf("[VENC_MESSAGE] content - Enable AF  !!\n");
		g_tMsgMember.eContent = msgCEnableAF;
	}else if (strcasecmp(szTemp, "AF_OFF") == 0){
		printf("[VENC_MESSAGE] content - Disable AF  !!\n");
		g_tMsgMember.eContent = msgCDisableAF;   
	}else if (strcasecmp(szTemp, "AF_ZoomIn") == 0){
		printf("[VENC_MESSAGE] content - AF Zoom In !!\n");
		g_tMsgMember.eContent = msgCAFZoomIn;  
	}else if (strcasecmp(szTemp, "AF_ZoomOut") == 0){
		printf("[VENC_MESSAGE] content - AF Zoom Out !!\n");
		g_tMsgMember.eContent = msgCAFZoomOut; 
	}else if (strcasecmp(szTemp, "AF_FocusFar") == 0){
		printf("[VENC_MESSAGE] content - AF Focus Far!!\n");
		g_tMsgMember.eContent = msgCAFFocusFar; 
	}else if (strcasecmp(szTemp, "AF_FocusNear") == 0){
		printf("[VENC_MESSAGE] content - AF Focus Near!!\n");
		g_tMsgMember.eContent = msgCAFFocusNear;
	}else if (strcasecmp(szTemp, "FRCalculate_ON") == 0){
		printf("[VENC_MESSAGE] content - turn on calculate frame rate !!\n");
		g_tMsgMember.eContent = msgCEnableFrameRateCal;
	}else if (strcasecmp(szTemp, "FRCalculate_OFF") == 0){
		printf("[VENC_MESSAGE] content - turn off calculate frame rate !!\n");
		g_tMsgMember.eContent = msgCDisableFrameRateCal;
	}else if (strcasecmp(szTemp, "ISPEnable") == 0){
#ifdef _AUTOSCENE__DEBUG_		
		printf("[VENC_MESSAGE] content - ISPEnable  !!\n");
#endif
		g_tMsgMember.eContent = msgCISPEnable;
	}else if (strcasecmp(szTemp, "ISPDisable") == 0){
#ifdef _AUTOSCENE__DEBUG_		
		printf("[VENC_MESSAGE] content - ISPDisable  !!\n");
#endif		
		g_tMsgMember.eContent = msgCISPDisable;	  
	}else if (strcasecmp(szTemp, "ISPUpdateState") == 0){
#ifdef _AUTOSCENE__DEBUG_		
		printf("[VENC_MESSAGE] content - ISPUpdateState  !!\n");
#endif		
		g_tMsgMember.eContent = msgCISPUpdateState;	 
	}else if (strcasecmp(szTemp, "AutoSceneIsRun") == 0){
		printf("[VENC_MESSAGE] content - AutoSceneIsRun  !!\n");
		g_tMsgMember.eContent = msgAutoSceneIsRun;
	}else if (strcasecmp(szTemp, "AutoSceneIsNotRun") == 0){
		printf("[VENC_MESSAGE] content - AutoSceneIsNotRun  !!\n");
		g_tMsgMember.eContent = msgAutoSceneIsNotRun;
	}else if (strcasecmp(szTemp, "GetQualityParameter") == 0){
		printf("[VENC_MESSAGE] content - GetQualityParameter  !!\n");
		g_tMsgMember.eContent = msgGetQualityParameter;  
    }else if (strcasecmp(szTemp, "SetROI") == 0){
		printf("[VENC_MESSAGE] content - SetROI  !!\n");
		g_tMsgMember.eContent = msgSetROI; 
	}else if (strcasecmp(szTemp, "GetSnapshot") == 0){
#ifdef _JPEG_SNAPSHOT_			
		printf("[VENC_MESSAGE] content - get one snapshot !!\n");
		g_tMsgMember.eContent = msgCGetSnapshot;
#endif
	}else if (strcasecmp(szTemp, "ClearIBPEInternalBuffer") == 0){ 
		printf("[VENC_MESSAGE] content - clear IBPE internal buffer !!\n");
		g_tMsgMember.eContent = msgCClearInternalBuffer;
    } else {
		printf("[VENC_MESSAGE] unsupport content !!\n");
		g_tMsgMember.eContent = msgCUnKnown;
	}

	VencMessge_VerifyMsg(ptVencInfo);
}

/* ========================================================================== */
#ifdef VIRTUAL_PTZ

DWORD gdwVPTZHostIndex = 0;

static void VencSlave_VPTZEnableMsg(HANDLE hObject, const CHAR *szEnable, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *)(hObject);
	TEncoderMsgOptions tEncoderMsgOpt;
	memset(&(tEncoderMsgOpt), 0, sizeof(TEncoderMsgOptions));

	if ((szEnable[0] == 't') || (szEnable[0] == 'T')) {
		tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_SET_VPTZ_ENABLE;
	} else {
		tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_SET_VPTZ_DISABLE;
	}
	VencEncoder_MsgReceive(ptVencInfo->phEncoderProcObj[gdwVPTZHostIndex], tEncoderMsgOpt);    
}

static void VencSlave_VPTZPanMsg(HANDLE hObject, const CHAR *szTmp, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *)(hObject);
	TEncoderMsgOptions tEncoderMsgOpt;
	memset(&(tEncoderMsgOpt), 0, sizeof(TEncoderMsgOptions));

	tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_SET_VPTZ_PAN;
	tEncoderMsgOpt.aflUserData[0] = mystrtof(szTmp, NULL);
	VencEncoder_MsgReceive(ptVencInfo->phEncoderProcObj[gdwVPTZHostIndex], tEncoderMsgOpt);
}

static void VencSlave_VPTZTiltMsg(HANDLE hObject, const CHAR *szTmp, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *)(hObject);
	TEncoderMsgOptions tEncoderMsgOpt;
	memset(&(tEncoderMsgOpt), 0, sizeof(TEncoderMsgOptions));

	tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_SET_VPTZ_TILT;
	tEncoderMsgOpt.aflUserData[0] = mystrtof(szTmp, NULL);
	VencEncoder_MsgReceive(ptVencInfo->phEncoderProcObj[gdwVPTZHostIndex], tEncoderMsgOpt);
}
static void VencSlave_VPTZZoomMsg(HANDLE hObject, const CHAR *szTmp, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *)(hObject);
	TEncoderMsgOptions tEncoderMsgOpt;
	memset(&(tEncoderMsgOpt), 0, sizeof(TEncoderMsgOptions));

	tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_SET_VPTZ_ZOOM;
	tEncoderMsgOpt.aflUserData[0] = mystrtof(szTmp, NULL);
	VencEncoder_MsgReceive(ptVencInfo->phEncoderProcObj[gdwVPTZHostIndex], tEncoderMsgOpt);
}
static void VencSlave_VPTZSpeedMsg(HANDLE hObject, const CHAR *szTmp, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *)(hObject);
	TEncoderMsgOptions tEncoderMsgOpt;
	memset(&(tEncoderMsgOpt), 0, sizeof(TEncoderMsgOptions));

	tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_SET_VPTZ_SPEED;
	tEncoderMsgOpt.aflUserData[0] = mystrtof(szTmp, NULL);
	VencEncoder_MsgReceive(ptVencInfo->phEncoderProcObj[gdwVPTZHostIndex], tEncoderMsgOpt);
}
static void VencSlave_VPTZHostMsg(HANDLE hObject, const CHAR *szTmp, SDWORD len)
{
	CHAR *pszVPTZHost = NULL;
	//printf("[venc_message](%d)PTZ host=%s    len=%d\n",__LINE__,szTmp,len);
	pszVPTZHost = (CHAR *)malloc(sizeof(CHAR)*(len+1));
	memset( pszVPTZHost,0x0,sizeof(CHAR)*(len+1));
	memcpy(pszVPTZHost,szTmp,len);
	pszVPTZHost[len]='\0';

	if (strncmp(pszVPTZHost,"encoder",strlen("encoder")) == 0)
	{
		gdwVPTZHostIndex = strtol(strtok(pszVPTZHost,"encoder"),NULL,10);
	}
	else
	{
		gdwVPTZHostIndex = 0;
	}
	//printf("[venc_message](%d) pszVPTZHost=%s gdwVPTZHostIndex=%d\n",__LINE__,pszVPTZHost,gdwVPTZHostIndex);

	if (pszVPTZHost!=NULL)
	{
		free(pszVPTZHost);
		pszVPTZHost = NULL;

	}

}

#endif //VIRTUAL_PTZ 
DWORD gdwEncHostIndex = 0;

static void VencSlave_EncHostMsg(HANDLE hObject, const CHAR *szTmp, SDWORD len)
{
    CHAR *pszEncHost = NULL;
    //printf("[venc_message](%d)PTZ host=%s    len=%d\n",__LINE__,szTmp,len);
    pszEncHost = (CHAR *)malloc(sizeof(CHAR)*(len+1));
    memset( pszEncHost,0x0,sizeof(CHAR)*(len+1));
    memcpy(pszEncHost,szTmp,len);
    pszEncHost[len]='\0';
    
    if (strncmp(pszEncHost,"encoder",strlen("encoder")) == 0)
    {
      gdwEncHostIndex = strtol(strtok(pszEncHost,"encoder"),NULL,10);
    }
    else
    {
       gdwEncHostIndex = 0;
    }
  //  printf("[venc_message](%d) pszEncHost=%s gdwEncHostIndex=%d\n",__LINE__,pszEncHost,gdwEncHostIndex);
    
    if (pszEncHost != NULL)
    {
      free(pszEncHost);
      pszEncHost = NULL;
    
    }
    
}
static void VencSlave_EncCodecMsg(HANDLE hObject, const CHAR *szTmp, SDWORD len)
{
     TVencInfo *ptVencInfo = (TVencInfo *)(hObject);
    TEncoderMsgOptions tEncoderMsgOpt;
    memset(&(tEncoderMsgOpt), 0, sizeof(TEncoderMsgOptions));
    /*
      0:H264,1:SVC,2:MPEG4,3:MJPEG
    */
    tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_SET_CODEC_TYPE;
    tEncoderMsgOpt.aflUserData[0] = mystrtof(szTmp, NULL);
    VencEncoder_MsgReceive(ptVencInfo->phEncoderProcObj[gdwEncHostIndex], tEncoderMsgOpt);
 //   printf("[venc_message](%d)gdwEncHostIndex=%d codec=%u\n",__LINE__,gdwEncHostIndex,(DWORD)tEncoderMsgOpt.aflUserData[0]);
  
}
static void VencSlave_EncModeMsg(HANDLE hObject, const CHAR *szTmp, SDWORD len)
{
    TVencInfo *ptVencInfo = (TVencInfo *)(hObject);
    TEncoderMsgOptions tEncoderMsgOpt;
    memset(&(tEncoderMsgOpt), 0, sizeof(TEncoderMsgOptions));
    /*
    RATE_CTRL_NONE = 0,         // constant quality and variable bitrate
    RATE_CTRL_VQCB = 1,         // variable quality and constant bitrate
    RATE_CTRL_CQCB = 2,         // constant quality and constant bitrate
    RATE_CTRL_STRICT_VQCB = 3,  // strict variable quality and constant bitrate, never overflow
    RATE_CTRL_CVBR = 4          // constant quality when bitrate meet the requirement. 
    */
    tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_SET_CONTROL_MODE;
    tEncoderMsgOpt.aflUserData[0] = mystrtof(szTmp, NULL);
    VencEncoder_MsgReceive(ptVencInfo->phEncoderProcObj[gdwEncHostIndex], tEncoderMsgOpt);
//    printf("[venc_message](%d)gdwEncHostIndex=%d mode=%u\n",__LINE__,gdwEncHostIndex,(DWORD)tEncoderMsgOpt.aflUserData[0]);
}

static void VencSlave_EncQuantMsg(HANDLE hObject, const CHAR *szTmp, SDWORD len)
{
    TVencInfo *ptVencInfo = (TVencInfo *)(hObject);
    TEncoderMsgOptions tEncoderMsgOpt;
    memset(&(tEncoderMsgOpt), 0, sizeof(TEncoderMsgOptions));
    
    tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_SET_QUANT;
    tEncoderMsgOpt.aflUserData[0] = mystrtof(szTmp, NULL);
    VencEncoder_MsgReceive(ptVencInfo->phEncoderProcObj[gdwEncHostIndex], tEncoderMsgOpt);
//    printf("[venc_message](%d)gdwEncHostIndex=%d quant=%u\n",__LINE__,gdwEncHostIndex,(DWORD)tEncoderMsgOpt.aflUserData[0]);

}
static void VencSlave_EncBitRateMsg(HANDLE hObject, const CHAR *szTmp, SDWORD len)
{
    TVencInfo *ptVencInfo = (TVencInfo *)(hObject);
    TEncoderMsgOptions tEncoderMsgOpt;
    memset(&(tEncoderMsgOpt), 0, sizeof(TEncoderMsgOptions));
    
    tEncoderMsgOpt.eenMsgOptType = VENCENCODER_MSGOPTION_SET_BITRATE;
    tEncoderMsgOpt.aflUserData[0] = mystrtof(szTmp, NULL);
    VencEncoder_MsgReceive(ptVencInfo->phEncoderProcObj[gdwEncHostIndex], tEncoderMsgOpt);
//    printf("[venc_message](%d)gdwEncHostIndex=%d bitrate=%u\n",__LINE__,gdwEncHostIndex,(DWORD)tEncoderMsgOpt.aflUserData[0]);

}
/* ========================================================================== */
SCODE VencMessage_Initial(TVencInfo *ptVencInfo, CHAR* szMsgFifo)
{
	TMsgReaderInitialOpts tMsgReaderInitOpts;
	TXmlWrapperTreeMap MsgHdlTreeMap[] = 
	{
		{"control/request/host", NULL, &VencMessage_GetHost, NULL},
		{"control/request/content", NULL, &VencMessage_GetContent, NULL},
		{"control/encode/host", NULL, &VencSlave_EncHostMsg, NULL},
		{"control/encode/codec", NULL, &VencSlave_EncCodecMsg, NULL},
		{"control/encode/mode", NULL, &VencSlave_EncModeMsg, NULL},
		{"control/encode/quant", NULL, &VencSlave_EncQuantMsg, NULL},
		{"control/encode/bitrate", NULL, &VencSlave_EncBitRateMsg, NULL},
#ifdef VIRTUAL_PTZ
		{"control/ptz/virtual/host", NULL, &VencSlave_VPTZHostMsg, NULL},
		{"control/ptz/virtual/enable", NULL, &VencSlave_VPTZEnableMsg, NULL},
		{"control/ptz/virtual/pan", NULL, &VencSlave_VPTZPanMsg, NULL},
		{"control/ptz/virtual/tilt", NULL, &VencSlave_VPTZTiltMsg, NULL},
		{"control/ptz/virtual/zoom", NULL, &VencSlave_VPTZZoomMsg, NULL},
		{"control/ptz/virtual/speed", NULL, &VencSlave_VPTZSpeedMsg, NULL},
#endif //VIRTUAL_PTZ

		{NULL, NULL, NULL}
	};

	tMsgReaderInitOpts.dwVersion = MSGREADER_VERSION;
	tMsgReaderInitOpts.pcMsgPath = szMsgFifo;
	tMsgReaderInitOpts.ptCallBackHandler = MsgHdlTreeMap;
	tMsgReaderInitOpts.ptv_time = NULL;
	tMsgReaderInitOpts.hUsrObj = (HANDLE)ptVencInfo;

	if (MsgReader_Initial(&(ptVencInfo->hMsgReaderObj), &tMsgReaderInitOpts) != S_OK)
	{
		fprintf(stderr, "[VENC_MESSAGE] MsgDove_Initial Fail!!\n");
		return -1;
	}
	DBPRINT0("[VENC_MESSAGE] MsgDove_Initial Successfully!!\n");

	return S_OK;
}

/* ========================================================================== */
SCODE VencMessage_Start(HANDLE hMessageProcObj)
{
	BYTE abyBuffer[512];
	fd_set rfds;
	int iFifoFD;
	struct timeval tv;
	int iReadSz;
	int ret = -1;

	iFifoFD = MsgReader_GetFifoFd(hMessageProcObj);
	syslog(LOG_INFO, "[VENC_MESSAGE] main thread pid: %d\n", getpid());

	while (g_bTerminate == FALSE) {
		memset(abyBuffer, 0, 512);
		FD_ZERO(&rfds);
		FD_SET(iFifoFD, &rfds);
		tv.tv_sec = 2;
		tv.tv_usec = 0;
		//		if (select(iFifoFD + 1, &rfds, NULL, NULL, &tv) > 0) {
		//The timeout is NULL.
		//That means that select() will block indefinitely until one of the file descriptors is ready.
		//Any signal will cause select to return immediately. So we must check for EINTR 			// and maybe repeat select() again.	
		ret = select(iFifoFD + 1, &rfds, NULL, NULL, NULL);
		if (ret == -1)
		{	
			if (errno == EINTR) {
				//printf("errno==EINTR g_bTerminate=%d\n",g_bTerminate);
				//perror("select");
				continue;

			}
			return S_FAIL;		
		}	


		if (FD_ISSET(iFifoFD,&rfds)) {
			iReadSz = read(iFifoFD, abyBuffer, 512);
			MsgReader_ParseMsg(hMessageProcObj, abyBuffer, iReadSz);
		}
	}
	return S_OK;
	}

	/* ========================================================================== */
	SCODE VencMessage_Stop(HANDLE hMessageProcObj)
	{
		return S_OK;
	}

	/* ========================================================================== */
	SCODE VencMessage_Release(HANDLE* phMessageProcObj)
	{
		MsgReader_Release(phMessageProcObj);
		return S_OK;
	}
