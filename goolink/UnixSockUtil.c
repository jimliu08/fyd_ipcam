/*
 *******************************************************************************
 * $Header: $
 *
 *  Copyright (c) 2006-2010 VN Inc. All rights reserved.
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
 *  | VN INC.                                                    |
 *  +-----------------------------------------------------------------+
 *
 * $History: $
 * 
 *******************************************************************************
 */

/*!
 *******************************************************************************
 * Copyright 2006-2010 VN, Inc. All rights reserved.
 *
 * \file
 * UnixSockUtil.c
 *
 * \brief
 * Unix Domain Socket Utilities implementation
 *
 * \date
 * 2008/08/11
 *
 * \author
 * DJhow.Tu
 *
 *
 *******************************************************************************
 */
#include <stdio.h>
#include "UnixSockUtil.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <unistd.h>

int UnixDgram_CreateSock(const char *local_path)
{
	int fd;

	// Create the unix domain socket as datagram
	fd = socket(PF_UNIX, SOCK_DGRAM, 0);
	if (fd < 0)
	{
		return -1;
	}

	// If local_path isn't NULL, bind the socket to the path
	if (local_path != NULL)
	{
		struct sockaddr_un local_addr;
		int iRet;

		unlink(local_path);
		memset(&local_addr, 0, sizeof(local_addr));	
		local_addr.sun_family = AF_UNIX;
		strncpy(local_addr.sun_path, local_path, sizeof(local_addr.sun_path));
		iRet = bind(fd, (struct sockaddr *) &local_addr, sizeof(local_addr));
		if (iRet < 0)
		{
			close(fd);
			return -2;
		}

		iRet = chmod(local_path, 0666);
		if (iRet < 0)
		{
			close(fd);
			return -3;
		}
	}

	return fd;
}

int UnixDgram_Connect(int fd, const char *peer_path)
{
	struct sockaddr_un peer_addr;
	int iRet;
	
	if (peer_path == NULL)
	{
		return -1;
	}
	
	// Connect
	memset(&peer_addr, 0, sizeof(peer_addr));	
	peer_addr.sun_family = AF_UNIX;
	strncpy(peer_addr.sun_path, peer_path, sizeof(peer_addr.sun_path));
	
	iRet = connect(fd, (struct sockaddr *) &peer_addr, sizeof(peer_addr));
	if (iRet < 0)
	{
		return -2;
	}

	return iRet;
}

int UnixDgram_Sendto(int fd, const void *buffer, int bufflen, const char *peer_path)
{
	struct sockaddr_un peer_addr;

	if (peer_path == NULL)
	{
		return -1;
	}

	memset(&peer_addr, 0, sizeof(peer_addr));
	peer_addr.sun_family = AF_UNIX;
	strncpy(peer_addr.sun_path, peer_path, sizeof(peer_addr.sun_path));

	return sendto(fd, buffer, bufflen, 0, (struct sockaddr *)&peer_addr, sizeof(peer_addr));
}
