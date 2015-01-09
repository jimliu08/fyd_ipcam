#ifndef __SENSOR_H__
#define __SENSOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif



/** 传感器数据上报 */
typedef struct
{
	unsigned char sensor_addr[2];   ///< 地址码
	unsigned char sensor_type;      ///< 类型
	unsigned char sensor_data;      ///< 数据码
}sensor_ask_data_t;



#ifdef __cplusplus
}
#endif

#endif

