// StCheckAdjustLinetolineDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StCheckAdjustLinetolineDlg.h"
#include "afxdialogex.h"
#include "StDeviceEBus.h"

unsigned __stdcall AdjustLinetolineThread( void *pParam )
{
	StCheckAdjustLinetolineDlg * aDlg = (StCheckAdjustLinetolineDlg *)pParam;
	do
	{
		if( aDlg->GetThreadStop() )
			break;

		BOOL bReval = aDlg->AdjustExecute();

		if( !bReval )
			break;	//NG�ł͂Ȃ��G���[

		if( aDlg->GetResultCount()>=aDlg->GetStandardCount() )
			break;	//�J�E���g�I��
		if( aDlg->GetJudge()==1 )	//����I��
			break;

		Sleep(200);	//�����œ�������OK�{�^���������Ȃ��Ȃ�B

	}while(1);

	_endthreadex(0);
	return 0;
}

// StCheckAdjustLinetolineDlg �_�C�A���O

IMPLEMENT_DYNAMIC(StCheckAdjustLinetolineDlg, CDialogEx)

StCheckAdjustLinetolineDlg::StCheckAdjustLinetolineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(StCheckAdjustLinetolineDlg::IDD, pParent)
{
	Initialize();
}

StCheckAdjustLinetolineDlg::StCheckAdjustLinetolineDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx, CWnd* pParent /*=NULL*/)
	: CDialogEx(StCheckAdjustLinetolineDlg::IDD, pParent)
	, StCheckBase( szClassName, szVarName, aDeviceBase, aDialogEx )
{
	Initialize();
}

#define ST_DISPLAYTEXT 1
StCheckAdjustLinetolineDlg::~StCheckAdjustLinetolineDlg()
{
	CloseHandle(hDisplayMutex);
	KillTimer(ST_DISPLAYTEXT);
}

void StCheckAdjustLinetolineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(StCheckAdjustLinetolineDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// StCheckAdjustLinetolineDlg ���b�Z�[�W �n���h���[

BOOL StCheckAdjustLinetolineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������

	//����Thread�쐬-------------
	m_SetValue = m_nInitValue;
	m_ResultCount = 0;
	m_hThread = (HANDLE)_beginthreadex( NULL, 0, AdjustLinetolineThread, this,  0, &m_mThreadID );


	//�\���pSetTimer
	hDisplayMutex = CreateMutex(NULL,FALSE,NULL);
	SetTimer( ST_DISPLAYTEXT, 200, NULL );		//unsigned int iID;

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void StCheckAdjustLinetolineDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	if( nIDEvent==ST_DISPLAYTEXT )
	{
		DisplayBufferListBox();
	}

	CDialogEx::OnTimer(nIDEvent);
}

void StCheckAdjustLinetolineDlg::Initialize(void)
{
	m_nStart = 0;
	m_nEnd = 4095;

	m_nTimer = 1000;
	m_nDeviceCode = 0;
	m_nPage = 0;
	m_nAddress = 0;
	m_nInitValue = 0;
	m_dblMinAverage = 190;
	m_dblMaxAverage = 210;

	m_nAdjustCount = 10;

	m_bThreadStop = FALSE;

}

BOOL StCheckAdjustLinetolineDlg::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction == _T("Execute") )
	{
	}
	else
	if( szFunction=="SetStartLine" )
	{
		m_nStart = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetEndLine" )
	{
		m_nEnd = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTimer" )
	{
		m_nTimer = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetDeviceCode" )
	{
		m_nDeviceCode = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetPage" )
	{
		m_nPage = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAddress" )
	{
		m_nAddress = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetInitValue" )
	{
		m_nInitValue = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMinAverage" )
	{
		m_dblMinAverage = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMaxAverage" )
	{
		m_dblMaxAverage = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAdjustCount" )
	{
		m_nAdjustCount = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}


	return bReval;
}

BOOL StCheckAdjustLinetolineDlg::WriteData(BYTE byteDeviceCode, BYTE bytePage, BYTE byteAddress, BYTE byteData)
{
	StDeviceEBus *aDevice = (StDeviceEBus *)m_DeviceBase;
	if( !aDevice->IsConnected() ) return FALSE;

	BYTE byteSendData[6];
	byteSendData[0] = 0x02;
	byteSendData[1] = (byteDeviceCode<<2) | 0x02 | (bytePage&1);
	byteSendData[2] = byteAddress;
	byteSendData[3] = 1;
	byteSendData[4] = byteData;
	byteSendData[5] = 0x03;
	BYTE byteRcvData[4] = {2,0,1,3};	//Write���̐��탊�[�h�l

	//��1.0.0.1060
	//BYTE GetRcvData[4+2];
	BYTE GetRcvData[4];
	//��1.0.0.1060
	size_t getRcvSize = sizeof(GetRcvData);
	BOOL bRevCtrl = aDevice->SerialControl( _T("UART0"), byteSendData, sizeof(byteSendData), GetRcvData, &getRcvSize );
	if( bRevCtrl )
	{
		if( getRcvSize!=sizeof(byteRcvData) )
		{
			bRevCtrl = FALSE;
		}
	}
	if( bRevCtrl )
	{

//TRACE(TEXT("@@@@  GetRcvData=%d:%d:%d:%d:\n"), GetRcvData[0], GetRcvData[1], GetRcvData[2], GetRcvData[3]);

		if( memcmp(GetRcvData,byteRcvData,getRcvSize)!=0 )
			bRevCtrl = FALSE;
	}

	return bRevCtrl;
}

void UpdateLogListBoxScrollBar(CListBox *pListBox);

BOOL StCheckAdjustLinetolineDlg::AdjustExecute(void)
{

	BOOL bReval = FALSE;
	m_iJudge = 2;	//2:������

	do
	{
		BYTE setValue = m_SetValue;
		if( m_ResultCount>0 )	//�Q��ڈȍ~
		{
			if( m_ResultMinAverage>m_dblMaxAverage )
			{
				if( m_ResultCount>1 )
				{
					setValue-=1;
				}
				else
				{
					setValue -= (BYTE)(m_ResultMinAverage - m_dblMaxAverage + 1.0);
				}
			}
			else
			if( m_ResultMinAverage<m_dblMinAverage )
			{
				if( m_ResultCount>1 )
				{
					setValue+=1;
				}
				else
				{
					setValue += (BYTE)( m_dblMinAverage - m_ResultMinAverage + 1.0);
				}
			}
		}
		//�l��ݒ肵�ASetTimer��ɉ摜�擾�]��
		if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//GigE�p
		{
			bReval = WriteData( m_nDeviceCode, m_nPage, m_nAddress, setValue );
			if( !bReval )
				break;
		}
		else
		{
			break;
		}
		Sleep(m_nTimer);


//TRACE(TEXT("@@@@@@@@@ StCheckAdjustLinetolineDlg m_DeviceBase=0x%x\n"), m_DeviceBase );

		if( m_Buffer ) delete m_Buffer;
		m_Buffer = new StBuffer;
		bReval = m_DeviceBase->GetStBuffer(m_Buffer);	//�ϊ����Ď擾�����肩���B�����_�ł̓R�s�[

		size_t nElementSize=1;
		double dblMinAverage=0.0;
		INT  nLine = -1;
		INT  nElement = -1;
		if( bReval )
			bReval = GetMinimumLineAverage( m_Buffer, m_nStart, m_nEnd, &nElementSize, &dblMinAverage, &nLine, &nElement );

		if( bReval )
		{

//TRACE(TEXT("@@@@  setValue=%d dblMinAverage(%f)\n"), setValue, dblMinAverage);

			//�ʃX���b�h�Ȃ̂ł����ŕ\�����ł��Ȃ��B�B�B�B

			CString szText;
			szText.Format(_T("mindata=%.02f value=%d"), dblMinAverage, setValue );
//TRACE(szText);
//TRACE(_T("\n"));

			DisplayListBox(szText);
			//CListBox aList;
			//aList.Attach(GetDlgItem(IDC_LIST_LINETOLINE)->m_hWnd);
			//aList.AddString(szText);
			//UpdateLogListBoxScrollBar(&aList);
			//aList.Detach();

			m_SetValue = setValue;
			m_ResultMinAverage = dblMinAverage;
			m_ResultLine = nLine;
			m_ResultElement = nElement;
			m_ResultCount++;

			if( dblMinAverage<m_dblMinAverage || m_dblMinAverage>m_dblMaxAverage )
				m_iJudge=0;
			if( m_iJudge==2 ) m_iJudge = 1;	//1:OK

			//���ʐ��l�\��
			//ResultText();
		}
		else
		{
			m_iJudge = 0;	//0:NG
		}
	}while(0);

	if( m_iJudge==2 ) m_iJudge = 0;	//0:NG

	//���ʕ\��
	//JudgeText(m_iJudge==1);

	return bReval;
}

BOOL StCheckAdjustLinetolineDlg::Execute(void)
{
	BOOL bReval = FALSE;
	if( m_hThread )
	{
		WaitForSingleObject( m_hThread, INFINITE );
		if( m_iJudge==1 || m_iJudge==0 ) //NG����TRUE�ŕԂ�
			bReval=TRUE;
	}
	return bReval;
}

void StCheckAdjustLinetolineDlg::DisplayBufferListBox(void)
{
	::WaitForSingleObject(hDisplayMutex,INFINITE);

	HWND hListBox = ::GetDlgItem(m_hWnd, IDC_LIST_LINETOLINE);
	CListBox *pListBox = (CListBox *)CWnd::FromHandle( hListBox );

	for( DWORD i=0; i<m_vDisplayString.size(); i++ )
	{
		int nIndex = pListBox->AddString(m_vDisplayString[i]);
		UpdateLogListBoxScrollBar(pListBox);
		pListBox->UpdateWindow();
		UpdateData(FALSE);
	}

	m_vDisplayString.clear();

	::ReleaseMutex(hDisplayMutex);
}


void StCheckAdjustLinetolineDlg::DisplayListBox(CString szMessge)
{

	::WaitForSingleObject(hDisplayMutex,INFINITE);

	m_vDisplayString.push_back(szMessge);

	::ReleaseMutex(hDisplayMutex);

}

//��1.0.0.1025
BOOL StCheckAdjustLinetolineDlg::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetCount" )
	{
		dblValue = GetResultCount();
		bReval = TRUE;
	}
	else
	if( szFunction=="GetElement" )
	{
		dblValue = m_ResultElement;
		bReval = TRUE;
	}
	else
	if( szFunction=="GetMinAverage" )
	{
		dblValue = m_ResultMinAverage;
		bReval = TRUE;
	}
	else
	if( szFunction=="GetLine" )
	{
		dblValue = m_ResultLine;
		bReval = TRUE;
	}
	return bReval;
}
//��1.0.0.1025
