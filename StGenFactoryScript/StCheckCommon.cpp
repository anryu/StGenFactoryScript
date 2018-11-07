#include "StdAfx.h"
#include "StCheckCommon.h"
#include "StCommon.h"
//▼1.0.0.1035
//▼1.0.0.1039
//#include "../CommonHeader/StSaveFileImage.h"
//#include "../CommonHeader/StTransformsImage.h"
//#include "StBufferInfo.h"
//▲1.0.0.1039
//▲1.0.0.1035

StCheckCommon::StCheckCommon(void)
{
	Initialize();
}

StCheckCommon::StCheckCommon(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
{
	Initialize();
}

StCheckCommon::~StCheckCommon(void)
{
}

void StCheckCommon::Initialize(void)
{
	m_szFileName = _T("");
	mSaveFilePath = ".";
	m_nCSVScopeLeft = -1;
	m_nCSVScopeRight = -1;
	//▼1.0.0.1036
	m_nWaitFrameTimeout = 30000;
	m_szLastErrorMessage=_T("");
	//▲1.0.0.1036
}


BOOL StCheckCommon::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="SetFileName" )
	{
		m_szFileName = szArgument;
		//▼1.0.0.1035
		if( m_szFileName.Left(1)==_T("\"") ) m_szFileName = m_szFileName.Mid(1);
		if( m_szFileName.Right(1)==_T("\"") ) m_szFileName = m_szFileName.Left(m_szFileName.GetLength()-1);
		//▲1.0.0.1035
		bReval = TRUE;
	}
	else
	if( szFunction=="SaveBitmapExecute" )
	{
		m_szLastErrorMessage = _T("");
		//▼1.0.0.1062
		//bReval = SaveBitmap();
		if( m_nScriptCheckMode==0 ) bReval = SaveBitmap();
		else bReval = TRUE;
		//▲1.0.0.1062
		if( !bReval ) m_szLastErrorMessage=_T("SaveBitmap Error");
	}
	else
	if( szFunction=="SaveCSVExecute" )
	{
		m_szLastErrorMessage = _T("");
		//▼1.0.0.1062
		//bReval = SaveCSV();
		if( m_nScriptCheckMode==0 ) bReval = SaveCSV();
		else bReval = TRUE;
		//▲1.0.0.1062
		if( !bReval ) m_szLastErrorMessage=_T("SaveCSV Error");
	}
	else
	if( szFunction=="SaveAllExecute" )
	{
		m_szLastErrorMessage = _T("");
		//▼1.0.0.1062
		//bReval = SaveAll();
		if( m_nScriptCheckMode==0 ) bReval = SaveAll();
		else bReval = TRUE;
		//▲1.0.0.1062
		//▼1.0.0.1055
		if( !bReval ) m_szLastErrorMessage=_T("SaveAll Error");
		//▲1.0.0.1055
	}
	else
	if( szFunction=="SetCSVScopeLeft" )
	{
		m_nCSVScopeLeft = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetCSVScopeRight" )
	{
		m_nCSVScopeRight = _ttoi(szArgument);
		bReval = TRUE;
	}
	//▼1.0.0.1035
	else
	if( szFunction=="SavePNGExecute" )
	{
		m_szLastErrorMessage = _T("");
		//▼1.0.0.1062
		//bReval = SavePNG();
		if( m_nScriptCheckMode==0 ) bReval = SavePNG();
		else bReval = TRUE;
		//▲1.0.0.1062
		if( !bReval ) m_szLastErrorMessage=_T("SavePNG Error");
	}
	else
	if( szFunction=="SaveJpegExecute" )
	{
		m_szLastErrorMessage = _T("");
		//▼1.0.0.1062
		//bReval = SaveJpeg();
		if( m_nScriptCheckMode==0 ) bReval = SaveJpeg();
		else bReval = TRUE;
		//▲1.0.0.1062
		if( !bReval ) m_szLastErrorMessage=_T("SaveJpeg Error");
	}
	//▲1.0.0.1035
	//▼1.0.0.1055
	else
	if( szFunction=="SaveHistogramExecute" )
	{
		m_szLastErrorMessage = _T("");
		//▼1.0.0.1062
		//bReval = SaveHistogram();
		if( m_nScriptCheckMode==0 ) bReval = SaveHistogram();
		else bReval = TRUE;
		//▲1.0.0.1062
		if( !bReval ) m_szLastErrorMessage=_T("SaveHistogram Error");
	}
	//▲1.0.0.1055

	//▼1.0.0.1036
	else
	if( szFunction=="WaitFrameExecute" )
	{
		m_szLastErrorMessage = _T("");
		INT nWaitFrame = _ttoi(szArgument);
		//▼1.0.0.1062
		//bReval = WaitFrameExecute(nWaitFrame);
		if( m_nScriptCheckMode==0 ) bReval = WaitFrameExecute(nWaitFrame);
		else bReval = TRUE;
		//▲1.0.0.1062
		if( !bReval ) m_szLastErrorMessage=_T("WaitFrameTimeout");
	}
	else
	if( szFunction=="SetWaitFrameTimeout" || szFunction=="WaitFrameTimeout" )
	{
		m_nWaitFrameTimeout = _ttoi(szArgument);
		bReval = TRUE;
	}
	//▲1.0.0.1036

	return bReval;
}



BOOL StCheckCommon::SaveBitmap(StBuffer *aBuffer,LPCTSTR szDateFormat)
{
	BOOL bReval = FALSE;

	CString szDeviceModelName;
	CString szSerialNumber;
	StString aText;
	bReval = GetDeviceModelName(aText);
	if( bReval )
		szDeviceModelName = aText.GetUnicode();
	bReval = GetSerialNumber(aText);
	if( bReval )
		szSerialNumber = aText.GetUnicode();

	CString szFileName;
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
	szFileName += _T("_") + dateFormat + _T("_") + m_szFileName;
	if(  szFileName.Find(_T("."))<0 )
	{
		szFileName += _T(".bmp");
	}

	//%H%M%S部分を削除
	//dateFormat = gt.Format("%Y%m%d");
	dateFormat = dateFormat.Left(dateFormat.GetLength()-6);

	CString szFullPath = mSaveFilePath + _T("\\") + szDeviceModelName + _T("\\") + dateFormat + _T("\\bmp\\");

	CStCommon::CheckDirectoryExist(szFullPath,TRUE);

	CString szFullFileName = szFullPath + szFileName;

	if( !aBuffer )
	{
		if( m_Buffer ) delete m_Buffer;
		m_Buffer = new StBuffer;
		m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー
		aBuffer = m_Buffer;

//StPixelType aType;
//aBuffer->GetPixelType(aType);
//TRACE( "aType=0x%x\n", aType );
//PBYTE adata = (PBYTE)aBuffer->GetDataPointer();
//adata += 4915198;
//TRACE( "aData=%d\n", *(PWORD)adata );

	}

	bReval = SaveBitmapFile(aBuffer, szFullFileName);

	return bReval;
}

BOOL StCheckCommon::SaveCSV(StBuffer *aBuffer, LPCTSTR szDateFormat)
{
	BOOL bReval = FALSE;

	CString szDeviceModelName;
	CString szSerialNumber;
	StString aText;
	bReval = GetDeviceModelName(aText);
	if( bReval )
		szDeviceModelName = aText.GetUnicode();
	bReval = GetSerialNumber(aText);
	if( bReval )
		szSerialNumber = aText.GetUnicode();

	CString szFileName;
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
	szFileName += _T("_") + dateFormat + _T("_") + m_szFileName;
	if(  szFileName.Find(_T("."))<0 )
	{
		szFileName += _T(".csv");
	}

	//%H%M%S部分を削除
	//dateFormat = gt.Format("%Y%m%d");
	dateFormat = dateFormat.Left(dateFormat.GetLength()-6);

	CString szFullPath = mSaveFilePath + _T("\\") + szDeviceModelName + _T("\\") + dateFormat + _T("\\csv\\");

	CStCommon::CheckDirectoryExist(szFullPath,TRUE);

	CString szFullFileName = szFullPath + szFileName;


//if( szFileName==_T("ffmd=1_LEDon_opck=2_10bit") )
//{
//	TRACE(TEXT("@@@@@@@@@ m_DeviceBase=0x%x\n"), m_DeviceBase );
//}


	if( !aBuffer )
	{
		if( m_Buffer ) delete m_Buffer;
		m_Buffer = new StBuffer;
		bReval = m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー
		if( bReval )
			aBuffer = m_Buffer;
	}

	if( bReval )
	{
		//bReval = SaveCSVFile(aBuffer, szFullPath, szFileName);
		bReval = SaveCSVFile(aBuffer, szFullPath, szFileName, m_nCSVScopeLeft, m_nCSVScopeRight );
		//BOOL StCheckBase::SaveCSVFile(StBuffer *aBuffer, LPCTSTR szFilePath, LPCTSTR szFileName, INT nScopeLeft, INT nScopeRight )
	}
	
	return bReval;
}

BOOL StCheckCommon::SaveAll()
{
	BOOL bReval = FALSE;
	CTime gt=CTime::GetCurrentTime();
	CString dateFormat = gt.Format("%Y%m%d%H%M%S");

	if( m_Buffer ) delete m_Buffer;
	m_Buffer = new StBuffer;
	m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー

	bReval = SaveBitmap(m_Buffer, dateFormat);
	if( bReval )
		bReval = SaveCSV(m_Buffer, dateFormat);

	return bReval;
}
//▼1.0.0.1035
BOOL StCheckCommon::SavePNG(StBuffer *aBuffer,LPCTSTR szDateFormat)
{
	return SaveImage( aBuffer, szDateFormat, 0 );
}
BOOL StCheckCommon::SaveJpeg(StBuffer *aBuffer,LPCTSTR szDateFormat)
{
	return SaveImage( aBuffer, szDateFormat, 1 );
}

BOOL StCheckCommon::SaveImage(StBuffer *aBuffer,LPCTSTR szDateFormat, INT nImageMode)
{
	BOOL bReval = FALSE;

	CString szDeviceModelName;
	CString szSerialNumber;
	StString aText;
	bReval = GetDeviceModelName(aText);
	if( bReval )
		szDeviceModelName = aText.GetUnicode();
	bReval = GetSerialNumber(aText);
	if( bReval )
		szSerialNumber = aText.GetUnicode();

	CString szFileName;
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
	szFileName += _T("_") + dateFormat + _T("_") + m_szFileName;
	if(  szFileName.Find(_T("."))<0 )
	{
		if( nImageMode==0 )
			szFileName += _T(".png");
		else
			szFileName += _T(".jpg");
	}

	//%H%M%S部分を削除
	//dateFormat = gt.Format("%Y%m%d");
	dateFormat = dateFormat.Left(dateFormat.GetLength()-6);

	CString szFullPath = mSaveFilePath + _T("\\") + szDeviceModelName + _T("\\") + dateFormat + _T("\\Image\\");

	CStCommon::CheckDirectoryExist(szFullPath,TRUE);

	CString szFullFileName = szFullPath + szFileName;

	if( !aBuffer )
	{
		if( m_Buffer ) delete m_Buffer;
		m_Buffer = new StBuffer;
		m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー
		aBuffer = m_Buffer;
	}

	//▼1.0.0.1039
	//INT nWidth = aBuffer->GetWidth();
	//INT nHeight = aBuffer->GetHeight();
	//StBufferInfo aInfo(aBuffer);
	//▲1.0.0.1039
	//BOOL bColor = aInfo.GetColor();

	//▼1.0.0.1039
	//StBuffer dstBuffer;
	//INT nDestLinePitch = aBuffer->GetWidth() * 3;
	//nDestLinePitch = (nDestLinePitch + 3) & (~0x3);
	//dstBuffer.Alloc( nDestLinePitch * aBuffer->GetHeight() );
	//dstBuffer.SetWidth(aBuffer->GetWidth());
	//dstBuffer.SetHeight(aBuffer->GetHeight());
	//dstBuffer.SetLinePitch(nDestLinePitch);
	//dstBuffer.SetPixelType(StPixelBGR8);

	////	Converter
	//bReval = StTransformsImage::Transform( aBuffer, &dstBuffer).IsOK();
	//if( bReval )
	//{
	//	StSaveFileImage aSaveFileImage;
	//	bReval = aSaveFileImage.executeFileSave(szFullFileName, (const unsigned char *)dstBuffer.GetDataPointer(), aBuffer->GetWidth(), aBuffer->GetHeight(), 1.0, 1.0);
	//}
	//dstBuffer.Free();
	bReval = SaveImageFile( aBuffer, szFullFileName);
	//▲1.0.0.1039

	return bReval;
}
//▲1.0.0.1035

//▼1.0.0.1055
BOOL StCheckCommon::SaveHistogram(StBuffer *aBuffer, LPCTSTR szDateFormat)
{
	BOOL bReval = FALSE;

	CString szDeviceModelName;
	CString szSerialNumber;
	StString aText;
	bReval = GetDeviceModelName(aText);
	if( bReval )
		szDeviceModelName = aText.GetUnicode();
	bReval = GetSerialNumber(aText);
	if( bReval )
		szSerialNumber = aText.GetUnicode();

	CString szFileName;
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
	szFileName += _T("_") + dateFormat + _T("_") + m_szFileName;
	if(  szFileName.Find(_T("."))<0 )
	{
		szFileName += _T(".csv");
	}

	//%H%M%S部分を削除
	dateFormat = dateFormat.Left(dateFormat.GetLength()-6);

	CString szFullPath = mSaveFilePath + _T("\\") + szDeviceModelName + _T("\\") + dateFormat + _T("\\csv\\");

	CStCommon::CheckDirectoryExist(szFullPath,TRUE);

	CString szFullFileName = szFullPath + szFileName;

	if( !aBuffer )
	{
		if( m_Buffer ) delete m_Buffer;
		m_Buffer = new StBuffer;
		bReval = m_DeviceBase->GetStBuffer(m_Buffer);
		if( bReval )
			aBuffer = m_Buffer;
	}

	if( bReval )
	{
		bReval = SaveHistogramFile(aBuffer, szFullPath, szFileName, m_nCSVScopeLeft, m_nCSVScopeRight );
	}
	return bReval;
}
//▲1.0.0.1055

//▼1.0.0.1036
BOOL StCheckCommon::WaitFrameExecute(INT nWaitFrame)
{
	m_iJudge=2;
	BOOL bReval = m_DeviceBase->WaitFrame(nWaitFrame, m_nWaitFrameTimeout);
	if( bReval )	m_iJudge=1;
	else			m_iJudge=0;

	return bReval;
}
//▲1.0.0.1036

