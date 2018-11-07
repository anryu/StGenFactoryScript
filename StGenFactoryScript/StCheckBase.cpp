#include "StdAfx.h"
#include "StCheckBase.h"
#include <math.h>
#include "StBufferInfo.h"
#include "../CommonHeader/StTransformsImage.h"
#include <afx.h>
//▼1.0.0.1039
#include "../CommonHeader/StSaveFileImage.h"
//▲1.0.0.1039
//▼1.0.0.1044
#include "StDeviceEBus.h"
#include "StDeviceMultiCam.h"
//▲1.0.0.1044

StCheckBase::StCheckBase(void)
	: m_iJudge(-1)
	, m_Buffer(NULL)
	, m_DeviceBase(NULL)
	, m_pDialog(NULL)
{
	memset( m_nColorIndex, 0, sizeof(m_nColorIndex) );
	//▼1.0.0.1062
	m_nScriptCheckMode = 0;
	//▲1.0.0.1062

}

StCheckBase::StCheckBase(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialogEx)
	: m_iJudge(-1)
	, m_Buffer(NULL)
	, m_ClassName(szClassName)
	, m_VarName(szVarName)
	, m_DeviceBase(aDeviceBase)
	, m_pDialog(aDialogEx)
{
	memset( m_nColorIndex, 0, sizeof(m_nColorIndex) );
	//▼1.0.0.1062
	m_nScriptCheckMode = 0;
	//▲1.0.0.1062
}

StCheckBase::~StCheckBase(void)
{
	if( m_Buffer )
	{
		delete m_Buffer;
		m_Buffer = NULL;
	}
}


//▼1.0.0.1044


BOOL StCheckBase::WriteDataEBus(BYTE byteDeviceCode, BYTE bytePage, BYTE byteAddress, BYTE byteData)
{
	StDeviceEBus *aDevice = (StDeviceEBus *)m_DeviceBase;
	if( !aDevice->IsConnected() ) return FALSE;

	BYTE byteSendData[6];
	byteSendData[0] = 0x02;
	byteSendData[1] = (byteDeviceCode<<2) | 0x02 | (bytePage&1);
	byteSendData[2] = byteAddress;
	byteSendData[3] = 1;
	byteSendData[4] = byteData;
	byteSendData[5] = 0x03;
	BYTE byteRcvData[4] = {2,0,1,3};	//Write時の正常リード値
	//▼1.0.0.1060
	//BYTE GetRcvData[4+2];
	BYTE GetRcvData[4];
	//▲1.0.0.1060
	size_t getRcvSize = sizeof(GetRcvData);

	BOOL bRevCtrl = aDevice->SerialControl( _T("UART0"), byteSendData, sizeof(byteSendData), GetRcvData, &getRcvSize );
	if( bRevCtrl )
	{
		if( getRcvSize!=sizeof(byteRcvData) )
		{
			bRevCtrl = FALSE;
		}
	}
	if( bRevCtrl )
	{
		if( memcmp(GetRcvData,byteRcvData,getRcvSize)!=0 )
			bRevCtrl = FALSE;
	}

	return bRevCtrl;
}

BOOL StCheckBase::WriteDataMultiCamAscii(StString &szCommand, BYTE byteData)
{
	StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
	if( !aDevice->IsOpened() ) return FALSE;
	StSerialComm *aSerialComm = aDevice->GetSerialComm();
	if( !aSerialComm ) return FALSE;

	TCHAR szSendText[256];
	TCHAR szRcvText[256];

	CString szCommandText(szCommand.GetAscii());

	_stprintf_s( szSendText, _countof(szSendText), _T("%s=%u"), szCommandText, byteData );
	BOOL bReval = aSerialComm->SendText( szSendText, szRcvText, _countof(szRcvText) );

	TCHAR *szRcvCmpText = {_T("OK")};
	if( bReval )
	{
		if( _tcscmp(szRcvText,szRcvCmpText)!=0 )
			bReval = FALSE;
	}
	return bReval;
}

BOOL StCheckBase::WriteDataMultiCamAscii(StString &szCommand)
{
	StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
	if( !aDevice->IsOpened() ) return FALSE;
	StSerialComm *aSerialComm = aDevice->GetSerialComm();
	if( !aSerialComm ) return FALSE;

	TCHAR szSendText[256];
	TCHAR szRcvText[256];

	CString szCommandText(szCommand.GetAscii());

	_stprintf_s( szSendText, _countof(szSendText), _T("%s"), szCommandText );
	BOOL bReval = aSerialComm->SendText( szSendText, szRcvText, _countof(szRcvText) );

	TCHAR *szRcvCmpText = {_T("OK")};
	if( bReval )
	{
		if( _tcscmp(szRcvText,szRcvCmpText)!=0 )
			bReval = FALSE;
	}
	return bReval;
}



BOOL StCheckBase::WriteDataMultiCamBinary(BYTE byteDeviceCode, BYTE bytePage, BYTE byteAddress, BYTE byteData)
{
	StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
	if( !aDevice->IsOpened() ) return FALSE;
	StSerialComm *aSerialComm = aDevice->GetSerialComm();
	if( !aSerialComm ) return FALSE;

	BYTE byteSendData[6];
	byteSendData[0] = 0x02;
	byteSendData[1] = (byteDeviceCode<<2) | 0x02 | (bytePage&1);
	byteSendData[2] = byteAddress;
	byteSendData[3] = 1;
	byteSendData[4] = byteData;
	byteSendData[5] = 0x03;
	BYTE byteRcvData[4] = {2,0,1,3};	//Write時の正常リード値
	BYTE GetRcvData[4+2];
	size_t getRcvSize = sizeof(GetRcvData);

	BOOL bRevCtrl = aSerialComm->SendBin(byteSendData,sizeof(byteSendData),GetRcvData,&getRcvSize);

	if( bRevCtrl )
	{
		if( getRcvSize!=sizeof(byteRcvData) )
		{
			bRevCtrl = FALSE;
		}
	}
	if( bRevCtrl )
	{
		if( memcmp(GetRcvData,byteRcvData,getRcvSize)!=0 )
			bRevCtrl = FALSE;
	}

	return bRevCtrl;
}

//▲1.0.0.1044



//-----------------------------------------------------------------------------
//ビットマップ形式で保存
//-----------------------------------------------------------------------------
BOOL bSaveBitmapFile( DWORD dwWidth,DWORD dwHeight,DWORD dwSize, INT iColor, PBYTE pbyteSrcData, LPCTSTR fileName )
{

	LPBITMAPFILEHEADER	bmFileHeader;
	LPBITMAPINFOHEADER	bmInfoHeader;

	PBYTE	pbyteBuffer;
	DWORD	dwHeaderSize;
	DWORD	dwImageSize;
	DWORD	dwBufferSize;
	BOOL	bReval = FALSE;

	//4の倍数
	//DWORD dwBufferWidthLine = (dwWidth+3)&(~3);

	if( iColor==0 )
	{
		dwHeaderSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) << 8);
	}
	else
	{
		dwHeaderSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		//dwBufferWidthLine = (dwWidth*3+3)&(~3);
	}

	//dwImageSize = dwBufferWidthLine * dwHeight;
	dwImageSize = dwSize;


	dwBufferSize = dwHeaderSize + dwImageSize;

	//バッファ作成
	pbyteBuffer = (PBYTE)malloc(sizeof(BYTE) * (dwBufferSize));
	if(pbyteBuffer == NULL)
	{
		return(FALSE);
	}

	//ヘッダー部を0で初期化
	memset(pbyteBuffer, 0, dwHeaderSize);

	//画像データをコピー
	memcpy(pbyteBuffer + dwHeaderSize,pbyteSrcData,dwImageSize);

	//ファイルヘッダーをセット
	bmFileHeader = (LPBITMAPFILEHEADER)pbyteBuffer;
	bmFileHeader->bfType = (((WORD)'M') << 8) | (WORD)'B';
	bmFileHeader->bfSize = dwBufferSize;
	bmFileHeader->bfOffBits = dwHeaderSize;

	//インフォヘッダーをセット
	bmInfoHeader = (LPBITMAPINFOHEADER)(pbyteBuffer + sizeof(BITMAPFILEHEADER));
	bmInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
	bmInfoHeader->biWidth = dwWidth;
	bmInfoHeader->biHeight = (-1)*(INT)dwHeight;
	bmInfoHeader->biPlanes = 1;

	bmInfoHeader->biSizeImage = dwBufferSize;

	if(iColor==0)
	{
		//インフォヘッダーをセット
		bmInfoHeader->biBitCount = 8;
		bmInfoHeader->biClrUsed = 256;
		bmInfoHeader->biClrImportant = 256;
		//▼2007/01/24(0.0.3.4) 8ビットビットマップ用カラーテーブル作成を高速化
		//カラーテーブルをセット
		DWORD	dwCurPixelValue = 256;		//モノクロテーブル作成用
		PDWORD	pdwPosRGBTable = (PDWORD)((PBYTE)bmInfoHeader + sizeof(BITMAPINFOHEADER));
		do{
			dwCurPixelValue--;
			pdwPosRGBTable[dwCurPixelValue] = (dwCurPixelValue << 16) | (dwCurPixelValue << 8) | (dwCurPixelValue);
		}while(dwCurPixelValue);
	}
	else
	{
		//インフォヘッダーをセット
		bmInfoHeader->biBitCount = 24;
	}

	FILE	*fp=NULL;
	//fp = _tfopen(fileName,_T("wb"));
	_tfopen_s(&fp,fileName,_T("wb"));
	if(fp)
	{
		fwrite(pbyteBuffer,sizeof(BYTE),dwBufferSize,fp);
		bReval = TRUE;
		fclose(fp);
	}

	if( pbyteBuffer ) delete [] pbyteBuffer;

	return(bReval);
}


BOOL StCheckBase::SaveBitmapFile(StBuffer *pBuffer, LPCTSTR szFileFullName)
{
	UINT uiWidth = pBuffer->GetWidth();
	UINT uiHeight = pBuffer->GetHeight();
	INT iDestLinePitch = uiWidth;
	INT nElement =  GetElementFromBuffer(pBuffer);

//if( nElement==0 )
//{
//	StPixelType aType;
//	pBuffer->GetPixelType(aType);
//	TRACE(TEXT("GetPixelType(0x%x)\n"), aType);
//}

	if( nElement==3 )
	{
		iDestLinePitch *= 3;
	}
	//４の倍数に切り上げ
	iDestLinePitch = (iDestLinePitch + 3) & (~0x3);

	INT iDestBufferSize = iDestLinePitch * uiHeight;

	char *pDispBuffer = new char [iDestBufferSize];
	if( pDispBuffer==NULL ) return FALSE;
	//画像変換
	StBuffer dstBuffer;
	dstBuffer.Attach( pDispBuffer, iDestBufferSize );
	dstBuffer.SetWidth(uiWidth);
	dstBuffer.SetHeight(uiHeight);
	dstBuffer.SetLinePitch(iDestLinePitch);
	dstBuffer.SetPixelType(nElement==1?StPixelMono8:StPixelBGR8);
	StResult aTransformResult = StTransformsImage::Transform( pBuffer, &dstBuffer);
	dstBuffer.Detach();

	BOOL bReval = FALSE;
	if( aTransformResult.IsOK() )
	{
		bReval = bSaveBitmapFile( uiWidth, uiHeight, iDestBufferSize, nElement==3?1:0, (PBYTE)pDispBuffer, szFileFullName );
	}
	delete [] pDispBuffer;

	return bReval;
}

//▼1.0.0.1039
//PNG or Jpeg 拡張子で判断
BOOL StCheckBase::SaveImageFile(StBuffer *aBuffer, LPCTSTR szFileFullName)
{
	BOOL bReval = FALSE;

	StBuffer dstBuffer;
	INT nDestLinePitch = aBuffer->GetWidth() * 3;
	nDestLinePitch = (nDestLinePitch + 3) & (~0x3);
	dstBuffer.Alloc( nDestLinePitch * aBuffer->GetHeight() );
	dstBuffer.SetWidth(aBuffer->GetWidth());
	dstBuffer.SetHeight(aBuffer->GetHeight());
	dstBuffer.SetLinePitch(nDestLinePitch);
	dstBuffer.SetPixelType(StPixelBGR8);

	//	Converter
	bReval = StTransformsImage::Transform( aBuffer, &dstBuffer).IsOK();
	if( bReval )
	{
		StSaveFileImage aSaveFileImage;
		bReval = aSaveFileImage.executeFileSave(szFileFullName, (const unsigned char *)dstBuffer.GetDataPointer(), aBuffer->GetWidth(), aBuffer->GetHeight(), 1.0, 1.0);
	}
	dstBuffer.Free();

	return bReval;

}
//▲1.0.0.1039

//===============================================================================
//dblOutAverage:全画面のレベル平均
//dblOutSDAve:縦方向１ラインの標準偏差の平均
//dblOutSDMax:縦方向１ラインの標準偏差の最大値
//dblOutSDMPix:縦方向１ラインの標準偏差の最大値の位置
//===============================================================================
template <class X> BOOL GetSNAverageValue( X *ptRaw, WORD wElementsPerPixel, DWORD dwWidth, DWORD dwHeight, DWORD nSrcLinePitch, DWORD dwStartLine, DWORD dwEndLine
			, double *dblOutAverage, double *dblOutSDAve, double *dblOutSDMax, int *nOutSDMPix, double **ppdblOutAverage, double **ppdblOutSDAve, size_t buffersize )
{
	if( dwEndLine<dwStartLine )
		return FALSE;
	if( dwHeight==0 )
		return FALSE;
	if( dwStartLine>=dwWidth )
		return FALSE;
	if( dwEndLine>=dwWidth )
		return FALSE;

	for( int i=0; i< wElementsPerPixel; i++ )		//0:Blue 1:Greeeen 2:Red   0:BW
	{
		double *pdblOutAverage = NULL;
		double *pdblOutSDAve = NULL;
		if( ppdblOutAverage ) pdblOutAverage = ppdblOutAverage[i];
		if( ppdblOutSDAve ) pdblOutSDAve = ppdblOutSDAve[i];

		double dblSDAve = 0.0;
		double dblSDMax = 0.0;
		int nSDMPix = 0;
		double sumData=0.0;

		X *pImgTop = ptRaw + i;
		pImgTop += dwStartLine * wElementsPerPixel;
		for( DWORD x=dwStartLine; x<=dwEndLine; x++ )
		{
			double dblSigXn = 0.0;
			double dblSigXn2 = 0.0;
			X *pImg = pImgTop;
			
			for( DWORD y=0; y<dwHeight; y++ )	//縦方向の合計
			{
				dblSigXn += *pImg;
				dblSigXn2 += (*pImg)*(*pImg);
				PBYTE pbImg =  (PBYTE)pImg;
				pImg = (X *)(pbImg + nSrcLinePitch);
			}
			if( buffersize>x-dwStartLine )
			{
				if( pdblOutAverage )
					pdblOutAverage[x-dwStartLine] = dblSigXn / dwHeight;
			}
			double dSD = sqrt( (double)(dwHeight*dblSigXn2-dblSigXn*dblSigXn)/(dwHeight*(dwHeight-1)) );		//縦方向の標準偏差
		
			if( buffersize>x-dwStartLine )
			{
				if( pdblOutSDAve )
					pdblOutSDAve[x-dwStartLine] = dSD;
			}

			if( dSD > dblSDMax )		//最大標準偏差
			{
				dblSDMax = dSD;
				nSDMPix = x;
			}
			dblSDAve += dSD;

			//平均用
			sumData += dblSigXn;
			
			pImgTop += wElementsPerPixel;
		}

		double dblAverage = sumData / ( dwHeight * (dwEndLine - dwStartLine + 1) );
		dblSDAve /= (dwEndLine - dwStartLine + 1);
		//代入
		dblOutAverage[i] = dblAverage;
		dblOutSDAve[i] = dblSDAve;
		dblOutSDMax[i] = dblSDMax;
		nOutSDMPix[i] = nSDMPix;
	}
	return TRUE;
}


BOOL StCheckBase::SaveCSVFile(StBuffer *aBuffer, LPCTSTR szFilePath, LPCTSTR szFileName, INT nScopeLeft, INT nScopeRight )
{
	UINT uiWidth = aBuffer->GetWidth();
	UINT uiHeight = aBuffer->GetHeight();
	//INT iDestLinePitch = uiWidth;
	INT nElement =  GetElementFromBuffer(aBuffer);
	INT nPixelSize = GetPixelSize(aBuffer);

	if( nScopeLeft<0 ) nScopeLeft = 0;
	if( nScopeRight<0 ) nScopeRight = uiWidth-1;

	BOOL bReval = TRUE;

	//数値解析
	double *dblCalcAve = NULL;
	double *dblCalcSDAve = NULL;
	double *dblCalcSDMax = NULL;
	int *nCalcSDMaxPix = NULL;

	double **ppdblCalcAve = NULL;
	double **ppdblCalcSDMax = NULL;

	do
	{
		//バッファ作成//---------------------------
		dblCalcAve = new double [nElement];
		if( !dblCalcAve ) break;
		dblCalcSDAve = new double [nElement];
		if( !dblCalcSDAve ) break;
		dblCalcSDMax = new double [nElement];
		if( !dblCalcSDMax ) break;
		nCalcSDMaxPix = new int [nElement];
		if( !nCalcSDMaxPix ) break;

		ppdblCalcAve = new double *[nElement];
		if( !ppdblCalcAve ) break;
		memset( ppdblCalcAve, 0, sizeof(double *) * nElement);

		ppdblCalcSDMax = new double *[nElement];
		if( !ppdblCalcSDMax ) break;
		memset( ppdblCalcSDMax, 0, sizeof(double *) * nElement);

		size_t bufferSize = nScopeRight - nScopeLeft + 1;
		for( int i=0; i<nElement; i++ )
		{
			ppdblCalcAve[i] = new double [bufferSize];
			if( !ppdblCalcAve[i] )
			{
				bReval = FALSE;
				break;
			}
			ppdblCalcSDMax[i] = new double [bufferSize];
			if( !ppdblCalcSDMax[i] )
			{
				bReval = FALSE;
				break;
			}
		}
		if( !bReval ) break;
		//---------------------------
		if( (nPixelSize/nElement)==8 )
		{
			bReval = GetSNAverageValue( (unsigned char *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), nScopeLeft, nScopeRight
				, dblCalcAve, dblCalcSDAve, dblCalcSDMax, nCalcSDMaxPix, ppdblCalcAve, ppdblCalcSDMax, bufferSize );
		}
		else
		if( (nPixelSize/nElement)==16 )
		{
			bReval = GetSNAverageValue( (unsigned short *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), nScopeLeft, nScopeRight
				, dblCalcAve, dblCalcSDAve, dblCalcSDMax, nCalcSDMaxPix, ppdblCalcAve, ppdblCalcSDMax, bufferSize );
		}

		if( !bReval ) break;

		CString szFileFullName = szFilePath;
		szFileFullName += szFileName;

		CStdioFile aFile;
		if( !aFile.Open( szFileFullName, CFile::modeCreate|CFile::modeWrite|CFile::typeText, NULL ) )
		{
			bReval = FALSE;
			break;
		}

		aFile.SeekToBegin();

		CString szTmp;
		//Line No.1
		szTmp = _T("FileName=[");
		szTmp += szFileName;
		szTmp += _T("]");
		
		aFile.WriteString(szTmp + _T("\n"));

		//Line No.2
		szTmp.Format(_T("ImageSize=(%u/%u) AverageRange=(%d/%d)"),uiWidth, uiHeight, nScopeLeft, nScopeRight );
		aFile.WriteString(szTmp + _T("\n"));


		LPCTSTR szColor[] = {_T("Red"),_T("Green"),_T("Blue")};
		for( int i=0; i<nElement; i++ )
		{
			CString aColor(_T(""));
			if( nElement==3 )
			{
				aColor = szColor[i];
				aColor += _T(":");
			}
			szTmp.Format(_T("%sAverage=[%.02f]/SDAve=[%.06f]/SDMax=[%.06f]at[%d]pix"), aColor, dblCalcAve[i], dblCalcSDAve[i], dblCalcSDMax[i], nCalcSDMaxPix[i] );
			aFile.WriteString(szTmp + _T("\n"));
		}

		if( nElement==3 )
		{
			szTmp.Format(_T("No.,%s:Average,%s:SD,%s:Average,%s:SD,%s:Average,%s:SD"), szColor[0],  szColor[0],  szColor[1],  szColor[1],  szColor[2],  szColor[2] );
			aFile.WriteString(szTmp + _T("\n"));
		
			for( int x=0; x< (int)bufferSize; x++ )
			{
				szTmp.Format(_T("%d,%.02f,%.06f,%.02f,%.06f,%.02f,%.06f") , x + nScopeLeft, ppdblCalcAve[0][x], ppdblCalcSDMax[0][x], ppdblCalcAve[1][x], ppdblCalcSDMax[1][x], ppdblCalcAve[2][x], ppdblCalcSDMax[2][x] );
				aFile.WriteString(szTmp + _T("\n"));
			}
		}
		else
		{
			szTmp.Format(_T("No.,Average,SD"));
			aFile.WriteString(szTmp + _T("\n"));
			for( int x=0; x< (int)bufferSize; x++ )
			{
				szTmp.Format(_T("%d,%.02f,%.06f") , x + nScopeLeft, ppdblCalcAve[0][x], ppdblCalcSDMax[0][x] );
				aFile.WriteString(szTmp + _T("\n"));
			}
		}

		aFile.Close();

		bReval = TRUE;
	}while(0);

	if( dblCalcAve ) delete [] dblCalcAve;
	if( dblCalcSDAve ) delete [] dblCalcSDAve;
	if( dblCalcSDMax ) delete [] dblCalcSDMax;
	if( nCalcSDMaxPix ) delete [] nCalcSDMaxPix;

	if( ppdblCalcAve )
	{
		for( int i=0; i<nElement; i++ )
		{
			delete [] ppdblCalcAve[i];
		}
		delete [] ppdblCalcAve;
	}

	if( ppdblCalcSDMax )
	{
		for( int i=0; i<nElement; i++ )
		{
			delete [] ppdblCalcSDMax[i];
		}
		delete [] ppdblCalcSDMax;
	}

	return bReval;
}

//▼1.0.0.1055





//===============================================================================
//ppHistogramData:ヒストグラムデータ出力[wElementsPerPixel][nDataSize]
//===============================================================================
template <class X> BOOL GetHistogramValue( X *ptRaw, WORD wElementsPerPixel, DWORD dwWidth, DWORD dwHeight, DWORD nSrcLinePitch, DWORD dwStartLine, DWORD dwEndLine
			, INT nDataSize, PINT *ppHistogramData )
{
	if( dwEndLine<dwStartLine )
		return FALSE;
	if( dwHeight==0 )
		return FALSE;
	if( dwStartLine>=dwWidth )
		return FALSE;
	if( dwEndLine>=dwWidth )
		return FALSE;

	for( int i=0; i< wElementsPerPixel; i++ )		//0:Blue 1:Greeeen 2:Red   0:BW
	{
		PINT pHistogramData = ppHistogramData[i];
		memset( pHistogramData, 0, sizeof(INT)*nDataSize );

		X *ptLineTop = ptRaw + i;
		for( int y=0; y<dwHeight; y++ )
		{
			X *ptData = ptLineTop;
			for( int x=0; x<dwWidth; x++ )
			{
				if( x>=dwStartLine && x<=dwEndLine )
				{
					if( *ptData>=0 && *ptData<nDataSize )
					{
						pHistogramData[*ptData]++;
					}
				}
				ptData += wElementsPerPixel;
			}
			PBYTE pbyteLineTop = (PBYTE)ptLineTop;
			ptLineTop = (X *)(pbyteLineTop + nSrcLinePitch);
		}
	}
	return TRUE;
}

BOOL StCheckBase::SaveHistogramFile(StBuffer *aBuffer, LPCTSTR szFilePath, LPCTSTR szFileName, INT nScopeLeft, INT nScopeRight )
{
	UINT uiWidth = aBuffer->GetWidth();
	UINT uiHeight = aBuffer->GetHeight();
	//INT iDestLinePitch = uiWidth;
	INT nElement =  GetElementFromBuffer(aBuffer);
	INT nPixelSize = GetPixelSize(aBuffer);
	INT nPixelBits = GetPixelBits(aBuffer);
	INT nDataSize = (1<<nPixelBits);

	if( nScopeLeft<0 ) nScopeLeft = 0;
	if( nScopeRight<0 ) nScopeRight = uiWidth-1;

	BOOL bReval = FALSE;

	//数値解析
	//double *dblCalcAve = NULL;
	//double *dblCalcSDAve = NULL;
	//double *dblCalcSDMax = NULL;
	//int *nCalcSDMaxPix = NULL;

	//double **ppdblCalcAve = NULL;
	//double **ppdblCalcSDMax = NULL;

	PINT *ppHistogramData = NULL;
	do
	{
		//バッファ作成//---------------------------
		
		ppHistogramData = new PINT [nElement];
		if( !ppHistogramData ) break;
		bReval = TRUE;
		for( int i=0; i<nElement; i++ )
		{
			ppHistogramData[i] = new INT [nDataSize];
			if( ppHistogramData[i]==NULL )
			{
				bReval = FALSE;
				break;
			}
			memset( ppHistogramData[i], 0, sizeof(INT)*nDataSize );
		}
		if( !bReval ) break;

		//---------------------------
		if( (nPixelSize/nElement)==8 )
		{
			bReval = GetHistogramValue( (unsigned char *)aBuffer->GetDataPointer(), nElement, uiWidth, uiHeight, aBuffer->GetLinePitch(), nScopeLeft, nScopeRight
				, nDataSize, ppHistogramData );
		}
		else
		if( (nPixelSize/nElement)==16 )
		{
			bReval = GetHistogramValue( (unsigned short *)aBuffer->GetDataPointer(), nElement, uiWidth, uiHeight, aBuffer->GetLinePitch(), nScopeLeft, nScopeRight
				, nDataSize, ppHistogramData );
		}

		if( !bReval ) break;

		CString szFileFullName = szFilePath;
		szFileFullName += szFileName;

		CStdioFile aFile;
		if( !aFile.Open( szFileFullName, CFile::modeCreate|CFile::modeWrite|CFile::typeText, NULL ) )
		{
			bReval = FALSE;
			break;
		}

		aFile.SeekToBegin();

		CString szText;
		CString szTmp;
		//Line No.1
		//szTmp = _T("FileName=[");
		//szTmp += szFileName;
		//szTmp += _T("]");
		//
		//aFile.WriteString(szTmp + _T("\n"));

		//Line No.2
		//szTmp.Format(_T("ImageSize=(%u/%u) HistogramRange=(%d/%d)"),uiWidth, uiHeight, nScopeLeft, nScopeRight );
		//aFile.WriteString(szTmp + _T("\n"));

		if( nElement==3 )
		{
			szText = "\"data\",\"Red count\",\"Green count\",\"Blue count\"\n";
		}
		else
		if( nElement==1 )
		{
			szText = "\"data\",\"count\"\n";
		}
		else
		{
			szText.Format(_T("\"data\""));
			for( int j=0; j<nElement; j++ )
			{
				szTmp.Format(_T(",\"count%i\""), j );
				szText+=szTmp;
			}
			szText+=(_T("\n"));
		}
		aFile.WriteString(szText);

		for( int i=0; i<nDataSize; i++ )
		{
			szText.Format(_T("%i"),i);
			for( int j=0; j<nElement; j++ )
			{
				szTmp.Format(_T(",%i"), ppHistogramData[j][i] );
				szText+=szTmp;
			}
			szText+=(_T("\n"));
			aFile.WriteString(szText);
		}

		aFile.Close();

		bReval = TRUE;
	}while(0);

	if( ppHistogramData )
	{
		for( int i=0; i<nElement; i++ )
		{
			if( ppHistogramData[i] )
			{
				delete [] ppHistogramData[i];
			}
		}
		delete [] ppHistogramData;
	}
	return bReval;
}
//▲1.0.0.1055

//StBufferからElement取得
INT StCheckBase::GetElementFromBuffer(StBuffer *pBuffer)
{
	StBufferInfo aInfo(pBuffer);

	return aInfo.GetElement();
}

//StBufferからBitsPerPixel取得
INT StCheckBase::GetPixelSize(StBuffer *pBuffer)
{
	StBufferInfo aInfo(pBuffer);

	return aInfo.GetPixelSize();
}

//▼1.0.0.1055
//StBufferからPixelBits取得
INT StCheckBase::GetPixelBits(StBuffer *pBuffer)
{
	StBufferInfo aInfo(pBuffer);

	return aInfo.GetPixelBits();
}
//▲1.0.0.1055

//StBufferからColorIndex取得しセット

INT StCheckBase::SetElementColorIndexFromBuffer(StBuffer *pBuffer)
{
	StBufferInfo aInfo(pBuffer);

	for( int i=0; i<_countof(m_nColorIndex); i++ )
	{
		m_nColorIndex[i] = aInfo.GetElementColorIndex(i);
	}
	return TRUE;
}

INT StCheckBase::GetElementColorIndex(INT iPos)
{
	return m_nColorIndex[iPos];
}


//StBufferからColorIndex取得
//0:R 1:G 2:B 3:A -1:Mono 
INT StCheckBase::GetElementColorIndexFromBuffer(StBuffer *pBuffer, INT iPos)
{
	StBufferInfo aInfo(pBuffer);

	return aInfo.GetElementColorIndex(iPos);
}


//===============================================================================
//dblOutAverage:全画面のレベル平均
//dblOutSDAve:縦方向１ラインの標準偏差の平均
//dblOutSDMax:縦方向１ラインの標準偏差の最大値
//dblOutSDMPix:縦方向１ラインの標準偏差の最大値の位置
//===============================================================================
/*
template <class X> BOOL GetSNAverageValue( X *ptRaw, WORD wElementsPerPixel, DWORD dwWidth, DWORD dwHeight, DWORD nSrcLinePitch, DWORD dwStartLine, DWORD dwEndLine
			, double *dblOutAverage, double *dblOutSDAve, double *dblOutSDMax, int *nOutSDMPix )
{
	if( dwEndLine<dwStartLine ) return FALSE;
	if( dwHeight==0 ) return FALSE;
	if( dwStartLine>=dwWidth ) return FALSE;
	if( dwEndLine>=dwWidth ) return FALSE;

	for( int i=0; i< wElementsPerPixel; i++ )		//0:Blue 1:Greeeen 2:Red   0:BW
	{
		double dblSDAve = 0.0;
		double dblSDMax = 0.0;
		int nSDMPix = 0;
		double sumData=0.0;

		X *pImgTop = ptRaw + i;
		pImgTop += dwStartLine * wElementsPerPixel;
		for( DWORD x=dwStartLine; x<=dwEndLine; x++ )
		{
			double dblSigXn = 0.0;
			double dblSigXn2 = 0.0;
			X *pImg = pImgTop;
			
			for( DWORD y=0; y<dwHeight; y++ )	//縦方向の合計
			{
				dblSigXn += *pImg;
				dblSigXn2 += (*pImg)*(*pImg);
				PBYTE pbImg =  (PBYTE)pImg;
				pImg = (X *)(pbImg + nSrcLinePitch);
			}
		
			double dSD = sqrt( (double)(dwHeight*dblSigXn2-dblSigXn*dblSigXn)/(dwHeight*(dwHeight-1)) );		//縦方向の標準偏差
				
			if( dSD > dblSDMax )		//最大標準偏差
			{
				dblSDMax = dSD;
				nSDMPix = x;
			}
			dblSDAve += dSD;

			//平均用
			sumData += dblSigXn;
			
			pImgTop += wElementsPerPixel;
		}

		double dblAverage = sumData / ( dwHeight * (dwEndLine - dwStartLine + 1) );
		dblSDAve /= (dwEndLine - dwStartLine + 1);
		//代入
		dblOutAverage[i] = dblAverage;
		dblOutSDAve[i] = dblSDAve;
		dblOutSDMax[i] = dblSDMax;
		nOutSDMPix[i] = nSDMPix;
	}
	return TRUE;
}
*/

//-------------------------------------------------------------------------------
//標準偏差および平均取得
//
//-------------------------------------------------------------------------------
//▼1.0.0.1028
//BOOL StCheckBase::GetSNAverage( StBuffer *aBuffer, INT iStart, INT iEnd, double *dblOutAverage, double *dblOutSDAve, double *dblOutSDMax, PINT nOutSDMPix )
BOOL StCheckBase::GetSNAverage( StBuffer *aBuffer, INT iStart, INT iEnd, double *dblOutAverage, double *dblOutSDAve, double *dblOutSDMax, PINT nOutSDMPix
								, double *pdblOutLineAverageMin, double *pdblOutLineAverageMax, PINT pnOutLineAverageMinPos, PINT pnOutLineAverageMaxPos )
//▲1.0.0.1028
{
	BOOL bReval = FALSE;
	
	INT nElement = GetElementFromBuffer(aBuffer);
	INT nPixelSize = GetPixelSize(aBuffer);
	if( nElement==0 )
		return FALSE;
	if( nPixelSize%8 )
		return FALSE;
	if( iStart>iEnd )
		return FALSE;
	if( iEnd>=(INT)aBuffer->GetWidth() )
		return FALSE;

	double *dblAverage = NULL;
	double *dblSDAve = NULL;
	double *dblSDMax = NULL;
	int *nSDMPix = NULL;
	//▼1.0.0.1028
	double	*pdblLineAverageMin = NULL;
	double	*pdblLineAverageMax = NULL;
	int *pnLineAverageMinPos = NULL;
	int *pnLineAverageMaxPos = NULL;

	double **ppdblCalcAve = NULL;
	//▲1.0.0.1028

	do
	{
		dblAverage = new double [nElement];
		if( !dblAverage )
			break;
		dblSDAve = new double [nElement];
		if( !dblSDAve )
			break;
		dblSDMax = new double [nElement];
		if( !dblSDMax )
			break;
		nSDMPix = new int [nElement];
		if( !nSDMPix )
			break;

		//▼1.0.0.1028
		ppdblCalcAve = new double *[nElement];
		if( !ppdblCalcAve ) break;
		memset( ppdblCalcAve, 0, sizeof(double *) * nElement);

		bReval = TRUE;
		size_t bufferSize = iEnd - iStart + 1;
		for( int i=0; i<nElement; i++ )
		{
			ppdblCalcAve[i] = new double [bufferSize];
			if( !ppdblCalcAve[i] )
			{
				bReval = FALSE;
				break;
			}
		}
		if( !bReval ) break;
		//▲1.0.0.1028

		if( (nPixelSize/nElement)==8 )
		{
			bReval = GetSNAverageValue( (unsigned char *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd
			//▼1.0.0.1028
				//, dblAverage, dblSDAve, dblSDMax, nSDMPix, NULL, NULL, 0 );
				, dblAverage, dblSDAve, dblSDMax, nSDMPix, ppdblCalcAve, NULL, bufferSize );
			//▲1.0.0.1028
		}
		else
		if( (nPixelSize/nElement)==16 )
		{
			bReval = GetSNAverageValue( (unsigned short *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd
			//▼1.0.0.1028
			//	, dblAverage, dblSDAve, dblSDMax, nSDMPix, NULL, NULL, 0 );
				, dblAverage, dblSDAve, dblSDMax, nSDMPix, ppdblCalcAve, NULL, bufferSize );
			//▲1.0.0.1028
		}

		if( bReval )
		{
			for( int i=0; i<nElement; i++ )
			{
				if( dblOutAverage )	dblOutAverage[i] = dblAverage[i];
				if( dblOutSDAve )	dblOutSDAve[i] = dblSDAve[i];
				if( dblOutSDMax )	dblOutSDMax[i] = dblSDMax[i];
				if( nOutSDMPix )	nOutSDMPix[i] = nSDMPix[i];

				//▼1.0.0.1028
				double *pdblAve = ppdblCalcAve[i];
				double dblMax=pdblAve[0];
				double dblMin=dblMax;
				INT nMaxPos = iStart;
				INT nMinPos = iStart;
				for( int j=1; j<(int)bufferSize; j++ )
				{
					if( dblMax<pdblAve[j] )
					{
						dblMax = pdblAve[j];
						nMaxPos = j + iStart;
					}
					else
					if( dblMin>pdblAve[j] )
					{
						dblMin = pdblAve[j];
						nMinPos = j + iStart;
					}
				}
				if( pdblOutLineAverageMin )	pdblOutLineAverageMin[i] = dblMin;
				if( pdblOutLineAverageMax )	pdblOutLineAverageMax[i] = dblMax;
				if( pnOutLineAverageMinPos )	pnOutLineAverageMinPos[i] = nMinPos;
				if( pnOutLineAverageMaxPos )	pnOutLineAverageMaxPos[i] = nMaxPos;
				//▲1.0.0.1028
			}
		}
	}while(0);
	if( dblAverage ) delete dblAverage;
	if( dblSDAve ) delete dblSDAve;
	if( dblSDMax ) delete dblSDMax;
	if( nSDMPix ) delete nSDMPix;

	//▼1.0.0.1028
	if( ppdblCalcAve )
	{
		for( int i=0; i<nElement; i++ )
		{
			delete [] ppdblCalcAve[i];
		}
		delete [] ppdblCalcAve;
	}
	//▲1.0.0.1028

	return bReval;
}

//===============================================================================
//piContinuous:最大連続数
//piLine:最大連続数の位置
//piColor:最大連続数の色0:Red 1:Green 2:Blue -1:BW
//===============================================================================
template <class X> BOOL GetSpecialBehaviorValue( X *ptRaw, WORD wElementsPerPixel, DWORD dwWidth, DWORD dwHeight, DWORD nSrcLinePitch, DWORD dwStartLine, DWORD dwEndLine
//▼1.0.0.1044
//												, INT iMinLevel, INT iMaxLevel, INT iInOutMode, INT iColorMode, PINT piContinuous, PINT piLine, PINT piColor )
												, INT iMinLevel, INT iMaxLevel, INT iInOutMode, INT iColorMode, PINT piContinuous, PINT piLine, PINT piColor, double *pdblLineAverage )
//▲1.0.0.1044

{
	if( dwEndLine<dwStartLine ) return FALSE;
	if( dwHeight==0 ) return FALSE;
	if( dwStartLine>=dwWidth ) return FALSE;
	if( dwEndLine>=dwWidth ) return FALSE;

	INT nMaxContinueCount = 0;
	INT nLine = -1;
	INT nColor = -1;
	for( int i=0; i< wElementsPerPixel; i++ )		//0:Blue 1:Greeeen 2:Red   0:BW
	{
		if( ((iColorMode>>i)&1)==0 )
			continue;

		X *pImgTop = ptRaw + i;
		pImgTop += dwStartLine * wElementsPerPixel;
		for( DWORD x=dwStartLine; x<=dwEndLine; x++ )
		{

			INT nContinueCount = 0;
			X *pImgData = pImgTop;

			for( DWORD y=0; y<dwHeight; y++ )	//縦方向の合計
			{
				INT iIO = 0;	//外
				if( *pImgData>=iMinLevel && *pImgData<=iMaxLevel )
				{
					iIO = 1;	//内
				}
				if( iInOutMode!=iIO )
				{
					nContinueCount++;
					if( nMaxContinueCount < nContinueCount )
					{
						nMaxContinueCount=nContinueCount;
						nLine = x;
						if( wElementsPerPixel==3 )
							nColor = i;
					}
				}
				else
				{
					nContinueCount=0;
				}
				pImgData = (X *)((char *)pImgData + nSrcLinePitch);
			}
			pImgTop += wElementsPerPixel;
		}

	}

	*piContinuous = nMaxContinueCount;
	*piLine = nLine;
	*piColor = nColor;
	//▼1.0.0.1044
	if( pdblLineAverage && nLine>=0 )
	{
		double dblLineAverage = 0.0;

		X *pImgTop = ptRaw + (nColor<0?0:nColor);
		pImgTop += nLine * wElementsPerPixel;

		X *pImgData = pImgTop;

		for( DWORD y=0; y<dwHeight; y++ )	//縦方向の合計
		{
			dblLineAverage += *pImgData;
			pImgData = (X *)((char *)pImgData + nSrcLinePitch);
		}
		dblLineAverage /= dwHeight;

		*pdblLineAverage = dblLineAverage;
	}
	//▲1.0.0.1044

	return TRUE;
}


//-------------------------------------------------------------------------------
//特定挙動検査用
//
//1)StartLineとEndLine内で、縦1ラインのそれぞれの画素レベルがMaxLevel-MinLevelの範囲(あるいは範囲外)にあるかどうかを調べる。
//2)規格外の画素が連続している数が一番多いライン位置および色を取得する。
//-------------------------------------------------------------------------------
BOOL StCheckBase::GetSpecialBehavior( StBuffer *aBuffer, INT iStart, INT iEnd, INT iMinLevel, INT iMaxLevel, INT iInOutMode
//▼1.0.0.1044
	                                   //, INT iColorMode, PINT piContinuous, PINT piLine, PINT piColor )
	                                   , INT iColorMode, PINT piContinuous, PINT piLine, PINT piColor, double *pdblLineAverage )
//▲1.0.0.1044
{
	BOOL bReval = FALSE;
	INT nElement = GetElementFromBuffer(aBuffer);
	INT nPixelSize = GetPixelSize(aBuffer);
	if( nElement==0 ) return FALSE;
	if( nPixelSize%8 ) return FALSE;
	if( iStart>iEnd ) return FALSE;
	if( iEnd>=(INT)aBuffer->GetWidth() ) return FALSE;

	INT iContinuous = 0;
	INT iLine = -1;
	INT iColor = -1;
	//▼1.0.0.1044
	double dblLineAverage = -1.0;
	//▲1.0.0.1044
	if( (nPixelSize/nElement)==8 )
	{
		bReval =  GetSpecialBehaviorValue( (unsigned char *)aBuffer->GetDataPointer(), nElement,aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd
	//▼1.0.0.1044
												//, iMinLevel, iMaxLevel, iInOutMode, iColorMode, &iContinuous, &iLine, &iColor );
												, iMinLevel, iMaxLevel, iInOutMode, iColorMode, &iContinuous, &iLine, &iColor, &dblLineAverage );
	//▲1.0.0.1044
	}
	else
	if( (nPixelSize/nElement)==16 )
	{
		bReval =  GetSpecialBehaviorValue( (unsigned short *)aBuffer->GetDataPointer(), nElement,aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd
	//▼1.0.0.1044
												//, iMinLevel, iMaxLevel, iInOutMode, iColorMode, &iContinuous, &iLine, &iColor );
												, iMinLevel, iMaxLevel, iInOutMode, iColorMode, &iContinuous, &iLine, &iColor, &dblLineAverage );
	//▲1.0.0.1044
	}
	*piContinuous = iContinuous;
	*piLine = iLine;
	*piColor = iColor;
	//▼1.0.0.1044
	*pdblLineAverage = dblLineAverage;
	//▲1.0.0.1044

	return bReval;
}



//===============================================================================
//pdblDifference:最大差分
//piHighLow:上か下か
//piLine:最大連続数の位置
//pdblLineAverage：各ラインの平均値
//piLineMin：各ラインの最小画素値
//piLineMax  各ラインの最大画素値
//bufferSize  :バッファサイズ  0の場合無視
//===============================================================================
template <class X> BOOL GetLineAverageDifferenceValue( X *ptRaw, WORD wElementsPerPixel, DWORD dwWidth, DWORD dwHeight, DWORD nSrcLinePitch, INT iStart, INT iEnd, double * pdblDifference, PINT piHighLow, PINT piLine
											, double **pdblLineAverage, INT **piLineMin, INT **piLineMax, size_t bufferSize )
{
	if( iEnd<iStart ) return FALSE;
	if( dwHeight==0 ) return FALSE;
	if( iStart>=(INT)dwWidth ) return FALSE;
	if( iEnd>=(INT)dwWidth ) return FALSE;

	INT nLines = iEnd - iStart + 1;
	if( bufferSize>0 && bufferSize<(DWORD)nLines )  return FALSE;

	double *pdblLineAverageValue = NULL;
	PINT piLineMinValue = NULL;
	PINT piLineMaxValue = NULL;
	do
	{
		pdblLineAverageValue = new double [nLines];
		if( !pdblLineAverageValue ) break;
		piLineMinValue = new INT [nLines];
		if( !piLineMinValue ) break;
		piLineMaxValue = new INT [nLines];
		if( !piLineMaxValue ) break;

		for( int i=0; i< wElementsPerPixel; i++ )		//0:Blue 1:Greeeen 2:Red   0:BW
		{

			double dblMaxDifference = 0.0;
			INT nLinePos = -1;
			INT nHighLow = -1;
			X *pImgTop = ptRaw + i;
			pImgTop += iStart * wElementsPerPixel;
			for( INT x=0; x<nLines; x++ )
			{

				INT nContinueCount = 0;
				X *pImgData = pImgTop;

				double dblSum = 0.0;
				INT nMax = 0;
				INT nMin = 0x7FFFFFFF;
				for( DWORD y=0; y<dwHeight; y++ )	//縦方向の合計
				{
//if( *pImgData==0 )
//TRACE(TEXT("@@@@@@@@@ pImgData[%d:%d]=0 "), x,y );

					nMax = nMax>*pImgData?nMax:*pImgData;
					nMin = nMin<*pImgData?nMin:*pImgData;
					dblSum += (double)*pImgData;
					pImgData = (X *)((char *)pImgData + nSrcLinePitch);
				}
				//代入-------
				pdblLineAverageValue[x] = dblSum / dwHeight;
				piLineMinValue[x] = nMin;
				piLineMaxValue[x] = nMax;

				double dblDifferenceValue = (double)nMax - pdblLineAverageValue[x];
				if( dblDifferenceValue>dblMaxDifference )
				{
					dblMaxDifference = dblDifferenceValue;
					nLinePos = x;
					nHighLow = 1;
				}
				dblDifferenceValue = pdblLineAverageValue[x] - (double)nMin;
				if( dblDifferenceValue>dblMaxDifference )
				{
					dblMaxDifference = dblDifferenceValue;
					nLinePos = x;
					nHighLow = 0;
				}

				pImgTop += wElementsPerPixel;
			}

			pdblDifference[i] = dblMaxDifference;
			piHighLow[i] = nHighLow;
			piLine[i] = nLinePos;


			//代入
//TRACE(TEXT("@@@@@@@@@ in pdblLineAverage=0x%x\n"), pdblLineAverage );
//TRACE(TEXT("@@@@@@@@@ in pdblLineAverage[%d]=0x%x\n"), 0, pdblLineAverage[0] );
			if( pdblLineAverage )
				memcpy( pdblLineAverage[i], pdblLineAverageValue, sizeof(double) * nLines );
//TRACE(TEXT("@@@@@@@@@ out pdblLineAverage=0x%x\n"), pdblLineAverage );
//TRACE(TEXT("@@@@@@@@@ out pdblLineAverage[%d]=0x%x\n"), 0, pdblLineAverage[0] );
			if( piLineMin )
				memcpy( piLineMin[i], piLineMinValue, sizeof(INT) * nLines );
			if( piLineMax )
				memcpy( piLineMax[i], piLineMaxValue, sizeof(INT) * nLines );

		}
	}while(0);

	if( pdblLineAverageValue ) delete [] pdblLineAverageValue;
	if( piLineMinValue ) delete [] piLineMinValue;
	if( piLineMaxValue ) delete [] piLineMaxValue;

	return TRUE;
}

//-------------------------------------------------------------------------------
//デジタルデータ取りこぼし検査用
//
//1)StartLineとEndLine内で、各縦ラインごとに画素値の最大最小平均を取得する。
//2)各ラインの平均と最大、最小の差分を求め、最大差分を規格と比較する。
//-------------------------------------------------------------------------------
BOOL StCheckBase::GetLineAverageDifference( StBuffer *aBuffer, INT iStart, INT iEnd, double * pdblDifference, PINT piHighLow, PINT piLine, size_t *pElementSize
											, double **pdblLineAverage, INT **piLineMin, INT **piLineMax, size_t bufferSize )
{
	BOOL bReval = FALSE;
	INT nElement = GetElementFromBuffer(aBuffer);
	INT nPixelSize = GetPixelSize(aBuffer);
	if( nElement==0 ) return FALSE;
	if( nPixelSize%8 ) return FALSE;
	if( iStart>iEnd ) return FALSE;
	if( iEnd>=(INT)aBuffer->GetWidth() ) return FALSE;
	if( *pElementSize<(DWORD)nElement ) return FALSE;

	if( (nPixelSize/nElement)==8 )
	{
		bReval = GetLineAverageDifferenceValue( (unsigned char *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd, pdblDifference, piHighLow, piLine
												, pdblLineAverage, piLineMin, piLineMax, bufferSize );
	}
	else
	if( (nPixelSize/nElement)==16 )
	{
		bReval = GetLineAverageDifferenceValue( (unsigned short *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd, pdblDifference, piHighLow, piLine
												, pdblLineAverage, piLineMin, piLineMax, bufferSize );
	}
	if( bReval )
		*pElementSize = nElement;

	return bReval;

}


//===============================================================================
//wElementsPerPixel:1画素の要素(BW:1 Color:3)
//dwWidth:横サイズ(全画像のサイズ、セパレート考慮なし)
//dwHeight:縦サイズ
//nSrcLinePitch:1ラインのサイズ
//iStart:スタート位置(各セパレートごと)
//iEnd:エンド位置(各セパレートごと)
//iCheckWidth:検査の横サイズ
//iSeparate:分割数

//pdblDifference:最大差分
//piLine:最大差分の位置
//pdblDifferenceRatio：最大差分比率
//piLineRatio：最大差分比率の位置

//画像の縦方向を平均化する。(１ライン分のデータにする。)
//iCheckWidthの幅で最大-最小が一番大きい位置を求める。
//求めた位置で平均取得し、平均値から一番はなれた画素値、位置をもとめ、％化する。
//===============================================================================
template <class X> BOOL GetWidthLineAverageDifferenceValue( X *ptRaw, WORD wElementsPerPixel, DWORD dwWidth, DWORD dwHeight, DWORD nSrcLinePitch, INT iStart, INT iEnd, INT iCheckWidth, INT iSeparate
															, double * pdblDifference, PINT piLine, double * pdblDifferenceRatio, PINT piLineRatio, double * pdblAverage )
{

	if( iEnd<iStart ) return FALSE;
	if( dwHeight==0 ) return FALSE;
	INT nSeparateWidth = dwWidth / iSeparate;
	if( iEnd >= nSeparateWidth ) return FALSE;
	INT nLines = iEnd - iStart + 1;

	double *pdblLineAverageValue = NULL;
	do
	{
		pdblLineAverageValue = new double [nLines * iSeparate];
		if( !pdblLineAverageValue ) break;
		for( int i=0; i< wElementsPerPixel; i++ )		//0:Blue 1:Greeeen 2:Red   0:BW
		{

			//▼1.0.0.1015
			//double dblMaxDiff = 0.0;
			double dblMaxDiff = -1.0;
			//▲1.0.0.1015
			INT nMaxElement = -1;
			INT nMaxSeparate = -1;
			INT nMaxLine = -1;

			double dblAllAverage = 0.0;

			for( int j=0; j< iSeparate; j++ )
			{
				INT nStart = ( nSeparateWidth * j + iStart ) * wElementsPerPixel;
				X *pImgTop = ptRaw + i;
				pImgTop += nStart;
				for( INT x=0; x<nLines; x++ )
				{

					INT nContinueCount = 0;
					X *pImgData = pImgTop;

					double dblSum = 0.0;
					for( DWORD y=0; y<dwHeight; y++ )	//縦方向の合計
					{
						dblSum += (double)*pImgData;
						pImgData = (X *)((char *)pImgData + nSrcLinePitch);
					}
					//代入-------
					pdblLineAverageValue[x + j * nLines] = dblSum / dwHeight;
				
					dblAllAverage += dblSum;

					pImgTop += wElementsPerPixel;
				}

				for( INT x=0; x<nLines - iCheckWidth; x++ )
				{
					double dblMin = 9999.0;
					double dblMax = 0.0;
					for( INT wd=0; wd<iCheckWidth; wd++ )
					{
						dblMin = min(dblMin,pdblLineAverageValue[x + wd + j * nLines]);
						dblMax = max(dblMax,pdblLineAverageValue[x + wd + j * nLines]);
					}
					double dblDiff = dblMax - dblMin;
					if( dblMaxDiff < dblDiff )
					{
						dblMaxDiff = dblDiff;
						nMaxElement = i;
						nMaxSeparate = j;
						nMaxLine = x;
					}
				}
			}

			//画像の平均、ゴミ検査用----------
			if( pdblAverage )
				//▼1.0.0.1043
				//*pdblAverage = dblAllAverage / (iSeparate * nLines * dwHeight);
				pdblAverage[i] = dblAllAverage / (iSeparate * nLines * dwHeight);
				//▲1.0.0.1043

			if( pdblDifference )
			{
				pdblDifference[i] = dblMaxDiff;
			}
			if( piLine )
			{
				piLine[i] = nMaxSeparate * nSeparateWidth + nMaxLine + iStart;
			}

			//-----求めた位置から再取得------------
			double dblAverage = 0.0;
			for( INT wd=0; wd<iCheckWidth; wd++ )
			{
				dblAverage += pdblLineAverageValue[nMaxLine + wd + nMaxSeparate * nLines];
			}
			dblAverage /= iCheckWidth;
			//▼1.0.0.1015
			//double dblMaxDiffRatio = 0.0;
			double dblMaxDiffRatio = -1.0;
			//▲1.0.0.1015
			INT nMaxPos = -1;
			for( INT wd=0; wd<iCheckWidth; wd++ )
			{
				double dblDiff = fabs( dblAverage - pdblLineAverageValue[nMaxLine + wd + nMaxSeparate * nLines] );
				if( dblMaxDiffRatio < dblDiff )
				{
					dblMaxDiffRatio = dblDiff;
					nMaxPos = nMaxSeparate * nSeparateWidth + wd + nMaxLine + iStart;
				}
			}

			if( dblAverage>0.0 )
			{
				if( pdblDifferenceRatio )
					pdblDifferenceRatio[i] = dblMaxDiffRatio / dblAverage * 100;
			}
			if( piLineRatio )
				piLineRatio[i] = nMaxPos;

		}
	}while(0);

	if( pdblLineAverageValue )
		delete [] pdblLineAverageValue;

	return TRUE;



}

//▼1.0.0.1041
//===============================================================================
//wElementsPerPixel:1画素の要素(BW:1 Color:3)
//dwWidth:横サイズ(全画像のサイズ、セパレート考慮なし)
//dwHeight:縦サイズ
//nSrcLinePitch:1ラインのサイズ
//iStart:スタート位置(各セパレートごと)
//iEnd:エンド位置(各セパレートごと)
//iCheckWidth:検査の横サイズ
//iSeparate:分割数

//pdblDifference:最大差分
//piLine:最大差分の位置
//pdblDifferenceRatio：最大差分比率
//piLineRatio：最大差分比率の位置

//画像の縦方向を平均化する。(１ライン分のデータにする。)
//iCheckWidthの幅で最大-平均,平均-最小が一番大きい位置を求める。
//求めた位置で平均取得し、平均値から一番はなれた画素値、位置をもとめ、％化する。
//===============================================================================
template <class X> BOOL GetWidthLineAverageDifferenceValue2( X *ptRaw, WORD wElementsPerPixel, DWORD dwWidth, DWORD dwHeight, DWORD nSrcLinePitch, INT iStart, INT iEnd, INT iCheckWidth, INT iSeparate
															, double * pdblDifference, PINT piLine, double * pdblDifferenceRatio, PINT piLineRatio, double * pdblAverage )
{

	if( iEnd<iStart ) return FALSE;
	if( dwHeight==0 ) return FALSE;
	INT nSeparateWidth = dwWidth / iSeparate;
	if( iEnd >= nSeparateWidth ) return FALSE;
	INT nLines = iEnd - iStart + 1;

	double *pdblLineAverageValue = NULL;
	do
	{
		pdblLineAverageValue = new double [nLines * iSeparate];
		if( !pdblLineAverageValue ) break;
		for( int i=0; i< wElementsPerPixel; i++ )		//0:Blue 1:Greeeen 2:Red   0:BW
		{

			double dblMaxDiff = -1.0;
			//INT nMaxElement = -1;
			INT nMaxSeparate = -1;
			INT nMaxLine = -1;

			double dblMaxDiffRatio = -1.0;
			//INT nMaxElement = -1;
			INT nMaxSeparateRatio = -1;
			INT nMaxLineRatio = -1;


			double dblAllAverage = 0.0;
			for( int j=0; j< iSeparate; j++ )
			{
				INT nStart = ( nSeparateWidth * j + iStart ) * wElementsPerPixel;
				X *pImgTop = ptRaw + i;
				pImgTop += nStart;
				for( INT x=0; x<nLines; x++ )
				{

					INT nContinueCount = 0;
					X *pImgData = pImgTop;

					double dblSum = 0.0;
					for( DWORD y=0; y<dwHeight; y++ )	//縦方向の合計
					{
						dblSum += (double)*pImgData;
						pImgData = (X *)((char *)pImgData + nSrcLinePitch);
					}
					//代入-------
					pdblLineAverageValue[x + j * nSeparateWidth] = dblSum / dwHeight;
				
					dblAllAverage += dblSum;

					pImgTop += wElementsPerPixel;
				}

				
				INT nMaxDifferencePos = -1;
				for( INT x=0; x<nLines - iCheckWidth; x++ )
				{
					double dblMin = 9999.0;
					double dblMax = 0.0;
					double dblAreaAverage = 0.0;
					INT nPoxMin = -1;
					INT nPoxMax = -1;
					//平均取得
					for( INT wd=0; wd<iCheckWidth; wd++ )
					{
						if( dblMin>pdblLineAverageValue[x + wd + j * nSeparateWidth] )
						{
							dblMin = pdblLineAverageValue[x + wd + j * nSeparateWidth];
							nPoxMin = x + wd + j * nSeparateWidth;
						}
						if( dblMax<pdblLineAverageValue[x + wd + j * nSeparateWidth] )
						{
							dblMax = pdblLineAverageValue[x + wd + j * nSeparateWidth];
							nPoxMax = x + wd + j * nSeparateWidth;
						}
						dblAreaAverage += pdblLineAverageValue[x + wd + j * nSeparateWidth];
					}
					dblAreaAverage /= iCheckWidth;
					if( dblMaxDiff<dblAreaAverage-dblMin )
					{
						dblMaxDiff = dblAreaAverage-dblMin;
						//nMaxElement = i;
						nMaxSeparate = j;
						nMaxLine = nPoxMin;

					}
					if( dblMaxDiff<dblMax-dblAreaAverage )
					{
						dblMaxDiff = dblMax-dblAreaAverage;
						//nMaxElement = i;
						nMaxSeparate = j;
						nMaxLine = nPoxMax;
					}

					if( dblAreaAverage>0.0 )
					{
						if( dblMaxDiffRatio<(dblAreaAverage-dblMin)/dblAreaAverage )
						{
							dblMaxDiffRatio = (dblAreaAverage-dblMin)/dblAreaAverage;
							//nMaxElement = i;
							nMaxSeparateRatio = j;
							nMaxLineRatio = nPoxMin;

						}
						if( dblMaxDiffRatio<(dblMax-dblAreaAverage)/dblAreaAverage )
						{
							dblMaxDiffRatio = (dblMax-dblAreaAverage)/dblAreaAverage;
							//nMaxElement = i;
							nMaxSeparateRatio = j;
							nMaxLineRatio = nPoxMax;
						}
					}
				}
			}

			//画像の平均、ゴミ検査用----------
			if( pdblAverage )
				*pdblAverage = dblAllAverage / (iSeparate * nLines * dwHeight);

			if( pdblDifference )
			{
				pdblDifference[i] = dblMaxDiff;
			}
			if( piLine )
			{
				piLine[i] = nMaxLine;
			}

			if( pdblDifferenceRatio )
				//▼1.0.0.1042
				//pdblDifferenceRatio[i] = dblMaxDiffRatio;
				pdblDifferenceRatio[i] = dblMaxDiffRatio * 100;	//%変換
				//▲1.0.0.1042
			if( piLineRatio )
				piLineRatio[i] = nMaxLineRatio;

		}
	}while(0);

	if( pdblLineAverageValue )
		delete [] pdblLineAverageValue;

	return TRUE;

}
//▲1.0.0.1041




//▼1.0.0.1043
//===============================================================================
//wElementsPerPixel:1画素の要素(BW:1 Color:3)
//dwWidth:横サイズ(全画像のサイズ、セパレート考慮なし)
//dwHeight:縦サイズ
//nSrcLinePitch:1ラインのサイズ
//iStart:スタート位置(各セパレートごと)
//iEnd:エンド位置(各セパレートごと)
//iCheckWidth:検査の横サイズ
//iSeparate:分割数

//pdblDifference:最大差分
//piLine:最大差分の位置
//pdblDifferenceRatio：最大差分比率
//piLineRatio：最大差分比率の位置

//画像の縦方向を平均化する。(１ライン分のデータにする。)
//iCheckWidthの幅で最大-平均,平均-最小が一番大きい位置を求める。
//求めた位置で平均取得し、平均値から一番はなれた画素値、位置をもとめ、％化する。
//===============================================================================
template <class X> BOOL GetWidthLineAverageDifferenceLineValue( X *ptRaw, WORD wElementsPerPixel, DWORD dwWidth, DWORD dwHeight, DWORD nSrcLinePitch, INT iStart, INT iEnd, INT iCheckWidth, INT iSeparate
															, double dblLimitDiffRatio, double * pdblDifferenceRatio, PINT piLineRatio, double * pdblAverage, PINT piLineContinuousCount, PINT piLineContinuousStart, PINT piLineContinuousEnd )
{

	if( iEnd<iStart ) return FALSE;
	if( dwHeight==0 ) return FALSE;
	INT nSeparateWidth = dwWidth / iSeparate;
	if( iEnd >= nSeparateWidth ) return FALSE;
	INT nLines = iEnd - iStart + 1;

	double *pdblLineAverageValue = NULL;
	do
	{
		pdblLineAverageValue = new double [nLines * iSeparate];
		if( !pdblLineAverageValue ) break;
		for( int i=0; i< wElementsPerPixel; i++ )		//0:Blue 1:Greeeen 2:Red   0:BW
		{

			double dblMaxDiff = -1.0;
			INT nMaxSeparate = -1;
			INT nMaxLine = -1;

			double dblMaxDiffRatio = -1.0;
			INT nMaxSeparateRatio = -1;
			INT nMaxLineRatio = -1;
			INT nMaxLineCount = 0;

			double dblTemporaryMaxDiffRatio = -1.0;
			INT nTemporaryMaxLineRatio = -1;

			INT nMaxLineStart = -1;
			INT nMaxLineEnd = -1;
			INT nLineStart = -1;
			INT nLineEnd = -1;

			double dblAllAverage = 0.0;
			for( int j=0; j< iSeparate; j++ )
			{
				INT nStart = ( nSeparateWidth * j + iStart ) * wElementsPerPixel;
				X *pImgTop = ptRaw + i;
				pImgTop += nStart;
				for( INT x=0; x<nLines; x++ )
				{

					INT nContinueCount = 0;
					X *pImgData = pImgTop;

					double dblSum = 0.0;
					for( DWORD y=0; y<dwHeight; y++ )	//縦方向の合計
					{
						dblSum += (double)*pImgData;
						pImgData = (X *)((char *)pImgData + nSrcLinePitch);
					}
					//代入-------
					pdblLineAverageValue[x + j * nSeparateWidth] = dblSum / dwHeight;
				
					dblAllAverage += dblSum;

					pImgTop += wElementsPerPixel;
				}

				
				INT nMaxDifferencePos = -1;

				INT nOverLimitCountMax=0;

				for( INT x=0; x<nLines - iCheckWidth; x++ )
				{
					//double dblMin = 9999.0;
					//double dblMax = 0.0;
					double dblAreaAverage = 0.0;
					INT nPoxMin = -1;
					INT nPoxMax = -1;
					//平均取得
					for( INT wd=0; wd<iCheckWidth; wd++ )
					{
						//if( dblMin>pdblLineAverageValue[x + wd + j * nSeparateWidth] )
						//{
						//	dblMin = pdblLineAverageValue[x + wd + j * nSeparateWidth];
						//	nPoxMin = x + wd + j * nSeparateWidth;
						//}
						//if( dblMax<pdblLineAverageValue[x + wd + j * nSeparateWidth] )
						//{
						//	dblMax = pdblLineAverageValue[x + wd + j * nSeparateWidth];
						//	nPoxMax = x + wd + j * nSeparateWidth;
						//}
						dblAreaAverage += pdblLineAverageValue[x + wd + j * nSeparateWidth];
					}
					dblAreaAverage /= iCheckWidth;

					//-------------------
					//dblPerLimit
					INT nContinuousOverCount=0;
					INT nContinuousOverCountMax=0;
//if( x==16015 || x==nLines - iCheckWidth - 1 )
//	Sleep(1);

					double dblMaxPer = 0.0;
					INT nPerPosMax = -1;

					INT nTemporaryLineStart = -1;
					INT nTemporaryLineEnd = -1;

					//double dblMinPer = 0;
					for( INT wd=0; wd<iCheckWidth && dblAreaAverage>0.0; wd++ )
					{

//if( x==nLines - iCheckWidth - 1 && x + wd + j * nSeparateWidth==16015 )
//	Sleep(1);

						double dblPer = fabs(dblAreaAverage-pdblLineAverageValue[x + wd + j * nSeparateWidth])/dblAreaAverage;
						dblPer *= 100;	//%変換
						if( dblLimitDiffRatio<dblPer )
						{
							if( nContinuousOverCount==0 )
								nTemporaryLineStart = x + wd + j * nSeparateWidth;
							nTemporaryLineEnd = x + wd + j * nSeparateWidth;
							nContinuousOverCount++;
							if( dblMaxPer<dblPer )
							{
								dblMaxPer = dblPer;
								nPoxMax = x + wd + j * nSeparateWidth;
							}
						}
						else
						{
							if( nContinuousOverCountMax<nContinuousOverCount )
							{
								nContinuousOverCountMax=nContinuousOverCount;
								dblMaxDiff = dblMaxPer;
								nPerPosMax = nPoxMax;

								nLineStart = nTemporaryLineStart;
								nLineEnd = nTemporaryLineEnd;

							}
							nContinuousOverCount = 0;
							dblMaxPer = 0.0;
							nPoxMax = -1;
						}

						if( dblTemporaryMaxDiffRatio<dblPer )
						{
							dblTemporaryMaxDiffRatio = dblPer;
							nTemporaryMaxLineRatio = x + wd + j * nSeparateWidth;
						}

					}

					BOOL bUpdate = FALSE;
					if( nMaxLineCount<nContinuousOverCountMax )
					{
						bUpdate = TRUE;
					}
					else
					if( nMaxLineCount==nContinuousOverCountMax )
					{
						if( dblMaxDiffRatio < dblMaxDiff )
						{
							bUpdate = TRUE;
						}
					}
					if( bUpdate )
					{
						dblMaxDiffRatio = dblMaxDiff;
						nMaxSeparateRatio = j;
						nMaxLineRatio = nPerPosMax;
						nMaxLineCount = nContinuousOverCountMax;
						nMaxLineStart = nLineStart;
						nMaxLineEnd = nLineEnd;
					}
					//-------------------



					//if( dblMaxDiff<dblAreaAverage-dblMin )
					//{
					//	dblMaxDiff = dblAreaAverage-dblMin;
					//	//nMaxElement = i;
					//	nMaxSeparate = j;
					//	nMaxLine = nPoxMin;

					//}
					//if( dblMaxDiff<dblMax-dblAreaAverage )
					//{
					//	dblMaxDiff = dblMax-dblAreaAverage;
					//	//nMaxElement = i;
					//	nMaxSeparate = j;
					//	nMaxLine = nPoxMax;
					//}

					//if( dblAreaAverage>0.0 )
					//{
					//	if( dblMaxDiffRatio<(dblAreaAverage-dblMin)/dblAreaAverage )
					//	{
					//		dblMaxDiffRatio = (dblAreaAverage-dblMin)/dblAreaAverage;
					//		//nMaxElement = i;
					//		nMaxSeparateRatio = j;
					//		nMaxLineRatio = nPoxMin;

					//	}
					//	if( dblMaxDiffRatio<(dblMax-dblAreaAverage)/dblAreaAverage )
					//	{
					//		dblMaxDiffRatio = (dblMax-dblAreaAverage)/dblAreaAverage;
					//		//nMaxElement = i;
					//		nMaxSeparateRatio = j;
					//		nMaxLineRatio = nPoxMax;
					//	}
					//}
				}
			}	//Separete LoopEnd

			//画像の平均、ゴミ検査用----------
			if( pdblAverage )
				pdblAverage[i] = dblAllAverage / (iSeparate * nLines * dwHeight);

			//if( pdblDifference )
			//{
			//	pdblDifference[i] = dblMaxDiff;
			//}
			//if( piLine )
			//{
			//	piLine[i] = nMaxLine;
			//}

			if( dblMaxDiffRatio<0.0 )
			{
				dblMaxDiffRatio = dblTemporaryMaxDiffRatio;
				nMaxLineRatio = nTemporaryMaxLineRatio;
			}


			if( pdblDifferenceRatio )
				pdblDifferenceRatio[i] = dblMaxDiffRatio;
			if( piLineRatio )
				piLineRatio[i] = nMaxLineRatio;
			if( piLineContinuousCount )
				piLineContinuousCount[i] = nMaxLineCount;

			if( piLineContinuousStart )
				piLineContinuousStart[i] = nMaxLineStart;
			if( piLineContinuousEnd )
				piLineContinuousEnd[i] = nMaxLineEnd;

		}
	}while(0);

	if( pdblLineAverageValue )
		delete [] pdblLineAverageValue;

	return TRUE;

}
//▲1.0.0.1043

//-------------------------------------------------------------------------------
//画素欠陥検査用
//ゴミ検査用
//
//1)StartLineとEndLine内で、各縦ラインごとに画素値の平均を取得する。
//2)検査開始ラインから検査幅分、最大最小の平均値を取得し、差分を求める。
//3)求めた差分が一番大きいライン値、および差分値を取得する。
//4)3で求めたライン値からSetWidthLines分、平均取得し、SetWidthLines分の各ライン平均との差分を取得する。
//5)4で求めた最大差分を規格と比較し、最大差分および位置を取得する。
//6)4で求めた最大差分の位置からSetWidthLines分、平均を求め、各ラインの平均との差分を求め比率および位置を取得する
//-------------------------------------------------------------------------------
//▼1.0.0.1041
//BOOL StCheckBase::GetWidthLineAverageDifference( StBuffer *aBuffer, INT iStart, INT iEnd, INT iWidth, INT iSeparate, size_t *pElementSize
BOOL StCheckBase::GetWidthLineAverageDifference( StBuffer *aBuffer, INT iStart, INT iEnd, INT iWidth, INT iSeparate, INT nMode, size_t *pElementSize
//▲1.0.0.1041
//▼1.0.0.1043
											//, double * pdblDifference, PINT piLine, double * pdblDifferenceRatio, PINT piLineRatio, double *pdblAverage )
											, double * pdblDifference, PINT piLine, double * pdblDifferenceRatio, PINT piLineRatio, double *pdblAverage, double dblLimitDiffRatio, PINT piLineContinuousCount, PINT piLineContinuousStart, PINT piLineContinuousEnd )
//▲1.0.0.1043
{

	BOOL bReval = FALSE;
	INT nElement = GetElementFromBuffer(aBuffer);
	INT nPixelSize = GetPixelSize(aBuffer);
	if( nElement==0 ) return FALSE;
	if( nPixelSize%8 ) return FALSE;
	if( iStart>iEnd ) return FALSE;
	if( iEnd>=(INT)aBuffer->GetWidth() ) return FALSE;
	if( *pElementSize<(DWORD)nElement ) return FALSE;

	//▼1.0.0.1041
	//if( (nPixelSize/nElement)==8 )
	//{
	//	bReval = GetWidthLineAverageDifferenceValue( (unsigned char *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd, iWidth, iSeparate
	//											, pdblDifference, piLine, pdblDifferenceRatio, piLineRatio, pdblAverage );
	//}
	//else
	//if( (nPixelSize/nElement)==16 )
	//{
	//	bReval = GetWidthLineAverageDifferenceValue( (unsigned short *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd, iWidth, iSeparate
	//											, pdblDifference, piLine, pdblDifferenceRatio, piLineRatio, pdblAverage );
	//}
	if( nMode== 0 )
	{
		if( (nPixelSize/nElement)==8 )
		{
			bReval = GetWidthLineAverageDifferenceValue( (unsigned char *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd, iWidth, iSeparate
													, pdblDifference, piLine, pdblDifferenceRatio, piLineRatio, pdblAverage );
		}
		else
		if( (nPixelSize/nElement)==16 )
		{
			bReval = GetWidthLineAverageDifferenceValue( (unsigned short *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd, iWidth, iSeparate
													, pdblDifference, piLine, pdblDifferenceRatio, piLineRatio, pdblAverage );
		}
	}
	else
	//▼1.0.0.1043
	if( nMode==1 )
	//▲1.0.0.1043
	{
		if( (nPixelSize/nElement)==8 )
		{
			bReval = GetWidthLineAverageDifferenceValue2( (unsigned char *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd, iWidth, iSeparate
													, pdblDifference, piLine, pdblDifferenceRatio, piLineRatio, pdblAverage );
		}
		else
		if( (nPixelSize/nElement)==16 )
		{
			bReval = GetWidthLineAverageDifferenceValue2( (unsigned short *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd, iWidth, iSeparate
													, pdblDifference, piLine, pdblDifferenceRatio, piLineRatio, pdblAverage );
		}
	}
	//▲1.0.0.1041
	//▼1.0.0.1043
	else
	if( nMode==2 )
	{
		if( (nPixelSize/nElement)==8 )
		{

			bReval = GetWidthLineAverageDifferenceLineValue( (unsigned char *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd, iWidth, iSeparate
													, dblLimitDiffRatio, pdblDifferenceRatio, piLineRatio, pdblAverage, piLineContinuousCount, piLineContinuousStart, piLineContinuousEnd );
		}
		else
		if( (nPixelSize/nElement)==16 )
		{
			bReval = GetWidthLineAverageDifferenceLineValue( (unsigned short *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd, iWidth, iSeparate
													, dblLimitDiffRatio, pdblDifferenceRatio, piLineRatio, pdblAverage, piLineContinuousCount, piLineContinuousStart, piLineContinuousEnd );
		}
	}
	//▲1.0.0.1043


	if( bReval )
		*pElementSize = nElement;

	return bReval;

}


//===============================================================================
//pdblDifference:最大差分
//piLine:最大差分の位置
//pdblDifferenceRatio：最大差分比率
//piLineRatio：最大差分比率の位置
//===============================================================================
template <class X> BOOL GetMinimumLineAverageValue( X *ptRaw, WORD wElementsPerPixel, DWORD dwWidth, DWORD dwHeight, DWORD nSrcLinePitch, INT iStart, INT iEnd, double * pdblMinAverage, PINT piLine, PINT piElement )
{

	if( iEnd<iStart ) return FALSE;
	if( dwHeight==0 ) return FALSE;
	if( iEnd >= (INT)dwWidth ) return FALSE;
	INT nLines = iEnd - iStart + 1;

	do
	{
		double dblMin = 9999.0;
		INT nMinPos = -1;
		INT nMinElement = -1;
		for( int i=0; i< wElementsPerPixel; i++ )		//0:Blue 1:Greeeen 2:Red   0:BW
		{

			//double dblMaxDiff = 0.0;
			//INT nMaxElement = -1;
			//INT nMaxSeparate = -1;
			//INT nMaxLine = -1;


			INT nStart = iStart * wElementsPerPixel;
			X *pImgTop = ptRaw + i;
			pImgTop += nStart;
			for( INT x=0; x<nLines; x++ )
			{

				INT nContinueCount = 0;
				X *pImgData = pImgTop;

				double dblSum = 0.0;
				for( DWORD y=0; y<dwHeight; y++ )	//縦方向の合計
				{
					dblSum += (double)*pImgData;
					pImgData = (X *)((char *)pImgData + nSrcLinePitch);
				}
				//代入-------
				double dblLineAverageValue = dblSum / dwHeight;
				
				if( dblMin>dblLineAverageValue )
				{
					dblMin = dblLineAverageValue;
					nMinPos = x;
					nMinElement = i;
				}

				pImgTop += wElementsPerPixel;
			}
		}

		if( pdblMinAverage ) *pdblMinAverage = dblMin;
		if( piLine ) *piLine = nMinPos;
		if( piElement ) *piElement = nMinElement;

	}while(0);


	return TRUE;



}


//-------------------------------------------------------------------------------
//Linetoline用
//各縦ラインの平均を求め、最小値が規格内かどうか判定する。
//
//1)StartLineとEndLine内で、各縦ラインごとに画素値の平均を取得する。
//2)各ラインの平均値の最小値および位置を取得する。
//-------------------------------------------------------------------------------
BOOL StCheckBase::GetMinimumLineAverage( StBuffer *aBuffer, INT iStart, INT iEnd, size_t *pElementSize, double * pdblMinAverage, PINT piLine, PINT piElement )
{

	BOOL bReval = FALSE;
	INT nElement = GetElementFromBuffer(aBuffer);
	INT nPixelSize = GetPixelSize(aBuffer);
	if( nElement==0 ) return FALSE;
	if( nPixelSize%8 ) return FALSE;
	if( iStart>iEnd ) return FALSE;
	if( iEnd>=(INT)aBuffer->GetWidth() ) return FALSE;
	if( *pElementSize<(DWORD)nElement ) return FALSE;

	if( (nPixelSize/nElement)==8 )
	{
		bReval = GetMinimumLineAverageValue( (unsigned char *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd, pdblMinAverage, piLine, piElement );
	}
	else
	if( (nPixelSize/nElement)==16 )
	{
		bReval = GetMinimumLineAverageValue( (unsigned short *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd, pdblMinAverage, piLine, piElement );
	}
	if( bReval )
		*pElementSize = nElement;

	return bReval;

}

//===============================================================================
//pdblDifference:最大差分
//piLine:最大差分の位置
//pdblDifferenceRatio：最大差分比率
//piLineRatio：最大差分比率の位置
//===============================================================================
template <class X> BOOL GetEffectivePixelValue( X *ptRaw, WORD wElementsPerPixel, DWORD dwWidth, DWORD dwHeight, DWORD nSrcLinePitch, INT iStart, INT iEnd, INT iLevel, PINT piEffectiveStart, PINT piEffectiveEnd )
{

	if( iEnd<iStart ) return FALSE;
	if( dwHeight==0 ) return FALSE;
	if( iEnd >= (INT)dwWidth ) return FALSE;
	INT nLines = iEnd - iStart + 1;

	do
	{
		double dblMin = 9999.0;
		INT nMinPos = -1;
		INT nMinElement = -1;
		for( int i=0; i< wElementsPerPixel; i++ )		//0:Blue 1:Greeeen 2:Red   0:BW
		{
			INT nHighLowFlag = 0;
			INT nEffectiveStart = -1;
			INT nEffectiveEnd = -1;

			INT nStart = iStart * wElementsPerPixel;
			X *pImgTop = ptRaw + i;
			pImgTop += nStart;
			for( INT x=0; x<nLines; x++ )
			{

				INT nContinueCount = 0;
				X *pImgData = pImgTop;

				double dblSum = 0.0;
				for( DWORD y=0; y<dwHeight; y++ )	//縦方向の合計
				{
					dblSum += (double)*pImgData;
					pImgData = (X *)((char *)pImgData + nSrcLinePitch);
				}
				//代入-------
				double dblLineAverageValue = dblSum / dwHeight;
				
				if( nHighLowFlag == 0 )	//レベルより上の時
				{
					if( dblLineAverageValue>=(double)iLevel )
					{
						nEffectiveStart = x + nStart;
						nHighLowFlag = 1;
					}
				}
				else				 	//レベルより下の時
				{
					if( dblLineAverageValue<(double)iLevel )
					{
						nEffectiveEnd = x - 1 + nStart;
					}
				}
				pImgTop += wElementsPerPixel;
			}

			if( nHighLowFlag==1 && nEffectiveEnd<0 )
				nEffectiveEnd = iEnd;

			if( piEffectiveStart )	piEffectiveStart[i] = nEffectiveStart;
			if( piEffectiveEnd )	piEffectiveEnd[i] = nEffectiveEnd;
		}

	}while(0);


	return TRUE;



}

//-------------------------------------------------------------------------------
//有効画素検査用
//有効画素位置を求め、規格と同じかどうか判定する。
//
//1)設定範囲内で縦ラインの平均を取得する。
//2)取得した平均が設定レベル以上の最初、最後のライン位置を取得する。
//3)取得した最初、最後のライン位置が規格と同じ場合OKとする。違う場合NGとする。
//-------------------------------------------------------------------------------
BOOL StCheckBase::GetEffectivePixel( StBuffer *aBuffer, INT iStart, INT iEnd, INT iLevel, size_t *pElementSize, PINT piEffectiveStart, PINT piEffectiveEnd )
{

	BOOL bReval = FALSE;
	INT nElement = GetElementFromBuffer(aBuffer);
	INT nPixelSize = GetPixelSize(aBuffer);
	if( nElement==0 ) return FALSE;
	if( nPixelSize%8 ) return FALSE;
	if( iStart>iEnd ) return FALSE;
	if( iEnd>=(INT)aBuffer->GetWidth() ) return FALSE;
	if( *pElementSize<(DWORD)nElement ) return FALSE;

	if( (nPixelSize/nElement)==8 )
	{
		bReval = GetEffectivePixelValue( (unsigned char *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd, iLevel, piEffectiveStart, piEffectiveEnd );
	}
	else
	if( (nPixelSize/nElement)==16 )
	{
		bReval = GetEffectivePixelValue( (unsigned short *)aBuffer->GetDataPointer(), nElement, aBuffer->GetWidth(), aBuffer->GetHeight(), aBuffer->GetLinePitch(), iStart, iEnd, iLevel, piEffectiveStart, piEffectiveEnd );
	}
	if( bReval )
		*pElementSize = nElement;

	return bReval;

}

//▼1.0.0.1040
//-------------------------------------------------------------------------------
//CSVゴミ検査用
//
//1)検査開始ラインから検査幅分、最大最小の値を取得し、差分を求める。
//2)求めた差分が一番大きいライン値、および差分値を取得する。
//3)2で求めたライン値からiWidth分、平均取得し、iWidth分の各ライン平均との差分を取得する。
//4)3で求めた最大差分を規格と比較し、最大差分および位置を取得する。
//5)3で求めた最大差分の位置からiWidth分、平均を求め、各ラインの平均との差分を求め比率および位置を取得する
//-------------------------------------------------------------------------------
//▼1.0.0.1041
//BOOL StCheckBase::GetWidthLineAverageDifference( double *pdblBuffer, INT iSize, INT iStart, INT iEnd, INT iWidth, INT iSeparate, size_t *pElementSize
BOOL StCheckBase::GetWidthLineAverageDifference( double *pdblBuffer, INT iSize, INT iStart, INT iEnd, INT iWidth, INT iSeparate, INT nMode, size_t *pElementSize
//▲1.0.0.1041
//▼1.0.0.1043
											//, double * pdblDifference, PINT piLine, double * pdblDifferenceRatio, PINT piLineRatio, double *pdblAverage )
											, double * pdblDifference, PINT piLine, double * pdblDifferenceRatio, PINT piLineRatio, double *pdblAverage, double dblLimitDiffRatio, PINT piLineContinuousCount, PINT piLineContinuousStart, PINT piLineContinuousEnd )
//▲1.0.0.1043
{
	BOOL bReval = FALSE;
	if( iStart>iEnd ) return FALSE;

	//▼1.0.0.1041
	//bReval = GetWidthLineAverageDifferenceValue( pdblBuffer, 1, iSize, 1, iSize, iStart, iEnd, iWidth, iSeparate
	//										, pdblDifference, piLine, pdblDifferenceRatio, piLineRatio, pdblAverage );
	if( nMode==0 )
	{
		bReval = GetWidthLineAverageDifferenceValue( pdblBuffer, 1, iSize, 1, iSize, iStart, iEnd, iWidth, iSeparate
												, pdblDifference, piLine, pdblDifferenceRatio, piLineRatio, pdblAverage );
	}
	else
	//▼1.0.0.1043
	if( nMode==1 )
	//▲1.0.0.1043
	{
		bReval = GetWidthLineAverageDifferenceValue2( pdblBuffer, 1, iSize, 1, iSize, iStart, iEnd, iWidth, iSeparate
												, pdblDifference, piLine, pdblDifferenceRatio, piLineRatio, pdblAverage );
	}
	//▲1.0.0.1041
	//▼1.0.0.1043
	else
	if( nMode==2 )
	{
		bReval = GetWidthLineAverageDifferenceLineValue( pdblBuffer, 1, iSize, 1, iSize, iStart, iEnd, iWidth, iSeparate
												, dblLimitDiffRatio, pdblDifferenceRatio, piLineRatio, pdblAverage, piLineContinuousCount, piLineContinuousStart, piLineContinuousEnd );

	}
	//▲1.0.0.1043

	return bReval;

}
//▲1.0.0.1040

//▼1.0.0.1044

template <class X> BOOL GetMinLevelValue( X *ptRaw, WORD wElementsPerPixel, DWORD dwWidth, DWORD dwHeight, DWORD nSrcLinePitch, DWORD dwStartLine, DWORD dwEndLine
//▼1.0.0.1067b
////▼1.0.0.1058
//			//, INT *pnMin )
//			, INT *pnMin, INT *pnMinLine )
////▲1.0.0.1058
			, INT *pnMin, INT *pnMinLine, INT nBunch )	//nBunch: Even Oddの束数
//▲1.0.0.1067b
{
	//▼1.0.0.1067b
	if( nBunch<=0 ) nBunch=1;
	INT nPos;
	//▲1.0.0.1067b
	X *pImg, *pImgData;
	for( int k=0; k<wElementsPerPixel; k++)							// 0:BW
	{
		pImg = ptRaw + k;
		for( int i=0; i<dwWidth; i++ )
		{
			if(dwStartLine<=i && i<=dwEndLine)
			{
				pImgData = pImg;
				for(int j=0; j<dwHeight; j++)
				{
					//▼1.0.0.1067b
					//if( pnMin[i&1]>(*pImgData) )
					//{
					//	pnMin[i&1]=(*pImgData);
					//	//▼1.0.0.1058
					//	pnMinLine[i&1]= (j<<16) + i;
					//	//▲1.0.0.1058
					//}

					nPos = (i/nBunch)&1;
					if( pnMin[nPos]>(*pImgData) )
					{
						pnMin[nPos]=(*pImgData);
						pnMinLine[nPos]= (j<<16) + i;
					}
					//▲1.0.0.1067b

					pImgData = (X *)((char *)pImgData + nSrcLinePitch);
				}
			}
			pImg += 1;
		}
	}
	return TRUE;
}


//EOの最小レベル取得
//dataWidth １画素のビット数
//モノクロのみで対応
//オフセット設定中
//▼1.0.0.1067b
//▼1.0.0.1058
//BOOL StCheckBase::GetMinLevel( StBuffer *aBuffer, INT nStart, INT nEnd, int *pnMin )
BOOL StCheckBase::GetMinLevel( StBuffer *aBuffer, INT nStart, INT nEnd, int *pnMin, int *pnMinLine, int nBunch )
//▲1.0.0.1058
//▲1.0.0.1067b
{

	INT nElement = GetElementFromBuffer(aBuffer);
	INT nPixelSize = GetPixelSize(aBuffer);
	if( nElement==0 )
		return FALSE;
	if( nPixelSize%8 )
		return FALSE;
	if( nStart>nEnd )
		return FALSE;
	if( nEnd>=(INT)aBuffer->GetWidth() )
		return FALSE;

	INT nWidth = aBuffer->GetWidth();		//画像高さ
	INT nHeight = aBuffer->GetHeight();		//画像高さ
	INT nLinePitch = aBuffer->GetLinePitch();		//画像高さ
	INT nMinData[2] = {9999,9999};
	//▼1.0.0.1058
	INT nMinLine[2] = {-1,-1};
	//▲1.0.0.1058
	unsigned char *pInImg = (unsigned char *)aBuffer->GetDataPointer();
	BOOL bReval = FALSE;
	if( nPixelSize==8 )		//8bit  1byte
	{
		//▼1.0.0.1067b
		////▼1.0.0.1058
		////bReval = GetMinLevelValue( pInImg, nElement, nWidth, nHeight, nLinePitch, nStart, nEnd, &nMinData[0] );
		//bReval = GetMinLevelValue( pInImg, nElement, nWidth, nHeight, nLinePitch, nStart, nEnd, &nMinData[0], &nMinLine[0] );
		////▲1.0.0.1058
		bReval = GetMinLevelValue( pInImg, nElement, nWidth, nHeight, nLinePitch, nStart, nEnd, &nMinData[0], &nMinLine[0], nBunch );
		//▲1.0.0.1067b
		}
	else
	{						//2byte
		//▼1.0.0.1067b
		////▼1.0.0.1058
		//bReval = GetMinLevelValue( (unsigned short *)pInImg, nElement, nWidth, nHeight, nLinePitch, nStart, nEnd, &nMinData[0], &nMinLine[0] );
		////▲1.0.0.1058
		bReval = GetMinLevelValue( (unsigned short *)pInImg, nElement, nWidth, nHeight, nLinePitch, nStart, nEnd, &nMinData[0], &nMinLine[0], nBunch );
		//▲1.0.0.1067b
	}
	if( bReval )
	{
		if( pnMin )
		{
			for( INT i=0; i<2; i++ )
			{
				pnMin[i] = nMinData[i];		//Even Odd
			}
		}
		//▼1.0.0.1058
		if( pnMinLine )
		{
			for( INT i=0; i<2; i++ )
			{
				pnMinLine[i] = nMinLine[i];		//Even Odd
			}
		}
		//▲1.0.0.1058
	}
	return bReval;
}

//▲1.0.0.1044

//▼1.0.0.1048

template <class X> BOOL GetChannelAverageValue( X *ptRaw, WORD wElementsPerPixel, DWORD dwImageWidth, DWORD dwImageHeight, INT nChannel, PINT pnStartLine, INT nWidth
			, double *pAverage )
{
	PINT64 pSum = new INT64 [nChannel];
	if( pSum==NULL ) return FALSE;

	for( int c=0; c<wElementsPerPixel; c++ )
	{
		memset( pSum, 0, sizeof(INT64)*nChannel );
		for( int i=0; i<nChannel/2; i++ )
		{
			X *pImgX = ptRaw + pnStartLine[i] * wElementsPerPixel;
			for( int y=0; y<dwImageHeight; y++ )
			{
				X *pImg = pImgX;
				for( int x=0; x<nWidth; x++ )
				{
					pSum[(x&1)+i*2] += *pImg;
					pImg += wElementsPerPixel;
				}
				pImgX += dwImageWidth * wElementsPerPixel;
			}
		}
		INT nCount = dwImageHeight * nWidth / 2;
		for( int i=0; i<nChannel/2; i++ )
		{
			pAverage[ i + c * nChannel ] = double (pSum[i]) / nCount;
		}
	}

	return TRUE;
}



BOOL StCheckBase::GetChannelAverage( StBuffer *aBuffer, PINT pnStart, INT nWidth, INT nChannel, double *pAverage )
{

	INT nElement = GetElementFromBuffer(aBuffer);
	INT nPixelSize = GetPixelSize(aBuffer);

	INT nImageWidth = aBuffer->GetWidth();		//画像
	INT nImageHeight = aBuffer->GetHeight();	//画像
	INT nLinePitch = aBuffer->GetLinePitch();	//画像


	if( nElement==0 )
		return FALSE;
	//if( nPixelSize%8 )
	//	return FALSE;
	for( int i=0; i<nChannel/2; i++ )
	{
		if( pnStart[i]+nWidth>nImageWidth )
			return FALSE;
	}
	unsigned char *pInImg = (unsigned char *)aBuffer->GetDataPointer();
	BOOL bReval = FALSE;
	if( nPixelSize==8 )		//8bit  1byte
	{
		bReval = GetChannelAverageValue( (unsigned char *)pInImg, nElement, nImageWidth, nImageHeight, nChannel, pnStart, nWidth, pAverage );
	}
	else
	{						//2byte
		bReval = GetChannelAverageValue( (unsigned short *)pInImg, nElement, nImageWidth, nImageHeight, nChannel, pnStart, nWidth, pAverage );
	}
	return bReval;


}
//▲1.0.0.1048
