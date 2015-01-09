/*
 * $Header: /rd_2/project/Mozart/Linux_Libraries/AAC4EnC/AAC4Enc_Lib/AAC4Enc.h 20    13/05/28 11:36a Jaja $
 *
 * Copyright 2009 VN, Inc.  All rights reserved.
 *
 * aac4enc.h : header for AAC Core model
 *
 * $History: AAC4Enc.h $
 * 
 * *****************  Version 20  *****************
 * User: Jaja         Date: 13/05/28   Time: 11:36a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/AAC4EnC/AAC4Enc_Lib
 *
 * *****************  Version 19  *****************
 * User: Jaja         Date: 13/01/16   Time: 5:24p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/AAC4EnC/AAC4Enc_Lib
 * UPDATE: Update version number from 2.1.0.12 to 2.1.0.13 - DONE.
 *
 * *****************  Version 18  *****************
 * User: Angel        Date: 12/09/07   Time: 5:38p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/AAC4EnC/AAC4Enc_Lib
 *
 * *****************  Version 17  *****************
 * User: Angel        Date: 12/05/09   Time: 10:13a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/AAC4EnC/AAC4Enc_Lib
 *
 * *****************  Version 14  *****************
 * User: Angel        Date: 12/03/23   Time: 5:35p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AAC4Enc/AAC4Enc_Lib
 *
 * *****************  Version 13  *****************
 * User: Angel        Date: 12/01/19   Time: 2:38p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AAC4Enc/AAC4Enc_Lib
 *
 * *****************  Version 12  *****************
 * User: Angel        Date: 12/01/05   Time: 7:23p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AAC4Enc/AAC4Enc_Lib
 *
 * *****************  Version 11  *****************
 * User: Angel        Date: 11/12/01   Time: 4:34p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AAC4Enc/AAC4Enc_Lib
 *
 * *****************  Version 10  *****************
 * User: Angel        Date: 09/12/11   Time: 5:21p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AAC4Enc/AAC4Enc_Lib
 *
 * *****************  Version 9  *****************
 * User: Angel        Date: 09/04/09   Time: 5:30p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AAC4Enc/AAC4Enc_Lib
 *
 * *****************  Version 8  *****************
 * User: Angel        Date: 09/02/27   Time: 11:34a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AAC4Enc/AAC4Enc_Lib
 *
 * *****************  Version 7  *****************
 * User: Angel        Date: 09/01/09   Time: 11:41a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AAC4Enc/AAC4Enc_Lib
 *
 * *****************  Version 6  *****************
 * User: Angel        Date: 09/01/09   Time: 11:39a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AAC4Enc/AAC4Enc_Lib
 *
 * *****************  Version 5  *****************
 * User: Angel        Date: 08/09/11   Time: 4:28p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AAC4Enc/AAC4Enc_Lib
 *
 * *****************  Version 4  *****************
 * User: Lawrence.hu  Date: 08/02/01   Time: 11:50a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AAC4Enc/AAC4Enc_Lib
 *
 * *****************  Version 3  *****************
 * User: Alan         Date: 07/12/24   Time: 10:22p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AAC4Enc/AAC4Enc_Lib
 *
 * *****************  Version 2  *****************
 * User: Jaja         Date: 07/09/28   Time: 3:06p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 2.0.0.0 to 2.1.0.0 - DONE.
 *
 * *****************  Version 39  *****************
 * User: Yancheng     Date: 05/01/31   Time: 3:27p
 * Updated in $/rd_2/project/Vivaldi/Libraries/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 2.0.0.10 to 2.0.0.11.
 *
 * *****************  Version 38  *****************
 * User: Yancheng     Date: 04/12/28   Time: 2:55p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 2.0.0.9 to 2.0.0.10.
 *
 * *****************  Version 37  *****************
 * User: Yancheng     Date: 04/11/17   Time: 1:50p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 2.0.0.8 to 2.0.0.9.
 *
 * *****************  Version 36  *****************
 * User: Yancheng     Date: 04/10/22   Time: 1:38p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 2.0.0.7 to 2.0.0.8.
 *
 * *****************  Version 35  *****************
 * User: Yancheng     Date: 04/09/15   Time: 11:08a
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 2.0.0.6 to 2.0.0.7.
 *
 * *****************  Version 34  *****************
 * User: Yancheng     Date: 04/09/09   Time: 3:38p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * Update version number from 2.0.0.5 to 2.0.0.6.
 *
 * *****************  Version 33  *****************
 * User: Yancheng     Date: 04/07/14   Time: 4:28p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 2.0.0.3 to 2.0.0.4.
 *
 * *****************  Version 32  *****************
 * User: Yancheng     Date: 04/04/20   Time: 12:41p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update from 2.0.0.2 to 2.0.0.3.
 *
 * *****************  Version 31  *****************
 * User: Yancheng     Date: 04/04/14   Time: 4:27p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 2.0.0.1 to 2.0.0.2.
 *
 * *****************  Version 30  *****************
 * User: Yancheng     Date: 04/04/13   Time: 1:40p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 2.0.0.0 to 2.0.0.1.
 *
 * *****************  Version 29  *****************
 * User: Yancheng     Date: 04/04/09   Time: 6:09p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.3.0.1 to 2.0.0.0.
 *
 * *****************  Version 28  *****************
 * User: Yancheng     Date: 04/04/08   Time: 5:37p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.3.0.0 to 1.3.0.1.
 *
 * *****************  Version 27  *****************
 * User: Cchuang      Date: 04/03/24   Time: 22:21
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.2.0.6 to 1.3.0.0.
 *
 * *****************  Version 26  *****************
 * User: Yancheng     Date: 04/01/29   Time: 7:37p
 * Updated in $/rd_2/project/Vivaldi/Libraries/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.2.0.5 to 1.2.0.6.
 *
 * *****************  Version 25  *****************
 * User: Yancheng     Date: 04/01/02   Time: 11:04a
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * Update: Update version number from 1.2.0.4 to 1.2.0.5.
 *
 * *****************  Version 24  *****************
 * User: Yancheng     Date: 03/12/03   Time: 6:07p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * Update version number from 1.2.0.3 to 1.2.0.4.
 *
 * *****************  Version 23  *****************
 * User: Cchuang      Date: 03/11/26   Time: 4:16p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.2.0.2 to 1.2.0.3
 *
 * *****************  Version 22  *****************
 * User: Yancheng     Date: 03/11/25   Time: 8:39p
 * Updated in $/rd_2/project/Vivaldi/Libraries/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.2.0.1 to 1.2.0.2.
 *
 * *****************  Version 21  *****************
 * User: Yancheng     Date: 03/11/13   Time: 2:04p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.2.0.0 to 1.2.0.1.
 *
 * *****************  Version 20  *****************
 * User: Yancheng     Date: 03/10/21   Time: 7:05p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.1.0.0 to 1.2.0.0.
 *
 * *****************  Version 19  *****************
 * User: Yancheng     Date: 03/10/13   Time: 7:45p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 *
 * *****************  Version 18  *****************
 * User: Cchuang      Date: 03/09/25   Time: 11:53a
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.0.0.15 to 1.0.0.16
 *
 * *****************  Version 17  *****************
 * User: Cchuang      Date: 03/09/18   Time: 6:37p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.0.0.14 to 1.0.0.15
 *
 * *****************  Version 16  *****************
 * User: Cchuang      Date: 03/09/01   Time: 3:21p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.0.0.13 to 1.0.0.14
 *
 * *****************  Version 15  *****************
 * User: Cchuang      Date: 03/08/22   Time: 5:45p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.0.0.12 to 1.0.0.13
 *
 * *****************  Version 14  *****************
 * User: Cchuang      Date: 03/08/18   Time: 2:07p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.0.0.11 to 1.0.0.12
 *
 * *****************  Version 13  *****************
 * User: Cchuang      Date: 03/08/18   Time: 2:02p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 *
 * *****************  Version 12  *****************
 * User: Cchuang      Date: 03/08/04   Time: 4:50p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.0.0.10 to 1.0.0.11
 *
 * *****************  Version 11  *****************
 * User: Cchuang      Date: 03/07/28   Time: 9:09p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.0.0.9 to 1.0.0.10
 *
 * *****************  Version 10  *****************
 * User: Cchuang      Date: 03/07/21   Time: 5:21p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.0.0.8 to 1.0.0.9
 *
 * *****************  Version 9  *****************
 * User: Cchuang      Date: 03/07/17   Time: 3:00p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.0.0.7 to 1.0.0.8
 *
 * *****************  Version 8  *****************
 * User: Yancheng     Date: 03/07/15   Time: 2:25p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.0.0.6 to 1.0.0.7
 *
 * *****************  Version 7  *****************
 * User: Cchuang      Date: 03/07/15   Time: 10:05a
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.0.0.5 to 1.0.0.6
 *
 * *****************  Version 6  *****************
 * User: Yancheng     Date: 03/07/14   Time: 10:45a
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.0.0.4 to 1.0.0.5.
 *
 * *****************  Version 5  *****************
 * User: Yancheng     Date: 03/07/11   Time: 7:04p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: Update version number from 1.0.0.3 to 1.0.0.4.
 *
 * *****************  Version 4  *****************
 * User: Cchuang      Date: 03/07/11   Time: 6:36p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * UPDATE: update to version 1.0.0.3
 *
 * *****************  Version 3  *****************
 * User: Yancheng     Date: 03/07/07   Time: 9:44a
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * 1.update version number to 1.0.0.2.
 * 2.modify the data-flow of quantized spectral coeff from 32-bit to
 * 16-bit.
 *
 * *****************  Version 2  *****************
 * User: Yancheng     Date: 03/07/02   Time: 5:01p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * update version number to 1.0.0.1
 *
 * *****************  Version 1  *****************
 * User: Cchuang      Date: 03/06/26   Time: 1:22p
 * Created in $/rd_2/audio/AAC/C/Encoder/AAC4Enc/AAC4Enc_Lib
 * Everything is new
 *
 * *****************  Version 4  *****************
 * User: Yancheng     Date: 03/03/27   Time: 5:41p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AACENC_v2/enc_lib
 * [version 1.0.0.0]
 * Release at 2003/03/27
 * 1.BUG:the  inaccurate rate control is fixed.
 * 2.REFINEMENT:shrink instance memory.
 * 3.REFINEMENT:improve long block Psy-model.
 * 4.REFINEMENT:valid the functioning of strereo mode option and simplify
 * the aac4encFraming() & aac4encReadOneFrame().
 * 5.REFINEMENT:blos control support mono file.
 * 6.MODIFICATION:cancel "emphasis" field @ ADTS header.
 * 7.MODIFICATION:upgrade pbengine to 4.0.0.3.
 * 8.FEATURE:add version control API.
 *
 * *****************  Version 3  *****************
 * User: Yancheng     Date: 03/01/23   Time: 10:30a
 * Updated in $/rd_2/audio/AAC/C/Encoder/AACENC_v2/enc_lib
 * 1. Remove old-version computation-intensive code,
 *    such as Psy-Model, Quantization engine and etc.
 * 2. Enhance the short-block control. Disable the
 *    calculation of short-block PE but set as 1.0.
 * 3. Use frequency-line cut-off than SFB-based.
 * 4. Construct grouping mechanism but deactivated.
 * 5. New center-clipping threshold generation algorithm
 *    and post-masking compensation for block-switch
 *    control.
 * 6. Restrict the happening of the different block types
 *    for channel-pair.
 * 7. fixed some bus, such as memset section data when
 *    zero input and better bit-allocation.
 *
 * *****************  Version 2  *****************
 * User: Yancheng     Date: 02/12/24   Time: 6:21p
 * Updated in $/rd_2/audio/AAC/C/Encoder/AACENC_v2/enc_lib
 * interface updatation, add
 * AAC4Enc_QueryMemSize()
 * AAC4Enc_FlushBuffer()
 * AAC4Enc_Quit()
 * AAC4Enc_SetOption()
 *
 * *****************  Version 1  *****************
 * User: Yancheng     Date: 02/09/26   Time: 10:36a
 * Created in $/rd_2/audio/AAC/C/Encoder/AACENC_v2/enc_lib
 * modification for new software and file architecture.
 * 1. disable FFT-based Psy-model
 * 2. null block-switching functionality
 * 3. 12 samplingFreq input is ok
 * 4. some bugs from varing bit-rate option fixed
 *
 * *****************  Version 1  *****************
 * User: Yancheng     Date: 02/03/25   Time: 9:53a
 * Created in $/rd_2/audio/AAC/C/Encoder/AACENC_v2/enc_lib
 * Quality enhancement version from Chi-Kuang's AAC encoder
 *
 */

/* =========================================================================================== */
#ifndef __AAC4ENC_H__
#define __AAC4ENC_H__

/* =========================================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typedef.h"
#include "errordef.h"
#include "global_codec.h"
#include "audio_error.h"

/* =========================================================================================== */
#define AAC4ENC_VERSION MAKEFOURCC(2, 1, 0, 14)
/* Version 2.1.0.4 modification, 2009.01.09 */
#define AAC4ENC_ID_VERSION "2.1.0.14"
/* ======================================== */
/* =========================================================================================== */
typedef enum aac4_enc_option_flags
{
    /*! Fields \a pfnOnEnterCriticalSec, \a pfnOnLeaveCriticalSec, and
     * \a dwSemaphore in \b TAAC4EncInitOptions is valid. */
    AAC4ENC_CRITICALSEC_CALLBACK  = 1,

    /*! Fields \a pfnOnSendData and \a dwUserData in TAAC4EncInitOptions or
     * TAAC4EncOptions is valid. */
    AAC4ENC_SENDDATA_CALLBACK     = 2,

    /*! Fields \a pbyBuf in \b TAAC4EncInitOptions is valid. */
    AAC4ENC_NONCIRCULAR_BUFFER    = 4,

    /*! Fields \a dwEncBufSize in \b TAAC4EncInitOptions is valid. */
    AAC4ENC_CIRCULAR_BUFFER       = 8,

    /*! Fields \a pInterOutBuffer and \a dwOutBufSize in
     * \b TAAC4EncOptions is valid. */
    AAC4ENC_SET_OUTPUTBUFFER      = 16,

    /*! Fields \a pInBuffer in TAAC4EncOptions is valid. */
    AAC4ENC_SET_INPUTPCMBUFFER    = 32

} TAAC4EncOptionFlags;

/*!
 * A data structure of \b AAC4Enc object. Used in function \b AAC4Enc_Initial
 * or \b AAC4Enc_Reset to set or reset initial options.
 */
typedef struct aac4_enc_init_options
{
    /*! The is a version control parameter. Set this value as
     * \b AAC4Enc_VERSION. */
    DWORD               dwVersion;

    /*! This flag should be the combination of elements in \b TAAC4EncOptionFlags
     * using | (or) operator to indicate which of following elements are valid
     * for initialization. */
    DWORD               dwFlags;

    /*! A pointer to an OS dependent callback function. This function is used
     * to enter an OS blocked section or to get a semaphore. You can use the
     * \a dwSemaphore field to pass the data to this callback function. */
    FOnEnterCriticalSec pfnOnEnterCriticalSec;

    /*! A pointer to an OS dependent callback function. This function is used
     * to leave an OS blocked section or to get a semaphore. You can use the
     * dwSemaphore field to pass the data to this callback function. */
    FOnLeaveCriticalSec pfnOnLeaveCriticalSec;

    /*! A pointer to an OS dependent callback function. In the push mode,
     * when the internal bitstream buffer is full, the encoder will call this
     * function to send bitstreams to the output interface. In the pull mode,
     * when the internal bitstream buffer is full, the encoder will call this
     * function to wait a little time interval to release control to another
     * thread. You can use the dwUserData field to pass the data to this
     * callback function. */
    FOnSendData         pfnOnSendData;

    /*! A variable passed to the \a pfnOnEnterCriticalSec and \a
     * pfnOnLeaveCriticalSec callback function. */
    DWORD               dwSemaphore;

    /*! A variable passed to the \a pfnOnSendData callback function. */
    DWORD               dwUserData;

    /*! This parameter can be assigned if you want to allocate memory for object
     * outside. If it is NULL, the object will allocate memory for itself. Using
     * \b AAC4Enc_QueryMemSize function to get the memory needed. */
    void                *pObjectMem;

    /*! A pointer to the input PCM data. */
    SWORD               *pswInBuffer;

    /*! A pointer to the internal output bitstream buffer. */
    BYTE                *pbyBuf;

    /*! Use MPEG-2 (0) or MPEG-4 (1). */
    BOOL                bMpeg4;

    /*! Use VMA AE Pow-3/4. This parameter should be discarded when AAC4Enc
	 * is pure sofeware. */
	BOOL                bPow34;

    /*! Internal output bitstream buffer size. */
    DWORD               dwEncBufSize;

    /*! Constant bit-rate:
     * fixed between 8 and 320 kbps per channel.
     * Assign (-1) will let program pick one setting for you. */
    SDWORD              sdwBitRate;

    /*! Sampling frequency of input in Hz (8k ~ 96k). */
    DWORD               dwSampleRate;

    /*! This parameter can be assigned to indicate desired output channel
     * configurations. Seven operation modes are supported as follows:
     * \li \c -1, auto detect: usually pick (1) for stereo and (3) for mono.
     * \li \c  0, stereo: share unused bandwidth.
     * \li \c  1, joint stereo: enable M/S, I/S, or both when necessary.
     * \li \c  2, dual channel: encode each channel total separately.
     * \li \c  3, mono: single channel encoding.*/
    SWORD               swStereoMode;

    /*! Number of MDCT frequency line which will be encoded (0~1024).
     * It is suggested to assign (-1) to let program automatically set. */
    SWORD               swBandWidth;

    /*! Bit-stream format:
     * (0) raw data,
     * (1) ADTS,
     * (2) ADIF. */
    WORD                wADTS;

    /*! Number of channels to be encoded (1~2). */
    WORD                wChannel;

} TAAC4EncInitOptions;

typedef struct aac4_enc_options
{
    /*! The is a version control parameter. Set this value as
     * \b AAC4Enc_VERSION. */
    DWORD dwVersion;

    /*! This flag should be the combination of the elements in \b TAAC4EncOptionFlags
     * using | (or) operator to indicate which of following elements are valid
     * for options setup. */
    DWORD dwFlags;

    /*! A pointer to the PCM input buffer. */
    void *pInBuffer;

    /*! A pointer to the encoder internal output buffer. */
    void *pInterOutBuffer;

    /*! The size of the encoder internal output buffer in Byte. */
    DWORD dwOutBufSize;

} TAAC4EncOptions;

typedef struct aac4_enc_state
{
    /*! The PCM data size in Byte under processing. */
	DWORD dwInSize;

    /*! Internal frame counter. */
    DWORD dwFrameIndx;

    /*! The encoded bitstream size in Byte. */
    DWORD dwBitreamSize;

    /*! The channel number corresponding to the encoded bitstream. */
    DWORD dwChannel;

    /*! The sample-rate corresponding to the encoded bitstream. */
    DWORD dwSampleRate;

} TAAC4EncState;

/* =========================================================================================== */
#ifdef __cplusplus
extern "C" {
#endif

/* Main encoding process functions */
SCODE AAC4Enc_Initial(HANDLE *phObject, TAAC4EncInitOptions *ptInitOptions);
SCODE AAC4Enc_OneFrame(HANDLE hObject, TAAC4EncState *ptState);
SCODE AAC4Enc_Release(HANDLE *phObject);
SCODE AAC4Enc_SpecificConfig(HANDLE hObject, BYTE *pbyBuf, DWORD *pdwEncBytes, DWORD *pdwProfileLevel, DWORD dwBufSize);

/* This function can't be called simultaneously with the main encoding process functions. */
SCODE AAC4Enc_SetOptions(HANDLE hObject, TAAC4EncOptions *ptOptions);

/* These functions can be called simultaneously with the main encoding process functions. */
SCODE AAC4Enc_TakeData(HANDLE hObject, BYTE *pbyBuf, DWORD dwNumBytes);
SCODE AAC4Enc_Quit(HANDLE hObject);
DWORD AAC4Enc_QueryFilledBufferSpace(HANDLE hObject);
DWORD AAC4Enc_QueryMemSize(TAAC4EncInitOptions *ptInitOptions);
SCODE AAC4Enc_Reset(HANDLE hObject, TAAC4EncInitOptions *ptInitOptions);
SCODE AAC4Enc_FlushBuffer(HANDLE hObject);
DWORD AAC4Enc_GetReqBuffSize(DWORD dwSampleRate, SDWORD sdwBitRate, SWORD swStereoMode);

SCODE AAC4Enc_GetVersionInfo(BYTE *pbyMajor, BYTE *pbyMinor, BYTE *pbyBuild, BYTE *pbyRevision);

#ifdef __cplusplus
}
#endif

/* =========================================================================================== */
#endif //__AAC4ENC_H__
