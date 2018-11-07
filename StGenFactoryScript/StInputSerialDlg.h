#pragma once

#include "stcheckbase.h"

// StInputSerialDlg �_�C�A���O

//�K�����CDialogEx�̌p���L�q������K�v����
//StCheckBase���ɋL�q����ƕs�
//class StInputSerialDlg : public StCheckBase, public CDialogEx
class StInputSerialDlg : public CDialogEx, public StCheckBase
{
	DECLARE_DYNAMIC(StInputSerialDlg)

public:
	StInputSerialDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	StInputSerialDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx = NULL, CWnd* pParent = NULL);   // �R���X�g���N�^�[
	virtual ~StInputSerialDlg();

	void SetLimitText(int iLimitText){m_iLimitText = iLimitText;}
	BOOL Execute(void);
	void GetSerialNumber(CString &szSerialNumber){szSerialNumber = m_szSerialNumber;}
	LPCTSTR GetSerialNumber(void){return m_szSerialNumber;}
	void GetWorkerName(CString &szWorkerName){szWorkerName = m_szWorkerName;}
	LPCTSTR GetWorkerName(void){return m_szWorkerName;}
	void GetHinmokuCode(CString &szHinmokuCode){szHinmokuCode = m_szHinmokuCode;}
	LPCTSTR GetHinmokuCode(void){return m_szHinmokuCode;}

	//��1.0.0.1025
	 BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, CString &szValue);
	//��1.0.0.1025

	//��1.0.0.1031
	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	BOOL GetCheck(void){return m_bCheck;}
	//��1.0.0.1031

private:
	int m_iLimitText;
	//int m_iDefaultWorkerCount;
	TCHAR m_szIniFileName[MAX_PATH];
	TCHAR m_szWorkerFileName[MAX_PATH];
	void SetWorker(void);
	void GetWorker(void);
	BOOL EnableObject(void);

	//Result---
	CString m_szSerialNumber;
	CString m_szWorkerName;
	CString m_szHinmokuCode;

	//��1.0.0.1031
	void Initialize(void);
	INT m_nCheckMode;       //0:Check���Ȃ� 1:����ꍇNG(�����p) 2:�Ȃ��ꍇNG(���p)
	CString m_szSerialNumberFileName;
	BOOL m_bCheck;
	//��1.0.0.1031

public:

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_INPUTSERIAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditSerialnumber();
	afx_msg void OnCbnEditchangeComboWorkername();
};
