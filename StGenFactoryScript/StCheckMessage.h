#pragma once
#include "stcheckbase.h"
class StCheckMessage :
	public StCheckBase
{
public:
	StCheckMessage(void);
	StCheckMessage(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog=NULL);
	~StCheckMessage(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
 	//Å•1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);
	//Å£1.0.0.1025

	BOOL Execute(void);
	INT GetMessageJudge(void){return m_nMessageJudge;}
	LPCTSTR GetTextButton(void){return m_szTextButton;}

private:

	void SetOKButtonShow(BOOL bData){m_bOKButtonShow = bData;}
	void SetNGButtonShow(BOOL bData){m_bNGButtonShow = bData;}
	void SetTextButtonShow(BOOL bData){m_bTextButtonShow = bData;}
	void SetTextButton(LPCTSTR szMessage){m_szTextButton = szMessage;}
	void SetMessage(LPCTSTR szMessage){m_szMessage = szMessage;}

	void Initialize(void);

	BOOL m_bOKButtonShow;
	BOOL m_bNGButtonShow;
	BOOL m_bTextButtonShow;
	CString m_szMessage;
	CString m_szTextButton;

	INT m_nMessageJudge;

};

