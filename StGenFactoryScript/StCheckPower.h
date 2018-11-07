#pragma once
#include "stcheckbase.h"
#include "StSerialComm.h"
#include "StTexioControl.h"
//▼1.0.0.1052
#include "StInterfaceGenICam.h"
//▲1.0.0.1052

class StCheckPower :
	public StCheckBase
{
public:
	StCheckPower(void);
	StCheckPower(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog=NULL);
	~StCheckPower(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
  	//▼1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);
	//▲1.0.0.1025

	BOOL Execute(void);

	void SetSerialComm(StSerialComm *pSerialComm)
	{
		m_pSerialComm = pSerialComm;
	}
	void SetTexioControl(StTexioControl *pTexioControl)
	{
		m_pTexioControl = pTexioControl;
	}

	//---結果取得-------------------------------
	//▼1.0.0.1048
	//INT GetCurrent(void){return m_nResultCurrent;}
	double GetCurrent(void){return m_dblResultCurrent;}
	//▲1.0.0.1048
	//▼1.0.0.1052
	void SetInterfaceGenICam(StInterfaceGenICam *aInterfaceGenICam)
	{
		m_InterfaceGenICam = aInterfaceGenICam;
	}
	//▲1.0.0.1052

private:
	void Initialize(void);
	StSerialComm *m_pSerialComm;
	StTexioControl *m_pTexioControl;
	//▼1.0.0.1052
	StInterfaceGenICam *m_InterfaceGenICam;
	StString szCommand;
	//▲1.0.0.1052

	//モード---------
	INT m_nMode;	//0:ラインセンサー用治具 1:GigE用治具

	//規格値-----
	//▼1.0.0.1052
	//INT m_nCurrentMax;
	//INT m_nCurrentMin;
	double m_dblCurrentMax;
	double m_dblCurrentMin;
	//▲1.0.0.1052

	//結果-----
	//▼1.0.0.1048
	//INT m_nResultCurrent;
	//BOOL GetCurrentFromComm(INT *piCurrent);
	double m_dblResultCurrent;
	BOOL GetCurrentFromComm(double *pdblCurrent);
	//▲1.0.0.1048
	BOOL GetCurrentFromLineComm(INT *piCurrent);
	BOOL GetCurrentFromPoEComm(INT *piCurrent);
	BOOL GetCurrentFromTexio(INT *piCurrent);
	//▼1.0.0.1048
	BOOL GetCurrentFromPoEComm2(double *pdblCurrent);
	//▲1.0.0.1048
	//▼1.0.0.1052
	BOOL GetCurrentFromInterface(double *pdblCurrent);
	//▲1.0.0.1052

};


