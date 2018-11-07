#include "StdAfx.h"
#include "StCustomDevice.h"

#define _GENICAM
#ifdef _GENICAM
#include <GenApi/GenApi.h>
using namespace GenApi;
#else

#endif

StCustomDevice::StCustomDevice(void)
#ifdef _PLEORA
	: mPipeline( &mStream )
#endif
{
	m_bStreamCreated = false;
	m_bStarted = false;
	m_lThreadDisplay = NULL;
	m_Preview = NULL;

	m_szDefPixelFormat="";
	m_szDefExposureMode="";
	m_dblDefAcquisitionFramerate = 0.0;
	m_dblDefExposureTime = 0.0;
	//▼1.0.0.21
	m_dblDefBlackLevel = -1.0;
	//▲1.0.0.21

	//m_dwEndian = 0;
	m_dwTechnologyAgnosticFunctionSaveAddress = 0;
	m_dwTechnologyAgnosticDoNotSaveAddress=0;
	m_dwTechnologyAgnosticCameraAdministratorAddress=0;
	m_dwTechnologyAgnosticCameraFeatureAddress=0;
	//▼1.0.0.22
	m_dwTechnologyAgnosticCameraStatusAddress = 0;
	//▲1.0.0.22
	m_dwFPGAAdjustmentAddress=0;
	m_dwSensorAdjustmentAddress=0;
	m_dwVendorUniqueControlAddress=0;
	m_dwCameraFeatureAddress=0;
	m_dwDefectivePixelCorrectionAddress = 0;

	m_dwControlPacketSizeMax = 0;


	mCameraType = NULL;
#ifndef _PLEORA
	mGenDeviceInfo = NULL;
#endif

}


StCustomDevice::~StCustomDevice(void)
{
	StopStreams();
}

//StResult StCustomDevice::StartStreams(CPreview *aDisplayWnd)
StResult StCustomDevice::StartStreams(void *aDisplayWnd)
{
	StResult aResult = StResult::Code::NOT_STARTSTREAM;
#ifdef _PLEORA

    PvResult lResult = mPipeline.Start();
	if( lResult.IsOK() )
	{
		aResult = StResult::Code::OK;
	}

#else
	aResult = CreateStream();
#endif
	if( aResult.IsOK() )
	{
		CreateThread(aDisplayWnd);
		m_bStreamCreated = true;
		m_Preview = aDisplayWnd;
	}
	return aResult; 
}

StResult StCustomDevice::StopStreams(void)
{
	DeleteThread();
#ifdef _PLEORA
	if( mPipeline.IsStarted() )
	{
		mPipeline.Stop();
	}

#else
	DeleteStream();
#endif
	m_bStreamCreated = false;
	m_Preview = NULL;
	return StResult::Code::OK;
}

//StResult StCustomDevice::CreateThread(CPreview *Preview)
StResult StCustomDevice::CreateThread(void *Preview)
{
	StResult aResult = StResult::Code::NOT_INITIALIZED;
	BOOL bReval=FALSE;
	if( m_lThreadDisplay==NULL )
	{
		aResult = StResult::Code::OK;
		StInt32 iNumStream = 1;
#ifdef _PLEORA
#else
		iNumStream = GetNumStream();
#endif
		for( int i=0; i<iNumStream && i<1; i++ )	//１つだけ------------------------
		{
#ifdef _PLEORA
			//PvStream *pStream = &mStream;
			PvPipeline *pStream = &mPipeline;
#else
			StGenStream *pStream = GetGenStream(i);
#endif
			if( pStream )
			{
				CaptureThread *lThreadDisplay = new CaptureThread( Preview, (void*)pStream );
				SetCaptureThread(lThreadDisplay);
				//// Start threads
				lThreadDisplay->Start();
				lThreadDisplay->SetPriority( THREAD_PRIORITY_ABOVE_NORMAL );
			}
			else
			{
				aResult = StResult::Code::NOT_INITIALIZED;
			}
		}
	}
	return aResult;
}

void StCustomDevice::DeleteThread(void)
{
	if( m_lThreadDisplay ){

		m_lThreadDisplay->Stop();
		m_lThreadDisplay->IsDone(INFINITE);
		//Sleep(1000);
		delete m_lThreadDisplay;
		m_lThreadDisplay = NULL;
	}
}
StResult StCustomDevice::StartAcquisition(void)
{
	StResult lResult = StResult::Code::NOT_STARTACQUISITION;
	if( !IsStreamCreated() ) return lResult;

	//Buffer作成
	int lPayloadSizeValue = 0;
	bool bReval = GetPayloadSize( lPayloadSizeValue );
	if( !bReval ) return lResult;
#ifdef _PLEORA
	mPipeline.SetBufferSize(lPayloadSizeValue);
    mPipeline.Reset();
    mStream.GetParameters()->ExecuteCommand( "Reset" );

	mAcquisitionStateManager->Start();

	//PvResult aResult = mStream.Open();
	//lResult = aResult.GetCode();
#else
	if( GetNumStream()==0 )	return lResult;
	StGenStream * aStream = GetGenStream(0);
	aStream->SetBufferCount(16);
	lResult = StResult::Code::NOT_STARTACQUISITION;
	aStream->SetBufferSize(lPayloadSizeValue);
	lResult = aStream->StartStream();
	if( lResult.IsOK() ){

		//m_lThreadDisplay->Start();
		//m_lThreadDisplay->SetPriority( THREAD_PRIORITY_ABOVE_NORMAL );

		TLParamsLocked(true);
		//lResult = StartAcquisition();
		bReval = SetGenCommandParameter( "AcquisitionStart" );
		if( bReval )
		{
			lResult = StResult::Code::OK;
		}
		else
		{
			TLParamsLocked(false);
		}
	}
#endif

	//StartAcquisition
	if( lResult.IsOK() )
	{
		m_bStarted = true;
	}

	return lResult;

}

StResult StCustomDevice::StopAcquisition(void)
{
	if( !IsStarted() ) return StResult::Code::OK;

	StResult lResult = StResult::Code::GENERIC_ERROR;

	bool bReval = SetGenCommandParameter( "AcquisitionStop" );

	if( bReval ) 
	{
		TLParamsLocked(false);
		m_bStarted = false;
		Sleep(1000);	//とりあえず？StopStream中に画像データが転送してきてバッファ解放と処理が重なってしまうためエラーが起きる

#ifdef _PLEORA

		mStream->
#else
		if( GetNumStream()==0 )	return lResult;
		StGenStream * aStream = GetGenStream(0);
		lResult = aStream->StopStream();
#endif
						//どこで解放処理をおこなうか？
		//m_lThreadDisplay->Stop();
		//m_lThreadDisplay->IsDone(INFINITE);

	}

	//if( bReval ) lResult = StResult::Code::OK;

	return lResult;

}

BOOL StCustomDevice::InitCameraSetting(CCameraType* aCameraType)
{
	BOOL bReval = TRUE;
	//UserSetLoadを行う。
	if( bReval )
	{
		StString aString("Default");
		bReval = SetGenEnumParameter("UserSetSelector",aString);
	}
	if( bReval )
	{
		bReval = SetGenCommandParameter("UserSetLoad");
	}


	//▼1.0.0.21
	if( bReval )
		bReval = GetGenFloatParameter("BlackLevel",m_dblDefBlackLevel);
	if( bReval )
		bReval = SetGenFloatParameter("BlackLevel",aCameraType->GetCommonBlackLevel());
	//▲1.0.0.21

	//AcquisitionFrameRateよりExposureTimeが優先される........
	if( bReval )
		bReval = GetGenEnumParameter("ExposureMode",m_szDefExposureMode);
	if( bReval )
		bReval = SetGenEnumParameter("ExposureMode","Timed");

	if( bReval )
		bReval = GetGenEnumParameter("PixelFormat",m_szDefPixelFormat);
	if( bReval )
		bReval = SetGenEnumParameter("PixelFormat",aCameraType->GetCommonPixelFormat());

	if( bReval )
		bReval = GetGenFloatParameter("ExposureTime",m_dblDefExposureTime);
	if( bReval )
		bReval = SetGenFloatParameter("ExposureTime",aCameraType->GetCommonExposureTime());

	if( bReval )
		bReval = GetGenFloatParameter("AcquisitionFrameRate",m_dblDefAcquisitionFramerate);
	if( bReval )
		bReval = SetGenFloatParameter("AcquisitionFrameRate",aCameraType->GetCommonAcquisitionFrameRate());

	//管理者モードにする
	if( bReval )
	{
		//int iData = 0x12345678;
		//bReval = SetIntegerValue( &iData, _T("TechnologyAgnosticDoNotSaveAddress"), 0x0C, 0, 31 );
		bReval = SetAccessMode(1);
		//int aData = 0;
		//bReval = GetIntegerValue( &aData, _T("TechnologyAgnosticDoNotSave"), 0x0C, 0, 31 );
	}

	return bReval;
}

BOOL StCustomDevice::DefalutCameraSetting(void)
{
	BOOL bReval;
	//管理者モード解除
	//int iData = 0;
	//bReval = SetIntegerValue( &iData, _T("TechnologyAgnosticDoNotSaveAddress"), 0x0C, 0, 31 );
	bReval = SetAccessMode(0);

	if( bReval )
		bReval = SetGenFloatParameter("AcquisitionFrameRate",m_dblDefAcquisitionFramerate);
	if( bReval )
		bReval = SetGenFloatParameter("ExposureTime",m_dblDefExposureTime);
	if( bReval )
		bReval = SetGenEnumParameter("PixelFormat",m_szDefPixelFormat);
	if( bReval )
		bReval = SetGenEnumParameter("ExposureMode",m_szDefExposureMode);
	m_szDefPixelFormat = "";
	m_szDefExposureMode = "";
	m_dblDefAcquisitionFramerate = 0.0;
	m_dblDefExposureTime = 0.0;
	//▼1.0.0.21
	m_dblDefBlackLevel = -1.0;
	//▲1.0.0.21
	return bReval;
}



//---SetDeviceSerialNumber--------------------------------------------------------
BOOL StCustomDevice::SetDeviceSerialNumber( const char *pData  )
{

	DWORD dwAddress=GetAddress(m_szDeviceSerialNumberBaseName, m_dwDeviceSerialNumberOffset);
	DWORD dwLength=m_dwDeviceSerialNumberSize;
	if( strlen(pData)>=dwLength ) return FALSE;
	char *pBuffer = new char [dwLength];
	if( !pBuffer ) return FALSE;

	memset(pBuffer,0,dwLength);
	strcpy(pBuffer,pData);
	BOOL bReval = Write(dwAddress, pBuffer, dwLength );
	delete [] pBuffer;
	return bReval;
}

//---GetDeviceSerialNumber--------------------------------------------------------
BOOL StCustomDevice::GetDeviceSerialNumber( char *pData, int iSize  )
{

	DWORD dwAddress=GetAddress(m_szDeviceSerialNumberBaseName, m_dwDeviceSerialNumberOffset);
	DWORD dwLength=m_dwDeviceSerialNumberSize;
	char *pBuffer = new char [dwLength+1];
	if( !pBuffer ) return FALSE;
	memset(pBuffer,0,dwLength);
	BOOL bReval = Read(dwAddress, pBuffer, dwLength );
	if( bReval )
	{
		if( iSize>strlen(pBuffer) )
			strcpy_s(pData,iSize,pBuffer);
		else
		{
			bReval = FALSE;
			//memcpy(pData,pBuffer,iSize);
		}
	}
	delete [] pBuffer;
	return bReval;
}

//---SetDeviceModelName--------------------------------------------------------
BOOL StCustomDevice::SetDeviceModelName( const char *pData )
{

	DWORD dwAddress=GetAddress(m_szDeviceModelNameBaseName, m_dwDeviceModelNameOffset);
	DWORD dwLength=m_dwDeviceModelNameSize;
	if( strlen(pData)>=dwLength ) return FALSE;
	char *pBuffer = new char [dwLength];
	if( !pBuffer ) return FALSE;

	memset(pBuffer,0,dwLength);
	strcpy(pBuffer,pData);
	BOOL bReval = Write(dwAddress, pBuffer, dwLength );
	delete [] pBuffer;
	return bReval;
}

//---GetDeviceModelName--------------------------------------------------------
BOOL StCustomDevice::GetDeviceModelName( char *pData, size_t bufSize  )
{

	DWORD dwAddress=GetAddress(m_szDeviceModelNameBaseName, m_dwDeviceModelNameOffset);
	DWORD dwLength=m_dwDeviceModelNameSize;
	char *pBuffer = new char [dwLength+1];
	if( !pBuffer ) return FALSE;
	memset(pBuffer,0,dwLength);
	BOOL bReval = Read(dwAddress, pBuffer, dwLength );
	if( bReval )
	{
		if( bufSize>strlen(pData) )
			strcpy_s(pData,bufSize,pBuffer);
		else
		{
			bReval = FALSE;
			//memcpy(pData,pBuffer,bufSize);
		}
	}
	delete [] pBuffer;
	return bReval;
}


//---SetDeviceVendorName--------------------------------------------------------
BOOL StCustomDevice::SetDeviceVendorName( const char *pData )
{

	DWORD dwAddress=GetAddress(m_szDeviceVendorNameBaseName, m_dwDeviceVendorNameOffset);
	DWORD dwLength=m_dwDeviceVendorNameSize;
	if( strlen(pData)>=dwLength ) return FALSE;
	char *pBuffer = new char [dwLength];
	if( !pBuffer ) return FALSE;

	memset(pBuffer,0,dwLength);
	strcpy(pBuffer,pData);
	BOOL bReval = Write(dwAddress, pBuffer, dwLength );
	delete [] pBuffer;
	return bReval;
}

//---GetDeviceVendorName--------------------------------------------------------
BOOL StCustomDevice::GetDeviceVendorName( char *pData, size_t bufSize  )
{

	DWORD dwAddress=GetAddress(m_szDeviceVendorNameBaseName, m_dwDeviceVendorNameOffset);
	DWORD dwLength=m_dwDeviceVendorNameSize;
	char *pBuffer = new char [dwLength+1];
	if( !pBuffer ) return FALSE;
	memset(pBuffer,0,dwLength);
	BOOL bReval = Read(dwAddress, pBuffer, dwLength );
	if( bReval )
	{
		if( bufSize>strlen(pData) )
			strcpy_s(pData,bufSize,pBuffer);
		else
		{
			bReval = FALSE;
			//memcpy(pData,pBuffer,bufSize);
		}
	}
	delete [] pBuffer;
	return bReval;
}


//---SetDeviceManufacturerInfo--------------------------------------------------------
BOOL StCustomDevice::SetDeviceManufacturerInfo( const char *pData )
{

	DWORD dwAddress=GetAddress(m_szDeviceManufacturerInfoBaseName, m_dwDeviceManufacturerInfoOffset);
	DWORD dwLength=m_dwDeviceManufacturerInfoSize;
	if( strlen(pData)>=dwLength ) return FALSE;
	char *pBuffer = new char [dwLength];
	if( !pBuffer ) return FALSE;

	memset(pBuffer,0,dwLength);
	strcpy(pBuffer,pData);
	BOOL bReval = Write(dwAddress, pBuffer, dwLength );
	delete [] pBuffer;
	return bReval;
}

//---GetDeviceManufacturerInfo--------------------------------------------------------
BOOL StCustomDevice::GetDeviceManufacturerInfo( char *pData, size_t bufSize  )
{

	DWORD dwAddress=GetAddress(m_szDeviceManufacturerInfoBaseName, m_dwDeviceManufacturerInfoOffset);
	DWORD dwLength=m_dwDeviceManufacturerInfoSize;
	char *pBuffer = new char [dwLength+1];
	if( !pBuffer ) return FALSE;
	memset(pBuffer,0,dwLength);
	BOOL bReval = Read(dwAddress, pBuffer, dwLength );
	if( bReval )
	{
		if( bufSize>strlen(pData) )
			strcpy_s(pData,bufSize,pBuffer);
		else
		{
			bReval = FALSE;
			//memcpy(pData,pBuffer,bufSize);
		}
	}
	delete [] pBuffer;
	return bReval;
}


//-----------------------------------------------------------


BOOL StCustomDevice::GetIntegerValue( int *pData, LPCTSTR szName, DWORD offset, INT iLSB, INT iMSB )
{
	BOOL bReval=FALSE;

	DWORD dwMask = 0xFFFFFFFF;
	//if( iLSB>=0 && iMSB>=0 && iMSB>=iLSB )
	//	dwMask = ((1<<(iMSB-iLSB+1))-1)<<iLSB;
	if( iLSB>=0 && iMSB>=0 && iMSB>=iLSB )
	{
		dwMask=0;
		for( int i=iLSB; i<=iMSB; i++ )
			dwMask |= (1<<i);
	}
	INT iShift = iLSB<0?0:iLSB;
	DWORD dwAddress=GetAddress(szName, offset);
	if( dwAddress )
	{
		DWORD dwData;
		bReval = Read(dwAddress, (char *)&dwData, sizeof(dwData) );
		if( bReval )
		{
			if( m_dwEndian!=0 ) dwData = htonl(dwData);
			*pData = (dwData&dwMask)>>iShift;
		}
	}
	return bReval;
}

BOOL StCustomDevice::SetIntegerValue( int *pData, LPCTSTR szName, DWORD offset, INT iLSB, INT iMSB )
{
	BOOL bReval=FALSE;

	DWORD dwMask = 0xFFFFFFFF;
	//if( iLSB>=0 && iMSB>=0 && iMSB>=iLSB )
	//	dwMask = ((1<<(iMSB-iLSB+1))-1)<<iLSB;

	if( iLSB>=0 && iMSB>=0 && iMSB>=iLSB )
	{
		dwMask=0;
		for( int i=iLSB; i<=iMSB; i++ )
			dwMask |= (1<<i);
	}

	INT iShift = iLSB<0?0:iLSB;
	DWORD dwAddress=GetAddress(szName, offset);
	do
	{
		if( dwAddress )
		{
			DWORD dwData=0;
			if( dwMask != 0xFFFFFFFF )
			{
				bReval = Read(dwAddress, (char *)&dwData, sizeof(dwData) );
				if( !bReval ) break;
				if( m_dwEndian!=0 ) dwData = htonl(dwData);
				dwData &= ~dwMask;
			}
			dwData |= ((*pData)<<iShift)&dwMask;
			if( m_dwEndian!=0 ) dwData = htonl(dwData);
			bReval = Write(dwAddress, (char *)&dwData, sizeof(dwData) );
		}
	}while(0);
	return bReval;
}


BOOL StCustomDevice::GetAccessMode( int *pData )
{
	return GetIntegerValue( pData, m_szAccessModeBaseName, m_dwAccessModeOffset, m_iAccessModeLSB, m_iAccessModeMSB );
}

BOOL StCustomDevice::SetAccessMode( int iData )
{
	//int iData = *pData;
	if( iData!=0 ) iData = m_iAccessModePasscode;
	return SetIntegerValue( &iData, m_szAccessModeBaseName, m_dwAccessModeOffset, m_iAccessModeLSB, m_iAccessModeMSB );
}

BOOL StCustomDevice::GetLinkConfiguration2LaneLoad( int *pData )
{
	return GetIntegerValue( pData, m_szLinkConfiguration2LaneLoadBaseName, m_dwLinkConfiguration2LaneLoadOffset, m_iLinkConfiguration2LaneLoadLSB, m_iLinkConfiguration2LaneLoadMSB );
}

INT StCustomDevice::GetLinkConfiguration2LaneLoad()
{
	INT iData = -1;
	GetLinkConfiguration2LaneLoad( &iData );
	return iData;
}

BOOL StCustomDevice::SetLinkConfiguration2LaneLoad( int iData )
{
	//int iData = *pData;
	if( iData!=0 ) iData = m_iLinkConfiguration2LaneLoadPasscode;
	return SetIntegerValue( &iData, m_szLinkConfiguration2LaneLoadBaseName, m_dwLinkConfiguration2LaneLoadOffset, m_iLinkConfiguration2LaneLoadLSB, m_iLinkConfiguration2LaneLoadMSB );
}


BOOL StCustomDevice::GetOffsetDigitalGain( int *pData, int iMode )
{
	//dwFPGAAdjustmentAddress + 0x0004 範囲0~255
	//_tcscpy_s(m_szOffsetDigitalGainBaseName,_countof(m_szOffsetDigitalGainBaseName), _T("FPGAAdjustmentAddress"));
	//m_dwOffsetDigitalGainOffset = 0x0004;
	//m_iOffsetDigitalGainLSB = 0;
	//m_iOffsetDigitalGainMSB = 7;
	//m_dwEndian = 1; //BigEndian
	return GetIntegerValue( pData, m_szOffsetDigitalGainBaseName, m_dwOffsetDigitalGainOffset[iMode], m_iOffsetDigitalGainLSB[iMode], m_iOffsetDigitalGainMSB[iMode] );
}

BOOL StCustomDevice::SetOffsetDigitalGain( int *pData, int iMode )
{
	//dwFPGAAdjustmentAddress + 0x0004 範囲0~255
	//_tcscpy_s(m_szOffsetDigitalGainBaseName,_countof(m_szOffsetDigitalGainBaseName), _T("FPGAAdjustmentAddress"));
	//m_dwOffsetDigitalGainOffset = 0x0004;
	//m_iOffsetDigitalGainLSB = 0;
	//m_iOffsetDigitalGainMSB = 7;
	//m_dwEndian = 1; //BigEndian
	return SetIntegerValue( pData, m_szOffsetDigitalGainBaseName, m_dwOffsetDigitalGainOffset[iMode], m_iOffsetDigitalGainLSB[iMode], m_iOffsetDigitalGainMSB[iMode] );
}

BOOL StCustomDevice::GetVerticalFixedPatternCorrectionMode( int *pData )
{
	//dwFPGAAdjustmentAddress + 0x0020
	//_tcscpy_s(m_szVerticalFixedPatternCorrectionModeBaseName,_countof(m_szVerticalFixedPatternCorrectionModeBaseName), _T("FPGAAdjustmentAddress"));
	//m_dwVerticalFixedPatternCorrectionModeOffset = 0x0020;
	//m_iVerticalFixedPatternCorrectionModeLSB = 0;
	//m_iVerticalFixedPatternCorrectionModeMSB = 0;
	//m_dwEndian = 1; //BigEndian
	return GetIntegerValue( pData, m_szVerticalFixedPatternCorrectionModeBaseName, m_dwVerticalFixedPatternCorrectionModeOffset, m_iVerticalFixedPatternCorrectionModeLSB, m_iVerticalFixedPatternCorrectionModeMSB );
}

BOOL StCustomDevice::SetVerticalFixedPatternCorrectionMode( int *pData )
{
	//dwFPGAAdjustmentAddress + 0x0020
	//_tcscpy_s(m_szVerticalFixedPatternCorrectionModeBaseName,_countof(m_szVerticalFixedPatternCorrectionModeBaseName), _T("FPGAAdjustmentAddress"));
	//m_dwVerticalFixedPatternCorrectionModeOffset = 0x0020;
	//m_iVerticalFixedPatternCorrectionModeLSB = 0;
	//m_iVerticalFixedPatternCorrectionModeMSB = 0;
	//m_dwEndian = 1; //BigEndian
	return SetIntegerValue( pData, m_szVerticalFixedPatternCorrectionModeBaseName, m_dwVerticalFixedPatternCorrectionModeOffset, m_iVerticalFixedPatternCorrectionModeLSB, m_iVerticalFixedPatternCorrectionModeMSB );
}

BOOL StCustomDevice::GetVerticalFixedGainCorrectionMode( int *pData )
{
	return GetIntegerValue( pData, m_szVerticalFixedGainCorrectionModeBaseName, m_dwVerticalFixedGainCorrectionModeOffset, m_iVerticalFixedGainCorrectionModeLSB, m_iVerticalFixedGainCorrectionModeMSB );
}

BOOL StCustomDevice::SetVerticalFixedGainCorrectionMode( int *pData )
{
	return SetIntegerValue( pData, m_szVerticalFixedGainCorrectionModeBaseName, m_dwVerticalFixedGainCorrectionModeOffset, m_iVerticalFixedGainCorrectionModeLSB, m_iVerticalFixedGainCorrectionModeMSB );
}


BOOL StCustomDevice::GetClampLevel( int *pData, int iMode, int iLine )
{
	//dwSensorAdjustmentAddress + 0x0008 + iMode * 12
	//_tcscpy_s(m_szClampLevelBaseName,_countof(m_szClampLevelBaseName), _T("SensorAdjustmentAddress"));
	//m_dwClampLevelOffset[iMode] = 0x0014;	//iMode==1の時0x0014　２Lane用
	//m_iClampLevelLSB[iMode] = 0;
	//m_iClampLevelMSB[iMode] = 7;
	//m_dwEndian = 1; //BigEndian
	return GetIntegerValue( pData, m_szClampLevelBaseName, m_dwClampLevelOffset[iMode][iLine], m_iClampLevelLSB[iMode][iLine], m_iClampLevelMSB[iMode][iLine] );
}
BOOL StCustomDevice::SetClampLevel( int *pData, int iMode, int iLine )
{
	//dwSensorAdjustmentAddress + 0x0008 + iMode * 12
	//_tcscpy_s(m_szClampLevelBaseName,_countof(m_szClampLevelBaseName), _T("SensorAdjustmentAddress"));
	//m_dwClampLevelOffset[iMode] = 0x0014;	//iMode==1の時0x0014　２Lane用
	//m_iClampLevelLSB[iMode] = 0;
	//m_iClampLevelMSB[iMode] = 7;
	//m_dwEndian = 1; //BigEndian
	return SetIntegerValue( pData, m_szClampLevelBaseName, m_dwClampLevelOffset[iMode][iLine], m_iClampLevelLSB[iMode][iLine], m_iClampLevelMSB[iMode][iLine] );
}

BOOL StCustomDevice::GetDefectiveSymptomAdjustment( int *pData, int iMode, int iLine )
{
	//dwSensorAdjustmentAddress + 0x0008 + iMode * 12
	//_tcscpy_s(m_szClampLevelBaseName,_countof(m_szClampLevelBaseName), _T("SensorAdjustmentAddress"));
	//m_dwClampLevelOffset[iMode] = 0x0014;	//iMode==1の時0x0014　２Lane用
	//m_iClampLevelLSB[iMode] = 0;
	//m_iClampLevelMSB[iMode] = 7;
	//m_dwEndian = 1; //BigEndian
	return GetIntegerValue( pData, m_szClampLevelBaseName, m_dwClampLevelOffset[0][iMode], m_iClampLevelLSB[0][iMode], m_iClampLevelMSB[0][iMode] );
}
BOOL StCustomDevice::SetDefectiveSymptomAdjustment( int *pData, int iMode, int iLine )
{
	//dwSensorAdjustmentAddress + 0x0008 + iMode * 12
	//_tcscpy_s(m_szClampLevelBaseName,_countof(m_szClampLevelBaseName), _T("SensorAdjustmentAddress"));
	//m_dwClampLevelOffset[iMode] = 0x0014;	//iMode==1の時0x0014　２Lane用
	//m_iClampLevelLSB[iMode] = 0;
	//m_iClampLevelMSB[iMode] = 7;
	//m_dwEndian = 1; //BigEndian
	return SetIntegerValue( pData, m_szDefectiveSymptomAdjustmentBaseName, m_dwDefectiveSymptomAdjustmentOffset[iMode][iLine], m_iDefectiveSymptomAdjustmentLSB[iMode][iLine], m_iDefectiveSymptomAdjustmentMSB[iMode][iLine] );
}

BOOL StCustomDevice::GetAdjustLevelGain( int *pData, int iMode )
{
	return GetIntegerValue( pData, m_szADCGainBaseName, m_dwADCGainOffset[iMode], m_iADCGainLSB[iMode], m_iADCGainMSB[iMode] );
}
BOOL StCustomDevice::SetAdjustLevelGain( int *pData, int iMode )
{
	return SetIntegerValue( pData, m_szADCGainBaseName, m_dwADCGainOffset[iMode], m_iADCGainLSB[iMode], m_iADCGainMSB[iMode] );
}

BOOL StCustomDevice::GetPixelOffsetForTable( int *pData )
{
	BOOL bReval=FALSE;









	return bReval;
}

BOOL StCustomDevice::GetPixelCorrectMaxCount( int *pData )
{
	//m_szPixelCorrectMaxBaseName="VendorUniqueControlAddress";
	//m_dwPixelCorrectMaxOffset = 8;
	//m_iPixelCorrectMaxLSB = 0;
	//m_iPixelCorrectMaxMSB = 0;
	return GetIntegerValue( pData, m_szPixelCorrectMaxBaseName, m_dwPixelCorrectMaxOffset, m_iPixelCorrectMaxLSB, m_iPixelCorrectMaxMSB );
}

BOOL SwapByte( PBYTE pSrc, PBYTE pDst, size_t bufSize )
{
	if( bufSize&0x3 ) return FALSE;
	int loop = bufSize>>2;
	BYTE byteTmp[4];
	for( int i=0; i<loop; i++, pSrc+=4, pDst+=4 )
	{
		memcpy( byteTmp, pSrc, 4 );
		pDst[3] = byteTmp[0]; 
		pDst[2] = byteTmp[1]; 
		pDst[1] = byteTmp[2]; 
		pDst[0] = byteTmp[3]; 
	}
	return TRUE;
}

//
BOOL StCustomDevice::SetColumnBlackLevelAdjustment( int nMode, int nTableSize, PBYTE pbyteTable )
{
	BOOL bReval=FALSE;
	DWORD WriteMaxSize = GetControlPacketSizeMax();
	if( WriteMaxSize==0 ) return FALSE;
	DWORD TopAddress=GetAddress(m_szColumnBlackLevelAdjustmentBaseName, m_dwColumnBlackLevelAdjustmentOffset[nMode]);
	if( m_dwEndian==0 )
	{
		do
		{
			DWORD WriteSize = nTableSize>WriteMaxSize?WriteMaxSize:nTableSize;
			bReval = Write(TopAddress, (char *)pbyteTable, WriteSize );
			TopAddress += WriteSize;
			pbyteTable += WriteSize;
			nTableSize -= WriteSize;
		}while(nTableSize>0 && bReval);
	}
	else
	{
		PBYTE pData = new BYTE [WriteMaxSize];
		if( !pData ) return FALSE;
		do
		{
			DWORD WriteSize = nTableSize>WriteMaxSize?WriteMaxSize:nTableSize;

			bReval = SwapByte( pbyteTable, pData, WriteSize );
			if( !bReval ) break;
			
			bReval = Write(TopAddress, (char *)pData, WriteSize );
			TopAddress += WriteSize;
			pbyteTable += WriteSize;
			nTableSize -= WriteSize;
		}while(nTableSize>0 && bReval);
		delete [] pData;
	}

	return bReval;
}

BOOL StCustomDevice::SetColumnGainAdjustment( int nMode, int nGainTableSize, PBYTE pbyteGainTable )
{
	BOOL bReval=FALSE;
	DWORD WriteMaxSize = GetControlPacketSizeMax();
	if( WriteMaxSize==0 ) return FALSE;
	DWORD TopAddress=GetAddress(m_szColumnGainAdjustmentBaseName, m_dwColumnGainAdjustmentOffset[nMode]);
	if( m_dwEndian==0 )
	{
		do
		{
			DWORD WriteSize = nGainTableSize>WriteMaxSize?WriteMaxSize:nGainTableSize;
			bReval = Write(TopAddress, (char *)pbyteGainTable, WriteSize );
			TopAddress += WriteSize;
			pbyteGainTable += WriteSize;
			nGainTableSize -= WriteSize;
		}while(nGainTableSize>0 && bReval);
	}

	else
	{
		PBYTE pData = new BYTE [WriteMaxSize];
		if( !pData ) return FALSE;
		do
		{
			DWORD WriteSize = nGainTableSize>WriteMaxSize?WriteMaxSize:nGainTableSize;

			bReval = SwapByte( pbyteGainTable, pData, WriteSize );
			if( !bReval ) break;

//memset(pData,0,WriteSize );

			bReval = Write(TopAddress, (char *)pData, WriteSize );

//TRACE(_T(" @@@@@@@@@@@@@@@@@TopAddress=0x%08x\n"), TopAddress);
//TRACE(_T(" @@@@@@@@@@@@@@@@@pData1=%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:\n"), pbyteGainTable[0], pbyteGainTable[1], pbyteGainTable[2], pbyteGainTable[3], pbyteGainTable[4], pbyteGainTable[5], pbyteGainTable[6], pbyteGainTable[7], pbyteGainTable[8], pbyteGainTable[9], pbyteGainTable[10], pbyteGainTable[11]);
//TRACE(_T(" @@@@@@@@@@@@@@@@@pData2=%02x:%02x:%02x:%02x\n"), pbyteGainTable[9], pbyteGainTable[50], pbyteGainTable[52], pbyteGainTable[54]);
			TopAddress += WriteSize;
			pbyteGainTable += WriteSize;
			nGainTableSize -= WriteSize;
		}while(nGainTableSize>0 && bReval);

		delete [] pData;
		
	}

	return bReval;
}

BOOL StCustomDevice::SetPixelCorrectPosition( int nMode, int nTableSize, PBYTE pbyteTable )
{
	BOOL bReval=FALSE;
	DWORD WriteMaxSize = GetControlPacketSizeMax();
	if( WriteMaxSize==0 ) return FALSE;
	DWORD TopAddress=GetAddress(m_szPixelCorrectBaseName, m_dwPixelCorrectOffset);
	if( m_dwEndian==0 )
	{
		do
		{
			DWORD WriteSize = nTableSize>WriteMaxSize?WriteMaxSize:nTableSize;
			bReval = Write(TopAddress, (char *)pbyteTable, WriteSize );
			TopAddress += WriteSize;
			pbyteTable += WriteSize;
			nTableSize -= WriteSize;
		}while(nTableSize>0 && bReval);
	}

	else
	{
		PBYTE pData = new BYTE [WriteMaxSize];
		if( !pData ) return FALSE;
		do
		{
			DWORD WriteSize = nTableSize>WriteMaxSize?WriteMaxSize:nTableSize;

			bReval = SwapByte( pbyteTable, pData, WriteSize );
			if( !bReval ) break;

//memset(pData,0,WriteSize );

			bReval = Write(TopAddress, (char *)pData, WriteSize );

//TRACE(_T(" @@@@@@@@@@@@@@@@@TopAddress=0x%08x\n"), TopAddress);
//TRACE(_T(" @@@@@@@@@@@@@@@@@pData1=%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:\n"), pbyteGainTable[0], pbyteGainTable[1], pbyteGainTable[2], pbyteGainTable[3], pbyteGainTable[4], pbyteGainTable[5], pbyteGainTable[6], pbyteGainTable[7], pbyteGainTable[8], pbyteGainTable[9], pbyteGainTable[10], pbyteGainTable[11]);
//TRACE(_T(" @@@@@@@@@@@@@@@@@pData2=%02x:%02x:%02x:%02x\n"), pbyteGainTable[9], pbyteGainTable[50], pbyteGainTable[52], pbyteGainTable[54]);
			TopAddress += WriteSize;
			pbyteTable += WriteSize;
			nTableSize -= WriteSize;
		}while(nTableSize>0 && bReval);

		delete [] pData;
		
	}

	return bReval;
}

//画素欠補正保存
BOOL StCustomDevice::SavePixelCorrectPosition( void )
{
	DWORD TopAddress=GetAddress(m_szSavePixelCorrectBaseName, m_dwSavePixelCorrectOffset);

	INT iLSB = m_iSavePixelCorrectLSB;
	INT iMSB = m_iSavePixelCorrectMSB;
	DWORD dwMask = 0xFFFFFFFF;
	if( iLSB>=0 && iMSB>=0 && iMSB>=iLSB )
	{
		dwMask=0;
		for( int i=iLSB; i<=iMSB; i++ )
			dwMask |= (1<<i);
	}
	INT iShift = iLSB<0?0:iLSB;
		
	DWORD dwData = (m_iSavePixelCorrectValue<<iShift)&dwMask;
	DWORD WriteSize = sizeof(dwData);
	if( m_dwEndian!=0 )
	{
		SwapByte( (PBYTE)&dwData, (PBYTE)&dwData, WriteSize );
	}
	BOOL bReval = Write(TopAddress, (char *)&dwData, WriteSize );
	return bReval;
}

//------------------------------------------------------
#define BASE_ADDRESS 0x01000000
DWORD StCustomDevice::GetCategoryAddress(DWORD categoryOffset)
{
	DWORD dwAddress=0;

	DWORD dwData=0;
	bool bReval = Read(BASE_ADDRESS + categoryOffset, (char *)&dwData, sizeof(dwData) );
	if( bReval )
	{
		if( m_dwEndian!=0 )
			dwData = htonl(dwData);
		if( dwData&0x01000000 )
		{
			dwAddress = BASE_ADDRESS + (dwData&0x00FFFFFF);
		}
	}
	return dwAddress;
}

DWORD StCustomDevice::GetTechnologyAgnosticFunctionSaveAddress()
{
	return GetCategoryAddress(0x0074);
}
DWORD StCustomDevice::GetTechnologyAgnosticDoNotSaveAddress()
{
	return GetCategoryAddress(0x0078);
}
DWORD StCustomDevice::GetTechnologyAgnosticCameraAdministratorAddress()
{
	return GetCategoryAddress(0x0084);
}
DWORD StCustomDevice::GetTechnologyAgnosticCameraFeatureAddress()
{
	return GetCategoryAddress(0x0080);
}
//▼1.0.0.22
DWORD StCustomDevice::GetTechnologyAgnosticCameraStatusAddress()
{
	return GetCategoryAddress(0x007C);
}
//▲1.0.0.22
DWORD StCustomDevice::GetFPGAAdjustmentAddress()
{
	return GetCategoryAddress(0x0060);
}
DWORD StCustomDevice::GetSensorAdjustmentAddress()
{
	return GetCategoryAddress(0x0064);
}
DWORD StCustomDevice::GetVendorUniqueControlAddress()
{
	return GetCategoryAddress(0x0048);
}
DWORD StCustomDevice::GetCameraFeatureAddress()
{
	return GetCategoryAddress(0x0080);
}
DWORD StCustomDevice::GetDefectivePixelCorrectionAddress()
{
	return GetCategoryAddress(0x004C);
}


DWORD StCustomDevice::GetAddress(LPCTSTR szAddressName, int offsetAddress)
{
	DWORD dwAddress=0;
	if( _tcscmp(szAddressName, _T("TechnologyAgnosticDoNotSaveAddress"))==0 )
	{
		if( m_dwTechnologyAgnosticDoNotSaveAddress==0 )
		{
			m_dwTechnologyAgnosticDoNotSaveAddress = GetTechnologyAgnosticDoNotSaveAddress();
		}
		if( m_dwTechnologyAgnosticDoNotSaveAddress )
		{
			dwAddress = m_dwTechnologyAgnosticDoNotSaveAddress + offsetAddress;
		}
	}
	else
	if( _tcscmp(szAddressName, _T("TechnologyAgnosticFunctionSaveAddress"))==0 )
	{
		if( m_dwTechnologyAgnosticFunctionSaveAddress==0 )
		{
			m_dwTechnologyAgnosticFunctionSaveAddress = GetTechnologyAgnosticFunctionSaveAddress();
		}
		if( m_dwTechnologyAgnosticFunctionSaveAddress )
		{
			dwAddress = m_dwTechnologyAgnosticFunctionSaveAddress + offsetAddress;
		}
	}
	else
	if( _tcscmp(szAddressName, _T("TechnologyAgnosticCameraAdministratorAddress"))==0 )
	{
		if( m_dwTechnologyAgnosticCameraAdministratorAddress==0 )
		{
			m_dwTechnologyAgnosticCameraAdministratorAddress = GetTechnologyAgnosticCameraAdministratorAddress();
		}
		if( m_dwTechnologyAgnosticCameraAdministratorAddress )
		{
			dwAddress = m_dwTechnologyAgnosticCameraAdministratorAddress + offsetAddress;
		}
	}
	else
	if( _tcscmp(szAddressName, _T("TechnologyAgnosticCameraFeatureAddress"))==0 )
	{
		if( m_dwTechnologyAgnosticCameraFeatureAddress==0 )
		{
			m_dwTechnologyAgnosticCameraFeatureAddress = GetTechnologyAgnosticCameraFeatureAddress();
		}
		if( m_dwTechnologyAgnosticCameraFeatureAddress )
		{
			dwAddress = m_dwTechnologyAgnosticCameraFeatureAddress + offsetAddress;
		}
	}

	//▼1.0.0.22
	else
	if( _tcscmp(szAddressName, _T("TechnologyAgnosticCameraStatusAddress"))==0 )
	{
		if( m_dwTechnologyAgnosticCameraStatusAddress==0 )
		{
			m_dwTechnologyAgnosticCameraStatusAddress = GetTechnologyAgnosticCameraStatusAddress();
		}
		if( m_dwTechnologyAgnosticCameraStatusAddress )
		{
			dwAddress = m_dwTechnologyAgnosticCameraStatusAddress + offsetAddress;
		}
	}
	//▲1.0.0.22

	else
	if( _tcscmp(szAddressName, _T("FPGAAdjustmentAddress"))==0 )
	{
		if( m_dwFPGAAdjustmentAddress==0 )
		{
			m_dwFPGAAdjustmentAddress = GetFPGAAdjustmentAddress();
		}
		if( m_dwFPGAAdjustmentAddress )
		{
			dwAddress = m_dwFPGAAdjustmentAddress + offsetAddress;
		}
	}
	else
	if( _tcscmp(szAddressName, _T("SensorAdjustmentAddress"))==0 )
	{
		if( m_dwSensorAdjustmentAddress==0 )
		{
			m_dwSensorAdjustmentAddress = GetSensorAdjustmentAddress();
		}
		if( m_dwSensorAdjustmentAddress )
		{
			dwAddress = m_dwSensorAdjustmentAddress + offsetAddress;
		}
	}
	else
	if( _tcscmp(szAddressName, _T("VendorUniqueControlAddress"))==0 )
	{
		if( m_dwVendorUniqueControlAddress==0 )
		{
			m_dwVendorUniqueControlAddress = GetVendorUniqueControlAddress();
		}
		if( m_dwVendorUniqueControlAddress )
		{
			dwAddress = m_dwVendorUniqueControlAddress + offsetAddress;
		}
	}
	else
	if( _tcscmp(szAddressName, _T("CameraFeatureAddress"))==0 )
	{
		if( m_dwCameraFeatureAddress==0 )
		{
			m_dwCameraFeatureAddress = GetCameraFeatureAddress();
		}
		if( m_dwCameraFeatureAddress )
		{
			dwAddress = m_dwCameraFeatureAddress + offsetAddress;
		}
	}
	else
	if( _tcscmp(szAddressName, _T("DefectivePixelCorrectionAddress"))==0 )
	{
		if( m_dwDefectivePixelCorrectionAddress==0 )
		{
			m_dwDefectivePixelCorrectionAddress = GetDefectivePixelCorrectionAddress();
		}
		if( m_dwDefectivePixelCorrectionAddress )
		{
			dwAddress = m_dwDefectivePixelCorrectionAddress + offsetAddress;
		}
	}
	else
	if( _tcslen(szAddressName)==0 )
	{
		dwAddress = offsetAddress;
	}

	return dwAddress;
}

DWORD StCustomDevice::GetControlPacketSizeMax(void)
{
	if( m_dwControlPacketSizeMax==0 )
	{
		DWORD dwData=0;
		//CXPのBootstrapRegister
		DWORD dwAddress=0x0000400C;
		BOOL bReval = Read(dwAddress, (char *)&dwData, 4 );
		if( bReval )
		{
			if( m_dwEndian!=0 ) dwData = htonl(dwData);
			if( dwData>24 )
			{
				dwData -= 24;	//CXPの仕様で24引く
				m_dwControlPacketSizeMax = dwData;
			}
		}
	}
	return m_dwControlPacketSizeMax;
}


unsigned int GetFileSize(CString fileName)
{
	fpos_t fsize = 0;

	FILE *fp = _tfopen(fileName,_T("rb")); 
	if( fp )
	{
		/* ファイルサイズを調査 */ 
		fseek(fp,0,SEEK_END); 
		fgetpos(fp,&fsize); 
 
		fclose(fp);
	}
	return (unsigned int)fsize;
}

//-------------------------------------------------------------------













BOOL StCustomDevice::WriteXMLFile(LPCTSTR szXMLFile)
{
	CString sFileFullPath(szXMLFile);
	bool bZip=false;
	if( sFileFullPath.Right(4)==".zip" )
		bZip=true;

	short SchemaVersionMajor=0;
	short SchemaVersionMinor=0;
	short SchemaVersionSubMinor=0;
	short VersionMajor=0;
	short VersionMinor=0;
	short VersionSubMinor=0;


	//ファイル名取得
	CString szFileName("");
	int index = sFileFullPath.ReverseFind('\\');
	if( index<=0 ){
		index = sFileFullPath.ReverseFind('/');
	}
	if( index>0  ){
		szFileName = sFileFullPath.Mid( index+1 );
	}

	//ファイルサイズ取得
	unsigned int iFileSize = GetFileSize(sFileFullPath);

	DWORD dwXMLMaxSize = 0x100000;
	if( iFileSize>dwXMLMaxSize )
	{
		//ファイルサイズが大きすぎるエラー
		//mResult = StResult::Code::XMLFILE_SIZE_ERROR;
		return FALSE;
	}

#ifdef _GENICAM

	//フルパスに全角文字があっても大丈夫なように。
	FILE *fp;
	errno_t errRet = _tfopen_s( &fp, sFileFullPath, _T("rb") );
	PBYTE pBuff = NULL;
	fpos_t fsize = 0;
	if( fp )
	{
		fseek(fp, 0, SEEK_END);
		fgetpos(fp, &fsize); 
		fseek(fp, 0, SEEK_SET);
		pBuff = new BYTE [(unsigned int)fsize+1];
		if( pBuff )
		{
			pBuff[fsize]=0;	//最後のNULL必要
			unsigned long origsize = fread(pBuff, 1, (size_t)fsize, fp); // データを読み込む
			if( origsize!=fsize )
			{
				delete [] pBuff;
				pBuff = NULL;
				fsize = 0;
			}
		}
		fclose( fp );
	}

	CNodeMapRef rNodeMap;
	if( pBuff==NULL )
	{
		//フルパスファイル名でノード作成
		gcstring fullfilename(sFileFullPath);
		if( bZip )
		{
			rNodeMap._LoadXMLFromZIPFile(fullfilename);
		}
		else
		{
			rNodeMap._LoadXMLFromFile(fullfilename);
		}
	}
	else
	{
		//ファイルの内容でノード作成
		if( bZip )
		{
			rNodeMap._LoadXMLFromZIPData(pBuff, fsize);
		}
		else
		{
			gcstring gcstrXml = (char *)pBuff;
			rNodeMap._LoadXMLFromString(gcstrXml);
		}
		delete [] pBuff;
		pBuff = NULL;
	}

	gcstring aname = rNodeMap._GetDeviceName();

	INodeMap *pINodemap( rNodeMap._Ptr );
	CDeviceInfoPtr ptrDeviceInfo(pINodemap);
	Version_t SchemaVersion;
	memset( &SchemaVersion,0,sizeof(SchemaVersion) );
	ptrDeviceInfo->GetSchemaVersion(SchemaVersion);
	SchemaVersionMajor = SchemaVersion.Major;
	SchemaVersionMinor = SchemaVersion.Minor;
	SchemaVersionSubMinor = SchemaVersion.SubMinor;
	Version_t Version;
	memset( &Version,0,sizeof(Version) );
	ptrDeviceInfo->GetDeviceVersion(Version);
	VersionMajor = Version.Major;
	VersionMinor = Version.Minor;
	VersionSubMinor = Version.SubMinor;

#else

	//ファイル名から取得する
	//命名規則
	//Sentech-STC_CXP_CM-1_1_0-2_0_0.xml
	//??????-????????-(SchemaVersion)-(XMLVersion)-???.xml
	index = szFileName.Find('-');
	if( index<0 )
	{
		mResult = StResult::Code::GENICAM_XML_ERROR;
		return;
	}
	CString szSchemaVersion = szFileName.Mid(index+1);	//Sentech削除
	index = szSchemaVersion.Find('-');
	if( index<0 )
	{
		mResult = StResult::Code::GENICAM_XML_ERROR;
		return;
	}
	szSchemaVersion = szSchemaVersion.Mid(index+1);	//STC_CXP_CM削除

	INT iSchemaIndex = szSchemaVersion.Find('-');
	if( iSchemaIndex<0 )
	{
		mResult = StResult::Code::GENICAM_XML_ERROR;
		return;
	}
	CString szVersion = szSchemaVersion.Mid(iSchemaIndex+1);
	szSchemaVersion = szSchemaVersion.Left(iSchemaIndex);

	index = szVersion.Find('-');
	if( index<0 )
	{
		index = szVersion.Find('.');
		if( index<0 )
		{
			mResult = StResult::Code::GENICAM_XML_ERROR;
			return;
		}
	}
	szVersion = szVersion.Left(index);

	index = szSchemaVersion.Find('_');
	if( index<0 )
	{
		mResult = StResult::Code::GENICAM_XML_ERROR;
		return;
	}
	SchemaVersionMajor = _ttoi(szSchemaVersion.Left(index));

	szSchemaVersion = szSchemaVersion.Mid(index+1);
	index = szSchemaVersion.Find('_');
	if( index<0 )
	{
		mResult = StResult::Code::GENICAM_XML_ERROR;
		return;
	}
	SchemaVersionMinor = _ttoi(szSchemaVersion.Left(index));
	SchemaVersionSubMinor = _ttoi(szSchemaVersion.Mid(index+1));
	 


	index = szVersion.Find('_');
	if( index<0 )
	{
		mResult = StResult::Code::GENICAM_XML_ERROR;
		return;
	}
	VersionMajor = _ttoi(szVersion.Left(index));

	szVersion = szVersion.Mid(index+1);
	index = szVersion.Find('_');
	if( index<0 )
	{
		mResult = StResult::Code::GENICAM_XML_ERROR;
		return;
	}
	VersionMinor = _ttoi(szVersion.Left(index));
	VersionSubMinor = _ttoi(szVersion.Mid(index+1));
#endif

	//-------------------------------------------------------------------------------
	//--書き込み-----------------------------------------------------------------------------
	//-------------------------------------------------------------------------------

	DWORD dwControlTimes=0;
	DWORD dwControlCount=0;
	bool bAdmin=false;
	CString szManifest("");
	DWORD dwXMLVersion=0;
	DWORD dwSchemaVersion=0;
	//アドレス固定-----------------
	DWORD dwXMLAddress = 0x90001000;

	size_t ReadManifestSize=0;
	bool bReval=false;
	char *pWriteXML=NULL;
	size_t writeXmlSize=0;
	DWORD dwControlPacketSizeMax=0;

	do
	{
		//0x0000400C ControlPacketSizeMax
		//ControlPacketSizeMaxをよんで最大パケットサイズを取得
		dwControlPacketSizeMax = GetControlPacketSizeMax(); //通信回数 1
		if( dwControlPacketSizeMax==0 )
		{
			//mResult = StResult::Code::CONTROL_ERROR;
			break;
		}

#ifdef _AVAL
		dwControlPacketSizeMax = 96;
#endif

		//XML書込みの通信回数を決める
		dwControlTimes = iFileSize / dwControlPacketSizeMax + ( (iFileSize%dwControlPacketSizeMax)>0?1:0 );
		//m_iWriteAll = dwControlTimes + 2 + 2 + 1 + 1 + 2;
		//m_iReadAll = dwControlTimes;
		//m_iWriteCount = 0;

		//WriteAdministrator( 1 );	//通信回数 2
		if(  !SetAccessMode( 1 ) )
			break;
		//m_iWriteCount += 2;

		//SetProgressbar();

		bAdmin=true;

		bReval = true;


		//Manifest書込み-----------------------------------------
		//Local: MyFilename.zip;B8000;33A?SchemaVersion=1.0.0
		szManifest.Format(_T("Local:%s;%x;%x?SchemaVersion=%i.%i.%i"),szFileName,dwXMLAddress,iFileSize,SchemaVersionMajor,SchemaVersionMinor,SchemaVersionSubMinor);
//TRACE(_T(" @@@@@szManifest=%s\n"),szManifest );
		ReadManifestSize=szManifest.GetLength()+1;//読み込みする時のサイズをとっておく	

		bReval = WriteManifest(szManifest); //通信回数 2
		//m_iWriteCount += 2;
		//SetProgressbar();
		if( !bReval )
		{
			//mResult = StResult::Code::CONTROL_ERROR;
			break;
		}

		//XMLVersion書込み-----------------------------------------
		dwXMLVersion = ((VersionMajor&0xff)<<16) | ((VersionMinor&0xff)<<8) | (VersionSubMinor&0xff);
		if( GetEndian()!=0 )
			dwXMLVersion = htonl(dwXMLVersion);
		bReval = WriteXmlVersion((PBYTE)&dwXMLVersion,4); //通信回数 1
		//m_iWriteCount += 1;
		//SetProgressbar();
		if( !bReval )
		{
			//mResult = StResult::Code::CONTROL_ERROR;
			break;
		}

		//SchemaVersion書込み-----------------------------------------
		dwSchemaVersion = ((SchemaVersionMajor&0xff)<<16) | ((SchemaVersionMinor&0xff)<<8) | (SchemaVersionSubMinor&0xff);
		if( GetEndian()!=0 )
			dwSchemaVersion = htonl(dwSchemaVersion);
		bReval = WriteSchemaVersion((PBYTE)&dwSchemaVersion,4); //通信回数 1
		//m_iWriteCount += 1;
		//SetProgressbar();
		if( !bReval )
		{
			//mResult = StResult::Code::CONTROL_ERROR;
			break;
		}

		//XMLファイルサイズ読み込み-----------------------------------------
		//size_t xmlSize=0;
		//bReval = GetXMLfileSize( m_sFileFullPath, &writeXmlSize );
		if( !bReval || writeXmlSize==0 )
		writeXmlSize = GetFileSize( sFileFullPath );
		if( writeXmlSize==0 )
		{
			//mResult = StResult::Code::FILE_ERROR;
			break;
		}
		//4の倍数に切り上げ
		DWORD dwWriteXmlSize = (writeXmlSize&(~3)) + ((writeXmlSize&3)>0?4:0);
		pWriteXML = new char [dwWriteXmlSize];
		if( !pWriteXML )
		{
			//mResult = StResult::Code::NOT_ENOUGH_MEMORY;
			break;
		}
		memset( pWriteXML, 0, dwWriteXmlSize*sizeof(char) );

		//XMLファイル書込み-----------------------------------------
		bReval = Writefile( sFileFullPath, dwXMLAddress, dwControlPacketSizeMax, pWriteXML, &writeXmlSize );
		if( !bReval )
		{
			//mResult = StResult::Code::CONTROL_ERROR;
			break;
		}

	}while(0);
	if( bAdmin )
		SetAccessMode( 0 );
	//m_iWriteCount += 2;
	//SetProgressbar();

	//-------------------------------------------------------------------------------
	//--読み込み-----------------------------------------------------------------------------
	//-------------------------------------------------------------------------------
	char *pReadXML = NULL;
	while(bReval)
	{
		//m_iMode = 1;
		//m_iReadAll += 4;
		//m_iReadCount = 0;

		//Manifest読み込み-----------------------------------------
		CString szReadManifest("");
		bReval = ReadManifest(szReadManifest, ReadManifestSize); //通信回数 2
		//m_iReadCount+=2;
		//SetProgressbar();
		if( !bReval ) break;
		if( szManifest.Compare(szReadManifest)!=0 )
		{
			//mResult = StResult::Code::MANIFEST_COMPARE_ERROR;
			break;
		}

		//XMLVersion読み込み-----------------------------------------
		DWORD dwReadXMLVersion=0;
		bReval = ReadXmlVersion((PBYTE)&dwReadXMLVersion,4); //通信回数 1
//TRACE(_T(" @@@@@dwXMLVersion=%d[0x%08x]\n"),dwXMLVersion,dwReadXMLVersion );
		//dwReadXMLVersion = htonl(dwReadXMLVersion);
//TRACE(_T(" @@@@@dwXMLVersion=%d[0x%08x]\n"),dwXMLVersion,dwReadXMLVersion );
		//m_iReadCount+=1;
		//SetProgressbar();
		if( !bReval )
		{
			//mResult = StResult::Code::CONTROL_ERROR;
			bReval = FALSE;
			break;
		}
		if( dwXMLVersion!=dwReadXMLVersion )
		{
			//mResult = StResult::Code::XMLVERSION_COMPARE_ERROR;
			bReval = FALSE;
			break;
		}

		//SchemaVersion読み込み-----------------------------------------
		DWORD dwReadSchemaVersion=0;
		bReval = ReadSchemaVersion((PBYTE)&dwReadSchemaVersion,4); //通信回数 1
		//dwReadSchemaVersion = htonl(dwReadSchemaVersion);
		//m_iReadCount+=1;
		//SetProgressbar();
		if( !bReval )
		{
			//mResult = StResult::Code::CONTROL_ERROR;
			bReval = FALSE;
			break;
		}
		if( dwSchemaVersion!=dwReadSchemaVersion )
		{
			//mResult = StResult::Code::SCHEMAVERSION_COMPARE_ERROR;
			bReval = FALSE;
			break;
		}

		//XMLファイル読み込み-----------------------------------------
		
		//ここで４の倍数に切り上げ
		size_t xmlSize = (writeXmlSize&(~3)) + ((writeXmlSize&3)>0?4:0);

		pReadXML = new char [xmlSize];
		if( !pReadXML )
		{
			//mResult = StResult::Code::NOT_ENOUGH_MEMORY;
			bReval = FALSE;
			break;
		}

		bReval = ReadData(pReadXML, xmlSize, dwXMLAddress, dwControlPacketSizeMax );
		if( !bReval )
		{
			//mResult = StResult::Code::CONTROL_ERROR;
			break;
		}
//TRACE(_T(" @@@@@ pWriteXML=[%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:] \n"), pWriteXML[0], pWriteXML[1], pWriteXML[2], pWriteXML[3], pWriteXML[4], pWriteXML[5], pWriteXML[6], pWriteXML[7]);
//TRACE(_T(" @@@@@ pReadXML=[%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:] \n"), pReadXML[0], pReadXML[1], pReadXML[2], pReadXML[3], pReadXML[4], pReadXML[5], pReadXML[6], pReadXML[7]);
		//Compareするのは4の倍数とはかぎらない。
		bReval = CompareData( pWriteXML, writeXmlSize, pReadXML, writeXmlSize );
		if( !bReval )
		{
			//mResult = StResult::Code::XMLFILE_COMPARE_ERROR;
			break;
		}
		break;
	}

	if( pWriteXML ) delete [] pWriteXML;
	if( pReadXML ) delete [] pReadXML;

	return bReval;

}

BOOL StCustomDevice::WriteManifest(CString &szManifest)
{
	BOOL bReval=TRUE;
	//CXP BootstrapRegister-------
	DWORD dwAddress = 0x00000018;
	//XMLURLアドレス取得
	DWORD dwXMLURLAddress=0;
	bReval = Read(dwAddress, (char *)&dwXMLURLAddress, 4);
	if( !bReval ) return FALSE;
	if( dwXMLURLAddress==0 ) return FALSE;
	if( GetEndian()!=0 )
		dwXMLURLAddress = htonl(dwXMLURLAddress);

	//XMLURL書込み取得
	StString aManifest(szManifest);
	size_t size = aManifest.GetLength() + 1;
	size = ( size + 3 ) / 4 * 4;
	char *pBuffer = new char [size];
	if( pBuffer==NULL )  return FALSE;
	memset( pBuffer, 0, size);
	sprintf_s( pBuffer, size, "%s", aManifest.GetAscii() );

//TRACE(_T(" @@@@@ pBuffer=[%02x:%02x:%02x:%02x] \n"), pBuffer[0], pBuffer[1], pBuffer[2], pBuffer[3] );


	bReval = Write( dwXMLURLAddress, pBuffer, size );
	delete [] pBuffer;

	return bReval;
}

BOOL StCustomDevice::ReadManifest(CString &szManifest, size_t ManifestSize)
{
	BOOL bReval=TRUE;
	DWORD dwAddress = 0x00000018;
	//XMLURLアドレス取得
	DWORD dwXMLURLAddress=0;
	bReval = Read(dwAddress,(char *)&dwXMLURLAddress,4);
	if( !bReval ) return FALSE;
	if( dwXMLURLAddress==0 ) return FALSE;
	if( GetEndian()!=0 )
		dwXMLURLAddress = htonl(dwXMLURLAddress);

	//XMLURL書込み取得
	size_t size = ManifestSize + 1;
	size = ( size + 3 ) / 4 * 4;
	char *pBuffer = new char [size];
	if( pBuffer==NULL )  return FALSE;
	//memset( pBuffer, 0, size);
	//sprintf_s( pBuffer, size, "%s", szManifest );

	bReval = Read( dwXMLURLAddress, pBuffer, size );
	//いちおう最後にNULLを入れる
	if( bReval )
	{
		pBuffer[size-1] = NULL;
		szManifest = pBuffer;
	}
	delete [] pBuffer;

	return bReval;
}





BOOL StCustomDevice::WriteXmlVersion(PBYTE pData, size_t size)
{
	BOOL bReval=TRUE;

	//TEST-----------------------------
	////DWORD dwAddress = 0x01160004;	//1280
	//DWORD dwAddress = 0x01160008;	//1024
	////DWORD dwAddress = 0x0116000C;	//127
	//DWORD dwData=0;
	//bReval = mDevice.Read( &dwData, dwAddress, 4 );
	//TRACE(_T(" @@@@@dwAddress=0x%x  dwData=%i bReval=%d\n"), dwAddress, dwData, bReval);
	//TEST-----------------------------

	DWORD dwAddress = 0x00000010;

	bReval = Write( dwAddress, (char *)pData, size );

	return bReval;
}

BOOL StCustomDevice::ReadXmlVersion(PBYTE pData, size_t size)
{
	BOOL bReval=TRUE;

	//TEST-----------------------------
	////DWORD dwAddress = 0x01160004;	//1280
	//DWORD dwAddress = 0x01160008;	//1024
	////DWORD dwAddress = 0x0116000C;	//127
	//DWORD dwData=0;
	//bReval = mDevice.Read( &dwData, dwAddress, 4 );
	//TRACE(_T(" @@@@@dwAddress=0x%x  dwData=%i bReval=%d\n"), dwAddress, dwData, bReval);
	//TEST-----------------------------

	DWORD dwAddress = 0x00000010;

	bReval = Read( dwAddress, (char *)pData, size );

	return bReval;
}

BOOL StCustomDevice::WriteSchemaVersion(PBYTE pData, size_t size)
{
	BOOL bReval=TRUE;

	DWORD dwAddress = 0x00000014;

	bReval = Write( dwAddress, (char *)pData, size );

	return bReval;
}

BOOL StCustomDevice::ReadSchemaVersion(PBYTE pData, size_t size)
{
	BOOL bReval=TRUE;

	DWORD dwAddress = 0x00000014;

	bReval = Read( dwAddress, (char *)pData, size );

	return bReval;
}

//*pSizeは４の倍数
BOOL StCustomDevice::Writefile( LPCTSTR pFilename, DWORD dwAddress, DWORD dwControlPacketSizeMax, char *pBuffer, size_t *pSize, bool bSwap )
{
	BOOL bReval=FALSE;

	FILE *fp = _tfopen(pFilename,_T("rb")); 
	if( fp )
	{
		fpos_t fsize = 0;
		/* ファイルサイズを調査 */ 
		fseek(fp,0,SEEK_END); 
		fgetpos(fp,&fsize); 
		fseek(fp,0,SEEK_SET); 

		do
		{
			if( *pSize<fsize ) break;
			size_t getSize = fread( pBuffer, 1, fsize, fp );
			if( getSize!=fsize ) break;
			*pSize=fsize;

			if( bSwap)
			{
				for( int i=0; i<fsize; i++ )
				{
					pBuffer[i] = ((pBuffer[i]&0x80)>>7) | ((pBuffer[i]&0x40)>>5) | ((pBuffer[i]&0x20)>>3) | ((pBuffer[i]&0x10)>>1) | ((pBuffer[i]&0x08)<<1) | ((pBuffer[i]&0x04)<<3) | ((pBuffer[i]&0x02)<<5) | ((pBuffer[i]&0x01)<<7);
				}
			}
			//ここで４の倍数に切り上げ
			fsize = (fsize&(~3)) + ((fsize&3)>0?4:0);
			do
			{
				size_t WriteSize = fsize>dwControlPacketSizeMax?dwControlPacketSizeMax:fsize;

				bReval = Write( dwAddress, pBuffer, WriteSize );
				//m_iWriteCount += 1;
				//SetProgressbar();

//				char readBuf[4];
//				mDevice.Read( readBuf, dwAddress, 4 );
//TRACE(_T(" @@@@@ pWriteXML=[%02x:%02x:%02x:%02x] readBuf=[%02x:%02x:%02x:%02x] \n"), pXMLBuffer[0], pXMLBuffer[1], pXMLBuffer[2], pXMLBuffer[3], readBuf[0], readBuf[1], readBuf[2], readBuf[3]);

//bReval = true;
				dwAddress+=WriteSize;
				pBuffer+=WriteSize;
				fsize -= WriteSize;
			}while( fsize>0 && bReval );
		}while(0);

		fclose(fp);
	}

	return bReval;
}

BOOL StCustomDevice::ReadData( char * pBuffer, size_t DataSize, DWORD dwAddress, DWORD dwControlPacketSizeMax )
{
	BOOL bReval=FALSE;
	do
	{
		size_t ReadSize = DataSize>dwControlPacketSizeMax?dwControlPacketSizeMax:DataSize;

		bReval = Read( dwAddress, pBuffer, ReadSize );
		//m_iReadCount+=1;
		//SetProgressbar();

		dwAddress+=ReadSize;
		DataSize -= ReadSize;
		pBuffer += ReadSize;

	}while( DataSize>0 && bReval );

	return bReval;
}

BOOL StCustomDevice::CompareData( const char * pFileBuffer, size_t FileSize, const char * pReadBuffer, size_t ReadSize )
{
	BOOL bReval=FALSE;
	if( FileSize!=ReadSize )
		return bReval;
	//if( memcmp(pFileBuffer,pReadBuffer,ReadSize) == 0 )
	//	bReval = true;


//TRACE(_T(" @@@@@pFileBuffer="));
//for( int i=0; i<16; i++ )
//{
//TRACE(_T("%02x:"), pFileBuffer[0x80+i]);
//}
//TRACE(_T("\n"));
//TRACE(_T(" @@@@@  pReadBuffer="));
//for( int i=0; i<16; i++ )
//{
//TRACE(_T("%02x:"), pReadBuffer[0x80+i]);
//}
//TRACE(_T("\n"));

	int iCount=0;
	bReval=TRUE;
	do
	{
		size_t cmpsize = ReadSize>16?16:ReadSize;
		if( memcmp(pFileBuffer,pReadBuffer,cmpsize)!=0 )
		{
			bReval=FALSE;
//TRACE(_T(" @@@@@  memcmp error line=%d(0x%x)\n"),iCount,iCount);
//
//
//TRACE(_T(" @@@@@pFileBuffer="));
//for( int i=0; i<cmpsize; i++ )
//{
//TRACE(_T("%02x:"), pFileBuffer[i]);
//}
//TRACE(_T("\n"));
//TRACE(_T(" @@@@@  pReadBuffer="));
//for( int i=0; i<cmpsize; i++ )
//{
//TRACE(_T("%02x:"), pReadBuffer[i]);
//}
//TRACE(_T("\n"));

			break;
		}
		iCount++;
		ReadSize-=cmpsize;
		if( ReadSize==0 ) break;
		pFileBuffer+=cmpsize;
		pReadBuffer+=cmpsize;
	}while(1);

	return bReval;
}


BOOL StCustomDevice::WriteBayer(DWORD dwBayer)
{
	BOOL bReval = FALSE;

	//書込み
	if( !SetAccessMode( 1 ) )
		return bReval;
	//bReval = SetIntegerValue( (int *)&dwBayer, LPCTSTR szName, DWORD offset, INT iLSB, INT iMSB )
	bReval = SetIntegerValue( (int *)&dwBayer, m_szBayerInfoBaseName, m_dwBayerInfoOffset, m_iBayerInfoLSB, m_iBayerInfoMSB );
	SetAccessMode( 0 );

	if( bReval )
	{
		//読込
		DWORD dwReadBayer=0;
		bReval = GetIntegerValue( (int *)&dwReadBayer, m_szBayerInfoBaseName, m_dwBayerInfoOffset, m_iBayerInfoLSB, m_iBayerInfoMSB );
		if( bReval )
		{
			if( dwReadBayer!=dwBayer ) bReval = FALSE;
		}
	}
	return bReval;
}

BOOL StCustomDevice::WriteCustomerCode(INT iCode)
{
	BOOL bReval = FALSE;

	//書込み
	if( !SetAccessMode( 1 ) )
		return bReval;
	//bReval = SetIntegerValue( (int *)&dwBayer, LPCTSTR szName, DWORD offset, INT iLSB, INT iMSB )
	bReval = SetIntegerValue( (int *)&iCode, m_szCustomerCodeBaseName, m_dwCustomerCodeOffset, m_iCustomerCodeLSB, m_iCustomerCodeMSB );
	SetAccessMode( 0 );

	if( bReval )
	{
		//読込
		INT iReadCode=0;
		bReval = GetIntegerValue( (int *)&iReadCode, m_szCustomerCodeBaseName, m_dwCustomerCodeOffset, m_iCustomerCodeLSB, m_iCustomerCodeMSB );
		if( bReval )
		{
			if( iReadCode!=iCode ) bReval = FALSE;
		}
	}
	return bReval;
}

BOOL StCustomDevice::WriteGamma(DWORD dwGamma)
{
	BOOL bReval = FALSE;

	//書込み
	if( !SetAccessMode( 1 ) )
		return bReval;
	//bReval = SetIntegerValue( (int *)&dwBayer, LPCTSTR szName, DWORD offset, INT iLSB, INT iMSB )
	bReval = SetIntegerValue( (int *)&dwGamma, m_szGammaBaseName, m_dwGammaOffset, m_iGammaLSB, m_iGammaMSB );
	SetAccessMode( 0 );

	if( bReval )
	{
		//読込
		DWORD dwReadGamma=0;
		bReval = GetIntegerValue( (int *)&dwReadGamma, m_szGammaBaseName, m_dwGammaOffset, m_iGammaLSB, m_iGammaMSB );
		if( bReval )
		{
			if( dwReadGamma!=dwGamma ) bReval = FALSE;
		}
	}
	return bReval;
}

//▼1.0.0.22
//2Lane 8M 150fps切り替え表示フラグ書込み
BOOL StCustomDevice::WriteConfigurationImageSizeDefaultImpl(DWORD dwData)
{
	BOOL bReval = FALSE;

	//書込み
	if( !SetAccessMode( 1 ) )
		return bReval;
	//bReval = SetIntegerValue( (int *)&dwBayer, LPCTSTR szName, DWORD offset, INT iLSB, INT iMSB )
	bReval = SetIntegerValue( (int *)&dwData, m_szConfigurationImageSizeDefaultImplBaseName, m_dwConfigurationImageSizeDefaultImplOffset, m_iConfigurationImageSizeDefaultImplLSB, m_iConfigurationImageSizeDefaultImplMSB );
	SetAccessMode( 0 );

	if( bReval )
	{
		//読込
		DWORD dwReadData=0;
		bReval = GetIntegerValue( (int *)&dwReadData, m_szConfigurationImageSizeDefaultImplBaseName, m_dwConfigurationImageSizeDefaultImplOffset, m_iConfigurationImageSizeDefaultImplLSB, m_iConfigurationImageSizeDefaultImplMSB );
		if( bReval )
		{
			if( dwReadData!=dwData ) bReval = FALSE;
		}
	}
	return bReval;
}
//▲1.0.0.22

BOOL StCustomDevice::WriteModelName(LPCTSTR szModelName)
{
	BOOL bReval = FALSE;
	//書込み
	if( !SetAccessMode( 1 ) )
		return bReval;

	StString aModelName = szModelName;
	bReval = SetDeviceModelName(aModelName.GetAscii());

	SetAccessMode( 0 );

	if( bReval )
	{
		//読込
		char szReadModelName[256];
		memset(szReadModelName,0,sizeof(szReadModelName));
		bReval = GetDeviceModelName( szReadModelName, _countof(szReadModelName) );
		if( bReval )
		{
			if( aModelName!=szReadModelName ) bReval = FALSE;
		}
	}
	return bReval;
}

BOOL StCustomDevice::WriteDeviceVendorName(LPCTSTR szDeviceVendorName)
{
	BOOL bReval = FALSE;
	//書込み
	if( !SetAccessMode( 1 ) )
		return bReval;

	StString aDeviceVendorName = szDeviceVendorName;
	bReval = SetDeviceVendorName(aDeviceVendorName.GetAscii());

	SetAccessMode( 0 );

	if( bReval )
	{
		//読込
		char szReadDeviceVendorName[256];
		memset(szReadDeviceVendorName,0,sizeof(szReadDeviceVendorName));
		bReval = GetDeviceVendorName( szReadDeviceVendorName, _countof(szReadDeviceVendorName) );
		if( bReval )
		{
			if( aDeviceVendorName!=szReadDeviceVendorName ) bReval = FALSE;
		}
	}
	return bReval;
}

BOOL StCustomDevice::WriteDeviceManufacturerInfo(LPCTSTR szDeviceManufacturerInfo)
{
	BOOL bReval = FALSE;
	//書込み
	if( !SetAccessMode( 1 ) )
		return bReval;

	StString aDeviceManufacturerInfo = szDeviceManufacturerInfo;
	bReval = SetDeviceManufacturerInfo(aDeviceManufacturerInfo.GetAscii());

	SetAccessMode( 0 );

	if( bReval )
	{
		//読込
		char szReadDeviceManufacturerInfo[256];
		memset(szReadDeviceManufacturerInfo,0,sizeof(szReadDeviceManufacturerInfo));
		bReval = GetDeviceManufacturerInfo( szReadDeviceManufacturerInfo, _countof(szReadDeviceManufacturerInfo) );
		if( bReval )
		{
			if( aDeviceManufacturerInfo!=szReadDeviceManufacturerInfo ) bReval = FALSE;
		}
	}
	return bReval;
}


#ifdef _PLEORA

bool StCustomDevice::TLParamsLocked( bool lock )
{
    PvGenParameterArray *lDeviceParams = GetGenParameters();
    PvGenInteger *lTLLocked = dynamic_cast<PvGenInteger *>( lDeviceParams->Get( "TLParamsLocked" ) );
    if ( lTLLocked != NULL )
    {
		lTLLocked->SetValue( lock ? 1 : 0 );
		return true;
	}
	return false;
}
bool StCustomDevice::GetGenIntParameter( StString lFeature, int & iData, bool Verify, bool IgnoreCache )
{
    PvGenParameterArray *lDeviceParams = GetGenParameters();
    PvGenInteger *lGenFeature = dynamic_cast<PvGenInteger *>( lDeviceParams->Get( lFeature.GetAscii() ) );

    if ( lGenFeature != NULL )
    {
		PvInt64 aValue;
		if( lGenFeature->GetValue( aValue ).IsOK() )
		{
			iData = aValue;
			return true;
		}
	}
	return false;
}

bool StCustomDevice::GetGenIntMinMaxParameter( StString lFeature, int & iMin, int & iMax )
{
    PvGenParameterArray *lDeviceParams = GetGenParameters();
    PvGenInteger *lGenFeature = dynamic_cast<PvGenInteger *>( lDeviceParams->Get( lFeature.GetAscii() ) );

	bool bReval = false;
    if ( lGenFeature != NULL )
    {
		PvInt64 aMin, aMax;
		if( lGenFeature->GetMin( aMin ).IsOK() )
		{
			if( lGenFeature->GetMax( aMax ).IsOK() )
			{
				bReval =  true;
			}
		}
		if( bReval )
		{
			iMin = aMin;
			iMax = aMax;
		}
	}
	return bReval;
}

bool StCustomDevice::SetGenIntParameter( StString lFeature, int iData )
{
    PvGenParameterArray *lDeviceParams = GetGenParameters();
    PvGenInteger *lGenFeature = dynamic_cast<PvGenInteger *>( lDeviceParams->Get( lFeature.GetAscii() ) );
    if ( lGenFeature != NULL )
    {
		if( lGenFeature->SetValue( iData ).IsOK() )
		{
			return true;
		}
	}
	return false;
}

bool StCustomDevice::GetGenEnumParameter( StString lFeature, int & iData, bool Verify, bool IgnoreCache )
{
    PvGenParameterArray *lDeviceParams = GetGenParameters();
    PvGenEnum *lGenFeature = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( lFeature.GetAscii() ) );
    if ( lGenFeature != NULL )
    {
		PvInt64 aValue;
		if( lGenFeature->GetValue( aValue ).IsOK() )
		{
			iData = aValue;
			return true;
		}
	}
	return false;
}

bool StCustomDevice::GetGenEnumParameter( StString lFeature, StString & strData, bool Verify, bool IgnoreCache )
{
    PvGenParameterArray *lDeviceParams = GetGenParameters();
    PvGenEnum *lGenFeature = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( lFeature.GetAscii() ) );
    if ( lGenFeature != NULL )
    {
		PvString aValue;
		if( lGenFeature->GetValue( aValue ).IsOK() )
		{
			strData = aValue.GetAscii();
			return true;
		}
	}
	return false;
}


bool StCustomDevice::SetGenEnumParameter( StString lFeature, int iData )
{
    PvGenParameterArray *lDeviceParams = GetGenParameters();
    PvGenEnum *lGenFeature = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( lFeature.GetAscii() ) );
    if ( lGenFeature != NULL )
    {
		if( lGenFeature->SetValue( iData ).IsOK() )
		{
			return true;
		}
	}
	return false;
}

bool StCustomDevice::SetGenEnumParameter( StString lFeature, StString strData )
{
    PvGenParameterArray *lDeviceParams = GetGenParameters();
    PvGenEnum *lGenFeature = dynamic_cast<PvGenEnum *>( lDeviceParams->Get( lFeature.GetAscii() ) );
    if ( lGenFeature != NULL )
    {
		PvString aValue(strData.GetAscii());
		if( lGenFeature->SetValue( aValue ).IsOK() )
		{
			return true;
		}
	}
	return false;
}

bool StCustomDevice::GetGenFloatParameter( StString lFeature, double & dblData, bool Verify, bool IgnoreCache )
{
    PvGenParameterArray *lDeviceParams = GetGenParameters();
    PvGenFloat *lGenFeature = dynamic_cast<PvGenFloat *>( lDeviceParams->Get( lFeature.GetAscii() ) );
    if ( lGenFeature != NULL )
    {
		if( lGenFeature->GetValue( dblData ).IsOK() )
		{
			return true;
		}
	}
	return false;
}

bool StCustomDevice::GetGenFloatMinMaxParameter( StString lFeature, double & dblMin, double & dblMax )
{
    PvGenParameterArray *lDeviceParams = GetGenParameters();
    PvGenFloat *lGenFeature = dynamic_cast<PvGenFloat *>( lDeviceParams->Get( lFeature.GetAscii() ) );

	bool bReval = false;
    if ( lGenFeature != NULL )
    {
		double aMin, aMax;
		if( lGenFeature->GetMin( aMin ).IsOK() )
		{
			if( lGenFeature->GetMax( aMax ).IsOK() )
			{
				bReval =  true;
			}
		}
		if( bReval )
		{
			dblMin = aMin;
			dblMax = aMax;
		}
	}
	return bReval;
}

bool StCustomDevice::SetGenFloatParameter( StString lFeature, double dblData )
{
    PvGenParameterArray *lDeviceParams = GetGenParameters();
    PvGenFloat *lGenFeature = dynamic_cast<PvGenFloat *>( lDeviceParams->Get( lFeature.GetAscii() ) );
    if ( lGenFeature != NULL )
    {
		if( lGenFeature->SetValue( dblData ).IsOK() )
		{
			return true;
		}
	}
	return false;
}

bool StCustomDevice::GetGenBoolParameter( StString lFeature, bool & bData, bool Verify, bool IgnoreCache )
{
    PvGenParameterArray *lDeviceParams = GetGenParameters();
    PvGenBoolean *lGenFeature = dynamic_cast<PvGenBoolean *>( lDeviceParams->Get( lFeature.GetAscii() ) );
    if ( lGenFeature != NULL )
    {
		bool lData;
		if( lGenFeature->GetValue( lData ).IsOK() )
		{
			bData = lData;
			return true;
		}
	}
	return false;
}

bool StCustomDevice::SetGenBoolParameter( StString lFeature, bool bData )
{
    PvGenParameterArray *lDeviceParams = GetGenParameters();
    PvGenBoolean *lGenFeature = dynamic_cast<PvGenBoolean *>( lDeviceParams->Get( lFeature.GetAscii() ) );
    if ( lGenFeature != NULL )
    {
		if( lGenFeature->SetValue( bData ).IsOK() )
		{
			return true;
		}
	}
	return false;
}

bool StCustomDevice::GetGenStringParameter( StString lFeature, StString & strData, bool Verify, bool IgnoreCache )
{
    PvGenParameterArray *lDeviceParams = GetGenParameters();
    PvGenString *lGenFeature = dynamic_cast<PvGenString *>( lDeviceParams->Get( lFeature.GetAscii() ) );
    if ( lGenFeature != NULL )
    {
		PvString lData;
		if( lGenFeature->GetValue( lData ).IsOK() )
		{
			strData = lData.GetAscii();
			return true;
		}
	}
	return false;
}

bool StCustomDevice::SetGenStringParameter( StString lFeature, StString & strData )
{
    PvGenParameterArray *lDeviceParams = GetGenParameters();
    PvGenString *lGenFeature = dynamic_cast<PvGenString *>( lDeviceParams->Get( lFeature.GetAscii() ) );
    if ( lGenFeature != NULL )
    {
		PvString lData(strData.GetAscii());
		if( lGenFeature->SetValue( lData ).IsOK() )
		{
			return true;
		}
	}
	return false;
}


bool StCustomDevice::SetGenCommandParameter( StString lFeature )
{
    PvGenParameterArray *lDeviceParams = GetGenParameters();
    PvGenCommand *lGenFeature = dynamic_cast<PvGenCommand *>( lDeviceParams->Get( lFeature.GetAscii() ) );
    if ( lGenFeature != NULL )
    {
		if( lGenFeature->Execute().IsOK() )
		{
			return true;
		}
	}
	return false;
}


bool StCustomDevice::GetPayloadSize( int & iData )
{
	return GetGenIntParameter( "PayloadSize", iData);
}

#endif






//▼1.0.0.22
BOOL StCustomDevice::SetReg99(int iData)
{
	BOOL bReval = FALSE;
	//書込み
	if( !SetAccessMode( 1 ) )
		return bReval;
	bReval = SetIntegerValue( &iData, m_szReg99BaseName, m_dwReg99Offset, m_dwReg99LSB, m_dwReg99MSB );
	SetAccessMode( 0 );

	if( bReval )
	{
		//読込
		INT iReadData=0;
		bReval = GetReg99( &iReadData );
		if( bReval )
		{
			if( iReadData!=iData ) bReval = FALSE;
		}
	}
	return bReval;

}

BOOL StCustomDevice::GetReg99( int *pData )
{
	return GetIntegerValue( pData, m_szReg99BaseName, m_dwReg99Offset, m_dwReg99LSB, m_dwReg99MSB );
}

BOOL StCustomDevice::GetReg99Status( int *pData )
{
	return GetIntegerValue( pData, m_szReg99StatusBaseName, m_dwReg99StatusOffset, m_dwReg99StatusLSB, m_dwReg99StatusMSB );
}

BOOL StCustomDevice::GetHorizontalFixedGainCorrectionMode( int *pData )
{
	return GetIntegerValue( pData, m_szHorizontalFixedGainCorrectionModeBaseName, m_dwHorizontalFixedGainCorrectionModeOffset, m_iHorizontalFixedGainCorrectionModeLSB, m_iHorizontalFixedGainCorrectionModeMSB );
}

BOOL StCustomDevice::SetHorizontalFixedGainCorrectionMode( int *pData )
{
	return SetIntegerValue( pData, m_szHorizontalFixedGainCorrectionModeBaseName, m_dwHorizontalFixedGainCorrectionModeOffset, m_iHorizontalFixedGainCorrectionModeLSB, m_iHorizontalFixedGainCorrectionModeMSB );
}

BOOL StCustomDevice::SetRowGainAdjustment( int nMode, int nGainTableSize, PBYTE pbyteGainTable )
{
	BOOL bReval=FALSE;
	DWORD WriteMaxSize = GetControlPacketSizeMax();
	if( WriteMaxSize==0 ) return FALSE;
	DWORD TopAddress=GetAddress(m_szRowGainAdjustmentBaseName, m_dwRowGainAdjustmentOffset[nMode]);
	if( m_dwEndian==0 )
	{
		do
		{
			DWORD WriteSize = nGainTableSize>WriteMaxSize?WriteMaxSize:nGainTableSize;
			bReval = Write(TopAddress, (char *)pbyteGainTable, WriteSize );
			TopAddress += WriteSize;
			pbyteGainTable += WriteSize;
			nGainTableSize -= WriteSize;
		}while(nGainTableSize>0 && bReval);
	}

	else
	{
		PBYTE pData = new BYTE [WriteMaxSize];
		if( !pData ) return FALSE;
		do
		{
			DWORD WriteSize = nGainTableSize>WriteMaxSize?WriteMaxSize:nGainTableSize;

			bReval = SwapByte( pbyteGainTable, pData, WriteSize );
			if( !bReval ) break;

//memset(pData,0,WriteSize );

			bReval = Write(TopAddress, (char *)pData, WriteSize );

//TRACE(_T(" @@@@@@@@@@@@@@@@@TopAddress=0x%08x\n"), TopAddress);
//TRACE(_T(" @@@@@@@@@@@@@@@@@pData1=%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:\n"), pbyteGainTable[0], pbyteGainTable[1], pbyteGainTable[2], pbyteGainTable[3], pbyteGainTable[4], pbyteGainTable[5], pbyteGainTable[6], pbyteGainTable[7], pbyteGainTable[8], pbyteGainTable[9], pbyteGainTable[10], pbyteGainTable[11]);
//TRACE(_T(" @@@@@@@@@@@@@@@@@pData2=%02x:%02x:%02x:%02x\n"), pbyteGainTable[9], pbyteGainTable[50], pbyteGainTable[52], pbyteGainTable[54]);
			TopAddress += WriteSize;
			pbyteGainTable += WriteSize;
			nGainTableSize -= WriteSize;
		}while(nGainTableSize>0 && bReval);

		delete [] pData;
		
	}

	return bReval;
}


//▲1.0.0.22










