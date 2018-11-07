#pragma once

#include "stcheckbase.h"

// StCheckDustDlg �_�C�A���O

//�K�����CDialogEx�̌p���L�q������K�v����
class StCheckDustDlg : public CDialogEx, public StCheckBase
{
	DECLARE_DYNAMIC(StCheckDustDlg)

public:
	StCheckDustDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	StCheckDustDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx = NULL, CWnd* pParent = NULL);   // �R���X�g���N�^�[
	virtual ~StCheckDustDlg();

	//��1.0.0.1026
	//BOOL Execute(void){return TRUE;}
	BOOL Execute(void);
	//��1.0.0.1026
	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	//��1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);
	//��1.0.0.1025

	//---�p�����[�^�ݒ�-------------------------------
	void SetStart(INT nData);
	void SetEnd(INT nData);
	void SetWidthLines(INT nData);
	void SetSeparate(INT nData);
	void SetBMPFileName(LPCTSTR szData);
	//��1.0.0.1039
	void SetCSVFileName(LPCTSTR szData);
	void SetPNGFileName(LPCTSTR szData);
	//��1.0.0.1039
	//��1.0.0.1041
	void SetCheckMode(INT nData);
	//��1.0.0.1041

	//�K�i�l-----
	void SetDifferenceMax(double dblData);
	void SetAverageLowLevel(double dblData);
	void SetAverageHighLevel(double dblData);
	//��1.0.0.1043
	void SetContinuousCountMax(INT nData){m_nContinuousCountMax = nData;}
	//��1.0.0.1043

	//---���ʎ擾-------------------------------
	INT GetResultElement(void);
	double GetAverage(void);
	double GetResultDifference(int nIndex);
	INT GetResultLine(int nIndex);
	//��1.0.0.1024
	double GetResultAverage(int nIndex);
	//��1.0.0.1024
	//��1.0.0.1043
	INT GetResultContinuousCount(int nIndex);
	INT GetResultContinuousStart(int nIndex);
	INT GetResultContinuousEnd(int nIndex);
	//��1.0.0.1043

	//����Thread
	BOOL GetThreadStop(void){return m_bThreadStop;}

	//��1.0.0.1026
	BOOL CheckExecute(INT nDisplayMode=1);
	//��1.0.0.1026
	//BMP�ۑ�
	//void SetDeviceModelName(LPCTSTR szText){mDeviceModelName = szText;}
	//void SetSerialNumber(LPCTSTR szText){mSerialNumber = szText;}
	void SetSaveFilePath(LPCTSTR szText){mSaveFilePath = szText;}

	//��1.0.0.1026
	INT GetMode(void){return m_nMode;}	//0:Dialog���[�h 1:Dialog�Ȃ����[�h
	//��1.0.0.1026

private:
	void Initialize(void);
	void Release(void);
	void EnableButton(BOOL bStart, BOOL bInit=FALSE);
	//BOOL m_bStart;

	COLORREF m_Color;
	CFont *m_pDefFont;
	CFont m_boldFont;

	//�\��
	BOOL JudgeText(BOOL bJudge);
	BOOL ResultText(void);
	BOOL SetLimitText(void);
	BOOL AverageText(double dblAverage,INT nElement);

	//����Thread
	HANDLE m_hThread;
	unsigned int m_mThreadID;
	BOOL m_bThreadStop;

	//�p�����[�^
	INT m_nStart;
	INT m_nEnd;
	INT m_nWidthLines;
	INT m_nSeparate;
	//��1.0.0.1041
	INT m_nCheckMode;
	//��1.0.0.1041

	//�K�i�l-----
	double m_dblAverageLowLevel;
	double m_dblAverageHighLevel;
	double m_dblDifferenceMax;
	//��1.0.0.1043
	INT m_nContinuousCountMax;
	//��1.0.0.1043

	//����-----
	INT m_iResultElement;
	double m_dblResultAverage;
	double *m_pdblResultDifferenceRatio;
	INT *m_piResultLineRatio;
	//��1.0.0.1024
	double *m_pdblResultAverage;
	//��1.0.0.1024
	//��1.0.0.1043
	PINT m_pnResultContinuousCount;
	PINT m_pnResultContinuousStart;
	PINT m_pnResultContinuousEnd;

	INT m_nWorstContinuousCount;
	//��1.0.0.1043

	//BMP�ۑ�
	double m_dblWorstDifference;
	StBuffer *m_WorstBuffer;
	CString m_szBMPFileName;
	//CString mDeviceModelName;
	//CString mSerialNumber;
	CString mSaveFilePath;
	//��1.0.0.1039
	CString m_szCSVFileName;
	CString m_szPNGFileName;
	//��1.0.0.1039

	//��1.0.0.1026
	INT m_nMode;	//0:Dialog���[�h 1:Dialog�Ȃ����[�h
	BOOL SaveBitmapMain(void);
	//��1.0.0.1026

	//��1.0.0.1028
	//Thread���ŕ\��������ƁAStop�������Ƀ_�C�A���O��Thread�������̂ŁA�����X���b�h���ł̕\���Ńt���[�Y���Ă��܂��B
	double m_dblDisplayAverage;
	//��1.0.0.1028


public:
// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CHECKDUST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
