// WaveformDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "WaveformDlg.h"
#include "afxdialogex.h"
#include "StBufferInfo.h"

#define START_X 40
#define START_Y 16
#define END_X 16
#define END_Y 30

// CWaveformDlg ダイアログ
#define ST_WAVEFORMDISPLAY 102

//▼1.0.0.1067
//extern void DoEvents();
//▲1.0.0.1067


IMPLEMENT_DYNAMIC(CWaveformDlg, CDialogEx)

CWaveformDlg::CWaveformDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWaveformDlg::IDD, pParent)
	, m_lPrevious(0)
{
	m_SizeBuffer = 0;
	m_pGraphBuffer = NULL;
	m_PixelBits = 0;

	m_nImageMaxWidth = 0;
	m_nImageMaxHeight = 0;

	m_nCurHeight = 0;
	m_nCurWidth = 0;

	m_hMutex = CreateMutex(NULL, FALSE, NULL);

	m_dstFrmBuffer = NULL;

	m_nElement = 1;

	//▼1.0.0.1047
	m_nRuledLine = 200;
	//▲1.0.0.1047

}

CWaveformDlg::~CWaveformDlg()
{
	::WaitForSingleObject(m_hMutex,INFINITE);  //<-これがないとフリーズする。

	if( m_pGraphBuffer )
		delete [] m_pGraphBuffer;
	m_pGraphBuffer = NULL;

	::ReleaseMutex(m_hMutex);

	if( m_hMutex )
		//▼1.0.0.1047
		//ReleaseMutex(m_hMutex);
		CloseHandle(m_hMutex);
		//▲1.0.0.1047
	m_hMutex = NULL;
}

void CWaveformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaveformDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CWaveformDlg メッセージ ハンドラー


BOOL CWaveformDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	//最大横サイズ×3
	m_SizeBuffer = m_nImageMaxWidth * 3;
	if( m_nImageMaxWidth>0 )
	{
		if( m_pGraphBuffer ) delete [] m_pGraphBuffer;
		m_pGraphBuffer = new INT [m_SizeBuffer];
	}
	m_nDispSizeX = max( m_nImageMaxWidth, m_nImageMaxHeight )+START_X+END_X;
	m_nDispSizeY = 256+START_Y+END_Y;

	/*

	//m_dwGraphLine = m_iImageSizeY/2;	//画像の中心
	m_pGraphBuffer = (PBYTE)malloc( m_sizeBuffer );

	m_dwWidthAfterRotation=m_iImageSizeX;
	m_dwHeightAfterRotation=m_iImageSizeY;


	m_iDispSizeX = max( m_iImageSizeX, m_iImageSizeY )+START_X+END_X;
//	m_iDispSizeX = m_dwWidthAfterRotation+START_X+END_X;
	m_iDispSizeY = 256+START_Y+END_Y;


	*/


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CWaveformDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	//if( m_pGraphBuffer )
	//	free( m_pGraphBuffer );
	//m_pGraphBuffer = NULL;

	//INT data=1;
	//CStCamGigEWareApp& lApp = *reinterpret_cast<CStCamGigEWareApp*>( AfxGetApp() );
	//lApp.m_pMainWnd->SendMessage(WM_APP_SENDDATA, 0, (LPARAM)&data);


}

void CWaveformDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 描画メッセージで CDialogEx::OnPaint() を呼び出さないでください。
	PaintMain();

}

void CWaveformDlg::PaintMain()
{
	//CPaintDC dc(this); // device context for painting

	CClientDC dc(this);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 描画メッセージで CDialogEx::OnPaint() を呼び出さないでください。
	if( m_PixelBits==0 ) return;

	//グラフ表示
	int iScrollX = 0;
	int iScrollY = 0;
	
    // クライアント領域のサイズを取得する
    CRect rc;
    GetClientRect(&rc);

	CBrush dBrush, *pOldBrush=0;
	CPen dPen, *pOldPen=0;  // Construct it, then initialize
	COLORREF COL_PEN[] = { RGB(0,0,255), RGB(0,255,0), RGB(255,0,0), RGB(255,255,255), RGB(0,0,0), RGB(64,64,64) };
	CRect dRect, wRect;

	int iStartY = START_Y;
	int iStartX = START_X;
	int iEndY = END_Y;
	int iEndX = END_X;



	INT nEffectiveValueY = 1<<m_PixelBits;

	//	グラフの表示
	//int iHeight = 256 - 1;
	int iHeight = nEffectiveValueY - 1;
	int iWidth = m_nImageMaxWidth - 1;

	INT iDispSizeX = rc.Width();
	INT iDispSizeY = rc.Height();

	if( iDispSizeX<START_X+END_X+1){
		iDispSizeX = START_X+END_X+1;
	}
	double dblXStretch = (double)(iDispSizeX-START_X-END_X)/m_nImageMaxWidth;
	if( iDispSizeY<START_Y+END_Y+1)
	{
		iDispSizeY = START_Y+END_Y+1;
	}
	//double dblYStretch = (double)(iDispSizeY-START_Y-END_Y)/256.0;
	double dblYStretch = (double)(iDispSizeY-START_Y-END_Y)/nEffectiveValueY;

//	iDispSizeX = (int)(m_dwWidthAfterRotation*dblXStretch)+START_X+END_X;
//	iDispSizeY = (int)(256*dblYStretch)+START_Y+END_Y;
	
//	if( iDispSizeX!=m_iDispSizeX || iDispSizeY!=m_iDispSizeY ){
//		SetSize();
//	}


	iHeight = (int)(iHeight*dblYStretch);
	iWidth = (int)(iWidth*dblXStretch);

	//枠表示
	dRect.left = iStartX - iScrollX;
	dRect.right = dRect.left + iWidth;
	dRect.top = iStartY - iScrollY;
	dRect.bottom = dRect.top + iHeight;
	wRect.left = dRect.left - iStartX;
	wRect.right = wRect.left + m_nDispSizeX - 1;
	wRect.top = dRect.top - iStartY;
	wRect.bottom = wRect.top + m_nDispSizeY - 1;


   // 裏画面用の DC を作成する
    CDC dcMem;
    dcMem.CreateCompatibleDC(&dc);

    // 裏画面作成用のビットマップを用意する
	CBitmap bmp;
	int iRcWidth = rc.Width();
	int iRcHeight = rc.Height();

	bmp.CreateCompatibleBitmap(&dc, iRcWidth, iRcHeight);
	CBitmap* pOldBmp = dcMem.SelectObject(&bmp);

	dBrush.CreateSolidBrush( COL_PEN[3] );
	pOldBrush = (CBrush*)dcMem.SelectObject( &dBrush );

	//白背景
	dPen.CreatePen( PS_SOLID, 0, COL_PEN[3] );
	pOldPen	= (CPen*)dcMem.SelectObject( &dPen );
	dcMem.Rectangle( &rc );		//枠も書いている
	dPen.DeleteObject();

	//クロ枠
	dPen.CreatePen( PS_SOLID, 2, COL_PEN[4] );
	dcMem.SelectObject( &dPen );
	CRect Waku(dRect);
	Waku.left -=1;
	Waku.bottom +=1;
	dcMem.Rectangle( &Waku );
	dPen.DeleteObject();

	//Grid
	dPen.CreatePen( PS_DASHDOT, 1, COL_PEN[4] );
	dcMem.SelectObject( &dPen );

	//数値表示 
	int iNumY[5];
	int ratio=1;
	//if( m_dwBitsPerPixel==STCAM_TRANSER_BITS_PER_PIXEL_10 )
	//	ratio=4;
	//else if( m_dwBitsPerPixel==STCAM_TRANSER_BITS_PER_PIXEL_12 )
	//	ratio=16;
	for( int i=0; i<5; i++ )
	{
		//iNumY[i] = max( 0, 64*ratio*i-1 );
		iNumY[i] = max( 0, nEffectiveValueY*i/4-1 );
	}
	//int iLineX[5] = {  0, iWidth/4,  iWidth/2, iWidth*3/4, iWidth };
	int iLineY[5] = {  0, iHeight/4,  iHeight/2, iHeight*3/4, iHeight };
	TCHAR szText[MAX_PATH];
	for( int xy=0; xy<5; xy++ ){
		_stprintf( szText, TEXT("%d"), iNumY[xy] );
		int len = (int)wcslen( szText );
		//左  文字列
		wRect.right = dRect.left-4;
		wRect.left = wRect.right-48;
		wRect.top = dRect.bottom-iLineY[xy]-8;
		wRect.bottom = wRect.top + 16;
		dcMem.DrawText( szText, len, &wRect, DT_RIGHT );
		//横 罫線
		if( xy!=0 && xy!=4 ){
			dcMem.MoveTo( dRect.left, dRect.bottom-iLineY[xy] );
			dcMem.LineTo( dRect.right, dRect.bottom-iLineY[xy] );
		}
	}

	int iPos=0;
	int iXPos=0;
	do{
		_stprintf( szText, TEXT("%d"), iPos );
		int len = (int)wcslen( szText );
		//下文字列
		wRect.left = dRect.left + iXPos - 60/2;
		wRect.right = wRect.left + 60;
		wRect.top = dRect.bottom + 1;
		wRect.bottom = wRect.top + 32;
		dcMem.DrawText( szText, len, &wRect, DT_CENTER );
		//縦 罫線
		if( iPos!=0 && iXPos!=iWidth ){
			dcMem.MoveTo( iXPos+dRect.left, dRect.top );
			dcMem.LineTo( iXPos+dRect.left, dRect.bottom );
		}
		//▼1.0.0.1047
		//iPos+=200;
		iPos += m_nRuledLine;
		//▲1.0.0.1047
		iXPos = (int)(iPos*dblXStretch);
	}while(iXPos<=iWidth);
	dPen.DeleteObject();


	//PSTCAM_COMMON_PARAM	psCommonParametor = (PSTCAM_COMMON_PARAM)m_hCamera;
	//WaitForSingleObject(psCommonParametor->stDisplayWindowFeature.stPreviewBuffer.hGraphMutex,INFINITE);
	//if( !m_hCamera ){
	//	goto _EXIT;
	//}

	::WaitForSingleObject(m_hMutex,INFINITE);


	if( !m_pGraphBuffer )
	{
		goto _EXIT;
	}

	//グラフ表示
	BOOL bColor = FALSE;
	if( m_nElement==3 ) bColor = TRUE;

	//Graphデータは８ビット
	int colorNum = 1;
	if( bColor )
	{
		colorNum = 3;
	}
	COLORREF ColPenGraph;

	WORD wInData=0;
	//WORD wMask=0xff;
	WORD wMask=nEffectiveValueY-1;
	int bytePerPixel = 1;
	WORD wPer = (wMask>>8)+1;
	for( int zz=0; zz<colorNum; zz++ ){
		if( m_pGraphBuffer ){
			ColPenGraph = COL_PEN[zz];
			if( !bColor ) ColPenGraph = COL_PEN[5];
		    if( dPen.CreatePen( PS_SOLID, 1, ColPenGraph ) )
			{
				dcMem.SelectObject( &dPen );

				PINT pInByte = m_pGraphBuffer + zz * bytePerPixel;

				//wInData =  ((*(PWORD)pInByte)&wMask)/wPer;
				wInData =  ((*(PWORD)pInByte)&wMask);
				wInData = (WORD)(wInData*dblYStretch);
				dcMem.MoveTo( dRect.left, dRect.bottom-wInData );
				for( int i=1; i<m_nImageMaxWidth; i++ ){
					pInByte = m_pGraphBuffer + zz * bytePerPixel + i*colorNum*bytePerPixel;

					//wInData =  ((*(PWORD)pInByte)&wMask)/wPer;
					wInData =  ((*(PWORD)pInByte)&wMask);
////描画更新確認のため。。。。。。。。。。
//{
//static int iCount=0;
//wInData = ((i+iCount)&0xff)/wPer;
//iCount++;
//}
					wInData = (WORD)(wInData*dblYStretch);

					dcMem.LineTo( dRect.left+(int)(i*dblXStretch), dRect.bottom-wInData );
				}
				dPen.DeleteObject();
			}
		}
	}

	dcMem.SelectObject( pOldBrush );
	dBrush.DeleteObject();

_EXIT:
	//ReleaseMutex(psCommonParametor->stDisplayWindowFeature.stPreviewBuffer.hGraphMutex);

	dcMem.SelectObject( pOldPen );

    // 裏画面(dcMem)から表画面(dc)に一気に画像を転送する
    dc.BitBlt(0, 0, iRcWidth, iRcHeight, &dcMem, 0, 0, SRCCOPY);

	dcMem.SelectObject(pOldBmp);
	dcMem.DeleteDC();
	bmp.DeleteObject();

	::ReleaseMutex(m_hMutex);

	//▼1.0.0.1067
//	DoEvents();
	//▲1.0.0.1067

}

void CWaveformDlg::AdjustWindow( int sizeX, int sizeY )
{
	//ウィンドウサイズを画像サイズに合わせる
	//CRect rectImage = CRect(0,0,sizeX,sizeY);
	//DWORD dwStyle = GetWindowLong(m_hWnd,GWL_STYLE);
	//DWORD dwExStyle = GetWindowLong(m_hWnd,GWL_EXSTYLE);
	//AdjustWindowRectEx(&rectImage,dwStyle,FALSE,dwExStyle);

	////これを有効にすると画像取り込みのバッファ確保できなくてエラー
	////m_iDestWidth = sizeX;
	////m_iDestHeight = sizeY;
	//m_nImageMaxWidth = sizeX;
	//m_nImageMaxHeight = sizeY;

	////デスクトップサイズ取得
	//CRect	rectDesktop;
	//SystemParametersInfo(SPI_GETWORKAREA, 0, &rectDesktop, 0);
	//if(rectDesktop.Width() < rectImage.Width())
	//{
	//	rectImage.left = rectDesktop.left;
	//	rectImage.right = rectDesktop.right;
	//}
	//if(rectDesktop.Height() < rectImage.Height())
	//{
	//	rectImage.top = rectDesktop.top;
	//	rectImage.bottom = rectDesktop.bottom;
	//}

	////プレビューウィンドウを画面内に収めるモードを追加
	//SetWindowPos(NULL,0,0,rectImage.Width(),rectImage.Height(),SWP_NOACTIVATE | SWP_NOMOVE);

	////強制的にWM_SIZEを送り出してサイズ調整
	//SendMessage(WM_SIZE, 0, MAKELPARAM(rectImage.Width(), rectImage.Height()));

}



void CWaveformDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_nCurHeight = cy;
	m_nCurWidth = cx;

}

void CWaveformDlg::SetImageSize( INT sizeX, INT sizeY )
{
	m_nImageMaxWidth = sizeX;
	m_nImageMaxHeight = sizeY;

	m_drawSizeX = m_nImageMaxWidth;
	m_drawSizeY = 256;

	m_nDispSizeX = m_drawSizeX + START_X + END_X;
	m_nDispSizeY = m_drawSizeY + START_Y + END_Y;
}





template <class X> BOOL GetColumnAverage( X *ptRaw, WORD wElementsPerPixel, DWORD dwWidth, DWORD dwHeight, DWORD nSrcLinePitch
			, INT *pOutAverage, size_t bufferSize )
{

	if( dwWidth*wElementsPerPixel>bufferSize ) return FALSE;
	if( dwHeight==0 ) return FALSE;

	memset( pOutAverage, 0, sizeof(INT)*bufferSize );

	DWORD dwLine = dwWidth * wElementsPerPixel;

	X *pImgTop = ptRaw;
	for( DWORD y=0; y<dwHeight; y++ )
	{
		X *pImg = pImgTop;
		for( DWORD x=0; x<dwLine; x++ )
		{
			pOutAverage[x] += (INT)(*pImg++);
		}
		PBYTE pbImg =  (PBYTE)pImgTop;
		pImgTop = (X *)(pbImg + nSrcLinePitch);
	}

	for( DWORD x=0; x<dwLine; x++ )
	{
		pOutAverage[x] /= dwHeight;
	}
	return TRUE;
}




void CWaveformDlg::StWaveformDisplayMain(StBuffer* buffer)
{
	//return;


    if (!buffer) return;

	if( !m_pGraphBuffer )
	{
		Sleep(1);
		return;
	}

	//StPixelType aType = buffer->GetPixelType();
	unsigned int uiID = buffer->GetID();

//TRACE(TEXT("@@@@@@@@@ GetID=%i\n"),uiID );


	unsigned int uiWidth = buffer->GetWidth();
	unsigned int uiHeight = buffer->GetHeight();
	//▼1.0.0.1046
	if( uiHeight==0 )
	{
		uiHeight = buffer->GetDeliveredImageHeight();
	}
//TRACE( _T("@@@ buffer=0x%08x uiID=%i,uiHeight=%i\n"), buffer, uiID,uiHeight );
	if( uiHeight==0 )
	{
		return;
	}
	//▲1.0.0.1046
	if( uiWidth>m_nImageMaxWidth ) return;

//TRACE( _T("@@@ GetLinePitch=%i(0x%x)\n"), buffer->GetLinePitch(), buffer->GetLinePitch() );


	//映像保存---------------------------------------------------------------------------
	StBufferInfo aBufferInfo(buffer);
	INT nElement = aBufferInfo.GetElement();
	m_nElement = nElement;

	INT nPixelSize = aBufferInfo.GetPixelSize();

	if( (nPixelSize%(nElement*8))!=0 ) return;
	INT nBytePerElement = nPixelSize/(nElement*8);

	//WaitForSingleObject(m_hEImageMutex,INFINITE);

	::WaitForSingleObject(m_hMutex,INFINITE);
//TRACE(TEXT("@@@@@@@@@ m_pGraphBuffer=0x%x m_SizeBuffer=%d\n"), m_pGraphBuffer, m_SizeBuffer );
	if( nBytePerElement==1 )
	{
		GetColumnAverage( (unsigned char *)buffer->GetDataPointer(), nElement, uiWidth, uiHeight, buffer->GetLinePitch()
					, m_pGraphBuffer, m_SizeBuffer );
	}
	else
	if( nBytePerElement==2 )
	{
		GetColumnAverage( (unsigned short *)buffer->GetDataPointer(), nElement, uiWidth, uiHeight, buffer->GetLinePitch()
					, m_pGraphBuffer, m_SizeBuffer );
	}

	m_PixelBits = aBufferInfo.GetPixelBits();
	::ReleaseMutex(m_hMutex);


	//-----------------------------------------------------------------------
	/*
	//表示Buffer確保--------------------------------------------------------------------------------
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
			
		//４の倍数に切り上げ
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

		//InitScrollBar();	//これをおこなうと不具合？？？スレッド化させるとうまくいくかも
//TRACE(TEXT("@@@@@@@@@ StGenImageDisplay ibInitScrollBar .....m_pntCurOffet(%d:%d)...........\n") , m_pntCurOffet.x, m_pntCurOffet.y );
		//スレッド化させるとGUI操作できなくなる。
		//HANDLE aThread = (HANDLE)_beginthreadex( NULL, 0, bInitScrollBar, this,  0, &iID );
		SetTimer( ST_INIT_SCROLLBAR, 10, NULL );		//unsigned int iID;

//TRACE(TEXT("@@@@  StGenImageDisplay 2f \n"));
	}

	m_iDisplayBufferPos = (m_iDisplayBufferPos+1)%DISPLAY_BUFFER_COUNT;
	m_pDispBuffer = m_ppDisplayBuffer[m_iDisplayBufferPos];

	//Bitmap作成
	CreateBitmapInfo(0);

	//画像変換
	StBuffer dstBuffer;
	dstBuffer.Attach( m_pDispBuffer, m_iDestBufferSize );
	dstBuffer.SetWidth(m_iDestWidth);
	dstBuffer.SetHeight(m_iDestHeight);
	dstBuffer.SetLinePitch(m_iDestLinePitch);
	dstBuffer.SetPixelType(m_iDestColor==0?StPixelMono8:StPixelBGR8);
	//dstBuffer.SetPixelType(StPixelBGR8);
	StResult aTransformResult = StTransformsImage::Transform( buffer, &dstBuffer);
	dstBuffer.Detach();
	*/
	//ReleaseMutex(m_hEImageMutex);

	//if( aTransformResult.IsOK() )
	{
		//表示のFPSを抑える-------表示のFPSが高いとうまく表示されないことがある。負荷が高いから？---30FPS
		DWORD lCurrent = ::GetTickCount();
		long lDelta = ( lCurrent - m_lPrevious ) - ( 1000 / 15 );

		if( lDelta>0 )
		{
			m_lPrevious = ::GetTickCount();

//			Invalidate(FALSE);

PaintMain();

		}
	}


}

INT CWaveformDlg::GetImagePosX(INT posX)
{
	INT nOutPosX  = -1;
	if( m_nImageMaxWidth<=0 ) return nOutPosX;

    CRect rc;
    GetClientRect(&rc);

	//if( posX<START_X || posX>rc.Width()-END_X )  return nOutPosX;

	INT iDispSizeX = rc.Width();
	if( iDispSizeX<START_X+END_X+1)
	{
		iDispSizeX = START_X+END_X+1;
	}
	double dblXStretch = (double)(iDispSizeX-START_X-END_X)/m_nImageMaxWidth;

	nOutPosX = posX - START_X;
	nOutPosX = (INT)(nOutPosX/dblXStretch);

	if( nOutPosX<0 ) nOutPosX = 0;
	else if( m_nImageMaxWidth<=nOutPosX ) nOutPosX = m_nImageMaxWidth-1;

	return nOutPosX;
}


void CWaveformDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	::WaitForSingleObject(m_hMutex,INFINITE);

	INT nImagePos = GetImagePosX(point.x);


	//if( m_pGraphBuffer && nImagePos<m_SizeBuffer )
	if( m_pGraphBuffer && nImagePos<m_SizeBuffer && nImagePos>=0)
	{
		INT nAverage = m_pGraphBuffer[nImagePos];
		CString szTitle;
		szTitle.Format(_T("Waveform PosX[%d] Average[%d]"), nImagePos, nAverage);
		SetWindowText(szTitle);

	}

	::ReleaseMutex(m_hMutex);

//{
//static int nCount=0;
//CString szString;
//szString.Format(_T("OnLButtonUp[%d] (%d:%d)"), nCount++, point.x, point.y);
//OutputDebugString(szString);
//}

	CDialogEx::OnLButtonUp(nFlags, point);


}

void CWaveformDlg::StWaveformDisplay(StBuffer* buffer)
{
	//SetTimerの使用をヤメ
	StWaveformDisplayMain(buffer);
	//m_dstFrmBuffer = buffer;
	//SetTimer( ST_WAVEFORMDISPLAY, 1, NULL );
}

void CWaveformDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	if( ST_WAVEFORMDISPLAY==nIDEvent )
	{
		KillTimer(nIDEvent);
		StWaveformDisplayMain(m_dstFrmBuffer);
		m_dstFrmBuffer = NULL;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CWaveformDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	CDialogEx::OnLButtonDown(nFlags, point);
}
