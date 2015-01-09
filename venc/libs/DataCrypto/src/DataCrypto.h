/*
 * $Header: /rd_2/project/Mozart/Linux_Libraries/DataCrypto/DataCrypto_Lib/DataCrypto.h 14    13/01/16 3:54p Yiming.liu $
 *
 * Copyright 2008 VN, Inc.  All rights reserved.
 *
 * Description:
 *
 * $History: DataCrypto.h $
 * 
 * *****************  Version 14  *****************
 * User: Yiming.liu   Date: 13/01/16   Time: 3:54p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/DataCrypto/DataCrypto_Lib
 * MODIFICATION: Add return error code and refine error massage - DONE.
 * 
 * *****************  Version 13  *****************
 * User: Yiming.liu   Date: 12/04/24   Time: 5:13p
 * Updated in $/rd_2/project/Mozart/Linux_Libraries/DataCrypto/DataCrypto_Lib
 * UPDATE: Update version number from 2.0.0.6 to 2.0.0.7 - DONE.
 * 
 * *****************  Version 11  *****************
 * User: Sean         Date: 11/07/29   Time: 6:02p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/DataCrypto/DataCrypto_Lib
 * UPDATE: Update version number from 2.0.0.5 to 2.0.0.6 - DONE.
 * 
 * *****************  Version 10  *****************
 * User: Albert.shen  Date: 09/01/08   Time: 10:43p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/DataCrypto/DataCrypto_Lib
 * 
 * *****************  Version 9  *****************
 * User: Albert.shen  Date: 09/01/08   Time: 11:38a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/DataCrypto/DataCrypto_Lib
 * 
 * *****************  Version 8  *****************
 * User: Albert.shen  Date: 08/08/20   Time: 5:28p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/DataCrypto/DataCrypto_Lib
 * UPDATE: Update version number from 2.0.0.2 to 2.0.0.3 - DONE.
 * MODIFICATION: Modify profile section to support __ASM_ARCH_PLATFORM_HAYDN_H__
 * time and request-grant interval information profiling - DONE.
 * 
 * *****************  Version 7  *****************
 * User: Albert.shen  Date: 08/06/10   Time: 2:30p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/DataCrypto/DataCrypto_Lib
 * UPDATE: Update version number from 2.0.0.1 to 2.0.0.2 - DONE.
 * 
 * *****************  Version 6  *****************
 * User: Albert.shen  Date: 08/04/18   Time: 9:43a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/DataCrypto/DataCrypto_Lib
 * BUG : In CRYPTO_HASH_TYPE_SHA_256 and CRYPTO_HASH_TYPE_SHA_224 the initial tables is
 * not assigned correctly - FIXED.
 * Update version number from 2.0.0.0 to 2.0.0.1 - DONE. 
 *
 * *****************  Version 5  *****************
 * User: Albert.shen  Date: 08/02/01   Time: 1:02p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/DataCrypto/DataCrypto_Lib
 * Update version number from 1.0.0.2 to 2.0.0.0 - DONE.
 * MODIFICATION : Add TDataCryptoState, DataCrypto_CheckParam(), ECryptoOperationFlags
 * ECryptoEncryptType, ECryptoEncryptMode, ECryptoHashType, ECryptoHashMode,
 * ECryptoHashStat - DONE.
 * MODIFICATION : Remove parameters which are replaced with TDataCryptoState in 
 * TDataCryptoInitOptions - DONE.
 * MODIFICATION : Add DataCrypto_CheckParam() - DONE.
 * MODIFICATION : Remove DataCrypto_SetOptions() and TDataCryptoOptions - DONE.
 * MODIFICATION : Modify DataCrypto_ProcessOneFrame() and DataCrypto_HashInit()
 * to use TDataCryptoState - DONE.
 * 
 * *****************  Version 4  *****************
 * User: Albert.shen  Date: 08/01/28   Time: 7:03p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/DataCrypto/DataCrypto_Lib
 * Update version number from 1.0.0.0 to 1.0.0.2 - DONE.
 *
 * *****************  Version 3  *****************
 * User: Albert.shen  Date: 08/01/23   Time: 10:53a
 * Updated in $/rd_2/project/SoC/Linux_Libraries/DataCrypto/DataCrypto_Lib
 * Update version number from 1.0.0.0 to 1.0.0.1 - DONE.
 *
 * *****************  Version 2  *****************
 * User: Albert.shen  Date: 07/12/19   Time: 3:07p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/DataCrypto/DataCrypto_Lib
 * 
 * *****************  Version 1  *****************
 * User: Albert.shen  Date: 07/12/19   Time: 10:30a
 * Created in $/rd_2/project/SoC/Linux_Libraries/DataCrypto/DataCrypto_Lib
 * 
 * 
 */
/* =========================================================================================== */
#ifndef __DATACRYPTO_H__
#define __DATACRYPTO_H__

/* =========================================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typedef.h"
#include "errordef.h"
#include "global_codec.h"

/* =========================================================================================== */

/* =========================================================================================== */
#define DATACRYPTO_VERSION MAKEFOURCC(2, 0, 0, 8)
#define DATACRYPTO_ID_VERSION "2.0.0.8"

/* =========================================================================================== */
#define DATACRYPTO_IN_BUS_NUM	0
#define DATACRYPTO_OUT_BUS_NUM	0

/* =========================================================================================== */
/* Version 2.0.0.0 modification, 2008.01.29 */
typedef enum crypto_operation_flags
{
	CRYPTO_OP_ENCRYPT = 0,
	CRYPTO_OP_DECRYPT = 1,
	CRYPTO_OP_HASH = 2
} ECryptoOperationFlags;

typedef enum crypto_encrypt_type
{
	CRYPTO_ENCRYPT_TYPE_AES = 0,
	CRYPTO_ENCRYPT_TYPE_TDES = 1,
	CRYPTO_ENCRYPT_TYPE_DES = 2
} ECryptoEncryptType;

typedef enum crypto_encrypt_mode
{
	CRYPTO_ENCRYPT_MODE_EBC = 0,
	CRYPTO_ENCRYPT_MODE_CBC = 1,
	CRYPTO_ENCRYPT_MODE_CFB = 2,
	CRYPTO_ENCRYPT_MODE_OFB = 3,
	CRYPTO_ENCRYPT_MODE_CTR = 4
} ECryptoEncryptMode;


typedef enum crypto_hash_type
{
	CRYPTO_HASH_TYPE_SHA_1 = 0,
	CRYPTO_HASH_TYPE_SHA_256 = 2,
	CRYPTO_HASH_TYPE_SHA_224 = 3,
	CRYPTO_HASH_TYPE_SHA_512 = 4,
	CRYPTO_HASH_TYPE_SHA_384 = 5
} ECryptoHashType;

typedef enum crypto_hash_mode
{
	CRYPTO_HASH_MODE_HASHING_ONLY = 0,
	CRYPTO_HASH_MODE_HMAC = 1
} ECryptoHashMode;

typedef enum crypto_hash_stat
{
	CRYPTO_HASH_STAT_BEGIN = 0,
	CRYPTO_HASH_STAT_END = 1,
	CRYPTO_HASH_STAT_MID = 2
} ECryptoHashStat;
/* ======================================== */


/* =========================================================================================== */
typedef struct data_crypto_init_options
{
/* Version 2.0.0.0 modification, 2008.01.29 */
	DWORD dwVersion;
	void *pObjectMem;
/* ======================================== */

} TDataCryptoInitOptions;

/* Version 2.0.0.0 modification, 2008.01.29 */
/* ======================================== */

/* Version 2.0.0.0 modification, 2008.01.29 */
typedef struct data_crypto_state
{
	ECryptoOperationFlags eOpMode;			// OP_ENCRYPTION, OP_DECRYPTION, OP_HASH

	ECryptoEncryptType eCryptoType;    //ENCRYPT_AES, ENCRYPT_TDES, ENCRYPT_DES
	ECryptoEncryptMode eCryptoMode;	   //EBC_MODE, CBC_MODE, CFB_MODE, OFB_MODE, CTR_MODE

	ECryptoHashType eHashType;	  //HASH_SHA_1, HASH_SHA_256, HASH_SHA_224, HASH_SHA_512, HASH_SHA_384
	ECryptoHashMode eHashMode;	  //HASHING_ONLY, HMAC
	ECryptoHashStat eHashStat;	  // HASH_BEGIN, HASH_END, HASH_MIDDLE

	DWORD dwKeySize;
	DWORD dwTextSize;
	DWORD dwHashSize;

	BYTE *pbyKey;		  // Key buffer, 128byte alignment
	BYTE *pbyInitVector;  // Initialization vector buffer, 16byte	 alignment
	BYTE *pbyInput;	  // Input buffer, 128byte alignment
	BYTE *pbyOutput;	  // Output buffer, 128byte alignment

} TDataCryptoState;
/* ======================================== */

/* =========================================================================================== */
SCODE DataCrypto_Initial(HANDLE *phObject, TDataCryptoInitOptions *ptInitOptions);
SCODE DataCrypto_Release(HANDLE *phObject);

SCODE DataCrypto_ProcessOneFrame(HANDLE hObject, TDataCryptoState *ptState);

/* Version 2.0.0.0 modification, 2008.02.01 */
SCODE DataCrypto_CheckParam(HANDLE hObject, TDataCryptoState *ptState);
//SCODE DataCrypto_SetOptions(HANDLE hObject, TDataCryptoOptions *ptOptions);
/* ======================================== */
DWORD DataCrypto_QueryMemSize(TDataCryptoInitOptions *ptInitOptions);

SCODE DataCrypto_GetVersionInfo(BYTE *pbyMajor, BYTE *pbyMinor, BYTE *pbyBuild, BYTE *pbyRevision);
/* =========================================================================================== */
#endif //__DATACRYPTO_H__
