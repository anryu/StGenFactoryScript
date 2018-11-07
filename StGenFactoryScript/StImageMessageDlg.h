#pragma once
#include "afxwin.h"
#include "Resource.h"
//▼1.0.0.1048
//#include "../CommonHeader/StString.h"
#include "CustomDialogEx.h"
//▲1.0.0.1048


// StImageMessageDlg ダイアログ

//void DoEvents();

//▼1.0.0.1048
//class StImageMessageDlg : public CDialogEx
class StImageMessageDlg : public CCustomDialogEx
//▲1.0.0.1048
{
	DECLARE_DYNAMIC(StImageMessageDlg)

public:
	StImageMessageDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~StImageMessageDlg();

	void SetImageSizeX(INT nData){m_nImageSizeX = nData;}
	void SetImageSizeY(INT nData){m_nImageSizeY = nData;}
	void SetMessageSizeX(INT nData){m_nMessageSizeX = nData;}
	void SetMessageSizeY(INT nData){m_nMessageSizeY = nData;}

	void SetImageFile(LPCTSTR szData){m_szImageFile = szData;}
	//▼1.0.0.1015
	//void SetMessage(LPCTSTR szData){m_szMessage = szData;}
	void SetMessage(LPCTSTR szData)
	{
		CString szTmp = szData;
		CString szMessage(_T(""));
		do
		{
			INT iPos = szTmp.Find(_T("\\n"));
			if( iPos<0 ) 
			{
				szMessage += szTmp;
				break;
			}
			else
			{
				szMessage += szTmp.Left(iPos);
				szMessage += _T("\n");
				szTmp = szTmp.Mid(iPos+2);
			}
		}while(szTmp.GetLength());
		m_szMessage = szMessage;
		//m_szMessage.Replace();
	}
	//▲1.0.0.1015
	INT GetImageSizeX(void){return m_nImageSizeX;}
	INT GetImageSizeY(void){return m_nImageSizeY;}
	INT GetMessageSizeX(void){return m_nMessageSizeX;}
	INT GetMessageSizeY(void){return m_nMessageSizeY;}
	INT GetWindowPosX(void){return m_nWindowPosX;}
	INT GetWindowPosY(void){return m_nWindowPosY;}

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);

	//▼1.0.0.1048
	//StString m_ClassName;
	//StString m_VarName;
	//▲1.0.0.1048

private:
	INT m_nImageSizeX;
	INT m_nImageSizeY;
	INT m_nMessageSizeX;
	INT m_nMessageSizeY;
	CString m_szImageFile;
	CString m_szMessage;

	void SetWindowPosX(INT nData){m_nWindowPosX = nData;}
	void SetWindowPosY(INT nData){m_nWindowPosY = nData;}
	INT m_nWindowPosX;
	INT m_nWindowPosY;

public:
// ダイアログ データ
	enum { IDD = IDD_DIALOG_IMAGEMESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	CStatic m_StaticImage;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	CStatic m_staticMessage;
	CEdit m_editMessage;
	CStatic m_staticImageMessage;
};
