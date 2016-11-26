/* ----------------------------------------------------------
�ļ����ƣ�DS_AudioVideoDevices.h

���ߣ��ؽ���

MSN��splashcn@msn.com

�汾��ʷ��
V1.0    2010��10��09��
�����ʽ�汾��

����������
��ȡ��Ƶ��Ƶ�����豸�б�

�ӿں�����
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
	WCHAR FriendlyName[MAX_FRIENDLY_NAME_LENGTH];   // �豸�Ѻ���  
	WCHAR MonikerName[MAX_MONIKER_NAME_LENGTH];     // �豸Moniker��  
} TDeviceName;
#endif  

#ifdef __cplusplus  
extern "C"
{
#endif  

	/*
	���ܣ���ȡ��Ƶ��Ƶ�����豸�б�
	����˵����
	vectorDevices�����ڴ洢���ص��豸�Ѻ�����Moniker��
	guidValue��
	CLSID_AudioInputDeviceCategory����ȡ��Ƶ�����豸�б�
	CLSID_VideoInputDeviceCategory����ȡ��Ƶ�����豸�б�
	����ֵ��
	�������
	˵����
	����DirectShow
	�б��еĵ�һ���豸Ϊϵͳȱʡ�豸
	capGetDriverDescriptionֻ�ܻ���豸������
	*/
	HRESULT DS_GetAudioVideoInputDevices(std::vector<TDeviceName> &vectorDevices, REFGUID guidValue);

#ifdef __cplusplus  
}
#endif  