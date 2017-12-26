#include "stdafx.h"
#include "AppLauncher.h"

using namespace std;
using namespace Utility;

/**************************************************************************//*
* * Calls the launcher based on current OS
*
* @retval           Success means XP else later
*
****************************************************************************/
void AppLauncher::launch(const wstring& filePath)const
{
	LPWSTR path = const_cast<LPWSTR>(filePath.c_str());

	if (Is_Win_Server())
		launchAppinWinNT(path);
	else
		launchAppinXP(path);
}

bool AppLauncher::shouldIQuit(const std::wstring &fpath) const
{
	return caseInsensitiveCmp(fpath, L"Quit");
}

bool AppLauncher::launchAppinProcess(LPWSTR lpszCommandLine)const
{
	bool rc = FALSE;

	if (lpszCommandLine && (lpszCommandLine[0] != _T('\0')))
	{
		STARTUPINFOW si = { 0 };
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOWNORMAL;

		PROCESS_INFORMATION pi = { 0 };

		rc = ::CreateProcessW(NULL,lpszCommandLine, NULL, NULL, FALSE,
			NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi) ?true :false;
		
		// close process and thread handles now (app will continue to run)
		if (pi.hProcess)
			::CloseHandle(pi.hProcess);
		if (pi.hThread)
			::CloseHandle(pi.hThread);
	}
	return rc;
}

/**************************************************************************//*
* * Finds default IAssocHandler handler for specified by the extension.
*   retunrs the Handler whose executable path matches with default exe path
*   returned by AssocQueryString .
*
* @param[in]        extension name
* @param[in]        Recommended handlers filter
*
* @retval          CComPtr<IAssocHandler>   default found handle
*
****************************************************************************/
CComPtr<IAssocHandler> AppLauncher::findDefaultHandle
							(PCWSTR extension, ASSOC_FILTER filter)const
{

	wstring defaultExePath(getDefaultAssociation(extension));

	CComPtr<IEnumAssocHandlers> enumerator;
	
	if (SUCCEEDED(SHAssocEnumHandlers(extension, filter, &enumerator)))
	{
		for (CComPtr<IAssocHandler> handler;
			enumerator->Next(1, &handler, nullptr) == S_OK;
			handler.Release())
		{
			LPWSTR execPath;
			handler->GetName(&execPath);

			wstring recomExepath(execPath);
			if (defaultExePath.compare(recomExepath) == 0)
			{
				wcout << ExePathString << recomExepath << endl;
				return handler;;;
			}		

		}
	}
	return (CComPtr<IAssocHandler>)nullptr;
}

/**************************************************************************//*
* * Gets an object that can be used to carry out actions on the specified file
*
* @param[in]        hwnd handle to owner window
* @param[in]        LPCWSTR specified file
* @param[in]        REFIID A reference to the IID
* @param[in]        void ** To get interface pointer requested in riid

* @retval           HRESULT   result (S_OK or Error code)
*
****************************************************************************/

HRESULT AppLauncher::GetUIObjectOfFile(HWND hwnd, LPCWSTR pszPath, REFIID riid, void **ppv)const
{
	*ppv = NULL;
	LPITEMIDLIST pidl;
	SFGAOF sfgao;

	auto hr = SHParseDisplayName(pszPath, NULL, &pidl, 0, &sfgao);

	if (hr >= 0)
	{
		IShellFolder *psf;
		LPCITEMIDLIST pidlChild;
		hr = SHBindToParent(pidl, IID_IShellFolder, (void**)&psf, &pidlChild);

		if (hr >= 0)
		{
			hr = psf->GetUIObjectOf(hwnd, 1, &pidlChild, riid, NULL, ppv);
			psf->Release();
		}
		CoTaskMemFree(pidl);
	}
	return hr;
}

/**************************************************************************//*
* * Launch the passed filename in WinNT or later
*
* @retval           Success means XP else later
*
****************************************************************************/
void AppLauncher::launchAppinWinNT(LPWSTR filepath)const
{
	LPWSTR extension = PathFindExtensionW(filepath);

	//3. Find the associated handler
	auto handler = findDefaultHandle(extension, ASSOC_FILTER_NONE);

	if (handler != nullptr)
	{
		CComPtr<IDataObject> dobj;
		
		if (S_OK == GetUIObjectOfFile(nullptr, filepath, IID_PPV_ARGS(&dobj)))
		{
			//4. invoke the handler to launch app
			auto res = handler->Invoke(dobj);
		}
		handler.Release();
	}
	else
		wcout << NoDefaultFoundError;
}

/**************************************************************************//*
* * * * Launch the passed filename in XP
*
* @retval           Success means XP else later
*
****************************************************************************/
void AppLauncher::launchAppinXP(LPWSTR filepath)const
{
	wstring defaultProcess{ getDefaultAssociation(filepath) };

	if (!defaultProcess.empty())
	{
		wstringstream wss;
		wss << defaultProcess << L"   " << filepath;

		launchAppinProcess(&wss.str()[0]);
	}

}

