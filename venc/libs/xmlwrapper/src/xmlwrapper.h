/*
 *******************************************************************************
 * $Header: $
 *
 *  Copyright (c) 2000-2006 VN Inc. All rights reserved.
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
 * $History: $
 * 
 *******************************************************************************
 */

/*!
 *******************************************************************************
 * Copyright 2000-2006 VN, Inc. All rights reserved.
 *
 * \file
 * XmlWrapper.h
 *
 * \brief
 * XML configuration file parsing and call user defined call back function library header file
 *
 * \date
 * 2006/12/07
 *
 * \author
 * Ming-Jing Tsai
 *
 *
 *******************************************************************************
 */

#ifndef _XMLWRAPPER_H_
#define _XMLWRAPPER_H_

#include "typedef.h"
#include "errordef.h"   
#include "global_codec.h"
#include "expat.h"

/*! FOUR_CC Version code of your \b XmlWrapper instance. */
#define XMLWRAPPER_VERSION		MAKEFOURCC(4, 0, 0, 2)

/* ======================================================================================================= */
#if 0
SCODE XmlWrapper_Initial(HANDLE *phObject, TXmlWrapperInitOptions *ptInitOptions);
SCODE XmlWrapper_Reset(HANDLE hObject);
SCODE XmlWrapper_Release(HANDLE *phObject);

SCODE XmlWrapper_SetHandler(HANDLE hObject, TXmlWrapperTreeMap *ptTreeMap, TXmlWrapperNamespaceMap *ptNSMap);
SCODE XmlWrapper_ReadFile_UsrDefFunc(const char *filename, HANDLE hXMLWrapperObject, HANDLE hUserData);
SCODE XmlWrapper_ReadBuf_UsrDefFunc(BYTE *pbyBuffer, HANDLE hXMLWrapperObject, HANDLE hUserData);
SCODE XmlWrapper_ReadFile_UsrDefFunc_SameXmlHdl(const CHAR *szFileName, HANDLE hXMLWrapperObject, HANDLE hUserData);
SCODE XmlWrapper_ReadBuf_UsrDefFunc_SameXmlHdl(BYTE *pbyBuffer, HANDLE hXMLWrapperObject, HANDLE hUserData);
#endif

/* ======================================================================================================= */
/*! Not calling call-back function while parsing start element and chracter data of other sibling
under the same parent XML Path. */
#define IGNORE_SIBLING_CONFIG	0x80060001

/*! Not calling call-back function while parsing start element and chracter data of child Data. */
#define IGNORE_CHILD_CONFIG    0x80060002

/* ======================================================================================================= */
/*! Type of \b StartElement Call Back Function */
typedef SCODE (* StartElemHandler) (void *userData, const CHAR* s, const CHAR** atts);

/*! Type of \b XML_CharacterDataHandler, XML_EndElementHandler, define in <expat.h> */
#if 0 
typedef void (*XML_CharacterDataHandler)(void *userData, const CHAR *s, SDWORD len);
typedef void (XMLCALL *XML_EndElementHandler) (void *userData, const XML_Char *name);
#endif

/* ======================================================================================================= */
/*! XmlWrapper initial options */
typedef struct xmlwrapper_initial_options
{
	/*! This is a version control parameter. */
	DWORD			dwVersion;
} TXmlWrapperInitOptions;

/* ======================================================================================================= */
/*! \b XmlWrapper tree map (used for construct xmltree and handler) */
typedef struct xmlwrapper_treemap
{
	/*! Config name to be parsed */
	const char	*szElemName;
	/*! start element call back function  */
	StartElemHandler			pfStartHdl;
	/*! charater data call back function prototype, define by <expat.h> */
	XML_CharacterDataHandler	pfChDataHdl;	
	XML_EndElementHandler		pfEndHdl;
} TXmlWrapperTreeMap;

/* ======================================================================================================= */
/*! \b XmlWrapper namespace map (used for construct prefix and namespace mapping relation) */
typedef struct xmlwrapper_namespacemap
{
	/*! prefix */
	const char	*szPrefix;
	/*! namespace  */
	const char	*szNamespace;
} TXmlWrapperNamespaceMap;

#ifdef __cplusplus
extern "C" {
#endif

/*!
******************************************************************************
* \brief
* Create handle of TXMLWRAPPER object, this function should be called before calling any other function
*
* \param phObject
* \a (o) pointer to receive the handle of the TXMLWRAPPER object
*
* \param ptInitOptions
* \a (i) Pointer of XmlWrapper initial options
*
* \retval S_OK
* Create object ok
*
* \retval ERR_INVALID_VERSION;
* library version number incorrect
*
* \retval ERR_OUT_OF_MEMORY;
* no enough memory for TXMLWRAPPER
*
* \remark
* A XmlWrapper object shall be initialized before using it.
*
* \see XmlWrapper_Release
*
******************************************************************************
*/
SCODE XmlWrapper_Initial(HANDLE *phObject, TXmlWrapperInitOptions *ptInitOptions);

/*!
******************************************************************************
* \brief
* Reset XML Parser
*
* \param hObject
* \a (i) handle of TXMLWRAPPER
*
* \retval S_OK
* Reset xml parser succeed
*
* \retval S_FAIL
* Reset xml parser fail
*
* \remark
* MUST call this function before reload config file or read new config file
******************************************************************************
*/
SCODE XmlWrapper_Reset(HANDLE hObject);

/*!
******************************************************************************
* \brief
* Release TXMLWRAPPER object : free memories of xml parser and xml handler tree
*
* \param phObject
* \a (i) pointer of TXMLWRAPPER Handle
*
* \retval S_OK
* Release succeed
*
* \retval S_FAIL
* Release fail
*
* \see XmlWrapper_Initial
*
******************************************************************************
*/
SCODE XmlWrapper_Release(HANDLE *phObject);

/*!
******************************************************************************
* \brief
* Construct XMLTree and Set Handler
*
* \param hObject
* \a (o)  handle of XmlWrapper Object
*
* \param ptTreeMap
* \a (i) Pointer of TXmlWrapperTreeMap
*
* \param ptNSMap
* \a (i) Pointer of TXmlWrapperNamespaceMap
*
* \retval S_OK
* Construct XMLTree Succeed
*
* \retval ERR_OUT_OF_MEMORY
* No enough memory to construct XMLTree
*
******************************************************************************
*/
SCODE XmlWrapper_SetHandler(HANDLE hObject, TXmlWrapperTreeMap *ptTreeMap, TXmlWrapperNamespaceMap *ptNSMap);

/*!
******************************************************************************
* \brief
* Read config file and call the corresponding call back function
*
* \param filename
* \a (i) Configuration file name
*
* \param hXMLWrapperObject
* \a (i) handle of XmlWrapper Object
*
* \param hUsrData
* \a (i) handle of user data
*
* \retval S_OK
* Get configurations ok
*
* \retval S_FAIL
* Parse XMLFile Fail
******************************************************************************
*/
SCODE XmlWrapper_ReadFile_UsrDefFunc(const CHAR *filename, HANDLE hXMLWrapperObject, HANDLE hUsrData);

/*!
******************************************************************************
* \brief
* Read a buffer and call the corresponding call back function
*
* \param pbyBuffer
* \a (i) Source xml buffer, end with '\0'
*
* \param hXMLWrapperObject
* \a (i) handle of XmlWrapper Object
*
* \param hUsrData
* \a (i) handle of user data
*
* \retval S_OK
* Get configurations ok
*
* \retval S_FAIL
* Parse buffer Fail
******************************************************************************
*/
SCODE XmlWrapper_ReadBuf_UsrDefFunc(BYTE *pbyBuffer, HANDLE hXMLWrapperObject, HANDLE hUserData);

/*!
******************************************************************************
* \brief
* Parsing szFileName with the same TXmlWrapperTreeMap Handler,
* If szFileName == NULL, reloading the same Config File (hXMLWrapperObject->szFileName) that is just parsed
*
* \param filename
* \a (i) Configuration file name
*
* \param hXMLWrapperObject
* \a (i) handle of XmlWrapper Object
*
* \param hUserData
* \a (i) handle of user data
*
* \retval S_OK
* Get configurations ok
*
* \retval S_FAIL
* Parse XMLFile Fail
******************************************************************************
*/
SCODE XmlWrapper_ReadFile_UsrDefFunc_SameXmlHdl(const CHAR *szFileName, HANDLE hXMLWrapperObject, HANDLE hUserData);

/*!
******************************************************************************
* \brief
* Parsing pbyBuffer with the same TXmlWrapperTreeMap Handler,
*
* \param pbyBuffer
* \a (i) Source xml buffer, end with '\0'
*
* \param hXMLWrapperObject
* \a (i) handle of XmlWrapper Object
*
* \param hUserData
* \a (i) handle of user data
*
* \retval S_OK
* Get configurations ok
*
* \retval S_FAIL
* Parse XMLFile Fail
******************************************************************************
*/
SCODE XmlWrapper_ReadBuf_UsrDefFunc_SameXmlHdl(BYTE *pbyBuffer, HANDLE hXMLWrapperObject, HANDLE hUserData);

#ifdef __cplusplus
}
#endif

#endif /* _XMLWRAPPER_H_ */
/*! \example xmlwrapper_app.c
 *  This is a example of how to use this library.
 */

