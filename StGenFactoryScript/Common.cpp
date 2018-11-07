#include "stdafx.h"
#include <windows.h>
//��1.3.0.10������Ή�
#include "HookMsgBox.h"
//��1.3.0.10������Ή�

#ifdef __cplusplus
extern "C"{
#endif

	
//��2007/10/15 v0.50 Beta03 ���X�g���N���b�v�{�[�h�փR�s�[����@�\��ǉ�
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#ifdef UNICODE
#define CF_TCHAR	CF_UNICODETEXT
#else
#define CF_TCHAR	CF_TEXT
#endif

BOOL gbCopyToClipBoard(LPCTSTR szData)
{
	HGLOBAL hGlobal = GlobalAlloc(GHND | GMEM_SHARE,
			(lstrlen(szData) + 1) * sizeof(TCHAR));
	PTSTR	pGlobal = (PTSTR)GlobalLock(hGlobal);
	lstrcpy(pGlobal, szData);
	GlobalUnlock(hGlobal);

	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_TCHAR, hGlobal);
	CloseClipboard();
	return(TRUE);

}
//��2007/10/15 v0.50 Beta03 ���X�g���N���b�v�{�[�h�փR�s�[����@�\��ǉ�


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL gbCheckDirectoryExist(LPCTSTR lpszPath,BOOL bCreate)
{
	
	BOOL bResult=TRUE;
	WIN32_FIND_DATA wfd;

	HANDLE hFile = FindFirstFile(lpszPath, &wfd);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		bResult = TRUE;
		FindClose(hFile);
#if 0
		OutputDebugString(TEXT("Directory Exist:"));
		OutputDebugString(lpszPath);
		OutputDebugString(TEXT("\n"));
#endif
	}
	else
	{
		if(!bCreate)
		{
			bResult = FALSE;
		}
		else
		{
			TCHAR szTmpPath[MAX_PATH];	
			wcscpy_s(szTmpPath,MAX_PATH,lpszPath);
			wcscpy_s(_tcsrchr(szTmpPath,'\\'), sizeof(szTmpPath)/sizeof(szTmpPath[0])-wcslen(szTmpPath),_T(""));
			bResult = gbCheckDirectoryExist(szTmpPath,bCreate);
			if(bResult)
			{
				bResult = CreateDirectory(lpszPath, NULL);
#if 0
		OutputDebugString(TEXT("Create Directory:"));
		OutputDebugString(lpszPath);
		OutputDebugString(TEXT("\n"));
#endif
			}
		}
	}

	return(bResult);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL gbCheckFileExist(LPCTSTR lpszFullPathFileName)
{
	BOOL bResult=FALSE;
	WIN32_FIND_DATA wfd;

	HANDLE hFile = FindFirstFile(lpszFullPathFileName, &wfd);

	if(hFile != INVALID_HANDLE_VALUE)
		bResult = TRUE;

	FindClose(hFile);

	return bResult;
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL gbCheckDllFileExist(LPCTSTR lpszFileName)
{
	BOOL bResult = FALSE;
	TCHAR szFileName[MAX_PATH];
	INT iPos;

	for(iPos=0;iPos<3;iPos++)
	{
		//Target Directory Setting
		switch(iPos)
		{
		case(0):
			//Sys dir
			GetSystemDirectory(szFileName,MAX_PATH);
			break;
		case(1):
			//Win dir
			GetWindowsDirectory(szFileName,MAX_PATH);
			break;
		case(2):
			//Current Directory
			GetCurrentDirectory(MAX_PATH,szFileName);
			break;
		/*
		case(3):
			//Application Directory
			GetModuleFileName(NULL,szFileFilterName,MAX_PATH);
			wcscpy_s(_tcsrchr(szFileName,'\\'),_T(""));
			break;
		*/

		}
		wcscat_s(szFileName,sizeof(szFileName)/sizeof(szFileName[0]),TEXT("\\"));
		wcscat_s(szFileName,sizeof(szFileName)/sizeof(szFileName[0]),lpszFileName);
		bResult = gbCheckFileExist(szFileName);
		if(bResult) break;
	}
	return bResult;
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL gbShowErrorMsg(HWND hwnd, DWORD dwErrorCode)
{
	BOOL bResult = TRUE;

	HLOCAL hlocal = NULL;

	BOOL bOk = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,
		dwErrorCode,
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),// MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		(LPTSTR)&hlocal,
		0,
		NULL);

/*
	if(!bOk)
	{
		//StCam.dll�ŗL�̃��b�Z�[�W���H
		HMODULE	hDll = LoadLibraryEx(TEXT("StCamMsg.dll"), NULL, DONT_RESOLVE_DLL_REFERENCES);

		if(hDll != NULL)
		{
			FormatMessage(
					FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
					hDll,
					dwErrorCode,
					MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
					(PTSTR)&hlocal,
					0,
					NULL);
			FreeLibrary(hDll);
		}
	}
*/	
	if(hlocal != NULL)
	{
		//��1.3.0.10������Ή�
		//AfxMessageBox((LPCTSTR)LocalLock(hlocal),MB_OK,NULL);
		AfxMessageBoxHooked((LPCTSTR)LocalLock(hlocal),MB_OK);
		//��1.3.0.10������Ή�
		LocalFree(hlocal);
	}
	else
	{
		//��1.3.0.10������Ή�
		//AfxMessageBox(TEXT("Error Number Not Found."),MB_OK,NULL);
		AfxMessageBoxHooked(TEXT("Error Number Not Found."),MB_OK);
		//��1.3.0.10������Ή�

	}



	return bResult;
}

//-----------------------------------------------------------------------------
//INI�t�@�C��������
//-----------------------------------------------------------------------------
BOOL WriteINIFile(LPCTSTR szFileName,TCHAR *szSectionName,TCHAR *szKeyName, TCHAR *szValue)
{
	TCHAR szPathName[MAX_PATH];
	BOOL	bReval = TRUE;

	//�f�B���N�g�����쐬
	wcscpy_s(szPathName, sizeof(szPathName)/sizeof(szPathName[0]), szFileName);
	wcscpy_s(_tcsrchr(szPathName,'\\'), sizeof(szPathName)/sizeof(szPathName[0])-wcslen(szPathName),_T(""));
	gbCheckDirectoryExist(szPathName,TRUE);

	//�t�@�C����ۑ�
	bReval = WritePrivateProfileString(szSectionName,szKeyName,szValue,szFileName);


	return(bReval);
}

//-----------------------------------------------------------------------------
//�G���[���b�Z�[�W�\������у��O������
//-----------------------------------------------------------------------------
BOOL ErrorMsgProc(LPCTSTR szErrorType,LPCTSTR szErrorMsg,BOOL bShowMsg)
{
	TCHAR szLogFileName[MAX_PATH];
	TCHAR szErrorTime[MAX_PATH];

	BOOL	bReval = TRUE;
	FILE	*fp;

	//�������ԏ��̎擾
	struct tm today;
	__time64_t long_time;
	_time64( &long_time ); 
	_localtime64_s( &today, &long_time ); 

	_stprintf_s(szErrorTime, _countof(szErrorTime),TEXT("%04d/%02d/%02d %02d:%02d:%02d"),
		today.tm_year + 1900 ,
		today.tm_mon + 1,
		today.tm_mday,
		today.tm_hour,
		today.tm_min,
		today.tm_sec);

	//���O�t�@�C������ݒ�
	GetModuleFileName(NULL,szLogFileName,MAX_PATH);
	_tcslwr_s(szLogFileName,sizeof(szLogFileName));	//�������֕ϊ�
	_stprintf_s(_tcsrchr(szLogFileName,'.'), _countof(szLogFileName)-_tcslen(szLogFileName),_T(""));

	//��1.0.0.1015 LOG�t�@�C�����ύX
	//wcscat_s(szLogFileName, sizeof(szLogFileName)/sizeof(szLogFileName[0]),TEXT(".log"));
	_tcscat_s(szLogFileName, _countof(szLogFileName),TEXT("_NG.txt"));
	//��1.0.0.1015 LOG�t�@�C�����ύX

	if( 0!=_tfopen_s( &fp, szLogFileName,TEXT("at")))
//	if(NULL==(fp = _tfopen(szLogFileName,TEXT("at"))))
	{
		return(FALSE);
	}

	_ftprintf(fp,TEXT("[%s]%s\n"),szErrorTime,szErrorType);
	_ftprintf(fp,TEXT("%s\n"),szErrorMsg);

	fclose(fp);

	//���b�Z�[�W��\��
	if(bShowMsg)
	{
		//MessageBox(NULL,szErrorMsg,szErrorType,MB_OK | MB_ICONEXCLAMATION);
		//��1.3.0.10������Ή�
		//AfxMessageBox(szErrorMsg,MB_OK | MB_ICONEXCLAMATION);
		AfxMessageBoxHooked(szErrorMsg,MB_OK | MB_ICONEXCLAMATION);
		//��1.3.0.10������Ή�
	}

	return(bReval);
}
BOOL	bGetSlopeAndIntercept(DOUBLE dblX1,DOUBLE dblY1,DOUBLE dblX2,DOUBLE dblY2,DOUBLE *pdblSlope,DOUBLE *pdblIntercept)
{
	BOOL bReval = TRUE;

	//�X���Z�o
	if(dblX1 == dblX2)	return(FALSE);
	*pdblSlope = (dblY2-dblY1)/(dblX2-dblX1);

	//�ؕЎZ�o
	*pdblIntercept = ((dblY1 - *pdblSlope * dblX1) + (dblY2 - *pdblSlope * dblX2))/2.0;

	return(bReval);
}

BOOL	bGetIntersectionOfTowLines(DOUBLE dblSlope1,DOUBLE dblIntercept1,DOUBLE dblSlope2,DOUBLE dblIntercept2,DOUBLE *pdblX,DOUBLE *pdblY)
{
	BOOL bReval = TRUE;

	*pdblX = -(dblIntercept1 - dblIntercept2) / (dblSlope1 - dblSlope2);
	*pdblY = ((*pdblX * dblSlope1 + dblIntercept1) + (*pdblX * dblSlope2 + dblIntercept2))/2.0;

	return(bReval);
}


//��2007/09/27 v0.49 Beta1 ��Ǝғ��͋@�\��ǉ�

void RemoveCharFromString(LPTSTR szTarget, LPCTSTR szSearch)
{
	LPTSTR szCurPos = szTarget;
	do{
		LPTSTR szPos = _tcspbrk(szCurPos, szSearch);
		if(NULL == szPos) break;
		TCHAR szTail[MAX_PATH] = {0};
		wcscpy_s(szTail, sizeof(szTail)/sizeof(szTail[0]), _tcsinc(szPos));
		wcscpy_s(szPos, sizeof(szPos)/sizeof(szPos[0]), szTail);
		szCurPos = szPos;
	}while(1);
}
//��2007/09/27 v0.49 Beta1 ��Ǝғ��͋@�\��ǉ�



//��2007/09/27 v0.49 Beta1 ��Ǝғ��͋@�\��ǉ�

void Trim(LPTSTR szTarget)
{
	LPCTSTR szSearch = TEXT(" �@");

	//�擪�̃X�y�[�X���폜
	LPTSTR szCurPos = szTarget;
	do{
		LPTSTR szPos = _tcspbrk(szCurPos, szSearch);
		if(NULL == szPos) break;
		
		//���������ꏊ���擪����Ȃ������烋�[�v�𔲂���
		if(szCurPos != szPos) break;

		TCHAR szTail[MAX_PATH] = {0};
		wcscpy_s(szTail, sizeof(szTail)/sizeof(szTail[0]),_tcsinc(szPos));
		wcscpy_s(szPos, sizeof(szTail)/sizeof(szTail[0]), szTail);
		szCurPos = szPos;
	}while(1);


	//���̃X�y�[�X���폜
	do{
		LPTSTR szPos = _tcspbrk(szTarget, szSearch);
		if(NULL == szPos) break;
		do
		{
			LPTSTR szNext =  _tcspbrk(_tcsinc(szPos), szSearch);
			if(NULL == szNext) break;
			szPos = szNext;
		}while(1);
		
		//���������ꏊ���Ōザ��Ȃ������烋�[�v�𔲂���
		if(1 < _tcsclen(szPos)) break;

		wcscpy_s(szPos, sizeof(szPos)/sizeof(szPos[0]), TEXT(""));
	}while(1);

}
//��2007/09/27 v0.49 Beta1 ��Ǝғ��͋@�\��ǉ�

//��2009/01/26 v0.64 Beta01 �قȂ�J���������N�{�[�h�ɓ���COM�|�[�g�����蓖�Ă��Ă��܂�MultiCam�s��Ώ�
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void mvGetRegEnum(HKEY keyRootKey,LPCTSTR	lpszSubKey,CStringArray &strArrayRegList)
{	
	HKEY keyKey;
	
	if(!RegOpenKeyEx(keyRootKey,lpszSubKey,0,KEY_QUERY_VALUE,&keyKey))
	{
		long lngIndex=0;

		do
		{
			TCHAR szKeyName[1024];
			TCHAR szKeyValue[1024];
			unsigned char uchrKeyValue[1024];
			unsigned long ulngKeyNameLen=sizeof(szKeyName)/sizeof(TCHAR);
			unsigned long ulngKeyValueLen=sizeof(uchrKeyValue)/sizeof(unsigned char);
			unsigned long ulngType=0;

			if(!RegEnumValue(keyKey,lngIndex,szKeyName,&ulngKeyNameLen,
					NULL,&ulngType,uchrKeyValue,&ulngKeyValueLen))
			{
				switch(ulngType)
					{
					case(REG_SZ):
						swprintf(szKeyValue, sizeof(szKeyValue)/sizeof(szKeyValue[0]),TEXT("%s"), uchrKeyValue );
						strArrayRegList.Add(szKeyValue);
//						strArrayRegList.Add(uchrKeyValue);
						break;
				}
			}
			else
			{
				break;
			}

			lngIndex++;
		}while(1);
		RegCloseKey(keyKey);
	}

}
//��2009/01/26 v0.64 Beta01 �قȂ�J���������N�{�[�h�ɓ���COM�|�[�g�����蓖�Ă��Ă��܂�MultiCam�s��Ώ�
//��2009/01/26 v0.64 Beta01 �قȂ�J���������N�{�[�h�ɓ���COM�|�[�g�����蓖�Ă��Ă��܂�MultiCam�s��Ώ�
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL IsUniqueCOMPortName(LPCTSTR szPortName)
{

	CStringArray strArrayPortNameList;
	HKEY keyRootKey=HKEY_LOCAL_MACHINE;
	LPCTSTR	lpszSubKey=_T("HARDWARE\\DEVICEMAP\\SERIALCOMM");

	mvGetRegEnum(keyRootKey,lpszSubKey,strArrayPortNameList);


	BOOL	isUniqueCOMPortName = TRUE;
	for(int iList=0; iList<strArrayPortNameList.GetSize();iList++)
	{
		if(0 == strArrayPortNameList.GetAt(iList).Compare(szPortName))
		{
			isUniqueCOMPortName = FALSE;
			break;
		}
	}


	return(isUniqueCOMPortName);
}
//��2009/01/26 v0.64 Beta01 �قȂ�J���������N�{�[�h�ɓ���COM�|�[�g�����蓖�Ă��Ă��܂�MultiCam�s��Ώ�

//-----------------------------------------------------------------------------
//PTouch���x�����
//-----------------------------------------------------------------------------
INT PtouchLabelPrint( CString strSerial, CString strQRCode, CString strModelName, CString strMAC )
{

	INT iRet=-1;
	/*

	BSTR bstrMAC = NULL;
	BSTR bstrSerial = NULL;
	BSTR bstrQRCode = NULL;
	BSTR bstrModelName = NULL;
	BSTR bstrPath = NULL;
	try {
		IDocumentPtr pObjDoc(__uuidof(bpac::Document));
		CString strPath;// = TEMPLATE_PATH;

		TCHAR szFileName[MAX_PATH];
		GetModuleFileName(NULL,szFileName,MAX_PATH);
		wcscpy_s(_tcsrchr(szFileName,'\\'), sizeof(szFileName)/sizeof(szFileName[0])-wcslen(szFileName),_T(""));
		wcscat_s(szFileName, sizeof(szFileName)/sizeof(szFileName[0]),TEXT("\\GigE-Factory.lbx"));

		strPath = szFileName;
//		strPath = _T("GigE-Factory.lbx");
		 // �e���v���[�g�I�[�v��
		bstrPath = strPath.AllocSysString();
		if (pObjDoc->Open(bstrPath))
		{

			INT iLenght = strMAC.GetLength();
			for( int i=iLenght-1; i>0; i-- ){
				if( (i&1)==0 ){
					strMAC.Insert( i,_T("-") );
				}
			}

			bstrMAC = strMAC.AllocSysString();
			if( pObjDoc->GetObject(_bstr_t(_T("MAC"))) ){
				pObjDoc->GetObject(_bstr_t(_T("MAC")))->PutText(bstrMAC);
			}

			bstrSerial = strSerial.AllocSysString();
			bstrQRCode = strQRCode.AllocSysString();
			bstrModelName = strModelName.AllocSysString();
			if( pObjDoc->GetObject(_bstr_t(_T("Model-No"))) ){
				pObjDoc->GetObject(_bstr_t(_T("Model-No")))->PutText(bstrModelName);
			}
			if( pObjDoc->GetObject(_bstr_t(_T("Serial-No"))) ){
				pObjDoc->GetObject(_bstr_t(_T("Serial-No")))->PutText(bstrSerial);
			}
			if( pObjDoc->GetObject(_bstr_t(_T("Barcode"))) ){
				pObjDoc->GetObject(_bstr_t(_T("Barcode")))->PutText(bstrQRCode);
			}

			// pObjDoc->SetMediaById(pObjDoc->Printer->GetMediaId(), true);

			// ����ݒ�J�n
			pObjDoc->StartPrint(_bstr_t(_T("")), bpac::bpoDefault);
			// ����W���u�̒ǉ� (1�����)
			pObjDoc->PrintOut(1, bpac::bpoDefault);
			// ����ݒ�I��(=����J�n)
			pObjDoc->EndPrint();
			// �e���v���[�g�N���[�Y
			pObjDoc->Close();

			iRet=0;
		}else{
			//��1.3.0.10������Ή�
			//AfxMessageBox(_T("lbx�t�@�C��������܂���"));
			AfxMessageBoxHooked(_T("There is no lbx file."),MB_OK);
			//��1.3.0.10������Ή�
			iRet=1;
		}

	}
	catch (_com_error e) {
		//��1.3.0.10������Ή�
		//AfxMessageBox(e.ErrorMessage());
		AfxMessageBoxHooked(e.ErrorMessage(),MB_OK);
		//��1.3.0.10������Ή�
		iRet=2;
	}
	if( bstrSerial )
		SysFreeString(bstrSerial);
	if( bstrQRCode )
		SysFreeString(bstrQRCode);
	if( bstrModelName )
		SysFreeString(bstrModelName);
	if( bstrMAC )
		SysFreeString(bstrMAC);
	if( bstrPath )
		SysFreeString(bstrPath);
		*/
	return iRet;
}

//------------------------------------------------------------------------------------------
//LOGOUT�p�@Debug
//------------------------------------------------------------------------------------------
BOOL GetErrorLogFileName( LPTSTR szFileName )
{
	BOOL bReval = TRUE;

	//�������t�@�C�����擾
	if(GetModuleFileName(NULL,szFileName,MAX_PATH))
	{
		wcscpy_s(_tcsrchr(szFileName,'\\'), MAX_PATH-wcslen(szFileName),_T("\\errorlog.txt"));
	}
	else
	{
		bReval = FALSE;
	}

	return(bReval);
}
BOOL LogFileWriteString( LPCTSTR  text )
{
	TCHAR szErrLogFileName[MAX_PATH];
	GetErrorLogFileName(szErrLogFileName);
	CStdioFile LogFile;
	if( !LogFile.Open(szErrLogFileName,CFile::modeNoTruncate|CFile::modeCreate|CFile::modeWrite|CFile::typeText ) ){;
		return false;
	}
	LogFile.SeekToEnd();
	LogFile.WriteString( text );
	LogFile.WriteString(_T("\n"));

	LogFile.Close();
	return true;
}
BOOL LogFileWriteCode( BYTE errdata )
{
	TCHAR szErrLogFileName[MAX_PATH];
	GetErrorLogFileName(szErrLogFileName);
	CStdioFile LogFile;
	if( !LogFile.Open(szErrLogFileName,CFile::modeNoTruncate|CFile::modeCreate|CFile::modeWrite|CFile::typeText ) ){;
		return false;
	}
	LogFile.SeekToEnd();

	TCHAR text[2048];
	swprintf_s( text, 2048, _T("ErrorCode(0x%x)"),errdata );
	LogFile.WriteString( text );
	LogFile.WriteString(_T("\n"));

	LogFile.Close();
	return true;
}

BOOL LogFileWriteRecieve( PBYTE perrdata, INT iLen )
{
	TCHAR szErrLogFileName[MAX_PATH];
	GetErrorLogFileName(szErrLogFileName);
	CStdioFile LogFile;
	if( !LogFile.Open(szErrLogFileName,CFile::modeNoTruncate|CFile::modeCreate|CFile::modeWrite|CFile::typeText ) ){;
		return false;
	}
	LogFile.SeekToEnd();

	TCHAR text[2048], buf[2048];
	swprintf_s( text, 2048, _T("["));
	for( int i=0; i<iLen; i++ ){
		swprintf( buf, 2048, _T("%02x:"), perrdata[i]);
		wcscat_s( text, 2048, buf );
	}
	wcscat_s( text, 2048, _T("]") );
	LogFile.WriteString( text );
	LogFile.WriteString(_T("\n"));

	LogFile.Close();
	return true;
}
			
BOOL LogFileDelete( void )
{
	TCHAR szErrLogFileName[MAX_PATH];
	GetErrorLogFileName(szErrLogFileName);
	return DeleteFile(szErrLogFileName);
//	return true;
}

//��1.3.0.10������Ή�
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL GetOSMajorMinorVersion(PDWORD pdwMajor, PDWORD pdwMinor)
{
	BOOL bReval = TRUE;
	//OS�o�[�W�����擾
	OSVERSIONINFO osvi;

	ZeroMemory(&osvi, sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(osvi);

	//�o�[�W�������̎擾
	if(bReval = GetVersionEx(&osvi))
	{
		*pdwMajor = osvi.dwMajorVersion;
		*pdwMinor = osvi.dwMinorVersion;
	}

	return(bReval);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL IsAfterOrEqualToVista()
{
	//�o�[�W�������擾
	DWORD dwMajor = 0;
	DWORD dwMinor = 0;
	GetOSMajorMinorVersion(&dwMajor, &dwMinor);
	return(6 <= dwMajor);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
typedef LANGID (WINAPI *funcSetThreadUILanguage)(LANGID LangId);
BOOL SetLocale(LCID nLocale)
{
	BOOL bReval = TRUE;
	do
	{
		bReval = SetThreadLocale(nLocale);
		if(!bReval) break;

		if(IsAfterOrEqualToVista())
		{
			HMODULE hKernel32 = LoadLibrary(TEXT("kernel32.dll"));
			if(hKernel32 != NULL)
			{
				do
				{
					funcSetThreadUILanguage pSetThreadUILanguage = (funcSetThreadUILanguage)GetProcAddress(hKernel32, "SetThreadUILanguage");
					if(pSetThreadUILanguage != NULL)
					{
						pSetThreadUILanguage(nLocale);
					}
				}while(FALSE);
				FreeLibrary(hKernel32);
			}
		}
	}while(FALSE);
	return(bReval);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LCID GetLocale(void)
{
	LCID nLocale = GetThreadLocale();
	return(nLocale);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#define SUPPORTED_LANG_COUNT 2
INT	GetSupportedLangCount(void)
{
	return(SUPPORTED_LANG_COUNT);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LCID	GetSupportedLCID(INT index)
{
	//SUBLANG_NEUTRAL�ɂ��Ă��܂���SetThreadLocale��GetThreadLocale�Ŏ擾�����ۂɈقȂ�l���Ԃ��ė���H
	//LCID lcid = MAKELCID(MAKELANGID(LANG_ENGLISH, /*SUBLANG_NEUTRAL*/SUBLANG_DEFAULT), SORT_DEFAULT);
	LCID lcid = MAKELCID(MAKELANGID(LANG_JAPANESE, /*SUBLANG_NEUTRAL*/SUBLANG_DEFAULT), SORT_DEFAULT);
	switch(index)
	{
	case(1):
		//TAIWAN
		lcid = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_DEFAULT/*SUBLANG_NEUTRAL*/), SORT_DEFAULT);
		//lcid = MAKELCID(MAKELANGID(LANG_JAPANESE, /*SUBLANG_NEUTRAL*/SUBLANG_DEFAULT), SORT_DEFAULT);
		break;
	case(2):
		//lcid = MAKELCID(MAKELANGID(LANG_GERMAN, /*SUBLANG_NEUTRAL*/SUBLANG_DEFAULT), SORT_DEFAULT);
		break;
	case(3):
		//lcid = MAKELCID(MAKELANGID(LANG_SPANISH, /*SUBLANG_NEUTRAL*/SUBLANG_DEFAULT), SORT_DEFAULT);
		break;
	case(4):
		//TAIWAN
		//lcid = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL/*SUBLANG_NEUTRAL*/), SORT_DEFAULT);
		break;
	//case(4):
	//	//SIMPLIFIED
	//	lcid = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_DEFAULT);
	//	break;
	//case(5):
	//	//TAIWAN
	//	lcid = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL/*SUBLANG_NEUTRAL*/), SORT_DEFAULT);
	//	break;

	}
	return(lcid);

}
//��1.3.0.10������Ή�


BOOL GetModuleFilePath(CString &szPath)
{
	BOOL bReval = TRUE;

	TCHAR szModulePath[MAX_PATH];
	GetModuleFileName(NULL,szModulePath,_countof(szModulePath));
	_tcscpy( _tcsrchr(szModulePath,'\\'),_T("") );
	szPath = szModulePath;
	return bReval;
}



#ifdef __cplusplus
}
#endif
