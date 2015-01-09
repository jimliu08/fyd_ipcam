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
 * SharedBuffMgr_App.c
 *
 * \brief
 * A sample code to get sharedbuffer from venc encoder.
 *
 * \date
 * 2010/04/13
 *
 * \author
 * Vincent Ho
 *
 *******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>                 
#include <unistd.h>                
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
//#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
//#include <sys/types.h>

#include "SharedBuffMgr.h"
#include "ubuffer.h"
#include "xmlwrapper.h"
#include "SockSrc.h"

#include "goolink_apiv2.h"
#include "common_def.h"
#include "sensor.h"


typedef unsigned long long u64;

#define VERSION "1.0.0.0"
#define MSGQ_PATH "/tmp/msgq"

static char const rcsid[] = "$Id$";


/* =========================================================================================== */

typedef enum command_type
{
	letNeedIntra,
	letNeedConf,
	letBitstrmStart,
	letBitstrmStop,
	letGetSnapShot
} ECommandType;

/* =========================================================================================== */
SCODE CreateFifo(char *pData, int *piFIFO, int iFlag);
SCODE SendCmdMsg(int iChNo, int iCmdFiFo, ECommandType etype);
SCODE GetTotalStreamNum(char *szVencCfg, DWORD *pdwTotalNum);
BOOL  IsSupportSnapShot(void);

#if 0
int venc_main(int argc, char **argv)
{
	// For SharedBufferMgr
	TSharedBuffMgrInitOptions tSharedBuffMgrInitOpts;
	TSharedBuffMgrState tSharedBuffMgrState;
	HANDLE hSharedBuffMgrObj = NULL;
	int fd = -1;
	DWORD	 dwStreamTotalNum = -1;
	
	// For select()
	int Maxfd = -1;
	fd_set readset;
	struct timeval       timeout;
	int                  iRes = 0;

	// For Command Fifo
	int     iFIFO_fd = -1;
	CHAR	*szFIFOPathName = "/tmp/venc/c0/command.fifo";

	// For parsing UBuffer
	TUBuffer *ptUBuffer = NULL;	
	DWORD dwUBuffPayloadLen;
	BYTE  *pbyUBuffPayload;

	// For dump files
	FILE    *pfH264Output = NULL;
	FILE    *pfMPEG4Output = NULL;
	FILE    *pfJPEGOutput = NULL;
	FILE	*pfSnapShotOutput = NULL;
	CHAR    aszJPEGFilePath[30];
	CHAR    aszDumpPath[2048];
	// General use
	int     istream_no = 0;
	int     iframeno = 0;
	DWORD   dwCodecType = 0;
	BOOL    bNeedIntra = TRUE;
	BOOL    bSendIntraMsg = FALSE;
	int     i = 0;
//	DWORD dwCounter = 0;

	BOOL bDumpBitstream=FALSE; // dump bistream
	
	BOOL bGetSnapShot=FALSE; //dump snapshot

	// Parsing arguments
	if (argc != 5)
	{
		printf("Usage: %s istream_no(0/1/2/3/4) frame_num path bDump(0/1/2)\n", argv[0]);
		exit(1);
	}
	istream_no = atoi(argv[1]);
	iframeno = atoi(argv[2]);

	memset(aszDumpPath,0x0,sizeof(aszDumpPath));
	if(argv[3]!=NULL)
	{
		strncpy(aszDumpPath,argv[3],strlen(argv[3]));
	}
	else
	{
		printf("Dump path is NULL!!\n");
		exit(0);
	}
	
	if (GetTotalStreamNum("/etc/conf.d/c0/venc_conf.xml", &dwStreamTotalNum) != S_OK)
	{
		printf("Parse /etc/conf.d/c0/venc_conf.xml to get total stream number failed!!\n");
		exit(0);
	}
	else
	{
		printf("Total Stream number is %d\n",dwStreamTotalNum);
	}
	
	if (istream_no >= dwStreamTotalNum)
	{
		printf("The istream_no %d is larger than total stream number %d\n", istream_no ,dwStreamTotalNum);
		exit(0);
	}
	
	if (atoi(argv[4])==1)
	{
		bDumpBitstream=TRUE;
	}
	else if(atoi(argv[4])==2)
	{
		bGetSnapShot=TRUE;
	}
	
	printf("venc_receiver version %s %s\n", rcsid,VERSION);
	if (bDumpBitstream)
	{
		if ((pfH264Output=fopen(aszDumpPath, "wb")) == NULL)
		{
			printf("Open output file %s fail !!\n",aszDumpPath);
			perror("error");
			goto exit;
		}
		if ((pfMPEG4Output=fopen(aszDumpPath, "wb")) == NULL)
		{
			printf("Open output file %s fail !!\n",aszDumpPath);
			perror("error");
			goto exit;
		}
	}
	
	if (bGetSnapShot)
	{
		iframeno  = 1;
		if (IsSupportSnapShot() == FALSE)
		{
			printf("The snapshot function does not supported by venc!\n");
			goto exit;
		}
		
		if ((pfSnapShotOutput=fopen(aszDumpPath, "wb")) == NULL)
		{
			printf("Open output file %s fail !!\n",aszDumpPath);
			perror("error");
			goto exit;
		}
		
	}
	// Initialize command FIFO between venc
	if (CreateFifo(szFIFOPathName, &iFIFO_fd, O_WRONLY) != S_OK)
	{
		printf("Create Fifo fail...\n");
	}
	printf("Create Fifo success...\n");

	// Initialize SharedBuffMgr instance
	tSharedBuffMgrInitOpts.dwVersion = SHAREDBUFFMGR_VERSION;
	tSharedBuffMgrInitOpts.pObjectMem = NULL;
	
	if (bGetSnapShot == FALSE)
	{
		tSharedBuffMgrInitOpts.dwMinorNum = istream_no + 1;
	}
	else
	{
		tSharedBuffMgrInitOpts.dwMinorNum = istream_no + 1 + dwStreamTotalNum;	
	}
	
	tSharedBuffMgrInitOpts.eProcessRole = esbmprReader;

	if (SharedBuffMgr_Initial(&hSharedBuffMgrObj, &tSharedBuffMgrInitOpts) != S_OK)
	{
		printf("Initialize shared buffer manager object fail !!\n");
		exit(1);
	}
	printf("Initialize shared buffer manager object sucessful !!\n");

	// Retrieve fd of /dev/buff_mgr0 for using select()
	SharedBuffMgr_GetFileDescriptor(hSharedBuffMgrObj, &fd);	
	Maxfd = fd;
	FD_ZERO(&readset);

	if (bGetSnapShot == FALSE)
	{
		// Generate start command msg and send through fifo
		SendCmdMsg(istream_no, iFIFO_fd, letBitstrmStart);
	}
	else
	{
		SendCmdMsg(istream_no, iFIFO_fd, letGetSnapShot);
	}
	// Looping to get bitstream from ShardBuffMgr
	while (i < iframeno)
	{
		timeout.tv_sec = 0;
		timeout.tv_usec = 500;

		FD_SET(fd, &readset);

		iRes = select(Maxfd+1, &readset, NULL, NULL, &timeout);
		if (iRes <= 0) 
		{
			continue;
		}			
		if (FD_ISSET(fd, &readset))
		{
			if (SharedBuffMgr_GetBuffer(hSharedBuffMgrObj, &tSharedBuffMgrState) == S_OK)
			{
				if (bGetSnapShot == FALSE)
				{
					ptUBuffer = (TUBuffer *)tSharedBuffMgrState.pbyHdrAddr;

                    printf("ptUBuffer->dwDataType=0x%02X\n", ptUBuffer->dwDataType);
					if (ptUBuffer->dwDataType == FOURCC_CONF)
					{
						// get a conf ubuffer, 
						// venc will send conf ubuffer when
						// 1. venc receive a forceCI message
						// 2. when the stream be changed codec type
						if (ptUBuffer->dwFollowingDataType == FOURCC_H264)
						{
							printf("stream codec is changed to H.264\n");
						}
						else if (ptUBuffer->dwFollowingDataType == FOURCC_MP4V)
						{
							printf("stream codec is changed to Mpeg4\n");
						}
						else if (ptUBuffer->dwFollowingDataType == FOURCC_JPEG)
						{
							printf("stream codec is changed to JPEG\n");
						}
						dwUBuffPayloadLen = 0;
						pbyUBuffPayload = NULL;
						dwCodecType = ptUBuffer->dwFollowingDataType;
						bNeedIntra = TRUE;
					}
					else
					{
						if (dwCodecType == 0)
						{
							//Unknown codec type, send FourceCI command to request a conf ubuffer
							SendCmdMsg(istream_no, iFIFO_fd, letNeedConf);
							printf("send needconf\n");
						}
						else  //dwCodecType != 0
						{   
							if (bNeedIntra == TRUE)
							{
								if (!ptUBuffer->bIsSync) // check if a Intra frame
								{
									if (bSendIntraMsg == FALSE)
									{
										// If you need to start recording or streaming the bitstream from a I frame
										// You can send FourceIntra command to venc, and venc will encode the next
										// frame by I when venc received the command.
										// Otherwise you have to wait I frame according to venc's current I interval settings.
										SendCmdMsg(istream_no, iFIFO_fd, letNeedIntra);
										bSendIntraMsg = TRUE;
									}
									// Skip this frame since it's not a Intra frame
									SharedBuffMgr_ReleaseBuffer(hSharedBuffMgrObj, &tSharedBuffMgrState);
									continue;
								}
								bNeedIntra = FALSE;
								printf("get intra\n");
							}

							dwUBuffPayloadLen = ptUBuffer->dwSize - sizeof(TUBuffer) - ptUBuffer->dwUserDataSize;
							pbyUBuffPayload = tSharedBuffMgrState.pbyDataAddr;
							printf("(new)stream%d frame(%d):Size = %d\n", istream_no, i, dwUBuffPayloadLen);

							if (ptUBuffer->dwDataType == FOURCC_H264)
							{
								if (pfH264Output)
								{

									BYTE    *pbyUBufPld =   pbyUBuffPayload;
									DWORD   dwFrameByte = dwUBuffPayloadLen;
									DWORD dwByteStreamHeader = 0x01000000;
									DWORD   dwNALSize, dwTtlSize;
									dwTtlSize = 0;
									for (; dwFrameByte > sizeof(DWORD);) {
										if(bDumpBitstream)
										{
											fwrite(&dwByteStreamHeader, sizeof(BYTE), sizeof(DWORD), pfH264Output);
										}
										memcpy(&dwNALSize, pbyUBufPld, sizeof(DWORD));
										pbyUBufPld += sizeof(DWORD);
										if(bDumpBitstream)
										{
											fwrite(pbyUBufPld, sizeof(BYTE), dwNALSize, pfH264Output);
										}
										dwFrameByte -= sizeof(DWORD);
										pbyUBufPld += dwNALSize;
										dwFrameByte -= dwNALSize;
									}


									//fwrite(pbyUBuffPayload, dwUBuffPayloadLen, sizeof(BYTE), pfH264Output);
								}
								i++;
							}
							else if (ptUBuffer->dwDataType == FOURCC_MP4V)
							{
								DWORD   	dwBsBuffOffset = 0;
								WORD		wHdrTag = 0;
								WORD		wHdrLen = 0;
								BYTE		*pbyUBuffHdr = (BYTE *)tSharedBuffMgrState.pbyHdrAddr;
								if (ptUBuffer->dwUserDataSize > 0)
								{
									wHdrTag = *((WORD *)(pbyUBuffHdr+sizeof(TUBuffer)));
									if (wHdrTag == TAG_MP4V_EXTENINFO)
									{
										wHdrLen = *((WORD *)(pbyUBuffHdr+sizeof(TUBuffer)+sizeof(WORD)));
										dwBsBuffOffset = *((DWORD *)(pbyUBuffHdr+sizeof(TUBuffer)+wHdrLen));	
									}
								}
								if (pfMPEG4Output)
								{
									if( bDumpBitstream)
									{
										fwrite(pbyUBuffPayload+dwBsBuffOffset, dwUBuffPayloadLen, sizeof(BYTE), pfMPEG4Output);

									}
								}
								i++;
							}
							else if (ptUBuffer->dwDataType == FOURCC_JPEG)
							{
								if(bDumpBitstream)
								{
									sprintf(aszJPEGFilePath, "%s%i",aszDumpPath,i);
									if ((pfJPEGOutput = fopen(aszJPEGFilePath, "wb")) == NULL)
									{
										printf("Can't Open Output file %s\n", aszJPEGFilePath);
										perror("error");
									}
									else
									{
										fwrite(pbyUBuffPayload, dwUBuffPayloadLen, sizeof(BYTE), pfJPEGOutput);
										fclose(pfJPEGOutput);
									}
								}
								i++;
							}
						}
					} 
				}// if bGetSnapShot == FALSE
				else
				{
					DWORD	dwSnapShotFrameBytes = *((DWORD *)tSharedBuffMgrState.pbyHdrAddr);
					if (tSharedBuffMgrState.pbyDataAddr != NULL)
					{	
						printf("SnapShot size is %d\n",dwSnapShotFrameBytes);
						fwrite(tSharedBuffMgrState.pbyDataAddr, sizeof(BYTE), dwSnapShotFrameBytes, pfSnapShotOutput);
						fclose(pfSnapShotOutput);
					}
					break;
				}
				SharedBuffMgr_ReleaseBuffer(hSharedBuffMgrObj, &tSharedBuffMgrState);
				//dwCounter++;
				//if ( dwCounter%300 == 0)
					//printf("SharedBuffMgr_GetBuffer success %d times ...\n", dwCounter);
			}
		} // if FD_ISSET
	}

exit:

	if (bGetSnapShot == FALSE)
	{
		if (iFIFO_fd != -1) 
		{
			// Generate stop command msg and send through fifo
			SendCmdMsg(istream_no, iFIFO_fd, letBitstrmStop);
		}
	}
	if (hSharedBuffMgrObj != NULL)
	{
		// Release SharedBuffMgr instance
		if (SharedBuffMgr_Release(&hSharedBuffMgrObj) != S_OK)
		{
			printf("Release shared buffer manager object fail !!\n");
			exit(1);
		}
		printf("SharedBuffMgr_Release success ....\n");
	}
	if (iFIFO_fd != -1)
	{
		close(iFIFO_fd);
	}
	if (pfMPEG4Output != NULL)
	{
		fclose(pfMPEG4Output);
	}
	
	if (pfH264Output != NULL)
	{
		fclose(pfH264Output);
	}
	return 0;
}
#endif


void live_callback(frame_info_t *frame)
{
    unsigned char ismainorsub;
    unsigned char isiframe;

    //printf("frame info: channel=%d, encode_type=%d, stream_type=%d, frame_type=%d, size=%d\n",
    //    frame->channel, frame->encode_type, frame->stream_type, frame->frame_type, frame->size);
    
    if (frame->stream_type == VENC_STREAM_MAIN)
    {
        ismainorsub = 1;
    }
    else
    {
        ismainorsub = 0;
    }

    if (frame->frame_type == VIDEO_I_FRAME)
    {
        isiframe = 1;
    }
    else
    {
        isiframe = 0;
    }

    if (frame->encode_type == ENCODE_TYPE_H264)
    {
        glnk_sendvideodata(frame->channel, ismainorsub, isiframe, frame->buf, frame->size);
    }
    else if (frame->encode_type == ENCODE_TYPE_G711A)
    {
        glnk_sendaudiodata(frame->channel, frame->buf, frame->size);
    }
    else if (frame->encode_type == ENCODE_TYPE_AAC4)
    {
        glnk_sendaudiodata(frame->channel, frame->buf, frame->size);
    }
    else
    {
        printf("unknow encode type!\n");
    }

}


int receiver_init(void)
{
	// For SharedBufferMgr
	TSharedBuffMgrInitOptions tSharedBuffMgrInitOpts;
	TSharedBuffMgrState tSharedBuffMgrState;
	HANDLE hSharedBuffMgrObj = NULL;
	int fd = -1;
	DWORD	 dwStreamTotalNum = -1;
	
	// For select()
	int Maxfd = -1;
	fd_set readset;
	struct timeval       timeout;
	int                  iRes = 0;

	// For Command Fifo
	int     iFIFO_fd = -1;
	CHAR	*szFIFOPathName = "/tmp/venc/c0/command.fifo";

    // 音频编码相关
    char *szRecvSockPath = "/tmp/aenc/c0/audio0.sck";
    char *szCmdFiFoPath = "/tmp/aenc/c0/command.fifo";
    TSockSrcInitOpts tSockSrcInitOpts;
    HANDLE hSockSrc = NULL;
    TSrcUBufInfo tLMSrcUBufInfo;
    TUBuffer    *ptUBuffHdr = NULL;
    TUBufferConfAAC4 *ptAac4Conf = NULL;
    TUBufferConfGAMR *ptGamrConf = NULL;
    TUBufferConfG711 *ptG711Conf = NULL;
    BYTE *pbySpecConf;
    int iSpecConfLen;


	// For parsing UBuffer
	TUBuffer *ptUBuffer = NULL;	
	DWORD dwUBuffPayloadLen;
	BYTE  *pbyUBuffPayload;

	// General use
	int     istream_no = 0;
	DWORD   dwCodecType = 0;
	BOOL    bNeedIntra = TRUE;
	BOOL    bSendIntraMsg = FALSE;
	int     i = 0;
//	DWORD dwCounter = 0;

	BOOL bDumpBitstream=FALSE; // dump bistream
	
	BOOL bGetSnapShot=FALSE; //dump snapshot

    bDumpBitstream=TRUE;
	istream_no = 0;     // 


	
	if (GetTotalStreamNum("/etc/conf.d/c0/venc_conf.xml", &dwStreamTotalNum) != S_OK)
	{
		printf("Parse /etc/conf.d/c0/venc_conf.xml to get total stream number failed!!\n");
		exit(0);
	}
	else
	{
		printf("Total Stream number is %d\n",dwStreamTotalNum);
	}
	
	if (istream_no >= dwStreamTotalNum)
	{
		printf("The istream_no %d is larger than total stream number %d\n", istream_no ,dwStreamTotalNum);
		exit(0);
    }
	
	printf("venc_receiver version %s %s\n", rcsid,VERSION);

	// Initialize command FIFO between venc
	if (CreateFifo(szFIFOPathName, &iFIFO_fd, O_WRONLY) != S_OK)
	{
		printf("Create Fifo fail...\n");
	}
	printf("Create Fifo success...\n");

	// Initialize SharedBuffMgr instance
	tSharedBuffMgrInitOpts.dwVersion = SHAREDBUFFMGR_VERSION;
	tSharedBuffMgrInitOpts.pObjectMem = NULL;
	
	if (bGetSnapShot == FALSE)
	{
		tSharedBuffMgrInitOpts.dwMinorNum = istream_no + 1;
	}
	else
	{
		tSharedBuffMgrInitOpts.dwMinorNum = istream_no + 1 + dwStreamTotalNum;	
	}
	
	tSharedBuffMgrInitOpts.eProcessRole = esbmprReader;

	if (SharedBuffMgr_Initial(&hSharedBuffMgrObj, &tSharedBuffMgrInitOpts) != S_OK)
	{
		printf("Initialize shared buffer manager object fail !!\n");
		exit(1);
	}
	printf("Initialize shared buffer manager object sucessful !!\n");

	// Retrieve fd of /dev/buff_mgr0 for using select()
	SharedBuffMgr_GetFileDescriptor(hSharedBuffMgrObj, &fd);	
	Maxfd = fd;
	FD_ZERO(&readset);

	if (bGetSnapShot == FALSE)
	{
		// Generate start command msg and send through fifo
		SendCmdMsg(istream_no, iFIFO_fd, letBitstrmStart);
	}
	else
	{
		SendCmdMsg(istream_no, iFIFO_fd, letGetSnapShot);
	}

    //音频从UNIX域套接字获取流
    memset(&tSockSrcInitOpts, 0x0, sizeof(TSockSrcInitOpts));
    tSockSrcInitOpts.szRecvSockPath=szRecvSockPath;
    tSockSrcInitOpts.szCmdFiFoPath=szCmdFiFoPath;
    if(SockSrc_Initial(&hSockSrc, &tSockSrcInitOpts)==S_FAIL)
    {
        printf("[%s:%s:%d] SockSrc_Initial error!", __func__, __FILE__, __LINE__);
        return -1;
    }
    //Send command to aenc to get the CI information, ie. FOURCC_CONF ubuffer.
    SockSrc_EventHandler(hSockSrc, LetNeedIntra);
    //Send command to aenc to start encoding
    SockSrc_EventHandler(hSockSrc, LetBitstrmStart);

    
	// Looping to get bitstream from ShardBuffMgr
	while (1)
	{
		timeout.tv_sec = 0;
		timeout.tv_usec = 500;

		FD_SET(fd, &readset);

		iRes = select(Maxfd+1, &readset, NULL, NULL, &timeout);
		if (iRes <= 0) 
		{
			continue;
		}			
		if (FD_ISSET(fd, &readset))
		{
			if (SharedBuffMgr_GetBuffer(hSharedBuffMgrObj, &tSharedBuffMgrState) == S_OK)
			{
				if (bGetSnapShot == FALSE)
				{
					ptUBuffer = (TUBuffer *)tSharedBuffMgrState.pbyHdrAddr;

                    //printf("ptUBuffer->dwDataType=0x%02X\n", ptUBuffer->dwDataType);
					if (ptUBuffer->dwDataType == FOURCC_CONF)
					{
						// get a conf ubuffer, 
						// venc will send conf ubuffer when
						// 1. venc receive a forceCI message
						// 2. when the stream be changed codec type
						if (ptUBuffer->dwFollowingDataType == FOURCC_H264)
						{
							printf("stream codec is changed to H.264\n");
						}
						else if (ptUBuffer->dwFollowingDataType == FOURCC_MP4V)
						{
							printf("stream codec is changed to Mpeg4\n");
						}
						else if (ptUBuffer->dwFollowingDataType == FOURCC_JPEG)
						{
							printf("stream codec is changed to JPEG\n");
						}
						dwUBuffPayloadLen = 0;
						pbyUBuffPayload = NULL;
						dwCodecType = ptUBuffer->dwFollowingDataType;
						bNeedIntra = TRUE;
					}
					else
					{
						if (dwCodecType == 0)
						{
							//Unknown codec type, send FourceCI command to request a conf ubuffer
							SendCmdMsg(istream_no, iFIFO_fd, letNeedConf);
							printf("send needconf\n");
						}
						else  //dwCodecType != 0
						{   
							if (bNeedIntra == TRUE)
							{
								if (!ptUBuffer->bIsSync) // check if a Intra frame
								{
									if (bSendIntraMsg == FALSE)
									{
										// If you need to start recording or streaming the bitstream from a I frame
										// You can send FourceIntra command to venc, and venc will encode the next
										// frame by I when venc received the command.
										// Otherwise you have to wait I frame according to venc's current I interval settings.
										SendCmdMsg(istream_no, iFIFO_fd, letNeedIntra);
										bSendIntraMsg = TRUE;
									}
									// Skip this frame since it's not a Intra frame
									SharedBuffMgr_ReleaseBuffer(hSharedBuffMgrObj, &tSharedBuffMgrState);
									continue;
								}
								bNeedIntra = FALSE;
								printf("get intra\n");
							}

							dwUBuffPayloadLen = ptUBuffer->dwSize - sizeof(TUBuffer) - ptUBuffer->dwUserDataSize;
							pbyUBuffPayload = tSharedBuffMgrState.pbyDataAddr;
							//printf("(new)stream%d frame(%d):Size = %d\n", istream_no, i++, dwUBuffPayloadLen);

							if (ptUBuffer->dwDataType == FOURCC_H264)
							{


								BYTE    *pbyUBufPld =   pbyUBuffPayload;
								DWORD   dwFrameByte = dwUBuffPayloadLen;
								DWORD dwByteStreamHeader = 0x01000000;
								DWORD   dwNALSize, dwTtlSize;
								
                                frame_info_t frame;
                                static char frameBuf[1024 * 200];  //帧BUF，最大为100K
                                char *pFrameBuf = frameBuf;
                                unsigned char frame_type = 0;
                                
                                dwTtlSize = 0;
                                frame.channel = 0;
                                frame.encode_type = ENCODE_TYPE_H264;
                                frame.size = 0;
                                frame.frame_type = UNKONW_FRAME_TYPE;
                                if (istream_no == 0)
                                {
                                    frame.stream_type = VENC_STREAM_MAIN;
                                }
                                else 
                                {
                                    frame.stream_type = VENC_STREAM_SUB;
                                }

                                if (ptUBuffer->bIsSync == TRUE)
                                {
                                    frame.frame_type = VIDEO_I_FRAME;
                                }
                                else
                                {
                                    frame.frame_type = VIDEO_P_FRAME;
                                }

								for (; dwFrameByte > sizeof(DWORD);) 
                                {
                                    memcpy(pFrameBuf, &dwByteStreamHeader, sizeof(DWORD));  //添加NALU头
									memcpy(&dwNALSize, pbyUBufPld, sizeof(DWORD));   //NAL 长度
									pbyUBufPld += sizeof(DWORD);
                                    pFrameBuf += sizeof(DWORD);
                                    frame.size += sizeof(DWORD);
                                    dwFrameByte -= sizeof(DWORD);

                                    frame_type = *(BYTE *)pbyUBufPld & 0x1f;

//                                    if (frame_type == 7 || frame_type == 8 || frame_type == 5)
//                                    {
//                                        frame.frame_type = VIDEO_I_FRAME;
//                                    }
//                                    else
//                                    {
//                                        frame.frame_type = VIDEO_P_FRAME;
//                                    }
                                    
                                    memcpy(pFrameBuf, pbyUBufPld, dwNALSize);
                                    pbyUBufPld += dwNALSize;
                                    pFrameBuf += dwNALSize;
                                    frame.size += dwNALSize;
									dwFrameByte -= dwNALSize;

								}
                                frame.buf = frameBuf;
                                live_callback(&frame);
							}

						}
					} 
				}// if bGetSnapShot == FALSE

				SharedBuffMgr_ReleaseBuffer(hSharedBuffMgrObj, &tSharedBuffMgrState);
				//dwCounter++;
				//if ( dwCounter%300 == 0)
					//printf("SharedBuffMgr_GetBuffer success %d times ...\n", dwCounter);
			}
		} // if FD_ISSET

        if (SockSrc_GetUBuffer(hSockSrc, &tLMSrcUBufInfo) != S_OK)
        {
            printf("get audio bitstream fail\n");
        }
        else
        {
            frame_info_t frame = {0};
            DWORD bG711Complaw = FOURCC_ALAW;     
            ptUBuffHdr=(TUBuffer *)tLMSrcUBufInfo.pbyUBuffHdr;

            if (ptUBuffHdr->dwDataType == FOURCC_CONF)  //receive conf ubuff
            {
                if (ptUBuffHdr->dwFollowingDataType == FOURCC_AAC4)
                {

                    ptAac4Conf = (TUBufferConfAAC4 *)ptUBuffHdr;
                    pbySpecConf = ((BYTE *)ptAac4Conf)+sizeof(TUBufferConfAAC4);
                    iSpecConfLen = ((int)ptAac4Conf->dwSize)-sizeof(TUBufferConfAAC4);

                    //printf("AAC4 with Channel number %ul\n", ptAac4Conf->dwChannelNumber);
                    //printf("          SampleRate %ul\n", ptAac4Conf->dwSampleRate);
                    //printf("          profile level %ul\n", ptAac4Conf->dwProfileLevel);
                    //printf("sepc len %d\n", iSpecConfLen);

                }
                else if (ptUBuffHdr->dwFollowingDataType == FOURCC_GAMR)
                {
                    ptGamrConf = (TUBufferConfGAMR *)ptUBuffHdr;
                    printf("GAMR with sample rate %ul\n", ptGamrConf->dwSampleRate);
                }
                else if (ptUBuffHdr->dwFollowingDataType == FOURCC_G711)
                {
                    ptG711Conf = (TUBufferConfG711 *)ptUBuffHdr;
                    printf("G711 with channel number %ul\n", ptG711Conf->dwChannelNumber);
                    if (ptG711Conf->dwCompressionLaw == FOURCC_ALAW)
                    {
                        printf("          compressionlaw is alaw\n");
                        bG711Complaw = FOURCC_ALAW;
                    }
                    else if (ptG711Conf->dwCompressionLaw == FOURCC_ULAW)
                    {
                        printf("          compressionlaw is ulaw\n");
                        bG711Complaw = FOURCC_ULAW;
                    }
                    else
                    {
                        printf("          compressionlaw is unknow\n");
                    }
                }

                
            }
            else
            {
                if (ptUBuffHdr->dwDataType == FOURCC_AAC4)
                {
                    //printf("AAC4     :sec %ul, usec %ul\n", ptUBuffHdr->dwSec, ptUBuffHdr->dwUSec);
                    //printf("         :size %d\n", tLMSrcUBufInfo.iUBuffPayloadLen);

                    frame.buf = (char *)tLMSrcUBufInfo.pbyUBuffPayload;
                    frame.size = tLMSrcUBufInfo.iUBuffPayloadLen;
                    frame.encode_type = ENCODE_TYPE_AAC4;
                    live_callback(&frame);
                }
                else if (ptUBuffHdr->dwDataType == FOURCC_GAMR)
                {
                    //printf("GAMR     :sec %ul, usec %ul\n", ptUBuffHdr->dwSec, ptUBuffHdr->dwUSec);
                    //printf("         :size %d\n", tLMSrcUBufInfo.iUBuffPayloadLen);
                    frame.buf = (char *)tLMSrcUBufInfo.pbyUBuffPayload;
                    frame.size = tLMSrcUBufInfo.iUBuffPayloadLen;
                    frame.encode_type = ENCODE_TYPE_GAMR;
                    live_callback(&frame);
                }
                else if (ptUBuffHdr->dwDataType == FOURCC_G711)
                {
                    //printf("G711     :sec %ul, usec %ul\n", ptUBuffHdr->dwSec, ptUBuffHdr->dwUSec);
                    //printf("         :size %d\n", tLMSrcUBufInfo.iUBuffPayloadLen);
                    frame.buf = (char *)tLMSrcUBufInfo.pbyUBuffPayload;
                    frame.size = tLMSrcUBufInfo.iUBuffPayloadLen;
                    if (bG711Complaw == FOURCC_ALAW)
                    {
                        frame.encode_type = ENCODE_TYPE_G711A;
                    }
                    else
                    {
                        frame.encode_type = ENCODE_TYPE_G711U;
                    }
                    live_callback(&frame);
                }
            }
        }
    


        usleep(1000);
        
	}


    //Send command to aenc to stop encoding.
    if (hSockSrc != NULL)
    {
        SockSrc_EventHandler(hSockSrc, letBitstrmStop);
        SockSrc_Release(&hSockSrc);
    }



	if (bGetSnapShot == FALSE)
	{
		if (iFIFO_fd != -1) 
		{
			// Generate stop command msg and send through fifo
			SendCmdMsg(istream_no, iFIFO_fd, letBitstrmStop);
		}
	}
	if (hSharedBuffMgrObj != NULL)
	{
		// Release SharedBuffMgr instance
		if (SharedBuffMgr_Release(&hSharedBuffMgrObj) != S_OK)
		{
			printf("Release shared buffer manager object fail !!\n");
			exit(1);
		}
		printf("SharedBuffMgr_Release success ....\n");
	}
	if (iFIFO_fd != -1)
	{
		close(iFIFO_fd);
	}

	return 0;
}

#define MSGQ_PATH "/tmp/msgq"
#define MSGQ_BUFF_SIZE  1024

/**
  消息队列结构体，第一个字段必须为long类型，msgsnd
  中msgsz指定的大小不包含消息类型占的四字节
 */

typedef struct
{
    long type;
    char mtext[MSGQ_BUFF_SIZE];
}msg_buf_t;


static int g_msg_id;

static int init_msg_queue(void)
{
    key_t key = ftok(MSGQ_PATH, 'a');
    g_msg_id = msgget(key, S_IRUSR | S_IWUSR | IPC_CREAT);// | IPC_EXCL);	
    if (g_msg_id == -1)
    {
        printf("Message Queue init error\n");
        return -1;
    }

    return 0;
}

static int send_to_msg_queue(msg_type_e type, void *mtext, int mtextLen)
{
    int ret;
    msg_buf_t msgBuf;

    msgBuf.type = (long)type;
    memcpy(msgBuf.mtext, mtext, mtextLen);

    ret = msgsnd(g_msg_id, &msgBuf, mtextLen, IPC_NOWAIT);
    if (-1 == ret)
    {
        printf("%s error, %s\n", __func__, strerror(errno));
        return -1;
    }

    return 0;
}

static int recv_from_msg_queue(msg_type_e type, msg_buf_t *msg)
{
    int ret;
    int msgflags;

    msgflags = MSG_NOERROR | IPC_NOWAIT;
    ret = msgrcv(g_msg_id, msg, MSGQ_BUFF_SIZE, type, msgflags);
    if (-1 == ret)
    {
        if (ENOMSG == errno) //Queue no messages
        {
            return 0;
        }
        printf("%s error, %s\n", __func__, strerror(errno));
        return -1;
    }

    return ret;
}

/* 获取系统当前时间(精确到毫秒) */
u64 get_cur_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (u64)tv.tv_sec * 1000 + (u64)tv.tv_usec / 1000;
}


static pthread_t g_alarm_td;
void process_alarm_thread(void)
{
    static msg_buf_t msg_buf;

    while (1)
    {
        if (recv_from_msg_queue(MSG_TYPE_433_DATA, &msg_buf) > 0)
        {
            sensor_ask_data_t sensor_data;
            memcpy(&sensor_data, msg_buf.mtext, sizeof(sensor_ask_data_t));
        
            printf("recv 433 data, addr %02X %02X data %02X \n", 
                sensor_data.sensor_addr[0], sensor_data.sensor_addr[1], sensor_data.sensor_data);
        
            if (sensor_data.sensor_addr[0] == 0xC2 &&
                sensor_data.sensor_addr[1] == 0x9D &&
                sensor_data.sensor_data == 0x00)
            {
                printf("Door break alarm\n");

                static u64 cur_time = 0;
                static u64 last_time = 0;
                    
                cur_time = get_cur_time_ms();
                if (cur_time - last_time > 2000)
                {
                    PushAlarm alarm;

                    struct tm *ltime;
                    time_t times;
                    times = time(NULL);
                    ltime = localtime(&times);
                    
                    alarm.alarm_type = PAT_BREAKDOOR_ALARM;
                    alarm.channel = 1;
                    alarm.timestamp.year = ltime->tm_year + 1900;
                    alarm.timestamp.month = ltime->tm_mon + 1;
                    alarm.timestamp.day =ltime->tm_mday;
                    alarm.timestamp.hour = ltime->tm_hour;
                    alarm.timestamp.minute = ltime->tm_min;
                    alarm.timestamp.second = ltime->tm_sec; 
                    printf("%04d-%02d-%02d %02d:%02d:%02d ", alarm.timestamp.year, alarm.timestamp.month, alarm.timestamp.day,
                        alarm.timestamp.hour, alarm.timestamp.minute, alarm.timestamp.second);
                    if (glnk_push_alarm(alarm))
                    {
                        printf("push alarm succeed\n");
                    }
                    else
                    {
                        printf("push alarm fail\n");
                    }
                    last_time = cur_time;
                }

            }
        }
    }

}

int main(void)
{
    InitNetParam netparam;
    
	//signal(SIGINT, &CTRLC);

	printf("Goolink SDK Version is 0x%x \n", (int)glnk_get_version() );
	
	strcpy(netparam.dev, "ra0");
	netparam.localTCPport = 49000;
	netparam.localUDPport = 59000;
	strcpy(netparam.udid, "zz0810b576");  //gid (goolink id)
	netparam.channelnum = 32;
	netparam.issupportsubvideostream= 1;
	netparam.maxsession = 16;
		
	glnk_destroy();

	printf("glnk_init=%d\n", glnk_init(&netparam)); 

    if (init_msg_queue() < 0)
    {
        printf("init msg queue error\n");
        return -1;
    }
    
    pthread_create(&g_alarm_td, NULL, (void *)process_alarm_thread, NULL);
    receiver_init();


    return 0;
}



/* =========================================================================================== */
SCODE CreateFifo(char *pData, int *piFIFO, int iFlag)
{
	int iFIFO,res;

    printf("FIFO path = %s\r\n",(char*)pData);

	if(access((char*)pData, F_OK) == -1)
	{
		res = mkfifo((char*)pData, 0777);
		if (res != 0)
		{
			printf("Could not create fifo %s\n",(char*)pData);
			return S_FAIL;
		}
	}

	//use blocking mode to open write or it will return -1 
	//if open read is not ready yet 

	if( iFlag == O_RDONLY )
		iFIFO = open( pData, O_RDONLY | O_NONBLOCK);
	else    	
		iFIFO = open( pData, O_WRONLY);

	if(  iFIFO  < 0  )
	{
		printf("open fifo failed %d\r\n",errno);
		return S_FAIL;    
	}
	else 
	{
		printf("open FIFO: %d success\r\n",iFIFO);
		*(int*)piFIFO = iFIFO;
		return S_OK;    
	}
}

static void GetTotalNum(void *userData, const char *s, int len)
{
	DWORD *pdwTotalNum = (DWORD *)userData;
	CHAR szTemp[len+1];
	DWORD dwVal = 0;

	memset(szTemp, 0x0, sizeof(szTemp));
	snprintf(szTemp, sizeof(szTemp), "%s", s);
	dwVal = strtoul(szTemp, (char**)NULL, 10);

	if (pdwTotalNum != NULL)
	{
		*pdwTotalNum = dwVal;
	}
	return;
}
SCODE GetTotalStreamNum(char *szVencCfg, DWORD *pdwTotalNum)
{
	int sRet;
	HANDLE hXmlWrapObj;
	
	TXmlWrapperTreeMap ptParsingMap[] = 
	{
		{"root/venc_encoder/total_num", NULL, &GetTotalNum, NULL},
		{NULL, NULL, NULL}
	};
	TXmlWrapperInitOptions tInitOptions;
	memset(&tInitOptions, 0x0, sizeof(TXmlWrapperInitOptions));

	
	tInitOptions.dwVersion = XMLWRAPPER_VERSION;
	if (XmlWrapper_Initial(&hXmlWrapObj, &tInitOptions) != S_OK)
	{
		return S_FAIL;
	}
	if (XmlWrapper_SetHandler(hXmlWrapObj, ptParsingMap, NULL) != S_OK)
	{
		printf("%s %d : XmlWrapper_SetHandler Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	
		
	if ((sRet = XmlWrapper_ReadFile_UsrDefFunc(szVencCfg, hXmlWrapObj, pdwTotalNum)) != S_OK)
	{
		printf("%s %d : XmlWrapper_ReadFile_UsrDefFunc Fail! %x (%s)\n", __FILE__, __LINE__, sRet, szVencCfg);
		return S_FAIL;
	}
	if (XmlWrapper_Release(&hXmlWrapObj) != S_OK)
	{
		printf("Release XmlWrapper Fail! \n");
		return S_FAIL;
	}
	return sRet;
}

#define BUILD_CONFIG "/usr/local/firmware/.config"
static int foundKeywordOccurrence(char *fname, char *szkeyowrd) {
	
	FILE *fp;
	int find_result = 0;
	char temp[512];
	
	
	if((fp=fopen(fname, "r")) == NULL) {
		printf("Can not open %s\n", fname);
		goto exit;
	}

	while(fgets(temp, 512, fp) != NULL) {
		if((strstr(temp, szkeyowrd)) != NULL) {
			find_result++;
		}
		memset(temp, 0x0, sizeof(temp));
	}
	if(fp) {
		fclose(fp);
	}
exit:	
   	return find_result;
}

BOOL IsSupportSnapShot(void)
{
	BOOL bRet = FALSE;
	if (foundKeywordOccurrence(BUILD_CONFIG, "CONFIG_JPEG_SNAPSHOT=y") > 0)
	{
		bRet = TRUE;
	}
	return bRet;
}
/* =========================================================================================== */
/* Fifo message is defined in XML formate
 * <control>
 *    <request>
 *        <host> %s </host>
 *        <content> %s </content>
 *    </request>
 * </control>
 *
 * The elements of <host> are encoder0, encoder1 and encoder2 to indicate stream1, stream2 or stream3
 * The elements of <content> are start, stop, fourceCI and fourceIntra.
 *      start: request venc to start encodeing for the <host> stream.
 *      stop:  request venc to stop encodeing for the <host> stream.
 *      fourceCI: request venc to send a CONF type Ubuffer
 *      fourceIntra: request venc to encode a IntraFrame(I Frame).
 * */

#define CONTROL_MSG_START           "<content>start</content>"
#define CONTROL_MSG_STOP            "<content>stop</content>"
#define CONTROL_MSG_FORCECI         "<content>forceCI</content>"
#define CONTROL_MSG_FORCEINTRA      "<content>forceIntra</content>"
#define CONTROL_MSG_GETSNAPSHOT      "<content>GetSnapshot</content>"
#define CONTROL_MSG_PREFIX			"<control><request>"
#define CONTROL_MSG_POSTFIX			"</request></control>"

SCODE SendCmdMsg(int iChNo, int iCmdFiFo, ECommandType eType)
{
	CHAR    acTemp[512];
	int     iLen;

	switch(eType)
	{
		case letGetSnapShot:
			printf("Get snapshot\n");
			if (iCmdFiFo >= 0)
			{
				acTemp[0] = 1;
				// force it using a long mode
				acTemp[1] = 0x84;

				sprintf(&(acTemp[6]), "%s<host>encoder%d</host>%s%s", 
						CONTROL_MSG_PREFIX, 
						iChNo, 
						CONTROL_MSG_GETSNAPSHOT,
						CONTROL_MSG_POSTFIX);
				iLen = strlen(&(acTemp[6]));
				acTemp[5] =  iLen >> 24;
				acTemp[4] = (iLen & 0x00FF0000) >> 16;
				acTemp[3] = (iLen & 0x0000FF00) >> 8;
				acTemp[2] = (iLen & 0x000000FF);

				write(iCmdFiFo, acTemp, iLen+6);
				printf("write command %d bytes: %s\r\n", iLen+6, acTemp+6);
			}
			
			break;
		case letNeedIntra:
			printf("Need Intra\n");
			if (iCmdFiFo >= 0)
			{
				acTemp[0] = 1;
				// force it using a long mode
				acTemp[1] = 0x84;

				sprintf(&(acTemp[6]), "%s<host>encoder%d</host>%s%s", 
						CONTROL_MSG_PREFIX, 
						iChNo, 
						CONTROL_MSG_FORCEINTRA,
						CONTROL_MSG_POSTFIX);
				iLen = strlen(&(acTemp[6]));
				acTemp[5] =  iLen >> 24;
				acTemp[4] = (iLen & 0x00FF0000) >> 16;
				acTemp[3] = (iLen & 0x0000FF00) >> 8;
				acTemp[2] = (iLen & 0x000000FF);

				write(iCmdFiFo, acTemp, iLen+6);
				printf("write command %d bytes: %s\r\n", iLen+6, acTemp+6);
			}
			break;
		case letNeedConf:
			printf("Need Conf\n");
			if (iCmdFiFo >= 0)
			{
				acTemp[0] = 1;
				// force it using a long mode
				acTemp[1] = 0x84;

				sprintf(&(acTemp[6]), "%s<host>encoder%d</host>%s%s", 
						CONTROL_MSG_PREFIX, 
						iChNo,
						CONTROL_MSG_FORCECI,
						CONTROL_MSG_POSTFIX);
				iLen = strlen(&(acTemp[6]));
				acTemp[5] =  iLen >> 24;
				acTemp[4] = (iLen & 0x00FF0000) >> 16;
				acTemp[3] = (iLen & 0x0000FF00) >> 8;
				acTemp[2] = (iLen & 0x000000FF);

				write(iCmdFiFo, acTemp, iLen+6);
				printf("write command %d bytes: %s\r\n", iLen+6, acTemp+6);
			}
			break;
		case letBitstrmStart:
			printf("Bitstrm start\n");
			if (iCmdFiFo >= 0)
			{
				acTemp[0] = 1;
				// force it using a long mode
				acTemp[1] = 0x84;

				sprintf(&(acTemp[6]), "%s<host>encoder%d</host>%s%s", 
						CONTROL_MSG_PREFIX, 
						iChNo, 
						CONTROL_MSG_START,
						CONTROL_MSG_POSTFIX);
				iLen = strlen(&(acTemp[6]));
				acTemp[5] =  iLen >> 24;
				acTemp[4] = (iLen & 0x00FF0000) >> 16;
				acTemp[3] = (iLen & 0x0000FF00) >> 8;
				acTemp[2] = (iLen & 0x000000FF);

				write(iCmdFiFo, acTemp, iLen+6);
				printf("write command %d bytes: %s\r\n", iLen+6, acTemp+6);
			}
			break;
		case letBitstrmStop:
			printf("Bitstrm stop\n");
			if (iCmdFiFo >= 0)
			{
				acTemp[0] = 1;
				// force it using a long mode
				acTemp[1] = 0x84;

				sprintf(&(acTemp[6]), "%s<host>encoder%d</host>%s%s", 
						CONTROL_MSG_PREFIX, 
						iChNo, 
						CONTROL_MSG_STOP,
						CONTROL_MSG_POSTFIX);
				iLen = strlen(&(acTemp[6]));
				acTemp[5] =  iLen >> 24;
				acTemp[4] = (iLen & 0x00FF0000) >> 16;
				acTemp[3] = (iLen & 0x0000FF00) >> 8;
				acTemp[2] = (iLen & 0x000000FF);

				write(iCmdFiFo, acTemp, iLen+6);
				printf("write command %d bytes: %s\r\n", iLen+6, acTemp+6);
			}
			break;
		default:
			return S_FAIL;
	}
	return S_OK;
}
