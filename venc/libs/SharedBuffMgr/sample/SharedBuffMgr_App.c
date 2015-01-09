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
 * A sample code to get sharedbuffer from encoder.
 *
 * \date
 * 2009/03/12
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "SharedBuffMgr.h"

/* =========================================================================================== */
int main(int argc, char **argv)
{
	TSharedBuffMgrInitOptions tInitOptions;
    TSharedBuffMgrState tState;

	int Maxfd;
    fd_set readset, writeset;
    struct timeval       timeout;
    int                  iRes = 0;         
    
	HANDLE hObject1;
	int fd1;
	
	HANDLE hObject2;
	int fd2; 

	//initial writer shared memory object
    tInitOptions.dwVersion = SHAREDBUFFMGR_VERSION;
	tInitOptions.dwBufNum = 4;
    tInitOptions.dwHeaderSize = 128;
    tInitOptions.dwDataSize = 1024;
    tInitOptions.eAlignType = ALIGN_TYPE_8_BYTE;
	tInitOptions.pObjectMem = NULL;
	tInitOptions.dwBusIndex = 0;
	tInitOptions.dwMinorNum = 0;
	tInitOptions.eProcessRole = esbmprWriter;
	tInitOptions.byInitialVal = 0x0;
	if (SharedBuffMgr_Initial(&hObject1, &tInitOptions) != S_OK) {
		printf("[Shared App] Initialize shared buffer manager object for writer failed !!\n");
		exit(1);
	}    
	printf("[Shared App] Initialize shared buffer manager object sucessfully for writer !!\n");
	SharedBuffMgr_GetFileDescriptor(hObject1, &fd1);
	
	// initial reader shared memory object
    tInitOptions.dwVersion = SHAREDBUFFMGR_VERSION;
	tInitOptions.pObjectMem = NULL;
	tInitOptions.dwMinorNum = 0;
	tInitOptions.eProcessRole = esbmprReader;
	if (SharedBuffMgr_Initial(&hObject2, &tInitOptions) != S_OK) {
		printf("[Shared App] Initialize shared buffer manager object for reader failed !!\n");
		exit(1);
	}    
	printf("[Shared App] Initialize shared buffer manager object sucessfully for reader !!\n");
	SharedBuffMgr_GetFileDescriptor(hObject2, &fd2);

	if (fd1 > fd2)
		Maxfd = fd1;
	else
		Maxfd = fd2;

	FD_ZERO(&readset);
	FD_ZERO(&writeset);
	
   	while(1) {
    	FD_SET(fd1, &writeset);
    	FD_SET(fd2, &readset);
        
        timeout.tv_sec = 0;
       	timeout.tv_usec = 500;
		
		iRes = select(Maxfd+1, &readset, &writeset, NULL, &timeout);
       	if (iRes <= 0) {
            continue;
        }
   		if (FD_ISSET(fd1, &writeset)) {
   			if (SharedBuffMgr_GetBuffer(hObject1, &tState) == S_OK) {
				printf("[Shared App] writer get successfully, (Hdr,Data) addr = (%p,%p) \n", tState.pbyHdrAddr, tState.pbyDataAddr);
   			    SharedBuffMgr_ReleaseBuffer(hObject1, &tState);
	        }
   		}
   		if (FD_ISSET(fd2, &readset)) {
   			if (SharedBuffMgr_GetBuffer(hObject2, &tState) == S_OK) {
				printf("[Shared App] reader get successfully, (Hdr,Data) addr = (%p,%p) \n", tState.pbyHdrAddr, tState.pbyDataAddr);
	            SharedBuffMgr_ReleaseBuffer(hObject2, &tState);
	        }
   		}   		
   	}
    
	if (SharedBuffMgr_Release(&hObject1) != S_OK) {
		printf("[Shared App] Release shared buffer manager object fail !!\n");
		exit(1);
	}
	
	if (SharedBuffMgr_Release(&hObject2) != S_OK) {
		printf("[Shared App] Release shared buffer manager object fail !!\n");
		exit(1);
	}	
	printf("[Shared App] SharedBuffMgr_Release success ....\n");	
    return 0;
}
