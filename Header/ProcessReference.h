#pragma once

/**************************************************************************//*
	An IUnknown process object. 
	While calling SHGetInstanceExplorer , shell extension will get a pointer to this object. 
	When the object is destructed, it unregisters itself as the process reference
	and waits for the reference count to drop to zero. 
****************************************************************************/

class ProcessReference : public IUnknown
{
public:
	HRESULT  __stdcall QueryInterface(REFIID riid, void **ppv)
	{
		if (riid == IID_IUnknown) {
			*ppv = static_cast<IUnknown*>(this);
			AddRef();
			return S_OK;
		}
		*ppv = NULL; return E_NOINTERFACE;
	}

	ULONG  __stdcall AddRef()
	{
		return InterlockedIncrement(&m_ref);
	}

	
	ULONG  __stdcall Release()
	{
		LONG lRef = InterlockedDecrement(&m_ref);
		/*posts a dummy thread message so that the  
			message loop will wake up.*/
		if (lRef == 0) 
			PostThreadMessage(m_thread, WM_NULL, 0, 0);
		return lRef;
	}

	ProcessReference()
		: m_ref(1), m_thread(GetCurrentThreadId())
	{
		SHSetInstanceExplorer(this);
	}

	~ProcessReference()
	{
		SHSetInstanceExplorer(NULL);
		Release();

		MSG msg;
		while (m_ref && GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

private:
	LONG m_ref;
	DWORD m_thread;
};