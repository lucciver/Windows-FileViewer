# Windows-FileViewer
An command line utility which opens any file extension in its associated application
========================================================================
    CONSOLE APPLICATION : MagicApp Project Overview
========================================================================

Files overview
1. AppLauncher   -  Wrapper class which contains COM object and Smart COM initiazer object which finds the IAssoc handler for the given extension. 
2. Processref    -  Process reference COM object to avoid dangling reference.
3. SmartCoinit   -  Smart Class to wrap COM init/uninit to avoid memory leaks.
4. Utility       -  Helper functions to parse, validate the path/relative path and finding the host OS (XP or WinNT).
5. MessageString -  Help and error messages.
6. Main 		 -  Takes user input and calls AppLauncher to launch.

Usage -

1. Open Cmd.exe -> MagicApp.exe FILENAME (with or without quotes)
2. Double click MagicApp.exe : It will ask for user input i.e. FileName ( without quotes only)

	reason - (Double quotes are only needed for shell parsers that use a space as a delimiter for commands and parameters. )
https://groups.google.com/forum/#!topic/microsoft.public.vb.general.discussion/aR71k4-L4YE

3. When ask for user input , following actions are supported -
	a. File path ( absolute and relative)
	b. Help
	c. Quit

Test Method -
1. In Main.cpp - Turn "TESTMODE" ON or OFF (1 or 0)

The system has been tested with following test cases
1. different file types {doc,pdf,png,wmv,mp3,lua,ini,txt,pptx,xls}
2. spaces at the begining, end and middle
3. Change the default file type from properties to different application 
4. Relative path is supported
5. Incorrect file path , extensions

	
References :
https://www.codeproject.com/Articles/13601/COM-in-plain-C [Basic of COM] 
https://msdn.microsoft.com/en-us/library/office/cc839627.aspx [Implementnig IUknown]
https://stackoverflow.com/questions/37417757/extract-icon-from-uwp-application [To get the IAssoHandlers]
https://blogs.msdn.microsoft.com/oldnewthing/20080528-00/?p=22163/ [For SHSetInstanceExplorer]
https://msdn.microsoft.com/en-us/library/windows/desktop/ms644928(v=vs.85).aspx#creating_loop [Message loop]
