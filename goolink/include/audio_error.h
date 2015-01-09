/************************************************************************
*                                                                       *
*   audio_error.h --  error code definitions for audio codec module     *
*                                                                       *
*   Copyright (c) VN Corp.  All rights reserved.                   *
*                                                                       *
************************************************************************/

#ifndef _AUDIO_ERROR_
#define _AUDIO_ERROR_

// Severity code 2

//--------------------------------------------------------------------------------
// Audio Codec Error Codes
//--------------------------------------------------------------------------------
#define AUDIO_E_FIRST 0x80020000
#define AUDIO_E_LAST  0x8002FFFF
#define AUDIO_S_FIRST 0x00020000
#define AUDIO_S_LAST  0x0002FFFF

#define AUDIO_S_OUTPUTBUFFER	0x00020010
#define AUDIO_E_OUTPUTBUFFER	0x80020010
#define AUDIO_E_BITRATE     	0x80020011

#endif //_AUDIO_ERROR_
