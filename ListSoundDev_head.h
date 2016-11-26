#ifndef _LISTSOUNDDEV_HEAD_
#define _LISTSOUNDDEV_HEAD_
#include<tchar.h>
#include <dshow.h>
#include<iostream>
#include<vector>
#include<mmsystem.h>
#include<mmreg.h>
#include<dsound.h>
#include <atlstr.h>
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "Quartz.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")

#pragma comment(lib, "strmiids")
using namespace std;

typedef struct _DevItem
{
	LPCSTR strName;
	GUID guid;
} DevItem;
#endif
