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
 * frameratectrl.h
 *
 * \brief
 * Header file of frame rate control library
 * 
 * \date
 * 2009/09/21
 * 
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _FRAMERATECTRL_H_
#define _FRAMERATECTRL_H_
/* ========================================================================= */

#include "global_codec.h"

#if 0
// --------------------- function brief ----------------------------------------
SCODE FrameRateCtrl_Initial(HANDLE *phObject, TFrameRateInitOptions *ptFRCtrlInitOpts);
SCODE FrameRateCtrl_Release(HANDLE *phObject);
SCODE FrameRateCtrl_SetOption(HANDLE hObject, TFRCtrlOptions *ptFRCtrlOpts);
SCODE FrameRateCtrl_OneFrame(HANDLE hObject, TFrameRateCtrlState *ptFRCtrlState);
#endif

/* ========================================================================= */

/*! FOUR_CC Version code of your \b FrameRateCtrl instance. */
#define FRAMERATECTRL_VERSION   MAKEFOURCC(2, 0, 0, 3)

/* ========================================================================= */

/*! the data structure indicates the initial options of frame rate control */
typedef struct frameratectrl_initoptions
{
    //! version of frame rate control
    DWORD 	dwVersion;
   	//! The frames in one second. 0: Disable frame rate control. Other value: Frame rate.
    DWORD 	dwFrameRate;
} TFrameRateInitOptions;

/*! the enumeration indicates the flags of options to set */
typedef enum frctrl_options_flag
{
    //! Change frame rate
    FRCTRL_CHANGE_FRAMERATE = 1,
} EFRCtrlOptionFlags;

/*! the data structure indicates the pointer of frames */
typedef struct frameratectrl_state
{    	
	//! Time of second unit
	DWORD	dwSecond;
	//! Time of millisecond unit
	DWORD	dwMilliSecond;
	//! Time of microsecond unit
	DWORD	dwMicroSecond;
	//! available or not
	BOOL	bAvailable;
} TFrameRateCtrlState;

typedef struct frctrl_options
{
    /*! Option flag, which will decide the content of adwUserData */
	EFRCtrlOptionFlags eOptionFlags;

 	DWORD adwUserData[3];
} TFRCtrlOptions;
/* ========================================================================= */

SCODE FrameRateCtrl_Initial(HANDLE *phObject, TFrameRateInitOptions *ptFRCtrlInitOpts);
SCODE FrameRateCtrl_Release(HANDLE *phObject);
SCODE FrameRateCtrl_SetOption(HANDLE hObject, TFRCtrlOptions *ptFRCtrlOpts);
SCODE FrameRateCtrl_OneFrame(HANDLE hObject, TFrameRateCtrlState *ptFRCtrlState);

/* ========================================================================= */
#endif //_FRAMERATECTRL_H_
