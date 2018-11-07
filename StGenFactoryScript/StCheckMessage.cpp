#include "StdAfx.h"
#include "StCheckMessage.h"
#include "MessageDlg.h"


StCheckMessage::StCheckMessage(void)
{
	Initialize();
}

StCheckMessage::StCheckMessage(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
{
	Initialize();
}

StCheckMessage::~StCheckMessage(void)
{
}
void StCheckMessage::Initialize(void)
{

	m_bOKButtonShow = TRUE;
	m_bNGButtonShow = TRUE;
	m_bTextButtonShow = TRUE;
	m_szMessage = _T("");
	m_szTextButton = _T("");

	m_nMessageJudge = -1;

}

//-------------------------------------------------------------------------------
//é¿çs
//-------------------------------------------------------------------------------
BOOL StCheckMessage::Execute(void)
{

	CMessageDlg aMessageDlg;

	aMessageDlg.SetOKButtonShow(m_bOKButtonShow);
	aMessageDlg.SetNGButtonShow(m_bNGButtonShow);
	aMessageDlg.SetTextButtonShow(m_bTextButtonShow);
	aMessageDlg.SetTextButton(m_szTextButton);
	aMessageDlg.SetMessage(m_szMessage);

	aMessageDlg.DoModal();
	m_nMessageJudge = aMessageDlg.GetJudge();

	if( m_nMessageJudge==1 )
	{
		m_iJudge = 0;	//NG
	}
	else
	{
		m_iJudge = 1;	//OK
	}
	return TRUE;
}

BOOL GetBooleanFromText(LPCTSTR szArgument)
{

	BOOL bReval = TRUE;
	CString aArgument(szArgument);
	aArgument.MakeLower();
	if( aArgument == _T("true") )
		bReval = TRUE;
	else
	if( aArgument == _T("false") ) 
		bReval = FALSE;
	else
	{
		bReval = (BOOL)_ttoi(szArgument);
	}
	return bReval;
}

BOOL StCheckMessage::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="SetOKButtonShow" )
	{
		SetOKButtonShow(GetBooleanFromText(szArgument));
		bReval = TRUE;
	}
	else
	if( szFunction=="SetNGButtonShow" )
	{
		SetNGButtonShow(GetBooleanFromText(szArgument));
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTextButtonShow" )
	{
		SetTextButtonShow(GetBooleanFromText(szArgument));
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTextButton" )
	{
		SetTextButton(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMessage" )
	{
		SetMessage(szArgument);
		bReval = TRUE;
	}
	return bReval;

}

//Å•1.0.0.1025
BOOL StCheckMessage::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetJudge" )
	{
		dblValue = GetMessageJudge();
		bReval = TRUE;
	}
	return bReval;
}
//Å£1.0.0.1025


