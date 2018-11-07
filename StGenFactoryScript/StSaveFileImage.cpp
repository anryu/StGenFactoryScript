#include "StdAfx.h"
//#include "../CommonHeader/StSaveFileImage.h"
#include "StSaveFileImage.h"

#include <Windows.h>
#include <GdiPlus.h>
#include <Gdiplusinit.h>
#pragma comment( lib, "gdiplus.lib" )

StSaveFileImage::StSaveFileImage(void)
{
}


StSaveFileImage::~StSaveFileImage(void)
{
}


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

int GetEncoderInfo(const WCHAR* format, Gdiplus::ImageCodecInfo* aImageCodecInfo)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*aImageCodecInfo = pImageCodecInfo[j];
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

bool StSaveFileImage::executeJpeg(LPCTSTR filename, const unsigned char *pBGRImage, int nWidth, int nHeight, double dblRatioWidth, double dblRatioHeight)
{
	bool bReval = false;


	unsigned char* pbyteSaveBuffer = NULL;

	//拡大縮小
	int nDestWidth = (int)(nWidth * dblRatioWidth);
	int nDestHeight = (int)(nHeight * dblRatioHeight);
	PBYTE	pbyteBuffer = NULL;

	if( nWidth!=nDestWidth || nHeight != nDestHeight )
	{
		BOOL bStretch = FALSE;
		do
		{
			pbyteSaveBuffer = new BYTE [nDestWidth * nDestHeight * 3];
			if( pbyteSaveBuffer == NULL ) break;

			//2.creat HBITMAP
			//[src bmp]
			BITMAPINFO bmSrcInfo;

			ZeroMemory(&bmSrcInfo, sizeof(bmSrcInfo));
			bmSrcInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmSrcInfo.bmiHeader.biWidth = nWidth;
			bmSrcInfo.bmiHeader.biHeight = nHeight;
			bmSrcInfo.bmiHeader.biPlanes = 1;
			bmSrcInfo.bmiHeader.biBitCount = 24;
			bmSrcInfo.bmiHeader.biCompression = BI_RGB;

			HDC hMemDC,copyhMemDC;
			PBYTE pbyteSrc=NULL;
			HDC hDC = GetDC( NULL );
			HBITMAP hBitmap = CreateDIBSection(
						hDC,
						(LPBITMAPINFO)&bmSrcInfo,
						DIB_RGB_COLORS,
						(void**)&pbyteSrc,
						NULL,0);

			if( !pbyteSrc )
			{
				ReleaseDC( NULL, hDC );
				break;
			}
			memcpy( pbyteSrc, pBGRImage, nWidth * nHeight * 3 );

			//[dst bmp]
			BITMAPINFO bmOutInfo;

			ZeroMemory(&bmOutInfo, sizeof(bmOutInfo));
			bmOutInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmOutInfo.bmiHeader.biWidth = nDestWidth;
			bmOutInfo.bmiHeader.biHeight = nDestHeight;
			bmOutInfo.bmiHeader.biPlanes = 1;
			bmOutInfo.bmiHeader.biBitCount = 24;
			bmOutInfo.bmiHeader.biCompression = BI_RGB;

			PBYTE pbyteOut=NULL;
			HBITMAP hOutBitmap = CreateDIBSection(
						hDC,
						(LPBITMAPINFO)&bmOutInfo,
						DIB_RGB_COLORS,
						(void**)&pbyteOut,
						NULL,0);

			if( !pbyteOut )
			{
				DeleteObject( hBitmap );
				ReleaseDC( NULL, hDC );
				break;
			}


			//3.create DC
			hMemDC = CreateCompatibleDC( NULL );
			copyhMemDC = CreateCompatibleDC( NULL );


			//4.select hBitmap
			HGDIOBJ hOld=NULL, hSrcOld=NULL;
			hSrcOld = SelectObject ( hMemDC, hBitmap);
			hOld = SelectObject( copyhMemDC, hOutBitmap );

			//5.set stretch
			SetStretchBltMode ( copyhMemDC, COLORONCOLOR );

			//6.stretch
			bStretch = StretchBlt(
					copyhMemDC,
					0, 0,
					nDestWidth, nDestHeight,
					hMemDC,
					0, 0,
					nWidth, nHeight,
					SRCCOPY);
			
			//7.save data
			if( bStretch )
			{
				//Stretch成功 pbyteOutに入っている.
				memcpy( pbyteSaveBuffer, pbyteOut, nDestWidth*nDestHeight*3 );
			}

			//8.delete hBitmap
			SelectObject(copyhMemDC,hOld);
			SelectObject(hMemDC,hSrcOld);

			DeleteObject( hOutBitmap );
			DeleteObject( hBitmap );

			//9.delete DC
			DeleteDC ( hMemDC );
			DeleteDC ( copyhMemDC );

			ReleaseDC( NULL, hDC );
		}while(0);

		if( !bStretch )
		{
			if( pbyteSaveBuffer != NULL ) delete [] pbyteSaveBuffer;
			pbyteSaveBuffer = NULL;
		}
		//if( !pbyteBuffer )
		//	return false;
	}
	else
	{
		pbyteSaveBuffer = (unsigned char*)pBGRImage;
	}

	if( !pbyteSaveBuffer )
	{
		return false;
	}

	//GDI+の初期化
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::Status aStatus = Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	if( aStatus == Gdiplus::Ok )
	{
		{
			//bitmapの作成
			Gdiplus::Bitmap bmp( nDestWidth, nDestHeight );

			//データを転送
			const unsigned char* pPix = pbyteSaveBuffer;
			for( int iY = 0; iY < nDestHeight; ++iY ){
				for( int iX = 0; iX < nDestWidth; ++iX ){
					Gdiplus::Color color( pPix[2], pPix[1], pPix[0] );
					bmp.SetPixel( iX, iY, color );
					pPix += 3;
				}
			}

			DeleteFile(filename);

			//jpegとしてセーブ
			CLSID id;
			GetEncoderClsid( L"image/jpeg", &id );
			aStatus = bmp.Save( filename, &id );	//品質指定しない場合、おそらく75%
			if( aStatus == Gdiplus::Ok )
				bReval = true;

			//品質を指定する場合　....回転EncoderTransformation　色深度EncoderColorDepth
			//Gdiplus::EncoderParameters aEncoderParams;
			//aEncoderParams.Count = 1;
			//aEncoderParams.Parameter[0].Guid = Gdiplus::EncoderQuality;
			//aEncoderParams.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
			//aEncoderParams.Parameter[0].NumberOfValues = 1;
			//ULONG quality = 75;
			//aEncoderParams.Parameter[0].Value = &quality;
			//aStatus = bmp.Save( filename, &id, &aEncoderParams );
			//if( aStatus == Gdiplus::Ok )
			//	bReval = true;


			//Gdiplus::ImageCodecInfo aImageCodecInfo;
			//GetEncoderInfo( L"image/jpeg", &aImageCodecInfo );
			//aStatus = bmp.Save( filename, &aImageCodecInfo, &aEncoderParams );
			//if( aStatus == Gdiplus::Ok )
			//	bReval = true;

		}
		//GDI+の開放
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}

	if( pbyteSaveBuffer != pBGRImage && pbyteSaveBuffer!=NULL )
	{
		delete [] pbyteSaveBuffer;
	}
	return bReval;

}


BOOL bSaveBitmapFileColor( DWORD dwWidth, DWORD dwHeight, DWORD dwSrcWidth, DWORD dwSrcHeight, PBYTE pbyteSrcData, LPCTSTR szFineName )
{
	FILE *fp = NULL;

	if( NO_ERROR != _tfopen_s( &fp, szFineName, _T("wb") ) )
	{
		return FALSE;
	}

	LPBITMAPFILEHEADER	pbmFileHeader;
	LPBITMAPINFOHEADER	pbmInfoHeader;

	PBYTE	pbyteBuffer = NULL;
	BOOL	bReval = FALSE;
	do{
		//1.create bitmap
		//[dst bmp]
		DWORD dwHeaderSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		DWORD dwImageSize = dwWidth * dwHeight * 3;

		DWORD dwBufferSize = dwHeaderSize + dwImageSize;
		//create buffer
		pbyteBuffer = (PBYTE)malloc(sizeof(BYTE) * (dwBufferSize));
		if(pbyteBuffer == NULL)
		{
			break;
		}
		//initialize
		memset(pbyteBuffer, 0, dwHeaderSize);
		//image pointer
		PBYTE pImageData = pbyteBuffer + dwHeaderSize;
		//set fileheader
		pbmFileHeader = (LPBITMAPFILEHEADER)pbyteBuffer;
		pbmFileHeader->bfType = (((WORD)'M') << 8) | (WORD)'B';
		pbmFileHeader->bfSize = dwBufferSize;
		pbmFileHeader->bfOffBits = dwHeaderSize;
		//set infoheader
		pbmInfoHeader = (LPBITMAPINFOHEADER)(pbyteBuffer + sizeof(BITMAPFILEHEADER));
		pbmInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
		pbmInfoHeader->biWidth = dwWidth;
		pbmInfoHeader->biHeight = -(LONG)dwHeight;
		pbmInfoHeader->biPlanes = 1;
		pbmInfoHeader->biBitCount = 24;
		pbmInfoHeader->biSizeImage = dwWidth * dwHeight * (pbmInfoHeader->biBitCount >> 3);

		if( dwWidth!=dwSrcWidth || dwHeight != dwSrcHeight )
		{
			//2.creat HBITMAP
			//[src bmp]
			BITMAPINFO bmSrcInfo;

			ZeroMemory(&bmSrcInfo, sizeof(bmSrcInfo));
			bmSrcInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmSrcInfo.bmiHeader.biWidth = dwSrcWidth;
			bmSrcInfo.bmiHeader.biHeight = dwSrcHeight;
			bmSrcInfo.bmiHeader.biPlanes = 1;
			bmSrcInfo.bmiHeader.biBitCount = 24;
			bmSrcInfo.bmiHeader.biCompression = BI_RGB;

			HDC hMemDC,copyhMemDC;
			PBYTE pbyteSrc=NULL;
			HDC hDC = GetDC( NULL );
			HBITMAP hBitmap = CreateDIBSection(
						hDC,
						(LPBITMAPINFO)&bmSrcInfo,
						DIB_RGB_COLORS,
						(void**)&pbyteSrc,
						NULL,0);

			if( !pbyteSrc )
			{
				ReleaseDC( NULL, hDC );
				break;
			}
			memcpy( pbyteSrc, pbyteSrcData, dwSrcWidth * dwSrcHeight * 3 );

			//[dst bmp]
			BITMAPINFO bmOutInfo;

			ZeroMemory(&bmOutInfo, sizeof(bmOutInfo));
			bmOutInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmOutInfo.bmiHeader.biWidth = dwWidth;
			bmOutInfo.bmiHeader.biHeight = dwHeight;
			bmOutInfo.bmiHeader.biPlanes = 1;
			bmOutInfo.bmiHeader.biBitCount = 24;
			bmOutInfo.bmiHeader.biCompression = BI_RGB;

			PBYTE pbyteOut=NULL;
			HBITMAP hOutBitmap = CreateDIBSection(
						hDC,
						(LPBITMAPINFO)&bmOutInfo,
						DIB_RGB_COLORS,
						(void**)&pbyteOut,
						NULL,0);

			if( !pbyteOut )
			{
				DeleteObject( hBitmap );
				ReleaseDC( NULL, hDC );
				break;
			}

			//3.create DC
			hMemDC = CreateCompatibleDC( NULL );
			copyhMemDC = CreateCompatibleDC( NULL );

			//4.select hBitmap
			HGDIOBJ hOld=NULL, hSrcOld=NULL;
			hSrcOld = SelectObject ( hMemDC, hBitmap);
			hOld = SelectObject( copyhMemDC, hOutBitmap );

			//5.set stretch
			SetStretchBltMode ( copyhMemDC, COLORONCOLOR );

			//6.stretch
			BOOL bStretch = StretchBlt(
					copyhMemDC,
					0, 0,
					dwWidth, dwHeight,
					hMemDC,
					0, 0,
					dwSrcWidth, dwSrcHeight,
					SRCCOPY);
			
			//7.save data
			if( bStretch )
			{
				memcpy( pImageData, pbyteOut, dwHeight*dwWidth*3 );
				size_t writeSize = fwrite( pbyteBuffer, sizeof(BYTE), dwBufferSize, fp );
				if( writeSize==dwBufferSize) bReval = TRUE;
			}

			//8.delete hBitmap
			SelectObject(copyhMemDC,hOld);
			SelectObject(hMemDC,hSrcOld);

			DeleteObject( hOutBitmap );
			DeleteObject( hBitmap );

			//9.delete DC
			DeleteDC ( hMemDC );
			DeleteDC ( copyhMemDC );

			ReleaseDC( NULL, hDC );
		}
		else
		{
			// dwWidth==dwSrcWidth && dwHeight == dwSrcHeight
			//7.save data
			memcpy( pImageData, pbyteSrcData, dwHeight*dwWidth*3 );
			size_t writeSize = fwrite( pbyteBuffer, sizeof(BYTE), dwBufferSize, fp );
			if( writeSize==dwBufferSize) bReval = TRUE;
		}

	}while(0);
	
	if( pbyteBuffer )	free( pbyteBuffer );
	fclose( fp );

	return(bReval);
}

bool StSaveFileImage::executeBitmap(LPCTSTR filename, const unsigned char *pBGRImage, int nWidth, int nHeight, double dblRatioWidth, double dblRatioHeight)
{
	bool bReval = false;


	unsigned char* pbyteSaveBuffer = NULL;

	//拡大縮小
	int nDestWidth = (int)(nWidth * dblRatioWidth);
	int nDestHeight = (int)(nHeight * dblRatioHeight);
	PBYTE	pbyteBuffer = NULL;

	if( nWidth!=nDestWidth || nHeight != nDestHeight )
	{
		BOOL bStretch = FALSE;
		do
		{
			pbyteSaveBuffer = new BYTE [nDestWidth * nDestHeight * 3];
			if( pbyteSaveBuffer == NULL ) break;

			//2.creat HBITMAP
			//[src bmp]
			BITMAPINFO bmSrcInfo;

			ZeroMemory(&bmSrcInfo, sizeof(bmSrcInfo));
			bmSrcInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmSrcInfo.bmiHeader.biWidth = nWidth;
			bmSrcInfo.bmiHeader.biHeight = nHeight;
			bmSrcInfo.bmiHeader.biPlanes = 1;
			bmSrcInfo.bmiHeader.biBitCount = 24;
			bmSrcInfo.bmiHeader.biCompression = BI_RGB;

			HDC hMemDC,copyhMemDC;
			PBYTE pbyteSrc=NULL;
			HDC hDC = GetDC( NULL );
			HBITMAP hBitmap = CreateDIBSection(
						hDC,
						(LPBITMAPINFO)&bmSrcInfo,
						DIB_RGB_COLORS,
						(void**)&pbyteSrc,
						NULL,0);

			if( !pbyteSrc )
			{
				ReleaseDC( NULL, hDC );
				break;
			}
			memcpy( pbyteSrc, pBGRImage, nWidth * nHeight * 3 );

			//[dst bmp]
			BITMAPINFO bmOutInfo;

			ZeroMemory(&bmOutInfo, sizeof(bmOutInfo));
			bmOutInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmOutInfo.bmiHeader.biWidth = nDestWidth;
			bmOutInfo.bmiHeader.biHeight = nDestHeight;
			bmOutInfo.bmiHeader.biPlanes = 1;
			bmOutInfo.bmiHeader.biBitCount = 24;
			bmOutInfo.bmiHeader.biCompression = BI_RGB;

			PBYTE pbyteOut=NULL;
			HBITMAP hOutBitmap = CreateDIBSection(
						hDC,
						(LPBITMAPINFO)&bmOutInfo,
						DIB_RGB_COLORS,
						(void**)&pbyteOut,
						NULL,0);

			if( !pbyteOut )
			{
				DeleteObject( hBitmap );
				ReleaseDC( NULL, hDC );
				break;
			}


			//3.create DC
			hMemDC = CreateCompatibleDC( NULL );
			copyhMemDC = CreateCompatibleDC( NULL );


			//4.select hBitmap
			HGDIOBJ hOld=NULL, hSrcOld=NULL;
			hSrcOld = SelectObject ( hMemDC, hBitmap);
			hOld = SelectObject( copyhMemDC, hOutBitmap );

			//5.set stretch
			SetStretchBltMode ( copyhMemDC, COLORONCOLOR );

			//6.stretch
			bStretch = StretchBlt(
					copyhMemDC,
					0, 0,
					nDestWidth, nDestHeight,
					hMemDC,
					0, 0,
					nWidth, nHeight,
					SRCCOPY);
			
			//7.save data
			if( bStretch )
			{
				//Stretch成功 pbyteOutに入っている.
				memcpy( pbyteSaveBuffer, pbyteOut, nDestWidth*nDestHeight*3 );
			}

			//8.delete hBitmap
			SelectObject(copyhMemDC,hOld);
			SelectObject(hMemDC,hSrcOld);

			DeleteObject( hOutBitmap );
			DeleteObject( hBitmap );

			//9.delete DC
			DeleteDC ( hMemDC );
			DeleteDC ( copyhMemDC );

			ReleaseDC( NULL, hDC );
		}while(0);

		if( !bStretch )
		{
			if( pbyteSaveBuffer != NULL ) delete [] pbyteSaveBuffer;
			pbyteSaveBuffer = NULL;
		}
		//if( !pbyteBuffer )
		//	return false;
	}
	else
	{
		pbyteSaveBuffer = (unsigned char*)pBGRImage;
	}

	if( !pbyteSaveBuffer )
	{
		return false;
	}

	//GDI+の初期化
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::Status aStatus = Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	if( aStatus == Gdiplus::Ok )
	{
		{
			//bitmapの作成
			Gdiplus::Bitmap bmp( nDestWidth, nDestHeight );

			//データを転送
			const unsigned char* pPix = pbyteSaveBuffer;
			for( int iY = 0; iY < nDestHeight; ++iY ){
				for( int iX = 0; iX < nDestWidth; ++iX ){
					Gdiplus::Color color( pPix[2], pPix[1], pPix[0] );
					bmp.SetPixel( iX, iY, color );
					pPix += 3;
				}
			}

			DeleteFile(filename);

			//jpegとしてセーブ
			CLSID id;
			GetEncoderClsid( L"image/bmp", &id );
			aStatus = bmp.Save( filename, &id );
			if( aStatus == Gdiplus::Ok )
				bReval = true;

		}
		//GDI+の開放
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}

	if( pbyteSaveBuffer != pBGRImage && pbyteSaveBuffer!=NULL )
	{
		delete [] pbyteSaveBuffer;
	}
	return bReval;

}




BOOL bSaveBitmapFileMono( DWORD dwWidth, DWORD dwHeight, DWORD dwSrcWidth, DWORD dwSrcHeight, PBYTE pbyteSrcData, LPCTSTR szFineName )
{
	FILE *fp = NULL;

	if( NO_ERROR != _tfopen_s( &fp, szFineName, _T("wb") ) )
	{
		return FALSE;
	}

	LPBITMAPFILEHEADER	pbmFileHeader;
	LPBITMAPINFOHEADER	pbmInfoHeader;

	BOOL	bReval = FALSE;
	PBYTE	pbyteBuffer = NULL;
	PBYTE	pbmSrcInfo = NULL;
	PBYTE	pbmOutInfo = NULL;

	do{
		//1.create bitmap header
		DWORD dwHeaderSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		dwHeaderSize += (sizeof(RGBQUAD) << 8);		//palette table
		DWORD dwImageSize = dwWidth * dwHeight;

		DWORD dwBufferSize = dwHeaderSize + dwImageSize;
		//create buffer
		pbyteBuffer = (PBYTE)malloc(sizeof(BYTE) * (dwBufferSize));
		if(pbyteBuffer == NULL)
		{
			break;
		}
		//initialize
		memset(pbyteBuffer, 0, dwHeaderSize);
		//image pointer
		PBYTE pImageData = pbyteBuffer + dwHeaderSize;
		//set fileheader
		pbmFileHeader = (LPBITMAPFILEHEADER)pbyteBuffer;
		pbmFileHeader->bfType = (((WORD)'M') << 8) | (WORD)'B';
		pbmFileHeader->bfSize = dwBufferSize;
		pbmFileHeader->bfOffBits = dwHeaderSize;
		//set infoheader
		pbmInfoHeader = (LPBITMAPINFOHEADER)(pbyteBuffer + sizeof(BITMAPFILEHEADER));
		pbmInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
		pbmInfoHeader->biWidth = dwWidth;
		pbmInfoHeader->biHeight = -(LONG)dwHeight;
		pbmInfoHeader->biPlanes = 1;
		pbmInfoHeader->biBitCount = 8;
		pbmInfoHeader->biClrUsed = 256;
		pbmInfoHeader->biClrImportant = 256;
		pbmInfoHeader->biSizeImage = dwWidth * dwHeight * (pbmInfoHeader->biBitCount >> 3);
		//set palette
		DWORD	dwCurPixelValue = 256;
		PDWORD	pdwPosRGBTable = (PDWORD)((PBYTE)pbmInfoHeader + sizeof(BITMAPINFOHEADER));
		do{
			dwCurPixelValue--;
			pdwPosRGBTable[dwCurPixelValue] = (dwCurPixelValue << 16) | (dwCurPixelValue << 8) | (dwCurPixelValue);
		}while(dwCurPixelValue);

		if( dwWidth!=dwSrcWidth || dwHeight != dwSrcHeight )
		{
			//2.creat HBITMAP
			//[src bmp]
			size_t bmInfoSize = sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD);
			pbmSrcInfo = new BYTE [bmInfoSize];
			if( pbmSrcInfo==NULL ) break;
			ZeroMemory( pbmSrcInfo, bmInfoSize );
			PBITMAPINFO pbmSrcBitmapInfo = (PBITMAPINFO)pbmSrcInfo;
			PBITMAPINFOHEADER pbmiSrcHeader = &pbmSrcBitmapInfo->bmiHeader;

			pbmiSrcHeader->biSize = sizeof(BITMAPINFOHEADER);
			pbmiSrcHeader->biWidth = dwSrcWidth;
			pbmiSrcHeader->biHeight = dwSrcHeight;
			pbmiSrcHeader->biPlanes = 1;
			pbmiSrcHeader->biBitCount = 8;
			pbmiSrcHeader->biSizeImage = dwSrcWidth * dwSrcHeight;
			pbmiSrcHeader->biCompression = BI_RGB;	//BI_RGB
			pbmiSrcHeader->biClrUsed = 256;
			pbmiSrcHeader->biClrImportant = 256;
			RGBQUAD *palette = pbmSrcBitmapInfo->bmiColors;
			for( int i=0; i<256; i++,palette++ )
			{
				palette->rgbBlue = (BYTE)i;
				palette->rgbGreen = (BYTE)i;
				palette->rgbRed = (BYTE)i;
				palette->rgbReserved = (BYTE)0;
			}


			//[out bmp]
			pbmOutInfo = new BYTE [bmInfoSize];
			if( pbmOutInfo==NULL ) break;
			ZeroMemory( pbmOutInfo, bmInfoSize );
			PBITMAPINFO pbmOutBitmapInfo = (PBITMAPINFO)pbmOutInfo;
			PBITMAPINFOHEADER pbmiOutHeader = &pbmOutBitmapInfo->bmiHeader;

			pbmiOutHeader->biSize = sizeof(BITMAPINFOHEADER);
			pbmiOutHeader->biWidth = dwWidth;
			pbmiOutHeader->biHeight = dwHeight;
			pbmiOutHeader->biPlanes = 1;
			pbmiOutHeader->biBitCount = 8;
			pbmiOutHeader->biSizeImage = dwWidth * dwHeight;
			pbmiOutHeader->biCompression = BI_RGB;	//BI_RGB
			pbmiOutHeader->biClrUsed = 256;
			pbmiOutHeader->biClrImportant = 256;
			palette = pbmOutBitmapInfo->bmiColors;
			for( int i=0; i<256; i++,palette++ )
			{
				palette->rgbBlue = (BYTE)i;
				palette->rgbGreen = (BYTE)i;
				palette->rgbRed = (BYTE)i;
				palette->rgbReserved = (BYTE)0;
			}

			//[src bmp]
			HDC hMemDC, copyhMemDC;
			PBYTE pbyteSrc = NULL;
			HDC hDC = GetDC( NULL );
			HBITMAP hBitmap = CreateDIBSection(
						hDC,
						(LPBITMAPINFO)pbmSrcBitmapInfo,
						DIB_RGB_COLORS,
						(void**)&pbyteSrc,
						NULL,0);

			if( !pbyteSrc ){
				ReleaseDC( NULL, hDC );
				break;
			}
			memcpy( pbyteSrc, pbyteSrcData, dwSrcWidth * dwSrcHeight );

			//[out bmp]
			PBYTE pbyteOut = NULL;
			HBITMAP hOutBitmap = CreateDIBSection(
						hDC,
						(LPBITMAPINFO)pbmOutBitmapInfo,
						DIB_RGB_COLORS,
						(void**)&pbyteOut,
						NULL,0);

			if( !pbyteOut )
			{
				DeleteObject( hBitmap );
				ReleaseDC( NULL, hDC );
				break;
			}

			//3.create DC
			hMemDC = CreateCompatibleDC( NULL );
			copyhMemDC = CreateCompatibleDC( NULL );

			//4.select hBitmap
			HGDIOBJ hOld=NULL, hSrcOld=NULL;
			hSrcOld = SelectObject ( hMemDC, hBitmap);
			hOld = SelectObject( copyhMemDC, hOutBitmap );

			//5.set stretch
			SetStretchBltMode ( copyhMemDC, COLORONCOLOR );

			//6.stretch
			BOOL bStretch = StretchBlt(
					copyhMemDC,
					0, 0,
					dwWidth, dwHeight,
					hMemDC,
					0, 0,
					dwSrcWidth, dwSrcHeight,
					SRCCOPY);
			
			//7.save data
			if( bStretch )
			{
				memcpy( pImageData, pbyteOut, dwHeight*dwWidth );
				size_t writeSize = fwrite( pbyteBuffer, sizeof(BYTE), dwBufferSize, fp );
				if( writeSize==dwBufferSize) bReval = TRUE;
			}

			//8.delete hBitmap
			SelectObject(copyhMemDC,hOld);
			SelectObject(hMemDC,hSrcOld);

			DeleteObject( hOutBitmap );
			DeleteObject( hBitmap );

			//9.delete DC
			DeleteDC ( hMemDC );
			DeleteDC ( copyhMemDC );

			ReleaseDC( NULL, hDC );
		}
		else
		{
			// dwWidth==dwSrcWidth && dwHeight == dwSrcHeight
			//7.save data
			memcpy( pImageData, pbyteSrcData, dwHeight*dwWidth );
			size_t writeSize = fwrite( pbyteBuffer, sizeof(BYTE), dwBufferSize, fp );
			if( writeSize==dwBufferSize) bReval = TRUE;
		}

	}while(0);

	if( pbmOutInfo )	free( pbmOutInfo );
	if( pbmSrcInfo )	free( pbmSrcInfo );
	if( pbyteBuffer )	free( pbyteBuffer );
	fclose( fp );

	return(bReval);
}

bool StSaveFileImage::executeBitmapMono8(LPCTSTR filename, const unsigned char *pImage, int nWidth, int nHeight, double dblRatioWidth, double dblRatioHeight)
{
	BOOL bReval = false;

	DWORD dwDstWidth = (DWORD)(nWidth * dblRatioWidth);
	DWORD dwDstHeight = (DWORD)(nHeight * dblRatioHeight);

	bReval = bSaveBitmapFileMono( dwDstWidth, dwDstHeight, nWidth, nHeight, (PBYTE)pImage, filename );
	return (bool)bReval;
}

bool StSaveFileImage::executeFileSave(LPCTSTR filename, const unsigned char *pBGRImage, int nWidth, int nHeight, double dblRatioWidth, double dblRatioHeight)
{
	bool bReval = false;


	unsigned char* pbyteSaveBuffer = NULL;

	//拡大縮小
	int nDestWidth = (int)(nWidth * dblRatioWidth);
	int nDestHeight = (int)(nHeight * dblRatioHeight);
	PBYTE	pbyteBuffer = NULL;

	if( nWidth!=nDestWidth || nHeight != nDestHeight )
	{
		BOOL bStretch = FALSE;
		do
		{
			pbyteSaveBuffer = new BYTE [nDestWidth * nDestHeight * 3];
			if( pbyteSaveBuffer == NULL ) break;

			//2.creat HBITMAP
			//[src bmp]
			BITMAPINFO bmSrcInfo;

			ZeroMemory(&bmSrcInfo, sizeof(bmSrcInfo));
			bmSrcInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmSrcInfo.bmiHeader.biWidth = nWidth;
			bmSrcInfo.bmiHeader.biHeight = nHeight;
			bmSrcInfo.bmiHeader.biPlanes = 1;
			bmSrcInfo.bmiHeader.biBitCount = 24;
			bmSrcInfo.bmiHeader.biCompression = BI_RGB;

			HDC hMemDC,copyhMemDC;
			PBYTE pbyteSrc=NULL;
			HDC hDC = GetDC( NULL );
			HBITMAP hBitmap = CreateDIBSection(
						hDC,
						(LPBITMAPINFO)&bmSrcInfo,
						DIB_RGB_COLORS,
						(void**)&pbyteSrc,
						NULL,0);

			if( !pbyteSrc )
			{
				ReleaseDC( NULL, hDC );
				break;
			}
			memcpy( pbyteSrc, pBGRImage, nWidth * nHeight * 3 );

			//[dst bmp]
			BITMAPINFO bmOutInfo;

			ZeroMemory(&bmOutInfo, sizeof(bmOutInfo));
			bmOutInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmOutInfo.bmiHeader.biWidth = nDestWidth;
			bmOutInfo.bmiHeader.biHeight = nDestHeight;
			bmOutInfo.bmiHeader.biPlanes = 1;
			bmOutInfo.bmiHeader.biBitCount = 24;
			bmOutInfo.bmiHeader.biCompression = BI_RGB;

			PBYTE pbyteOut=NULL;
			HBITMAP hOutBitmap = CreateDIBSection(
						hDC,
						(LPBITMAPINFO)&bmOutInfo,
						DIB_RGB_COLORS,
						(void**)&pbyteOut,
						NULL,0);

			if( !pbyteOut )
			{
				DeleteObject( hBitmap );
				ReleaseDC( NULL, hDC );
				break;
			}


			//3.create DC
			hMemDC = CreateCompatibleDC( NULL );
			copyhMemDC = CreateCompatibleDC( NULL );


			//4.select hBitmap
			HGDIOBJ hOld=NULL, hSrcOld=NULL;
			hSrcOld = SelectObject ( hMemDC, hBitmap);
			hOld = SelectObject( copyhMemDC, hOutBitmap );

			//5.set stretch
			SetStretchBltMode ( copyhMemDC, COLORONCOLOR );

			//6.stretch
			bStretch = StretchBlt(
					copyhMemDC,
					0, 0,
					nDestWidth, nDestHeight,
					hMemDC,
					0, 0,
					nWidth, nHeight,
					SRCCOPY);
			
			//7.save data
			if( bStretch )
			{
				//Stretch成功 pbyteOutに入っている.
				memcpy( pbyteSaveBuffer, pbyteOut, nDestWidth*nDestHeight*3 );
			}

			//8.delete hBitmap
			SelectObject(copyhMemDC,hOld);
			SelectObject(hMemDC,hSrcOld);

			DeleteObject( hOutBitmap );
			DeleteObject( hBitmap );

			//9.delete DC
			DeleteDC ( hMemDC );
			DeleteDC ( copyhMemDC );

			ReleaseDC( NULL, hDC );
		}while(0);

		if( !bStretch )
		{
			if( pbyteSaveBuffer != NULL ) delete [] pbyteSaveBuffer;
			pbyteSaveBuffer = NULL;
		}
		//if( !pbyteBuffer )
		//	return false;
	}
	else
	{
		pbyteSaveBuffer = (unsigned char*)pBGRImage;
	}

	if( !pbyteSaveBuffer )
	{
		return false;
	}

	//GDI+の初期化
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::Status aStatus = Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	if( aStatus == Gdiplus::Ok )
	{
		{
			//bitmapの作成
			Gdiplus::Bitmap bmp( nDestWidth, nDestHeight );

			//データを転送
			const unsigned char* pPix = pbyteSaveBuffer;
			for( int iY = 0; iY < nDestHeight; ++iY ){
				for( int iX = 0; iX < nDestWidth; ++iX ){
					Gdiplus::Color color( pPix[2], pPix[1], pPix[0] );
					bmp.SetPixel( iX, iY, color );
					pPix += 3;
				}
			}

			DeleteFile(filename);

			//拡張子取得
			CString szExt(filename);
			INT nPos = szExt.ReverseFind('.');
			if( nPos>0 )
			{
				szExt = szExt.Mid(nPos);
			}

			WCHAR format[32];
			_stprintf_s(format,_countof(format),L"image/bmp");
			if( szExt.Compare(_T(".jpg"))==0 )
			{
				_stprintf_s(format,_countof(format),L"image/jpeg");
			}
			else
			if( szExt.Compare(_T(".png"))==0 )
			{
				_stprintf_s(format,_countof(format),L"image/png");
			}
			else
			if( szExt.Compare(_T(".bmp"))==0 )
			{
				_stprintf_s(format,_countof(format),L"image/bmp");
			}

			CLSID id;
			GetEncoderClsid( format, &id );
			aStatus = bmp.Save( filename, &id );
			if( aStatus == Gdiplus::Ok )
				bReval = true;

		}
		//GDI+の開放
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}

	if( pbyteSaveBuffer != pBGRImage && pbyteSaveBuffer!=NULL )
	{
		delete [] pbyteSaveBuffer;
	}
	return bReval;

}


bool StSaveFileImage::executeFileSaveGrayScale(LPCTSTR filename, const unsigned char *pImage, int nWidth, int nHeight, int bytePerPixel)
{
	bool bReval = false;

	unsigned char* pbyteSaveBuffer = (unsigned char*)pImage;

	//拡大縮小
	int nDestWidth = nWidth;
	int nDestHeight = nHeight;
	PBYTE	pbyteBuffer = NULL;
	if( !pbyteSaveBuffer )
	{
		return false;
	}

	//GDI+の初期化
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::Status aStatus = Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	if( aStatus == Gdiplus::Ok )
	{
		{
			DWORD dwPixelFormat = PixelFormat8bppIndexed;
			if( bytePerPixel==2 )
			{
				dwPixelFormat = PixelFormat16bppGrayScale;
			}
			//bitmapの作成
			Gdiplus::Bitmap bmp( nDestWidth, nDestHeight, dwPixelFormat );

			if(  bytePerPixel==2 )
			{
				//データを転送
				const unsigned short* pPix = (const unsigned short*)pbyteSaveBuffer;
				for( int iY = 0; iY < nDestHeight; ++iY ){
					for( int iX = 0; iX < nDestWidth; ++iX ){
						bmp.SetPixel( iX, iY, *pPix++ );
					}
				}
			}
			else
			{
				//データを転送
				const unsigned char* pPix = pbyteSaveBuffer;
				for( int iY = 0; iY < nDestHeight; ++iY ){
					for( int iX = 0; iX < nDestWidth; ++iX ){
						bmp.SetPixel( iX, iY, *pPix++ );
					}
				}
			}
			DeleteFile(filename);

			//拡張子取得
			CString szExt(filename);
			INT nPos = szExt.ReverseFind('.');
			if( nPos>0 )
			{
				szExt = szExt.Mid(nPos);
			}

			WCHAR format[32];
			_stprintf_s(format,_countof(format),L"image/bmp");
			if( szExt.Compare(_T(".jpg"))==0 )
			{
				_stprintf_s(format,_countof(format),L"image/jpeg");
			}
			else
			if( szExt.Compare(_T(".png"))==0 )
			{
				_stprintf_s(format,_countof(format),L"image/png");
			}
			else
			if( szExt.Compare(_T(".bmp"))==0 )
			{
				_stprintf_s(format,_countof(format),L"image/bmp");
			}

			CLSID id;
			GetEncoderClsid( format, &id );

			//品質を指定する場合　....回転EncoderTransformation　色深度EncoderColorDepth
			//Gdiplus::EncoderParameters aEncoderParams;
			//aEncoderParams.Count = 1;
			//aEncoderParams.Parameter[0].Guid = Gdiplus::EncoderQuality;
			//aEncoderParams.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
			//aEncoderParams.Parameter[0].NumberOfValues = 1;
			//ULONG quality = 75;
			//aEncoderParams.Parameter[0].Value = &quality;



			aStatus = bmp.Save( filename, &id );
			if( aStatus == Gdiplus::Ok )
				bReval = true;

		}
		//GDI+の開放
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}

	//if( pbyteSaveBuffer != pImage && pbyteSaveBuffer!=NULL )
	//{
	//	delete [] pbyteSaveBuffer;
	//}
	return bReval;

}
