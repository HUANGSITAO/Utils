#pragma once
#include "captureaudio.h"

 struct Audio_Data
{
	INT nTotalLen;		//��Ч����
	LPBYTE pData;		//����
	INT nPos_Head;	//����ͷ�α�
	INT nPos_End;	//д�߽�
	INT nPos_Tail;		//����β�α�
};

class Dsoundcap : public CAdoFrameHandler {
public:
	Dsoundcap();
	~Dsoundcap();
	bool init();
	void startCap();
	void stopCap();
public: // override the CAdoFrameHandler
	void AdoFrameData(BYTE* pBuffer, long lBufferSize);  // ���������һ�£��Ϳ��Բɼ���
	void ApullFrameDate(LPBYTE pData, INT nDataLen);
protected:
	CCaptureAudio   m_cap_ado; // ���������������ɼ���Ƶ���ݵ�	

private:
	bool isRunning = false;
};


