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
 * venc_display_conf.h
 *
 * \brief
 * Header file of video display process setoption implement
 * 
 * \date
 * 2009/09/11
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_DISPLAY_CONF_H_
#define _VENC_DISPLAY_CONF_H_

/*
   <output>off</output> <!--on,off-->
   <output_format>HDMI</output_format> <!-- NTSC, PAL, HDMI, HDSDI -->
   <output_resolution>1280x720</output_resolution>
   <output_freq>30</output_freq> <!--30,60-->
 */

typedef enum venc_display_output_en
{
	vDispOutEnable = 1,
	vDispOutDisable,
	vDispUnknownState
} EDispOutEn;

//! output format
typedef enum venc_display_output_format
{
	vDispOutFmtNTSC = 1,
	vDispOutFmtPAL,
	vDispOutFmtHDMI,
	vDispOutFmtHDSDI,
	vDispOutFmtUnknown
} EDispOutFmt;

#endif //_VENC_DISPLAY_CONF_H_

