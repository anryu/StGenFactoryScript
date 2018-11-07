#include "StdAfx.h"
#include "StCommonBase.h"


StCommonBase::StCommonBase(void)
{
}

StCommonBase::StCommonBase(LPCTSTR szClassName, LPCTSTR szVarName)
	: m_ClassName(szClassName)
	, m_VarName(szVarName)
{
}

StCommonBase::~StCommonBase(void)
{
}
