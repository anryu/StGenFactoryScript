#pragma once

#include "stcheckbase.h"

// StCameraControlDlg �_�C�A���O

class StCameraControlDlg : public CDialogEx, public StCheckBase
{
	DECLARE_DYNAMIC(StCameraControlDlg)

public:
	StCameraControlDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	StCameraControlDlg( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, CDialogEx *aDialogEx = NULL, CWnd* pParent = NULL);   // �R���X�g���N�^�[
	virtual ~StCameraControlDlg();

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	BOOL Execute(void){return TRUE;}
	INT GetWindowPosX(void){return m_nWindowPosX;}
	INT GetWindowPosY(void){return m_nWindowPosY;}

private:
	void Initialize(void);
	//INT m_nEndian;	//0:LittleEndian 1:BigEndian
	//INT m_nEndianOriginal;	//0:LittleEndian 1:BigEndian
	INT m_nWindowPosX;
	INT m_nWindowPosY;

public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CAMERACONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonReadstring();
	afx_msg void OnBnClickedButtonWrite2();
	afx_msg void OnBnClickedButtonWritestring();
	afx_msg void OnEnKillfocusEditWritevalue();
	afx_msg void OnEnKillfocusEditWritevaluedec();
};
