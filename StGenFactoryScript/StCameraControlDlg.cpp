// StCameraControlDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StCameraControlDlg.h"
#include "afxdialogex.h"
#include "StDeviceEBus.h"
#include "StDeviceGenICam.h"


// StCameraControlDlg ダイアログ

IMPLEMENT_DYNAMIC(StCameraControlDlg, CDialogEx)

StCameraControlDlg::StCameraControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(StCameraControlDlg::IDD, pParent)
{
	Initialize();
}

StCameraControlDlg::StCameraControlDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx, CWnd* pParent /*=NULL*/)
	: CDialogEx(StCameraControlDlg::IDD, pParent)
	, StCheckBase( szClassName, szVarName, aDeviceBase, aDialogEx )
{
	Initialize();
}


StCameraControlDlg::~StCameraControlDlg()
{
}

void StCameraControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(StCameraControlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_READ, &StCameraControlDlg::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_READSTRING, &StCameraControlDlg::OnBnClickedButtonReadstring)
	ON_BN_CLICKED(IDC_BUTTON_WRITE2, &StCameraControlDlg::OnBnClickedButtonWrite2)
	ON_BN_CLICKED(IDC_BUTTON_WRITESTRING, &StCameraControlDlg::OnBnClickedButtonWritestring)
	ON_EN_KILLFOCUS(IDC_EDIT_WRITEVALUE, &StCameraControlDlg::OnEnKillfocusEditWritevalue)
	ON_EN_KILLFOCUS(IDC_EDIT_WRITEVALUEDEC, &StCameraControlDlg::OnEnKillfocusEditWritevaluedec)
END_MESSAGE_MAP()


// StCameraControlDlg メッセージ ハンドラー


BOOL StCameraControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	//m_nEndian = m_nEndianOriginal;
	//if( m_nEndian<0 || m_nEndian>=2 )
	//{
	//	if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL用
	//	{
	//	}
	//	else
	//	if( m_DeviceBase->m_ClassName=="StDeviceEBus"			//EBus用
	//		|| m_DeviceBase->m_ClassName=="StDeviceGenICam"		//GenICam用
	//		)
	//	{
	//		bool bReval=false;
	//		INT nEndian = -1;
	//		TCHAR szText[128];
	//		if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )
	//		{
	//			StDeviceGenICam *aDevice = (StDeviceGenICam *)m_DeviceBase;
	//			bReval = aDevice->GetGenICamControl(_T("DeviceRegistersEndianness"),szText,_countof(szText));
	//		}
	//		else
	//		{
	//			StDeviceEBus *aDevice = (StDeviceEBus *)m_DeviceBase;
	//			bReval = aDevice->GetGenICamControl(_T("DeviceRegistersEndianness"),szText,_countof(szText));
	//		}
	//		if( bReval )
	//		{
	//			if( _tcscmp(szText,_T("Little"))==0 )
	//				nEndian = 0;
	//			else
	//			if( _tcscmp(szText,_T("Big"))==0 )
	//				nEndian = 1;
	//		}
	//		if( nEndian<0 || nEndian>=2 )
	//		{
	//			if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )
	//			{
	//				StDeviceGenICam *aDevice = (StDeviceGenICam *)m_DeviceBase;
	//				bReval = aDevice->GetGenICamControl(_T("DeviceTLType"),szText,_countof(szText));
	//			}
	//			else
	//			{
	//				StDeviceEBus *aDevice = (StDeviceEBus *)m_DeviceBase;
	//				bReval = aDevice->GetGenICamControl(_T("DeviceRegistersEndianness"),szText,_countof(szText));
	//			}
	//			if( _tcscmp(szText,_T("USB3Vision"))==0 )
	//				nEndian = 0;
	//			else
	//			if( _tcscmp(szText,_T("CoaXPress"))==0 )
	//				nEndian = 1;
	//			else
	//			if( _tcscmp(szText,_T("GigEVision"))==0 )
	//				nEndian = 1;
	//		}

	//		m_nEndian = nEndian;
	//	}
	//}

	TCHAR szEndian[32];
	INT nEndian = -1;
	if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )
	{
		StDeviceGenICam *aDevice = (StDeviceGenICam *)m_DeviceBase;
		nEndian = aDevice->GetEndian();
	}
	if( nEndian==1 )
	{
		_stprintf_s(szEndian,_countof(szEndian),_T("Big Endian"));
	}
	else
	if( nEndian==0 )
	{
		_stprintf_s(szEndian,_countof(szEndian),_T("Little Endian"));
	}
	else
	{
		_stprintf_s(szEndian,_countof(szEndian),_T("Not supported"));
	}
	GetDlgItem(IDC_EDIT_ENDIAN)->SetWindowText(szEndian);	

	GetDlgItem(IDC_EDIT_READ)->SetWindowText(_T("00000000"));	
	GetDlgItem(IDC_EDIT_WRITE)->SetWindowText(_T("00000000"));	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void StCameraControlDlg::Initialize(void)
{
	m_nWindowPosX=-1;
	m_nWindowPosY=-1;
}

BOOL StCameraControlDlg::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;

	CString szFunction(szFunc);
	//if( szFunction == _T("SetEndian") || szFunction == _T("Endian") )
	//{
	//	if( _tcscmp(szArgument,_T("Little"))==0 )
	//		m_nEndianOriginal = 0;
	//	else
	//	if( _tcscmp(szArgument,_T("Big"))==0 )
	//		m_nEndianOriginal = 1;
	//	else
	//	if( _tcscmp(szArgument,_T("Auto"))==0 )
	//		m_nEndianOriginal = -1;
	//	else
	//		m_nEndianOriginal = _ttoi(szArgument);
	//	bReval = TRUE;
	//}
	//else
	if( szFunction == _T("SetWindowPosX") || szFunction == _T("WindowPosX") )
	{
		m_nWindowPosX = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction == _T("SetWindowPosY") || szFunction == _T("WindowPosY") )
	{
		m_nWindowPosY = _ttoi(szArgument);
		bReval = TRUE;
	}
	return bReval;
}


void StCameraControlDlg::OnBnClickedButtonRead()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	BOOL bSupported = FALSE;
	CString errString("Not supported");
	CString hexString("NG");
	CString decString("NG");

	bool bReval = false;
	unsigned int uiAddress=0;
	unsigned int uiData=0;
	CString szText(_T(""));
	GetDlgItem(IDC_EDIT_READ)->GetWindowText(szText);	
	uiAddress = _tcstoul(szText, NULL, 16);
	if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//EBus用
	{
		StDeviceEBus *aDevice = (StDeviceEBus *)m_DeviceBase;
		bReval = aDevice->ReadMemory(uiAddress,(unsigned char *)&uiData,sizeof(uiData)).IsOK();
		bSupported = TRUE;
	}
	else
	if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL用
	{
	}
	else
	if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )	//GenICam用
	{
		StDeviceGenICam *aDevice = (StDeviceGenICam *)m_DeviceBase;
		bReval = aDevice->ReadMain(uiAddress,(char *)&uiData,sizeof(uiData));
		bSupported = TRUE;
	}
	if( bSupported )
	{
		if( bReval )
		{
			errString = "OK";
			hexString.Format(_T("0x%08X"),uiData);
			decString.Format(_T("%i"),uiData);
		}
		else
		{
			errString = "NG";
		}
	}

	GetDlgItem(IDC_EDIT_READVALUE)->SetWindowText(hexString);	
	GetDlgItem(IDC_EDIT_READVALUEDEC)->SetWindowText(decString);	
	GetDlgItem(IDC_EDIT_READRES)->SetWindowText(errString);	
}


void StCameraControlDlg::OnBnClickedButtonReadstring()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	BOOL bSupported = FALSE;
	CString errString("Not supported");
	CString textString("NG");

	bool bReval=FALSE;
	unsigned int uiAddress=0;
	unsigned int uiData=0;
	CString szText(_T(""));
	GetDlgItem(IDC_EDIT_READ)->GetWindowText(szText);	
	uiAddress = _tcstoul(szText, NULL, 16);
	GetDlgItem(IDC_EDIT_READSTRINGSIZE)->GetWindowText(szText);	
	UINT uiSize = _tcstoul(szText, NULL, 0);
	if( uiSize>0 && (uiSize&3)==0 )
	{
		PBYTE pbyteBuffer = new BYTE [uiSize+1];
		if( pbyteBuffer )
		{
			memset(pbyteBuffer,0,uiSize+1);
			UINT uiData=0;
			if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//EBus用
			{
				bSupported = TRUE;
				StDeviceEBus *aDevice = (StDeviceEBus *)m_DeviceBase;
				bReval = aDevice->ReadMemory(uiAddress,(unsigned char *)pbyteBuffer,uiSize).IsOK();
			}
			else
			if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL用
			{
			}
			else
			if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )	//GenICam用
			{
				bSupported = TRUE;
				StDeviceGenICam *aDevice = (StDeviceGenICam *)m_DeviceBase;
				bReval = aDevice->ReadMain(uiAddress,(char *)pbyteBuffer,uiSize,TRUE);
			}
				
			if( bSupported )
			{
				if( bReval )
				{
					errString = "OK";
					textString = pbyteBuffer;
				}
				else
				{
					errString = "NG";
				}
			}
			delete [] pbyteBuffer;
		}
	}

	GetDlgItem(IDC_EDIT_READSTRING)->SetWindowText(textString);	
	GetDlgItem(IDC_EDIT_READRES)->SetWindowText(errString);	
}


void StCameraControlDlg::OnBnClickedButtonWrite2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CString errString("Not supported");
	CString hexString("NG");
	CString decString("NG");

	CString szText("");
	GetDlgItem(IDC_EDIT_WRITE)->GetWindowText(szText);	
	unsigned int uiAddress = _tcstoul(szText, NULL, 16);
	GetDlgItem(IDC_EDIT_WRITEVALUE)->GetWindowText(szText);	
	unsigned int uiData = _tcstoul(szText, NULL, 16);

	BOOL bSupported = FALSE;
	BOOL bReval;
	if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//EBus用
	{
		bSupported = TRUE;
		StDeviceEBus *aDevice = (StDeviceEBus *)m_DeviceBase;
		bReval = aDevice->WriteMemory(uiAddress,(unsigned char *)&uiData,sizeof(uiData)).IsOK();
	}
	else
	if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL用
	{
	}
	else
	if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )	//GenICam用
	{
		bSupported = TRUE;
		StDeviceGenICam *aDevice = (StDeviceGenICam *)m_DeviceBase;
		bReval = aDevice->WriteMain(uiAddress,(char *)&uiData,sizeof(uiData));
	}

	if( bSupported )
	{
		if( bReval )
		{
			errString = "OK";
		}
		else
		{
			errString = "NG";
		}
	}

	GetDlgItem(IDC_EDIT_WRITERES)->SetWindowText(errString);	
}


void StCameraControlDlg::OnBnClickedButtonWritestring()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CString errString("Not supported");
	CString hexString("NG");
	CString decString("NG");

	do
	{
		CString szText("");
		GetDlgItem(IDC_EDIT_WRITE)->GetWindowText(szText);	
		unsigned int uiAddress = _tcstoul(szText, NULL, 16);

		GetDlgItem(IDC_EDIT_WRITESTRINGSIZE)->GetWindowText(szText);	
		unsigned int uiData = _tcstoul(szText, NULL, 10);

		GetDlgItem(IDC_EDIT_WRITESTRINGVALUE)->GetWindowText(szText);	
		if( szText.GetLength()>uiData-1 )
		{
			errString = "NG(length)";
			break;
		}

		char * pszString = new char [uiData]; 
		if( !pszString )
		{
			errString = "NG(memory)";
			break;
		}
		memset(pszString,0,uiData);
		StString aText = szText;
		strcpy_s(pszString,uiData,aText.GetAscii());

		BOOL bSupported = FALSE;
		BOOL bReval;

		if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//EBus用
		{
			bSupported = TRUE;
			StDeviceEBus *aDevice = (StDeviceEBus *)m_DeviceBase;
			bReval = aDevice->WriteMemory(uiAddress,(unsigned char *)pszString,uiData).IsOK();
		}
		else
		if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL用
		{
		}
		else
		if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )	//GenICam用
		{
			bSupported = TRUE;
			StDeviceGenICam *aDevice = (StDeviceGenICam *)m_DeviceBase;
			bReval = aDevice->WriteMain(uiAddress,pszString,uiData,TRUE);
		}
		delete [] pszString;

		if( bSupported )
		{
			if( bReval )
			{
				errString = "OK";
			}
			else
			{
				errString = "NG";
			}
		}


	}while(0);

	GetDlgItem(IDC_EDIT_WRITERES)->SetWindowText(errString);	

}


void StCameraControlDlg::OnEnKillfocusEditWritevalue()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CString szText("");
	GetDlgItem(IDC_EDIT_WRITEVALUE)->GetWindowText(szText);	
	if( szText.GetLength() )
	{
		unsigned int uiValue = _tcstoul(szText, NULL, 16);
		szText.Format(_T("%i"),uiValue);
	}
	GetDlgItem(IDC_EDIT_WRITEVALUEDEC)->SetWindowText(szText);	
}


void StCameraControlDlg::OnEnKillfocusEditWritevaluedec()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CString szText("");
	GetDlgItem(IDC_EDIT_WRITEVALUEDEC)->GetWindowText(szText);	
	if( szText.GetLength() )
	{
		unsigned int uiValue = _tcstoul(szText, NULL, 10);
		szText.Format(_T("%08X"),uiValue);
	}
	GetDlgItem(IDC_EDIT_WRITEVALUE)->SetWindowText(szText);	
}
