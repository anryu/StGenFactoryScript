#include "StdAfx.h"
#include "StCheckPixelDefect.h"


StCheckPixelDefect::StCheckPixelDefect(void)
{
	Initialize();
}

StCheckPixelDefect::StCheckPixelDefect(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
{
	Initialize();
}


StCheckPixelDefect::~StCheckPixelDefect(void)
{
	if( m_pdblResultDifferenceRatio ) delete [] m_pdblResultDifferenceRatio;
	if( m_piResultLineRatio ) delete [] m_piResultLineRatio;

}

void StCheckPixelDefect::Initialize(void)
{

	m_nStart = 0;
	m_nEnd = 4095;

	m_nWidthLines = 100;
	m_dblDifferenceMax = 10.0;

	m_nSeparate = 1;
	//▼1.0.0.1041
	m_nCheckMode = 0;
	//▲1.0.0.1041

	m_iResultElement = 0;
	m_pdblResultDifferenceRatio = NULL;
	m_piResultLineRatio = NULL;
}
//-------------------------------------------------------------------------------
//検査実行
//-------------------------------------------------------------------------------
BOOL StCheckPixelDefect::Execute(void)
{
	BOOL bReval = FALSE;
	m_iJudge = 2;	//2:処理中
	if( m_Buffer ) delete m_Buffer;
	m_Buffer = new StBuffer;
	m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー

	size_t nElement = GetElementFromBuffer(m_Buffer);
	double * pdblDifference = NULL;
	PINT piLine = NULL;
	double * pdblDifferenceRatio = NULL;
	PINT piLineRatio = NULL;

	do
	{

		pdblDifference = new double [nElement];
		if( !pdblDifference ) break;
		piLine = new INT [nElement];
		if( !piLine ) break;
		pdblDifferenceRatio = new double [nElement];
		if( !pdblDifferenceRatio ) break;
		piLineRatio = new INT [nElement];
		if( !piLineRatio ) break;


		//▼1.0.0.1041
		//bReval = GetWidthLineAverageDifference( m_Buffer, m_nStart, m_nEnd, m_nWidthLines, m_nSeparate, &nElement, pdblDifference, piLine, pdblDifferenceRatio, piLineRatio );
		bReval = GetWidthLineAverageDifference( m_Buffer, m_nStart, m_nEnd, m_nWidthLines, m_nSeparate, m_nCheckMode, &nElement, pdblDifference, piLine, pdblDifferenceRatio, piLineRatio );
		//▲1.0.0.1041
		if( bReval )
		{
			m_iResultElement = nElement;
			SetElementColorIndexFromBuffer(m_Buffer);

			if( m_pdblResultDifferenceRatio ) delete [] m_pdblResultDifferenceRatio;
			m_pdblResultDifferenceRatio = new double [m_iResultElement];
			if( m_piResultLineRatio ) delete [] m_piResultLineRatio;
			m_piResultLineRatio = new INT [m_iResultElement];
			if( m_pdblResultDifferenceRatio )
				memcpy( m_pdblResultDifferenceRatio, pdblDifferenceRatio, sizeof(double) * m_iResultElement);
			if( m_piResultLineRatio )
				memcpy( m_piResultLineRatio, piLineRatio, sizeof(INT) * m_iResultElement);

			//判定
			for( int i=0; i<(int)nElement; i++ )
			{
				if( pdblDifferenceRatio[i]>m_dblDifferenceMax )
					m_iJudge=0;
			}
			if( m_iJudge==2 )
				m_iJudge=1;
		}
		else
		{
			m_iJudge = 0;	//0:NG
		}

	}while(0);

	if(pdblDifference) delete [] pdblDifference;
	if(piLine) delete [] piLine;
	if(pdblDifferenceRatio) delete [] pdblDifferenceRatio;
	if(piLineRatio) delete [] piLineRatio;

	if( m_iJudge == 2 )
		m_iJudge = 0;

	return bReval;


}

void StCheckPixelDefect::SetWidthLines(INT nData)
{
	m_nWidthLines = nData;
}

void StCheckPixelDefect::SetDifferenceMax(double dblData)
{
	m_dblDifferenceMax = dblData;
}

void StCheckPixelDefect::SetStart(INT nData)
{
	m_nStart = nData;
}

void StCheckPixelDefect::SetEnd(INT nData)
{
	m_nEnd = nData;
}
void StCheckPixelDefect::SetSeparate(INT nData)
{
	m_nSeparate = nData;
}
//▼1.0.0.1041
void StCheckPixelDefect::SetCheckMode(INT nData)
{
	m_nCheckMode = nData;
}
//▲1.0.0.1041

BOOL StCheckPixelDefect::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="SetWidthLines" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetWidthLines(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetDifferenceMax" )
	{
		double dblData = _ttof(szArgument);
		SetDifferenceMax(dblData);
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
	else
	if( szFunction=="SetSeparate" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetSeparate(nData);
		bReval = TRUE;
	}
	//▼1.0.0.1041
	else
	if( szFunction=="SetCheckMode" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetCheckMode(nData);
		bReval = TRUE;
	}
	//▲1.0.0.1041

	return bReval;

}

INT StCheckPixelDefect::GetResultElement(void)
{
	return m_iResultElement;
}

double StCheckPixelDefect::GetResultDifference(int nIndex)
{
	if( nIndex>=m_iResultElement || m_pdblResultDifferenceRatio==NULL ) return -1.0;
	return m_pdblResultDifferenceRatio[nIndex];
}

INT StCheckPixelDefect::GetResultLine(int nIndex)
{
	if( nIndex>=m_iResultElement || m_piResultLineRatio==NULL ) return -1;
	return m_piResultLineRatio[nIndex];
}

//▼1.0.0.1025
BOOL StCheckPixelDefect::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetElement" )
	{
		dblValue = GetResultElement();
		bReval = TRUE;
	}
	else
	if( szFunction=="GetDifferenceRatio" )
	{
		INT nIndex = _ttoi(szArg);
		dblValue = GetResultDifference(nIndex);
		bReval = TRUE;
	}
	else
	if( szFunction=="GetLine" )
	{
		INT nIndex = _ttoi(szArg);
		dblValue = GetResultLine(nIndex);
		bReval = TRUE;
	}
	return bReval;
}
//▲1.0.0.1025
