
// StGenFactoryScript.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CStGenFactoryScriptApp:
// このクラスの実装については、StGenFactoryScript.cpp を参照してください。
//

class CStGenFactoryScriptApp : public CWinApp
{
public:
	CStGenFactoryScriptApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CStGenFactoryScriptApp theApp;