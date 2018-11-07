// StCheckAdjustGainDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StCheckAdjustGainDlg.h"
#include "afxdialogex.h"


// StCheckAdjustGainDlg �_�C�A���O

IMPLEMENT_DYNAMIC(StCheckAdjustGainDlg, CDialogEx)

StCheckAdjustGainDlg::StCheckAdjustGainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(StCheckAdjustGainDlg::IDD, pParent)
{

}

StCheckAdjustGainDlg::StCheckAdjustGainDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx, CWnd* pParent /*=NULL*/)
	: CDialogEx(StCheckAdjustGainDlg::IDD, pParent)
	, StCheckBase( szClassName, szVarName, aDeviceBase, aDialogEx )
{
	Initialize();
}

StCheckAdjustGainDlg::~StCheckAdjustGainDlg()
{
}

void StCheckAdjustGainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(StCheckAdjustGainDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// StCheckAdjustGainDlg ���b�Z�[�W �n���h���[
void StCheckAdjustGainDlg::Initialize(void)
{
	m_nChannelCount = 4;
	m_nStartChannel1 = 3691;
	m_nStartChannel3 = 3712;
	m_nWidth = 20;
	m_nTimer = 1000;	//�����Q�C�������̃Q�C���Z�b�g���Ă��烌�x���擾����܂ł̑҂�����(mSec)
	m_nBaseChannel = 1;
	m_nRange = 2;
	m_nDiff = 10;

}


BOOL StCheckAdjustGainDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������
	BOOL bReval;
	//GainPos��ǂ�
	m_nDefGainPos = 0;

	if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//MultiCam�p
	{
		//gainpos




		//bReval = WriteData( m_nDeviceCode, m_nPage, m_nAddress, setValue );
		//if( !bReval )
		//	break;
	}
	else
	{
		bReval = FALSE;
	}


	//Edit�ɕ\��



	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void StCheckAdjustGainDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	__super::OnTimer(nIDEvent);
}

BOOL StCheckAdjustGainDlg::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);

	if( szFunction == _T("Execute") )
	{
	}
	else
	if( szFunction=="SetChannelCount" || szFunction=="ChannelCount" )
	{
		m_nChannelCount = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetStartChannel1" || szFunction=="StartChannel1" )
	{
		m_nStartChannel1 = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetStartChannel3" || szFunction=="StartChannel3" )
	{
		m_nStartChannel3 = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetWidth" || szFunction=="Width" )
	{
		m_nWidth = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTimer" || szFunction=="Timer" )
	{
		m_nTimer = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetBaseChannel" || szFunction=="BaseChannel" )
	{
		m_nBaseChannel = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetRange" || szFunction=="Range" )
	{
		m_nRange = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetDiff" || szFunction=="Diff" )
	{
		m_nDiff = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}


	return bReval;
}
BOOL StCheckAdjustGainDlg::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	return bReval;
}

BOOL StCheckAdjustGainDlg::Execute(void)
{
	BOOL bReval = FALSE;
	return bReval;

}
BOOL StCheckAdjustGainDlg::AdjustExecute(void)
{
	BOOL bReval = FALSE;
	return bReval;

}
