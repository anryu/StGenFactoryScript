#pragma once
#include "stcheckbase.h"
class StCheckPixelDefect :
	public StCheckBase
{
public:
	StCheckPixelDefect(void);
	StCheckPixelDefect(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog=NULL);
	~StCheckPixelDefect(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	//▼1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);
	//▲1.0.0.1025
	BOOL Execute(void);

	//---パラメータ設定-------------------------------
	void SetStart(INT nData);
	void SetEnd(INT nData);
	void SetWidthLines(INT nData);
	void SetDifferenceMax(double dblData);
	void SetSeparate(INT nData);
	//▼1.0.0.1041
	void SetCheckMode(INT nData);
	//▲1.0.0.1041

	//結果取得
	INT GetResultElement(void);
	double GetResultDifference(int nIndex);
	INT GetResultLine(int nIndex);

private:
	void Initialize(void);

	//パラメータ
	INT m_nStart;
	INT m_nEnd;
	INT m_nWidthLines;
	double m_dblDifferenceMax;
	INT m_nSeparate;
	//▼1.0.0.1041
	INT m_nCheckMode;
	//▲1.0.0.1041

	//結果
	INT m_iResultElement;
	double *m_pdblResultDifferenceRatio;
	INT *m_piResultLineRatio;


};

