#pragma once

#include "ProcessReference.h"
#include "SmartCoInit.h"
#include "Utility.h"
using namespace std;

class AppLauncher
{
	using Handler = CComPtr<IAssocHandler>;
public:
	AppLauncher() = default;

	~AppLauncher() = default;

	void launch(const wstring& filePath)const;

	bool shouldIQuit(const wstring &)const;
	
	
private:
	
	void launchAppinWinNT(LPWSTR filepath)const;

	void launchAppinXP(LPWSTR filepath)const;
	
	bool launchAppinProcess(LPWSTR lpszCommandLine)const;

	Handler findDefaultHandle(PCWSTR extension,
								ASSOC_FILTER filter) const;

	HRESULT GetUIObjectOfFile(HWND hwnd, LPCWSTR
					pszPath, REFIID riid, void **ppv) const;

	SmartCoInit m_coInit;
	ProcessReference m_objRef;	
};
