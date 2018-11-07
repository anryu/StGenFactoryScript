#pragma once

#include "../CommonHeader/StString.h"
class StCommonBase
{
public:
	StCommonBase(void);
	StCommonBase(LPCTSTR szClassName, LPCTSTR szVarName);
	~StCommonBase(void);

	StString *GetClassName(void){return &m_ClassName;}
	StString *GetVarName(void){return &m_VarName;}

	//��1.0.0.1062
	void SetScriptCheckMode(INT nScriptCheckMode){m_nScriptCheckMode=nScriptCheckMode;}
	//��1.0.0.1062

protected:
	StString m_ClassName;
	StString m_VarName;

	//��1.0.0.1062
	INT m_nScriptCheckMode;
	//��1.0.0.1062

};

