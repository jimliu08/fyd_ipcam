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
 * Copyright 2007-2010 VN, Inc. All rights reserved.
 *
 * \file
 * vencslave_txtoverlay.c
 *
 * \brief
 * Text overlay of venc
 *
 * \date
 * 2008/04/09
 *
 * \author
 * Cheng-Ching Huang
 *
 *******************************************************************************
 */

#include "vencslave_txtoverlay.h"

/* ========================================================================= */
#ifdef TEXT_OVERLAY
SCODE VencSlave_TextOverlay_Initial(TTextOverlayOptions *pData, const DWORD dwVideoMaxWidth)
{
	FT_Error	ftErr;

	ftErr = FT_Init_FreeType(&pData->ftLib);              /* initialize library */
	if (ftErr != 0) {
		fprintf(stderr, "FT_Init_FreeType failed\n");
		pData->ftLib = NULL;
		pData->pbyTextBitMap = NULL;
	} else {
		pData->ftFace = NULL;
		/* May allocate this buffer by memmgr. */
		pData->pbyTextBitMap = 
			malloc(dwVideoMaxWidth * MAX_OVERLAYED_TEXT_HEIGHT);
		if (pData->pbyTextBitMap == NULL) {
			FT_Done_FreeType(pData->ftLib);
			pData->ftLib = NULL;
			fprintf(stderr, "malloc memory for text overlay failed!\n");
		}
	}	

	pData->dwCurrentSecond = 0;
	return S_OK;
}
void VencSlave_TextOverlay_Release(TTextOverlayOptions *pData)
{
	FT_Int      ftInt;
    for (ftInt = 0; ftInt < MAX_CHAR_NUM; ftInt++) {
        FT_Bitmap_Done(pData->ftLib, &pData->BitmapRec[ftInt].bitmap);
    }
	FT_Done_Face(pData->ftFace);
	FT_Done_FreeType(pData->ftLib);
	if (pData->pbyTextBitMap)
		free(pData->pbyTextBitMap);
}
long GetUnicodeCharacter(const unsigned char *text,int *length)
{
	unsigned long c;

	if (*length < 1)
		return(-1);
	c = text[0];
	if ((c & 0x80) == 0)
	{
		*length = 1;
		return((long) c);
	}
	if ((*length < 2) || ((text[1] & 0xc0) != 0x80))
	{
		*length = 0;
		return(-1);
	}
	if ((c & 0xe0) != 0xe0)
	{
		*length = 2;
		c = (text[0] & 0x1f) << 6;
		c|=text[1] & 0x3f;
		return((long) c);
	}
	if ((*length < 3) || ((text[2] & 0xc0) != 0x80))
	{
		*length = 0;
		return(-1);
	}
	if ((c & 0xf0) != 0xf0)
	{
		*length = 3;
		c = (text[0] & 0xf) << 12;
		c|=(text[1] & 0x3f) << 6;
		c|=text[2] & 0x3f;
		return((long) c);
	}
	if ((*length < 4) || ((c & 0xf8) != 0xf0) || ((text[3] & 0xc0) != 0x80))
	{
		*length = 0;
		return(-1);
	}
	*length = 4;
	c = (text[0] & 0x7) << 18;
	c|=(text[1] & 0x3f) << 12;
	c|=(text[2] & 0x3f) << 6;
	c|=text[3] & 0x3f;
	return((long) c);
}
SCODE VencSlave_TextOverlay_EncodeUnicode(const char *text_ptr,
		WORD *unicodechar_ptr)
{
	long
		c_ptr;

	const char
		*p_txt;

	unsigned short
		*q_buf;

	int
		char_length;


	int counter = 0;
	if ((text_ptr == (char *) NULL) || (*text_ptr == '\0'))
		return((SCODE ) NULL);


	char_length = strlen(text_ptr);

	q_buf = unicodechar_ptr;
	for (p_txt = text_ptr; *p_txt != '\0'; p_txt += char_length)
	{
		char_length = strlen(p_txt);
		c_ptr = GetUnicodeCharacter((const unsigned char *) p_txt,&char_length);
		if (c_ptr < 0)
		{
			q_buf = unicodechar_ptr;
			for (p_txt = text_ptr; *p_txt != '\0'; p_txt++)
				*q_buf++=(unsigned char) *p_txt;
			break;
		}
		*q_buf = (unsigned short) c_ptr;
		q_buf++;
	}
	counter = q_buf-unicodechar_ptr;
	return(counter);
}

static SCODE vencslave_gen_bitmap(WORD c, BITMAP_REC *pBitmapRec, FT_Face  ftFace,FT_Library	ftLib)
{
	FT_Error	ftErr;
	FT_GlyphSlot  slot;
	if ((ftFace != NULL) && (ftLib != NULL))
	{
		ftErr = FT_Load_Char(ftFace, c, FT_LOAD_RENDER);
		if (ftErr) {
			fprintf(stderr, "FT_Load_Char error! (%d) unicode(dec) \"%d\"\n", ftErr, c);
			return S_FAIL;	
		}
		FT_Bitmap_New(&pBitmapRec->bitmap);
		slot = ftFace->glyph;
		pBitmapRec->bitmap_left = slot->bitmap_left;
		pBitmapRec->bitmap_top = slot->bitmap_top;
		pBitmapRec->horiAdvance = slot->metrics.horiAdvance;
		FT_Bitmap_Convert(ftLib, &slot->bitmap, &pBitmapRec->bitmap, 1);		
	}
	return S_OK;
}

SCODE VencSlave_TextOverlay_SetOptions(TTextOverlayOptions *pData, TTextOverlayOptions *pOptions)
{
	FT_Error	ftErr;
	FT_Int      ftInt;
//    FT_GlyphSlot  slot;

	memcpy(pData->szCustomText, pOptions->szCustomText, MAX_CUSTOM_TEXT_LEN + 1);
	pData->bMilliSecEnable = pOptions->bMilliSecEnable;

	if (pData->ftLib) {
		if (pData->ftFace) {
			FT_Done_Face(pData->ftFace);
            for (ftInt = 0; ftInt < MAX_CHAR_NUM; ftInt++) {
                FT_Bitmap_Done(pData->ftLib, &pData->BitmapRec[ftInt].bitmap);
            }
		}
		ftErr = FT_New_Face(pData->ftLib, 
				pOptions->szFontPath, 
				0, 
				&pData->ftFace); /* create face object */
		if (ftErr != 0) {
			pData->ftFace = NULL;
			pData->bTextOverlayEnable = FALSE;
			printf("%s:%d: FT_New_Face failed.\n", __FILE__, __LINE__);
		} else {
  			FT_Face  face = pData->ftFace;

			pData->dwBitMapStride = 0; 
			pData->bTextOverlayEnable = pOptions->bTextOverlayEnable;
			pData->sdwNominalHeight = 24;

			/* we usually use the font-file that only contains one size of font. 
			   It needs to be modified if we want to support ttf or something fancier. */
			if (face->face_flags & FT_FACE_FLAG_FIXED_SIZES) {
			
				pData->sdwNominalHeight = 
					face->available_sizes[face->num_fixed_sizes-1].y_ppem >> 6;	
			}
			/* the line-height is twice of the nominal height. 
			   you may modify this to fit your requirement. */
			pData->sdwLineHeight = pData->sdwNominalHeight << 1;
			pData->sdwOrigin = pData->sdwNominalHeight + (pData->sdwNominalHeight >> 1);
			pData->sdwLeftMargin = pData->sdwNominalHeight >> 1;
			pData->sdwRightMargin = pData->sdwLeftMargin;


			ftErr = FT_Select_Charmap(pData->ftFace, FT_ENCODING_UNICODE );
			if (ftErr != 0) {
                        pData->ftFace = NULL;
                        pData->bTextOverlayEnable = FALSE;
                        printf("%s:%d: FT_New_Face failed.\n", __FILE__, __LINE__);
			}
			ftErr = FT_Set_Char_Size( pData->ftFace, 24 * 64, 0, 100, 0 ); /* FIXME set character size to 24 by default for TTF fonts. Later modify ttf font size through web UI */
			
			
            for (ftInt = 0; ftInt < MAX_CHAR_NUM; ftInt++) {
				if (vencslave_gen_bitmap(ftInt, pData->BitmapRec + ftInt, pData->ftFace, pData->ftLib) != S_OK)
				{
					fprintf(stderr, "FT_Load_Char error! (%d) ASCII(dec) \"%d\"\n", ftErr, ftInt);
					pData->bTextOverlayEnable = FALSE;
					return 0;
				}
			}
		}
	} else {
		pData->bTextOverlayEnable = FALSE;
	}
	return S_OK;
}

/*! return advance */
static SDWORD VencSlave_TextOverlay_Kernel(TTextOverlayOptions *pData,  WORD c, 
		SDWORD sdwStartPos, DWORD dwStride)
{
    int iChar = (int)c;
    FT_Bitmap *pftBitmap;

	/* These two implements are the same, 
	   the latter one is the optimized version but hard to understand. 
	   Thus I keep the crap but easy to read code here. 
	 */
    if ((iChar >= 0) &&  (iChar <= 127))
	{
		FT_Int m, n, area;
		BYTE	*pbyDestBitmap, *pbySrcBitmap;
        pftBitmap = &pData->BitmapRec[iChar].bitmap;
        
		if ((sdwStartPos + pData->BitmapRec[iChar].bitmap_left + pData->BitmapRec[iChar].bitmap.width) > dwStride) {
			fprintf(stderr, "[vencslave_txtoverlay](%d)The text string is too long to put in this video.\n",__LINE__);
			return 0;
		}
		
		pbyDestBitmap = pData->pbyTextBitMap 
			+ (pData->sdwOrigin - pData->BitmapRec[iChar].bitmap_top) * dwStride /* yMin * stride */
			+ sdwStartPos + pData->BitmapRec[iChar].bitmap_left; 					/* xMin */
		pbySrcBitmap = pftBitmap->buffer;
		area = pftBitmap->rows * pftBitmap->width;
		for (m = 0, n = 0; n < area; m++, n ++) {
			if (m == pftBitmap->width) {
				pbyDestBitmap += dwStride;
				m = 0;
			}
			pbyDestBitmap[m] = pbySrcBitmap[n]  ? 255: 0;
		}
		return (pData->BitmapRec[iChar].horiAdvance >> 6);
		
	}
    else
	{
		BITMAP_REC bitmapRec;
		FT_Int m, n, area;
		BYTE	*pbyDestBitmap, *pbySrcBitmap;
   
		memset(&bitmapRec,0x0,sizeof(BITMAP_REC));
		
		if (vencslave_gen_bitmap(c, &bitmapRec, pData->ftFace, pData->ftLib) != S_OK)
		{
			return 0;
		}
		
		if ((sdwStartPos + bitmapRec.bitmap_left + bitmapRec.bitmap.width) > dwStride) {
			fprintf(stderr, "[vencslave_txtoverlay](%d)The text string is too long to put in this video.\n",__LINE__);
			FT_Bitmap_Done(pData->ftLib, &bitmapRec.bitmap);
			return 0;
		}
		
		pftBitmap = &bitmapRec.bitmap;
	
		pbyDestBitmap = pData->pbyTextBitMap 
			+ (pData->sdwOrigin - bitmapRec.bitmap_top) * dwStride /* yMin * stride */
			+ sdwStartPos + bitmapRec.bitmap_left; 					/* xMin */
		pbySrcBitmap = pftBitmap->buffer;
		area = pftBitmap->rows * pftBitmap->width;
		//printf("[vencslave_txtoverlay](%d)\n",__LINE__);
		for (m = 0, n = 0; n < area; m++, n ++) {
			if (m == pftBitmap->width) {
				pbyDestBitmap += dwStride;
				m = 0;
			}
			pbyDestBitmap[m] = pbySrcBitmap[n]  ? 255: 0;
		}
		FT_Bitmap_Done(pData->ftLib, &bitmapRec.bitmap);
		//printf("[vencslave_txtoverlay](%d)\n",__LINE__);
		return (bitmapRec.horiAdvance >> 6);
		
	}    
}
void VencSlave_TextOverlay(TVideoFrameBuffer *ptTempVFB, TTextOverlayOptions *pData, const DWORD dwVideoMaxWidth)
{
	CHAR	szLocalText[MAX_OVERLAYED_TEXT_LEN + 1];
	//CHAR	szMilliSec[8];
	WORD    szUnicodeBuf[MAX_OVERLAYED_TEXT_LEN + 1];
	int	j = 0,Charlength = 0,index = 0;
	struct tm *ptmVFB;
	/* only perform this if the setting is changed or the printed text and timestamp is changed. */
	if ((pData->dwCurrentSecond != ptTempVFB->dwSecond) ||
			pData->bMilliSecEnable ||
			(pData->dwBitMapStride != ptTempVFB->dwStride)) {
		/* re-draw the buffer */
		//size_t	strLen = strlen(szLocalText);
		//CHAR	*pcTmp;
		WORD *pcTmp;
		SDWORD	sdwStartPos;

		ptmVFB = localtime((time_t *)&(ptTempVFB->dwSecond));
		index = strftime(szLocalText, sizeof(szLocalText), pData->szCustomText, ptmVFB);
		if (pData->bMilliSecEnable){
			//snprintf(szMilliSec, sizeof(szMilliSec), " (%u)", ptTempVFB->dwMilliSecond);
			//strcat(szLocalText, szMilliSec);
			sprintf(szLocalText + index, " (%u)", ptTempVFB->dwMilliSecond);
		} 
		pData->dwCurrentSecond = ptTempVFB->dwSecond;

		//printf("%s:%d: redraw \n", __FILE__, __LINE__);

		/* check if the overlay buffer is not enough */
		if (ptTempVFB->dwStride * pData->sdwLineHeight > dwVideoMaxWidth * MAX_OVERLAYED_TEXT_HEIGHT) {
			/* do nothing */
			fprintf(stderr, "Text-overlay buffer is not enough!");
			return;
		}

		Charlength = VencSlave_TextOverlay_EncodeUnicode(szLocalText,&(szUnicodeBuf[0]));
		/* clear the buffer first. */
		memset(pData->pbyTextBitMap, 0, ptTempVFB->dwStride * pData->sdwLineHeight);
		sdwStartPos = pData->sdwLeftMargin;
		
		pcTmp = &(szUnicodeBuf[0]);
		for ( index = 0; index < Charlength; index++ ){
			SDWORD	sdwAdvance;
			sdwAdvance = VencSlave_TextOverlay_Kernel(pData, pcTmp[index], 
					sdwStartPos, ptTempVFB->dwStride);
			if (sdwAdvance == 0) {
				break;
			}
			sdwStartPos += sdwAdvance;
		}

		pData->dwBitMapWidth = ((sdwStartPos + pData->sdwRightMargin + 3) >> 2) << 2;
		pData->dwBitMapWidth = (pData->dwBitMapWidth < ptTempVFB->dwStride) 
			? pData->dwBitMapWidth 
			: ptTempVFB->dwStride;

		pData->dwBitMapStride = ptTempVFB->dwStride;
	}

	/* copy the buffer to the actual video frame */
	for (j = 0; j < pData->sdwLineHeight; j++) {
		index = j * pData->dwBitMapStride;
		memcpy(ptTempVFB->pbyBufY + index, pData->pbyTextBitMap + index, pData->dwBitMapWidth);
	}
}

#else // not defined TEXT_OVERLAY
SCODE VencSlave_TextOverlay_Initial(TTextOverlayOptions *pData, const DWORD dwVideoMaxWidth)
{
    return S_OK;
}

void VencSlave_TextOverlay_Release(TTextOverlayOptions *pData)
{
	return;
}

SCODE VencSlave_TextOverlay_SetOptions(TTextOverlayOptions *pData, TTextOverlayOptions *pOptions)
{
	return S_OK;
}

void VencSlave_TextOverlay(TVideoFrameBuffer *ptTempVFB, TTextOverlayOptions *pOptions, const DWORD dwVideoMaxWidth)
{
	return;
}
#endif // not defined TEXT_OVERLAY


