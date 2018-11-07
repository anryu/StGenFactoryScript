#include "StdAfx.h"
#include <windows.h>
#include "StPTouch.h"
#include "StCommon.h"

#include "StDeviceEBus.h"
//▼1.0.0.1015
#include "StDeviceMultiCam.h"
//▲1.0.0.1015

#include "HookMsgBox.h"

#include "Common.h"

//#import "C:\\Program Files\\Common Files\\Brother\\b-PAC\\bpac.dll"
//using namespace bpac;

//IDocumentPtr pObjDoc;


StPTouch::StPTouch(void)
{
	Initialize();
}

StPTouch::StPTouch(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
{
	Initialize();
}

StPTouch::~StPTouch(void)
{
	m_vIDTable.clear();
}

void StPTouch::Initialize(void)
{
	m_szDeviceModelName = _T("");
	m_szHinmokuCode = _T("");
	m_szSerialNumber = _T("");

	m_szLabel = _T("");
	m_nConfirmMessage = 0;

	////Ptouch
	//CoInitialize(NULL);
 
//	IDocumentPtr pObjDoc(__uuidof(bpac::Document));
	//__uuidof(bpac::Document)をメインスレッドで取得する必要がある？？
	//別スレッドだとフリーズしてしまう。
	//pObjDoc =  (IDocumentPtr)__uuidof(bpac::Document);
	//pObjDoc =  (IDocumentPtr)__uuidof(Document);

	//▼1.0.0.1047
	m_szHinmokuFileName = _T("");
	//▲1.0.0.1047
}


//-------------------------------------------------------------------------------
//検査実行
//print
//予約語
//MACADDRESS
//SERIALNUMBER
//QRLONG
//QRSHORT
//MODELNAME
//-------------------------------------------------------------------------------
//メインスレッドで宣言必要の為
//とりあえずg_pObjDocをグローバル変数にした。
//
//extern	IDocumentPtr pObjDoc;

BOOL StPTouch::Execute(void)
{
	BOOL bReval = FALSE;
	/*
	if( m_nConfirmMessage!=0 )
	{
		if( IDYES != AfxMessageBoxHooked(_T("P-touchラベル印刷しますか?"), MB_ICONQUESTION | MB_YESNO) )
		{
			return TRUE;
		}
	}




	try
	{
		//HRESULT aResult = CoInitialize(NULL);
 
		//IDocumentPtr pObjDoc(__uuidof(bpac::Document));

		TCHAR szLabelFullFileName[MAX_PATH];
		CStCommon::GetExeFilePath(szLabelFullFileName, _countof(szLabelFullFileName));
		_tcscat_s(szLabelFullFileName, _countof(szLabelFullFileName), _T("\\CONFIG\\PTouch\\"));
		_tcscat_s(szLabelFullFileName, _countof(szLabelFullFileName), m_szLabel);

		CString strPath = szLabelFullFileName;
		BSTR bstrPath = strPath.AllocSysString();
		if (pObjDoc->Open(bstrPath))
		{

			for( int i=0; i<m_vIDTable.size(); i++ )
			{
				CString szID = m_vIDTable[i].szText;
				CString szText = m_vIDTable[i].szText;
				if( szID==_T("MACADDRESS") )
				{
					if( m_DeviceBase->m_ClassName==_T("StDeviceEBus") )
					{
						StDeviceEBus *aDevice = (StDeviceEBus *)m_DeviceBase;
						PvString aMACAddress;
						aDevice->GetMACAddressStr(aMACAddress);
						szText = aMACAddress.GetUnicode();
						//":"から"-"に変換
						szText.Replace(':', '-');
					}
				}
				else
				if( szID==_T("SERIALNUMBER") )
				{
					szText = m_szSerialNumber;
				}
				else
				if( szID==_T("QRLONG") )
				{
					szText = m_szSerialNumber + _T("_") + m_szHinmokuCode + _T("_") + m_szDeviceModelName;
				}
				else
				if( szID==_T("QRSHORT") )
				{
					szText = m_szSerialNumber + _T("_") + m_szHinmokuCode;
				}
				else
				if( szID==_T("MODELNAME") )
				{
					szText = m_szDeviceModelName;
				}
				if( pObjDoc->GetObject(_bstr_t(m_vIDTable[i].szID)) )
				{
					pObjDoc->GetObject(_bstr_t(m_vIDTable[i].szID))->PutText(szText.AllocSysString());
				}
			}

			// 印刷設定開始
			pObjDoc->StartPrint(_bstr_t(_T("")), bpac::bpoDefault);
			// 印刷ジョブの追加 (1部印刷)
			pObjDoc->PrintOut(1, bpac::bpoDefault);
			// 印刷設定終了(=印刷開始)
			pObjDoc->EndPrint();
			// テンプレートクローズ
			pObjDoc->Close();
			bReval = TRUE;
		}
		else
		{

		}
	}
	catch(_com_error e)
	{
		AfxMessageBoxHooked(e.ErrorMessage(),MB_OK);
	}

	//AllocSysStringを行ったあとの解放必要？？SysFreeString(bstr);
	*/

	return bReval;
}

BOOL StPTouch::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);

	if( szFunction=="Execute" )
	{
		bReval = Execute();
	}
	else
	if( szFunction=="Print" )
	{
		bReval = Execute();
	}
	else
	if( szFunction=="Clear" )
	{
		m_vIDTable.clear();
		bReval = TRUE;
	}
	else
	if( szFunction=="SetLabel" )
	{
		m_szLabel = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetDeviceModelName" )
	{
		m_szDeviceModelName = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetConfirmMessage" )
	{
		m_nConfirmMessage = _ttoi(szArgument);
		bReval = TRUE;
	}
	//▼1.0.0.1047
	else
	if( szFunction=="SetHinmokuFileName" )
	{
		m_szHinmokuFileName = szArgument;
		bReval = TRUE;
	}
	//▲1.0.0.1047
	else
	if( szFunction=="AddID" )	//必要なし
	{
		//S_PTOUCH_TABLE aValue;
		//aValue.szID = szArgument;
		//aValue.szText = _T("");
		//m_vIDTable.push_back(aValue);
		//bReval = TRUE;
	}
	else
	{
		S_PTOUCH_TABLE aValue;
		aValue.szID = szFunction;
		aValue.szText = szArgument;
		m_vIDTable.push_back(aValue);
		bReval = TRUE;

		//for( int i=0; i<m_vIDTable.size(); i++ )
		//{

		//	if( szFunction == m_vIDTable[i].szID )
		//	{
		//		m_vIDTable[i].szText = szArgument;
		//		bReval = TRUE;
		//		break;
		//	}
		//}
	}

	return bReval;

}


//-------------------------------------------------------------------------------
BOOL StPTouch::GetID(INT nIndex, CString &szID)
{
	if( nIndex>=m_vIDTable.size() ) return FALSE;
	szID = m_vIDTable[nIndex].szID;
	return TRUE;
}
BOOL StPTouch::GetText(CString szID, CString &szText)
{
	BOOL bReval = FALSE;
	for( int i=0; i<m_vIDTable.size(); i++ )
	{
		if( m_vIDTable[i].szID==szID )
		{
			szText = m_vIDTable[i].szText;
			GetTextFromText(szText, szText);
			bReval = TRUE;
			break;
		}
	}
	return bReval;
}

BOOL StPTouch::GetTextFromText(CString szInText, CString &szOutText)
{
	BOOL bReval=TRUE;
	if( szInText==_T("MACADDRESS") && m_DeviceBase )
	{
		if( m_DeviceBase->m_ClassName==_T("StDeviceEBus") )
		{
			StDeviceEBus *aDevice = (StDeviceEBus *)m_DeviceBase;
			PvString aMACAddress;
			//▼1.0.0.1015
			//aDevice->GetMACAddressStr(aMACAddress);
			aDevice->GetMACAddressCapitalStr(aMACAddress);
			//▲1.0.0.1015
			szOutText = aMACAddress.GetUnicode();
			//":"から"-"に変換
			szOutText.Replace(':', '-');
		}
	}
	else
	if( szInText==_T("SERIALNUMBER") )
	{
		//▼1.0.0.1047
		//▼1.0.0.1015
		//if( m_DeviceBase->m_ClassName==_T("StDeviceMultiCam") )
		//{
		//	StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
		//	StString aSerialNumber;
		//	aDevice->GetSerialNumber(aSerialNumber);
		//	m_szSerialNumber = aSerialNumber.GetAscii();
		//}
		//szOutText = m_szSerialNumber;
		//▲1.0.0.1015
		CString szSerialNumber;
		GetSerialNumber2(szSerialNumber);
		szOutText = szSerialNumber;
		//▲1.0.0.1047

	}
	else
	if( szInText==_T("QRLONG") )
	{
		//▼1.0.0.1047
		////▼1.0.0.1015
		//if( m_DeviceBase->m_ClassName==_T("StDeviceMultiCam") )
		//{
		//	StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
		//	StString aDeviceModelName;
		//	aDevice->GetDeviceModelName(aDeviceModelName);
		//	m_szDeviceModelName = aDeviceModelName.GetAscii();
		//	StString aSerialNumber;
		//	aDevice->GetSerialNumber(aSerialNumber);
		//	m_szSerialNumber = aSerialNumber.GetAscii();
		//	StString aHinmokuCode;
		//	aDevice->GetHinmokuCode(aHinmokuCode);
		//	m_szHinmokuCode = aHinmokuCode.GetAscii();
		//}
		////▲1.0.0.1015
		//szOutText = m_szSerialNumber + _T("_") + m_szHinmokuCode + _T("_") + m_szDeviceModelName;

		CString szSerialNumber = _T("");
		CString szHinmokuCode = _T("");
		CString szDeviceModelName = _T("");
		GetSerialNumber2(szSerialNumber);
		GetHinmokuCode2(szHinmokuCode);
		GetDeviceModelName2(szDeviceModelName);
		szOutText = szSerialNumber + _T("_") + szHinmokuCode + _T("_") + szDeviceModelName;
		//▲1.0.0.1047
	}
	else
	if( szInText==_T("QRSHORT") )
	{
		//▼1.0.0.1047
		////▼1.0.0.1015
		//if( m_DeviceBase->m_ClassName==_T("StDeviceMultiCam") )
		//{
		//	StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
		//	StString aSerialNumber;
		//	aDevice->GetSerialNumber(aSerialNumber);
		//	m_szSerialNumber = aSerialNumber.GetAscii();
		//	StString aHinmokuCode;
		//	aDevice->GetHinmokuCode(aHinmokuCode);
		//	m_szHinmokuCode = aHinmokuCode.GetAscii();
		//}
		////▲1.0.0.1015
		//szOutText = m_szSerialNumber + _T("_") + m_szHinmokuCode;
		CString szSerialNumber = _T("");
		CString szHinmokuCode = _T("");
		GetSerialNumber2(szSerialNumber);
		GetHinmokuCode2(szHinmokuCode);
		szOutText = szSerialNumber + _T("_") + szHinmokuCode;
		//▲1.0.0.1047
	}
	else
	if( szInText==_T("MODELNAME") )
	{
		//▼1.0.0.1047
		//▼1.0.0.1015
		//if( m_DeviceBase->m_ClassName==_T("StDeviceMultiCam") )
		//{
		//	StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
		//	StString aDeviceModelName;
		//	aDevice->GetDeviceModelName(aDeviceModelName);
		//	m_szDeviceModelName = aDeviceModelName.GetAscii();
		//}
		//▲1.0.0.1015
		//szOutText = m_szDeviceModelName;
		//まず品目コードから取得する。
		CString szDeviceModelName = _T("");
		GetDeviceModelName2(szDeviceModelName);
		szOutText = szDeviceModelName;
		//▲1.0.0.1047
	}
	else
	{
		szOutText = szInText;
	}
	return bReval;
}

//▼1.0.0.1047
BOOL StPTouch::GetHinmokuCode2(CString &szHinmokuCode)
{
	BOOL bReval=FALSE;
	if( m_szHinmokuCode.IsEmpty() )
	{
		StString szText;
		bReval = GetSerialNumber(szText);
		if( bReval ) szHinmokuCode = szText.GetUnicode();
	}
	if( !bReval )
	{
		szHinmokuCode = m_szHinmokuCode;
		bReval=TRUE;
	}
	return bReval;
}

BOOL StPTouch::GetSerialNumber2(CString &szSerialNumber)
{
	BOOL bReval=FALSE;
	if( m_szSerialNumber.IsEmpty() )
	{
		StString szText;
		bReval = GetSerialNumber(szText);
		if( bReval ) szSerialNumber = szText.GetUnicode();
	}
	if( !bReval )
	{
		szSerialNumber = m_szSerialNumber;
		bReval=TRUE;
	}
	return bReval;
}

BOOL StPTouch::GetDeviceModelName2(CString &szDeviceModelName)
{
	BOOL bReval=FALSE;

	if( m_szDeviceModelName.IsEmpty() )
	{
		//品目コード対応ファイルから取得
		CString szModelName;
		bReval = GetModelNameFromHinmokuFile( szModelName );
		if( bReval ) szDeviceModelName = szModelName;

		//スクリプト記述のModelNameから取得
		if( !bReval )
		{
			StString szText;
			bReval = GetDeviceModelName(szText);
			if( bReval ) szDeviceModelName = szText.GetUnicode();
		}
	}

	if( !bReval )
	{
		szDeviceModelName = m_szDeviceModelName;
		bReval=TRUE;
	}
	return bReval;
}

BOOL StPTouch::GetModelNameFromHinmokuFile( CString &szModelName )
{
	BOOL bReval=FALSE;
	if( m_szHinmokuFileName.GetLength()>0 )
	{

		CString szHinmokuCode;
		GetHinmokuCode2(szHinmokuCode);
		if( szHinmokuCode.GetLength()>0 )
		{

			CString szFileName = m_szHinmokuFileName;
			if( szFileName.Mid(2,2)!=_T(":\\") )
			{
				CString szFilePath;
				GetModuleFilePath(szFilePath);
				szFileName = szFilePath + _T("\\") + szFileName;
			}
			CStdioFile stdFile;
			if(stdFile.Open( szFileName, CFile::modeRead,NULL)!=0)
			{
				CString tmp;
				stdFile.SeekToBegin();
				while(stdFile.ReadString(tmp))
				{
					tmp.Trim();
					if( szHinmokuCode==tmp.Left(szHinmokuCode.GetLength()) )
					{
						tmp = tmp.Mid(szHinmokuCode.GetLength());
						tmp.Trim();
						if( tmp.Left(1).Compare(_T("="))==0 )
						{
							szModelName = tmp.Mid(1).Trim();
							bReval = TRUE;
							break;
						}
					}
				}
				stdFile.Close();
			}
		}

	}
	return bReval;
}
//▲1.0.0.1047
