#pragma once
#include "shlwapi.h"
class CStCommon
{
private:
	//����ăC���X�^���X�����Ȃ��悤�ɃR���X�g���N�^��private�Ɏw��
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
	//�p�X�Ɗg���q��������EXE�t�@�C�������擾
	static DWORD GetExeName(LPTSTR szExeName, rsize_t size)
	{
		DWORD dwError = NO_ERROR;
		do
		{
			//EXE�t�@�C���̃t���p�X���擾
			if(!GetModuleFileName(NULL, szExeName, size))
			{
				dwError = GetLastError();
				break;
			}

			//�p�X���폜
			PathStripPath(szExeName);

			//�g���q���폜
			PathRemoveExtension(szExeName);

		}while(FALSE);
		return(dwError);
	};
	//��2011/03/04 v1.00 Beta07 �������̃��\�[�X��
	static BOOL IsAfterOrEqualToVista()
	{
		//�o�[�W�������擾
		DWORD dwMajor = 0;
		DWORD dwMinor = 0;
		GetOSVersion(&dwMajor, &dwMinor);
		return(6 <= dwMajor);
	};
	//��2011/03/04 v1.00 Beta07 �������̃��\�[�X��
	//ini�t�@�C���̕ۑ���t�H���_���擾(�Ō��\�͂Ȃ�)

	
	//LogFile��Ini�t�@�C���͓���t�H���_
	static DWORD GetLogFilePath(LPTSTR szPath, rsize_t size)
	{
		return GetIniFilePath( szPath, size);
	}

	static DWORD GetExeFilePath(LPTSTR szPath, rsize_t size)
	{
		DWORD dwError = NO_ERROR;
		do
		{
			//EXE�t�@�C���̃t���p�X���擾
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
			//�o�[�W�������擾
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

				//EXE�t�@�C���̃t���p�X���擾
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
				//EXE�t�@�C����(�g���q�Ȃ�)���擾
				TCHAR szExeName[MAX_PATH];
				dwError = GetExeName(szExeName, MAX_PATH);
				if(NO_ERROR != dwError)	break;

				//����t�H���_���擾����
				dwError = SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath);	//HRESULT
				if(NO_ERROR != dwError)	break;

				//�������t�H���_
				_tcscat_s(szPath, size, TEXT("\\"));
				_tcscat_s(szPath, size, m_szManufacturer);
				
				//�A�v�����t�H���_
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


	//ini�t�@�C�������擾����
	static DWORD GetIniFileName(LPTSTR szIniFileName, rsize_t size)
	{
		DWORD dwError = NO_ERROR;

		GetModuleFileName(NULL,szIniFileName,MAX_PATH);
		_tcscpy_s(_tcsrchr(szIniFileName,'.'),size-wcslen(szIniFileName),_T(""));
		_tcscat_s(szIniFileName,size,_T(".ini"));

		//do
		//{
		//	//EXE�����擾
		//	TCHAR szExeName[MAX_PATH];
		//	dwError = GetExeName(szExeName, MAX_PATH);
		//	if(NO_ERROR != dwError) break;

		//	//�p�X���擾
		//	dwError = GetIniFilePath(szIniFileName, size);
		//	if(NO_ERROR != dwError) break;


		//	//�t�H���_�[���쐬
		//	SHCreateDirectoryEx(NULL, szIniFileName, NULL);

		//	//�t�@�C�����𐶐�
		//	//��2011/03/04 v1.00 Beta07 ini�t�@�C�������s��C��
		//	//_tcscpy_s(szIniFileName, size, TEXT("\\"));
		//	//_tcscpy_s(szIniFileName, size, szExeName);
		//	//_tcscpy_s(szIniFileName, size, TEXT(".ini"));
		//	_tcscat_s(szIniFileName, size, TEXT("\\"));
		//	_tcscat_s(szIniFileName, size, szExeName);
		//	_tcscat_s(szIniFileName, size, TEXT(".ini"));
		//	//��2011/03/04 v1.00 Beta07 ini�t�@�C�������s��C��
		//}while(FALSE);
		return(dwError);
	};

	//��2012/07/18 v1.21 Beta03 FPGA�������ݒǉ�
	//rbf�t�@�C�������擾����
	//��2012/08/02 v1.23 Beta01 FPGA�����݃��O�o�͒ǉ�
	//static DWORD GetrbfFileName(LPTSTR szrbfFileName, rsize_t size, LPTSTR szCameraName, LPTSTR szrbf)
	static DWORD GetrbfFilePath(LPTSTR szrbfFilePath, rsize_t size, LPTSTR szCameraName, LPTSTR szrbfFileName)
	//��2012/08/02 v1.23 Beta01 FPGA�����݃��O�o�͒ǉ�
	{
		DWORD dwError = NO_ERROR;
		
		//��2012/08/07 v1.23 Beta08 FPGA�t�@�C���Ǎ��ݏC��
		//TCHAR	szIniFilePath[MAX_PATH];
		TCHAR	szrbfPath[MAX_PATH];
		//��2012/08/07 v1.23 Beta08 FPGA�t�@�C���Ǎ��ݏC��
		CString strFileName,strFileFullPath;
		
		WIN32_FIND_DATA FindFileData;
		do
		{
			//��2012/07/31 v1.23 Beta01 HCB FPGA�t�@�C�����ύX
			/*
			//�p�X���擾
			dwError = GetIniFilePath(szrbfFileName, size);
			if(NO_ERROR != dwError) break;

			//��2012/07/31 v1.23 Beta01 HCB FPGA�t�@�C�����ύX
			//_tcscat_s(szrbfFileName, size, TEXT("\\rbf"));
			_tcscat_s(szrbfFileName, size, TEXT("\\rbf\\"));
			_tcscat_s(szrbfFileName, size, szCameraName);

			//�t�H���_�[���쐬
			SHCreateDirectoryEx(NULL, szrbfFileName, NULL);

			//�t�@�C�����𐶐�
			_tcscat_s(szrbfFileName, size, TEXT("\\"));
			//_tcscat_s(szrbfFileName, size, szCameraName);
			//_tcscat_s(szrbfFileName, size, TEXT(".rbf"));
			_tcscat_s(szrbfFileName, size, TEXT("*.rbf"));
			//��2012/07/31 v1.23 Beta01 HCB FPGA�t�@�C�����ύX
			*/

			//��2012/08/01 v1.22 Beta01 FPGA�����ݏC��
			//�p�X���擾
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
			//��2012/08/01 v1.22 Beta01 FPGA�����ݏC��

			//rbf�t�@�C������
			_tcscat_s(szrbfFilePath, size, TEXT("\\rbf\\"));
			_tcscat_s(szrbfFilePath, size, szCameraName);

			//��2012/08/03 v1.23 Beta03 FPGA�����ݏ����C��
			//�t�H���_�[���쐬
			SHCreateDirectoryEx(NULL, szrbfFilePath, NULL);
			//��2012/08/03 v1.23 Beta03 FPGA�����ݏ����C��

			//��2012/08/07 v1.23 Beta08 FPGA�t�@�C���Ǎ��ݏC��
			_tcscpy_s(szrbfPath, size, szrbfFilePath);
			//��2012/08/07 v1.23 Beta08 FPGA�t�@�C���Ǎ��ݏC��

			_tcscat_s(szrbfFilePath, MAX_PATH, TEXT("\\*_*_*.rbf"));

			FindFirstFile(szrbfFilePath, &FindFileData);
			strFileName = FindFileData.cFileName;

			//��2012/08/02 v1.23 Beta01 FPGA�����݃��O�o�͒ǉ�
			//rbf�t�@�C����
			_stprintf(szrbfFileName,TEXT("%s"),strFileName);
			//��2012/08/02 v1.23 Beta01 FPGA�����݃��O�o�͒ǉ�

			//�t���p�X�쐬
			//��2012/08/07 v1.23 Beta08 FPGA�t�@�C���Ǎ��ݏC��
			//dwError = GetIniFilePath(szIniFilePath, MAX_PATH);
			//��2012/08/03 v1.23 Beta03 FPGA�����ݏ����C��
			//_tcscat_s(szIniFilePath, MAX_PATH, TEXT("\\rbf\\FZ-SC\\"));
			//_tcscat_s(szIniFilePath, size, TEXT("\\rbf\\"));
			//_tcscat_s(szIniFilePath, size, szCameraName);
			//_tcscat_s(szIniFilePath, size, TEXT("\\"));
			_tcscat_s(szrbfPath, size, TEXT("\\"));
			//��2012/08/03 v1.23 Beta03 FPGA�����ݏ����C��
			//strFileFullPath = CString(szIniFilePath) + strFileName;
			strFileFullPath = CString(szrbfPath) + strFileName;
			//��2012/08/07 v1.23 Beta08 FPGA�t�@�C���Ǎ��ݏC��
			_stprintf(szrbfFilePath,TEXT("%s"),strFileFullPath);
			//��2012/07/31 v1.23 Beta01 HCB FPGA�t�@�C�����ύX
		}while(FALSE);
		return(dwError);
	};
	//��2012/07/18 v1.21 Beta03 FPGA�������ݒǉ�


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
//�t�H���_�[���쐬
SHCreateDirectoryEx(NULL, szPath, NULL);


*/