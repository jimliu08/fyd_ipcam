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
 * xmlmgr_app.c
 *
 * \brief
 * XmlMgr example application
 *
 * \date
 * 2006/10/25
 *
 * \author
 * Rey Cheng
 *
 *******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "xmlmgr.h"

int main(int argc, char *argv[])
{
	char				*szFileName="xmlmgr_example.xml";
	HANDLE				hXmlConf;
	TXmlMgrInitOptions	tInitOpt;

	/* initial */
	tInitOpt.dwVersion = XMLMGR_VERSION;
	if (XmlMgr_Initial(&hXmlConf, &tInitOpt) != S_OK)
	{
		fprintf(stderr, "Initial fail\n");
		exit(1);
	}
	/* read file */
	if (XmlMgr_ReadFile(hXmlConf, szFileName) != S_OK)
	{
		fprintf(stderr, "Open XML file %s fail\n", szFileName);
		exit(1);
	}
	/* get config */
	{
		char szValue0[32];
		char szValue1[32];
		char szValue2[32];
		char szValue3[32];
		char szValue4[32];

		TXmlMgrParsingMap atParsingMap[] = {	
			{"/xmlmgr_example/no_attr", xmlmgr_set_string, szValue0},
			{"/xmlmgr_example/has_attr/one_attr[@id='0']", xmlmgr_set_string, szValue1},
			{"/xmlmgr_example/has_attr/one_attr[@id='1']", xmlmgr_set_string, szValue2},
			{"/xmlmgr_example/has_attr/two_attr[@id='2' subid='0']", xmlmgr_set_string, szValue3},
			{"/xmlmgr_example/has_attr/two_attr[@id='2' subid='1']", xmlmgr_set_string, szValue4},
			{NULL, 0, NULL}
		};
		XmlMgr_GetMultiConfValue(hXmlConf, atParsingMap);

		fprintf(stderr, "%s %d : %s\n", __FILE__, __LINE__, szValue0);
		fprintf(stderr, "%s %d : %s\n", __FILE__, __LINE__, szValue1);
		fprintf(stderr, "%s %d : %s\n", __FILE__, __LINE__, szValue2);
		fprintf(stderr, "%s %d : %s\n", __FILE__, __LINE__, szValue3);
		fprintf(stderr, "%s %d : %s\n", __FILE__, __LINE__, szValue4);

		
		fprintf(stderr, "%s %d : %s\n", __FILE__, __LINE__, XmlMgr_GetConfValue(hXmlConf, "/xmlmgr_example/no_attr"));
		fprintf(stderr, "%s %d : %s\n", __FILE__, __LINE__, XmlMgr_GetConfValue(hXmlConf, "/xmlmgr_example/has_attr/one_attr[@id='0']"));
		fprintf(stderr, "%s %d : %s\n", __FILE__, __LINE__, XmlMgr_GetConfValue(hXmlConf, "/xmlmgr_example/has_attr/one_attr[@id='1']"));
		fprintf(stderr, "%s %d : %s\n", __FILE__, __LINE__, XmlMgr_GetConfValue(hXmlConf, "/xmlmgr_example/has_attr/two_attr[@id='2' subid='0']"));
		fprintf(stderr, "%s %d : %s\n", __FILE__, __LINE__, XmlMgr_GetConfValue(hXmlConf, "/xmlmgr_example/has_attr/two_attr[@id='2' subid='1']"));
	}
	{
		const char *aszElem[5] = 
		{
			"/xmlmgr_example/no_attr", 
			"/xmlmgr_example/has_attr/one_attr[@id='0']",
			"/xmlmgr_example/has_attr/one_attr[@id='1']",
			"/xmlmgr_example/has_attr/two_attr[@id='2' subid='0']",
			"/xmlmgr_example/has_attr/two_attr[@id='2' subid='1']"
		};
		const char *aszVal[5] = 
		{
			"value00",
			"value01",
			"value02",
			"value03",
			"value04"
		};
		XmlMgr_SetMultiConfValue(hXmlConf, 5, aszElem, aszVal);
		/* write back */
		if (XmlMgr_WriteFile(hXmlConf, "/tmp/xmlmgr_example2.xml") != S_OK)
		{
			fprintf(stderr, "Write XML file %s fail\n", "/tmp/xmlmgr_example2.xml");
			exit(1);			
		}

		
		XmlMgr_SetConfValue(hXmlConf, "/xmlmgr_example/no_attr", "value000");
		XmlMgr_SetConfValue(hXmlConf, "/xmlmgr_example/has_attr/one_attr[@id='0']", "value001");
		XmlMgr_SetConfValue(hXmlConf, "/xmlmgr_example/has_attr/one_attr[@id='1']", "value002");
		XmlMgr_SetConfValue(hXmlConf, "/xmlmgr_example/has_attr/two_attr[@id='2' subid='0']", "value003");
		XmlMgr_SetConfValue(hXmlConf, "/xmlmgr_example/has_attr/two_attr[@id='2' subid='1']", "value004");

		/* write back */
		if (XmlMgr_WriteFile(hXmlConf, "/tmp/xmlmgr_example3.xml") != S_OK)
		{
			fprintf(stderr, "Write XML file %s fail\n", "/tmp/xmlmgr_example3.xml");
			exit(1);			
		}
	}

	/* release */
	XmlMgr_Release(&hXmlConf);

	return 0;
}
