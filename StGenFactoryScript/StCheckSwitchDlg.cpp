// StCheckSwitchDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StCheckSwitchDlg.h"
#include "afxdialogex.h"

#include "StDeviceMultiCam.h"
#include "StCheckControlRetry.h"
//#include "StCommon.h"

// StCheckSwitchDlg �_�C�A���O

IMPLEMENT_DYNAMIC(StCheckSwitchDlg, CDialogEx)

StCheckSwitchDlg::StCheckSwitchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(StCheckSwitchDlg::IDD, pParent)
{
	Initialize();
}

StCheckSwitchDlg::StCheckSwitchDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx, CWnd* pParent /*=NULL*/)
	: CDialogEx(StCheckSwitchDlg::IDD, pParent)
	, StCheckBase( szClassName, szVarName, aDeviceBase, aDialogEx )
{
	Initialize();
}


StCheckSwitchDlg::~StCheckSwitchDlg()
{
}

void StCheckSwitchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(StCheckSwitchDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// StCheckSwitchDlg ���b�Z�[�W �n���h���[
#define _TIMER_SWITCH 1

INT nSWITCH_ID[] = { IDC_CHECK1, IDC_CHECK2, IDC_CHECK3, IDC_CHECK4, IDC_CHECK5, IDC_CHECK6, IDC_CHECK7, IDC_CHECK8 };
TCHAR szLastMessage[128];

typedef struct SwitchMessage
{
	PTCHAR pMessage;
	INT nSwitch;
};
SwitchMessage swMessage[] = 
{
	_T("�X�C�b�`�����ׂ�OFF���Ă�������!"), 0, 
	_T("1��ON���ĉ�����!"), 1, 
	_T("1��OFF���ĉ�����!"), 0, 
	_T("2��ON���ĉ�����!"), 2, 
	_T("2��OFF���ĉ�����!"), 0, 
	_T("3��ON���ĉ�����!"), 4, 
	_T("3��OFF���ĉ�����!"), 0, 
	_T("4��ON���ĉ�����!"), 8, 
	_T("4��OFF���ĉ�����!"), 0, 
	_T("5��ON���ĉ�����!"), 16, 
	_T("5��OFF���ĉ�����!"), 0, 
	_T("6��ON���ĉ�����!"), 32, 
	_T("6��OFF���ĉ�����!"), 0, 
	_T("7��ON���ĉ�����!"), 64, 
	_T("7��OFF���ĉ�����!"), 0, 
	_T("8��ON���ĉ�����!"), 128, 
	_T("8��OFF���ĉ�����!"), 0, 
	szLastMessage, 0,
};

BOOL StCheckSwitchDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


int StCheckSwitchDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B
	//�ŏI�̃X�C�b�`�ʒu�����߂�B
	INT nCount = _countof(swMessage);
	swMessage[nCount-1].nSwitch = m_nLastSwitch;

	TCHAR szTmp[128];
	_tcscpy_s( szTmp, _countof(szTmp), _T("") );
	if( m_nLastSwitch>0 )
	{
		TCHAR tmp[128];
		for( int i=0; i<_countof(nSWITCH_ID); i++ )
		{
			if( ((m_nLastSwitch>>i)&1)!=0 )
			{
				_stprintf_s(tmp,_countof(tmp), _T("%i "),i+1);
				_tcscat_s(szTmp, _countof(szTmp), tmp);
			}
		}
		_tcscat_s(szTmp, _countof(szTmp), _T("��ON���Ă��������I"));
	}
	_tcscpy_s( szLastMessage, _countof(szLastMessage), szTmp );

	m_nIndex = 0;
	m_nOldIndex = -1;

	SetTimer( _TIMER_SWITCH, 100, NULL );

	return 0;
}


void StCheckSwitchDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	if( _TIMER_SWITCH == nIDEvent )
	{
		INT nSwitch;
		BOOL bReval = GetCameraSwitch(nSwitch);
		if( bReval )
		{
			//Display---
			CButton objButton;
			for( int i=0; i<_countof(nSWITCH_ID); i++ )
			{
				objButton.Attach(GetDlgItem(nSWITCH_ID[i])->m_hWnd);
				objButton.SetCheck(((nSwitch>>i)&1)==0?false:true);
				objButton.Detach();
			}

			//Beep----------
			if( swMessage[m_nIndex].nSwitch==nSwitch )
			{
				if( m_nDurationSwitch>0 )
				{
					Beep(m_nFreqSwitch, m_nDurationSwitch);
				}
				m_nIndex++;
			}
			//Check----------
			if( m_nIndex>=_countof(swMessage) )
			{
				KillTimer(_TIMER_SWITCH);
				OnOK();
				return;
			}
			//Message----------
			if( m_nIndex != m_nOldIndex )
			{
				((CStatic *)GetDlgItem(IDC_STATIC_TEXT))->SetWindowText(swMessage[m_nIndex].pMessage);
				m_nOldIndex = m_nIndex;
			}
		}
	}
	__super::OnTimer(nIDEvent);
}

void StCheckSwitchDlg::Initialize(void)
{
	m_nLastSwitch = 0;
	m_nDurationSwitch = 0;
	m_nFreqSwitch = 0;
}

BOOL StCheckSwitchDlg::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;

	CString szFunction(szFunc);
	if( szFunction == _T("SetLastPos") || szFunction == _T("LastPos") )
	{
		m_nLastSwitch = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction == _T("SetDuration") || szFunction == _T("Duration") )
	{
		m_nDurationSwitch = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction == _T("SetFreq") || szFunction == _T("Freq") )
	{
		m_nFreqSwitch = _ttoi(szArgument);
		bReval = TRUE;
	}

	return bReval;

}

BOOL StCheckSwitchDlg::GetCameraSwitch(INT &nSwitch)
{
	BOOL bReval = FALSE;
	

	do
	{
		if( !m_DeviceBase ) break;
		if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//EBus�p
		{
		}
		else
		if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL�p
		{
			StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
			if( !aDevice ) break;
			StCheckControlRetry *aControlRetry = NULL;
			if( aDevice->IsOpened() )
			{
				aControlRetry = (StCheckControlRetry *)aDevice->GetControlRetry();
			}
			if( !aControlRetry ) break;

			aControlRetry->SetSendData(_T("dipsw"));
			bReval = aControlRetry->Execute();
			if( !bReval ) break;
			CString szRcvData;
			aControlRetry->GetLastRecieveData(szRcvData);	//�߂�l
			nSwitch = _ttoi(szRcvData);
		}
		else
		if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )	//GenICam�p
		{
		}
	}while(0);


	return bReval;

}

