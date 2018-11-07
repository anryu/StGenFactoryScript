
// StGenFactoryScriptDlg.h : ヘッダー ファイル
//

#pragma once
#include "Preview.h"
#include "WaveformDlg.h"
#include "afxwin.h"
#include <vector>
#include "StStruct.h"
#include "StInspectMain.h"
//▼1.0.0.1062
#include "ErrorListDlg.h"
#include "checklistboxforcheck.h"
//▲1.0.0.1062

// CStGenFactoryScriptDlg ダイアログ
class CStGenFactoryScriptDlg : public CDialogEx
{
// コンストラクション
public:
	CStGenFactoryScriptDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_STGENFACTORYSCRIPT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

//User data
public:
	int MessageScript(CString szMessge, UINT uType);
	BOOL CreatePreview(int sizeX, int sizeY);
	void DeletePreview(void);
	void *GetPreview(void){return (void *)m_Preview;}
	//▼1.0.0.1047
	//BOOL CreateWaveform(int sizeX, int sizeY);
	BOOL CreateWaveform(int sizeX, int sizeY, int nRuledLine);
	//▲1.0.0.1047
	void DeleteWaveform(void);
	void *GetWaveform(void){return (void *)m_Waveform;}
	void InitializeListBox();
	void DisplayListBox(CString szMessge);
	//▼1.0.0.1072
	void SetListBoxCurSel(INT nIndex);
	void ListBoxCurSel(INT nIndex = -1);
	//▲1.0.0.1072

	void EnableObject(BOOL bEnabled);

	//▼1.0.0.1067a
	void DisplayBufferListBox(void);
	//▲1.0.0.1067a

private:
	TCHAR m_szAppVersion[256];
	BOOL SetSelectFileComboBox();
	//▼1.0.0.1069
	BOOL SetTextComboBox();
	BOOL SetSelectModelComboBox();
	//▲1.0.0.1069
	//▼1.0.0.1072
	BOOL SetLockCheckBox(BOOL bCheck);
	BOOL LockComboBox(void);
	//▲1.0.0.1072
	BOOL WriteSelectFile();
	CString m_szIniFile;
	BOOL GetIniFileName(CString &szFileName);
	CString m_szModulePath;
	BOOL GetModulePath(CString &szPath);
	CPreview *m_Preview;
	CWaveformDlg *m_Waveform;
	std::vector<CString> m_vDisplayString;
	HANDLE hDisplayMutex;

	//▼1.0.0.1072
	INT m_nCheckListBoxIndex;
	//▲1.0.0.1072

	//▼1.0.0.1067a
	//void DisplayBufferListBox(void);
	HANDLE m_hDisplayThread;
	void DisplayBufferListBoxThread(void);
	//▲1.0.0.1067a
	//HANDLE hPTouchMutex;
	BOOL PrintPTouch(void *pInspect);
	INT		m_ForwardingCheckMode;	//0:工程内検査, 1:出荷検査
	INT		m_FactoryMode;	        //0:Japan, 1:Korea

	//▼1.0.0.1062
	INT		m_nScriptCheckMode;		//0:通常 1:Check
	CErrorListDlg *m_pErrorListDlg;
	//▲1.0.0.1062

	CStructPortName m_StructPortName;
	//▼1.00.1017
	CStructPortName m_StructAddress;
	//▲1.00.1017
	//▼1.00.1020
	CStructLightChannel m_StructLightChannel;
	//▲1.00.1020


	//検査メイン
	StInspectMain *m_StInspectMain;


	//▼1.0.0.1015
	//ForwardingCheckMode
	BOOL SetButtonName(INT nMode);
	//Factory
	BOOL FactoryExistFileRead(INT nMode);
	std::vector<CString> m_vFactoryExistFile;
	BOOL FactoryExistCheck(LPCTSTR szFileName);
	//CameraType
	INT		m_CameraType;	        //0:All 1:GigE 2:CL
	BOOL CameraTypeExistFileRead(INT nType);
	std::vector<CString> m_vCameraTypeExistFile;
	BOOL CameraTypeExistCheck(LPCTSTR szFileName);
	//▲1.0.0.1015
	//▼1.0.0.1026
	CString m_szSoftwareVersion;
	//▲1.0.0.1026
	//▼1.0.0.1069
	BOOL m_bEnabledCheckBox;
	void UpdateCheckBoxEnabled(BOOL bEnabled);
	void InitializeCheckBox(BOOL bChecked=TRUE);
	TCHAR m_szIniFileName[MAX_PATH];

	BOOL GetModelFileName(LPTSTR lpModelFileName, size_t size);
	BOOL IsCheckFeature(LPCTSTR szFeature);
	//▲1.0.0.1069

	//▼1.0.0.1072a
	INT CheckFeaturesWritingFormat(LPCTSTR lpModelFileName);
	BOOL InitializeListBoxNonNumberMode(LPCTSTR lpModelFileName);
	BOOL InitializeCheckListNonNumberMode(LPCTSTR lpModelFileName,LPCTSTR szModelScript);
	//▲1.0.0.1072a


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonExecute();
	CListBox m_ResultListBox;
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeComboSelectfile();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnMenuitemSelectPort();
	afx_msg void OnMenuitemOpenLogfolder();
	afx_msg void OnMenuitemCheckType(UINT ID);
	afx_msg void OnMenuitemFactoryType(UINT ID);
	//▼1.0.0.1015
	afx_msg void OnMenuitemCameraType(UINT ID);
	//▲1.0.0.1015
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMenuitemSettingterminal();
	afx_msg void OnMenuitemSettinglightchannel();
	//▼1.0.0.1062
	afx_msg void OnMenuitemScriptcheck(UINT ID);
	//▲1.0.0.1062
	afx_msg void OnMenuitemCheckBoxEnabled();
	afx_msg void OnMenuitemCheckBoxAllChecked();
	afx_msg void OnMenuitemCheckBoxAllUnchecked();
	afx_msg void OnInitMenu(CMenu* pMenu);
	//▼1.0.0.1069
	afx_msg void OnRadioitemScript(UINT ID);
	//▲1.0.0.1069
private:
	int m_xvRadioSelectMode;
	CCheckListBoxForCheck m_listCheckItem;
public:
	afx_msg void OnBnClickedCheckLockCameraType();
};
