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
 * venc_encoder_svc.c
 *
 * \brief
 * SVC relative function implement.
 * 
 * \date
 * 2012/09/17
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#include "venc_encoder_local.h"

#ifdef _SVC
/* ========================================================================== */
void VencEncoder_SVCApplyOpt(HANDLE hObject)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
    SCODE scRet;
	TSVCOptRec *ptSVCOptRec = &(ptEncoderInfo->tSVCOptRec);
	TSVCEncOptions tSVCEncOpt;    

	if (ptSVCOptRec->bQualityGroup == TRUE) {
		if (ptSVCOptRec->dwBitrate > 0)
		{
			memset(&tSVCEncOpt,0x0,sizeof(TSVCEncOptions));
			tSVCEncOpt.eOptionFlags = VIDEO_CHANGE_BITRATE;
			tSVCEncOpt.adwUserData[0] = ptSVCOptRec->dwBitrate;
			if (ptEncoderInfo->hSVCEncObj != NULL)
			{
				scRet = SVCEnc_SetOptions(ptEncoderInfo->hSVCEncObj, &tSVCEncOpt);
				assert(scRet == S_OK);
				DBPRINT1("[VENC_ENCODER] Set SVC Bitrate %u\n", tSVCEncOpt.adwUserData[0]);
			}
			memset(&tSVCEncOpt,0x0,sizeof(TSVCEncOptions));
			tSVCEncOpt.eOptionFlags = VIDEO_SET_RATE_CONTROL; 
			tSVCEncOpt.adwUserData[0] = ptSVCOptRec->ercfSVCQualityType;
			if (ptEncoderInfo->hSVCEncObj != NULL)
			{
				scRet = SVCEnc_SetOptions(ptEncoderInfo->hSVCEncObj, &tSVCEncOpt);
				assert(scRet == S_OK);
				DBPRINT1("[VENC_ENCODER] Set SVC RateCtrl: %d\n", tSVCEncOpt.adwUserData[0]);
			}
		}
		ptSVCOptRec->bBitrate = FALSE;
		ptSVCOptRec->bSetRateControl = FALSE;
		memset(&tSVCEncOpt,0x0,sizeof(TSVCEncOptions));
		tSVCEncOpt.eOptionFlags = VIDEO_CHANGE_QUANT;
		tSVCEncOpt.adwUserData[0] = ptSVCOptRec->dwQuant;
		tSVCEncOpt.adwUserData[1] = 51;
		tSVCEncOpt.adwUserData[2] = 0;
		if (ptEncoderInfo->hSVCEncObj != NULL)
		{
			scRet = SVCEnc_SetOptions(ptEncoderInfo->hSVCEncObj, &tSVCEncOpt);
			assert(scRet == S_OK);
			DBPRINT1("[VENC_ENCODER] Set SVC Quant %u\n",tSVCEncOpt.adwUserData[0]);
		}
		ptSVCOptRec->bQuant = FALSE;
		ptSVCOptRec->bQualityGroup = FALSE;
	}

	if (ptSVCOptRec->bForceIntra == TRUE) {
		memset(&tSVCEncOpt,0x0,sizeof(TSVCEncOptions));
		tSVCEncOpt.eOptionFlags = VIDEO_FORCE_INTRA;
		if (ptEncoderInfo->hSVCEncObj != NULL)
		{
			scRet = SVCEnc_SetOptions(ptEncoderInfo->hSVCEncObj, &tSVCEncOpt);
			assert(scRet == S_OK);
			DBPRINT0("[VENC_ENCODER] Set SVC ForceIntra\n");
		}
		ptSVCOptRec->bForceIntra = FALSE;

	}

	if (ptSVCOptRec->bIntraInterval == TRUE) {
		// call force intra first
		memset(&tSVCEncOpt,0x0,sizeof(TSVCEncOptions));
		tSVCEncOpt.eOptionFlags = VIDEO_FORCE_INTRA;
		if (ptEncoderInfo->hSVCEncObj != NULL)
		{
			scRet = SVCEnc_SetOptions(ptEncoderInfo->hSVCEncObj, &tSVCEncOpt);
			assert(scRet == S_OK);
			DBPRINT0("[VENC_ENCODER] Set SVC ForceIntra\n");
		}
		// call change interval
		memset(&tSVCEncOpt,0x0,sizeof(TSVCEncOptions));
		tSVCEncOpt.eOptionFlags = VIDEO_CHANGE_INTRA_INTERVAL;        
		tSVCEncOpt.adwUserData[0] = ptSVCOptRec->dwIntraInterval;
		if (ptEncoderInfo->hSVCEncObj != NULL)
		{  
			scRet = SVCEnc_SetOptions(ptEncoderInfo->hSVCEncObj, &tSVCEncOpt);
			assert(scRet == S_OK);
			DBPRINT1("[VENC_ENCODER] Set SVC IntraInterval %u\n",tSVCEncOpt.adwUserData[0]);

		}		
		ptSVCOptRec->bIntraInterval = FALSE;
	}

	if (ptSVCOptRec->bSetHDREnable == TRUE)
	{
		memset(&tSVCEncOpt,0x0,sizeof(TSVCEncOptions));
		tSVCEncOpt.eOptionFlags = VIDEO_CHANGE_SCALABILITY_HEADER_EN;
		tSVCEncOpt.adwUserData[0] = ptSVCOptRec->bHDREnable;
		if (ptEncoderInfo->hSVCEncObj != NULL)
		{
			scRet = SVCEnc_SetOptions(ptEncoderInfo->hSVCEncObj, &tSVCEncOpt);
			assert(scRet == S_OK);
			printf("[VENC_ENCODER_PROCESS](%d) HDR enable=%d scRet=%s \n",__LINE__,tSVCEncOpt.adwUserData[0],(scRet == S_OK)?"OK":"FAIL");

		}
		ptSVCOptRec->bSetHDREnable = FALSE;
		// DBPRINT1("[VENC_ENCODER] Set SVC HDR enable %s\n",tSVCEncOpt.adwUserData[0]?"TRUE":"FALSE");
	}

	if (ptSVCOptRec->bSetLayerNum == TRUE)
	{
		memset(&tSVCEncOpt,0x0,sizeof(TSVCEncOptions));
		tSVCEncOpt.eOptionFlags = VIDEO_CHANGE_SCALABILITY_TEMPORAL_LAYER_NUM;
		tSVCEncOpt.adwUserData[0] = ptSVCOptRec->dwLayerNum;
		if (ptEncoderInfo->hSVCEncObj != NULL)
		{
			scRet = SVCEnc_SetOptions(ptEncoderInfo->hSVCEncObj, &tSVCEncOpt);
			assert(scRet == S_OK);
			printf("[VENC_ENCODER_PROCESS](%d) dwLayerNum=%d scRet=%s \n",__LINE__,ptSVCOptRec->dwLayerNum,(scRet == S_OK)?"OK":"FAIL");

		}
		ptSVCOptRec->bSetLayerNum = FALSE;
#if 0
		ptEncoderInfo->bForceCI = TRUE;
		printf("[VENC_ENCODER_PROCESS](%d)Tell the rtspserver the svc num has been changed!!(ptEncoderInfo->bForceCI=%d)\n",__LINE__,ptEncoderInfo->bForceCI);
#endif
		//printf("[VENC_ENCODER_PROCESS](%d) Set SVC layer num %u\n",__LINE__,tSVCEncOpt.adwUserData[0]);
	}
	return;
}

/* ========================================================================== */
void VencEncoder_SVCForceCI(HANDLE hObject, EVideoFormat vFormat)
{
#define BYTESTREAM_HEADER_LEN 0
     TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
	HANDLE hSVCEncObj = ptEncoderInfo->hSVCEncObj;
	TSharedBuffMgrState tSharedBuffMgrState;
	TUBufferConfSVC *ptubcSVC;
	BYTE	*pbyHdrAddr;
	SCODE 	scResult = S_FAIL;
	fd_set               	fdsWrite;
	struct timeval 			timeout;
	int						fdShardBuffer;
	int     				iResult;

	WORD	wSeqByte; 
	WORD	wPicByte;

	BYTE abySPSBuf[128];
	BYTE abyPPSBuf[128];

	memset(abySPSBuf, 0, sizeof(abySPSBuf));
	memset(abyPPSBuf, 0, sizeof(abyPPSBuf));
	if (!hSVCEncObj)
		return;
	ptEncoderInfo->pfnGetOutputBufferFD(ptEncoderInfo->hOutputSrdObj, &fdShardBuffer);
	if (ptEncoderInfo->pfnRequestOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState) != S_OK)
	{
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		FD_ZERO(&fdsWrite);
		FD_SET(fdShardBuffer, &fdsWrite);

		iResult = select(fdShardBuffer + 1, NULL, &fdsWrite, NULL, &timeout);

		if (iResult <= 0) {
			DBPRINT0("SVC select timeout.... \n");
		}
		if (!FD_ISSET(fdShardBuffer, &fdsWrite))
			return;
		if (ptEncoderInfo->pfnRequestOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState) != S_OK)
			return;
	}

	ptubcSVC = (TUBufferConfSVC *)(tSharedBuffMgrState.pbyHdrAddr);
	pbyHdrAddr = (BYTE*)(tSharedBuffMgrState.pbyHdrAddr);
	memset(ptubcSVC, 0, sizeof(TUBufferConfSVC));

	wSeqByte = SHAREDBUFFER_HEADERSIZE - sizeof(TUBufferConfSVC);
	scResult = SVCEnc_SpecificConfig_SPS(hSVCEncObj, abySPSBuf, &wSeqByte);
	assert(scResult == S_OK);
	wSeqByte -= sizeof(DWORD); // because ByteStreamHeader is 4 bytes
	memcpy((pbyHdrAddr + sizeof(TUBufferConfSVC)), &wSeqByte, sizeof(WORD));
	memcpy((pbyHdrAddr + sizeof(TUBufferConfSVC) + sizeof(WORD)), abySPSBuf+sizeof(DWORD), wSeqByte);

	wPicByte = SHAREDBUFFER_HEADERSIZE - sizeof(TUBufferConfSVC) - sizeof(WORD) - wSeqByte;
	scResult = SVCEnc_SpecificConfig_PPS(hSVCEncObj, abyPPSBuf, &wPicByte);
	assert(scResult == S_OK);
	wPicByte -= sizeof(DWORD);
	memcpy((pbyHdrAddr + sizeof(TUBufferConfSVC) + sizeof(WORD) + wSeqByte), &wPicByte, sizeof(WORD));
	memcpy((pbyHdrAddr + sizeof(TUBufferConfSVC) + sizeof(WORD) + wSeqByte + sizeof(WORD)), abyPPSBuf+sizeof(DWORD), wPicByte);

	ptubcSVC->dwSize = sizeof(TUBufferConfSVC) + sizeof(WORD) + wSeqByte + sizeof(WORD) + wPicByte;
	ptubcSVC->dwDataType = FOURCC_CONF;
	ptubcSVC->dwFollowingDataType = FOURCC_H264;
	ptubcSVC->dwTrackID = 1;
	ptubcSVC->dwWidth  = g_avfInfo[vFormat].wEncWidth;
	ptubcSVC->dwHeight = g_avfInfo[vFormat].wEncHeight;
	ptubcSVC->bSVCTEn = ptEncoderInfo->tSVCOptRec.bHDREnable;
	ptubcSVC->dwSVCTLayerNum = ptEncoderInfo->tSVCOptRec.dwLayerNum;
	// it should Get abyProfileLevelId from H264 library API 
	ptubcSVC->abyProfileLevelId[0] = *(pbyHdrAddr + sizeof(TUBufferConfSVC) + sizeof(WORD) + 1);
	ptubcSVC->abyProfileLevelId[1] = *(pbyHdrAddr + sizeof(TUBufferConfSVC) + sizeof(WORD) + 2)&0xE0;
	ptubcSVC->abyProfileLevelId[2] = *(pbyHdrAddr + sizeof(TUBufferConfSVC) + sizeof(WORD) + 3);

	ptEncoderInfo->pfnReleaseOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState);
}

/* ========================================================================== */
void VencEncoder_SVCEncode(HANDLE hObject, TVideoFrameBuffer *ptVFB)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
	int						iEncCnt;
	struct timeval 			timeout;
	int						fdShardBuffer;
	int     				iResult;
	fd_set              	fdsWrite;
	int						i;
	// for Streaming server
	TSharedBuffMgrState    tSharedBuffMgrState;

	// SVC Encoder stuff
	HANDLE                 hSVCEncObj = ptEncoderInfo->hSVCEncObj;
	TSVCEncOptions        tSVCEncOpt;    
	TSVCEncState          tSVCEncState;
	SCODE                  scResult;
	BYTE                   *pbyBuffer;
	TUBuffer 			   *ptubBuff;
	DWORD					dwFrameBytes = 0;
	DWORD					dwNALSize;
	DWORD					dwTtlSize = 0;

	iEncCnt = 0;
	if (!hSVCEncObj)
		return;

	tSVCEncOpt.eOptionFlags    = VIDEO_CHANGE_FRAME_BUFFER;
	tSVCEncOpt.adwUserData[0]  = (DWORD)ptVFB->pbyBufY;
	tSVCEncOpt.adwUserData[1]  = (DWORD)ptVFB->pbyBufU;
	tSVCEncOpt.adwUserData[2]  = (DWORD)ptVFB->pbyBufV;
	if (SVCEnc_SetOptions(ptEncoderInfo->hSVCEncObj, &tSVCEncOpt) != S_OK)
		return;

	ptEncoderInfo->pfnGetOutputBufferFD(ptEncoderInfo->hOutputSrdObj, &fdShardBuffer);
	if (ptEncoderInfo->pfnRequestOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState) != S_OK)
	{
		timeout.tv_sec = 0;
		timeout.tv_usec = 10000;
		FD_ZERO(&fdsWrite);
		FD_SET(fdShardBuffer, &fdsWrite);
		iResult = select(fdShardBuffer + 1, NULL, &fdsWrite, NULL, &timeout);
		if (iResult <= 0) {
			DBPRINT0("SVC select timeout.... \n");
			return;
		}
		if (!FD_ISSET(fdShardBuffer, &fdsWrite))
			return;
		if (ptEncoderInfo->pfnRequestOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState) != S_OK)
			return;
	}

	while(iEncCnt < 3) {
		//set output buffer to H264 encoder
		memset(&tSVCEncOpt,0x0,sizeof(TSVCEncOptions ));
		tSVCEncOpt.eOptionFlags = VIDEO_CHANGE_BITSTREAM_BUFFER;
		tSVCEncOpt.adwUserData[0] = (DWORD)(tSharedBuffMgrState.pbyDataAddr);
		tSVCEncOpt.adwUserData[1] = ptEncoderInfo->_tInitNeedInfo.dwBitStreamSize;
		scResult = SVCEnc_SetOptions(hSVCEncObj, &tSVCEncOpt);
		assert(scResult == S_OK);

		//time of SVC video
		memset(&tSVCEncState, 0, sizeof(TSVCEncState)); 
		tSVCEncState.dwSeconds = ptVFB->dwSecond;
		tSVCEncState.dwTicks = (ptVFB->dwMilliSecond * ptEncoderInfo->dwSVCTimeResolution / 1000)+(ptVFB->dwMicroSecond * ptEncoderInfo->dwSVCTimeResolution / 1000000);
		tSVCEncState.dwEncUserDataLength = 0;

		scResult = SVCEnc_OneFrame(hSVCEncObj, &tSVCEncState);
		if (scResult == S_OK) {
#ifdef _DUMP_ENCODER
			if (tSVCEncState.bIDR == TRUE) {
				ptEncoderInfo->bFirstIntra = TRUE;
			}
			if (ptEncoderInfo->bFirstIntra == TRUE) {
				printf("[venc_encoder_process](%d)Enter Dump bitstream...\n", __LINE__);
				fwrite(tSharedBuffMgrState.pbyDataAddr, sizeof(BYTE), tSVCEncState.dwEncBytes, ptEncoderInfo->pfSVCOutput);
				printf("[venc_encoder_process](%d)Leave Dump bitstream...\n", __LINE__);

			}
#endif // _DUMP_ENCODER
			dwFrameBytes = tSVCEncState.dwEncBytes;

			ptubBuff = (TUBuffer *)(tSharedBuffMgrState.pbyHdrAddr);
			memset(ptubBuff, 0, sizeof(TUBuffer));
			pbyBuffer = (BYTE *)(tSharedBuffMgrState.pbyHdrAddr);

			if ((dwFrameBytes > 0) && (tSVCEncState.dwNALCount > 0)) {
				ptubBuff->dwDataType = FOURCC_H264;
				ptubBuff->dwTrackID = 1;
				ptubBuff->dwSec = ptVFB->dwSecond;
				ptubBuff->dwUSec = ptVFB->dwMilliSecond * 1000+ptVFB->dwMicroSecond;
				ptubBuff->dwSeqNo = ptEncoderInfo->dwSVCSeqNo++;
				ptubBuff->bIsSync = tSVCEncState.bIDR;
				ptubBuff->bIsBoundary = TRUE;       
				ptubBuff->dwUserDataSize = 0;
				ptubBuff->dwSize = sizeof(TUBuffer) + dwFrameBytes;
				for(i = 0; i < tSVCEncState.dwNALCount; i++) {
					dwNALSize = tSVCEncState.adwNALBytes[i]-sizeof(DWORD);
					memcpy(tSharedBuffMgrState.pbyDataAddr+dwTtlSize, &dwNALSize, sizeof(DWORD));
					dwTtlSize += dwNALSize+sizeof(DWORD);
				}
				break;
			} else {
				printf("dwFrameBytes <= 0 or tSVCEncState.dwNALCount <= 0, skip this frame !!\n");
				break;
			}
		} else {
			printf("[VENC_ENCODER] Error encoding SVC video %d, #%d\n", scResult, iEncCnt);
			iEncCnt++;
			// change Quant to that it can encode
			memset(&tSVCEncOpt,0x0,sizeof(TSVCEncOptions ));
			tSVCEncOpt.eOptionFlags = VIDEO_CHANGE_QUANT;
			tSVCEncOpt.adwUserData[0] = ptEncoderInfo->tSVCOptRec.dwQuant + iEncCnt*2;
			tSVCEncOpt.adwUserData[1] = 51;
			tSVCEncOpt.adwUserData[2] = 0;
			scResult = SVCEnc_SetOptions(hSVCEncObj, &tSVCEncOpt);
			assert(scResult == S_OK);		        		
		}
	} // end while(iEncCnt < 3)
	ptEncoderInfo->pfnReleaseOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState);
	// restore the original settings
	if (iEncCnt != 0) {
		// change quant value
		tSVCEncOpt.eOptionFlags = VIDEO_CHANGE_QUANT;
		tSVCEncOpt.adwUserData[0] = ptEncoderInfo->tSVCOptRec.dwQuant;
		tSVCEncOpt.adwUserData[1] = 51;
		tSVCEncOpt.adwUserData[2] =0;
		scResult = SVCEnc_SetOptions(hSVCEncObj, &tSVCEncOpt);
		assert(scResult == S_OK);
	}
}

/* ========================================================================== */
void VencEncoder_SVCRestart(HANDLE hObject)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
    TSVCEncOptions tSVCEncOpt;

    tSVCEncOpt.eOptionFlags = VIDEO_RESTART;
    if (ptEncoderInfo->hSVCEncObj != NULL) {
        SVCEnc_SetOptions(ptEncoderInfo->hSVCEncObj, &tSVCEncOpt);
        printf("[VENC_ENCODER_SVC](%d) restart SVC encoder!\n",__LINE__);
    }
}
/* ========================================================================== */
void VencEncoder_SVCChangeFrameSize(HANDLE hObject)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
    TSVCEncOptions tSVCEncOpt;
    SCODE scRet;

    memset(&tSVCEncOpt, 0x0, sizeof(TSVCEncOptions));
    tSVCEncOpt.eOptionFlags = VIDEO_CHANGE_FRAME_SIZE;
    tSVCEncOpt.adwUserData[0] = g_avfInfo[ptEncoderInfo->vfCurtFormat].wEncWidth;
    tSVCEncOpt.adwUserData[1] = g_avfInfo[ptEncoderInfo->vfCurtFormat].wEncHeight;
    if (ptEncoderInfo->hSVCEncObj != NULL) {
        scRet = SVCEnc_SetOptions(ptEncoderInfo->hSVCEncObj, &tSVCEncOpt);
        assert(scRet == S_OK);
        printf("Change SVC Encode size %u,%u\n", tSVCEncOpt.adwUserData[0], tSVCEncOpt.adwUserData[1]);
    }
}

/* ========================================================================== */
void VencEncoder_SVCChangeInFrameSize(HANDLE hObject, DWORD dwStride)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
    TSVCEncOptions tSVCEncOpt;
    SCODE scRet;

    memset(&tSVCEncOpt, 0x0, sizeof(TSVCEncOptions));
    tSVCEncOpt.eOptionFlags = VIDEO_CHANGE_IN_FRAME_SIZE;
    tSVCEncOpt.adwUserData[0] = dwStride;
    tSVCEncOpt.adwUserData[1] = g_avfInfo[ptEncoderInfo->vfCurtFormat].wEncHeight;
    if (ptEncoderInfo->hSVCEncObj != NULL) {
        scRet = SVCEnc_SetOptions(ptEncoderInfo->hSVCEncObj, &tSVCEncOpt);
        printf("Change SVC Input size %u,%u\n", tSVCEncOpt.adwUserData[0], tSVCEncOpt.adwUserData[1]);
        assert(scRet == S_OK);
    }
}

#endif // _SVC
