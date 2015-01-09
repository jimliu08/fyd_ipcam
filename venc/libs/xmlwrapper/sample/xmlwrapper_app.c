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
 */

/*!
 *******************************************************************************
 * Copyright 2000-2006 VN, Inc. All rights reserved.
 *
 * \file
 * xmlwrapper_app.c
 *
 * \brief
 * XmlWrapper example application
 *
 * \date
 * 2006/12/11
 *
 * \author
 * Ming Jing Tsai
 *
 *******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xmlwrapper.h"
#include "xmlwrapper_app_local.h"

//=== handler tree : defined in "xmlwrapper_callback.c"
extern TXmlWrapperTreeMap ptTreeMap1[];
extern TXmlWrapperTreeMap ptTreeMap2[];
extern TXmlWrapperTreeMap ptTreeMap3[];
extern TXmlWrapperNamespaceMap ptNSMap3[];

int main()
{	
	const CHAR* filename1 ="xmlwrapper_app1.conf";
	const CHAR* filename2 ="xmlwrapper_app2.conf";
	const CHAR* filename3 ="xmlwrapper_app3.conf";
	const CHAR* filename4 ="namespace.xml";
	//===xmlwrapper var
	HANDLE    hXmlWrapperObject;
	HANDLE    hXmlWrapperObject2;
	HANDLE    hXmlWrapperObject4;
	TXmlWrapperInitOptions tInitOptions;	
	//==== filename1 xml conf var
	TAPPHandler tahHandle1;
	TAPPInfo taiInfo;	
	//=== filename2 xml conf var
	TAPPHandler tahHandle2;
	//==== filename3 xml conf var
	TAPPHandler tahHandle3;
	//==== filename4 xml conf var
	TAPPHandler tahHandle4;
	//free memory
	TAPPFlow *ptFlow;
	
	/* parsing xmlwrapper_app1.conf */
	//==============================================================================			
	/* INITIAL */	
	tInitOptions.dwVersion = XMLWRAPPER_VERSION;
	if (XmlWrapper_Initial(&hXmlWrapperObject, &tInitOptions) != S_OK)
	{		
		printf("%s %d : Initial XmlWrapper Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}	

	/* PARSER */
	if (XmlWrapper_SetHandler(hXmlWrapperObject, ptTreeMap1, NULL) != S_OK)
	{
		printf("%s %d : XmlWrapper_SetHandler Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	
	memset(&taiInfo, 0, sizeof(TAPPInfo));
	memset(&tahHandle1, 0, sizeof(TAPPHandler));
	tahHandle1.ptaiInfo = &taiInfo;	
	printf("Parse %s:\n", filename1);
	if (XmlWrapper_ReadFile_UsrDefFunc(filename1, hXmlWrapperObject, &tahHandle1) != S_OK)
	{		
		printf("%s %d : XmlWrapper_ReadFile_UsrDefFunc Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	
	/* Reloading config file */	
	//==============================================================================	
	printf("\nReloading %s:\n", filename1);
	if (XmlWrapper_ReadFile_UsrDefFunc_SameXmlHdl(NULL, hXmlWrapperObject, &tahHandle1) != S_OK)		
	{		
		printf("%s %d : XmlWrapper_ReloadFile_UsrDefFunc Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	
	/* free memory */	
	while(tahHandle1.ptafFlow)
	{
		ptFlow = tahHandle1.ptafFlow;
		tahHandle1.ptafFlow = tahHandle1.ptafFlow->ptNext;
		free(ptFlow);
	}	
	

	/* parsing xmlwrapper_app2.conf using ReadBuf*/	
	//==============================================================================
	tInitOptions.dwVersion = XMLWRAPPER_VERSION;
	if (XmlWrapper_Initial(&hXmlWrapperObject2, &tInitOptions) != S_OK)
	{		
		printf("%s %d : Initial XmlWrapper Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}		
	/* set new handler */
	if (XmlWrapper_SetHandler(hXmlWrapperObject2, ptTreeMap2, NULL) != S_OK)
	{
		printf("%s %d : XmlWrapper_SetHandler Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}	
	
	memset(&tahHandle2, 0, sizeof(TAPPHandler));	
	printf("\nParse New Conf %s:\n", filename2);
	char *pchBuffer = "<flow ver=\"1.1.0.0\"> \
			<first>XmlWrapper_Initial -> XmlWrapper_SetHandler -> XmlWrapper_ReadFile_UsrDefFunc</first> \
			<Read_NewConf>XmlWrapper_Reset->XmlWrapper_ReadFile_UsrDefFunc</Read_NewConf> \
			<reloading>XmlWrapper_ReloadFile_UsrDefFunc</reloading> \
			<exit>XmlWrapper_Release</exit> \
		</flow>";
//	printf("%s \n", pchBuffer);
	if (XmlWrapper_ReadBuf_UsrDefFunc((BYTE *)pchBuffer, hXmlWrapperObject2, &tahHandle2) != S_OK)
	{		
		printf("%s %d : XmlWrapper_ReadFile_UsrDefFunc Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	
	/* free memory */	
	while(tahHandle2.ptafFlow)
	{
		ptFlow = tahHandle2.ptafFlow;
		tahHandle2.ptafFlow = tahHandle2.ptafFlow->ptNext;
		free(ptFlow);
	}	

	/* xmlwrapper_app3.conf: parsing with the same TreeMapHandler as xmlwrapper_app1.conf*/	
	//==============================================================================
	memset(&tahHandle3, 0, sizeof(TAPPHandler));
	printf("\nParse New Conf %s:\n", filename3);
	if (XmlWrapper_ReadFile_UsrDefFunc_SameXmlHdl(filename3, hXmlWrapperObject, &tahHandle3) != S_OK)
	{		
		printf("%s %d : XmlWrapper_ReadFile_UsrDefFunc Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	/* free memory */	
	while(tahHandle3.ptafFlow)
	{
		ptFlow = tahHandle3.ptafFlow;
		tahHandle3.ptafFlow = tahHandle3.ptafFlow->ptNext;
		free(ptFlow);
	}

	/* INITIAL */	
	tInitOptions.dwVersion = XMLWRAPPER_VERSION;
	if (XmlWrapper_Initial(&hXmlWrapperObject4, &tInitOptions) != S_OK)
	{		
		printf("%s %d : Initial XmlWrapper Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}	

	/* PARSER */
	if (XmlWrapper_SetHandler(hXmlWrapperObject4, ptTreeMap3, ptNSMap3) != S_OK)
	{
		printf("%s %d : XmlWrapper_SetHandler Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	
	memset(&taiInfo, 0, sizeof(TAPPInfo));
	memset(&tahHandle4, 0, sizeof(TAPPHandler));
	tahHandle4.ptaiInfo = &taiInfo;	
	printf("Parse %s:\n", filename4);
	if (XmlWrapper_ReadFile_UsrDefFunc(filename4, hXmlWrapperObject4, &tahHandle4) != S_OK)
	{		
		printf("%s %d : XmlWrapper_ReadFile_UsrDefFunc Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	
	/* free memory */	
	while(tahHandle4.ptafFlow)
	{
		ptFlow = tahHandle4.ptafFlow;
		tahHandle4.ptafFlow = tahHandle4.ptafFlow->ptNext;
		free(ptFlow);
	}	

	/* RELEASE */
	//==============================================================================		
	if (XmlWrapper_Release(&hXmlWrapperObject) != S_OK)
	{
		printf("Release XmlWrapper Fail! \n");
		return S_FAIL;
	}
	if (XmlWrapper_Release(&hXmlWrapperObject2) != S_OK)
	{
		printf("Release XmlWrapper Fail! \n");
		return S_FAIL;
	}
	if (XmlWrapper_Release(&hXmlWrapperObject4) != S_OK)
	{
		printf("Release XmlWrapper Fail! \n");
		return S_FAIL;
	}
	printf("release succeed!\n");

	return 0;
}
