/*
 * $Header: /rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib/GAMREnc.h 8     13/01/16 8:54p Jaja $
 *
 * Copyright 2009 VN, Inc. All rights reserved.
 *
 * gamrenc.h :
 *
 *  header file of amr_enc, included externally
 *
 * $History: GAMREnc.h $
 * 
 * *****************  Version 8  *****************
 * User: Jaja         Date: 13/01/16   Time: 8:54p
 * Updated in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib
 * UPDATE: Update version number from 4.0.0.6 to 4.0.0.7 - DONE.
 *
 * *****************  Version 7  *****************
 * User: Andy.tsao    Date: 13/01/16   Time: 6:31p
 * Updated in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib
 *
 * *****************  Version 6  *****************
 * User: Angel        Date: 12/09/07   Time: 2:12p
 * Updated in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib
 *
 * *****************  Version 5  *****************
 * User: Yc.lu        Date: 10/01/27   Time: 5:55p
 * Updated in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib
 * GAMREnc update to 4.0.0.5.
 *
 * *****************  Version 4  *****************
 * User: Yc.lu        Date: 09/12/11   Time: 10:17a
 * Updated in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib
 * Update to 4.0.0.4
 *
 * *****************  Version 3  *****************
 * User: Angel        Date: 09/02/27   Time: 11:58a
 * Updated in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib
 *
 * *****************  Version 2  *****************
 * User: Angel        Date: 09/01/09   Time: 10:49a
 * Updated in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib
 *
 * *****************  Version 1  *****************
 * User: Angel        Date: 09/01/08   Time: 1:43p
 * Created in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib
 *
 * *****************  Version 10  *****************
 * User: Angel        Date: 08/01/31   Time: 1:51p
 * Updated in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib
 *
 * *****************  Version 9  *****************
 * User: Angel        Date: 08/01/22   Time: 4:41p
 * Updated in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib
 *
 * *****************  Version 8  *****************
 * User: Angel        Date: 08/01/22   Time: 2:35p
 * Updated in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib
 *
 * *****************  Version 7  *****************
 * User: Angel        Date: 08/01/03   Time: 7:56p
 * Updated in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib
 *
 * *****************  Version 6  *****************
 * User: Aren         Date: 06/10/16   Time: 5:24p
 * Updated in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib
 * MODIFICATION: compatible to PBEngine linux version 2.0.0.1
 * UPDATE: version 3.0.0.0
 *
 * *****************  Version 5  *****************
 * User: Aren         Date: 05/02/18   Time: 4:40p
 * Updated in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib
 * UPDATE: version 2.0.0.2
 *
 * *****************  Version 4  *****************
 * User: Aren         Date: 05/02/18   Time: 4:19p
 * Updated in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib
 * MODIFICATION: Comment errata in TGAMREncInitOptions
 *
 * *****************  Version 3  *****************
 * User: Aren         Date: 05/02/18   Time: 2:13p
 * Updated in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMREnc_Lib
 * UPDATE: version 2.0.0.1
 *
 * *****************  Version 2  *****************
 * User: Aren         Date: 03/05/16   Time: 6:47p
 * Updated in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMRENC_LIB
 * FEATURE: Change to version 2.0.0.0
 * FEATURE: Add GAMREnc_FlushBuffer
 * FEATURE: Add GAMREnc_Quit
 * FEATURE: Add fields pInputBuffer and dwInputSize in TGAMREncOptions to
 * set the new input buffer.
 * FEATURE: Remove the function GAMREnc_SetNewBuf
 *
 * *****************  Version 1  *****************
 * User: Aren         Date: 03/03/17   Time: 4:27p
 * Created in $/rd_2/speech/GAMR/ARM/GAMR_ARM9/GAMRENC_LIB
 * Optimized on ARM9
 *
 * *****************  Version 1  *****************
 * User: Aren         Date: 03/02/19   Time: 8:54p
 * Created in $/rd_2/speech/GAMR/C/GAMR_PC/GAMREnc_Lib
 * New Version
 *
 * *****************  Version 1  *****************
 * User: Aren         Date: 02/05/07   Time: 2:11p
 * Created in $/rd_2/speech/GAMR/C/GAMR_PC/enc_lib
 *
 *
 */

  /*!
 *******************************************************************************
 *
 * Copyright 2000-2002 VN, Inc. All rights reserved.
 *
 * \file
 * gamrenc.h
 *
 * \brief
 * GSM-AMR encoder's interface
 *
 * \date
 * 2002/12/10
 *
 * \author
 * Aren Chen
 *
 **************************************************************************** */

#ifndef __GAMRENC_H__
#define __GAMRENC_H__

#include "typedef.h"
#include "global_codec.h"

/* ------------------------------------------------------------------------ */
/*! @defgroup GAMREnc GSM-AMR Encoder Object
 *  @{
 */

/* ------------------------------------------------------------------------ */
/*!
 * FOUR_CC Version code of the \b GAMREnc
 */
#define  GAMRENC_VERSION MAKEFOURCC(4, 0, 0, 7)
#define GAMRENC_ID_VERSION "4.0.0.7"

/* ------------------------------------------------------------------------ */
/*! Flags used in fields \a dwFlags of \b TGAMREncInitOptions & \b TGAMREncOptions
 * structure for setting initial options. */
typedef enum {

	/*! Fields \a pfnOnEnterCriticalSec, \a pfnOnLeaveCriticalSec, and
	 * \a dwSemaphore in \b TGAMREncInitOptions are valid. */
	GAMRENC_CRITICALSEC_CALLBACK	= 0x10000,

    /*! Fields \a pfnOnSendData and \a dwUserData in \b TGAMREncInitOptions
	 *  are valid. */
	GAMRENC_SENDDATA_CALLBACK		= 0x20000,

    /*! Field \a bEnableDTX in \b TGAMREncInitOptions is valid. */
	GAMRENC_DTX						= 0x1,

    /*! Field \a dwVADOption in \b TGAMREncInitOptions is valid. */
	GAMRENC_VAD						= 0x2,

    /*! Field \a tmrMode in \b TGAMREncInitOptions or \b TGAMREncOptions is valid. */
	GAMRENC_MODE					= 0x4,

    /*! Field \a dwOutputFormat in \b TGAMREncInitOptions is valid. */
	GAMRENC_OUTPUTFORMAT			= 0x8,

    /*! Field \a pSpeechIn in \b TGAMREncInitOptions or \b TGAMREncOptions is valid. */
	GAMRENC_INPUT_BUFFER			= 0x10,

    /*! Fields \a pOutBuffer and \a dwOutBufferSize in \b TGAMREncOptions are valid. */
	GAMRENC_OUTPUT_BUFFER			= 0x20

} EGAMREncFlags;

//---------------------------------------------------------------------------
/*! Stream format used in fields \a iStreamFormat of \b TGAMREncInitOptions
 * structure for setting initial options. */
typedef enum {

	/*! G.192 format */
	GAMROF_G192   = 0,

	/*! Packed format: the first byte is an index of frametypes and modes,
	 * the following bitstream is packed bitstream. */
	GAMROF_PACKED = 1

} EGAMROutputFormat;

//---------------------------------------------------------------------------
/*!
 * GAMREnc mode used in fields \a tmrMode of \b TGAMREncInitOptions and \b TGAMREncOptions
 * structure for setting initial options.
 * Also used in filed \a tmrActualMode of \b TGAMREncState structure for getting encoded status.
 */
typedef enum {

	/*! Mode rate: 4.75 kbps */
	GAMRMODEOFMR475 = 0,

	/*! Mode rate: 5.15 kbps */
	GAMRMODEOFMR515,

	/*! Mode rate: 5.9 kbps */
	GAMRMODEOFMR59,

	/*! Mode rate: 6.7 kbps */
	GAMRMODEOFMR67,

	/*! Mode rate: 7.4 kbps */
	GAMRMODEOFMR74,

	/*! Mode rate: 7.95 kbps */
	GAMRMODEOFMR795,

	/*! Mode rate: 10.2 kbps */
	GAMRMODEOFMR102,

	/*! Mode rate: 12.2 kbps */
	GAMRMODEOFMR122,

	/*! Set to DTX mode */
	GAMRMODEOFMRDTX

} EGAMREncModeRate;


//---------------------------------------------------------------------------
/*! GAMREnc frame type used in the field \a efTXFrameType of \b TGAMREncState
 *  structure for getting encoded status. */
typedef enum {

	/*! Frame: Speech good */
	GAMRTX_SPEECH_GOOD = 0,

	/*! Frame: SID of first */
	GAMRTX_SID_FIRST,

	/*! Frame: SID of update */
	GAMRTX_SID_UPDATE,

	/*! Frame: no data */
	GAMRTX_NO_DATA,

	/*! Frame: speech degraded */
	GAMRTX_SPEECH_DEGRADED,

	/*! Frame: speech bad */
	GAMRTX_SPEECH_BAD,

	/*! Frame: SID bad */
	GAMRTX_SID_BAD,

	/*! Frame: onset */
	GAMRTX_ONSET

} EGAMREncTXFrame;

// ----------------------------------------------------------------------------
/*! GAMREnc VAD Options for setting the field \a dwVADOption in
 *  \b TGAMREncInitOptions.
 */
typedef enum {

	/*! VAD option 1 */
	GAMR_VAD1 = 1,

	/*! VAD option 2 */
	GAMR_VAD2 = 2

} EGAMREncVADOption;

/* ------------------------------------------------------------------------ */
/*! A data structure of \b GAMREnc object. Used in function \b GAMREnc_Initial to
 * set initial options. */
typedef struct {
    /*! This is a version control parameter. Set this value to
     */
	DWORD					dwVersion;

    /*! This flag should be the combinations of elements in \b TGAMREncFlags
     * using | (or) operator to indicate which of following elements are valid
     * for initial options setup. */
	DWORD					dwFlags;

    /*! This parameter can be assigned if you want to allocate memory for object
     * outside. If it is NULL, the object will allocate memory for itself. Using
     * \b GAMREnc_QueryMemSize function to get the memory needed. */
    void *					pObjectMem;

    /*! A pointer to an OS dependent callback function. This function is used
     * to enter an OS blocked section or to get a semaphore. You can use the
     * \a dwSemaphore field to pass the data to this callback function. */
	TOnEnterCriticalSec		pfnOnEnterCriticalSec;

    /*! A pointer to an OS dependent callback function. This function is used
     * to leave an OS blocked section or to get a semaphore. You can use the
     * dwSemaphore field to pass the data to this callback function. */
	TOnLeaveCriticalSec		pfnOnLeaveCriticalSec;

    /*! A pointer to an OS dependent callback function. In the push mode,
     * when the internal bitstream buffer is full, the encoder will call this
     * function to send bitstreams to the output interface. In the pull mode,
     * when the internal bitstream buffer is full, the encoder will call this
     * function to wait a little time interval to release control to another
     * thread. You can use the dwUserData field to pass the data to this
     * callback function. */
	TOnSendData				pfnOnSendData;

    /*! The variable passed to the \a pfnOnEnterCriticalSec and \a
     * pfnOnLeaveCriticalSec callback function. */
	DWORD					dwSemaphore;

    /*! The variable passed to the \a pfnOnSendData callback function. */
	DWORD					dwUserData;

	/*! The flag to select the buffer type of circular or frame base */
	BOOL					bUseCircularBuf;

    /*! The internal bitstream buffer size in byte. This value should be the power of 2.
	 */
    DWORD					dwBufferSize;

    /*! Set DTX to enable/disable */
	BOOL					bEnableDTX;

    /*! Set VAD option to \b GAMR_VAD1 or \b GAMR_VAD2 */
    DWORD					dwVADOption;

	/*! The output stream format dependes on \b GAMROF_G192 or \b GAMROF_PACKED. */
    DWORD					dwOutputFormat;

    /*! Set encoding mode according to \b EGAMREncModeRate */
    EGAMREncModeRate		eMode;

	/*!
	 * The pointer of the external input speech buffer.
	 * Make sure to be 32-bit alignment.
	 */
	void *					pvSpeechIn;

} TGAMREncInitOptions;

//---------------------------------------------------------------------------
/*! A data structure of \b GAMREnc object to set options while encoding. */
typedef struct {

    /*! This is a version control parameter. Set this value to
    */
    DWORD					dwVersion;

    /*! This flag should be the combinations of elements in \b TGAMREncFlags
     * using | (or) operator to indicate which of following elements are valid
     * for options setup. */
	DWORD					dwFlags;

	/*! The pointer of the external input speech buffer */
	PVOID					pvSpeechIn;

    /*! New pointer of output buffer. */
    PVOID					pvOutBuffer;

    /*! The size in byte is to the new output buffer. */
	DWORD					dwOutBufferSize;

	/*! Set enoding mode */
    EGAMREncModeRate		eMode;

} TGAMREncOptions;

//---------------------------------------------------------------------------
/*! A data structure of \b GAMREnc object used in function \b GAMREnc_OneFrame. The
 * fields in the structure is the encoding status. */
typedef struct {

	/*! The actual mode of the encoder in this frame. */
	EGAMREncModeRate		eActualMode;

	/*! The actual TX frametype of the encoder in this frame. */
    EGAMREncTXFrame			eTXFrameType;

} TGAMREncState;


/* ------------------------------------------------------------------------ */
/*!
 *******************************************************************************
 * \brief
 * Create a \b GAMREnc instance and initialize it.
 *
 * \param phObject
 * \a (o) the pointer to receive the handle of this \b GAMREnc instance.
 *
 * \param pInitOptions
 * \a (i/o) the pointer of data structure \b TGAMREncInitOptions for setting the
 * initialization parameters of the \b GAMREnc instance. If you did not set some
 * parameters which are needed, the default value will be set and write back to
 * the corresponding fields.
 *
 * \retval S_OK
 * Create and initialize this instance successfully.
 * \retval S_FAIL
 * Create or initialize this instance failed.
 *
 * \note
 * Make sure the initialization options in the \b TGAMREncInitOptions structure
 * are set properly. This function must be called before any other functions of
 * the \b GAMREnc.
 *
 * \see TGAMREncInitOptions
 * \see GAMREnc_Release
 *
 **************************************************************************** */
SCODE DLLAPI GAMREnc_Initial(HANDLE *phObject, TGAMREncInitOptions *ptInitOptions);

/*!
 *******************************************************************************
 * \brief
 * Reset the exist \b GAMREnc instance to initial state.
 *
 * \param hObject
 * \a (i) a handle of \b GAMREnc instance.
 *
 * \param pInitOptions
 * \a (i) the pointer of data structure \b TGAMREncInitOptions for setting the new
 * initialization parameters of the \b GAMREnc instance. If this parameter is
 * NULL, this fuction only reset the internal state. If you did not set some
 * parameters which are needed, the default value will be set and write back to
 * the corresponding fields.
 *
 * \retval S_OK
 * Reset and initialize this instance successfully.
 * \retval S_FAIL
 * Reset or initialize this object failed.
 *
 * \note
 * Call this function to reset the \b GAMREnc instance to initial state. If you
 * call this function in the thread other than the thread running
 * \b GAMREnc_OneFrame, you must make sure that the \b GAMREnc_OneFrame is \b NOT
 * running at the same time.
 *
 * \see TGAMREncInitOptions
 * \see GAMREnc_Initial
 * \see GAMREnc_OneFrame
 *
 **************************************************************************** */
SCODE DLLAPI GAMREnc_Reset(HANDLE hObject, TGAMREncInitOptions *ptInitOptions);

/*!
 *******************************************************************************
 * \brief
 * Change \b GAMREnc's option after it has been initialized.
 *
 * \param hObject
 * \a (i) a handle of \b GAMREnc instance.
 *
 * \param pEncOptions
 * \a (i) the pointer of data structure \b TGAMREncOptions for changing the
 * parameters after the instance has been initialized.
 *
 * \retval S_OK
 * Change options successfully.
 * \retval S_FAIL
 * Change options failed.
 *
 * \note
 * Call this function to change options of the initialized \b GAMREnc instance.
 * If you call this function in the thread other than the thread running
 * \b GAMREnc_OneFrame, you must make sure that the \b GAMREnc_OneFrame is \b NOT
 * running at the same time.
 *
 * \see TGAMREncOptions
 * \see GAMREnc_Initial
 * \see GAMREnc_OneFrame
 *
 **************************************************************************** */
SCODE DLLAPI GAMREnc_SetOptions(HANDLE hObject, TGAMREncOptions *ptOptions);

/*!
 *******************************************************************************
 * \brief
 * Query the memory size needed to initialize \b GAMREnc instance with given
 * options - \a pInitOptions.
 *
 * \param pInitOptions
 * \a (i) the pointer to \b TGAMREncInitOptions structure to let \b GAMREnc
 * object to know how to estimate memory size.
 *
 * \return Memory size needed for this instance with given options in bytes. If
 * it is zero, it means that it can not estimate the size, you should let this
 * instance to allocate memory itself.
 *
 * \note
 * Call this function when you need to allocate memory outside the instance.
 *
 * \see TGAMREncInitOptions
 * \see GAMREnc_Initial
 *
 **************************************************************************** */
DWORD DLLAPI GAMREnc_QueryMemSize(TGAMREncInitOptions *ptInitOptions);

/*!
 *******************************************************************************
 * \brief
 * Encode one frame of \b GAMREnc.
 *
 * \param hObject
 * \a (i) a handle of \b GAMREnc instance.
 *
 * \param pEncState
 * \a (o) the pointer of data structure \b TGAMREncState to receiving the encoding
 * status of current frame. If this pointer is NULL, the status will not be
 * reported.
 *
 * \retval S_OK
 * Encode one frame successfully.
 * \retval S_FAIL
 * Encode one frame failed.
 *
 * \note
 * This function encode one frame, and report the status of current frame. This
 * function can't be run with \b GAMREnc_SetOptions, \b GAMREnc_Reset at the same
 * time in different thread with the same instance's handle. In multithread
 * mode, you can call \b GAMREnc_Quit to notify this function leaving ASAP.
 *
 * \see TGAMREncState
 * \see GAMREnc_SetOptions
 * \see GAMREnc_Initial
 * \see GAMREnc_Quit
 *
 **************************************************************************** */
SCODE DLLAPI GAMREnc_ProcessOneFrame(HANDLE hObject, TGAMREncState *ptState);

/*!
 *******************************************************************************
 * \brief
 * Get the bitstream from the internal buffer of the \b GAMREnc instance to the
 * output bitsream buffer.
 *
 * \param hObject
 * \a (i) a handle of \b GAMREnc instance.
 *
 * \param pbyBuf
 * \a (o) the pointer of the output bitstream buffer.
 *
 * \param dwNumBytes
 * \a (i) the size of the bitstream you want to get from the internal buffer of
 * the \b GAMREnc instance.
 *
 * \retval S_OK
 * Get the bitstream from the internal buffer successfully.
 *
 * \retval S_FAIL
 * Get the bitstream from the internal buffer failed.
 *
 * \note
 * Make sure that the size you take is smaller than or equal to the filled space
 * in the internal buffer. Otherwise, it will make the outputs bitstream
 * incorrect. You can call \b GAMREnc_QueryFilledBufferSpace function. This
 * function is thread safe with the function \b GAMREnc_OnFrame, and can be runed
 * simutaneously in different threads.
 *
 * \see GAMREnc_QueryFilledBufferSpace
 * \see GAMREnc_OneFrame
 *
 **************************************************************************** */
SCODE DLLAPI GAMREnc_TakeData(HANDLE hObject, BYTE *pbyOutputBuf, DWORD dwNumBytes);

/*!
 *******************************************************************************
 * \brief
 * Query the fullness of the internal buffer of the \b GAMREnc instance.
 *
 * \param hObject
 * \a (i) a handle of \b GAMREnc instance.
 *
 * \return
 * The fullness of the internal buffer.
 *
 * \note
 * NONE
 *
 * \see GAMREnc_TakeData
 *
 **************************************************************************** */
DWORD DLLAPI GAMREnc_QueryFilledBufferSpace(HANDLE hObject);

/*!
 *******************************************************************************
 * \brief
 * Flush the internal buffer of the \b GAMREnc instance.
 *
 * \param hObject
 * \a (i) a handle of \b GAMREnc instance.
 *
 * \retval S_OK
 * Flush buffer successfully.
 * \retval S_FAIL
 * Flush buffer failed.
 *
 * \note
 * NONE
 *
 **************************************************************************** */
SCODE DLLAPI GAMREnc_FlushBuffer(HANDLE hObject);

/*!
 *******************************************************************************
 * \brief
 * Force to quit the \b GAMREnc as soon as posible.
 *
 * \param hObject
 * \a (i) a handle of \b GAMREnc instance.
 *
 * \retval S_OK
 * Quit successfully.
 * \retval S_FAIL
 * Quit failed.
 *
 * \note
 * NONE
 *
 * \see GAMREnc_OneFrame
 *
 **************************************************************************** */
SCODE DLLAPI GAMREnc_Quit(HANDLE hObject);

/*!
 *******************************************************************************
 * \brief
 * Query the version of the \b GAMREnc.
 *
 * \param pbyMajor
 * \a (o) a pointer to recieve the major of the version.
 *
 * \param pbyMinor
 * \a (o) a pointer to recieve the minor of the version.
 *
 * \param pbyBuild
 * \a (o) a pointer to recieve the build of the version.
 *
 * \param pbyRevision
 * \a (o) a pointer to recieve the revision of the version.
 *
 * \retval S_OK
 * Get the bitstream from the internal buffer successfully.
 *
 * \retval S_FAIL
 * Get the bitstream from the internal buffer failed.
 *
 * \note
 * NONE
 *
 * \see
 * NONE
 *
 **************************************************************************** */
SCODE DLLAPI GAMREnc_GetVersionInfo(BYTE *pbyMajor, BYTE *pbyMinor, BYTE *pbyBuild, BYTE *pbyRevision);

/*!
 *******************************************************************************
 * \brief
 * Delete an instance of the \b GAMREnc object.
 *
 * \param phObject
 * \a (o) the pointer to the handle of the \b GAMREnc instance.
 *
 * \retval S_OK
 * Release the instance successfully.
 * \retval S_FAIL
 * Release the instance failed.
 *
 * \note
 * After the instance is released, the handle of this instance will be set to
 * zero.
 *
 * \see GAMREnc_Initial
 *
 **************************************************************************** */
SCODE DLLAPI GAMREnc_Release(HANDLE *phObject);

//---------------------------------------------------------------------------
/** \example gamrenc_app.c
 * This is an example of how to use the GSM-AMR encoder in single thread mode.
 */

/*! @} */

/* ------------------------------------------------------------------------ */
#endif	// __GAMRENC_H__

/* ------------------------------------------------------------------------ */

