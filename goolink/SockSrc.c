#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "UnixSockUtil.h"
#include "SockSrc.h"
#define VREC_AENC_PIOID 1
typedef struct socket_source
{
	
	int					iSrcFd;
	int					iCmdFiFo;
	fd_set				tReadfdsRec;
	fd_set				tReadfds;
	BYTE				*pbyBuf;
	int					iBufSz;
	int					iIOId;
} TSockSrc;


int   SockSrc_GetFd(HANDLE hSockSrc)
{
  
    return ((TSockSrc *)hSockSrc)->iSrcFd;

}

DWORD getIOID(char *szSockPath)
{
  
  char * pch;
  char  *tmp;
  DWORD dwIOID=0;
  //printf ("Splitting string \"%s\" into tokens:\n",str);
  printf("%s\n", szSockPath);
  pch = strtok (szSockPath,"/");
  int i=0;
  printf (" %d\n", __LINE__);
  sleep(10);
  printf ("%s [%d]\n",pch, __LINE__);
  while (pch != NULL)
  {
    if(i==3)
    {
      //printf ("%s\n",pch);
      tmp = strtok (pch,"audio.sck");
      printf ("%s\n",tmp);
      dwIOID= strtol(tmp,NULL,10);
      break;
    }
    i++;
    pch = strtok (NULL, "/");
  }
  
  return dwIOID;
}

SCODE SockSrc_Initial(HANDLE *phSockSrc, TSockSrcInitOpts *ptInitOpts)
{

  	TSockSrc *ptSockSrc;
	CHAR *szRecvSockPathPrefix=NULL;
	CHAR *szRecvSockPath=NULL;
	CHAR *szID=NULL;
	assert(phSockSrc && ptInitOpts);

	ptSockSrc = (TSockSrc *)malloc(sizeof(TSockSrc));
	if (ptSockSrc == NULL)
	{
		printf("[%s:%s:%d] malloc error!\n", 
						__func__, __FILE__, __LINE__);
		return S_FAIL;
	}
	memset(ptSockSrc, 0, sizeof(TSockSrc));
	*phSockSrc = ptSockSrc;



	ptSockSrc->iSrcFd	= -1;
	ptSockSrc->iCmdFiFo	= -1;
	
	if (ptInitOpts->szRecvSockPath == NULL)
	{
		printf("[%s:%s:%d] Invalid sock path %s!\n", 
						__func__, __FILE__, __LINE__, 
						ptInitOpts->szRecvSockPath);
		goto error_handle;
	}
	
	//printf("[SockSrc](%d)ptInitOpts->szRecvSockPath=%s\n",__LINE__,ptInitOpts->szRecvSockPath);
	if(strchr(ptInitOpts->szRecvSockPath,'@')!=NULL)
	{
	    szRecvSockPathPrefix=strtok(ptInitOpts->szRecvSockPath, "@");
	    //printf("[SockSrc](%d)szRecvSockPathPrefix=%s\n",__LINE__,szRecvSockPathPrefix);
	    szID=strtok(NULL,"@");
	    if(szID!=NULL)
	    {
		ptSockSrc->iIOId=strtol(szID, (char **)NULL,10);
		//printf("[SockSrc](%d)ptSockSrc->iIOId=%d\n",__LINE__,ptSockSrc->iIOId);
		szRecvSockPath=(CHAR *)malloc(sizeof(CHAR)*(strlen(szRecvSockPathPrefix)+2));
		//printf("[SockSrc](%d)\n",__LINE__);
		memset(szRecvSockPath,0x0,(strlen(szRecvSockPathPrefix)+2));
		//printf("[SockSrc](%d)\n",__LINE__);
		snprintf(szRecvSockPath,(strlen(szRecvSockPathPrefix)+2),"%s%d",szRecvSockPathPrefix,ptSockSrc->iIOId);
		//printf("[SockSrc](%d)szRecvSockPath=%s\n",__LINE__,szRecvSockPath);
		//printf("[SockSrc](%d)szID=%s\n",__LINE__,szID);
	    #if 0 
	      // strtok uses it's own static buffer so you should not call free.
		if(szRecvSockPathPrefix!=NULL)
		{
		    free(szRecvSockPathPrefix);
		    szRecvSockPathPrefix=NULL;
		}
		if(szID != NULL)
		{
		    free(szID);//this will cause the "Segmentat fault".
		    szID=NULL;
		}
		
	    #endif   
	    }
	    else
	    {		
		ptSockSrc->iIOId=0;
	    
	      
	    }
	    //printf("[SockSrc](%d)\n",__LINE__);
	    if ((ptSockSrc->iSrcFd=UnixDgram_CreateSock(szRecvSockPath)) < 0)
	    {
		    printf("[%s:%s:%d] UnixDgram_CreateSock error!\n", 
						    __func__, __FILE__, __LINE__);
		    goto error_handle;
	    }
	     printf("[SockSrc](%d)socket path=%s\n",__LINE__,szRecvSockPath);
	    //printf("[SockSrc](%d)\n",__LINE__);
	    if(szRecvSockPath!=NULL)
	    {
		free(szRecvSockPath);
		szRecvSockPath=NULL;
	    
	    }
	    
	}
	else
	{
	    printf("[SockSrc](%d)socket path=%s\n",__LINE__,ptInitOpts->szRecvSockPath);
	    if ((ptSockSrc->iSrcFd=UnixDgram_CreateSock(ptInitOpts->szRecvSockPath)) < 0)
	    {
		   printf("[%s:%s:%d] UnixDgram_CreateSock error!\n", 
						    __func__, __FILE__, __LINE__);
		    goto error_handle;
	    }
	    //ptSockSrc->iIOId=getIOID(ptInitOpts->szRecvSockPath);
	    ptSockSrc->iIOId = 0;
	    printf("[SockSrc](%d)IOID=%d\n",__LINE__,ptSockSrc->iIOId);
	}    
	    
	printf("[SockSrc](%d)\n",__LINE__);
	ptSockSrc->iBufSz = MAX_MP4V_UBUFFER;
	ptSockSrc->pbyBuf = (BYTE *)malloc(ptSockSrc->iBufSz);
	if (ptSockSrc->pbyBuf == NULL)
	{
		printf("[%s:%s:%d] malloc error!\n", 
						__func__, __FILE__, __LINE__);
		goto error_handle;
	}
	printf("[SockSrc](%d)\n",__LINE__);
	// Create command fifo
	
	if (ptInitOpts->szCmdFiFoPath != NULL)
	{
	  printf("[SockSrc](%d)\n",__LINE__);
	  printf("szCmdFiFoPath=%s\n",ptInitOpts->szCmdFiFoPath);
		if(access(ptInitOpts->szCmdFiFoPath, F_OK) == -1)
		{
		  printf("szCmdFiFoPath=%s\n",ptInitOpts->szCmdFiFoPath);
			if (mkfifo(ptInitOpts->szCmdFiFoPath, 0777) != 0)
			{
				printf("[%s:%s:%d] Could not create fifo %s\n", 
								__func__, __FILE__, __LINE__, 
								ptInitOpts->szCmdFiFoPath);
				goto error_handle;
			}
		}
//printf("[SockSrc](%d)\n",__LINE__);
		ptSockSrc->iCmdFiFo = open(ptInitOpts->szCmdFiFoPath, O_WRONLY);//|O_NONBLOCK);
//printf("[SockSrc](%d)\n",__LINE__);
		if(ptSockSrc->iCmdFiFo < 0)
		{
			printf("[%s:%s:%d] open fifo %s failed: %d\r\n", 
							__func__, __FILE__, __LINE__, 
							ptInitOpts->szCmdFiFoPath, errno);
			goto error_handle;
		}
		printf("[%s:%s:%d] open fifo %s ok: %d\r\n", 
						__func__, __FILE__, __LINE__, 
						ptInitOpts->szCmdFiFoPath, ptSockSrc->iCmdFiFo);
	}

	//FD_ZERO(&ptSockSrc->tReadfdsRec);
	//FD_SET(ptSockSrc->iSrcFd, &ptSockSrc->tReadfdsRec);
	//FD_ZERO(&ptSockSrc->tReadfds);

	return S_OK;
error_handle:

	SockSrc_Release(phSockSrc);
	return S_FAIL;
  
  
}
SCODE SockSrc_Release(HANDLE *phSockSrc)
{
  TSockSrc *ptSockSrc;

  ptSockSrc = (TSockSrc *)*phSockSrc;

   if (ptSockSrc->iSrcFd >= 0)
   {
	close(ptSockSrc->iSrcFd);
   }
   if (ptSockSrc->iCmdFiFo >= 0)
   {
	close(ptSockSrc->iCmdFiFo);
   }
   if (ptSockSrc->pbyBuf != NULL)
   {
	free(ptSockSrc->pbyBuf);
   }
    free(ptSockSrc);
    *phSockSrc = NULL;

  return S_OK;
}

SCODE SockSrc_GetUBuffer(HANDLE hSockSrc, TSrcUBufInfo *ptUBuffInfo)
{
  
  	TSockSrc	*ptSockSrc;
	TUBuffer	*ptUBuffer;
	BYTE		*pbyBuf;
	int			iRet;

	ptSockSrc			= (TSockSrc *)hSockSrc;
	ptSockSrc->tReadfds	= ptSockSrc->tReadfdsRec;
	pbyBuf				= ptSockSrc->pbyBuf;

	//iRet = select(ptSockSrc->iSrcFd+1, &ptSockSrc->tReadfds, NULL, NULL, NULL);
	//if ((iRet > 0) && FD_ISSET(ptSockSrc->iSrcFd, &ptSockSrc->tReadfds))
	//{
		iRet = recv(ptSockSrc->iSrcFd, pbyBuf, ptSockSrc->iBufSz, 0);
		if (iRet > 0)
		{
			ptUBuffer = (TUBuffer *)pbyBuf;

			if (ptUBuffer->dwSize != iRet)
			{
				printf("[%s:%s:%d] recv a broken ubuffer!\n", __func__, __FILE__, __LINE__);
				return S_FAIL;
			}

			if (ptUBuffer->dwDataType == FOURCC_CONF)
			{
				ptUBuffInfo->pbyUBuffHdr = pbyBuf;
				ptUBuffInfo->iUBuffHdrLen = ptUBuffer->dwSize;
				ptUBuffInfo->pbyUBuffPayload = NULL;
				ptUBuffInfo->iUBuffPayloadLen = 0;
			}
			else
			{
				ptUBuffInfo->pbyUBuffHdr = pbyBuf;
				ptUBuffInfo->iUBuffHdrLen = sizeof(TUBuffer)+(int)ptUBuffer->dwUserDataSize;
				ptUBuffInfo->pbyUBuffPayload = pbyBuf+ptUBuffInfo->iUBuffHdrLen;
				ptUBuffInfo->iUBuffPayloadLen = ptUBuffer->dwSize-ptUBuffInfo->iUBuffHdrLen;
			}
		
			return S_OK;
		}
	//}
	
	return S_FAIL;

}

SCODE SockSrc_ReleaseUBuffer(HANDLE hSockSrc, TSrcUBufInfo *ptUBuffInfo)
{
  return S_OK;
  
}

#define CONTROL_MSG_START		"<output>start</output>"
#define CONTROL_MSG_STOP		"<output>stop</output>"	
#define CONTROL_MSG_FORCECI		"<forceCI/>"	
#define CONTROL_MSG_FORCEINTRA		"<forceIntra/>"	
#define CONTROL_MSG_LEN 		128
SCODE SockSrc_EventHandler(HANDLE hSockSrc, ESrcEventType eType)
{
 TSockSrc	*ptSockSrc;
	
	//CHAR		acTemp[100];
	CHAR		acTemp[512];
	int iLen=0;
	CHAR szMsg[CONTROL_MSG_LEN];
	ptSockSrc = (TSockSrc *)hSockSrc;
	
	
	memset(szMsg,0x0,CONTROL_MSG_LEN);
	
	switch(eType)
	{
		case LetNeedIntra:
			printf("Need Intra\n");
			if (ptSockSrc->iCmdFiFo >= 0)
			{
			  #if 0
				acTemp[1] = sprintf(acTemp+2, CONTROL_MSG_FORCEINTRA);
				acTemp[0] = 0x02;
				write(ptSockSrc->iCmdFiFo, acTemp, acTemp[1]+2);
				printf("write command %d bytes: %s\r\n", acTemp[1], acTemp+2);
			  #endif
				
				acTemp[0] = 1;
				// force it using a long mode
				acTemp[1] = 0x84;
				snprintf(szMsg,CONTROL_MSG_LEN,"<control id=\"%d\">%s</control>", ptSockSrc->iIOId,CONTROL_MSG_FORCEINTRA);
				sprintf(&(acTemp[6]),szMsg );
				iLen = strlen(&(acTemp[6]));
				acTemp[5] =  iLen >> 24;
				acTemp[4] = (iLen & 0x00FF0000) >> 16;
				acTemp[3] = (iLen & 0x0000FF00) >> 8;
				acTemp[2] = (iLen & 0x000000FF);

				write(ptSockSrc->iCmdFiFo, acTemp, iLen+6);
				printf("write command %d bytes: %s\r\n", iLen+6, acTemp+6);
			}
			break;
		case LetNeedConf:
			printf("Need Conf\n");
			if (ptSockSrc->iCmdFiFo >= 0)
			{
			  #if 0
				acTemp[1] = sprintf(acTemp+2, CONTROL_MSG_FORCECI);
				acTemp[0] = 0x02;
				write(ptSockSrc->iCmdFiFo, acTemp, acTemp[1]+2);
				printf("write command %d bytes: %s\r\n", acTemp[1], acTemp+2);
			  #endif
				acTemp[0] = 1;
				// force it using a long mode
				acTemp[1] = 0x84;
				snprintf(szMsg,CONTROL_MSG_LEN,"<control id=\"%d\">%s</control>", ptSockSrc->iIOId,CONTROL_MSG_FORCECI);
				sprintf(&(acTemp[6]),szMsg );
				iLen = strlen(&(acTemp[6]));
				acTemp[5] =  iLen >> 24;
				acTemp[4] = (iLen & 0x00FF0000) >> 16;
				acTemp[3] = (iLen & 0x0000FF00) >> 8;
				acTemp[2] = (iLen & 0x000000FF);

				write(ptSockSrc->iCmdFiFo, acTemp, iLen+6);
				printf("write command %d bytes: %s\r\n", iLen+6, acTemp+6);
			  
			  
			}
			break;
		case LetBitstrmStart:
			printf("Bitstrm start\n");
			if (ptSockSrc->iCmdFiFo >= 0)
			{
			  #if 0
				acTemp[1] = sprintf(acTemp+2, CONTROL_MSG_START);
				acTemp[0] = 0x02;
				write(ptSockSrc->iCmdFiFo, acTemp, acTemp[1]+2);
				printf("write command %d bytes: %s\r\n", acTemp[1], acTemp+2);
			  #endif
				acTemp[0] = 1;
				// force it using a long mode
				acTemp[1] = 0x84;
				snprintf(szMsg,CONTROL_MSG_LEN,"<control id=\"%d\">%s</control>", ptSockSrc->iIOId,CONTROL_MSG_START);
				sprintf(&(acTemp[6]),szMsg );
				iLen = strlen(&(acTemp[6]));
				acTemp[5] =  iLen >> 24;
				acTemp[4] = (iLen & 0x00FF0000) >> 16;
				acTemp[3] = (iLen & 0x0000FF00) >> 8;
				acTemp[2] = (iLen & 0x000000FF);

				write(ptSockSrc->iCmdFiFo, acTemp, iLen+6);
				printf("write command %d bytes: %s\r\n", iLen+6, acTemp+6);
			      
			}
			break;
		case LetBitstrmStop:
			printf("Bitstrm stop\n");
			if (ptSockSrc->iCmdFiFo >= 0)
			{
			  #if 0
				acTemp[1] = sprintf(acTemp+2, CONTROL_MSG_STOP);
				acTemp[0] = 0x02;
				write(ptSockSrc->iCmdFiFo, acTemp, acTemp[1]+2);
				printf("write command %d bytes: %s\r\n", acTemp[1], acTemp+2);
			  #endif
				acTemp[0] = 1;
				// force it using a long mode
				acTemp[1] = 0x84;
				snprintf(szMsg,CONTROL_MSG_LEN,"<control id=\"%d\">%s</control>", ptSockSrc->iIOId,CONTROL_MSG_STOP);
				sprintf(&(acTemp[6]),szMsg );
				iLen = strlen(&(acTemp[6]));
				acTemp[5] =  iLen >> 24;
				acTemp[4] = (iLen & 0x00FF0000) >> 16;
				acTemp[3] = (iLen & 0x0000FF00) >> 8;
				acTemp[2] = (iLen & 0x000000FF);

				write(ptSockSrc->iCmdFiFo, acTemp, iLen+6);
				printf("write command %d bytes: %s ptSockSrc->iCmdFiFo=%d\r\n", iLen+6, acTemp+6,ptSockSrc->iCmdFiFo);
			  
			}
			break;
		default:
			return S_FAIL;
	}

	return S_OK;
}
