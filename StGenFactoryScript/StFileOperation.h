#pragma once
#include "stcommonbase.h"
class StFileOperation :
	public StCommonBase
{
public:
	StFileOperation(void);
	StFileOperation(LPCTSTR szClassName, LPCTSTR szVarName, LPCTSTR szArgument);
	~StFileOperation(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
private:
	BOOL Initialize(void);
	BOOL Output( LPCTSTR szArgument);

	INT m_nMode;			//0:�㏑�����[�h 1:�ǉ����[�h 2:�w�b�_�[���[�h
	CString m_szFileName;


};

