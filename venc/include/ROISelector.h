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
 * ROISelector.h
 *
 * \brief
 * Header file of ROI Selector library
 * 
 * \date
 * 2011/06/23
 * 
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _ROISELECTOR_H_
#define _ROISELECTOR_H_
/* ========================================================================= */

#include "global_codec.h"
#define MAX_REC_NUM 16

#if 0
// --------------------- function brief ----------------------------------------
SCODE ROISelector_Initial(HANDLE *phObject, TROISelectorInitOptions *ptROIInitOpts);
SCODE ROISelector_Release(HANDLE *phObject);
SCODE ROISelector_SetOption(HANDLE hObject, TROISelectorOptions *ptROIOpts);
SCODE ROISelector_OneFrame(HANDLE hObject, TROISelectorState *ptROIState);
#endif

/* ========================================================================= */

/*! FOUR_CC Version code of your \b ROI Selector instance. */
#define ROISELECTOR_VERSION   MAKEFOURCC(1, 0, 0, 0)

/* ========================================================================= */

/*! the data structure indicates the initial options of ROI Selector */
typedef struct roiselector_initoptions
{
    //! version of ROI Selector
    DWORD 	dwVersion;
   	//! The frame width
    DWORD 	dwFrameWidth;
   	//! The frame height
    DWORD 	dwFrameHeight;
   	//! The macro block width
    DWORD 	dwMacroBlockWidth;
   	//! The macro block height
    DWORD 	dwMacroBlockHeight;
} TROISelectorInitOptions;

/*! the enumeration indicates the flags of options to set */
typedef enum roiselector_options_flag
{
    //! Change size
    ROI_CHANGE_SIZE = 1,
} EROIOptionFlags;

/*! the data structure indicates the rectangle structure */
typedef struct roiselector_rec
{
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwWidth;
    DWORD   dwHeight;
} TROIRectangle;

/*! the data structure indicates the pointer of one frame statement */
typedef struct roiselector_state
{   
    //! total rectangle number 	
    DWORD           dwRecNum;
    //! rectangle information 	
	TROIRectangle   atROIRectangle[MAX_REC_NUM];
    //! output data
    DWORD           adwTotalIndexNum[MAX_REC_NUM];
    DWORD           *apdwBlockIndex[MAX_REC_NUM];
} TROISelectorState;

typedef struct roiselector_options
{
    /*! Option flag, which will decide the content of adwUserData */
	EROIOptionFlags eOptionFlags;
 	DWORD adwUserData[4];
} TROISelectorOptions;
/* ========================================================================= */

SCODE ROISelector_Initial(HANDLE *phObject, TROISelectorInitOptions *ptROIInitOpts);
SCODE ROISelector_Release(HANDLE *phObject);
SCODE ROISelector_SetOption(HANDLE hObject, TROISelectorOptions *ptROIOpts);
SCODE ROISelector_OneFrame(HANDLE hObject, TROISelectorState *ptROIState);

/* ========================================================================= */
#endif //_ROISELECTOR_H_
