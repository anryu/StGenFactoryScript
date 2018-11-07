#pragma once
#include "stcheckbase.h"
class StCheckDataLose :
	public StCheckBase
{
public:
	StCheckDataLose(void);
	StCheckDataLose(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog=NULL);
	~StCheckDataLose(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	//▼1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);
	//▲1.0.0.1025

	BOOL Execute(void);

	//---パラメータ設定-------------------------------
	void SetStart(INT nData);
	void SetEnd(INT nData);
	void SetDifferenceMax(double dblData);
	void SetCSVFileName(LPCTSTR szData);

	INT GetResultElement(void);
	double GetResultDifference(int nIndex);
	INT GetResultHighLow(int nIndex);
	INT GetResultLine(int nIndex);

	//void SetDeviceModelName(LPCTSTR szText){mDeviceModelName = szText;}
	//void SetSerialNumber(LPCTSTR szText){mSerialNumber = szText;}
	void SetSaveFilePath(LPCTSTR szText){mSaveFilePath = szText;}

//▼1.0.0.1063
protected:
	void Initialize(void);
	double GetDifferenceMax(void){return m_dblDifferenceMax;}
	INT GetStart(void){return m_nStart;}
	INT GetEnd(void){return m_nEnd;}
	//結果
	INT m_iResultElement;
	double *m_pdblResultDifference;
	INT *m_piResultHighLow;
	INT *m_piResultLine;

	CString mSaveFilePath;

	BOOL SaveCheckExecute(StBuffer *aBuffer, BOOL bCheck=TRUE, LPCTSTR szDateFormat=NULL, LPCTSTR szFileName=NULL);
//▲1.0.0.1063

private:
	//▼1.0.0.1063
	//void Initialize(void);
	//▲1.0.0.1063
	//BOOL SaveResultCSVFile(void);
	//BOOL SaveResultCSVFile( INT nElement, double (*pdblLineAverage)[], INT (*piLineMin)[], INT (*piLineMax)[], size_t bufferSize );
	//▼1.0.0.1063
	//BOOL SaveResultCSVFile( INT nElement, double **pdblLineAverage, INT **piLineMin, INT **piLineMax, size_t bufferSize );
	BOOL SaveResultCSVFile( INT nElement, double **pdblLineAverage, INT **piLineMin, INT **piLineMax, size_t bufferSize, LPCTSTR szDateFormat=NULL, LPCTSTR szCSVFileName=NULL );
	//▲1.0.0.1063
	//パラメータ
	INT m_nStart;
	INT m_nEnd;
	double m_dblDifferenceMax;
	CString m_szCSVFileName;

	//▼1.0.0.1063
	//結果
	//INT m_iResultElement;
	//double *m_pdblResultDifference;
	//INT *m_piResultHighLow;
	//INT *m_piResultLine;

	//CString mSaveFilePath;
	//▲1.0.0.1063

};

