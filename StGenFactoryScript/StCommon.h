#pragma once
#include "shlwapi.h"
class CStCommon
{
private:
	//誤ってインスタンス化しないようにコンストラクタをprivateに指定
	CStCommon(void);
	~CStCommon(void);
	static LPCTSTR m_szManufacturer;

public:
	static DWORD GetOSVersion(PDWORD pdwMajor, PDWORD pdwMinor)
	{
		DWORD dwError = NO_ERROR;
		do
		{
			OSVERSIONINFO osvi = {0};
			osvi.dwOSVersionInfoSize = sizeof(osvi);

			if(!GetVersionEx(&osvi))
			{
				dwError = GetLastError();
				break;
			}

			*pdwMajor = osvi.dwMajorVersion;
			*pdwMinor = osvi.dwMinorVersion;
		}while(FALSE);
		return(dwError);
	};
	//パスと拡張子を除いたEXEファイル名を取得
	static DWORD GetExeName(LPTSTR szExeName, rsize_t size)
	{
		DWORD dwError = NO_ERROR;
		do
		{
			//EXEファイルのフルパスを取得
			if(!GetModuleFileName(NULL, szExeName, size))
			{
				dwError = GetLastError();
				break;
			}

			//パスを削除
			PathStripPath(szExeName);

			//拡張子を削除
			PathRemoveExtension(szExeName);

		}while(FALSE);
		return(dwError);
	};
	//▼2011/03/04 v1.00 Beta07 文字情報のリソース化
	static BOOL IsAfterOrEqualToVista()
	{
		//バージョンを取得
		DWORD dwMajor = 0;
		DWORD dwMinor = 0;
		GetOSVersion(&dwMajor, &dwMinor);
		return(6 <= dwMajor);
	};
	//▲2011/03/04 v1.00 Beta07 文字情報のリソース化
	//iniファイルの保存先フォルダを取得(最後の\はなし)

	
	//LogFileとIniファイルは同一フォルダ
	static DWORD GetLogFilePath(LPTSTR szPath, rsize_t size)
	{
		return GetIniFilePath( szPath, size);
	}

	static DWORD GetExeFilePath(LPTSTR szPath, rsize_t size)
	{
		DWORD dwError = NO_ERROR;
		do
		{
			//EXEファイルのフルパスを取得
			TCHAR szExeFullPath[MAX_PATH];
			if(!GetModuleFileName(NULL, szExeFullPath, MAX_PATH))
			{
				dwError = GetLastError();
				break;
			}

			_tcscpy_s(szPath, size, szExeFullPath);
			if(!PathRemoveFileSpec(szPath))
			{
				dwError = GetLastError();
				break;
			}
		}while(0);
		return(dwError);
	}

	static DWORD GetIniFilePath(LPTSTR szPath, rsize_t size)
	{
		DWORD dwError = NO_ERROR;
		do
		{
			//バージョンを取得
			DWORD dwMajor = 0;
			DWORD dwMinor = 0;
			dwError = GetOSVersion(&dwMajor, &dwMinor);
			if(NO_ERROR != dwError)
			{
				break;
			}

			if(dwMajor < 6)
			{
				//XP

				//EXEファイルのフルパスを取得
				TCHAR szExeFullPath[MAX_PATH];
				if(!GetModuleFileName(NULL, szExeFullPath, MAX_PATH))
				{
					dwError = GetLastError();
					break;
				}

				_tcscpy_s(szPath, size, szExeFullPath);
				if(!PathRemoveFileSpec(szPath))
				{
					dwError = GetLastError();
					break;
				}
			}
			else
			{
				//Vista, 7
				//EXEファイル名(拡張子なし)を取得
				TCHAR szExeName[MAX_PATH];
				dwError = GetExeName(szExeName, MAX_PATH);
				if(NO_ERROR != dwError)	break;

				//特殊フォルダを取得する
				dwError = SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath);	//HRESULT
				if(NO_ERROR != dwError)	break;

				//製造元フォルダ
				_tcscat_s(szPath, size, TEXT("\\"));
				_tcscat_s(szPath, size, m_szManufacturer);
				
				//アプリ名フォルダ
				_tcscat_s(szPath, size, TEXT("\\"));
				_tcscat_s(szPath, size, szExeName);
			}
		}while(FALSE);
		return(dwError);
	};


	static DWORD GetWorkerFileName(LPTSTR szPath, rsize_t size)
	{
		DWORD dwError = GetIniFilePath( szPath, size);

		_tcscat_s( szPath, size, _T("\\Worker.txt") );

		return dwError;
	}


	//iniファイル名を取得する
	static DWORD GetIniFileName(LPTSTR szIniFileName, rsize_t size)
	{
		DWORD dwError = NO_ERROR;

		GetModuleFileName(NULL,szIniFileName,MAX_PATH);
		_tcscpy_s(_tcsrchr(szIniFileName,'.'),size-wcslen(szIniFileName),_T(""));
		_tcscat_s(szIniFileName,size,_T(".ini"));

		//do
		//{
		//	//EXE名を取得
		//	TCHAR szExeName[MAX_PATH];
		//	dwError = GetExeName(szExeName, MAX_PATH);
		//	if(NO_ERROR != dwError) break;

		//	//パスを取得
		//	dwError = GetIniFilePath(szIniFileName, size);
		//	if(NO_ERROR != dwError) break;


		//	//フォルダーを作成
		//	SHCreateDirectoryEx(NULL, szIniFileName, NULL);

		//	//ファイル名を生成
		//	//▼2011/03/04 v1.00 Beta07 iniファイル生成不具合修正
		//	//_tcscpy_s(szIniFileName, size, TEXT("\\"));
		//	//_tcscpy_s(szIniFileName, size, szExeName);
		//	//_tcscpy_s(szIniFileName, size, TEXT(".ini"));
		//	_tcscat_s(szIniFileName, size, TEXT("\\"));
		//	_tcscat_s(szIniFileName, size, szExeName);
		//	_tcscat_s(szIniFileName, size, TEXT(".ini"));
		//	//▲2011/03/04 v1.00 Beta07 iniファイル生成不具合修正
		//}while(FALSE);
		return(dwError);
	};

	//▼2012/07/18 v1.21 Beta03 FPGA書き込み追加
	//rbfファイル名を取得する
	//▼2012/08/02 v1.23 Beta01 FPGA書込みログ出力追加
	//static DWORD GetrbfFileName(LPTSTR szrbfFileName, rsize_t size, LPTSTR szCameraName, LPTSTR szrbf)
	static DWORD GetrbfFilePath(LPTSTR szrbfFilePath, rsize_t size, LPTSTR szCameraName, LPTSTR szrbfFileName)
	//▲2012/08/02 v1.23 Beta01 FPGA書込みログ出力追加
	{
		DWORD dwError = NO_ERROR;
		
		//▼2012/08/07 v1.23 Beta08 FPGAファイル読込み修正
		//TCHAR	szIniFilePath[MAX_PATH];
		TCHAR	szrbfPath[MAX_PATH];
		//▲2012/08/07 v1.23 Beta08 FPGAファイル読込み修正
		CString strFileName,strFileFullPath;
		
		WIN32_FIND_DATA FindFileData;
		do
		{
			//▼2012/07/31 v1.23 Beta01 HCB FPGAファイル名変更
			/*
			//パスを取得
			dwError = GetIniFilePath(szrbfFileName, size);
			if(NO_ERROR != dwError) break;

			//▼2012/07/31 v1.23 Beta01 HCB FPGAファイル名変更
			//_tcscat_s(szrbfFileName, size, TEXT("\\rbf"));
			_tcscat_s(szrbfFileName, size, TEXT("\\rbf\\"));
			_tcscat_s(szrbfFileName, size, szCameraName);

			//フォルダーを作成
			SHCreateDirectoryEx(NULL, szrbfFileName, NULL);

			//ファイル名を生成
			_tcscat_s(szrbfFileName, size, TEXT("\\"));
			//_tcscat_s(szrbfFileName, size, szCameraName);
			//_tcscat_s(szrbfFileName, size, TEXT(".rbf"));
			_tcscat_s(szrbfFileName, size, TEXT("*.rbf"));
			//▲2012/07/31 v1.23 Beta01 HCB FPGAファイル名変更
			*/

			//▼2012/08/01 v1.22 Beta01 FPGA書込み修正
			//パスを取得
			//dwError = GetIniFilePath(szrbfFileName, size);
			//if(NO_ERROR != dwError) break;
			TCHAR szExeFullPath[MAX_PATH];
			if(!GetModuleFileName(NULL, szExeFullPath, MAX_PATH))
			{
				dwError = GetLastError();
				break;
			}

			_tcscpy_s(szrbfFilePath, size, szExeFullPath);
			if(!PathRemoveFileSpec(szrbfFilePath))
			{
				dwError = GetLastError();
				break;
			}
			//▲2012/08/01 v1.22 Beta01 FPGA書込み修正

			//rbfファイル検索
			_tcscat_s(szrbfFilePath, size, TEXT("\\rbf\\"));
			_tcscat_s(szrbfFilePath, size, szCameraName);

			//▼2012/08/03 v1.23 Beta03 FPGA書込み処理修正
			//フォルダーを作成
			SHCreateDirectoryEx(NULL, szrbfFilePath, NULL);
			//▲2012/08/03 v1.23 Beta03 FPGA書込み処理修正

			//▼2012/08/07 v1.23 Beta08 FPGAファイル読込み修正
			_tcscpy_s(szrbfPath, size, szrbfFilePath);
			//▲2012/08/07 v1.23 Beta08 FPGAファイル読込み修正

			_tcscat_s(szrbfFilePath, MAX_PATH, TEXT("\\*_*_*.rbf"));

			FindFirstFile(szrbfFilePath, &FindFileData);
			strFileName = FindFileData.cFileName;

			//▼2012/08/02 v1.23 Beta01 FPGA書込みログ出力追加
			//rbfファイル名
			_stprintf(szrbfFileName,TEXT("%s"),strFileName);
			//▲2012/08/02 v1.23 Beta01 FPGA書込みログ出力追加

			//フルパス作成
			//▼2012/08/07 v1.23 Beta08 FPGAファイル読込み修正
			//dwError = GetIniFilePath(szIniFilePath, MAX_PATH);
			//▼2012/08/03 v1.23 Beta03 FPGA書込み処理修正
			//_tcscat_s(szIniFilePath, MAX_PATH, TEXT("\\rbf\\FZ-SC\\"));
			//_tcscat_s(szIniFilePath, size, TEXT("\\rbf\\"));
			//_tcscat_s(szIniFilePath, size, szCameraName);
			//_tcscat_s(szIniFilePath, size, TEXT("\\"));
			_tcscat_s(szrbfPath, size, TEXT("\\"));
			//▲2012/08/03 v1.23 Beta03 FPGA書込み処理修正
			//strFileFullPath = CString(szIniFilePath) + strFileName;
			strFileFullPath = CString(szrbfPath) + strFileName;
			//▲2012/08/07 v1.23 Beta08 FPGAファイル読込み修正
			_stprintf(szrbfFilePath,TEXT("%s"),strFileFullPath);
			//▲2012/07/31 v1.23 Beta01 HCB FPGAファイル名変更
		}while(FALSE);
		return(dwError);
	};
	//▲2012/07/18 v1.21 Beta03 FPGA書き込み追加


	static BOOL CheckDirectoryExist(LPCTSTR lpszPath,BOOL bCreate)
	{
	
		BOOL bResult=TRUE;
		WIN32_FIND_DATA wfd;

		HANDLE hFile = FindFirstFile(lpszPath, &wfd);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			bResult = TRUE;
			FindClose(hFile);
#if 0
			OutputDebugString(TEXT("Directory Exist:"));
			OutputDebugString(lpszPath);
			OutputDebugString(TEXT("\n"));
#endif
		}
		else
		{
			if(!bCreate)
			{
				bResult = FALSE;
			}
			else
			{
				TCHAR szTmpPath[MAX_PATH];	
				_tcscpy_s(szTmpPath,_countof(szTmpPath),lpszPath);
				_tcscpy(_tcsrchr(szTmpPath,'\\'),_T(""));
				bResult = CheckDirectoryExist(szTmpPath,bCreate);
				if(bResult)
				{
					bResult = CreateDirectory(lpszPath, NULL);
#if 0
					OutputDebugString(TEXT("Create Directory:"));
					OutputDebugString(lpszPath);
					OutputDebugString(TEXT("\n"));
#endif
				}
			}
		}


		return(bResult);
	}

};
/*
//フォルダーを作成
SHCreateDirectoryEx(NULL, szPath, NULL);


*/