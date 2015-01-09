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
 * venc_encoder_mp4v.c
 *
 * \brief
 * MP4V relative function implement.
 * 
 * \date
 * 2012/09/14
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#include "venc_encoder_local.h"

#ifdef _VMA_MEBE
/* ========================================================================== */
void VencEncoder_Mp4VApplyOpt(HANDLE hObject)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
	SCODE scRet;
	TMp4VOptRec *ptMp4VOptRec = &(ptEncoderInfo->tMp4VOptRec);
	TMp4VEncOptions tMp4VEncOpt;    

	if (ptMp4VOptRec->bQualityGroup == TRUE) {
		if (ptMp4VOptRec->dwBitrate > 0)
		{
			tMp4VEncOpt.eOptionFlags = VIDEO_CHANGE_BITRATE;
			tMp4VEncOpt.adwUserData[0] = ptMp4VOptRec->dwBitrate;
			if (ptEncoderInfo->hMp4VEncObj != NULL)
			{
				scRet = Mp4VEnc_SetOptions(ptEncoderInfo->hMp4VEncObj, &tMp4VEncOpt);
				assert(scRet == S_OK);

				DBPRINT1("[VENC_ENCODER] Set MPEG4 Bitrate %u\n", tMp4VEncOpt.adwUserData[0]);
			}

			tMp4VEncOpt.eOptionFlags = VIDEO_SET_RATE_CONTROL; 
			tMp4VEncOpt.adwUserData[0] = ptMp4VOptRec->ercfMPEG4QualityType;
			if (ptEncoderInfo->hMp4VEncObj!=NULL)
			{
				scRet = Mp4VEnc_SetOptions(ptEncoderInfo->hMp4VEncObj, &tMp4VEncOpt);
				assert(scRet == S_OK);

				DBPRINT1("[VENC_ENCODER] Set MPEG4 RateCtrl: %d\n", tMp4VEncOpt.adwUserData[0]);
			}
		}
		ptMp4VOptRec->bBitrate = FALSE;
		ptMp4VOptRec->bSetRateControl = FALSE;

		//	    if (ptMp4VOptRec->bQuant == TRUE) { // Venc_SlaveApp_SetMPEG4Quant
		tMp4VEncOpt.eOptionFlags = VIDEO_CHANGE_QUANT;        
		tMp4VEncOpt.adwUserData[0] = ptMp4VOptRec->dwQuant;
		if (ptEncoderInfo->hMp4VEncObj != NULL) 
		{
			scRet = Mp4VEnc_SetOptions(ptEncoderInfo->hMp4VEncObj, &tMp4VEncOpt);
			assert(scRet == S_OK);

			DBPRINT1("[VENC_ENCODER] Set MPEG4 Quant %u\n",tMp4VEncOpt.adwUserData[0]);
		}
		ptMp4VOptRec->bQuant = FALSE;
		//	    }
		ptMp4VOptRec->bQualityGroup = FALSE;
	}

	if (ptMp4VOptRec->bForceIntra == TRUE) {
		tMp4VEncOpt.eOptionFlags = VIDEO_FORCE_INTRA;
		if (ptEncoderInfo->hMp4VEncObj != NULL)
		{
			scRet = Mp4VEnc_SetOptions(ptEncoderInfo->hMp4VEncObj, &tMp4VEncOpt);
			assert(scRet == S_OK);
			DBPRINT0("[VENC_ENCODER] Set MPEG4 ForceIntra\n");
		}
		ptMp4VOptRec->bForceIntra = FALSE;
	}

	if (ptMp4VOptRec->bIntraInterval == TRUE) {
		// call force intra first
		tMp4VEncOpt.eOptionFlags = VIDEO_FORCE_INTRA;
		if (ptEncoderInfo->hMp4VEncObj != NULL)
		{
			scRet = Mp4VEnc_SetOptions(ptEncoderInfo->hMp4VEncObj, &tMp4VEncOpt);
			assert(scRet == S_OK);
			DBPRINT0("[VENC_ENCODER] Set MPEG4 ForceIntra\n");
		}
		// call change interval
		tMp4VEncOpt.eOptionFlags = VIDEO_CHANGE_INTRA_INTERVAL;        
		tMp4VEncOpt.adwUserData[0] = ptMp4VOptRec->dwIntraInterval;
		if (ptEncoderInfo->hMp4VEncObj != NULL)
		{
			scRet = Mp4VEnc_SetOptions(ptEncoderInfo->hMp4VEncObj, &tMp4VEncOpt);
			assert(scRet == S_OK);
			DBPRINT1("[VENC_ENCODER] Set MPEG4 IntraInterval %u\n",tMp4VEncOpt.adwUserData[0]);
		}
		ptMp4VOptRec->bIntraInterval = FALSE;
	}
}

/* ========================================================================== */
void VencEncoder_Mp4VForceCI(HANDLE hObject, EVideoFormat vFormat)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
	HANDLE hMp4VEncObj = ptEncoderInfo->hMp4VEncObj;
	DWORD dwSpecConfSize, dwProfileLevel;
	TSharedBuffMgrState tSharedBuffMgrState;
	TUBufferConfMP4V *ptubcMP4V ;//= (TUBufferConfMP4V *)(ptEncoderInfo->pbyBuffer);  
	BYTE* pbyBuffer;
	fd_set               	fdsWrite;
	struct timeval 			timeout;
	int						fdShardBuffer;
	int     				iResult;
	SCODE 	scResult = S_FAIL;
	if (!hMp4VEncObj)
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
			DBPRINT0("Mp4V select timeout.... \n");
			return;
		}
		if (!FD_ISSET(fdShardBuffer, &fdsWrite))
			return;
		if (ptEncoderInfo->pfnRequestOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState) != S_OK)
			return;
	}

	ptubcMP4V = (TUBufferConfMP4V *)(tSharedBuffMgrState.pbyHdrAddr);
	pbyBuffer = (BYTE*)tSharedBuffMgrState.pbyHdrAddr;
	memset(ptubcMP4V, 0, sizeof(TUBufferConfMP4V));

	scResult = Mp4VEnc_SpecificConfig(hMp4VEncObj, 
			pbyBuffer + sizeof(TUBufferConfMP4V), 
			&dwSpecConfSize, &dwProfileLevel,
			32 * 1024 - sizeof(TUBufferConfMP4V));
	assert(scResult == S_OK);
	ptubcMP4V->dwSize = sizeof(TUBufferConfMP4V) + dwSpecConfSize;
	ptubcMP4V->dwDataType = FOURCC_CONF;
	ptubcMP4V->dwFollowingDataType = FOURCC_MP4V;    
	ptubcMP4V->dwTrackID = 1;
	ptubcMP4V->dwWidth  = g_avfInfo[vFormat].wEncWidth;            
	ptubcMP4V->dwHeight = g_avfInfo[vFormat].wEncHeight;
	ptubcMP4V->dwProfileLevel = dwProfileLevel;

	ptEncoderInfo->pfnReleaseOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState);

	printf("header size %u, DecSpec size %u\n", sizeof(TUBufferConfMP4V), dwSpecConfSize);
	printf("spec size %u %u \n", ptubcMP4V->dwWidth, ptubcMP4V->dwHeight);
}

/* ========================================================================== */
void VencEncoder_Mp4VEncode(HANDLE hObject, TVideoFrameBuffer *ptVFB)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
	int						iEncCnt;
	struct timeval 			timeout;
	int						fdShardBuffer;
	int     				iResult;
	fd_set              	fdsWrite;
	WORD					wHdrTag = 0;
	WORD					wHdrLen = 0;
	// for Streaming server
	TSharedBuffMgrState    	tSharedBuffMgrState;

	// Mp4 Encoder stuff
	HANDLE                 	hMp4VEncObj = ptEncoderInfo->hMp4VEncObj;
	TMp4VEncOptions        	tMP4VEncOpt;
	TMp4VEncState          	tMp4VEncState;

	SCODE                  	scResult;
	BYTE                   	*pbyBuffer;
	TUBuffer 				*ptubBuff;

	DWORD                  	dwFrameBytes = 0;
	DWORD                  	dwPacketBytes = 0;
	int						i;

	iEncCnt = 0;
	if (!hMp4VEncObj)
		return;

	tMP4VEncOpt.eOptionFlags    = VIDEO_CHANGE_FRAME_BUFFER;
	tMP4VEncOpt.adwUserData[0]  = (DWORD)ptVFB->pbyBufY;
	tMP4VEncOpt.adwUserData[1]  = (DWORD)ptVFB->pbyBufU;
	tMP4VEncOpt.adwUserData[2]  = (DWORD)ptVFB->pbyBufV;
	if (Mp4VEnc_SetOptions(hMp4VEncObj, &tMP4VEncOpt) != S_OK)
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
			printf("MP4V select timeout.... \n");
		}

		if (!FD_ISSET(fdShardBuffer, &fdsWrite))
			return;
		if (ptEncoderInfo->pfnRequestOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState) != S_OK)
			return;
	}
	while(iEncCnt < 3) {
		// set output bitstream buffer to mpeg4 encoder
		tMP4VEncOpt.eOptionFlags = VIDEO_CHANGE_BITSTREAM_BUFFER;
		tMP4VEncOpt.adwUserData[0] = (DWORD)(tSharedBuffMgrState.pbyDataAddr);
		tMP4VEncOpt.adwUserData[1] = ptEncoderInfo->_tInitNeedInfo.dwBitStreamSize;

		assert(tMP4VEncOpt.adwUserData[1]>12000);
		scResult = Mp4VEnc_SetOptions(hMp4VEncObj, &tMP4VEncOpt);
		assert(scResult == S_OK);   

		//time of mpeg4 video
		memset(&tMp4VEncState, 0, sizeof(TMp4VEncState)); 
		tMp4VEncState.dwSeconds = ptVFB->dwSecond;
		tMp4VEncState.dwTicks = (ptVFB->dwMilliSecond * ptEncoderInfo->dwMp4VTimeResolution / 1000)+( ptVFB->dwMicroSecond * ptEncoderInfo->dwMp4VTimeResolution / 1000000);
		tMp4VEncState.dwEncUserDataLength = 0;
		tMp4VEncState.pdwPacketSize = ptEncoderInfo->pdwPacketCountBuf;
		//  printf("[venc_encoder_process]%d\n",__LINE__);
		scResult = Mp4VEnc_OneFrame(hMp4VEncObj, &tMp4VEncState);
		// printf("[venc_encoder_process]%d\n",__LINE__);
		if (scResult == S_OK) {
			for (i = 0; i < tMp4VEncState.dwPacketCount; i++) {
				dwFrameBytes += tMp4VEncState.pdwPacketSize[i];     
				//printf("packet size (%d) %d\n", i, tMp4VEncState.pdwPacketSize[i]);
			}
			ptubBuff = (TUBuffer *)(tSharedBuffMgrState.pbyHdrAddr);
			memset(ptubBuff, 0, sizeof(TUBuffer));
			pbyBuffer = (BYTE *)tSharedBuffMgrState.pbyHdrAddr;
			switch (tMp4VEncState.dwPacketCount) {
				case 0:
					printf("PacketCount is zero, skip the frame !!\n");
					break;
				default:
					for (i = 0; i < tMp4VEncState.dwPacketCount; i++) {
						dwPacketBytes += tMp4VEncState.pdwPacketSize[i];
					}                        
					ptubBuff->dwDataType = FOURCC_MP4V;
					ptubBuff->dwTrackID = 1;
					ptubBuff->dwSec = ptVFB->dwSecond;
					ptubBuff->dwUSec = ptVFB->dwMilliSecond * 1000+ptVFB->dwMicroSecond;
					ptubBuff->dwSeqNo = ptEncoderInfo->dwMp4vSeqNo++;
					ptubBuff->bIsSync = tMp4VEncState.bIntraVOP;
					ptubBuff->bIsBoundary = TRUE;
					wHdrTag = TAG_MP4V_EXTENINFO;
					wHdrLen = 2*sizeof(WORD);

					ptubBuff->dwUserDataSize = 2*sizeof(WORD)+sizeof(DWORD);
					ptubBuff->dwSize = sizeof(TUBuffer) + dwPacketBytes+ ptubBuff->dwUserDataSize;
					// copy 2 byte tag 
					memcpy(pbyBuffer+sizeof(TUBuffer), &wHdrTag, sizeof(WORD));
					// copy 2 byte len 
					memcpy(pbyBuffer+sizeof(TUBuffer)+sizeof(WORD), &wHdrLen, sizeof(WORD));
					// copy 4 byte BsBuffOffset
					memcpy(pbyBuffer+sizeof(TUBuffer)+2*sizeof(WORD), &tMp4VEncState.dwBsBuffOffset, sizeof(DWORD));
					break;
			} //switch
#ifdef _DUMP_ENCODER
			fwrite(tSharedBuffMgrState.pbyDataAddr+tMp4VEncState.dwBsBuffOffset, sizeof(BYTE), dwFrameBytes, ptEncoderInfo->pfMp4VOutput);
#endif // _DUMP_ENCODER
			break;               
		} else {
			printf("[VENC_ENCODER] Error encoder MP4 video %d, #%d\n", scResult, iEncCnt);
			tMp4VEncState.dwPacketCount = 0;
			iEncCnt++;
			// change Quant to that it can encode
			tMP4VEncOpt.eOptionFlags = VIDEO_CHANGE_QUANT;
			tMP4VEncOpt.adwUserData[0] = ptEncoderInfo->tMp4VOptRec.dwQuant + iEncCnt*2;
			scResult = Mp4VEnc_SetOptions(hMp4VEncObj, &tMP4VEncOpt);
			assert(scResult == S_OK);
		}
	}//end while(iEncCnt < 3)
	ptEncoderInfo->pfnReleaseOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState);
	// restore the original settings
	if (iEncCnt != 0) {
		// change quant value or bitrate
		tMP4VEncOpt.eOptionFlags = VIDEO_CHANGE_QUANT;
		tMP4VEncOpt.adwUserData[0] = ptEncoderInfo->tMp4VOptRec.dwQuant;
		scResult = Mp4VEnc_SetOptions(hMp4VEncObj, &tMP4VEncOpt);
		assert(scResult == S_OK);
	}
}

/* ========================================================================== */
void VencEncoder_Mp4VRestart(HANDLE hObject)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
    TMp4VEncOptions tMp4VEncOpt;

    tMp4VEncOpt.eOptionFlags = VIDEO_RESTART;
    if (ptEncoderInfo->hMp4VEncObj != NULL) {
        Mp4VEnc_SetOptions(ptEncoderInfo->hMp4VEncObj, &tMp4VEncOpt);
        printf("[VENC_ENCODER_Mp4V](%d) restart mp4v encoder!\n",__LINE__);
    }
}

/* ========================================================================== */
void VencEncoder_Mp4VChangeFrameSize(HANDLE hObject)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
    TMp4VEncOptions tMp4VEncOpt;
    SCODE scRet;

    memset(&tMp4VEncOpt, 0x0, sizeof(TMp4VEncOptions));
    tMp4VEncOpt.eOptionFlags = VIDEO_CHANGE_FRAME_SIZE; // it will chagne the initial option value: dwEncWidth/dwEncHeight
    tMp4VEncOpt.adwUserData[0] = g_avfInfo[ptEncoderInfo->vfCurtFormat].wEncWidth;
    tMp4VEncOpt.adwUserData[1] = g_avfInfo[ptEncoderInfo->vfCurtFormat].wEncHeight;
    if (ptEncoderInfo->hMp4VEncObj != NULL) {
        scRet = Mp4VEnc_SetOptions(ptEncoderInfo->hMp4VEncObj, &tMp4VEncOpt);
        assert(scRet == S_OK);
        printf("Change MP4V Encode size %u,%u\n", tMp4VEncOpt.adwUserData[0], tMp4VEncOpt.adwUserData[1]);
    }
}

/* ========================================================================== */
void VencEncoder_Mp4VChangeInFrameSize(HANDLE hObject, DWORD dwStride)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
    TMp4VEncOptions tMp4VEncOpt;
    SCODE scRet;

    memset(&tMp4VEncOpt, 0x0, sizeof(TMp4VEncOptions));
    tMp4VEncOpt.eOptionFlags = VIDEO_CHANGE_IN_FRAME_SIZE; // it will change the initial option value: dwInWidth/dwInHeight
    tMp4VEncOpt.adwUserData[0] = dwStride;
    tMp4VEncOpt.adwUserData[1] = g_avfInfo[ptEncoderInfo->vfCurtFormat].wEncHeight;
    if (ptEncoderInfo->hMp4VEncObj != NULL) {
        scRet = Mp4VEnc_SetOptions(ptEncoderInfo->hMp4VEncObj, &tMp4VEncOpt);
        assert(scRet == S_OK);
        printf("Change MP4V Input size %u,%u\n", tMp4VEncOpt.adwUserData[0], tMp4VEncOpt.adwUserData[1]);
    }
}

/* ========================================================================== */
void VencEncoder_Mp4VChangeEncodeMode(HANDLE hObject, DWORD dwUserData0, DWORD dwUserData1, DWORD dwUserData2)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
    TMp4VEncOptions tMp4VEncOpt;
    SCODE scRet;

    memset(&tMp4VEncOpt, 0x0, sizeof(TMp4VEncOptions));
    tMp4VEncOpt.eOptionFlags = VIDEO_CHANGE_ENCODE_MODE;
    tMp4VEncOpt.adwUserData[0]  = dwUserData0;
    tMp4VEncOpt.adwUserData[1]  = dwUserData1;
    tMp4VEncOpt.adwUserData[2]  = dwUserData2;
    if (ptEncoderInfo->hMp4VEncObj != NULL) {
		scRet = Mp4VEnc_SetOptions(ptEncoderInfo->hMp4VEncObj, &tMp4VEncOpt);
        assert(scRet == S_OK);
	}
}

#endif // _VMA_MEBE
