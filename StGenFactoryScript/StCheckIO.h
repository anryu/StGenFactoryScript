#pragma once
#include "stcheckbase.h"
#include "StSerialComm.h"
//▼1.0.0.1053
#include "StTerminal.h"
//▲1.0.0.1053

class StCheckIO :
	public StCheckBase
{
public:
	StCheckIO(void);
	StCheckIO(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog=NULL);
	~StCheckIO(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
 
	BOOL Execute(void);

	void SetSerialComm(StSerialComm *pSerialComm)
	{
		m_pSerialComm = pSerialComm;
	}
	//▼1.0.0.1053
	void SetTerminal(StTerminal *pTerminal)
	{
		m_pTerminal = pTerminal;
	}
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg,double &dblValue);
	//▲1.0.0.1053
private:
	void Initialize(void);
	StSerialComm *m_pSerialComm;

	//▼1.0.0.1053
	BOOL SerialExecute(void);
	BOOL TerminalExecute(void);
	StTerminal *m_pTerminal;
	INT m_nMode;	//0:Serial冶具　1:ラズパイ冶具
	INT m_nCheckWaitTime;

	INT m_nCameraStatus;
	INT m_nCameraSetValue;
	INT m_nJiguStatus;
	INT m_nJiguSetValue;
	//▲1.0.0.1053

	BOOL CreatePinBuffer(void);
	INT m_nPinCount;
	PINT m_piPinMode;	//0:In 1:Out ----2:InOutは未定
};

