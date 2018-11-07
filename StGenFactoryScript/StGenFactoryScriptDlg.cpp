
// StGenFactoryScriptDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StGenFactoryScriptDlg.h"
#include "afxdialogex.h"
#include <math.h>
//▼1.00.1047
#include <locale.h>
//▲1.00.1047

//#include "../Common/Common.h"

#include "StCommon.h"
#include "CVersion.h"
//#include "StInspectMain.h"

//#include "StInputSerialDlg.h"
#include "Common.h"
#include "HookMsgBox.h"
#include "SelComPortDlg.h"

#include "StPTouch.h"

//▼1.00.1017
#include "SettingTerminalDlg.h"
//▲1.00.1017
//▼1.00.1020
#include "SettingLightChannelDlg.h"
//▲1.00.1020

//▼1.00.1039
#include "CheckListBoxForCheck.h"
//▲1.00.1039

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//HANDLE hPTouchMutex;

#define ST_DISPLAYMAIN  1001
#define ST_RETRY		1002

// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CStGenFactoryScriptDlg ダイアログ

//IDocumentPtr pObjDoc;

//▼1.0.0.1067
//extern void DoEvents();
//▲1.0.0.1067

CStGenFactoryScriptDlg::CStGenFactoryScriptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStGenFactoryScriptDlg::IDD, pParent)
	, m_ForwardingCheckMode( 0 )
	, m_FactoryMode( 0 )
	//▼1.0.0.1048
	//, m_StructPortName( TRUE, 3 )
	//▲1.0.0.1048
	//▼1.0.0.1015
	, m_CameraType( 0 )
	//▲1.0.0.1015
	//▼1.0.0.1048
	////▼1.0.0.1017
	//, m_StructAddress( TRUE, 3, 1 )
	////▲1.0.0.1017
	, m_StructPortName( TRUE, 9 )
	, m_StructAddress( TRUE, 9, 1 )
	//▲1.0.0.1048
	//▼1.0.0.1062
	, m_nScriptCheckMode( 0 )
	, m_pErrorListDlg(NULL)
	//▲1.0.0.1062

	//▼1.0.0.1069
	, m_xvRadioSelectMode(0)
	, m_bEnabledCheckBox(FALSE)
	//▲1.0.0.1069
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	TCHAR		szIniFileName[MAX_PATH];
	CStCommon::GetIniFileName(szIniFileName, MAX_PATH);
	m_ForwardingCheckMode = GetPrivateProfileInt( _T("Setting"), _T("ForwardingCheckMode"), m_ForwardingCheckMode, szIniFileName);
	m_FactoryMode = GetPrivateProfileInt( _T("Setting"), _T("FactoryMode"), m_FactoryMode, szIniFileName);
	//▼1.0.0.1015
	m_CameraType = GetPrivateProfileInt( _T("Setting"), _T("CameraType"), m_CameraType, szIniFileName);
	//▲1.0.0.1015

	//▼1.0.0.1069
	_tcscpy_s(m_szIniFileName,_countof(m_szIniFileName),szIniFileName);
	m_xvRadioSelectMode = GetPrivateProfileInt( _T("Setting"), _T("RadioSelectMode"), m_xvRadioSelectMode, m_szIniFileName);
	m_bEnabledCheckBox = GetPrivateProfileInt( _T("Setting"), _T("EnabledCheckBox"), m_bEnabledCheckBox, m_szIniFileName);
	//▲1.0.0.1069

	//▼1.0.0.1072
	m_xvRadioSelectMode = 1;
	//▲1.0.0.1072


	//Ptouch
	CoInitialize(NULL);
 
//	IDocumentPtr pObjDoc(__uuidof(bpac::Document));
	//__uuidof(bpac::Document)をメインスレッドで取得する必要がある？？
	//別スレッドだとフリーズしてしまう。
	//pObjDoc =  (IDocumentPtr)__uuidof(bpac::Document);

}

void CStGenFactoryScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESULT, m_ResultListBox);
	DDX_Radio(pDX, IDC_RADIO_SCRIPT, m_xvRadioSelectMode);
	DDX_Control(pDX, IDC_LIST_CHECK_ITEM, m_listCheckItem);
}

BEGIN_MESSAGE_MAP(CStGenFactoryScriptDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CStGenFactoryScriptDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CStGenFactoryScriptDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE, &CStGenFactoryScriptDlg::OnBnClickedButtonExecute)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_SELECTFILE, &CStGenFactoryScriptDlg::OnCbnSelchangeComboSelectfile)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CStGenFactoryScriptDlg::OnBnClickedButtonTest)
	ON_COMMAND(ID_MENUITEM_SELECT_PORT, &CStGenFactoryScriptDlg::OnMenuitemSelectPort)
	ON_COMMAND(ID_MENUITEM_OPEN_LOGFOLDER, &CStGenFactoryScriptDlg::OnMenuitemOpenLogfolder)
	ON_COMMAND_RANGE(ID_MENUITEM_CHECK_TYPE_0, ID_MENUITEM_CHECK_TYPE_1, OnMenuitemCheckType)
	ON_COMMAND_RANGE(ID_FACTORY_JAPAN, ID_FACTORY_KOREA, OnMenuitemFactoryType)
	//▼1.0.0.1046
	//ON_COMMAND_RANGE(ID_CAMERATYPE_ALL, ID_CAMERATYPE_CL, OnMenuitemCameraType)
	ON_COMMAND_RANGE(ID_CAMERATYPE_ALL, ID_CAMERATYPE_CXP, OnMenuitemCameraType)
	//▲1.0.0.1046
	ON_WM_INITMENUPOPUP()
	ON_WM_TIMER()
	ON_COMMAND(ID_MENUITEM_SETTINGTERMINAL, &CStGenFactoryScriptDlg::OnMenuitemSettingterminal)
	ON_COMMAND(ID_MENUITEM_SETTINGLIGHTCHANNEL, &CStGenFactoryScriptDlg::OnMenuitemSettinglightchannel)
	//▼1.0.0.1062
	ON_COMMAND_RANGE(ID_SCRIPTCHECK_OFF, ID_SCRIPTCHECK_ON, OnMenuitemScriptcheck)
	//▲1.0.0.1062
	ON_COMMAND(ID_MENUITEM_CHECK_BOX_ENABLED, &CStGenFactoryScriptDlg::OnMenuitemCheckBoxEnabled)
	ON_COMMAND(ID_MENUITEM_CHECK_BOX_ALL_CHECKED, &CStGenFactoryScriptDlg::OnMenuitemCheckBoxAllChecked)
	ON_COMMAND(ID_MENUITEM_CHECK_BOX_ALL_UNCHECKED, &CStGenFactoryScriptDlg::OnMenuitemCheckBoxAllUnchecked)
	ON_WM_INITMENU()
	//▼1.0.0.1069
	ON_COMMAND_RANGE(IDC_RADIO_SCRIPT, IDC_RADIO_MODEL, OnRadioitemScript)
	//▲1.0.0.1069
	ON_BN_CLICKED(IDC_CHECK_LOCK_CAMERA_TYPE, &CStGenFactoryScriptDlg::OnBnClickedCheckLockCameraType)
END_MESSAGE_MAP()


// CStGenFactoryScriptDlg メッセージ ハンドラー

BOOL CStGenFactoryScriptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	TCHAR szText[MAX_PATH];
	CStCommon::GetExeName( szText, MAX_PATH);

	TCHAR	szVersionString[MAX_PATH];
	DWORD	dwProductVersionMS;
	DWORD	dwProductVersionLS;
	
	CVersion *pobjVersion = new CVersion(AfxGetApp()->m_hInstance);
	pobjVersion->bGetVersion(&dwProductVersionMS,&dwProductVersionLS);
	delete pobjVersion;
	_stprintf_s(m_szAppVersion,_countof(m_szAppVersion),
		TEXT("%d.%d.%d.%d"),
		(dwProductVersionMS >> 16),
		(dwProductVersionMS & 0xFFFF),
		(dwProductVersionLS >> 16),
		(dwProductVersionLS & 0xFFFF));

	_tcscpy_s(szVersionString,_countof(szVersionString),szText);
	_tcscat_s(szVersionString,_countof(szVersionString),_T(" ("));
	_tcscat_s(szVersionString,_countof(szVersionString),m_szAppVersion);
	_tcscat_s(szVersionString,_countof(szVersionString),_T(")"));

#ifdef _DEBUG
	_tcscat_s(szVersionString,_countof(szVersionString), TEXT("[DEBUG]"));
#endif //_DEBUG
	SetWindowText(szVersionString);

	//▼1.0.0.1026
	m_szSoftwareVersion = szVersionString;
	//▲1.0.0.1026

	//▼1.0.0.1015
	FactoryExistFileRead(m_FactoryMode);
	CameraTypeExistFileRead(m_CameraType);
	//▲1.0.0.1015
	//▼1.0.0.1069
	//SetSelectFileComboBox();
	SetTextComboBox();
	//▲1.0.0.1069
	//▼1.0.0.1072
	SetLockCheckBox(TRUE);
	LockComboBox();
	//▲1.0.0.1072
	//▼1.0.0.1015
	//ボタン名
	SetButtonName(m_ForwardingCheckMode);
	//▲1.0.0.1015


	hDisplayMutex = CreateMutex(NULL,FALSE,NULL);
	//hPTouchMutex = CreateMutex(NULL,FALSE,NULL);

	m_Preview = NULL;

	m_Waveform = NULL;

	m_StInspectMain = NULL;

	//カメラ選択ロックをチェックする。
	//CButton objButton;
	//objButton.Attach(GetDlgItem(IDC_CHECK_LOCK_CAMERA_TYPE)->m_hWnd);
	//objButton.SetCheck(BST_CHECKED);
	//objButton.Detach();


	//初期化
	//m_iCurCameraTypeListNo = 0;
	//m_ForwardingCheckMode = 0;
	//m_Preview = NULL;

	//Iniファイル読み込み
	//mInitialFileRead();

	//カメラ情報表示を更新  -- カメラ選択コンボボックス
	//UpdateCameraInfo(TRUE);

	//Description
	//InitializeDescription();

	//リストボックス
	//InitializeListBox();
	//チェックボックス--項目名
	//InitializeCheckBox();
	//チェックボックスEnabled
	//UpdateCheckBoxEnabled(m_bEnabledCheckBox);
	//チェックボックス
	//InitializeCheckBox();

	//ボタン名
//	InitializeButtonName();





	//Ptouch
	//CoInitialize(NULL);

	//▼1.0.0.1069
	//リストボックス
	InitializeListBox();
	//チェックボックスEnabled
	UpdateCheckBoxEnabled(m_bEnabledCheckBox);

	UpdateData(FALSE);
	//▲1.0.0.1069

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CStGenFactoryScriptDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CStGenFactoryScriptDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CStGenFactoryScriptDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CStGenFactoryScriptDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnOK();
}

void CStGenFactoryScriptDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}

int CStGenFactoryScriptDlg::MessageScript(CString szMessge, UINT uType)
{
	return AfxMessageBox( szMessge, uType );
}


BOOL CStGenFactoryScriptDlg::GetIniFileName(CString &szFileName)
{
	BOOL bReval = TRUE;
	if( m_szIniFile.GetLength()==0 )
	{
		TCHAR szIniFileName[MAX_PATH];
		GetModuleFileName(NULL,szIniFileName,_countof(szIniFileName));
		_tcscpy( _tcsrchr(szIniFileName,'.'),_T("") );
		_tcscat_s( szIniFileName, _countof(szIniFileName), _T(".ini") );
		m_szIniFile = szIniFileName;
	}
	szFileName = m_szIniFile;
	return bReval;
}

BOOL CStGenFactoryScriptDlg::GetModulePath(CString &szPath)
{
	BOOL bReval = TRUE;
	if( m_szModulePath.GetLength()==0 )
	{
		TCHAR szModulePath[MAX_PATH];
		GetModuleFileName(NULL,szModulePath,_countof(szModulePath));
		_tcscpy( _tcsrchr(szModulePath,'\\'),_T("") );
		m_szModulePath = szModulePath;
	}
	szPath = m_szModulePath;
	return bReval;
}




//▼1.0.0.1015
#define INSPMODE_FILELIST0 _T("FactoryModeFileList_J.txt") //Japan
#define INSPMODE_FILELIST1 _T("FactoryModeFileList_K.txt") //Korea
#define INSPMODE_FILELIST2 _T("FactoryModeFileList_C.txt") //China
BOOL CStGenFactoryScriptDlg::FactoryExistFileRead(INT nMode)
{
	m_vFactoryExistFile.clear();
	PTCHAR pszFileName[] = { INSPMODE_FILELIST0, INSPMODE_FILELIST1, INSPMODE_FILELIST2 };
	if( nMode>=_countof(pszFileName) || nMode<0 || pszFileName[nMode]==NULL )
	{
		return FALSE;
	}

	CString szFileName;
	GetModulePath(szFileName);
	szFileName += _T("\\CONFIG\\");
	szFileName += pszFileName[nMode];

	//▼1.0.0.1047
	LPCTSTR szDefLocale = _tsetlocale(LC_CTYPE, _T("jpn"));
	//▲1.0.0.1047
	CStdioFile sFile;
	if( sFile.Open(szFileName,CFile::modeRead,NULL )!=0 )
	{
		sFile.SeekToBegin();
		CString tmp;
		while( sFile.ReadString(tmp) )
		{
			tmp.Trim();
			if( tmp.GetLength() )
				m_vFactoryExistFile.push_back(tmp);
		}
		sFile.Close();
	}
	//▼1.0.0.1047
	_tsetlocale(LC_CTYPE, szDefLocale);
	//▲1.0.0.1047

	return TRUE;
}

BOOL CStGenFactoryScriptDlg::FactoryExistCheck(LPCTSTR szFileName)
{
	if( m_vFactoryExistFile.size()==0 ) return TRUE;
	BOOL bExist = FALSE;
	CString szFile = szFileName;
	for( int i=0; i<m_vFactoryExistFile.size(); i++ )
	{
		if( szFile.Find(m_vFactoryExistFile[i])>=0 )
		{
			bExist=TRUE;
			break;
		}
	}
	return bExist;
}

//CameraType
#define CAMERATYPE_FILELIST0 _T("CameraTypeFileList_ALL.txt") //All
#define CAMERATYPE_FILELIST1 _T("CameraTypeFileList_GE.txt") //GigE
#define CAMERATYPE_FILELIST2 _T("CameraTypeFileList_CL.txt") //CameraLink
//▼1.0.0.1046
#define CAMERATYPE_FILELIST3 _T("CameraTypeFileList_CXP.txt") //CXP
//▲1.0.0.1046
BOOL CStGenFactoryScriptDlg::CameraTypeExistFileRead(INT nType)
{
	m_vCameraTypeExistFile.clear();
	//▼1.0.0.1046
	//PTCHAR pszFileName[] = { CAMERATYPE_FILELIST0, CAMERATYPE_FILELIST1, CAMERATYPE_FILELIST2 };
	PTCHAR pszFileName[] = { CAMERATYPE_FILELIST0, CAMERATYPE_FILELIST1, CAMERATYPE_FILELIST2, CAMERATYPE_FILELIST3 };
	//▲1.0.0.1046
	if( nType>=_countof(pszFileName) || nType<0 || pszFileName[nType]==NULL )
	{
		return FALSE;
	}

	CString szFileName;
	GetModulePath(szFileName);
	szFileName += _T("\\CONFIG\\");
	szFileName += pszFileName[nType];
	CStdioFile sFile;
	if( sFile.Open(szFileName,CFile::modeRead,NULL )!=0 )
	{
		sFile.SeekToBegin();
		CString tmp;
		while( sFile.ReadString(tmp) )
		{
			tmp.Trim();
			if( tmp.GetLength() )
				m_vCameraTypeExistFile.push_back(tmp);
		}
		sFile.Close();
	}
	return TRUE;
}

BOOL CStGenFactoryScriptDlg::CameraTypeExistCheck(LPCTSTR szFileName)
{
	if( m_vCameraTypeExistFile.size()==0 ) return TRUE;
	BOOL bExist = FALSE;
	CString szFile = szFileName;
	for( int i=0; i<m_vCameraTypeExistFile.size(); i++ )
	{
		if( szFile.Find(m_vCameraTypeExistFile[i])>=0 )
		{
			bExist=TRUE;
			break;
		}
	}
	return bExist;
}

BOOL CStGenFactoryScriptDlg::SetButtonName(INT nMode)
{
	CButton *pButton = (CButton *)GetDlgItem(IDC_BUTTON_EXECUTE);
	LPCTSTR szText[] = {_T("工程内検査開始"), _T("出荷検査開始")};
	pButton->SetWindowText(szText[nMode]);
	return TRUE;
}
//▲1.0.0.1015

#define SCRIPTPATH _T(".\\CONFIG\\script")
//▼1.0.0.1061
#define SUBSCRIPTPATH _T(".\\CONFIG\\subscript")
//▲1.0.0.1061
//▼1.0.0.1069
#define MODELPATH _T(".\\CONFIG\\model")
#define MODELSCRIPTPATH _T(".\\CONFIG\\modelscript")
//▲1.0.0.1069
BOOL CStGenFactoryScriptDlg::SetSelectFileComboBox()
{
	CString tmp;
	CStdioFile sFile;
	BOOL bReval = TRUE;

	WIN32_FIND_DATA FindFileData;

	CString szScriptPath;
	GetModulePath(szScriptPath);
	szScriptPath += _T("\\");
	szScriptPath += SCRIPTPATH;
	HANDLE hFind = FindFirstFile( szScriptPath + _T("\\*.txt"), &FindFileData);
	if( hFind!=INVALID_HANDLE_VALUE )
	{
		CString szIniFileName;
		GetIniFileName(szIniFileName);
		TCHAR szCurrentFileName[MAX_PATH];
		GetPrivateProfileString(_T("Script"),_T("FileName"),_T(""),szCurrentFileName,_countof(szCurrentFileName),szIniFileName);
		CComboBox aSelectComboBox;
		aSelectComboBox.Attach(GetDlgItem(IDC_COMBO_SELECTFILE)->m_hWnd);
		//▼1.0.0.1015
		aSelectComboBox.ResetContent();
		//▲1.0.0.1015
		do
		{
			//▼1.0.0.1015
			BOOL bFactoryExist = FactoryExistCheck(FindFileData.cFileName);
			BOOL bCameraTypeExist = CameraTypeExistCheck(FindFileData.cFileName);
			if( bFactoryExist && bCameraTypeExist )
			//▲1.0.0.1015
			aSelectComboBox.AddString(FindFileData.cFileName);
		}while( FindNextFile( hFind, &FindFileData) );
		FindClose(hFind);
		if( _tcslen(szCurrentFileName)>0 )
		{
			if( aSelectComboBox.SelectString(-1,szCurrentFileName)==CB_ERR )
			{
				aSelectComboBox.SetCurSel(0);
			}
		}
		else
		{
			aSelectComboBox.SetCurSel(0);
		}
		aSelectComboBox.Detach();
	}
	return bReval;
}

//▼1.0.0.1072

BOOL CStGenFactoryScriptDlg::SetLockCheckBox(BOOL bCheck)
{
	CButton objButton;
	objButton.Attach(GetDlgItem(IDC_CHECK_LOCK_CAMERA_TYPE)->m_hWnd);
	objButton.SetCheck(bCheck?BST_CHECKED:BST_UNCHECKED);
	objButton.Detach();
	return TRUE;
}

BOOL CStGenFactoryScriptDlg::LockComboBox()
{
	CButton objButton;
	CComboBox objComboBox;
	objButton.Attach(GetDlgItem(IDC_CHECK_LOCK_CAMERA_TYPE)->m_hWnd);
	objComboBox.Attach(GetDlgItem(IDC_COMBO_SELECTFILE)->m_hWnd);
	objComboBox.EnableWindow(!objButton.GetCheck());
	objComboBox.Detach();
	objButton.Detach();

	return TRUE;
}
//▲1.0.0.1072

//▼1.0.0.1069
BOOL CStGenFactoryScriptDlg::SetTextComboBox()
{
	BOOL bReval = FALSE;
	if( m_xvRadioSelectMode == 0 )
	{
		bReval = SetSelectFileComboBox();
	}
	else
	{
		bReval = SetSelectModelComboBox();
	}
	return bReval;
}

BOOL CStGenFactoryScriptDlg::SetSelectModelComboBox()
{
	CString tmp;
	CStdioFile sFile;
	BOOL bReval = TRUE;

	WIN32_FIND_DATA FindFileData;

	CString szScriptPath;
	GetModulePath(szScriptPath);
	szScriptPath += _T("\\");
	szScriptPath += MODELPATH;
	HANDLE hFind = FindFirstFile( szScriptPath + _T("\\*.txt"), &FindFileData);
	if( hFind!=INVALID_HANDLE_VALUE )
	{
		CString szIniFileName;
		GetIniFileName(szIniFileName);
		TCHAR szCurrentModelName[MAX_PATH];
		GetPrivateProfileString(_T("Script"),_T("ModelName"),_T(""),szCurrentModelName,_countof(szCurrentModelName),szIniFileName);
		CComboBox aSelectComboBox;
		aSelectComboBox.Attach(GetDlgItem(IDC_COMBO_SELECTFILE)->m_hWnd);
		aSelectComboBox.ResetContent();
	
		TCHAR szDisplayName[MAX_PATH];
		CString szModelFileName;
		do
		{
			_tcscpy_s(szDisplayName,_countof(szDisplayName),_T(""));
			szModelFileName = szScriptPath + _T("\\") + FindFileData.cFileName;
			GetPrivateProfileString(_T("Setting"), _T("DisplayName"), _T(""), szDisplayName, _countof(szDisplayName), szModelFileName);
			if( _tcslen(szDisplayName)>0 )
			{
				BOOL bFactoryExist = FactoryExistCheck(szDisplayName);
				BOOL bCameraTypeExist = CameraTypeExistCheck(szDisplayName);
				if( bFactoryExist && bCameraTypeExist )
				{
					aSelectComboBox.AddString(szDisplayName);
				}
			}
		}while( FindNextFile( hFind, &FindFileData) );
		FindClose(hFind);
		if( _tcslen(szCurrentModelName)>0 )
		{
			if( aSelectComboBox.SelectString(-1,szCurrentModelName)==CB_ERR )
			{
				aSelectComboBox.SetCurSel(0);
			}
		}
		else
		{
			aSelectComboBox.SetCurSel(0);
		}
		aSelectComboBox.Detach();
	}
	//▼1.0.0.72
	else
	{
		CComboBox aSelectComboBox;
		aSelectComboBox.Attach(GetDlgItem(IDC_COMBO_SELECTFILE)->m_hWnd);
		aSelectComboBox.ResetContent();
		aSelectComboBox.Detach();
	}
	//▲1.0.0.72
	return bReval;
}

BOOL CStGenFactoryScriptDlg::IsCheckFeature(LPCTSTR szFeature)
{
	//INT nCount = m_listCheckItem.GetCount();
	INT nCur = m_listCheckItem.FindStringExact(0,szFeature);
	if( nCur<0 ) return FALSE;
	INT nCheckItemID = m_listCheckItem.GetItemData(nCur);
	//▼1.0.0.1072
	nCheckItemID &= 0xFFFF;
	//▲1.0.0.1072
	//for( int i=0; i<nCount; i++ )
	//{
	//	m_listCheckItem.GetCheck(i);
	//	
	//}
	return m_listCheckItem.IsChecked(nCheckItemID);
}
//▲1.0.0.1069


BOOL CStGenFactoryScriptDlg::WriteSelectFile()
{
	CString szFileName;
	CComboBox aSelectComboBox;
	aSelectComboBox.Attach(GetDlgItem(IDC_COMBO_SELECTFILE)->m_hWnd);
	aSelectComboBox.GetLBText(aSelectComboBox.GetCurSel(), szFileName);
	aSelectComboBox.Detach();

	CString szIniFileName;
	GetIniFileName(szIniFileName);

	//▼1.0.0.1069
	//WritePrivateProfileString(_T("Script"),_T("FileName"),szFileName,szIniFileName);
	CString szKeyName;

	if( m_xvRadioSelectMode==0 )
	{
		szKeyName = _T("FileName");
	}
	else
	{
		szKeyName = _T("ModelName");
	}
	WritePrivateProfileString(_T("Script"),szKeyName,szFileName,szIniFileName);
	//▲1.0.0.1069
	return TRUE;
}

void CStGenFactoryScriptDlg::OnBnClickedButtonExecute()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	if( m_StInspectMain ) return;


	CWaitCursor aCursor;
	TCHAR szFileName[MAX_PATH];
	CComboBox aSelectComboBox;
	aSelectComboBox.Attach(GetDlgItem(IDC_COMBO_SELECTFILE)->m_hWnd);
	aSelectComboBox.GetWindowText(szFileName,_countof(szFileName));
	aSelectComboBox.Detach();




	//TCHAR szScriptFileName[MAX_PATH];
	CString szScriptPath;
	GetModulePath(szScriptPath);
	szScriptPath += _T("\\");
	//▼1.0.0.1061
	CString szSubScriptPath = szScriptPath;
	szSubScriptPath += SUBSCRIPTPATH;
	//▲1.0.0.1061

	//▼1.0.0.1069
	CString szModelMaster;
	CString szModelScript;
	if( m_xvRadioSelectMode!=0 )
	{
		TCHAR lpModelFileName[MAX_PATH];
		GetModelFileName( lpModelFileName, _countof(lpModelFileName) );
		szModelMaster = lpModelFileName;

		szModelScript = szScriptPath;
		szModelScript += MODELSCRIPTPATH;

		//▼1.0.0.1072b
		////▼1.0.0.1072
		//szModelScript += _T("\\");
		//szModelScript += szFileName;
		////▲1.0.0.1072

		TCHAR lpBaseDirectory[MAX_PATH];
		GetPrivateProfileString(_T("Setting"), _T("BaseDirectory"), _T(""), lpBaseDirectory, _countof(lpBaseDirectory), szModelMaster);
		if( _tcslen(lpBaseDirectory)>0 )
		{
			szModelScript += _T("\\");
			szModelScript += lpBaseDirectory;
		}
		//▲1.0.0.1072b

	}
	//▲1.0.0.1069

	szScriptPath += SCRIPTPATH;
	szScriptPath += _T("\\");
	szScriptPath += szFileName;

	m_ResultListBox.ResetContent();
	m_ResultListBox.UpdateWindow();

	EnableObject(FALSE);

//OutputDebugString(_T(" \n"));
//OutputDebugString(_T("StInspectMain \n"));

	m_StInspectMain = new StInspectMain(this);
	m_StInspectMain->SetCheckMode(m_ForwardingCheckMode);
	m_StInspectMain->SetFactoryMode(m_FactoryMode);
	m_StInspectMain->SetStructPortName(&m_StructPortName);
	//▼1.00.1017
	m_StInspectMain->SetStructAddress(&m_StructAddress);
	//▲1.00.1017


	//▼1.0.0.1069
	//m_StInspectMain->SetScriptFileName(szScriptPath);
	if( m_xvRadioSelectMode==0 )
	{
		m_StInspectMain->SetScriptFileName(szScriptPath);
	}
	else
	{
		m_StInspectMain->ClearScriptModelFileNames();
		TCHAR szKeyName[64];
		TCHAR szFeatureFile[MAX_PATH];
		TCHAR szFeatureName[MAX_PATH];
		INT nCount=0;
		//▼1.0.0.1072
		m_StInspectMain->ClearCheckListIndex();
		INT nCheckListIndex = -1;
		m_StInspectMain->ClearCheckMode();
		//▲1.0.0.1072



		//▼1.0.0.1072b
		INT nMode = CheckFeaturesWritingFormat(szModelMaster);
		if( nMode )
		{
			//数字なしモード
			InitializeCheckListNonNumberMode(szModelMaster, szModelScript);
		}
		else
		{

		//▲1.0.0.1072b

		do
		{
			BOOL bAdd = TRUE;
			_stprintf_s(szKeyName,_countof(szKeyName),_T("FileName_%i"),nCount);
			GetPrivateProfileString(_T("Features"), szKeyName, _T(""), szFeatureFile, _countof(szFeatureFile), szModelMaster);
			
			//▼1.0.0.1072
			//if( _tcslen(szFeatureFile)==0 ) break;
			if( _tcscmp(szFeatureFile,_T("end"))==0 ) break;
			if( nCount>100 ) break;	//※とりあえず100まで
			if( _tcslen(szFeatureFile)==0 )
			{
				nCount++;
				continue;
			}
			//▲1.0.0.1072


			_stprintf_s(szKeyName,_countof(szKeyName),_T("FeatureName_%i"),nCount);
			GetPrivateProfileString(_T("Features"), szKeyName, _T(""), szFeatureName, _countof(szFeatureName), szModelMaster);

			_stprintf_s(szKeyName,_countof(szKeyName),_T("CheckMode_%i"),nCount);
			INT nCheckMode = GetPrivateProfileInt(_T("Features"), szKeyName, 0xFFFF, szModelMaster);

			if( _tcslen(szFeatureName)!=0 )
			{
				bAdd = IsCheckFeature(szFeatureName);
				//▼1.0.0.1072
				nCheckListIndex++;
				//▲1.0.0.1072
			}
			if( bAdd )
			{
				m_StInspectMain->AddScriptModelFileName(szModelScript + _T("\\") + szFeatureFile);
				//▼1.0.0.1072
				m_StInspectMain->AddCheckListIndex(_tcslen(szFeatureName)!=0?nCheckListIndex:-1);
				m_StInspectMain->AddCheckMode(nCheckMode);
				//▲1.0.0.1072

				//▼1.0.0.1072a
				//m_StInspectMain->AddScriptFileName(szFeatureFile);
				//m_StInspectMain->AddScriptFileInfo(szFeatureInfo);
				//▲1.0.0.1072a
			}
			nCount++;
		}while(1);


		//▼1.0.0.1072b
		}
		//▲1.0.0.1072b

	}
	//▲1.0.0.1069

	//▼1.0.0.1072
	m_nCheckListBoxIndex = -1;
	//▲1.0.0.1072

	//▼1.0.0.1061
	m_StInspectMain->SetSubScriptPath(szSubScriptPath);
	//▲1.0.0.1061
	//▼1.0.0.1026
	m_StInspectMain->SetSoftwareVersion(m_szSoftwareVersion);
	//▲1.0.0.1026
	//▼1.0.0.1062
	m_StInspectMain->SetScriptCheckMode(m_nScriptCheckMode);
	//▲1.0.0.1062


//OutputDebugString(_T("StInspectMain Execute \n"));
	m_StInspectMain->Execute();

//OutputDebugString(_T("StInspectMain SetStructLightChannel \n"));
	//▼1.00.1020
	m_StInspectMain->SetStructLightChannel(&m_StructLightChannel);
	//▲1.00.1020


//OutputDebugString(_T("SetPriorityClass \n"));

	//StInspectMain aInspect(this);
	//aInspect.SetCheckMode(m_ForwardingCheckMode);
	//aInspect.SetStructPortName(&m_StructPortName);
	//aInspect.SetScriptFileName(szScriptPath);
	//aInspect.Execute();

	//このスレッドの優先度を上げる
	HANDLE hProcess = GetCurrentProcess();
	SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);


//OutputDebugString(_T("SetTimer \n"));
	//画像表示がうまくいかないため、メッセージ表示をTimerをつかうようにした。
	//▼1.0.0.1014
	SetTimer( ST_DISPLAYMAIN, 500, NULL );	//100mS間隔
	//SetTimer( ST_DISPLAYMAIN, 100, NULL );	//100mS間隔
	//SetTimer( ST_DISPLAYMAIN, 150, NULL );	//100mS間隔
	//▲1.0.0.1014

	/*
	//別スレッドからGUI操作できないためこのようにした。
	//検査結果の画面表示用-----------
	do
	{
		BOOL bReval = aInspect.WaitForInspection(100);
		DisplayBufferListBox();


		if( bReval ) break;

		//PTouch印刷ここで行うしかないか？
		//PTOUCHの実装がある場合必要...............
		PrintPTouch(&aInspect);

	}while(1);

	//終了時１秒ほど待つ
	Sleep(1000);

	EnableObject(TRUE);
	*/


}

BOOL CStGenFactoryScriptDlg::CreatePreview(int sizeX, int sizeY)
{
	
	if( !m_Preview )
	{
		RECT rect;
		GetWindowRect(&rect);
		m_Preview = new CPreview;
		m_Preview->Create(IDD_DIALOG_PREVIEW,CWnd::GetDesktopWindow());
		m_Preview->SetWindowPos(
							NULL,
							rect.right - rect.left,
							0,
							0,0,SWP_NOZORDER | SWP_NOSIZE);

		m_Preview->AdjustWindow(sizeX,sizeY);

		m_Preview->m_hMainGUIWnd = this->m_hWnd;
//TRACE(TEXT("@@@@@@@@@ m_Preview=0x%x\n"), m_Preview );

	}
	m_Preview->ShowWindow(SW_SHOW);
	::ShowWindowAsync(m_Preview->m_hWnd,SW_SHOW);
	
	return TRUE;
}

void CStGenFactoryScriptDlg::DeletePreview(void)
{
	
	if( m_Preview )
	{
//TRACE(TEXT("@@@@@@DeletePreview  m_Preview=0x%x\n"), m_Preview );
		//なぜかm_Preview->ShowWindow(SW_HIDE);を実行するとフリーズする。。。
		//理由わからず。。。。。。
		//m_Preview->ShowWindow(SW_HIDE);
		delete m_Preview;
		m_Preview=NULL;
	}
	
}
//▼1.0.0.1047
//BOOL CStGenFactoryScriptDlg::CreateWaveform(int sizeX, int sizeY)
BOOL CStGenFactoryScriptDlg::CreateWaveform(int sizeX, int sizeY, int nRuledLine)
//▲1.0.0.1047
{
	
	if( !m_Waveform )
	{
		RECT rect;
		GetWindowRect(&rect);
		m_Waveform = new CWaveformDlg;

		m_Waveform->SetImageSize( sizeX, sizeY );

		m_Waveform->Create(IDD_DIALOG_WAVEFORM,CWnd::GetDesktopWindow());
		//m_Waveform->SetWindowPos(
		//					NULL,
		//					rect.right - rect.left,
		//					0,
		//					0,0,SWP_NOZORDER | SWP_NOSIZE);


		//デスクトップサイズを取得
		CRect	objRectDeskTop;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &objRectDeskTop, 0);


		INT nWindowSizeY = min(m_Waveform->GetDispSizeY(),objRectDeskTop.bottom-1)+35;
		INT nWindowOffsetY = objRectDeskTop.bottom - nWindowSizeY;

		m_Waveform->SetWindowPos( NULL, 0, nWindowOffsetY,
								min(m_Waveform->GetDispSizeX(),objRectDeskTop.right-1),
								nWindowSizeY,
								SWP_NOZORDER|SWP_SHOWWINDOW );
		//m_Waveform->SetWindowPos( NULL, 0, 0,
		//						min(m_Waveform->GetDispSizeX(),objRectDeskTop.right-1),
		//						min(m_Waveform->GetDispSizeY(),objRectDeskTop.bottom-1)+35,
		//						SWP_NOZORDER|SWP_SHOWWINDOW );

		//m_Waveform->AdjustWindow(sizeX,sizeY);

		//m_Waveform->m_hMainGUIWnd = this->m_hWnd;
		//▼1.0.0.1047
		m_Waveform->SetRuledLine(nRuledLine);
		//▲1.0.0.1047
	}
	m_Waveform->ShowWindow(SW_SHOW);
	::ShowWindowAsync(m_Waveform->m_hWnd,SW_SHOW);
	
	return TRUE;
}

void CStGenFactoryScriptDlg::DeleteWaveform(void)
{
	
	if( m_Waveform )
	{
		//m_Waveform->ShowWindow(SW_HIDE);
		delete m_Waveform;
		m_Waveform=NULL;
	}
	
}

void UpdateLogListBoxScrollBar(CListBox *pListBox)
{
	CDC *pDC = pListBox->GetDC();
	CFont *pFont = pListBox->GetFont();

	//リストボックスのフォントを選択し古いフォントを保存
	CFont *pOldFont = pDC->SelectObject(pFont);

	//テキストメトリックを取得
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	int dx = 0;
	for(int i = 0; i < pListBox->GetCount(); i++)
	{
		CString str;
		pListBox->GetText(i, str);
		CSize sz = pDC->GetTextExtent(str);

		//
		sz.cx += tm.tmAveCharWidth;

		if(dx < sz.cx) dx = sz.cx;
	}

	//フォントの選択を戻す
	pDC->SelectObject(pOldFont);
	pListBox->ReleaseDC(pDC);

	pListBox->SetHorizontalExtent(dx);

	pListBox->SendMessage(WM_VSCROLL, SB_BOTTOM, 0);

}

void CStGenFactoryScriptDlg::DisplayListBox(CString szMessge)
{

	::WaitForSingleObject(hDisplayMutex,INFINITE);

	m_vDisplayString.push_back(szMessge);

	::ReleaseMutex(hDisplayMutex);

	/*
	//HWND hListBox = ::GetDlgItem(m_hWnd, IDC_LIST_RESULT);
	//CListBox *pListBox = (CListBox *)CWnd::FromHandle( hListBox );
	CListBox *pListBox = &m_ResultListBox;
	//int nIndex = pListBox->AddString(szMessge);
	int nIndex = m_ResultListBox.AddString(szMessge);
	m_ResultListBox.UpdateWindow();

	//スクロールバー更新
	UpdateLogListBoxScrollBar(pListBox);


	pListBox->UpdateWindow();
	//if( pIndex ) *pIndex = nIndex;
	UpdateData(FALSE);
	*/
}

//▼1.0.0.1072
void CStGenFactoryScriptDlg::SetListBoxCurSel(INT nIndex)
{
	m_nCheckListBoxIndex = nIndex;
}
void CStGenFactoryScriptDlg::ListBoxCurSel(INT nIndex)
{
	if( nIndex<0 ) nIndex = m_nCheckListBoxIndex;
	if( nIndex>=0 ) 
		m_listCheckItem.SetCurSel(nIndex);
}
//▲1.0.0.1072

//▼1.0.0.1067a
//表示を別スレッドにする。グラフ等のダイアログをクリックするとフリーズするから
unsigned __stdcall ResultDisplayThread( void *pParam )
{
	CStGenFactoryScriptDlg * aDlg = (CStGenFactoryScriptDlg *)pParam;
	aDlg->DisplayBufferListBox();
	//▼1.0.0.1072
	aDlg->ListBoxCurSel();
	//▲1.0.0.1072
	_endthreadex(0);
	return 0;
}

void CStGenFactoryScriptDlg::DisplayBufferListBoxThread(void)
{
	unsigned int nThreadID;
	m_hDisplayThread = (HANDLE)_beginthreadex( NULL, 0, ResultDisplayThread, this,  0, &nThreadID );
}
//▲1.0.0.1067a

void CStGenFactoryScriptDlg::DisplayBufferListBox(void)
{

	//{
	//	static int iCount=0;
	//	TCHAR szText[256];
	//	_stprintf_s(szText,_countof(szText),TEXT("IN 1(%i) DisplayBufferListBox\n"),iCount++);
	//	OutputDebugString(szText);
	//}

	if( m_vDisplayString.size()==0 ) return;

	::WaitForSingleObject(hDisplayMutex,INFINITE);

	//HWND hListBox = ::GetDlgItem(m_hWnd, IDC_LIST_RESULT);
	//CListBox *pListBox = (CListBox *)CWnd::FromHandle( hListBox );


//{
//TCHAR szText[256];
//_stprintf_s(szText,_countof(szText),TEXT("m_vDisplayString.size()(%i)\n"),m_vDisplayString.size());
//OutputDebugString(szText);
//}

	for( DWORD i=0; i<m_vDisplayString.size(); i++ )
	{
		UpdateData(TRUE);
		if( i==0 )
		{
			::SetFocus(m_ResultListBox.m_hWnd);
		}
		int nIndex = m_ResultListBox.AddString(m_vDisplayString[i]);
		//UpdateLogListBoxScrollBar(pListBox);
		UpdateLogListBoxScrollBar(&m_ResultListBox);
		m_ResultListBox.UpdateWindow();
		UpdateData(FALSE);

		//▼1.0.0.1067
//		DoEvents();
		//▲1.0.0.1067

	}

	m_vDisplayString.clear();

	::ReleaseMutex(hDisplayMutex);

	//▼1.0.0.1067
//	DoEvents();
	//▲1.0.0.1067


}
//なぜかPTouchはメインスレッドで動作させないと、フリーズしてしまう。
BOOL CStGenFactoryScriptDlg::PrintPTouch(void *pInspect)
{
	BOOL bReval=FALSE;

	StInspectMain *aInspect = (StInspectMain *)pInspect;

	if( aInspect->IsExecutePrintPTouch()==FALSE ) return FALSE;

	//WaitForSingleObject(hPTouchMutex,INFINITE);
	IDocumentPtr pObjDoc(__uuidof(bpac::Document));

	StPTouch *aPTouch= (StPTouch *)aInspect->GetPTouch();
	while( aPTouch )
	{
		INT nConfirmMessage = aPTouch->GetConfirmMessage();
		if( nConfirmMessage!=0 )
		{
			if( IDYES != AfxMessageBoxHooked(_T("P-touchラベル印刷しますか?"), MB_ICONQUESTION | MB_YESNO) )
			{
				aInspect->SetPTouchErrorCode(0);
				break;
			}
		}
		CString szLabel;
		aPTouch->GetLabel(szLabel);
		TCHAR szLabelFullFileName[MAX_PATH];
		CStCommon::GetExeFilePath(szLabelFullFileName, _countof(szLabelFullFileName));
		_tcscat_s(szLabelFullFileName, _countof(szLabelFullFileName), _T("\\CONFIG\\PTouch\\"));
		_tcscat_s(szLabelFullFileName, _countof(szLabelFullFileName), szLabel);

		CString strPath = szLabelFullFileName;
		BSTR bstrPath = strPath.AllocSysString();
		if (pObjDoc->Open(bstrPath))
		{
			INT nIDCount = aPTouch->GetIDCount();
			for( int i=0; i<nIDCount; i++ )
			{
				CString szID;
				aPTouch->GetID(i,szID);

				CString szText;
				aPTouch->GetText(szID,szText);
				if( pObjDoc->GetObject(_bstr_t(szID)) )
				{
					pObjDoc->GetObject(_bstr_t(szID))->PutText(szText.AllocSysString());
				}
			}
			// 印刷設定開始
			pObjDoc->StartPrint(_bstr_t(_T("")), bpac::bpoDefault);
			// 印刷ジョブの追加 (1部印刷)
			pObjDoc->PrintOut(1, bpac::bpoDefault);
			// 印刷設定終了(=印刷開始)
			pObjDoc->EndPrint();
			// テンプレートクローズ
			pObjDoc->Close();
			aInspect->SetPTouchErrorCode(0);
		}
		else
		{
			aInspect->SetPTouchErrorCode(1);
		}
		break;
	}
	//ErrorCode代入


	aInspect->SetExecutePrintPTouch(FALSE); //終わったらFALSEにする。

	//ReleaseMutex(hPTouchMutex);
	return bReval;
}




void CStGenFactoryScriptDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	//▼1.0.0.1062
	if( m_pErrorListDlg )
	{
		delete m_pErrorListDlg;
	}
	//▲1.0.0.1062

	DeletePreview();

	if( m_StInspectMain ) delete m_StInspectMain;


	CloseHandle(hDisplayMutex);
	//CloseHandle(hPTouchMutex);

}


void CStGenFactoryScriptDlg::OnCbnSelchangeComboSelectfile()
{

	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	WriteSelectFile();

	//リストボックス
	InitializeListBox();

	//▼1.0.0.1072
	SetLockCheckBox(TRUE);
	LockComboBox();
	//▲1.0.0.1072

}


void CStGenFactoryScriptDlg::OnBnClickedButtonTest()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	//HRESULT aResult = CoInitialize(NULL);


	StPTouch aPTouch;
	aPTouch.function(_T("SetLabel"), _T("GigE-Factory.lbx"));
	aPTouch.function(_T("AddID"), _T("MACADDRESS"));
	aPTouch.function(_T("MACADDRESS"), _T("11-22-33-44-55-66"));
	aPTouch.Execute();


//PtouchLabelPrint( _T("12A4567"), _T("abcd"), _T("STC-QQQ"), _T("001122334455") );


	//StInputSerialDlg aDlg;
	//aDlg.DoModal();

	//StInputSerialDlg *pDlg = new StInputSerialDlg(_T("StInputSerialDlg"), _T("aInputSerialDlg"), NULL );

	//pDlg->DoModal();

	//delete pDlg;



}

void CStGenFactoryScriptDlg::EnableObject(BOOL bEnabled)
{
	UpdateData(TRUE);

	//CComboBox aCombobox;
	//aCombobox.Attach(GetDlgItem(IDC_COMBO_SELECTFILE)->m_hWnd);
	//aCombobox.EnableWindow(bEnabled);
	//aCombobox.Detach();

	//CButton aButton;
	//aButton.Attach(GetDlgItem(IDC_BUTTON_EXECUTE)->m_hWnd);
	//aButton.EnableWindow(bEnabled);
	//aButton.Detach();

	//aButton.Attach(GetDlgItem(IDOK)->m_hWnd);
	//aButton.EnableWindow(bEnabled);
	//aButton.Detach();

	//aButton.Attach(GetDlgItem(IDCANCEL)->m_hWnd);
	//aButton.EnableWindow(bEnabled);
	//aButton.Detach();


	//CWnd aWnd;
	UINT uiID[] = { IDC_COMBO_SELECTFILE, IDC_BUTTON_EXECUTE, IDOK, IDCANCEL };
	for( int i=0; i<_countof(uiID); i++ )
	{
		CWnd* pWnd = this->GetDlgItem(uiID[i]);
		pWnd->EnableWindow(bEnabled);

	//	aWnd.Attach(GetDlgItem(uiID[i])->m_hWnd);
	//	aWnd.EnableWindow(bEnabled);
	//	aWnd.Detach();
	}
	
	UpdateData(FALSE);
}

void CStGenFactoryScriptDlg::OnMenuitemSelectPort()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	CSelComPortDlg aDlg;
	aDlg.SetStructPortName(&m_StructPortName);

	aDlg.DoModal();

}

TCHAR * _tcsrstr(TCHAR *s, TCHAR *f)
{
    size_t slen = _tcslen(s);
    size_t flen = _tcslen(f);
    TCHAR *p;

    if (slen < flen) {
        return NULL;
    }

    for (p = s + slen - flen; p >= s; p--) {
        if (_tcsncmp(p, f, flen) != 0) {
            continue;
        }
        return p;
    }
    return NULL;
}
BOOL BackFolder( LPTSTR pszLogFileName )
{
	BOOL bReval = FALSE;
	PTCHAR szStr = _tcsrstr(pszLogFileName,_T("\\"));
	if( szStr )
	{
		_tcscpy( szStr, _T("") );
		bReval = TRUE;
	}
	return bReval;
}

void CStGenFactoryScriptDlg::OnMenuitemOpenLogfolder()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	
	TCHAR	szLogFileName[1024];
	TCHAR	szCheckDate[MAX_PATH];
	//if( m_iCurCameraTypeListNo>=m_iCameraTypeListNum ) return;
	//PS_TARGET_CAMERA_TYPE psTargetCameraType = &m_psCameraTypeList[m_iCurCameraTypeListNo];

	//検査時間情報の取得
	struct tm today;
	__time64_t long_time;
	_time64( &long_time ); 
	_localtime64_s( &today, &long_time ); 

	_stprintf_s(szCheckDate, _countof(szCheckDate),TEXT("%04d%02d%02d"),today.tm_year + 1900 ,today.tm_mon + 1,today.tm_mday);

	//ログファイル名を設定
	//GetModuleFileName(NULL,szLogFileName,MAX_PATH);
	//wcscpy_s(_tcsrchr(szLogFileName,'\\'),_countof(szLogFileName)-wcslen(szLogFileName),_T(""));
	//CStCommon::GetLogFilePath(szLogFileName, MAX_PATH);
	CStCommon::GetIniFilePath( szLogFileName, _countof(szLogFileName) );

	
	_stprintf_s(szLogFileName,_countof(szLogFileName),TEXT("%s\\Check"),szLogFileName);
	do
	{
		if( gbCheckDirectoryExist(szLogFileName,FALSE) )
		{
			ShellExecute( NULL, _T("open"), szLogFileName, NULL, NULL, SW_SHOWNORMAL );
			break;
		}
		else
		{

			BOOL bReval = BackFolder(szLogFileName);
			if( !bReval )
			{
				AfxMessageBoxHookedID(IDS_NOT_EXIST_LOGFOLDER,MB_OK);
				break;
			}
		}
	}while(1);
	
	
}

void CStGenFactoryScriptDlg::OnMenuitemCheckType(UINT ID) 
{
	m_ForwardingCheckMode = ID - ID_MENUITEM_CHECK_TYPE_0;
	//SetCheckItemStatus(TRUE);
	//SetDialogText();
	TCHAR		szIniFileName[MAX_PATH];
	CStCommon::GetIniFileName(szIniFileName, MAX_PATH);
	CString szData;
	szData.Format(_T("%d"),m_ForwardingCheckMode);
	WritePrivateProfileString( _T("Setting"), _T("ForwardingCheckMode"), szData, szIniFileName);
	//▼1.0.0.1015
	//ボタン名
	SetButtonName(m_ForwardingCheckMode);
	//▲1.0.0.1015
	//▼1.0.0.1072
	m_listCheckItem.SetCheck(0xFFFF,m_ForwardingCheckMode);
	//▲1.0.0.1072
}

void CStGenFactoryScriptDlg::OnMenuitemFactoryType(UINT ID) 
{
	//▼1.0.0.1015
	INT nOrgFactoryMode = m_FactoryMode;
	//▲1.0.0.1015
	m_FactoryMode = ID - ID_FACTORY_JAPAN;
	//SetCheckItemStatus(TRUE);
	//SetDialogText();
	TCHAR		szIniFileName[MAX_PATH];
	CStCommon::GetIniFileName(szIniFileName, MAX_PATH);
	CString szData;
	szData.Format(_T("%d"),m_FactoryMode);
	WritePrivateProfileString( _T("Setting"), _T("FactoryMode"), szData, szIniFileName);
	//▼1.0.0.1015
	if( m_FactoryMode!=nOrgFactoryMode )
	{
		FactoryExistFileRead(m_FactoryMode);
		//▼1.0.0.1069
		//SetSelectFileComboBox();
		SetTextComboBox();
		//▲1.0.0.1069
	}
	//▲1.0.0.1015
}

//▼1.0.0.1015
void CStGenFactoryScriptDlg::OnMenuitemCameraType(UINT ID) 
{
	INT nOrgCameraType = m_CameraType;
	m_CameraType = ID - ID_CAMERATYPE_ALL;
	TCHAR		szIniFileName[MAX_PATH];
	CStCommon::GetIniFileName(szIniFileName, MAX_PATH);
	CString szData;
	szData.Format(_T("%d"),m_CameraType);
	WritePrivateProfileString( _T("Setting"), _T("CameraType"), szData, szIniFileName);
	//▼1.0.0.1072a
	TCHAR szOldModelName[128];
	CComboBox aSelectComboBox;
	aSelectComboBox.Attach(GetDlgItem(IDC_COMBO_SELECTFILE)->m_hWnd);
	aSelectComboBox.GetWindowText(szOldModelName,_countof(szOldModelName));
	aSelectComboBox.Detach();
	//▲1.0.0.1072a
	if( m_CameraType!=nOrgCameraType )
	{
		CameraTypeExistFileRead(m_CameraType);
		//▼1.0.0.1069
		//SetSelectFileComboBox();
		SetTextComboBox();
		//▲1.0.0.1069
	}

	//▼1.0.0.1072a
	TCHAR szCurrentModelName[128];
	aSelectComboBox.Attach(GetDlgItem(IDC_COMBO_SELECTFILE)->m_hWnd);
	aSelectComboBox.GetWindowText(szCurrentModelName,_countof(szCurrentModelName));
	aSelectComboBox.Detach();
	if( _tcscmp(szOldModelName,szCurrentModelName)!=0 )
	{
		//リストボックス
		InitializeListBox();
	}
	//▲1.0.0.1072a

}
//▲1.0.0.1015

void CStGenFactoryScriptDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialogEx::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	CMenu *pobjMenu = GetMenu();

	for(UINT uiID = ID_MENUITEM_CHECK_TYPE_0;uiID <= ID_MENUITEM_CHECK_TYPE_1;uiID++)
	{
		UINT uiChecked = MF_UNCHECKED;
		if((uiID - ID_MENUITEM_CHECK_TYPE_0) == m_ForwardingCheckMode)
			uiChecked = MF_CHECKED;
		pobjMenu->CheckMenuItem(uiID, uiChecked);
	}

	//FactoryMode
	for(UINT uiID = ID_FACTORY_JAPAN;uiID <= ID_FACTORY_KOREA;uiID++)
	{
		UINT uiChecked = MF_UNCHECKED;
		if((uiID - ID_FACTORY_JAPAN) == m_FactoryMode)
			uiChecked = MF_CHECKED;
		pobjMenu->CheckMenuItem(uiID, uiChecked);
	}
	//▼1.0.0.1015
	//CameraType
	//▼1.0.0.1046
	//for(UINT uiID = ID_CAMERATYPE_ALL;uiID <= ID_CAMERATYPE_CL;uiID++)
	for(UINT uiID = ID_CAMERATYPE_ALL;uiID <= ID_CAMERATYPE_CXP;uiID++)
	//▲1.0.0.1046
	{
		UINT uiChecked = MF_UNCHECKED;
		if((uiID - ID_CAMERATYPE_ALL) == m_CameraType)
			uiChecked = MF_CHECKED;
		pobjMenu->CheckMenuItem(uiID, uiChecked);
	}
	//▲1.0.0.1015

	//▼1.0.0.1062
	//ScriptCheckMode
	for(UINT uiID = ID_SCRIPTCHECK_OFF;uiID <= ID_SCRIPTCHECK_ON;uiID++)
	{
		UINT uiChecked = MF_UNCHECKED;
		if((uiID - ID_SCRIPTCHECK_OFF) == m_nScriptCheckMode)
			uiChecked = MF_CHECKED;
		pobjMenu->CheckMenuItem(uiID, uiChecked);
	}
	//▲1.0.0.1062

}





void CStGenFactoryScriptDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	if( ST_DISPLAYMAIN==nIDEvent )
	{
		KillTimer(ST_DISPLAYMAIN);

		do
		{
			//if( !m_StInspectMain )
			//{
			//	//KillTimer(ST_DISPLAYMAIN);
			//	break;
			//}

			//▼1.0.0.1008
			INT nJudge = 0;
			INT nRetryMode = 0;
			if( m_StInspectMain )
				nRetryMode = m_StInspectMain->GetRetryMode();
			//▲1.0.0.1008

			//検査結果の画面表示用-----------
			while(m_StInspectMain)
			{
				//BOOL bReval = m_StInspectMain->WaitForInspection(100);
				BOOL bReval = m_StInspectMain->WaitForInspection(1);
				//▼1.0.0.1067a
				DisplayBufferListBox();
//				DisplayBufferListBoxThread();
				//▲1.0.0.1067a

				//▼1.0.0.1072
				ListBoxCurSel();
				//▲1.0.0.1072

				if( bReval )	//終了
				{
					//▼1.0.0.1008
					m_StInspectMain->GetResult((PDWORD)&nJudge);
					//▲1.0.0.1008

					//KillTimer(ST_DISPLAYMAIN);
					//終了時１秒ほど待つ
					Sleep(1000);

					EnableObject(TRUE);

					//▼1.0.0.1062
					INT nErrorListCount = m_StInspectMain->GetErrorListCount();
					if( nErrorListCount>0 )
					{
						if( !m_pErrorListDlg )
						{
							m_pErrorListDlg = new CErrorListDlg;
						}
	
						if( m_pErrorListDlg && !m_pErrorListDlg->m_hWnd )
						{
							m_pErrorListDlg->Create( CErrorListDlg::IDD, NULL );
							RECT rc1;
							m_pErrorListDlg->GetWindowRect(&rc1);
							//INT nPosX = m_pErrorListDlg->GetWindowPosX();
							INT nPosX = 50;
							if( nPosX<0 )	nPosX = rc1.left;
							//INT nPosY = m_pErrorListDlg->GetWindowPosY();
							INT nPosY = 50;
							if( nPosY<0 )	nPosY = rc1.top;
							m_pErrorListDlg->SetWindowPos( NULL, nPosX, nPosY, rc1.right-rc1.left, rc1.bottom-rc1.top, SWP_NOZORDER );
						}
						if( m_pErrorListDlg )
						{
							CString szTmp, szText;
							m_pErrorListDlg->ClearStringList();
							for( int i=0; i<nErrorListCount; i++ )
							{
								m_StInspectMain->GetErrorListString(szTmp,i);
								//m_pErrorListDlg->SetStringList(szText);
								szText += szTmp;
								szText += _T("\r\n");
							}
							m_pErrorListDlg->SetStringList(szText);
							m_pErrorListDlg->ShowWindow(SW_SHOW);
//							DoEvents();
						}
					}

					//▲1.0.0.1062

					delete m_StInspectMain;				
					m_StInspectMain = NULL;
					break;
				}
				//PTouch印刷ここで行うしかないか？
				//PTOUCHの実装がある場合必要...............
				PrintPTouch(m_StInspectMain);
				break;
			}
			if( !m_StInspectMain )
			{
				//KillTimer(ST_DISPLAYMAIN);
				//ここで繰返し処理
				//▼1.0.0.1008
				//判定がNGの場合は終了させる。
				if( nRetryMode && nJudge )
				{
					//▼1.0.0.1062
					if( m_nScriptCheckMode==0 )
					//▲1.0.0.1062
					SetTimer( ST_RETRY, 500, NULL );
				}
				//▲1.0.0.1008

				break;
			}
			//▼1.0.0.1014
			SetTimer( ST_DISPLAYMAIN, 100, NULL );	//100mS間隔
			//SetTimer( ST_DISPLAYMAIN, 300, NULL );
			//▲1.0.0.1014

		}while(0);

	}

	//▼1.0.0.1008
	else
	if( ST_RETRY==nIDEvent )
	{
		KillTimer(ST_RETRY);
		OnBnClickedButtonExecute();
	}
	//▲1.0.0.1008

	CDialogEx::OnTimer(nIDEvent);
}

//▼1.00.1017
void CStGenFactoryScriptDlg::OnMenuitemSettingterminal()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	CSettingTerminalDlg aDlg;
	aDlg.SetStructAddress(&m_StructAddress);

	aDlg.DoModal();

}
//▲1.00.1017


void CStGenFactoryScriptDlg::OnMenuitemSettinglightchannel()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	SettingLightChannelDlg aDlg;
	aDlg.SetStructLightChannel(&m_StructLightChannel);

	aDlg.DoModal();

}

//▼1.0.0.1062
void CStGenFactoryScriptDlg::OnMenuitemScriptcheck(UINT ID) 
{
	INT nOrgScriptCheckMode = m_nScriptCheckMode;
	m_nScriptCheckMode = ID - ID_SCRIPTCHECK_OFF;
	//TCHAR		szIniFileName[MAX_PATH];
	//CStCommon::GetIniFileName(szIniFileName, MAX_PATH);
	//CString szData;
	//szData.Format(_T("%d"),m_nScriptCheckMode);
	//WritePrivateProfileString( _T("Setting"), _T("CameraType"), szData, szIniFileName);
	if( m_nScriptCheckMode!=nOrgScriptCheckMode )
	{
		CString szText;
		if( m_nScriptCheckMode!=0 ) szText = _T("スクリプトチェック");
		GetDlgItem(IDC_STATIC_SCRIPTCHECK)->SetWindowText( szText );
	}
}
//▲1.0.0.1062

//▼1.0.0.1069
void CStGenFactoryScriptDlg::UpdateCheckBoxEnabled(BOOL bEnabled)
{
	m_listCheckItem.EnableWindow(bEnabled);
}

void CStGenFactoryScriptDlg::InitializeCheckBox(BOOL bChecked)
{
	//▼1.0.0.1072
	//INT nCount = m_listCheckItem.GetCount();
	//for( int i = 0; i < nCount; i ++ )
	//{
	//	m_listCheckItem.SetCheckItem( i, bChecked?BST_CHECKED : BST_UNCHECKED );
	//}

	m_listCheckItem.SetCheck(0xFFFF,m_ForwardingCheckMode,bChecked);
	//▲1.0.0.1072

	TCHAR		szIniFileName[MAX_PATH];
	CStCommon::GetIniFileName(szIniFileName, MAX_PATH);
	CString szData;
	szData.Format(_T("%d"),m_bEnabledCheckBox);
	WritePrivateProfileString( _T("Setting"), _T("EnabledCheckBox"), szData, m_szIniFileName);
}

void CStGenFactoryScriptDlg::OnMenuitemCheckBoxEnabled()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	m_bEnabledCheckBox = !m_bEnabledCheckBox;
	UpdateCheckBoxEnabled(m_bEnabledCheckBox);
}


void CStGenFactoryScriptDlg::OnMenuitemCheckBoxAllChecked()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	InitializeCheckBox();
}


void CStGenFactoryScriptDlg::OnMenuitemCheckBoxAllUnchecked()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	InitializeCheckBox(FALSE);
}


void CStGenFactoryScriptDlg::OnInitMenu(CMenu* pMenu)
{
	CDialogEx::OnInitMenu(pMenu);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	if(m_bEnabledCheckBox)	pMenu->CheckMenuItem(ID_MENUITEM_CHECK_BOX_ENABLED,MF_CHECKED);
	else					pMenu->CheckMenuItem(ID_MENUITEM_CHECK_BOX_ENABLED,MF_UNCHECKED);
}

void CStGenFactoryScriptDlg::OnRadioitemScript(UINT ID) 
{
	UpdateData();

	SetTextComboBox();
	//リストボックス
	InitializeListBox();

	CString szData;
	szData.Format(_T("%d"),m_xvRadioSelectMode);
	WritePrivateProfileString( _T("Setting"), _T("RadioSelectMode"), szData, m_szIniFileName);

}


BOOL CStGenFactoryScriptDlg::GetModelFileName(LPTSTR lpModelFileName, size_t size)
{
	BOOL bReval = FALSE;

	//ComboBoxから機種名取得
	CString szDisplayModelName;
	CComboBox aSelectComboBox;
	aSelectComboBox.Attach(GetDlgItem(IDC_COMBO_SELECTFILE)->m_hWnd);

	//▼1.0.0.72
	//aSelectComboBox.GetLBText(aSelectComboBox.GetCurSel(),szDisplayModelName);
	if( aSelectComboBox.GetCurSel()>=0 )
		aSelectComboBox.GetLBText(aSelectComboBox.GetCurSel(),szDisplayModelName);
	//▲1.0.0.72
	
	aSelectComboBox.Detach();
	if( szDisplayModelName.GetLength()==0 ) return FALSE;


	//ファイル検査して機種名とDisplayNameが一致するファイル名取得
	WIN32_FIND_DATA FindFileData;
	CString szScriptPath;
	GetModulePath(szScriptPath);
	szScriptPath += _T("\\");
	szScriptPath += MODELPATH;
	HANDLE hFind = FindFirstFile( szScriptPath + _T("\\*.txt"), &FindFileData);
	if( hFind!=INVALID_HANDLE_VALUE )
	{
		TCHAR szDisplayName[MAX_PATH];
		CString szModelFileName;
		do
		{
			_tcscpy_s(szDisplayName,_countof(szDisplayName),_T(""));
			szModelFileName = szScriptPath + _T("\\") + FindFileData.cFileName;
			GetPrivateProfileString(_T("Setting"), _T("DisplayName"), _T(""), szDisplayName, _countof(szDisplayName), szModelFileName);
			if( szDisplayModelName==szDisplayName )
			{
				_tcscpy_s(lpModelFileName,size,szModelFileName);
				bReval = TRUE;
				break;
			}
		}while( FindNextFile( hFind, &FindFileData) );
		FindClose(hFind);
	}
	return bReval;
}


//▼1.0.0.1072a
//Featuresの書き方チェック
//return:  0:_xxの数字あり  1:数字なし
INT CStGenFactoryScriptDlg::CheckFeaturesWritingFormat(LPCTSTR lpModelFileName)
{

	INT nMode=1;

	TCHAR szKeyName[64];
	TCHAR szString[MAX_PATH];
	_stprintf_s( szKeyName, _countof(szKeyName), _T("FileName") );
	GetPrivateProfileString(_T("Features"), szKeyName, _T(""), szString, _countof(szString), lpModelFileName);
	if( _tcslen(szString)>0)	nMode=1;
	else	nMode=0;
	return nMode;

}

BOOL CStGenFactoryScriptDlg::InitializeListBoxNonNumberMode(LPCTSTR lpModelFileName)
{
	BOOL bReval = FALSE;
	
	
	//INT nMode=1;

	//TCHAR szKeyName[64];
	//TCHAR szString[MAX_PATH];
	//_stprintf_s( szKeyName, _countof(szKeyName), _T("FileName") );
	//GetPrivateProfileString(_T("Features"), szKeyName, _T(""), szString, _countof(szString), lpModelFileName);
	//if( _tcslen(szString)>0)	nMode=1;
	//else	nMode=0;

	CStdioFile modelFile;

	if( modelFile.Open(lpModelFileName,CFile::modeRead,NULL)!=0 )
	{
		modelFile.SeekToBegin();
		CString szLine;
		BOOL bInFeatures=FALSE;
		INT nIndex = 0;

		CString szFileName(_T(""));
		CString szFeatureName(_T(""));
		CString szCheckMode(_T(""));
		INT nCheckMode=0xFFFF;

		while( modelFile.ReadString(szLine) )
		{
			szLine.Trim();
			if( bInFeatures==FALSE )
			{
				if( szLine==_T("[Features]") )
				{
					bInFeatures = TRUE;
				}
			}
			else
			{
				if( szLine.Left(1)==_T("[") && szLine.Right(1)==_T("]") )	//次のセクションで終了
					break;
				
				// = を検索
				INT nEqualPos = szLine.Find(_T("="));
				if( nEqualPos>0 )
				{
					CString szValue = szLine.Mid(nEqualPos+1).Trim();

					if( szLine.Left(nEqualPos).Trim()==_T("FileName") )
					{
						//前のリスト表示
						if( szFeatureName.GetLength()>0 )
							m_listCheckItem.AddItem(szFeatureName, nIndex++, nCheckMode);
						szFileName = szValue;
						szFeatureName = _T("");	//初期化
						nCheckMode = 0xFFFF;	//初期化
						//-------------------
					}
					else if( szLine.Left(nEqualPos).Trim()==_T("FeatureName") )
					{
						if( szFeatureName.GetLength()==0 )
							szFeatureName = szValue;
					}
					else if( szLine.Left(nEqualPos).Trim()==_T("CheckMode") )
					{
						if( nCheckMode==0xFFFF )
							nCheckMode = _ttoi(szValue);
					}
				}
			}
		}

		if( szFeatureName.GetLength()>0 )
			m_listCheckItem.AddItem(szFeatureName, nIndex++, nCheckMode);

		if( nIndex>0 )
			bReval=TRUE;

	}

	return bReval;
}


BOOL CStGenFactoryScriptDlg::InitializeCheckListNonNumberMode(LPCTSTR lpModelFileName,LPCTSTR szModelScript)
{
	BOOL bReval = FALSE;
	

	CStdioFile modelFile;

	if( modelFile.Open(lpModelFileName,CFile::modeRead,NULL)!=0 )
	{
		modelFile.SeekToBegin();
		CString szLine;
		BOOL bInFeatures=FALSE;
		INT nIndex = 0;

		CString szFileName(_T(""));
		CString szFeatureName(_T(""));
		CString szCheckMode(_T(""));
		INT nCheckMode=0xFFFF;

		while( modelFile.ReadString(szLine) )
		{
			szLine.Trim();
			if( bInFeatures==FALSE )
			{
				if( szLine==_T("[Features]") )
				{
					bInFeatures = TRUE;
				}
			}
			else
			{
				if( szLine.Left(1)==_T("[") && szLine.Right(1)==_T("]") )	//次のセクションで終了
					break;
				
				// = を検索
				INT nEqualPos = szLine.Find(_T("="));
				if( nEqualPos>0 )
				{
					CString szValue = szLine.Mid(nEqualPos+1).Trim();

					if( szLine.Left(nEqualPos).Trim()==_T("FileName") )
					{
						//前のリスト表示
						
						//if( szFeatureName.GetLength()>0 )
						//	m_listCheckItem.AddItem(szFeatureName, nIndex++, nCheckMode);


						BOOL bAdd=FALSE;
						if( _tcslen(szFileName)!=0 )
						{
							bAdd=TRUE;
							if( _tcslen(szFeatureName)!=0 )
							{
								bAdd = IsCheckFeature(szFeatureName);
								nIndex++;
							}
						}
						if( bAdd )
						{
							m_StInspectMain->AddScriptModelFileName(CString(szModelScript) + _T("\\") + szFileName);
							m_StInspectMain->AddCheckListIndex(_tcslen(szFeatureName)!=0?nIndex:-1);
							m_StInspectMain->AddCheckMode(nCheckMode);
						}

						szFileName = szValue;
						szFeatureName = _T("");	//初期化
						nCheckMode = 0xFFFF;	//初期化
						//-------------------
					}
					else if( szLine.Left(nEqualPos).Trim()==_T("FeatureName") )
					{
						if( szFeatureName.GetLength()==0 )
							szFeatureName = szValue;
					}
					else if( szLine.Left(nEqualPos).Trim()==_T("CheckMode") )
					{
						if( nCheckMode==0xFFFF )
							nCheckMode = _ttoi(szValue);
					}
				}
			}
		}

		//---------------------------------------------
		BOOL bAdd=FALSE;
		if( _tcslen(szFileName)!=0 )
		{
			bAdd=TRUE;
			if( _tcslen(szFeatureName)!=0 )
			{
				bAdd = IsCheckFeature(szFeatureName);
				nIndex++;
			}
		}
		if( bAdd )
		{
			m_StInspectMain->AddScriptModelFileName(CString(szModelScript) + _T("\\") + szFileName);
			m_StInspectMain->AddCheckListIndex(_tcslen(szFeatureName)!=0?nIndex:-1);
			m_StInspectMain->AddCheckMode(nCheckMode);
		}
		//---------------------------------------------

		if( nIndex>0 )
			bReval=TRUE;

	}

	return bReval;
}


//▲1.0.0.1072a

void CStGenFactoryScriptDlg::InitializeListBox()
{
	m_listCheckItem.ResetContent();

	if( m_xvRadioSelectMode!=0 )
	{
		TCHAR lpModelFileName[MAX_PATH];
		BOOL bReval = GetModelFileName( lpModelFileName, _countof(lpModelFileName) );
		if( bReval )
		{
			//▼1.0.0.1072a
			INT nMode = CheckFeaturesWritingFormat(lpModelFileName);
			if( nMode )
			{
				//数字なしモード
				InitializeListBoxNonNumberMode(lpModelFileName);
			}
			else
			{
			//▲1.0.0.1072a


			TCHAR szKeyName[64];
			TCHAR szString[MAX_PATH];
			INT nCount=0;
			INT nID=0;
			do
			{
				_stprintf_s(szKeyName,_countof(szKeyName),_T("FileName_%i"),nCount);
				GetPrivateProfileString(_T("Features"), szKeyName, _T(""), szString, _countof(szString), lpModelFileName);
				//▼1.0.0.1072
				//if( _tcslen(szString)==0 ) break;
				if( _tcscmp(szString,_T("end"))==0 ) break;
				if( nCount>100 ) break;
				if( _tcslen(szString)==0 )
				{
					nCount++;
					continue;
				}
				//▲1.0.0.1072
				_stprintf_s(szKeyName,_countof(szKeyName),_T("FeatureName_%i"),nCount);
				GetPrivateProfileString(_T("Features"), szKeyName, _T(""), szString, _countof(szString), lpModelFileName);
				if( _tcslen(szString)!=0 )
				{
					//▼1.0.0.1072
					//m_listCheckItem.AddItem(szString, nID++);
					_stprintf_s(szKeyName,_countof(szKeyName),_T("CheckMode_%i"),nCount);
					INT nCheckMode = GetPrivateProfileInt(_T("Features"), szKeyName, 0xFFFF, lpModelFileName);
					m_listCheckItem.AddItem(szString, nID++, nCheckMode);
					//▲1.0.0.1072
				}
				nCount++;
			}while(1);

			//▼1.0.0.1072a
			}
			//▲1.0.0.1072a
		}
	}
	InitializeCheckBox(TRUE);
}

//▲1.0.0.1069


void CStGenFactoryScriptDlg::OnBnClickedCheckLockCameraType()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	LockComboBox();
}
