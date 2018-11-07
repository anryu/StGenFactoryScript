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

	INT m_nMode;			//0:上書きモード 1:追加モード 2:ヘッダーモード
	CString m_szFileName;


};

