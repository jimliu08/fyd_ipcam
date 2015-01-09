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
 * venc_videoin_conf.h
 *
 * \brief
 * Header file of videoin process setoption implement
 * 
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#ifndef _VENC_VIDEOIN_CONF_H_
#define _VENC_VIDEOIN_CONF_H_

#include "global_codec.h" //for EPixelFormatFlags


#define MAX_MOTION_WIN_NUM 	16
#define MAX_CAPTION_LEN 	14
#define NON_USED_WINDOW		0xFFFFFFFF



typedef struct videoin_config
{
	BOOL 	bDeInterlace;			// Deinterlacer on/off
	DWORD 	dwMotionThreshold;		// threshold for a motion pixel detection, 0~1023
	DWORD 	dwStaticPeriod;			// period for a static pixel decision, 0~15
	BOOL 	bResetStatistics;		// reset statistics for deinterlacing
	BOOL 	bDemo;					// demonstration of the deinterlacing capability
	BOOL 	bSubtitleDetect;		// enable the subtitle detection
	BOOL 	bTmprlNR;				// enable temporal noise reduction
} TVideoInCfg;

/* ============================================================================================= */

/*The following structs are new IBPE state config for V3*/
//=================================================================//
typedef enum ibpe_deinterlacer_mode
{
	idemWeave = 0,
	idemBlend = 1,		// 1,2,1 blending on the adjacent two fields.
	idemMotionAdaptiveBasic = 2,
	idemMotionAdaptiveAdvanced = 3
} EIBPEDeInterlacerMode;


typedef enum ibpe_noise_reduction_mode
{
	inrmNormal2D = 0,
	inrmNormal3D1Ref = 1,		// 1,2,1 blending on the adjacent two fields.
	inrmNormal3D2Ref  = 2,
	inrmStrong  =3,
	inrmBlend = 4
} EIBPENRMode;

typedef enum ibpe_deinterlacer_pic_mode
{
	idePicINTERLEAVE = 0,	
	idePicPLANAR = 1

}EIBPEDeInterlacerPicFormat;

typedef struct ibpe_ctrl
{
  /* IBPE_CTRL */
  BOOL bYCmpntEn;
  BOOL bCCmpntEn;
  BOOL bYOutEn;
  BOOL bCOutEn;
  EPixelFormatFlags ePixelFormat;
DWORD dwCurrHeight;
DWORD dwCurrWidth;
  
}TIBPECtrl;
//=======================================================================//

typedef struct privacy_mask_config
{
    BOOL    	bEnable;
    BOOL 	bOutPMaskEn;

    BOOL 	bColorChange;
    DWORD   	dwColor;
    DWORD 	dwPMaskFillY;
    DWORD 	dwPMaskFillCb;
    DWORD 	dwPMaskFillCr;
    BOOL	bSwitchChange;
    DWORD   	dwSwitch;
} TPMaskCfg;

// motion content
typedef struct motion_windows_info
{
    BOOL            bEnabled;
    char            szName[MAX_CAPTION_LEN + 1];
    DWORD           dwLeft;
    DWORD           dwTop;
    DWORD           dwWidth;
    DWORD           dwHeight;
    DWORD           dwSensitivity;
    DWORD           dwObjSize;
    DWORD			dwSerialNum;
    // new add item
    BOOL			bEnabledChange;
	BOOL			bPositionChange;
	BOOL			bWindowSizeChange;
	BOOL			bDetectThresholdChange;
	//! if anyone of bEnableChange, bPositionChange, bWindowSizeChange, bDetectThresholdChange changed, bAnyFieldChange will set to TRUE,
	//! we used this field to check if any item change, it will speed up the search time.
	BOOL			bAnyFieldChange;
} TMotionWinInfo;

typedef struct motion_config
{
    BOOL            bMotionEnabled;//bMotionDtctEn;
    TMotionWinInfo  atmwCfg[MAX_MOTION_WIN_NUM];
	DWORD           dwCurWinId;
	// new add item
	BOOL			bMotionEnabledChange;
	// the four items should not changed after initialization.
	FLOAT			fXTranslate;
	FLOAT			fYTranslate;
	FLOAT			fXScale;
	FLOAT			fYScale;
	BOOL 	bMDPMaskEn;
} TMotionCfg;

typedef struct edge_enhancement_config
{
    BOOL  bEdgeEnhancementEnable;
	DWORD dwEdgeEnhanceStrength;
	DWORD dwEdgeEnhanceClip;
	DWORD dwEdgeEnhanceEdgeSensitivity;		//
	DWORD dwEdgeEnhanceNoiseSensitivity;	//0~63
	
	BOOL  bSecondPhaseEdgeEnhancementEnable;
	DWORD dwSecondPhaseEdgeEnhanceStrength;
	DWORD dwSecondPhaseEdgeEnhanceClip;
	DWORD dwSecondPhaseEdgeEnhanceEdgeSensitivity;		//
	DWORD dwSecondPhaseEdgeEnhanceNoiseSensitivity;	//0~63
	
} TEECfg;

typedef struct dei_config
{
  BOOL bDeInterlacerEnable;
//  BOOL bDeInterlaceInFormatPlanar;
  EIBPEDeInterlacerPicFormat eDeInterlacePicFormat;
  EIBPEDeInterlacerMode eDeInterlaceMode;
  
  
}TDEICfg;

typedef struct denoise_config
{
    BOOL	bDeNoiseEnable;
    EIBPENRMode  eNoiseReductionMode;
    DWORD dwNoiseReductionNormalStrength;			//0~200;  default:15
    DWORD dwNoiseReductionStrongRefStrength;
	DWORD dwNoiseReductionStrongCurrWeight;
	BOOL bNoiseReductionStrongMotionAdaptiveEn;  
	
	BOOL	bSecondPhaseDeNoiseEnable;
    EIBPENRMode  eSecondPhaseNoiseReductionMode;
    DWORD dwSecondPhaseNoiseReductionNormalStrength;			//0~200;  default:15
	DWORD dwSecondPhaseNoiseReductionStrongRefStrength;
	DWORD dwSecondPhaseNoiseReductionStrongCurrWeight;
	BOOL bSecondPhaseNoiseReductionStrongMotionAdaptiveEn; 
} TDNCfg;

typedef enum geometric_mode
{
	geomtcmBarrel1 = 0,
	geomtcmBarrel2,
	geomtcmPincushion1,
	geomtcmPincushion2,
	geomtcmUserDefine,
	geomtcmNum
} EGeoMtcMode;

typedef enum geometric_resolution
{
	geomtcres640x480 = 0,
	geomtcres1280x720,
	geomtcres1280x800,
	geomtcres1280x1024,
	geomtcres1920x1080,
	geomtcres2048x1536,
	geomtcres2560x1920,
	geomtcresNum
} EGeoMtcRes;

typedef struct geometric_typevalue
{
	DWORD	dwFwdMpgParam1Y;
	DWORD	dwFwdMpgParam2Y;
	DWORD	dwFwdMpgParam3Y;
	BOOL	bPosFwdMpgParam1Y;
	BOOL	bPosFwdMpgParam2Y;
	BOOL	bPosFwdMpgParam3Y;
	DWORD	dwFwdMpgParam1CbCr;
	DWORD	dwFwdMpgParam2CbCr;
	DWORD	dwFwdMpgParam3CbCr;
	BOOL	bPosFwdMpgParam1CbCr;
	BOOL	bPosFwdMpgParam2CbCr;
	BOOL	bPosFwdMpgParam3CbCr;
} TGeoMtcTypeValue;

typedef struct geometric_config
{
    BOOL			bEnable;
    //! select mode 
    EGeoMtcMode			eGMSelectMode;
    EGeoMtcMode			eGMCurModeId;
    EGeoMtcRes			eGMCurResId;
    TGeoMtcTypeValue 		aaGMTypeValue[geomtcmNum][geomtcresNum];
 
} TGeoMtcCfg;

typedef enum photometric_mode
{
	photomtcdisable = 0,
	photomtcm1,
	photomtcm2,
	photomtcmUserDefine1,
	photomtcmUserDefine2,
	photomtcmNum
} EPhotoMtcMode;

typedef struct photometric_config
{
	// if anyone of below value change, it will be set to TRUE
	BOOL	bModified;
	EPhotoMtcMode ePhotoMtcMode;
	DWORD dwLSCRatio;
} TPhotoMtcCfg;
typedef struct venc_videoin_ibp_options
{
	BOOL    	bIBPEEnable;
	BOOL 		bTWICEIBPEEnable;
	TIBPECtrl 	tIBPECtrl;
	TEECfg 		tEdgeEnhancementCfg;
	TDNCfg		tDeNoiseCfg;
	TGeoMtcCfg	tGeoMtcCfg;
	TDEICfg 	tDeiCfg;	
} TVideoInIBPOptions;

SCODE VencVideoIn_SetImgBackEndProcessReConfig(HANDLE hObject);
SCODE VencVideoIn_SetPMaskReConfig(HANDLE hObject);
SCODE VencVideoIn_SetMotionReConfig(HANDLE hObject);
SCODE VencVideoIn_PhotoMtcReConfig(HANDLE hObject);
SCODE VencVideoIn_PMaskSetoptions(HANDLE hObject);
SCODE VencVideoIn_MotionSetoptions(HANDLE hObject);
SCODE VencVideoIn_ImgBackEndProcessSetoptions(HANDLE hObject);
SCODE VencVideoIn_PhotoMtcSetoptions(HANDLE hObject);

#endif // _VENC_VIDEOIN_CONF_H_
