// StInputSerialDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StInputSerialDlg.h"
#include "afxdialogex.h"
#include "ResourceString.h"
#include "StCommon.h"
//��1.0.0.1031
#include <fstream> 
#include <iostream> 
#include <string> 
//��1.0.0.1031

// StInputSerialDlg �_�C�A���O

IMPLEMENT_DYNAMIC(StInputSerialDlg, CDialogEx)

StInputSerialDlg::StInputSerialDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(StInputSerialDlg::IDD, pParent)
{
	//��1.0.0.1031
	//m_iLimitText = 14;
	Initialize();
	//��1.0.0.1031
}

StInputSerialDlg::StInputSerialDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx, CWnd* pParent /*=NULL*/)
	: CDialogEx(StInputSerialDlg::IDD, pParent)
	, StCheckBase( szClassName, szVarName, aDeviceBase, aDialogEx )
	//: StCheckBase( szVarName, szClassName, aDeviceBase )
	//, CDialogEx(StInputSerialDlg::IDD, pParent)
{
	//��1.0.0.1031
	//m_iLimitText = 14;
	Initialize();
	//��1.0.0.1031
}

StInputSerialDlg::~StInputSerialDlg()
{
}

void StInputSerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(StInputSerialDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &StInputSerialDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &StInputSerialDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_SERIALNUMBER, &StInputSerialDlg::OnEnChangeEditSerialnumber)
	ON_CBN_EDITCHANGE(IDC_COMBO_WORKERNAME, &StInputSerialDlg::OnCbnEditchangeComboWorkername)
END_MESSAGE_MAP()


// StInputSerialDlg ���b�Z�[�W �n���h���[


BOOL StInputSerialDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������
	
	CEdit objEdit;
	objEdit.Attach(GetDlgItem(IDC_EDIT_SERIALNUMBER)->m_hWnd);
	objEdit.LimitText(m_iLimitText);
	objEdit.Detach();

	//CComboBox objComboBox;
	//objComboBox.Attach(GetDlgItem(IDC_COMBO_WORKERNAME)->m_hWnd);

	//objComboBox.Detach();

	CResourceString::SetResourceText(this, IDS_INPUTSERIAL);
	CResourceString::SetResourceText(this, IDC_STATIC_SERIALNUMBER, IDS_STATIC_SERIAL);
	CResourceString::SetResourceText(this, IDC_STATIC_WORKERNAME, IDS_STATIC_WORKERNAME);

	CStCommon::GetIniFileName(m_szIniFileName, _countof(m_szIniFileName));

	//Ini�t�@�C���ɕۑ�
	CStCommon::GetIniFileName(m_szWorkerFileName, _countof(m_szWorkerFileName));
	//CStCommon::GetWorkerFileName(m_szWorkerFileName, _countof(m_szWorkerFileName));

	m_szSerialNumber = _T("");
	m_szWorkerName = _T("");
	m_szHinmokuCode = _T("");

	SetWorker();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

//��1.0.0.1031
void StInputSerialDlg::Initialize(void)
{
	m_iLimitText = 14;
	m_nCheckMode = 0;       //0:Check���Ȃ� 1:����ꍇNG(�����p) 2:�Ȃ��ꍇNG(���p)
	m_szSerialNumberFileName = _T("SerialNumber.txt");
	m_bCheck = TRUE;
}
//��1.0.0.1031

void StInputSerialDlg::SetWorker(void)
{
	LPCTSTR	szSection = TEXT("WorkerInformation");
	LPCTSTR szKey_Count = TEXT("Count");
	
	int	iListIndex = 0;
	INT iWorkerCount = GetPrivateProfileInt(szSection, szKey_Count, 0, m_szWorkerFileName);
	
	CComboBox objComboBox;
	objComboBox.Attach(GetDlgItem(IDC_COMBO_WORKERNAME)->m_hWnd);
	//m_iDefaultWorkerCount = objComboBox.GetCount();
	
	TCHAR	szKey[MAX_PATH];
	TCHAR	szTmp[MAX_PATH];
	INT i;

	//��Ǝ҂̒ǉ�
	for( i=0; i < iWorkerCount; i++ )
	{
		_stprintf_s( szKey, _countof(szKey), TEXT("WorkerName_%03u"), i);
		GetPrivateProfileString(szSection, szKey, _T("NonRegister"), szTmp, _countof(szTmp), m_szWorkerFileName);
		objComboBox.AddString(szTmp);
	}
	TCHAR	szWorkerName[MAX_PATH];
	_stprintf_s( szKey, _countof(szKey), TEXT("LastWorkerName") );
	GetPrivateProfileString( szSection, szKey, _T(""), szWorkerName, _countof(szWorkerName), m_szWorkerFileName );


	//���݂̍�Ǝ҂̃��X�g�ԍ����擾
	iWorkerCount = objComboBox.GetCount();
	for(i=0; i < iWorkerCount;i++)
	{
		objComboBox.GetLBText(i, szTmp);
		if(0 == _tcscmp(szTmp, szWorkerName))
		{
			iListIndex = i;
		}
	}
	objComboBox.SetCurSel(iListIndex);
	objComboBox.Detach();
}

void StInputSerialDlg::GetWorker(void)
{
	LPCTSTR	szSection = TEXT("WorkerInformation");
	LPCTSTR szKey_Count = TEXT("Count");
	TCHAR	szKey[MAX_PATH];
	CComboBox objComboBox;
	INT i;
	TCHAR	szTmp[MAX_PATH];
	
	CString	szWorkerName;
	objComboBox.Attach( GetDlgItem(IDC_COMBO_WORKERNAME)->m_hWnd );
	objComboBox.GetWindowText( szWorkerName );
	szWorkerName.Replace(_T("="),_T(""));
	szWorkerName.Replace(_T("��"),_T(""));
	//RemoveCharFromString( szWorkerName, TEXT("=��") );
	szWorkerName.Trim();

	INT iWorkerCount = objComboBox.GetCount();

	//�V�K�̍�Ǝ҂��ǂ������`�F�b�N
	BOOL bNewWorker = TRUE;
	for( i=0; i<iWorkerCount; i++ )
	{
		objComboBox.GetLBText(i, szTmp);
		if(0 == _tcscmp( szWorkerName, szTmp ) )
		{
			bNewWorker = FALSE;
			break;
		}
	}
	objComboBox.Detach();

	//��Ǝ҃��X�g�̏o��(INI�t�@�C���ւ̕ۑ�)
	if(bNewWorker)
	{

		iWorkerCount++;
		_stprintf_s( szTmp, _countof(szTmp), TEXT("%u"), iWorkerCount );
		WritePrivateProfileString(szSection, szKey_Count, szTmp, m_szWorkerFileName);

		_stprintf_s( szKey, _countof(szKey), TEXT("WorkerName_%03u"), iWorkerCount - 1);
		WritePrivateProfileString( szSection, szKey, szWorkerName, m_szWorkerFileName );
	}

	_stprintf_s( szKey, _countof(szKey), TEXT("LastWorkerName") );
	WritePrivateProfileString( szSection, szKey, szWorkerName, m_szWorkerFileName );

}


//-------------------------------------------------------------------------------
//�������s
//-------------------------------------------------------------------------------
BOOL StInputSerialDlg::Execute(void)
{
	//���g�p
	BOOL bReval = FALSE;
	//this->DoModal();
	return bReval;
}


void StInputSerialDlg::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B

	CDialogEx::OnCancel();
}


void StInputSerialDlg::OnBnClickedOk()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	GetWorker();

	CComboBox objComboBox;
	objComboBox.Attach( GetDlgItem(IDC_COMBO_WORKERNAME)->m_hWnd );
	objComboBox.GetWindowText( m_szWorkerName );
	m_szWorkerName.Replace(_T("="),_T(""));
	m_szWorkerName.Replace(_T("��"),_T(""));
	objComboBox.Detach();

	CString szSerialNumber;
	CEdit objEdit;
	objEdit.Attach( GetDlgItem(IDC_EDIT_SERIALNUMBER)->m_hWnd );
	objEdit.GetWindowText( szSerialNumber );
	objEdit.Detach();

	int pos = szSerialNumber.Find(_T("_"),0);
	if( pos<0 )
	{
		m_szSerialNumber = szSerialNumber;
	}
	else
	{
		m_szSerialNumber = szSerialNumber.Left(pos);
		//�i�ڃR�[�h
		m_szHinmokuCode = szSerialNumber.Mid(pos+1);
		int pos = m_szHinmokuCode.Find(_T("_"),0);
		if( pos>=0 )
			m_szHinmokuCode = m_szHinmokuCode.Left(pos);
	}

	//LOG�ɏo��


	//��1.0.0.1031
	m_bCheck = TRUE;
	if( m_nCheckMode>0 )
	{
		BOOL bExist = FALSE;
		//FileOpen
		TCHAR szFullFileName[MAX_PATH];
		CStCommon::GetExeFilePath(szFullFileName, _countof(szFullFileName));
		_tcscat_s(szFullFileName, _countof(szFullFileName), _T("\\CONFIG\\Standard\\"));
		_tcscat_s(szFullFileName, _countof(szFullFileName), m_szSerialNumberFileName);

		std::ifstream ifs(szFullFileName);

		if( !ifs.fail() )
		{

			std::string str;
			while(getline(ifs, str))
			{
				if( m_szSerialNumber==str.c_str() )
				{
					bExist = TRUE;
					break;
				}
			}
		}

		if( m_nCheckMode==1 )
		{
			if( bExist ) m_bCheck = FALSE;
		}
		else
		if( m_nCheckMode==2 )
		{
			if( !bExist ) m_bCheck = FALSE;
		}
	}
	//��1.0.0.1031

	CDialogEx::OnOK();
}


BOOL StInputSerialDlg::EnableObject(void)
{
	BOOL bEnabled = FALSE;
	CString szSerialNumber;
	CString szWorkerName;

	CEdit aEdit;
	aEdit.Attach(GetDlgItem(IDC_EDIT_SERIALNUMBER)->m_hWnd);
	aEdit.GetWindowText(szSerialNumber);
	aEdit.Detach();

	CComboBox aComboBox;

	aComboBox.Attach(GetDlgItem(IDC_COMBO_WORKERNAME)->m_hWnd);
	//if( aComboBox.GetCurSel()>=0 )
	//{
	//	aComboBox.GetLBText(aComboBox.GetCurSel(),WorkerName);
	//}
	//else
	//{
		aComboBox.GetWindowText(szWorkerName);
	//}
	aComboBox.Detach();
	szWorkerName.Trim();

	if( szSerialNumber.GetLength()>0 && szWorkerName.GetLength()>0 )
	{
		bEnabled= TRUE;
	}
	GetDlgItem(IDOK)->EnableWindow(bEnabled);
	return bEnabled;
}


void StInputSerialDlg::OnEnChangeEditSerialnumber()
{
	// TODO:  ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A���̃R���g���[����
	// ���̒ʒm�𑗐M����ɂ́A__super::OnInitDialog() �֐����I�[�o�[���C�h���A
	// CRichEditCtrl().SetEventMask() ��
	// OR ��Ԃ� ENM_CHANGE �t���O���}�X�N�ɓ���ČĂяo���K�v������܂��B

	// TODO:  �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����Ă��������B
	EnableObject();

}


void StInputSerialDlg::OnCbnEditchangeComboWorkername()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	EnableObject();
}
//��1.0.0.1025
BOOL StInputSerialDlg::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, CString &szValue)
{
	BOOL bReval = FALSE;

	CString szFunction(szFunc);
	if( szFunction=="GetSerialNumber" )
	{
		szValue = m_szSerialNumber;
		bReval = TRUE;
	}
	else
	if( szFunction=="GetWorkerName" )
	{
		szValue = m_szWorkerName;
		bReval = TRUE;
	}
	else
	if( szFunction=="GetHinmokuCode" )
	{
		szValue = m_szHinmokuCode;
		bReval = TRUE;
	}
	return bReval;
}
//��1.0.0.1025
//��1.0.0.1031
BOOL StInputSerialDlg::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;

	CString szFunction(szFunc);
	if( szFunction == _T("SetCheckMode") )
	{
		m_nCheckMode = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetSerialNumberFile" )
	{
		m_szSerialNumberFileName = szArgument;
		bReval = TRUE;
	}
	return bReval;
}
//��1.0.0.1031
