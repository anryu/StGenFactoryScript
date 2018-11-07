#pragma once
#include "stcheckbase.h"
class StCheckEllitoLVDS :
	public StCheckBase
{
public:
	StCheckEllitoLVDS(void);
	StCheckEllitoLVDS(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase);
	~StCheckEllitoLVDS(void);

	BOOL Execute(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);

	BOOL GetResultText( INT nIndex, LPTSTR szResult, size_t size );


private:
	void Initialize(void);

	char m_delay[33];
	char m_clock[5];
	char m_sync[5];
	//char m_defDelay[33];
	//char m_defClock[5];
	//char m_defSync[5];
	WORD m_wLMAP[33];//LMAP

	//std::vector <CString> vszStac;
};

