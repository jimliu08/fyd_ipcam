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
 */

/*!
 *******************************************************************************
 * Copyright 2000-2006 VN, Inc. All rights reserved.
 *
 * \file
 * xmlwrapper_app_callback.c
 *
 * \brief
 * Xmlwrapper app callback function
 *
 * \date
 * 2006/12/11
 *
 * \author
 * Ming Jing Tsai
 *
 *
 *******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xmlwrapper.h"
#include "xmlwrapper_app_local.h"

/* function decleration */
//============================================================================
/* start element handler */
SCODE xmlwrapper_app_Flow(void *userData, const CHAR* s, const CHAR** atts);

/* character data handler*/
/* Flow */
void xmlwrapper_app_SetFirst(void *userData, const char *s, int len);
void xmlwrapper_app_SetReloading(void *userData, const char *s, int len);
void xmlwrapper_app_SetNewConf(void *userData, const char *s, int len);
void xmlwrapper_app_SetExit(void *userData, const char *s, int len);

/* Info */
void xmlwrapper_app_SetVer(void *userData, const char *s, int len);
void xmlwrapper_app_SetAuthor(void *userData, const char *s, int len);
void xmlwrapper_app_SetDate(void *userData, const char *s, int len);
void xmlwrapper_app_SetUsage(void *userData, const char *s, int len);

/* first xml conf treemap */
//============================================================================
TXmlWrapperTreeMap ptTreeMap1[] = 
{	
	/* Info */
	{"root/xml_wrapper/info/version", NULL, &xmlwrapper_app_SetVer, NULL},
	{"root/xml_wrapper/info/author", NULL, &xmlwrapper_app_SetAuthor, NULL},
	{"root/xml_wrapper/info/date", NULL, &xmlwrapper_app_SetDate, NULL},
	{"root/xml_wrapper/info/usage", NULL, &xmlwrapper_app_SetUsage, NULL},

	/* Flow */
	{"root/xml_wrapper/flow", &xmlwrapper_app_Flow, NULL, NULL},
	{"root/xml_wrapper/flow/first", NULL, &xmlwrapper_app_SetFirst, NULL},	
	{"root/xml_wrapper/flow/reloading", NULL, &xmlwrapper_app_SetReloading, NULL},
	{"root/xml_wrapper/flow/exit", NULL, &xmlwrapper_app_SetExit, NULL},
	{NULL, NULL, NULL, NULL}
};

/* second xml conf treemap */
//============================================================================
TXmlWrapperTreeMap ptTreeMap2[] = 
{		
	/* Flow */
	{"flow", &xmlwrapper_app_Flow, NULL, NULL},	
	{"flow/first", NULL, &xmlwrapper_app_SetFirst, NULL},	
	{"flow/Read_NewConf", NULL, &xmlwrapper_app_SetNewConf, NULL},
	{"flow/reloading", NULL, &xmlwrapper_app_SetReloading, NULL},		
	{"flow/exit", NULL, &xmlwrapper_app_SetExit, NULL},
	{NULL, NULL, NULL, NULL}
};

TXmlWrapperTreeMap ptTreeMap3[] = 
{	
	/* Info */
	{"soap:Envelope/soap:Body/trt:GetStreamUri/trt:StreamSetup/tt:Stream", NULL, &xmlwrapper_app_SetVer, NULL},
	{"soap:Envelope/soap:Body/trt:GetStreamUri/trt:StreamSetup/tt:Transport/tt:Protocol", NULL, &xmlwrapper_app_SetAuthor, NULL},
	{"soap:Envelope/soap:Body/trt:GetStreamUri/trt:ProfileToken", NULL, &xmlwrapper_app_SetUsage, NULL},
	{NULL, NULL, NULL, NULL}
};

TXmlWrapperNamespaceMap ptNSMap3[] = 
{	
	{"soap", "http://www.w3.org/2003/05/soap-envelope"},
	{"trt", "http://www.onvif.org/ver10/media/wsdl"},
	{"tt", "http://www.onvif.org/ver10/schema"},
	{NULL, NULL}
};

//============================================================================
SCODE xmlwrapper_app_Flow(void *userData, const CHAR* s, const CHAR** atts)
{		
	printf("%s : ", __func__);
	TAPPHandler *ptahHandler = (TAPPHandler *) userData;
	TAPPFlow *ptafFlow, **pptafFlow;

	if (*atts)
	{	
		if (strcmp("ver", atts[0]) == 0)
		{
			pptafFlow =  &(ptahHandler->ptafFlow);
			ptafFlow = *pptafFlow;			
			while(ptafFlow)
			{				
				// reloading
				if (strcmp(ptafFlow->szVer, atts[1]) == 0)
				{
					ptahHandler->hTmp = ptafFlow;
					printf("Ver:%s already exist!\n", ptafFlow->szVer);
					return S_OK;				
				}
				pptafFlow = &(ptafFlow->ptNext);
				ptafFlow = *pptafFlow;								
			}
			// New Flow 
			ptafFlow = malloc(sizeof(TAPPFlow));
			memset(ptafFlow, 0, sizeof(TAPPFlow));
			strncpy(ptafFlow->szVer,atts[1], strlen(atts[1]));
			*pptafFlow = ptafFlow;
			ptahHandler->hTmp = ptafFlow;
			printf("Ver:%s \n", ptafFlow->szVer);
			return S_OK;
		}
	}
	// Flow must have attribute 
	printf("No attribute Found!\n");
	return IGNORE_CHILD_CONFIG;
}

//============================================================================
void xmlwrapper_app_SetFirst(void *userData, const char *s, int len)
{
	CHAR szTmp[MAX_DATA_LENGTH];
	TAPPHandler *ptahHandler = (TAPPHandler *) userData;
	TAPPFlow *ptafFlow = (TAPPFlow *) ptahHandler->hTmp;

	printf("%s : ", __func__);
	
	memcpy(szTmp, s, len);	
	szTmp[len] = '\0';
	if (strcmp(ptafFlow->szFirst, szTmp) != 0)
	{		
		memcpy(ptafFlow->szFirst, szTmp, len);	
		ptafFlow->szFirst[len] = '\0';
		printf("%s\n", ptafFlow->szFirst);		
	}
	else 
	{
		printf("First not changed! \n");
	}
}

//============================================================================
void xmlwrapper_app_SetNewConf(void *userData, const char *s, int len)
{
	CHAR szTmp[MAX_DATA_LENGTH];
	TAPPHandler *ptahHandler = (TAPPHandler *) userData;
	TAPPFlow *ptafFlow = (TAPPFlow *) ptahHandler->hTmp;

	printf("%s : ", __func__);
	
	memcpy(szTmp, s, len);	
	szTmp[len] = '\0';
	if (strcmp(ptafFlow->szNewConf, szTmp))
	{
		memcpy(ptafFlow->szNewConf, szTmp, len);
		ptafFlow->szNewConf[len] = '\0';
		printf("%s \n", ptafFlow->szNewConf);
	}
	else 
	{
		printf("NewConf not changed! \n");
	}
}

//============================================================================	
void xmlwrapper_app_SetReloading(void *userData, const char *s, int len)
{
	CHAR szTmp[MAX_DATA_LENGTH];
	TAPPHandler *ptahHandler = (TAPPHandler *) userData;
	TAPPFlow *ptafFlow = (TAPPFlow *) ptahHandler->hTmp;

	printf("%s : ", __func__);
	
	memcpy(szTmp, s, len);	
	szTmp[len] = '\0';
	if (strcmp(ptafFlow->szReloading, szTmp))
	{
		memcpy(ptafFlow->szReloading, szTmp, len);
		ptafFlow->szReloading[len] = '\0';
		printf("%s \n", ptafFlow->szReloading);
	}
	else 
	{
		printf("Reloading not changed! \n");
	}
}

//============================================================================
void xmlwrapper_app_SetExit(void *userData, const char *s, int len)
{
	CHAR szTmp[MAX_DATA_LENGTH];
	TAPPHandler *ptahHandler = (TAPPHandler *) userData;
	TAPPFlow *ptafFlow = (TAPPFlow *) ptahHandler->hTmp;

	printf("%s : ", __func__);
	
	memcpy(szTmp, s, len);	
	szTmp[len] = '\0';
	if (strcmp(ptafFlow->szExit, szTmp))
	{
		memcpy(ptafFlow->szExit, szTmp, len);
		ptafFlow->szExit[len] = '\0';
		printf("%s \n", ptafFlow->szExit);
	}
	else 
	{
		printf("Exit not changed! \n");
	}
}

//============================================================================
void xmlwrapper_app_SetVer(void *userData, const char *s, int len)
{
	CHAR szTmp[MAX_DATA_LENGTH];
	TAPPHandler *ptahHandler = (TAPPHandler *) userData;
	TAPPInfo *ptaiInfo = (TAPPInfo *) ptahHandler->ptaiInfo;

	printf("%s : ", __func__);
	
	memcpy(szTmp, s, len);	
	szTmp[len] = '\0';
	if (strcmp(ptaiInfo->szVer, szTmp))
	{
		memcpy(ptaiInfo->szVer, szTmp, len);
		ptaiInfo->szVer[len] = '\0';
		printf("%s \n", ptaiInfo->szVer);
	}
	else 
	{
		printf("Version not changed! \n");
	}
}

//============================================================================
void xmlwrapper_app_SetAuthor(void *userData, const char *s, int len)
{
	CHAR szTmp[MAX_DATA_LENGTH];
	TAPPHandler *ptahHandler = (TAPPHandler *) userData;
	TAPPInfo *ptaiInfo = (TAPPInfo *) ptahHandler->ptaiInfo;

	printf("%s : ", __func__);
	
	memcpy(szTmp, s, len);	
	szTmp[len] = '\0';
	if (strcmp(ptaiInfo->szAuthor, szTmp))
	{
		memcpy(ptaiInfo->szAuthor, szTmp, len);
		ptaiInfo->szAuthor[len] = '\0';
		printf("%s \n", ptaiInfo->szAuthor);
	}
	else 
	{
		printf("Author not changed! \n");
	}
}

//============================================================================
void xmlwrapper_app_SetDate(void *userData, const char *s, int len)
{
	CHAR szTmp[MAX_DATA_LENGTH];
	TAPPHandler *ptahHandler = (TAPPHandler *) userData;
	TAPPInfo *ptaiInfo = (TAPPInfo *) ptahHandler->ptaiInfo;

	printf("%s : ", __func__);
	
	memcpy(szTmp, s, len);	
	szTmp[len] = '\0';
	if (strcmp(ptaiInfo->szDate, szTmp))
	{
		memcpy(ptaiInfo->szDate, szTmp, len);
		ptaiInfo->szDate[len] = '\0';
		printf("%s \n", ptaiInfo->szDate);
	}
	else 
	{
		printf("Date not changed! \n");
	}
}

//============================================================================
void xmlwrapper_app_SetUsage(void *userData, const char *s, int len)
{
	CHAR szTmp[MAX_DATA_LENGTH];
	TAPPHandler *ptahHandler = (TAPPHandler *) userData;
	TAPPInfo *ptaiInfo = (TAPPInfo *) ptahHandler->ptaiInfo;

	printf("%s : ", __func__);
	
	memcpy(szTmp, s, len);	
	szTmp[len] = '\0';
	if (strcmp(ptaiInfo->szUsage, szTmp))
	{
		memcpy(ptaiInfo->szUsage, szTmp, len);
		ptaiInfo->szUsage[len] = '\0';
		printf("%s \n", ptaiInfo->szUsage);
	}
	else 
	{
		printf("Usage not changed! \n");
	}
}



