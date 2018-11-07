#include "StdAfx.h"
#include "StControl.h"


StControl::StControl(void)
	: m_ClassName(_T(""))
	, m_VarName(_T(""))
{
	//▼1.0.0.1020 クラス継承の元のハンドルを保存
	Initialize();
	//▲1.0.0.1020
}

StControl::StControl( LPCTSTR szClassName, LPCTSTR szVarName )
	: m_ClassName(szClassName)
	, m_VarName(szVarName)
{
	//▼1.0.0.1020 クラス継承の元のハンドルを保存
	Initialize();
	//▲1.0.0.1020
}

StControl::~StControl(void)
{
}

//▼1.0.0.1020 クラス継承の元のハンドルを保存
void StControl::Initialize(void)
{
	m_MainHandle = NULL;
	//▼1.0.0.1062
	m_nScriptCheckMode=0;
	//▲1.0.0.1062
}
//▲1.0.0.1020
