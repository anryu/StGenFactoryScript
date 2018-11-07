#pragma once

class CResourceString
{
public:
	CResourceString(void){};
	~CResourceString(void){};

	static void SetResourceText(CWnd *pParentWnd, INT nCtrlID, UINT nResID)
	{
		CWnd *pWnd = pParentWnd->GetDlgItem(nCtrlID);
		if(pWnd != NULL)
		{
			CString strMsg;
			strMsg.LoadString(nResID);
			pWnd->SetWindowText(strMsg);
		}
	}
	static void SetResourceText(CWnd *pWnd, UINT nResID)
	{
		if(pWnd != NULL)
		{
			CString strMsg;
			strMsg.LoadString(nResID);
			pWnd->SetWindowText(strMsg);
		}
	}
	static void SetResourceText(CWnd *pParentWnd, INT nCtrlID, LPCTSTR szText)
	{
		CWnd *pWnd = pParentWnd->GetDlgItem(nCtrlID);
		if(pWnd != NULL)
		{
			pWnd->SetWindowText(szText);
		}
	}
	static void SetResourceText(CWnd *pWnd, LPCTSTR szText)
	{
		if(pWnd != NULL)
		{
			pWnd->SetWindowText(szText);
		}
	}

};
