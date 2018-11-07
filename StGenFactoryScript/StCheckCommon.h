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

	//Å•1.0.0.1036
	void GetLastErrorMessage(CString &szMessage){szMessage = m_szLastErrorMessage;}
	void ClearLastErrorMessage(void){m_szLastErrorMessage=_T("");}
	//Å£1.0.0.1036

	//Å•1.0.0.1056
	BOOL SaveBitmap(StBuffer *aBuffer=NULL, LPCTSTR szDateFormat=NULL);
	//Å£1.0.0.1056
	//Å•1.0.0.1057
	BOOL SaveCSV(StBuffer *aBuffer=NULL, LPCTSTR szDateFormat=NULL);
	//Å£1.0.0.1057

private:
	void Initialize(void);

	//ïœêîñº
	CString m_szFileName;

	//Å•1.0.0.1056
	//BOOL SaveBitmap(StBuffer *aBuffer=NULL, LPCTSTR szDateFormat=NULL);
	//Å£1.0.0.1056
	//Å•1.0.0.1057
	//BOOL SaveCSV(StBuffer *aBuffer=NULL, LPCTSTR szDateFormat=NULL);
	//Å£1.0.0.1057
	BOOL SaveAll();

	CString mSaveFilePath;
	INT m_nCSVScopeLeft;
	INT m_nCSVScopeRight;

	//Å•1.0.0.1035
	BOOL SavePNG(StBuffer *aBuffer=NULL, LPCTSTR szDateFormat=NULL);
	BOOL SaveJpeg(StBuffer *aBuffer=NULL, LPCTSTR szDateFormat=NULL);
	BOOL SaveImage(StBuffer *aBuffer,LPCTSTR szDateFormat, INT nImageMode);
	//Å£1.0.0.1035
	//Å•1.0.0.1055
	BOOL SaveHistogram(StBuffer *aBuffer=NULL, LPCTSTR szDateFormat=NULL);
	//Å£1.0.0.1055
	//Å•1.0.0.1036
	INT m_nWaitFrameTimeout;
	BOOL WaitFrameExecute(INT nWaitFrame);
	CString m_szLastErrorMessage;
	//Å£1.0.0.1036

};

