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
 * venc_app_setupvml.c
 *
 * \brief
 * Initial each required component.
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

void VencApp_GetSensorConfigPath(HANDLE hObject, const CHAR *szConfigFile, SDWORD len);
void VencApp_GetCapabilityPath(HANDLE hObject, const CHAR *szConfigFile, SDWORD len);
void VencApp_SetMasterInitial(HANDLE hObject, const CHAR *szInitial, SDWORD len);
void VencApp_SetMasterConfigPath(HANDLE hObject, const CHAR *szConfigFile, SDWORD len);
void VencApp_SetVideoInSharedBufferPath(HANDLE hObject, const CHAR *szSharedBufferPath, SDWORD len);
void VencApp_SetVideoInInitial(HANDLE hObject, const CHAR *szInitial, SDWORD len);
void VencApp_SetVideoInConfigPath(HANDLE hObject, const CHAR *szConfigFile, SDWORD len);
void VencApp_SetEncoderTotalNum(HANDLE hObject, const CHAR *szEncoderTotalNum, SDWORD len);
SCODE VencApp_SetEncoderTag(HANDLE hObject, const CHAR* szIDTag, const CHAR** atts);
void VencApp_SetEncoderConfigPath(HANDLE hObject, const CHAR *szConfigFile, SDWORD len);
void VencApp_SetMotionInitial(HANDLE hObject, const CHAR *szInitial, SDWORD len);
void VencApp_SetMotionConfigPath(HANDLE hObject, const CHAR *szConfigFile, SDWORD len);
void VencApp_SetDisplayInitial(HANDLE hObject, const CHAR *szInitial, SDWORD len);
void VencApp_SetDisplayConfigPath(HANDLE hObject, const CHAR *szConfigFile, SDWORD len);
void VencApp_GetVICOutput_DRAMLocation(HANDLE hObject, const CHAR *szLocation, SDWORD len);
void VencApp_GetLDCRefFrame_DRAMLocation(HANDLE hObject, const CHAR *szLocation, SDWORD len);
void VencApp_GetDMACOutput_DRAMLocation(HANDLE hObject, const CHAR *szLocation, SDWORD len);
void VencApp_GetIREOutput0_DRAMLocation(HANDLE hObject, const CHAR *szLocation, SDWORD len);
void VencApp_GetIREOutput1_DRAMLocation(HANDLE hObject, const CHAR *szLocation, SDWORD len);
void VencApp_GetBitStreamOutput_DRAMLocation(HANDLE hObject, const CHAR *szLocation, SDWORD len);
void VencApp_GetVOCOutput_DRAMLocation(HANDLE hObject, const CHAR *szLocation, SDWORD len);
void VencApp_GetMotionSnapshotOutput_DRAMLocation(HANDLE hObject, const CHAR *szLocation, SDWORD len);
void VencApp_GetMp4VBusNum(HANDLE hObject, const CHAR *szLocation, SDWORD len);
void VencApp_GetH264BusNum(HANDLE hObject, const CHAR *szLocation, SDWORD len);
void VencApp_GetSVCBusNum(HANDLE hObject, const CHAR *szLocation, SDWORD len);
void VencApp_GetSupportVideoCodec(HANDLE hObject, const CHAR *szSupportCodec, SDWORD len);

/* =========================================================================================== */
SCODE VencApp_InitMemMgr(HANDLE *phMemMgrObj)
{
    TMemMgrInitOptions tMemMgrInitOptions;
    memset(&tMemMgrInitOptions, 0, sizeof(TMemMgrInitOptions));

    tMemMgrInitOptions.dwVersion        = MEMMGR_VERSION;
    tMemMgrInitOptions.pObjectMem       = (void *)NULL;
    return (MemMgr_Initial(phMemMgrObj, &tMemMgrInitOptions));
}

/* =========================================================================================== */
SCODE VencApp_InitXmlWrapper(HANDLE *phXMLWrapperObj)
{
    TXmlWrapperInitOptions txpInitOptions;
        
    txpInitOptions.dwVersion = XMLWRAPPER_VERSION;

    if (XmlWrapper_Initial(phXMLWrapperObj, &txpInitOptions) != S_OK){
        fprintf(stderr, "[VENC] Initial XMLWrapper object fail !! \n");
        return S_FAIL;
    }

    if (XmlWrapper_SetHandler(*phXMLWrapperObj, ptTreeMap,NULL) != S_OK) {
        fprintf(stderr, "[VENC] Set XMLWrapper handler fail !! \n");
        return S_FAIL;
    }

    return S_OK;
}

/* ============================================================================================= */
void VencApp_GetVideoCapResolution(HANDLE hObject, const CHAR *szResolution, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    CHAR    *szRes;
    char    *szToken = NULL;
    DWORD   dwWidth, dwHeight;

    szRes = malloc(len+1);
    memset(szRes, 0, len+1);
    strncpy(szRes, szResolution, len);
    dwWidth = strtol(szRes, &szToken, 10);
    dwHeight = strtol(szToken+1, (char**) NULL, 10);

	if (szToken == NULL) return;
	
	if (dwWidth > ptVencInfo->tCapabilityRec.dwVideoMaxWidth) {
	    printf("[VENC] Capture width range is out of sensor capability !!\n");
	    dwWidth = ptVencInfo->tCapabilityRec.dwVideoMaxWidth;
	}
	
	if (dwHeight > ptVencInfo->tCapabilityRec.dwVideoMaxHeight) {
	    printf("[VENC] Capture height range is out of sensor capability !!\n");
	    dwHeight = ptVencInfo->tCapabilityRec.dwVideoMaxHeight;
	}

	ptVencInfo->tCapabilityRec.dwVideoCapWidth = dwWidth;
	ptVencInfo->tCapabilityRec.dwVideoCapHeight = dwHeight;

}
/* ============================================================================================= */
static SDWORD GetParam(FILE *pfInput)
{
    CHAR acTmpText[200];
    SDWORD sdwRet;

    fscanf(pfInput, "%s", acTmpText);
    sdwRet = atoi(acTmpText);
    fgets(acTmpText, 200, pfInput);

    return sdwRet;
}

/* ============================================================================================= */
SCODE VencApp_GetInitNeedInfo(TVencInfo *ptVencInfo)
{
	 char    aszSensorName[16];
	// get buffer number, that should be used in initial VideoCap
	// get sensor max support size, that match VIDEO_MAX_WIDTH in past definition
	{
		FILE 	*pfInput = NULL;
        char 	aszPath[64];
		
        sprintf(aszPath, "%s/dwBufNum", ptVencInfo->tVencCfgPath.szCapabilityPath);
		// open /sys/... file
	    if ((pfInput = fopen(aszPath, "r")) == NULL) {
	        printf("[VENC] Open %s fail !!\n", aszPath);
	        return S_FAIL;
	    }
		fscanf(pfInput, "%d", &ptVencInfo->tCapabilityRec.dwBufNum);
		fclose(pfInput);

        sprintf(aszPath, "%s/MaxFrameWidth", ptVencInfo->tVencCfgPath.szCapabilityPath);
		// open /sys/... file
	    if ((pfInput = fopen(aszPath, "r")) == NULL) {
	        printf("[VENC] Open %s fail !!\n", aszPath);
	        return S_FAIL;
	    }
		fscanf(pfInput, "%d", &ptVencInfo->tCapabilityRec.dwVideoMaxWidth);
		fclose(pfInput);

        sprintf(aszPath, "%s/MaxFrameHeight", ptVencInfo->tVencCfgPath.szCapabilityPath);
		// open /sys/... file
	    if ((pfInput = fopen(aszPath, "r")) == NULL) {
	        printf("[VENC] Open %s fail !!\n", aszPath);
	        return S_FAIL;
	    }
		fscanf(pfInput, "%d", &ptVencInfo->tCapabilityRec.dwVideoMaxHeight);
		fclose(pfInput);

        {
           
            // query sensor name to get progressive or interlace scan, but it's workaround.
            // This information should query from driver
            sprintf(aszPath, "%s/SensorName", ptVencInfo->tVencCfgPath.szCapabilityPath);
    		// open /sys/... file
    	    if ((pfInput = fopen(aszPath, "r")) == NULL) {
    	        printf("[VENC] Open %s fail !!\n", aszPath);
    	        return S_FAIL;
    	    }
    		fscanf(pfInput, "%s", aszSensorName);
    		fclose(pfInput);
    		
    		if (strcasecmp(aszSensorName, "TW2866") == 0) {
				ptVencInfo->tCapabilityRec.bInterlaceScan = TRUE;
				ptVencInfo->tCapabilityRec.bCMOSSensor = FALSE;
    		} else if (strcasecmp(aszSensorName, "NVP1114A") == 0) {
				ptVencInfo->tCapabilityRec.bInterlaceScan = TRUE;
				ptVencInfo->tCapabilityRec.bCMOSSensor = FALSE;
    		} else {
				ptVencInfo->tCapabilityRec.bInterlaceScan = FALSE;
				ptVencInfo->tCapabilityRec.bCMOSSensor = TRUE;
    		}
        }
	}

	// get capture size, that match VIDEO_ENC_WIDTH in past definition
	{
		HANDLE hXmlWrapperObj;
		TXmlWrapperInitOptions tXmlWrapperInitOptions;
		
		// Initial XMLWrapper to get VideoCapWidth
	    TXmlWrapperTreeMap ptVideoCapTreeMap[] = 
	    {
		    {"root/venc_master/setting/resolution", NULL, &VencApp_GetVideoCapResolution, NULL},
	    	{NULL, NULL, NULL}
		};
		
		tXmlWrapperInitOptions.dwVersion = XMLWRAPPER_VERSION;
		if (XmlWrapper_Initial(&hXmlWrapperObj, &tXmlWrapperInitOptions) != S_OK) {
			fprintf(stderr, "[VENC] Initial XMLWrapper object fail !! \n");
			return S_FAIL;
		}
		
		if (XmlWrapper_SetHandler(hXmlWrapperObj, ptVideoCapTreeMap,NULL) != S_OK) {
			fprintf(stderr, "[VENC] Set XMLWrapper handler fail !! \n");
			return S_FAIL;
		}
		
		if (XmlWrapper_ReadFile_UsrDefFunc(ptVencInfo->tVencCfgPath.szMasterConfigFile, hXmlWrapperObj, ptVencInfo) != S_OK) {
			fprintf(stderr, "[VENC] XmlWrapper_ReadFile_UsrDefFunc %s Fail! \n", ptVencInfo->tVencCfgPath.szMasterConfigFile);
			return S_FAIL;
		}
		
		if (XmlWrapper_Release(&hXmlWrapperObj) != S_OK) {
			printf("[VENC] Release XmlWrapper Fail! \n");
			return S_FAIL;
		}
	}

	// get sensor in size, that match VIDEO_IN_WIDTH in past definition
	{
		FILE 	*pfSensorCfgInput = NULL;
		DWORD	dwIndex;
		
		// open sensor config file
	    if ((pfSensorCfgInput = fopen(ptVencInfo->tVencCfgPath.szSensorConfigFile, "r")) == NULL) {
	        printf("[VENC] Open sensor config file %s fail !!\n", ptVencInfo->tVencCfgPath.szSensorConfigFile);
	        return S_FAIL;
	    }
		printf("[VENC] Open sensor config file %s success !!\n", ptVencInfo->tVencCfgPath.szSensorConfigFile);
		
		// ignore 12 fields
		for (dwIndex = 0; dwIndex < 12; dwIndex++) {
			GetParam(pfSensorCfgInput);
		}

		ptVencInfo->tCapabilityRec.dwVideoInWidth = GetParam(pfSensorCfgInput);
		ptVencInfo->tCapabilityRec.dwVideoInHeight = GetParam(pfSensorCfgInput);
		
		#if defined(_5M2MSWITCH)
			ptVencInfo->tCapabilityRec.dwVideoInWidth = 2560;
			ptVencInfo->tCapabilityRec.dwVideoInHeight = 1920;
		#elif defined(_3M2MSWITCH)
			ptVencInfo->tCapabilityRec.dwVideoInWidth = 2048;
			ptVencInfo->tCapabilityRec.dwVideoInHeight = 1536;
		#endif
		if (pfSensorCfgInput != NULL) {
			fclose(pfSensorCfgInput);
		}
	}
	ptVencInfo->tCapabilityRec.szSensorName = strdup(aszSensorName);
	printf("[venc_app_init](%d)SensorName: %s\n",__LINE__,ptVencInfo->tCapabilityRec.szSensorName);
	if (strcasecmp(aszSensorName, "MN34030PL") == 0)
	{
	  
	  if ((ptVencInfo->tCapabilityRec.dwVideoCapWidth ==1280) && (ptVencInfo->tCapabilityRec.dwVideoCapHeight== 720))
	  {
	      ptVencInfo->tCapabilityRec.dwMaxCapFrameRate = 60;
	  }
	  else
	  {
	      ptVencInfo->tCapabilityRec.dwMaxCapFrameRate = 30;
	  }
	}
	else if (strcasecmp(aszSensorName, "MN34041PL") == 0)
	{
		ptVencInfo->tCapabilityRec.dwMaxCapFrameRate = 60;
	}
	else
	{
		ptVencInfo->tCapabilityRec.dwMaxCapFrameRate = 30;
	}   
	printf("[venc_app_init](%d)dwMaxCapFrameRate=%d  bCMOSSensor=%d\n",__LINE__,ptVencInfo->tCapabilityRec.dwMaxCapFrameRate,ptVencInfo->tCapabilityRec.bCMOSSensor);
	
	return S_OK;
}


/* ============================================================================================= */
SCODE VencApp_ParseVencConfig(TVencInfo *ptVencInfo)
{
	HANDLE hXmlWrapperObj;
	TXmlWrapperInitOptions tXmlWrapperInitOptions;
	
	// Initial XMLWrapper to get all relative config
    TXmlWrapperTreeMap ptCfgTreeMap[] = 
    {
		{"root/sensor_config_path", NULL, &VencApp_GetSensorConfigPath, NULL},
	    {"root/sys_path", NULL, &VencApp_GetCapabilityPath, NULL},
	    {"root/buffer_info/vic_output/dram_location", NULL, &VencApp_GetVICOutput_DRAMLocation, NULL},
	    {"root/buffer_info/ldc_ref_frame/dram_location", NULL, &VencApp_GetLDCRefFrame_DRAMLocation, NULL},
	    {"root/buffer_info/dmac_output/dram_location", NULL, &VencApp_GetDMACOutput_DRAMLocation, NULL},
	    {"root/buffer_info/ire_output0/dram_location", NULL, &VencApp_GetIREOutput0_DRAMLocation, NULL},
	    {"root/buffer_info/ire_output1/dram_location", NULL, &VencApp_GetIREOutput1_DRAMLocation, NULL},
	    {"root/buffer_info/bitstream_output/dram_location", NULL, &VencApp_GetBitStreamOutput_DRAMLocation, NULL},
	    {"root/buffer_info/voc_output/dram_location", NULL, &VencApp_GetVOCOutput_DRAMLocation, NULL},
	    {"root/buffer_info/motion_snapshot_output/dram_location", NULL, &VencApp_GetMotionSnapshotOutput_DRAMLocation, NULL},
	    {"root/bus_info/mp4v_bus_num", NULL, &VencApp_GetMp4VBusNum, NULL},
	    {"root/bus_info/h264_bus_num", NULL, &VencApp_GetH264BusNum, NULL},
	    {"root/bus_info/svc_bus_num",NULL, &VencApp_GetSVCBusNum, NULL},
	    {"root/venc_master/initial", NULL, &VencApp_SetMasterInitial, NULL},
	    {"root/venc_master/config_path", NULL, &VencApp_SetMasterConfigPath, NULL},
	    {"root/venc_videoin/sharebuffer/path", NULL, &VencApp_SetVideoInSharedBufferPath, NULL},
	    {"root/venc_videoin/initial", NULL, &VencApp_SetVideoInInitial, NULL},
	    {"root/venc_videoin/config_path", NULL, &VencApp_SetVideoInConfigPath, NULL},
	    {"root/venc_encoder/total_num", NULL, &VencApp_SetEncoderTotalNum, NULL},
	    {"root/venc_encoder/encoder", &VencApp_SetEncoderTag, NULL, NULL},
	    {"root/venc_encoder/encoder/config_path", NULL, &VencApp_SetEncoderConfigPath, NULL},
	    {"root/venc_encoder/support_codectype",NULL,&VencApp_GetSupportVideoCodec,NULL},
	    {"root/venc_motion/initial", NULL, &VencApp_SetMotionInitial, NULL},
	    {"root/venc_motion/config_path", NULL, &VencApp_SetMotionConfigPath, NULL},
	    {"root/venc_display/initial", NULL, &VencApp_SetDisplayInitial, NULL},
	    {"root/venc_display/config_path", NULL, &VencApp_SetDisplayConfigPath, NULL},
	    {NULL, NULL, NULL}
	};
	
	tXmlWrapperInitOptions.dwVersion = XMLWRAPPER_VERSION;
	if (XmlWrapper_Initial(&hXmlWrapperObj, &tXmlWrapperInitOptions) != S_OK) {
		fprintf(stderr, "[VENC] Initial XMLWrapper object fail !! \n");
		return S_FAIL;
	}
	
	if (XmlWrapper_SetHandler(hXmlWrapperObj, ptCfgTreeMap,NULL) != S_OK) {
		fprintf(stderr, "[VENC] Set XMLWrapper handler fail !! \n");
		return S_FAIL;
	}
	
	if (XmlWrapper_ReadFile_UsrDefFunc(ptVencInfo->szConfigFile, hXmlWrapperObj, ptVencInfo) != S_OK) {
		fprintf(stderr, "[VENC] XmlWrapper_ReadFile_UsrDefFunc %s Fail! \n", ptVencInfo->szConfigFile);
		return S_FAIL;
	}
	
	if (XmlWrapper_Release(&hXmlWrapperObj) != S_OK) {
		printf("[VENC] Release XmlWrapper Fail! \n");
		return S_FAIL;
	}
	return S_OK;
}

/* ============================================================================================= */
void VencApp_GetCapabilityPath(HANDLE hObject, const CHAR *szConfigFile, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    CHAR szCfgPath[len+1];

    snprintf(szCfgPath, len+1, "%s", szConfigFile);
    ptVencInfo->tVencCfgPath.szCapabilityPath = strdup(szCfgPath);
	printf("[VENC] capability path = %s !!\n", ptVencInfo->tVencCfgPath.szCapabilityPath);
}

/* ============================================================================================= */
void VencApp_GetSensorConfigPath(HANDLE hObject, const CHAR *szConfigFile, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    CHAR szCfgPath[len+1];

    snprintf(szCfgPath, len+1, "%s", szConfigFile);
    ptVencInfo->tVencCfgPath.szSensorConfigFile = strdup(szCfgPath);
	printf("[VENC] sensor config file = %s !!\n", ptVencInfo->tVencCfgPath.szSensorConfigFile);
}

/* ============================================================================================= */
void VencApp_GetVICOutput_DRAMLocation(HANDLE hObject, const CHAR *szLocation, SDWORD len)
{
    TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    DWORD dwLocation = strtoul(szLocation, (char**)NULL, 10);
    
    if ((dwLocation == 0) || (dwLocation == 1)) {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwVICOutputDRAMLoc = dwLocation;
    } else {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwVICOutputDRAMLoc = 0;	// if others, set it to 0
    }
}

/* ============================================================================================= */
void VencApp_GetLDCRefFrame_DRAMLocation(HANDLE hObject, const CHAR *szLocation, SDWORD len)
{
    TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    DWORD dwLocation = strtoul(szLocation, (char**)NULL, 10);
    
    if ((dwLocation == 0) || (dwLocation == 1)) {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwLDCRefFrameDRAMLoc = dwLocation;
    } else {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwLDCRefFrameDRAMLoc = 0;	// if others, set it to 0
    }
}

/* ============================================================================================= */
void VencApp_GetDMACOutput_DRAMLocation(HANDLE hObject, const CHAR *szLocation, SDWORD len)
{
    TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    DWORD dwLocation = strtoul(szLocation, (char**)NULL, 10);
    if ((dwLocation == 0) || (dwLocation == 1)) {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwDMACOutputDRAMLoc = dwLocation;
    } else {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwDMACOutputDRAMLoc = 0;	// if others, set it to 0
    }
}

/* ============================================================================================= */
void VencApp_GetIREOutput0_DRAMLocation(HANDLE hObject, const CHAR *szLocation, SDWORD len)
{
    TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    DWORD dwLocation = strtoul(szLocation, (char**)NULL, 10);
    
    if ((dwLocation == 0) || (dwLocation == 1)) {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwIREOutput0DRAMLoc = dwLocation;
    } else {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwIREOutput0DRAMLoc = 0;	// if others, set it to 0
    }
}

/* ============================================================================================= */
void VencApp_GetIREOutput1_DRAMLocation(HANDLE hObject, const CHAR *szLocation, SDWORD len)
{
    TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    DWORD dwLocation = strtoul(szLocation, (char**)NULL, 10);
    
    if ((dwLocation == 0) || (dwLocation == 1)) {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwIREOutput1DRAMLoc = dwLocation;
    } else {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwIREOutput1DRAMLoc = 0;	// if others, set it to 0
    }
}

/* ============================================================================================= */
void VencApp_GetBitStreamOutput_DRAMLocation(HANDLE hObject, const CHAR *szLocation, SDWORD len)
{
    TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    DWORD dwLocation = strtoul(szLocation, (char**)NULL, 10);
    
    if ((dwLocation == 0) || (dwLocation == 1)) {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwBitStreamOutputDRAMLoc = dwLocation;
    } else {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwBitStreamOutputDRAMLoc = 0;	// if others, set it to 0
    }
}

/* ============================================================================================= */
void VencApp_GetVOCOutput_DRAMLocation(HANDLE hObject, const CHAR *szLocation, SDWORD len)
{
    TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    DWORD dwLocation = strtoul(szLocation, (char**)NULL, 10);
    
    if ((dwLocation == 0) || (dwLocation == 1)) {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwVOCOutputDRAMLoc = dwLocation;
    } else {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwVOCOutputDRAMLoc = 0;	// if others, set it to 0
    }
}

/* ============================================================================================= */
void VencApp_GetMotionSnapshotOutput_DRAMLocation(HANDLE hObject, const CHAR *szLocation, SDWORD len)
{
    TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    DWORD dwLocation = strtoul(szLocation, (char**)NULL, 10);
    
    if ((dwLocation == 0) || (dwLocation == 1)) {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwMotionSnapshotOutputDRAMLoc = dwLocation;
    } else {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwMotionSnapshotOutputDRAMLoc = 0;	// if others, set it to 0
    }
}

/* ============================================================================================= */
void VencApp_GetMp4VBusNum(HANDLE hObject, const CHAR *szLocation, SDWORD len)
{
    TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    DWORD dwLocation = strtoul(szLocation, (char**)NULL, 10);
    
    if ((dwLocation == 0) || (dwLocation == 1)) {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwMp4VBusNum = dwLocation;
    } else {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwMp4VBusNum = 0;	// if others, set it to 0
    }
}

/* ============================================================================================= */
void VencApp_GetH264BusNum(HANDLE hObject, const CHAR *szLocation, SDWORD len)
{
    TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    DWORD dwLocation = strtoul(szLocation, (char**)NULL, 10);
    
    if ((dwLocation == 0) || (dwLocation == 1)) {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwH264BusNum = dwLocation;
    } else {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwH264BusNum = 0;	// if others, set it to 0
    }
}

/* ============================================================================================= */
void VencApp_GetSVCBusNum(HANDLE hObject, const CHAR *szLocation, SDWORD len)
{
    TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    DWORD dwLocation = strtoul(szLocation, (char**)NULL, 10);
    
    if ((dwLocation == 0) || (dwLocation == 1)) {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwSVCBusNum = dwLocation;
    } else {
    	ptVencInfo->tVencCfgPath.tDRAMParam.dwSVCBusNum = 0;	// if others, set it to 0
    }


}
/* ============================================================================================= */
void VencApp_SetMasterInitial(HANDLE hObject, const CHAR *szInitial, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    CHAR szTemp[len+1];
	BOOL bEnabled = FALSE;
    snprintf(szTemp, len+1, "%s", szInitial);

    if (strcmp(szTemp, "on") == 0)
    	bEnabled = TRUE;

	ptVencInfo->tVencCfgPath.bMasterEnable = bEnabled;
	
	printf("[VENC] master initial = %s !!\n", (ptVencInfo->tVencCfgPath.bMasterEnable == TRUE)? "TRUE": "FALSE");
}

/* ============================================================================================= */
void VencApp_SetMasterConfigPath(HANDLE hObject, const CHAR *szConfigFile, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    CHAR szCfgPath[len+1];

    snprintf(szCfgPath, len+1, "%s", szConfigFile);
    ptVencInfo->tVencCfgPath.szMasterConfigFile = strdup(szCfgPath);
	printf("[VENC] master config file = %s !!\n", ptVencInfo->tVencCfgPath.szMasterConfigFile);
}

/* ============================================================================================= */
void VencApp_SetVideoInSharedBufferPath(HANDLE hObject, const CHAR *szSharedBufferPath, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    CHAR szSrdBufPath[len+1];
	CHAR *byTemp;

    snprintf(szSrdBufPath, len+1, "%s", szSharedBufferPath);
	byTemp = strrchr(strdup(szSrdBufPath), 'r');
	byTemp++;
	ptVencInfo->tVencCfgPath.dwSharedBufferMinorNum = (DWORD) strtol(byTemp,NULL,10);
	printf("[VENC] VideoIn-Encoder Sharedbuffer Device Node = %u !!\n", ptVencInfo->tVencCfgPath.dwSharedBufferMinorNum);
}

/* ============================================================================================= */
void VencApp_SetVideoInInitial(HANDLE hObject, const CHAR *szInitial, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    CHAR szTemp[len+1];
	BOOL bEnabled = FALSE;
    snprintf(szTemp, len+1, "%s", szInitial);

    if (strcmp(szTemp, "on") == 0)
    	bEnabled = TRUE;

	ptVencInfo->tVencCfgPath.bVideoInEnable = bEnabled;
	
	printf("[VENC] Videoin initial = %s !!\n", (ptVencInfo->tVencCfgPath.bVideoInEnable == TRUE)? "TRUE": "FALSE");
}

/* ============================================================================================= */
void VencApp_SetVideoInConfigPath(HANDLE hObject, const CHAR *szConfigFile, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    CHAR szCfgPath[len+1];

    snprintf(szCfgPath, len+1, "%s", szConfigFile);
    ptVencInfo->tVencCfgPath.szVideoInConfigFile = strdup(szCfgPath);
	printf("[VENC] Videoin config file = %s !!\n", ptVencInfo->tVencCfgPath.szVideoInConfigFile);
}

/* ============================================================================================= */
void VencApp_SetMotionInitial(HANDLE hObject, const CHAR *szInitial, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    CHAR szTemp[len+1];
	BOOL bEnabled = FALSE;
    snprintf(szTemp, len+1, "%s", szInitial);

    if (strcmp(szTemp, "on") == 0)
    	bEnabled = TRUE;

	ptVencInfo->tVencCfgPath.bMotionEnable = bEnabled;
	
	printf("[VENC] motion initial = %s !!\n", (ptVencInfo->tVencCfgPath.bMotionEnable == TRUE)? "TRUE": "FALSE");
}

/* ============================================================================================= */
void VencApp_SetMotionConfigPath(HANDLE hObject, const CHAR *szConfigFile, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    CHAR szCfgPath[len+1];

    snprintf(szCfgPath, len+1, "%s", szConfigFile);
    ptVencInfo->tVencCfgPath.szMotionConfigFile = strdup(szCfgPath);
	printf("[VENC] Motion sending process config file = %s !!\n", ptVencInfo->tVencCfgPath.szMotionConfigFile);
}

/* ============================================================================================= */
void VencApp_SetDisplayInitial(HANDLE hObject, const CHAR *szInitial, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    CHAR szTemp[len+1];
	BOOL bEnabled = FALSE;
    snprintf(szTemp, len+1, "%s", szInitial);

    if (strcmp(szTemp, "on") == 0)
    	bEnabled = TRUE;

	ptVencInfo->tVencCfgPath.bDisplayEnable = bEnabled;
	
	printf("[VENC] Display initial = %s !!\n", (ptVencInfo->tVencCfgPath.bDisplayEnable == TRUE)? "TRUE": "FALSE");
}

/* ============================================================================================= */
void VencApp_SetDisplayConfigPath(HANDLE hObject, const CHAR *szConfigFile, SDWORD len)
{
    TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    CHAR szCfgPath[len+1];
    memset(szCfgPath,0x0,sizeof(szCfgPath));
    snprintf(szCfgPath, len+1, "%s", szConfigFile);
    ptVencInfo->tVencCfgPath.szDisplayConfigFile = strdup(szCfgPath);
	printf("[VENC] Display config file = %s !!\n", ptVencInfo->tVencCfgPath.szDisplayConfigFile);
}
/*===============================================================================================*/
void VencApp_GetSupportVideoCodec(HANDLE hObject, const CHAR *szTemp, SDWORD len)
{
  TVencInfo *ptVencInfo = (TVencInfo *) hObject;
  CHAR szSupportVideoCodec[len+1];
  
  CHAR *delim = SUPPORT_CODEC_STR_DELIM;
  DWORD dwVideoCodecInitMask = 0;
  CHAR *p = NULL;
  
  memset(szSupportVideoCodec,0x0,sizeof(szSupportVideoCodec));
  snprintf(szSupportVideoCodec, len+1, "%s", szTemp);
   
  p = strtok(szSupportVideoCodec,delim); 
  //printf("%s\n",p);
  //printf("strlen(p)=%d\n",strlen(p));
  if (strncmp(p,JPEG_STR,strlen(p)) == 0)
  {
    dwVideoCodecInitMask |= VIDEO_CODEC_JPEG_INIT_BIT;
  }
  else if (strncmp(p,MP4V_STR,strlen(p)) == 0)
  {
    dwVideoCodecInitMask |=  VIDEO_CODEC_MP4V_INIT_BIT;
  }
  else if (strncmp(p,H264_STR,strlen(p)) == 0)
  {
    dwVideoCodecInitMask |=  VIDEO_CODEC_H264_INIT_BIT;
  }
  else if (strncmp(p,SVC_STR,strlen(p)) == 0)
  {
    dwVideoCodecInitMask |=  VIDEO_CODEC_SVC_INIT_BIT;
  }
  
  while(1)
  {
     p = strtok(NULL,delim);
     
     if (p == NULL)
     {  
       break;
     }
     else
     {	if (strncmp(p,JPEG_STR,strlen(p)) == 0)
	{
	  dwVideoCodecInitMask |= VIDEO_CODEC_JPEG_INIT_BIT;
	}
	else if (strncmp(p,MP4V_STR,strlen(p)) == 0)
	{
	  dwVideoCodecInitMask |=  VIDEO_CODEC_MP4V_INIT_BIT;
	}
	else if (strncmp(p,H264_STR,strlen(p)) == 0)
	{
	  dwVideoCodecInitMask |=  VIDEO_CODEC_H264_INIT_BIT;
	}
	else if (strncmp(p,SVC_STR,strlen(p)) == 0)
	{
	  dwVideoCodecInitMask |=  VIDEO_CODEC_SVC_INIT_BIT;
	}
     
     }
     //printf("next: %s \n",p);
  }
  ptVencInfo->tCapabilityRec.dwVideoCodecInitMask = dwVideoCodecInitMask;
  printf("[venc_app_init](%d)VideoCodecInitMask:%08x\n",__LINE__,ptVencInfo->tCapabilityRec.dwVideoCodecInitMask);
  
  return;
}
/* ============================================================================================= */
void VencApp_SetEncoderTotalNum(HANDLE hObject, const CHAR *szEncoderTotalNum, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *) hObject;
	DWORD dwEncoderTotalNum;
	DWORD	dwCurId;
	
	dwEncoderTotalNum = atoi(szEncoderTotalNum);
	if (dwEncoderTotalNum < 0) {
	    ptVencInfo->tVencCfgPath.dwEncoderTotalNum = 0;
#if 0
	} else if (dwEncoderTotalNum > MAX_SLAVE_NUM) {
	    ptVencInfo->tVencCfgPath.dwEncoderTotalNum = MAX_SLAVE_NUM;
	}
#endif
	} else {
	    ptVencInfo->tVencCfgPath.dwEncoderTotalNum = dwEncoderTotalNum;
	}
	ptVencInfo->pdwCurrSwitch = (DWORD *)malloc(sizeof(DWORD)*ptVencInfo->tVencCfgPath.dwEncoderTotalNum);
	for (dwCurId = 0; dwCurId < ptVencInfo->tVencCfgPath.dwEncoderTotalNum; dwCurId++) {
		ptVencInfo->pdwCurrSwitch[dwCurId] = -1;
	}
	ptVencInfo->tVencCfgPath.pszEncoderConfigFile = (CHAR **)malloc(sizeof(CHAR*)*ptVencInfo->tVencCfgPath.dwEncoderTotalNum);
	for (dwCurId = 0; dwCurId < ptVencInfo->tVencCfgPath.dwEncoderTotalNum; dwCurId++) {
		ptVencInfo->tVencCfgPath.pszEncoderConfigFile[dwCurId]=NULL;
	}
	printf("[VENC] Video Encoder Total Number = %u !!\n", ptVencInfo->tVencCfgPath.dwEncoderTotalNum);
}

/* ========================================================================= */
SCODE VencApp_SetEncoderTag(HANDLE hObject, const CHAR *szIDTag, const CHAR** atts)
{
	TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    DWORD  dwCurId;

    if (atts) {
        if (strcmp(atts[0], "id") != 0) {
            DBPRINT0("[VENC] No id attribute \n");
            return IGNORE_CHILD_CONFIG;            
        }
        dwCurId = strtoul(atts[1], (char**)NULL, 10);
        if (dwCurId < 0 || dwCurId >= ptVencInfo->tVencCfgPath.dwEncoderTotalNum)
        	return IGNORE_CHILD_CONFIG;
        ptVencInfo->tVencCfgPath.dwCurId = dwCurId;
        return S_OK;
    }
    DBPRINT0("[VENC] No attribute \n");
    return IGNORE_CHILD_CONFIG;
}

/* ============================================================================================= */
void VencApp_SetEncoderConfigPath(HANDLE hObject, const CHAR *szConfigFile, SDWORD len)
{
	TVencInfo *ptVencInfo = (TVencInfo *) hObject;
    CHAR szCfgPath[len+1];
    DWORD dwCurId = ptVencInfo->tVencCfgPath.dwCurId;

    snprintf(szCfgPath, len+1, "%s", szConfigFile);
    ptVencInfo->tVencCfgPath.pszEncoderConfigFile[dwCurId] = strdup(szCfgPath);
	printf("[VENC] slave %u config file = %s !!\n", dwCurId, ptVencInfo->tVencCfgPath.pszEncoderConfigFile[dwCurId]);
}
