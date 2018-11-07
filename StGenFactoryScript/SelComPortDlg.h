#pragma once

#include "StStruct.h"

// CSelComPortDlg ダイアログ

class CSelComPortDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelComPortDlg)

public:
	CSelComPortDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSelComPortDlg();

	void SetStructPortName(CStructPortName *pStructPortName){m_pStructPortName=pStructPortName;}
	CStructPortName * GetStructPortName(void){return m_pStructPortName;}

private:
	void UpdateComPortList(void);
	void InitSelectorList(void);

	CStructPortName *m_pStructPortName;
	CStructPortName m_StructPortName;

public:
// ダイアログ データ
	enum { IDD = IDD_DIALOG_SELECTPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboSelector();
	afx_msg void OnCbnSelchangeComboPortname();
};
