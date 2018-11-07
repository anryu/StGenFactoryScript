#pragma once

#include "PowerCtrl.h"
#include "../CommonHeader/StString.h"

class StTexioControl
{
public:
	StTexioControl(void);
	//Å•1.0.0.1062
	//StTexioControl(LPCTSTR szClassName, LPCTSTR szVarName);
	//Å•1.0.0.1069
	//StTexioControl(LPCTSTR szClassName, LPCTSTR szVarName, INT nScriptCheckMode=0);
	StTexioControl(LPCTSTR szClassName, LPCTSTR szVarName, INT nScriptCheckMode=0, INT nPCAddress = 1, INT nSystemAddress = 1);
	//Å£1.0.0.1069
	//Å£1.0.0.1062
	~StTexioControl(void);

	//Å•1.0.0.1069
	BOOL Connect(void);
	BOOL IsConnected(void);
	//Å£1.0.0.1069
	BOOL PowerOn(void);
	BOOL PowerOff(void);
	BOOL SetVoltage(double aVoltage);
	BOOL SetVoltage(LPCTSTR szVoltage);
	BOOL GetVoltage(double *aVoltage);
	BOOL GetCurrent(double *aCurrent);
	//Å•1.0.0.1060
	BOOL SetCurrentLimit(double aCurrentLimit);
	BOOL SetCurrentLimit(LPCTSTR szCurrentLimit);
	BOOL GetCurrentLimit(double *aCurrentLimit);
	//Å£1.0.0.1060
	//Å•1.0.0.1069
	BOOL SetPCAddress(INT nAddress);
	BOOL SetPCAddress(LPCTSTR szCurrentLimit);
	BOOL SetSystemAddress(INT nAddress);
	BOOL SetSystemAddress(LPCTSTR szCurrentLimit);
	BOOL Reconnect(void);
	//Å£1.0.0.1069
	//Å•1.0.0.1018
	BOOL MainPowerOn(void);
	BOOL MainPowerOff(void);
	BOOL PowerOn(LPCTSTR szChannel);
	BOOL PowerOff(LPCTSTR szChannel);
	//Å£1.0.0.1018

	//Å•1.0.0.1013
	BOOL SetChannel(char channel);
	BOOL SetChannel(LPCTSTR szChannel);
	BOOL GetChannel(PBYTE aChannel);
	//Å£1.0.0.1013

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);

	StString m_ClassName;
	StString m_VarName;

private:
	CPowerCtrl	m_PowerCtrl;
	//Å•1.0.0.1062
	INT m_nScriptCheckMode;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	INT m_nPCAddress;
	INT m_nSystemAddress;
	//Å£1.0.0.1069

};

