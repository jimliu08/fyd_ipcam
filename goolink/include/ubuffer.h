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
 * ubuffer.h
 *
 * \brief
 * UBuffer, Universal Buffer for short, is a general purpose buffer 
 * for media data.
 * 
 * \date
 * 2005/10/11
 * 2010/12/20
 *
 * \author Cheng-Ching Huang ubuffer creator
 * \author djhow.tu@VN.com ubuffer modifier
 *
 *******************************************************************************
 */

/*! \mainpage Introduction
    \par Purpose
    Ubuffer is an universal buffer format to carry multimedia data.<br>
    Ubuffer format suits to video and audio bitstreams but doesn't confine to them.
    \par Two portions
    There're two portions in an ubuffer: \em ubuffer \em header \em data and \em ubuffer \em media \em data.
    See the diagram below.<br>
    Ubuffer header data describes ubuffer media data.<br> 
    When we call ubuffer, it means ubuffer header data plus ubuffer media data.

<pre>
    +-+-+-+-+-+-+-+-+-+-+-+
    |                     |
    | ubuffer header data |
    |                     |
    +-+-+-+-+-+-+-+-+-+-+-+
    |                     |
    |                     |
    |                     |
    | ubuffer media data  |
    |                     |
    |                     |
    |                     |
    +-+-+-+-+-+-+-+-+-+-+-+
</pre>
    \par Two types
    There're two type of ubuffers: \em ubuffer-conf \em ubuffer and \em not-ubuffer-conf \em ubuffer.<br>
    We call an \em ubuffer-conf \em ubuffer an \em ubuffer \em conf and a \em not-ubuffer-conf \em ubuffer an \em ubuffer for short.<br>
    Ubuffer conf only has ubuffer header data portion, the ubuffer media data portion of a ubuffe conf has zero bytes.<br>
    When an ubuffer genetrator produces an ubuffer stream, it must first send out an ubuffer conf to describe the following ubuffers.<br> 
    See the diagram below.

<pre>

                  +-+-+-+-+-+    +-+-+-+-+-+    +-+-+-+-+-+
                  |         |    |         |    | ubuffer |      
    ubuffer   --> | ubuffer |--> | ubuffer |--> | conf    | --> ubuffer    
    generator     |         |    |         |    +-+-+-+-+-+     receiver 
                  +-+-+-+-+-+    +-+-+-+-+-+    

</pre> 

    This manual is divided in the following sections:
    - \subpage ubufhdrdata.
    - \subpage ubufinbuf.
    - \subpage videoinubuf.
    - \subpage audioinubuf.
    - \subpage mdatainubuf.
*/

#ifndef _UBUFFER_H_
#define _UBUFFER_H_

#include <stdio.h>
#include "global_codec.h"

/*! UBuffer ID. To embed this string into your code to let others can use ident to know what ubuffer.h you include. */
#define UBUFFER_ID	"$Id: ubuffer.h 40345 2011-06-17 09:45:22Z jeff.liao $"

/*! \page ubufhdrdata Ubuffer header data
    \section components Ubuffer header data components
    There're 3 components in an ubuffer header data: \em ubuffer \em prototype, \em conf \em specific \em data and \em user \em data.<br>
    Every ubuffer (includeing ubuffer conf) begins an ubuffer prototype.<br>
    Conf specific data occurs only in ubuffer conf.<br>
    Every ubuffer may contain user data or contain no user data depending on the media type carried by it and 
    the ubuffer usage of the application programmer.

<pre>
    +-+-+-+-+-+-+-+-+-+-+-+-+
    |                       |
    |   ubuffer prototype   |
    |                       |
    |                       |
    +-+-+-+-+-+-+-+-+-+-+-+-+
    | [conf specific data]  |
    +-+-+-+-+-+-+-+-+-+-+-+-+
    |      [user data]      |
    +-+-+-+-+-+-+-+-+-+-+-+-+
</pre>
    \section ubufproto Ubuffer prototype
    Ubuffer prototype is 44 bytes long, each field is decribed in the followings: 
<pre>
      1byte   1byte   1byte   1byte
    |<----->|<----->|<----->|<----->| 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |            dwSize             |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |          dwDataType           |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |      dwFollowingDataType      |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |           dwTrackID           |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |             dwSec             |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |            dwUSec             |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |            dwSeqNo            |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |           dwIsSync            |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |      dwCompositionOffset      |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |         bIsBoundary           |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |        dwUserDataSize         |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
</pre>
    \li \c dwSize: size of ubuffer hedaer data plus ubuffer media data.
    \li \c dwDataType: type of this ubuffer in FOURCC format.
    \li \c dwFollowingDataType: type of the following ubuffer in FOURCC format.
    \li \c dwTrackID: track ID of the media data carried in this ubuffer.
    \li \c dwSec: time information of media data carried in this ubuffer, second portion since the UNIX Epoch (00:00:00 UTC, January 1, 1970).
    \li \c dwUSec: time information of media data carried in this ubuffer, micro-second portion since the UNIX Epoch (00:00:00 UTC, January 1, 1970).
    \li \c dwSeqNo: sequence number of this ubuffer.
    \li \c dwIsSync: is this ubuffer carries the Intra-frame or not.
    \li \c dwCompositionOffset: meanless, ignore this safely.
    \li \c bIsBoundary: does this ubuffer carry the end of one frame.
    \li \c dwUserDataSize: size of user data in ubuffer header data.
 */

/*! Define the UBuffer prototype here */
#define UBUFFER_PROTOTYPE	\
	DWORD	dwSize;\
	DWORD	dwDataType;\
	DWORD	dwFollowingDataType;\
	DWORD	dwTrackID;\
	DWORD	dwSec;\
	DWORD	dwUSec;\
	DWORD	dwSeqNo;\
	BOOL	bIsSync;\
	DWORD	dwCompositionOffset;\
	BOOL	bIsBoundary;\
	DWORD	dwUserDataSize

/*! \brief UBuffer prototype
 */
typedef struct u_buffer
{
    /*! see define#UBUFFER_PROTOTYPE */
	UBUFFER_PROTOTYPE;
	/* [conf specific data] */
	/* [user data] */
} TUBuffer;

/*! \page ubufhdrdata Ubuffer header data
    \section confspecdata conf specific data
    Conf specific data only occurs in ubuffer conf, and is different in different media type's ubuffer conf.
    \section usrdata User data
    User data is a composite of many user data items. See the diagram below.<br>
    You can add any user data item in ubuffer.<br>
    Some media type's ubuffer may define its own \em must \em have user data item.
<pre>
    +-+-+-+-+-+-+-+-+-+-+
    | user data item 1  |
    +-+-+-+-+-+-+-+-+-+-+
    | user data item 2  |
    +-+-+-+-+-+-+-+-+-+-+
    | user data item 3  |
    +-+-+-+-+-+-+-+-+-+-+
</pre>
    Each user data item has 4 bytes header including 2 byte tag field and 2 bytes lenght field.<br>
    See the diagram below.
<pre>
     2 bytes 2 bytes             length bytes
    |<----->|<----->|<--------------------------------->|
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |  tag  | length|       user data item content      |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
</pre>
    The User data item will be identified by tag, different user data item has different tag value.<br>
    The length in user data item header means length to the next tag, it doesn't count in the user date item's header size.<br>
    dwUserDataSize in ubuffer prototype must be larger than the sum of all the user data items in user data portion but not must be equal to it.
*/

/*! 
    \brief The tag identify mpeg-4 packet mode info.

    This type of USERDATA is using to carry the length of each mp4v packet in this ubuffer.<br>
    The struct of TAG_MP4V_PACKETINFO user data item:<br>
    First 4 byte is user data item header. And then follows 4 bytes indicating how many packets.<br>
    If there're n packets, the tail are 4*n bytes, each i-th 4 bytes represents i-th packet's size in the buffer
<pre>
       2 bytes     2 bytes           4 bytes                 4 bytes                      4 bytes         
    |<--------->|<--------->|<--------------------->|<--------------------->|    |<--------------------->|
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ -+  +-+-+-+-+-+-+-+-+-+-+-+-+
    |   0x0032  |  4+4*n    |    packet number=n    |     packet size (1)   |... |     packet size (n)   |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ -+  +-+-+-+-+-+-+-+-+-+-+-+-+
</pre>
 */
#define TAG_MP4V_PACKETINFO		0x0032

/*! 
    \brief The tag identify jpeg extension info.

    This type of USERDATA is using to carry the length of the total JPEG image of this ubuffer.<br>
    The struct of TAG_JPEG_EXTENIFO user data item:
<pre>
       2 bytes     2 bytes           4 bytes    
    |<--------->|<--------->|<--------------------->|
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   0x0071  |     4     |    jpeg frame size    |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
</pre>
 */
#define TAG_JPEG_EXTENINFO		0x0071

/*! 
    \brief The tag identify mpeg-4 extension info.

    This type of USERDATA is using to carry the  offset of the  mpeg-4  bitstream of this ubuffer.<br>
    The struct of TAG_MP4V_EXTENINFO user data item:
<pre>
       2 bytes     2 bytes           4 bytes    
    |<--------->|<--------->|<--------------------->|
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   0x0072  |     4     |    bitsteam offset    |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
</pre>
 */
#define TAG_MP4V_EXTENINFO	 	0x0072

/*! \page ubufinbuf Ubuffer in buffer
    \c case \c 1: ubuffer header data and ubuffer media data are concatenated.<br>
    See the diagram below.
<pre>
    +-+-+-+-+-+-+-+-+-+-+-+
    |                     |
    | ubuffer header data |
    |                     |
    +-+-+-+-+-+-+-+-+-+-+-+
    |                     |
    |                     |
    |                     |
    | ubuffer media data  |
    |                     |
    |                     |
    |                     |
    +-+-+-+-+-+-+-+-+-+-+-+
</pre>
    \c case \c 2: ubuffer header data and ubuffer media data are not concatenated.<br>
    See the diagram below.
<pre>
    +-+-+-+-+-+-+-+-+-+-+-+          +-+-+-+-+-+-+-+-+-+-+-+
    |                     |          |                     |
    | ubuffer header data |          |                     |
    |                     |          |                     |
    +-+-+-+-+-+-+-+-+-+-+-+          | ubuffer media data  |
                                     |                     |
                                     |                     |
                                     |                     |
                                     +-+-+-+-+-+-+-+-+-+-+-+
</pre>
    Either in case1 or case2 doesn't violate the ubuffer definition.<br>
    Using case1 or case2 is the choice of application program.<br>
    In case 2, It's the application program's responsibility to maintain the relationship between these two related but separated buffers.
*/

/*! \page videoinubuf Video in ubuffer
    \section mp4vinubuf Mp4v in ubuffer
    \par Ubuffer conf for mp4v
	Format of ubuffer conf for mp4v is described in the diagram below.<br>
    Each header field is decribed in the followings:
    \li \c dwSize: size of this ubuffer conf.
    \li \c dwDataType: must be FOURCC_CONF.
    \li \c dwFollowingDataType: must be FOURCC_MP4V.
    \li \c dwTrackID: track id of this mp4v stream.
    \li \c dwSec,\c dwUSEc,\c dwSeqNo,\c dwIsSync,\c dwCompositionOffset,\c bIsBoundary: meanless here.
    \li \c dwUserDataSize: user data size in this ubuffer conf.
    \li \c dwWidth: width of the video.
    \li \c dwHeight: height of the video.
    \li \c dwProfileLevel: profile level of the mp4v video.
    \li \c mp4v \c specific \c conf: variable length binary mp4v specific conf data.
<pre>
      1byte   1byte   1byte   1byte
    |<----->|<----->|<----->|<----->| 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                               |      
    |                               |      
    |      ubuffer prototype        |      
    |                               |      
    |                               |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |            dwWidth            |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |            dwHeight           |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |         dwProfileLevel        |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |      mp4v specific conf       |      
    |      (variable length)        |      
    |      ...                      |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |          [user data]          |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
</pre>
    \par Ubuffer to carry mp4v bitstream
    Each header field is decribed in the followings:
    \li \c dwSize: size of this ubuffer.
    \li \c dwDataType: must be FOURCC_MP4V.
    \li \c dwFollowingDataType: meanless here.
    \li \c dwTrackID: track id of this mp4v stream.
    \li \c dwSec: second portion of timestamp.
    \li \c dwUSEc: micro second portion of timestamp.
    \li \c dwSeqNo: sequence number of this ubuffer.
    \li \c dwIsSync: if carrying I-frame set to TRUE, otherwise set to FALSE.
    \li \c dwCompositionOffset: meanless here.
    \li \c bIsBoundary: if carrying last portion of one mp4v frame set to TRUE, otherwise set to FALSE.
    \li \c dwUserDataSize: user data size in this ubuffer conf.
    \par
    Simply put mp4v bitstream in ubuffer media data.<br>
    If this ubuffer is carring mp4v I-frame, set bIsSync to TRUE, otherwise set to FALSE.<br>
    One mp4v frame can be carried in multiple ubuffers;<br>
    If this ubuffer isn't the last ubuffer of one mp4v bitstream frame, bIsBoundary must set to FALSE, otherwise must set to TRUE.<br>
    One ubuffer can't carry multiple mp4v frames.
 */

/*! \brief UBuffer of Configuration for MP4V
    \see u_buffer

    UBuffer configuration for MP4V. 
    The following is the decoder specific config 
 */
typedef struct u_buffer_conf_mp4v
{
    /*! see define#UBUFFER_PROTOTYPE */
	UBUFFER_PROTOTYPE;
	/* specific */
	/*! The width of this video */
	DWORD	dwWidth;
	/*! The height of this video */
	DWORD	dwHeight;
	/*! Profile level defined in ISO/IEC 14496-2 */
	DWORD	dwProfileLevel;
	/* Decoder specific info (dwSize - sizeof(TUBufferConfMP4V) bytes long) */
}TUBufferConfMP4V;

/*! \page videoinubuf Video in ubuffer
    \section h264inubuf H264 in ubuffer
    \par Ubuffer conf for h264
	Format of ubuffer conf for h264 is described in the diagram below.<br>
    Each header field is decribed in the followings:
    \li \c dwSize: size of this ubuffer conf.
    \li \c dwDataType: must be FOURCC_CONF.
    \li \c dwFollowingDataType: must be FOURCC_H264.
    \li \c dwTrackID: track id of this h264 stream.
    \li \c dwSec,\c dwUSEc,\c dwSeqNo,\c dwIsSync,\c dwCompositionOffset,\c bIsBoundary: meanless here.
    \li \c dwUserDataSize: user data size in this ubuffer conf.
    \li \c dwWidth: width of the video.
    \li \c dwHeight: height of the video.
    \li \c abyProfileLevel: 3 bytes long and defined in RFC 3984.
    \li \c SPS \c and \c PPS \c special \c format: variable length in special format to carry SPS and PPS, see the format definition in the following paragraph.
<pre>
      1byte   1byte   1byte   1byte
    |<----->|<----->|<----->|<----->| 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                               |      
    |                               |      
    |      ubuffer prototype        |      
    |                               |      
    |                               |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |            dwWidth            |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |            dwHeight           |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   abyProfileLevelID   |       |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   SPS and PPS spcial format   |      
    |      (variable length)        |      
    |      ...                      |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |          [user data]          |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
</pre>
    \par SPS and PPS special format in ubuffer conf for h264
    SPS stands for sequence parameter set, PPS stands for picture parameter set.<br>
    It's a 2 bytes length field followed by one SPS or PPS NAL.<br>
    Ubuffer conf for h264 \em must contains at least one SPS and at least one PPS.<br>
    There's no limitation of how may SPS/PPSs can be carried here;<br>
    Unless the physic buffer isn't big enough, this should be handled by programmer.<br>
    The value in the length field means how many bytes of the following SPS or PPS.<br>
    The length field is host byte order (in ARM, it is little endian).

<pre>
       2 bytes             length(i) bytes             2 bytes 
    |<--------->|<--------------------------------->|<--------->|
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    | length(i) |        NAL of SPS or PPS          |length(i+1)| .........
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
</pre>
    \par Ubuffer to carry h264 bitstream
    Each header field is decribed in the followings:
    \li \c dwSize: size of this ubuffer.
    \li \c dwDataType: must be FOURCC_H264.
    \li \c dwFollowingDataType: meanless here.
    \li \c dwTrackID: track id of this h264 stream.
    \li \c dwSec: second portion of timestamp.
    \li \c dwUSEc: micro second portion of timestamp.
    \li \c dwSeqNo: sequence number of this ubuffer.
    \li \c dwIsSync: if carrying IDR frame set to TRUE, otherwise set to FALSE.
    \li \c dwCompositionOffset: meanless here.
    \li \c bIsBoundary: if carrying last portion of one h264 frame set to TRUE, otherwise set to FALSE.
    \li \c dwUserDataSize: user data size in this ubuffer conf.
    \par
    Store multiple NALs in ubuffer media data portion.<br>
    One NAL can't be segmented into multiple ubuffers.<br>
    One ubuffer can only carry one frame's NALs.<br>
    If this ubuffer is carrying the end of one frame's NAL, set bIsBoundary to TRUE, otherwise set to FALSE.<br>
    If this ubuffer is carring IDR frame, set bIsSync to TRUE, otherwise set to FALSE.<br>
    The format is 4 bytes NAL length field (host byte order), indicating the following NAL length, followed by one NAL and another 4 bytes NAL length field followed by another NAL, and so on.<br>

<pre>
       4 bytes             length(i) bytes             4 bytes 
    |<--------->|<--------------------------------->|<--------->|
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    | length(i) |              NAL(i)               |length(i+1)| .........
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
</pre>
 */

/*! \brief UBuffer of Configuration for H.264
    \see u_buffer
	\see isom_sample264.c

  UBuffer configuration for H.264. The following payload of this buffer is structured as 
  a series of the length of NAL unit, sequence parameter set (SPS) or picture parameter set (PPS), 
  and the NAL unit itself. The length of the fields that describe the length of NAL unit is 2 byte
  and is in host byte order (little-endian). 
 <pre> 
  sizeof(TUBufferConfH264) bytes    2 bytes   m bytes, m < 65536
|<------------------------------>|<-------->|<------------------>|
+--------------------------------+----------+--------------------+-------+-----------
|    TUBufferConfH264            |Length (m)|    SPS or PPS      |Length |................
+--------------------------------+----------+--------------------+-------+-----------
 </pre>
 
	\note 
	The payload of H.264 UBuffer, which means that u_buffer::dwDataType is FOURCC_H264, 
	is formated as a series of the length of NAL unit, and the NAL unit itself. 
	The length of the fields that describe the length of NAL unit is 4 byte
	and is in host byte order (little-endian). 
 <pre> 
     sizeof(TUBuffer) bytes          4 bytes     n bytes, n < 65536
|<---------------------------->|<------------->|<------------------>|
+------------------------------+---------------+--------------------+-------+-----------
|           TUBuffer           |   Length (n)  |        NAL         |Length |................
+------------------------------+---------------+--------------------+-------+-----------
 </pre>
 */
typedef struct u_buffer_conf_h264
{
    /*! see define#UBUFFER_PROTOTYPE */
	UBUFFER_PROTOTYPE;
	/* specific */
	/*! The width of this video */
	DWORD	dwWidth;
	/*! The height of this video */
	DWORD	dwHeight;
	/*! Profile level defined in RFC 3984 */
	BYTE	abyProfileLevelId[3];
	/*! Fill this dummy byte for 4-byte alignment */
	BYTE	abyDummy[1];
	/*! Enable SVCT*/
	BOOL bSVCTEn;
	/*! Layer number of SVCT*/
	DWORD dwSVCTLayerNum;
 	/*! Split SPS and PPS into different RTP packets*/
    BOOL bSplitSPSandPPS;
}TUBufferConfH264;

/*! \page videoinubuf Video in ubuffer
    \section jpeginubuf Jpeg in ubuffer
    \par Ubuffer conf for jpeg
	Format of ubuffer conf for jpeg is described in the diagram below.<br>
    Each header field is decribed in the followings:
    \li \c dwSize: size of this ubuffer conf.
    \li \c dwDataType: must be FOURCC_CONF.
    \li \c dwFollowingDataType: must be FOURCC_JPEG.
    \li \c dwTrackID: track id of this jpeg stream.
    \li \c dwSec,\c dwUSEc,\c dwSeqNo,\c dwIsSync,\c dwCompositionOffset,\c bIsBoundary: meanless here.
    \li \c dwUserDataSize: user data size in this ubuffer conf.
    \li \c dwWidth: width of the video.
    \li \c dwHeight: height of the video.
    \li \c dwHeaderLen: Indicates the number of bytes to skip from the beginning of the stream to find the first pixel of the image.
    \li \c dwXDensity: Specify the horizontal pixel aspect ratio.
    \li \c dwYDensity: Specify the vertical pixel aspect ratio.
    \li \c dwNumComponents: Indicates whether the image has Y component only or is Y, Cr, Cb. It shall be equal to 1 or 3.

<pre>
      1byte   1byte   1byte   1byte
    |<----->|<----->|<----->|<----->| 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                               |      
    |                               |      
    |      ubuffer prototype        |      
    |                               |      
    |                               |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |            dwWidth            |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |            dwHeight           |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |         dwHeaderLen           |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |          dwXDensity           |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |          dwYDensity           |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |        dwNumComponents        |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |          [user data]          |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
</pre>
    \par Ubuffer to carry jpeg bitstream
    Each header field is decribed in the followings:
    \li \c dwSize: size of this ubuffer.
    \li \c dwDataType: must be FOURCC_JPEG.
    \li \c dwFollowingDataType: meanless here.
    \li \c dwTrackID: track id of this jpeg stream.
    \li \c dwSec: second portion of timestamp.
    \li \c dwUSEc: micro second portion of timestamp.
    \li \c dwSeqNo: sequence number of this ubuffer.
    \li \c dwIsSync: always set to TRUE.
    \li \c dwCompositionOffset: meanless here.
    \li \c bIsBoundary: if carrying last portion of one jpeg frame set to TRUE, otherwise set to FALSE.
    \li \c dwUserDataSize: user data size in this ubuffer conf.
    \par
    Simply put jpeg bitstream in ubuffer media data.<br>
    Always set bIsSync to TRUE.<br>
    One jpeg frame can be carried in multiple ubuffers.<br>
    One ubuffer can't carry multiple jpeg frames.<br>
    If this ubuffer isn't the last ubuffer of one jpeg bitstream frame, bIsBoundary must set to FALSE, otherwise must set to TRUE.<br>
    Each ubuffer must contain one user data item as following, to indicate the total jpeg frame size of the frame carried in this ubuffer.

<pre>
       2 bytes     2 bytes           4 bytes    
    |<--------->|<--------->|<--------------------->|
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   0x0071  |     4     |    jpeg frame size    |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
</pre>
 */

/*! \brief UBuffer of Configuration for JPEG
    \see u_buffer

  UBuffer configuration for JPEG. */
typedef struct u_buffer_conf_jpeg
{
    /*! see define#UBUFFER_PROTOTYPE */
	UBUFFER_PROTOTYPE;
	/* specific */
	/*! width of this image */
	DWORD	dwWidth;
	/*! height of this image */
	DWORD	dwHeight;
	/*! Indicates the number of bytes to skip from the beginning of the 
	  stream to find the first pixel of the image. */
	DWORD	dwHeaderLen;
	/*! Specify the pixel aspect ratio */
	DWORD	dwXDensity;
	/*! Specify the pixel aspect ratio */
	DWORD	dwYDensity;
	/*! Indicates whether the image has Y component only or is Y, Cr, Cb. 
	  It shall be equal to 1 or 3. */
	DWORD	dwNumComponents;
}TUBufferConfJPEG;

/*! \brief UBuffer of Configuration for H263
    \see u_buffer

  UBuffer configuration for H263. */
typedef struct u_buffer_conf_h263
{
    /*! see define#UBUFFER_PROTOTYPE */
	UBUFFER_PROTOTYPE;
	/* specific */
	/*! The width of this video */
	DWORD	dwWidth;
	/*! The height of this video */
	DWORD	dwHeight;
	/*! H.263 level defined in ITU-T Recommendation H.263 (annex X): "Annex X, Profiles and levels definition" */
	DWORD	dwH263Level;
	/*! H.263 profile defined in ITU-T Recommendation H.263 (annex X): "Annex X, Profiles and levels definition" */
	DWORD	dwH263Profile;
}TUBufferConfH263;

/*! \page audioinubuf Audio in ubuffer
    \section aac4inubuf AAC4 in ubuffer
    \par Ubuffer conf for AAC4
	Format of ubuffer conf for aac4 is described in the diagram below.<br>
    Each header field is decribed in the followings:
    \li \c dwSize: size of this ubuffer conf.
    \li \c dwDataType: must be FOURCC_CONF.
    \li \c dwFollowingDataType: must be FOURCC_AAC4.
    \li \c dwTrackID: track id of this aac4 stream.
    \li \c dwSec,\c dwUSEc,\c dwSeqNo,\c dwIsSync,\c dwCompositionOffset,\c bIsBoundary: meanless here.
    \li \c dwUserDataSize: user data size in this ubuffer conf.
    \li \c dwChannelNumber: channel number of the audio.
    \li \c dwSampleRate: sample rate of the audio.
    \li \c dwProfileLevel: Profile level defined in ISO/IEC 14496-3.
    \li \c dwXDensity: Specify the horizontal pixel aspect ratio.
    \li \c dwYDensity: Specify the vertical pixel aspect ratio.
    \li \c aac4 \c specific \c conf: variable length binary aac4 specific conf.

<pre>
      1byte   1byte   1byte   1byte
    |<----->|<----->|<----->|<----->| 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                               |      
    |                               |      
    |      ubuffer prototype        |      
    |                               |      
    |                               |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |        dwChannelNumber        |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |         dwSampleRate          |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |         dwProfileLevel        |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |       aac4 specific conf      |      
    |       (variable length)       |
    |       ...                     |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |          [user data]          |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
</pre>
    \par Ubuffer to carry AAC4 bitstream
    Each header field is decribed in the followings:
    \li \c dwSize: size of this ubuffer.
    \li \c dwDataType: must be FOURCC_AAC4.
    \li \c dwFollowingDataType: meanless here.
    \li \c dwTrackID: track id of this aac4 stream.
    \li \c dwSec: second portion of timestamp.
    \li \c dwUSEc: micro second portion of timestamp.
    \li \c dwSeqNo: sequence number of this ubuffer.
    \li \c dwIsSync: always set to TRUE.
    \li \c dwCompositionOffset: meanless here.
    \li \c bIsBoundary: always set to TRUE.
    \li \c dwUserDataSize: user data size in this ubuffer conf.
    \par
    Simply put AAC4 bitstream in ubuffer media data.<br>
    One ubuffer carry exactly one AAC4 frame.<br>
*/

/*! \brief UBuffer of Configuration for AAC4
    \see u_buffer

 UBuffer configuration for AAC4. The following is the decoder specific config */
typedef struct u_buffer_conf_aac4
{
    /*! see define#UBUFFER_PROTOTYPE */
	UBUFFER_PROTOTYPE;
	/* specific */
	/*! Channel number of this audio */
	DWORD	dwChannelNumber;	
	/*! Sample rate for this audio. */
	DWORD	dwSampleRate;	
	/*! Profile level defined in ISO/IEC 14496-3 */
	DWORD	dwProfileLevel;
	/* Decoder specific info (dwSize - sizeof(TUBufferConfAAC4) bytes long) */
}TUBufferConfAAC4;

/*! \page audioinubuf Audio in ubuffer
    \section gamrinubuf GAMR in ubuffer
    \par Ubuffer conf for GAMR
	Format of ubuffer conf for gamr is described in the diagram below.<br>
    Each header field is decribed in the followings:
    \li \c dwSize: size of this ubuffer conf.
    \li \c dwDataType: must be FOURCC_CONF.
    \li \c dwFollowingDataType: must be FOURCC_GAMR.
    \li \c dwTrackID: track id of this gamr stream.
    \li \c dwSec,\c dwUSEc,\c dwSeqNo,\c dwIsSync,\c dwCompositionOffset,\c bIsBoundary: meanless here.
    \li \c dwUserDataSize: user data size in this ubuffer conf.
    \li \c dwSampleRate: sample rate of the audio.
    \li \c szVendor: 4 character code of the manufacturer of the codec, It can be safely ignored.
    \li \c byDecVersion: Version of the vendor's decoder which can decode the encoded stream in the best way. The value is set to 0 if decoder version has no importance for the vendor. It can be safely ignored.
    \li \c wModeSet: The active codec modes, a value of 0x81FF means all modes.
    \li \c byModeChangePeriod: If no restriction is applied, set to 0.
    \li \c byFramePerSample: How many gamr frames in the following ubuffers.

<pre>
      1byte   1byte   1byte   1byte
    |<----->|<----->|<----->|<----->| 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                               |      
    |                               |      
    |      ubuffer prototype        |      
    |                               |      
    |                               |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |         dwSampleRate          |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |          szVendor[4]          |      dv: byDecVersion (1 byte)    
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   dv  |       |       ms      |      ms: wModeSet (2 bytes)
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   mp  |   fs  |               |      mp: byModeChangePeriod (1 byte)    
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |          [user data]          |      fs: byFramePerSample (1 byte)    
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
</pre>
    \par Ubuffer to carry GAMR bitstream
    Each header field is decribed in the followings:
    \li \c dwSize: size of this ubuffer.
    \li \c dwDataType: must be FOURCC_GAMR.
    \li \c dwFollowingDataType: meanless here.
    \li \c dwTrackID: track id of this gamr stream.
    \li \c dwSec: second portion of timestamp.
    \li \c dwUSEc: micro second portion of timestamp.
    \li \c dwSeqNo: sequence number of this ubuffer.
    \li \c dwIsSync: always set to TRUE.
    \li \c dwCompositionOffset: meanless here.
    \li \c bIsBoundary: always set to TRUE.
    \li \c dwUserDataSize: user data size in this ubuffer conf.
    \par
    GAMR bitstream in ubuffer media data follows the storage format in RFC 3267.<br>
    One ubuffer can carries multiple speech frames, but one speech frame can not straddle multiple ubuffers.<br>
    The timestamp of ubuffer is the timestamp of the first speech frame in this ubuffer's media data.<br>
*/

/*! \brief UBuffer of Configuration for GAMR
    \see u_buffer

 UBuffer configuration for GAMR. 

	\note 
	The format of the payload of GAMR UBuffer, which means that u_buffer::dwDataType is FOURCC_GAMR, 
	must follow the storage format in RFC 3267.
 
 */
typedef struct u_buffer_conf_gamr
{
    /*! see define#UBUFFER_PROTOTYPE */
	UBUFFER_PROTOTYPE;
	/* specific */
	DWORD	dwSampleRate;	
	/*! 4 character code of the manufacturer of the codec. 
	  It can be safely ignored. */
	char	szVendor[4];
	/*! Version of the vendor's decoder which can decode the encoded stream 
	  in the best way. The value is set to 0 if decoder version has no 
	  importance for the vendor. It can be safely ignored. */
	BYTE	byDecVersion;
	/*! The active codec modes. A value of 0x81FF means all modes. */
	WORD	wModeSet;
	/*! If no restriction is applied, set to 0. */
	BYTE	byModeChangePeriod;
	/*! The number of frames to be considered as one sample inside 
	  the 3GP file. */
	BYTE	byFramesPerSample;
}TUBufferConfGAMR;

/*! \page audioinubuf Audio in ubuffer
    \section g711inubuf G711 in ubuffer
    \par Ubuffer conf for G711
	Format of ubuffer conf for g711 is described in the diagram below.<br>
    Each header field is decribed in the followings:
    \li \c dwSize: size of this ubuffer conf.
    \li \c dwDataType: must be FOURCC_CONF.
    \li \c dwFollowingDataType: must be FOURCC_G711.
    \li \c dwTrackID: track id of this g711 stream.
    \li \c dwSec,\c dwUSEc,\c dwSeqNo,\c dwIsSync,\c dwCompositionOffset,\c bIsBoundary: meanless here.
    \li \c dwUserDataSize: user data size in this ubuffer conf.
    \li \c dwCompressionLaw: FOURCC_ALAW for g711 a-law or FOURCC_ULAW for g711 u-law.
    \li \c dwChannelNumber: channel number of the audio.

<pre>
      1byte   1byte   1byte   1byte
    |<----->|<----->|<----->|<----->| 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                               |      
    |                               |      
    |      ubuffer prototype        |      
    |                               |      
    |                               |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |       dwCompressionLaw        |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |        dwChannelNumber        |    
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |          [user data]          | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
</pre>

    \par Ubuffer to carry G711 bitstream
    Each header field is decribed in the followings:
    \li \c dwSize: size of this ubuffer.
    \li \c dwDataType: must be FOURCC_G711.
    \li \c dwFollowingDataType: meanless here.
    \li \c dwTrackID: track id of this g711 stream.
    \li \c dwSec: second portion of timestamp.
    \li \c dwUSEc: micro second portion of timestamp.
    \li \c dwSeqNo: sequence number of this ubuffer.
    \li \c dwIsSync: always set to TRUE.
    \li \c dwCompositionOffset: meanless here.
    \li \c bIsBoundary: always set to TRUE.
    \li \c dwUserDataSize: user data size in this ubuffer conf.
    \par
    Simply put g711 bitsteam in ubuffer media data portion.<br>
    One ubuffer can carries multiple speech frames, but one speech frame can not straddle multiple ubuffers.<br>
    The timestamp of ubuffer is the timestamp of the first speech frame in this ubuffer's media data.<br>
*/

/*! \brief UBuffer of Configuration for G711
    \see u_buffer

  UBuffer configuration for G711. */
typedef struct u_buffer_conf_g711
{
    /*! see define#UBUFFER_PROTOTYPE */
	UBUFFER_PROTOTYPE;
	/*! FOURCC_ALAW or FOURCC_ULAW */
	DWORD dwCompressionLaw;
	/*! Channel number of this audio */
	DWORD dwChannelNumber;
}TUBufferConfG711;

/*! \page audioinubuf Audio in ubuffer
    \section g726inubuf G726 in ubuffer
    \par Ubuffer conf for G726
    Format of ubuffer conf for g726 is described in the diagram below.<br>
    Each header field is decribed in the followings:
    \li \c dwSize: size of this ubuffer conf.
    \li \c dwDataType: must be FOURCC_CONF.
    \li \c dwFollowingDataType: must be FOURCC_G726.
    \li \c dwTrackID: track id of this g726 stream.
    \li \c dwSec,\c dwUSEc,\c dwSeqNo,\c dwIsSync,\c dwCompositionOffset,\c bIsBoundary: meanless here.
    \li \c dwUserDataSize: user data size in this ubuffer conf.
    \li \c dwCodewordBits: Bit size of a sample, be one of 2-bits, 3-bits, 4-bits or 5-bits.
    \li \c dwChannelNumber: channel number of the audio.

<pre>
      1byte   1byte   1byte   1byte
    |<----->|<----->|<----->|<----->| 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                               |      
    |                               |      
    |      ubuffer prototype        |      
    |                               |      
    |                               |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |        dwCodewordBits         |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |        dwChannelNumber        |    
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |          [user data]          | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
</pre>

    \par Ubuffer to carry G726 bitstream
    Each header field is decribed in the followings:
    \li \c dwSize: size of this ubuffer.
    \li \c dwDataType: must be FOURCC_G726.
    \li \c dwFollowingDataType: meanless here.
    \li \c dwTrackID: track id of this g726 stream.
    \li \c dwSec: second portion of timestamp.
    \li \c dwUSEc: micro second portion of timestamp.
    \li \c dwSeqNo: sequence number of this ubuffer.
    \li \c dwIsSync: always set to TRUE.
    \li \c dwCompositionOffset: meanless here.
    \li \c bIsBoundary: always set to TRUE.
    \li \c dwUserDataSize: user data size in this ubuffer conf.
    \par
    Simply put g726 bitsteam in ubuffer media data portion.<br>
    One ubuffer can carries multiple speech frames, but one speech frame can not straddle multiple ubuffers.<br>
    The timestamp of ubuffer is the timestamp of the first speech frame in this ubuffer's media data.<br>
*/

/*! \brief UBuffer of Configuration for G726
    \see u_buffer

  UBuffer configuration for G726. */
typedef struct u_buffer_conf_g726
{
    /*! see define#UBUFFER_PROTOTYPE */
	UBUFFER_PROTOTYPE;
	/*! Bit size of a sample, be one of 2-bits, 3-bits, 4-bits or 5-bits. */
	DWORD dwCodewordBits;
	/*! Channel number of this audio */
	DWORD dwChannelNumber;
}TUBufferConfG726;

/*! \brief UBuffer of Configuration for G722
    \see u_buffer

  UBuffer configuration for G722. */
typedef struct u_buffer_conf_g722
{
    /*! see define#UBUFFER_PROTOTYPE */
	UBUFFER_PROTOTYPE;
	/*! Decoding mode 1~3 */
	DWORD dwMode;
	/*! Channel number of this audio */
	DWORD	dwChannelNumber;
}TUBufferConfG722;

/*! \brief UBuffer of Configuration for G729
    \see u_buffer

  UBuffer configuration for G729. */
typedef struct u_buffer_conf_g729
{
    /*! see define#UBUFFER_PROTOTYPE */
	UBUFFER_PROTOTYPE;
	/*! FOURCC_729A or FOURCC_729B */
	DWORD dwAnnex;
	/*! Silence compression 0~5 */
	DWORD dwDTX;
	/*! Channel number of this audio */
	DWORD	dwChannelNumber;
}TUBufferConfG729;

/*! \page mdatainubuf Meta data in ubuffer
    \section mdatainubuf Meta data in ubuffer
    \par Ubuffer conf for Meta data
    Format of ubuffer conf for meta data is described in the diagram below.<br>
    Each header field is decribed in the followings:
    \li \c dwSize: size of this ubuffer conf.
    \li \c dwDataType: must be FOURCC_CONF.
    \li \c dwFollowingDataType: must be FOURCC_MDAT.
    \li \c dwTrackID: track id of this meta data stream.
    \li \c dwSec,\c dwUSEc,\c dwSeqNo,\c dwIsSync,\c dwCompositionOffset,\c bIsBoundary: meanless here.
    \li \c dwUserDataSize: user data size in this ubuffer conf.
    \li \c dwSampleRate: sample rate of the meta data.

<pre>
      1byte   1byte   1byte   1byte
    |<----->|<----->|<----->|<----->| 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                               |      
    |                               |      
    |      ubuffer prototype        |      
    |                               |      
    |                               |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |         dwSampleRate          |      
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |          [user data]          | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
</pre>

    \par Ubuffer to carry Meta data
    Each header field is decribed in the followings:
    \li \c dwSize: size of this ubuffer.
    \li \c dwDataType: must be FOURCC_MDAT.
    \li \c dwFollowingDataType: meanless here.
    \li \c dwTrackID: track id of this meta data stream.
    \li \c dwSec: second portion of timestamp.
    \li \c dwUSEc: micro second portion of timestamp.
    \li \c dwSeqNo: sequence number of this ubuffer.
    \li \c dwIsSync: defined by application.
    \li \c dwCompositionOffset: meanless here.
    \li \c bIsBoundary: defined by application.
    \li \c dwUserDataSize: user data size in this ubuffer conf.
    \par
    Meta data is specified by application, programmer define his/her own meta data format carried in ubuffer media data portion.
 */

/*! \brief UBuffer of Configuration for Meta data
    \see u_buffer

	UBuffer configuration for meta data */ 	 
typedef struct u_buffer_conf_mdat 	 
{ 	 
    /*! see define#UBUFFER_PROTOTYPE */
	UBUFFER_PROTOTYPE;
	/*! sample rate of this meta data stream */
	DWORD   dwSampleRate; 	 
}TUBufferConfMDAT;
	 

#define OFFSETOF(s,m) (size_t)&(((s *)0)->m)

/*! \brief Get the size from an ubuffer

	Read the u_buffer::dwSize from an arbitray address without alignment
  */
static inline DWORD UBUFFER_SIZE(BYTE *ptr) 
{
	union {DWORD dw; BYTE aby[sizeof(DWORD)];} u;
	size_t	offset = OFFSETOF(TUBuffer, dwSize);
	u.aby[0] = ptr[offset]; u.aby[1] = ptr[offset+1];
	u.aby[2] = ptr[offset+2]; u.aby[3] = ptr[offset+3];
	/*@-usedef@*/
	return u.dw;
	/*@=usedef@*/
}

/*! \brief Get the track-id from an ubuffer

	Read the u_buffer::dwTrackID from an arbitray address without alignment
  */
static inline DWORD UBUFFER_TRACKID(BYTE *ptr) 
{
	union {DWORD dw; BYTE aby[sizeof(DWORD)];} u;
	size_t	offset = OFFSETOF(TUBuffer, dwTrackID);
	u.aby[0] = ptr[offset]; u.aby[1] = ptr[offset+1];
	u.aby[2] = ptr[offset+2]; u.aby[3] = ptr[offset+3];
	/*@-usedef@*/
	return u.dw;
	/*@=usedef@*/
}

/*! \brief Get the data-type from an ubuffer

	Read the u_buffer::dwDataType from an arbitray address without alignment
  */
static inline DWORD UBUFFER_DATATYPE(BYTE *ptr) 
{
	union {DWORD dw; BYTE aby[sizeof(DWORD)];} u;
	size_t	offset = OFFSETOF(TUBuffer, dwDataType);
	u.aby[0] = ptr[offset]; u.aby[1] = ptr[offset+1];
	u.aby[2] = ptr[offset+2]; u.aby[3] = ptr[offset+3];
	/*@-usedef@*/
	return u.dw;
	/*@=usedef@*/
}

/*! \brief Get the data-type string from an ubuffer

	Read the u_buffer::dwDataType in a zero-terminated string from an arbitray address without alignment
  */
static inline char * UBUFFER_DATATYPESTR(BYTE *ptr) 
{
	static char szType[5];
	size_t	offset = OFFSETOF(TUBuffer, dwDataType);

	szType[0] = ptr[offset];     szType[1] = ptr[offset + 1]; 
	szType[2] = ptr[offset + 2]; szType[3] = ptr[offset + 3]; 
	return szType;
}

/*! \brief Verify if it is a valid u_buffer

  This inline function will verify the u_buffer is valid or not. 
  The method is to verify that both dwDataType and dwFollowingDataType
  MUST be printable characters (i.e. 31 < c < 127), 
  and the dwUSec must smaller than 1,000,000. 
  */
static inline BOOL UBUFFER_IS_VALID(BYTE *ptr) 
{
	union {DWORD dw; BYTE aby[sizeof(DWORD)];} u;
	size_t	offset = OFFSETOF(TUBuffer, dwUSec);
	int	i;
	BYTE	*pTmp;

	/* Comparing dwUSec is simpler and stronger to reject. */
	u.aby[0] = ptr[offset]; u.aby[1] = ptr[offset+1];
	u.aby[2] = ptr[offset+2]; u.aby[3] = ptr[offset+3];
	/*@-usedef@*/
	if (u.dw >= 1000000)	return FALSE;
	/*@=usedef@*/

	pTmp = ptr + OFFSETOF(TUBuffer, dwDataType);
	for (i = 3; i >= 0; i --) {
		if(*pTmp < 32)	return FALSE;
		if(*pTmp++ > 126)	return FALSE;
	}
	pTmp = ptr + OFFSETOF(TUBuffer, dwFollowingDataType);
	for (i = 3; i >= 0; i --) {
		if(*pTmp < 32)	return FALSE;
		if(*pTmp++ > 126)	return FALSE;
	}

	return TRUE;
}

#endif //_UBUFFER_H_

