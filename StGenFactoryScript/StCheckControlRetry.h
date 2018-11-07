#pragma once
#include "stcheckbase.h"
class StCheckControlRetry :
	public StCheckBase
{
public:
	StCheckControlRetry(void);
	StCheckControlRetry(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog=NULL);
	~StCheckControlRetry(void);

	BOOL Execute(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	//▼1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg,double &dblValue);
	//▲1.0.0.1025

	void SetTimer(INT nTimer){m_nTimer = nTimer;}
	void SetRetry(INT nRetry){m_nRetry = nRetry;}
	void SetBusyCommand(LPCTSTR szData){m_szBusyCommand = szData;}
	void SetBusyStatus(LPCTSTR szData){m_szBusyStatus = szData;}

	void SetSendData(LPCTSTR szData){m_szSend = szData;}
	void SetRecieveData(LPCTSTR szData){m_szReturn = szData;}
	void SetInspectionMain(void *pInspectionMain){m_pInspectionMain = pInspectionMain;}

	void GetSendData(CString &szData){szData = m_szSend;}
	void GetReturn(CString &szData){szData = m_szReturn;}
	void GetResult(CString &szData){szData = m_szResult;}	

	INT GetRetry(void){return m_nRetry;}
	void GetBusyResult(CString &szData){szData = m_szBusyResult;}
	//▼1.0.0.1020
	void GetLastRecieveData(CString &szData){szData = m_szLastRecieveData;}
	//▲1.0.0.1020
	//▼1.0.0.1022
	void GetLastRecieveValue(INT &szData){szData = m_nLastRecieveValue;}
	//▲1.0.0.1022

	//▼1.0.0.1021
	BOOL SetCommTimeout(DWORD dwReadIntervalTimeout,DWORD dwReadTotalTimeoutConstant,DWORD dwReadTotalTimeoutMultiplier,DWORD dwWriteTotalTimeoutConstant,DWORD dwWriteTotalTimeoutMultiplier);
	BOOL GetCommTimeout(PDWORD dwReadIntervalTimeout,PDWORD dwReadTotalTimeoutConstant,PDWORD dwReadTotalTimeoutMultiplier,PDWORD dwWriteTotalTimeoutConstant,PDWORD dwWriteTotalTimeoutMultiplier);
	//▲1.0.0.1021

private:
	INT m_nTimer;
	INT m_nRetry;
	CString m_szBusyCommand;
	CString m_szBusyStatus;
	CString m_szBusyResult;
	CString m_szSend;
	CString m_szReturn;
	CString m_szResult;
	void Initialize(void);
	void *m_pInspectionMain;

	//▼1.0.0.1020
	CString m_szLastRecieveData;
	//▲1.0.0.1020
	//▼1.0.0.1022
	INT m_nLastRecieveValue;
	//▲1.0.0.1022

	//▼カメラリンク用　eBUSでは使用しない--Ascii通信用--------------
	CString m_szSendSTX;
	CString m_szSendETX;
	CString m_szRecieveSTX;
	CString m_szRecieveETX;
	void SetSendSTX(LPCTSTR szData){m_szSendSTX = szData;}
	void SetSendETX(LPCTSTR szData){m_szSendETX = szData;}
	void SetRecieveSTX(LPCTSTR szData){m_szRecieveSTX = szData;}
	void SetRecieveETX(LPCTSTR szData){m_szRecieveETX = szData;}
	//▼1.0.0.1015
	CString m_szRcvReplaceSpace;
	void SetRcvReplaceSpace(LPCTSTR szData){m_szRcvReplaceSpace = szData;}
	//▲1.0.0.1015

	//▼1.0.0.1047
	BOOL m_bMainControl;
	BOOL m_bRetryControl;
	//▲1.0.0.1047


};

