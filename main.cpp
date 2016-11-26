#include"ListSoundDev_head.h"
#include "EncodingConverter.h"
#include<Windows.h>
#include<fcntl.h>
#include<io.h>
#include<stdio.h>
#include<cstdlib>
//#include<string>
//用来保存枚举出来的采集设备对象；
std::vector<DevItem>	m_CapDevices;
BOOL CALLBACK DSEnumCallback(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext)
{
	std::vector<DevItem> *pLst = (std::vector<DevItem> *) lpContext;
	if (pLst)
	{
		DevItem item;
		memset(&item, 0, sizeof(item));
		item.strName = lpcstrDescription;
		if (lpGuid)
			item.guid = *lpGuid;
		else
			item.guid = GUID_NULL;
		pLst->push_back(item);
		return TRUE;
	}
	return FALSE;
}
BOOL CALLBACK DSEnumCallback1(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext)
{
	std::vector<std::string> *names = (std::vector<std::string> *) lpContext;
	names->push_back(std::string(lpcstrDescription));
	return TRUE;
}
#if 1
int main()
{ 
	std::vector<DevItem>::iterator it;
	HRESULT hr = S_OK;
	setlocale(LC_ALL, "chs");
#if _UNICODE
	hr = DirectSoundCaptureEnumerate((LPDSENUMCALLBACKW)DSEnumCallback, (LPVOID)&m_CapDevices);
#else
	hr = DirectSoundCaptureEnumerate(DSEnumCallback, (LPVOID)&m_CapDevices);
#endif
	//在我这里第一个设备名输出不正确，后两个正确，上边直接输出lpcstrDescription结果正确，不知道为什么
	for (it = m_CapDevices.begin(); it != m_CapDevices.end(); it++){
#if _UNICODE //Unicode字符集
		char newStr[100];
		wcstombs(newStr, (wchar_t*)it->strName, 100);
		printf("newStr=%s\n", newStr);
#else //多字节字符集
		printf("%s --- %d\n", it->strName, strlen(it->strName));
#endif
	}
}
#endif
#if 0  //
int main(){
	std::vector<std::string> names;
	if (!FAILED(DirectSoundEnumerate(&DSEnumCallback1, &names))){
		std::vector<std::string>::iterator it;
		int n = 34;
		for (it = names.begin(); it != names.end(); it++){
			std::string str = it->substr(0, n);
			cout << str << endl;
		}
	}
}
#endif