/*
 *******************************************************************************
 * $Header: $
 *
 *  Copyright (c) 2007-2012 VN Inc. All rights reserved.
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
 * Copyright 2007-2012 VN, Inc. All rights reserved.
 *
 * \file
 * RtspSrvr.h
 *
 * \brief
 * Rtsp Server Library header file.
 *
 * \date
 * 2008/08/11
 *
 * \author
 * djhow.tu@VN.com
 *
 *
 *******************************************************************************
 */

/*! \mainpage RtspSrvr library 
 *
 * \section intro_sec Introduction
 * \par
 * RtspSrvr is a multimedia live streaming server library compliant with IETF's 
 * standards for real-time streaming of multimedia content over Internet.
 * <br>
 * RtspSrvr implements RTSP(<a href="http://www.ietf.org/rfc/rfc2326.txt">RFC 2326</a>) 
 * and RTP/RTCP(<a href="http://www.ietf.org/rfc/rfc3550.txt">RFC 3550</a>) protocol stacks.
 * <br>
 * RtspSrvr supports three kinds of transport: rtsp with rtp over udp, 
 * rtsp with rtp over tcp and
 * <a href="http://developer.apple.com/quicktime/icefloe/dispatch028.html">rtsp over http</a>. 
 * <br>
 * RtspSrvr supports three kinds of media: video, audio and meta data(mdata).
 * For each media type, we list its supported encodings:
 * <ul>
 *   <li>video</li>
 *     <ul>
 *       <li>h264(<a href="http://www.ietf.org/rfc/rfc3984.txt">RFC3984</a>)</li>
 *       <li>mpeg4(<a href="http://www.ietf.org/rfc/rfc3016.txt">RFC3016</a>)</li>
 *       <li>jpeg(<a href="http://www.ietf.org/rfc/rfc2435.txt">RFC2435</a>)</li>
 *     </ul>
 *   <li>audio</li>
 *     <ul>
 *       <li>aac4(<a href="http://www.ietf.org/rfc/rfc3640.txt">RFC3640</a>)</li>
 *       <li>amr-nb(<a href="http://www.ietf.org/rfc/rfc3267.txt">RFC3267</a>)</li>
 *       <li>g711 u-law and a-law(<a href="http://www.ietf.org/rfc/rfc3551.txt">RFC3551</a>)</li>
 *       <li>g726(<a href="http://www.ietf.org/rfc/rfc3551.txt">RFC3551</a>)</li>
 *     </ul>
 *   <li>mdata</li>
 *     <ul>
 *       <li>VN defined meta data (only used with other VN programs).</li>
 *     </ul>
 * </ul>
 * \section prog_sec Programming with RtspSrvr
 * \par
 * RtspSrvr receives the bitstream wrapped in ubuffer format and then
 * stream out the bitstream to rtsp client in rtp packets. See the figure below.<br>
 * \image html "lmc_rtsps_internet.png" "RtspSrvr get bitstream from LiveMediaSrc"
 * \image latex "lmc_rtsps_internet.png" "RtspSrvr get bitstream from LiveMediaSrc" width=12cm
 * To use RtspSrvr library:
 * <ol>
 *   <li>You need to implement your own LiveMediaSrcs (you will need LiveMediaSrc.h 
 * and LiveMediaSrc_protect.h to do so).</li>
 *   <li>After creating your own implemented LiveMediaSrc Objects, set them into rtsp_server_initial_options (You will need RtspSrvr.h).</li>
 *   <li>Then call the RtspSrvr_Initial(), this will allocate all the needed resources in RtspSrvr.</li>
 *   <li>Then call the RtspSrvr_Start(), this will let rtsp server start operating.</li>
 *   <li>You can call RtspSrvr_Stop() to stop the rtsp server. Before your program exitting, you may call RtspSrvr_Release() to release all the RtspSrvr allocated resources.</li>
 * </ol>
 * \section prog_arch_sec RtspSrvr architecture
 * \par
 * RtspSrvr is multi-threading inside, there is one "RtspSrvr main thread" to take care all the rtsp protocol part duties and connection management.<br>
 * For each LiveMediaSrc set from user, there is one "Rtp streamer thead" being created. Its main duty is to get ubuffer from this LiveMediaSrc object, and then stream out the bitstream to the rtsp client side by rtp packets.<br>
 * See the figure below:
 *
 * \image html "RtspSrvr_arch.png" "RtspSrvr architecture"
 * \image latex "RtspSrvr_arch.png" "RtspSrvr architecture" width=12cm
 */

#ifndef __RTSP_SERVER_H__
#define __RTSP_SERVER_H__

//#include "common.h"
#include "typedef.h"
#include "global_codec.h"

#include "LiveMediaSrc.h"

#define RTSP_SERVER_VERSION		MAKEFOURCC(8, 0, 0, 2)

#if 0 
SCODE RtspSrvr_Initial(HANDLE *phRtspSrvrObj, TRtspSrvrInitOpts *ptOpts);
SCODE RtspSrvr_Release(HANDLE *phRtspSrvrObj);
SCODE RtspSrvr_Start(HANDLE hRtspSrvrObj);
SCODE RtspSrvr_Stop(HANDLE hRtspSrvrObj);
#endif

/*! 
 *********************************************************************
 * The access file related information.
 * e.g. If you access the streaming by url rtsp://172.17.255.19/live1.sdp,
 * then the access name is live1.sdp.
 *********************************************************************
*/
typedef struct access_file_information
{
	//! The access file name.
	CHAR	*szAccessName;
	//! Which video track is related to this access file,
	//! set to -1 to disable video track.
	int		iVTrackNo;
	//! Which audio track is related to this access file,
	//! set to -1 to disable audio track.
	int		iATrackNo;
	//! Which mdata track is related to this aceess file,	
	//! set to -1 to disable mdata track.
	int		iDTrackNo;

    //! The multicast enable/disable
    BOOL    bMltcstEnable;
	//! The multicast ip address of this access file,
	CHAR	*szMltcstIP;
	//! The Time To Live value of the multicast,
	//! this value is meanless when multicast is diabled.
	BYTE	byMltcstTTL;
	//! The multicast rtp port number of video track,
	//! this value is meanless when multicast is diabled or video is disabled.
	USHORT	usMltcstVideoPort;
	//! The multicast rtp port number of audio track,
	//! this value is meanless when multicast is diabled or audio is disabled.
	USHORT	usMltcstAudioPort;
	//! The multicast rtp port number of mdata track,
	//! this value is meanless when multicast is diabled or mdata is disabled.
	USHORT	usMltcstMDataPort;
} TAcsFileInfo;
typedef TAcsFileInfo*	PTAcsFileInfo;

/*! 
 *********************************************************************
 * The RtspSrvr initial options.
 ********************************************************************* 
*/
typedef struct rtsp_server_initial_options
{
	//! The ip address of the machine,
	//! set to NULL to let RtspSrvr decide by itselt.
	const char		*szSrvIPAddr;
	//! The rtsp protocol listening port number.
	USHORT			usSrvListenPort;
	//! The starting port number of rtp over udp.
	USHORT			usRtpStartPort;
	USHORT			usBlockSize;
	DWORD			httpserver_type;
	BOOL			bIPv6;
	//! The authentication mode, set to "none" or "basic", "digest" isn't supported now.
	//! \details See RFC 2617 HTTP Authentication: Basic and Digest Access Authentication.
	const char		*szAuthMode;
	//! The fdipc path with VN's boa http server,
	//! set to NULL to disable rtsp over http or when you don't use VN's modified boa http server.
	//! \note You must use VN's modified boa http server to enable rtsp over http.
	//! \details About rtps over http: <a href="http://developer.apple.com/quicktime/icefloe/dispatch028.html">Tunnelling RTSP and RTP through HTTP.</a>
	const char		*szRtspOverHttpFdipcPath;
	//! Array of video LiveMediaSrc handles, see LiveMediaSrc.h.
	//! The posion in this array, started from 0, is the video track number in TAcsFileInfo.
	HANDLE			*ahLiveVSrc;
	//! Numbers of video source handle in the array above.
	int				iLiveVSrcNum;
	//! Array of audio LiveMediaSrc handles, see LiveMediaSrc.h.
	//! The posion in this array, started from 0, is the audio track number in TAcsFileInfo.
	HANDLE			*ahLiveASrc;
	//! Numbers of audio source handle in the array above.
	int				iLiveASrcNum;
	//! Array of mdata LiveMediaSrc handles, see LiveMediaSrc.h.
	//! The posion in this array, started from 0, is the mdata track number in TAcsFileInfo.
	HANDLE			*ahLiveDSrc;
	//! Numbers of mdata source handle in the array above.
	int				iLiveDSrcNum;
	//! Array of pointers to TAcsFileInfo.
	//! \Note szAccessName in TAcsFileInfo should be unique among all TAcsFileInfos.		
	PTAcsFileInfo	*aptAcsFileInfo;
	//! Numbers of access file info pointer in the array above.
	int				iAcsFileInfoNum;
#ifdef _QOS_
	//! This rate control flag for QoS
	ERateCtrlFlags  eRateCtrlFlag;
#endif
} TRtspSrvrInitOpts;

/*! 
 *********************************************************************
 *
 * \brief
 * Create a RtspSrvr object.
 *
 * \param[out] phRtspSrvrObj
 * Pointer to the handle of RtspSrvr object.
 *
 * \param[in] ptOpts
 * Pointer to a RtspSrvr initial Options.
 *
 * \retval S_OK
 * Create the RtspSrvr object ok.
 * \retval S_FAIL
 * Create the RtspSrvr object failed.
 *
 * \remarks
 * After this object being initiated successfully, the *phRtspSrvrObj will be set to a RtspSrvr object.
 * This function should be called before calling any other RtspSrvr functions.
 *
 * \see 
 * RtspSrvr_Release.
 *
 ******************************************************************** 
*/
SCODE RtspSrvr_Initial(HANDLE *phRtspSrvrObj, TRtspSrvrInitOpts *ptOpts);

/*! 
 *********************************************************************
 *
 * \brief
 * Release the RtspSrvr object.
 *
 * \param[in,out] phRtspSrvrObj
 * Pointer to the handle of RtspSrvr object.
 *
 * \retval S_OK
 * Release the RtspSrvr object ok.
 * \retval S_FAIL
 * Release the RtspSrvr object failed.
 *
 * \remarks
 * After this object being released, the *phRtspSrvrObj will be set to NULL.
 *
 * \see 
 * RtspSrvr_Initial.
 *
 ******************************************************************** 
*/
SCODE RtspSrvr_Release(HANDLE *phRtspSrvrObj);

/*! 
 *********************************************************************
 *
 * \brief
 * Start the RtspSrvr object.
 *
 * \param[in] hRtspSrvrObj
 * The handle of RtspSrvr object.
 *
 * \retval S_OK
 * Start the RtspSrvr object ok.
 * \retval S_FAIL
 * Start the RtspSrvr object failed.
 *
 * \remarks
 * You should call the API to let rtsp server start to operate.
 *
 * \see 
 * RtspSrvr_Stop.
 *
 ******************************************************************** 
*/
SCODE RtspSrvr_Start(HANDLE hRtspSrvrObj);

/*! 
 *********************************************************************
 *
 * \brief
 * Stop the RtspSrvr object.
 *
 * \param[in] hRtspSrvrObj
 * The handle of RtspSrvr object.
 *
 * \retval S_OK
 * Stop the RtspSrvr object ok.
 * \retval S_FAIL
 * Stop the RtspSrvr object failed.
 *
 * \remarks
 * You should call the API to let rtsp server stop operating.
 *
 * \see 
 * RtspSrvr_Start.
 *
 ******************************************************************** 
*/
SCODE RtspSrvr_Stop(HANDLE hRtspSrvrObj);

// advanced usage, don't use it if you don't know how to use it
// This function can be called only after the RtspSrvr_Initial and before RtspSrvr_Start
typedef struct rtspsrvr_acsfile_rtpport_map
{
	USHORT	usRtpPortStartAt;
	USHORT	usRtpPortLessThan;
} TRtspSrvrAcsFileRtpPortMap;

SCODE RtspSrvr_SetAcsFileRtpPortMap(HANDLE hRtspSrvrObj, TRtspSrvrAcsFileRtpPortMap *ptMap, DWORD dwMapNum);

// advanced usage, don't use it if you don't know how to use it
// This function can be called only after the RtspSrvr_Initial and before RtspSrvr_Start
SCODE RtspSrvr_SetupDestination(HANDLE hRtspSrvrObj, BOOL bSupport);
#endif // __RTSP_SERVER_H__
