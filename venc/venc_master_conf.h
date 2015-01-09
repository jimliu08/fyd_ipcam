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
 * venc_master_conf.h
 *
 * \brief
 * Header file of video master process setoption implement.
 *
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_MASTER_CONF_H_
#define _VENC_MASTER_CONF_H_

#include "venc_master_local.h"

// define for color entry
#define VIDEO_COLOR_MONO    0
#define VIDEO_COLOR_COLOR   1
#define MAX_VIDEO_COLOR     1

// define for Red Gain Value
#define WB_MAX_REDGAIN 8191
#define WB_MIN_REDGAIN 0
#define WB_MAX_BLUEGAIN 8191
#define WB_MIN_BLUEGAIN 0
#define WB_MAX_GREENGAIN 8191
#define WB_MIN_GREENGAIN 0

// define for video modulation type entry
#define VIDEO_MODULATION_NTSC       0
#define VIDEO_MODULATION_PAL        1
#define VIDEO_MODULATION_AUTO       2
#define MAX_VIDEO_MODULATION        2

#define MAX_VIDEO_RESOLUTION_LEN   11

#define MAX_AE_WINDOW_NUM 9
#define APP_AE_WINDOW_NUM 6
#define MAX_AE_GRID_NUM (16*15)

#define MAX_WB_GRID_NUM (16*15)

#define CE_MODE_AUTO   		0
#define CE_MODE_DEFAULT   	1
#define CE_MODE_USER		2

#define MAX_USER_PATH_LEN 127
#define MAX_TABLE_SIZE	256*4	// 256 elements, each element has most len 3, and 1 for comma.

#define GAMMA_TABLE_SIZE (4096*3)////256*3->4096*3
#define GAMMA_TABLE_TEXT_SIZE (GAMMA_TABLE_SIZE*4) // GAMMA_TABLE_SIZE elements, each element has most len 3, and 1 for comma.

typedef struct video_master_config
{
    //! Sensor capture range's resolution
    CHAR     szCapResolution[MAX_VIDEO_RESOLUTION_LEN + 1];
    //! Sensor color mode color/mono (default: color)
    BOOL     bColor;
    //! Flip the video (default: false)
    BOOL     bFlip;
    //! Mirror the video (default: false)
    BOOL     bMirror;
    BOOL     bEnAutoIRIS;
    BOOL     bEnIRCut;
} TVideoMasterCfg;

typedef enum white_balance_mode
{
	wbmAuto = 0,
	wbmSimple,
	wbmManual,
	wbmIncandescentLight,
	wbmCoolWhiteFluorescentLight,
	wbmSunLight,
	wbmCloudy,
	wbmSunShade,
	wbmUserDefine1,
	wbmUserDefine2,
	wbmNum
} EWBMode;

typedef struct white_balance_typevalue
{
	//! Red Gain
    DWORD    dwRedGain;
	//! Blue Gain
    DWORD    dwBlueGain;
	//! Green Gain
    DWORD   dwGreenGain;
} TWBTypeValue;

typedef struct white_balance_grid
{
  BOOL bSelected;
  DWORD dwWeight;
}TWBGrid;


typedef struct white_balance_config
{
	// if one of WB/RBGain value change, it will be set to TRUE
	BOOL			bModified;
	//! WhiteBalance Select mode (default: Auto)
    EWBMode    		eWBSelectMode;
    //! iterator used
    EWBMode			eWBCurModeId;
	TWBTypeValue	aWBTypeValue[wbmNum];
	
	BOOL 	bGridModified;
	DWORD   dwCurGridID;
	TWBGrid atWBGrids[MAX_WB_GRID_NUM];
	
	BOOL bFullFrameGrid;
	
	BOOL bFullFrameGridSwitch;
} TWBCfg;

typedef struct resolution_powerline_config
{
	//! if resolution or power line frequency change, it will be set to TRUE
	BOOL		bModified;
	BOOL		bResolutionModified;
	BOOL		bFrequencyModified;
    //! Sensor power line frequency 50/60
    DWORD    	dwMode;
} TResPLCfg;

typedef struct color_correction_config
{
	// if anyone of below value change, it will be set to TRUE
	BOOL		bModified;
    FLOAT       flCoeff_RR;
    FLOAT       flCoeff_GR;
    FLOAT       flCoeff_BR;
    FLOAT       flCoeff_RG;
    FLOAT       flCoeff_GG;
    FLOAT       flCoeff_BG;
    FLOAT       flCoeff_RB;
    FLOAT       flCoeff_GB;
    FLOAT       flCoeff_BB;
    FLOAT       flOffset_R;
    FLOAT       flOffset_G;
    FLOAT       flOffset_B;
} TColorCorrectionCfg;

typedef struct tone_mapping_config
{
	// if anyone of below value change, it will be set to TRUE
	BOOL	bModified;
	BOOL	bEnable;
	DWORD	dwTMLevel;
} TTMCfg;


typedef struct black_clamp_config
{
  BOOL	bModified;
  DWORD dwBlackClampGr ; // 0~255,100 
  DWORD dwBlackClampR ; // 0~255,100 
  DWORD dwBlackClampGb; // 0~255,100 
  DWORD dwBlackClampB; // 0~255,100 
}TBlackClampCfg;


typedef struct de_impulse_config
{
  BOOL	bModified;
  BOOL bDeImpulseEn; // from UI
  DWORD	dwDcBotThrd ; // 0~65535 from UI,256
  DWORD	dwDcTopThrd ; // 0~65535 Fixed in xml,0xffff
  DWORD	dwMaxTopLevel; // 0~65535  Fixed in xml,0x4000
  DWORD	dwMaxBotLevel; // 0~65535  Fixed in xml,0x2000
  DWORD	dwMinTopLevel; // 0~65535  Fixed in xml,0x4000
  DWORD	dwMinBotLevel; // 0~65535  Fixed in xml,0x2000
  
}TDeImpulseCfg;

typedef struct contrast_enhancement_config
{
	// if anyone of below value change, it will be set to TRUE
	BOOL	bModified;
	BOOL	bEnable;
	DWORD	dwCEMode;
	DWORD	dwCEBrightness;
	DWORD	dwDefaultMethod;
	DWORD 	dwCounter;
	BYTE 	*pbyCollect;
	BYTE	abyCEUserDefineTbl[256];
	DWORD 	dwPosition;
} TCECfg;

typedef struct gamma_table_config
{
	DWORD dwCounter;
	BYTE *pbyCollect;
	BYTE abyGammaTbl[GAMMA_TABLE_SIZE];
	DWORD dwPosition;
} TGammaTableCfg;

typedef struct auto_exposure_window_info
{
    DWORD           dwStartX;
    DWORD           dwStartY;
    DWORD           dwWidth;
    DWORD           dwHeight;
    // the value is dwWidth * dwHeight
    DWORD			dwWindowPixels;
    QWORD			qwAELuminance;	// get from VideoCap state
    DWORD			dwWeight;
} TAEWinInfo;

typedef struct auto_exposure_info
{
	TAEWinInfo	tAEWindow[MAX_AE_WINDOW_NUM];
} TAEInfo;


typedef struct auto_exposure_grid_range
{
    DWORD           dwStartX;
    DWORD           dwStartY;
    DWORD           dwWidth;
    DWORD           dwHeight;
  
}TAEGridRange;

typedef struct auto_exposure_grid
{
  BOOL bSelected;
  DWORD dwWeight;
}TAEGrid;

typedef struct auto_exposure_config
{
	FLOAT			fXTranslate;
	FLOAT			fYTranslate;
	FLOAT			fXScale;
	FLOAT			fYScale;

	//! if any of these true, it will be set to TRUE
	BOOL		bModified;
	BOOL		bGroupModified;
	BOOL		bMaxShutterModified;
	DWORD		dwMaxShutter;
	BOOL		bMaxGainModified;
	DWORD		dwMaxGain;
	BOOL		bMinShutterModified;
	DWORD		dwMinShutter;
	BOOL		bMinGainModified;
	DWORD		dwMinGain;
	BOOL		bTargetLuminanceModified;
	DWORD		dwTargetLuminance;
	DWORD		dwTargetOffset;
	
	DWORD 		dwMode;
	BOOL		bModeModified;

	DWORD		dwCurWinId;
	BOOL		bMeasureWindowModified;
	TAEWinInfo	tAEWindow[APP_AE_WINDOW_NUM];
	
	BOOL bGridRangeModified;
	TAEGridRange tAEGridRange;
	
	
	BOOL 	bGridModified;
	DWORD   dwCurGridID;
	TAEGrid atAEGrids[MAX_AE_GRID_NUM];
	
	
} TAECfg;
#define AF_CURRENT_CMD_SIZE 128
#define AF_CHANGE_VALUE "changevalue"
#define AF_CHANGE_SPEED "changespeed" 
#define AF_CHANGE_WIN	"changewindow"
#define AF_CHANGE_ZOOM  "changezoom"
#define AF_CHANGE_ZOOM_SPEED "changezoomspeed"
#define AF_CHANGE_NOISE_THRESHOLD "changenoisethreshold"
#define AF_CHANGE_REFOCUS "changerefocus"

typedef struct auto_focus_config
{
  BOOL	bModified;
  BOOL	bValueModified;
  DWORD dwValue;
  BOOL	bSpeedModified;
  DWORD dwSpeed;
  BOOL	bWindowModified;
  DWORD dwStartX;
  DWORD dwStartY;
  DWORD dwWidth;
  DWORD	dwHeight;
  DWORD dwZoomValue;
  BOOL  bZoomValueModified;
  DWORD dwZoomSpeed;
  BOOL bZoomSpeedModified;
  DWORD dwNoiseThreshold;
  BOOL bNoiseThresholdModified;
  BOOL bReFocus;
  BOOL bReFocusModified;
  
  
  FLOAT	fViewXTranslate;
  FLOAT	fViewYTranslate;
  FLOAT	fViewXScale;
  FLOAT	fViewYScale;
  
  DWORD dwViewWidth;
  DWORD	dwViewHeight;
  BOOL bViewResolutionModified;
  
  BOOL	bCurrentCmdModified;
  CHAR szCurrentCmd[AF_CURRENT_CMD_SIZE];

  
} TAFCfg;


typedef enum wdr_level
{
  wdrLevel1=0,
  wdrLevel2,
  wdrLevel3,
  wdrLevelCustomized,
  wdrLevelUnkown,
  wdrLevelNum

}EWDRLevel;

typedef struct wdr_config
{
  BOOL	bModified;
  BOOL bEnable;
  BOOL bEnableModified;
  EWDRLevel eLevel;
  BOOL 	bLevelModified;
  DWORD dwLevelValue;
  BOOL bLevelValueModified;

} TWDRCfg;


typedef enum image_adjustment_mode
{
	imgadjmFixed = 0, //Normal(Day)
	imgadjmDynamic,
	imgadjmNight,//Night
	imgadjmAuto,//Auto
	imgadjmNum
} EImgAdjMode;

typedef struct image_adjustment_config
{
	BOOL		bModified;
	BOOL		bSelectModeModified;
	EImgAdjMode eImgAdjSelectMode;
	BOOL		bBrightModified;
    DWORD       dwBrightness;
	BOOL		bContrastModified;
    DWORD       dwContrast;
	BOOL		bSaturationModified;
    DWORD       dwSaturation;
	DWORD		dwStartLum;
	DWORD		dwEndLum;
	DWORD		dwTolerance;
} TImgAdjCfg;

/* ============================================================================================= */
SCODE VencMaster_ResPLSetoptions(HANDLE hObject);
SCODE VencMaster_WBSetoptions(HANDLE hObject);
SCODE VencMaster_CCSetoptions(HANDLE hObject);
SCODE VencMaster_TMSetoptions(HANDLE hObject);
SCODE VencMaster_BlackCalmpSetoptions(HANDLE hObject);
SCODE VencMaster_DeImpulseSetoptions(HANDLE hObject);
SCODE VencMaster_CESetoptions(HANDLE hObject);
SCODE VencMaster_GammaTableSetoptions(HANDLE hObject);
SCODE VencMaster_AESetoptions(HANDLE hObject);
SCODE VencMaster_AFSetoptions(HANDLE hObject);
SCODE VencMaster_AFSetoptionsFromMsgCtrl(HANDLE hObject);
SCODE VencMaster_ImgAdjSetoptions(HANDLE hObject);
SCODE VencMaster_WDRSetoptions(HANDLE hObject);
//-------------StartElementHandler-------------

//-------------CharDataHandler-------------
//setting
void VencMaster_SetCapResolution(HANDLE hObject, const CHAR *szResolution, SDWORD len);
void VencMaster_SetVideoColor(HANDLE hObject, const CHAR *szColor, SDWORD len);
void VencMaster_SetVideoCMOSMode(HANDLE hObject, const CHAR *szFreq, SDWORD len);
void VencMaster_SetVideoFlip(HANDLE hObject, const CHAR *szFlip, SDWORD len);
void VencMaster_SetVideoMirror(HANDLE hObject, const CHAR *szMirror, SDWORD len);

#endif // _VENC_MASTER_CONF_H_
