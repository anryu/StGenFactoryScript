#pragma once


// CSettingLightChannel �_�C�A���O

class CSettingLightChannelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingLightChannelDlg)

public:
	CSettingLightChannelDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSettingLightChannelDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_SETTINGLIGHTCHANNEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
