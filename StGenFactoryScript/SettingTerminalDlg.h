#pragma once

#include "StStruct.h"

// CSettingTerminalDlg ダイアログ

class CSettingTerminalDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingTerminalDlg)

public:
	CSettingTerminalDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSettingTerminalDlg();

	void SetStructAddress(CStructPortName *pStructAddress){m_pStructAddress=pStructAddress;}
	CStructPortName * GetStructAddress(void){return m_pStructAddress;}

private:
	//void UpdateComPortList(void);
	void InitSelectorList(void);

	CStructPortName *m_pStructAddress;
	CStructPortName m_StructAddress;

public:

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SETTINGTERMINAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboSelector();
	afx_msg void OnEnKillfocusEditTerminaladdress();
	afx_msg void OnBnClickedOk();
};
