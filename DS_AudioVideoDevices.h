/* ----------------------------------------------------------
文件名称：DS_AudioVideoDevices.h

作者：秦建辉

MSN：splashcn@msn.com

版本历史：
V1.0    2010年10月09日
完成正式版本。

功能描述：
获取音频视频输入设备列表

接口函数：
DS_GetAudioVideoInputDevices
------------------------------------------------------------ */
#pragma once  

#include <windows.h>  
#include <vector>  
#include <dshow.h>  

#ifndef MACRO_GROUP_DEVICENAME  
#define MACRO_GROUP_DEVICENAME  

#define MAX_FRIENDLY_NAME_LENGTH    128  
#define MAX_MONIKER_NAME_LENGTH     256  

typedef struct _TDeviceName
{
	WCHAR FriendlyName[MAX_FRIENDLY_NAME_LENGTH];   // 设备友好名  
	WCHAR MonikerName[MAX_MONIKER_NAME_LENGTH];     // 设备Moniker名  
} TDeviceName;
#endif  

#ifdef __cplusplus  
extern "C"
{
#endif  

	/*
	功能：获取音频视频输入设备列表
	参数说明：
	vectorDevices：用于存储返回的设备友好名及Moniker名
	guidValue：
	CLSID_AudioInputDeviceCategory：获取音频输入设备列表
	CLSID_VideoInputDeviceCategory：获取视频输入设备列表
	返回值：
	错误代码
	说明：
	基于DirectShow
	列表中的第一个设备为系统缺省设备
	capGetDriverDescription只能获得设备驱动名
	*/
	HRESULT DS_GetAudioVideoInputDevices(std::vector<TDeviceName> &vectorDevices, REFGUID guidValue);

#ifdef __cplusplus  
}
#endif  