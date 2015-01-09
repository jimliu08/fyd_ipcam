/*
 * $Header: /rd_2/project/SoC/Linux_Libraries/G711Enc/G711Enc_Lib/G711Enc.h 17    13/01/15 3:55p Aphon.wu $
 *
 * Copyright 2009 VN, Inc. All rights reserved.
 *
 * Description:
 *
 * $History: G711Enc.h $
 * 
 * *****************  Version 17  *****************
 * User: Aphon.wu     Date: 13/01/15   Time: 3:55p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 16  *****************
 * User: Angel        Date: 12/08/29   Time: 3:43p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 15  *****************
 * User: Yc.lu        Date: 10/01/27   Time: 5:28p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Enc/G711Enc_Lib
 * Update to Version 1.0.0.9.
 * 
 * *****************  Version 14  *****************
 * User: Angel        Date: 09/12/11   Time: 6:08p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 13  *****************
 * User: Yc.lu        Date: 09/12/11   Time: 10:02a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Enc/G711Enc_Lib
 * Update to 1.0.0.7
 * 
 * *****************  Version 12  *****************
 * User: Angel        Date: 09/02/27   Time: 10:38a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 11  *****************
 * User: Angel        Date: 09/01/09   Time: 10:18a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 10  *****************
 * User: Angel        Date: 09/01/09   Time: 9:47a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 9  *****************
 * User: Angel        Date: 08/02/15   Time: 3:40p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 8  *****************
 * User: Angel        Date: 08/02/13   Time: 11:45a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 7  *****************
 * User: Angel        Date: 08/02/13   Time: 11:35a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 6  *****************
 * User: Angel        Date: 07/12/28   Time: 8:21p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 4  *****************
 * User: Angel        Date: 06/11/02   Time: 8:03p
 * Updated in $/rd_2/speech/G711/ARM/Encoder/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 3  *****************
 * User: Angel        Date: 06/10/31   Time: 5:02p
 * Updated in $/rd_2/speech/G711/ARM/Encoder/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 2  *****************
 * User: Angel        Date: 06/10/26   Time: 10:29a
 * Updated in $/rd_2/speech/G711/ARM/Encoder/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 1  *****************
 * User: Angel        Date: 06/10/25   Time: 4:28p
 * Created in $/rd_2/speech/G711/ARM/Encoder/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 5  *****************
 * User: Angel        Date: 06/10/24   Time: 11:02p
 * Updated in $/rd_2/speech/G711/C/Encoder/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 4  *****************
 * User: Angel        Date: 06/10/18   Time: 1:16p
 * Updated in $/rd_2/speech/G711/C/Encoder/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 3  *****************
 * User: Angel        Date: 06/10/16   Time: 7:48p
 * Updated in $/rd_2/speech/G711/C/Encoder/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 2  *****************
 * User: Angel        Date: 06/10/16   Time: 2:27p
 * Updated in $/rd_2/speech/G711/C/Encoder/G711Enc/G711Enc_Lib
 * 
 * *****************  Version 1  *****************
 * User: Angel        Date: 06/10/14   Time: 2:50p
 * Created in $/rd_2/speech/G711/C/Encoder/G711Enc/G711Enc_Lib
 *
 */

/* ============================================================================================== */
#ifndef __G711ENC_H__
#define __G711ENC_H__

/* ============================================================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "errordef.h"
#include "global_codec.h"

/* ============================================================================================== */
#define G711ENC_VERSION MAKEFOURCC(1, 0, 0, 11)
#define G711ENC_ID_VERSION "1.0.0.10"

/*Version 1.0.0.4 modification,Angel Hu,2008.02.13*/
/*Version 1.0.0.8 modification, 2009.12.11 */
//#define __USE_BIG_ENDIAN__
/* ======================================= */
/**************************************************/
/* ==============================================================================================*/
typedef enum g711_enc_mode
{
	G711ENC_MODE_A_LAW = 0,
	G711ENC_MODE_U_LAW = 1,
}EG711EncMode;  

/* ============================================================================================== */
typedef struct g711_enc_init_options
{
	DWORD dwVersion;
	DWORD dwInFrameSize;

	EG711EncMode eEncMode;

	void *pObjectMem;
} TG711EncInitOptions;

typedef struct g711_enc_state
{

	SWORD *pswInFrame;
	BYTE *pbyOutFrame;
} TG711EncState;

/* ===============================================================================================*/
SCODE G711Enc_Initial(HANDLE *phObject, TG711EncInitOptions *ptInitOptions);
SCODE G711Enc_Release(HANDLE *phObject);

SCODE G711Enc_ProcessOneFrame(HANDLE hObject, TG711EncState *ptState);

SCODE G711Enc_CheckParam(HANDLE hObject, TG711EncState *ptState);
DWORD G711Enc_QueryMemSize(TG711EncInitOptions *ptInitOptions);

SCODE G711Enc_GetVersionInfo(BYTE *pbyMajor, BYTE *pbyMinor, BYTE *pbyBuild, BYTE *pbyRevision);

/* ===============================================================================================*/
#endif //__G711ENC_H__
