#pragma once
#include "stcheckbase.h"


class StWriteFileData :	public StCheckBase
{
public:
	StWriteFileData(void);
	StWriteFileData(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void* pParent=NULL, void *aDialog=NULL);
	~StWriteFileData(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);

	BOOL Execute(void);

private:
	void Initialize(void);

	CString m_szFileName;
	CString m_szFilePath;

	INT m_nAddressMagnification;
	INT m_nAddressOffset;

};

