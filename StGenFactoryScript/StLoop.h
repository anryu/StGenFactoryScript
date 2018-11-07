#pragma once
#include "stcheckbase.h"
#include "StValue.h"
#include <vector>
class StLoop :
	public StCheckBase
{
public:
	StLoop(void);
	StLoop(LPCTSTR szClassName, LPCTSTR szVarName, LPCTSTR szJudgement, void *aDialog=NULL);
	~StLoop(void);

	BOOL Execute(void){return TRUE;}

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);

	void SetStartLine(INT nData){m_nStartLine = nData;}
	void SetEndLine(INT nData){m_nEndLine = nData;}
	void SetValue(std::vector <StValue *> &VarName){m_vecValue = VarName;}

	INT GetNextLine(void);

private:
	void Initialize(void);
	BOOL Judgement(void);
	INT m_nStartLine;
	INT m_nEndLine;
	INT m_nNextLine;
	BOOL m_Judge;
	StString m_szJudgementSentence;

	std::vector <StValue *> m_vecValue;



};

