#ifndef _EPTZ_H_
#define _EPTZ_H_

#include "typedef.h"
#include "errordef.h"
#include "global_codec.h"
#include "video_error.h"

#define EPTZ_VERSION MAKEFOURCC(3, 1, 0, 0)

typedef enum eptz_speed
{
	esSetSmooth,
	esSetInstant
} EEptzSpeed;

typedef enum eptz_option
{
	eoSetPTZ,	//adwUserData[0] = the address of p value, adwUserData[1] = the address of t value, adwUserData[2] = the address of z value,
	eoGetPTZ,	//adwUserData[0] = the address of p value, adwUserData[1] = the address of t value, adwUserData[2] = the address of z value,
	eoSetSpeed,	//adwUserData[0] = esSetSmooth, adwUserData[1] = the address of speed value
				//adwUserData[0] = esSetInstant,adwUserData[1] = useless
} EEptzOption;

typedef struct eptz_options
{
	EEptzOption	eEptzType;
	DWORD		adwUserData[3];
	
} TEptzOpts;

typedef struct eptz_init_opts
{
	DWORD				dwVersion;
	FLOAT				fP, fT, fZ;
	EPixelFormatFlags	ePixelFormat;	//input format
	EAlignType			eAddrAlignType;	//for output component address alignment. 
										//When you set it as ALIGN_TYPE_8_BYTE, pbyCloseUpViewCmpt1 will be aligned by 8 bytes;
										//pbyCloseUpViewCmpt2 will be aligned by 8 bytes, if ePixelFormat is NV12;
										//pbyCloseUpViewCmpt2 will be aligned by 4 bytes, if ePixelFormat is YUV420.
										//When you set it as ALIGN_TYPE_1_BYTE, all output components will NOT be aligned;
	EAlignType			eSizeAlignType;	//for output width and height alignment. 
										//When you set it as ALIGN_TYPE_2_BYTE, dwCloseUpViewWidth and dwCloseUpViewHeight will be aligned by 2 bytes and 
} TEptzInitOpts;

typedef struct eptz_state
{
	DWORD				dwVideoBufStride;

	//input	
	DWORD				dwFullViewWidth;
	DWORD				dwFullViewHeight;
	BYTE				*pbyFullViewCmpt1;
	BYTE				*pbyFullViewCmpt2;
	BYTE				*pbyFullViewCmpt3;
	
	//output
	DWORD				dwCloseUpViewWidth;
	DWORD				dwCloseUpViewHeight;
	BYTE				*pbyCloseUpViewCmpt1;
	BYTE				*pbyCloseUpViewCmpt2;
	BYTE				*pbyCloseUpViewCmpt3;

} TEptzState;

SCODE eptz_init(HANDLE *phEptz, TEptzInitOpts *ptInitOpts);
SCODE eptz_rls(HANDLE *phEptz);

SCODE eptz_oneframe(HANDLE hEptz, TEptzState *ptState);
SCODE eptz_options(HANDLE hEptz, TEptzOpts *ptOpts);
#endif // _EPTZ_H_
