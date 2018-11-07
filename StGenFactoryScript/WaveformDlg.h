#pragma once

#include "../CommonHeader/StBuffer.h"

// CWaveformDlg ダイアログ

class CWaveformDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWaveformDlg)

public:
	CWaveformDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CWaveformDlg();

	void AdjustWindow( int sizeX, int sizeY );
	void SetImageSize( INT sizeX, INT sizeY );

	INT GetDispSizeX(){return m_nDispSizeX;}
	INT GetDispSizeY(){return m_nDispSizeY;}

	void StWaveformDisplay(StBuffer* buffer);
	
	//▼1.0.0.1047
	void SetRuledLine(INT nRuledLine){m_nRuledLine = nRuledLine;}
	//▲1.0.0.1047

private:
	size_t m_SizeBuffer;
	PINT m_pGraphBuffer;
	INT m_PixelBits;

	//画像サイズ
	INT m_nImageMaxWidth;
	INT m_nImageMaxHeight;

	//
	INT m_nCurHeight;
	INT m_nCurWidth;

	INT m_nElement;


	//画面サイズ
	INT m_nDispSizeX;
	INT m_nDispSizeY;

	//グラフ部表示サイズ
	INT m_drawSizeX;
	INT m_drawSizeY;
	
	//▼1.0.0.1047
	//グラフ部表示サイズ罫線間隔
	INT m_nRuledLine;
	//▲1.0.0.1047

	DWORD m_lPrevious;

	HANDLE m_hMutex;

	//値表示
	INT GetImagePosX(INT posX);

	//
	void StWaveformDisplayMain(StBuffer* buffer);
	StBuffer* m_dstFrmBuffer;

	void PaintMain();


public:
// ダイアログ データ
	enum { IDD = IDD_DIALOG_WAVEFORM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
