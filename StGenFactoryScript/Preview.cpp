// Preview.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "Preview.h"
#include "afxdialogex.h"
#include "StCommon.h"
#include "../CommonHeader/StTransformsImage.h"


// CPreview �_�C�A���O

//��1.0.0.1067
//extern void DoEvents();
//��1.0.0.1067


IMPLEMENT_DYNAMIC(CPreview, CDialogEx)

unsigned __stdcall bInitScrollBar( void *pParam )
{
	CPreview *pPreview = (CPreview *)pParam;
	pPreview->InitScrollBar(FALSE);

//TRACE(TEXT("@@@@@@@@@ ibInitScrollBar IN......m_pntCurOffet(%d:%d)...........\n") , pPreview->m_pntCurOffet.x, pPreview->m_pntCurOffet.y );

	return 0;
}

CPreview::CPreview(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPreview::IDD, pParent)
	, m_iMagnify(0)
	, m_lPrevious(0)
{
	//�ϐ�������������
	InitParam();
	InitImages();

	//INI�t�@�C������ݒ���擾
	LPCTSTR 	lpszSection;
	LPCTSTR 	lpszEntry;
	TCHAR		szIniFileName[MAX_PATH];
	CStCommon::GetIniFileName(szIniFileName, MAX_PATH);

	lpszSection = _T("Preview");

	//�v���r���[�J���[���[�h���擾
	lpszEntry	= _T("ColorFg");
	m_bColorPreview = TRUE;
	m_bColorPreview = (BOOL)GetPrivateProfileInt(lpszSection,lpszEntry,m_bColorPreview,szIniFileName);

	//��2007/03/29 v0.45 Beta2 �v���r���[�E�B���h�E����ʓ��Ɏ��߂郂�[�h��ǉ�
	lpszEntry	= _T("PreviewWindowSize");
	m_bPreviewWindowSize = FALSE;
	m_bPreviewWindowSize = (BOOL)GetPrivateProfileInt(lpszSection,lpszEntry,m_bPreviewWindowSize,szIniFileName);
	//��2007/03/29 v0.45 Beta2 �v���r���[�E�B���h�E����ʓ��Ɏ��߂郂�[�h��ǉ�


	//��f�l�X�V�^�C�~���O���擾
	lpszEntry	= _T("PixelValueUpdateOnlyMouseMove");
	//m_bPixelValueUpdateOnlyMouseMove = TRUE;
	m_bPixelValueUpdateOnlyMouseMove = FALSE;
	m_bPixelValueUpdateOnlyMouseMove = (BOOL)GetPrivateProfileInt(lpszSection,lpszEntry,m_bPixelValueUpdateOnlyMouseMove,szIniFileName);


	//
	m_hEImageMutex = CreateMutex(NULL,FALSE,NULL);
	m_hSnapShotMutex = CreateMutex(NULL,FALSE,NULL);
	m_hSnapShotEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hUserRawCallbackMutex = CreateMutex(NULL,FALSE,NULL);
	m_hUserGDICallbackMutex = CreateMutex(NULL,FALSE,NULL);
	m_hScrollbarMutex = CreateMutex(NULL,FALSE,NULL);
	m_pRawCallbackFunc = NULL;
	m_pRawCallbackContext = NULL;
	m_iFrameNo = 0;
	m_pGDICallbackFunc = NULL;
	m_pGDICallbackContext = NULL;

	m_hGUIWnd = NULL;

	m_hMainGUIWnd = NULL;

	m_sMousePoint.x = 0;
	m_sMousePoint.y = 0;


	//�摜�\���I�t�Z�b�g
	m_pntCurOffet.x = 0;
	m_pntCurOffet.y = 0;
	m_rectClientSize.left = 0;
	m_rectClientSize.right = 0;
	m_rectClientSize.top = 0;
	m_rectClientSize.bottom = 0;


	m_dstImgBuffer = NULL;

}

CPreview::~CPreview()
{
	ReleaseImages();

	CloseHandle(m_hEImageMutex);
	CloseHandle(m_hSnapShotMutex);
	CloseHandle(m_hSnapShotEvent);
	CloseHandle(m_hUserRawCallbackMutex);
	CloseHandle(m_hUserGDICallbackMutex);
	CloseHandle(m_hScrollbarMutex);
}

void CPreview::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPreview, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()
	ON_COMMAND_RANGE(IDM_SIZE_0, IDM_SIZE_7, OnMenuPreviewSize)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPreview ���b�Z�[�W �n���h���[

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CPreview::GetClientRectIncludeScrollBar(CRect *pRect)
{
	GetClientRect(pRect);


	for(int i = 0; i < 2;i++)
	{
		SCROLLINFO	scrollInfo;
		//�X�N���[���o�[�\���̂�������
		memset(&scrollInfo, 0, sizeof(SCROLLINFO));
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
		int nBar = (i == 0)?SB_HORZ : SB_VERT;
		
		//�X�N���[���o�[���X�V
		GetScrollInfo(nBar, &scrollInfo, SIF_ALL);

		if(scrollInfo.nMax != scrollInfo.nMin)
		{
			if(i == 0)
			{
				//���X�N���[���o�[�̍���
				pRect->bottom += GetSystemMetrics(SM_CYHSCROLL);
			}
			else
			{
				//�c�X�N���[���o�[�̕�
				pRect->left += GetSystemMetrics(SM_CXVSCROLL);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CPreview::InitScrollBar(BOOL bInvalidFlag)
{
//TRACE(TEXT("@@@@@@@@@ InitScrollBar WaitForSingleObject IN\n") );

	WaitForSingleObject(m_hScrollbarMutex,INFINITE);
//TRACE(TEXT("@@@@@@@@@ InitScrollBar WaitForSingleObject Out\n") );

	m_bHScroll = FALSE;
	m_bVScroll = FALSE;

	//���݂̃N���C�A���g�E�B���h�E�T�C�Y���擾
	GetClientRect(&m_rectClientSize);

	//�X�N���[���o�[���܂񂾃N���C�A���g�T�C�Y���擾
	CRect rectClientSizeWithSB;
	GetClientRectIncludeScrollBar(&rectClientSizeWithSB);

	double dblBairitsu = mGetBairitsu();
	DWORD	dwDispEnableWidth = (DWORD)(rectClientSizeWithSB.Width() / dblBairitsu);
	DWORD	dwDispEnableHeight = (DWORD)(rectClientSizeWithSB.Height() / dblBairitsu);


	do{
		if(m_bHScroll)
		{
			dwDispEnableHeight = (DWORD)((rectClientSizeWithSB.Height() - GetSystemMetrics(SM_CYHSCROLL)) / dblBairitsu);
		}
		if(m_bVScroll)
		{
			dwDispEnableWidth = (DWORD)((rectClientSizeWithSB.Width() - GetSystemMetrics(SM_CXVSCROLL)) / dblBairitsu);		
		}
		//�X�N���[���o�[�\���ɂ��摜���B��Ă��܂�Ȃ����`�F�b�N
		//�摜�\���ɏ\���ȃT�C�Y�����邩�ǂ������`�F�b�N
		if( (int)dwDispEnableWidth < m_iDestWidth && !m_bHScroll )
		{
			m_bHScroll = TRUE;
			continue;
		}
		if( (int)dwDispEnableHeight < m_iDestHeight && !m_bVScroll )
		{
			m_bVScroll = TRUE;
			continue;
		}
		break;
	}while(1);

//TRACE(TEXT("@@@@@@@@@ InitScrollBar 1\n") );
	//�c���̃X�N���[���o�[�����ɐݒ�
	SCROLLINFO	pScrollInfo[2];
	for(int i = 0; i < 2;i++)
	{

		//�X�N���[���o�[�\���̂�������
		memset(&pScrollInfo[i], 0, sizeof(SCROLLINFO));
		pScrollInfo[i].cbSize = sizeof(SCROLLINFO);
		pScrollInfo[i].fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
		int nBar = (i == 0)?SB_HORZ : SB_VERT;
		if(i == 0)
		{
			//������
			nBar = SB_HORZ;

			if(m_bHScroll)
			{
				pScrollInfo[i].nMax = m_iDestWidth - 1;
				pScrollInfo[i].nPage = dwDispEnableWidth;
				pScrollInfo[i].nPos = m_pntCurOffet.x;
			}
		}
		else
		{
			//�c����
			nBar = SB_VERT;
			if(m_bVScroll)
			{
				pScrollInfo[i].nMax = m_iDestHeight - 1;
				pScrollInfo[i].nPage = dwDispEnableHeight;
				pScrollInfo[i].nPos = m_pntCurOffet.y;
			}
		}

		//���݂̈ʒu���ő�l�𒴂��Ă���ꍇ�͍ő�l�ɏC��
		INT		iMaxPos = pScrollInfo[i].nMax - pScrollInfo[i].nPage + 1;
		if(iMaxPos < 0)	iMaxPos = 0;

		if(iMaxPos < pScrollInfo[i].nPos)
		{
			pScrollInfo[i].nPos = iMaxPos;
		}
//TRACE(TEXT("@@@@@@@@@ InitScrollBar a%d\n"), i );

		//�X���b�h�o�R����SetScrollInfo�Ńt���[�Y����H
		if( bInvalidFlag )
		{
			//�X�N���[���o�[���X�V
			SetScrollInfo(nBar, &pScrollInfo[i], TRUE);
		}
//TRACE(TEXT("@@@@@@@@@ InitScrollBar b%d\n"), i );
	}
	SetOffset(pScrollInfo[0].nPos, pScrollInfo[1].nPos, FALSE);
//TRACE(TEXT("@@@@@@@@@ InitScrollBar 2\n") );

	ReleaseMutex(m_hScrollbarMutex);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CPreview::ProcScroll(int nBar, UINT nSBCode, UINT nPos)
{
	INT		iNewPos = 0;
	BOOL	bReval = TRUE;

	if(
		(SB_VERT == nBar) &&
		(!m_bVScroll)
	)
		return;
	else if(
		(SB_HORZ == nBar) &&
		(!m_bHScroll)
	)
		return;

	SCROLLINFO	sScrollInfo;

	memset(&sScrollInfo, 0, sizeof(SCROLLINFO));
	sScrollInfo.cbSize = sizeof(SCROLLINFO);
	sScrollInfo.fMask = SIF_ALL;
	GetScrollInfo(nBar, &sScrollInfo);
	
	INT		iMaxPos = sScrollInfo.nMax - sScrollInfo.nPage + 1;
	if(SB_HORZ == nBar)
	{
		iNewPos = m_pntCurOffet.x;
	}
	else
	{
		iNewPos = m_pntCurOffet.y;
	}
	
	switch(nSBCode)
	{
	case SB_LINEUP:
		if(0 < iNewPos)		iNewPos--;
		break;
	case SB_LINEDOWN:
		iNewPos++;
		break;
	case SB_PAGEUP:
		if((INT)sScrollInfo.nPage < iNewPos) iNewPos -= sScrollInfo.nPage;
		else							iNewPos = 0;
		break;
	case SB_PAGEDOWN:
		iNewPos += sScrollInfo.nPage;
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		iNewPos = sScrollInfo.nTrackPos;
		break;
	case SB_TOP:
		iNewPos = 0;
		break;
	case SB_BOTTOM:
		iNewPos = iMaxPos;
		break;
	}

	if(iNewPos < sScrollInfo.nMin)	iNewPos = sScrollInfo.nMin;
	else if(iMaxPos < iNewPos) iNewPos =  iMaxPos;

	if(SB_HORZ == nBar)
	{
		m_pntCurOffet.x = iNewPos;
	}
	else
	{
		m_pntCurOffet.y = iNewPos;
	}

	sScrollInfo.fMask = SIF_POS;
	sScrollInfo.nPos = iNewPos;
	SetScrollInfo(nBar, &sScrollInfo, TRUE);

	SetOffset(m_pntCurOffet.x, m_pntCurOffet.y, TRUE);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CPreview::SetOffset(INT iOffsetX, INT iOffsetY, BOOL bRedraw)
{

	//�摜�\���I�t�Z�b�g
	m_pntCurOffet.x = iOffsetX;
	m_pntCurOffet.y = iOffsetY;

//if( m_pntCurOffet.x!=0 && iOffsetX==0)
//	Sleep(1);
	Invalidate(bRedraw);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//�ϐ��̏�����
void CPreview::InitParam(void)
{
	// Flags
	//m_bLive= FALSE;
	//m_bOnGoingRefresh= FALSE;

	//��2007/06/05 v0.47 Beta1 �X�N���[���o�[���Ȃ��Ƃ��ɃS�~�����ŉ�ʂ��X�N���[������Ă��܂��o�O���C��
	m_bHScroll = FALSE;
	m_bVScroll = FALSE;
	//��2007/06/05 v0.47 Beta1 �X�N���[���o�[���Ȃ��Ƃ��ɃS�~�����ŉ�ʂ��X�N���[������Ă��܂��o�O���C��

	m_pvSnapShotBuffer = NULL;

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL CPreview::AddRawCallbackFunc(fStCamRawCallbackFunc pfNewFunc,PVOID pContext)
{
	::WaitForSingleObject(m_hUserRawCallbackMutex,INFINITE);
	m_pRawCallbackFunc = pfNewFunc;
	m_pRawCallbackContext = pContext;
	::ReleaseMutex(m_hUserRawCallbackMutex);
	return(TRUE);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL CPreview::AddGDICallbackFunc(fStCamPreviewGDICallbackFunc pfNewFunc,PVOID pContext)
{
	::WaitForSingleObject(m_hUserGDICallbackMutex,INFINITE);
	m_pGDICallbackFunc = pfNewFunc;
	m_pGDICallbackContext = pContext;
	::ReleaseMutex(m_hUserGDICallbackMutex);
	return(TRUE);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CPreview::UpdateTitleText(void)
{
	TCHAR	szMsg[1024];
	BYTE	pbyteBGR[3] = {0,0,0};
	TCHAR	szColor[10];
	INT		iColorNo;
	WORD	wRawValue = 0;
	PBYTE pbyteTmp;


	CPoint pointImage;
	GetImagePoint(&pointImage);

	if( m_iSaveLastIndex<0 ) return;


	::WaitForSingleObject(m_hEImageMutex,INFINITE);

//TRACE(TEXT("@@@@@@@@@ m_iSaveLastIndex=%d IN\n") , m_iSaveLastIndex );
	m_srcSaveBuffer[m_iSaveLastIndex].WaitForSingleObject2(INFINITE);
//TRACE(TEXT("@@@@@@@@@ m_iSaveLastIndex=%d OUT\n") , m_iSaveLastIndex );

	StBuffer *aBuffer = &m_srcSaveBuffer[m_iSaveLastIndex];
	if(
		(m_pBitmapInfo != NULL)
		&& (m_pDispBuffer != NULL)
		&& (pointImage.x < m_iDestWidth )
		&& (pointImage.y < m_iDestHeight)
		//&& (pointImage.x < aBuffer->GetWidth())
		//&& (pointImage.y < aBuffer->GetHeight())
	)
	{
		switch(m_pBitmapInfo->bmiHeader.biBitCount)
		{
		case(8):
			pbyteBGR[0] = m_pDispBuffer[pointImage.y * m_iDestLinePitch + pointImage.x];
			pbyteBGR[1] = pbyteBGR[0];
			pbyteBGR[2] = pbyteBGR[0];
			break;
		case(32):
			pbyteTmp = (PBYTE)&m_pDispBuffer[pointImage.y * m_iDestLinePitch + pointImage.x * 4];
			pbyteBGR[0] = pbyteTmp[0];
			pbyteBGR[1] = pbyteTmp[1];
			pbyteBGR[2] = pbyteTmp[2];
			break;
		default:
		//case(24):
			pbyteTmp = (PBYTE)&m_pDispBuffer[pointImage.y * m_iDestLinePitch + pointImage.x * 3];
			pbyteBGR[0] = pbyteTmp[0];
			pbyteBGR[1] = pbyteTmp[1];
			pbyteBGR[2] = pbyteTmp[2];
			break;
		}
//TRACE(TEXT("@@@@@@@@@ x:y=%d:%d Width=%d\n"), pointImage.x, pointImage.y,aBuffer->GetWidth()  );
//TRACE(TEXT("@@@@@@@@@ aBuffer->GetDataPointer()=0x%x size=%d\n"), aBuffer->GetDataPointer(), aBuffer->GetSize()  );
//TRACE(TEXT("@@@@@@@@@ aBuffer->GetAllocSize()=%d m_iSaveLastIndex=%d\n"), aBuffer->GetAllocSize() , m_iSaveLastIndex );

		if(8 != ((aBuffer->GetPixelType()>>16) & 0xFF ) )
			wRawValue = ((PWORD)aBuffer->GetDataPointer())[pointImage.y * aBuffer->GetWidth() + pointImage.x];
		else
			wRawValue = ((PBYTE)aBuffer->GetDataPointer())[pointImage.y * aBuffer->GetWidth() + pointImage.x];
	}
	StPixelType aType = aBuffer->GetPixelType();

	m_srcSaveBuffer[m_iSaveLastIndex].ReleaseMutex();

	::ReleaseMutex(m_hEImageMutex);


	iColorNo = (((pointImage.y &1)<<1) + (pointImage.x &1));

	wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT("?"));
	switch(aType)
	{
	case(StPixelBayerRG8):
	case(StPixelBayerRG10):
	case(StPixelBayerRG12):
	case(StPixelBayerRG16):
		if(iColorNo == 0)		wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT(" R"));
		else if(iColorNo == 1)	wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT("Gr"));
		else if(iColorNo == 2)	wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT("Gb"));
		else if(iColorNo == 3)	wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT(" B"));
		break;

	case(StPixelBayerGR8):
	case(StPixelBayerGR10):
	case(StPixelBayerGR12):
	case(StPixelBayerGR16):
		if(iColorNo == 0)		wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT("Gr"));
		else if(iColorNo == 1)	wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT(" R"));
		else if(iColorNo == 2)	wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT(" B"));
		else if(iColorNo == 3)	wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT("Gb"));
		break;
	case(StPixelBayerGB8):
	case(StPixelBayerGB10):
	case(StPixelBayerGB12):
	case(StPixelBayerGB16):
		if(iColorNo == 0)		wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT("Gb"));
		else if(iColorNo == 1)	wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT(" B"));
		else if(iColorNo == 2)	wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT(" R"));
		else if(iColorNo == 3)	wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT("Gr"));
		break;
	case(StPixelBayerBG8):
	case(StPixelBayerBG10):
	case(StPixelBayerBG12):
	case(StPixelBayerBG16):
		if(iColorNo == 0)		wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT(" B"));
		else if(iColorNo == 1)	wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT("Gb"));
		else if(iColorNo == 2)	wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT("Gr"));
		else if(iColorNo == 3)	wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT(" R"));
		break;
	case(StPixelMono8):
	case(StPixelMono10):
	case(StPixelMono12):
	case(StPixelMono14):
	case(StPixelMono16):
		wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT("BW"));
		break;
	default:
		wcscpy_s(szColor,sizeof(szColor)/sizeof(szColor[0]),TEXT("Other"));
		break;
	}

	TCHAR szBairitsu[1024];
	if(0 <= m_iMagnify)
	{
		swprintf(szBairitsu,sizeof(szBairitsu)/sizeof(szBairitsu[0]), TEXT(" x %.0f"), pow(2.0, m_iMagnify));
	}
	else
	{
		swprintf(szBairitsu,sizeof(szBairitsu)/sizeof(szBairitsu[0]), TEXT(" x 1 / %.0f"), pow(2.0, -m_iMagnify));
	}

	_stprintf(szMsg,TEXT("�v���r���[��� (x,y) = (%03d,%03d)/(R,G,B)=(%03d,%03d,%03d)/[%s]=%04u %s"),
		pointImage.x,pointImage.y,
		pbyteBGR[2],pbyteBGR[1],pbyteBGR[0],
		szColor,
		wRawValue, szBairitsu);

	SetWindowText(szMsg);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CPreview::SetMagnify(INT iMagnify)
{
	m_iMagnify = iMagnify;

	InitScrollBar();
	Invalidate(TRUE);
	UpdateTitleText();
}

void CPreview::AdjustWindow(int sizeX, int sizeY)
{
	//�E�B���h�E�T�C�Y���摜�T�C�Y�ɍ��킹��
	CRect rectImage = CRect(0,0,sizeX,sizeY);
	DWORD dwStyle = GetWindowLong(m_hWnd,GWL_STYLE);
	DWORD dwExStyle = GetWindowLong(m_hWnd,GWL_EXSTYLE);
	AdjustWindowRectEx(&rectImage,dwStyle,FALSE,dwExStyle);

	//�����L���ɂ���Ɖ摜��荞�݂̃o�b�t�@�m�ۂł��Ȃ��ăG���[
	//m_iDestWidth = sizeX;
	//m_iDestHeight = sizeY;
	m_iImageMaxWidth = sizeX;
	m_iImageMaxHeight = sizeY;

	//�f�X�N�g�b�v�T�C�Y�擾
	CRect	rectDesktop;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rectDesktop, 0);
	if(rectDesktop.Width() < rectImage.Width())
	{
		rectImage.left = rectDesktop.left;
		rectImage.right = rectDesktop.right;
	}
	if(rectDesktop.Height() < rectImage.Height())
	{
		rectImage.top = rectDesktop.top;
		rectImage.bottom = rectDesktop.bottom;
	}

	//�v���r���[�E�B���h�E����ʓ��Ɏ��߂郂�[�h��ǉ�
	SetWindowPos(NULL,0,0,rectImage.Width(),rectImage.Height(),SWP_NOACTIVATE | SWP_NOMOVE);

	//�����I��WM_SIZE�𑗂�o���ăT�C�Y����
	SendMessage(WM_SIZE, 0, MAKELPARAM(rectImage.Width(), rectImage.Height()));

}

void CPreview::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	// �`�惁�b�Z�[�W�� CDialogEx::OnPaint() ���Ăяo���Ȃ��ł��������B
	PaintMain();
}


void CPreview::PaintMain()
{
	//CPaintDC dc(this); // device context for painting
	//�����ł�CPaintDC�͎g�p�ł��Ȃ��̂�CClientDC���g�p
	CClientDC dc(this);


	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	// �`�惁�b�Z�[�W�� CDialogEx::OnPaint() ���Ăяo���Ȃ��ł��������B
	::WaitForSingleObject(m_hEImageMutex,INFINITE);
	if(
		(m_pDispBuffer != NULL) &&
		(m_pBitmapInfo != NULL)
		)
	{
		
		HDC		hMemDc = CreateCompatibleDC(dc);
		HBITMAP hBitmap = CreateCompatibleBitmap(dc,m_pBitmapInfo->bmiHeader.biWidth,abs(m_pBitmapInfo->bmiHeader.biHeight));	
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDc,hBitmap);

//�`��X�V�m�F�̂��߁B�B�B�B�B�B�B�B�B�B
//{
//static int iCount=0;
//int loop = m_pBitmapInfo->bmiHeader.biWidth * abs(m_pBitmapInfo->bmiHeader.biHeight);
//char *pTop = m_pDispBuffer;
//for( int i=0; i<loop; i++ )
//{
//	//pTop[2] = pTop[1] = pTop[0] = ((i+iCount)&0xff);
//	pTop[0] = ((i+iCount)&0xff);
//	pTop+=1;
//}
//iCount++;
//}

		SetDIBits(
			hMemDc,
			hBitmap,
			0,
			//m_pBitmapInfo->bmiHeader.biWidth,
			abs(m_pBitmapInfo->bmiHeader.biHeight),
			m_pDispBuffer,
			m_pBitmapInfo,
			DIB_RGB_COLORS);

//TRACE(TEXT("SetDIBits(%d)\n"), iDIB);

//		SetStretchBltMode(hMemDc,STRETCH_DELETESCANS);
//		int ret = StretchDIBits (hMemDc, 0, 0, m_pBitmapInfo->bmiHeader.biWidth, abs(m_pBitmapInfo->bmiHeader.biHeight),
//                                0, 0, m_pBitmapInfo->bmiHeader.biWidth, abs(m_pBitmapInfo->bmiHeader.biHeight),
//                                m_pDispBuffer, m_pBitmapInfo, DIB_RGB_COLORS , SRCCOPY );
//TRACE(TEXT("StretchDIBits(%d)\n"), ret);


		//���[�U��`�̃R�[���o�b�N�֐��Ăяo��
		::WaitForSingleObject(m_hUserGDICallbackMutex,INFINITE);
		if(m_pGDICallbackFunc != NULL)
		{
			m_pGDICallbackFunc(
				hMemDc,
				m_iDestWidth,
				m_iDestHeight,
				m_iFrameNo,
				m_pGDICallbackContext,
				NULL);
		}
		::ReleaseMutex(m_hUserGDICallbackMutex);
		


		
		SetStretchBltMode(dc, COLORONCOLOR);
		
		struct {
			int OffsetX;
			int OffsetY;
			int Width;
			int Height;
		} sSrcImage, sDestImage;

		sSrcImage.OffsetX = m_pntCurOffet.x;
		sSrcImage.OffsetY = m_pntCurOffet.y;
		sSrcImage.Width = m_iDestWidth - m_pntCurOffet.x;
		sSrcImage.Height = m_iDestHeight - m_pntCurOffet.y;


		double dblBairitsu = mGetBairitsu();
		sDestImage.OffsetX = 0;
		sDestImage.OffsetY = 0;
		sDestImage.Width = (int)(sSrcImage.Width * dblBairitsu);
		sDestImage.Height = (int)(sSrcImage.Height * dblBairitsu);


		int nClientWidth = m_rectClientSize.Width();
		//�E���̉B��镔��������
		if(nClientWidth < sDestImage.Width)
		{
			sSrcImage.Width -= (INT)((sDestImage.Width - nClientWidth) / dblBairitsu);
			sDestImage.Width = (INT)(sSrcImage.Width * dblBairitsu);
		}


		int nClientHeight = m_rectClientSize.Height();
		//�����̉B��镔��������
		if(nClientHeight < sDestImage.Height)
		{
			sSrcImage.Height -= (INT)((sDestImage.Height - nClientHeight) / dblBairitsu);
			sDestImage.Height = (INT)(sSrcImage.Height * dblBairitsu);
		}
		
//	DoEvents();

		StretchBlt(
			dc.m_hDC, 
			sDestImage.OffsetX, sDestImage.OffsetY, sDestImage.Width, sDestImage.Height,	//DEST			 
			hMemDc, 
			sSrcImage.OffsetX, sSrcImage.OffsetY, sSrcImage.Width, sSrcImage.Height, //SRC
			SRCCOPY);

		
		//if(sDestImage.Width < m_rectClientSize.Width())
		//{
		//	RECT rect = {sDestImage.Width, 0, m_rectClientSize.Width(), sDestImage.Height};
		//	FillRect(dc.m_hDC, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
		//}
		//if(sDestImage.Height < m_rectClientSize.Height())
		//{
		//	RECT rect = {0, sDestImage.Height, m_rectClientSize.Width(), m_rectClientSize.Height()};
		//	FillRect(dc.m_hDC, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
		//}
		
		SelectObject(hMemDc,hOldBitmap);
		DeleteObject(hBitmap);
		DeleteDC(hMemDc);
		//m_bOnGoingRefresh = FALSE;
	}
	else
	{
		RECT rect = {0, 0, m_rectClientSize.Width(), m_rectClientSize.Height()};
		FillRect(dc.m_hDC, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
	}
	::ReleaseMutex(m_hEImageMutex);

	if(!m_bPixelValueUpdateOnlyMouseMove)
	{
		//�}�E�X�ʒu�̉�f�l��\������
//		UpdateTitleText();
	}

	//��1.0.0.1067
//	DoEvents();
	//��1.0.0.1067

}


void CPreview::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	if(m_hMainGUIWnd)
	{
		::PostMessage(m_hMainGUIWnd,WM_CL_LBUTTON_UP,point.x,point.y);
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CPreview::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	m_sMousePoint = point;

	//�}�E�X�ʒu�̉�f�l��\������
	UpdateTitleText();

	CDialogEx::OnMouseMove(nFlags, point);
}


void CPreview::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	InitScrollBar();
}


void CPreview::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	ProcScroll(SB_HORZ, nSBCode, nPos);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CPreview::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	ProcScroll(SB_VERT, nSBCode, nPos);

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CPreview::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	int nBar = SB_HORZ;
	UINT nSBCode = SB_PAGEUP;
	UINT nPos = 0;
	if(zDelta < 0)
	{
		nSBCode = SB_PAGEDOWN;
	}
	ScreenToClient(&pt);

	if(
		(0 < m_iDestWidth) &&
		(0 < pt.x)
	)
	{
		if(pt.y * m_iDestWidth < pt.x * m_iDestHeight)
		{
			nBar = SB_VERT;
		}
	}
	ProcScroll(nBar, nSBCode, nPos);

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CPreview::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	CMenu menu;
	menu.LoadMenu(IDR_MENU_PREVIEW_POPUP);
	CMenu* pContextMenu = menu.GetSubMenu(0);


	//��1.0.0.1058
	//for(INT nID = IDM_SIZE_0; nID <= IDM_SIZE_6; nID++)
	//{
	//	pContextMenu->CheckMenuItem(nID, (m_iMagnify + IDM_SIZE_3 == nID)?MF_CHECKED:MF_UNCHECKED );
	//}
	for(INT nID = IDM_SIZE_0; nID <= IDM_SIZE_7; nID++)
	{
		pContextMenu->CheckMenuItem(nID, (m_iMagnify + IDM_SIZE_4 == nID)?MF_CHECKED:MF_UNCHECKED );
	}
	//��1.0.0.1058
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CPreview::OnMenuPreviewSize(UINT nID)
{
	//��1.0.0.1058
	//SetMagnify(nID - IDM_SIZE_3);
	SetMagnify(nID - IDM_SIZE_4);
	//��1.0.0.1058
}

//======================================================================================================================
int iCheckColor( StPixelType pixelType, INT iDisplayFormat )
{
	int iColor = 1;	//BGR

	if( pixelType==StPixelMono8 ||
		pixelType==StPixelMono8Signed ||
		pixelType==StPixelMono10 ||
		pixelType==StPixelMono10Packed ||
		pixelType==StPixelMono12 ||
		pixelType==StPixelMono12Packed ||
		pixelType==StPixelMono14 ||
		pixelType==StPixelMono16
		
		|| pixelType==StPixelMono8_16Bit
		|| pixelType==StPixelMono10pmsb_16Bit
		|| pixelType==StPixelMono12pmsb_16Bit
		|| pixelType==StPixelMono14pmsb_16Bit
		|| pixelType==StPixelMono16_16Bit
		)
	{
		iColor = 0; //MONO
	}
	if( iColor )
	{
		if( (pixelType&STBITSPERPIXELMASK)==STPIXEL32BIT || (pixelType&STBITSPERPIXELMASK)==STPIXEL64BIT )
			iColor = 2;	//BGRa
	}

	if( iColor!=0 ) //�ݒ�ŕ\�����[�h��ύX����悤�ɂ���
	{
		if( iDisplayFormat==0 )	iColor = 1;	//BGR
		else					iColor = 2;	//BGRa

	}
	return iColor;
}

void CPreview::InitImages(void)
{



	m_iDestWidth=0;
	m_iDestHeight=0;
	m_iDestLinePitch=0;
	m_iDestBufferSize=0;
	m_iDestColor=0;
	m_iDisplayBufferPos=-1;
	m_ppDisplayBuffer=NULL;


	m_pDispBuffer=NULL;
	m_pBitmapInfo=NULL;

	m_iImageMaxWidth=0;
	m_iImageMaxHeight=0;

	m_iSaveIndex = 0;
	m_iSaveLastIndex = -1;

}

void CPreview::ReleaseImages(void)
{
	::WaitForSingleObject(m_hEImageMutex,INFINITE);
	DeleteDisplayBuffer();
	for( int i=0; i<IMAGE_SAVE_COUNT; i++ )
		m_srcSaveBuffer[i].Free();

	ReleaseBitmapInfo();
	InitImages();

	::ReleaseMutex(m_hEImageMutex);
}

void CPreview::StBufferCopy(StBuffer* SrcBuffer, StCustomBuffer* DstBuffer)
{

	if( !SrcBuffer || !DstBuffer ) return; 

	if( !SrcBuffer->GetDataPointer() ) return;
	if( SrcBuffer->GetSize()==0 ) return;
	//if( !DstBuffer->GetDataPointer() ) return;

//TRACE(TEXT("@@@@@@@@@ DstBuffer=0x%x IN\n") , DstBuffer );
	DstBuffer->WaitForSingleObject2(INFINITE);
//TRACE(TEXT("@@@@@@@@@ DstBuffer=0x%x OUT\n") , DstBuffer );
//#ifdef _PLEORA
//	if( SrcBuffer->GetImage()->GetWidth()!=DstBuffer->GetImage()->GetWidth()
//		|| SrcBuffer->GetImage()->GetHeight()!=DstBuffer->GetImage()->GetHeight()
//		|| SrcBuffer->GetImage()->GetPixelType()!=DstBuffer->GetImage()->GetPixelType() )
//	{
//		DstBuffer->Free();
//		DstBuffer->GetImage()->Alloc(SrcBuffer->GetImage()->GetWidth(),SrcBuffer->GetImage()->GetHeight(),SrcBuffer->GetImage()->GetPixelType() );
//	}
//#else

	//��1.0.0.1046
	//UINT uiHeight = SrcBuffer->GetHeight();
	//if( uiHeight==0 )
	//	uiHeight = SrcBuffer->GetDeliveredImageHeight();
	//��1.0.0.1046


	if( SrcBuffer->GetWidth()!=DstBuffer->GetWidth()
		|| SrcBuffer->GetHeight()!=DstBuffer->GetHeight()
		//��1.0.0.1046
		//|| SrcBuffer->GetDeliveredImageHeight()!=DstBuffer->GetDeliveredImageHeight()
		//��1.0.0.1046
		|| SrcBuffer->GetPixelType()!=DstBuffer->GetPixelType() )
	{
		DstBuffer->Free();
		DstBuffer->Alloc(SrcBuffer->GetSize());
//TRACE(TEXT("@@@@  GetAllocSize=%d(%d)\n"), DstBuffer->GetAllocSize(), SrcBuffer->GetSize());
		DstBuffer->SetWidth(SrcBuffer->GetWidth());
		DstBuffer->SetHeight(SrcBuffer->GetHeight());
		//��1.0.0.1046
		//DstBuffer->SetDeliveredImageHeight(SrcBuffer->GetDeliveredImageHeight());
		//��1.0.0.1046
		DstBuffer->SetPixelType(SrcBuffer->GetPixelType());
//TRACE(TEXT("@@@@  GetWidth=%d, GetHeight=%d, GetPixelType=0x%x,\n"), DstBuffer->GetWidth(), DstBuffer->GetHeight(), DstBuffer->GetPixelType());
//PBYTE aData = (PBYTE)SrcBuffer->GetDataPointer();
//TRACE( _T("@@@@ aData=%d\n"), *(PWORD)aData );
	}
//#endif
	DstBuffer->SetID(SrcBuffer->GetID());
	DstBuffer->SetTimestamp(SrcBuffer->GetTimestamp());
	memcpy( (void *)DstBuffer->GetDataPointer(), SrcBuffer->GetDataPointer(), SrcBuffer->GetSize() );
	DstBuffer->SetSize(SrcBuffer->GetSize());

	DstBuffer->ReleaseMutex();

}

void CPreview::StBufferCopy(StBuffer* SrcBuffer, StBuffer* DstBuffer)
{
//#ifdef _PLEORA
//	if( SrcBuffer->GetImage()->GetWidth()!=DstBuffer->GetImage()->GetWidth()
//		|| SrcBuffer->GetImage()->GetHeight()!=DstBuffer->GetImage()->GetHeight()
//		|| SrcBuffer->GetImage()->GetPixelType()!=DstBuffer->GetImage()->GetPixelType()
//		|| SrcBuffer->GetSize()!=DstBuffer->GetSize()
//		)
//	{
//		DstBuffer->Free();
//		DstBuffer->GetImage()->Alloc(SrcBuffer->GetImage()->GetWidth(),SrcBuffer->GetImage()->GetHeight(),SrcBuffer->GetImage()->GetPixelType() );
//	}
//#else
	if( SrcBuffer->GetWidth()!=DstBuffer->GetWidth()
		|| SrcBuffer->GetHeight()!=DstBuffer->GetHeight()
		|| SrcBuffer->GetPixelType()!=DstBuffer->GetPixelType()
		|| SrcBuffer->GetSize()!=DstBuffer->GetAllocSize()
		)
	{
		DstBuffer->Free();
		StResult aResult = DstBuffer->Alloc(SrcBuffer->GetSize());

		DstBuffer->SetWidth(SrcBuffer->GetWidth());
		DstBuffer->SetHeight(SrcBuffer->GetHeight());
		DstBuffer->SetPixelType(SrcBuffer->GetPixelType());
	}
//#endif

	DstBuffer->SetID(SrcBuffer->GetID());
	DstBuffer->SetTimestamp(SrcBuffer->GetTimestamp());
	//DstBuffer->SetSize(SrcBuffer->GetSize());

//if( SrcBuffer->GetSize()!=DstBuffer->GetSize() )
//{
//TRACE(_T(" @@@@@@@@@@@@@@@SrcBuffer->GetSize()=%i DstBuffer->GetSize()=%i \n"), SrcBuffer->GetSize(),DstBuffer->GetSize());
//
//	Sleep(1);
//}
	memcpy( (void *)DstBuffer->GetDataPointer(), SrcBuffer->GetDataPointer(), SrcBuffer->GetSize() );
}

#define ST_INIT_SCROLLBAR 100
#define ST_IMAGEDISPLAY 101

//#ifdef _PLEORA
//void CPreview::StGenImageDisplay(PvBuffer* buffer)
//#else
void CPreview::StGenImageDisplayMain(StBuffer* buffer)
//#endif
{
    if (!buffer) return;
    //if (!this) return; //Stop����this��0�ɂȂ鎞������B�@�o�O����H

	//if( buffer->GetPayloadType() != PAYLOAD_TYPE_IMAGE && buffer->GetPayloadType() != PAYLOAD_TYPE_CHUNK_DATA && buffer->GetPayloadType() != PAYLOAD_TYPE_DEVICE_SPECIFIC ) return;
//#ifdef _PLEORA
//	PvPixelType aType = buffer->GetImage()->GetPixelType();
//	unsigned int uiWidth = buffer->GetImage()->GetWidth();
//	unsigned int uiHeight = buffer->GetImage()->GetHeight();
//#else


	//Mutex�͂����ɕK�v��

	//-----------------------------------------------------------------------
	::WaitForSingleObject(m_hEImageMutex,INFINITE);


	StPixelType aType = buffer->GetPixelType();
	unsigned int uiWidth = buffer->GetWidth();
	unsigned int uiHeight = buffer->GetHeight();

	//��1.0.0.1046
	if( uiHeight==0 )
	{
		uiHeight = buffer->GetDeliveredImageHeight();
	}
	if( uiHeight==0 )
	{
		return;
	}
	//��1.0.0.1046

//{
//char *pBuffer = (char *)buffer->GetDataPointer();
//TRACE(TEXT("@@@@@@@@@ Width=%u Height=%u aOutBuffer[%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x]\n"),uiWidth, uiHeight, pBuffer[0],pBuffer[1],pBuffer[2],pBuffer[3],pBuffer[4],pBuffer[5],pBuffer[6],pBuffer[7]);
//}


//#endif
	//const char *pBuffer = (const char *)buffer->GetDataPointer() + buffer->GetImageOffset();

	//�f���ۑ�---------------------------------------------------------------------------
//TRACE(TEXT("E: this=0x%x m_iSaveIndex=%d m_srcSaveBuffer=0x%x\n"),this,m_iSaveIndex,m_srcSaveBuffer);
	StBufferCopy(buffer,&m_srcSaveBuffer[m_iSaveIndex]);
//TRACE(TEXT("@@@@@@@@@ m_srcSaveBuffer[m_iSaveIndex]->GetAllocSize()=%d m_iSaveLastIndex=%d m_iSaveIndex=%d\n"), m_srcSaveBuffer[m_iSaveIndex].GetAllocSize() , m_iSaveLastIndex , m_iSaveIndex );
	m_iSaveLastIndex = m_iSaveIndex;
	m_iSaveIndex = (m_iSaveIndex+1)%IMAGE_SAVE_COUNT;

	////-----------------------------------------------------------------------
	//WaitForSingleObject(m_hEImageMutex,INFINITE);

	//�\��Buffer�m��--------------------------------------------------------------------------------
	int	iColor = iCheckColor( aType, 0 );

	if( iColor!=m_iDestColor || m_iDestWidth != uiWidth || m_iDestHeight != uiHeight )
	//if( m_iDestWidth != uiWidth || m_iDestHeight != uiHeight )
	{
		DeleteDisplayBuffer();

		m_iDestLinePitch = uiWidth;
		//m_iDestLinePitch *= 3;
		if( iColor )
		{
			m_iDestLinePitch *= 3;
			//m_iDestLinePitch *= (iColor==1?3:4);
		}
			
		//�S�̔{���ɐ؂�グ
		m_iDestLinePitch = (m_iDestLinePitch + 3) & (~0x3);
			
		m_iDestBufferSize = m_iDestLinePitch * uiHeight;

		if( !CreateDisplayBuffer() )
		{
			m_iDestBufferSize = 0;
			return;
		}
//TRACE(TEXT("@@@@  StGenImageDisplay 2e \n"));

		m_iDestWidth = uiWidth;
		m_iDestHeight = uiHeight;
		m_iDestColor = iColor;

		//InitScrollBar();	//����������Ȃ��ƕs��H�H�H�X���b�h��������Ƃ��܂���������
//TRACE(TEXT("@@@@@@@@@ StGenImageDisplay ibInitScrollBar .....m_pntCurOffet(%d:%d)...........\n") , m_pntCurOffet.x, m_pntCurOffet.y );
		//�X���b�h���������GUI����ł��Ȃ��Ȃ�B
		//HANDLE aThread = (HANDLE)_beginthreadex( NULL, 0, bInitScrollBar, this,  0, &iID );
		SetTimer( ST_INIT_SCROLLBAR, 10, NULL );		//unsigned int iID;

//TRACE(TEXT("@@@@  StGenImageDisplay 2f \n"));
	}

	m_iDisplayBufferPos = (m_iDisplayBufferPos+1)%DISPLAY_BUFFER_COUNT;
	m_pDispBuffer = m_ppDisplayBuffer[m_iDisplayBufferPos];

	//Bitmap�쐬
	CreateBitmapInfo(0);

	//�摜�ϊ�
	StBuffer dstBuffer;
	dstBuffer.Attach( m_pDispBuffer, m_iDestBufferSize );
	dstBuffer.SetWidth(m_iDestWidth);
	dstBuffer.SetHeight(m_iDestHeight);
	dstBuffer.SetLinePitch(m_iDestLinePitch);
	dstBuffer.SetPixelType(m_iDestColor==0?StPixelMono8:StPixelBGR8);
	//dstBuffer.SetPixelType(StPixelBGR8);


//TRACE(TEXT("@@@@@@@@@ buffer[%d:%d] dstBuffer[%d:%d]\n"),buffer->GetWidth(),buffer->GetHeight(),dstBuffer.GetWidth(),dstBuffer.GetHeight()  );

	StResult aTransformResult = StTransformsImage::Transform( buffer, &dstBuffer);
	dstBuffer.Detach();

	::ReleaseMutex(m_hEImageMutex);

	if( aTransformResult.IsOK() )
	{
		//�\����FPS��}����-------�\����FPS�������Ƃ��܂��\������Ȃ����Ƃ�����B���ׂ���������H---30FPS
		DWORD lCurrent = ::GetTickCount();
		long lDelta = ( lCurrent - m_lPrevious ) - ( 1000 / 15 );

		if( lDelta>0 )
		{
			m_lPrevious = ::GetTickCount();
			//Invalidate(FALSE);
			PaintMain();

		}
	}
	/*
	//---------------------------------------------------------------------
	//���[�U��`�̃R�[���o�b�N�֐��Ăяo��
	//---------------------------------------------------------------------
	::WaitForSingleObject(m_hUserRawCallbackMutex,INFINITE);
	if(m_pRawCallbackFunc != NULL)
	{
		//StBuffer *aBuffer = &srcSaveBuffer[m_iSaveLastIndex];
		StBuffer *aBuffer = buffer;
		m_pRawCallbackFunc( aBuffer, 
			m_pRawCallbackContext,
			NULL);
	}
	::ReleaseMutex(m_hUserRawCallbackMutex);


	//---------------------------------------------------------------------
	//�X�i�b�v�V���b�g���L���ȏꍇ�̓f�[�^���R�s�[����
	//---------------------------------------------------------------------
	::WaitForSingleObject(m_hSnapShotMutex,INFINITE);
	if(m_pvSnapShotBuffer != NULL)
	{
		//StBufferCopy(&srcSaveBuffer[m_iSaveIndex],m_pvSnapShotBuffer);
		StBufferCopy(buffer,m_pvSnapShotBuffer);

		//*m_pvSnapShotBuffer = &srcSaveBuffer[m_iSaveIndex];
		//srcSaveBuffer[m_iSaveIndex].SetSnapShot();

		SetEvent(m_hSnapShotEvent);

		m_pvSnapShotBuffer = NULL;	//���̃t���[���R�s�[���Ȃ��悤��NULL���
//TRACE("E:SnapShotDataCopy\n");
	}
	::ReleaseMutex(m_hSnapShotMutex);
	*/
}

BOOL CPreview::CreateDisplayBuffer()
{
	if( !m_iDestBufferSize ) return FALSE;
	DeleteDisplayBuffer();
	if( !m_ppDisplayBuffer )
	{
		m_ppDisplayBuffer = new char * [DISPLAY_BUFFER_COUNT];
		if( !m_ppDisplayBuffer ) return FALSE;
	}
	if( m_ppDisplayBuffer )
	{
		for( int i=0; i<DISPLAY_BUFFER_COUNT; i++ )
		{
			m_ppDisplayBuffer[i] = new char [m_iDestBufferSize];
			if( !m_ppDisplayBuffer[i] )
			{
				DeleteDisplayBuffer();
				return FALSE;
			}
		}
	}
	m_iDisplayBufferPos = -1;
	return TRUE;
}

void CPreview::DeleteDisplayBuffer()
{
	if( m_ppDisplayBuffer )
	{
		for( int i=0; i<DISPLAY_BUFFER_COUNT; i++ )
		{
			if( m_ppDisplayBuffer[i] )
			{
				delete [] m_ppDisplayBuffer[i];
			}
		}
		delete [] m_ppDisplayBuffer;
		m_ppDisplayBuffer = NULL;
	}
}


//�\����24BIT�Œ�
void CPreview::CreateBitmapInfo( int iDisplayMode )
{

	bool bCreate=false;
	
	unsigned int uiBpp = 8;
	if( m_iDestColor )
	{
		uiBpp *= (m_iDestColor==1?3:4);
	}

	if (m_pBitmapInfo)
    {
		if( -m_pBitmapInfo->bmiHeader.biHeight!=m_iDestHeight ) bCreate=true;
		if( m_pBitmapInfo->bmiHeader.biWidth!=m_iDestWidth ) bCreate=true;
		if( m_pBitmapInfo->bmiHeader.biBitCount!=uiBpp ) bCreate=true;
		if( bCreate==true )
		{
			ReleaseBitmapInfo();
		}
    }else bCreate = true;

	if( !bCreate ) return;
	
    switch (uiBpp)
    {
    case 8:
        m_pBitmapInfo =(BITMAPINFO*) malloc(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));
        m_pBitmapInfo->bmiHeader.biClrImportant = 256;
        m_pBitmapInfo->bmiHeader.biClrUsed = 256;
        for (int i = 0 ; i < 256; i++)
        {
            m_pBitmapInfo->bmiColors[i].rgbBlue = i;
            m_pBitmapInfo->bmiColors[i].rgbGreen = i;
            m_pBitmapInfo->bmiColors[i].rgbRed = i;
        }
        break;
    case 24:
    case 32:
    default:
        //m_pBitmapInfo =(BITMAPINFO*) malloc(sizeof(BITMAPINFOHEADER));
        //m_pBitmapInfo->bmiHeader.biClrImportant = 0;
        //m_pBitmapInfo->bmiHeader.biClrUsed = 0;
        m_pBitmapInfo =(BITMAPINFO*) malloc(sizeof(BITMAPINFO));
		memset(m_pBitmapInfo,0,sizeof(BITMAPINFO));
        m_pBitmapInfo->bmiHeader.biClrImportant = 0;
        m_pBitmapInfo->bmiHeader.biClrUsed = 0;
        break;
    }

    m_pBitmapInfo->bmiHeader.biBitCount = uiBpp;
    m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
    m_pBitmapInfo->bmiHeader.biHeight = 0 - m_iDestHeight;
    m_pBitmapInfo->bmiHeader.biPlanes = 1;
    m_pBitmapInfo->bmiHeader.biSize =  sizeof( BITMAPINFOHEADER );
    m_pBitmapInfo->bmiHeader.biSizeImage = m_iDestBufferSize;
    m_pBitmapInfo->bmiHeader.biWidth  = m_iDestWidth;
    m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
    m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0; 

}

void CPreview::ReleaseBitmapInfo( void )
{
	if( m_pBitmapInfo )
	{
		free(m_pBitmapInfo);
		m_pBitmapInfo = NULL;
	}
}

//-----------------------------------------------------------------------------
//�Î~����擾����
//-----------------------------------------------------------------------------
//BOOL CPreview::GetSnapShotImage(PVOID pvBuffer, DWORD dwMilliseconds)
BOOL CPreview::GetSnapShotImage(StBuffer * pvBuffer, DWORD dwMilliseconds)
{
	DWORD dwReval;

	//TRACE(TEXT("S:GetSnapShotImage(0x%08X)\n"),pvBuffer);
	::WaitForSingleObject(m_hSnapShotMutex,INFINITE);
//{
//TCHAR szMsg[1024];
//swprintf(szMsg,TEXT("++++++++++++++ GetSnapShotImage 1\n") );
//OutputDebugString(szMsg);
//}

	m_pvSnapShotBuffer = pvBuffer;
//{
//TCHAR szMsg[1024];
//swprintf(szMsg,TEXT("++++++++++++++ GetSnapShotImage 2\n") );
//OutputDebugString(szMsg);
//}
	::ResetEvent(m_hSnapShotEvent);
//{
//TCHAR szMsg[1024];
//swprintf(szMsg,TEXT("++++++++++++++ GetSnapShotImage 3\n") );
//OutputDebugString(szMsg);
//}
	::ReleaseMutex(m_hSnapShotMutex);

//{
//TCHAR szMsg[1024];
//SYSTEMTIME st;
//GetSystemTime(&st);
//swprintf(szMsg,TEXT("++++++++++++++ GetSnapShotImage 4 %02d:%02d:%02d:%02d\n"),st.wHour,st.wMinute,st.wSecond,st.wMilliseconds );
//OutputDebugString(szMsg);
//}

	//��2009/12/02 v0.78 Beta09 �摜�擾���̉f���X�V�Ή�
	//dwReval = WaitForSingleObject(m_hSnapShotEvent,dwMilliseconds);
#ifdef ENABLE_WAITTING_REFRESH
	do
	{
		dwReval = MsgWaitForMultipleObjects(1, &m_hSnapShotEvent, FALSE, dwMilliseconds, QS_PAINT);
		if(dwReval == (WAIT_OBJECT_0 + 1))
		{
			MSG msg;
			if(PeekMessage(&msg, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
			{
				DispatchMessage(&msg);
			}
		}
	}while(dwReval == (WAIT_OBJECT_0 + 1));
#else	//ENABLE_WAITTING_REFRESH
	dwReval = ::WaitForSingleObject(m_hSnapShotEvent,dwMilliseconds);
#endif //ENABLE_WAITTING_REFRESH
	//��2009/12/02 v0.78 Beta09 �摜�擾���̉f���X�V�Ή�
//{
//TCHAR szMsg[1024];
//SYSTEMTIME st;
//GetSystemTime(&st);
//swprintf(szMsg,TEXT("++++++++++++++ GetSnapShotImage 5 dwReval=0x%x WAIT_OBJECT_0=0x%x  %02d:%02d:%02d:%02d\n"), dwReval, WAIT_OBJECT_0,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds );
//OutputDebugString(szMsg);
//}

	//WaitForSingleObject(m_hSnapShotMutex,INFINITE);
	//m_pvSnapShotBuffer = NULL;
	//ReleaseMutex(m_hSnapShotMutex);	

	if(dwReval == WAIT_OBJECT_0)
	{
		return(TRUE);
	}
	else
	{
		::WaitForSingleObject(m_hSnapShotMutex,INFINITE);
		m_pvSnapShotBuffer = NULL;	//�G���[���ɂ͕K���K�v
		::ReleaseMutex(m_hSnapShotMutex);	
		return(FALSE);
	}

//TRACE(TEXT("E:GetSnapShotImage\n"));
}

BOOL CPreview::ImageCenterScroll(void)
{
//TRACE(TEXT("@@@@@@@@@ ImageCenterScroll WaitForSingleObject IN\n") );
	::WaitForSingleObject(m_hScrollbarMutex,INFINITE);
//TRACE(TEXT("@@@@@@@@@ ImageCenterScroll WaitForSingleObject Out\n") );

	InitScrollBar();
	Invalidate(TRUE);

	SCROLLINFO	sScrollInfo;
	INT		iMaxPos;
	//Horizontal
	int nBar = SB_HORZ;
	memset(&sScrollInfo, 0, sizeof(SCROLLINFO));
	sScrollInfo.cbSize = sizeof(SCROLLINFO);
	sScrollInfo.fMask = SIF_ALL;
	GetScrollInfo(nBar, &sScrollInfo);
	sScrollInfo.fMask = SIF_POS;
	iMaxPos = sScrollInfo.nMax - sScrollInfo.nPage + 1;
	m_pntCurOffet.x = iMaxPos/2;
	sScrollInfo.nPos = m_pntCurOffet.x;
	SetScrollInfo(nBar, &sScrollInfo, TRUE);

	SetOffset(m_pntCurOffet.x, m_pntCurOffet.y, TRUE);

//TRACE(TEXT("@@@@@@@@@ iMaxPosX=%d((%d-%d+1))\n"),iMaxPos,sScrollInfo.nMax,sScrollInfo.nPage  );

	//Vertical
	nBar = SB_VERT;
	memset(&sScrollInfo, 0, sizeof(SCROLLINFO));
	sScrollInfo.cbSize = sizeof(SCROLLINFO);
	sScrollInfo.fMask = SIF_ALL;
	GetScrollInfo(nBar, &sScrollInfo);
	sScrollInfo.fMask = SIF_POS;
	iMaxPos = sScrollInfo.nMax - sScrollInfo.nPage + 1;
	m_pntCurOffet.y = iMaxPos/2;
	sScrollInfo.nPos = m_pntCurOffet.y;
	SetScrollInfo(nBar, &sScrollInfo, TRUE);

//TRACE(TEXT("@@@@@@@@@ iMaxPosY=%d((%d-%d+1))\n"),iMaxPos,sScrollInfo.nMax,sScrollInfo.nPage  );

	SetOffset(m_pntCurOffet.x, m_pntCurOffet.y, TRUE);

	::ReleaseMutex(m_hScrollbarMutex);

	return TRUE;

	
}


void CPreview::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	if( ST_INIT_SCROLLBAR==nIDEvent )
	{
		KillTimer(nIDEvent);
		InitScrollBar();
	}
	else
	if( ST_IMAGEDISPLAY==nIDEvent )
	{
		KillTimer(nIDEvent);
		StGenImageDisplayMain(m_dstImgBuffer);
		m_dstImgBuffer = NULL;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CPreview::StGenImageDisplay(StBuffer* buffer)
{
	//SetTimer�̎g�p������

//UINT32 aAllocSize = buffer->GetAllocSize();
//UINT32 aSize = buffer->GetSize();
//TRACE(TEXT("@@@ aAllocSize(%d) aSize(%d)\n"),aAllocSize,aSize);
//StPixelType aType;
//aType = buffer->GetPixelType();
//PBYTE aData = (PBYTE)buffer->GetDataPointer();
//TRACE( _T("@@@ aType=0x%x\n"), aType );
//TRACE( _T("@@@ aData=%d\n"), *(PWORD)aData );

	StGenImageDisplayMain(buffer);
	//m_dstImgBuffer = buffer;
	//SetTimer(ST_IMAGEDISPLAY, 1, NULL);
}

