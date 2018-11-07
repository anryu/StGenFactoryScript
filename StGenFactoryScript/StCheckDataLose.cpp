#include "StdAfx.h"
#include "StCheckDataLose.h"
#include "StCommon.h"


StCheckDataLose::StCheckDataLose(void)
{
	Initialize();
}

StCheckDataLose::StCheckDataLose(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
{
	Initialize();
}

StCheckDataLose::~StCheckDataLose(void)
{
	if( m_pdblResultDifference ) delete [] m_pdblResultDifference;
	if( m_piResultHighLow ) delete [] m_piResultHighLow;
	if( m_piResultLine ) delete [] m_piResultLine;
	//▼1.0.0.1063
	m_pdblResultDifference = NULL;
	m_piResultHighLow = NULL;
	m_piResultLine = NULL;
	//▲1.0.0.1063

}

void StCheckDataLose::Initialize(void)
{
	m_nStart = 0;
	m_nEnd = 2047;
	m_dblDifferenceMax = 0.0;
	m_szCSVFileName = "";

	m_iResultElement = 0;
	m_pdblResultDifference = NULL;
	m_piResultHighLow = NULL;
	m_piResultLine = NULL;

	//mDeviceModelName = "";
	//mSerialNumber = "";
	mSaveFilePath = ".";
}

//▼1.0.0.1063
BOOL StCheckDataLose::SaveCheckExecute(StBuffer *aBuffer, BOOL bCheck, LPCTSTR szDateFormat, LPCTSTR szFileName )
{
	BOOL bReval = FALSE;
	INT nEnd = m_nEnd;
	INT nStart = m_nStart;

	size_t nElement = GetElementFromBuffer(aBuffer);
	double *pdblDifference = NULL;
	INT *piHighLow = NULL;
	INT *piLine = NULL;

	double **dblLineAverage = NULL;
	INT **piLineMin = NULL;
	INT **piLineMax = NULL;
	INT nLines = m_nEnd - nStart + 1;

	do
	{
		pdblDifference = new double [nElement];
		if( !pdblDifference ) break;
		piHighLow = new INT [nElement];
		if( !piHighLow ) break;
		piLine = new INT [nElement];
		if( !piLine ) break;
		//----------------------
		dblLineAverage = new double * [nElement];
		if( !dblLineAverage ) break;
		memset( dblLineAverage, NULL, sizeof(double *) * nElement );
		double *pDblBuffer = NULL;
		for( int i=0; i<(int)nElement; i++ )
		{
			dblLineAverage[i] = pDblBuffer = new double [nLines];
			if( !pDblBuffer ) break;
		}
		if( !pDblBuffer ) break;
		//----------------------
		piLineMin = new INT * [nElement];
		if( !piLineMin ) break;
		memset( piLineMin, NULL, sizeof(INT *) * nElement );
		INT *piBuffer = NULL;
		for( int i=0; i<(int)nElement; i++ )
		{
			piLineMin[i] = piBuffer = new INT [nLines];
			if( !piBuffer ) break;
		}
		if( !piBuffer ) break;
		//----------------------
		piLineMax = new INT * [nElement];
		if( !piLineMax ) break;
		memset( piLineMax, NULL, sizeof(INT *) * nElement );
		for( int i=0; i<(int)nElement; i++ )
		{
			piLineMax[i] = piBuffer = new INT [nLines];
			if( !piBuffer ) break;
		}
		if( !piBuffer ) break;

		bReval = GetLineAverageDifference( aBuffer, nStart, nEnd, pdblDifference, piHighLow, piLine, &nElement
										   , dblLineAverage, piLineMin, piLineMax, nLines );
		if( bReval )
		{
			if( bCheck )
			{
				m_iResultElement = nElement;
				SetElementColorIndexFromBuffer(m_Buffer);

				if( m_pdblResultDifference ) delete [] m_pdblResultDifference;
				m_pdblResultDifference = new double [m_iResultElement];
				if( m_piResultHighLow ) delete [] m_piResultHighLow;
				m_piResultHighLow = new INT [m_iResultElement];
				if( m_piResultLine ) delete [] m_piResultLine;
				m_piResultLine = new INT [m_iResultElement];

				if( m_pdblResultDifference )
					memcpy( m_pdblResultDifference, pdblDifference, sizeof(double) * m_iResultElement);
				if( m_piResultHighLow )
					memcpy( m_piResultHighLow, piHighLow, sizeof(INT) * m_iResultElement);
				if( m_piResultLine )
					memcpy( m_piResultLine, piLine, sizeof(INT) * m_iResultElement);
				//判定
				for( int i=0; i<(int)nElement; i++ )
				{
					if( pdblDifference[i]>GetDifferenceMax() )
						m_iJudge=0;
				}
				if( m_iJudge==2 )
					m_iJudge=1;
			}

			//CSV保存
			if( m_szCSVFileName.GetLength()>0 || szFileName )
			{
				SaveResultCSVFile( nElement, dblLineAverage, piLineMin, piLineMax, nLines, szDateFormat, szFileName );
			}
		}
		else
		{
			if( bCheck )
				m_iJudge = 0;	//0:NG
		}

	}while(0);

	if(pdblDifference) delete [] pdblDifference;
	if(piHighLow) delete [] piHighLow;
	if(piLine) delete [] piLine;

	//----------------------
	if( dblLineAverage )
	{
		for( int i=0; i<(int)nElement; i++ )
		{
			double * pdblAverage =  dblLineAverage[i];
			if( pdblAverage ) delete [] pdblAverage;
		}
		delete [] dblLineAverage;
	}
	//----------------------
	if( piLineMin )
	{
		for( int i=0; i<(int)nElement; i++ )
		{
			INT * pLine =  piLineMin[i];
			if( pLine ) delete [] pLine;
			//if( piLineMin[i] ) delete [] piLineMin[i];
		}
		delete [] piLineMin;
	}
	
	//----------------------
	if( piLineMax )
	{
		for( int i=0; i<(int)nElement; i++ )
		{
			INT * pLine =  piLineMax[i];
			if( pLine ) delete [] pLine;
			//if( piLineMax[i] ) delete [] piLineMax[i];
		}
		delete [] piLineMax;
	}

	if( bCheck )
	{
		if( m_iJudge==2 )
			m_iJudge = 0;
	}

	return bReval;
}
//▲1.0.0.1063

//-------------------------------------------------------------------------------
//検査実行
//-------------------------------------------------------------------------------
BOOL StCheckDataLose::Execute(void)
{
	BOOL bReval = FALSE;
	m_iJudge = 2;	//2:処理中
	if( m_Buffer ) delete m_Buffer;
	m_Buffer = new StBuffer;
	m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー

	//▼1.0.0.1063
	bReval = SaveCheckExecute(m_Buffer);

//	size_t nElement = GetElementFromBuffer(m_Buffer);
//	double *pdblDifference = NULL;
//	INT *piHighLow = NULL;
//	INT *piLine = NULL;
//
//	double **dblLineAverage = NULL;
//	INT **piLineMin = NULL;
//	INT **piLineMax = NULL;
//	INT nLines = m_nEnd - m_nStart + 1;
//
//	do
//	{
//		pdblDifference = new double [nElement];
//		if( !pdblDifference ) break;
//		piHighLow = new INT [nElement];
//		if( !piHighLow ) break;
//		piLine = new INT [nElement];
//		if( !piLine ) break;
//		//----------------------
//		dblLineAverage = new double * [nElement];
//		if( !dblLineAverage ) break;
//		memset( dblLineAverage, NULL, sizeof(double *) * nElement );
//		double *pDblBuffer = NULL;
//		for( int i=0; i<(int)nElement; i++ )
//		{
//			dblLineAverage[i] = pDblBuffer = new double [nLines];
//			if( !pDblBuffer ) break;
//		}
//		if( !pDblBuffer ) break;
//		//----------------------
//		piLineMin = new INT * [nElement];
//		if( !piLineMin ) break;
//		memset( piLineMin, NULL, sizeof(INT *) * nElement );
//		INT *piBuffer = NULL;
//		for( int i=0; i<(int)nElement; i++ )
//		{
//			piLineMin[i] = piBuffer = new INT [nLines];
//			if( !piBuffer ) break;
//		}
//		if( !piBuffer ) break;
//		//----------------------
//		piLineMax = new INT * [nElement];
//		if( !piLineMax ) break;
//		memset( piLineMax, NULL, sizeof(INT *) * nElement );
//		for( int i=0; i<(int)nElement; i++ )
//		{
//			piLineMax[i] = piBuffer = new INT [nLines];
//			if( !piBuffer ) break;
//		}
//		if( !piBuffer ) break;
//
////TRACE(TEXT("@@@@@@@@@ in dblLineAverage[%d]=0x%x\n"), 0, dblLineAverage[0] );
//
//		bReval = GetLineAverageDifference( m_Buffer, m_nStart, m_nEnd, pdblDifference, piHighLow, piLine, &nElement
//										   , dblLineAverage, piLineMin, piLineMax, nLines );
//										   //, (double (*)[])dblLineAverage, (INT (*)[])piLineMin, (INT (*)[])piLineMax, nLines );
////TRACE(TEXT("@@@@@@@@@ out dblLineAverage[%d]=0x%x\n"), 0, dblLineAverage[0] );
//		if( bReval )
//		{
//			m_iResultElement = nElement;
//			SetElementColorIndexFromBuffer(m_Buffer);
//
//			if( m_pdblResultDifference ) delete [] m_pdblResultDifference;
//			m_pdblResultDifference = new double [m_iResultElement];
//			if( m_piResultHighLow ) delete [] m_piResultHighLow;
//			m_piResultHighLow = new INT [m_iResultElement];
//			if( m_piResultLine ) delete [] m_piResultLine;
//			m_piResultLine = new INT [m_iResultElement];
//
//			if( m_pdblResultDifference )
//				memcpy( m_pdblResultDifference, pdblDifference, sizeof(double) * m_iResultElement);
//			if( m_piResultHighLow )
//				memcpy( m_piResultHighLow, piHighLow, sizeof(INT) * m_iResultElement);
//			if( m_piResultLine )
//				memcpy( m_piResultLine, piLine, sizeof(INT) * m_iResultElement);
//
//			//判定
//			for( int i=0; i<(int)nElement; i++ )
//			{
//				if( pdblDifference[i]>m_dblDifferenceMax )
//					m_iJudge=0;
//			}
//			if( m_iJudge==2 )
//				m_iJudge=1;
//
//			//CSV保存
//			if( m_szCSVFileName.GetLength()>0 )
//			{
//				SaveResultCSVFile( nElement, dblLineAverage, piLineMin, piLineMax, nLines );
//				//SaveResultCSVFile( nElement, (double (*)[])dblLineAverage, (INT (*)[])piLineMin, (INT (*)[])piLineMax, nLines );
//			}
//		}
//		else
//		{
//			m_iJudge = 0;	//0:NG
//		}
//
//	}while(0);
//
//	if(pdblDifference) delete [] pdblDifference;
//	if(piHighLow) delete [] piHighLow;
//	if(piLine) delete [] piLine;
//
//	//----------------------
//	if( dblLineAverage )
//	{
////TRACE(TEXT("@@@@@@@@@ dblLineAverage=0x%x\n"), dblLineAverage );
//		for( int i=0; i<(int)nElement; i++ )
//		{
////TRACE(TEXT("@@@@@@@@@ dblLineAverage[%d]=0x%x\n"), i, dblLineAverage[i] );
//			double * pdblAverage =  dblLineAverage[i];
//			if( pdblAverage ) delete [] pdblAverage;
//			//if( dblLineAverage[i] ) delete [] dblLineAverage[i];
//		}
//		delete [] dblLineAverage;
//	}
//	//----------------------
//	if( piLineMin )
//	{
//		for( int i=0; i<(int)nElement; i++ )
//		{
//			INT * pLine =  piLineMin[i];
//			if( pLine ) delete [] pLine;
//			//if( piLineMin[i] ) delete [] piLineMin[i];
//		}
//		delete [] piLineMin;
//	}
//	
//	//----------------------
//	if( piLineMax )
//	{
//		for( int i=0; i<(int)nElement; i++ )
//		{
//			INT * pLine =  piLineMax[i];
//			if( pLine ) delete [] pLine;
//			//if( piLineMax[i] ) delete [] piLineMax[i];
//		}
//		delete [] piLineMax;
//	}
//
//	if( m_iJudge == 2 )
//		m_iJudge = 0;

	//▲1.0.0.1063
	return bReval;
}

void StCheckDataLose::SetStart(INT nData)
{
	m_nStart = nData;
}
void StCheckDataLose::SetEnd(INT nData)
{
	m_nEnd = nData;
}
void StCheckDataLose::SetDifferenceMax(double dblData)
{
	m_dblDifferenceMax = dblData;
}
void StCheckDataLose::SetCSVFileName(LPCTSTR szData)
{
	m_szCSVFileName = szData;
}

BOOL StCheckDataLose::function(LPCTSTR szFunc, LPCTSTR szArgument)
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
		INT nData =_tcstol(szArgument,NULL,0);
		SetEnd(nData);
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
	if( szFunction=="SetCSVFileName" )
	{
		SetCSVFileName(szArgument);
		bReval = TRUE;
	}

	return bReval;
}

INT StCheckDataLose::GetResultElement(void)
{
	return m_iResultElement;
}

double StCheckDataLose::GetResultDifference(int nIndex)
{
	if( nIndex>=m_iResultElement ) return -1.0;
	return m_pdblResultDifference[nIndex];
}

INT StCheckDataLose::GetResultHighLow(int nIndex)
{
	if( nIndex>=m_iResultElement ) return -1;
	return m_piResultHighLow[nIndex];
}

INT StCheckDataLose::GetResultLine(int nIndex)
{
	if( nIndex>=m_iResultElement ) return -1;
	return m_piResultLine[nIndex];
}
//▼1.0.0.1063
BOOL StCheckDataLose::SaveResultCSVFile( INT nElement, double **pdblLineAverage, INT **piLineMin, INT **piLineMax, size_t bufferSize, LPCTSTR szDateFormat, LPCTSTR szCSVFileName )
{

	INT nLines = m_nEnd - m_nStart + 1;
	if( nLines<1 ) return FALSE;
	if( nElement!=1 && nElement!=3 ) return FALSE;

	CString szFullFileName;
	CString szText;
	{
		CString szFileName;
		BOOL bReval;

		CString szDeviceModelName;
		CString szSerialNumber;
		StString aText;
		bReval = GetDeviceModelName(aText);
		if( bReval )
			szDeviceModelName = aText.GetUnicode();
		bReval = GetSerialNumber(aText);
		if( bReval )
			szSerialNumber = aText.GetUnicode();

		CTime gt=CTime::GetCurrentTime();
		CString dateFormat = gt.Format("%Y%m%d%H%M%S");
		if( szDateFormat )
		{
			dateFormat = szDateFormat;
		}
		szFileName = szDeviceModelName;
		if( szSerialNumber.GetLength()>0 )
		{
			szFileName += _T("_") + szSerialNumber;
		}
		CString cCSVFileName;
		if( szCSVFileName==NULL )	cCSVFileName = m_szCSVFileName;
		else						cCSVFileName = szCSVFileName;
		szFileName += _T("_") + dateFormat + _T("_") + cCSVFileName;
		if(  szFileName.Find(_T("."))<0 )
		{
			szFileName += _T(".csv");
		}
		//%H%M%S部分を削除
		dateFormat = dateFormat.Left(dateFormat.GetLength()-6);
		CString szFullPath = mSaveFilePath + _T("\\") + szDeviceModelName + _T("\\") + dateFormat + _T("\\csv\\");
		CStCommon::CheckDirectoryExist(szFullPath,TRUE);
		szFullFileName = szFullPath + szFileName;
	}

	CStdioFile stdFile;
	if( !stdFile.Open(szFullFileName,CFile::modeCreate|CFile::modeWrite|CFile::typeText ,NULL) )
	{
		return FALSE;
	}
	stdFile.SeekToBegin();

	if( nElement==1 )
	{

		szText.Format( _T("atpix,Average,Min,Max\n") );
		stdFile.WriteString(szText);	//1行目
		double *pdblAverage = &(*pdblLineAverage)[0];
		INT *piMin = &(*piLineMin)[0];
		INT *piMax = &(*piLineMax)[0];
		for( INT i=0; i<(INT)bufferSize; i++ )
		{
			szText.Format( _T("%d,%.02f,%d,%d\n"), i+m_nStart, pdblAverage[i], piMin[i], piMax[i] );
			stdFile.WriteString(szText);
		}
	}
	else
	if( nElement==3 )
	{
		szText.Format( _T("atpix,R:Average,R:Min,R:Max,G:Average,G:Min,G:Max,B:Average,B:Min,B:Max\n") );
		stdFile.WriteString(szText);	//1行目

		double *pdblAverageB = &(*pdblLineAverage)[0];
		INT *piMinB = &(*piLineMin)[0];
		INT *piMaxB = &(*piLineMax)[0];
		double *pdblAverageG = &(*pdblLineAverage)[1];
		INT *piMinG = &(*piLineMin)[1];
		INT *piMaxG = &(*piLineMax)[1];
		double *pdblAverageR = &(*pdblLineAverage)[2];
		INT *piMinR = &(*piLineMin)[2];
		INT *piMaxR = &(*piLineMax)[2];

		for( INT i=0; i<(INT)bufferSize; i++ )
		{
			szText.Format( _T("%d,%.02f,%d,%d,%.02f,%d,%d,%.02f,%d,%d\n"), i+m_nStart, pdblAverageR[i],piMinR[i],piMaxR[i],pdblAverageG[i],piMinG[i],piMaxG[i],pdblAverageB[i],piMinB[i],piMaxB[i] );
			stdFile.WriteString(szText);
		}
	}

	stdFile.Close();
	return TRUE;
}

/*
//BOOL StCheckDataLose::SaveResultCSVFile( INT nElement, double (*pdblLineAverage)[], INT (*piLineMin)[], INT (*piLineMax)[], size_t bufferSize )
BOOL StCheckDataLose::SaveResultCSVFile( INT nElement, double **pdblLineAverage, INT **piLineMin, INT **piLineMax, size_t bufferSize )
{
	INT nLines = m_nEnd - m_nStart + 1;
	if( nLines<1 ) return FALSE;
	if( nElement!=1 && nElement!=3 ) return FALSE;

	CString szFullFileName;
	CString szFileName;
	CString szFullPath;
	CString szText;
	CString szDeviceModelName;
	CString szSerialNumber;
	BOOL bReval;
	StString aText;
	bReval = GetDeviceModelName(aText);
	if( bReval )
		szDeviceModelName = aText.GetUnicode();
	bReval = GetSerialNumber(aText);
	if( bReval )
		szSerialNumber = aText.GetUnicode();

	CTime gt=CTime::GetCurrentTime();
	CString dateFormat = gt.Format("%Y%m%d%H%M%S");
	szFileName =  szDeviceModelName;
	if( szSerialNumber.GetLength()>0 )
	{
		szFileName += _T("_") + szSerialNumber;
	}
	szFileName += _T("_") + dateFormat + _T("_") + m_szCSVFileName;
	if(  szFileName.Find(_T("."))<0 )
	{
		szFileName += _T(".csv");
	}

	dateFormat = gt.Format("%Y%m%d");
	szFullPath = mSaveFilePath + _T("\\") + szDeviceModelName + _T("\\") + dateFormat + _T("\\csv\\");

	CStCommon::CheckDirectoryExist(szFullPath,TRUE);

	szFullFileName = szFullPath + szFileName;


	CStdioFile stdFile;
	if( !stdFile.Open(szFullFileName,CFile::modeCreate|CFile::modeWrite|CFile::typeText ,NULL) )
	{
		return FALSE;
	}
	stdFile.SeekToBegin();

	if( nElement==1 )
	{

		szText.Format( _T("atpix,Average,Min,Max\n") );
		stdFile.WriteString(szText);	//1行目
		double *pdblAverage = &(*pdblLineAverage)[0];
		INT *piMin = &(*piLineMin)[0];
		INT *piMax = &(*piLineMax)[0];
		for( INT i=0; i<(INT)bufferSize; i++ )
		{
			szText.Format( _T("%d,%.02f,%d,%d\n"), i+m_nStart, pdblAverage[i], piMin[i], piMax[i] );
			stdFile.WriteString(szText);
		}
	}
	else
	if( nElement==3 )
	{
		szText.Format( _T("atpix,R:Average,R:Min,R:Max,G:Average,G:Min,G:Max,B:Average,B:Min,B:Max\n") );
		stdFile.WriteString(szText);	//1行目

		double *pdblAverageB = &(*pdblLineAverage)[0];
		INT *piMinB = &(*piLineMin)[0];
		INT *piMaxB = &(*piLineMax)[0];
		double *pdblAverageG = &(*pdblLineAverage)[1];
		INT *piMinG = &(*piLineMin)[1];
		INT *piMaxG = &(*piLineMax)[1];
		double *pdblAverageR = &(*pdblLineAverage)[2];
		INT *piMinR = &(*piLineMin)[2];
		INT *piMaxR = &(*piLineMax)[2];

		for( INT i=0; i<(INT)bufferSize; i++ )
		{
			szText.Format( _T("%d,%.02f,%d,%d,%.02f,%d,%d,%.02f,%d,%d\n"), i+m_nStart, pdblAverageR[i],piMinR[i],piMaxR[i],pdblAverageG[i],piMinG[i],piMaxG[i],pdblAverageB[i],piMinB[i],piMaxB[i] );
			stdFile.WriteString(szText);
		}
	}

	stdFile.Close();
	return TRUE;
}
*/
//▲1.0.0.1063

//▼1.0.0.1025
BOOL StCheckDataLose::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetElement" )
	{
		dblValue = GetResultElement();
		bReval = TRUE;
	}
	else
	if( szFunction=="GetDifference" )
	{
		INT nIndex = _ttoi(szArg);
		dblValue = GetResultDifference(nIndex);
		bReval = TRUE;
	}
	else
	if( szFunction=="GetHighLow" )
	{
		INT nIndex = _ttoi(szArg);
		dblValue = GetResultHighLow(nIndex);
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
