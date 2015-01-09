/*
 * $Header: /rd_2/project/Mozart/Linux_Libraries/MassMemAccess/MassMemAccess_Lib/MassMemAccess.h 53    13/01/16 11:50a Yiming.liu $
 *
 * Copyright 2009 VN, Inc.  All rights reserved.
 *
 * Description:
 *
 * $History: MassMemAccess.h $
 * 
 * *****************  Version 53  *****************
 * User: Yiming.liu   Date: 13/01/16   Time: 11:50a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * 
 * *****************  Version 52  *****************
 * User: Yiming.liu   Date: 12/10/08   Time: 5:34p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * 
 * *****************  Version 51  *****************
 * User: Yiming.liu   Date: 12/09/25   Time: 11:23a
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * MODIFICATION: Remove cache operations - DONE.
 * 
 * *****************  Version 50  *****************
 * User: Yiming.liu   Date: 12/04/24   Time: 5:06p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * 
 * *****************  Version 49  *****************
 * User: Sean         Date: 12/02/15   Time: 1:31p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update the version number from 9.0.0.2 to 9.0.0.3
 * 
 * *****************  Version 47  *****************
 * User: Sean         Date: 11/07/15   Time: 7:16p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update the version number from 9.0.0.1 to 9.0.0.2
 * 
 * *****************  Version 46  *****************
 * User: Sean         Date: 11/06/10   Time: 11:50p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update the version number from 9.0.0.0 to 9.0.0.1 - DONE.
 * 
 * *****************  Version 45  *****************
 * User: Sean         Date: 11/06/10   Time: 3:23p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update the version number from 8.0.0.1 to 9.0.0.0 - DONE.
 * FEATURE: Add bUsePhysAddr in TMassMemAccessState - DONE.
 * FEATURE: Replace TVideoSignalOptions with  TMassMemAccessOptions -
 * DONE.
 * 
 * *****************  Version 44  *****************
 * User: Alan         Date: 10/12/02   Time: 5:46p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update the version number from 8.0.0.0 to 8.0.0.1 - DONE.
 *
 * *****************  Version 43  *****************
 * User: Alan         Date: 10/09/10   Time: 3:25p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * BUG: The privacy mask could not support width larger than 2048 - FIXED.
 *
 * *****************  Version 41  *****************
 * User: Lawrence.hu  Date: 09/10/15   Time: 1:34a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update the version number to 7.1.0.1 - DONE.
 *
 * *****************  Version 40  *****************
 * User: Lawrence.hu  Date: 09/10/15   Time: 1:29a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * MODIFICATION: Modify the constraints in MassMemAccess_CheckParam() -
 * DONE.
 *
 * *****************  Version 39  *****************
 * User: Lawrence.hu  Date: 09/10/13   Time: 4:04p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update the version number to 7.1.0.0 - DONE.
 *
 * *****************  Version 38  *****************
 * User: Lawrence.hu  Date: 09/10/13   Time: 11:25a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update versin number - DONE.
 *
 * *****************  Version 37  *****************
 * User: Lawrence.hu  Date: 09/10/13   Time: 10:44a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * MODIFICATION: Add MassMemAccess_CheckParam() in MassMemAccess_Options.c
 * - DONE.
 *
 * *****************  Version 36  *****************
 * User: Lawrence.hu  Date: 09/06/12   Time: 9:54p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update version number from 6.3.0.0 to 7.0.0.0 - DONE.
 *
 * *****************  Version 35  *****************
 * User: Lawrence.hu  Date: 09/06/12   Time: 10:35a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * FEATURE: Add dwMaxBurstLength in TMassMemAccessState - DONE.
 *
 * *****************  Version 34  *****************
 * User: Lawrence.hu  Date: 09/06/10   Time: 10:43a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update the version number - DONE.
 *
 * *****************  Version 33  *****************
 * User: Lawrence.hu  Date: 09/06/10   Time: 10:41a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update the version number - DONE.
 *
 * *****************  Version 32  *****************
 * User: Lawrence.hu  Date: 09/06/10   Time: 10:34a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * FEATURE: Add MassMemAccess_StartOneFrame() and
 * MassMemAccess_WaitOneFrameComplete() - DONE.
 *
 * *****************  Version 31  *****************
 * User: Lawrence.hu  Date: 09/05/25   Time: 6:53p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update the version number - DONE.
 *
 * *****************  Version 30  *****************
 * User: Lawrence.hu  Date: 09/05/23   Time: 4:26p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update the version number - DONE.
 *
 * *****************  Version 29  *****************
 * User: Lawrence.hu  Date: 09/04/09   Time: 1:39p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * MODIFICATION: Rename SAMPLE_ID to MASSMEMACCESS_ID in
 * MassMemAccess_Setup.c - DONE.
 *
 * *****************  Version 28  *****************
 * User: Lawrence.hu  Date: 09/04/01   Time: 10:10p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * BUG: Rename bl2DEnable and blFillingEnable to b2DEnable and
 * bFillingEnable - DONE.
 * BUG: Move EMassMemAccessMode from MassMemAccess.h to
 * MassMemAccess_Locals.h - DONE.
 *
 * *****************  Version 27  *****************
 * User: Lawrence.hu  Date: 09/03/31   Time: 10:06a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update MassMemAccess version number - DONE.
 *
 * *****************  Version 26  *****************
 * User: Lawrence.hu  Date: 09/01/13   Time: 1:18p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * MODIFICATION: Add ID string declaration - DONE.
 *
 * *****************  Version 25  *****************
 * User: Lawrence.hu  Date: 09/01/12   Time: 9:56p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * FEATURE: Synchronize with VPL_DMAC device driver version 5.0.0.0 on
 *  Haydn SoC.
 * FEATURE: Synchronize with VPL_DMAC device driver version 7.1.0.0 on
 *  Mozart SoC.
 * MODIFICATION: Remove dwBurstNum from TMassMemAccessState - DONE.
 * MODIFICATION: Remove eMassMemAccessMode from TMassMemAccessState -
 * DONE.TMassMemAccessState - DONE.
 *
 * *****************  Version 22  *****************
 * User: Lawrence.hu  Date: 09/01/10   Time: 4:42p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update the version number to 6.1.0.0 - DONE.
 *
 * *****************  Version 21  *****************
 * User: Lawrence.hu  Date: 08/11/07   Time: 8:40p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * MODIFICATION: Modify the annotation - DONE.
 *
 * *****************  Version 20  *****************
 * User: Lawrence.hu  Date: 08/10/27   Time: 12:22p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * FEATURE: Add burst_num to set burst number in DMAC - DONE.
 *
 * *****************  Version 19  *****************
 * User: Lawrence.hu  Date: 08/07/29   Time: 12:31p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * MODIFICATION: Add SrcStride, DstWidth, DstHeight and DstStride MMR to
 * support 2D DMA - DONE.
 * MODIFICATION: Add FillingConstant to support Constant_Filling mode -
 * DONE.
 * MODIFICATION: Add MaskAddr MMR to support Privacy_Mask mode - DONE.
 *
 * *****************  Version 17  *****************
 * User: Sor          Date: 08/01/28   Time: 11:46p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update version number from 4.0.0.4 to 4.0.0.5 - DONE.
 *
 * *****************  Version 16  *****************
 * User: Sor          Date: 08/01/17   Time: 1:02a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update version number from 4.0.0.3 to 4.0.0.4 - DONE.
 *
 * *****************  Version 15  *****************
 * User: Sor          Date: 08/01/13   Time: 7:56p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update version number from 4.0.0.2 to 4.0.0.3 - DONE.
 *
 * *****************  Version 14  *****************
 * User: Sor          Date: 08/01/13   Time: 12:47a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update version number from 4.0.0.1 to 4.0.0.2 - DONE.
 *
 * *****************  Version 13  *****************
 * User: Alan         Date: 07/12/26   Time: 6:53p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 *
 * *****************  Version 12  *****************
 * User: Sor          Date: 07/12/23   Time: 8:38p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * Modify dwDstBusNum to dwDstBusNum and pbyDstAddr to pbyDstAddr -
 * DONE.
 *
 * *****************  Version 11  *****************
 * User: Alan         Date: 07/12/11   Time: 11:37p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 *
 * *****************  Version 10  *****************
 * User: Alan         Date: 07/11/13   Time: 6:15p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 *
 * *****************  Version 9  *****************
 * User: Sor          Date: 06/07/27   Time: 4:56p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update version number from 3.0.0.4 to 3.0.0.5 - DONE.
 *
 * *****************  Version 8  *****************
 * User: Sor          Date: 06/07/11   Time: 6:46p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update version number from 3.0.0.3 to 3.0.0.4 - DONE.
 *
 * *****************  Version 7  *****************
 * User: Sor          Date: 06/06/05   Time: 5:24p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update version number from 3.0.0.2 to 3.0.0.3 - DONE.
 *
 * *****************  Version 6  *****************
 * User: Sor          Date: 06/06/02   Time: 5:53p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update version number from 3.0.0.1 to 3.0.0.2 - DONE.
 *
 * *****************  Version 5  *****************
 * User: Sor          Date: 06/04/27   Time: 11:46a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update version number from 3.0.0.0 to 3.0.0.1 - DONE.
 *
 * *****************  Version 4  *****************
 * User: Sor          Date: 06/04/24   Time: 6:31p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * UPDATE: Update version number from 2.0.0.0 to 3.0.0.0 - DONE.
 *
 * *****************  Version 3  *****************
 * User: Sor          Date: 06/04/24   Time: 3:42p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * FEATURE: Synchronize with MemMgr_Lib version 3.0.0.0 - DONE.
 *
 * *****************  Version 2  *****************
 * User: Sor          Date: 05/10/13   Time: 4:28p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * FEATURE: Increase cached and noncached memory offset setting from 1 to
 * 3 to support multiple AHBs - DONE.
 *
 * *****************  Version 1  *****************
 * User: Sor          Date: 05/09/27   Time: 9:39a
 * Created in $/rd_2/project/SoC/Linux_Libraries/MassMemAccess/MassMemAccess_Lib
 * Mass Memory Access Linux Library Version 1.0.0.0.
 *
 */

/* ============================================================================================= */
#ifndef __MASSMEMACCESS_H__
#define __MASSMEMACCESS_H__

/* ============================================================================================= */
#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "errordef.h"
#include "global_codec.h"

//#define __ASM_ARCH_PLATFORM_MOZART_H__
/* ============================================================================================= */
#ifdef __ASM_ARCH_PLATFORM_MOZART_H__
/* Version 6.3.0.0 modification, 2009.06.12 */
#define MAX_BURST_LENGTH   256
/* ======================================== */
#endif //__ASM_ARCH_PLATFORM_MOZART_H__

/* ============================================================================================= */
#define MASSMEMACCESS_VERSION MAKEFOURCC(9, 1, 0, 1)
#define MASSMEMACCESS_ID_VERSION "9.1.0.1"

/* ============================================================================================= */
typedef struct mass_mem_access_init_options
{
	DWORD dwVersion;
/* Version 8.0.0.0 modification, 2010.09.09 */
#ifdef __ASM_ARCH_PLATFORM_MOZART_H__
	DWORD dwMaxFrameWidth;
	DWORD dwMaxFrameHeight;
	DWORD dwMaskBusNum;
	BOOL bMaskInit;
#endif //__ASM_ARCH_PLATFORM_MOZART_H__
/* ======================================== */
	void *pObjectMem;
} TMassMemAccessInitOptions;

/* ============================================================================================= */
typedef struct mass_mem_access_state
{
#ifdef __ASM_ARCH_PLATFORM_MOZART_H__
    BOOL b2DEnable;
    BOOL bFillingEnable;
/* Version 9.0.0.0 modification, 2011.06.08 */
	BOOL bUsePhysAddr;	
/* ======================================== */
#endif //__ASM_ARCH_PLATFORM_MOZART_H__
	DWORD dwSrcBusNum;
	DWORD dwDstBusNum;
	DWORD dwTransSize;
#ifdef __ASM_ARCH_PLATFORM_MOZART_H__
	DWORD dwSrcStride;
	DWORD dwDstWidth;
	DWORD dwDstHeight;
	DWORD dwDstStride;
	DWORD dwFillingConstant;
/* Version 6.3.0.0 modification, 2009.06.12 */
	DWORD dwMaxBurstLength;
/* ======================================== */
/* Version 8.0.0.0 modification, 2010.09.09 */
	BOOL bMaskEnable;
/* ======================================== */
#endif //__ASM_ARCH_PLATFORM_MOZART_H__
	BYTE *pbySrcAddr;
/* Version 4.0.0.0 modification, 2007.12.23 */
	BYTE *pbyDstAddr;
/* ======================================== */
} TMassMemAccessState;

/* Version 9.0.0.0 modification, 2011.06.10 */
/* ============================================================================================= */
typedef struct mass_mem_access_options
{
    EVideoSignalOptionFlags eOptionFlags;
    DWORD adwUserData[4];
} TMassMemAccessOptions;
/* ======================================== */

/* ============================================================================================= */
SCODE MassMemAccess_Initial(HANDLE *phObject, TMassMemAccessInitOptions *ptInitOptions);
SCODE MassMemAccess_Release(HANDLE *phObject);

SCODE MassMemAccess_ProcessOneFrame(HANDLE hObject, TMassMemAccessState *ptState);
SCODE MassMemAccess_StartOneFrame(HANDLE hObject, TMassMemAccessState *ptState);
SCODE MassMemAccess_WaitOneFrameComplete(HANDLE hObject, TMassMemAccessState *ptState);

/* Version 8.0.0.0 modification, 2010.09.09 */
#ifdef __ASM_ARCH_PLATFORM_MOZART_H__
/* Version 9.0.0.0 modification, 2011.06.10 */
SCODE MassMemAccess_SetOptions(HANDLE hObject, TMassMemAccessOptions *ptOptions);
/* ======================================== */
#endif //__ASM_ARCH_PLATFORM_MOZART_H__
/* ======================================== */
/* Version 7.1.0.0 modification, 2009.10.13 */
SCODE MassMemAccess_CheckParam(HANDLE hObject, TMassMemAccessState *ptState);
/* ======================================== */

DWORD MassMemAccess_QueryMemSize(TMassMemAccessInitOptions *ptInitOptions);

SCODE MassMemAccess_GetVersionInfo(BYTE *pbyMajor, BYTE *pbyMinor, BYTE *pbyBuild, BYTE *pbyRevision);

/* ============================================================================================= */
#endif //__MASSMEMACCESS_H__

/* ============================================================================================= */
