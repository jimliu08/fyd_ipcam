/************************************************************************
*                                                                       *
*   protocol_error.h --  error code definitions for protocol module     *
*                                                                       *
*   Copyright (c) VN Corp.  All rights reserved.                   *
*                                                                       *
************************************************************************/

#ifndef _PROTOCOL_ERROR_
#define _PROTOCOL_ERROR_

// Severity code 3

//--------------------------------------------------------------------------------
// Streaming Client Error Codes
//--------------------------------------------------------------------------------
#define STREAM_E_RUNNINGCONNECTION 	0x80030000
#define STREAM_E_CREATURL 			0x80030001
#define STREAM_E_NOSETTING 			0x80030002
#define STREAM_E_MEDIASETUPPING		0x80030003
#define STREAM_E_NOTSUPPORT			0x80030004
#define STREAM_E_NOTRUNNING			0x80030005
#define STREAM_E_LAST  				0x800300FF
#define STREAM_S_WAITSTOP 			0x00030000
#define STREAM_S_STOPCONNECTION		0x00030001
#define STREAM_S_LAST  				0x000300FF


//--------------------------------------------------------------------------------
// Packetizer/Unpacketizer Error Codes
//--------------------------------------------------------------------------------
#define PACKET_E_REQUESTBUFFAIL		0x80030100
#define PACKET_E_INVALIDMODE		0x80030101
#define PACKET_E_INVALIDTYPE		0x80030102
#define PACKET_E_EXCEEDMAXBUFSIZE	0x80030103
#define PACKET_E_LAST  				0x800301FF
#define PACKET_S_FIRST 				0x00030100
#define PACKET_S_LAST  				0x000301FF


//--------------------------------------------------------------------------------
// Parser/Composer Error Codes
//--------------------------------------------------------------------------------
#define PARSE_E_FIRST 0x80030200
#define PARSE_E_LAST  0x800302FF
#define PARSE_S_FIRST 0x00030200
#define PARSE_S_LAST  0x000302FF



#endif //_PROTOCOL_ERROR_
