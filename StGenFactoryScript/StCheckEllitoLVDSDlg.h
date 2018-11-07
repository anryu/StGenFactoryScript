#pragma once

#include "stcheckbase.h"

// StCheckEllitoLVDSDlg ダイアログ

//必ず先にCDialogExの継承記述をする必要あり
class StCheckEllitoLVDSDlg : public CDialogEx, public StCheckBase
{
	DECLARE_DYNAMIC(StCheckEllitoLVDSDlg)

public:
	StCheckEllitoLVDSDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	StCheckEllitoLVDSDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx = NULL, CWnd* pParent = NULL);   // コンストラクター
	virtual ~StCheckEllitoLVDSDlg();

	BOOL Execute(void){return TRUE;}

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);

	void SetSaveFilePath(LPCTSTR szSavePath){m_szSavePath = szSavePath;}

private:
	void Initialize(void);
	void Release(void);
	void InitDisplay(); 

	int m_nDataPos;

	char m_delay[33];
	char m_clock[5];
	char m_sync[5];
	char m_defDelay[33];
	char m_defClock[5];
	char m_defSync[5];

	INT m_nSliceSETable[5][2];

	WORD m_wLMAP[33];//LMAP
	//CSpinButtonCtrl *m_SpinClock[5];

	INT m_nEditClockID[5];
	INT m_nSpinClockID[5];
	INT m_nEditSyncID[5];
	INT m_nSpinSyncID[5];
	INT m_nButtonDelayID[8];
	void ChangeClock(INT nClock);
	void ChangeSync(INT nSync);
	void ChangeDelay(INT nData);
	void ChangeListBox(INT nData);

	//Control
	void *m_pControlRetry;
	INT m_nDefRetry;
	CString m_szReturn;
	DWORD m_dwDefTimeout[5];

	CString m_szSavePath;

public:
// ダイアログ データ
	enum { IDD = IDD_DIALOG_ELLITOLVDS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListLmap();
	//afx_msg void OnEnChangeEditClock(UINT nID);
	//afx_msg void OnEnChangeEditSync(UINT nID);
	afx_msg void OnEnChangeEditClock1();
	afx_msg void OnEnChangeEditClock2();
	afx_msg void OnEnChangeEditClock3();
	afx_msg void OnEnChangeEditClock4();
	afx_msg void OnEnChangeEditClock5();
	afx_msg void OnEnChangeEditSync1();
	afx_msg void OnEnChangeEditSync2();
	afx_msg void OnEnChangeEditSync3();
	afx_msg void OnEnChangeEditSync4();
	afx_msg void OnEnChangeEditSync5();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckDelay(UINT nID);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnDestroy();
};
