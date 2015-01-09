/*
 * $Header: /RD_1/Project/VVTK1000_Linux/warden/apps/webcam/audio/rtpamrpack/src/rtpamrpack.h 1     05/07/29 18:54 Cchuang $
 *
 * Copyright 2000-2005 VN, Inc. All rights reserved.
 *
 * rtpamrpack.h :
 *
 *  header file of rtpamrpack, included externally
 *
 * $History: rtpamrpack.h $
 * 
 * *****************  Version 1  *****************
 * User: Cchuang      Date: 05/07/29   Time: 18:54
 * Created in $/RD_1/Project/VVTK1000_Linux/warden/apps/webcam/audio/rtpamrpack/src
 * 
 * *****************  Version 1  *****************
 * User: Cchuang      Date: 05/06/30   Time: 21:06
 * Created in $/RD_1/Project/VVTK1000/Warden/apps/webcam/audio/RTPAMRPACK/src
 * 
 * *****************  Version 1  *****************
 * User: Aren         Date: 05/03/04   Time: 5:41p
 * Created in $/rd_2/speech/GAMR/ARM/RTPAMRPack_ARM9/RTPAMRPack_Lib
 * New interface
 *
 */

  /*!
 *******************************************************************************
 *
 * Copyright 2000-2005 VN, Inc. All rights reserved.
 *
 * \file
 * rtpamrpack.h
 *
 * \brief
 * RTP packetizer of GSM-AMR interface according to RFC 1890.
 * This version isn't compliant with RFC 1890 fully.
 * Interleaving of frames is not supported.
 *
 * \date
 * 2005/03/04
 *
 * \author
 * Aren Chen
 *
 **************************************************************************** */

/* ------------------------------------------------------------------------ */
#ifndef __RTPAMRPACK_H__
#define __RTPAMRPACK_H__

/* ------------------------------------------------------------------------ */
#include "global_codec.h"

/* ======================================================================== */
/*! @defgroup RTPAMRPack RTP AMR Packetizer Object
 *  @{
 */

/* ======================================================================== */
/*!
 * FOUR_CC Version code of the \b RTPAMRPack
 */
#define RTPAMRPACK_VERSION   MAKEFOURCC(2, 1, 0, 1)

/* ------------------------------------------------------------------------ */
/*! RTPAMRPack IF Options for setting the field \a dwAMR_IF in
 *  \b TRTPAMRPackInitOptions.
 */
typedef enum {

	/*! AMR Interface Format 1. Most applications adopt this one. */
	AMR_IF1 = 1,

	/*! AMR Interface Format 2 */
	AMR_IF2 = 2

} TRTPAMRIFOption;

/* ------------------------------------------------------------------------ */
/*! RTPAMRPack IF Options for setting the field \a dwPayloadFormat in
 *  \b TRTPAMRPackInitOptions. RFC 3267
 */
typedef enum {

	/*! RTP payload format mode in bandwidth efficient. */
	RTP_AMR_FORMAT_BANDWIDTH_EFFICIENT_MODE = 1,

	/*! RTP payload format mode in octet aligned. Most applications adopt this one. */
	RTP_AMR_FORMAT_OCTET_ALIGNED_MODE = 2, 

	/*! AMR Storage Format */
	RTP_AMR_FORMAT_STORAGE = 3

} TRTPAMRModeOption;

/* ------------------------------------------------------------------------ */
/*! RTPAMRPack mode rates are allowable in the field \a byModeSet of \b TRTPAMRPackInitOptions.
 *  They can be combined with | (or) operator.
 */
typedef enum {

	/*! Mode rate 4.75 kbps of GSM-AMR for allowing */
	MODESET_MR475 	= 0x01,

	/*! Mode rate 5.15 kbps of GSM-AMR for allowing */
	MODESET_MR515 	= 0x02,

	/*! Mode rate 5.9 kbps of GSM-AMR for allowing */
	MODESET_MR59 	= 0x04,

	/*! Mode rate 6.7 kbps of GSM-AMR for allowing */
	MODESET_MR67 	= 0x08,

	/*! Mode rate 7.4 kbps of GSM-AMR for allowing */
	MODESET_MR74	= 0x10,

	/*! Mode rate 7.95 kbps of GSM-AMR for allowing */
	MODESET_MR795	= 0x20,

	/*! Mode rate 10.2 kbps of GSM-AMR for allowing */
	MODESET_MR102	= 0x40,

	/*! Mode rate 12.2 kbps of GSM-AMR for allowing */
	MODESET_MR122	= 0x80,

	/*! All mode rates of GSM-AMR for allowing */
	MODESET_ALL		= 0xFF

} TRTPGAMREncTXFrame;

/*! Mode rates of CMR (Codec Mode Request) for setting parameter
 *  \a dwCMR in \b TRTPAMRPackOptions.
 */
typedef enum {

	/*!
	 * Mode rate 4.75 of CMR frame type.
	 */
	CMR_FRAMETYPE_MR475		= 0,

	/*!
	 * Mode rate 5.15 of CMR frame type.
	 */
	CMR_FRAMETYPE_MR515		= 1,

	/*!
	 * Mode rate 5.90 of CMR frame type.
	 */
	CMR_FRAMETYPE_MR59		= 2,

	/*!
	 * Mode rate 6.70 of CMR frame type.
	 */
	CMR_FRAMETYPE_MR67		= 3,

	/*!
	 * Mode rate 7.40 of CMR frame type.
	 */
	CMR_FRAMETYPE_MR74		= 4,

	/*!
	 * Mode rate 7.95 of CMR frame type.
	 */
	CMR_FRAMETYPE_MR795		= 5,

	/*!
	 * Mode rate 10.2 of CMR frame type.
	 */
	CMR_FRAMETYPE_MR102		= 6,

	/*!
	 * Mode rate 12.2 of CMR frame type.
	 */
	CMR_FRAMETYPE_MR122		= 7,

	/*!
	 * No request of CMR frame type.
	 */
	CMR_FRAMETYPE_DEFAULT	= 15

} TCMRFrameType;

/* ======================================================================== */
/*! A data structure of \b RTPAMRPack object. Used in function \b RTPAMRPack_Initial 
 *  and \b RTPAMRPack_QueryMemSize to set initial options.
 */
typedef struct {

    /*! This is a version control parameter. Set this value to
     * \b RTPAMRPACK_VERSION. */
	DWORD					dwVersion;

	/*! This parameter can be assigned if you want to allocate memory for object
	 * outside. If it is NULL, the object will allocate memory for itself. Using
	 * \b RTPAMRPack_QueryMemSize function gets the memory needed. */
	void					*pObjectMem;
	
	/*! Set AMR interface format according to \b TRTPAMRIFOption. */
	DWORD					dwAMR_IF;
	
	/*! The following presents AMR MIME Registrations for SDP. */

	/*! Set the payload format according to \b TRTPAMRModeOption. */
	DWORD					dwPayloadFormat;	// BandwidthEfficient or OctetAligned

	/*! Set the permissible mode rate according to \b TGAMREncTXFrame. 
	 *  Any item can combine with others by | (or) operator.
	 *  If all 8 modes will be supported, \b MODESET_ALL can just be used simply.
	 */
	BYTE					byModeSet;

	/*! Set the period to change mode in the unit frame block. It can change
	 *  any time, if this field set to 1.
	 */
	DWORD					dwModeChangePeriod;

	/*! Set to \b TRUE if it is limited to change mode to neighboring one.
	 *  Or it can change mode between any one.
	 */
	BOOL					bModeChangeNeighbor;

	/*! Set the maximum time in millisecond that can be in one payload/packet.
	 *  This value \b MUST be a multiple of 20ms, e.g. 80, 100, ...
	 */
	DWORD					dwMaxPTime;

	/*! Set to \b TRUE then the payload will include CRC field.
	 *  Or the CRC will be ignored.
	 *  When this field is TRUE, \a dwPayloadFormat will be set to
	 *  \b RTP_AMR_FORMAT_OCTET_ALIGNED_MODE automatically.
	 */
	BOOL					bUseCRC;

	/*! Set to \b TRUE then the payload will employ robust sorting.
	 *  Or it will be in simple sorting.
	 *  When this field is TRUE, \a dwPayloadFormat will be set to
	 *  \b RTP_AMR_FORMAT_OCTET_ALIGNED_MODE automatically.
	 */
	BOOL					bRobustSorting;

	/*! Set the maximum number of frame-block for interleaving.
	 *  If this field is set to \b 0, no interleaving is presented.
	 *  When this field is not 0, \a dwPayloadFormat will be set to
	 *  \b RTP_AMR_FORMAT_OCTET_ALIGNED_MODE automatically.
	 *  \b Warning! In this version, it can \b NOT be set but 0.
	 */
	DWORD					dwInterleavingMaxNum;

	/*! Set the channel number. Do \b NOT set to 0.
	 */
	DWORD					dwNumChannel;

} TRTPAMRPackInitOptions;

/* ------------------------------------------------------------------------ */
/*! A data structure of \b RTPAMRPack object. Used in function \b RTPAMRPack_SetOptions 
 *  to set options while packetizing.
 */
typedef struct {

    /*! This is a version control parameter. Set this value to
     * \b RTPAMRPACK_VERSION. */
	DWORD					dwVersion;

	/*! Set the codec mode request according to \b TCMRFrameType.
     *  This value is as same as what AMR encoder desire.
     */
	DWORD					dwCMR;

	/*! Set the number of audio frames.
     *  This value is as same as what AMR encoder desire.
     */
	DWORD					dwAUNumber;

	/*! Inform the audio frame sizes for each fram.
     *  This field will not militate up to now.
     */
	DWORD					*pdwAUSize;

	/*! The I/O buffer of RTPAMRPack. But your input must be placed x 
	 *  bytes after pIOBuffer. x can be get by calling 
	 *  \b RTPAMRPack_GetReservedSize. */
	void					*pIOBuffer;

} TRTPAMRPackOptions;

/* ------------------------------------------------------------------------ */
/*! A data structure of \b RTPAMRPack object. Used in function \b RTPAMRPack_OneFrame 
 *  The fields in the structure report the packetizing status.
 */
typedef struct {

	/*! The packet size in byte from this packetizing. */
	DWORD dwPacketSize;

} TRTPAMRPackState;

/* ======================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *******************************************************************************
 * \brief
 * Query the version of the \b RTPAMRPack.
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
SCODE DLLAPI RTPAMRPack_GetVersionInfo(BYTE *pbyMajor, BYTE *pbyMinor, BYTE *pbyBuild, BYTE *pbyRevision);

/*!
 *******************************************************************************
 * \brief
 * Query the memory size needed to initialize \b RTPAMRPack instance with given
 * options - \a pInitOptions.
 *
 * \param pInitOptions
 * \a (i) the pointer to \b TRTPAMRPackInitOptions structure to let \b RTPAMRPack
 * object to know how to estimate memory size.
 *
 * \return Memory size needed for this instance with given options in bytes. If
 * it is zero, it means that it can not estimate the size, you should let this
 * instance to allocate memory itself.
 *
 * \note
 * Call this function when you need to allocate memory outside the instance.
 *
 * \see TRTPAMRPackInitOptions
 * \see RTPAMRPack_Initial
 *
 **************************************************************************** */
DWORD DLLAPI RTPAMRPack_QueryMemSize(const void *pInitOptions);

/*!
 *******************************************************************************
 * \brief
 * Create a \b RTPAMRPack instance and initialize it.
 *
 * \param phObject
 * \a (o) the pointer to receive the handle of this \b RTPAMRPack instance.
 *
 * \param pInitOptions
 * \a (i/o) the pointer of data structure \b TRTPAMRPackInitOptions for setting the
 * initialization parameters of the \b RTPAMRPack instance. If you did not set some
 * parameters which are needed, the default value will be set..
 *
 * \retval S_OK
 * Create and initialize this instance successfully.
 * \retval S_FAIL
 * Create or initialize this instance failed.
 *
 * \note
 * Make sure the initialization options in the \b TRTPAMRPackInitOptions structure
 * are set properly. This function must be called before any other functions of
 * the \b RTPAMRPack.
 *
 * \see TRTPAMRPackInitOptions
 * \see RTPAMRPack_Release
 *
 **************************************************************************** */
SCODE DLLAPI RTPAMRPack_Initial(HANDLE *phObject,const void *pInitOptions);

/*!
 *******************************************************************************
 * \brief
 * Reset the exist \b RTPAMRPack instance to initial state.
 *
 * \param hObject
 * \a (i) a handle of \b RTPAMRPack instance.
 *
 * \param pInitOptions
 * \a (i) the pointer of data structure \b TRTPAMRPackInitOptions for setting the new
 * initialization parameters of the \b RTPAMRPack instance. If this parameter is
 * NULL, this fuction only reset the internal state. If you did not set some
 * parameters which are needed, the default value will be set.
 *
 * \retval S_OK
 * Reset and initialize this instance successfully.
 * \retval S_FAIL
 * Reset or initialize this object failed.
 *
 * \note
 * Call this function to reset the \b RTPAMRPack instance to initial state. If you
 * call this function in the thread other than the thread running
 * \b RTPAMRPack_OneFrame, you must make sure that the \b RTPAMRPack_OneFrame is \b NOT
 * running at the same time. You should \b NOT reset with another \a pInitOptions status
 * because of insufficient memory which required.
 *
 * \see TRTPAMRPackInitOptions
 * \see RTPAMRPack_Initial
 * \see RTPAMRPack_OneFrame
 *
 **************************************************************************** */
SCODE DLLAPI RTPAMRPack_Reset(HANDLE hObject, const void *pInitOptions);

/*!
 *******************************************************************************
 * \brief
 * Change \b RTPAMRPack's option after it has been initialized.
 *
 * \param hObject
 * \a (i) a handle of \b RTPAMRPack instance.
 *
 * \param pPackOptions
 * \a (i) the pointer of data structure \b TRTPAMRPackOptions for changing the
 * parameters after the instance has been initialized.
 *
 * \retval S_OK
 * Change options successfully.
 * \retval S_FAIL
 * Change options failed.
 *
 * \note
 * Call this function to change options of the initialized \b RTPAMRPack instance.
 * After this function, \b RTPAMRPack_GetReservedSize should be called to get the reserved size.
 * If you call this function in the thread other than the thread running
 * \b RTPAMRPack_OneFrame, you must make sure that the \b RTPAMRPack_OneFrame is \b NOT
 * running at the same time.
 *
 * \see TRTPAMRPackOptions
 * \see RTPAMRPack_Initial
 * \see RTPAMRPack_GetReservedSize
 * \see RTPAMRPack_OneFrame
 *
 **************************************************************************** */
SCODE DLLAPI RTPAMRPack_SetOptions(HANDLE hObject,const void *pPackOptions);

/*!
 *******************************************************************************
 * \brief
 * Get the reserved size of the \b RTPAMRPack instance that should be reserved
 * in \a pIOBuffer of \b TRTPAMRPackOptions.
 *
 * \param hObject
 * \a (i) a handle of \b RTPAMRPack instance.
 *
 * \return Reserved size in byte that should be reserved in \a pIOBuffer of
 * \b TRTPAMRPackOptions.
 *
 * \note
 * This function should be called after \b RTPAMRPack_SetOptions and before
 * \b RTPAMRPack_OneFrame. It can calculate size according to \a dwAUNumber
 * of \b TRTPAMRPackOptions.
 *
 * \see TRTPAMRPackOptions
 * \see RTPAMRPack_SetOptions
 * \see RTPAMRPack_OneFrame
 *
 **************************************************************************** */
DWORD DLLAPI RTPAMRPack_GetReservedSize(HANDLE hObject);

/*!
 *******************************************************************************
 * \brief
 * Packetize one packet (frame) of \b RTPAMRPack.
 *
 * \param hObject
 * \a (i) a handle of \b RTPAMRPack instance.
 *
 * \param pPackState
 * \a (o) the pointer of data structure \b TRTPAMRPackState to receiving the encoding
 * status of current frame. If this pointer is NULL, the status will not be
 * reported.
 *
 * \retval S_OK
 * Packetize one frame successfully.
 * \retval S_FAIL
 * Packetize one frame failed.
 *
 * \note
 * This function packetize one packet (frame), and report the status of current packet (frame).
 * Before this function, \b RTPAMRPack_SetOptions and \b RTPAMRPack_GetReservedSize should
 * be call and do something properly about AMR bitstream.
 * This function can't be run with \b RTPAMRPack_SetOptions, \b RTPAMRPack_Reset at the same
 * time in different thread with the same instance's handle.
 *
 * \see TRTPAMRPackState
 * \see RTPAMRPack_Initial
 * \see RTPAMRPack_SetOptions
 * \see RTPAMRPack_GetReservedSize
 *
 **************************************************************************** */
SCODE DLLAPI RTPAMRPack_OneFrame(HANDLE hObject, void *pPackState);
	  
/*!
 *******************************************************************************
 * \brief
 * Delete an instance of the \b RTPAMRPack object.
 *
 * \param phObject
 * \a (o) the pointer to the handle of the \b RTPAMRPack instance.
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
 * \see RTPAMRPack_Initial
 *
 **************************************************************************** */
SCODE DLLAPI RTPAMRPack_Release(HANDLE *phObject);

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

/* ======================================================================== */
/** \example rtpamrpack_app.c
 * This is an example of how to use the RTP AMR packetizer in single thread mode.
 */
 
/*! @} */

/* ======================================================================== */

#endif	// __RTPAMRPACK_H__

