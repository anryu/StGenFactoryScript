#include "stdafx.h"
#include "CVersion.h"
 
#pragma comment(lib,"version.lib ")
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CVersion::CVersion(HINSTANCE hInstance)
{
	m_bGotVersionDataFg = FALSE;
	m_hInstance = hInstance;

	//Get Application Name
	//CWinApp *App=(CWinApp*)AfxGetApp();
	//_tcscpy(m_szAppName,App->m_pszAppName);
	//App->m_hInstance

	//Get EXE File Name
	GetModuleFileName(m_hInstance ,m_szModuleFileName,MAX_PATH);


}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CVersion::~CVersion()
{

}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL CVersion::bGetVersion(PDWORD pdwProductVersionMS,PDWORD pdwProductVersionLS)
{
	//バージョン情報を取得
	if(!mbGetVersionInfo())
		return(FALSE);

	*pdwProductVersionMS = m_dwProductVersionMS;
	*pdwProductVersionLS = m_dwProductVersionLS;
	return(TRUE);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL CVersion::mbGetVersionInfo(void)
{
	DWORD dwHandle;
	VS_FIXEDFILEINFO *VersionFixedFileInfo;
	BOOL bReval = FALSE;

	//バージョン情報取得済の場合は終了
	if(m_bGotVersionDataFg)
		return(TRUE);

	//Get Version Information
	DWORD dwVersionInfoSize = GetFileVersionInfoSize(m_szModuleFileName,&dwHandle);
	PBYTE pbyteVersionInfo = new BYTE[dwVersionInfoSize];
	if(pbyteVersionInfo == NULL)
	{

	}
	else
	{
		INT iReval=GetFileVersionInfo(m_szModuleFileName,dwHandle,dwVersionInfoSize,pbyteVersionInfo);
		if(iReval)
		{
			UINT uintSize;
			iReval=VerQueryValue(pbyteVersionInfo,TEXT("\\"),(void **)&VersionFixedFileInfo,&uintSize);
			if(iReval)
			{
				m_dwProductVersionMS = VersionFixedFileInfo->dwProductVersionMS;
				m_dwProductVersionLS = VersionFixedFileInfo->dwProductVersionLS;

				m_bGotVersionDataFg = TRUE;
				bReval = TRUE;
			}
		}
		delete pbyteVersionInfo;
	}
	return(bReval);
}


