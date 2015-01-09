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
 * venc_encoder_jpeg.c
 *
 * \brief
 * JPEG relative function implement.
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
#ifdef _VMA_JEBE
/* ========================================================================== */
void VencEncoder_JPEGApplyOpt(HANDLE hObject)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
    SCODE scRet;
    TJPEGOptRec *ptJPEGOptRec = &(ptEncoderInfo->tJPEGOptRec);
    TJPEGEncOptions tJPEGEncOpt;

    if (ptJPEGOptRec->bQuant == TRUE) {
        tJPEGEncOpt.eOptionFlags = VIDEO_CHANGE_QUANT;
        tJPEGEncOpt.adwUserData[0] = ptJPEGOptRec->dwQuant;
        if (ptEncoderInfo->hJPEGEncObj != NULL) {
            scRet = JPEGEnc_SetOptions(ptEncoderInfo->hJPEGEncObj, &tJPEGEncOpt);
            assert(scRet == S_OK);
            DBPRINT1("[VENC_ENCODER] JPEG Quant %u\n",tJPEGEncOpt.adwUserData[0]);
        }
        ptJPEGOptRec->bQuant = FALSE;
    }
    memset(ptJPEGOptRec, 0, sizeof(TJPEGOptRec));
}

/* ========================================================================== */
void VencEncoder_JPEGForceCI(HANDLE hObject, EVideoFormat vFormat)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
    TSharedBuffMgrState tSharedBuffMgrState;
    TUBufferConfJPEG *ptubcJPEG ;//= (TUBufferConfJPEG *)(ptEncoderInfo->pbyBuffer);
    fd_set                  fdsWrite;
    struct timeval          timeout;
    int                     fdShardBuffer;
    int                     iResult;

    ptEncoderInfo->pfnGetOutputBufferFD(ptEncoderInfo->hOutputSrdObj, &fdShardBuffer);
	if (ptEncoderInfo->pfnRequestOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState) != S_OK)
	{
		timeout.tv_sec = 0;
		timeout.tv_usec = 10000;
		FD_ZERO(&fdsWrite);
		FD_SET(fdShardBuffer, &fdsWrite);
		iResult = select(fdShardBuffer + 1, NULL, &fdsWrite, NULL, &timeout);
		if (iResult <= 0) {
			DBPRINT0("MJPEG select timeout.... \n");
			return;
		}
		if (!FD_ISSET(fdShardBuffer, &fdsWrite))
			return;
		if (ptEncoderInfo->pfnRequestOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState) != S_OK)
			return;
	}
	ptubcJPEG = (TUBufferConfJPEG *)(tSharedBuffMgrState.pbyHdrAddr);
	memset(ptubcJPEG, 0, sizeof(TUBufferConfJPEG));

	ptubcJPEG->dwSize = sizeof(TUBufferConfJPEG);
	ptubcJPEG->dwDataType = FOURCC_CONF;
	ptubcJPEG->dwFollowingDataType = FOURCC_JPEG;
	ptubcJPEG->dwTrackID = 1;
	ptubcJPEG->dwWidth = g_avfInfo[vFormat].wEncWidth;
	ptubcJPEG->dwHeight = g_avfInfo[vFormat].wEncHeight;
	ptubcJPEG->dwHeaderLen = 589;
	ptubcJPEG->dwXDensity = 2;
	ptubcJPEG->dwYDensity = 2;
	ptubcJPEG->dwNumComponents = 3;

	ptEncoderInfo->pfnReleaseOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState);
}

/* ========================================================================== */
void VencEncoder_JPEGEncode(HANDLE hObject, TVideoFrameBuffer *ptVFB)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
	int						iEncCnt;
	struct timeval 			timeout;
	int						fdShardBuffer;
	int     				iResult;
	fd_set              	fdsWrite;

	// for Streaming server
	TSharedBuffMgrState    tSharedBuffMgrState;

	// JPEG Encoder stuff
	HANDLE                 hJPEGEncObj = ptEncoderInfo->hJPEGEncObj;
	TJPEGEncOptions        tJPEGEncOpt;
	TJPEGEncState          tJPEGEncState;
	SCODE                  scResult;
	BYTE                   *pbyBuffer;
	TUBuffer 			   *ptubBuff;
	WORD                   wHdrTag, wHdrLen;
	DWORD                  dwFrameBytes = 0;

#ifdef _DUMP_ENCODER
	char aszJPEGFilePath[30];
	sprintf(aszJPEGFilePath, "/tmp/test%02ld.jpg", ptEncoderInfo->dwFrameCount);
	ptEncoderInfo->pfJPEGOutput = fopen(aszJPEGFilePath, "wb");
#endif // _DUMP_ENCODER

	iEncCnt = 0;
	if (!hJPEGEncObj)
		return;

	tJPEGEncOpt.eOptionFlags    = VIDEO_CHANGE_FRAME_BUFFER;
	tJPEGEncOpt.adwUserData[0]  = (DWORD)ptVFB->pbyBufY;
	tJPEGEncOpt.adwUserData[1]  = (DWORD)ptVFB->pbyBufU;
	tJPEGEncOpt.adwUserData[2]  = (DWORD)ptVFB->pbyBufV;
	if (JPEGEnc_SetOptions(hJPEGEncObj, &tJPEGEncOpt) != S_OK)
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
			DBPRINT0("MJPEG select timeout.... \n");
		}
		if (!FD_ISSET(fdShardBuffer, &fdsWrite))
			return;

		if (ptEncoderInfo->pfnRequestOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState) != S_OK)
			return;
	}
	while(iEncCnt < 3) {
		// set output buffer to JPEG encoder
		tJPEGEncOpt.eOptionFlags = VIDEO_CHANGE_BITSTREAM_BUFFER;
		tJPEGEncOpt.adwUserData[0] = (DWORD)(tSharedBuffMgrState.pbyDataAddr);
		tJPEGEncOpt.adwUserData[1] = ptEncoderInfo->_tInitNeedInfo.dwBitStreamSize;

		scResult = JPEGEnc_SetOptions(hJPEGEncObj, &tJPEGEncOpt);
		assert(scResult == S_OK);

		memset(&tJPEGEncState, 0, sizeof(TJPEGEncState));
		scResult = JPEGEnc_OneFrame(hJPEGEncObj, &tJPEGEncState);
		if (scResult == S_OK) {
			dwFrameBytes = tJPEGEncState.dwEncSize;
			ptubBuff = (TUBuffer *)(tSharedBuffMgrState.pbyHdrAddr);
			memset(ptubBuff, 0, sizeof(TUBuffer)+sizeof(DWORD));
			pbyBuffer = (BYTE *)(tSharedBuffMgrState.pbyHdrAddr);
			wHdrTag = TAG_JPEG_EXTENINFO;
			wHdrLen = sizeof(DWORD);

			ptubBuff->dwSize = sizeof(TUBuffer) + dwFrameBytes + (sizeof(DWORD)<<1);
			ptubBuff->dwDataType = FOURCC_JPEG;
			ptubBuff->dwTrackID = 1;
			ptubBuff->dwSec = ptVFB->dwSecond;
			ptubBuff->dwUSec = ptVFB->dwMilliSecond * 1000 + ptVFB->dwMicroSecond;
			ptubBuff->dwSeqNo = ptEncoderInfo->dwJPEGSeqNo++;
			ptubBuff->bIsSync = TRUE;
			ptubBuff->bIsBoundary = TRUE;
			ptubBuff->dwUserDataSize = 2 * sizeof(DWORD);                   
			// copy 2 byte tag 
			memcpy(pbyBuffer+sizeof(TUBuffer), &wHdrTag, 2);
			// copy 2 byte len 
			memcpy(pbyBuffer+sizeof(TUBuffer)+2, &wHdrLen,2);
			// copy 4 byte Jpeg Size 
			memcpy(pbyBuffer+sizeof(TUBuffer)+sizeof(DWORD), 
					&tJPEGEncState.dwEncSize, sizeof(DWORD));						
#ifdef _DUMP_ENCODER
			if (ptEncoderInfo->pfJPEGOutput == NULL) {
				printf("can't open jpeg output\n");
				break;
			}
			fwrite(tSharedBuffMgrState.pbyDataAddr, sizeof(BYTE), dwFrameBytes, ptEncoderInfo->pfJPEGOutput);
			fclose(ptEncoderInfo->pfJPEGOutput);
			(ptEncoderInfo->dwFrameCount < 500)?ptEncoderInfo->dwFrameCount++:(ptEncoderInfo->dwFrameCount = 0);
#endif // _DUMP_ENCODER       
			break;
		} else {
			printf("[VENC_ENCODER] Error encoder JPEG image %d, #%d\n", scResult, iEncCnt);
			iEncCnt++;
			dwFrameBytes = 0;
			// change quant value
			tJPEGEncOpt.eOptionFlags  = VIDEO_CHANGE_QUANT;
			tJPEGEncOpt.adwUserData[0] = ptEncoderInfo->tJPEGOptRec.dwQuant + iEncCnt*20;
			scResult = JPEGEnc_SetOptions(hJPEGEncObj, &tJPEGEncOpt);
			assert(scResult == S_OK);
		}
	}//end while(iEncCnt < 3)
	ptEncoderInfo->pfnReleaseOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState);		                 
	// restore the original quant if necessary
	if (iEncCnt != 0) {
		tJPEGEncOpt.eOptionFlags  = VIDEO_CHANGE_QUANT;
		tJPEGEncOpt.adwUserData[0] = ptEncoderInfo->tJPEGOptRec.dwQuant;
		scResult = JPEGEnc_SetOptions(hJPEGEncObj, &tJPEGEncOpt);
		assert(scResult == S_OK);
	}        	
#ifdef _DUMP_ENCODER
	if (ptEncoderInfo->pfJPEGOutput != NULL)
		fclose(ptEncoderInfo->pfJPEGOutput);
#endif    
	return;
}

/* ========================================================================== */
void VencEncoder_JPEGChangeFrameSize(HANDLE hObject)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
    TJPEGEncOptions tJPEGEncOpt;
    SCODE scRet;

    memset(&tJPEGEncOpt, 0x0, sizeof(TJPEGEncOptions));
    tJPEGEncOpt.eOptionFlags = VIDEO_CHANGE_FRAME_SIZE;
    tJPEGEncOpt.adwUserData[0] = g_avfInfo[ptEncoderInfo->vfCurtFormat].wEncWidth;
    tJPEGEncOpt.adwUserData[1] = g_avfInfo[ptEncoderInfo->vfCurtFormat].wEncHeight;
    if (ptEncoderInfo->hJPEGEncObj != NULL) {
        scRet = JPEGEnc_SetOptions(ptEncoderInfo->hJPEGEncObj, &tJPEGEncOpt);
        assert(scRet == S_OK);
        printf("Change JPEG Encode size %u,%u\n", tJPEGEncOpt.adwUserData[0], tJPEGEncOpt.adwUserData[1]);
    }
}

/* ========================================================================== */
void VencEncoder_JPEGChangeInFrameSize(HANDLE hObject, DWORD dwStride)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
    TJPEGEncOptions tJPEGEncOpt;
    SCODE scRet;

    memset(&tJPEGEncOpt, 0x0, sizeof(TJPEGEncOptions));
    tJPEGEncOpt.eOptionFlags = VIDEO_CHANGE_IN_FRAME_SIZE;
    tJPEGEncOpt.adwUserData[0] = dwStride;
    tJPEGEncOpt.adwUserData[1] = g_avfInfo[ptEncoderInfo->vfCurtFormat].wEncHeight;
    if (ptEncoderInfo->hJPEGEncObj != NULL) {
        scRet = JPEGEnc_SetOptions(ptEncoderInfo->hJPEGEncObj, &tJPEGEncOpt);
        assert(scRet == S_OK);
        printf("Change JPEG Input size %u,%u\n", tJPEGEncOpt.adwUserData[0], tJPEGEncOpt.adwUserData[1]);
    }
}

#endif // _VMA_JEBE
