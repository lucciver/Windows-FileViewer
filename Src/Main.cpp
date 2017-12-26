// FileBrwose.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AppLauncher.h"
#include "MessageStrings.h"

using namespace std;

#define welcomeStr	"\n\n\t\t________________________________________________\n"\
					"\n\n\t\tWelcome to MagicApp, an easy App to launch files\n" \
					" \n\n\t\t________________________________________________\n"\

#define TESTMODE 0

#if TESTMODE
vector<wstring> testStrings = { L"Quit",
								L"C:\\windws\\win.ini",
								L"C:\\windws\\win.ini",
								L"C:/windws/win.ini",
								L"C:/windws/win.abc",
								L"D:\\test.pptx",
								L"D:/test.lua",
								L"D:\\test\\test.lua",
								L"D:\\test\\test.txt",
								L"D:\\test\\test.pdf",
								L"D:\\test\\test.pptx",
								L"D:\\test\\test.wmv",
								L"D:\\test\\test.jpg",
								L"C:\\Users\\rkumar\\Documents\\visual studio 2013\\Projects\\FileBrwose\\test\\test.wmv",
								L".\test\test.wmv",
								L"D:\\test\\test.pdf",
								L"Help"	
							};
#endif

auto userInput()->wstring
{
	wcout << userInputString;
	wstring fname;
#if TESTMODE
	fname = testStrings.at(testStrings.size()-1);
	testStrings.pop_back();
#else 
	getline(std::wcin, fname);
#endif
	
	return fname;
}; 


int main(int argc, wchar_t *argv[], wchar_t *envp[])
{
	cout << welcomeStr;
	wstring filePath{ L"" };

	if (argc >= 2)
		filePath = argv[1];
	
	AppLauncher app;
	while (!app.shouldIQuit(filePath))
	{
		if (Utility::validateFilePath(filePath))
		{
			app.launch(filePath);
			filePath.clear();
		}
			filePath = userInput();
	} ;
	wcout << QuitString;
	return 1;
}