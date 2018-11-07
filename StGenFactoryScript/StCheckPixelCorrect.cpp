#include "StdAfx.h"
#include "StCheckPixelCorrect.h"
#include "StInspectMain.h"
#include "StCheckControlRetry.h"
#include "Common.h"


StCheckPixelCorrect::StCheckPixelCorrect(void)
{
	Initialize();
}

StCheckPixelCorrect::StCheckPixelCorrect(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase)
	:StCheckBase( szClassName, szVarName, aDeviceBase )
{
	Initialize();
}

StCheckPixelCorrect::~StCheckPixelCorrect(void)
{
}

void StCheckPixelCorrect::Initialize(void)
{
	m_nMode = 0;
	m_nSelectMode = 0;
	m_szDirectory = "";
	m_szSensorSerialNumber = "";

	m_pControlRetry = NULL;
	m_pInspectionMain = NULL;

}

BOOL ChangeEllitoFormat(LPCTSTR szSrcData, CString &szData)
{
	BOOL bReval = TRUE;
	CString SrcData(szSrcData);
	INT nSrcLen = SrcData.GetLength();
	//USER>�폜
	PTCHAR szUSER = _T("USER>");
	INT nLen = _tcslen(szUSER);
	INT nStart = nSrcLen - nLen + 1;
	for( int i=nStart; i>=0; i-- )
	{
		if( SrcData.Mid(i,nLen).Compare(szUSER)==0 )
		{
			CString szTail = SrcData.Mid(i+nLen);
			SrcData = SrcData.Left(i) + szTail;
		}
	}
	szData = SrcData.Trim();
	return bReval;
}

//-------------------------------------------------------------------------------
//�������s
//-------------------------------------------------------------------------------
BOOL StCheckPixelCorrect::Execute(LPCTSTR szSensorSerialNumber)
{
	if( szSensorSerialNumber )
		SetSensorSerialNumber(szSensorSerialNumber);

	return Execute();
}

BOOL StCheckPixelCorrect::Execute(void)
{
	BOOL bReval = FALSE;
	if( m_nMode==0 )
		bReval = ExecuteEllito();
	return bReval;
}

BOOL StCheckPixelCorrect::ExecuteEllito(void)
{
	BOOL bReval = FALSE;
	m_iJudge = 2;	//2:������

	CString szLastError(_T(""));

	//�t�@�C��������
	CString szFullFileName("");
	INT nSelectMode = m_nSelectMode;
	if( m_szSensorSerialNumber.IsEmpty() )
	{
		nSelectMode = 1;
	}
	BOOL bSerialFileCompare = TRUE;

	CString szDirectory = m_szDirectory;
	if( nSelectMode==0 )
	{
		//�t�@�C���w�胂�[�h�@�V���A���͕K�������Ă���B

		if( szDirectory.Mid(2,2)!=_T(":\\") )
		{
			CString szFilePath;
			GetModuleFilePath(szFilePath);
			szDirectory = szFilePath + _T("\\") + szDirectory;
		}
		CString szFindFileName = szDirectory;
		szFindFileName += _T("\\*.*");
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = FindFirstFile( szFindFileName, &FindFileData );
		if( hFind == INVALID_HANDLE_VALUE )
		{
			szFullFileName = "";
		}
		else
		{
			do
			{
				CString  szFindFile  = FindFileData.cFileName;
				if( szFindFile.Find(m_szSensorSerialNumber)>=0 )
				{
					szFullFileName = szDirectory + _T("\\") + szFindFile;
					break;
				}
			}while( FindNextFile(hFind,&FindFileData) );
		}
	}
	else
	{
		//�t�@�C���I�����[�h
		TCHAR chCurrentDir[MAX_PATH];
		GetCurrentDirectory( _countof(chCurrentDir), chCurrentDir );

		CFileDialog dlgLoad( TRUE,0,0,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
								TEXT("Text File(*.txt)|*.txt|AllFiles(*.*)|*.*||"));
		dlgLoad.m_ofn.lpstrTitle = TEXT("Select file");
		dlgLoad.m_ofn.lpstrDefExt = TEXT("txt");
		dlgLoad.m_ofn.lpstrInitialDir = szDirectory;
		if(dlgLoad.DoModal() != IDOK)
		{
			szFullFileName = "";
		}
		else
		{
			szFullFileName = dlgLoad.m_ofn.lpstrFile;
		}
		SetCurrentDirectory(chCurrentDir);

		//�V���A���Ɣ�r���A����Ă�����NG�ɂ���
		if( !szFullFileName.IsEmpty() && !m_szSensorSerialNumber.IsEmpty() )
		{
			if( szFullFileName.Find(m_szSensorSerialNumber)<0 )
			{
				bSerialFileCompare = FALSE;
				szLastError = _T("File - Serial Compare Error");
			}
		}
	}

	if( bSerialFileCompare )
	{

		//8320bit ��16bit �Ŋ�����0-519 �̒l
		//OpticalBlack�p128bit �摜�p 8192 bit
		DWORD nDpixData[520];	//�K��520����
		memset( nDpixData, 0, sizeof(nDpixData) );

		//2)�t�@�C�������f�����擾
		CStdioFile stdFile;
		if(stdFile.Open( szFullFileName, CFile::modeRead,NULL)!=0)
		{
			CString tmp;
			stdFile.SeekToBegin();
			while(stdFile.ReadString(tmp))
			{
				tmp.TrimLeft();
				tmp.TrimRight();
				//�������nDpixData�ɑ��
				int nPixAddress=0;
				if( !tmp.IsEmpty() && tmp.Left(1).Compare(_T("#")) )
				{
					nPixAddress = _ttoi(tmp);
					nPixAddress += 128;

					//nPixAddress��8320�ȏ�̏ꍇ�A�I�[�o�[�t���[�Ȃ̂ő�����Ȃ�
					if( nPixAddress>=sizeof(nDpixData)/sizeof(nDpixData[0])*16 )
					{
						continue;
					}

					int nBlock = nPixAddress / 16;
					int nRegion = nPixAddress / 64;

					int nFind = 0;
					for (int i = 0; i < 4; i++)
					{
						if ((nRegion * 4 + i) == nBlock)
						{
							//�␳�Y���u���b�N
							int nTemp = nPixAddress % nBlock;
							nDpixData[nBlock] = (0xffff >> nTemp);
							nFind = 1;
						}
						else if (nFind == 1)
						{
							nDpixData[nRegion * 4 + i] = 0xffff;
						}
					}
				}

			}
			stdFile.Close();
		}
		else
		{
			szLastError = _T("File Open Error");
		}

		//3)��f���o�^
		if( szLastError.IsEmpty() )
		{
		
			StCheckControlRetry *aControlRetry = NULL;
			//StSerialComm *aSerialComm = NULL;
			StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
			if( aDevice->IsOpened() )
			{
				//aSerialComm = aDevice->GetSerialComm();
				aControlRetry = (StCheckControlRetry *)aDevice->GetControlRetry();
			}
			CString szSendData;
			//if( aSerialComm )
			if( aControlRetry )
			{
				DWORD dwDef1=0, dwDef2=0, dwDef3=0, dwDef4=0, dwDef5=0;
				aControlRetry->GetCommTimeout(&dwDef1,&dwDef2,&dwDef3,&dwDef4,&dwDef5);
				aControlRetry->SetCommTimeout(50,50,0,0,0);


				INT nDefRetry = aControlRetry->GetRetry();

				//char revText[64], cmdText[64];
				//TCHAR szRcvText[256];
				bReval = TRUE;
				//BYTE byteCmpRevData[] = { 0x0d, 0x0a, 0x55, 0x53, 0x45, 0x52, 0x3e }; //CR LF U S E R >

				aControlRetry->SetRetry(0);
				//aControlRetry->SetRecieveData(_T(" USER>"));
				aControlRetry->SetRecieveData(_T("USER>"));
				for (int i = 0; i < sizeof(nDpixData)/sizeof(nDpixData[0]); i++)
				{
					//�ʐM�t�H�[�}�b�g���Ⴄ�B�B�B
					//szSendData.Format(_T("PDC %i %04x"), i, nDpixData[i]);
					DWORD dwData = (i<<16) | nDpixData[i];
					szSendData.Format(_T("PDC 0x%08x"), dwData );

					//memset( szRcvText, 0, sizeof(szRcvText) );
					//bReval = aSerialComm->SendText( szSendData, szRcvText, _countof(szRcvText) );
					aControlRetry->SetSendData(szSendData);

					if( m_pInspectionMain )
					{
						StInspectMain *pInspectionMain = (StInspectMain *)m_pInspectionMain;
						pInspectionMain->DisplayListBox(szSendData);
					}
					bReval = aControlRetry->Execute();
					if( !bReval )
					{
						szLastError = _T("Control Error");
						break;
					}
//{
//char temp[128];
//sprintf( temp, "RET: USER>\n");
//TRACE( temp );
//sprintf( temp, "%s\n", cmdText );
////sprintf( temp, "%d: %s\n", i , cmdText );
//TRACE( temp );
//}
					//revText�̔�r      ��r�K�v�Ȃ��H
					//CString szReturn;
					//aControlRetry->GetReturn(szReturn);
					//if( szReturn.Compare()!=0 )
					////if( memcmp( revText, byteCmpRevData, sizeof(byteCmpRevData)) )
					//{
					//	bReval = FALSE;
					//	break;
					//}
				}

				//PDI 2
				if( bReval )
				{
					//sprintf( cmdText, "PDI 2" );
					//memset( revText, 0, sizeof(revText) );
					//ret = objRS232C.SendAndRecieve( revText, cmdText, m_RSLOOP, sizeof(byteCmpRevData) );
					//if( ret!=0 )
					//{
					//	bReval = FALSE;
					//}

					aControlRetry->SetRetry(200);
					//aControlRetry->SetTimer(100);
					szSendData.Format(_T("PDI 2"));
					aControlRetry->SetSendData(szSendData);

					if( m_pInspectionMain )
					{
						StInspectMain *pInspectionMain = (StInspectMain *)m_pInspectionMain;
						pInspectionMain->DisplayListBox(szSendData);
					}

					bReval = aControlRetry->Execute();
					if( !bReval )
					{
						szLastError = _T("Control Error");
					}
				}
				//4)��f���ǂݍ��ݔ�r
				if( bReval )
				{
					aControlRetry->SetRetry(0);
					//char byteRevEndData[] = { 0x55, 0x53, 0x45, 0x52, 0x3e, 0x00 }; //U S E R >
					//aControlRetry->SetRecieveData(_T("USER>"));
					aControlRetry->SetRecieveData(_T(""));
					for (int i = 0; i < sizeof(nDpixData)/sizeof(nDpixData[0]); i++)
					{

						//DWORD dwData = i<<16 | nDpixData[i];
						szSendData.Format(_T("RDC %d"), i );


						aControlRetry->SetSendData(szSendData);

						if( m_pInspectionMain )
						{
							StInspectMain *pInspectionMain = (StInspectMain *)m_pInspectionMain;
							pInspectionMain->DisplayListBox(szSendData);
						}

						bReval = aControlRetry->Execute();
						if( !bReval )
						{
							szLastError = _T("Control Error");
							break;
						}

						//�l�擾
						CString szLastRecieveData;
						aControlRetry->GetLastRecieveData(szLastRecieveData);


						//strcpy( cmdText, szSendData );
						//memset( revText, 0, sizeof(revText) );


						////CRLF 0 CRLF USER>    //10�����`
						//ret = objRS232C.SendAndRecieve( revText, cmdText, m_RSLOOP, 10, &byteRevEndData[0] );
						//if( ret!=0 )
						//{
						//	szLastError = _T("Control Error");
						//	bReval = FALSE;
						//	break;
						//}
//{
//	char temp[128];
//	sprintf( temp, "%d: %s\n", i , cmdText );
//	TRACE( temp );
//	sprintf( temp, "rev: %s\n", revText );
//	TRACE( temp );
//}

						//revText�̔�r
						//ChangeTakanoFormat(revText);
						CString szData;
						ChangeEllitoFormat(szLastRecieveData,szData);
						if( nDpixData[i]!=(unsigned long)_ttoi(szData) )
						{
							//szLastError = _T("Compare Error");
							szLastError.Format(_T("Compare Error: %d (%u!=%u)"),i,nDpixData[i],_ttoi(szLastRecieveData) );
							bReval = FALSE;
							break;
						}

					}
				}

				aControlRetry->SetRetry(nDefRetry);
				aControlRetry->SetCommTimeout(dwDef1,dwDef2,dwDef3,dwDef4,dwDef5);
			
				//objRS232C.Close();
			}

		}

	}
	if( szLastError.GetLength()>0 )
		m_szLastError = szLastError;


	if( bReval ) m_iJudge=1;	//1:OK
	else		 m_iJudge=0;	//0:NG

	return bReval;
}

BOOL StCheckPixelCorrect::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);

	if( szFunction=="Directory" || szFunction=="SetDirectory" )
	{
		SetDirectory(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="Mode" || szFunction=="SetMode" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetMode(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SelectMode" || szFunction=="SetSelectMode" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetSelectMode(nData);
		bReval = TRUE;
	}
	//else
	//if( szFunction=="ControlRetry" || szFunction=="SetControlRetry" )
	//{
	//	SetControlRetry((void*)szArgument);
	//	bReval = TRUE;
	//}
	//else
	//if( szFunction=="Execute" )
	//{
	//	bReval = Execute();
	//}

	return bReval;
}
