#pragma once

#include "StStruct.h"

// SettingLightChannelDlg �_�C�A���O

class SettingLightChannelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SettingLightChannelDlg)

public:
	SettingLightChannelDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~SettingLightChannelDlg();

	void SetStructLightChannel(CStructLightChannel *pStructLightChannel ){m_pStructLightChannel = pStructLightChannel;}
private:
	//void UpdateComPortList(void);
	void InitList(void);

	CStructLightChannel *m_pStructLightChannel;
	CStructLightChannel m_StructLightChannel;

public:
// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_SETTINGLIGHTCHANNEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
