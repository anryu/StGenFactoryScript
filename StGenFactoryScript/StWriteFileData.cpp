#include "StdAfx.h"
#include "StWriteFileData.h"
#include "StInspectMain.h"


StWriteFileData::StWriteFileData(void)
{
	Initialize();
}

StWriteFileData::StWriteFileData(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void* pParent, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
{
	Initialize();
}

StWriteFileData::~StWriteFileData(void)
{
}

void StWriteFileData::Initialize(void)
{
	m_nAddressMagnification = 1;
	m_nAddressOffset = 0;
	m_szFileName = "";
	m_szFilePath = "";

}

typedef struct _ST_WRITE_ADDRESSDATA
{
	DWORD		dwAddress;		//アドレス
	INT 		nData;			//データ
}ST_WRITE_ADDRESSDATA, *PST_WRITE_ADDRESSDATA;

//-------------------------------------------------------------------------------
//書込み実行
//-------------------------------------------------------------------------------
BOOL StWriteFileData::Execute(void)
{
	BOOL bReval = FALSE;
	m_iJudge = 2;	//2:処理中

	std::vector <ST_WRITE_ADDRESSDATA> vData; 
	do
	{
		//ファイル読込みからデータ作成
		//TCHAR szFullFileName[MAX_PATH];
		CString szFilePath = m_szFilePath;
		if( szFilePath.GetLength()==0 || szFilePath.Left(1)==_T("\\") )
		{
			TCHAR szFullFilePath[MAX_PATH];
			CStCommon::GetExeFilePath(szFullFilePath, _countof(szFullFilePath));
			if( szFilePath.GetLength()==0 ) 
				_tcscat_s(szFullFilePath, _countof(szFullFilePath), _T("\\CONFIG\\Standard\\"));
			else
			{
				_tcscat_s(szFullFilePath, _countof(szFullFilePath), szFilePath);
				//_tcscat_s(szFullFilePath, _countof(szFullFilePath), _T("\\"));
			}
			szFilePath = szFullFilePath;
		}
		if( szFilePath.Right(1)!=_T("\\") )
		{
			szFilePath += _T("\\");
		}
		CString szFullFileName = szFilePath + m_szFileName;


		CStdioFile aFile;
		if( !aFile.Open( szFullFileName, CFile::modeRead, NULL ) )
		{
			bReval = FALSE;
			break;
		}

		ST_WRITE_ADDRESSDATA aData;
		CString tmp;
		while( aFile.ReadString(tmp) )
		{
			tmp.Trim();
			if( !tmp.IsEmpty() && tmp.Left(1).Compare(_T("#"))!=0 )
			{
				INT nCammaPos = tmp.Find(_T(","));
				if( nCammaPos>0 )
				{
					aData.dwAddress = _tcstol( tmp.Left(nCammaPos),NULL,0);
					aData.nData = _tcstol( tmp.Mid(nCammaPos+1),NULL,0);
					vData.push_back(aData);
				}
			}
		}
		aFile.Close();
		if( vData.size()==0 ) break;


		//送信--------------
		if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//GigE用
		{
			bReval = FALSE;
			break;
		}
		else
		if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL用
		{
			bReval = FALSE;
			break;
		}

		else
		if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )	//GenICam用
		{
			//TAKANO
			StDeviceGenICam *aDevice = (StDeviceGenICam *)m_DeviceBase;
			if( !aDevice->IsOpened() ) break;

			for( int i=0; i<vData.size(); i++ )
			{
				DWORD dwAddress = vData[i].dwAddress * m_nAddressMagnification + m_nAddressOffset;
				//▼1.0.0.1060
				//bReval = aDevice->Write(dwAddress,(char *)&vData[i].nData,sizeof(vData[i].nData));
				bReval = aDevice->WriteMain(dwAddress,(char *)&vData[i].nData,sizeof(vData[i].nData));
				//▲1.0.0.1060
				if( !bReval ) break;
			}
		}
		if( bReval ) m_iJudge = 1;

		//bReval = TRUE;
	}while(0);
	if( m_iJudge==2 ) m_iJudge = 0;	//0:NG 1:OK

	return bReval;
}

//-------------------------------------------------------------------------------
//function
//-------------------------------------------------------------------------------
BOOL StWriteFileData::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction == _T("Execute") )
	{
	}
	else
	if( szFunction=="SetAddressMagnification" || szFunction=="AddressMagnification" )
	{
		m_nAddressMagnification = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAddressOffset" || szFunction=="AddressOffset" )
	{
		m_nAddressOffset = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetFileName" || szFunction=="FileName" )
	{
		m_szFileName = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetFilePath" || szFunction=="FilePath" )
	{
		m_szFilePath = szArgument;
		bReval = TRUE;
	}
	return bReval;

}