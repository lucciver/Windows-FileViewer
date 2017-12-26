#pragma once

#ifndef UTILITY_H
#define UTILITY_H

#include "MessageStrings.h"

#define MAX_LEN 512


using namespace std;

namespace Utility
{


/**************************************************************************//*
* * Find the current OS is Windows 2000 or later
*
* @retval           Success Windows
*
****************************************************************************/
	static bool Is_Win_Server()
	{
		OSVERSIONINFOEX osvi;
		DWORDLONG dwlConditionMask = 0;
		int op = VER_GREATER_EQUAL;

		// Initialize the OSVERSIONINFOEX structure.

		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		osvi.dwMajorVersion = 5;
		osvi.dwMinorVersion = 0;
		osvi.wServicePackMajor = 0;
		osvi.wServicePackMinor = 0;
		osvi.wProductType = VER_NT_SERVER;

		// Initialize the condition mask.

		VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, op);
		VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, op);
		VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMAJOR, op);
		VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMINOR, op);

		// Perform the test.

		return VerifyVersionInfo(
			&osvi,
			VER_MAJORVERSION | VER_MINORVERSION |
			VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR,
			dwlConditionMask) ? true :false;
	}

	static bool caseInsensitiveCmp(wstring str1, wstring str2)
	{
		if (str1.size() != str2.size())
			return false;
		else
		return (str1.empty() | str2.empty()) ? false :
			std::equal(str1.begin(), str1.end(),
			str2.begin(),
			[](wchar_t a, wchar_t b)
		{
			return tolower(a) == tolower(b);
		});
	}

	static wstring getDefaultAssociation(LPCWSTR extension)
	{
		LPWSTR buffer = new wchar_t[MAX_LEN];
		DWORD bufferLen = MAX_LEN;

		HRESULT hres = AssocQueryStringW(0, ASSOCSTR::ASSOCSTR_EXECUTABLE, extension, NULL, buffer, &bufferLen);
		std::wstringstream stream;

		if (S_OK == hres)
			stream << buffer;
		else
			wcout << GetLastError();

		delete buffer;
		return stream.str();
	}

	static bool getAbsolutePath(wstring& ws)
	{
		wchar_t full[_MAX_PATH];
		bool hasValueChanged = false;
		if (_wfullpath(full, ws.data(), _MAX_PATH) != NULL)
		{
			ws = full;
			hasValueChanged = true;
		}
		return hasValueChanged;
	}

	static bool validateFilePath(std::wstring &fpath)
	{
		bool isValid = false;
		
		fpath = regex_replace(fpath, std::wregex(L"^ +| +$|( ) +"), L"$1");
		
		if (fpath.find_first_of(L".") != string::npos)
			getAbsolutePath(fpath);
		
		if (fpath.empty())
			wcout << InvalidArgs;

		else if (caseInsensitiveCmp(fpath, L"Help"))
			wcout << HelpString;

		else if (PathFileExistsW(fpath.data()))
		{
			isValid = true;; //Success
		}
		else
			wcout << FoundError << fpath << endl;

		return isValid;
	};

	

}
#endif 