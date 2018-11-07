#pragma once
#include "stcheckbase.h"
class StCheckBaudrate :
	public StCheckBase
{
public:
	StCheckBaudrate(void);
	StCheckBaudrate::StCheckBaudrate(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase);
	~StCheckBaudrate(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	BOOL Execute(void);
private:
	void Initialize(void);

//	StSerialComm *m_SerialComm;

};

