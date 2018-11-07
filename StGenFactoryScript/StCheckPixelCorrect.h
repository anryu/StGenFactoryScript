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

	//�p�����[�^
	INT m_nMode;		//0:�^�J�mEllito 1�`:���Ή�
	INT m_nSelectMode;	//0:�t�@�C�������I�� 1:�t�@�C���蓮�I��

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

