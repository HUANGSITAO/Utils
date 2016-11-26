#pragma once
#include "captureaudio.h"

 struct Audio_Data
{
	INT nTotalLen;		//有效数据
	LPBYTE pData;		//负载
	INT nPos_Head;	//数据头游标
	INT nPos_End;	//写边界
	INT nPos_Tail;		//数据尾游标
};

class Dsoundcap : public CAdoFrameHandler {
public:
	Dsoundcap();
	~Dsoundcap();
	bool init();
	void startCap();
	void stopCap();
public: // override the CAdoFrameHandler
	void AdoFrameData(BYTE* pBuffer, long lBufferSize);  // 这个类重载一下，就可以采集了
	void ApullFrameDate(LPBYTE pData, INT nDataLen);
protected:
	CCaptureAudio   m_cap_ado; // 这个对象就是用来采集音频数据的	

private:
	bool isRunning = false;
};


