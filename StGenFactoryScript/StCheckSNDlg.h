#pragma once

#include "stcheckbase.h"

// StCheckSNDlg �_�C�A���O

//�K�����CDialogEx�̌p���L�q������K�v����
class StCheckSNDlg : public CDialogEx, public StCheckBase
{
	DECLARE_DYNAMIC(StCheckSNDlg)

public:
	StCheckSNDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	StCheckSNDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx = NULL, CWnd* pParent = NULL);   // �R���X�g���N�^�[
	virtual ~StCheckSNDlg();


	BOOL Execute(void){return TRUE;}

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	//��1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg,double &dblValue);
	//��1.0.0.1025

	//---�p�����[�^�ݒ�-------------------------------
	void SetStart(INT nData);
	void SetEnd(INT nData);


	//---�K�i�ݒ�-------------------------------
	void SetAverageMin(double dblData);
	void SetAverageMax(double dblData);
	void SetSNMax(double dblData);
	void SetSN(double dblData);

	//---���ʎ擾-------------------------------
	INT GetElement(void){return m_nElement;}
	double GetAverage(INT nIndex){return m_dblResultAverage[nIndex];}
	double GetSN(INT nIndex){return m_dblResultSN[nIndex];}
	double GetSNMax(INT nIndex){return m_dblResultSNMax[nIndex];}
	INT GetSDMPix(INT nIndex){return m_nResultSDMPix[nIndex];}
	//INT GetColorIndex(INT nIndex){return m_nColorIndex[nIndex];}

	//����Thread
	BOOL GetThreadStop(void){return m_bThreadStop;}
	BOOL CheckExecute(void);
	//��1.0.0.1056
	HANDLE m_hMessageMutex;

	void SetSaveFilePath(LPCTSTR szSavePath){mSaveFilePath = szSavePath;}
	//��1.0.0.1056

private:
	void Initialize(void);

	COLORREF m_Color;
	CFont *m_pDefFont;
	CFont m_boldFont;

	//�\��
	BOOL JudgeText(BOOL bJudge);
	BOOL ResultText(void);
	BOOL SetLimitText(void);

	//����Thread
	HANDLE m_hThread;
	unsigned int m_mThreadID;
	BOOL m_bThreadStop;

	//�p�����[�^
	INT m_nStart;
	INT m_nEnd;
	//��1.0.0.1056
	INT m_nMode;	//0:�]�����[�h 1:NG���ł���X�g�b�v���[�h
	//��1.0.0.1056
	//�K�i�l-----
	double m_dblPermAverageMin;
	double m_dblPermAverageMax;

	//��1.0.0.1056
	double m_dblPermLineAverageMin;
	double m_dblPermLineAverageMax;

	double m_dblPermAverageColorMin[3];
	double m_dblPermAverageColorMax[3];

	double m_dblPermLineAverageColorMin[3];
	double m_dblPermLineAverageColorMax[3];
	//��1.0.0.1056

	double m_dblPermSNMax;
	double m_dblPermSN;

	//����-----
	INT m_nElement;
	//INT m_nColorIndex[4];	//0:R 1:G 2:B 3:A -1:Mono 
	double m_dblResultAverage[4];
	double m_dblResultSN[4];
	double m_dblResultSNMax[4];
	INT m_nResultSDMPix[4];

	//��1.0.0.1056
	double m_dblReslutLineAverageMin[4];
	double m_dblReslutLineAverageMax[4];
	INT m_nResultLineAverageMinPos[4];
	INT m_nResultLineAverageMaxPos[4];

	BOOL m_bResultNG;
	double m_dblResultNGAverage[4];
	double m_dblResultNGSN[4];
	double m_dblResultNGSNMax[4];
	INT m_nResultNGSDMPix[4];


	double m_dblReslutNGLineAverageMin[4];
	double m_dblReslutNGLineAverageMax[4];
	INT m_nResultNGLineAverageMinPos[4];
	INT m_nResultNGLineAverageMaxPos[4];

	CString m_szImageFileName;
	CString mSaveFilePath;
	StBuffer mNGImageBuffer;
	//��1.0.0.1056

	BOOL m_bResult;

public:
// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CHECKSN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
