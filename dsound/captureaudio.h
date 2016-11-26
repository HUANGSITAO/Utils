#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "Quartz.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")


#ifndef _CAPTURE_SOUND_H_
#define _CAPTURE_SOUND_H_
#define NUM_REC_NOTIFICATIONS  16
class CAdoFrameHandler {
public:
	virtual void AdoFrameData(BYTE* pBuffer, long lBufferSize) = 0;
};
class CCaptureAudio
{
public:
	BOOL        m_bRecording=false;  //recording now ? also used by event recv thread
protected:
	LPDIRECTSOUNDCAPTURE8    m_pCapDev=NULL;   //capture device ptr
	LPDIRECTSOUNDCAPTUREBUFFER m_pCapBuf=NULL;   //capture loop buffer ptr
	LPDIRECTSOUNDNOTIFY8    m_pNotify=NULL;   //capture auto-notify event callback handler ptr
	GUID        m_guidCapDevId;  //capture device id
	WAVEFORMATEX      m_wfxInput;   //input wave format description struct
	DSBPOSITIONNOTIFY     m_aPosNotify[NUM_REC_NOTIFICATIONS + 1]; //notify flag array 
	HANDLE        m_hNotifyEvent;   //notify event 
	BOOL        m_abInputFmtSupported[20];
	DWORD        m_dwCapBufSize;  //capture loop buffer size 
	DWORD        m_dwNextCapOffset;//offset in loop buffer 
	DWORD        m_dwNotifySize;  //notify pos when loop buffer need to emit the event
	CAdoFrameHandler*     m_frame_handler; // outer frame data dealer ptr 
public: // callback func to add enum devices string name 
	static BOOL CALLBACK enum_dev_proc(LPGUID lpGUID, LPCTSTR lpszDesc,
		LPCTSTR lpszDrvName, LPVOID lpContext);
	static UINT notify_capture_thd(LPVOID data);
protected:
	HRESULT InitDirectSound(GUID dev_id = GUID_NULL);
	HRESULT FreeDirectSound();
	HRESULT InitNotifications();
	HRESULT CreateCaptureBuffer(WAVEFORMATEX * wfx);
	HRESULT StartOrStopRecord(BOOL bStartRec);
	HRESULT RecordCapturedData();
	void    SetWavFormat(WAVEFORMATEX * wfx);
public:
	CCaptureAudio(void);
	~CCaptureAudio(void);
	BOOL EnumDevices(HWND hList);
	BOOL Open(void);
	BOOL Close();
	void GrabAudioFrames(BOOL bGrabAudioFrames, CAdoFrameHandler* frame_handler);
};
#endif
