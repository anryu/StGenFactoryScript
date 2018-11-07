#include "StdAfx.h"
#include "StTexioControl.h"


StTexioControl::StTexioControl(void)
	: m_ClassName(_T(""))
	, m_VarName(_T(""))
{
	m_PowerCtrl.bConnect();
}
//Å•1.0.0.1062
//StTexioControl::StTexioControl(LPCTSTR szClassName, LPCTSTR szVarName)
//Å•1.0.0.1069
//StTexioControl::StTexioControl(LPCTSTR szClassName, LPCTSTR szVarName, INT nScriptCheckMode)
StTexioControl::StTexioControl(LPCTSTR szClassName, LPCTSTR szVarName, INT nScriptCheckMode, INT nPCAddress, INT nSystemAddress)
//Å£1.0.0.1069
//Å£1.0.0.1062
	: m_ClassName(szClassName)
	, m_VarName(szVarName)
	//Å•1.0.0.1062
	, m_nScriptCheckMode(nScriptCheckMode)
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	, m_nPCAddress(nPCAddress)
	, m_nSystemAddress(nSystemAddress)
	//Å£1.0.0.1069
{
	//Å•1.0.0.1069
	////Å•1.0.0.1062
	////m_PowerCtrl.bConnect();
	//if( !m_nScriptCheckMode )
	//	m_PowerCtrl.bConnect();
	////Å£1.0.0.1062
	//Å£1.0.0.1069
}
StTexioControl::~StTexioControl(void)
{
	//Å•1.0.0.1062
	//m_PowerCtrl.bDisconnect();
	if( !m_nScriptCheckMode )
		m_PowerCtrl.bDisconnect();
	//Å£1.0.0.1062
}
//Å•1.0.0.1069
BOOL StTexioControl::Connect(void)
{
	if( m_nScriptCheckMode ) return TRUE;
	m_PowerCtrl.SetPCAddress(m_nPCAddress);
	m_PowerCtrl.SetSystemAddress(m_nSystemAddress);
	return m_PowerCtrl.bConnect();
}
BOOL StTexioControl::IsConnected(void)
{
	if( m_nScriptCheckMode ) return TRUE;
	return m_PowerCtrl.IsConnected();
}
//Å£1.0.0.1069

BOOL StTexioControl::PowerOn(void)
{
	//Å•1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	if( !IsConnected() )
	{
		//Å•1.0.0.1071
		//if( Connect() ) return FALSE;
		//Å•1.0.0.1072
		//if( !Connect() ) return FALSE;
		if( !Connect() )
		{
#ifdef _DEBUG
			return TRUE;
#else
			return FALSE;
#endif
		}
		//Å£1.0.0.1072
		//Å£1.0.0.1071
	}
	//Å£1.0.0.1069
	return m_PowerCtrl.bPowerOn();
}
BOOL StTexioControl::PowerOff(void)
{
	//Å•1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	if( !IsConnected() )
	{
		//Å•1.0.0.1071
		//if( Connect() ) return FALSE;
		//Å•1.0.0.1072
		//if( !Connect() ) return FALSE;
		if( !Connect() )
		{
#ifdef _DEBUG
			return TRUE;
#else
			return FALSE;
#endif
		}
		//Å£1.0.0.1072
		//Å£1.0.0.1071
	}
	//Å£1.0.0.1069
	return m_PowerCtrl.bPowerOff();
}

//Å•1.0.0.1018
BOOL StTexioControl::PowerOn(LPCTSTR szChannel)
{
	//Å•1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	if( !IsConnected() )
	{
		//Å•1.0.0.1071
		//if( Connect() ) return FALSE;
		//Å•1.0.0.1072
		//if( !Connect() ) return FALSE;
		if( !Connect() )
		{
#ifdef _DEBUG
			return TRUE;
#else
			return FALSE;
#endif
		}
		//Å£1.0.0.1072
		//Å£1.0.0.1071
	}
	//Å£1.0.0.1069
	INT aChannel = -1;
	if( _tcslen(szChannel)>0 )
	{
		aChannel = _ttoi(szChannel);
	}
	return aChannel>=0?m_PowerCtrl.bPowerOn(aChannel):m_PowerCtrl.bPowerOn();
}
BOOL StTexioControl::PowerOff(LPCTSTR szChannel)
{
	//Å•1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	if( !IsConnected() )
	{
		//Å•1.0.0.1071
		//if( Connect() ) return FALSE;
		//Å•1.0.0.1072
		//if( !Connect() ) return FALSE;
		if( !Connect() )
		{
#ifdef _DEBUG
			return TRUE;
#else
			return FALSE;
#endif
		}
		//Å£1.0.0.1072
		//Å£1.0.0.1071
	}
	//Å£1.0.0.1069
	INT aChannel = -1;
	if( _tcslen(szChannel)>0 )
	{
		aChannel = _ttoi(szChannel);
	}
	return aChannel>=0?m_PowerCtrl.bPowerOff(aChannel):m_PowerCtrl.bPowerOff();
}
//Å£1.0.0.1018


BOOL StTexioControl::SetVoltage(double aVoltage)
{
	//Å•1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	if( !IsConnected() )
	{
		//Å•1.0.0.1071
		//if( Connect() ) return FALSE;
		//Å•1.0.0.1072
		//if( !Connect() ) return FALSE;
		if( !Connect() )
		{
#ifdef _DEBUG
			return TRUE;
#else
			return FALSE;
#endif
		}
		//Å£1.0.0.1072
		//Å£1.0.0.1071
	}
	//Å£1.0.0.1069
	return m_PowerCtrl.bSetVoltage(aVoltage);
}
BOOL StTexioControl::SetVoltage(LPCTSTR szVoltage)
{
	//Å•1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	if( !IsConnected() )
	{
		//Å•1.0.0.1071
		//if( Connect() ) return FALSE;
		//Å•1.0.0.1072
		//if( !Connect() ) return FALSE;
		if( !Connect() )
		{
#ifdef _DEBUG
			return TRUE;
#else
			return FALSE;
#endif
		}
		//Å£1.0.0.1072
		//Å£1.0.0.1071
	}
	//Å£1.0.0.1069
	double aVoltage = _ttof(szVoltage);
	return m_PowerCtrl.bSetVoltage(aVoltage);
}
BOOL StTexioControl::GetVoltage(double *aVoltage)
{
	//Å•1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	if( !IsConnected() )
	{
		//Å•1.0.0.1071
		//if( Connect() ) return FALSE;
		//Å•1.0.0.1072
		//if( !Connect() ) return FALSE;
		if( !Connect() )
		{
#ifdef _DEBUG
			return TRUE;
#else
			return FALSE;
#endif
		}
		//Å£1.0.0.1072
		//Å£1.0.0.1071
	}
	//Å£1.0.0.1069
	return m_PowerCtrl.bGetVoltage(aVoltage);
}
BOOL StTexioControl::GetCurrent(double *aCurrent)
{
	//Å•1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	if( !IsConnected() )
	{
		//Å•1.0.0.1071
		//if( Connect() ) return FALSE;
		//Å•1.0.0.1072
		//if( !Connect() ) return FALSE;
		if( !Connect() )
		{
#ifdef _DEBUG
			return TRUE;
#else
			return FALSE;
#endif
		}
		//Å£1.0.0.1072
		//Å£1.0.0.1071
	}
	//Å£1.0.0.1069
	return m_PowerCtrl.bGetCurrent(aCurrent);
}

//Å•1.0.0.1060
BOOL StTexioControl::SetCurrentLimit(double aCurrentLimit)
{
	//Å•1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	if( !IsConnected() )
	{
		//Å•1.0.0.1071
		//if( Connect() ) return FALSE;
		//Å•1.0.0.1072
		//if( !Connect() ) return FALSE;
		if( !Connect() )
		{
#ifdef _DEBUG
			return TRUE;
#else
			return FALSE;
#endif
		}
		//Å£1.0.0.1072
		//Å£1.0.0.1071
	}
	//Å£1.0.0.1069
	return m_PowerCtrl.bSetCurrentLimit(aCurrentLimit);
}
BOOL StTexioControl::SetCurrentLimit(LPCTSTR szCurrentLimit)
{
	//Å•1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	if( !IsConnected() )
	{
		//Å•1.0.0.1071
		//if( Connect() ) return FALSE;
		//Å•1.0.0.1072
		//if( !Connect() ) return FALSE;
		if( !Connect() )
		{
#ifdef _DEBUG
			return TRUE;
#else
			return FALSE;
#endif
		}
		//Å£1.0.0.1072
		//Å£1.0.0.1071
	}
	//Å£1.0.0.1069
	double aCurrentLimit = _ttof(szCurrentLimit);
	return m_PowerCtrl.bSetCurrentLimit(aCurrentLimit);
}
BOOL StTexioControl::GetCurrentLimit(double *aCurrentLimit)
{
	//Å•1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	if( !IsConnected() )
	{
		//Å•1.0.0.1071
		//if( Connect() ) return FALSE;
		//Å•1.0.0.1072
		//if( !Connect() ) return FALSE;
		if( !Connect() )
		{
#ifdef _DEBUG
			return TRUE;
#else
			return FALSE;
#endif
		}
		//Å£1.0.0.1072
		//Å£1.0.0.1071
	}
	//Å£1.0.0.1069
	return m_PowerCtrl.bGetCurrentLimit(aCurrentLimit);
}
//Å£1.0.0.1060
//Å•1.0.0.1069
BOOL StTexioControl::SetPCAddress(INT nAddress)
{
	m_nPCAddress = nAddress;
	return TRUE;
}
BOOL StTexioControl::SetPCAddress(LPCTSTR szCurrentLimit)
{
	m_nPCAddress = _ttoi(szCurrentLimit);
	return TRUE;
}
BOOL StTexioControl::SetSystemAddress(INT nAddress)
{
	m_nSystemAddress = nAddress;
	return TRUE;
}
BOOL StTexioControl::SetSystemAddress(LPCTSTR szCurrentLimit)
{
	m_nSystemAddress = _ttoi(szCurrentLimit);
	return TRUE;
}
BOOL StTexioControl::Reconnect(void)
{
	if( m_nScriptCheckMode ) return TRUE;

	return TRUE;
}
//Å£1.0.0.1069


//Å•1.0.0.1018
BOOL StTexioControl::MainPowerOn(void)
{
	//Å•1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	if( !IsConnected() )
	{
		//Å•1.0.0.1072
		//if( !Connect() ) return FALSE;
		if( !Connect() )
		{
#ifdef _DEBUG
			return TRUE;
#else
			return FALSE;
#endif
		}
		//Å£1.0.0.1072
	}
	//Å£1.0.0.1069
	return m_PowerCtrl.bMainPowerOn();
}

BOOL StTexioControl::MainPowerOff(void)
{
	//Å•1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	if( !IsConnected() )
	{
		//Å•1.0.0.1072
		//if( !Connect() ) return FALSE;
		if( !Connect() )
		{
#ifdef _DEBUG
			return TRUE;
#else
			return FALSE;
#endif
		}
		//Å£1.0.0.1072
	}
	//Å£1.0.0.1069
	return m_PowerCtrl.bMainPowerOff();
}
//Å£1.0.0.1018


//Å•1.0.0.1013
BOOL StTexioControl::SetChannel(char channel)
{
	//Å•1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	if( !IsConnected() )
	{
		//Å•1.0.0.1072
		//if( !Connect() ) return FALSE;
		if( !Connect() )
		{
#ifdef _DEBUG
			return TRUE;
#else
			return FALSE;
#endif
		}
		//Å£1.0.0.1072
	}
	//Å£1.0.0.1069
	m_PowerCtrl.bSetChannel(channel);
	return TRUE;
}
BOOL StTexioControl::SetChannel(LPCTSTR szChannel)
{
	//Å•1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	if( !IsConnected() )
	{
		//Å•1.0.0.1072
		//if( !Connect() ) return FALSE;
		if( !Connect() )
		{
#ifdef _DEBUG
			return TRUE;
#else
			return FALSE;
#endif
		}
		//Å£1.0.0.1072
	}
	//Å£1.0.0.1069
	BYTE aChannel = _ttoi(szChannel);
	m_PowerCtrl.bSetChannel(aChannel);
	return TRUE;
}

BOOL StTexioControl::GetChannel(PBYTE aChannel)
{
	//Å•1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//Å£1.0.0.1062
	//Å•1.0.0.1069
	if( !IsConnected() )
	{
		//Å•1.0.0.1072
		//if( !Connect() ) return FALSE;
		if( !Connect() )
		{
#ifdef _DEBUG
			return TRUE;
#else
			return FALSE;
#endif
		}
		//Å£1.0.0.1072
	}
	//Å£1.0.0.1069
	*aChannel = m_PowerCtrl.bGetChannel();
	return TRUE;
}
//Å£1.0.0.1013

BOOL StTexioControl::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction == _T("Execute") )
	{
	}
	//Å•1.0.0.1018
	else
	if( szFunction=="MainPowerOn" )
	{
		bReval = MainPowerOn();
	}
	else
	if( szFunction=="MainPowerOff" )
	{
		bReval = MainPowerOff();
	}
	else
	if( szFunction=="PowerOn" )
	{
		bReval = PowerOn(szArgument);
	}
	else
	if( szFunction=="PowerOff" )
	{
		bReval = PowerOff(szArgument);
	}
	//else
	//if( szFunction=="PowerOn" )
	//{
	//	bReval = PowerOn();
	//}
	//else
	//if( szFunction=="PowerOff" )
	//{
	//	bReval = PowerOff();
	//}
	//Å£1.0.0.1018
	else
	if( szFunction=="SetVoltage" )
	{
		bReval = SetVoltage(szArgument);
	}
	//Å•1.0.0.1013
	else
	if( szFunction=="SetChannel" )
	{
		bReval = SetChannel(szArgument);
	}
	//Å£1.0.0.1013
	//Å•1.0.0.1060
	else
	if( szFunction=="SetCurrentLimit" || szFunction=="CurrentLimit" )
	{
		bReval = SetCurrentLimit(szArgument);
	}
	//Å£1.0.0.1060
	//Å•1.0.0.1069
	else
	if( szFunction=="SetPCAddress" || szFunction=="PCAddress" )
	{
		bReval = SetPCAddress(szArgument);
	}
	else
	if( szFunction=="SetSystemAddress" || szFunction=="SystemAddress" )
	{
		bReval = SetSystemAddress(szArgument);
	}
	else
	if( szFunction=="Reconnect" )
	{
		bReval = Reconnect();
	}
	//Å£1.0.0.1069

	return bReval;

}