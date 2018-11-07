#pragma once

//▼1.0.0.1063
//#include "stcheckbase.h"
#include "StCheckDataLose.h"
//▲1.0.0.1063

// StCheckDataLoseDlg ダイアログ
//▼1.0.0.1063
//class StCheckDataLoseDlg : public CDialogEx, public StCheckBase
class StCheckDataLoseDlg : public CDialogEx, public StCheckDataLose
//▲1.0.0.1063
{
	DECLARE_DYNAMIC(StCheckDataLoseDlg)

public:
	StCheckDataLoseDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	StCheckDataLoseDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx = NULL, CWnd* pParent = NULL);   // コンストラクター
	virtual ~StCheckDataLoseDlg();

	BOOL Execute(void){return TRUE;}

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	//▼1.0.0.1063
	//BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg,double &dblValue);

	//---パラメータ設定-------------------------------
	//void SetStart(INT nData);
	//void SetEnd(INT nData);
	//void SetCSVFileName(LPCTSTR szData);
	//void SetBMPFileName(LPCTSTR szData);

	////---規格設定-------------------------------
	//void SetDifferenceMax(double dblData);
	//▲1.0.0.1063

	void SetMode(INT nData);

	//---結果取得-------------------------------
	//▼1.0.0.1063
	//INT GetResultElement(void){return m_iResultElement;}
	//▲1.0.0.1063
	double GetResultDifference(int nIndex);
	INT GetResultHighLow(int nIndex);
	INT GetResultLine(int nIndex);

	//検査Thread
	BOOL GetThreadStop(void){return m_bThreadStop;}
	BOOL CheckExecute(void);
	HANDLE m_hMessageMutex;
	//▼1.0.0.1063
	//void SetSaveFilePath(LPCTSTR szText){mSaveFilePath = szText;}

private:
	void Initialize(void);
	//▲1.0.0.1063

private:
	//▼1.0.0.1063
	//void Initialize(void);
	//▲1.0.0.1063
	COLORREF m_Color;
	CFont *m_pDefFont;
	CFont m_boldFont;

	//表示
	BOOL JudgeText(BOOL bJudge);
	BOOL ResultText(void);
	BOOL SetLimitText(void);

	//検査Thread
	HANDLE m_hThread;
	unsigned int m_mThreadID;
	BOOL m_bThreadStop;
	//▼1.0.0.1063
	//パラメータ
	//INT m_nStart;
	//INT m_nEnd;
	//double m_dblDifferenceMax;
	//▲1.0.0.1063
	INT m_nMode;

	//結果
	//▼1.0.0.1063
	//INT m_iResultElement;
	//double *m_pdblResultDifference;
	//INT *m_piResultHighLow;	//0:低いほう 1:高いほう
	//INT *m_piResultLine;
	//▲1.0.0.1063

	BOOL m_bResultNG;
	double *m_pdblResultNGDifference;
	INT *m_piResultNGHighLow;	//0:低いほう 1:高いほう
	INT *m_piResultNGLine;

	//CString mSaveCSVFilePath;
	//CString mSaveBMPFilePath;
	CString m_szImageFileName;
	//▼1.0.0.1063
	//CString mSaveFilePath;
	//▲1.0.0.1063

	StBuffer mNGImageBuffer;

	BOOL GetDataLose(void);

public:
// ダイアログ データ
	enum { IDD = IDD_DIALOG_CHECKDATALOSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};
