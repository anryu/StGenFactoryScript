#include "StdAfx.h"
#include "StCheckSN.h"


StCheckSN::StCheckSN(void)
{
	Initialize();
}

StCheckSN::StCheckSN(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
{
	Initialize();
//TRACE(TEXT("@@@@@StCheckSN::StCheckSN m_DeviceBase=0x%x\n"), m_DeviceBase );

}

StCheckSN::~StCheckSN(void)
{
}

void StCheckSN::Initialize(void)
{
	m_nPermAverageMin = 180.0;
	m_nPermAverageMax = 220.0;

	m_nPermSNMax = 3.0;
	m_nPermSN = 3.0;

	m_nStart = 0;
	m_nEnd = 4096;

	//▼1.0.0.1028
	m_dblPermLineAverageMin = -1.0;
	m_dblPermLineAverageMax = -1.0;
	//▲1.0.0.1028

	//▼1.0.0.1043
	for( int i=0; i<_countof(m_dblPermAverageColorMin); i++ )
	{
		m_dblPermAverageColorMin[i] = m_nPermAverageMin;
	}
	for( int i=0; i<_countof(m_dblPermAverageColorMax); i++ )
	{
		m_dblPermAverageColorMax[i] = m_dblPermLineAverageMax;
	}

	for( int i=0; i<_countof(m_dblPermLineAverageColorMin); i++ )
	{
		m_dblPermLineAverageColorMin[i] = m_dblPermLineAverageMin;
	}
	for( int i=0; i<_countof(m_dblPermLineAverageColorMax); i++ )
	{
		m_dblPermLineAverageColorMax[i] = m_dblPermLineAverageMax;
	}
	//▲1.0.0.1043

}
//-------------------------------------------------------------------------------
//検査実行
//-------------------------------------------------------------------------------
BOOL StCheckSN::Execute(void)
{
	BOOL bReval = FALSE;
	m_iJudge = 2;	//2:処理中
	if( m_Buffer ) delete m_Buffer;
	m_Buffer = new StBuffer;

//TRACE(TEXT("@@@@@StCheckSN::Execute m_DeviceBase=0x%x\n"), m_DeviceBase );

//if( m_nPermAverageMin==751 )
//{
//	Sleep(1);
//}
	m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー

	//SN取得
	//double dblAverage[3];
	//double dblOutSDAve[3];
	//double dblOutSDMax[3];
	//INT nSDMPix[3];

	//▼1.0.0.1028
	//bReval = GetSNAverage( m_Buffer, m_nStart, m_nEnd, m_dblResultAverage, m_dblResultSN, m_dblResultSNMax, m_nResultSDMPix );
	bReval = GetSNAverage( m_Buffer, m_nStart, m_nEnd, m_dblResultAverage, m_dblResultSN, m_dblResultSNMax, m_nResultSDMPix, m_dblReslutLineAverageMin, m_dblReslutLineAverageMax, m_nResultLineAverageMinPos, m_nResultLineAverageMaxPos );

//PBYTE pBuffer = (PBYTE)m_Buffer->GetDataPointer();
//TRACE(TEXT("@@@@@@@@@ pBuffer=[%02x:%02x:%02x:%02x:%02x:%02x]\n") , pBuffer[0], pBuffer[1], pBuffer[2], pBuffer[3], pBuffer[4], pBuffer[5] );
//StPixelType aType = m_Buffer->GetPixelType();
//TRACE(TEXT("@@@@@@@@@ aType=[%x]\n") , aType );

	//▲1.0.0.1028

//TRACE(TEXT("@@@m_dblResultSN(%f)\n"),m_dblResultSN[0]);
//TRACE(TEXT("@@@m_dblResultSNMax(%f)\n"),m_dblResultSNMax[0]);

	if( bReval )
	{
		m_nElement = GetElementFromBuffer(m_Buffer);
		SetElementColorIndexFromBuffer(m_Buffer);
		for( int i=0; i<m_nElement && m_iJudge==2; i++ )
		{
			//m_nColorIndex[i] = GetElementColorIndexFromBuffer(m_Buffer, i);

			//▼1.0.0.1043
			if( m_nElement==3 )	//カラーの時
			{
				INT nColorIndex = GetElementColorIndexFromBuffer(m_Buffer, i);

				if( m_dblResultAverage[nColorIndex]<m_dblPermAverageColorMin[i] || m_dblPermAverageColorMax[nColorIndex]<m_dblResultAverage[i] )
				{
					m_iJudge=0;
				}
				if( m_nPermSNMax<m_dblResultSNMax[nColorIndex] )
				{
					m_iJudge=0;
				}
				if( m_nPermSN<m_dblResultSN[nColorIndex] )
				{
					m_iJudge=0;
				}

				if( m_dblPermLineAverageColorMin[i]>=0.0 )
				{
					if( m_dblPermLineAverageColorMin[i]>m_dblReslutLineAverageMin[nColorIndex] )
					{
						m_iJudge=0;
					}
				}
				if( m_dblPermLineAverageColorMax[i]>=0.0 )
				{
					if( m_dblPermLineAverageColorMax[i]<m_dblReslutLineAverageMax[nColorIndex] )
					{
						m_iJudge=0;
					}
				}
				continue;
			}
			//▲1.0.0.1043


			if( m_dblResultAverage[i]<m_nPermAverageMin || m_nPermAverageMax<m_dblResultAverage[i] )
			{
				m_iJudge=0;
			}
			if( m_nPermSNMax<m_dblResultSNMax[i] )
			{
				m_iJudge=0;
			}
			if( m_nPermSN<m_dblResultSN[i] )
			{
				m_iJudge=0;
			}

			//▼1.0.0.1028
			if( m_dblPermLineAverageMin>=0.0 )
			{
				if( m_dblPermLineAverageMin>m_dblReslutLineAverageMin[i] )
				{
					m_iJudge=0;
				}
			}
			if( m_dblPermLineAverageMax>=0.0 )
			{
				if( m_dblPermLineAverageMax<m_dblReslutLineAverageMax[i] )
				{
					m_iJudge=0;
				}
			}
			//▲1.0.0.1028
		}
		if( m_iJudge==2 ) m_iJudge = 1;	//1:OK
	}
	else
	{
		m_iJudge = 0;	//0:NG
	}
	return bReval;
}

void StCheckSN::SetStart(INT nData)
{
	m_nStart = nData;
}

void StCheckSN::SetEnd(INT nData)
{
	m_nEnd = nData;
}

void StCheckSN::SetAverageMin(double dblData)
{
	m_nPermAverageMin = dblData;
}

void StCheckSN::SetAverageMax(double dblData)
{
	m_nPermAverageMax = dblData;
}

void StCheckSN::SetSNMax(double dblData)
{
	m_nPermSNMax = dblData;
}

void StCheckSN::SetSN(double dblData)
{
	m_nPermSN = dblData;
}


BOOL StCheckSN::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="SetAverageMin" )
	{
		double dblAverageMin = _ttof(szArgument);
		SetAverageMin(dblAverageMin);
		//▼1.0.0.1043
		//カラーにも同じ値を入れる。
		for( int i=0; i<_countof(m_dblPermAverageColorMin); i++ )
			m_dblPermAverageColorMin[i] = dblAverageMin;
		//▲1.0.0.1043
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAverageMax" )
	{
		double dblAverageMax = _ttof(szArgument);
		SetAverageMax(dblAverageMax);
		//▼1.0.0.1043
		//カラーにも同じ値を入れる。
		for( int i=0; i<_countof(m_dblPermAverageColorMax); i++ )
			m_dblPermAverageColorMax[i] = dblAverageMax;
		//▲1.0.0.1043
		bReval = TRUE;
	}
	else
	if( szFunction=="SetSNMax" )
	{
		double dblAverageMin = _ttof(szArgument);
		SetSNMax(dblAverageMin);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetSN" )
	{
		double dblAverageMin = _ttof(szArgument);
		SetSN(dblAverageMin);
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

	//▼1.0.0.1028
	else
	if( szFunction=="SetLineAverageMin" )
	{
		m_dblPermLineAverageMin = _ttof(szArgument);
		//▼1.0.0.1043
		//カラーにも同じ値を入れる。
		for( int i=0; i<_countof(m_dblPermLineAverageColorMin); i++ )
			m_dblPermLineAverageColorMin[i] = m_dblPermLineAverageMin;
		//▲1.0.0.1043

		bReval = TRUE;
	}
	else
	if( szFunction=="SetLineAverageMax" )
	{
		m_dblPermLineAverageMax = _ttof(szArgument);
		//▼1.0.0.1043
		//カラーにも同じ値を入れる。
		for( int i=0; i<_countof(m_dblPermLineAverageColorMax); i++ )
			m_dblPermLineAverageColorMax[i] = m_dblPermLineAverageMax;
		//▲1.0.0.1043
		bReval = TRUE;
	}
	//▲1.0.0.1028

	//▼1.0.0.1043
	else
	if( szFunction=="SetAverageRedMin" )
	{
		m_dblPermAverageColorMin[0] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAverageGreenMin" )
	{
		m_dblPermAverageColorMin[1] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAverageBlueMin" )
	{
		m_dblPermAverageColorMin[2] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAverageRedMax" )
	{
		m_dblPermAverageColorMax[0] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAverageGreenMax" )
	{
		m_dblPermAverageColorMax[1] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAverageBlueMax" )
	{
		m_dblPermAverageColorMax[2] = _ttof(szArgument);
		bReval = TRUE;
	}

	else
	if( szFunction=="SetLineAverageRedMin" )
	{
		m_dblPermLineAverageColorMin[0] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetLineAverageGreenMin" )
	{
		m_dblPermLineAverageColorMin[1] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetLineAverageBlueMin" )
	{
		m_dblPermLineAverageColorMin[2] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetLineAverageRedMax" )
	{
		m_dblPermLineAverageColorMax[0] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetLineAverageGreenMax" )
	{
		m_dblPermLineAverageColorMax[1] = _ttof(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetLineAverageBlueMax" )
	{
		m_dblPermLineAverageColorMax[2] = _ttof(szArgument);
		bReval = TRUE;
	}
	//▲1.0.0.1043

	return bReval;

}

//▼1.0.0.1025
BOOL StCheckSN::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetAverage" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_countof(m_dblResultAverage) )
		{
			dblValue = m_dblResultAverage[nIndex];
			bReval = TRUE;
		}
	}
	else
	if( szFunction=="GetSN" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_countof(m_dblResultSN) )
		{
			dblValue = m_dblResultSN[nIndex];
			bReval = TRUE;
		}
	}
	else
	if( szFunction=="GetSNMax" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_countof(m_dblResultSNMax) )
		{
			dblValue = m_dblResultSNMax[nIndex];
			bReval = TRUE;
		}
	}
	else
	if( szFunction=="GetSDMPix" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_countof(m_nResultSDMPix) )
		{
			dblValue = m_nResultSDMPix[nIndex];
			bReval = TRUE;
		}
	}

	//▼1.0.0.1028
	else
	if( szFunction=="GetLineAverageMin" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_countof(m_dblReslutLineAverageMin) )
		{
			dblValue = m_dblReslutLineAverageMin[nIndex];
			bReval = TRUE;
		}
	}
	else
	if( szFunction=="GetLineAverageMax" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_countof(m_dblReslutLineAverageMax) )
		{
			dblValue = m_dblReslutLineAverageMax[nIndex];
			bReval = TRUE;
		}
	}
	else
	if( szFunction=="GetLineAverageMinPos" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_countof(m_nResultLineAverageMinPos) )
		{
			dblValue = m_nResultLineAverageMinPos[nIndex];
			bReval = TRUE;
		}
	}
	else
	if( szFunction=="GetLineAverageMaxPos" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_countof(m_nResultLineAverageMaxPos) )
		{
			dblValue = m_nResultLineAverageMaxPos[nIndex];
			bReval = TRUE;
		}
	}
	//▲1.0.0.1028

	return bReval;
}
//▲1.0.0.1025


