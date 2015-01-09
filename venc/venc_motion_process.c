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
 * venc_motion_process.c
 *
 * \brief
 * Sending motion data function.
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

#if defined(_MD_TRIGER_ROI_)
	#include <unistd.h>
	#include <sys/mman.h>
	#include <sys/shm.h>
	#include <sys/stat.h> 
#endif
/* ========================================================================== */
SCODE VencMotion_WriteConf(TMotionInfo *ptInfo)
{
	TUBufferConfMDAT    tMDataConf;
	struct iovec        iov[1];
	struct msghdr       MsgHeader;
	int                 iRet        = -1;

	memset( &tMDataConf, 0, sizeof(TUBufferConfMDAT) );

	tMDataConf.dwSize               = sizeof(TUBufferConfMDAT);
	tMDataConf.dwDataType           = FOURCC_CONF;
	tMDataConf.dwFollowingDataType  = FOURCC_MDAT;
	tMDataConf.dwSampleRate         = 90000;

	MsgHeader.msg_name          = &(ptInfo->tRtspSckInfo.sunx);
	MsgHeader.msg_namelen       = sizeof(ptInfo->tRtspSckInfo.sunx);
	MsgHeader.msg_iov           = iov;
	MsgHeader.msg_iovlen        = 1;
	MsgHeader.msg_control       = NULL;
	MsgHeader.msg_controllen    = 0;
	MsgHeader.msg_flags         = 0;

	iov[0].iov_base = &tMDataConf;
	iov[0].iov_len  = sizeof(TUBufferConfMDAT);

	iRet = sendmsg(ptInfo->tRtspSckInfo.fdSock, &MsgHeader, 0);
	if (iRet != sizeof(TUBufferConfMDAT)) {
		fprintf(stderr, "%s Err: sendmsg to RTSP(conf): (%d) %s\n", 
				MOT_PRE, errno, strerror(errno));
		return S_FAIL;
	}
	ptInfo->tRtspSckInfo.bNeedConf = FALSE;
	return S_OK;
}

/* ========================================================================== */
TSharedBuffMgrState* VencMotion_RequestInputBuf(TMotionInfo *ptInfo)
{
	TSharedBuffMgrState     *ptSrdBufMgrState   = NULL;

	fd_set                  fdRead;
	struct timeval          timeout;
	int                     fd                  = ptInfo->iVideoInSrdFD;
	int                     iRet                = -1;
	SCODE                   scRet               = S_FAIL;

	FD_ZERO(&fdRead);
	timeout.tv_sec  = 2;
	timeout.tv_usec = 0;

	FD_SET(fd, &fdRead);

	iRet = select(fd+1, &fdRead, NULL, NULL, NULL/*&timeout*/);
	if (iRet == 0) {
		fprintf(stderr, "%s shared buffer select timeout\n", MOT_PRE);
		return NULL;
	}
	else if (iRet < 0) {
		fprintf(stderr, "%s Err: shared buffer select: (%d) %s\n", 
				MOT_PRE, errno, strerror(errno));
		return NULL;
	}

	if ( FD_ISSET(fd, &fdRead) ) {
		scRet = SharedBuffMgr_GetBuffer( ptInfo->hVideoInSrdObj, 
				&(ptInfo->tSrdBufMgrState) );
		if (scRet == S_OK) {
			ptSrdBufMgrState = &(ptInfo->tSrdBufMgrState);
		}
	}
	return ptSrdBufMgrState;
}

/* ========================================================================== */
SCODE VencMotion_ReleaseInputBuf(HANDLE                 hVideoInSrdObj, 
		TSharedBuffMgrState    *ptState)
{
	SharedBuffMgr_ReleaseBuffer(hVideoInSrdObj, ptState);
	return S_OK;
}

/* ========================================================================== */
SCODE VencMotion_ReSetEncoder(HANDLE hJPEGEncObj, EVideoFormat vFormat)
{
#ifdef _VMA_JEBE
	TJPEGEncOptions tJPEGEncOpt;
	SCODE           scRet;

	if (hJPEGEncObj != NULL) {
		tJPEGEncOpt.eOptionFlags    = VIDEO_CHANGE_FRAME_SIZE;
		tJPEGEncOpt.adwUserData[0]  = g_avfInfo[vFormat].wEncWidth;
		tJPEGEncOpt.adwUserData[1]  = g_avfInfo[vFormat].wEncHeight;    
		scRet = JPEGEnc_SetOptions(hJPEGEncObj, &tJPEGEncOpt);
		assert(scRet == S_OK);
		MOT_DP("%s Change JPEG Encode size %ux%u\n", 
				MOT_PRE, 
				tJPEGEncOpt.adwUserData[0], 
				tJPEGEncOpt.adwUserData[1]);

		tJPEGEncOpt.eOptionFlags    = VIDEO_CHANGE_IN_FRAME_SIZE;
		tJPEGEncOpt.adwUserData[0]  = g_avfInfo[vFormat].wEncWidth;
		tJPEGEncOpt.adwUserData[1]  = g_avfInfo[vFormat].wEncHeight;
		scRet = JPEGEnc_SetOptions(hJPEGEncObj, &tJPEGEncOpt);
		assert(scRet == S_OK);
		MOT_DP("%s Change JPEG Input size %ux%u\n", MOT_PRE, 
				tJPEGEncOpt.adwUserData[0], tJPEGEncOpt.adwUserData[1]);        
	}
#endif
	return S_OK;
}

/* ========================================================================== */
#ifdef _VMA_JEBE
SCODE VencMotion_Encoder(TMotionInfo *ptInfo, TSharedBuffMgrState *ptState)
{
	FILE                *pfJPEGOut      = NULL;
	TVideoFrameBuffer   *ptVFB          = NULL;
	SCODE                scRet          = S_FAIL;
	TJPEGEncOptions      tJPEGEncOpt;

	ptVFB     = (TVideoFrameBuffer *)(ptState->pbyHdrAddr);
	pfJPEGOut = fopen(ptInfo->szSnapPath, "wb");  
	if (pfJPEGOut == NULL) {
		fprintf(stderr, "%s Err: fopen \"%s\": (%d) %s\n", 
				MOT_PRE, ptInfo->szSnapPath, errno, strerror(errno));
		scRet = S_FAIL;
	}
	else if (ptInfo->hJPEGEncObj == NULL) {           //! && (pfJPEGOut != NULL)
		BYTE abyFakeContent[128];

		memset(abyFakeContent, 0xff, 128);
		fwrite(abyFakeContent, sizeof(BYTE),128, pfJPEGOut);        
		fclose(pfJPEGOut);
		scRet = S_OK;
	}
	else {              //! (ptInfo->hJPEGEncObj != NULL) && (pfJPEGOut != NULL)
		//! set input buffer to JPEG encoder
		tJPEGEncOpt.eOptionFlags    = VIDEO_CHANGE_FRAME_BUFFER;
		tJPEGEncOpt.adwUserData[0]  = (DWORD)ptVFB->pbyBufY;
		tJPEGEncOpt.adwUserData[1]  = (DWORD)ptVFB->pbyBufU;
		tJPEGEncOpt.adwUserData[2]  = (DWORD)ptVFB->pbyBufV;
		scRet = JPEGEnc_SetOptions(ptInfo->hJPEGEncObj, &tJPEGEncOpt);
		assert(scRet == S_OK);

		//! set output buffer to JPEG encoder
		tJPEGEncOpt.eOptionFlags   = VIDEO_CHANGE_BITSTREAM_BUFFER;
		tJPEGEncOpt.adwUserData[0] = (DWORD)ptInfo->pbyEncOutBuf;
		tJPEGEncOpt.adwUserData[1] = ptInfo->_tInitInfo.dwBitStreamSize;
		scRet = JPEGEnc_SetOptions(ptInfo->hJPEGEncObj, &tJPEGEncOpt);
		assert(scRet == S_OK);

		//! encode one frame
		{
			TJPEGEncState tJPEGEncState;

			memset(&tJPEGEncState, 0, sizeof(TJPEGEncState));
			scRet = JPEGEnc_OneFrame(ptInfo->hJPEGEncObj, &tJPEGEncState);
			if (scRet == S_OK) {
				fwrite(ptInfo->pbyEncOutBuf, 
						sizeof(BYTE), 
						tJPEGEncState.dwEncSize, 
						pfJPEGOut);
			} 
			else {
				fprintf(stderr, "%s Err: JPEGEnc_OneFrame - 0x%08x\n", 
						MOT_PRE, scRet);
			}
		}
		fclose(pfJPEGOut);
	}
	return scRet;
}
#endif
/* ========================================================================== */
SCODE VencMotion_SendToEvent(TMotionInfo *ptInfo, TSharedBuffMgrState *ptState)
{
	TVideoFrameBuffer   *ptVFB          = NULL;
	TUserData           *ptUserData     = NULL;
	TMotionResultInfo   *ptMtnResInfo   = NULL;
	DWORD               dwSecond        = 0xFFFFFFFF;
	DWORD               dwMotionNum     = 0xFFFFFFFF;
	DWORD               dwIndex         = 0xFFFFFFFF;
	BOOL                bShouldSend     = FALSE;
	struct iovec        iov[2];
	struct msghdr       MsgHeader;
	static DWORD        s_dwPreSec      = 0;
	struct tm           *ptmVFB;
	int                 iRet            = -1;

	ptVFB        = (TVideoFrameBuffer *)(ptState->pbyHdrAddr);
	ptUserData   = 
		(TUserData *)( (ptState->pbyHdrAddr) + sizeof(TVideoFrameBuffer) );
	ptMtnResInfo = (TMotionResultInfo *)( ptUserData + sizeof(TUserData) );
	dwMotionNum  = ptUserData->dwMotionNum;
	dwSecond     = ptVFB->dwMDSecond;

	//! dwSecond must larger then s_dwPreSec, or it needn't send it again.
	if (dwSecond <= s_dwPreSec) return S_FAIL;

	//! verify if there is a motion, if yes, encode to JPEG and write to event 
	//! socket.
	if (ptUserData->dwUserDataLength > 0 && dwMotionNum > 0) {
		for (dwIndex = 0; dwIndex < dwMotionNum; dwIndex++) {
			if (ptMtnResInfo->byPercent > ptMtnResInfo->byThreshold) {
				bShouldSend = TRUE;
				break;
			}
			ptMtnResInfo++;
		}
	}

	if (bShouldSend == TRUE) {
		ptmVFB = localtime( (time_t *)&(dwSecond) );
#ifdef _VMA_JEBE
		strftime(ptInfo->szSnapPath, 
				sizeof(ptInfo->szSnapPath), 
				"/tmp/event/Event-%F-%H-%M-%S.jpg", 
				ptmVFB);
#else
		strftime(ptInfo->szSnapPath, 
				sizeof(ptInfo->szSnapPath), 
				"/tmp/event/Event-%F-%H-%M-%S.bin", 
				ptmVFB);
#endif
		MsgHeader.msg_name          = &(ptInfo->tEvtSckInfo.sunx);
		MsgHeader.msg_namelen       = sizeof(ptInfo->tEvtSckInfo.sunx);
		MsgHeader.msg_iov           = iov;
		MsgHeader.msg_iovlen        = 2;
		MsgHeader.msg_control       = NULL;
		MsgHeader.msg_controllen    = 0;
		MsgHeader.msg_flags         = 0;

		iov[0].iov_base = &dwSecond;
		iov[0].iov_len  = sizeof(DWORD);
		iov[1].iov_base = ptInfo->szSnapPath;
		iov[1].iov_len  = strlen(ptInfo->szSnapPath);
		iRet = sendmsg(ptInfo->tEvtSckInfo.fdSock, &MsgHeader, 0); 
		if (iRet != (int)(iov[0].iov_len + iov[1].iov_len)) {
			fprintf(stderr, "%s Err: sendmsg to event: (%d) %s\n", 
					MOT_PRE, errno, strerror(errno));
			return S_FAIL;
		}
		s_dwPreSec = dwSecond;                //! record the latest motion time.
		return S_OK;
	}

	return S_FAIL;
}

/* ========================================================================== */
SCODE VencMotion_PackMediaData(TMotionInfo          *ptInfo, 
		TSharedBuffMgrState  *ptState)
{
	TUBuffer            tUBuffer;
	TVideoFrameBuffer   *ptVFB          = NULL;
	DWORD               dwSecond;
	DWORD               dwMilliSecond;	
	TUserData           *ptUserData     = NULL;
	TMotionResultInfo   *ptMtnResInfo   = NULL;
	DWORD               dwMotionNum;
	DWORD               dwIndex;
	BOOL 				bMotionEnable = FALSE;

	struct tm           *ptm;
	time_t              timeTmp;
	char                buffer[80];
	char                UTCTime[80];
	char                szHead[4096];
	char                szData[4096];
	int                 iszHeadLen;
	struct iovec        iov[3];
	struct msghdr       MsgHeader;
	int                 iWrite;
#if 1
	static DWORD dwPreSecond = 0;
	static DWORD dwPreMilliSecond = 0;
#endif	
	ptVFB         = (TVideoFrameBuffer *)(ptState->pbyHdrAddr);
	
	if (ptInfo->tRtspSckInfo.bNeedConf == TRUE) {
		VencMotion_WriteConf(ptInfo);
		if (ptInfo->tRtspSckInfo.bNeedConf == TRUE) return S_FAIL;
	}

	ptUserData   = (TUserData *) ((ptState->pbyHdrAddr) + sizeof(TVideoFrameBuffer));
	ptMtnResInfo = (TMotionResultInfo *) (ptUserData + sizeof(TUserData));
	dwMotionNum  = ptUserData->dwMotionNum;
	bMotionEnable = ptUserData->bMotionEnable;
#if 1
	
	dwSecond      = ptVFB->dwMDSecond;
	dwMilliSecond = ptVFB->dwMDMilliSecond;
	
	{
		SDWORD sdwDiff = ((SDWORD)dwSecond - (SDWORD)dwPreSecond) * 1000+ ((SDWORD)dwMilliSecond - (SDWORD)dwPreMilliSecond);
		
		dwPreSecond = dwSecond;
		dwPreMilliSecond = dwMilliSecond;
		
		if (sdwDiff < 0)
		{
//			printf("[venc_motion_process](%d)two frame time diff < 0\n",__LINE__);
		}
	}
#endif	
	
#if 0    
	//printf("dwUserDataLength = %u, dwMotionNum = %u !!\n", 
	//       ptUserData->dwUserDataLength, ptUserData->dwMotionNum);
	if (dwMotionNum > 0)
		printf("======= motion info ======== \n");
	for (dwIndex = 0; dwIndex < dwMotionNum; dwIndex++) {
		printf("addr = %p, Index = %u, Left = %u, Top = %u, Right = %u, "
				"Bottom = %u ,byPercent = %u!!\n",
				&(ptMtnResInfo->byPercent),
				ptMtnResInfo->dwIndex,
				ptMtnResInfo->dwLeft,
				ptMtnResInfo->dwTop,
				ptMtnResInfo->dwRight,
				ptMtnResInfo->dwBottom,
				ptMtnResInfo->byPercent);
		ptMtnResInfo++;
	}
	if (dwMotionNum > 0)
		printf("======= motion info End======== \n");
#endif //! 0

	if ( (ptUserData->dwUserDataLength > 0) && (dwMotionNum > 0) ) {
		//! compose the UTC time, the result is at UTCTime
		timeTmp = (time_t)(dwSecond);
		ptm = gmtime(&timeTmp);
		//strftime(buffer, 80, "%Y-%m-%dT%X." ,ptm);
		strftime(buffer, 80, "%X." ,ptm);
		sprintf(UTCTime, "%s%03u", buffer, dwMilliSecond);

		//! head
		sprintf(szHead, 
				"<?xml version=\"1.0\"?>\n"
				"<wsnt:NotificationMessage\n"
				"xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\"\n"
				"xmlns:tns1=\"http://www.onvif.org/ver10/topics\"\n"
				"xmlns:tt=\"http://www.onvif.org/ver10/schema\"\n"
				"xmlns:vat=\"http://www.vtcs.com/ver10/schema\">\n"
				"<wsnt:Topic Dialect=\"http://docs.oasis-open.org/wsn/t-1/TopicExpression/Concrete\">\n"
				"tns1:RuleEngine/MotionDetector/StatusReport\n"
				"</wsnt:Topic>\n"
				"<wsnt:Message>\n"
				"<tt:Message UtcTime=\"%s\">\n"
				"<tt:Source>\n"
				"<tt:SimpleItem Name=\"MDConfigurationToken\" Value=\"MDConfig1\"/>\n"
				"</tt:Source>\n"
				"<tt:Data>\n"
				"<tt:ElementItem Name=\"MotionData\">\n"
				"<vat:MotionData>\n"
				"<vat:Transformation>\n"
				"<tt:Translate x=\"%.2f\" y=\"%.2f\"/>\n"
				"<tt:Scale x=\"%.8f\" y=\"%.8f\"/>\n"
				"</vat:Transformation>\n", 
			UTCTime, 
			ptUserData->tMtnR2D.fXTranslate, 
			ptUserData->tMtnR2D.fYTranslate, 
			ptUserData->tMtnR2D.fXScale, 
			ptUserData->tMtnR2D.fYScale);

		//! data
		for (dwIndex = 0; dwIndex < dwMotionNum; dwIndex++) {
			//! dwIndex should be a motion.
			sprintf(szData, 
					"<vat:IncludeWindow id=\"%u\">\n"
					"<vat:Percent>%d</vat:Percent>\n"
					"<vat:Threshold>%d</vat:Threshold>\n"
					"<vat:BoundingBox left=\"%u\" top=\"%u\" right=\"%u\" bottom=\"%u\"/>\n"
					"</vat:IncludeWindow>\n",
					ptMtnResInfo->dwIndex, 
					ptMtnResInfo->byPercent,
					ptMtnResInfo->byThreshold,
					ptMtnResInfo->dwLeft,
					ptMtnResInfo->dwTop,
					ptMtnResInfo->dwRight,
					ptMtnResInfo->dwBottom);
			ptMtnResInfo++;
			strcat(szHead, szData);
		}
		//! tail
		strcat(szHead,     
				"</vat:MotionData>\n"
				"</tt:ElementItem>\n"
				"</tt:Data>\n"
				"</tt:Message>\n"
				"</wsnt:Message>\n"
				"</wsnt:NotificationMessage>");
		iszHeadLen = strlen(szHead);
		//printf("%s, len = %d\n", szHead, iszHeadLen);

		//! compose to ubuffer
		memset(&tUBuffer, 0, sizeof(TUBuffer));
		tUBuffer.dwSize         = sizeof(TUBuffer)+iszHeadLen;
		tUBuffer.dwDataType     = FOURCC_MDAT;
		tUBuffer.bIsSync        = 1;
		tUBuffer.bIsBoundary    = 1;
		tUBuffer.dwSec          = dwSecond;
		tUBuffer.dwUSec         = dwMilliSecond * 1000;

		MsgHeader.msg_name          = &(ptInfo->tRtspSckInfo.sunx);
		MsgHeader.msg_namelen       = sizeof(ptInfo->tRtspSckInfo.sunx);
		MsgHeader.msg_iov           = iov;
		MsgHeader.msg_iovlen        = 2;
		MsgHeader.msg_control       = NULL;
		MsgHeader.msg_controllen    = 0;
		MsgHeader.msg_flags         = 0;

		iov[0].iov_base = &tUBuffer;
		iov[0].iov_len  = sizeof(TUBuffer);
		iov[1].iov_base = szHead;
		iov[1].iov_len  = strlen(szHead);
		iWrite = sendmsg(ptInfo->tRtspSckInfo.fdSock, &MsgHeader, 0);
		if ( iWrite < (int)(sizeof(TUBuffer) + strlen(szHead)) ) {
			fprintf(stderr, "%s Err: sendmsg to RTSP(motion data): (%d) %s\n", 
					MOT_PRE, errno, strerror(errno));
			if (iWrite == -1) ptInfo->tRtspSckInfo.bNeedConf = TRUE;
		}
	} //! if ( (ptUserData->dwUserDataLength > 0) && (dwMotionNum > 0) )
#if 1
	else
	{
		if (bMotionEnable == TRUE)
		{
			//! compose the UTC time, the result is at UTCTime
			timeTmp = (time_t)(dwSecond);
			ptm = gmtime(&timeTmp);
			//strftime(buffer, 80, "%Y-%m-%dT%X." ,ptm);
			strftime(buffer, 80, "%X." ,ptm);
			sprintf(UTCTime, "%s%03u", buffer, dwMilliSecond);

			//! head
			sprintf(szHead, 
					"<?xml version=\"1.0\"?>\n"
					"<wsnt:NotificationMessage\n"
					"xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\"\n"
					"xmlns:tns1=\"http://www.onvif.org/ver10/topics\"\n"
					"xmlns:tt=\"http://www.onvif.org/ver10/schema\"\n"
					"xmlns:vat=\"http://www.vtcs.com/ver10/schema\">\n"
					"<wsnt:Topic Dialect=\"http://docs.oasis-open.org/wsn/t-1/TopicExpression/Concrete\">\n"
					"tns1:RuleEngine/MotionDetector/StatusReport\n"
					"</wsnt:Topic>\n"
					"<wsnt:Message>\n"
					"<tt:Message UtcTime=\"%s\">\n"
					"<tt:Source>\n"
					"<tt:SimpleItem Name=\"MDConfigurationToken\" Value=\"MDConfig1\"/>\n"
					"</tt:Source>\n"
					"<tt:Data>\n"
					"<tt:ElementItem Name=\"MotionData\">\n"
					"<vat:MotionData>\n"
					"<vat:Transformation>\n"
					"<tt:Translate x=\"%.2f\" y=\"%.2f\"/>\n"
					"<tt:Scale x=\"%.8f\" y=\"%.8f\"/>\n"
					"</vat:Transformation>\n", 
				UTCTime, 
				ptUserData->tMtnR2D.fXTranslate, 
				ptUserData->tMtnR2D.fYTranslate, 
				ptUserData->tMtnR2D.fXScale, 
				ptUserData->tMtnR2D.fYScale);
			//! tail
			strcat(szHead,     
					"</vat:MotionData>\n"
					"</tt:ElementItem>\n"
					"</tt:Data>\n"
					"</tt:Message>\n"
					"</wsnt:Message>\n"
					"</wsnt:NotificationMessage>");
			iszHeadLen = strlen(szHead);

			//printf("%s, len = %d\n", szHead, iszHeadLen);

			//! compose to ubuffer
			memset(&tUBuffer, 0, sizeof(TUBuffer));
			tUBuffer.dwSize         = sizeof(TUBuffer)+iszHeadLen;
			tUBuffer.dwDataType     = FOURCC_MDAT;
			tUBuffer.bIsSync        = 1;
			tUBuffer.bIsBoundary    = 1;
			tUBuffer.dwSec          = dwSecond;
			tUBuffer.dwUSec         = dwMilliSecond * 1000;

			MsgHeader.msg_name          = &(ptInfo->tRtspSckInfo.sunx);
			MsgHeader.msg_namelen       = sizeof(ptInfo->tRtspSckInfo.sunx);
			MsgHeader.msg_iov           = iov;
			MsgHeader.msg_iovlen        = 2;
			MsgHeader.msg_control       = NULL;
			MsgHeader.msg_controllen    = 0;
			MsgHeader.msg_flags         = 0;

			iov[0].iov_base = &tUBuffer;
			iov[0].iov_len  = sizeof(TUBuffer);
			iov[1].iov_base = szHead;
			iov[1].iov_len  = strlen(szHead);
			iWrite = sendmsg(ptInfo->tRtspSckInfo.fdSock, &MsgHeader, 0);
			if ( iWrite < (int)(sizeof(TUBuffer) + strlen(szHead)) ) {
				fprintf(stderr, "%s Err: sendmsg to RTSP(motion data): (%d) %s\n", 
						MOT_PRE, errno, strerror(errno));
				if (iWrite == -1) ptInfo->tRtspSckInfo.bNeedConf = TRUE;
			}
		}

	}
#endif	

	return S_OK;
}


#if defined(_MD_TRIGER_ROI_)

#define H264_MACRO_BLK_WIDTH 16
#define H264_MACRO_BLK_HEIGHT  16
#define H264_MACRO_BLK_SIZE (H264_MACRO_BLK_WIDTH*H264_MACRO_BLK_HEIGHT)
#define ROI_WIN_NUM 8
typedef struct roi_mm_time {
    DWORD   dwMotionSec;
    DWORD   dwMotionMilliSec;
    DWORD   dwCfgLeft;
    DWORD   dwCfgTop;
    DWORD   dwCfgWidth;
    DWORD   dwCfgHeight;
	
	DWORD dwROIX;
	DWORD dwROIY;
	DWORD dwROIWidth;
	DWORD dwROIHeight;
	
    BOOL    bEnable;
} TROIMotion;

typedef struct roi_dealwith {
    DWORD   dwTotalNum;
    BOOL    bAnyOneEnable;
    DWORD   dwMotionIndex[ROI_WIN_NUM];
    DWORD   dwWhatToDo[ROI_WIN_NUM];     // 1: enable, 2: disable
} TROIResult;

#define CMD_FIFO_PATH "/tmp/venc/c0/command.fifo"
static SCODE creat_cmdfifo(const char *szCmdFiFoPath, int *piCmdFiFo)
{
		
	SCODE scRet = S_OK;
	
	if (access(szCmdFiFoPath, F_OK) == -1)
	{
			if (mkfifo(szCmdFiFoPath, 0777) != 0)
			{
				printf("[venc_motion_process](%d)  Could not create fifo %s\n", __LINE__, szCmdFiFoPath);
				scRet = S_FAIL;
				goto exit;
			}
	}

	*piCmdFiFo = open(szCmdFiFoPath, O_WRONLY);//|O_NONBLOCK);
	if (*piCmdFiFo < 0)
	{
			printf("[venc_motion_process](%d) open fifo %s failed: %d\r\n", __LINE__,szCmdFiFoPath, errno);
			scRet = S_FAIL;
			goto exit;
	}
	printf("[venc_motion_process](%d) open fifo %s ok: %d\r\n", __LINE__, szCmdFiFoPath, *piCmdFiFo);
	
exit:

	return scRet;
	
}

static SCODE send_cmdfifo(DWORD dwEncoderID,char *szCmd,int iCmdFiFo)
{
	CHAR		acTemp[512];
	int iLen;
	
	SCODE scRet = S_OK;
	
	memset(acTemp,0x0,sizeof(acTemp));
	
	acTemp[0] = 1;
	// force it using a long mode
	acTemp[1] = 0x84;

	sprintf(&(acTemp[6]), "%s<host>encoder%d</host>%s%s%s%s", 
									"<control><request>", 
									dwEncoderID, 
									"<content>",
									szCmd,
									"</content>",
									"</request></control>");
	iLen = strlen(&(acTemp[6]));
	acTemp[5] =  iLen >> 24;
	acTemp[4] = (iLen & 0x00FF0000) >> 16;
	acTemp[3] = (iLen & 0x0000FF00) >> 8;
	acTemp[2] = (iLen & 0x000000FF);
	if (iCmdFiFo != -1)
	{
		write(iCmdFiFo, acTemp, iLen+6);		
	}
	else
	{
		scRet = S_FAIL;
	}
	
	return scRet;	
}

#define SHARED_MEM_PATH_NAME "md_trigger_roi_shm"
#define SHARED_MEM_LEN 8192 /*The SHARED_MEM_LEN in the venc_encoder_process.c must be modified if this is modified.*/
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
static SCODE create_shm(void)
{
  	int	fd, flags;
	char	*ptr;
	off_t	length;

	flags = O_RDWR | O_CREAT;
	
	length = SHARED_MEM_LEN;
	
	fd = shm_open(SHARED_MEM_PATH_NAME, flags, FILE_MODE);
	if (fd == -1)
	{
	  printf("[venc_motion_process](%d)%s\n",__LINE__,strerror(errno));
	  return S_FAIL;
	}
	ftruncate(fd, length);

	ptr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	close(fd);
	
	munmap(ptr, length);
	
	return S_OK;
}
static SCODE unlink_shm(void)
{    
    if (shm_unlink(SHARED_MEM_PATH_NAME) == -1){
	  printf("[venc_motion_process](%d)%s\n",__LINE__,strerror(errno));
	  return S_FAIL;
    }
    return S_OK;
}
static SCODE write_shm(const char *szContent)
{
  	int		i, fd;
	struct stat	stat;
	char	*ptr;
	
	fd = shm_open(SHARED_MEM_PATH_NAME, O_RDWR, FILE_MODE);
	
	if (fd == -1)
	{
	  printf("[venc_motion_process](%d)%s\n",__LINE__,strerror(errno));
	  return S_FAIL;
	}
	fstat(fd, &stat);
	ptr = mmap(NULL, stat.st_size, PROT_READ | PROT_WRITE,
			   MAP_SHARED, fd, 0);
	close(fd);
	
	for (i = 0; i < stat.st_size; i++)
		ptr[i] = szContent[i];
	
	munmap(ptr,stat.st_size);
	
	return S_OK;
}
static SCODE DoROI(TROIMotion *ptROIMotion, 
				   TMotionInfo *ptInfo, 
				   DWORD dwROIIndex,
				   BOOL bEnable,
				   SDWORD sdwDeltaQp,
				   DWORD dwEncodingInterval,
				   DWORD dwROIPlaneWidth,
				   DWORD dwROIPlaneHeight
  				)
{
	TROISelectorInitOptions tInitOptions;
    TROISelectorState tState;
    HANDLE hROIObject;
	DWORD   dwWidthPad = ((dwROIPlaneWidth + 15) >> 4) << 4;
	DWORD   dwHeightPad = ((dwROIPlaneHeight + 15) >> 4) << 4;
	DWORD dwROIMacroBlkNumForEachRegion = (dwWidthPad*dwHeightPad)/(H264_MACRO_BLK_SIZE);
	SCODE scRet = S_OK;
	DWORD dwIndex = 0;
	DWORD dwEndMBIndex = 0;
	DWORD dwStartMBIndex = 0;
	
	DWORD dwStartMBXNum = 0;
	DWORD dwStartMBYNum = 0;
	DWORD dwEndMBXNum = 0;
	DWORD dwEndMBYNum = 0;
	
	if (bEnable == TRUE)
	{	
		printf("[venc_motion_process](%d)Enable ROI at win %d\n",__LINE__,dwROIIndex);
	}
	else
	{
		printf("[venc_motion_process](%d)Disable ROI at win %d\n",__LINE__,dwROIIndex);
	}

	{
		tInitOptions.dwVersion = ROISELECTOR_VERSION;
		tInitOptions.dwFrameWidth = dwWidthPad;
		tInitOptions.dwFrameHeight = dwHeightPad;
		tInitOptions.dwMacroBlockWidth = H264_MACRO_BLK_WIDTH;
		tInitOptions.dwMacroBlockHeight = H264_MACRO_BLK_HEIGHT;
		
		if (ROISelector_Initial(&hROIObject, &tInitOptions) != S_OK)
		{
			printf("[venc_motion_process](%d)Initialize ROI Selector object fail !!\n",__LINE__);
			scRet = S_FAIL;
			goto exit;
		}
		
		tState.atROIRectangle[0].dwX = ptROIMotion->dwROIX;
		tState.atROIRectangle[0].dwY = ptROIMotion->dwROIY;
		tState.atROIRectangle[0].dwWidth = ptROIMotion->dwROIWidth;
		tState.atROIRectangle[0].dwHeight = ptROIMotion->dwROIHeight;
		tState.apdwBlockIndex[0] = (DWORD *)malloc(sizeof(DWORD)*dwROIMacroBlkNumForEachRegion);
		
		tState.dwRecNum = 1;
		
		if (hROIObject != NULL)
		{
			ROISelector_OneFrame(hROIObject, &tState);
		}
		else
		{	printf("[venc_motion_process](%d)Initialize ROI HANDLE is NULL !!\n",__LINE__);
			scRet = S_FAIL;
			goto exit;
		}
		
		dwEndMBIndex = (DWORD)(*(tState.apdwBlockIndex[0]+tState.adwTotalIndexNum[0]-1));
		dwStartMBIndex = (DWORD)(*tState.apdwBlockIndex[0]);
		dwStartMBXNum = (DWORD)(dwStartMBIndex % (dwWidthPad/H264_MACRO_BLK_WIDTH));
		dwStartMBYNum = (DWORD)(dwStartMBIndex / (dwWidthPad/H264_MACRO_BLK_WIDTH));
		dwEndMBXNum = (DWORD)(dwEndMBIndex % (dwWidthPad/H264_MACRO_BLK_WIDTH));
		dwEndMBYNum = (DWORD)(dwEndMBIndex / (dwWidthPad/H264_MACRO_BLK_WIDTH));
		
	}	
	{	
		CHAR szResult[SHARED_MEM_LEN];
		CHAR szReadBuf[SHARED_MEM_LEN];
		
		memset(szResult,0x0,sizeof(szResult));
		memset(szReadBuf,0x0,sizeof(szReadBuf));
		
		snprintf(szResult,sizeof(szResult),"<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?><root>"
											"<enable>%u</enable>"
											"<id>%u</id>"
											"<startMBX>%u</startMBX>"
											"<startMBY>%u</startMBY>"
											"<endMBX>%u</endMBX>"
											"<endMBY>%u</endMBY>"
											"<deltaQ>%d</deltaQ>"
											"<interval>%u</interval>"
											"</root>"
											,bEnable,dwROIIndex,dwStartMBXNum,dwStartMBYNum,dwEndMBXNum,dwEndMBYNum,sdwDeltaQp,dwEncodingInterval);
		
		if (write_shm(szResult) == S_FAIL)
		{
			printf("[venc_motion_process][%d]Can not write data to %s\n",__LINE__,SHARED_MEM_PATH_NAME);
		}
		else
		{
			send_cmdfifo(0,"setROI",ptInfo->iCmdFiFo);
		}
		
	}
	
exit:
{
	if (hROIObject != NULL)
	{
		if (ROISelector_Release(&hROIObject) != S_OK)
		{
			printf("[venc_motion_process](%d)Release ROI Selector object fail !!\n",__LINE__);
			scRet = S_FAIL;
		}
	}

	for (dwIndex = 0; dwIndex < tState.dwRecNum; dwIndex++)
	{
		if (tState.apdwBlockIndex[dwIndex]!= NULL)
		{
			free(tState.apdwBlockIndex[dwIndex]);
			tState.apdwBlockIndex[dwIndex]=NULL;
		}
	}
}	

	return scRet;
	
}

SCODE VencMotion_TriggerROI(TMotionInfo *ptInfo, TSharedBuffMgrState  *ptState)
{
	TVideoFrameBuffer   *ptVFB          = NULL;
	DWORD               dwSecond;
	DWORD               dwMilliSecond;
	TUserData           *ptUserData     = NULL;
	TMotionResultInfo   *ptMtnResInfo   = NULL;
	DWORD               dwMotionNum;
	DWORD               dwIndex;
    static BOOL bFirst = TRUE;    
    int i = 0;

	static TROIMotion tROIMotion[ROI_WIN_NUM];
	BOOL bROIEnable = FALSE;
    TROIResult  tROIResult;

	static SDWORD sdwPreDeltaQp = 0;
	static DWORD dwPreEncodingInterval = 0;
	static DWORD dwPreROIDuration = 0;
	static DWORD dwPreROIPlaneWidth = 0;
	static DWORD dwPreROIPlaneHeight = 0;

	SDWORD sdwDeltaQp = ptInfo->sdwROIDeltaQp;
	DWORD dwEncodingInterval = ptInfo->dwROIEncodingInterval;
	DWORD dwROIDuration = ptInfo->dwROIDuration;
	DWORD dwROIPlaneWidth = 0;
	DWORD dwROIPlaneHeight = 0;
		
    if (bFirst == TRUE) {
		for (i = 0; i < ROI_WIN_NUM; i++)
		{
            tROIMotion[i].dwMotionSec = 0;
            tROIMotion[i].dwMotionMilliSec = 0;
            tROIMotion[i].dwCfgLeft = 0;
            tROIMotion[i].dwCfgTop = 0;
            tROIMotion[i].dwCfgWidth = 0;
            tROIMotion[i].dwCfgHeight = 0;
            tROIMotion[i].bEnable = FALSE;
			tROIMotion[i].dwROIX = 0;
			tROIMotion[i].dwROIY = 0;
			tROIMotion[i].dwROIWidth = 0;
			tROIMotion[i].dwROIHeight = 0;
        }
        bFirst = FALSE;
    }    

    tROIResult.dwTotalNum = 0;
    tROIResult.bAnyOneEnable = FALSE;

	ptVFB         = (TVideoFrameBuffer *)(ptState->pbyHdrAddr);
	dwSecond      = ptVFB->dwMDSecond;
	dwMilliSecond = ptVFB->dwMDMilliSecond;
	
	dwROIPlaneWidth = g_avfInfo[ptVFB->vFormat].wEncWidth;
	dwROIPlaneHeight = g_avfInfo[ptVFB->vFormat].wEncHeight;
	
	if ((sdwPreDeltaQp != sdwDeltaQp) ||
		(dwPreEncodingInterval != dwEncodingInterval) ||
		(dwPreROIDuration != dwROIDuration) ||
		(dwPreROIPlaneWidth != dwROIPlaneWidth) ||
		(dwPreROIPlaneHeight != dwROIPlaneHeight)
	)
	{
		printf("[venc_motion_process](%d)(deltaQ,interval,w,h,duration)=(%d,%u,%u,%u,%u)\n",__LINE__,sdwDeltaQp,dwEncodingInterval,dwROIPlaneWidth,dwROIPlaneHeight,dwROIDuration);
		sdwPreDeltaQp = sdwDeltaQp;
		dwPreEncodingInterval = dwEncodingInterval;
		dwPreROIDuration = dwROIDuration;
		dwPreROIPlaneWidth = dwROIPlaneWidth;
		dwPreROIPlaneHeight = dwROIPlaneHeight;
	}

	ptUserData   = (TUserData *) ((ptState->pbyHdrAddr) + sizeof(TVideoFrameBuffer));
	ptMtnResInfo = (TMotionResultInfo *) (ptUserData + sizeof(TUserData));
	dwMotionNum  = ptUserData->dwMotionNum;
	// check motion first
	ptMtnResInfo = (TMotionResultInfo *) (ptUserData + sizeof(TUserData));
	for (dwIndex = 0; dwIndex < dwMotionNum; dwIndex++) 
	{
	    if (ptMtnResInfo->dwIndex < ROI_WIN_NUM) {
    	    if ((tROIMotion[ptMtnResInfo->dwIndex].bEnable == FALSE) || 
    	        (tROIMotion[ptMtnResInfo->dwIndex].dwCfgLeft != ptMtnResInfo->dwCfgLeft) ||
    	        (tROIMotion[ptMtnResInfo->dwIndex].dwCfgTop != ptMtnResInfo->dwCfgTop) ||
   	            (tROIMotion[ptMtnResInfo->dwIndex].dwCfgWidth != ptMtnResInfo->dwCfgWidth) ||
   	            (tROIMotion[ptMtnResInfo->dwIndex].dwCfgHeight != ptMtnResInfo->dwCfgHeight)){   // it need to enable ROI.
    	        
    	        tROIMotion[ptMtnResInfo->dwIndex].bEnable = TRUE;
    	        tROIMotion[ptMtnResInfo->dwIndex].dwCfgLeft = ptMtnResInfo->dwCfgLeft;
    	        tROIMotion[ptMtnResInfo->dwIndex].dwCfgTop = ptMtnResInfo->dwCfgTop;
    	        tROIMotion[ptMtnResInfo->dwIndex].dwCfgWidth = ptMtnResInfo->dwCfgWidth;
    	        tROIMotion[ptMtnResInfo->dwIndex].dwCfgHeight = ptMtnResInfo->dwCfgHeight;
				tROIMotion[ptMtnResInfo->dwIndex].dwROIX = ptMtnResInfo->dwLeft;
				tROIMotion[ptMtnResInfo->dwIndex].dwROIY = ptMtnResInfo->dwTop;
				tROIMotion[ptMtnResInfo->dwIndex].dwROIWidth = ptMtnResInfo->dwRight-ptMtnResInfo->dwLeft;
				tROIMotion[ptMtnResInfo->dwIndex].dwROIHeight = ptMtnResInfo->dwBottom-ptMtnResInfo->dwTop;
				
                tROIResult.dwMotionIndex[tROIResult.dwTotalNum] = ptMtnResInfo->dwIndex;
                tROIResult.dwWhatToDo[tROIResult.dwTotalNum] = 1;
                tROIResult.bAnyOneEnable = TRUE;
                tROIResult.dwTotalNum++;
    	    }
    	    
    	    // update motion sec.
    	    tROIMotion[ptMtnResInfo->dwIndex].dwMotionSec = dwSecond;
    	    tROIMotion[ptMtnResInfo->dwIndex].dwMotionMilliSec = dwMilliSecond;
    	}
        ptMtnResInfo++;
	}

    // check if any motion time over 1 min.
    for (i = 0; i < ROI_WIN_NUM; i++) 
	{
        if (tROIMotion[i].bEnable == TRUE) {
			//printf("[venc_motion_process](%d)tROIMotion[%d] is enable.\n",__LINE__,i);
            if ((tROIMotion[i].dwMotionSec + dwROIDuration) < dwSecond) 
			{   // disable this ROI
                tROIMotion[i].bEnable = FALSE;
                tROIMotion[i].dwMotionSec = 0;
                tROIMotion[i].dwMotionMilliSec = 0;
                tROIResult.dwMotionIndex[tROIResult.dwTotalNum] = i;
                tROIResult.dwWhatToDo[tROIResult.dwTotalNum] = 2;
                tROIResult.dwTotalNum++;
				//printf("[venc_motion_process](%d)ROI win %d => disable ROI\n",__LINE__,i);
				DoROI(&tROIMotion[i],ptInfo,i,FALSE,sdwDeltaQp,dwEncodingInterval,dwROIPlaneWidth,dwROIPlaneHeight);
				//The following delay is a workaround for that increase the successful ratio of cmdfifo sending with multiple motion windows.  
				system("usleep 10000");
				
			}
        }
    }

    if (tROIResult.dwTotalNum > 0) 
	{
        if (tROIResult.bAnyOneEnable == TRUE) {
            bROIEnable = TRUE;
        } else {
            for (i = 0; i < ROI_WIN_NUM; i++) {
                if (tROIMotion[i].bEnable == TRUE) {
                    bROIEnable = TRUE;
                    break;
                }
            }
        }
        for (i = 0; i < tROIResult.dwTotalNum; i++) 
		{
			DWORD id = tROIResult.dwMotionIndex[i];
			
			if (bROIEnable == TRUE)
			{
				if (tROIResult.dwWhatToDo[i] == 1)
				{
					DoROI(&tROIMotion[id],ptInfo,id,TRUE,sdwDeltaQp,dwEncodingInterval,dwROIPlaneWidth,dwROIPlaneHeight);
					//The following delay is a workaround for that increase the successful ratio of cmdfifo sending with multiple motion windows.
					system("usleep 10000");
				}
				
			}	
		}    
    }
	return S_OK;
}
#endif
/* ========================================================================== */
void* VencMotion_Loop(HANDLE hInstance)
{
	TMotionInfo         *ptInfo     = (TMotionInfo *)(hInstance);
	TSharedBuffMgrState *ptState    = NULL;
	EVideoFormat        vFormat;
	TVideoFrameBuffer   *ptVFB      = NULL;
#if defined(_MD_TRIGER_ROI_)
    BOOL bCreateShm = FALSE;
#endif
	vFormat = vfNum;
	syslog(LOG_INFO, "%s venc_motion thread pid: %d\n", MOT_PRE, getpid());
	//! or using default (join)? think about it
	pthread_detach(pthread_self());

	SharedBuffMgr_GetFileDescriptor( ptInfo->hVideoInSrdObj, 
			&(ptInfo->iVideoInSrdFD) );
	
#if defined(_MD_TRIGER_ROI_)	
	if (create_shm() == S_OK)
    {
      bCreateShm = TRUE;
    }
    
    if (creat_cmdfifo(CMD_FIFO_PATH, &(ptInfo->iCmdFiFo)) != S_OK)
	{
		ptInfo->iCmdFiFo = -1;
		printf("[venc_motion_process](%d)Create %s failed!!\n",__LINE__,CMD_FIFO_PATH);
	}
    
#endif

	while(1) {
		sem_wait(&(ptInfo->smStart));
		if (ptInfo->bTerminateThread == TRUE)    break;

		MOT_DP("%s Motion Loop Start\n", MOT_PRE);
		while (ptInfo->bRunning == TRUE) {

			//! bReCfg should set to true when first running
			if (ptInfo->bReCfg == TRUE) {
				sem_wait(&(ptInfo->smOpt));
				VencMotion_LoadConfig(hInstance);
				ptInfo->bReCfg = FALSE;
				sem_post(&(ptInfo->smOpt));
			}
			//! request input you frame from share buffer
			ptState = VencMotion_RequestInputBuf(ptInfo);
			if (ptState == NULL) {
				sched_yield();
				//! TODO: should wait by mutex.
				continue;
			}
#if defined(_MD_TRIGER_ROI_)
				VencMotion_TriggerROI(ptInfo, ptState);
#endif
			if (ptInfo->bSendSckEnable == TRUE) {
				VencMotion_PackMediaData(ptInfo, ptState);
			}
			if (ptInfo->bSendEvtEnable == TRUE) {
				SCODE  scRet;
				ptVFB = (TVideoFrameBuffer *)(ptState->pbyHdrAddr);
				if (vFormat != ptVFB->vFormat) {
#ifdef _VMA_JEBE
					if (ptInfo->hJPEGEncObj != NULL) {
						VencMotion_ReSetEncoder(ptInfo->hJPEGEncObj, 
								ptVFB->vFormat);
					}
#endif
					vFormat = ptVFB->vFormat;
				}
				scRet = VencMotion_SendToEvent(ptInfo, ptState);
				if (scRet == S_OK) {
#ifdef _VMA_JEBE
					VencMotion_Encoder(ptInfo, ptState);
#endif
					;
				}
			} //! if (ptInfo->bSendEvtEnable == TRUE)

			VencMotion_ReleaseInputBuf(ptInfo->hVideoInSrdObj, ptState);
			ptState = NULL;

		} //! while (ptInfo->bRunning == TRUE)

		if (ptState != NULL) {
			VencMotion_ReleaseInputBuf(ptInfo->hVideoInSrdObj, ptState);
			ptState = NULL;
		}
		ptInfo->bExitInnerLoop = TRUE;
		MOT_DP("%s Motion Exit inner loop\n", MOT_PRE);

	}   //! while(1)

	sem_post( &(ptInfo->smEnd) );
#if defined(_MD_TRIGER_ROI_)	
	if (bCreateShm == TRUE)
	{
		unlink_shm();
	}
#endif
	MOT_DP("%s Motion Exit thread\n", MOT_PRE);

	return 0;
}

/* ========================================================================== */
SCODE VencMotion_LoadConfig(HANDLE hObject)
{
	TMotionInfo *ptInfo = (TMotionInfo *)hObject;
	SCODE       scRet   = S_FAIL;

	if (ptInfo == NULL) return S_FAIL;

	MOT_DP("%s %s %s \n", MOT_PRE, __FUNCTION__, ptInfo->szCfgFile);

	scRet = XmlWrapper_ReadFile_UsrDefFunc(ptInfo->szCfgFile, 
			ptInfo->hXMLObj, 
			ptInfo);
	if (scRet != S_OK) {
		fprintf(stderr, "%s Err: XmlWrapper_ReadFile_UsrDefFunc - 0x%08x\n", 
				MOT_PRE, scRet);
		return S_FAIL;
	}

	if (XmlWrapper_Reset(ptInfo->hXMLObj) != S_OK)  {
		MOT_DP("%s Err: XmlWrapper_Reset\n", MOT_PRE);
		return S_FAIL;
	}

	return S_OK;
}

/* ========================================================================== */

