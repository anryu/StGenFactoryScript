// SettingLightChannelDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "SettingLightChannelDlg.h"
#include "afxdialogex.h"
#include "resource.h"


// SettingLightChannelDlg ダイアログ

IMPLEMENT_DYNAMIC(SettingLightChannelDlg, CDialogEx)

SettingLightChannelDlg::SettingLightChannelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SettingLightChannelDlg::IDD, pParent)
{
	m_pStructLightChannel = NULL;
}

SettingLightChannelDlg::~SettingLightChannelDlg()
{
}

void SettingLightChannelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SettingLightChannelDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SettingLightChannelDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SettingLightChannelDlg メッセージ ハンドラー


BOOL SettingLightChannelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	m_StructLightChannel.Copy(m_pStructLightChannel);

	InitList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

INT nLightChannelComboID[] = { IDC_COMBO_COLOR0, IDC_COMBO_COLOR1, IDC_COMBO_COLOR2, IDC_COMBO_COLOR3 };
void SettingLightChannelDlg::InitList(void)
{
	//if( !m_pStructPortName ) return;
	INT nStaticID[] = { IDC_STATIC_COLOR0, IDC_STATIC_COLOR1, IDC_STATIC_COLOR2, IDC_STATIC_COLOR3 };
	INT nChannelCount = 4;

	CComboBox objComboBox;

	for( int i=0; i<m_StructLightChannel.GetPortSelectorCount(); i++ )
	{
		if( i<_countof(nStaticID) )
		{
			CString szString;
			BOOL bReval = m_StructLightChannel.GetSelectorName(i, szString);
			if( bReval )
			{
				GetDlgItem(nStaticID[i])->SetWindowText( szString );
			}
		}

		if( i<_countof(nLightChannelComboID) )
		{
			INT nIndex = 0;
			BOOL bReval = m_StructLightChannel.GetIndex(i, nIndex);

			objComboBox.Attach(GetDlgItem(nLightChannelComboID[i])->m_hWnd);
			for( int j=0; j<nChannelCount; j++ )
			{
				CString szChannel;
				szChannel.Format(_T("Channel%i"), j);
				objComboBox.AddString(szChannel);
			}
			objComboBox.SetCurSel(nIndex<nChannelCount?nIndex:(nChannelCount-1));
			objComboBox.Detach();
		}
	}
}

void SettingLightChannelDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	CComboBox objComboBox;
	for( int i=0; i<m_StructLightChannel.GetPortSelectorCount(); i++ )
	{
		objComboBox.Attach(GetDlgItem(nLightChannelComboID[i])->m_hWnd);

		INT nChannelIndex = objComboBox.GetCurSel();
		m_StructLightChannel.SetIndex(i,nChannelIndex);
		objComboBox.Detach();
	}
	m_pStructLightChannel->Copy(&m_StructLightChannel);

	m_pStructLightChannel->WriteFileLightChannel();

	CDialogEx::OnOK();
}
