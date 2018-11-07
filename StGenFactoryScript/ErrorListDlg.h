#pragma once


// CErrorListDlg �_�C�A���O
#include <vector>

class CErrorListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CErrorListDlg)

public:
	CErrorListDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CErrorListDlg();

	void SetStringList(LPCTSTR szString);
	void ClearStringList(void);

private:
	std::vector<CString> m_vTextList;

public:
// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_ERRORLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
