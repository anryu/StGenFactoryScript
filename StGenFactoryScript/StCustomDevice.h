#pragma once
#define _PLEORA

#ifdef _PLEORA
#include <PvDevice.h>
#include <PvStream.h>
#include <PvPipeline.h>
#else
#include "../CommonHeader/StGenDevice.h"
#include "../CommonHeader/StGenStream.h"
#endif
#include "../CommonHeader/StResult.h"
#include "../CommonHeader/StString.h"
#include "CaptureThread.h"
#include "StStruct.h"

class StCustomDevice
#ifdef _PLEORA
	 : public PvDevice
#else
	 : public StGenDevice
#endif
{
public:
	StCustomDevice(void);
	~StCustomDevice(void);

	//StResult StartStreams(CPreview *aDisplayWnd=NULL);
	bool IsStreamCreated(){return m_bStreamCreated;}

	StResult StartStreams(void *aDisplayWnd=NULL);
	StResult StopStreams(void);
	StResult StartAcquisition(void);
	StResult StopAcquisition(void);

	bool IsStarted(){return m_bStarted;}
	//CPreview *GetPreview(){return m_Preview;}

	BOOL InitCameraSetting(CCameraType* aCameraType);
	BOOL DefalutCameraSetting(void);

	BOOL SetDeviceSerialNumber( const char *pData );
	BOOL GetDeviceSerialNumber( char *pData, int iSize );

	BOOL SetDeviceModelName( const char *pData );
	BOOL GetDeviceModelName( char *pData, size_t bufSize  );

	BOOL SetDeviceVendorName( const char *pData );
	BOOL GetDeviceVendorName( char *pData, size_t bufSize  );

	BOOL SetDeviceManufacturerInfo( const char *pData );
	BOOL GetDeviceManufacturerInfo( char *pData, size_t bufSize  );

	//---XML書込み------------------------
	BOOL WriteXMLFile(LPCTSTR szXMLFile);
	//BayerInfo書込み---------------------
	BOOL WriteBayer(DWORD dwBayer);
	//CustomerCode書込み---------------------
	BOOL WriteCustomerCode(INT iCode);
	//Gammaフラグ書込み---------------------
	BOOL WriteGamma(DWORD dwGamma);
	//2Lane 8M 150fps切り替え表示フラグ書込み
	BOOL WriteConfigurationImageSizeDefaultImpl(DWORD dwData);
	//ModelName書込み---------------------
	BOOL WriteModelName(LPCTSTR szModelName);
	//DeviceVendorName書込み---------------------
	BOOL WriteDeviceVendorName(LPCTSTR szDeviceVendorName);
	//DeviceManufacturerInfo書込み---------------------
	BOOL WriteDeviceManufacturerInfo(LPCTSTR szDeviceManufacturerInfo);


	void SetCameraType(CCameraType * aCameraType){mCameraType = aCameraType;}
	CCameraType * GetCameraType(void){return mCameraType;}

#ifdef _PLEORA
	StResult CustomDevOpen(PvDeviceInfo *pDeviceInfo)
	{
		if( IsConnected() )
		{
			return StResult::Code::OK;
		}
		// Connect device
		PvResult lResult = Connect(pDeviceInfo);
		if( !lResult.IsOK() )
		{
			return StResult::Code::NOT_CONNECTED;
		}

	    // Perform automatic packet size negociation
		lResult = NegotiatePacketSize();
		if( !lResult.IsOK() )
		{
			::Sleep( 2500 );
		}

		// Open stream - and retry if it fails
		for ( ;; )
		{
			// Open stream
			lResult = mStream.Open( pDeviceInfo->GetIPAddress() );
			if ( lResult.IsOK() )
			{
				break;
			}
			::Sleep( 1000 );
		}

		// Now that the stream is opened, set the destination on the device
		SetStreamDestination( mStream.GetLocalIPAddress(), mStream.GetLocalPort() );
		if ( !lResult.IsOK() )
		{
			Disconnect();
			return PvResult::Code::NOT_CONNECTED;
		}
		// Ready image reception
		StartStreaming();

		StResult aResult = lResult.GetCode();

		return aResult;
	}
	StResult CustomDevClose()
	{
		// If streaming, stop streaming
		StopStreaming();

		PvResult lResult = Disconnect();

		mStream.Close();

		StResult aResult = lResult.GetCode();

		return aResult;
	}


	// =============================================================================
	void StartStreaming()
	{
		// Start threads
		//mDisplayThread->Start( &mPipeline, mDevice.GetGenParameters() );
		//mDisplayThread->SetPriority( THREAD_PRIORITY_ABOVE_NORMAL );

		// Start pipeline
		mPipeline.Start();
	}

	// =============================================================================
	void StopStreaming()
	{
		// Stop display thread
		//if ( mDisplayThread != NULL )
		//{
		//	mDisplayThread->Stop( false );
		//}

		// Stop stream thread
		if ( mPipeline.IsStarted() )
		{
			mPipeline.Stop();
		}

		// Wait for display thread to be stopped
		//if ( mDisplayThread != NULL )
		//{
		//	mDisplayThread->WaitComplete();
		//}
	}










	bool TLParamsLocked( bool lock );
	bool GetGenIntParameter( StString lFeature, int & iData, bool Verify=false, bool IgnoreCache=false );
	bool GetGenIntMinMaxParameter( StString lFeature, int & iMin, int & iMax );
	bool SetGenIntParameter( StString lFeature, int iData );
	bool GetGenEnumParameter( StString lFeature, int & iData, bool Verify=false, bool IgnoreCache=false );
	bool GetGenEnumParameter( StString lFeature, StString & strData, bool Verify=false, bool IgnoreCache=false );
	bool SetGenEnumParameter( StString lFeature, int iData );
	bool SetGenEnumParameter( StString lFeature, StString strData );
	bool GetGenFloatParameter( StString lFeature, double & dblData, bool Verify=false, bool IgnoreCache=false );
	bool GetGenFloatMinMaxParameter( StString lFeature, double & dblMin, double & dblMax );
	bool SetGenFloatParameter( StString lFeature, double dblData );
	bool GetGenBoolParameter( StString lFeature, bool & bData, bool Verify=false, bool IgnoreCache=false );
	bool SetGenBoolParameter( StString lFeature, bool bData );
	bool GetGenStringParameter( StString lFeature, StString & strData, bool Verify=false, bool IgnoreCache=false );
	bool SetGenStringParameter( StString lFeature, StString & strData );
	bool SetGenCommandParameter( StString lFeature );
	bool GetPayloadSize( int & iData );

#else
	StGenDeviceInfo * GetGenDeviceInfo(void){return mGenDeviceInfo;}
	StResult CustomDevOpen(StGenDeviceInfo *pGenDeviceInfo, CCameraType * aCameraType = NULL)
	{
		StResult aResult = DevOpen(pGenDeviceInfo);
		if( aResult.IsOK() )
		{
			mGenDeviceInfo = pGenDeviceInfo;
			mCameraType = aCameraType;
		}
		return aResult;
	}
	StResult CustomDevClose()
	{
		StResult aResult = DevClose();
		if( aResult.IsOK() )
		{
			mGenDeviceInfo = NULL;
		}
		return aResult;
	}
#endif

private:

#ifdef _PLEORA
	PvStream mStream;
	PvPipeline mPipeline;
#endif


	BOOL WriteManifest(CString &szManifest);
	BOOL ReadManifest(CString &szManifest, size_t ManifestSize);
	BOOL WriteXmlVersion(PBYTE pData, size_t size);
	BOOL ReadXmlVersion(PBYTE pData, size_t size);
	BOOL WriteSchemaVersion(PBYTE pData, size_t size);
	BOOL ReadSchemaVersion(PBYTE pData, size_t size);

	BOOL Writefile( LPCTSTR pFilename, DWORD dwAddress, DWORD dwControlPacketSizeMax, char *pBuffer, size_t *pSize, bool bSwap=false );
	BOOL ReadData( char * pBuffer, size_t DataSize, DWORD dwAddress, DWORD dwControlPacketSizeMax );
	BOOL CompareData( const char * pFileBuffer, size_t FileSize, const char * pReadBuffer, size_t ReadSize );

	//-----通信用--------------------------------------------
public:
	BOOL GetAccessMode( int *pData );
	BOOL SetAccessMode( int iData );
	INT	 GetLinkConfiguration2LaneLoad();
	BOOL GetLinkConfiguration2LaneLoad( int *pData );
	BOOL SetLinkConfiguration2LaneLoad( int iData );
	BOOL GetOffsetDigitalGain( int *pData, int iMode );
	BOOL SetOffsetDigitalGain( int *pData, int iMode );
	BOOL GetVerticalFixedPatternCorrectionMode( int *pData );
	BOOL SetVerticalFixedPatternCorrectionMode( int *pData );
	BOOL GetVerticalFixedGainCorrectionMode( int *pData );
	BOOL SetVerticalFixedGainCorrectionMode( int *pData );
	BOOL GetClampLevel( int *pData, int iMode, int iLine );
	BOOL SetClampLevel( int *pData, int iMode, int iLine );
	BOOL GetDefectiveSymptomAdjustment( int *pData, int iMode, int iLine );
	BOOL SetDefectiveSymptomAdjustment( int *pData, int iMode, int iLine );
	BOOL GetAdjustLevelGain( int *pData, int iMode );
	BOOL SetAdjustLevelGain( int *pData, int iMode );
	BOOL GetPixelOffsetForTable( int *pData );
	BOOL GetPixelCorrectMaxCount( int *pData );

	BOOL SetColumnBlackLevelAdjustment( int nMode, int nTableSize, PBYTE pbyteTable );
	BOOL SetColumnGainAdjustment( int nMode, int nGainTableSize, PBYTE pbyteGainTable );
	BOOL SetPixelCorrectPosition( int nMode, int nTableSize, PBYTE pbyteTable );
	BOOL SavePixelCorrectPosition( void );

	//▼1.0.0.22
	BOOL SetReg99( int iData );
	BOOL GetReg99( int *pData );
	BOOL GetReg99Status( int *pData );

	BOOL GetHorizontalFixedGainCorrectionMode( int *pData );
	BOOL SetHorizontalFixedGainCorrectionMode( int *pData );

	BOOL SetRowGainAdjustment( int nMode, int nGainTableSize, PBYTE pbyteGainTable );
	//▲1.0.0.22


	//----------------------------------------------
	DWORD GetControlPacketSizeMax(void);	//StGenFactoryDlgからもよばれる

private:
	//CPreview *m_Preview;
	void *m_Preview;
	//StResult CreateThread(CPreview *Preview);
	StResult CreateThread(void *Preview);
	void DeleteThread(void);
	void SetCaptureThread(CaptureThread * lThreadDisplay){m_lThreadDisplay=lThreadDisplay;}
	CaptureThread * GetCaptureThread(void){return m_lThreadDisplay;}
	CaptureThread *m_lThreadDisplay;
	bool m_bStreamCreated;
	bool m_bStarted;

	double m_dblDefAcquisitionFramerate;
	double m_dblDefExposureTime;
	StString m_szDefPixelFormat;
	StString m_szDefExposureMode;
	//▼1.0.0.21
	double m_dblDefBlackLevel;
	//▲1.0.0.21

	BOOL GetIntegerValue( int *pData, LPCTSTR szName, DWORD offset, INT iLSB, INT iMSB );
	BOOL SetIntegerValue( int *pData, LPCTSTR szName, DWORD offset, INT iLSB, INT iMSB );
	//-----通信用--------------------------------------------
	DWORD GetCategoryAddress(DWORD categoryOffset);
	DWORD GetTechnologyAgnosticFunctionSaveAddress();
	DWORD GetTechnologyAgnosticDoNotSaveAddress();
	DWORD GetTechnologyAgnosticCameraAdministratorAddress();
	DWORD GetTechnologyAgnosticCameraFeatureAddress();
	//▼1.0.0.22
	DWORD GetTechnologyAgnosticCameraStatusAddress();
	//▲1.0.0.22
	DWORD GetFPGAAdjustmentAddress();
	DWORD GetSensorAdjustmentAddress();
	DWORD GetVendorUniqueControlAddress();
	DWORD GetCameraFeatureAddress();
	DWORD GetDefectivePixelCorrectionAddress();

	DWORD GetAddress(LPCTSTR szAddressName, int offsetAddress);
	DWORD m_dwTechnologyAgnosticFunctionSaveAddress;
	DWORD m_dwTechnologyAgnosticDoNotSaveAddress;
	DWORD m_dwTechnologyAgnosticCameraAdministratorAddress;
	DWORD m_dwTechnologyAgnosticCameraFeatureAddress;
	//▼1.0.0.22
	DWORD m_dwTechnologyAgnosticCameraStatusAddress;
	//▲1.0.0.22
	DWORD m_dwFPGAAdjustmentAddress;
	DWORD m_dwSensorAdjustmentAddress;
	DWORD m_dwVendorUniqueControlAddress;
	DWORD m_dwCameraFeatureAddress;
	DWORD m_dwDefectivePixelCorrectionAddress;
	//----------------------------------------------
	//BOOL GetIntegerValue( int *pData, LPCTSTR szName, DWORD offset, INT iLSB, INT iMSB, DWORD dwEndian );
	//BOOL SetIntegerValue( int *pData, LPCTSTR szName, DWORD offset, INT iLSB, INT iMSB, DWORD dwEndian );
	//BOOL GetOffsetDigitalGain( int *pData );
	//BOOL SetOffsetDigitalGain( int *pData );
	//BOOL GetVerticalFixedPatternCorrectionMode( int *pData );
	//BOOL SetVerticalFixedPatternCorrectionMode( int *pData );
	//BOOL GetClampLevel( int *pData, int iMode );
	//BOOL SetClampLevel( int *pData, int iMode );

	CCameraType * mCameraType;
#ifndef _PLEORA
	StGenDeviceInfo * mGenDeviceInfo;
#endif

public:
	DWORD GetEndian(void){return m_dwEndian;}
	//----------------------------------------------
	DWORD m_dwEndian;
	//----------------------------------------------
	DWORD m_dwControlPacketSizeMax;
	//----------------------------------------------

	//FPGAAdjustmentAddress + 0x0004 (D0:7)
	TCHAR m_szAccessModeBaseName[128];
	DWORD m_dwAccessModeOffset;
	INT m_iAccessModeLSB;
	INT m_iAccessModeMSB;
	INT m_iAccessModePasscode;

	//TechnologyAgnosticFunctionSaveAddress + 0x000C (D0:31)
	TCHAR m_szLinkConfiguration2LaneLoadBaseName[128];
	DWORD m_dwLinkConfiguration2LaneLoadOffset;
	INT m_iLinkConfiguration2LaneLoadLSB;
	INT m_iLinkConfiguration2LaneLoadMSB;
	INT m_iLinkConfiguration2LaneLoadPasscode;

	//TechnologyAgnosticCameraAdministratorAddress + 0x0004 (D0:2)
	TCHAR m_szBayerInfoBaseName[128];
	DWORD m_dwBayerInfoOffset;
	INT m_iBayerInfoLSB;
	INT m_iBayerInfoMSB;

	//TechnologyAgnosticCameraAdministratorAddress + 0x0004 (D0:2)
	TCHAR m_szCustomerCodeBaseName[128];
	DWORD m_dwCustomerCodeOffset;
	INT m_iCustomerCodeLSB;
	INT m_iCustomerCodeMSB;

	//TechnologyAgnosticCameraFeatureAddress(0x01200000) + 0x0030 (D0:0)
	TCHAR m_szGammaBaseName[128];
	DWORD m_dwGammaOffset;
	INT m_iGammaLSB;
	INT m_iGammaMSB;

	//▼1.0.0.22
	//2Lane 8M 150fps切り替え表示フラグ書込み
	//TechnologyAgnosticCameraFeatureAddress(0x01200000) + 0x0030 (D0:1)
	TCHAR m_szConfigurationImageSizeDefaultImplBaseName[128];
	DWORD m_dwConfigurationImageSizeDefaultImplOffset;
	INT m_iConfigurationImageSizeDefaultImplLSB;
	INT m_iConfigurationImageSizeDefaultImplMSB;
	//▲1.0.0.22

	//0x20B0
	TCHAR m_szDeviceSerialNumberBaseName[128];
	DWORD m_dwDeviceSerialNumberOffset;
	DWORD m_dwDeviceSerialNumberSize;

	//0x2020
	TCHAR m_szDeviceModelNameBaseName[128];
	DWORD m_dwDeviceModelNameOffset;
	DWORD m_dwDeviceModelNameSize;

	//0x
	TCHAR m_szDeviceVendorNameBaseName[128];
	DWORD m_dwDeviceVendorNameOffset;
	DWORD m_dwDeviceVendorNameSize;

	//0x
	TCHAR m_szDeviceManufacturerInfoBaseName[128];
	DWORD m_dwDeviceManufacturerInfoOffset;
	DWORD m_dwDeviceManufacturerInfoSize;

	//FPGAAdjustmentAddress + 0x0004 (D0:7)
	TCHAR m_szOffsetDigitalGainBaseName[128];
	DWORD m_dwOffsetDigitalGainOffset[6];
	INT m_iOffsetDigitalGainLSB[6];
	INT m_iOffsetDigitalGainMSB[6];

	//FPGAAdjustmentAddress + 0x0020 (D0)
	TCHAR m_szVerticalFixedPatternCorrectionModeBaseName[128];
	DWORD m_dwVerticalFixedPatternCorrectionModeOffset;
	INT m_iVerticalFixedPatternCorrectionModeLSB;
	INT m_iVerticalFixedPatternCorrectionModeMSB;

	//FPGAAdjustmentAddress + 0x0020 (D1)--------------
	TCHAR m_szVerticalFixedGainCorrectionModeBaseName[128];
	DWORD m_dwVerticalFixedGainCorrectionModeOffset;
	INT m_iVerticalFixedGainCorrectionModeLSB;
	INT m_iVerticalFixedGainCorrectionModeMSB;

	//SensorAdjustmentAddress + 0x0008 + iMode * 12 (D0:7)
	TCHAR m_szClampLevelBaseName[128];
	DWORD m_dwClampLevelOffset[6][2];
	INT m_iClampLevelLSB[6][2];
	INT m_iClampLevelMSB[6][2];

	//SensorAdjustmentAddress + 0x0008 + iMode * 12 (D0:7)
	TCHAR m_szDefectiveSymptomAdjustmentBaseName[128];
	DWORD m_dwDefectiveSymptomAdjustmentOffset[3][2];
	INT m_iDefectiveSymptomAdjustmentLSB[3][2];
	INT m_iDefectiveSymptomAdjustmentMSB[3][2];

	//SensorAdjustmentAddress + 0x0004 + iMode * 12 (D0:7)
	TCHAR m_szADCGainBaseName[128];
	DWORD m_dwADCGainOffset[3];
	INT m_iADCGainLSB[3];
	INT m_iADCGainMSB[3];

	//縦筋補正
	TCHAR m_szColumnBlackLevelAdjustmentBaseName[128];
	DWORD m_dwColumnBlackLevelAdjustmentOffset[6];
	TCHAR m_szColumnGainAdjustmentBaseName[128];
	DWORD m_dwColumnGainAdjustmentOffset[6];

	//画素欠補正最大数
	TCHAR m_szPixelCorrectMaxBaseName[128];
	DWORD m_dwPixelCorrectMaxOffset;
	INT m_iPixelCorrectMaxLSB;
	INT m_iPixelCorrectMaxMSB;

	//画素欠補正
	TCHAR m_szPixelCorrectBaseName[128];
	DWORD m_dwPixelCorrectOffset;
	//TCHAR m_iPixelCorrectLSB;
	//DWORD m_iPixelCorrectMSB;
	//画素欠補正保存
	TCHAR m_szSavePixelCorrectBaseName[128];
	DWORD m_dwSavePixelCorrectOffset;
	INT m_iSavePixelCorrectLSB;
	INT m_iSavePixelCorrectMSB;
	INT m_iSavePixelCorrectValue;

	//▼1.0.0.22
	TCHAR m_szReg99BaseName[128];
	DWORD m_dwReg99Offset;
	INT m_dwReg99LSB;
	INT m_dwReg99MSB;

	TCHAR m_szReg99StatusBaseName[128];
	DWORD m_dwReg99StatusOffset;
	INT m_dwReg99StatusLSB;
	INT m_dwReg99StatusMSB;

	//FPGAAdjustmentAddress + 0x0020 (D3)--------------
	TCHAR m_szHorizontalFixedGainCorrectionModeBaseName[128];
	DWORD m_dwHorizontalFixedGainCorrectionModeOffset;
	INT m_iHorizontalFixedGainCorrectionModeLSB;
	INT m_iHorizontalFixedGainCorrectionModeMSB;

	//横筋補正
	TCHAR m_szRowGainAdjustmentBaseName[128];
	DWORD m_dwRowGainAdjustmentOffset[2];

	//▲1.0.0.22

};

