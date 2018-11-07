#pragma once
#include "stcheckbase.h"
class StCheckTemperature :
	public StCheckBase
{
public:
	StCheckTemperature(void);
	StCheckTemperature(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog=NULL);
	~StCheckTemperature(void);

	//---ãKäiê›íË-------------------------------
	void SetMin(INT nData){m_nMinTemperature = nData;}
	void SetMax(INT nData){m_nMaxTemperature = nData;}
	void SetMode(INT nData){m_nMode = nData;}
	void SetCommand(LPCTSTR szCommand){m_szCommand = szCommand;}
	//Å•1.0.0.1022
	void SetRecieve(LPCTSTR szRecieve){m_szRecieve = szRecieve;}
	//Å£1.0.0.1022
	INT GetResultTemperature(void){return m_nResultTemperature;}

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
    //Å•1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);
	//Å£1.0.0.1025

	BOOL Execute(void);
private:
	void Initialize(void);
	BOOL ExecuteEllito(INT &nValue);

	//ãKäiíl-----
	INT m_nMinTemperature;
	INT m_nMaxTemperature;

	INT m_nMode;

	CString m_szCommand;
	//Å•1.0.0.1022
	CString m_szRecieve;
	//Å£1.0.0.1022
	//Å•1.0.0.1046
	BOOL ExecuteGenICam(INT &nValue);

	CString m_szSelectorFeature;
	CString m_szSelectorSelect;
	DWORD m_dwAddress;
	//Å£1.0.0.1046

	INT m_nResultTemperature;

};

