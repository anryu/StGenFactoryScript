// HookMsgBox.cpp ソースファイル
// StfAfx.hインクルード ﾌﾟﾘｺﾝﾊﾟｲﾙ済み～のエラーを出さないために必要
#include "StdAfx.h"
// ヘッダインクルード
#include "HookMsgBox.h"
#include "Common.h"

//////////////////////////////////////////////////////////////////
// フックハンドル用の変数をグローバルで宣言する。
HHOOK MyHookHandle;	// フックハンドル変数
// OKの文字列と、キャンセルの文字列
// ボタンの文字を変更したい場合はここを変更してください
LPCTSTR strOK = _T("OK");			// OKボタンの文字
LPCTSTR strCancel = _T("Cancel");	// キャンセルボタンの文字
LPCTSTR strYes = _T("Yes");	// OKボタンの文字
LPCTSTR strNo = _T("No");	// Noボタンの文字
LPCTSTR strRetry= _T("Retry");	// Noボタンの文字

////////////////////////////////////////////////////////////////////
//
//		メッセージボックス書き換えのための
//		フック関数（ローカルフック用）
//		※ グローバル関数である必要があります。
//		この関数の中を変えるときは慎重に行って下さい。
//
LRESULT CALLBACK MsgBoxHookProc (int nCode, WPARAM wParam ,LPARAM lParam )
{
	// コード判断
	if (nCode >= 0)
	{
		if ( nCode == HCBT_ACTIVATE)
		{
			if( GetLocale()!=GetSupportedLCID(0) )
			{

				// OKボタン(IDOK)の内容を書き換える
				SetDlgItemText( ( HWND )wParam, IDOK, strOK );
				// キャンセルボタン(IDCANCEL)の内容を書き換える
				SetDlgItemText( ( HWND )wParam, IDCANCEL, strCancel );

				// はいボタン(IDYES)の内容を書き換える
				SetDlgItemText( ( HWND )wParam, IDYES, strYes );

				// いいえボタン(IDNO)の内容を書き換える
				SetDlgItemText( ( HWND )wParam, IDNO, strNo );

				// 再試行ボタン(IDRETRY)の内容を書き換える
				SetDlgItemText( ( HWND )wParam, IDRETRY, strRetry );

				HRESULT ret;
				// フック関数をアンインストール(フック解除！）
				ret = UnhookWindowsHookEx ( MyHookHandle );
			}
		}
	}
	// 次のフックに渡す
	return CallNextHookEx ( MyHookHandle, nCode, wParam, lParam);
}
/////////////////////////////////////////////////////////////////////
//
//		AfxMessageBoxHooked(メッセージ, 
//
int AfxMessageBoxHooked( LPCTSTR message , UINT nType )
{
	// フック関数(MsgBoxHookProc)をインストールする SetWindowHookEx
	MyHookHandle = SetWindowsHookEx ( WH_CBT, MsgBoxHookProc, NULL, GetCurrentThreadId( ) ); 
	return ( AfxMessageBox(message, nType) ); 
	//return  MessageBox("フックメッセージボックス", "HookedMsgBox",MB_OKCANCEL);

}

int AfxMessageBoxHookedID( UINT nIDPrompt, UINT nType )
{
	// フック関数(MsgBoxHookProc)をインストールする SetWindowHookEx
	MyHookHandle = SetWindowsHookEx ( WH_CBT, MsgBoxHookProc, NULL, GetCurrentThreadId( ) ); 
	return ( AfxMessageBox(nIDPrompt, nType) ); 
	//return  MessageBox("フックメッセージボックス", "HookedMsgBox",MB_OKCANCEL);

}
