#ifndef __COMMON_API_DEF_H__
#define __COMMON_API_DEF_H__


typedef enum
{
	VIDEO_I_FRAME = 0,
	VIDEO_P_FRAME = 1,
	AUDIO_FRAME	= 2,

	UNKONW_FRAME_TYPE = -1,
}frame_type_e;


typedef enum
{
	ENCODE_TYPE_H264 = 0,
	ENCODE_TYPE_G711A = 100,
	ENCODE_TYPE_G711U = 101,
	ENCODE_TYPE_AAC4  = 102,
	ENCODE_TYPE_GAMR  = 102,
	ENCODE_TYPE_UNKNOW = -1,
}encode_type_e;

typedef enum 
{
	VENC_STREAM_NONE		 = 0,
	VENC_STREAM_MAIN        = 0x00000001,	//主码流
	VENC_STREAM_SUB         = 0x00000002,	//子码流
}venc_stream_e;  


typedef enum
{
    MSG_TYPE_433_DATA = 1,
        
    MSG_TYPE_UNKNOWN
}msg_type_e;



typedef struct frame_info_s
{
    int channel;
    encode_type_e encode_type;
    venc_stream_e stream_type;
    frame_type_e frame_type;


    char  *buf;
    size_t size;
}__attribute__((packed)) frame_info_t;



#endif /* __GOOLINK_H__ */
