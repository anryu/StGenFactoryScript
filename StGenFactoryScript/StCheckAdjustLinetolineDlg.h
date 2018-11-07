#pragma once

#include "stcheckbase.h"
#include <vector>
// StCheckAdjustLinetolineDlg ダイアログ

//必ず先にCDialogExの継承記述をする必要あり
class StCheckAdjustLinetolineDlg : public CDialogEx, public StCheckBase
{
	DECLARE_DYNAMIC(StCheckAdjustLinetolineDlg)

public:
	StCheckAdjustLinetolineDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	StCheckAdjustLinetolineDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx = NULL, CWnd* pParent = NULL);   // コンストラクター
	virtual ~StCheckAdjustLinetolineDlg();

	BOOL Execute(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
 	//▼1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);
	//▲1.0.0.1025

	BOOL AdjustExecute(void);

	//検査Thread
	BOOL GetThreadStop(void){return m_bThreadStop;}


	INT GetResultCount(void){return m_ResultCount;}
	INT GetStandardCount(void){return m_nAdjustCount;}

private:
	void Initialize(void);
	BOOL WriteData(BYTE byteDeviceCode, BYTE bytePage, BYTE byteAddress, BYTE byteData);

	//パラメータ
	INT m_nStart;
	INT m_nEnd;
	INT m_nTimer;
	INT m_nDeviceCode;
	INT m_nPage;
	INT m_nAddress;
	INT m_nInitValue;
	double m_dblMinAverage;
	double m_dblMaxAverage;
	INT m_nAdjustCount;

	BYTE m_SetValue;
	double m_ResultMinAverage;
	INT m_ResultLine;
	INT m_ResultElement;
	INT m_ResultCount;

	//検査Thread
	HANDLE m_hThread;
	unsigned int m_mThreadID;
	BOOL m_bThreadStop;

	std::vector<CString> m_vDisplayString;
	HANDLE hDisplayMutex;
	void DisplayBufferListBox(void);
	void DisplayListBox(CString szMessge);

public:
// ダイアログ データ
	enum { IDD = IDD_DIALOG_LINETOLINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
