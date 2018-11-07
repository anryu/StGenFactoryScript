// HookMsgBox.h ヘッダファイル
// 1度しか読ませないため
#pragma once

// プロトタイプ宣言
LRESULT CALLBACK MsgBoxHookProc (int nCode, WPARAM wParam ,LPARAM lParam );
int AfxMessageBoxHooked( LPCTSTR message , UINT nType );
int AfxMessageBoxHookedID( UINT nIDPrompt, UINT nType );

