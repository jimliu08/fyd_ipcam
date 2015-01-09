/*
 *******************************************************************************
 * $Header: /rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib/errordef.h 2     08/01/23 2:01p Alan $
 *
 * Copyright 2008 VN Inc. All rights reserved.
 *
 * $History: errordef.h $
 * 
 * *****************  Version 2  *****************
 * User: Alan         Date: 08/01/23   Time: 2:01p
 * Updated in $/rd_2/project/SoC/Linux_Libraries/VideoDisp/VideoDisp_Lib
 * 
 * *****************  Version 1  *****************
 * User: Joe          Date: 04/04/13   Time: 4:20p
 * Created in $/rd_common/common/SRC
 * 
 * *****************  Version 2  *****************
 * User: Joe          Date: 03/07/15   Time: 4:42p
 * Updated in $/rd_1/Project/TM1300_PSOS/FarSeer/common/src
 * Update SCODE definition.
 *
 * *****************  Version 1  *****************
 * User: Joe          Date: 03/03/10   Time: 10:54a
 * Created in $/rd_1/Project/TM1300_PSOS/FarSeer/COMMON/src
 * common used source. Including debug macros, type definitions, error
 * code definitions.
 *
 * *****************  Version 3  *****************
 * User: Allatin      Date: 03/01/13   Time: 5:01p
 * Updated in $/rd_common/common_header
 *
 * *****************  Version 2  *****************
 * User: Joe          Date: 03/01/13   Time: 3:52p
 * Updated in $/rd_common/common_header
 * 1. Change the header
 * 2. Change the definition of SCODE to avoid confliction with Windows
 * 3. Add an Error code ERR_INVALID_VERSION
 *
 *******************************************************************************
 */

/*!
 *******************************************************************************
 * Copyright 2008 VN Inc. All rights reserved.
 *
 * \file
 * errordef.h
 *
 * \brief
 * common error code and macros definitions
 *
 * \date
 * 2003/01/13
 *
 * \author
 * Jason Yang
 *
 *
 *******************************************************************************
 */

#ifndef _VIVOERRORDEF_
#define _VIVOERRORDEF_

//
//  SCODEs are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +-+-----------------------------+-------------------------------+
//  |S|        Facility             |               Code            |
//  +-+-----------------------------+-------------------------------+
//
//  where
//
//      S - Severity - indicates success/fail
//
//          0 - Success
//          1 - Fail
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//


//
// Define the facility codes
//
#define FACILITY_CODEC_VIDEO             1
#define FACILITY_CODEC_AUDIO             2
#define FACILITY_PROTOCOL                3
#define FACILITY_APP                     4
#define FACILITY_DATABASE                5


//
// return type
//
// to avoid conflicting with windows type
#if !defined(__wtypes_h__) && !defined(_SCODE_)
typedef unsigned int SCODE;
#define	_SCODE_
#endif

//
// Severity values
//
#ifndef SEVERITY_SUCCESS
#define SEVERITY_SUCCESS    0
#endif
#ifndef SEVERITY_ERROR
#define SEVERITY_ERROR      1
#endif

//
// Generic test for success on any status value.
//
#ifndef IS_SUCCESS
#define IS_SUCCESS(Status) ((SCODE)(Status) >> 31 == SEVERITY_SUCCESS)
#endif

//
// Generic test for error on any status value.
//
#ifndef IS_FAIL
#define IS_FAIL(Status) ((SCODE)(Status) >> 31 == SEVERITY_ERROR)
#endif

//
// Return the code
//
#ifndef SCODE_CODE
#define SCODE_CODE(sc)      ((sc) & 0xFFFF)
#endif

//
//  Return the facility
//
#ifndef SCODE_FACILITY
#define SCODE_FACILITY(sc)    (((sc) >> 16) & 0x1fff)
#endif

//
// Create an SCODE value from component pieces
//
#ifndef MAKE_SCODE
#define MAKE_SCODE(sev,fac,code) \
    ((SCODE) (((unsigned long)(sev)<<31) | ((unsigned long)(fac)<<16) | ((unsigned long)(code))) )
#endif

//
// general return codes
//
#ifndef S_OK
#define S_OK                           ((SCODE)  0)
#endif
#ifndef S_FAIL
#define S_FAIL                         ((SCODE) -1)
#endif

//--------------------------------------------------------------------------------
// VNDP Genaral API Error Codes
//--------------------------------------------------------------------------------

//
// MessageId: ERR_INVALID_HANDLE
//
// MessageText:
//
//  Invalid object handle.
//
#define ERR_INVALID_HANDLE            0x80000001

//
// MessageId: ERR_OUT_OF_MEMORY
//
// MessageText:
//
//  Memory allocate fail.
//
#define ERR_OUT_OF_MEMORY             0x80000002

//
// MessageId: ERR_INVALID_ARG
//
// MessageText:
//
//  One or more arguments are invalid.
//
#define ERR_INVALID_ARG               0x80000003

//
// MessageId: ERR_NOT_IMPLEMENT
//
// MessageText:
//
//  Not implemented
//
#define ERR_NOT_IMPLEMENT             0x80000004

//
// MessageId: ERR_INVALID_VERSION
//
// MessageText:
//
//  Invalid version number
//
#define ERR_INVALID_VERSION           0x80000005

#endif //_VIVOERRORDEF_
