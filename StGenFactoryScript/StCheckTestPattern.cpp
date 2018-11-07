#include "StdAfx.h"
#include "StCheckTestPattern.h"
#include "Common.h"


StCheckTestPattern::StCheckTestPattern(void)
{
}

StCheckTestPattern::StCheckTestPattern(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
{
	Initialize();
}

StCheckTestPattern::~StCheckTestPattern(void)
{
	if( m_pImageBuffer ) delete m_pImageBuffer;
}

void StCheckTestPattern::Initialize(void)
{
	m_nStart = 0;
	m_nEnd = 0;
	m_szFileName = "";
	m_szFilePath = "";

	m_pImageBuffer = NULL;
	m_nImageLineSize = 0;

	m_nResultX = -1;
	m_nResultY = -1;
}

BOOL StCheckTestPattern::GetTestPatternData(void)
{
	BOOL bReval = FALSE;
	do
	{
		if( m_szFileName.GetLength()==0 )
		{
			m_nErrorCode = StResult::Code::INVALID_PARAMETER;
			break;
		}
		if( m_nEnd<m_nStart)
		{
			m_nErrorCode = StResult::Code::INVALID_PARAMETER;
			break;
		}

		//バッファ作成
		INT nBufferSize = m_nEnd - m_nStart + 1;
		if( m_nImageLineSize!=nBufferSize )
		{
			if( m_pImageBuffer ) delete m_pImageBuffer;
			m_nImageLineSize = 0;
			m_pImageBuffer = new INT [nBufferSize];
			if( !m_pImageBuffer )
			{
				m_nErrorCode = StResult::Code::NOT_ENOUGH_MEMORY;
				break;
			}
			m_nImageLineSize = nBufferSize;
		}
		memset( m_pImageBuffer, -1, sizeof(*m_pImageBuffer)*nBufferSize );
		//ファイルオープン
		CString szFilePath;
		if( m_szFilePath.IsEmpty() )
		{
			GetModuleFilePath(szFilePath);
			szFilePath += _T("\\CONFIG\\TestPattern\\");
		}
		else
		{
			szFilePath = m_szFilePath;
			szFilePath += _T("\\");
		}

		StString szFileFullName = szFilePath + m_szFileName;
		CStdioFile aFile;
		if( !aFile.Open( szFileFullName, CFile::modeRead|CFile::typeText, NULL ) )
		{
			m_nErrorCode = StResult::Code::FILE_ERROR;
			break;
		}

		CString szTmp;
		aFile.SeekToBegin();
		while(aFile.ReadString(szTmp))
		{
			szTmp.Trim();
			INT nCommaPos = szTmp.Find(_T(","));
			if( nCommaPos<0 ) continue;
			INT nNo =_ttoi(szTmp.Left(nCommaPos)) - m_nStart;
			if( nNo>=0 && nNo<nBufferSize )
			{
				szTmp = szTmp.Mid(nCommaPos+1);
				nCommaPos = szTmp.Find(_T(","));
				if( nCommaPos ) szTmp = szTmp.Left(nCommaPos);
				INT nValue = _ttoi(szTmp);
				m_pImageBuffer[nNo] = nValue; 
			}
		}
		aFile.Close();
		//check
		for( int i=0; i<nBufferSize; i++ )
		{
			if( m_pImageBuffer[i]<0 )
			{
				m_nErrorCode = StResult::Code::FILE_ERROR;
				bReval = FALSE;
				break;
			}
		}

		bReval = TRUE;
	}while(0);
	if( !bReval )
	{
		if( m_pImageBuffer ) delete [] m_pImageBuffer;
		m_pImageBuffer = NULL;
		m_nImageLineSize = 0;
	}
	return bReval;
}

template <class X> BOOL CheckImage( X *ptRaw, INT nWidth, INT nHeight, INT nElement, PINT pnImageBuffer, size_t nImageBufferSize, INT nStartX, PINT pnX, PINT pnY )
{
	BOOL bReval = TRUE;

	INT nLineLength = nWidth * nElement;
	X *pTop = ptRaw + nStartX;
	for( int y=0; y<nHeight && bReval; y++ )
	{
		X *pData = pTop;
		for( size_t x=0; x<nImageBufferSize && bReval; x++ )
		{
			if( pnImageBuffer[x]!=*pData++ )
			{
				bReval = FALSE;
				if( pnX ) *pnX = x + nStartX;
				if( pnY ) *pnY = y;
			}
		}
		pTop += nLineLength;
	}
	return bReval;
}

BOOL StCheckTestPattern::CheckTestPattern(void)
{
	BOOL bReval = FALSE;
	StResult aResult;
	do
	{
		if( !m_pImageBuffer ) break;
		if( !m_Buffer ) break;
		INT nElement = GetElementFromBuffer(m_Buffer);
		INT nPixelSize = GetPixelSize(m_Buffer);
		INT nX = -1;
		INT nY = -1;
		if( nPixelSize==8 )
		{
			bReval = CheckImage( (unsigned char *)m_Buffer->GetDataPointer(), m_Buffer->GetWidth(), m_Buffer->GetHeight(), nElement, m_pImageBuffer, m_nImageLineSize, m_nStart, &nX, &nY );
		}
		else
		if( nPixelSize==16 )
		{
			bReval = CheckImage( (unsigned short *)m_Buffer->GetDataPointer(), m_Buffer->GetWidth(), m_Buffer->GetHeight(), nElement, m_pImageBuffer, m_nImageLineSize, m_nStart, &nX, &nY );
		}
		if( !bReval )
		{
			m_nResultX = nX;
			m_nResultY = nY;
		}
	}while(0);
	return bReval;
}

//-------------------------------------------------------------------------------
//検査実行
//-------------------------------------------------------------------------------
BOOL StCheckTestPattern::Execute(void)
{

	BOOL bReval = FALSE;
	m_iJudge = 2;	//2:処理中

	do
	{
		bReval = GetTestPatternData();
		if( !bReval ) break;

		if( m_Buffer ) delete m_Buffer;
		m_Buffer = new StBuffer;
		if( !m_Buffer )
		{
			m_nErrorCode = StResult::Code::NOT_ENOUGH_MEMORY;
			bReval = FALSE;
			break;
		}
		m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー
		
		//Check

		BOOL bCheck = CheckTestPattern();
		if( !bCheck )
		{
			m_iJudge = 0;
		}
		bReval = TRUE;
	}while(0);
	if( m_iJudge==2 ) m_iJudge = 1;	//1:OK

	return bReval;


}

BOOL StCheckTestPattern::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);

	if( szFunction=="SetStart" || szFunction=="Start" )
	{
		INT nData = _ttoi(szArgument);
		SetStart(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetEnd" || szFunction=="End" )
	{
		INT nData = _ttoi(szArgument);
		SetEnd(nData);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetFileName" || szFunction=="FileName" )
	{
		SetFileName(szArgument);
		bReval = TRUE;
	}

	return bReval;

}

//▼1.0.0.1025
BOOL StCheckTestPattern::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetX" )
	{
		dblValue = GetX();
		bReval = TRUE;
	}
	else
	if( szFunction=="GetY" )
	{
		dblValue = GetY();
		bReval = TRUE;
	}
	return bReval;
}
//▲1.0.0.1025

