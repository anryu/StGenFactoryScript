#pragma once
#include "stcheckbase.h"
#include "StValue.h"
#include <vector>

class StIF :
	public StCheckBase
{
public:
	StIF(void);
	StIF(LPCTSTR szClassName, LPCTSTR szVarName, LPCTSTR szJudgement);
	~StIF(void);

	BOOL Execute(void){return TRUE;}

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);

	void SetStartLine(INT nData)
	{
		Initialize();	//èâä˙âª
		m_nStartLine = nData;
	}
	void SetElseIfLine(INT nData)
	{
		m_vecElseIfLine.push_back(nData);
	}
	void SetElseLine(INT nData){m_nElseLine = nData;}
	void SetEndLine(INT nData){m_nEndLine = nData;}
	void SetValue(std::vector <StValue *> &VarName){m_vecValue = VarName;}
	void SetNowLine(INT nData){m_nNowLine = nData;}

	INT GetNextLine(void);

private:
	void Initialize(void);
	BOOL Judgement(void);
	INT m_nStartLine;
	std::vector <INT> m_vecElseIfLine;
	INT m_nElseLine;
	INT m_nEndLine;
	INT m_nNextLine;

	INT m_nNowLine;


	BOOL m_bJudgeIn;

	StString m_szJudgementSentence;

	std::vector <StValue *> m_vecValue;
	INT GetNextElseLine(void);

};

