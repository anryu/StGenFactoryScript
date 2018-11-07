#pragma once
#include "stcheckbase.h"
class StCheckPixelCorrect :
	public StCheckBase
{
public:
	StCheckPixelCorrect(void);
	StCheckPixelCorrect(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase);
	~StCheckPixelCorrect(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	BOOL Execute(void);
	BOOL Execute(LPCTSTR szSensorSerialNumber);

	void SetInspectionMain(void *pInspectionMain){m_pInspectionMain = pInspectionMain;}

private:
	void Initialize(void);

	//パラメータ
	INT m_nMode;		//0:タカノEllito 1～:未対応
	INT m_nSelectMode;	//0:ファイル自動選択 1:ファイル手動選択

	CString m_szDirectory;
	CString m_szSensorSerialNumber;

	void SetDirectory(LPCTSTR szData){m_szDirectory=szData;}
	void SetSensorSerialNumber(LPCTSTR szData){m_szSensorSerialNumber=szData;}
	void SetMode(INT nData){m_nMode=nData;}
	void SetSelectMode(INT nData){m_nSelectMode=nData;}

	BOOL ExecuteEllito(void);

	CString m_szLastError;

	void *m_pControlRetry;
	void SetControlRetry(void * pControlRetry){m_pControlRetry=pControlRetry;}

	void *m_pInspectionMain;

};

