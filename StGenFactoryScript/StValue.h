#pragma once
#include "../CommonHeader/StString.h"
//Å•1.0.0.1024
#include <vector>
//Å£1.0.0.1024

class StValue
{
public:
	StValue(void);
	StValue(LPCTSTR szVarName, LPCTSTR szArgument);
	~StValue(void);

	BOOL function(LPCTSTR szFunc);

	StString *GetClassName(void){return &m_ClassName;}
	StString *GetVarName(void){return &m_VarName;}

	INT GetMode(void){return m_nMode;}
	double GetValue(void){return m_dblValue;}
	LPCTSTR GetValueString(void){return m_szValue.GetUnicode();}

	//Å•1.0.0.1024
	void SetValue(std::vector <StValue *> &VarName){m_vecValue = VarName;}

	BOOL GetValue(LPCTSTR szFunc, double &dblValue);
	//Å£1.0.0.1024

private:
	void Initialize();

	StString m_ClassName;
	StString m_VarName;
	CString m_szInit;
	StString m_szValue;
	double m_dblValue;
	INT m_nMode;	//0:Value 1:String

	//Å•1.0.0.1024
	std::vector <StValue *> m_vecValue;
	//Å£1.0.0.1024

};

