#pragma once
#define _PLEORA

//#ifdef _PLEORA
//#include <PvBuffer.h>
//#else
#include "../CommonHeader/StBuffer.h"
//#endif


#include "resource.h"
#include <math.h>


// CPreview ダイアログ
//#ifdef _PLEORA
//typedef VOID (_stdcall *fStCamRawCallbackFunc)(PvBuffer *aBuffer, LPVOID lpContext,LPVOID lpReserved); 
//#else
typedef VOID (_stdcall *fStCamRawCallbackFunc)(StBuffer *aBuffer, LPVOID lpContext,LPVOID lpReserved); 
//#endif
typedef VOID (_stdcall *fStCamPreviewGDICallbackFunc)(HDC hDC, DWORD dwWidth, DWORD dwHeight,  DWORD dwFrameNo,LPVOID lpContext,LPVOID lpReserved); 

//#define WM_CL_LBUTTON_DBL_CLICK					(WM_APP + 0x1000)
#define WM_CL_LBUTTON_UP						(WM_APP + 0x1001)


#define IMAGE_SAVE_COUNT 4
#define DISPLAY_BUFFER_COUNT 2

#define ENABLE_WAITTING_REFRESH

//#ifdef _PLEORA
//class StCustomBuffer : public PvBuffer
//#else
class StCustomBuffer : public StBuffer
//#endif
{
public:
	StCustomBuffer(void)
	{
		m_hMutex = CreateMutex(NULL, FALSE, NULL);
		m_bSnapShotUsed = FALSE;
	}
	~StCustomBuffer()
	{
		CloseHandle(m_hMutex);
	}

	DWORD WaitForSingleObject2(DWORD dwMilliSecond)
	{
		return ::WaitForSingleObject(m_hMutex,dwMilliSecond);
	}
	BOOL ReleaseMutex(void)
	{
		return ::ReleaseMutex(m_hMutex);
	}
	BOOL IsSnapShotUsed(void){return m_bSnapShotUsed;}
	void SetSnapShot(void)
	{
		m_bSnapShotUsed=TRUE;
	}
	void ReleaseSnapShot(void)
	{
		m_bSnapShotUsed=FALSE;
	}

private:
	HANDLE m_hMutex;
	BOOL m_bSnapShotUsed;
};



class CPreview : public CDialogEx
{
	DECLARE_DYNAMIC(CPreview)

public:
	CPreview(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CPreview();

	void AdjustWindow(int sizeX, int sizeY);

//#ifdef _PLEORA
//	void StGenImageDisplay(PvBuffer *pBuffer);
//#else
	void StGenImageDisplay(StBuffer *pBuffer);

//#endif

	POINT	m_sMousePoint;		//マウスカーソルの位置
	HWND	m_hGUIWnd;
	HWND	m_hMainGUIWnd;

	BOOL AddRawCallbackFunc(fStCamRawCallbackFunc pfNewFunc,PVOID pContext);
	BOOL AddGDICallbackFunc(fStCamPreviewGDICallbackFunc pfNewFunc,PVOID pContext);

	BOOL GetImageSizeOrMax(PDWORD pdwWidth,PDWORD pdwHeight)
	{
		if( m_iSaveLastIndex<0 )
		{
			if( m_iImageMaxWidth<=0 || m_iImageMaxHeight<=0 )
				return FALSE;
			*pdwWidth = m_iImageMaxWidth;
			*pdwHeight = m_iImageMaxHeight;
			return TRUE;
		}

		StCustomBuffer *aStBuffer = &m_srcSaveBuffer[m_iSaveLastIndex];
		aStBuffer->WaitForSingleObject2(INFINITE);
//#ifdef _PLEORA
//		*pdwWidth = aStBuffer->GetImage()->GetWidth();
//		*pdwHeight = aStBuffer->GetImage()->GetHeight();
//#else
		*pdwWidth = aStBuffer->GetWidth();
		*pdwHeight = aStBuffer->GetHeight();

		//▼1.0.0.1046
		if(*pdwHeight==0 )
		{
			*pdwHeight = aStBuffer->GetDeliveredImageHeight();
		}
		//▲1.0.0.1046

//#endif
		aStBuffer->ReleaseMutex();
		return(TRUE);
	}

	BOOL GetImageSize(PDWORD pdwWidth,PDWORD pdwHeight)
	{
		if( m_iSaveLastIndex<0 ) return FALSE;
		StCustomBuffer *aStBuffer = &m_srcSaveBuffer[m_iSaveLastIndex];
		if( !aStBuffer ) return FALSE; 
		aStBuffer->WaitForSingleObject2(INFINITE);
//#ifdef _PLEORA
//		*pdwWidth = aStBuffer->GetImage()->GetWidth();
//		*pdwHeight = aStBuffer->GetImage()->GetHeight();
//#else
		*pdwWidth = aStBuffer->GetWidth();
		*pdwHeight = aStBuffer->GetHeight();
//#endif
		aStBuffer->ReleaseMutex();
		return(TRUE);
	}
//#ifdef _PLEORA
//	BOOL GetPixelType(PvPixelType *aPixelType)
//#else
	BOOL GetPixelType(StPixelType *aPixelType)
//#endif
	{
		if( m_iSaveLastIndex<0 ) return FALSE;
		StCustomBuffer *aStBuffer = &m_srcSaveBuffer[m_iSaveLastIndex];
		if( !aStBuffer ) return FALSE; 

		aStBuffer->WaitForSingleObject2(INFINITE);
//#ifdef _PLEORA
//		*aPixelType = aStBuffer->GetImage()->GetPixelType();
//#else
		*aPixelType = aStBuffer->GetPixelType();
//#endif
		aStBuffer->ReleaseMutex();
		return(TRUE);
	}
	void ProcScroll(int nBar, UINT nSBCode, UINT nPos);
//#ifdef _PLEORA
//	BOOL GetSnapShotImage(PvBuffer * pvBuffer, DWORD dwMilliseconds);
//#else
	BOOL GetSnapShotImage(StBuffer * pvBuffer, DWORD dwMilliseconds);
//#endif
	BOOL ImageCenterScroll(void);

	void GetImagePoint(CPoint *pPoint)
	{	
		int x = m_sMousePoint.x;
		int y = m_sMousePoint.y;

		//x /= this->mGetBairitsu();
		//y /= this->mGetBairitsu();
		x = (int)(x / this->mGetBairitsu());
		y = (int)(y /  this->mGetBairitsu());

		x += m_pntCurOffet.x;
		y += m_pntCurOffet.y;

		pPoint->x = x;
		pPoint->y = y;
	};

	void InitScrollBar(BOOL bInvalidFlag=TRUE);	//publicにしました。

	//▼1.0.0.1048
	void SetMagnify(INT iMagnify);
	//▲1.0.0.1048

private:
	StBuffer *m_dstImgBuffer;
	void StGenImageDisplayMain(StBuffer* buffer);
	void PaintMain();

protected:
	void GetClientRectIncludeScrollBar(CRect *pRect);
	//▼1.0.0.1048
	//void SetMagnify(INT iMagnify);
	//▲1.0.0.1048
	double mGetBairitsu()
	{
		return(pow(2.0, m_iMagnify));
	}
	INT		m_iMagnify;		//2のn乗倍
public:

	CPoint	m_pntCurOffet;
protected:
	CRect	m_rectClient;
	CRect	m_rectClientSize;


	HANDLE m_hScrollbarMutex;
	BOOL	m_bHScroll;
	BOOL	m_bVScroll;
	BOOL	m_bPreviewWindowSize;
	BOOL	m_bColorPreview;
	BOOL	m_bPixelValueUpdateOnlyMouseMove;


	//void InitScrollBar(void);
	void SetOffset(INT iOffsetX, INT iOffsetY, BOOL bRedraw);
	void InitParam(void);
	void UpdateTitleText(void);

	//EImage制御用
	HANDLE m_hEImageMutex;

	//SnapShot取得用
	HANDLE m_hSnapShotMutex;
	HANDLE m_hSnapShotEvent;
	//PVOID  m_pvSnapShotBuffer;
//#ifdef _PLEORA
//	PvBuffer *  m_pvSnapShotBuffer;
//#else
	StBuffer *  m_pvSnapShotBuffer;
//#endif

	//RAWコールバック用
	HANDLE					m_hUserRawCallbackMutex;
	fStCamRawCallbackFunc	m_pRawCallbackFunc;
	PVOID					m_pRawCallbackContext;
	INT						m_iFrameNo;

	//GDIコールバック用
	HANDLE m_hUserGDICallbackMutex;
	fStCamPreviewGDICallbackFunc	m_pGDICallbackFunc;
	PVOID							m_pGDICallbackContext;

	//表示用バッファ--------------------------------------------
	void InitImages(void);
	void ReleaseImages(void);
	BOOL CreateDisplayBuffer();
	void DeleteDisplayBuffer();
	void CreateBitmapInfo( int iDisplayMode );
	void ReleaseBitmapInfo( void );
	int m_iDestWidth;
	int m_iDestHeight;
	int m_iDestLinePitch;
	int m_iDestBufferSize;
	int m_iDestColor;
	int m_iDisplayBufferPos;
	char **m_ppDisplayBuffer;
	char * m_pDispBuffer;
	BITMAPINFO* m_pBitmapInfo;
	int m_iImageMaxWidth;
	int m_iImageMaxHeight;

	//保存用バッファ--------------------------------------------
//#ifdef _PLEORA
//	void StBufferCopy(PvBuffer* SrcBuffer, StCustomBuffer* DstBuffer);
//	void StBufferCopy(PvBuffer* SrcBuffer, PvBuffer* DstBuffer);
//#else
	void StBufferCopy(StBuffer* SrcBuffer, StCustomBuffer* DstBuffer);
	void StBufferCopy(StBuffer* SrcBuffer, StBuffer* DstBuffer);
//#endif
	StCustomBuffer m_srcSaveBuffer[IMAGE_SAVE_COUNT];
	INT m_iSaveIndex;
	INT m_iSaveLastIndex;

	//
	DWORD m_lPrevious;


// ダイアログ データ
	enum { IDD = IDD_DIALOG_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnMenuPreviewSize(UINT nID);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
