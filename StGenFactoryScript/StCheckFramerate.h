#pragma once
#include "stcheckbase.h"
class StCheckFramerate :
	public StCheckBase
{
public:
	StCheckFramerate(void);
	StCheckFramerate(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase);
	~StCheckFramerate(void);

	//---ÉpÉâÉÅÅ[É^ê›íË-------------------------------
	void SetMax(double dblData);
	void SetMin(double dblData);
	BOOL Execute(void);
	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);

	double GetFramerate(void){return m_dblResultFramerate;}
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);

private:
	void Initialize(void);

	double m_dblMaxFramerate;
	double m_dblMinFramerate;
	//åãâ 
	double m_dblResultFramerate;
	CString m_szCommand;

};

