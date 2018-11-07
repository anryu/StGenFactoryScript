#pragma once
#include "afxdialogex.h"
#include "../CommonHeader/StString.h"

class CCustomDialogEx :
	public CDialogEx
{
public:
	CCustomDialogEx::CCustomDialogEx(UINT nIDTemplate,CWnd* pParentWnd = NULL );
	~CCustomDialogEx(void);

	StString m_ClassName;
	StString m_VarName;

};

