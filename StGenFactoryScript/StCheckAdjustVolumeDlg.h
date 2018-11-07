#pragma once

#include "stcheckbase.h"

// StCheckAdjustVolumeDlg ダイアログ

class StCheckAdjustVolumeDlg : public CDialogEx, public StCheckBase
{
	DECLARE_DYNAMIC(StCheckAdjustVolumeDlg)

public:
	StCheckAdjustVolumeDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	StCheckAdjustVolumeDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx = NULL, CWnd* pParent = NULL);   // コンストラクター
	virtual ~StCheckAdjustVolumeDlg();

	BOOL Execute(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);

	BOOL AdjustExecute(void);

	//検査Thread
	//BOOL GetThreadStop(void){return m_bThreadStop;}


private:
	COLORREF m_color;
	CFont *m_pDefFont;
	CFont m_boldFont;

	void Initialize(void);
	BOOL JudgeText( bool bJudge );

	//検査Thread
	//HANDLE m_hThread;
	//unsigned int m_mThreadID;
	//BOOL m_bThreadStop;

	INT m_nChannelCount; //チャンネル数(ボリューム調整で使用)
	INT m_nLevelDiff;	//Even Oddの明度比較し差分の規格(ボリューム調整で使用)
	INT m_nWidth;	//Even Oddの明度比較するための横幅(ボリューム調整で使用)
	INT m_nCenter;	//４チャンネルの場合の中心位置(ボリューム調整で使用)2チャンネルの場合でも使用します。

	INT *m_pStart;


	double *m_pResultAverage;
	INT m_nResultChannel;
	INT m_nResultElement;
	double m_maxLevelDiff;
	INT m_maxPos[2];
	INT m_maxLevelColor;

	BOOL AdjustJudgeDisplay( void );


public:
// ダイアログ データ
	enum { IDD = IDD_DIALOG_VOLUME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};
