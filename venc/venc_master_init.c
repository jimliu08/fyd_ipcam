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
 * venc_master_init.c
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

#include "venc_master_local.h"

extern TXmlWrapperTreeMap ptMasterTreeMap[];

/* ============================================================================================= */
SCODE VencMaster_IRCut_Initial(TMasterInfo *ptMasterInfo)
{
    int fd = -1;
    
    ptMasterInfo->fdIRCutUART = -1;
    ptMasterInfo->fdIRCut = -1;
    
    fd = open(IRCUT_DEVICE, O_RDWR);
    if (fd == -1)
    {
	    printf("[venc_master_init](%d)IRCut_Initial() is  Failed! Because cannot open %s.(%s)\n",__LINE__,IRCUT_DEVICE,strerror(errno));
	    return S_FAIL;
    }
	ptMasterInfo->fdIRCut = fd;
	printf("[venc_master](%d)(fdIRCut)=(%d)\n",__LINE__,ptMasterInfo->fdIRCut); 
	return S_OK;
}

/* ============================================================================================= */
SCODE VencMaster_IRCut_Release(TMasterInfo *ptMasterInfo)
{
	if (ptMasterInfo->fdIRCut != -1)
	{
		if (close(ptMasterInfo->fdIRCut) == -1)
		{
			printf("[venc_master](%d)IRCut_Release() is  Failed!(%s)\n",__LINE__,strerror(errno));
		}
	}
	return S_OK;
}

/* ============================================================================================= */
DWORD VencMaster_GetMemory(HANDLE hMemObject,DWORD dwBusNum, DWORD dwSize, EAlignType eAlignType)
{
    TMemMgrState tMemMgrState;
    tMemMgrState.dwBusNum = dwBusNum;
    tMemMgrState.dwSize = dwSize;
    tMemMgrState.eAlignType = eAlignType;

    return (MemMgr_GetMemory(hMemObject, &tMemMgrState));
}

/* ============================================================================================= */
SDWORD GetParam(FILE *pfInput)
{
    CHAR acTmpText[200];
    SDWORD sdwRet;

    fscanf(pfInput, "%s", acTmpText);
    sdwRet = atoi(acTmpText);
    fgets(acTmpText, 200, pfInput);

    return sdwRet;
}

/* ============================================================================================= */
SCODE AutoExposure_Setting(TMasterInfo *ptMasterInfo, TVideoCapInitOptions *ptVideoCapInitOptions)
{
	DWORD dwIndex;

	ptMasterInfo->bSWAEEnable = ptVideoCapInitOptions->adwConfig[46];
	
	// window 0 - 8 setting, and initial
	for (dwIndex = 0; dwIndex < MAX_AE_WINDOW_NUM; dwIndex++) {
		ptMasterInfo->tAEInfo.tAEWindow[dwIndex].dwStartX = ptVideoCapInitOptions->adwConfig[64+4*dwIndex];
		ptMasterInfo->tAEInfo.tAEWindow[dwIndex].dwStartY = ptVideoCapInitOptions->adwConfig[65+4*dwIndex];
		ptMasterInfo->tAEInfo.tAEWindow[dwIndex].dwWidth = ptVideoCapInitOptions->adwConfig[66+4*dwIndex];
		ptMasterInfo->tAEInfo.tAEWindow[dwIndex].dwHeight = ptVideoCapInitOptions->adwConfig[67+4*dwIndex];
		ptMasterInfo->tAEInfo.tAEWindow[dwIndex].dwWindowPixels = ptMasterInfo->tAEInfo.tAEWindow[dwIndex].dwWidth * ptMasterInfo->tAEInfo.tAEWindow[dwIndex].dwHeight;
		ptMasterInfo->tAEInfo.tAEWindow[dwIndex].dwWeight = 1;
	}
	
	for (dwIndex = APP_AE_WINDOW_NUM; dwIndex < MAX_AE_WINDOW_NUM; dwIndex++) {
		ptMasterInfo->tAEInfo.tAEWindow[dwIndex].dwWeight = 0;
	}

	return S_OK;
}
/* ============================================================================================= */
SCODE VideoCap_Setting(TMasterInfo *ptMasterInfo, const _TMasterInitNeedInfo *const _ptInitNeedInfo)
{
    TVideoCapInitOptions tVideoCapInitOptions;
    DWORD dwFrameSize, dwYFrameSize, dwCFrameSize;
    BYTE  *pbyCapBuf;
   // BYTE  *pbyStatBuf;
    FILE  *pfInput;
    DWORD dwCounter;

    memset(&tVideoCapInitOptions, 0, sizeof(TVideoCapInitOptions));

    tVideoCapInitOptions.dwVersion             = VIDEOCAP_VERSION;

    tVideoCapInitOptions.dwMaxFrameWidth       = _ptInitNeedInfo->dwVideoMaxWidth;
    tVideoCapInitOptions.dwMaxFrameHeight      = _ptInitNeedInfo->dwVideoMaxHeight;
    tVideoCapInitOptions.dwCapWidth            = _ptInitNeedInfo->dwVideoCapWidth;
    tVideoCapInitOptions.dwCapHeight           = _ptInitNeedInfo->dwVideoCapHeight;
   	tVideoCapInitOptions.dwInWidth         	   = _ptInitNeedInfo->dwVideoInWidth;
   	tVideoCapInitOptions.dwInHeight        	   = _ptInitNeedInfo->dwVideoInHeight;
#if defined (_YUV422_)
    tVideoCapInitOptions.eOutPixelFormat       = YUV422;
#else
	tVideoCapInitOptions.eOutPixelFormat       = YUV420;
#endif	
	tVideoCapInitOptions.eFrequency			   = VIDEO_SIGNAL_FREQUENCY_60HZ;

	// sync with VideoCap library v7.3.0.3
#if defined (_NVP1114A) || defined (_TW2866) 
	tVideoCapInitOptions.byChNum0 = 4;
	tVideoCapInitOptions.byChNum1 = 4;
	tVideoCapInitOptions.bChClkEdge0 = 0;
	tVideoCapInitOptions.bChClkEdge1 = 0;
	tVideoCapInitOptions.eChIDType0 = SYNC_ID;
	tVideoCapInitOptions.eChIDType1 = SYNC_ID;
#else
	tVideoCapInitOptions.byChNum0 = 1;
	tVideoCapInitOptions.byChNum1 = 1;
	tVideoCapInitOptions.bChClkEdge0 = 0;
	tVideoCapInitOptions.bChClkEdge1 = 0;
	tVideoCapInitOptions.eChIDType0 = NO_CH_ID;
	tVideoCapInitOptions.eChIDType1 = NO_CH_ID;
#endif //_NVP1114A
#ifdef _VMA_IBPE
    tVideoCapInitOptions.dwBufNum              =  BUFF_SIZE+4;//_ptInitNeedInfo->dwBufNum;
#else
	tVideoCapInitOptions.dwBufNum              =  _ptInitNeedInfo->dwBufNum;
#endif
	
	printf("[venc_master_init](%d)Capture Buffer Num:%d\n",__LINE__,tVideoCapInitOptions.dwBufNum);
    tVideoCapInitOptions.dwFrameRate           = 0;
    tVideoCapInitOptions.dwDeviceNum           = ptMasterInfo->dwDevNum;

    if ((pfInput = fopen(_ptInitNeedInfo->szSensorConfigFile, "r")) == NULL) {
        printf("Open input configuration file %s fail !!\n", _ptInitNeedInfo->szSensorConfigFile);
        exit(1);
    }

	for (dwCounter = 0; dwCounter < 8; dwCounter++) {
		GetParam(pfInput); //ignore the first 8 field that doesn't used.
	}

    tVideoCapInitOptions.dwStartPixel          = GetParam(pfInput);		// line 9
    tVideoCapInitOptions.dwStartLine           = GetParam(pfInput);		// line 10

	GetParam(pfInput);	// ignore 11 field
	
	tVideoCapInitOptions.eFormat			   = GetParam(pfInput);		// line 12

	for (dwCounter = 0; dwCounter < 2; dwCounter++) {
		GetParam(pfInput); //ignore the 13~14 field that doesn't used.
	}

    tVideoCapInitOptions.bFlip                 = GetParam(pfInput);		// line 15
    tVideoCapInitOptions.bMirror               = GetParam(pfInput);		// line 16
    tVideoCapInitOptions.bFieldMode            = GetParam(pfInput);		// line 17
    tVideoCapInitOptions.dwBrightness          = (DWORD)(GetParam(pfInput)&0x000000FF);		// line 18
    tVideoCapInitOptions.dwContrast            = (DWORD)(GetParam(pfInput)&0x000000FF);		// line 19
    tVideoCapInitOptions.dwSaturation          = GetParam(pfInput);		// line 20
    tVideoCapInitOptions.bFieldInvert          = GetParam(pfInput);		// line 21
    tVideoCapInitOptions.pObjectMem            = NULL;


	for (dwCounter = 0; dwCounter < 135; dwCounter++) {
		tVideoCapInitOptions.adwConfig[dwCounter] = GetParam(pfInput);       
	}
	
    fclose(pfInput);
    	
    dwYFrameSize = tVideoCapInitOptions.dwMaxFrameWidth * tVideoCapInitOptions.dwMaxFrameHeight;
    dwCFrameSize = (tVideoCapInitOptions.eOutPixelFormat == YUV420)?(dwYFrameSize/4):(dwYFrameSize/2);
    dwFrameSize = dwYFrameSize + (dwCFrameSize << 1);

    if ((pbyCapBuf = (BYTE *)VencMaster_GetMemory(ptMasterInfo->hMemMgrObj,
    						_ptInitNeedInfo->dwVICOutputDRAMLoc,
                                              	dwFrameSize*tVideoCapInitOptions.dwBufNum,
                                              	ALIGN_TYPE_128_BYTE
                                              	))== NULL ) {
        printf("[VENC_MASTER] Allocate %u bytes video capture frame buffer fail !!\n", dwFrameSize*tVideoCapInitOptions.dwBufNum);
        return S_FAIL;
    } else {
        printf("[VENC_MASTER] Request %u bytes non-cached memory succeed !!\n", dwFrameSize*tVideoCapInitOptions.dwBufNum);
		//syslog(LOG_INFO, "[VENC_MEM] pbyCapBuf : %d bytes\n",  dwFrameSize*tVideoCapInitOptions.dwBufNum);
		
	}
    
    ptMasterInfo->pbyCapBuf = pbyCapBuf;
    
    tVideoCapInitOptions.pbyCapBufUsrBaseAddr = pbyCapBuf;
    
    
   // memset(pbyCapBuf, 0, (dwFrameSize<<2));
    memset(pbyCapBuf, 0, (dwFrameSize* tVideoCapInitOptions.dwBufNum));
 
#if   0 
    	// acer add for v3------------
	// init pbyStatBuf
	if ((pbyStatBuf = (BYTE *)VencMaster_GetMemory(ptMasterInfo->hMemMgrObj,
											  0,
											  544*8,
											  ALIGN_TYPE_128_BYTE
											  )) == NULL) {
		printf("[VENC_MASTER] Allocation 544*8 byte pbyStatBuf fail\n");
	} else {
		printf("[VENC_MASTER] Request 588*4 bytes non-cached memory succeed !!\n");
	}
	tVideoCapInitOptions.pbyStatBufUsrBaseAddr = pbyStatBuf;
	// acer add end ------------
#endif 
	  
	 //Jeff add this for V3
	{
		if ((ptMasterInfo->pbyStatAEWBBufUsrBaseAddr = (BYTE *)VencMaster_GetMemory(ptMasterInfo->hMemMgrObj,
											  0,
											  720*sizeof(DWORD),
											  ALIGN_TYPE_128_BYTE
											  )) == NULL) {
			printf("Allocate AEWB statistic buffer %d bytes fail !!\n", (int)720*sizeof(DWORD));	
		}
		
		tVideoCapInitOptions.pbyStatAEWBBufUsrBaseAddr = ptMasterInfo->pbyStatAEWBBufUsrBaseAddr;
		memset(ptMasterInfo->pbyStatAEWBBufUsrBaseAddr, 0, 720*sizeof(DWORD));
		if ((ptMasterInfo->pbyStatHistoBufUsrBaseAddr = (BYTE *)VencMaster_GetMemory(ptMasterInfo->hMemMgrObj,
											  0,
											  192*sizeof(DWORD),
											  ALIGN_TYPE_128_BYTE
											  )) == NULL) {
			printf("Allocate histo statistic buffer %d bytes fail !!\n", (int)192*sizeof(DWORD));
		}
		tVideoCapInitOptions.pbyStatHistoBufUsrBaseAddr = ptMasterInfo->pbyStatHistoBufUsrBaseAddr;
		memset(ptMasterInfo->pbyStatHistoBufUsrBaseAddr, 0, 192*sizeof(DWORD));
		if ((ptMasterInfo->pbyStatFocusBufUsrBaseAddr = (BYTE *)VencMaster_GetMemory(ptMasterInfo->hMemMgrObj,
											  0,
											  112*sizeof(DWORD),
											  ALIGN_TYPE_128_BYTE
											  )) == NULL) {
			printf("Allocate focus statistic buffer %d bytes fail !!\n", (int)112*sizeof(DWORD));
			
		}
		
		tVideoCapInitOptions.pbyStatFocusBufUsrBaseAddr = ptMasterInfo->pbyStatFocusBufUsrBaseAddr;
		memset(ptMasterInfo->pbyStatFocusBufUsrBaseAddr, 0, (7*7*2)*sizeof(DWORD));//8*7*2 -> 7*7*2 = 98  
	  
	}
	//Jeff add end
	if (VideoCap_Initial(&(ptMasterInfo->hVideoCapObject), &tVideoCapInitOptions) != S_OK) {
        printf("[VENC_MASTER] video_cap_initial err!\n");
        return S_FAIL;
    }
	// inital software AE, because we don't want to record tVideoCapInitOptions, so just initial here, not at venc_masterapp.c file.
    if (AutoExposure_Setting(ptMasterInfo, &tVideoCapInitOptions) != S_OK) {
        printf("[VENC_MASTER] Initial AutoExposure object fail !!\n");
        return S_FAIL;
    }

    if (VideoCap_Start(ptMasterInfo->hVideoCapObject) != S_OK) {
        printf("[VENC_MASTER] video_cap_start err!\n");
        return S_FAIL;
    }

#ifdef  _AF
    {
		#define AF_BIN_PATH "/etc/conf.d/AF.bin"
		TVideoSignalOptions tVideoSignalOptions;
		DWORD dwMemSize = 0;
		TMemMgrState tMemMgrState;
		BYTE *pbyAFAdjustBuf = NULL;
		FILE *pfAFInput = NULL;
	
		memset(&tMemMgrState,0x0,sizeof(TMemMgrState));
		memset(&tVideoSignalOptions,0x0,sizeof(TVideoSignalOptions));
		tVideoSignalOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_GET_AUTO_FOCUS_TABLE_SIZE;
		if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSignalOptions) != S_OK)
		{
			printf("[VENC_MASTER] call VIDEO_SIGNAL_OPTION_GET_AUTO_FOCUS_TABLE_SIZE  err!\n");
			return S_FAIL;
		}
		dwMemSize = tVideoSignalOptions.adwUserData[0];
		if (dwMemSize != 0)
		{
			tMemMgrState.dwBusNum = 0;
			tMemMgrState.dwSize = dwMemSize;
			tMemMgrState.eAlignType = ALIGN_TYPE_128_BYTE;
			if ((pbyAFAdjustBuf = (BYTE *)MemMgr_GetMemory(ptMasterInfo->hMemMgrObj, &tMemMgrState)) == NULL)
			{
				printf("[VENC_MASTER] Allocate AF adjust buffer %d fail !!\n", (int)(dwMemSize));
				return S_FAIL;
			}
			ptMasterInfo->pbyAFAdjustBuf = pbyAFAdjustBuf;
			memset(pbyAFAdjustBuf, 0, dwMemSize);

			if ((pfAFInput = fopen(AF_BIN_PATH, "rb")) != NULL)
			{
				// obtain file size:
				fseek (pfAFInput , 0 , SEEK_END);
				if (ftell(pfAFInput) == dwMemSize)
				{
					rewind (pfAFInput);
					fread(pbyAFAdjustBuf,1,dwMemSize,pfAFInput);
					printf("[VENC_MASTER] File Size is  %lu!!\n", ftell(pfAFInput));
				}
				else
				{
					printf("[VENC_MASTER] File Size is incorrect !!\n");
				}
				fclose(pfAFInput);
			}
			else
			{
				printf("[VENC_MASTER] Open %s fail !!\n", AF_BIN_PATH);
			}
			memset(&tVideoSignalOptions,0x0,sizeof(TVideoSignalOptions));
			tVideoSignalOptions.eOptionFlags = VIDEO_SIGNAL_OPTION_SET_AUTO_FOCUS_TABLE;
			tVideoSignalOptions.adwUserData[0] = (DWORD)(pbyAFAdjustBuf);
			if (VideoCap_SetOptions(ptMasterInfo->hVideoCapObject, &tVideoSignalOptions) != S_OK)
			{
				printf("[VENC_MASTER]  call VIDEO_SIGNAL_OPTION_SET_AUTO_FOCUS_TABLE err!\n");
				return S_FAIL;
			}
		}
    }
#endif
    return S_OK;
}

/* ============================================================================================= */
SCODE XMLWrapper_Setting(TMasterInfo *ptMasterInfo)
{
    TXmlWrapperInitOptions txpInitOptions;

    txpInitOptions.dwVersion = XMLWRAPPER_VERSION;
    if (XmlWrapper_Initial(&(ptMasterInfo->hXMLWrapperObj), &txpInitOptions) != S_OK) {
        fprintf(stderr, "[VENC_MASTER] Initial XMLWrapper object fail !! \n");
        return S_FAIL;
    }
    if (XmlWrapper_SetHandler(ptMasterInfo->hXMLWrapperObj, ptMasterTreeMap,NULL) != S_OK) {
        fprintf(stderr, "[VENC_MASTER] Set XMLWrapper handler fail !! \n");
        return S_FAIL;
    }
    return S_OK;
}

/* ============================================================================================= */
SCODE VencMaster_InitVML(TMasterInfo *ptMasterInfo)
{
    // Initial video capture instance
    if (VideoCap_Setting(ptMasterInfo, &(ptMasterInfo->_tInitNeedInfo)) != S_OK ) {
        DBPRINT0("[VENC_MASTER] Initial video_cap object fail !!\n");
        return S_FAIL;
    }

    // Initial XMLWrapper
	if (XMLWrapper_Setting(ptMasterInfo) != S_OK) {	
        DBPRINT0("[VENC_MASTER] Initial xml wrapper object fail !!\n");
		return S_FAIL;
	}
	
	return S_OK;
}

/* ============================================================================================= */
SCODE VencMaster_ReleaseVML(TMasterInfo *ptMasterInfo)
{
    VideoCap_Stop(ptMasterInfo->hVideoCapObject);

    VideoCap_Release(&(ptMasterInfo->hVideoCapObject));
    XmlWrapper_Release(&(ptMasterInfo->hXMLWrapperObj));
#ifdef  _AF
	if (ptMasterInfo->pbyAFAdjustBuf != NULL)
    {
      if (MemMgr_FreeMemory(ptMasterInfo->hMemMgrObj, (DWORD)ptMasterInfo->pbyAFAdjustBuf) != S_OK)
      {
		printf("[VENC_MASTER](%d) call MemMgr_FreeMemory() to release %p falied!!\n",__LINE__,ptMasterInfo->pbyAFAdjustBuf);
		ptMasterInfo->pbyAFAdjustBuf = NULL;
      }
    }
#endif
	if (ptMasterInfo->pbyCapBuf != NULL)
    {
      if (MemMgr_FreeMemory(ptMasterInfo->hMemMgrObj, (DWORD)ptMasterInfo->pbyCapBuf) != S_OK)
      {
		  printf("[VENC_MASTER](%d) call MemMgr_FreeMemory() to release %p falied!!\n",__LINE__,ptMasterInfo->pbyCapBuf);
      }
      else
      {
		ptMasterInfo->pbyCapBuf = NULL;
      } 
   	}
	
	if (ptMasterInfo->pbyStatAEWBBufUsrBaseAddr != NULL)
    {
      if (MemMgr_FreeMemory(ptMasterInfo->hMemMgrObj, (DWORD)ptMasterInfo->pbyStatAEWBBufUsrBaseAddr) != S_OK)
      {
		  printf("[VENC_MASTER](%d) call MemMgr_FreeMemory() to release %p falied!!\n",__LINE__,ptMasterInfo->pbyStatAEWBBufUsrBaseAddr);
      }
      else
      {
		  ptMasterInfo->pbyStatAEWBBufUsrBaseAddr = NULL;
      }
    }
	
	if (ptMasterInfo->pbyStatHistoBufUsrBaseAddr != NULL)
    {
      if (MemMgr_FreeMemory(ptMasterInfo->hMemMgrObj, (DWORD)ptMasterInfo->pbyStatHistoBufUsrBaseAddr) != S_OK)
      {
		  printf("[VENC_MASTER](%d) call MemMgr_FreeMemory() to release %p falied!!\n",__LINE__,ptMasterInfo->pbyStatHistoBufUsrBaseAddr);
      }
      else
      {
		  ptMasterInfo->pbyStatHistoBufUsrBaseAddr = NULL;
      }
     
    }
	
	if (ptMasterInfo->pbyStatFocusBufUsrBaseAddr != NULL)
    {
      if (MemMgr_FreeMemory(ptMasterInfo->hMemMgrObj, (DWORD)ptMasterInfo->pbyStatFocusBufUsrBaseAddr) != S_OK)
      {
		  printf("[VENC_MASTER](%d) call MemMgr_FreeMemory() to release %p falied!!\n",__LINE__,ptMasterInfo->pbyStatFocusBufUsrBaseAddr);
      }
      else
      {	
		  ptMasterInfo->pbyStatFocusBufUsrBaseAddr = NULL;
      }
     
    }
    
	return S_OK;
}
