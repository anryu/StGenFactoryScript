#pragma once

#include "StStruct.h"

// CSelComPortDlg �_�C�A���O

class CSelComPortDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelComPortDlg)

public:
	CSelComPortDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSelComPortDlg();

	void SetStructPortName(CStructPortName *pStructPortName){m_pStructPortName=pStructPortName;}
	CStructPortName * GetStructPortName(void){return m_pStructPortName;}

private:
	void UpdateComPortList(void);
	void InitSelectorList(void);

	CStructPortName *m_pStructPortName;
	CStructPortName m_StructPortName;

public:
// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_SELECTPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboSelector();
	afx_msg void OnCbnSelchangeComboPortname();
};
