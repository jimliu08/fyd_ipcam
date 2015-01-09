/*
 * $Header: /rd_2/project/SoC/Linux_Libraries/G726Enc/G726Enc_Lib/G726Enc.h 9     13/01/16 6:19p Andy.tsao $
 *
 * Copyright 2000-2001 VN, Inc. All rights reserved.
 *
 * g726enc.h :
 *
 *  header file of G.726 encoder, included externally
 *
 * History:
 *
 * $History: G726Enc.h $
 * 
 * *****************  Version 9  *****************
 * User: Andy.tsao    Date: 13/01/16   Time: 6:19p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G726Enc/G726Enc_Lib
 * 
 * *****************  Version 8  *****************
 * User: Jaja         Date: 12/12/25   Time: 10:24a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G726Enc/G726Enc_Lib
 *
 * *****************  Version 7  *****************
 * User: Yc.lu        Date: 10/11/29   Time: 7:27p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G726Enc/G726Enc_Lib
 *
 * *****************  Version 6  *****************
 * User: Yc.lu        Date: 10/03/03   Time: 10:23a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G726Enc/G726Enc_Lib
 * Fix comment for dwBufSize @ TG726EncOptions.
 *
 * *****************  Version 5  *****************
 * User: Yc.lu        Date: 10/03/02   Time: 6:26p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G726Enc/g726enc_lib
 * Update to version 1.0.0.4.
 *
 * *****************  Version 4  *****************
 * User: Yc.lu        Date: 10/01/27   Time: 6:05p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G726Enc/G726Enc_Lib
 * Update to Version 1.0.0.3.
 *
 * *****************  Version 3  *****************
 * User: Yc.lu        Date: 09/11/19   Time: 10:55a
 * Updated in $/rd_2/speech/G726/ARM/Encoder/G726Enc/G726Enc_Lib
 * Add SoC Hw module lock-in function.
 *
 * *****************  Version 2  *****************
 * User: Yc.lu        Date: 09/10/12   Time: 9:46a
 * Updated in $/rd_2/speech/G726/ARM/Encoder/G726Enc/G726Enc_Lib
 * Update to 1.0.0.1
 * 1. MODIFICATION: Add ID string declaration - DONE.
 *
 * *****************  Version 1  *****************
 * User: Yc.lu        Date: 09/10/05   Time: 6:39p
 * Created in $/rd_2/speech/G726/ARM/Encoder/G726Enc/G726Enc_Lib
 * G726Enc 1.0.0.0 created.
 *
 * *****************  Version 2  *****************
 * User: Chikuang     Date: 02/12/03   Time: 9:38p
 * Updated in $/rd_2/speech/G726/ARM/G726_ARM/enc_lib
 * add "bFrameBased" option to support new GB/PB engine
 *
 * *****************  Version 1  *****************
 * User: Chikuang     Date: 02/11/19   Time: 6:48p
 * Created in $/rd_2/speech/G726/ARM/G726_ARM/enc_lib
 *
 * *****************  Version 1  *****************
 * User: Chikuang     Date: 02/11/11   Time: 12:47p
 * Created in $/rd_2/speech/G726/C/G726_CODEC/enc_lib
 *
 */

/*!
 *******************************************************************************
 *
 * Copyright 2000-2002 VN, Inc. All rights reserved.
 *
 * \file
 * g726_enc.h
 *
 * \brief
 * G.726 encoder's interface
 *
 * \date
 * 2002/10/29
 *
 * \author
 * CK Chen
 *
 **************************************************************************** */

#ifndef __G726_ENC_H__
#define __G726_ENC_H__

#include "global_codec.h"

/*! @defgroup G726Enc G.726 Encoder Object
 *  @{
 */

/*!
 * FOUR_CC Version code of your \b G726Enc, you should update it if you have
 * changed your encoder's interface.
 */
#define G726ENC_VERSION   MAKEFOURCC(1, 0, 0, 7)
#define G726ENC_ID_VERSION "1.0.0.7"

/*! A data structure of \b G726Enc object used in function \b G726Enc_OneFrame and */
/* \b G726Enc_OneSymbol. The fields in the structure is the encoding status. */
typedef struct {
    /*! Size of encoded symbols*/
    DWORD dwOutSize;

} TG726EncState;

/*! A data structure of \b G726Enc object. Used in function \b G726Enc_Initial to
 * set initial options. */
typedef struct {
    DWORD               dwVersion;

    /*! This parameter can be assigned if you want to allocate memory for object
     * outside. If it is NULL, the object will allocate memory for itself. Using
     * \b G726Enc_QueryMemSize function to get the memory needed. */
    void               *pObjectMem;

    /*! A pointer to an OS dependent callback function. This function is used
     * to enter an OS blocked section or to get a semaphore. You can use the
     * \a dwSemaphore field to pass the data to this callback function. */
    TOnEnterCriticalSec pfnEnter;

    /*! A pointer to an OS dependent callback function. This function is used
     * to leave an OS blocked section or to get a semaphore. You can use the
     * dwSemaphore field to pass the data to this callback function. */
    TOnLeaveCriticalSec pfnLeave;

    /*! A pointer to an OS dependent callback function. In the push mode,
     * when the internal bitstream buffer is full, the encoder will call this
     * function to send bitstreams to the output interface. In the pull mode,
     * when the internal bitstream buffer is full, the encoder will call this
     * function to wait a little time interval to release control to another
     * thread. You can use the dwUserData field to pass the data to this
     * callback function. */
    TOnSendData         pfnSend;

    /*! The variable passed to the \a pfnOnEnterCriticalSec and \a
     * pfnOnLeaveCriticalSec callback function. */
    DWORD               dwSemaphore;

    /*! The variable passed to the \a pfnOnRequestData callback function. */
    DWORD               dwUserData;

    /*! The internal bitstream buffer size. This value should be the power of 2.*/
    DWORD               dwBufferSize;

    /*! Output ADPCM data bitrate: 2 for 16 kbps, 3 for 24 kbps, 4 for 32 kbps,
     * and 5 for 40 kbps G.726 ADPCM bitstream. */
    SDWORD             	swBitRate;

    /*! Input audio data format: 0 for linear PCM, 1 for u-Law, and 2 for A-Law.*/
    DWORD               dwMode;

    /*! Block size (in byte), indicating the size of data processed per G726Enc_OneFrame() call. */
    DWORD               dwBlockSize;

    /*! Use frame-based mode (TRUE) or not (FALSE). */
    BOOL                bFrameBased;

    /*! Pointer of input audio data buffer */
    void               	*pInBuffer;

} TG726EncInitOptions;


/*! A data structure of \b G726Enc object to set options while encoding. */
typedef struct {
    DWORD               dwVersion;

    /*! Output ADPCM data bitrate: 2 for 16 kbps, 3 for 24 kbps, 4 for 32 kbps,
     * and 5 for 40 kbps G.726 ADPCM bitstream. */
    SDWORD             	swBitRate;

    /*! Input PCM data format: 0 for linear PCM, 1 for u-Law, and 2 for A-Law.*/
    DWORD               dwMode;

    /*! Block size (in byte), indicating the size of data processed per G726Enc_OneFrame() call. */
    DWORD               dwBlockSize;

    /*! Pointer of output bitstream buffer. */
    void               	*pOutBuffer;

    /*! Pointer of input audio data buffer. */
    void               	*pInBuffer;

    /*! The internal bitstream buffer size in byte. This value should be the power of 2. Useful only in non-frame-based application. */
    DWORD               dwBufSize;

} TG726EncOptions;



/*!
 *******************************************************************************
 * \brief
 * Create a \b G726Enc instance and initialize it.
 *
 * \param phObject
 * \a (o) the pointer to receive the handle of this \b G726Enc instance.
 *
 * \param pInitOptions
 * \a (i/o) the pointer of data structure \b TG726EncInitOptions for setting the
 * initialization parameters of the \b G726Enc instance. If you did not set some
 * parameters which are needed, the default value will be set and write back to
 * the corresponding fields.
 *
 * \retval S_OK
 * Create and initialize this instance successfully.
 * \retval S_FAIL
 * Create or initialize this instance failed.
 *
 * \note
 * Make sure the initialization options in the \b TG726EncInitOptions structure
 * are set properly. This function must be called before any other functions of
 * the \b G726Enc.
 *
 * \see TG726EncInitOptions
 * \see G726Enc_Release
 *
 **************************************************************************** */
SCODE G726Enc_Initial(HANDLE *phObject, TG726EncInitOptions *pInitOptions);

/*!
 *******************************************************************************
 * \brief
 * Reset the exist \b G726Enc instance to initial state.
 *
 * \param hObject
 * \a (i) a handle of \b G726Enc instance.
 *
 * \param pInitOptions
 * \a (i) the pointer of data structure \b TG726EncInitOptions for setting the new
 * initialization parameters of the \b G726Enc instance. If this parameter is
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
 * Call this function to reset the \b G726Enc instance to initial state. If you
 * call this function in the thread other than the thread running
 * \b G726Enc_OneFrame, you must make sure that the \b G726Enc_OneFrame is \b NOT
 * running at the same time.
 *
 * \see TG726EncInitOptions
 * \see G726Enc_Initial
 * \see G726Enc_OneFrame
 *
 **************************************************************************** */
SCODE G726Enc_Reset(HANDLE hObject, TG726EncInitOptions *pInitOptions);

/*!
 *******************************************************************************
 * \brief
 * Change \b G726Enc's option after it has been initialized.
 *
 * \param hObject
 * \a (i) a handle of \b G726Enc instance.
 *
 * \param pEncOptions
 * \a (i) the pointer of data structure \b TG726EncOptions for changing the
 * parameters after the instance has been initialized.
 *
 * \retval S_OK
 * Change options successfully.
 * \retval S_FAIL
 * Change options failed.
 *
 * \note
 * Call this function to change options of the initialized \b G726Enc instance. If
 * you call this function in the thread other than the thread running
 * \b G726Enc_OneFrame, you must make sure that the \b G726Enc_OneFrame is \b NOT
 * running at the same time.
 *
 * \see TG726EncOptions
 * \see G726Enc_Initial
 * \see G726Enc_OneFrame
 *
 **************************************************************************** */
SCODE G726Enc_SetOptions(HANDLE hObject, TG726EncOptions *pEncOptions);


/*!
 *******************************************************************************
 * \brief
 * Encode eight symbols of \b G726Enc.
 *
 * \param hObject
 * \a (i) a handle of \b G726Enc instance.
 *
 * \param pEncState
 * \a (o) the pointer of data structure \b TG726EncState to receiving the encoding
 * status of current frame. If this pointer is NULL, the status will not be
 * reported.
 *
 * \retval S_OK
 * Encode eight symbols successfully.
 * \retval S_FAIL
 * Encode eight symbols failed.
 *
 * \note
 * This function encode one frame, and report the status of current frame. This
 * function can't be run with \b G726Enc_SetOptions, \b G726Enc_Reset at the same
 * time in different thread with the same instance's handle. In multithread
 * mode, you can call \b G726Enc_Quit to notify this function leaving ASAP.
 *
 * \see TG726EncState
 * \see G726Enc_SetOptions
 * \see G726Enc_Initial
 *
 **************************************************************************** */
SCODE G726Enc_OneFrame(HANDLE hObject, TG726EncState *pEncState);

/*!
 *******************************************************************************
 * \brief
 * Get the bitstream from the internal buffer of the \b G726Enc instance to the
 * output bitsream buffer.
 *
 * \param hObject
 * \a (i) a handle of \b G726Enc instance.
 *
 * \param pbyBuf
 * \a (o) the pointer of the output bitstream buffer.
 *
 * \param dwNumBytes
 * \a (i) the size of the bitstream you want to get from the internal buffer of
 * the \b G726Enc instance.
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
 * incorrect. You can call \b G726Enc_QueryFilledBufferSpace function. This
 * function is thread safe with the function \b G726Enc_OnFrame, and can be runed
 * simutaneously in different threads.
 *
 * \see G726Enc_QueryFilledBufferSpace
 * \see G726Enc_OneFrame
 *
 **************************************************************************** */
SCODE G726Enc_TakeData(HANDLE hObject, BYTE *pbyBuf, DWORD dwNumBytes);

/*!
 *******************************************************************************
 * \brief
 * Inform the \b G726Enc instance to exit \b G726Enc_OneFrame function.
 *
 * \param hObject
 * \a (i) a handle of \b G726Enc instance.
 *
 * \retval S_OK
 * This function always successfully.
 *
 * \note
 * This function won't make the \b G726Enc instance to exit, it just inform the
 * encoder to exit \b G726Enc_OneFrame function ASAP. You must check if it is
 * really exit manually.
 *
 * \see G726Enc_OneFrame
 *
 **************************************************************************** */
SCODE G726Enc_Quit(HANDLE hObject);

/*!
 *******************************************************************************
 * \brief
 * Query the fullness of the internal buffer of the \b G726Enc instance.
 *
 * \param hObject
 * \a (i) a handle of \b G726Enc instance.
 *
 * \return
 * The fullness of the internal buffer.
 *
 * \note
 * NONE
 *
 **************************************************************************** */
DWORD G726Enc_QueryFilledBufferSpace(HANDLE hObject);

/*!
 *******************************************************************************
 * \brief
 * Query the size of the \b G726Enc instance.
 *
 * \param pInitOptions
 * \a (i) the pointer of data structure \b TG726EncInitOptions for setting the new
 * initialization parameters of the \b G726Enc instance. If this parameter is
 * NULL, this fuction only reset the internal state. If you did not set some
 * parameters which are needed, the default value will be set and write back to
 * the corresponding fields.
 *
 * \return
 * The memory requirement size of \b G726Enc instance.
 *
 * \note
 * NONE
 *
 * \see G726Enc_TakeData
 *
 **************************************************************************** */
DWORD G726Enc_QueryMemSize(TG726EncInitOptions *pInitOptions);


/*!
 *******************************************************************************
 * \brief
 * Flush the internal buffer of the \b G726Enc instance.
 *
 * \param hObject
 * \a (i) a handle of \b G726Enc instance.
 *
 * \retval S_OK
 * Flush the internal buffer successfully.
 *
 * \retval S_FAIL
 * Flush the internal buffer failed.
 *
 * \note
 * Before calling this function, be sure that the \b G726Enc_OneFrame function has
 * already exited. You can call the G726Enc_Quit function to notify the
 * \b G726Enc_OneFrame function leaving immediately.
 *
 * \see G726Enc_Quit
 * \see G726Enc_OneFrame
 *
 **************************************************************************** */
SCODE G726Enc_FlushBuffer(HANDLE hObject);

/*!
 *******************************************************************************
 * \brief
 * Delete an instance of the \b G726Enc object.
 *
 * \param phObject
 * \a (i/o) the pointer to the handle of the \b G726Enc instance.
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
 * \see G726Enc_Initial
 *
 **************************************************************************** */
SCODE G726Enc_Release(HANDLE *phObject);

/** \example g726enc_app.c
 * This is an example of how to use the G.726 encoder in single thread mode.
 */

/*! @} */

#endif	// __G726_ENC_H__

