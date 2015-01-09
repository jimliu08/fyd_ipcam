#include "goolink_apiv2.h"

int32_t GLNK_RTVideoOpen_Callback(unsigned char channel,unsigned char ismainorsub, GLNK_VideoDataFormat* videoinfo);
int32_t GLNK_RTVideoClose_Callback(unsigned char channel, unsigned char ismainorsub);

int32_t GLNK_PTZOpen_Callback(uint32_t channel);
int32_t GLNK_PTZCmd_Callback(GLNK_PTZControlCmd ptzcmd,uint32_t channel, GLNK_ControlArgData* arg);
int32_t GLNK_PTZClose_Callback(uint32_t channel);

int32_t GLNK_AudioEncodeOpen_CallBack(unsigned char channel, GLNK_AudioDataFormat* audioinfo);
int32_t GLNK_AudioEncodeClose_CallBack(unsigned char channel);

int32_t GLNK_AudioDecodeOpen_CallBack(unsigned char channel, GLNK_AudioDataFormat *audioinfo);
int32_t GLNK_AudioDecode_CallBack(unsigned char channel, char* buffer, uint32_t length);
int32_t GLNK_AudioDecodeClose_CallBack(unsigned char channel);


int32_t GLNK_VideoFileSearch_CallBack(GLNK_V_SearchFileRequest *SearchFileInfo, GLNK_V_FileInfo **ptr, int32_t *size);
int32_t GLNK_RecordOpen_CallBack( char*  recordname, int32_t *recordfd,  GLNK_VideoDataFormat* videoinfo, GLNK_AudioDataFormat* audioinfo);
int32_t GLNK_RecordReadFrame_CallBack(int32_t recordfd, unsigned char *streamframetype, uint32_t *timestamp, int32_t *videoframeindex, void** streamdata, uint32_t *streamdatalen);
int32_t GLNK_RecordClose_CallBack(int32_t recordfd);

unsigned char GLNK_PwdAuth_Callback(char* username, char* pwd);
void GLNK_GetDevInfo_Callback(GLNK_V_DeviceInfo* devinfo);

int32_t GLNK_ResetUsrPsword_Callback(char* username, char* oldpwd, char* newpwd);
int32_t GLNK_SetRecordConfigure_Callback(GLNK_V_RecordChgRequest *Req);
int32_t GLNK_SearchWifi_Callback(char ** buf);
int32_t GLNK_WifiConfig_Callback(GLNK_V_WifiConfigRequest *Req);

int32_t GLNK_OpenDownLoadRecord_Callback( char* recordname, int32_t *recordfd,  char mode, uint32_t offset);
int32_t GLNK_CloseDownLoadRecord_Callback(int32_t recordfd);
int32_t GLNK_ReadDownLoadRecord_Callback(int32_t recordfd, char* data, int32_t datalen, int32_t *start_pos, int32_t *end_pos);

int32_t GLNK_GetVide_AudioConfig_Callback(int32_t channel, TLV_V_VA_GET_CONFIG_RSP *req);
int32_t GLNK_SetVide_AudioConfig_Callback(int32_t *result, TLV_V_VA_SET_CONFIG_REQ *req);
int32_t GLNK_SetNetWorkConfig_Callback(int32_t *result, TLV_V_Network *req);
int32_t GLNK_GetNetWorkConfig_Callback(TLV_V_Network *req);


int32_t GLNK_RTVideoOpenEXT_Callback(int32_t connectionID,unsigned char channel, GLNK_VideoDataFormat* videoinfo);
int32_t GLNK_RTVideoCloseEXT_Callback(int32_t connectionID);

int32_t GLNK_AudioEncodeOpenEXT_CallBack(int32_t connectionID, unsigned char channel,  GLNK_AudioDataFormat* audioinfo);
int32_t GLNK_AudioEncodeCloseEXT_CallBack(int32_t connectionID);

int32_t GLNK_AudioDecodeOpenEXT_CallBack(int32_t connectionID, unsigned char channel, GLNK_AudioDataFormat *audioinfo);
int32_t GLNK_AudioDecodeEXT_CallBack(int32_t connectionID, char* buffer, unsigned int length, GLNK_V_AudioInfo* audioframeinfo, char* extdata, int32_t extdatalen);
int32_t GLNK_AudioDecodeCloseEXT_CallBack(int32_t connectionID);

int32_t GLNK_DeviceUpDate_Callback(int32_t type,char *buffer, int32_t length);

int32_t GLNK_GetVersionFirmware_Callback(char* appbuf, char* solbuf, char* date, char* hardware);




