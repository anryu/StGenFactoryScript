#pragma once

#include "StStruct.h"

// SettingLightChannelDlg ダイアログ

class SettingLightChannelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SettingLightChannelDlg)

public:
	SettingLightChannelDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~SettingLightChannelDlg();

	void SetStructLightChannel(CStructLightChannel *pStructLightChannel ){m_pStructLightChannel = pStructLightChannel;}
private:
	//void UpdateComPortList(void);
	void InitList(void);

	CStructLightChannel *m_pStructLightChannel;
	CStructLightChannel m_StructLightChannel;

public:
// ダイアログ データ
	enum { IDD = IDD_DIALOG_SETTINGLIGHTCHANNEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
