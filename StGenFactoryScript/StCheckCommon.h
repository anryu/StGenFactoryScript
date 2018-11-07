#pragma once
#include "stcheckbase.h"
class StCheckCommon :
	public StCheckBase
{
public:
	StCheckCommon(void);
	StCheckCommon(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog=NULL);
	~StCheckCommon(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);

	BOOL Execute(void){return TRUE;}

	void SetSaveFilePath(LPCTSTR szText){mSaveFilePath = szText;}
	//void SetCSVScopeLeft(INT nData){m_nCSVScopeLeft = nData;}
	//void SetCSVScopeRight(INT nData){m_nCSVScopeRight = nData;}

	//��1.0.0.1036
	void GetLastErrorMessage(CString &szMessage){szMessage = m_szLastErrorMessage;}
	void ClearLastErrorMessage(void){m_szLastErrorMessage=_T("");}
	//��1.0.0.1036

	//��1.0.0.1056
	BOOL SaveBitmap(StBuffer *aBuffer=NULL, LPCTSTR szDateFormat=NULL);
	//��1.0.0.1056
	//��1.0.0.1057
	BOOL SaveCSV(StBuffer *aBuffer=NULL, LPCTSTR szDateFormat=NULL);
	//��1.0.0.1057

private:
	void Initialize(void);

	//�ϐ���
	CString m_szFileName;

	//��1.0.0.1056
	//BOOL SaveBitmap(StBuffer *aBuffer=NULL, LPCTSTR szDateFormat=NULL);
	//��1.0.0.1056
	//��1.0.0.1057
	//BOOL SaveCSV(StBuffer *aBuffer=NULL, LPCTSTR szDateFormat=NULL);
	//��1.0.0.1057
	BOOL SaveAll();

	CString mSaveFilePath;
	INT m_nCSVScopeLeft;
	INT m_nCSVScopeRight;

	//��1.0.0.1035
	BOOL SavePNG(StBuffer *aBuffer=NULL, LPCTSTR szDateFormat=NULL);
	BOOL SaveJpeg(StBuffer *aBuffer=NULL, LPCTSTR szDateFormat=NULL);
	BOOL SaveImage(StBuffer *aBuffer,LPCTSTR szDateFormat, INT nImageMode);
	//��1.0.0.1035
	//��1.0.0.1055
	BOOL SaveHistogram(StBuffer *aBuffer=NULL, LPCTSTR szDateFormat=NULL);
	//��1.0.0.1055
	//��1.0.0.1036
	INT m_nWaitFrameTimeout;
	BOOL WaitFrameExecute(INT nWaitFrame);
	CString m_szLastErrorMessage;
	//��1.0.0.1036

};

