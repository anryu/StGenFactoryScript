#pragma once

#include "stcheckbase.h"
#include "StSerialComm.h"
#include <vector>

// StLightControlDlg �_�C�A���O

class StLightControlDlg : public CDialogEx, public StCheckBase
{
	DECLARE_DYNAMIC(StLightControlDlg)

public:
	StLightControlDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	StLightControlDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx = NULL, CWnd* pParent = NULL);   // �R���X�g���N�^�[
	virtual ~StLightControlDlg();

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
  	//��1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);
	//��1.0.0.1025

	BOOL Execute(void);

	void SetSerialComm(StSerialComm *pSerialComm)
	{
		m_pSerialComm = pSerialComm;
	}
	BOOL LightAdjustExecute(void);



	BOOL SendInitValue(void);
	BOOL SetShadeGo(INT nColor);
	BOOL SetShadeValue(INT nValue);

private:
	void Initialize(void);

	//�p�����[�^
	StSerialComm *m_pSerialComm;
	INT m_nStart;
	INT m_nEnd;
	INT m_nTimer;
	CString m_szColor;
	INT m_nMaxValue[3];	//RGB��
	INT m_nInitValue[3];
	INT m_nTarget[3];
	INT m_nTargetRange[3];

	//INT m_nMaxValueR;
	//INT m_nMaxValueG;
	//INT m_nMaxValueB;
	//INT m_nInitValueR;
	//INT m_nInitValueG;
	//INT m_nInitValueB;
	//INT m_nTargetR;
	//INT m_nTargetG;
	//INT m_nTargetB;
	//INT m_nTargetRangeR;
	//INT m_nTargetRangeG;
	//INT m_nTargetRangeB;

	//����Thread
	HANDLE m_hThread;
	unsigned int m_mThreadID;
	BOOL m_bThreadStop;

	//Display
	void DisplayBufferListBox(void);
	HANDLE hDisplayMutex;
	std::vector<CString> m_vDisplayString;
	void DisplayListBox(CString szMessge);

public:
// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_LIGHTCONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
