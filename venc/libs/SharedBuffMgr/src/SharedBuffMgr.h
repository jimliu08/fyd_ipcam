
/* =========================================================================================== */
#ifndef __SHAREDBUFFMGR_H__
#define __SHAREDBUFFMGR_H__

/* =========================================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typedef.h"
#include "errordef.h"
#include "global_codec.h"

/* =========================================================================================== */
#define SHAREDBUFFMGR_VERSION MAKEFOURCC(2, 1, 0, 0)
 
/* =========================================================================================== */
typedef enum shared_buff_mgr_process_role
{
	esbmprReader = 0,
	esbmprWriter,
	esbmprSlowReader
} ESharedBuffMgrProcessRole;

/*typedef enum shared_buff_mgr_read_method
{
	esbmrmNext,
	esbmrmLatest
} ESharedBuffMgrReadMethod;
*/
typedef struct shared_buff_mgr_init_options
{
	// check driver version
	DWORD dwVersion;
	// how many buffer you want
	DWORD dwBufNum;
	// each buffer header size
  	DWORD dwHeaderSize;
  	// each buffer data size
    DWORD dwDataSize;
    // AlignType for MemMgr
    EAlignType eAlignType;
    // allocate external used
	void *pObjectMem;
	// Reader/Writer
	ESharedBuffMgrProcessRole eProcessRole;
	// bus index for MemMgr_GetMemory
	DWORD dwBusIndex; 
	// share buffer minor number
	DWORD dwMinorNum;
	// buffer initial value, only refers by writer
	BYTE byInitialVal;
	// --------- optional ---------------
	// read next buffer or the latest buffer , it is used when the role is reader, or it will be ignore by writer
	//ESharedBuffMgrReadMethod eReadMethod;
	// block or nonblock
	//BOOL bBlock; 	
} TSharedBuffMgrInitOptions;

typedef struct shared_buff_mgr_state
{
    DWORD dwIndex;
//    DWORD dwHdrSize;
//    DWORD dwDataSize;
    BYTE  *pbyHdrAddr;
    BYTE  *pbyDataAddr;
} TSharedBuffMgrState;

/* =========================================================================================== */
#ifdef __cplusplus
extern "C" {
#endif

/* Main shared buffer functions */
SCODE SharedBuffMgr_Initial(HANDLE *phObject, TSharedBuffMgrInitOptions *ptInitOptions);
SCODE SharedBuffMgr_Release(HANDLE *phObject);

DWORD SharedBuffMgr_QueryMemSize(TSharedBuffMgrInitOptions *ptInitOptions);
SCODE SharedBuffMgr_GetVersionInfo(BYTE *pbyMajor, BYTE *pbyMinor, BYTE *pbyBuild, BYTE *pbyRevision);

SCODE SharedBuffMgr_GetBuffer(HANDLE hObject, TSharedBuffMgrState *ptState);
SCODE SharedBuffMgr_ReleaseBuffer(HANDLE hObject, TSharedBuffMgrState *ptState);
SCODE SharedBuffMgr_StopGetBuffer(HANDLE hObject);
SCODE SharedBuffMgr_GetFileDescriptor(HANDLE hObject, int *pifd);
	  
#ifdef __cplusplus
}
#endif

/* =========================================================================================== */
#endif	//__SHAREDBUFFMGR_H__

