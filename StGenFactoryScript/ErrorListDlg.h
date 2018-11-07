#pragma once


// CErrorListDlg ダイアログ
#include <vector>

class CErrorListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CErrorListDlg)

public:
	CErrorListDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CErrorListDlg();

	void SetStringList(LPCTSTR szString);
	void ClearStringList(void);

private:
	std::vector<CString> m_vTextList;

public:
// ダイアログ データ
	enum { IDD = IDD_DIALOG_ERRORLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
