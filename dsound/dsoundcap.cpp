#include "stdafx.h"
#include "dsoundcap.h"
#include <stdio.h>
#include <../ffmpeg.h>

Audio_Data audio_Data;
const int MAX_BUFF_LEN= 192000/5;
ThreadStruct AdsoundCapStruct;

Dsoundcap::Dsoundcap()
{

}

Dsoundcap::~Dsoundcap()
{
	if (isRunning)
	{
		stopCap();
		audio_Data.nPos_Head = 0;
		audio_Data.nPos_End = 0;
		audio_Data.nPos_Tail = 0;		
		audio_Data.nTotalLen = 0;
		free(audio_Data.pData);		
		freeEventandMutex(&AdsoundCapStruct);	
		isRunning = false;		
	}
}

bool Dsoundcap::init()
{
	audio_Data.pData = new BYTE[MAX_BUFF_LEN];
	initEventandMutex(&AdsoundCapStruct);	
	return m_cap_ado.Open();		
}

void Dsoundcap::startCap()
{
	m_cap_ado.GrabAudioFrames(TRUE, this);	
	isRunning = true;
}

void Dsoundcap::stopCap()
{
	m_cap_ado.GrabAudioFrames(FALSE, NULL);
	m_cap_ado.Close();	
}

void Dsoundcap::AdoFrameData(BYTE * pBuffer, long lBufferSize)
{		
	
	WaitForSingleObject(AdsoundCapStruct.hMutex, INFINITE);
	if (audio_Data.nPos_Head <= audio_Data.nPos_Tail)			//头在前
	{
		//尾部溢出
		if (audio_Data.nPos_Tail + lBufferSize > MAX_BUFF_LEN)
		{
			if (audio_Data.nPos_Head>lBufferSize)//跳转到头部且有空间
			{
				audio_Data.nPos_End = audio_Data.nPos_Tail;
				audio_Data.nPos_Tail = 0;
				memcpy_s(audio_Data.pData, MAX_BUFF_LEN, pBuffer, lBufferSize);
				audio_Data.nPos_Tail += lBufferSize;
				audio_Data.nTotalLen += lBufferSize;
			}			
			else
			{
				Sleep(5);
				//printf("跳转到头部且无空间\n ");
			}
		}
		else
		{
			memcpy_s(audio_Data.pData + audio_Data.nPos_Tail, MAX_BUFF_LEN, pBuffer, lBufferSize);
			audio_Data.nPos_End = audio_Data.nPos_Tail += lBufferSize;
			audio_Data.nTotalLen += lBufferSize;
			//printf("audio_Data.nTotalLen=%d\n", audio_Data.nTotalLen);
		}
	}
	else //头在后
	{
		//尾部不超过头部
		if (audio_Data.nPos_Tail + lBufferSize < audio_Data.nPos_Head)
		{
			memcpy_s(audio_Data.pData + audio_Data.nPos_Tail, MAX_BUFF_LEN, pBuffer, lBufferSize);
			audio_Data.nPos_Tail += lBufferSize;
			audio_Data.nTotalLen += lBufferSize;
		}
		else {
			Sleep(5);
			//printf("头在后 尾部超过头部\n ");
		}
	}
	SetEvent(AdsoundCapStruct.hIevent);
	ReleaseMutex(AdsoundCapStruct.hMutex);
}

//FILE *pcmFile = NULL;
//int pcmFileLen;
void Dsoundcap::ApullFrameDate(LPBYTE pdesData, INT ndesLen)
{	
	//if (pcmFile == NULL)
	//{
	//	pcmFile = fopen("pcm48k16b2c", "wb");
	//	pcmFileLen = 0;
	//}
	//else {
	//	fwrite(pdesData, 1, ndesLen, pcmFile);
	//	pcmFileLen += ndesLen;
	//}
	//if (pcmFileLen>=1920000)//10s
	//{
	//	fflush(pcmFile);
	//	fclose(pcmFile);
	//}	
	while (SystemRunning)
	{
		WaitForSingleObject(AdsoundCapStruct.hMutex, INFINITE);
		if (audio_Data.nTotalLen >= ndesLen)
		{
			if (audio_Data.nPos_Head <=audio_Data.nPos_Tail)			//头在前
			{
				memcpy_s(pdesData, ndesLen, audio_Data.pData + audio_Data.nPos_Head, ndesLen);
				audio_Data.nPos_Head += ndesLen;
				audio_Data.nTotalLen -= ndesLen;
			}
			else//头在后
			{
				//超出边界
				if (audio_Data.nPos_Head + ndesLen > audio_Data.nPos_End)
				{
					//取到边界的数
					int ncopy = audio_Data.nPos_End - audio_Data.nPos_Head;
					if (ncopy>0)
					{
						memcpy_s(pdesData, ndesLen, audio_Data.pData + audio_Data.nPos_Head, ncopy);
					}					
					audio_Data.nPos_Head = 0; 
					audio_Data.nTotalLen -= ncopy;
					int ncopyed = ncopy;
					//取剩余数据
					ncopy = ndesLen - ncopy;
					memcpy_s(pdesData+ ncopyed, ndesLen, audio_Data.pData, ncopy);
					audio_Data.nPos_Head += ncopy;
					audio_Data.nTotalLen -= ncopy;
				}
				else
				{
					memcpy_s(pdesData, ndesLen, audio_Data.pData + audio_Data.nPos_Head, ndesLen);
					audio_Data.nPos_Head += ndesLen;
					audio_Data.nTotalLen -= ndesLen;
				}
			}
		}
		else
		{			
			ResetEvent(AdsoundCapStruct.hIevent);
			ReleaseMutex(AdsoundCapStruct.hMutex);
			WaitForSingleObject(AdsoundCapStruct.hIevent, A_PTS_DELAYTIME);
			continue;
		}
		ReleaseMutex(AdsoundCapStruct.hMutex);
		return;
	}	
}
