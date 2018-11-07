// ControlDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "ControlDlg.h"
#include "afxdialogex.h"


// CControlDlg ダイアログ

IMPLEMENT_DYNAMIC(CControlDlg, CDialogEx)

CControlDlg::CControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlDlg::IDD, pParent)
{
	m_szDisplayText="";
	mDeviceWnd = NULL;
}

CControlDlg::~CControlDlg()
{
	CloseGenWindow( &mDeviceWnd );
}

void CControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CControlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CControlDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CControlDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_GENICAM, &CControlDlg::OnBnClickedButtonGenicam)
	ON_BN_CLICKED(IDOK, &CControlDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CControlDlg メッセージ ハンドラー


void CControlDlg::OnBnClickedButtonSend()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if( !m_pDeviceEBus ) return;

	CString lText;
	((CEdit*)GetDlgItem(IDC_EDIT_CONTROL))->GetWindowText( lText );
	lText.Remove(' ');
	lText.Remove('　');
	lText.Remove(',');

	BYTE RcvData[1024];
	size_t sizeRcvData = _countof(RcvData);
	BOOL bReval = m_pDeviceEBus->SerialControl( _T("UART0"), lText, RcvData, &sizeRcvData );


	CString lDisplayText(_T(""));
	if( bReval )
	{
		TCHAR szText[16];
		for( DWORD i=0; i<sizeRcvData; i++ )
		{
			_stprintf_s( szText, _countof(szText), _T("%02x "), RcvData[i] );
			lDisplayText += szText;
		}
	}
	else
	{
		lDisplayText.Format( _T("Error") );
	}
	//lDisplayText += _T("\r\n");
	if( m_szDisplayText.GetLength()>0 )
		m_szDisplayText += _T("\r\n");
	m_szDisplayText += lDisplayText;

	CEdit objEdit;
	objEdit.Attach(GetDlgItem(IDC_EDIT_DISPLAY)->m_hWnd);
	objEdit.SetWindowText(m_szDisplayText);
	objEdit.LineScroll(objEdit.GetLineCount());
	objEdit.Detach();
}


void CControlDlg::OnBnClickedButtonClear()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_szDisplayText = "";
	GetDlgItem(IDC_EDIT_DISPLAY)->SetWindowText( m_szDisplayText );
}


void CControlDlg::OnBnClickedButtonGenicam()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if( !m_pDeviceEBus ) return;
	ShowGenWindow( 
		&mDeviceWnd, 
		m_pDeviceEBus->GetGenParameters(), 
		_T( "Device Control" ) );

}


void CControlDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnOK();
}

void CControlDlg::ShowGenWindow( PvGenBrowserWnd **aWnd, PvGenParameterArray *aParams, const CString &aTitle )
{
	if ( ( *aWnd ) != NULL )
	{
		if ( ( *aWnd )->GetHandle() != 0 )
		{
			CWnd lWnd;
			lWnd.Attach( ( *aWnd )->GetHandle() );

			// Window already visible, give it focus and bring it on top
			lWnd.BringWindowToTop();
			lWnd.SetFocus();

			lWnd.Detach();
			return;
		}

		// Window object exists but was closed/destroyed. Free it before re-creating
		CloseGenWindow( aWnd );
	}

	// Create, assigne parameters, set title and show modeless
	( *aWnd ) = new PvGenBrowserWnd;
	( *aWnd )->SetTitle( PvString( aTitle ) );
	( *aWnd )->SetGenParameterArray( aParams );
	( *aWnd )->ShowModeless( GetSafeHwnd() );
}

void CControlDlg::CloseGenWindow( PvGenBrowserWnd **aWnd )
{
	// If the window object does not even exist, do nothing
	if ( ( *aWnd ) == NULL )
	{
		return;
	}

	// If the window object exists and is currently created (visible), close/destroy it
	if ( ( *aWnd )->GetHandle() != 0 )
	{
		( *aWnd )->Close();
	}

	// Finally, release the window object
	delete ( *aWnd );
	( *aWnd ) = NULL;
}