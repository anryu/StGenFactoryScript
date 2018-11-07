// StCheckAdjustVolumeDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StCheckAdjustVolumeDlg.h"
#include "afxdialogex.h"
#include <math.h>

unsigned __stdcall AdjustVolumeThread( void *pParam )
{
	StCheckAdjustVolumeDlg * aDlg = (StCheckAdjustVolumeDlg *)pParam;
	do
	{
		//if( aDlg->GetThreadStop() )
		//	break;

		//BOOL bReval = aDlg->AdjustExecute();

		//if( !bReval )
		//	break;	//NG�ł͂Ȃ��G���[

		//if( aDlg->GetResultCount()>=aDlg->GetStandardCount() )
		//	break;	//�J�E���g�I��
		//if( aDlg->GetJudge()==1 )	//����I��
		//	break;

		Sleep(200);	//�����œ�������OK�{�^���������Ȃ��Ȃ�B

	}while(1);

	_endthreadex(0);
	return 0;
}

// StCheckAdjustVolumeDlg �_�C�A���O

IMPLEMENT_DYNAMIC(StCheckAdjustVolumeDlg, CDialogEx)

StCheckAdjustVolumeDlg::StCheckAdjustVolumeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(StCheckAdjustVolumeDlg::IDD, pParent)
{
	Initialize();
}

StCheckAdjustVolumeDlg::StCheckAdjustVolumeDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx, CWnd* pParent /*=NULL*/)
	: CDialogEx(StCheckAdjustVolumeDlg::IDD, pParent)
	, StCheckBase( szClassName, szVarName, aDeviceBase, aDialogEx )
{
	Initialize();
}

StCheckAdjustVolumeDlg::~StCheckAdjustVolumeDlg()
{
	if( m_pStart ) delete [] m_pStart;
}

void StCheckAdjustVolumeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(StCheckAdjustVolumeDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// StCheckAdjustVolumeDlg ���b�Z�[�W �n���h���[

#define ST_VOLUME_CHECK 1
BOOL StCheckAdjustVolumeDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������
	m_color = RGB(0,0,0);
	CStatic objStatic;
	objStatic.Attach(GetDlgItem(IDC_STATIC_VOLUMEJUDGE)->m_hWnd);
	m_pDefFont = objStatic.GetFont();
	objStatic.Detach();

	m_boldFont.CreateFont( 30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					  SHIFTJIS_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
					  DEFAULT_QUALITY, DEFAULT_PITCH, _T("�l�r �o�S�V�b�N") );


	//����Thread�쐬-------------
	//m_SetValue = m_nInitValue;
	//m_ResultCount = 0;
	//m_hThread = (HANDLE)_beginthreadex( NULL, 0, AdjustVolumeThread, this,  0, &m_mThreadID );

	SetTimer( ST_VOLUME_CHECK, 200, NULL );

	CString szText;
	szText.Format(_T("���x����<[%.2f]"),m_nLevelDiff);

	GetDlgItem(IDC_EDIT_VOLUMELIMIT)->SetWindowText( szText );

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


HBRUSH StCheckAdjustVolumeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ������ DC �̑�����ύX���Ă��������B
	if( pWnd->GetDlgCtrlID()==IDC_STATIC_VOLUMEJUDGE )
	{
		pDC->SetTextColor(m_color);
	}
	// TODO:  ����l���g�p�������Ȃ��ꍇ�͕ʂ̃u���V��Ԃ��܂��B
	return hbr;
}

void StCheckAdjustVolumeDlg::Initialize(void)
{
	m_nChannelCount = 0;
	m_nLevelDiff = 0;
	m_nWidth = 0;
	m_nCenter = 0;

	m_pStart = NULL;

	m_pResultAverage = NULL;
	m_nResultChannel = 0;
	m_nResultElement = 0;

}

BOOL StCheckAdjustVolumeDlg::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction == _T("Execute") )
	{
	}
	else
	if( szFunction=="SetChannelCount" || szFunction=="ChannelCount" )
	{
		INT nChannelCount = _tcstol(szArgument,NULL,0);
		if( m_nChannelCount!=nChannelCount )
		{
			if( m_pStart )  delete [] m_pStart;
			m_pStart = NULL;
			if( (nChannelCount/2)>0 )	m_pStart = new INT [nChannelCount/2];
			m_nChannelCount=nChannelCount;
		}
		bReval = TRUE;
	}
	else
	if( szFunction=="SetLevelDiff" || szFunction=="LevelDiff" )
	{
		m_nLevelDiff = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetCenter" || szFunction=="Center" )
	{
		m_nCenter = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetWidth" || szFunction=="Width" )
	{
		m_nWidth = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	{
		INT nIndex = -1;
		if( szFunction.Left(8)=="SetStart" )
		{
			nIndex = _tcstol(szFunction.Mid(8),NULL,0);
		}
		else
		if( szFunction.Left(5)=="Start" )
		{
			nIndex = _tcstol(szFunction.Mid(5),NULL,0);
		}
		if( nIndex>=0 && nIndex<(m_nChannelCount/2) )
		{
			m_pStart[nIndex] = _tcstol(szArgument,NULL,0);
			bReval = TRUE;
		}
	}
	return bReval;
}

BOOL StCheckAdjustVolumeDlg::Execute(void)
{
	BOOL bReval = FALSE;
	//if( m_hThread )
	{
		//WaitForSingleObject( m_hThread, INFINITE );
		//if( m_iJudge==1 || m_iJudge==0 ) //NG����TRUE�ŕԂ�
		//	bReval=TRUE;
	}
	return bReval;
}

//-----------------------------------------------------------
//
//
//
//-----------------------------------------------------------
BOOL StCheckAdjustVolumeDlg::AdjustExecute( void )
{
	BOOL bReval = FALSE;
	if( (m_nChannelCount/2)==0 ) return FALSE;
	if( m_pStart==NULL ) return FALSE;
	PINT pnStart = new INT [m_nChannelCount/2];
	if( pnStart==NULL ) return FALSE;

	m_iJudge = 2;	//2:������

	double *pAverage = NULL;
	do
	{
		memcpy( pnStart, m_pStart, sizeof(INT)*m_nChannelCount/2 );

		if( pnStart[0]<0 )
		{
			if( m_nChannelCount==2 )
			{
				pnStart[0] = m_nCenter - m_nWidth / 2;
			}
			else
			if( m_nChannelCount==4 )
			{
				pnStart[0] = m_nCenter - m_nWidth;
				pnStart[1] = m_nCenter;
			}
			else
			{
				return FALSE;
			}
		}
		for( int i=0; i<m_nChannelCount/2; i++ )
		{
			if( pnStart[i]<0 ) break;
		}

		if( m_Buffer ) delete m_Buffer;
		m_Buffer = new StBuffer;
		bReval = m_DeviceBase->GetStBuffer(m_Buffer);	//�ϊ����Ď擾�����肩���B�����_�ł̓R�s�[

		INT nElement = GetElementFromBuffer(m_Buffer);
		pAverage = new double [m_nChannelCount * nElement];
		if( pAverage == NULL )
		{
			bReval = FALSE;
			break;
		}
		if( bReval )
			bReval = GetChannelAverage( m_Buffer, pnStart, m_nWidth, m_nChannelCount, pAverage );
		if( bReval )
		{
			if( m_pResultAverage ) delete [] m_pResultAverage;
			m_pResultAverage = new double [nElement * m_nChannelCount]; 
			m_nResultChannel = m_nChannelCount;
			m_nResultElement = nElement;
			memcpy( m_pResultAverage, pAverage, sizeof(double) * nElement * m_nChannelCount);
		}
	}while(0);
	delete [] pnStart;
	if( pAverage ) delete [] pAverage;

	return bReval;
}

BOOL StCheckAdjustVolumeDlg::JudgeText( bool bJudge )
{
	CString text;
	if( bJudge )
	{
		m_color = RGB( 0,255,0 );
		text.Format(_T("OK"));
	}
	else
	{
		m_color = RGB( 255,0,0 );
		text.Format(_T("NG"));
	}
	CStatic objStatic;
	objStatic.Attach(GetDlgItem(IDC_STATIC_VOLUMEJUDGE)->m_hWnd);
	objStatic.SetWindowText(text);
	objStatic.SetFont( &m_boldFont, TRUE );
	objStatic.Detach();
	return TRUE;
}



void StCheckAdjustVolumeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	if( nIDEvent==ST_VOLUME_CHECK )
	{
		BOOL bReval = AdjustExecute();
		if( bReval )
		{
			//�\��Display
			AdjustJudgeDisplay();
		}
		else
		{
			m_iJudge = 0;	//0:NG
		}
	}
	__super::OnTimer(nIDEvent);
}

BOOL StCheckAdjustVolumeDlg::AdjustJudgeDisplay( void )
{
	CString szText,szTmp;
	szText.Empty();
	CString maxText;
	maxText.Empty();
	if( m_nResultElement==1 )	//Mono
	{
		for( int i=0; i<m_nResultChannel; i+=2 )
		{
			szTmp.Format( _T("Ch%i[%.2f] Ch%i[%.2f]\n"), i+1, m_pResultAverage[i], i+2, m_pResultAverage[i+1] );
			szText += szTmp;
		}

		for( int i=0; i<m_nResultChannel; i+=2 )
		{
			double levDiff = fabs(m_pResultAverage[i]-m_pResultAverage[i+1]);
			//if( levDiff>m_nLevelDiff )
			//{
			//	m_iJudge = 0;
			//}
			szTmp.Format( _T("Ch%i-%i���x����[%.2f]\n"), i+1, i+2, levDiff );
			szText += szTmp;
		}

		double maxDiff=-1.0;
		INT pos[2];
		for( int i=0; i<m_nResultChannel-1; i++ )
		{
			for( int j=i+1; j<m_nResultChannel; j++ )
			{
				double diff = fabs(m_pResultAverage[i]-m_pResultAverage[j]);
				if( diff>maxDiff )
				{
					maxDiff = diff;
					pos[0] = i;
					pos[1] = j;
				}
			}
		}
		if( maxDiff>m_nLevelDiff )
		{
			m_iJudge = 0;
		}
		CString szTmp2;
		m_maxLevelDiff = maxDiff;

		for( int i=0; i<2; i++ )
		{
			m_maxPos[i] = pos[i];
			szTmp.Format(_T("%iCh"), pos[i]+1);
			szTmp2 += szTmp;
			if( i<1 ) szTmp2 += _T("-");
		}
		maxText.Format(_T("�ő僌�x����[%.2f](%s)\n"), m_maxLevelDiff, szTmp2 );
	}
	else
	if( m_nResultElement==3 )	//Color
	{
		//for( int c=0; c<3; c++ )
		{
			for( int i=0; i<m_nResultChannel; i++ )
			{
				szTmp.Format( _T("Ch%i[R(%.2f) G(%.2f) B(%.2f)]\n"), i+1, m_pResultAverage[i * 3 + 2], m_pResultAverage[i * 3 + 1], m_pResultAverage[i * 3 + 0] );
				szText += szTmp;
			}
		}
		double levDiff[3];
		for( int i=0; i<m_nResultChannel; i+=2 )
		{
			levDiff[2] = fabs(m_pResultAverage[(i+0) * 3 + 0]-m_pResultAverage[(i+1) * 3 + 0]);
			levDiff[1] = fabs(m_pResultAverage[(i+0) * 3 + 1]-m_pResultAverage[(i+1) * 3 + 1]);
			levDiff[0] = fabs(m_pResultAverage[(i+0) * 3 + 2]-m_pResultAverage[(i+1) * 3 + 2]);
			//if( levDiff>m_nLevelDiff )
			//{
			//	m_iJudge = 0;
			//}
			szTmp.Format( _T("Ch%i-%i���x����[R(%.2f) G(%.2f) B(%.2f)]\n"), i+1, i+2, levDiff[0], levDiff[1], levDiff[2] );
			szText += szTmp;
		}

		//�ő卷�����ߌ���
		double maxDiff=-1.0;
		INT pos[2],posColor;
		for( int c=0; c<3; c++ )
		{
			for( int i=0; i<m_nResultChannel-1; i++ )
			{
				for( int j=i+1; j<m_nResultChannel; j++ )
				{
					double diff = fabs(m_pResultAverage[c*3+i]-m_pResultAverage[c*3+j]);
					if( diff>maxDiff )
					{
						maxDiff = diff;
						pos[0] = i;
						pos[1] = j;
						posColor = c;
					}
				}
			}
		}
		if( maxDiff>m_nLevelDiff )
		{
			m_iJudge = 0;
		}
		CString szTmp2;
		m_maxLevelDiff = maxDiff;
		m_maxLevelColor = posColor;
		if(posColor==0) szTmp2.Format(_T("B:"));
		else if(posColor==1) szTmp2.Format(_T("G:"));
		else if(posColor==2) szTmp2.Format(_T("R:"));
		for( int i=0; i<2; i++ )
		{
			m_maxPos[i] = pos[i];
			szTmp.Format(_T("%iCh"), pos[i]+1);
			szTmp2 += szTmp;
			if( i<1 ) szTmp2 += _T("-");
		}
		maxText.Format(_T("�ő僌�x����[%.2f](%s)\n"), m_maxLevelDiff, szTmp2 );
	}

	if( m_iJudge==2 ) m_iJudge = 1;

	GetDlgItem(IDC_EDIT_VOLUME)->SetWindowText( szText + maxText );

	JudgeText(m_iJudge==1?true:false);

	return TRUE;
}

void StCheckAdjustVolumeDlg::OnDestroy()
{
	__super::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	KillTimer(ST_VOLUME_CHECK);
}
