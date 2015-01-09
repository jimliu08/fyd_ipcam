#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

#include "goolink_apiv2.h"
#include <errno.h>

#define GTCALLBACKXILIAN
#define GTCALLBACKHUAYI
#define GTCALLBACK20140917

void tsinterflow_stresstest();

//static unsigned char isstopsendmedia[48];

//static char readbuf[24248];
//static char readbuf[1000000];//[444776];
//static char readbuf2[24200];

void get64nowtime(uint64_t * t);

static int isend = 0;
static int beginstream = 0;

void CTRLC()
{
	printf(" CTRL + C detected!\n");
	//glnk_stop();
	//sleep(1);
	isend = 1;
	//exit(1);
	printf("CTRL + C event end!\n");
	return;
}

void test4pushalarm()
{
		PushAlarm alarm;
		alarm.alarm_type = PAT_MOTION_DETECT;
		alarm.channel = 1;
		alarm.timestamp.year = 2014;
		alarm.timestamp.month = 3;
		alarm.timestamp.day = 6;
		alarm.timestamp.hour = 17;
		alarm.timestamp.minute = 50;
		alarm.timestamp.second = 32; 
		glnk_push_alarm(alarm);
}


#define audiopacketmax  3705
#define testvideointerval 40000ULL

#if 0
void test_api(char *gid)
{
	//threadstart(__func__);
	signal(SIGINT, &CTRLC);
	printf("gid = %s\n", gid);
	if(gid == NULL)
		printf("please input: ./GoolinkDemo xxx(uid)");
	pthread_attr_t attr;
	//glnk_destroy();
	unsigned short tcpport = 16849, *isiframe;

	int64_t now, sendtime, reporttime, showsessiontime, audiotime;

	int ffd, ffdv;
	unsigned char videodata[700];
	unsigned char audiodata[320*audiopacketmax], videoopened, audiooppened;
#if 1
	if( (ffdv=fopen( "704x576x8s.h264","r" ))==NULL ){
		printf( "openfile [%s] error [%s]\n","704x576x8s.h264",strerror(errno) );
		videoopened = 0;
	}
	else {
		videoopened =1;
	}
	if( (ffd=fopen( "320A.g711","r" ))==NULL ){
		printf( "openfile [%s] error [%s]\n", "320A.g711",strerror(errno) );
		audiooppened = 0;
	}
	else{
		audiooppened = 1;
		fread(audiodata,320,audiopacketmax,ffd);
		fclose(ffd);
	}
#endif	
	printf("Version is 0x%x \n", (int)glnk_get_version() );
	InitNetParam netparam;
	strcpy(netparam.dev ,"eth0");
	netparam.localTCPport = 49000;
	netparam.localUDPport = 59000;
	strcpy(netparam.udid, gid);  // ad01803425
	netparam.channelnum = 32;
	netparam.issupportsubvideostream= 1;
	netparam.maxsession = 16;
	
testloop:
	
	glnk_destroy();
	int result;
	printf("glnk_init=%d\n",glnk_init( &netparam)); //"zz019096bc"
	//printf("glnk_init= %d\n",glnk_init( &netparam)); //"zz019096bc"
	int sid = 0, i, j;
	int index = 0;
	int pthdjoin =0;
	int state;
	int loopcount = 0;
	int ret = 0;
	int audiopacketno = 0, *videopacketlen = 0, videopacketno, videooffset;
	char *videosendpoint;
	get64nowtime(&now);
	sendtime = now;
	reporttime = now + 10000000LL;
	showsessiontime = now;
	audiotime = now;
	videopacketno = 0;
	j = 0;
    
	while( isend == 0 )
	{
		//j++;
		if(1)//beginstream == 1)
		{
			if(j == 2000)
			{
				//test4pushalarm();
				printf("quit \n");
				break;
			}
			usleep(10000);
			get64nowtime(&now);
			if(0)//now > reporttime )
			{
				//printf("connection of server status [%d]\n", glnk_get_state_to_server());
				reporttime += 10000000LL;
			}
			if(videoopened == 1 && now > sendtime)
			{
				fread(readbuf,4,1,ffdv);
				videopacketlen = readbuf;
				fread(readbuf+4, ntohl(*videopacketlen),1, ffdv);
				isiframe = readbuf + 24;
				for(i = 0; i < 4; i ++)
				{
					if(*isiframe == 100)
					{
						glnk_sendvideodata( i, 1, 1,readbuf+28, ntohl(*videopacketlen) - 24);
						glnk_sendvideodata( i, 0, 1,readbuf+28, ntohl(*videopacketlen) - 24);
					}
					else if(*isiframe == 101)
					{
						glnk_sendvideodata( i, 1, 0,readbuf+28, ntohl(*videopacketlen) - 24);
						glnk_sendvideodata( i, 0, 0,readbuf+28, ntohl(*videopacketlen) - 24);
					}
				}
				sendtime += testvideointerval;//166667ULL;
				videopacketno++;
				if(videopacketno >= 720)
				{
					videopacketno = 0;
					fseek(ffdv, 0, SEEK_SET);
				}
			}		
			while(audiooppened == 1 && now > audiotime)
			{
				for(i = 0; i < 4; i ++)
				{
					glnk_sendaudiodata(i, audiodata+audiopacketno*320, 320);
				}
				audiotime += 25000ULL;
				audiopacketno = (audiopacketno + 1) % 640;
			}
		}
		else {
			usleep(200000);
		}
	}

	glnk_destroy();
	printf("%s:%d\n",__func__,__LINE__);
}
#endif


void* changeport(void * arg)
{
	unsigned short port;
	char udid[32];
	strncpy(udid, (char*)arg, 32);

	InitNetParam netparam;
	strcpy(netparam.dev ,"eth0");
	netparam.localTCPport = 49000;
	netparam.localUDPport = 59000;
	strncpy(netparam.udid, (char*)arg, 32);  // ad01803425
	netparam.channelnum = 32;
	netparam.issupportsubvideostream= 1;
	netparam.maxsession = 16;
	port = 5678;
	while(1){
		//scanf("%d", &port);
		getchar();
		if(port < 1024)
		{
			debugtimer();
			printf("port = %d is too low!\n",port);
			continue;
		}
		netparam.localTCPport = port;
		debugtimer();
		printf("begin destroy!\n");
		glnk_destroy();
		getchar();
		debugtimer();
		printf("after destroy,begin init!\n");
		if(glnk_init(&netparam) != 0)
		{
			debugtimer();
			printf("init fail!\n");
			glnk_destroy();
			exit(1);
		}
		else {
			debugtimer();
			printf("init ok!\n");
		}
	}
}

#if 0
int GLNK_main(int argc, char **argv)
{
	test_api(argv[1]);    
	return 0;
}
#endif

unsigned char GLNK_PwdAuth_Callback(char* username, char* pwd)
{
	printf(" %s:%d username:%s  password:%s \n",__func__,__LINE__,username,pwd);
	return 1;
}

void GLNK_GetDevInfo_Callback(GLNK_V_DeviceInfo* devinfo)
{
	strcpy(devinfo->productId, "Goolink");
	strcpy(devinfo->name, "IPC01");
	devinfo->channelNum = 4;
	devinfo->alarmType = 0;
}

int32_t GLNK_RTVideoOpen_Callback(unsigned char channel, unsigned char ismainorsub, GLNK_VideoDataFormat* videoinfo)
{
	printf("%s channel=%d ismainorsub=%d\n",__func__,channel, ismainorsub);
	if(channel >= 0)
	{
		videoinfo->codec = GLNK_CODEC_H264;
		videoinfo->bitrate = 4666666 / testvideointerval *  8  * 1024;
		videoinfo->width = 704;
		videoinfo->height = 576;
		videoinfo->framerate = 1000000/testvideointerval;
		videoinfo->colorDepth = 24;
		videoinfo->frameInterval = 25;
		//isstopsendmedia[channel+ismainorsub*64] = 0;
		//pthread_create(&testsendstreamdata, NULL, testsendmedia, (int)(channel+ismainorsub*MAXCHANNELSUPPORT));
		beginstream = 1;
		return 1;
	}
	else {
		return 0;
	}
}
int32_t GLNK_RTVideoClose_Callback(unsigned char channel, unsigned char ismainorsub)
{
	printf("%s channel=%d\n",__func__,channel);
	beginstream = 0;
	return 1;
}

int32_t GLNK_AudioEncodeOpen_CallBack(unsigned char channel, GLNK_AudioDataFormat* audioinfo)
{
	printf("%s channel=%d\n",__func__,channel);
	audioinfo->samplesRate = 8000;
	audioinfo->bitrate = 16;
	audioinfo->waveFormat = 0x7A19;   // g711
	audioinfo->channelNumber = 1;
	audioinfo->blockAlign = 1;		//块对齐, channelSize * (bitsSample/8)
	audioinfo->bitsPerSample =16;	//每采样比特数
	audioinfo->frameInterval = 25;	//帧间隔, 单位ms
	audioinfo->reserve = 0;
	return 1;
}
int32_t GLNK_AudioEncodeClose_CallBack(unsigned char channel)
{
	printf("%s channel=%d\n",__func__,channel);
	return 1;
}


static int filefd[16] = {0};
int32_t GLNK_AudioDecodeOpen_CallBack(unsigned char channel, GLNK_AudioDataFormat* audioinfo)
{
	if(filefd[channel] == 0)
	{
		char filename[20];
		sprintf(filename, "ch%d.g711", channel);
		//filefd[channel] = fopen(filename,"w+");
		printf("%s channel=%d\n",__func__,channel);
	}
	//printf("%s channel=%d\n",__func__,channel);
	return 1;
}

int32_t GLNK_AudioDecode_CallBack(unsigned char channel, char* buffer, uint32_t length)
{
	printf("%s channel=%d, length=%d\n",__func__,channel, length);
	if(filefd[channel] != 0)
	{
		//printf("%s channel=%d, length=%d\n",__func__,channel, length);
		//fwrite(buffer, length, 1, filefd[channel]);
	}
	
	return 1;
}

int32_t GLNK_AudioDecodeClose_CallBack(unsigned char channel)
{
	printf("%s channel=%d\n",__func__,channel);
	if(filefd[channel] != 0)
	{
		//fclose(filefd[channel]);
		filefd[channel] = 0;
		printf("%s channel=%d\n",__func__,channel);
	}
	
	return 1;
}

int32_t GLNK_PTZCmd_Callback(GLNK_PTZControlCmd ptzcmd,uint32_t channel, GLNK_ControlArgData* arg)
{
	printf("PTZ channel[%d]  ", channel);
	switch( ptzcmd)
	{
		case GLNK_PTZ_MV_STOP :
		{
			printf("stop!\n");
			break;
		}
		case GLNK_PTZ_ZOOM_DEC :
		{
			printf("zoom decrease!\n");
			break;
		}
		case GLNK_PTZ_ZOOM_INC :
		{
			printf("zoom increase!\n");
			break;
		}
		case GLNK_PTZ_FOCUS_INC :
		{
			printf("focus increase!\n");
			break;
		}
		case GLNK_PTZ_FOCUS_DEC :
		{
			printf("focus decrease!\n");
			break;
		}
		case GLNK_PTZ_MV_UP :
		{
			printf("move up!\n");
			break;
		}
		case GLNK_PTZ_MV_DOWN :
		{
			printf("move down!\n");
			break;
		}
		case GLNK_PTZ_MV_LEFT :
		{
			printf("move left!\n");
			break;
		}
		case GLNK_PTZ_MV_RIGHT :
		{
			printf("move right!\n");
			break;
		}
		case GLNK_PTZ_IRIS_INC :
		{
			printf("iris increase!\n");
			break;
		}
		case GLNK_PTZ_IRIS_DEC :
		{
			printf("iris decrease!\n");
			break;
		}
		case GLNK_PTZ_AUTO_CRUISE :
		{
			printf("auto cruise!\n");
			break;
		}
		case GLNK_PTZ_GOTO_PRESET :
		{
			printf("position reset!\n");
			break;
		}
		case GLNK_PTZ_SET_PRESET :
		{
			printf("position set!\n");
			break;
		}
		case GLNK_PTZ_CLEAR_PRESET :
		{
			printf("position clear!\n");
			break;
		}
		case GLNK_PTZ_ACTION_RESET :
		{
			printf("action reset!\n");
			break;
		}
		case GLNK_PTZ_MV_LEFTUP :
		{
			printf("move leftup!\n");
			break;
		}
		case GLNK_PTZ_MV_LEFTDOWN :
		{
			printf("move leftdown!\n");
			break;
		}
		case GLNK_PTZ_MV_RIGHTUP :
		{
			printf("move rightup!\n");
			break;
		}
		case GLNK_PTZ_MV_RIGHTDOWN :
		{
			printf("move rightdown!\n");
			break;
		}
		case GLNK_PTZ_CLEAR_TOUR :
		{
			printf("clear tour!\n");
			break;
		}
		case GLNK_PTZ_ADD_PRESET_TO_TOUR :
		{
			printf("preset tour!\n");
			break;
		}
		case GLNK_PTZ_DEL_PRESET_TO_TOUR :
		{
			printf("delete tour!\n");
			break;
		}
		default:
			break;
	}
    return 0;
}

int32_t GLNK_VideoFileSearch_CallBack(GLNK_V_SearchFileRequest *SearchFileInfo, GLNK_V_FileInfo **ptr, int32_t *size)
{
	int32_t ret = 0, i, filenum=8;
	GLNK_V_FileInfo *FileInfo;
	FileInfo = (GLNK_V_FileInfo *)calloc(filenum, sizeof(GLNK_V_FileInfo));
	if(FileInfo == NULL)
		return 0;
	else
	{
		memset(&FileInfo[0], 0, sizeof(GLNK_V_FileInfo));
		FileInfo[0].channel = 1;
		FileInfo[0].deviceId = 1;
		FileInfo[0].endTime.m_year = 2014;
		FileInfo[0].endTime.m_day = 21;
		FileInfo[0].endTime.m_month = 4;
		FileInfo[0].endTime.m_hour = 12;
		FileInfo[0].endTime.m_minute = 10;
		FileInfo[0].endTime.m_second = 0;
		
		FileInfo[0].startTime.m_year = 2014;
		FileInfo[0].startTime.m_day = 21;
		FileInfo[0].startTime.m_month = 4;
		FileInfo[0].startTime.m_hour = 12;
		FileInfo[0].startTime.m_minute = 31;
		FileInfo[0].startTime.m_second = 0;
		
		FileInfo[0].length = 4096;
		FileInfo[0].recordType = 0x01;
		FileInfo[0].frames = 1;
		strcpy(FileInfo[0].fileName, "704x576x8s.h264"); 
		
		memset(&FileInfo[1], 0, sizeof(GLNK_V_FileInfo));
		FileInfo[1].channel = 1;
		FileInfo[1].deviceId = 1;
		FileInfo[1].endTime.m_year = 2014;
		FileInfo[1].endTime.m_day = 21;
		FileInfo[1].endTime.m_month = 4;
		FileInfo[1].endTime.m_hour = 12;
		FileInfo[1].endTime.m_minute = 10;
		FileInfo[1].endTime.m_second = 0;
		
		FileInfo[1].startTime.m_year = 2014;
		FileInfo[1].startTime.m_day = 21;
		FileInfo[1].startTime.m_month = 4;
		FileInfo[1].startTime.m_hour = 12;
		FileInfo[1].startTime.m_minute = 31;
		FileInfo[1].startTime.m_second = 0;
		
		FileInfo[1].length = 4096;
		FileInfo[1].recordType = 0x01;
		FileInfo[1].frames = 1;
		strcpy(FileInfo[1].fileName, "704x576x8sxiaoxu.h264"); 

		for(i = 2; i < filenum; i ++)
		{
			memset(&FileInfo[i], 0, sizeof(GLNK_V_FileInfo));
			FileInfo[i].channel = 1;
			FileInfo[i].deviceId = 1;
			FileInfo[i].endTime.m_year = 2014;
			FileInfo[i].endTime.m_day = 21;
			FileInfo[i].endTime.m_month = 4;
			FileInfo[i].endTime.m_hour = 12;
			FileInfo[i].endTime.m_minute = 10;
			FileInfo[i].endTime.m_second = 0;
			
			FileInfo[i].startTime.m_year = 2014;
			FileInfo[i].startTime.m_day = 21;
			FileInfo[i].startTime.m_month = 4;
			FileInfo[i].startTime.m_hour = 12;
			FileInfo[i].startTime.m_minute = 31;
			FileInfo[i].startTime.m_second = 0;
			
			FileInfo[i].length = 4096;
			FileInfo[i].recordType = 0x01;
			FileInfo[i].frames = 1;
			strcpy(FileInfo[i].fileName, "704x576x8sxiaoxu.h264"); 
		}
	}
	*ptr = FileInfo;
	*size = filenum*sizeof(GLNK_V_FileInfo);
	ret = 1;
	return ret ;
}

int32_t GLNK_RecordOpen_CallBack( char*  recordname, int32_t *recordfd,  GLNK_VideoDataFormat* videoinfo, GLNK_AudioDataFormat* audioinfo)
{
	int ret = 0, fd;
	if( (fd=fopen( recordname,"r" ))==NULL ){
		printf( "openfile [%s] error [%s]\n",recordname,strerror(errno) );
		ret = 0;
	}
	else{
		*recordfd = fd;
		videoinfo->codec = GLNK_CODEC_H264;
		videoinfo->bitrate = 4666666 / testvideointerval *  8  * 1024;
		videoinfo->width = 704;
		videoinfo->height = 576;
		videoinfo->framerate = 1000000ULL / testvideointerval;
		videoinfo->colorDepth = 24;
		videoinfo->frameInterval = 25;

		audioinfo->samplesRate = 8000;
		audioinfo->bitrate = 16;
		audioinfo->waveFormat = 0x7A19;   // g711
		audioinfo->channelNumber = 1;
		audioinfo->blockAlign = 1;		//
		audioinfo->bitsPerSample =16;	//
		audioinfo->frameInterval = 25;	//

		ret = 1;
	}
	printf("%s, opening file %s fd=%d/%d ret =%d, framerate=%d, bitrate=%d\n",__func__,recordname, *recordfd, fd, ret, videoinfo->framerate, videoinfo->bitrate);
	return ret;
}

int32_t GLNK_RecordReadFrame_CallBack(int32_t recordfd, unsigned char *streamframetype, uint32_t* timestamp, int32_t *videoframeindex, void** streamdata, uint32_t *streamdatalen)
{
	printf("%s, recordfd[%d] streamtype[%d]\n",__func__,recordfd, *streamframetype);
	int ret  = 0, rret;
	static int videoframeindex_s = 1;
	short * frametype;
	char frameinfo[30];
	static int videopacketno = 0;
	static unsigned int vodtimestamp = 0;
	
	do{
		rret = fread(streamdatalen,1,4,(FILE *)recordfd);
		if(rret != 4)
		{
			printf("rret=%d, videopacketno=%d\n",rret, videopacketno);
			break;
		}
		*streamdatalen = ntohl(*streamdatalen) - 24;
		rret = fread(frameinfo, 1,24, (FILE *)recordfd);
		if(rret != 24)
		{
			printf("rret=%d, videopacketno=%d\n",rret, videopacketno);
			break;
		}

		frametype = (short *)(frameinfo + 20);
		if(*frametype == 100) // iframe
		{
			*streamframetype = 1;
			*videoframeindex = videoframeindex_s;
			videoframeindex_s ++;
			videopacketno ++;
			vodtimestamp += 166;
			
		}
		else if( *frametype == 101) // pframe
		{
			*videoframeindex = videoframeindex_s;
			*streamframetype = 0;
			videoframeindex_s ++;
			videopacketno ++;
			vodtimestamp += 166;
		}
		else{
			*streamframetype = 2; // audioframe
		}
		*timestamp = vodtimestamp;
		*streamdata = malloc(*streamdatalen);
		rret = fread(*streamdata, 1, *streamdatalen, (FILE *)recordfd);
		/*
		if(videopacketno >=144)
		{
			videopacketno = 0;
			fseek(recordfd, 0, SEEK_SET);
		}
		*/
		if(rret != *streamdatalen)
		{
			break;
		}
		ret = 1;
	}while(0);
	printf("ret = %d\n",ret);
	return ret;
}


int32_t GLNK_RecordClose_CallBack(int32_t recordfd)
{
	printf("%s, filefd[%d]\n",__func__,recordfd);
	fclose((FILE *)recordfd);
	return 1;
}

#ifdef GTCALLBACKXILIAN

int32_t GLNK_ResetUsrPsword_Callback(char* username, char* oldpwd, char* newpwd)
{
	printf(" %s:%d username:%s	oldpwd:%s  newpwd:%s \n",__func__,__LINE__,username,oldpwd,newpwd);
	return 1;
}

int32_t GLNK_SetRecordConfigure_Callback(GLNK_V_RecordChgRequest *Req)
{
	printf(" %s:%d \n",__func__,__LINE__);
	printf("Req->command = %d \n", Req->command);
	if(Req->command == 2)
	{
		printf("startTime = %d-%d-%d %d:%d:%d  \n",Req->startTime.m_year, Req->startTime.m_month, Req->startTime.m_day, 
			Req->startTime.m_hour, Req->startTime.m_minute, Req->startTime.m_second);
		printf("endTime = %d-%d-%d %d:%d:%d  \n",Req->endTime.m_year, Req->endTime.m_month, Req->endTime.m_day, 
		Req->endTime.m_hour, Req->endTime.m_minute, Req->endTime.m_second);
	}
	return 1 ;
}



int32_t GLNK_SearchWifi_Callback(char ** buf)
{
	printf(" %s:%d \n",__func__,__LINE__);
	GLNK_V_WifiInfo *WifiInfo;
	WifiInfo = (GLNK_V_WifiInfo *)calloc(2, sizeof(GLNK_V_WifiInfo));
	WifiInfo[0].level = 0;
	strcpy(WifiInfo[0].name, "HelloworldWifiName");
	strcpy(WifiInfo[0].ssid, "HelloworldWifissid");

	WifiInfo[1].level = 2;
	strcpy(WifiInfo[1].name, "116WifiName");
	strcpy(WifiInfo[1].ssid, "116Wifissid");

	*buf = (char *)WifiInfo;
	return 2;
}

int32_t GLNK_WifiConfig_Callback(GLNK_V_WifiConfigRequest *Req)
{
	printf("name:%s ssid:%s pwd:%s \n", Req->name, Req->ssid, Req->password);
	printf(" %s:%d \n",__func__,__LINE__);
	int ret = 0;
    
    
	ret = 1;
	return ret ;
}

#endif

#ifdef GTCALLBACKHUAYI

int32_t GLNK_OpenDownLoadRecord_Callback( char* recordname, int32_t *recordfd,  char mode, uint32_t offset)
{
	int32_t ret = 0, fd;
	fd=(int32_t)fopen( recordname,"r");	
	if(fd == NULL ){
		printf( "openfile [%s] error [%s]\n",recordname,strerror(errno) );
		ret = 0;
	}	
	else
	{
		if(mode == 2)
		{
			fseek(fd, offset, SEEK_SET);
		}
		*recordfd = fd;
		ret = 1;
	}
	
	printf("%s, opening file %s fd=%d/%d mode=%d, offset=%d, ret =%d\n",__func__,recordname, *recordfd, fd, mode, offset, ret);
	return ret;
}
int32_t GLNK_CloseDownLoadRecord_Callback(int32_t recordfd)
{
	printf("%s, filefd[%d]\n",__func__,recordfd);
	fclose(recordfd);
	return 1;

}
int32_t GLNK_ReadDownLoadRecord_Callback(int32_t recordfd, char* data, int32_t datalen, int32_t *start_pos, int32_t *end_pos)
{
	printf("%s, filefd[%d]  datalen = %d\n",__func__,recordfd, datalen);
	int32_t rret, ret;
	*start_pos = ftell(recordfd);
	rret = fread(data, 1, datalen, recordfd);
	if(rret <= 0)
	{
		printf("%s, ret = 0;\n",__func__);
		ret = 0;
	}
	else
	{
		printf("%s, rret = %d;\n",__func__, rret);
		*end_pos = ftell(recordfd);
		ret = rret;
	}
	return ret;

}


#endif

#ifdef GTCALLBACK20140917

int32_t GLNK_GetVide_AudioConfig_Callback(int channel, TLV_V_VA_GET_CONFIG_RSP *rsq)
{
	printf("%s, channel[%d]\n",__func__,channel);
	rsq->channel = channel;
	rsq->isOpenAudio = 0;
	rsq->mainStreamBitRate = 10;
	rsq->mainStreamFrameRate = 5;
	rsq->mainStreamResolution = RESOLUTION_720P;
	rsq->subStreamBitRate = 15;
	rsq->subStreamFrameRate = 20;
	rsq->subStreamResolution = RESOLUTION_QQVGA;
	return 1;
}

int32_t GLNK_SetVide_AudioConfig_Callback(int32_t *result, TLV_V_VA_SET_CONFIG_REQ *req)
{
	printf("%s, result[%d]\n",__func__,*result);
	printf("channel = %d\n", req->channel);
	printf("isOpenAudio = %d\n", req->isOpenAudio);
	printf("mainStreamBitRate = %d\n", req->mainStreamBitRate);
	printf("mainStreamFrameRate = %d\n", req->mainStreamFrameRate);
	printf("mainStreamFrameRate = %d\n", req->channel);
	printf("mainStreamResolution = %d\n", req->mainStreamResolution);
	printf("subStreamBitRate = %d\n", req->subStreamBitRate);
	printf("subStreamFrameRate = %d\n", req->subStreamFrameRate);
	printf("subStreamResolution = %d\n", req->subStreamResolution);
	*result = 1;
	return 1;
}

int32_t GLNK_SetNetWorkConfig_Callback(int32_t *result, TLV_V_Network *req)
{
	printf("%s, result[%d]\n",__func__,*result);
	printf("deviceId = %d\n", req->deviceId);
	printf("hostIP = %s\n", req->hostIP);
	printf("hostName = %s\n", req->hostName);
	printf("gateway = %s\n", req->gateway);
	printf("dnsServer = %s\n", req->dnsServer);
	printf("dnsServer2 = %s\n", req->dnsServer2);
	printf("subnetMask = %s\n", req->subnetMask);
	printf("tcpPort = %s\n", req->wifiHostIP);
	printf("hostName = %s\n", req->wifiHostName);
	printf("gateway = %s\n", req->wifiGateway);
	printf("dnsServer = %s\n", req->wifiDnsServer);
	printf("dnsServer2 = %s\n", req->wifiDnsServer2);
	printf("subnetMask = %s\n", req->wifiSubnetMask);
	printf("mac = %s\n", req->mac);
	printf("mac = %s\n", req->wifiMac);
	printf("IPMode = %d\n", req->IPMode);
	printf("IPMode = %d\n", req->wifiIPMode);

	*result = 1;
	return 1;

}

int32_t GLNK_GetNetWorkConfig_Callback(TLV_V_Network *req)
{
	printf("%s\n",__func__);
	//memset(req, 0, sizeof(TLV_V_Network));
	req->deviceId = 0;
	req->hostIP[0] = 196;
	req->hostIP[1] = 168;
	req->hostIP[2] = 1;
	req->hostIP[3] = 67;

	req->dnsServer[0] = 144;
	req->dnsServer[1] = 144;
	req->dnsServer[2] = 144;
	req->dnsServer[3] = 144;
	
	req->dnsServer2[0] = 8;
	req->dnsServer2[1] = 8;
	req->dnsServer2[2] = 8;
	req->dnsServer2[3] = 8;
	
	req->wifiHostIP[0] = 196;
	req->wifiHostIP[1] = 168;
	req->wifiHostIP[2] = 1;
	req->wifiHostIP[3] = 199;
	
	req->wifiDnsServer[0] = 123;
	req->wifiDnsServer[1] = 123;
	req->wifiDnsServer[2] = 123;
	req->wifiDnsServer[3] = 123;
	
	req->wifiDnsServer2[0] = 18;
	req->wifiDnsServer2[1] = 18;
	req->wifiDnsServer2[2] = 18;
	req->wifiDnsServer2[3] = 18;
	
	req->wifiIPMode = 1;

	req->IPMode = 2;

	return 1;
}

#endif

#if 0
int32_t GLNK_DeviceUpDate_Callback(int32_t type,char *buffer, int32_t length)
{
    return 1;
}

int32_t GLNK_GetVersionFirmware_Callback(char* appbuf, char* solbuf, char* date, char* hardware)
{
    return 1;
}
#endif

