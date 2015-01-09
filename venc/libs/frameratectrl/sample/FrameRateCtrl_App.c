
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "typedef.h"
#include "frameratectrl.h"
#include <unistd.h>

int main(int argc, char* argv[])
{
	// about frame rate control
    TFrameRateInitOptions tFRCtrlInitOptions;
    TFrameRateCtrlState tFRCtrlState;
    
    HANDLE hFRCtrlObject;
    // about file 
    FILE *pfInput;
    FILE *pfOutput;
    DWORD	dwSecond, dwMilliSecond, dwMicroSecond;
    
    if (argc != 4)
	{
		printf("Usage: %s \n", argv[0]);
        printf("           input_file\n");
		printf("           output_file\n");
		printf("           framerate setting\n");
		exit(1);
	}

	if ((pfInput=fopen(argv[1], "r")) == NULL)
	{
		printf("Open input file %s fail !!\n", argv[1]);
		exit(1);
	}

	if ((pfOutput=fopen(argv[2], "wb")) == NULL)
	{
		printf("Open output file %s fail !!\n", argv[2]);
		exit(1);
	}
 
    tFRCtrlInitOptions.dwVersion = FRAMERATECTRL_VERSION;
    tFRCtrlInitOptions.dwFrameRate = atoi(argv[3]);
    	
	if (FrameRateCtrl_Initial(&hFRCtrlObject, &tFRCtrlInitOptions) != S_OK)
	{
		printf("Initialize frame rate control object fail !!\n");
		exit(1);
	}

	printf("frame rate control  ...\n");
	fprintf(pfOutput, "frame rate set to %ld\n", tFRCtrlInitOptions.dwFrameRate);

	fscanf(pfInput, "%ld%ld%ld", &dwSecond, &dwMilliSecond, &dwMicroSecond);	
	while (!feof(pfInput)) {
		tFRCtrlState.dwSecond = dwSecond;
		tFRCtrlState.dwMilliSecond = dwMilliSecond;
		tFRCtrlState.dwMicroSecond = dwMicroSecond;
		FrameRateCtrl_OneFrame(hFRCtrlObject, &tFRCtrlState);
		fprintf(pfOutput, "%ld %ld %ld %s\n", dwSecond, dwMilliSecond, dwMicroSecond, (tFRCtrlState.bAvailable == TRUE)? "TRUE": "FALSE");
		fscanf(pfInput, "%ld%ld%ld", &dwSecond, &dwMilliSecond, &dwMicroSecond);
	}
	
    if (FrameRateCtrl_Release(&hFRCtrlObject) != S_OK)
        printf("Release frame rate control object fail!!\n");

    fclose(pfOutput);
    fclose(pfInput);
   
    return 0;
}
