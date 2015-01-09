/* ============================================================================================== */
#ifndef __AUTOSCENE_H__
#define __AUTOSCENE_H__

/* =========================================================================================== */
#include <stdio.h>
#include <stdlib.h>

//#include "vivo_codec.h"
#include "global_codec.h"
#include "typedef.h"

/* =========================================================================================== */
#define AUTOSCENE_VERSION MAKEFOURCC(4, 0, 1, 9)
#define AUTOSCENE_ID_VERSION "4.0.1.9"

/* =========================================================================================== */
/* name Define ISP function enable. */
#define ISP_SET_NONE		0x00000000
#define ISP_SET_GAMMA		0x00000001 //bit 0
#define ISP_SET_CE		0x00000002 //bit 1
#define ISP_SET_TM		0x00000004 //bit 2
#define ISP_SET_COLOR		0x00000008 //bit 3
#define ISP_SET_SBC		0x00000010 //bit 4
#define ISP_SET_ANTIALIASING	0x00000020 //bit 5
#define ISP_SET_YUVNR		0x00000040 //bit 6
#define ISP_SET_EDGE		0x00000080 //bit 7
#define ISP_SET_IRCUT		0x00000100 //bit 8
#define ISP_SET_AE		0x00000200 //bit 9
#define ISP_SET_CFADEIMPULSE	0x00000400 //bit 10
#define ISP_SET_BLACKCLAMP	0x00000800 //bit 11
#define ISP_SET_CFAMODE		0x00001000 //bit 12
#define ISP_SET_PRE_YUVNR	0x00002000 //bit 13
#define ISP_SET_PRE_EDGE	0x00004000 //bit 14
#define ISP_SET_AWB		0x00008000 //bit 15
#define ISP_SET_LSC		0x00010000 //bit 16
#define ISP_SET_ALL		0xFFFFFFFF

/* =========================================================================================== */
/*  For indicating that auto scene setting mode  */
typedef enum
{
	AUTO_SCENE_INITIAL_SETTING = 0,
	AUTO_SCENE_INDOOR_SETTING = 1,
	AUTO_SCENE_OUTDOOR_SETTING = 2,
	AUTO_SCENE_WDR_SETTING = 3,
	AUTO_SCENE_DWDR_SETTING = 4,
	NUM_OF_AUTO_SCENE_SETTING = 5,	
} EAutoSceneUpdateMode;

/* =========================================================================================== */
/*! Auto scene option flag enumeration */
typedef enum auto_scene_option_flags
{
	AUTO_SCENE_OPTION_SET_FREQUENCY = 1,
	AUTO_SCENE_OPTION_SET_AUTO_EXPOSURE_MODE = 2,
	AUTO_SCENE_OPTION_SET_EXPOSURE_LEVEL = 3,
	AUTO_SCENE_OPTION_SET_AUTO_EXPOSURE_MIN_SHUTTER = 4,
	AUTO_SCENE_OPTION_SET_AUTO_EXPOSURE_MAX_SHUTTER = 5,
	AUTO_SCENE_OPTION_SET_AUTO_EXPOSURE_MIN_GAIN = 6,
	AUTO_SCENE_OPTION_SET_AUTO_EXPOSURE_MAX_GAIN = 7,
	AUTO_SCENE_OPTION_SET_IRIS_MODE = 8,
	AUTO_SCENE_OPTION_SET_AUTO_IRIS_ACTIVE_TIME = 9,
	AUTO_SCENE_OPTION_SET_IRCUT_MODE = 10,
	AUTO_SCENE_OPTION_SET_GAMMA_LEVEL = 11,
	AUTO_SCENE_OPTION_SET_CE_LEVEL = 12,
	AUTO_SCENE_OPTION_SET_TM_LEVEL = 13,
	AUTO_SCENE_OPTION_SET_BRIGHT_LEVEL = 14,
	AUTO_SCENE_OPTION_SET_CONTRAST_LEVEL = 15,
	AUTO_SCENE_OPTION_SET_HUE_LEVEL = 16,
	AUTO_SCENE_OPTION_SET_SATURATION_LEVEL = 17,
	AUTO_SCENE_OPTION_SET_MONO = 18,
	AUTO_SCENE_OPTION_SET_NOISE_REDUCTION_MODE = 19,
	AUTO_SCENE_OPTION_SET_2D_NOISE_REDUCTION_LEVEL = 20,
	AUTO_SCENE_OPTION_SET_3D_NOISE_REDUCTION_LEVEL = 21,
	AUTO_SCENE_OPTION_SET_SHARPNESS_LEVEL = 22,
	AUTO_SCENE_OPTION_SET_DWDR_LEVEL = 23,
	AUTO_SCENE_OPTION_SET_AUTO_EXPOSURE_LOCK = 24,
	AUTO_SCENE_OPTION_SET_AUTO_WHITE_BALANCE_LOCK = 25,
	AUTO_SCENE_OPTION_SET_AUTO_WHITE_BALANCE_MODE = 26,
	AUTO_SCENE_OPTION_SET_WDR_RATIO = 27,
	// [NOTE]!! MAXIMUM NUMBER is 27
} EAutoSceneOptionFlags;

/* =========================================================================================== */
/*! A data structure for auto scene option */
typedef struct auto_scene_options
{
    EAutoSceneOptionFlags eOptionFlags;
    DWORD adwUserData[3];
} TAutoSceneOptions;

/* =========================================================================================== */
/*! A data structure for contrast enhance */
typedef struct contrast_enhance_param
{
	/*  Control Contrast enhancement enable : 1:TRUE, 0:FALSE */
	BOOL bEnCE;

	//DWORD dwCEMode;

	/*  Control Brightness signal : valid range (0-6) */
	DWORD dwCEBrightness;

	//DWORD dwDefaultMethod;

	/*  Contrast enhance table data buffer : up to 256 entries */
	DWORD *pdwCEUsrLut;

	/*  Control Table update : 1:TRUE, 0:FALSE */
	BOOL bTblUpdate;
} TContrastEnhanceParam;

/*! A data structure for tone mapping */
typedef struct tone_mapping_param
{
	/*  Control tone mapping enable : 1:TRUE, 0:FALSE */
	BOOL bEnTM;

	/*  Control The blending value of alpha=0.7 and alpha=1 :  valid range (0-4) */
	DWORD dwBlendLevel;
} TToneMappingParam;

typedef struct color_correction_param
{
	/*  Control color correction matrix : 
     		[RR] [GR] [BR] [RG] [GG] [BG] [RB] [GB] [BB]
     		valid range :
     			[RR] [GG] [BB] : -511 ~ 511
     			Others : -255 ~ 255
	 */
	SDWORD asdwRGB2RGBMatrix[9];		

	/** color correction offset value for R,G and B : valid range (-7-7) */
	SDWORD adwOffset[3];
} TColorCorrectionParam;

/*! A data structure for Bright & Saturation & Contrast  */
typedef struct sbc_param
{
	/*  Control The brightness on the output luminance :  valid range (-127~127, 0: off) */
	SDWORD sdwBright;

	/*  Control The Saturation on the output chrominance :  valid range (0~255, 128:off) */
	DWORD dwSaturation;

	/*  Control The contrast on the output luminance :  valid range (-127~127, 0:off) */
	SDWORD sdwContrast;
} TSBCParam;

/*! A data structure for CFA antialiasing. */
typedef struct antialiasing_param
{
	/*  Control the CFA horz./Vert. blending enable for image stability :  1:TRUE, 0:FALSE  */
	DWORD dwCFAHVBlendEn;

	/*  Control the threshold for Bilinear :  valid range (0~3FFFF(base on CFA bitwidth))  */
	DWORD dwCFAHVAvgThrd;

	/*  Control The minimum threshold for hv-blending :  valid range (0~3FFFF(base on CFA bitwidth))  */
	DWORD dwCFAHVBlendMinThrd;

	/*  Control The maximum threshold for hv-blending :  valid range (0~3FFFF(base on CFA bitwidth))  */
	DWORD dwCFAHVBlendMaxThrd;

	/*  Control false-color suppression enable :  1:TRUE, 0:FALSE  */
	DWORD dwCFAFCSEn;

	/*  Control edge-exclude threshold for false-color suppression :  valid range (0~3FFFF(base on CFA bitwidth))  */
	DWORD dwCFAFCSEdgeThrd;

	/*  Control minimum threshold for false-color suppression :  valid range (0~3FFFF(base on CFA bitwidth))  */
	DWORD dwCFAFCSMinThrd;

	/*  Control maximum threshold for false-color suppression :  valid range (0~3FFFF(base on CFA bitwidth))  */
	DWORD dwCFAFCSMaxThrd;

	/*  Control edge-exclude enable for false-color suppression :  1:TRUE, 0:FALSE  */
	DWORD dwCFAFCSExcludeEdge;

	/*  Control alpha value for false-color suppression :  valid range (0~511)  */
	DWORD dwCFAFCSAlpha;
} TAntialiasingParam;

/*! A data structure for YUV domain noise reduction. */
typedef struct YUV_NR_param
{
	/*  Control YUV NR enable : 1:TRUE, 0:FALSE */
	BOOL bEnYUVNR;

	/*  Control noise reduction mode : valid range (0: 2D, 1: 2D_1_REF, 2: 2D_2_REF, 3: 3D, 4: BLEND) */
	DWORD dwNRMode;

	/* The strength of noise reduction 2D mode, valid range (0~168)  */
	DWORD dw2DNRStrength;

	/* The ref frame strength of noise reduction 3D mode, valid range (0~9)*/
	DWORD dw3DNRRefStrength;

	/* The current frame weight of noise reduction 3D mode,  valid range (0~9).*/
	DWORD dw3DNRCurrWeight;

	/*  Enable motion adaptive in noise reduction 3D mode : 1:TRUE, 0:FALSE */
	BOOL bNRMotionAdaptiveEn;
}TYUVNRParam ;

/*! A data structure for edge enhance. */
typedef struct edge_enhance_param
{
	/*  Control edge enhancement enable : 1:TRUE, 0:FALSE */
	BOOL bEnEdgeEnhance;

	/*  Control edge detection sensitivity : valid range (0~7) */
	DWORD dwEdgeSensitivity;

	/*  Control edge detection noise sensitivity : valid range (0~63) */
	DWORD dwNoiseSensitivity;

	/*  Control edge enhancement strength : valid range (0~31) */
	DWORD dwEEStrength;

	/*  Control edge enhancement strength limit : valid range (0~31) */
	DWORD dwEEClip;
} TEdgeEnhanceParam;

/*! A data structure for auto exposure. */
typedef struct auto_exposure_param
{
	/*  Control mode of AE : valid range (0: Auto, 1: Black light, 2: Customized)  */
	DWORD dwMode;

	/*  Control lock of AE : valid range (1:TRUE, 0:FALSE)  */
	BOOL bLock;

	/*  Control convergence speed of AE :  valid range (0~10)  */
	DWORD dwSpeed;

	/*  Control iris status of AE : valid range (0:Fixed to largest, 1:Auto iris, 2:Manual iris)  */
	DWORD dwIrisStatus;

	/*  Control target luminace of AE :  valid range (0~255)  */
	DWORD dwTargetLuma;

	/*  Control  luminace offset of AE :  valid range (0~255)  */
	DWORD dwTargetOffset;

	/*  Control auto Iris active point(exposure time) :  valid range (1~1000000)  */
	DWORD dwIrisActiveTime;

	/* AE Shutter width lower bound (SWAEMode: related to MinShutter), valid range (1~1000000) */
	DWORD dwMinShutter;

	/* AE Shutter width upper bound (SWAEMode: related to MaxShutter), valid range (1~1000000) */
	DWORD dwMaxShutter;

	/* AE Analog gain lower bound (SWAEMode: MinGain), valid range (1~32(base on sensor)) */
	DWORD dwMinGain;

	/* AE Analog gain upper bound (SWAEMode: MaxGain), valid range (1~32(base on sensor)) */
	DWORD dwMaxGain;	
} TAutoExposureParam;

/*! A data structure for auto white balance. */
typedef struct auto_white_balance_param
{
	/*  Control lock of AWB : valid range (1:TRUE, 0:FALSE)  */
	BOOL bLock;

	/*  Control mode of AWB : valid range (0: Auto, 1: Full, 2: Customized)  */
	DWORD dwMode;

	/*  Control manual R gain of AWB : valid range (1~8191)  */
	DWORD dwCustomGainR;

	/*  Control manual B gain of AWB : valid range (1~8191)  */
	DWORD dwCustomGainB;
} TAutoWhiteBalanceParam;

/*! A data structure for lens shading correction. */
typedef struct lens_shading_correction_param
{
	/*  Control LSC enable : 1:TRUE, 0:FALSE */
	BOOL bLSCEn;

	/*  Control Table update : 1:TRUE, 0:FALSE */
	BOOL bTblUpdate;

	/*  LSC table data : buffer size base on video size */
	WORD *pwPhotoLSCTbl;
} TLensShadingCorrectionParam;

/*! A data structure for CFA deimpulse. */
typedef struct CFA_deimpulse_param
{
	/*  Control CFA deimpulse enable : 1:TRUE, 0:FALSE */
	BOOL bDeImpulseEn;

	/*  Control DC top threshold : valid range (0~(1<<(CFA bitwidth)-1))  */
	DWORD dwDcTopThrd;

	/*  Control DC bottom threshold : valid range (0~(1<<(CFA bitwidth)-1))  */
	DWORD dwDcBotThrd;

	/*  Control White impulse sensitivity : valid range (0~(1<<(CFA bitwidth)-1))  */
	DWORD dwMaxTopLevel;

	/*  Control White impulse strength : valid range (0~(1<<(CFA bitwidth)-1))  */
	DWORD dwMaxBotLevel;

	/*  Control Black impulse sensitivity : valid range (0~(1<<(CFA bitwidth)-1))  */
	DWORD dwMinTopLevel;

	/*  Control Black impulse strength : valid range (0~(1<<(CFA bitwidth)-1)) */
	DWORD dwMinBotLevel;
} TCFADeimpulseParam;

/*! A data structure for black clamp. */
typedef struct black_clamp_param
{
	/*  Control black clamp for Gr channel :  valid range (-127~127)  */
	SWORD swBlackClampGr;

	/*  Control black clamp for R channel :  valid range (-127~127)  */
	SWORD swBlackClampR;

	/*  Control black clamp for Gb channel :  valid range (-127~127)  */
	SWORD swBlackClampGb;

	/*  Control black clamp for B channel :  valid range (-127~127)  */
	SWORD swBlackClampB;
} TBlackClampParam;

/*! A data structure of \b Autoscene object used in function \b AutoScene_Initial. */
typedef struct auto_scene_init_options
{
	/*! auto scene library version number */
	DWORD dwVersion;

	/*! auto scene object memory address. Set to NULL to allocate object memory internally. */
	void *pObjectMem;

	/*! auto scene reference config file name. */
	CHAR *pRefCfgFile;

	/*! PreIsp enable for autoscene. */
	BOOL bEnPreYUVISP;
} TAutoSceneInitOptions;

/*! A data structure of \b autoscene object used in function \b AutoScene_ProcessOneFrame.
 * Fields in this structure will show the autoscene param. */
typedef struct auto_scene_state
{
	EAutoSceneUpdateMode eAutosceneUpdateMode;

	/*  Current exposure time come from sensor */
	DWORD dwCurrShutter;

	/*  Current gain come from sensor */
	DWORD dwCurrGain;

	/*  Current AE status come from AE */
	BOOL bAEStable;

	/* Current resize frame Y component base address */
	BYTE *pbyResizeYFrame;

	/* Current resize frame Cb component base address */
	BYTE *pbyResizeCbFrame;

	/* Current resize frame Cr component base address */
	BYTE *pbyResizeCrFrame;

	/* Current 16*15 grid window R G B sum statistics data */
	BYTE *pbyStatAEWBBuf;

	/* Current 64 bins R G B histogram statistics data */
	BYTE *pbyStatHistoBuf;

	/*  Current color temperature come from AWB*/
	DWORD dwCurrColorTemp;

	/* Define which ISP function must be update */
	DWORD dwParamUpdateSel;

	BYTE *pbyGammaLut;
	TContrastEnhanceParam tCEParam;
	TToneMappingParam tTMParam;
	TColorCorrectionParam tColorCorrectionParam;
	TSBCParam tSBCParam;
	TAntialiasingParam tAntialiasingParam;
	TAutoExposureParam tAEParam;
	TCFADeimpulseParam tCFADeimpulseParam;
	TBlackClampParam tBlackClampParam;
	DWORD dwCFAMode;
	TYUVNRParam tYUVNRParam;
	TEdgeEnhanceParam tEdgeEnhanceParam;
	TYUVNRParam tYUVPreNRParam;
	TEdgeEnhanceParam tPreEdgeEnhanceParam;
	TAutoWhiteBalanceParam tAWBParam;
	TLensShadingCorrectionParam tLSCParam;
	BOOL bEnIRCut;
} TAutoSceneState;

/* =========================================================================================== */
SCODE AutoScene_Initial(HANDLE *phObject, TAutoSceneInitOptions *ptInitOptions);
SCODE AutoScene_Release(HANDLE *phObject);
SCODE AutoScene_ProcessOneFrame(HANDLE hObject, TAutoSceneState *ptState);
DWORD AutoScene_QueryMemSize(TAutoSceneInitOptions *ptInitOptions);
SCODE AutoScene_GetVersionInfo(BYTE *pbyMajor, BYTE *pbyMinor, BYTE *pbyBuild, BYTE *pbyRevision);
SCODE AutoScene_SetOptions(HANDLE hObject, TAutoSceneOptions *ptOptions);
/* =========================================================================================== */
#endif //__AUTOSCENE_H__
