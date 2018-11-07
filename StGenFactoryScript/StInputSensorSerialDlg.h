#pragma once

#include "stcheckbase.h"
#include "afxwin.h"

// StInputSensorSerialDlg �_�C�A���O

class StInputSensorSerialDlg : public CDialogEx, public StCheckBase
{
	DECLARE_DYNAMIC(StInputSensorSerialDlg)

public:
	StInputSensorSerialDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	StInputSensorSerialDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx = NULL, CWnd* pParent = NULL);   // �R���X�g���N�^�[
	virtual ~StInputSensorSerialDlg();
	BOOL Execute(void);

  	//��1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, CString &szValue);
	//��1.0.0.1025

	void SetSensorSerial(LPCTSTR szSensorSerial){m_szSensorSerial = szSensorSerial;}
	void GetSensorSerial(LPTSTR szSensorSerial, size_t size)
	{
		_tcscpy_s( szSensorSerial, size, m_szSensorSerial);
	}
	LPCTSTR GetSensorSerial(void){return m_szSensorSerial;}

private:
	BOOL OKButtonEnabled(void);
	CString m_szSensorSerial;
	//��1.0.0.1043
	void DisplaySensorSerial(void);
	//��1.0.0.1043

public:

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_INPUTSENSORSERIAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmbSelChanged(UINT nID);
	CStatic m_StaticSensorSerial;
};
