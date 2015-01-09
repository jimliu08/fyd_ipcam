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
 * xmlmgr.h
 *
 * \brief
 * XML configuration file parsing (read/write) library header file
 *
 * \date
 * 2006/08/24
 *
 * \author
 * Rey Cheng
 *
 *
 *******************************************************************************
 */

#ifndef _XMLMGR_H_
#define _XMLMGR_H_

#include "typedef.h"
#include "errordef.h"
#include "global_codec.h"

#if 0
// --------------------- function brief ----------------------------------------
void xmlmgr_set_string(char *szConfValue, void *pObj);
void xmlmgr_set_long(char *szConfValue, void *pObj);
void xmlmgr_set_short(char *szConfValue, void *pObj);
SCODE XmlMgr_Initial(HANDLE *phObject);
SCODE XmlMgr_ReadFile(HANDLE hObject, const char *szFileName);
SCODE XmlMgr_WriteFile(HANDLE hObject, const char *szFileName);
char *XmlMgr_GetConfValue(HANDLE hObject, const char *szConfName);
SCODE XmlMgr_GetMultiConfValue(HANDLE hObject, TXmlMgrParsingMap *ptParsingMap);
SCODE XmlMgr_SetConfValue(HANDLE hObject, const char *szConfName, const char *szConfValue);
SCODE XmlMgr_SetMultiConfValue(HANDLE hObject, int iConfNum, const char *szConfNames[], const char *szConfValues[]);
SCODE XmlMgr_Reset(HANDLE hObject);
SCODE XmlMgr_Release(HANDLE *phObject);
#endif

/*! FOUR_CC Version code of your \b XmlMgr instance. */
#define XMLMGR_VERSION		MAKEFOURCC(1, 1, 0, 20)

/*! Type of the parsing function defined in struct xmlmgr_parsing_map, refer to xmlmgr_parsing.c for the default parsing functions */
typedef void (* FParsingAction) (char *szArgs, void *pObject);

/*! XmlMgr initial options */
typedef struct xmlmgr_initial_options
{
	/*! This is a version control parameter. */
	DWORD			dwVersion;
} TXmlMgrInitOptions;

/*! XmlMgr parsing map (used for getting multiple configurations) */
typedef struct xmlmgr_parsing_map
{
	/*! Config name to be parsed */
	const char *szName;
	/*! Corresponding parsing action, refer to xmlmgr_parsing.c for the default parsing functions  */
	FParsingAction pfnParsingAction;
	/*! Output to pObject */
	void *pObject;
} TXmlMgrParsingMap;

#ifdef __cplusplus
extern "C" {
#endif

/*!
******************************************************************************
* \brief
* Create handle of XmlManager object, this function should be called before calling any other function
*
* \param phObject
* \a (o) pointer to receive the handle of the XmlManager object
*
* \param pInitOpts
* Pointer of xmlmgr initial options
*
* \retval S_OK
* Create object ok
*
* \retval S_FAIL
* Create object failed
*
* \remark
* A XmlManager object shall be initialized before using it.
*
* \see XmlMgr_Release
*
******************************************************************************
*/
SCODE XmlMgr_Initial(HANDLE *phObject, TXmlMgrInitOptions *pInitOpts);

/*!
******************************************************************************
* \brief
* Read XML file and construct the XML parsing tree
*
* \param hObject
* Handle of the XmlManager object
*
* \param szFileName
* A pointer to a null terminated string. It is the name of the input file.
*
* \retval S_OK
* Read file ok
*
* \retval S_FAIL
* Read file failed
*
* \see XmlMgr_Initial
*
******************************************************************************
*/
SCODE XmlMgr_ReadFile(HANDLE hObject, const char *szFileName);

/*!
******************************************************************************
* \brief
* Write current XML parsing tree to the specific file
*
* \param hObject
* Handle of the XmlManager object
*
* \param szFileName
* A pointer to a null terminated string. It is the name of the output file.
*
* \retval S_OK
* Write file ok
*
* \retval S_FAIL
* Write file failed
*
* \see XmlMgr_ReadFile
*
******************************************************************************
*/
SCODE XmlMgr_WriteFile(HANDLE hObject, const char *szFileName);

/*!
******************************************************************************
* \brief
* Get the specific element's configuration from current XML parsing tree
*
* \param hObject
* Handle of the XmlManager object
*
* \param szConfName
* A pointer to a null terminated string. It is the name of the element.
*
* \retval S_OK
* Get configuration ok
*
* \retval S_FAIL
* Get configuration failed
*
* \remark
* Be sure to call XmlMgr_ReadFile() before calling this.
*
* \see XmlMgr_ReadFile
*
******************************************************************************
*/
char *XmlMgr_GetConfValue(HANDLE hObject, const char *szConfName);

/*!
******************************************************************************
* \brief
* Get the elements' (retrieve from ptParsingMap) configurations from current XML parsing tree
*
* \param hObject
* Handle of the XmlManager object
*
* \param ptParsingMap
* Pointer to the parsing map
*
* \retval S_OK
* Get configurations ok
*
* \retval S_FAIL
* Get configurations failed
*
* \remark
* Be sure to call XmlMgr_ReadFile() before calling this.
*
* \see XmlMgr_ReadFile
*
******************************************************************************
*/
SCODE XmlMgr_GetMultiConfValue(HANDLE hObject, TXmlMgrParsingMap *ptParsingMap);

/*!
******************************************************************************
* \brief
* Set the specific element's configuration with the given value
*
* \param hObject
* Handle of the XmlManager object
*
* \param szConfName
* A pointer to a null terminated string. It is the name of the element.
*
* \param szConfValue
* A pointer to a null terminated string. It is the value that will be set to the element.
*
* \retval S_OK
* Set configuration ok
*
* \retval S_FAIL
* Set configuration failed
*
* \remark
* Be sure to call XmlMgr_ReadFile() before calling this.
*
* \see XmlMgr_ReadFile
*
******************************************************************************
*/
SCODE XmlMgr_SetConfValue(HANDLE hObject, const char *szConfName, const char *szConfValue);

/*!
******************************************************************************
* \brief
* Set multiple elements' configurations with the given values
*
* \param hObject
* Handle of the XmlManager object
*
* \param iConfNum
* Number of elements to be set
*
* \param szConfNames
* An array of null terminated strings
*
* \param szConfValues
* An array of null terminated strings
*
* \retval S_OK
* Set configurations ok
*
* \retval S_FAIL
* Set configurations failed
*
* \remark
* Be sure to call XmlMgr_ReadFile() before calling this. This operation is not atomic.
*
* \see XmlMgr_ReadFile
*
******************************************************************************
*/
SCODE XmlMgr_SetMultiConfValue(HANDLE hObject, int iConfNum, const char **szConfNames, const char **szConfValues);

/*!
******************************************************************************
* \brief
* Reset this XmlManager object to parse another XML file.
*
* \param hObject
* The handle of the XmlManager object
*
* \retval S_OK
* Reset object ok
*
* \retval S_FAIL
* Reset object failed
*
* \remark
* Be sure to call this before parsing another file.
*
* \see XmlMgr_Initial
*
******************************************************************************
*/
SCODE XmlMgr_Reset(HANDLE hObject);

/*!
******************************************************************************
* \brief
* Release an XmlManager object
*
* \param phObject
* \a (i/o) pointer to the handle of the XmlManager object
*
* \retval S_OK
* Release object ok
*
* \retval S_FAIL
* Release object failed
*
* \remark
* After release the oject, the *phObject will be set to NULL
*
* \see XmlManager_Initial
*
******************************************************************************
*/
SCODE XmlMgr_Release(HANDLE *phObject);

/*!
******************************************************************************
* \brief
* Get configuration value (type of string)
*
* \param szConfValue
* The value of the configuration
*
* \param pObj
* Pointer of the object, whose value (in string format) will be set to szConfValue
*
* \see struct xmlmgr_parsing_map
*
******************************************************************************
*/
void xmlmgr_set_string(char *szConfValue, void *pObj);

/*!
******************************************************************************
* \brief
* Get configuration value (type of long - 4bytes)
*
* \param szConfValue
* The value of the configuration
*
* \param pObj
* Pointer of the object, whose value (in long format) will be set to szConfValue
*
* \see struct xmlmgr_parsing_map
*
******************************************************************************
*/
void xmlmgr_set_long(char *szConfValue, void *pObj);

/*!
******************************************************************************
* \brief
* Get configuration value (type of short - 2bytes)
*
* \param szConfValue
* The value of the configuration
*
* \param pObj
* Pointer of the object, whose value (in short format) will be set to szConfValue
*
* \see struct xmlmgr_parsing_map
*
******************************************************************************
*/
void xmlmgr_set_short(char *szConfValue, void *pObj);

#ifdef __cplusplus
}
#endif

#endif /* _XMLMGR_H_ */
/*! \example xmlmgr_app.c
 *  This is a example of how to use this library.
 */

