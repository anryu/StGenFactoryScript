#pragma once
#include "stcheckbase.h"
class StCheckSpBehavior :
	public StCheckBase
{
public:
	StCheckSpBehavior(void);
	StCheckSpBehavior(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog=NULL);
	~StCheckSpBehavior(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	//��1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg,double &dblValue);
	//��1.0.0.1025

	BOOL Execute(void);

	//---�p�����[�^�ݒ�-------------------------------
	void SetStart(INT nData);
	void SetEnd(INT nData);
	void SetColor(INT nData);
	void SetMaxLevel(INT nData);
	void SetMinLevel(INT nData);
	void SetInOutMode(INT nData);
	//---�K�i�ݒ�-------------------------------
	void SetContinuousCount(INT nData);
	//---�^�C�g���ݒ�-------------------------------
	void SetTitle(LPCTSTR szTitle);

	//---���ʎ擾-------------------------------
	INT GetContinuous(void){return m_iResultContinuous;}
	INT GetLine(void){return m_iResultLine;}
	INT GetColor(void){return m_iResultColor;}
	//��1.0.0.1044
	double GetLineAverage(void){return m_dblResultLineAverage;}
	//��1.0.0.1044


private:
	void Initialize(void);

	//�p�����[�^
	INT m_nStart;
	INT m_nEnd;
	INT m_nColor;

	INT m_nMaxLevel;
	INT m_nMinLevel;

	INT m_nInOutMode;
	//�K�i�l-----
	INT m_nContinuousCount;

	//����-----
	INT m_iResultContinuous;
	INT m_iResultLine;
	INT m_iResultColor;
	//��1.0.0.1044
	double 	m_dblResultLineAverage;
	//��1.0.0.1044

	//�^�C�g��-----
	CString m_szTitle;
};

