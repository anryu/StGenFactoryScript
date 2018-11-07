#pragma once

#include "stcheckbase.h"

// StCheckDustDlg ダイアログ

//必ず先にCDialogExの継承記述をする必要あり
class StCheckDustDlg : public CDialogEx, public StCheckBase
{
	DECLARE_DYNAMIC(StCheckDustDlg)

public:
	StCheckDustDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	StCheckDustDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx = NULL, CWnd* pParent = NULL);   // コンストラクター
	virtual ~StCheckDustDlg();

	//▼1.0.0.1026
	//BOOL Execute(void){return TRUE;}
	BOOL Execute(void);
	//▲1.0.0.1026
	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	//▼1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);
	//▲1.0.0.1025

	//---パラメータ設定-------------------------------
	void SetStart(INT nData);
	void SetEnd(INT nData);
	void SetWidthLines(INT nData);
	void SetSeparate(INT nData);
	void SetBMPFileName(LPCTSTR szData);
	//▼1.0.0.1039
	void SetCSVFileName(LPCTSTR szData);
	void SetPNGFileName(LPCTSTR szData);
	//▲1.0.0.1039
	//▼1.0.0.1041
	void SetCheckMode(INT nData);
	//▲1.0.0.1041

	//規格値-----
	void SetDifferenceMax(double dblData);
	void SetAverageLowLevel(double dblData);
	void SetAverageHighLevel(double dblData);
	//▼1.0.0.1043
	void SetContinuousCountMax(INT nData){m_nContinuousCountMax = nData;}
	//▲1.0.0.1043

	//---結果取得-------------------------------
	INT GetResultElement(void);
	double GetAverage(void);
	double GetResultDifference(int nIndex);
	INT GetResultLine(int nIndex);
	//▼1.0.0.1024
	double GetResultAverage(int nIndex);
	//▲1.0.0.1024
	//▼1.0.0.1043
	INT GetResultContinuousCount(int nIndex);
	INT GetResultContinuousStart(int nIndex);
	INT GetResultContinuousEnd(int nIndex);
	//▲1.0.0.1043

	//検査Thread
	BOOL GetThreadStop(void){return m_bThreadStop;}

	//▼1.0.0.1026
	BOOL CheckExecute(INT nDisplayMode=1);
	//▲1.0.0.1026
	//BMP保存
	//void SetDeviceModelName(LPCTSTR szText){mDeviceModelName = szText;}
	//void SetSerialNumber(LPCTSTR szText){mSerialNumber = szText;}
	void SetSaveFilePath(LPCTSTR szText){mSaveFilePath = szText;}

	//▼1.0.0.1026
	INT GetMode(void){return m_nMode;}	//0:Dialogモード 1:Dialogなしモード
	//▲1.0.0.1026

private:
	void Initialize(void);
	void Release(void);
	void EnableButton(BOOL bStart, BOOL bInit=FALSE);
	//BOOL m_bStart;

	COLORREF m_Color;
	CFont *m_pDefFont;
	CFont m_boldFont;

	//表示
	BOOL JudgeText(BOOL bJudge);
	BOOL ResultText(void);
	BOOL SetLimitText(void);
	BOOL AverageText(double dblAverage,INT nElement);

	//検査Thread
	HANDLE m_hThread;
	unsigned int m_mThreadID;
	BOOL m_bThreadStop;

	//パラメータ
	INT m_nStart;
	INT m_nEnd;
	INT m_nWidthLines;
	INT m_nSeparate;
	//▼1.0.0.1041
	INT m_nCheckMode;
	//▲1.0.0.1041

	//規格値-----
	double m_dblAverageLowLevel;
	double m_dblAverageHighLevel;
	double m_dblDifferenceMax;
	//▼1.0.0.1043
	INT m_nContinuousCountMax;
	//▲1.0.0.1043

	//結果-----
	INT m_iResultElement;
	double m_dblResultAverage;
	double *m_pdblResultDifferenceRatio;
	INT *m_piResultLineRatio;
	//▼1.0.0.1024
	double *m_pdblResultAverage;
	//▲1.0.0.1024
	//▼1.0.0.1043
	PINT m_pnResultContinuousCount;
	PINT m_pnResultContinuousStart;
	PINT m_pnResultContinuousEnd;

	INT m_nWorstContinuousCount;
	//▲1.0.0.1043

	//BMP保存
	double m_dblWorstDifference;
	StBuffer *m_WorstBuffer;
	CString m_szBMPFileName;
	//CString mDeviceModelName;
	//CString mSerialNumber;
	CString mSaveFilePath;
	//▼1.0.0.1039
	CString m_szCSVFileName;
	CString m_szPNGFileName;
	//▲1.0.0.1039

	//▼1.0.0.1026
	INT m_nMode;	//0:Dialogモード 1:Dialogなしモード
	BOOL SaveBitmapMain(void);
	//▲1.0.0.1026

	//▼1.0.0.1028
	//Thread内で表示させると、Stop押下時にダイアログのThreadが動くので、検査スレッド内での表示でフリーズしてしまう。
	double m_dblDisplayAverage;
	//▲1.0.0.1028


public:
// ダイアログ データ
	enum { IDD = IDD_DIALOG_CHECKDUST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
