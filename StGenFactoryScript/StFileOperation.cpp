#include "StdAfx.h"
#include "StFileOperation.h"


StFileOperation::StFileOperation(void)
{
	Initialize();
}

StFileOperation::StFileOperation(LPCTSTR szClassName, LPCTSTR szVarName, LPCTSTR szArgment)
	:StCommonBase( szClassName, szVarName )
	, m_szFileName(szArgment)
{
	Initialize();
}

StFileOperation::~StFileOperation(void)
{
}

BOOL StFileOperation::Initialize(void)
{
	m_nMode = 0;
	return TRUE;
}

BOOL StFileOperation::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="SetFileName" )
	{
		m_szFileName = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMode" || szFunction=="Mode" )
	{
		m_nMode = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="Output" || szFunction=="Write" )
	{
		//Å•1.0.0.1062
		//bReval = Output(szArgument);
		if( m_nScriptCheckMode==0 )
			bReval = Output(szArgument);
		else
			bReval = TRUE;
		//Å£1.0.0.1062
	}
	return bReval;
}


void ChangeEscapeSequence(CString szSrc, CString &szDst)
{
	PTCHAR pSrc[] = {_T("\\a"),_T("\\b"),_T("\\f"),_T("\\n"),_T("\\r"),_T("\\t"),_T("\\v"),_T("\\\\")};
	PTCHAR pDst[] = {_T("\a"),_T("\b"),_T("\f"),_T("\n"),_T("\r"),_T("\t"),_T("\v"),_T("\\")};

	BOOL bChange = FALSE;
	do{
		bChange = FALSE;
		for( int i=0; i<_countof(pSrc); i++ )
		{
			INT nPos = szSrc.Find(pSrc[i]);
			if( nPos>=0 )
			{
				szSrc = szSrc.Left(nPos) + pDst[i] + szSrc.Mid(nPos+_tcslen(pSrc[i]));
				bChange=TRUE;
				break;
			}
		}
	}while(bChange);
	szDst = szSrc;
}

BOOL StFileOperation::Output( LPCTSTR szArgument )
{
	BOOL bReval = FALSE;

	if( m_szFileName.GetLength()==0 ) return FALSE;
	//Save
	CStdioFile stdFile;
	UINT nOpenFlag = CFile::modeCreate|CFile::modeWrite|CFile::typeText;
	if( m_nMode!=0 ) nOpenFlag |= CFile::modeNoTruncate;
	if(stdFile.Open( m_szFileName, nOpenFlag, NULL)!=0)
	{
		BOOL bWrite = TRUE;
		ULONGLONG len = stdFile.SeekToEnd();
		if( m_nMode==2 && len>0 ) bWrite = FALSE; 
		if( bWrite )
		{
//TRACE(TEXT("@@@@@@@@@ _tcslen(szArgument)[%d]"), _tcslen(szArgument) );
			//stdFile.WriteString(szArgument,_tcslen(szArgument));
			CString szDst;
			ChangeEscapeSequence(szArgument, szDst);
			stdFile.WriteString(szDst);
		}
		stdFile.Close();
		bReval = TRUE;
	}

	return bReval;
}
