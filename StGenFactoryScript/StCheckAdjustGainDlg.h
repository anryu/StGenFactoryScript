#pragma once

#include "stcheckbase.h"

// StCheckAdjustGainDlg �_�C�A���O

class StCheckAdjustGainDlg : public CDialogEx, public StCheckBase
{
	DECLARE_DYNAMIC(StCheckAdjustGainDlg)

public:
	StCheckAdjustGainDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	StCheckAdjustGainDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx = NULL, CWnd* pParent = NULL);   // �R���X�g���N�^�[
	virtual ~StCheckAdjustGainDlg();
	BOOL Execute(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);

	BOOL AdjustExecute(void);

private:
	void Initialize(void);
	INT m_nChannelCount;
	INT m_nStartChannel1;
	INT m_nStartChannel3;
	INT m_nWidth;
	INT m_nTimer;
	INT m_nBaseChannel;
	INT m_nRange;
	INT m_nDiff;

	INT m_nDefGainPos;

public:
// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_ADJUSTGAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
