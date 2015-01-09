/*
 * $Header: /rd_2/Peripherals/TW2815/TW2815_Lib/video_error.h 13    08/01/21 10:30p Alan $
 *
 * Copyright 2008 VN Inc. All rights reserved.
 *
 * Description:
 *
 * $History: video_error.h $
 * 
 * *****************  Version 13  *****************
 * User: Alan         Date: 08/01/21   Time: 10:30p
 * Updated in $/rd_2/Peripherals/TW2815/TW2815_Lib
 *
 */

/* ============================================================================================== */
#ifndef _VIDEO_ERROR_
#define _VIDEO_ERROR_

// Severity code 1

//--------------------------------------------------------------------------------
// Video Codec Error Codes
//--------------------------------------------------------------------------------
#define VIDEO_E_FIRST 0x80010000
#define VIDEO_E_LAST  0x8001FFFF
#define VIDEO_S_FIRST 0x00010000
#define VIDEO_S_LAST  0x0001FFFF

#define VIDEO_S_STUFFING				0x00010001
#define VIDEO_S_MARKER					0x00010002
#define VIDEO_S_CONTROLLER_SUPPORT		0x00010003

#define VIDEO_E_MISSING_MARKER_BIT		0x80010001
#define VIDEO_E_NO_MARKER				0x80010002
#define VIDEO_E_ERROR_VP_HEADER			0x80010003
#define VIDEO_E_ERROR_MB_HEADER			0x80010004
#define VIDEO_E_ERROR_GB_FLUSH			0x80010005
#define VIDEO_E_MISSING_MB				0x80010006
#define VIDEO_E_EXCEEDING_ONES_COUNT	0x80010007
#define VIDEO_E_EXCEEDING_ZEROS_COUNT	0x80010008
#define VIDEO_E_OUT_RVLD_TABLE			0x80010009
#define VIDEO_E_OUT_BLK_BOUNDARY		0x8001000A
#define VIDEO_E_ESCAPE_CODE_ERROR		0x8001000B
#define VIDEO_E_NO_SUPPORT				0x8001000C
#define VIDEO_E_FLUSH_END_FAIL			0x8001000D

#define VIDEO_E_NULL_POINTER			0x8001000E
#define VIDEO_E_COMPONENT_NUM			0x8001000F
#define VIDEO_E_FRAME_SIZE				0x80010010
#define VIDEO_E_PIXEL_FORMAT			0x80010011
#define VIDEO_E_SUB_SAMPLE_RATIO		0x80010012
#define VIDEO_E_BUFFER_ALIGN			0x80010013

#endif //_VIDEO_ERROR_
