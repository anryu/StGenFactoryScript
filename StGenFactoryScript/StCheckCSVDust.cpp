#include "StdAfx.h"
#include "StCheckCSVDust.h"


StCheckCSVDust::StCheckCSVDust(void)
{
	Initialize();
}

StCheckCSVDust::StCheckCSVDust(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase)
	:StCheckBase( szClassName, szVarName, aDeviceBase )
{
	Initialize();
}

StCheckCSVDust::~StCheckCSVDust(void)
{
	Release();
}

void StCheckCSVDust::Initialize(void)
{
	m_iResultElement = 0;

	m_pdblResultDifferenceRatio = NULL;
	m_piResultLineRatio = NULL;
	m_pdblResultAverage = NULL;

	mSaveFilePath = ".";
	m_szFileName = "";

	m_nStart = 0;
	m_nEnd = 4095;
	m_nWidthLines = 4096;
	m_nSeparate = 1;
	//▼1.0.0.1041
	m_nCheckMode = 0;
	//▲1.0.0.1041
	//▼1.0.0.1043
	m_dblDifferenceMax = 0.0;
	m_nContinuousCountMax = 0;
	m_pnResultContinuousCount = NULL;
	m_pnResultContinuousStart = NULL;
	m_pnResultContinuousEnd = NULL;
	//▲1.0.0.1043

}

void StCheckCSVDust::Release(void)
{
	if( m_pdblResultDifferenceRatio ) delete [] m_pdblResultDifferenceRatio;
	m_pdblResultDifferenceRatio = NULL;
	if( m_piResultLineRatio ) delete [] m_piResultLineRatio;
	m_piResultLineRatio = NULL;
	if( m_pdblResultAverage ) delete [] m_pdblResultAverage;
	m_pdblResultAverage = NULL;
	//▼1.0.0.1043
	if( m_pnResultContinuousCount ) delete [] m_pnResultContinuousCount;
	m_pnResultContinuousCount = NULL;
	if( m_pnResultContinuousStart ) delete [] m_pnResultContinuousStart;
	m_pnResultContinuousStart = NULL;
	if( m_pnResultContinuousEnd ) delete [] m_pnResultContinuousEnd;
	m_pnResultContinuousEnd = NULL;
	//▲1.0.0.1043

}


BOOL StCheckCSVDust::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction == _T("Execute") )
	{
	}
	else
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
	//▼1.0.0.1043
	else
	if( szFunction=="SetContinuousCountMax" || szFunction=="ContinuousCountMax" )
	{
		INT nData = _tcstol(szArgument,NULL,0);
		SetContinuousCountMax(nData);
		bReval = TRUE;
	}
	//▲1.0.0.1043
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
	else
	if( szFunction=="SetAverageLowLevel" )
	{
		double dblData = _ttof(szArgument);
		SetAverageLowLevel(dblData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetAverageHighLevel" )
	{
		double dblData = _ttof(szArgument);
		SetAverageHighLevel(dblData);
		bReval = TRUE;
	}

	else
	if( szFunction=="SetFilePath" )
	{
		mSaveFilePath = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetFileName" )
	{
		m_szFileName = szArgument;
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

	//else
	//if( szFunction=="SetBMPFileName" )
	//{
	//	SetBMPFileName(szArgument);
	//	bReval = TRUE;
	//}

	return bReval;
}


double StCheckCSVDust::GetResultAverage(void)
{
	double dblValue=-1;
	if( m_pdblResultAverage )
	{
		dblValue = m_pdblResultAverage[0];
	}
	return dblValue;
}
double StCheckCSVDust::GetResultDifferenceRatio(void)
{
	double dblValue=-1;
	if( m_pdblResultDifferenceRatio )
	{
		dblValue = m_pdblResultDifferenceRatio[0];
	}
	return dblValue;
}
INT StCheckCSVDust::GetResultLineRatio(void)
{
	INT iValue=-1;
	if( m_piResultLineRatio )
	{
		iValue = m_piResultLineRatio[0];
	}
	return iValue;
}
//▼1.0.0.1043
INT StCheckCSVDust::GetResultContinuousCount(void)
{
	INT iValue=-1;
	if( m_pnResultContinuousCount )
	{
		iValue = m_pnResultContinuousCount[0];
	}
	return iValue;
}
INT StCheckCSVDust::GetResultContinuousStart(void)
{
	INT iValue=-1;
	if( m_pnResultContinuousStart )
	{
		iValue = m_pnResultContinuousStart[0];
	}
	return iValue;
}
INT StCheckCSVDust::GetResultContinuousEnd(void)
{
	INT iValue=-1;
	if( m_pnResultContinuousEnd )
	{
		iValue = m_pnResultContinuousEnd[0];
	}
	return iValue;
}
//▲1.0.0.1043

BOOL StCheckCSVDust::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetAverageLowLevel" )
	{
		dblValue = m_dblAverageLowLevel;
		bReval = TRUE;
	}
	else
	if( szFunction=="GetAverageHighLevel" )
	{
		dblValue = m_dblAverageHighLevel;
		bReval = TRUE;
	}
	else
	if( szFunction=="GetDifferenceMax" )
	{
		dblValue = m_dblDifferenceMax;
		bReval = TRUE;
	}
	//▼1.0.0.1043
	else
	if( szFunction=="GetContinuousCountMax" )
	{
		dblValue = m_nContinuousCountMax;
		bReval = TRUE;
	}
	else
	if( szFunction=="GetResultContinuousCount" )
	{
		dblValue = GetResultContinuousCount();
		bReval = TRUE;
	}
	else
	if( szFunction=="GetResultContinuousStart" )
	{
		dblValue = GetResultContinuousStart();
		bReval = TRUE;
	}
	else
	if( szFunction=="GetResultContinuousEnd" )
	{
		dblValue = GetResultContinuousEnd();
		bReval = TRUE;
	}
	//▲1.0.0.1043
	else
	if( szFunction=="GetResultAverage" )
	{
		INT nIndex = 0;
		dblValue = m_pdblResultAverage[nIndex];
		bReval = TRUE;
	}
	else
	if( szFunction=="GetResultDifferenceRatio" )
	{
		INT nIndex = 0;
		dblValue = m_pdblResultDifferenceRatio[nIndex];
		bReval = TRUE;
	}
	else
	if( szFunction=="GetResultLineRatio" )
	{
		INT nIndex = 0;
		dblValue = (double)m_piResultLineRatio[nIndex];
		bReval = TRUE;
	}
	return bReval;
}

//-------------------------------------------------------------------------------
//検査実行
//-------------------------------------------------------------------------------
BOOL StCheckCSVDust::Execute(void)
{
	BOOL bReval = FALSE;
		
	m_iJudge = 2;	//2:処理中


	size_t nElement = 1;

	double * pdblDifference = NULL;
	PINT piLine = NULL;
	double * pdblDifferenceRatio = NULL;
	PINT piLineRatio = NULL;

	double *pdblAverage = NULL;

	//▼1.0.0.1043
	PINT pnContinuousCount = NULL;
	PINT pnContinuousStart = NULL;
	PINT pnContinuousEnd = NULL;
	//▲1.0.0.1043

	double *pdblDataBuffer = NULL;

	do
	{
		CString szFullFileName;
		if( m_szFileName.GetLength()>0 )
		{
			szFullFileName = mSaveFilePath + _T("\\") + m_szFileName;
		}
		else
		{
			//ファイル選択モード
			TCHAR chCurrentDir[MAX_PATH];
			GetCurrentDirectory( _countof(chCurrentDir), chCurrentDir );

			CFileDialog dlgLoad( TRUE,0,0,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
									TEXT("Text File(*.csv)|*.csv|AllFiles(*.*)|*.*||"));
			dlgLoad.m_ofn.lpstrTitle = TEXT("Select file");
			dlgLoad.m_ofn.lpstrDefExt = TEXT("txt");
			dlgLoad.m_ofn.lpstrInitialDir = mSaveFilePath;
			if(dlgLoad.DoModal() != IDOK)
			{
				szFullFileName = "";
			}
			else
			{
				szFullFileName = dlgLoad.m_ofn.lpstrFile;
			}
			SetCurrentDirectory(chCurrentDir);
		}

		if( szFullFileName.GetLength()==0 ) break;



		pdblDataBuffer = new double [m_nEnd + 1]; 
		if( !pdblDataBuffer ) break;
		memset( pdblDataBuffer, 0, sizeof(double)*(m_nEnd + 1) );


		//ファイルから読み込み
		CStdioFile aFile;
		if( aFile.Open(szFullFileName,CFile::modeRead,NULL)!=0 )
		{
			aFile.SeekToBegin();
			CString tmp,data1,data2;
			INT nIndex;
			
			while( aFile.ReadString(tmp) )
			{
				tmp.Trim();
				if( !tmp.IsEmpty() )
				{
					nIndex = -1;
					INT nCammaPos = tmp.Find(_T(","));
					if( nCammaPos>0 )
					{
						data1=tmp.Left(nCammaPos);
						data1.Trim();
						if( data1==_T("0") || _ttoi(data1)>0  )
							nIndex = _ttoi(data1);
						tmp=tmp.Mid(nCammaPos+1);
					}
					if( nIndex>=0 && nIndex<=m_nEnd )
					{
						nCammaPos = tmp.Find(_T(","));
						if( nCammaPos>0 )
						{
							tmp = tmp.Left(nCammaPos);
						}
						tmp.Trim();
						pdblDataBuffer[nIndex] = _ttof(tmp);
					}
				}
			}
			aFile.Close();
		}
		else
		{
			m_iJudge = 0;	//0:NG
			break;
		}

		if( m_iResultElement!=nElement )
		{
			if( m_pdblResultDifferenceRatio ) delete [] m_pdblResultDifferenceRatio;
			m_pdblResultDifferenceRatio = NULL;
			if( m_piResultLineRatio ) delete [] m_piResultLineRatio;
			m_piResultLineRatio = NULL;
			if( m_pdblResultAverage ) delete [] m_pdblResultAverage;
			m_pdblResultAverage = NULL;
			//▼1.0.0.1043
			if( m_pnResultContinuousCount ) delete [] m_pnResultContinuousCount;
			m_pnResultContinuousCount = NULL;
			if( m_pnResultContinuousStart ) delete [] m_pnResultContinuousStart;
			m_pnResultContinuousStart = NULL;
			if( m_pnResultContinuousEnd ) delete [] m_pnResultContinuousEnd;
			m_pnResultContinuousEnd = NULL;
			//▲1.0.0.1043

			m_iResultElement = 0;
		}
		m_iResultElement = nElement;
		if( !m_pdblResultDifferenceRatio ) m_pdblResultDifferenceRatio = new double [m_iResultElement];
		memset(m_pdblResultDifferenceRatio,0,sizeof(double)*m_iResultElement );
		if( !m_piResultLineRatio ) m_piResultLineRatio = new INT [m_iResultElement];
		memset(m_piResultLineRatio,0,sizeof(INT)*m_iResultElement );
		if( !m_pdblResultAverage ) m_pdblResultAverage = new double [m_iResultElement];
		memset(m_pdblResultAverage,0,sizeof(double)*m_iResultElement );
		//▼1.0.0.1043
		if( !m_pnResultContinuousCount ) m_pnResultContinuousCount = new INT [m_iResultElement];
		memset(m_pnResultContinuousCount,0,sizeof(INT)*m_iResultElement );
		if( !m_pnResultContinuousStart ) m_pnResultContinuousStart = new INT [m_iResultElement];
		memset(m_pnResultContinuousStart,-1,sizeof(INT)*m_iResultElement );
		if( !m_pnResultContinuousEnd ) m_pnResultContinuousEnd = new INT [m_iResultElement];
		memset(m_pnResultContinuousEnd,-1,sizeof(INT)*m_iResultElement );
		//▲1.0.0.1043

		pdblDifference = new double [nElement];
		if( !pdblDifference ) break;
		piLine = new INT [nElement];
		if( !piLine ) break;
		pdblDifferenceRatio = new double [nElement];
		if( !pdblDifferenceRatio ) break;
		piLineRatio = new INT [nElement];
		if( !piLineRatio ) break;

		pdblAverage = new double [nElement];
		if( !pdblAverage ) break;
		for( int i=0; i<(int)nElement; i++ )
			pdblAverage[i] = -1.0;

		//▼1.0.0.1043
		pnContinuousCount = new INT [nElement];
		if( !pnContinuousCount ) break;
		for( int i=0; i<(int)nElement; i++ )
			pnContinuousCount[i] = 0;
		pnContinuousStart = new INT [nElement];
		if( !pnContinuousStart ) break;
		for( int i=0; i<(int)nElement; i++ )
			pnContinuousStart[i] = -1;
		pnContinuousEnd = new INT [nElement];
		if( !pnContinuousEnd ) break;
		for( int i=0; i<(int)nElement; i++ )
			pnContinuousEnd[i] = -1;
		//▲1.0.0.1043

		//▼1.0.0.1043
		////▼1.0.0.1041
		////bReval = GetWidthLineAverageDifference( pdblDataBuffer, m_nEnd+1, m_nStart, m_nEnd, m_nWidthLines, m_nSeparate, &nElement, pdblDifference, piLine, pdblDifferenceRatio, piLineRatio, pdblAverage );
		//bReval = GetWidthLineAverageDifference( pdblDataBuffer, m_nEnd+1, m_nStart, m_nEnd, m_nWidthLines, m_nSeparate, m_nCheckMode, &nElement, pdblDifference, piLine, pdblDifferenceRatio, piLineRatio, pdblAverage );
		////▲1.0.0.1041
		bReval = GetWidthLineAverageDifference( pdblDataBuffer, m_nEnd+1, m_nStart, m_nEnd, m_nWidthLines, m_nSeparate, m_nCheckMode, &nElement, 
			pdblDifference, piLine, pdblDifferenceRatio, piLineRatio, pdblAverage, m_dblDifferenceMax, pnContinuousCount, pnContinuousStart, pnContinuousEnd );
		//▲1.0.0.1043
		if( !bReval )
		{
			m_iJudge = 0;	//0:NG
			break;
		}

		//▼1.0.0.1043
		//if( pdblDifferenceRatio[0]>m_dblDifferenceMax )
		//{
		//	m_iJudge = 0;
		//}
		for( int i=0; i<nElement; i++ )
		{
			if( pnContinuousCount[i]<0 )	//Mode = 0 or 1
			{
				if( pdblDifferenceRatio[i]>m_dblDifferenceMax )
				{
					m_iJudge = 0;
				}
			}
			else
			{
				if( pnContinuousCount[i]>m_nContinuousCountMax )
				{
					m_iJudge = 0;
				}
			}
		}
		//▲1.0.0.1043
		//Average判定-----------------------------
		double dblAverage = pdblAverage[0];
		//▼1.0.0.1043
		if( nElement>1 ) dblAverage = pdblAverage[1]; //Green...
		//▲1.0.0.1043


		if( dblAverage<m_dblAverageLowLevel || dblAverage>m_dblAverageHighLevel  )
			m_iJudge = 0;

		//▼1.0.0.1043
		//m_pdblResultDifferenceRatio[0]= pdblDifferenceRatio[0];
		//m_piResultLineRatio[0] = piLineRatio[0];
		//m_pdblResultAverage[0]= dblAverage;
		for( int i=0; i<nElement; i++ )
		{
			m_pdblResultDifferenceRatio[i]= pdblDifferenceRatio[i];
			m_piResultLineRatio[i] = piLineRatio[i];
			m_pdblResultAverage[i]= pdblAverage[i];
			m_pnResultContinuousCount[i]= pnContinuousCount[i];
			m_pnResultContinuousStart[i]= pnContinuousStart[i];
			m_pnResultContinuousEnd[i]= pnContinuousEnd[i];
		}
		//▲1.0.0.1043
		break;
	}while(0);
	

	if(pdblAverage) delete [] pdblAverage;
	if(pdblDataBuffer) delete [] pdblDataBuffer;

	if(pdblDifference) delete [] pdblDifference;
	if(piLine) delete [] piLine;
	if(pdblDifferenceRatio) delete [] pdblDifferenceRatio;
	if(piLineRatio) delete [] piLineRatio;
	//▼1.0.0.1043
	if(pnContinuousCount) delete [] pnContinuousCount;
	if(pnContinuousStart) delete [] pnContinuousStart;
	if(pnContinuousEnd) delete [] pnContinuousEnd;
	//▲1.0.0.1043

	if( m_iJudge==2 ) m_iJudge = 1;	//1:OK

	return bReval;

}


