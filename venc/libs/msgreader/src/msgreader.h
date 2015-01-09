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
 * msgreader.h
 *
 * \brief
 * Goldberg - message reader header file
 *
 * \date
 * 2007/07/30
 *
 * \author
 * Erin Tsai
 *
 *
 *******************************************************************************
 */

#ifndef _MESSAGE_READER_H_
#define _MESSAGE_READER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>//stat
#include <fcntl.h>
#include <string.h>

#include "typedef.h"
#include "errordef.h"   
#include "global_codec.h"
#include "xmlwrapper.h"

/*! FOUR_CC Version code of your \b MsgReader instance. */
#define MSGREADER_VERSION		MAKEFOURCC(2, 2, 0, 0)

#define ERR_TIME_OUT		0x80060001

/* =========================================================================================== */
#if 0
SCODE MsgReader_Initial(HANDLE *phObject, TMsgReaderInitialOpts *ptInitOptions);
SCODE MsgReader_Start(HANDLE hObject, EMRMMsgReadMode emrmMode);
void MsgReader_Stop(HANDLE hObject);
int MsgReader_GetFifoFd(HANDLE hObject);
SCODE MsgReader_ParseMsg(HANDLE hObject, BYTE *pbyBuffer, DWORD dwBufferLength);
SCODE MsgReader_Release(HANDLE *phObject);
#endif

typedef enum msg_read_mode
{
	emrmBlocking,
	ersmNonblocking
} EMRMMsgReadMode;

/* =========================================================================================== */
typedef struct message_reader_initial_options
{
	/*! library Version Number */
	DWORD			   dwVersion;
	/*! Message command fifo path*/
	CHAR			   *pcMsgPath;
	/*! CallBackFucntion*/
	TXmlWrapperTreeMap *ptCallBackHandler;
	/*! Select Wait Time*/
	struct timeval     *ptv_time;
	/*! USROBJ for LibXmlWrapper */
	HANDLE             hUsrObj;
}TMsgReaderInitialOpts;

#ifdef __cplusplus
extern "C" {
#endif

/*!
******************************************************************************
* \brief
* Initialize MsgReader
*
* \param phObject
* \a (o) pointer to receive the handle of the MsgReader object
*
* \param ptInitOptions
* \a (i) Pointer of MsgReader initial options
*
* \retval S_OK
* Create object ok
*
* \retval ERR_INVALID_VERSION;
* library version number incorrect
*
* \retval ERR_INVALID_MESSAGE_FORMAT;
* client's message format type is not agreement with server's
*
* \retval ERR_FIFO_ACCESS_FAIL;
* open fifo failed
*
* \retval ERR_OUT_OF_MEMORY;
* no enough memory
*
* \remark
* 
*
* \see Message_Client_Release
*
******************************************************************************
*/
SCODE MsgReader_Initial(HANDLE *phObject, TMsgReaderInitialOpts *ptInitOptions);

/*!
******************************************************************************
* \brief
* Start to receive Message from Server, open specified message path(pcMsgPath), 
* if accesses successfully, MsgReader object will call callback function 
* while parsing the corresponding XML path
*
* \param hObject
* \a (i) pointer to receive the handle of the MsgReader object
*
* \param emrmMode
* Blocking mode or Non-blocking mode 
*
* \retval ERR_TIME_OUT
* Timeout in non-blocking mode
*
* \remark
* 
*
******************************************************************************
*/
SCODE MsgReader_Start(HANDLE hObject, EMRMMsgReadMode emrmMode);

/*!
******************************************************************************
* \brief
* Stop to parse Message from command fifo
*
* \param hObject
* \a (i) pointer to receive the handle of the MsgReader object
*
* \remark
* 
*
******************************************************************************
*/
void MsgReader_Stop(HANDLE hObject);

/*!
******************************************************************************
* \brief
* Release MsgReader
*
* \param phObject
* \a (o) pointer to receive the handle of the MsgReader object
*
* \retval S_OK
* release object ok
*
* \retval S_FAIL;
* Release object Fail
*
* \remark
*
* \see MsgReader_Initial
*
******************************************************************************
*/
SCODE MsgReader_Release(HANDLE *phObject);

/*!
******************************************************************************
* \brief
* Get the FIFO file descriptor of MsgReader
*
* \param hObject
* Handle of the MsgReader object
*
* \retval
* The file descriptor will be returned
*
* \remark
*
* \see MsgReader_Initial
*
******************************************************************************
*/
int MsgReader_GetFifoFd(HANDLE hObject);

/*!
******************************************************************************
* \brief
* Send the buffer to be parsed to MsgReader
*
* \param hObject
* Handle of the MsgReader object
*
* \param pbyBuffer
* The buffer to be parsed
*
* \param dwBufferLength
* Length of pbyBuffer
*
* \retval S_OK
* Parse the buffer successfully
*
* \retval S_FAIL
* Parse the buffer failed
*
* \remark
*
* \see MsgReader_Initial
*
******************************************************************************
*/
SCODE MsgReader_ParseMsg(HANDLE hObject, BYTE *pbyBuffer, DWORD dwBufferLength);

#ifdef __cplusplus
}
#endif

#endif//_MESSAGE_CLIENT_H_
