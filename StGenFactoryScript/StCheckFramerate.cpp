#include "StdAfx.h"
#include "StCheckFramerate.h"
#include "StInspectMain.h"


StCheckFramerate::StCheckFramerate(void)
{
	Initialize();
}

StCheckFramerate::StCheckFramerate(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase)
	:StCheckBase( szClassName, szVarName, aDeviceBase )
{
	Initialize();
}

StCheckFramerate::~StCheckFramerate(void)
{
}

void StCheckFramerate::Initialize(void)
{
	m_dblMaxFramerate = 0.0;
	m_dblMinFramerate = 0.0;
	m_dblResultFramerate = 0.0;
	m_szCommand = _T("");
}

//-------------------------------------------------------------------------------
//検査実行
//-------------------------------------------------------------------------------
BOOL StCheckFramerate::Execute(void)
{
	BOOL bReval = FALSE;
	if( !m_DeviceBase )
	{
		m_iJudge = 0;
		return FALSE;
	}
		
	m_iJudge = 2;	//2:処理中

	do
	{
		double dblValue=0.0;
		if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//GigE用
		{
			StDeviceEBus *aDevice = (StDeviceEBus *)m_DeviceBase;
			if( aDevice )
				bReval = aDevice->GetFunction( _T("DataStream"), _T("AcquisitionRate"), dblValue );
			break;
		}
		else
		if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL用
		{
			StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
			if( aDevice )
				bReval = aDevice->GetFramerate( &dblValue );
		}
		//▼1.0.0.1046
		else
		if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )	//GenICam用
		{
			if( m_szCommand.GetLength()>0 )
			{
				StDeviceGenICam *aDevice = (StDeviceGenICam *)m_DeviceBase;
				if( aDevice )		//StatisticsLineRate,StatisticsFrameRate
					bReval = aDevice->GetFunction( _T("DataStream"), m_szCommand, dblValue );
			}
		}
		//▲1.0.0.1046
		if( bReval )
		{
			m_dblResultFramerate = dblValue;
			if( m_dblMinFramerate<=m_dblResultFramerate && m_dblResultFramerate<=m_dblMaxFramerate )
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

void StCheckFramerate::SetMax(double dblData)
{
	m_dblMaxFramerate = dblData;
}
void StCheckFramerate::SetMin(double dblData)
{
	m_dblMinFramerate = dblData;
}

BOOL StCheckFramerate::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction == _T("Execute") )
	{
	}
	else
	if( szFunction=="SetMaxFramerate" || szFunction=="MaxFramerate" )
	{
		double dblData = _ttof(szArgument);
		SetMax(dblData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMinFramerate" || szFunction=="MinFramerate"  )
	{
		double dblData = _ttof(szArgument);
		SetMin(dblData);
		bReval = TRUE;
	}
	//▼1.0.0.1046
	else
	if( szFunction=="SetCommand" || szFunction=="Command"  )
	{
		m_szCommand = szArgument;
		bReval = TRUE;
	}
	//▲1.0.0.1046

	return bReval;
}

BOOL StCheckFramerate::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetFramerate" )
	{
		dblValue = GetFramerate();
		bReval = TRUE;
	}
	return bReval;
}
