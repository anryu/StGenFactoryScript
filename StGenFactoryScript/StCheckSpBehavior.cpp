#include "StdAfx.h"
#include "StCheckSpBehavior.h"


StCheckSpBehavior::StCheckSpBehavior(void)
{
	Initialize();
}

StCheckSpBehavior::StCheckSpBehavior(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
{
	Initialize();
}

StCheckSpBehavior::~StCheckSpBehavior(void)
{
}

void StCheckSpBehavior::Initialize(void)
{
	m_nStart = 0;
	m_nEnd = 2047;

	m_nMaxLevel = 220;
	m_nMinLevel = 180;

	m_nInOutMode = 0;
	//規格値-----
	m_nContinuousCount = 10;

	m_nColor = 1; //BW:1 Color:7


	m_iResultContinuous = -1;
	m_iResultLine = -1;
	m_iResultColor = -1;
	//▼1.0.0.1044
	m_dblResultLineAverage = -1.0;
	//▲1.0.0.1044

	m_Buffer = NULL;

}

//-------------------------------------------------------------------------------
//検査実行
//-------------------------------------------------------------------------------
BOOL StCheckSpBehavior::Execute(void)
{
	BOOL bReval = FALSE;

	m_iJudge = 2;	//2:処理中
	if( m_Buffer ) delete m_Buffer;
	m_Buffer = new StBuffer;
	m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー


	INT iContinuous = -1;
	INT iLine = -1;
	INT iColor = -1;
	double dblLineAverage = 0.0;
	bReval = GetSpecialBehavior( m_Buffer, m_nStart, m_nEnd, m_nMinLevel, m_nMaxLevel, m_nInOutMode
	//▼1.0.0.1044
	//                                   , m_nColor, &iContinuous, &iLine, &iColor );
	                                   , m_nColor, &iContinuous, &iLine, &iColor, &dblLineAverage );
	//▲1.0.0.1044
	if( bReval )
	{
		m_iResultContinuous = iContinuous;
		m_iResultLine = iLine;
		//▼1.0.0.1044
		m_dblResultLineAverage = dblLineAverage;
		//▲1.0.0.1044
		SetElementColorIndexFromBuffer(m_Buffer);
		m_iResultColor = 0;
		if( iColor>=0 )
			m_iResultColor = GetElementColorIndex(iColor);
		else
			m_iResultColor = -1;	//BW
		//m_iResultColor = iColor;
		if( m_iResultContinuous>m_nContinuousCount )
		{
			m_iJudge=0;
		}
		else
		{
			m_iJudge=1;
		}
	}
	else
	{
		m_iJudge = 0;	//0:NG
	}

	return bReval;
}

void StCheckSpBehavior::SetStart(INT nData)
{
	m_nStart = nData;
}

void StCheckSpBehavior::SetEnd(INT nData)
{
	m_nEnd = nData;
}

void StCheckSpBehavior::SetColor(INT nData)
{
	m_nColor = nData;
}

void StCheckSpBehavior::SetMaxLevel(INT nData)
{
	m_nMaxLevel = nData;
}

void StCheckSpBehavior::SetMinLevel(INT nData)
{
	m_nMinLevel = nData;
}

void StCheckSpBehavior::SetInOutMode(INT nData)
{
	m_nInOutMode = nData;
}

void StCheckSpBehavior::SetContinuousCount(INT nData)
{
	m_nContinuousCount = nData;
}
void StCheckSpBehavior::SetTitle(LPCTSTR szTitle)
{
	m_szTitle = szTitle;
}

BOOL StCheckSpBehavior::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction == _T("Execute") )
	{
	}
	else
	if( szFunction=="SetStartLine" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetStart(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetEndLine" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetEnd(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetColor" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetColor(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMaxLevel" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetMaxLevel(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMinLevel" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetMinLevel(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetInOutMode" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetInOutMode(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetContinuousCount" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetContinuousCount(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTitle" )
	{
		SetTitle(szArgument);
		bReval = TRUE;
	}

	return bReval;
}

//▼1.0.0.1025
BOOL StCheckSpBehavior::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetContinuous" )
	{
		dblValue = GetContinuous();
		bReval = TRUE;
	}
	else
	if( szFunction=="GetLine" )
	{
		dblValue = GetLine();
		bReval = TRUE;
	}
	else
	if( szFunction=="GetColor" )
	{
		dblValue = GetColor();
		bReval = TRUE;
	}

	//▼1.0.0.1044
	else
	if( szFunction=="GetLineAverage" )
	{
		dblValue = GetLineAverage();
		bReval = TRUE;
	}
	//▲1.0.0.1044

	return bReval;
}
//▲1.0.0.1025


