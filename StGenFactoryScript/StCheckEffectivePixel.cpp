#include "StdAfx.h"
#include "StCheckEffectivePixel.h"


StCheckEffectivePixel::StCheckEffectivePixel(void)
{
	Initialize();
}

StCheckEffectivePixel::StCheckEffectivePixel(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
{
	Initialize();
}

StCheckEffectivePixel::~StCheckEffectivePixel(void)
{
}

void StCheckEffectivePixel::Initialize(void)
{
	//パラメータ
	m_nStart = 0;
	m_nEnd = 4095;
	m_nLevel = 180;

	//規格値-----
	m_nEffectiveStart = 0;
	m_nEffectiveEnd = 4095;

}

//-------------------------------------------------------------------------------
//検査実行
//-------------------------------------------------------------------------------
BOOL StCheckEffectivePixel::Execute(void)
{

	BOOL bReval = FALSE;
	m_iJudge = 2;	//2:処理中
	if( m_Buffer ) delete m_Buffer;
	m_Buffer = new StBuffer;
	m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー

	bReval = GetEffectivePixel( m_Buffer, m_nStart, m_nEnd, m_nLevel, &m_nElement, m_nResultEffectiveStart, m_nResultEffectiveEnd );
	if( bReval )
	{
		//m_nElement = GetElementFromBuffer(m_Buffer);
		SetElementColorIndexFromBuffer(m_Buffer);

		for( int i=0; i<(int)m_nElement && m_iJudge==2; i++ )
		{
			//m_nColorIndex[i] = GetElementColorIndexFromBuffer(m_Buffer, i);
			if( m_nResultEffectiveStart[i]!=m_nEffectiveStart || m_nResultEffectiveEnd[i]!=m_nEffectiveEnd )
			{
				m_iJudge=0;
			}
		}
		if( m_iJudge==2 ) m_iJudge = 1;	//1:OK
	}
	else
	{
		m_iJudge = 0;	//0:NG
	}
	return bReval;

}

void StCheckEffectivePixel::SetStart(INT nData)
{
	m_nStart = nData;
}

void StCheckEffectivePixel::SetEnd(INT nData)
{
	m_nEnd = nData;
}

void StCheckEffectivePixel::SetLevel(INT nData)
{
	m_nLevel = nData;
}

void StCheckEffectivePixel::SetEffectiveStart(INT nData)
{
	m_nEffectiveStart = nData;
}

void StCheckEffectivePixel::SetEffectiveEnd(INT nData)
{
	m_nEffectiveEnd = nData;
}

BOOL StCheckEffectivePixel::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="SetLevel" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetLevel(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetEffectiveStart" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetEffectiveStart(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetEffectiveEnd" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetEffectiveEnd(nData);
		bReval = TRUE;
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

	return bReval;

}

//▼1.0.0.1025
BOOL StCheckEffectivePixel::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetElement" )
	{
		dblValue = GetElement();
		bReval = TRUE;
	}
	else
	if( szFunction=="GetEffectiveStart" )
	{
		INT nIndex = _ttoi(szArg);
		dblValue = GetResultEffectiveStart(nIndex);
		bReval = TRUE;
	}
	else
	if( szFunction=="GetEffectiveEnd" )
	{
		INT nIndex = _ttoi(szArg);
		dblValue = GetResultEffectiveEnd(nIndex);
		bReval = TRUE;
	}
	return bReval;
}
//▲1.0.0.1025


