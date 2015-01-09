/************************************************************************
*                                                                       *
*   app_error.h --  error code definitions for application module       *
*                                                                       *
*   Copyright (c) VN Corp.  All rights reserved.                   *
*                                                                       *
************************************************************************/

#ifndef _APP_ERROR_
#define _APP_ERROR_

// Severity code 4

//--------------------------------------------------------------------------------
// Audio In Error Codes
//--------------------------------------------------------------------------------
#define AUDIOIN_E_FIRST 0x80040000
#define AUDIOIN_E_LAST  0x8004007F
#define AUDIOIN_S_FIRST 0x00040000
#define AUDIOIN_S_LAST  0x0004007F

//
// MessageId: AUDIOOUT_E_EXAMPLE
//
// MessageText:
//
//  This is an error example.
//
//#define AUDIOOUT_E_EXAMPLE             0x80040000
//
// MessageId: AUDIOOUT_S_EXAMPLE
//
// MessageText:
//
//  This is an success example.
//
//#define AUDIOOUT_S_EXAMPLE             0x00040000

#define AUDIOIN_E_INVALIDHANDLE			ERR_INVALID_HANDLE
#define AUDIOIN_E_OUTOFMEMORY			ERR_OUT_OF_MEMORY
#define AUDIOIN_E_INVALIDARG			ERR_INVALID_ARG
#define AUDIOIN_E_BADFORMAT				0x80040000	// The specified wave format is not supported.
#define AUDIOIN_E_DSCNOTINITIALIZED		0x80040001	// DirectSound object is not initialized
#define AUDIOIN_E_OPENFILEERROR			0x80040002	// Open file error

#define AUDIOIN_S_INVALIDSTARTCAPTURE	0x00040000	// call StartCapture() when in StartCapture status
#define AUDIOIN_S_INVALIDSTOPCAPTURE	0x00040001	// call StopCapture() when in StopCapture status

//--------------------------------------------------------------------------------
// Audio Out Error Codes
//--------------------------------------------------------------------------------
#define AUDIOOUT_E_FIRST 0x80040080
#define AUDIOOUT_E_LAST  0x800400FF
#define AUDIOOUT_S_FIRST 0x00040080
#define AUDIOOUT_S_LAST  0x000400FF

#define AUDIOOUT_E_INVALIDHANDLE		ERR_INVALID_HANDLE
#define AUDIOOUT_E_OUTOFMEMORY			ERR_OUT_OF_MEMORY
#define AUDIOOUT_E_INVALIDARG			ERR_INVALID_ARG
#define AUDIOOUT_E_BADFORMAT			AUDIOIN_E_BADFORMAT
#define AUDIOOUT_E_DSNOTINITIALIZED		AUDIOIN_E_DSCNOTINITIALIZED
#define AUDIOOUT_E_INVALIDWAVEFILE		0x80040080	// Wave file is invalid
#define AUDIOOUT_E_BLANKWAVEFILE		0x80040081	// Wave file is blank
#define AUDIOOUT_E_BUFFEREXIST			0x80040082	// Some buffers still exist. You can't release DirectSound object before deleting these buffers.
#define AUDIOOUT_E_NOMOREBUFFERALLOWED	0x80040083	// The number of buffers is full. Can't create buffer any more.
#define AUDIOOUT_E_INVALIDVOLUME		0x80040084	// Invalid volume value
#define AUDIOOUT_E_INVALIDPAN			0x80040085	// Invalid pan value
#define AUDIOOUT_E_INVALIDFREQUENCY		0x80040086	// Invalid frequency value

#define AUDIOOUT_S_INVALIDPLAY			0x00040080	// call PlayBuffer() when in PLAY status
#define AUDIOOUT_S_INVALIDPAUSE			0x00040081	// call PauseBuffer() when in PAUSE status
#define AUDIOOUT_S_INVALIDSTOP			0x00040082	// call StopBuffer() when in Stop status

//--------------------------------------------------------------------------------
// Video Out Error Codes
//--------------------------------------------------------------------------------
#define VIDEOOUT_E_FIRST 0x80040100
#define VIDEOOUT_E_LAST  0x800401FF
#define VIDEOOUT_S_FIRST 0x00040100
#define VIDEOOUT_S_LAST  0x000401FF

#define VIDEOOUT							0x100				// VideoOut Component Base
#define VIDEOOUT_E_FAIL						S_FAIL
#define VIDEOOUT_E_NOTIMPL					ERR_NOT_IMPLEMENT	// this function is not implement yet
#define VIDEOOUT_E_INVALID_HANDLE			ERR_INVALID_HANDLE  // the handle is not valid
#define VIDEOOUT_E_INVALID_ARG				ERR_INVALID_ARG		// the parameter is not valid
#define VIDEOOUT_E_OUT_OF_MEMORY			ERR_OUT_OF_MEMORY

#define VIDEOOUT_E_NO_OVERLAY_SUPPORT		MAKE_SCODE (1,4,VIDEOOUT+1)// indicate this video card can't support overlay surface
#define VIDEOOUT_E_OVERLAY_NOT_AVAILABLE	MAKE_SCODE (1,4,VIDEOOUT+2)// overlay is not available now, maybe used by another application
#define VIDEOOUT_E_SURFACE_LOST				MAKE_SCODE (1,4,VIDEOOUT+3)// Surface lost 
#define VIDEOOUT_E_CAN_NOT_GET_CAPS			MAKE_SCODE (1,4,VIDEOOUT+4)// Get caps errro
#define VIDEOOUT_E_WRONG_MODE				MAKE_SCODE (1,4,VIDEOOUT+5)// the surface is create in diff mode
#define VIDEOOUT_E_INVALID_PIXELFORMAT		MAKE_SCODE (1,4,VIDEOOUT+6)// draw a bitmap to a fourcc surface
#define VIDEOOUT_E_NO_DIRECTDRAW_OBJECT		MAKE_SCODE (1,4,VIDEOOUT+7)// surface can't find the object of parent
#define VIDEOOUT_E_SURFACE_NOT_FOUND		MAKE_SCODE (1,4,VIDEOOUT+8)// surface not found => null

#define VIDEOOUT_S_OK						S_OK


//--------------------------------------------------------------------------------
// Net Client Error Codes
//--------------------------------------------------------------------------------
#define NETCLIENT_E_FIRST 0x80040200
#define NETCLIENT_E_LAST  0x800402FF
#define NETCLIENT_S_FIRST 0x00040200
#define NETCLIENT_S_LAST  0x000402FF

#define NETCLIENTBASE						0x200				// NetClient Component Base
#define NETCLIENT_E_FAIL					S_FAIL
#define NETCLIENT_S_OK						S_OK
#define NETCLIENT_E_AUTH					MAKE_SCODE (1,4,NETCLIENTBASE+1)

//--------------------------------------------------------------------------------
// Server Utility Error Codes
//--------------------------------------------------------------------------------
#define UTILITY_E_FIRST 0x80040300
#define UTILITY_E_LAST  0x800403FF
#define UTILITY_S_FIRST 0x00040300
#define UTILITY_S_LAST  0x000403FF


//--------------------------------------------------------------------------------
// Synchronization Error Codes
//--------------------------------------------------------------------------------
#define SYNC_E_FIRST 0x80040400
#define SYNC_E_LAST  0x800404FF
#define SYNC_S_FIRST 0x00040400
#define SYNC_S_LAST  0x000404FF


#endif //_APP_ERROR_
