// StImageMessageDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StImageMessageDlg.h"
#include "afxdialogex.h"
#include "StCommon.h"


// StImageMessageDlg ダイアログ

//▼1.0.0.1048
//IMPLEMENT_DYNAMIC(StImageMessageDlg, CDialogEx)
IMPLEMENT_DYNAMIC(StImageMessageDlg, CCustomDialogEx)
//▲1.0.0.1048

StImageMessageDlg::StImageMessageDlg(CWnd* pParent /*=NULL*/)
//▼1.0.0.1048
	//: CDialogEx(StImageMessageDlg::IDD, pParent)
	: CCustomDialogEx(StImageMessageDlg::IDD, pParent)
//▲1.0.0.1048
{
	m_nImageSizeX = 0;
	m_nImageSizeY = 0;
	m_nMessageSizeX = 0;
	m_nMessageSizeY = 0;
	m_szImageFile = _T("");
	m_szMessage = _T("");
	m_nWindowPosX = 0;
	m_nWindowPosY = 0;

	m_ClassName = _T("StImageMessageDlg");


}

StImageMessageDlg::~StImageMessageDlg()
{
}

void StImageMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	//▼1.0.0.1048
	//CDialogEx::DoDataExchange(pDX);
	CCustomDialogEx::DoDataExchange(pDX);
	//▲1.0.0.1048
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_StaticImage);
	//  DDX_Control(pDX, IDC_STATIC_MESSAGE, m_staticMessage);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_editMessage);
	DDX_Control(pDX, IDC_STATIC_IMAGEMESSAGE, m_staticImageMessage);
}


//▼1.0.0.1048
//BEGIN_MESSAGE_MAP(StImageMessageDlg, CDialogEx)
BEGIN_MESSAGE_MAP(StImageMessageDlg, CCustomDialogEx)
//▲1.0.0.1048
	ON_WM_TIMER()
END_MESSAGE_MAP()


// StImageMessageDlg メッセージ ハンドラー
//▼1.0.0.1048
//void DoEvents()
//{
//	MSG msg;
//	while(::PeekMessage(&msg,NULL,NULL,NULL,PM_NOREMOVE) )
//	{
//		if(!AfxGetApp()->PumpMessage() )
//			break;
//	}
//}
//▲1.0.0.1048

BOOL StImageMessageDlg::OnInitDialog()
{
	//▼1.0.0.1048
	//CDialogEx::OnInitDialog();
	CCustomDialogEx::OnInitDialog();
	//▲1.0.0.1048

	// TODO:  ここに初期化を追加してください
	if( m_szImageFile.GetLength()>0 )
	{
		TCHAR szExePath[MAX_PATH];
		CString szImageFullFile;
		CStCommon::GetExeFilePath(szExePath, _countof(szExePath));
		szImageFullFile += szExePath;

		szImageFullFile += _T("\\");
		szImageFullFile += m_szImageFile;
		HBITMAP aHBITMAP = (HBITMAP)::LoadImage(::AfxGetInstanceHandle(),szImageFullFile,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

		m_StaticImage.SetBitmap(aHBITMAP);
		//m_StaticImage.SetBitmap((HBITMAP)::LoadImage(::AfxGetInstanceHandle(),szImageFullFile,IMAGE_BITMAP,0,0,LR_LOADFROMFILE));
	}
	//CStatic objStatic;
	//objStatic.Attach(GetDlgItem(IDC_STATIC_MESSAGE)->m_hWnd);
	////objStatic.UpdateData(TRUE);
	//objStatic.SetWindowPos( NULL, m_nImageSizeX+15, 5, m_nMessageSizeX, m_nMessageSizeY, SWP_NOZORDER );
	//objStatic.SetWindowText(m_szMessage);
	////objStatic.UpdateData(FALSE);
	//objStatic.Detach();
	//DoEvents();

	//m_staticMessage.SetWindowPos( NULL, m_nImageSizeX+15, 5, m_nMessageSizeX, m_nMessageSizeY, SWP_NOZORDER );
	//m_staticMessage.SetWindowText(m_szMessage);
	//▼1.0.0.1015
	m_staticImageMessage.SetWindowPos( NULL, m_nImageSizeX+15, 10, m_nMessageSizeX, m_nMessageSizeY, SWP_NOZORDER );
	m_staticImageMessage.SetWindowText(m_szMessage);
	//m_editMessage.SetWindowPos( NULL, m_nImageSizeX+15, 5, m_nMessageSizeX, m_nMessageSizeY, SWP_NOZORDER );
	//m_editMessage.SetWindowText(m_szMessage);
	//▲1.0.0.1015
	
	
	
	
	//TRACE( _T("@@@@ SetWindowText=%s\n"), m_szMessage );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void StImageMessageDlg::PostNcDestroy()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	delete m_StaticImage;
	//▼1.0.0.1048
	//CDialogEx::PostNcDestroy();
	CCustomDialogEx::PostNcDestroy();
	//▲1.0.0.1048
}

BOOL StImageMessageDlg::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="SetImageSizeX" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetImageSizeX(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetImageSizeY" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetImageSizeY(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMessageSizeX" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetMessageSizeX(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMessageSizeY" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetMessageSizeY(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetImageFile" )
	{
		SetImageFile(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMessage" )
	{
		SetMessage(szArgument);
		if( m_hWnd )
		{
			//メッセージ更新のため必要
			//▼1.0.0.1043
			//m_editMessage.SetWindowText(m_szMessage);
			m_staticImageMessage.SetWindowText(m_szMessage);
			//▲1.0.0.1043
//TRACE( _T("@@@@ SetWindowText=%s\n"), m_szMessage );
		}
		bReval = TRUE;
	}
	else
	if( szFunction=="SetWindowPosX" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetWindowPosX(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetWindowPosY" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetWindowPosY(nData);
		bReval = TRUE;
	}

	return bReval;

}


void StImageMessageDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	//▼1.0.0.1048
	//CDialogEx::OnTimer(nIDEvent);
	CCustomDialogEx::OnTimer(nIDEvent);
	//▲1.0.0.1048
}
