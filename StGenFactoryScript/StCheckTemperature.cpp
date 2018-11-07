#include "StdAfx.h"
#include "StCheckTemperature.h"
#include "StInspectMain.h"
#include "StCheckControlRetry.h"
//▼1.0.0.1046
#include "StDeviceGenICam.h"
//▲1.0.0.1046


StCheckTemperature::StCheckTemperature(void)
{
	Initialize();
}

StCheckTemperature::StCheckTemperature(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
{
	Initialize();
}

StCheckTemperature::~StCheckTemperature(void)
{
}

void StCheckTemperature::Initialize(void)
{
	m_nMinTemperature = 0;
	m_nMaxTemperature = 0;
	m_nMode = 0;
	m_nResultTemperature = 0;
	//▼1.0.0.1022
	m_szCommand = _T("");
	m_szRecieve = _T("");
	//▲1.0.0.1022

	//▼1.0.0.1046
	m_szSelectorFeature = _T("");
	m_szSelectorSelect = _T("");
	m_dwAddress = 0;
	//▲1.0.0.1046

}


BOOL StCheckTemperature::ExecuteEllito(INT &nValue)
{
	BOOL bReval = FALSE;
	StCheckControlRetry *aControlRetry = NULL;
	INT nDefRetry = -1;
	CString szReturn(_T(""));

	do
	{


		if( !m_DeviceBase ) break;
		StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
		if( !aDevice ) break;
		if( aDevice->IsOpened() )
		{
			aControlRetry = (StCheckControlRetry *)aDevice->GetControlRetry();
		}
		if( !aControlRetry ) break;

		nDefRetry = aControlRetry->GetRetry();
		aControlRetry->SetRetry(0);

		//aControlRetry->SetRecieveData(_T("USER>"));

		aControlRetry->GetReturn(szReturn);
		//▼1.0.0.1022
		//aControlRetry->SetRecieveData(_T(""));
		aControlRetry->SetRecieveData(m_szRecieve);
		//▲1.0.0.1022
		aControlRetry->SetSendData(m_szCommand);

		//if( m_pInspectionMain )
		//{
		//	StInspectMain *pInspectionMain = (StInspectMain *)m_pInspectionMain;
		//	pInspectionMain->DisplayListBox(szSendData);
		//}
		bReval = aControlRetry->Execute();
		if( !bReval )
		{
		//	m_szLastError = _T("Control Error");
			break;
		}
		//▼1.0.0.1022
		//CString szRcvData;
		//aControlRetry->GetLastRecieveData(szRcvData);
		//INT nPos = szRcvData.Find(_T("USER>"));
		//if( nPos>0 )
		//{
		//	nValue = _ttoi(szRcvData.Left(nPos));
		//}

		//BOOL StSerialComm::GetRecieveResultValue( INT &szData )
		aControlRetry->GetLastRecieveValue(nValue);

		//▲1.0.0.1022


	}while(0);
	if( nDefRetry>=0 && aControlRetry)	aControlRetry->SetRetry(nDefRetry);
	if( szReturn.GetLength()>0 && aControlRetry)	aControlRetry->SetRecieveData(szReturn);

	return bReval;
}

//▼1.0.0.1046
BOOL StCheckTemperature::ExecuteGenICam(INT &nValue)
{
	BOOL bReval = FALSE;
	CString szReturn(_T(""));

	do
	{
		if( !m_DeviceBase ) break;
		StDeviceGenICam *aDevice = (StDeviceGenICam *)m_DeviceBase;
		if( !aDevice ) break;
		if( !aDevice->IsOpened() ) break;

		if( !m_szCommand.IsEmpty() )
		{
			if( !m_szSelectorFeature.IsEmpty() && !m_szSelectorSelect.IsEmpty() )
			{
				StString lFeature = m_szSelectorFeature;
				StString strData = m_szSelectorSelect;
				bReval = aDevice->SetGenEnumParameter( lFeature, strData );
				if( !bReval ) break;
			}
			double dblData;
			bReval = aDevice->GetGenICamControl( m_szCommand, dblData );
			if( bReval )
			{
				nValue = (INT)dblData;
			}
			else
			{
				INT nData;
				bReval = aDevice->GetGenICamControl( m_szCommand, nData );
				if( bReval )
				{
					nValue = nData;
				}
			}
		}
		else
		if( m_dwAddress!=0 )
		{
			INT nData;
			bReval = aDevice->ReadMain(m_dwAddress,(char *)&nData,sizeof(nData));
			if( bReval )
			{
				nValue = nData;
			}
		}

	}while(0);

	return bReval;
}
//▲1.0.0.1046




//-------------------------------------------------------------------------------
//検査実行
//-------------------------------------------------------------------------------
BOOL StCheckTemperature::Execute(void)
{
	BOOL bReval = FALSE;
	m_iJudge = 2;	//2:処理中
	do
	{
		INT nValue=0;
		if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//GigE用
		{
			m_iJudge = 0;
			break;
		}
		else
		if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL用
		{
			//m_iJudge = 0;
			if( m_nMode==0 )
				bReval = ExecuteEllito(nValue);
			else
			{
				m_iJudge = 0;	//0:NG
			}
		}
		//▼1.0.0.1046
		else
		if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )	//CL用
		{
			bReval = ExecuteGenICam(nValue);
		}
		//▲1.0.0.1046

		if( bReval )
		{
			m_nResultTemperature = nValue;
			if( m_nMinTemperature<=m_nResultTemperature && m_nResultTemperature<=m_nMaxTemperature )
			{
				m_iJudge = 1;	//1:OK
			}
			else
			{
				m_iJudge = 0;	//0:NG
			}
		}
		break;
	}while(0);
	if( m_iJudge==2 ) m_iJudge = 1;	//1:OK

	return bReval;

}


BOOL StCheckTemperature::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);

	if( szFunction=="MinTemperature" || szFunction=="SetMinTemperature" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetMin(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="MaxTemperature" || szFunction=="SetMaxTemperature" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetMax(nData);
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
	if( szFunction=="Command" || szFunction=="SetCommand" )
	{
		SetCommand(szArgument);
		bReval = TRUE;
	}
	//▼1.0.0.1022
	else
	if( szFunction=="Recieve" || szFunction=="SetRecieve" )
	{
		SetRecieve(szArgument);
		bReval = TRUE;
	}
	//▲1.0.0.1022
	//▼1.0.0.1046
	else
	if( szFunction=="SelectorFeature" || szFunction=="SetSelectorFeature" )
	{
		m_szSelectorFeature = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SelectorSelect" || szFunction=="SetSelectorSelect" )
	{
		m_szSelectorSelect = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="Address" || szFunction=="SetAddress" )
	{
		m_dwAddress = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	//▲1.0.0.1046

	return bReval;
}

//▼1.0.0.1025
BOOL StCheckTemperature::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetTemperature" )
	{
		dblValue = GetResultTemperature();
		bReval = TRUE;
	}
	return bReval;
}
//▲1.0.0.1025

