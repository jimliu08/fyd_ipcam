/* =========================================================================================== */
#ifndef __VIDEOCAP_H__
#define __VIDEOCAP_H__

/* =========================================================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "errordef.h"
#include "global_codec.h"
#include "MemMgr.h"

/* =========================================================================================== */
#define VIDEOCAP_VERSION MAKEFOURCC(13, 0, 0, 3)
#define VIDEOCAP_ID_VERSION "13.0.0.3"
//#define __USE_PROFILE__

/* =========================================================================================== */
#define VIDEOCAP_IN_BUS_NUM	    0
#define VIDEOCAP_OUT_BUS_NUM	0

/* =========================================================================================== */
typedef SCODE (* FOnSetVideoSignalOptions)(HANDLE hObject, TVideoSignalOptions *ptOptions);

/* ============================================================================================== */
typedef enum video_cap_rgb2ycbcr_flags
{
	BT601_0_255 = 1,
	BT601_16_235 = 2,
	BT709_0_255 = 3,
	BT709_16_235 = 4
} EVideoCapRGB2YCbCrFlags;

/* ============================================================================================== */
typedef enum video_cap_ch_id_type
{
	NO_CH_ID = 0,
	SYNC_ID = 1,
	BLANKING_ID = 2,
	BOTH_ID = 3
} EVideoCapChIDType;

/* =========================================================================================== */
/*! A data structure used in function
 * \b VideoCap_Initial to set the initial options. */
typedef struct video_cap_init_options
{
	/*! Handle of video device. */
	HANDLE hVideoSignalObject;

	/*! Callback function for setting video device options. */
	FOnSetVideoSignalOptions pfnSetOptions;

	/*! Video capture library version number */
	DWORD dwVersion;

	/*! Maximum frame buffer width, must be multiple of 16. */
	DWORD dwMaxFrameWidth;

	/*! Maximum frame buffer height, must be multiple of 16. */
	DWORD dwMaxFrameHeight;

	/*! Capture width after subsample, must be multiple of 16.*/
	DWORD dwCapWidth;

	/*! Capture height after subsample, must be multiple of 16..*/
	DWORD dwCapHeight;

	/*! Start pixel in horizontal axis.*/
	DWORD dwStartPixel;

	/*! Start line in vertical axis.*/
	DWORD dwStartLine;

	/*! Output pixel format. */
	EPixelFormatFlags eOutPixelFormat;

	/*! Write field data to memory.
	* - 0 => Write two single fields in SDRAM.
	* - 1 => Write one interleaved field in SDRAM.
	*/
	BOOL bFieldMode;

	/*! Invert the field number
	* - 0 => Disable field invert
	* - 1 => Enable field invert
	*/
	BOOL bFieldInvert;

	/*! Specify the video input data type.
	* - VIDEO_CAP_FORMAT_PROGRESSIVE_8BITS_RAW => For 8-bit CCIR601 raw data.
	* - VIDEO_CAP_FORMAT_PROGRESSIVE_16BITS_RAW => For 16-bit CCIR601 raw data.
	* - VIDEO_CAP_FORMAT_INTERLACE_CCIR656 => For interlaced two-field CCIR656 data.
	* - VIDEO_CAP_FORMAT_PROGRESSIVE_CCIR656 => For progressive one-field CCIR656 data.
	* - VIDEO_CAP_FORMAT_BAYER_PATTERN => For Bayer pattern input data.
	*/
	EVideoSignalFormat eFormat;

	/*! The input video frame width. */
	DWORD dwInWidth;

	/*! The input video frame height. */
	DWORD dwInHeight;

	/*! Internal frame buffer number, not implemented yet, should set to 4. */
	DWORD dwBufNum;

	/*! The capture image frames in one second.
	* - 0: Disable frame rate control.
	* - Other value: Frame rate.
	*/
	DWORD dwFrameRate;

	/*! Which device to be opended. */
	DWORD dwDeviceNum;

	/*! Flip the video */
	BOOL bFlip;

	/*! Mirror the video */
	BOOL bMirror;

	/*! Adjust the brightness */
	DWORD dwBrightness;

	/*! Adjust the contrast */
	DWORD dwContrast;

	/*! Adjust the saturation */
	DWORD dwSaturation;

	/*! Video signal frequency enumeration. Please refer to global_codec.h */
	EVideoSignalFrequency eFrequency;

	DWORD adwConfig[200];

	/*! Start addrress of input video buffer allocated outside.
	*	(The start address must be 128 bytes align.)
	*/
	BYTE *pbyCapBufUsrBaseAddr;
	BYTE *pbyStatAEWBBufUsrBaseAddr;
	BYTE *pbyStatHistoBufUsrBaseAddr;
	BYTE *pbyStatFocusBufUsrBaseAddr;

	BYTE byChNum0, byChNum1;
	BOOL bChClkEdge0, bChClkEdge1;
	EVideoCapChIDType eChIDType0, eChIDType1;

	/*! VideoCap object memory address. Set to NULL to allocate object memory internally. */
	void *pObjectMem;

} TVideoCapInitOptions;

/* =========================================================================================== */
/*! A data structure of \b VideoCap object used in function \b VideoCap_Getbuf.
 * Fields in this structure will show the VideoCap states. */
typedef struct video_cap_state
{
	/*! The variable indicates index(0-3) of current video frame captured.*/
	DWORD dwIndex;

	/*! The variable indicates frame count (from VideoCap_Initial() was called) of current video frame captured.*/
	DWORD dwFrameCount;

	/*! Current captured frame second information. */
	DWORD dwSecond;

	/*! Current captured frame millisecond information. */
	DWORD dwMilliSecond;

	DWORD dwMicroSecond;
	BOOL bTimeStampCal;
	DWORD dwInWidth;

	/*! Current captured frame width information. */
	DWORD dwOutWidth;

	/*! Current captured frame height information. */
	DWORD dwOutHeight;

	/*! Current captured frame stride information. */
	DWORD dwOutStride;

	/* Video FIFO full occurred during capturing this frame. */
	BOOL bFifoFull;

	/*! No video input signal was detected. */
	BOOL bNoSignal;

	/*! CCIR protocol error or width check fail*/
	BOOL bCCIRErr;

	/*! Which device to be opended. */
	DWORD dwDeviceNum;

	/*! Current captured frame Y component base address */
	BYTE *pbyYFrame;

	/*! Current captured frame Cb component base address */
	BYTE *pbyCbFrame;

	/*! Current captured frame Cr component base address */
	BYTE *pbyCrFrame;

	DWORD dwAFFocusValueHigh;
	DWORD dwAFFocusValueLow;
	DWORD dwAFCurrentStatus;

/* Version 12.0.0.0 modification, 2012.06.28 */
/* ======================================== */

	DWORD dwAWBRedGain;
	DWORD dwAWBBlueGain;

/* Version 12.0.0.0 modification, 2012.06.28 */
	QWORD qwAWBRedSum;
	QWORD qwAWBGreenSum;
	QWORD qwAWBBlueSum;
/* ======================================== */

	DWORD dwAEShutter;
	DWORD dwAEGain;

	DWORD dwZoomRatio;

	BOOL bAEStable;

	DWORD dwCapH;
	DWORD dwCapV;

} TVideoCapState;

/* =========================================================================================== */
/*! A data structure of \b VideoCap object used in function \b VideoCap_SetOption. */
typedef struct video_cap_options
{
    /*! Option flag, which will decide the content of adwUserData */
	EVideoSignalOptionFlags eOptionFlags;
 	DWORD adwUserData[3];
} TVideoCapOptions;

/* =========================================================================================== */
/*! A data structure of \b VideoCap object used by EVideoSignalOptionFlags of "VIDEO_SIGNAL_OPTION_SET_AUTO_SCENE" in function \b VideoCap_SetOption. */
typedef struct video_cap_isp_param
{
	/*! Adjust the gamma table  */
	BYTE *pbyGammaLut;

	/*! Adjust the contrast enhancement */
	DWORD *pdwCEParam;

	/*! Adjust the tome mapping */
	DWORD *pdwTMParam;

	/*! Adjust the RGB2RGB color matrix */
	SDWORD *psdwColorParam;

	/*! Adjust the saturation & brightness & contrast */
	DWORD *pdwSBCParam;

	/*! Adjust the CFA antialiasing */
	DWORD *pdwAntialiasingParam;

	/*! Adjust the auto exposure */
	DWORD *pdwAEParam;
/* Version 12.0.0.1 modification, 2012.11.02 */
	/*! Adjust the CFA deimpulse */
	DWORD *pdwDeimpulseParam;

	/*! Adjust the black clamp */
	SDWORD *psdwBlackClampParam;

	/*! Adjust the CFA mode */
	DWORD *pdwCFAMode;
/* ======================================== */

/* Version 13.0.0.0 modification, 2013.04.19 */
	/*! Adjust the auto white balance */
	DWORD *pdwAWBParam;

	/*! Adjust the lens shading correction */
	DWORD *pdwLSCParam;
/* ======================================== */
} TVideoCapIspParam;

/* =========================================================================================== */
/*!
 *******************************************************************************
 * \brief
 * Create VideoCap instance and initialize it.
 * \param phObject
 * \a (o) pointer to receive the instance's handle of VideoCap instance.
 * \param ptInitOptions
 * \a (i) the pointer of data structure \b TVideoCapInitOptions for setting
 * the initialization parameters of the \b VideoCap instance.
 * \retval S_OK
 * Initialize \b VideoCap OK.
 * \retval S_FAIL
 * Initialize \b VideoCap failed.
 * \remark
 * This function should be called before using this instance.
 * \see VideoCap_Release
 *******************************************************************************/
SCODE VideoCap_Initial(HANDLE *phObject, TVideoCapInitOptions *ptInitOptions);

/*!
 *******************************************************************************
 * \brief
 * Delete an instance of the \b VideoCap object.
 * \param phObject
 * \a (i) the pointer to the handle of the \b VideoCap instance.
 * \retval S_OK
 * Release VideoCap OK.
 * \retval S_FAIL
 * Release VideoCap failed.
 * \remark
 * After the instance is released, the handle of this instance will be set to
 * zero.
 * \see VideoCap_Initial
 *******************************************************************************/
SCODE VideoCap_Release(HANDLE *phObject);

/*!
 *******************************************************************************
 * \brief
 * Get video buffer for processing.
 * \param hObject
 * \a (i) the pointer to the handle of the \b VideoCap instance.
 * \param ptState
 * \a (i) the pointer of data structure \b TVideoCapStatus to receiving VideoCap
 * status.
 * \retval S_OK
 * Get video buffer successfully.
 * \retval S_FAIL
 * Get video buffer failed.
 * \note
 * If VPE or CMOS hardware error occur, field \a dwErrACK in TVideoCapStatus
 * would be filled with error code.
 * \see VideoCap_Releasebuf
 *******************************************************************************/
SCODE VideoCap_GetBuf(HANDLE hObject, TVideoCapState *ptState);

/*!
 *******************************************************************************
 * \brief
 * Release video buffer for next capture.
 * \param hObject
 * \a (i) the pointer to the handle of the \b VideoCap instance.
 * \retval S_OK
 * Release video buffer successfully.
 * \retval S_FAIL
 * Release video buffer failed.
 * \see VideoCap_Getbuf
 *******************************************************************************/
SCODE VideoCap_ReleaseBuf(HANDLE hObject, TVideoCapState *ptState);

/*!
 *******************************************************************************
 * \brief
 * Get video buffer for processing.
 * \param hObject
 * \a (i) the pointer to the handle of the \b VideoCap instance.
 * \param ptState
 * \a (i) the pointer of data structure \b TVideoCapOptions to provide capture
 * options.
 * \retval S_OK
 * Set option successfully.
 * \retval S_FAIL
 * Set option failed.
 * \note
 * This function can't be called when VideoCap is not stopped.
 * \see VideoCap_Stop
 *******************************************************************************/
SCODE VideoCap_SetOptions(HANDLE hObject, TVideoSignalOptions *ptOptions);

/*!
 *******************************************************************************
 * \brief
 * Wait for interrupt wake up in ISR of VPE device driver.
 * \param hObject
 * \a (i) the pointer to the handle of the \b VideoCap instance.
 * \retval S_OK
 * VideoCap_Sleep OK.
 * \retval S_FAIL
 * VideoCap_Sleep failed.
 * \note
 * Current process would be sleep until new frame capture OK.
 *******************************************************************************/
SCODE VideoCap_Sleep(HANDLE hObject);

/*!
 ******************************************************************************
 * \brief
 * Start videoin device to capture image to buffer
 *
 * \param hObject
 * \a (i) the pointer to the handle of the \b VideoCap instance.
 *
 * \retval S_OK
 * Start VideoCap library successful.
 *
 * \retval S_FAIL
 * Start VideoCap library failed.
 *
 * \see VideoCap_Release
 * \see VideoCap_Stop
 *
 ******************************************************************************
 */
SCODE VideoCap_Start(HANDLE hObject);

/*!
 ******************************************************************************
 * \brief
 * Stop the videoin device
 *
 * \param hObject
 * \a (i) the pointer to the handle of the \b VideoCap instance.
 *
 * \retval S_OK
 * Stop VideoCap library successful.
 *
 * \retval S_FAIL
 * Stop VideoCap library failed.
 *
 * \remark
 * Call this function to stop the VideoCap library operation.
 *
 * \see VideoCap_Release
 * \see VideoCap_Start
 *
 ******************************************************************************
*/
/* Version 2.0.0.0 modification, 2006.06.21 */
SCODE VideoCap_Stop(HANDLE hObject);
/* ======================================== */

/*!
 *******************************************************************************
 * \brief
 * Calculate the byte number required by VideoCap instance.
 * \param ptInitOptions
 * \a (i) the pointer of data structure \b TVideoCapInitOptions for setting
 * the initialization parameters of the \b VideoCap instance.
 * \retval
 * Memory size required by VideoCap instance in bytes.
 * \see VideoCap_Initial
 *******************************************************************************/
DWORD VideoCap_QueryMemSize(TVideoCapInitOptions *ptInitOptions);

/*!
 *******************************************************************************
 * \brief
 * Get version information of VideoCap library
 * \param pbyMajor
 * \a (o) the pointer to the major version number of the \b VideoCap library.
 * \param pbyMinor
 * \a (o) the pointer to the minor version number of the \b VideoCap library.
 * \param pbyBuild
 * \a (o) the pointer to the build version number of the \b VideoCap library.
 * \param pbyRevision
 * \a (o) the pointer to the revision number of the \b VideoCap library.
 * \retval S_OK
 * VideoCap_GetVersionInfo OK.
 * \retval S_FAIL
 * VideoCap_GetVersionInfo failed.
 * \note
 * Current process would be sleep until new frame capture OK.
 *******************************************************************************/
SCODE VideoCap_GetVersionInfo(TVersionNum *ptVersion);

/* =========================================================================================== */
/** \example VideoCap_App.c
 * This is an example of how to use the VideoCap Library.
 *
 */
/* =========================================================================================== */
#endif /*__VIDEOCAP_H__*/
