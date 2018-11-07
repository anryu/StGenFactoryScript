#pragma once

#include "../CommonHeader/StBuffer.h"

// CWaveformDlg �_�C�A���O

class CWaveformDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWaveformDlg)

public:
	CWaveformDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CWaveformDlg();

	void AdjustWindow( int sizeX, int sizeY );
	void SetImageSize( INT sizeX, INT sizeY );

	INT GetDispSizeX(){return m_nDispSizeX;}
	INT GetDispSizeY(){return m_nDispSizeY;}

	void StWaveformDisplay(StBuffer* buffer);
	
	//��1.0.0.1047
	void SetRuledLine(INT nRuledLine){m_nRuledLine = nRuledLine;}
	//��1.0.0.1047

private:
	size_t m_SizeBuffer;
	PINT m_pGraphBuffer;
	INT m_PixelBits;

	//�摜�T�C�Y
	INT m_nImageMaxWidth;
	INT m_nImageMaxHeight;

	//
	INT m_nCurHeight;
	INT m_nCurWidth;

	INT m_nElement;


	//��ʃT�C�Y
	INT m_nDispSizeX;
	INT m_nDispSizeY;

	//�O���t���\���T�C�Y
	INT m_drawSizeX;
	INT m_drawSizeY;
	
	//��1.0.0.1047
	//�O���t���\���T�C�Y�r���Ԋu
	INT m_nRuledLine;
	//��1.0.0.1047

	DWORD m_lPrevious;

	HANDLE m_hMutex;

	//�l�\��
	INT GetImagePosX(INT posX);

	//
	void StWaveformDisplayMain(StBuffer* buffer);
	StBuffer* m_dstFrmBuffer;

	void PaintMain();


public:
// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_WAVEFORM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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
