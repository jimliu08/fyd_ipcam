/*
 * $Header: /rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib/MemMgr.h 35    13/01/14 10:11 Jon.lin $
 *
 * Copyright 2009 VN, Inc.  All rights reserved.
 *
 * Description:
 *  
 *
 * $History: MemMgr.h $
 * 
 * *****************  Version 35  *****************
 * User: Jon.lin      Date: 13/01/14   Time: 10:11
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * revert ro 4.x.x.x, we only change revision number for ERROR CODE
 * redefine.
 * 
 * *****************  Version 34  *****************
 * User: Jon.lin      Date: 13/01/11   Time: 17:37
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/Sample/MemMgr_Lib
 * [5.0.0.0]Redefine Error Codes
 * 
 * *****************  Version 33  *****************
 * User: Vincent      Date: 12/02/04   Time: 9:12a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * BUG: Wrong index of cache copy in MemMgr_FreeMemory. - FIXED.
 * UPDATE: Update version number from 4.0.0.10 to 4.0.0.11 - DONE.
 * 
 * *****************  Version 32  *****************
 * User: Jon.lin      Date: 11/01/07   Time: 7:06p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * 
 * *****************  Version 31  *****************
 * User: Jon.lin      Date: 11/01/05   Time: 8:41p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * 
 * *****************  Version 30  *****************
 * User: Jon.lin      Date: 11/01/05   Time: 6:10p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * 
 * *****************  Version 29  *****************
 * User: Jon.lin      Date: 10/12/08   Time: 9:56a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * Version 4.0.0.7
 * 
 * *****************  Version 28  *****************
 * User: Ycchang      Date: 09/02/27   Time: 10:25a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * 
 * *****************  Version 27  *****************
 * User: Alan         Date: 09/01/08   Time: 4:32p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * MODIFICATION: Add ID string declaration - DONE.
 * UPDATE: Update version number from 4.0.0.5 to 4.0.0.6 - DONE.
 * 
 * *****************  Version 26  *****************
 * User: Sor          Date: 08/01/22   Time: 10:35p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * UPDATE: Update version number from 4.0.0.4 to 4.0.0.5 - DONE.
 * 
 * *****************  Version 25  *****************
 * User: Sor          Date: 08/01/18   Time: 8:43p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * UPDATE: Update version number from 4.0.0.3 to 4.0.0.4 - DONE.
 * 
 * *****************  Version 24  *****************
 * User: Sor          Date: 07/01/17   Time: 3:54p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * UPDATE: Update version number from 4.0.0.2 to 4.0.0.3 - DONE.
 * 
 * *****************  Version 23  *****************
 * User: Sor          Date: 06/12/15   Time: 9:57a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * UPDATE: Update version number from 4.0.0.1 to 4.0.0.2 - DONE.
 * 
 * *****************  Version 22  *****************
 * User: Sor          Date: 06/12/11   Time: 6:22p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * 
 * *****************  Version 21  *****************
 * User: Sor          Date: 06/12/11   Time: 6:21p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * UPDATE: Update version number from 4.0.0.0 to 4.0.0.1 - DONE.
 * 
 * *****************  Version 20  *****************
 * User: Sor          Date: 06/07/10   Time: 5:35p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * UPDATE: Update version number from 3.3.0.1 to 4.0.0.0 - DONE.
 * 
 * *****************  Version 19  *****************
 * User: Sor          Date: 06/07/10   Time: 5:34p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * FEATURE: Remove eMemType field from TMemMgrState - DONE.
 * 
 * *****************  Version 18  *****************
 * User: Sor          Date: 06/06/27   Time: 8:09p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * UPDATE: Update version number from 3.3.0.0 to 3.3.0.1 - DONE.
 * 
 * *****************  Version 17  *****************
 * User: Sor          Date: 06/06/02   Time: 11:43a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * UPDATE: Update version number from 3.2.0.0 to 3.3.0.0 - DONE.
 * 
 * *****************  Version 16  *****************
 * User: Sor          Date: 06/06/02   Time: 11:24a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * FEATURE: Add MemMgr_RetrieveMapTbl() to support Linux multi-thread
 * applications - DONE.
 * 
 * *****************  Version 15  *****************
 * User: Sor          Date: 06/05/26   Time: 6:13p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * 
 * *****************  Version 14  *****************
 * User: Sor          Date: 06/05/11   Time: 6:05p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * UPDATE: Update version number from 3.1.0.0 to 3.2.0.0 - DONE.
 * 
 * *****************  Version 13  *****************
 * User: Sor          Date: 06/05/11   Time: 5:59p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * FEATURE: Add MemMgr_MapPhysAddr() - DONE.
 * 
 * *****************  Version 12  *****************
 * User: Sor          Date: 06/04/26   Time: 3:03p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * 
 * *****************  Version 11  *****************
 * User: Sor          Date: 06/04/25   Time: 8:17p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * UPDATE: Update version number from 3.0.0.2 to 3.1.0.0 - DONE.
 * 
 * *****************  Version 10  *****************
 * User: Sor          Date: 06/04/25   Time: 8:17p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * FEATURE: Add MemMgr_FreeMemory() - DONE.
 * 
 * *****************  Version 9  *****************
 * User: Sor          Date: 06/04/25   Time: 6:29p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * FEATURE: Add MemMgr_GetVirtAddr() - DONE.
 * 
 * *****************  Version 8  *****************
 * User: Sor          Date: 06/04/24   Time: 11:42a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * UPDATE: Update version number from 3.0.0.1 to 3.0.0.2 - DONE.
 * 
 * *****************  Version 7  *****************
 * User: Sor          Date: 06/04/20   Time: 12:20p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * UPDATE: Update version number from 3.0.0.0 to 3.0.0.1 - DONE.
 * 
 * *****************  Version 5  *****************
 * User: Sor          Date: 06/04/19   Time: 4:59p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * FEATURE: Synchronized with VPL EDMC device driver version 1.0.0.0 -
 * DONE.
 * FEATURE: Modify TMemMgrInitOptions and TMemMgrState structure - DONE.
 * FEATURE: Add MemMgr_GetPhysAddr(), MemMgr_CacheInvalidate(),
 * MemMgr_CacheCopyBack(), and MemMgr_CacheFlush() - DONE.
 * 
 * *****************  Version 4  *****************
 * User: Sor          Date: 05/08/20   Time: 7:30p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * UPDATE: Update version number from 1.0.0.0 to 2.0.0.0 - DONE.
 * 
 * *****************  Version 3  *****************
 * User: Sor          Date: 05/08/20   Time: 7:30p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * FEATURE: Add eAlignType field to TMemMgrState structure to prevent
 * unnecessary align padding - DONE.
 * FEATURE: Remove EMemMgrMemType definitions and move it to EMemType in
 * global_codec.h - DONE.
 * MODIFICATION: Use built-in control instead of ioctl to manage the
 * cached and noncached memory - DONE.
 * 
 * *****************  Version 2  *****************
 * User: Sor          Date: 05/07/22   Time: 8:23p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * 
 * *****************  Version 1  *****************
 * User: Sor          Date: 05/07/22   Time: 5:27p
 * Created in $/rd_2/project/SoC/Linux_Libraries/Sample/MemMgr_Lib
 * 
 */

/* ============================================================================================== */
#ifndef __MEMMGR_H__
#define __MEMMGR_H__

/* ============================================================================================== */
#include "typedef.h"
#include "errordef.h"
#include "global_codec.h"

/* ============================================================================================== */
#define MEMMGR_VERSION MAKEFOURCC(4, 0, 0, 12)
/* Version 4.0.0.7 modification, 2010.12.08 */
#define MEMMGR_ID_VERSION "4.0.0.122"
/* ======================================== */

/* Version 2.0.0.0 modification, 2005.08.20 */
/* ======================================== */

/* ============================================================================================== */
typedef struct mem_mgr_init_options
{
	DWORD dwVersion;

/* Version 3.0.0.0 modification, 2006.04.19 */
/* ======================================== */

	void *pObjectMem;
} TMemMgrInitOptions;

typedef struct mem_mgr_state
{
/* Version 3.0.0.0 modification, 2006.04.19 */
	DWORD dwBusNum;
/* ======================================== */
	DWORD dwSize;

/* Version 2.0.0.0 modification, 2005.08.20 */
/* Version 4.0.0.0 modification, 2006.07.10 */
/* ======================================== */
	EAlignType eAlignType;
/* ======================================== */
} TMemMgrState;

/* ============================================================================================== */
SCODE MemMgr_Initial(HANDLE *phObject, TMemMgrInitOptions *ptInitOptions);
SCODE MemMgr_Release(HANDLE *phObject);

/* MemMgr_GetMemory() is used to allocate a new memory section. The return value will be the address 
   of that memory section. If there is no additional memory space, the return value will be NULL */
DWORD MemMgr_GetMemory(HANDLE hObject, TMemMgrState *ptState);
/* Version 3.1.0.0 modification, 2006.04.25 */
/* MemMgr_FreeMemory() is used to release an existed memory section. */
SCODE MemMgr_FreeMemory(HANDLE hObject, DWORD dwVirtAddr);
/* ======================================== */
/* Version 3.0.0.0 modification, 2006.04.19 */
/* MemMgr_GetPhysAddr() is used to translate a virtual address to physical address. */
DWORD MemMgr_GetPhysAddr(HANDLE hObject, DWORD dwVirtAddr);
/* Version 3.1.0.0 modification, 2006.04.25 */
/* MemMgr_GetVirtAddr() is used to translate a physical address to virtual address. */
DWORD MemMgr_GetVirtAddr(HANDLE hObject, DWORD dwPhysAddr);
/* ======================================== */
/* MemMgr_CacheInvalidate() is used to invalidate the cache tags with the specific memory section. */
SCODE MemMgr_CacheInvalidate(HANDLE hObject, DWORD dwVirtAddr, DWORD dwSize);
/* MemMgr_CacheCopyBack() is used to copy data from cache back to external memory with the specific
   memory section. */
SCODE MemMgr_CacheCopyBack(HANDLE hObject, DWORD dwVirtAddr, DWORD dwSize);
/* MemMgr_CacheFlush() is used to copy data from cache back to external memory and invalidate those
   cache tags with the specific memory section. */
SCODE MemMgr_CacheFlush(HANDLE hObject, DWORD dwVirtAddr, DWORD dwSize);
/* ======================================== */
/* Version 3.2.0.0 modification, 2006.05.11 */
/* MemMgr_MapPhysAddr() is used to map a physical memory section to virtual memory section. */
DWORD MemMgr_MapPhysAddr(HANDLE hObject, DWORD dwPhysAddr, DWORD dwSize);
/* ======================================== */
/* Version 3.3.0.0 modification, 2006.06.02 */
/* MemMgr_RetrieveMapTbl() is used to copy all mapping elements with dwSrcPID process ID to current 
   process. */
SCODE MemMgr_RetrieveMapTbl(HANDLE hObject, DWORD dwSrcPID);
/* ======================================== */

DWORD MemMgr_QueryMemSize(TMemMgrInitOptions *ptInitOptions);
SCODE MemMgr_GetVersionInfo(TVersionNum *ptVersion);

/* ============================================================================================== */
#endif //__MEMMGR_H__
