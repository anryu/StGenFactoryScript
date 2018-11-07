#pragma once


// CSettingLightChannel ダイアログ

class CSettingLightChannelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingLightChannelDlg)

public:
	CSettingLightChannelDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSettingLightChannelDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SETTINGLIGHTCHANNEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
