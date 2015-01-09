/*
 *******************************************************************************
 * $Header: /rd_common/common_header/common.h 2     03/02/14 3:34p Jason $
 *
 *  Copyright (c) 2000-2003 VN Inc. All rights reserved.
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
 *  | VN INC.                                                    |
 *  +-----------------------------------------------------------------+
 *
 * $History: common.h $
 * 
 * *****************  Version 2  *****************
 * User: Jason        Date: 03/02/14   Time: 3:34p
 * Updated in $/rd_common/common_header
 * 
 * *****************  Version 1  *****************
 * User: Jason        Date: 03/02/14   Time: 2:32p
 * Created in $/rd_common/common_header
 *
 *******************************************************************************
 */

/*!
 *******************************************************************************
 * Copyright 2000-2003 VN, Inc. All rights reserved.
 *
 * \file
 * common.h
 *
 * \brief
 * common definitions
 *
 * \date
 * 2003/02/11
 *
 * \author
 * Jason Yang
 *
 *
 *******************************************************************************
 */
#ifndef _COMMONDEF_H_
#define _COMMONDEF_H_

#ifndef DLLAPI
#ifdef _WIN32
  #ifdef _USRDLL
    #define DLLAPI __declspec(dllexport) __stdcall
  #else
    #define DLLAPI __stdcall
  #endif
#else
  #define DLLAPI
#endif
#endif

/*! Make from four character codes to one 32-bits DWORD */
#ifndef MAKEFOURCC
  #define MAKEFOURCC(ch0, ch1, ch2, ch3)                          \
              ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
              ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif //defined(MAKEFOURCC)



#endif // _COMMONDEF_H_

