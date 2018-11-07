// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

// 下で指定された定義の前に対象プラットフォームを指定しなければならない場合、以下の定義を変更してください。
// 異なるプラットフォームに対応する値に関する最新情報については、MSDN を参照してください。
#ifndef WINVER				// Windows XP 以降のバージョンに固有の機能の使用を許可します。
#define WINVER 0x0501		// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_WINNT		// Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501	// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_WINDOWS 0x0410 // これを Windows Me またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_IE			// IE 6.0 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_IE 0x0600	// これを IE. の他のバージョン向けに適切な値に変更してください。
#endif

#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから使用されていない部分を除外します。
// Windows ヘッダー ファイル:
//#include <windows.h>
#include <afxwin.h>



// TODO: プログラムに必要な追加ヘッダーをここで参照してください。
//#undef GCTLIDLL


//▼1.0.0.73 GenApi3.0
#define _GENAPI3


//●_GENAPI3が有効の時
//追加のインクルードディレクトリ
//C:\GenICam_Development\GenICam_V3_0_2-Win32_i86_VS120-Release-Development\library\CPP\include
//追加のライブラリディレクトリ
//C:\GenICam_Development\GenICam_V3_0_2-Win32_i86_VS120-Release-Development\library\CPP\lib\Win32_i86
//追加の依存ファイル
//GCBase_MD_VC120_v3_0.lib
//GenApi_MD_VC120_v3_0.lib
//プリプロセッサの定義(Debugのみ)
//GENICAM_USER_ALWAYS_LINK_RELEASE

//●_GENAPI3が無効の時
//追加のインクルードディレクトリ
//C:\Program Files\GenICam_v2_4\library\CPP\include
//追加のライブラリディレクトリ
//C:\Program Files\GenICam_v2_4\library\CPP\lib\Win32_i86

//追加の依存ファイル
//GCBase_MD_VC80_v2_4.lib
//GenApi_MD_VC80_v2_4.lib

//※StGenICamLibだけでなくStGenFactoryScriptでも行う。

#ifdef _GENAPI3
#pragma comment(lib,"GCBase_MD_VC120_v3_0.lib")
#pragma comment(lib,"GenApi_MD_VC120_v3_0.lib")
#ifdef _DEBUG
#define GENICAM_USER_ALWAYS_LINK_RELEASE
#endif
#include <TLI/TLI_v3.h>
#else
#pragma comment(lib,"GCBase_MD_VC80_v2_4.lib")
#pragma comment(lib,"GenApi_MD_VC80_v2_4.lib")
#include <TLI/TLI.h>
#endif
//▲1.0.0.73 GenApi3.0


//#include <TLI/TLI.h>
#include <GenApi/GenApi.h>
//#include <GenApi/EventAdapterGeneric.h>
using namespace GenICam;
//using namespace GenICam::Client;
using namespace GenApi;

#define ST_GENICAMLIB_EXPORTS


