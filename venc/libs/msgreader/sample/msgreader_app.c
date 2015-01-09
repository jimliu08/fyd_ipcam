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
 */

/*!
 *******************************************************************************
 * Copyright 2007-2010 VN, Inc. All rights reserved.
 *
 * \file
 * msgreader_app.c
 *
 * \brief
 * MsgReader Read messages from specified command fifo
 *
 * \date
 * 2007/07/31
 *
 * \author
 * Ming-Chin Tsai
 *
 *
 *******************************************************************************
 */

#include "msgreader.h"
#include "msgreader_app.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>


BOOL bTerminate = FALSE;
extern TXmlWrapperTreeMap MsgReaderTreeHdl[];

/* =========================================================================================== */
static void sig_kill(int signo)
{
	printf("[MsgReader_App]%s Receive SIGNAL!!!\n", __func__);
	if (signo == SIGTERM)
	{
		printf("[MsgReader_App]%s Receive SIGNAL SIGTERM!!!\n", __func__);
		bTerminate = TRUE;	
	}

}//sig_kill

/* =========================================================================================== */
int main()
{
	BYTE abyBuffer[512];
	TMsgReaderInitialOpts tInitOpts;
	HANDLE				hMsgReaderObj;
	TMsgReaderAppObj		tmdoReaderObj;
	struct timeval      tv_time;

	fd_set rfds;
	struct timeval tv;
	int iReadSz;

	tv_time.tv_sec = 3;
	tv_time.tv_usec = 0;
	tInitOpts.dwVersion = MSGREADER_VERSION;
	tInitOpts.pcMsgPath = "/tmp/cmd.fifo";
	tInitOpts.ptCallBackHandler = MsgReaderTreeHdl;
	tInitOpts.hUsrObj = (HANDLE)&tmdoReaderObj;
	tInitOpts.ptv_time = &tv_time;

	if (MsgReader_Initial(&hMsgReaderObj, &tInitOpts) != S_OK)
	{
		fprintf(stderr, "[MsgReader_App] MsgReader_Initial Fail!!\n");
		return -1;
	}
	printf("[MsgReader_App] MsgReader_Initial Successfully!!\n");

#if 1
	printf("pid %d\n", getpid());
	signal(SIGTERM, sig_kill);

	while (bTerminate == FALSE)
	{
		memset(abyBuffer, 0, 512);
		FD_ZERO(&rfds);
		FD_SET(MsgReader_GetFifoFd(hMsgReaderObj), &rfds);
		tv.tv_sec = 2;
		tv.tv_usec = 0;
		if (select(MsgReader_GetFifoFd(hMsgReaderObj) + 1, &rfds, NULL, NULL, &tv) > 0)
		{
			iReadSz = read(MsgReader_GetFifoFd(hMsgReaderObj), abyBuffer, 512);
			MsgReader_ParseMsg(hMsgReaderObj, abyBuffer, iReadSz);
		}
	}	
#else
	MsgReader_Start(hMsgReaderObj, emrmBlocking);
#endif

	//release msgreader
	MsgReader_Release(&hMsgReaderObj);
	printf("[MsgReader_App] MsgReader_Release Successfully!!!\n");

	return 0;
}
