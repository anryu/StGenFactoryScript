#pragma once
#include "stcheckbase.h"
class StCheckCSVDust :
	public StCheckBase
{
public:
	StCheckCSVDust(void);
	StCheckCSVDust(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase);
	~StCheckCSVDust(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);

	void SetSaveFilePath(LPCTSTR szText){mSaveFilePath = szText;}
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);

	double GetResultAverage(void);
	double GetResultDifferenceRatio(void);
	INT GetResultLineRatio(void);
	//▼1.0.0.1043
	INT GetResultContinuousCount(void);
	INT GetResultContinuousStart(void);
	INT GetResultContinuousEnd(void);
	//▲1.0.0.1043

	BOOL Execute(void);

private:
	void Initialize(void);
	void Release(void);

	void SetWidthLines(INT nData){m_nWidthLines = nData;}
	void SetStart(INT nData){m_nStart = nData;}
	void SetEnd(INT nData){m_nEnd = nData;}
	void SetSeparate(INT nData){m_nSeparate = nData;}


	void SetDifferenceMax(double dblData){m_dblDifferenceMax = dblData;}
	void SetAverageLowLevel(double dblData){m_dblAverageLowLevel = dblData;}
	void SetAverageHighLevel(double dblData){m_dblAverageHighLevel = dblData;}
	//▼1.0.0.1043
	void SetContinuousCountMax(INT nData){m_nContinuousCountMax = nData;}
	//▲1.0.0.1043
	//▼1.0.0.1041
	INT m_nCheckMode;
	void SetCheckMode(INT nData){m_nCheckMode = nData;}
	//▲1.0.0.1041


	//変数名
	CString m_szFileName;
	CString mSaveFilePath;

	//パラメータ
	INT m_nStart;
	INT m_nEnd;
	INT m_nWidthLines;
	INT m_nSeparate;

	//規格値-----
	double m_dblAverageLowLevel;
	double m_dblAverageHighLevel;
	double m_dblDifferenceMax;
	//▼1.0.0.1043
	INT m_nContinuousCountMax;
	//▲1.0.0.1043

	//結果-----
	INT m_iResultElement;
	//double m_dblResultAverage;
	double *m_pdblResultDifferenceRatio;
	INT *m_piResultLineRatio;
	double *m_pdblResultAverage;
	//▼1.0.0.1043
	PINT m_pnResultContinuousCount;
	PINT m_pnResultContinuousStart;
	PINT m_pnResultContinuousEnd;
	//▲1.0.0.1043


};

