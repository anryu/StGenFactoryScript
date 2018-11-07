// SelComPortDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "SelComPortDlg.h"
#include "afxdialogex.h"


// CSelComPortDlg ダイアログ

IMPLEMENT_DYNAMIC(CSelComPortDlg, CDialogEx)

CSelComPortDlg::CSelComPortDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelComPortDlg::IDD, pParent)
{
	m_pStructPortName = NULL;
}

CSelComPortDlg::~CSelComPortDlg()
{
}

void CSelComPortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSelComPortDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelComPortDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECTOR, &CSelComPortDlg::OnCbnSelchangeComboSelector)
	ON_CBN_SELCHANGE(IDC_COMBO_PORTNAME, &CSelComPortDlg::OnCbnSelchangeComboPortname)
END_MESSAGE_MAP()


// CSelComPortDlg メッセージ ハンドラー


BOOL CSelComPortDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	m_StructPortName.Copy(m_pStructPortName);

	InitSelectorList();

	UpdateComPortList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
//StComm_SearchComPort
//-----------------------------------------------------------------------------
//DLLEXPORT DWORD WINAPI StComm_SearchComPort(LPTSTR szComPortName, PDWORD pdwNumComPort)
DWORD WINAPI StComm_SearchComPort(LPTSTR szComPortName, size_t iNameSize, PDWORD pdwNumComPort)
{
	HKEY	hKey=NULL;
	
	DWORD	dwValueIndex = 0;
	DWORD dwErrorCode = NO_ERROR;
	//レジストリキーをオープン
#ifdef LOGOUT
{TCHAR text[256];
swprintf( text, 256, _T("StComm_SearchComPort(%d)\n"),1 ); 
LogFileWriteString( text );
}
#endif
	DWORD gdwErrorCode = RegOpenKeyEx(
					HKEY_LOCAL_MACHINE, 
					TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"),
					0, 
					KEY_QUERY_VALUE,
//					KEY_READ,
					&hKey);
#ifdef LOGOUT
{TCHAR text[256];
swprintf( text, 256, _T("StComm_SearchComPort(%d)\n"),2); 
LogFileWriteString( text );
}
#endif
	if(gdwErrorCode)	return(gdwErrorCode);

//DWORD dwCount;
//RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &dwCount, NULL, NULL, NULL, NULL);
//TRACE( "dwCount=%d \n", dwCount );

	wcscpy_s(szComPortName, iNameSize, TEXT(""));
#ifdef LOGOUT
{TCHAR text[256];
swprintf( text, 256, _T("StComm_SearchComPort(%d)\n"),3 ); 
LogFileWriteString( text );
}
#endif
	*pdwNumComPort = 0;

	do{
		DWORD dwType=0;
		TCHAR	szKeyName[MAX_PATH];
		memset( szKeyName,0,sizeof(szKeyName) );

		DWORD	dwKeyNameSize = sizeof(szKeyName)/sizeof(TCHAR);
//		DWORD	dwKeyNameSize = sizeof(szKeyName);
		BYTE	pbyteData[256];
		memset( pbyteData,0,sizeof(pbyteData) );
		TCHAR	szData[256];
		memset( szData,0,sizeof(szData) );
		DWORD	dwDataSize = sizeof(pbyteData);
		DWORD	dwRegEnumValueReval = 
			RegEnumValue(
				hKey, dwValueIndex,
				szKeyName, &dwKeyNameSize,
				NULL,
				&dwType, 
				pbyteData,&dwDataSize);
#ifdef LOGOUT
{TCHAR text[256];
swprintf( text, 256, _T("dwRegEnumValueReval(%d) dwValueIndex=%d \n"),dwRegEnumValueReval,dwValueIndex ); 
LogFileWriteString( text );
}
#endif		
		if(dwRegEnumValueReval) break;

//for( int i=0; i<dwDataSize; i++ ){
//TRACE( "%x ", pbyteData[i] );
//}
//TRACE( "\n", (char*)pbyteData, dwType, dwKeyNameSize );

		//文字列の場合のみ取得
		if(REG_SZ == dwType)
		{
			//重複していないかどうかを確認
#ifdef LOGOUT
{TCHAR text[256];
swprintf( text, 256, _T("pbyteData=%s dwType=%d dwKeyNameSize=%d \n"),(char*)pbyteData, dwType, dwKeyNameSize ); 
LogFileWriteString( text );
}
#endif

#ifdef LOGOUT
{TCHAR text[256];
swprintf( text, 256, _T("szComPortName=%s \n"),szComPortName ); 
LogFileWriteString( text );
}
#endif
//TRACE( "pbyteData=%s dwType=%d dwKeyNameSize=%d \n", (char*)pbyteData, dwType, dwKeyNameSize );
			if(NULL == _tcsstr(szComPortName, (TCHAR*)pbyteData))
			{
#ifdef LOGOUT
{TCHAR text[256];
swprintf( text, 256, _T("pbyteData=%s dwType=%d dwKeyNameSize=%d \n"),(char*)pbyteData, dwType, dwKeyNameSize ); 
LogFileWriteString( text );
}
#endif
				if(_tcslen(szComPortName))
					wcscat_s(szComPortName, iNameSize-_tcslen(szComPortName), TEXT("\\"));

				memset( szData,0,sizeof(szData) );
#ifdef UNICODE
#ifdef LOGOUT
{TCHAR text[256];
swprintf( text, 256, _T("MultiByteToWideChar(IN)\n") ); 
LogFileWriteString( text );
}
#endif
				MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,(char*)&pbyteData[0],(int)strlen((char*)pbyteData),szData,((int)sizeof(szData))/2);
#ifdef LOGOUT
{TCHAR text[256];
swprintf( text, 256, _T("MultiByteToWideChar(OUT)\n") ); 
LogFileWriteString( text );
}
#endif
				//もしくはmbstowcs(buf,str,(sizeof buf)/2);
#else
//				strcpy(szData,pbyteData);
				swprintf(szData,sizeof(szData)/sizeof(szData[0]),TEXT("%s"),pbyteData);
#endif
//TRACE( "szComPortName=%s\n", szComPortName );
//TRACE( "szData=%s\n", (PTCHAR)szData );

				//サイズ指定の関数使用でエラーがでる
//				wcscat_s(szComPortName, sizeof(szComPortName)/sizeof(szComPortName[0]), szData);
				wcscat(szComPortName, (PTCHAR)pbyteData);
				//▼2007/03/19 v1004 [HCA]COMポートの数を間違って返していたバグを修正
				//*pdwNumComPort++;
				(*pdwNumComPort)++;
				//▲2007/03/19 v1004 [HCA]COMポートの数を間違って返していたバグを修正
			}
		}
		dwValueIndex++;
	}while(1);

	RegCloseKey(hKey);

	if(NO_ERROR != dwErrorCode) gdwErrorCode = dwErrorCode;
	return(dwErrorCode);
}

void CSelComPortDlg::InitSelectorList(void)
{
	//if( !m_pStructPortName ) return;

	CComboBox objComboBox;

	objComboBox.Attach(GetDlgItem(IDC_COMBO_SELECTOR)->m_hWnd);
	for( int i=0; i<m_StructPortName.GetPortSelectorCount(); i++ )
	{
		CString szString;
		BOOL bReval = m_StructPortName.GetSelectorName(i, szString);
		if( bReval )
			objComboBox.AddString(szString);
	}
	objComboBox.SetCurSel(0);
	objComboBox.Detach();
}

void CSelComPortDlg::UpdateComPortList(void)
{
	TCHAR	szComPortList[4096];
	DWORD	dwCount = 0;
	DWORD	dwReval = NO_ERROR;

	//この関数がUNICODEに対応していないため、
	//プリプロセッサ定義から,_UNICODEを削除した
	//エントリーポイントをwWinMainCRTStartupから空欄にした
	dwReval = StComm_SearchComPort(szComPortList, _countof(szComPortList), &dwCount);
#ifdef LOGOUT
{TCHAR text[256];
swprintf( text, 256, _T("UpdateComPortList 1 \n") ); 
LogFileWriteString( text );
}
#endif		
	
	LPTSTR szTop = szComPortList;
	if(0 < dwCount)
	{
		CComboBox objComboBox;
		//▼1.0.0.1014
		//INT iIndex = 0;
		INT iIndex = -1;
		//▲1.0.0.1014
		objComboBox.Attach(GetDlgItem(IDC_COMBO_PORTNAME)->m_hWnd);

#ifdef LOGOUT
{TCHAR text[256];
swprintf( text, 256, _T("UpdateComPortList 2 \n") ); 
LogFileWriteString( text );
}
#endif		

		//
		CComboBox objComboBoxSelector;
		objComboBoxSelector.Attach(GetDlgItem(IDC_COMBO_SELECTOR)->m_hWnd);
		CString szSelectorName;
		objComboBoxSelector.GetWindowText(szSelectorName);
		objComboBoxSelector.Detach();

		CString szComPortName;
		m_StructPortName.GetPortName(szSelectorName,szComPortName);

		while(dwCount--)
		{
			LPTSTR szNext = _tcschr(szTop, TEXT('\\'));
			if(szNext != NULL)
			{
				*szNext = NULL;
				szNext++;
			}

			objComboBox.AddString(szTop);
			if(_tcscmp(szTop, szComPortName) == 0)
			{
				iIndex = objComboBox.GetCount() - 1;
			}
			szTop = szNext;
		}
#ifdef LOGOUT
{TCHAR text[256];
swprintf( text, 256, _T("UpdateComPortList 3 \n") ); 
LogFileWriteString( text );}
#endif		
		//▼1.0.0.1014
		objComboBox.AddString(_T(""));
		if( iIndex<0 )
		{
			iIndex = objComboBox.GetCount() - 1;
		}
		//▲1.0.0.1014
		objComboBox.SetCurSel(iIndex);
		objComboBox.Detach();
	}
}


void CSelComPortDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_pStructPortName->Copy(&m_StructPortName);

	CDialogEx::OnOK();
}


void CSelComPortDlg::OnCbnSelchangeComboSelector()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CString szSelectorName;
	CString szPortName;
	CComboBox objComboBox;
	objComboBox.Attach(GetDlgItem(IDC_COMBO_SELECTOR)->m_hWnd);
	objComboBox.GetWindowText(szSelectorName);
	objComboBox.Detach();

	m_StructPortName.GetPortName(szSelectorName,szPortName);

	objComboBox.Attach(GetDlgItem(IDC_COMBO_PORTNAME)->m_hWnd);


	//▼1.0.0.1014
	INT iIndex = objComboBox.GetCount()-1;
	//▲1.0.0.1014
	for(int i=0; i<objComboBox.GetCount(); i++ )
	{
		CString szPortList;
		objComboBox.GetLBText(i,szPortList);
		if( szPortName==szPortList )
		{
			//▼1.0.0.1014
			//objComboBox.SetCurSel(i);
			iIndex = i;
			//▲1.0.0.1014
			break;
		}
	}
	//▼1.0.0.1014
	objComboBox.SetCurSel(iIndex);
	//▲1.0.0.1014

	objComboBox.Detach();

}


void CSelComPortDlg::OnCbnSelchangeComboPortname()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	CString szSelectorName;
	CString szPortName;
	CComboBox objComboBox;
	objComboBox.Attach(GetDlgItem(IDC_COMBO_SELECTOR)->m_hWnd);
	objComboBox.GetWindowText(szSelectorName);
	objComboBox.Detach();

	objComboBox.Attach(GetDlgItem(IDC_COMBO_PORTNAME)->m_hWnd);
	objComboBox.GetWindowText(szPortName);
	objComboBox.Detach();

	m_StructPortName.SetPortName(szSelectorName,szPortName);

}
