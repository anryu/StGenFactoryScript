#pragma once

#include "stcheckbase.h"

// StCheckAdjustVolumeDlg �_�C�A���O

class StCheckAdjustVolumeDlg : public CDialogEx, public StCheckBase
{
	DECLARE_DYNAMIC(StCheckAdjustVolumeDlg)

public:
	StCheckAdjustVolumeDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	StCheckAdjustVolumeDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx = NULL, CWnd* pParent = NULL);   // �R���X�g���N�^�[
	virtual ~StCheckAdjustVolumeDlg();

	BOOL Execute(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);

	BOOL AdjustExecute(void);

	//����Thread
	//BOOL GetThreadStop(void){return m_bThreadStop;}


private:
	COLORREF m_color;
	CFont *m_pDefFont;
	CFont m_boldFont;

	void Initialize(void);
	BOOL JudgeText( bool bJudge );

	//����Thread
	//HANDLE m_hThread;
	//unsigned int m_mThreadID;
	//BOOL m_bThreadStop;

	INT m_nChannelCount; //�`�����l����(�{�����[�������Ŏg�p)
	INT m_nLevelDiff;	//Even Odd�̖��x��r�������̋K�i(�{�����[�������Ŏg�p)
	INT m_nWidth;	//Even Odd�̖��x��r���邽�߂̉���(�{�����[�������Ŏg�p)
	INT m_nCenter;	//�S�`�����l���̏ꍇ�̒��S�ʒu(�{�����[�������Ŏg�p)2�`�����l���̏ꍇ�ł��g�p���܂��B

	INT *m_pStart;


	double *m_pResultAverage;
	INT m_nResultChannel;
	INT m_nResultElement;
	double m_maxLevelDiff;
	INT m_maxPos[2];
	INT m_maxLevelColor;

	BOOL AdjustJudgeDisplay( void );


public:
// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_VOLUME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};
