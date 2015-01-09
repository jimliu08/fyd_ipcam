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
 * venc_encoder_h264.c
 *
 * \brief
 * H264 relative function implement.
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

#ifdef _H4EE
/* ========================================================================== */
void VencEncoder_H264ApplyOpt(HANDLE hObject)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
	SCODE scRet;
	TH264OptRec *ptH264OptRec = &(ptEncoderInfo->tH264OptRec);
	TH264EncOptions tH264EncOpt;    

	if (ptH264OptRec->bChangeProfile == TRUE) {
		
		memset(&tH264EncOpt, 0x0, sizeof(TH264EncOptions));
		
		tH264EncOpt.eOptionFlags =  VIDEO_CHANGE_ENCODE_MODE; 
		tH264EncOpt.adwUserData[0] = (DWORD) ptH264OptRec->dwProfile;
		if (ptEncoderInfo->hH264EncObj != NULL)
		{
			scRet = H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tH264EncOpt);
			assert(scRet == S_OK);
			printf("[VENC_ENCODER_PROCESS](%d) Set H264 Profile: %d\n", __LINE__,tH264EncOpt.adwUserData[0]);
		}
		
		ptH264OptRec->bChangeProfile = FALSE;
	}
	
	
	if (ptH264OptRec->bDeblock == TRUE) {
		
		memset(&tH264EncOpt, 0x0, sizeof(TH264EncOptions));
		
		tH264EncOpt.eOptionFlags = VIDEO_CHANGE_H264_ENCODE_DEBLOCKING_FLAG; 
		tH264EncOpt.adwUserData[0] = (DWORD) ptH264OptRec->bDeblockEnable;
		if (ptEncoderInfo->hH264EncObj != NULL)
		{
			scRet = H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tH264EncOpt);
			assert(scRet == S_OK);
			printf("[VENC_ENCODER_PROCESS](%d) Set H264 Deblocking: %d\n", __LINE__,tH264EncOpt.adwUserData[0]);
		}
		
		ptH264OptRec->bDeblock = FALSE;
	}
	
	if (ptH264OptRec->bQualityGroup == TRUE) {
		
		memset(&tH264EncOpt, 0x0, sizeof(TH264EncOptions));

		if (ptH264OptRec->dwBitrate > 0)
		{
			tH264EncOpt.eOptionFlags = VIDEO_CHANGE_BITRATE;
			tH264EncOpt.adwUserData[0] = ptH264OptRec->dwBitrate;
			if (ptEncoderInfo->hH264EncObj != NULL)
			{
				scRet = H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tH264EncOpt);
				assert(scRet == S_OK);
				printf("[VENC_ENCODER_PROCESS](%d) Set H264 Bitrate %u\n",  __LINE__,tH264EncOpt.adwUserData[0]);
			}

			tH264EncOpt.eOptionFlags = VIDEO_SET_RATE_CONTROL; 
			tH264EncOpt.adwUserData[0] = ptH264OptRec->ercfH264QualityType;
			if (ptEncoderInfo->hH264EncObj != NULL)
			{
				scRet = H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tH264EncOpt);
				assert(scRet == S_OK);
				printf("[VENC_ENCODER_PROCESS](%d) Set H264 RateCtrl: %d\n", __LINE__,tH264EncOpt.adwUserData[0]);
			}
		}

		ptH264OptRec->bBitrate = FALSE;
		ptH264OptRec->bSetRateControl = FALSE;

		tH264EncOpt.eOptionFlags = VIDEO_CHANGE_QUANT;
		tH264EncOpt.adwUserData[0] = ptH264OptRec->dwQuant;
		tH264EncOpt.adwUserData[1] = ptH264OptRec->dwMaxQuant;
		tH264EncOpt.adwUserData[2] = ptH264OptRec->dwMinQuant;
		if (ptEncoderInfo->hH264EncObj != NULL)
		{
			scRet = H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tH264EncOpt);
			assert(scRet == S_OK);
			printf("[VENC_ENCODER_PROCESS](%d) Set H264 Quant (init,max,min)=(%u,%u,%u)\n", __LINE__,tH264EncOpt.adwUserData[0],tH264EncOpt.adwUserData[1],tH264EncOpt.adwUserData[2]);
		}
		ptH264OptRec->bQuant = FALSE;


		ptH264OptRec->bQualityGroup = FALSE;
	}

	if (ptH264OptRec->bForceIntra == TRUE) {
		
		memset(&tH264EncOpt, 0x0, sizeof(TH264EncOptions));
		tH264EncOpt.eOptionFlags = VIDEO_FORCE_INTRA;
		if (ptEncoderInfo->hH264EncObj != NULL)
		{
			scRet = H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tH264EncOpt);
			assert(scRet == S_OK);
			printf("[VENC_ENCODER] Set H264 ForceIntra\n");
		}
		ptH264OptRec->bForceIntra = FALSE;
	}

	if (ptH264OptRec->bIntraInterval == TRUE) 
	{
		memset(&tH264EncOpt, 0x0, sizeof(TH264EncOptions));
		// call force intra first
		tH264EncOpt.eOptionFlags = VIDEO_FORCE_INTRA;
		if (ptEncoderInfo->hH264EncObj != NULL)
		{
			scRet = H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tH264EncOpt);
			assert(scRet == S_OK);
			printf("[VENC_ENCODER] Set H264 ForceIntra\n");
		}
		// call change interval
		tH264EncOpt.eOptionFlags = VIDEO_CHANGE_INTRA_INTERVAL;        
		tH264EncOpt.adwUserData[0] = ptH264OptRec->dwIntraInterval;
		if (ptEncoderInfo->hH264EncObj != NULL)
		{
			scRet = H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tH264EncOpt);
			assert(scRet == S_OK);
			printf("[VENC_ENCODER] Set H264 IntraInterval %u\n",tH264EncOpt.adwUserData[0]);
		}
		ptH264OptRec->bIntraInterval = FALSE;
	}

	if (ptH264OptRec->bSearchRangeGroup == TRUE)
	{
		memset(&tH264EncOpt,0x0,sizeof(TH264EncOptions));
		tH264EncOpt.eOptionFlags = VIDEO_CHANGE_SEARCH_RANGE;        
		tH264EncOpt.adwUserData[0] = ptH264OptRec->dwSearchRangeX;
		tH264EncOpt.adwUserData[1] = ptH264OptRec->dwSearchRangeY;
		printf("[VENC_ENCODER_PROCESS](%d) Set H264 Search Range (X,Y)=(%u,%u)\n",__LINE__,tH264EncOpt.adwUserData[0],tH264EncOpt.adwUserData[1]);
		if (ptEncoderInfo->hH264EncObj != NULL)
		{
			scRet = H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tH264EncOpt);
			assert(scRet == S_OK);
		}

		ptH264OptRec->bSearchRangeGroup = FALSE;
	}
	
	if (ptH264OptRec->bRefFrameNum == TRUE)
	{
		memset(&tH264EncOpt,0x0,sizeof(TH264EncOptions));
		tH264EncOpt.eOptionFlags = VIDEO_CHANGE_REFERENCE_FRAME_NUM;        
		tH264EncOpt.adwUserData[0] = ptH264OptRec->dwRefFrameNum;
		printf("[VENC_ENCODER_PROCESS](%d) Set H264 Ref. Frame Num %u\n",__LINE__,tH264EncOpt.adwUserData[0]);
		if (ptEncoderInfo->hH264EncObj != NULL)
		{
			scRet = H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tH264EncOpt);
			assert(scRet == S_OK);
		}

		ptH264OptRec->bRefFrameNum = FALSE;
	}
	
	if (ptH264OptRec->bFrameRateGroup == TRUE)
	{
		memset(&tH264EncOpt,0x0,sizeof(TH264EncOptions));
		tH264EncOpt.eOptionFlags = VIDEO_CHANGE_FRAME_RATE;        
		tH264EncOpt.adwUserData[0] = ptH264OptRec->dwMinFrameRate;
		tH264EncOpt.adwUserData[1] = ptH264OptRec->dwMaxFrameRate;
		printf("[VENC_ENCODER_PROCESS](%d) Set H264 Frame rate (max,min)=(%u,%u)\n",__LINE__,tH264EncOpt.adwUserData[1],tH264EncOpt.adwUserData[0]);
		if (ptEncoderInfo->hH264EncObj != NULL)
		{
			scRet = H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tH264EncOpt);
			assert(scRet == S_OK);
		}

		ptH264OptRec->bFrameRateGroup = FALSE;
	}
}

/* =========================================================================================== */
/*
   sizeof(TUBufferConfH264) bytes    2 bytes   m bytes, m < 65536    2 bytes      n bytes 
   |<------------------------------>|<-------->|<------------------>|<-------->|<--------->
   +--------------------------------+----------+--------------------+----------+-----------
   |    TUBufferConfH264            |Length (m)|    SPS             |Length (n)|  PPS
   +--------------------------------+----------+--------------------+----------+-----------
 */
void VencEncoder_H264ForceCI(HANDLE hObject, EVideoFormat vFormat)
{
#define BYTESTREAM_HEADER_LEN 0
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
	HANDLE hH264EncObj = ptEncoderInfo->hH264EncObj;
	TSharedBuffMgrState tSharedBuffMgrState;
	TUBufferConfH264 *ptubcH264;
	BYTE	*pbyHdrAddr;

	fd_set               	fdsWrite;
	struct timeval 			timeout;
	int						fdShardBuffer;
	int     				iResult;
	SCODE 	scResult = S_FAIL;
	WORD	wSeqByte; 
	WORD	wPicByte;

	BYTE abySPSBuf[128];
	BYTE abyPPSBuf[128];

	memset(abySPSBuf, 0, sizeof(abySPSBuf));
	memset(abyPPSBuf, 0, sizeof(abyPPSBuf));
	if (!hH264EncObj)
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
			DBPRINT0("H264 select timeout.... \n");
		}

		if (!FD_ISSET(fdShardBuffer, &fdsWrite))
			return;

		if (ptEncoderInfo->pfnRequestOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState) != S_OK)
			return;
	}

	ptubcH264 = (TUBufferConfH264 *)(tSharedBuffMgrState.pbyHdrAddr);
	pbyHdrAddr = (BYTE*)(tSharedBuffMgrState.pbyHdrAddr);
	memset(ptubcH264, 0, sizeof(TUBufferConfH264));

	wSeqByte = SHAREDBUFFER_HEADERSIZE - sizeof(TUBufferConfH264);
	scResult = H264Enc_SpecificConfig_SPS(hH264EncObj, abySPSBuf, &wSeqByte);
	assert(scResult == S_OK);
	wSeqByte -= sizeof(DWORD); // because ByteStreamHeader is 4 bytes
	memcpy((pbyHdrAddr + sizeof(TUBufferConfH264)), &wSeqByte, sizeof(WORD));
	memcpy((pbyHdrAddr + sizeof(TUBufferConfH264) + sizeof(WORD)), abySPSBuf+sizeof(DWORD), wSeqByte);

	wPicByte = SHAREDBUFFER_HEADERSIZE - sizeof(TUBufferConfH264) - sizeof(WORD) - wSeqByte;
	scResult = H264Enc_SpecificConfig_PPS(hH264EncObj, abyPPSBuf, &wPicByte);
	assert(scResult == S_OK);
	wPicByte -= sizeof(DWORD);
	memcpy((pbyHdrAddr + sizeof(TUBufferConfH264) + sizeof(WORD) + wSeqByte), &wPicByte, sizeof(WORD));
	memcpy((pbyHdrAddr + sizeof(TUBufferConfH264) + sizeof(WORD) + wSeqByte + sizeof(WORD)), abyPPSBuf+sizeof(DWORD), wPicByte);

	ptubcH264->dwSize = sizeof(TUBufferConfH264) + sizeof(WORD) + wSeqByte + sizeof(WORD) + wPicByte;
	ptubcH264->dwDataType = FOURCC_CONF;
	ptubcH264->dwFollowingDataType = FOURCC_H264;
	ptubcH264->dwTrackID = 1;
	ptubcH264->dwWidth  = g_avfInfo[vFormat].wEncWidth;
	ptubcH264->dwHeight = g_avfInfo[vFormat].wEncHeight;

	// it should Get abyProfileLevelId from H264 library API 
	ptubcH264->abyProfileLevelId[0] = *(pbyHdrAddr + sizeof(TUBufferConfH264) + sizeof(WORD) + 1);
	ptubcH264->abyProfileLevelId[1] = *(pbyHdrAddr + sizeof(TUBufferConfH264) + sizeof(WORD) + 2)&0xE0;
	ptubcH264->abyProfileLevelId[2] = *(pbyHdrAddr + sizeof(TUBufferConfH264) + sizeof(WORD) + 3);
	printf("abyProfileLevelId (0,1,2) = (%02x, %02x, %02x) \n", ptubcH264->abyProfileLevelId[0], ptubcH264->abyProfileLevelId[1], ptubcH264->abyProfileLevelId[2]);					

	ptEncoderInfo->pfnReleaseOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState);
	printf("header size %u\n", sizeof(TUBufferConfH264));
	printf("H264 Conf (dwWidth,dwHeight) = (%u, %u) \n", ptubcH264->dwWidth, ptubcH264->dwHeight);
}

/* =========================================================================================== */
void VencEncoder_H264Encode(HANDLE hObject, TVideoFrameBuffer *ptVFB)
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

	// H4EE Encoder stuff
	HANDLE                 hH264EncObj = ptEncoderInfo->hH264EncObj;
	TH264EncOptions        tH264EncOpt;    
	TH264EncState          tH264EncState;
	SCODE                  scResult;
	TUBuffer 			   *ptubBuff;
	DWORD					dwFrameBytes = 0;
	DWORD					dwNALSize;
	DWORD					dwTtlSize = 0;

	iEncCnt = 0;
	if (!hH264EncObj)
		return;

	tH264EncOpt.eOptionFlags    = VIDEO_CHANGE_FRAME_BUFFER;
	tH264EncOpt.adwUserData[0]  = (DWORD)ptVFB->pbyBufY;
	tH264EncOpt.adwUserData[1]  = (DWORD)ptVFB->pbyBufU;
	tH264EncOpt.adwUserData[2]  = (DWORD)ptVFB->pbyBufV;
	if (H264Enc_SetOptions(hH264EncObj, &tH264EncOpt) != S_OK)
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
			DBPRINT0("H264 select timeout.... \n");
			return;
		}
		if (!FD_ISSET(fdShardBuffer, &fdsWrite))
			return;

		if (ptEncoderInfo->pfnRequestOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState) != S_OK)
			return;
	}

	while(iEncCnt < 3) {
		//set output buffer to H264 encoder
		tH264EncOpt.eOptionFlags = VIDEO_CHANGE_BITSTREAM_BUFFER;
		tH264EncOpt.adwUserData[0] = (DWORD)(tSharedBuffMgrState.pbyDataAddr);
		tH264EncOpt.adwUserData[1] = ptEncoderInfo->_tInitNeedInfo.dwBitStreamSize;
		scResult = H264Enc_SetOptions(hH264EncObj, &tH264EncOpt);
		assert(scResult == S_OK);

		//time of H264 video
		//memset(&tH264EncState, 0, sizeof(TH264EncState)); 
		tH264EncState.dwSeconds = ptVFB->dwSecond;
		tH264EncState.dwTicks = (ptVFB->dwMilliSecond * ptEncoderInfo->dwH264TimeResolution / 1000)+(ptVFB->dwMicroSecond * ptEncoderInfo->dwH264TimeResolution / 1000000);
		tH264EncState.dwEncUserDataLength = 0;
#if 0
		gettimeofday(&pre_encode_time,NULL);
#endif
		scResult = H264Enc_OneFrame(hH264EncObj, &tH264EncState);
		if (scResult == S_OK) {
#if 0
			gettimeofday(&cur_encode_time,NULL);
#endif
#ifdef _DUMP_ENCODER
			if (tH264EncState.bIDR == TRUE) {
				ptEncoderInfo->bFirstIntra = TRUE;
			}
			if (ptEncoderInfo->bFirstIntra == TRUE) {
				printf("[venc_encoder_process](%d)Enter Dump bitstream...\n", __LINE__);
				fwrite(tSharedBuffMgrState.pbyDataAddr, sizeof(BYTE), tH264EncState.dwEncBytes, ptEncoderInfo->pfH264Output);
				printf("[venc_encoder_process](%d)Leave Dump bitstream...\n", __LINE__);

			}
#endif // _DUMP_ENCODER
			dwFrameBytes = tH264EncState.dwEncBytes;

			ptubBuff = (TUBuffer *)(tSharedBuffMgrState.pbyHdrAddr);
			memset(ptubBuff, 0, sizeof(TUBuffer));

			if ((dwFrameBytes > 0) && (tH264EncState.dwNALCount > 0)) {
				ptubBuff->dwDataType = FOURCC_H264;
				ptubBuff->dwTrackID = 1;
				ptubBuff->dwSec = ptVFB->dwSecond;
				ptubBuff->dwUSec = ptVFB->dwMilliSecond * 1000 + ptVFB->dwMicroSecond;
				ptubBuff->dwSeqNo = ptEncoderInfo->dwH264SeqNo++;
				ptubBuff->bIsSync = tH264EncState.bIDR;
				ptubBuff->bIsBoundary = TRUE;       
				ptubBuff->dwUserDataSize = 0;
				ptubBuff->dwSize = sizeof(TUBuffer) + dwFrameBytes;
				for(i = 0; i < tH264EncState.dwNALCount; i++) {
					dwNALSize = tH264EncState.adwNALBytes[i]-sizeof(DWORD);
					memcpy(tSharedBuffMgrState.pbyDataAddr+dwTtlSize, &dwNALSize, sizeof(DWORD));
					dwTtlSize += dwNALSize+sizeof(DWORD);
				}
				break;
			} else {
				printf("dwFrameBytes <= 0 or tH264EncState.dwNALCount <= 0, skip this frame !!\n");
				break;
			}
		} else {
			printf("[VENC_ENCODER] Error encoding H264 video %d, #%d\n", scResult, iEncCnt);
			iEncCnt++;
			// change Quant to that it can encode
			tH264EncOpt.eOptionFlags = VIDEO_CHANGE_QUANT;
			tH264EncOpt.adwUserData[0] = ptEncoderInfo->tH264OptRec.dwQuant + iEncCnt*2;
			tH264EncOpt.adwUserData[1] = ptEncoderInfo->tH264OptRec.dwMaxQuant;
			tH264EncOpt.adwUserData[2] = ptEncoderInfo->tH264OptRec.dwMinQuant;
			scResult = H264Enc_SetOptions(hH264EncObj, &tH264EncOpt);
			assert(scResult == S_OK);		        		
		}
	} // end while(iEncCnt < 3)
	ptEncoderInfo->pfnReleaseOutputBuffer(ptEncoderInfo->hOutputSrdObj, &tSharedBuffMgrState);
	// restore the original settings
	if (iEncCnt != 0) {
		// change quant value
		tH264EncOpt.eOptionFlags = VIDEO_CHANGE_QUANT;
		tH264EncOpt.adwUserData[0] = ptEncoderInfo->tH264OptRec.dwQuant;
		tH264EncOpt.adwUserData[1] =ptEncoderInfo->tH264OptRec.dwMaxQuant;
		tH264EncOpt.adwUserData[2] =ptEncoderInfo->tH264OptRec.dwMinQuant;
		scResult = H264Enc_SetOptions(hH264EncObj, &tH264EncOpt);
		assert(scResult == S_OK);
	}
}

/* =========================================================================================== */
void VencEncoder_H264Restart(HANDLE hObject)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
	TH264EncOptions tH264EncOpt;

	tH264EncOpt.eOptionFlags = VIDEO_RESTART;
	if (ptEncoderInfo->hH264EncObj != NULL) {
		H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tH264EncOpt);
		printf("[VENC_ENCODER_H264](%d) restart H264 encoder!\n",__LINE__);
	}
}
/* =========================================================================================== */
void VencEncoder_H264ChangeFrameSize(HANDLE hObject)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
	TH264EncOptions tH264EncOpt;
	SCODE scRet;

	memset(&tH264EncOpt, 0x0, sizeof(TH264EncOptions));
	tH264EncOpt.eOptionFlags = VIDEO_CHANGE_FRAME_SIZE;
	tH264EncOpt.adwUserData[0] = g_avfInfo[ptEncoderInfo->vfCurtFormat].wEncWidth;
	tH264EncOpt.adwUserData[1] = g_avfInfo[ptEncoderInfo->vfCurtFormat].wEncHeight;
	if (ptEncoderInfo->hH264EncObj != NULL) {
		scRet = H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tH264EncOpt);
		assert(scRet == S_OK);
		printf("Change H264 Encode size %u,%u\n", tH264EncOpt.adwUserData[0], tH264EncOpt.adwUserData[1]);
	}
}

/* =========================================================================================== */
void VencEncoder_H264ChangeInFrameSize(HANDLE hObject, DWORD dwStride)
{
	TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
	TH264EncOptions tH264EncOpt;
	SCODE scRet;

	memset(&tH264EncOpt, 0x0, sizeof(TH264EncOptions));
	tH264EncOpt.eOptionFlags = VIDEO_CHANGE_IN_FRAME_SIZE;
	tH264EncOpt.adwUserData[0] = dwStride;
	tH264EncOpt.adwUserData[1] = g_avfInfo[ptEncoderInfo->vfCurtFormat].wEncHeight;
	if (ptEncoderInfo->hH264EncObj != NULL) {
		scRet = H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tH264EncOpt);
		printf("Change H264 Input size %u,%u\n", tH264EncOpt.adwUserData[0], tH264EncOpt.adwUserData[1]);
		assert(scRet == S_OK);
	}
}

/* =========================================================================================== */
void VencEncoder_H264ChangeFrameTimeTick(HANDLE hObject, DWORD dwFrameRate)
{
    TEncoderInfo *ptEncoderInfo = (TEncoderInfo *) hObject;
    TH264EncOptions tEncOptions;

    memset(&tEncOptions ,0x0 ,sizeof(TH264EncOptions));
	tEncOptions.eOptionFlags = VIDEO_CHANGE_FRAME_TIME_TICKS_INFO;
	tEncOptions.adwUserData[0] = dwFrameRate * 2000;
	tEncOptions.adwUserData[1] = 1000;
	ptEncoderInfo->dwH264TimeResolution = ((tEncOptions.adwUserData[0] / tEncOptions.adwUserData[1]) >> 1);
	if (H264Enc_SetOptions(ptEncoderInfo->hH264EncObj, &tEncOptions) != S_OK) {
		fprintf(stderr,"[VENC_ENCODER] Set options VIDEO_CHANGE_FRAME_TIME_TICKS_INFO fail !!\n");
	} else {
		printf("[VENC_ENCODER] Frame set as: %d\n", dwFrameRate);
	}
}
#endif // H4EE
