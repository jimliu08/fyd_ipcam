/* ============================================================================================== */
#ifndef __DEFECTPIXELCORRECT_H__
#define __DEFECTPIXELCORRECT_H__

/* =========================================================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "global_codec.h"
#include "typedef.h"
#include "MemMgr.h"

/* =========================================================================================== */
#define DEFECTPIXELCORRECT_VERSION MAKEFOURCC(1, 0, 0, 0)
#define DEFECTPIXELCORRECT_ID_VERSION "1.0.0.0"

#define VIDEOCAP_IN_BUS_NUM	    0

typedef struct defect_pixel_info
{
	DWORD dwPixelNum;  
	DWORD dwLineNum;
	DWORD dwType; 
} TDefectPixelInfo;

/*! A data structure of \b DefectPixelCorrect object used in function \b DefectPixelCorrect_Initial. */
typedef struct defect_pixel_correct_init_options
{
	/*! defect pixel correct library version number */
	DWORD dwVersion;

	/*! defect pixel correct object memory address. Set to NULL to allocate object memory internally. */
	void *pObjectMem;

	/*! defect pixel Number. */
	DWORD dwDefectPixelNum;

	/*! defect pixel position. */
	TDefectPixelInfo *ptDPInfo;
} TDefectPixelCorrectInitOptions;

/*! A data structure of \b defect pixel correct object used in function \b DefectPixelCorrect_ProcessOneFrame.
 * Fields in this structure will show the defect pixel correct param. */
typedef struct defect_pixel_correct_state
{
	/* Mem Mgr handle */
	HANDLE hMemMgrObj;

	/* Current frame Y component base address */
	BYTE *pbyYFrame;

	/* Current frame Cb component base address */
	BYTE *pbyCbFrame;

	/* Current frame Cr component base address */
	BYTE *pbyCrFrame;

	/* Current frame Width */
	DWORD dwOutWidth;

	/* Current frame Height */
	DWORD dwOutHeight;
} TDefectPixelCorrectState;

/* =========================================================================================== */
SCODE DefectPixelCorrect_Initial(HANDLE *phObject, TDefectPixelCorrectInitOptions *ptInitOptions);
SCODE DefectPixelCorrect_Release(HANDLE *phObject);

SCODE DefectPixelCorrect_ProcessOneFrame(HANDLE hObject, TDefectPixelCorrectState *ptState);
DWORD DefectPixelCorrect_QueryMemSize(TDefectPixelCorrectInitOptions *ptInitOptions);
SCODE DefectPixelCorrect_GetVersionInfo(BYTE *pbyMajor, BYTE *pbyMinor, BYTE *pbyBuild, BYTE *pbyRevision);

/* =========================================================================================== */
#endif //__DEFECTPIXELCORRECT_H__
