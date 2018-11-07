
// StGenFactoryScript.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StGenFactoryScriptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStGenFactoryScriptApp

BEGIN_MESSAGE_MAP(CStGenFactoryScriptApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CStGenFactoryScriptApp コンストラクション

CStGenFactoryScriptApp::CStGenFactoryScriptApp()
{
	// 再起動マネージャーをサポートします
	//m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CStGenFactoryScriptApp オブジェクトです。

CStGenFactoryScriptApp theApp;


// CStGenFactoryScriptApp 初期化
//#define EURESYS_VIRTUAL_COM_PORT	"COM2"
BOOL CStGenFactoryScriptApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ダイアログにシェル ツリー ビューまたはシェル リスト ビュー コントロールが
	// 含まれている場合にシェル マネージャーを作成します。
//	CShellManager *pShellManager = new CShellManager;
//	MCSTATUS Status = McOpenDriver(NULL);

	//Status = McSetParamStr(MC_BOARD + 0, MC_SerialControlA, "COM2");
 //   Status = McSetParamStr(MC_BOARD + 0, MC_SerialControlA, EURESYS_VIRTUAL_COM_PORT);
	//if( Status!=MC_OK )
	//	return FALSE;


	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));

	CStGenFactoryScriptDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//  記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <キャンセル> で消された時のコードを
		//  記述してください。
	}

	// 上で作成されたシェル マネージャーを削除します。
	//if (pShellManager != NULL)
	//{
	//	delete pShellManager;
	//}

	// ダイアログは閉じられました。アプリケーションのメッセージ ポンプを開始しないで
	//  アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}



int CStGenFactoryScriptApp::ExitInstance()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
    //MCSTATUS Status = McCloseDriver();

	return CWinApp::ExitInstance();
}
