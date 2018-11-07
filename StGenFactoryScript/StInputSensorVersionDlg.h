#pragma once

#include "stcheckbase.h"
#include <vector>

// StInputSensorVersionDlg ダイアログ

//必ず先にCDialogExの継承記述をする必要あり
//StCheckBaseを先に記述すると不具合
class StInputSensorVersionDlg : public CDialogEx, public StCheckBase
{
	DECLARE_DYNAMIC(StInputSensorVersionDlg)

public:
	StInputSensorVersionDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	StInputSensorVersionDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx = NULL, CWnd* pParent = NULL);   // コンストラクター
	virtual ~StInputSensorVersionDlg();

	BOOL Execute(void);
	void SetSensorVersion(LPCTSTR szSensorVersion){m_szSensorVersion = szSensorVersion;}
	void GetSensorVersion(LPTSTR szSensorVersion, size_t size)
	{
		_tcscpy_s( szSensorVersion, size, m_szSensorVersion);
	}
	LPCTSTR GetSensorVersion(void){return m_szSensorVersion;}

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
  	//▼1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, CString &szValue);
	//▲1.0.0.1025

private:
	BOOL OKButtonEnabled(void);
	CString m_szSensorVersion;

	BOOL AddCheckVersion( LPCTSTR szVersion );
	BOOL ClearCheckVersion(void);
	BOOL VersionCheck(void);
	std::vector<CString> m_vCheckVersion;

public:
// ダイアログ データ
	enum { IDD = IDD_DIALOG_INPUTSENSORVERSION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCmbSelChanged(UINT nID);
};
