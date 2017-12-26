#pragma once

/**************************************************************************//*
* * This class just wwraps the COM init and release methods to avoid any memory leaks*
*
****************************************************************************/
class SmartCoInit
{
public:
	SmartCoInit() : m_hr(CoInitialize(NULL)) 
	{
	}
	
	~SmartCoInit() 
	{ 
		if (m_hr >= 0) 
			CoUninitialize(); 
	}
	
	operator HRESULT() const 
	{
		return m_hr; 
	}
	
	HRESULT m_hr;
};