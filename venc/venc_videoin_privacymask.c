/*
 *******************************************************************************
 * $Header: $
 *
 *  Copyright (c) 2007-2010 VN Inc. All rights reserved.
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
 *  | VN INC.                                                     |
 *  +-----------------------------------------------------------------+
 *
 * $History: $
 *
 *******************************************************************************
 */

/*!
 *******************************************************************************
 * Copyright 2011-2015 VN, Inc. All rights reserved.
 *
 * \file
 * venc_videoin_privacymask.c
 *
 * \brief
 * privacy mask function implementation.
 *
 * \date
 * 2012/07/19
 *
 * \author
 * Jeff Liao
 *
 *******************************************************************************
 */
#include "venc_videoin_local.h"
#if defined(_VMA_IBPE)	

static void SetPNGFilePath(void *userData, const char *s, int len)
{
	char *szPNGFilePath = (char *)userData;
	CHAR szBitmapPath[len+1];
	memset(szBitmapPath, 0x0, sizeof(szBitmapPath));
	snprintf(szBitmapPath, len+1, "%s", s);
	strncpy(szPNGFilePath, szBitmapPath, strlen(szBitmapPath));
	return;
}

static SCODE ExtractPNGFilePath(char *szFilePath, const char *szConfigFile)
{
	
	int sRet;
	HANDLE hXmlWrapObj;	
	TXmlWrapperTreeMap ptParsingMap[] = 
	{
		{"root/venc_videoin/pmask/file_path", NULL, &SetPNGFilePath, NULL},
		{NULL, NULL, NULL}
	};
	TXmlWrapperInitOptions tInitOptions;
	memset(&tInitOptions, 0x0, sizeof(TXmlWrapperInitOptions));

	if (szFilePath == NULL)
	{
		printf("%s %d :szFilePath is NULL! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	if (szConfigFile == NULL)
	{
		printf("%s %d :szConfigFile is NULL! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	tInitOptions.dwVersion = XMLWRAPPER_VERSION;
	if (XmlWrapper_Initial(&hXmlWrapObj, &tInitOptions) != S_OK)
	{
		return S_FAIL;
	}
	if (XmlWrapper_SetHandler(hXmlWrapObj, ptParsingMap, NULL) != S_OK)
	{
		printf("%s %d : XmlWrapper_SetHandler Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	if ((sRet = XmlWrapper_ReadFile_UsrDefFunc(szConfigFile, hXmlWrapObj, szFilePath)) != S_OK)
	{
		printf("%s %d : XmlWrapper_ReadBuf_UsrDefFunc Fail! %x\n", __FILE__, __LINE__, sRet);
		return S_FAIL;
	}
	if (XmlWrapper_Release(&hXmlWrapObj) != S_OK)
	{
		printf("%s %d : Release XmlWrapper Fail! \n", __FILE__, __LINE__);
		return S_FAIL;
	}
	return S_OK;
	
}

static SCODE ExtractPNGResolution(DWORD *pdwWidth, DWORD *pdwHeight, const char *szConfigFile)
{
	CHAR szPNGFilePath[1024];
	
	memset(szPNGFilePath, 0x0, sizeof(szPNGFilePath));
	
	*pdwWidth = 0;
	*pdwHeight = 0;
	
	if (ExtractPNGFilePath(szPNGFilePath, szConfigFile) == S_FAIL)
	{
		return S_FAIL;
	}
	else
	{
		FILE *fp = NULL;
		unsigned char png_signature[] = {137, 80, 78, 71, 13, 10, 26, 10};
		unsigned char ihdr_name[] = "IHDR";
		unsigned char szWidth[4];
		unsigned char szHeight[4];
		char szSignature[8];
		char szIHDR[4];
	
		fp = fopen(szPNGFilePath,"rb");
	
		if (fp != NULL)
		{
			fread(szSignature, 1, 8, fp);
			if (!memcmp(szSignature, png_signature, 8)) {
				// this is  a PNG !
				fseek(fp, 8+4, SEEK_SET); 
				fread(szIHDR, 1, 4, fp);
				rewind (fp);
				if (!memcmp(szIHDR, ihdr_name, 4)) {
					// Is an IHDR chunk, invalid PNG file!
					fseek(fp, 8+4+4, SEEK_SET);
					fread(szWidth, 1, 4, fp);
					rewind (fp);
					fseek(fp, 8+4+4+4, SEEK_SET);
					fread(szHeight, 1, 4, fp);
					rewind (fp);
					
					*pdwWidth = (szWidth[3]<<0) | (szWidth[2]<<8) | (szWidth[1]<<16) | (szWidth[0]<<24);
					*pdwHeight = (szHeight[3]<<0) | (szHeight[2]<<8) | (szHeight[1]<<16) | (szHeight[0]<<24);
				}
			}
			fclose(fp);
		}
		
	}
	return S_OK;
}

SCODE VencVideoIn_InitIBPEPMask(HANDLE hIBPEPMask, char *szConfigFile)
{
	TIBPEPMask	*pThis = (TIBPEPMask *)hIBPEPMask;
	TMemMgrState	tMemMgrState;
	DWORD dwPNGWidth = 0;
	DWORD dwPNGHeight = 0;
	
	if (ExtractPNGResolution(&dwPNGWidth, &dwPNGHeight, szConfigFile) != S_OK)
	{
		printf("[%s:%s:%d] Extract resolution of PNG file failed!!\n", __func__, __FILE__, __LINE__);
		return S_FAIL;
	}
	else
	{
		printf("[%s:%s:%d] Extract resolution of PNG file OK!! (w,h) = (%d, %d)\n", __func__, __FILE__, __LINE__,dwPNGWidth,dwPNGHeight);
	}
	
	if ((dwPNGHeight == 0) || (dwPNGWidth == 0))
	{
		printf("[%s:%s:%d] The resolution of PNG file is zero!!\n", __func__, __FILE__, __LINE__);
		return S_FAIL;
	}
	
	//Allocate privacy mask buffer for IBPE. 1 bit in the mask buffer corresponds to 2x2 pixels. 
	{
		pThis->dwPMaskWidth 	= (pThis->dwWidth + 1) >> 1;
		pThis->dwPMaskHeight 	= (pThis->dwHeight + 1) >> 1;
		pThis->dwPMaskStride 	= IBPE_GET64MULTIPLE(pThis->dwPMaskWidth);
		pThis->dwPMaskSz 		= (pThis->dwPMaskStride * pThis->dwPMaskHeight) >> 3;
		
		// allocate privacy mask buffer
		memset(&tMemMgrState, 0x0, sizeof(TMemMgrState));
		tMemMgrState.dwBusNum = 0;
		tMemMgrState.dwSize = pThis->dwPMaskSz;
		tMemMgrState.eAlignType	= ALIGN_TYPE_8_BYTE;
		pThis->pbyPMask = (BYTE *)MemMgr_GetMemory(pThis->hMemMgrObj, &tMemMgrState);
		if (pThis->pbyPMask == NULL)
		{
			printf("[%s:%s:%d] MemMgr_GetMemory error!\n", __func__, __FILE__, __LINE__);
			return S_FAIL;
		}
		memset(pThis->pbyPMask, 0, pThis->dwPMaskSz);
	}
	
	//Allocate BitMask buffer for the external bitmask file (ex: PNG file, raw bitmask file)
	{
		DWORD dwBitMaskSz, dwBitMaskStride;

		dwBitMaskStride	= IBPE_GET8MULTIPLE(dwPNGWidth);
		dwBitMaskSz		= (dwBitMaskStride*dwPNGHeight) >> 3;
		memset(&tMemMgrState, 0x0, sizeof(TMemMgrState));
		tMemMgrState.dwBusNum = 0;
		tMemMgrState.dwSize = dwBitMaskSz;
		tMemMgrState.eAlignType	= ALIGN_TYPE_8_BYTE;

		pThis->pbyBitMask = (BYTE *)MemMgr_GetMemory(pThis->hMemMgrObj, &tMemMgrState);
		if (pThis->pbyBitMask == NULL)
		{
			printf("[%s:%s:%d] MemMgr_GetMemory error!\n", __func__, __FILE__, __LINE__);
			return S_FAIL;
		}
		memset(pThis->pbyBitMask, 0, dwBitMaskSz);
	}

	// initialize BitMaskResizer
	{
		TBitMaskResizerInitOptions tBMaskInitOptions;

		tBMaskInitOptions.dwVersion = BITMASKRESIZER_VERSION;
		tBMaskInitOptions.pObjectMem = NULL;
		if (BitMaskResizer_Initial(&pThis->hBMaskObj, &tBMaskInitOptions) != S_OK) 
		{
			printf("Initialize bit-mask-resize object fail !!\n");
			return S_FAIL;
		}
	}

	// initialize png
	{
		TPNGStruct *ptPNGStruct;
		int iHeightIndex = 0;
		if ((ptPNGStruct = (TPNGStruct *)malloc(sizeof(TPNGStruct))) == NULL)
		{
			printf("[%s:%s:%d] malloc error!\n", __func__, __FILE__, __LINE__);
			return S_FAIL;
		}
		{
			ptPNGStruct->row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * dwPNGHeight);
			if (ptPNGStruct->row_pointers != NULL)
			{
				memset(ptPNGStruct->row_pointers, 0x0, sizeof(png_bytep) * dwPNGHeight);
			}
			else
			{
				printf("[%s:%s:%d] malloc error!\n", __func__, __FILE__, __LINE__);
				return S_FAIL;
			}
			
			ptPNGStruct->pbyPNGBuffer = (png_byte *) malloc(sizeof(png_byte) * dwPNGHeight * dwPNGWidth * 4);
			if (ptPNGStruct->pbyPNGBuffer != NULL)
			{
				memset(ptPNGStruct->pbyPNGBuffer, 0x0, sizeof(png_byte) * dwPNGHeight * dwPNGWidth * 4);
				for (iHeightIndex = 0; iHeightIndex < dwPNGHeight; iHeightIndex++)
				{
					ptPNGStruct->row_pointers[iHeightIndex] = (png_byte*) (ptPNGStruct->pbyPNGBuffer + (sizeof(png_byte) * iHeightIndex * dwPNGWidth *4));
				}
			}
			else
			{
				printf("[%s:%s:%d] malloc error!\n", __func__, __FILE__, __LINE__);
				return S_FAIL;
			}
		}

		pThis->ptPNGStruct = ptPNGStruct;
	}

	return S_OK;
}

SCODE VencVideoIn_IBPEPNG2BitMask(TIBPEPMask* pThis, FILE* pfInputPMask, unsigned int* pWidth, unsigned int* pHeight, unsigned int* pStride)
{
	TPNGStruct	*ptPNGStruct;
	char 		aszHeader[8];
	int 		iHeightIndex, iWidthIndex;
	int 		iBitIndex;
	BYTE 		*pbyTemp;

	ptPNGStruct = pThis->ptPNGStruct;
	// open file and test for it being a png
	if ((ptPNGStruct->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL)
	{
		printf("[%s:%s:%d]:png_create_read_struct failed !!\n", __func__, __FILE__, __LINE__);
		return S_FAIL;
	}
	if ((ptPNGStruct->info_ptr = png_create_info_struct(ptPNGStruct->png_ptr)) == NULL)
	{
		printf("[%s:%s:%d]:png_create_info_struct failed !!\n", __func__, __FILE__, __LINE__);
		png_destroy_read_struct(&ptPNGStruct->png_ptr, &ptPNGStruct->info_ptr, NULL);
		return S_FAIL;
	}

	fread(aszHeader, 1, 8, pfInputPMask);
	if (png_sig_cmp((png_bytep)aszHeader, 0, 8) != 0)
	{
		printf("[%s:%s:%d]:It is not recognized as a PNG file !!\n", __func__, __FILE__, __LINE__);
		png_destroy_read_struct(&ptPNGStruct->png_ptr, &ptPNGStruct->info_ptr, NULL);
		return S_FAIL;
	}

	png_init_io(ptPNGStruct->png_ptr, pfInputPMask);
	png_set_sig_bytes(ptPNGStruct->png_ptr, 8);

	png_read_info(ptPNGStruct->png_ptr, ptPNGStruct->info_ptr);

	//dwPassesNumber = png_set_interlace_handling(pThis->tPNGStruct.png_ptr);
	//png_read_update_info(ptPNGStruct->png_ptr, ptPNGStruct->info_ptr);

	if (png_get_color_type(ptPNGStruct->png_ptr, ptPNGStruct->info_ptr) != PNG_COLOR_TYPE_RGBA) {
		printf("[%s:%s:%d]:color_type of input file must be PNG_COLOR_TYPE_RGBA \n", __func__, __FILE__, __LINE__);
		png_destroy_read_struct(&ptPNGStruct->png_ptr, &ptPNGStruct->info_ptr, NULL);
		return S_FAIL;
	}
	
	png_read_image(ptPNGStruct->png_ptr, ptPNGStruct->row_pointers);

	*pWidth = png_get_image_width(ptPNGStruct->png_ptr, ptPNGStruct->info_ptr);
	*pHeight = png_get_image_height(ptPNGStruct->png_ptr, ptPNGStruct->info_ptr);
	*pStride = IBPE_GET8MULTIPLE(*pWidth);

	if ((*pWidth > pThis->dwWidth) || (*pHeight > pThis->dwHeight))
	{
		printf("[%s:%s:%d]:Width must be smaller than %d; Height must be smaller than %d!!\n", __func__, __FILE__, __LINE__, pThis->dwWidth, pThis->dwHeight);
		png_destroy_read_struct(&ptPNGStruct->png_ptr, &ptPNGStruct->info_ptr, NULL);
		return S_FAIL;
	}

	memset(pThis->pbyBitMask, 0, (((*pWidth) * (*pHeight)) >> 3));
	pbyTemp = pThis->pbyBitMask;
	for (iHeightIndex = 0; iHeightIndex < *pHeight; ++iHeightIndex) 
	{
		png_byte *pbRow = ptPNGStruct->row_pointers[iHeightIndex];
		iBitIndex = 8; // reset to 8
		for (iWidthIndex = 0; iWidthIndex < *pStride; ++iWidthIndex) 
		{
			if (iWidthIndex < *pWidth)
			{
				png_byte *pbPtr = &(pbRow[iWidthIndex*4]);
				/*					
									printf("Pixel at position [ %d - %d ] has the following RGBA values: %d - %d - %d - %d\n",
									iWidthIndex, iHeightIndex, ptr[0], ptr[1], ptr[2], ptr[3]);
				 */			// ptr[3] is alpha value
				*pbyTemp += (((pbPtr[3] != 0x0)? 1 : 0) << --iBitIndex);
			}
			else
			{
				--iBitIndex;
			}
			if (iBitIndex == 0) 
			{	// if iBitIndex == 0 means that it goes through 8 byte.
				pbyTemp++;
				iBitIndex = 8;
			}
		}
	}
	png_read_end(ptPNGStruct->png_ptr, ptPNGStruct->info_ptr);
	png_destroy_read_struct(&ptPNGStruct->png_ptr, &ptPNGStruct->info_ptr, NULL);

	return S_OK;
}

SCODE VencVideoIn_IBPERawBitMask(TIBPEPMask* pThis, FILE* pfInputPMask, unsigned int* pWidth, unsigned int* pHeight, unsigned int* pStride)
{
	unsigned int file_size;
	unsigned int rnt;

	*pWidth = pThis->dwWidth;
	*pHeight = pThis->dwHeight;
	*pStride = IBPE_GET8MULTIPLE(*pWidth);
	file_size = ((*pStride) * pThis->dwHeight) >> 3;

	rnt = fread(pThis->pbyBitMask, file_size, 1, pfInputPMask);

	if (rnt == file_size);
		return S_OK;

	printf("[%s:%s:%d]:Read raw bitmask file fail !!\n", __func__, __FILE__, __LINE__);
	return S_FAIL;
}

SCODE VencVideoIn_IBPEReadBitMask(HANDLE hIBPEPMask, CHAR *szFilePath)
{
	TIBPEPMask	*pThis;
	FILE 		*pfInputPMask;
	unsigned int dwWidth, dwHeight, dwStride;
	SCODE		sRet;

	// open file and test for it being a png
	if ((pfInputPMask = fopen(szFilePath, "r")) == NULL)
	{
		printf("[%s:%s:%d]:Open PrivacyMask bitmask file %s fail !!\n", __func__, __FILE__, __LINE__, szFilePath);
		return S_FAIL;
	}

	pThis = (TIBPEPMask *)hIBPEPMask;

	if (strstr(szFilePath, "png"))
		sRet = VencVideoIn_IBPEPNG2BitMask(pThis, pfInputPMask, &dwWidth, &dwHeight, &dwStride);
	else
		sRet = VencVideoIn_IBPERawBitMask(pThis, pfInputPMask, &dwWidth, &dwHeight, &dwStride);

	fclose(pfInputPMask);

	if (sRet != S_OK)
		return S_FAIL;

	{
		TBitMaskResizerState tBMaskState;

		// from original bitmask to Real Y mask
		tBMaskState.dwInWidth = dwWidth;
		tBMaskState.dwInHeight = dwHeight;
		tBMaskState.dwInStride = dwStride;
		tBMaskState.pbyInBitMask = pThis->pbyBitMask;

		tBMaskState.dwOutWidth = pThis->dwPMaskWidth;
		tBMaskState.dwOutHeight = pThis->dwPMaskHeight;
		tBMaskState.dwOutStride = pThis->dwPMaskStride;
		tBMaskState.pbyOutBitMask = pThis->pbyPMask;

		// Check the state parameters
		/*if (BitMaskResizer_CheckParam(pThis->hBMaskObj, &tBMaskState) != S_OK) {
			printf("[%s:%s:%d]:BitMaskResizer State parameters setting error !!\n", __func__, __FILE__, __LINE__);
			printf("Error code = 0x%08X\n", BitMaskResizer_CheckParam(pThis->hBMaskObj, &tBMaskState));
			return S_FAIL;
		}*/
		// Start processing
		if (BitMaskResizer_ProcessOneFrame(pThis->hBMaskObj, &tBMaskState) != S_OK) {
			printf("[%s:%s:%d]:BitMaskResizer ProcessOneFrame error !!\n", __func__, __FILE__, __LINE__);
			return S_FAIL;
		}

	}
	return S_OK;
}

SCODE VencVideoIn_ReleaseIBPEPMask(HANDLE hIBPEPMask)
{
	TIBPEPMask	*pThis;
	TPNGStruct	*ptPNGStruct;
	pThis = (TIBPEPMask *)hIBPEPMask;
	ptPNGStruct = pThis->ptPNGStruct;

	if (ptPNGStruct->pbyPNGBuffer != NULL)
	{
		free(ptPNGStruct->pbyPNGBuffer);
		ptPNGStruct->pbyPNGBuffer = NULL;
	}
	if (ptPNGStruct->row_pointers != NULL) 
	{
		free(ptPNGStruct->row_pointers);
		ptPNGStruct->row_pointers = NULL;
	}
	if (pThis->hMemMgrObj != NULL)
	{ 
		if (pThis->pbyPMask != NULL)
		{
			if (MemMgr_FreeMemory(pThis->hMemMgrObj, (DWORD)pThis->pbyPMask) != S_OK)
			{
				printf("Release IBPE Privacy Mask buffer failed!!\n ");
			}
		}
		if (pThis->pbyBitMask != NULL)
		{
			if (MemMgr_FreeMemory(pThis->hMemMgrObj, (DWORD)pThis->pbyBitMask) != S_OK)
			{
				printf("Release IBPE Privacy bit mask buffer failed!!\n ");
			}
		}
	}
	/* Release bit-mask-resize object */
	if (pThis->hBMaskObj != NULL)
	{
		if (BitMaskResizer_Release(&pThis->hBMaskObj) != S_OK)
		{
			printf("Release bit-mask-resize object fail !!\n");
		}
	}
	return S_OK;
}
#else
SCODE VencVideoIn_InitIBPEPMask(HANDLE hIBPEPMask)
{
	return S_OK;
}

SCODE VencVideoIn_IBPEPNG2BitMask(HANDLE hIBPEPMask, CHAR *szFilePath)
{
	return S_OK;
}

SCODE VencVideoIn_ReleaseIBPEPMask(HANDLE hIBPEPMask)
{
	return S_OK;
}
#endif
