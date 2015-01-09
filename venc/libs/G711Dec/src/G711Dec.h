/*
 * $Header: /rd_2/project/SoC/Linux_Libraries/G711Dec/G711Dec_Lib/G711Dec.h 11    13/01/15 1:52p Aphon.wu $
 *
 * Copyright 2009 VN, Inc. All rights reserved.
 *
 * Description:
 *
 * $History: G711Dec.h $
 * 
 * *****************  Version 11  *****************
 * User: Aphon.wu     Date: 13/01/15   Time: 1:52p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Dec/G711Dec_Lib
 * 
 * *****************  Version 10  *****************
 * User: Angel        Date: 12/08/30   Time: 5:53p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Dec/G711Dec_Lib
 * 
 * *****************  Version 9  *****************
 * User: Yc.lu        Date: 10/01/27   Time: 5:05p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Dec/G711Dec_Lib
 * Update to ver. 1.0.0.9.
 * 
 * *****************  Version 8  *****************
 * User: Yc.lu        Date: 09/12/11   Time: 9:49a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Dec/G711Dec_Lib
 * Update to 1.0.0.8
 * 
 * *****************  Version 7  *****************
 * User: Angel        Date: 09/02/27   Time: 11:07a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Dec/G711Dec_Lib
 * 
 * *****************  Version 6  *****************
 * User: Angel        Date: 09/01/09   Time: 10:25a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Dec/G711Dec_Lib
 * 
 * *****************  Version 5  *****************
 * User: Angel        Date: 08/02/15   Time: 3:58p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Dec/G711Dec_Lib
 * 
 * *****************  Version 4  *****************
 * User: Angel        Date: 08/02/13   Time: 2:11p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Dec/G711Dec_Lib
 * 
 * *****************  Version 3  *****************
 * User: Angel        Date: 08/01/03   Time: 9:22p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Dec/G711Dec_Lib
 * 
 * *****************  Version 2  *****************
 * User: Angel        Date: 06/12/27   Time: 6:05p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Dec/G711Dec_Lib
 * 
 * *****************  Version 1  *****************
 * User: Angel        Date: 06/11/03   Time: 3:57p
 * Created in $/rd_2/project/SoC/Linux_Libraries/G711Dec/G711Dec_Lib
 * 
 * *****************  Version 3  *****************
 * User: Angel        Date: 06/11/02   Time: 8:06p
 * Updated in $/rd_2/speech/G711/ARM/Decoder/G711Dec/G711Dec_Lib
 * 
 * *****************  Version 2  *****************
 * User: Angel        Date: 06/10/31   Time: 5:19p
 * Updated in $/rd_2/speech/G711/ARM/Decoder/G711Dec/G711Dec_Lib
 * 
 * *****************  Version 1  *****************
 * User: Angel        Date: 06/10/26   Time: 11:28a
 * Created in $/rd_2/speech/G711/ARM/Decoder/G711Dec/G711Dec_Lib
 * 
 */
 /* =========================================================================================== */
#ifndef __G711DEC_H__
#define __G711DEC_H__

/* ============================================================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "errordef.h"
#include "global_codec.h"

/* ============================================================================================== */
#define G711DEC_VERSION MAKEFOURCC(1, 0, 0, 11)
/* Version 1.0.0.7 modification, 2009.01.08 */
#define G711DEC_ID_VERSION "1.0.0.10"
/* ======================================== */
/*Version 1.0.0.5 modification, Angel Hu, 2008.02.13*/
//#define __USE_BIG_ENDIAN__
/****************************************************/
/* ==============================================================================================*/
typedef enum g711_dec_mode
{
	G711DEC_MODE_A_LAW = 0,
	G711DEC_MODE_U_LAW = 1,
//	G711ENC_MODE_A2U_LAW = 2
}EG711DecMode;

/* ============================================================================================== */
typedef struct g711_dec_init_options
{
	DWORD dwVersion;
	DWORD dwInFrameSize;

	EG711DecMode eDecMode;

	void *pObjectMem;
} TG711DecInitOptions;

typedef struct g711_dec_state
{

	BYTE *pbyInFrame;
	SWORD *pswOutFrame;
} TG711DecState;

/* ===============================================================================================*/
SCODE DLLAPI G711Dec_Initial(HANDLE *phObject, TG711DecInitOptions *ptInitOptions);
SCODE DLLAPI G711Dec_Release(HANDLE *phObject);

SCODE DLLAPI G711Dec_ProcessOneFrame(HANDLE hObject, TG711DecState *ptState);

SCODE DLLAPI G711Dec_CheckParam(HANDLE hObject, TG711DecState *ptState);
DWORD DLLAPI G711Dec_QueryMemSize(TG711DecInitOptions *ptInitOptions);

SCODE DLLAPI G711Dec_GetVersionInfo(BYTE *pbyMajor, BYTE *pbyMinor, BYTE *pbyBuild, BYTE *pbyRevision);

/* ===============================================================================================*/
#endif //__G711DEC_H__
