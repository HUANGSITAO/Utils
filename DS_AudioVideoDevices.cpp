#include "stdafx.h"
#include "DS_AudioVideoDevices.h"  

#pragma comment(lib, "Strmiids.lib")  

HRESULT DS_GetAudioVideoInputDevices(std::vector<TDeviceName> &vectorDevices, REFGUID guidValue)
{
	TDeviceName name;
	HRESULT hr;

	// 初始化  
	vectorDevices.clear();

	// 初始化COM  
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(hr))
	{
		return hr;
	}

	// 创建系统设备枚举器实例  
	ICreateDevEnum *pSysDevEnum = NULL;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&pSysDevEnum);
	if (FAILED(hr))
	{
		CoUninitialize();
		return hr;
	}

	// 获取设备类枚举器  
	IEnumMoniker *pEnumCat = NULL;
	hr = pSysDevEnum->CreateClassEnumerator(guidValue, &pEnumCat, 0);
	if (hr == S_OK)
	{
		// 枚举设备名称  
		IMoniker *pMoniker = NULL;
		ULONG cFetched;
		while (pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
		{
			IPropertyBag *pPropBag;
			hr = pMoniker->BindToStorage(NULL, NULL, IID_IPropertyBag, (void **)&pPropBag);
			if (SUCCEEDED(hr))
			{
				// 获取设备友好名  
				VARIANT varName;
				VariantInit(&varName);

				hr = pPropBag->Read(L"FriendlyName", &varName, NULL);
				if (SUCCEEDED(hr))
				{
					StringCchCopy(name.FriendlyName, MAX_FRIENDLY_NAME_LENGTH, varName.bstrVal);

					// 获取设备Moniker名  
					LPOLESTR pOleDisplayName = reinterpret_cast<LPOLESTR>(CoTaskMemAlloc(MAX_MONIKER_NAME_LENGTH * 2));
					if (pOleDisplayName != NULL)
					{
						hr = pMoniker->GetDisplayName(NULL, NULL, &pOleDisplayName);
						if (SUCCEEDED(hr))
						{
							StringCchCopy(name.MonikerName, MAX_MONIKER_NAME_LENGTH, pOleDisplayName);
							vectorDevices.push_back(name);
						}

						CoTaskMemFree(pOleDisplayName);
					}
				}

				VariantClear(&varName);
				pPropBag->Release();
			}

			pMoniker->Release();
		} // End for While  

		pEnumCat->Release();
	}

	pSysDevEnum->Release();
	CoUninitialize();

	return hr;
}