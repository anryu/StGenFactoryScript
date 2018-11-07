#include "StdAfx.h"
#include "StDeviceEBus.h"
#include <PvSerialPortIPEngine.h>

// =============================================================================
StDeviceEBus::StDeviceEBus(void)
	: mPipeline( &mStream )
	, mIPAddressStr("")
	, mMACAddressStr("")
	//▼1.0.0.1017
	, mMACAddressCapitalStr("")
	//▲1.0.0.1017
	, mSubnetMaskStr("")
	, mCallbackThread(NULL)
    , mAcquisitionStateManager( NULL )
	//▼1.0.0.1060
	, m_nSerialControlDelayTime(500)
	//▲1.0.0.1060
{
}

StDeviceEBus::StDeviceEBus(LPCTSTR szClassName, LPCTSTR szVarName)
	: StDeviceBase( szClassName, szVarName)
	, mPipeline( &mStream )
	, mIPAddressStr("")
	, mMACAddressStr("")
	//▼1.0.0.1017
	, mMACAddressCapitalStr("")
	//▲1.0.0.1017
	, mSubnetMaskStr("")
	, mCallbackThread(NULL)
    , mAcquisitionStateManager( NULL )
	//▼1.0.0.1060
	, m_nSerialControlDelayTime(500)
	//▲1.0.0.1060
	////▼1.0.0.1067
	//, m_nAutoConnect(0)
	//, m_nAutoConnectTimeout(0)
	////▲1.0.0.1067
{
}

// =============================================================================
StDeviceEBus::~StDeviceEBus(void)
{
	if( IsStarted() )
	{
		StopAcquisition();
	}

	if( IsConnected() )
	{
		DisconnectCustom();
	}

	UnregisterCallback();
    if ( mAcquisitionStateManager != NULL )
    {
        delete mAcquisitionStateManager;
        mAcquisitionStateManager = NULL;
    }
}

// =============================================================================
BOOL StDeviceEBus::ConnectCustom( PvDeviceInfo *aDI )
{
	//ASSERT( aDI != NULL );
	if ( aDI == NULL )	
    {
		return FALSE;
	}
	
	// Just in case we came here still connected...
	DisconnectCustom();

    // Device connection, packet size negociation and stream opening
	PvResult lResult = PvResult::Code::NOT_CONNECTED;

    // Connect device
	lResult = Connect( aDI, PvAccessControl );
    if ( !lResult.IsOK() )
    {
		DisconnectCustom();
        return FALSE;
    }

    // Perform automatic packet size negociation
    lResult = NegotiatePacketSize( 0, 1440 );
    if ( !lResult.IsOK() )
    {
        ::Sleep( 2500 );
    }

   // Open stream - and retry if it fails
	for ( ;; )
	{
        // Open stream
        lResult = mStream.Open( aDI->GetIPAddress() );
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
		DisconnectCustom();

		return FALSE;
	}

    // Register to all events of the parameters in the device's node map
    PvGenParameterArray *lGenDevice = GetGenParameters();

	// IP
	//TCHAR szIPAddress[64];
	//PvInt64 lIPAddress = 0;
	//lGenDevice->GetIntegerValue( "GevCurrentIPAddress", lIPAddress );
	//unsigned char *lIPPtr = reinterpret_cast<unsigned char *>( &lIPAddress );
	//_stprintf_s(szIPAddress,_countof(szIPAddress),_T("%i.%i.%i.%i"), lIPPtr[3], lIPPtr[2], lIPPtr[1], lIPPtr[0] );
	//mIPAddressStr = szIPAddress;
	mIPAddressStr = aDI->GetIPAddress();

	//▼1.0.0.1016　MACアドレス大文字のため取得部分変更
	// MAC address
	TCHAR szMACAddress[64];
	PvInt64 lMACAddress;
	lGenDevice->GetIntegerValue( "GevMACAddress", lMACAddress );
	unsigned char *lMACPtr = reinterpret_cast<unsigned char *>( &lMACAddress );
	_stprintf_s(szMACAddress,_countof(szMACAddress),_T("%02X:%02X:%02X:%02X:%02X:%02X"), lMACPtr[5], lMACPtr[4], lMACPtr[3], lMACPtr[2], lMACPtr[1], lMACPtr[0] );
	//▼1.0.0.1017
	//mMACAddressStr = szMACAddress;
	mMACAddressCapitalStr = szMACAddress;
	//▲1.0.0.1017
	//▲1.0.0.1016　MACアドレス大文字のため取得部分変更
	mMACAddressStr = aDI->GetMACAddress();

	//SubnetMask
	mSubnetMaskStr = aDI->GetSubnetMask();


	//DeviceModelName
	PvString szDeviceModelName;
	lGenDevice->GetString( _T("DeviceModelName"), szDeviceModelName );
	SetDeviceModelName(szDeviceModelName.GetUnicode());

	//SerialNumber
	PvString szDeviceID;
	lResult = lGenDevice->GetString( _T("DeviceID"), szDeviceID );
	if( !lResult.IsOK() )
	{
		lResult = lGenDevice->GetString( _T("DeviceSerialNumber"), szDeviceID );
	}
	SetSerialNumber(szDeviceID.GetUnicode());


    // Create acquisition state manager
    ASSERT( mAcquisitionStateManager == NULL );
    mAcquisitionStateManager = new PvAcquisitionStateManager( this, &mStream );

	StartStreaming();

	return TRUE;
}


// =============================================================================
BOOL StDeviceEBus::DisconnectCustom()
{
	// If streaming, stop streaming
//TRACE(TEXT("@@@@@@@@@ StopStreaming IN\n") );
	StopStreaming();

    // Release acquisition state manager
    if ( mAcquisitionStateManager != NULL )
    {
 //TRACE(TEXT("@@@@@@@@@ mAcquisitionStateManager IN\n") );
       delete mAcquisitionStateManager;
        mAcquisitionStateManager = NULL;
    }

 //TRACE(TEXT("@@@@@@@@@ Disconnect IN\n") );
	Disconnect();
    
 //TRACE(TEXT("@@@@@@@@@ Close IN\n") );
	mStream.Close();
	
	//Disconnectしても初期化しない。次回Connect時に使用する
	//mIPAddressStr = "";
	//mMACAddressStr = "";

	return TRUE;
}


// =============================================================================
void StDeviceEBus::StartStreaming()
{
	// Create display thread
	mCallbackThread = new CallbackThread( this );

	// Start threads
	mCallbackThread->Start();
	mCallbackThread->SetPriority( THREAD_PRIORITY_ABOVE_NORMAL );

    // Start pipeline
    mPipeline.Start();

//	Sleep(100);	//mPipeline.Start後にAcquisitionStartするまでに時間をおいたほうがいい。？？

}

// =============================================================================
void StDeviceEBus::StopStreaming()
{
    // Stop display thread
	if ( mCallbackThread != NULL )
	{
		mCallbackThread->Stop();

		delete mCallbackThread;
		mCallbackThread = NULL;
	}

	// Stop stream thread
    if ( mPipeline.IsStarted() )
    {
		mPipeline.Stop();
    }
}

// =============================================================================
void StDeviceEBus::OnImageCallback(PvBuffer *aBuffer)
{
	if( m_Listener )
	{

		//PvBufferからStBufferに変換
		StBuffer aConvBuffer;	//表示処理をOnTimer処理にした場合は、m_ConvBufferを使用して常に残しておく必要あり。。。。
//TRACE(TEXT("@@@@@@@@@ aBuffer size(%i) aType=0x%x aData=%d(0x%x)\n"),aBuffer->GetImage()->GetImageSize(), aBuffer->GetImage()->GetPixelType() , *(PWORD)aBuffer->GetImage()->GetDataPointer(), aBuffer->GetImage()->GetDataPointer() );

		//ConvertToStBufferFromPvBuffer(&m_ConvBuffer, aBuffer);
		ConvertToStBufferFromPvBuffer(&aConvBuffer, aBuffer);
//StPixelType aType;
//aType = m_ConvBuffer.GetPixelType();
//PBYTE aData = (PBYTE)m_ConvBuffer.GetDataPointer();
//TRACE(TEXT("@@@@@@@@@ m_ConvBuffer size(%i) aType=0x%x data=%d(0x%x)\n"),m_ConvBuffer.GetSize(), aType, *(PWORD)aData, aData );

		//OnStBufferCallback(&m_ConvBuffer);
		OnStBufferCallback(&aConvBuffer);

		//m_Listener->Invoke(&aConvBuffer,m_pUser);

		//m_Listener->Invoke(aBuffer,m_pUser);

		//使用後Detach
		//m_ConvBuffer.Detach();
		aConvBuffer.Detach();
	}
}

// =============================================================================
void StDeviceEBus::GetIPAddressStr( PvString &aString )
{
	aString = mIPAddressStr;
}

// =============================================================================
void StDeviceEBus::GetMACAddressStr( PvString &aString )
{
	aString = mMACAddressStr;
}

//▼1.0.0.1017
// =============================================================================
void StDeviceEBus::GetMACAddressCapitalStr( PvString &aString )
{
	aString = mMACAddressCapitalStr;
}
//▲1.0.0.1017


// =============================================================================
void StDeviceEBus::GetSubnetMaskStr( PvString &aString )
{
	aString = mSubnetMaskStr;
}

// =============================================================================
void StDeviceEBus::StartAcquisition(void)
{

    PvInt64 lPayloadSizeValue = GetPayloadSize();

    // If payload size is valid, force buffers re-alloc - better than 
    // adjusting as images are comming in
    if ( lPayloadSizeValue > 0 )
    {
        mPipeline.SetBufferSize( static_cast<PvUInt32>( lPayloadSizeValue ) );
    }

    // Never hurts to start streaming on a fresh pipeline/stream...
    mPipeline.Reset();

    // Reset stream statistics
    mStream.GetParameters()->ExecuteCommand( "Reset" );

    ASSERT( mAcquisitionStateManager != NULL );
    mAcquisitionStateManager->Start();
}

// =============================================================================
void StDeviceEBus::StopAcquisition(void)
{
    ASSERT( mAcquisitionStateManager != NULL );
    mAcquisitionStateManager->Stop();
}

// =============================================================================
bool StDeviceEBus::IsStarted(void)
{
    if( !mAcquisitionStateManager ) return FALSE;

    return mAcquisitionStateManager->GetState() == PvAcquisitionStateLocked;
}

// ==========================================================================
PvUInt64 StDeviceEBus::GetPayloadSize()
{
    if ( !IsConnected() )
    {
        return 0;
    }

    PvGenInteger *lPayloadSize = GetGenParameters()->GetInteger( "PayloadSize" );

    PvInt64 lPayloadSizeValue = 0;
    if ( lPayloadSize != NULL )
    {
        lPayloadSize->GetValue( lPayloadSizeValue );
    }
	return lPayloadSizeValue;
}

// ==========================================================================
StPixelType GetStPixelType(PvPixelType aType)
{
	StPixelType aPixelType = StPixelUndefined;
	switch(aType)
	{
	case PvPixelMono8:
		aPixelType = StPixelMono8;
		break;
	case PvPixelMono8s:
		aPixelType = StPixelMono8Signed;
		break;
	case PvPixelMono10:
		aPixelType = StPixelMono10;
		break;
	case PvPixelMono10Packed:
		aPixelType = StPixelMono10Packed;
		break;
	case PvPixelMono12:
		aPixelType = StPixelMono12;
		break;
	case PvPixelMono12Packed:
		aPixelType = StPixelMono12Packed;
		break;
	case PvPixelMono14:
		aPixelType = StPixelMono14;
		break;
	case PvPixelMono16:
		aPixelType = StPixelMono16;
		break;
	case PvPixelBayerGR8:
		aPixelType = StPixelBayerGR8;
		break;
	case PvPixelBayerRG8:
		aPixelType = StPixelBayerRG8;
		break;
	case PvPixelBayerGB8:
		aPixelType = StPixelBayerGB8;
		break;
	case PvPixelBayerBG8:
		aPixelType = StPixelBayerBG8;
		break;
	case PvPixelBayerGR10:
		aPixelType = StPixelBayerGR10;
		break;
	case PvPixelBayerRG10:
		aPixelType = StPixelBayerRG10;
		break;
	case PvPixelBayerGB10:
		aPixelType = StPixelBayerGB10;
		break;
	case PvPixelBayerBG10:
		aPixelType = StPixelBayerBG10;
		break;
	case PvPixelBayerGR12:
		aPixelType = StPixelBayerGR12;
		break;
	case PvPixelBayerRG12:
		aPixelType = StPixelBayerRG12;
		break;
	case PvPixelBayerGB12:
		aPixelType = StPixelBayerGB12;
		break;
	case PvPixelBayerBG12:
		aPixelType = StPixelBayerBG12;
		break;
	case PvPixelBayerGR10Packed:
		aPixelType = StPixelBayerGR10Packed;
		break;
	case PvPixelBayerRG10Packed:
		aPixelType = StPixelBayerRG10Packed;
		break;
	case PvPixelBayerGB10Packed:
		aPixelType = StPixelBayerGB10Packed;
		break;
	case PvPixelBayerBG10Packed:
		aPixelType = StPixelBayerBG10Packed;
		break;
	case PvPixelBayerGR12Packed:
		aPixelType = StPixelBayerGR12Packed;
		break;
	case PvPixelBayerRG12Packed:
		aPixelType = StPixelBayerRG12Packed;
		break;
	case PvPixelBayerGB12Packed:
		aPixelType = StPixelBayerGB12Packed;
		break;
	case PvPixelBayerBG12Packed:
		aPixelType = StPixelBayerBG12Packed;
		break;
	case PvPixelBayerGR16:
		aPixelType = StPixelBayerGR16;
		break;
	case PvPixelBayerRG16:
		aPixelType = StPixelBayerRG16;
		break;
	case PvPixelBayerGB16:
		aPixelType = StPixelBayerGB16;
		break;
	case PvPixelBayerBG16:
		aPixelType = StPixelBayerBG16;
		break;
	case PvPixelRGB8:
		aPixelType = StPixelRGB8Packed;
		break;
	case PvPixelBGR8:
		aPixelType = StPixelBGR8;
		break;
	case PvPixelRGBa8:
		aPixelType = StPixelRGBA8Packed;
		break;
	case PvPixelBGRa8:
		aPixelType = StPixelBGRa8;
		break;
	case PvPixelRGB10:
		aPixelType = StPixelRGB10Packed;
		break;
	case PvPixelBGR10:
		aPixelType = StPixelBGR10;
		break;
	case PvPixelRGB12:
		aPixelType = StPixelRGB12Packed;
		break;
	case PvPixelBGR12:
		aPixelType = StPixelBGR12;
		break;
	case PvPixelRGB16:
		aPixelType = StPixelRGB16Packed;
		break;
	};

	return aPixelType;

}

BOOL StDeviceEBus::ConvertToStBufferFromPvBuffer( StBuffer *aOutBuffer, PvBuffer *aInBuffer )
{
	BOOL bReval = FALSE;

	StPixelType aPixelType = GetStPixelType(aInBuffer->GetImage()->GetPixelType());
	if( aPixelType==StPixelUndefined )
	{
//TRACE(TEXT("@@@GetPixelType(0x%x)\n"),aInBuffer->GetImage()->GetPixelType());
		return bReval;
	}
	//aOutBuffer->Attach(aInBuffer->GetDataPointer(),aInBuffer->GetSize());	//最初でなければならない
	aOutBuffer->Attach(aInBuffer->GetDataPointer(),aInBuffer->GetImage()->GetImageSize());	//最初でなければならない
	aOutBuffer->SetPixelType(aPixelType);
	aOutBuffer->SetWidth(aInBuffer->GetImage()->GetWidth());
	aOutBuffer->SetHeight(aInBuffer->GetImage()->GetHeight());
	UINT uiBitsPerLine = aInBuffer->GetImage()->GetPixelSize(aInBuffer->GetImage()->GetPixelType()) * aInBuffer->GetImage()->GetWidth();
	UINT uiBytesPerLine = uiBitsPerLine/8 + ((uiBitsPerLine%8)>0 ?1:0);
	aOutBuffer->SetLinePitch(uiBytesPerLine);
	//aOutBuffer->SetSize(aInBuffer->GetSize());
	aOutBuffer->SetSize(aInBuffer->GetImage()->GetImageSize());
	aOutBuffer->SetID((StInt64)aInBuffer->GetBlockID());
	aOutBuffer->SetTimestamp((StInt64)aInBuffer->GetTimestamp());

	return bReval;
}
BOOL StDeviceEBus::SerialControl( LPCTSTR szPort, LPCTSTR szData, PBYTE pbyteReadData, size_t *pReadSize )
{
	BOOL bReval = FALSE;

	//szData文字列をバイト変換
	CString szSendData(szData);
	//スペースと,を除外
	szSendData.Remove(' ');
	szSendData.Remove('　');
	szSendData.Remove(',');

	if( (szSendData.GetLength()&1)!=0 ) return bReval;
	size_t sendSize = szSendData.GetLength()/2;
	PBYTE byteData = new BYTE [sendSize];
	if( !byteData ) return bReval;
	for( int i=0; i<sendSize; i++ )
	{
		byteData[i] = (BYTE)_tcstol( szSendData.Mid(i*2, 2), nullptr, 16 );
	}
	bReval = SerialControl( szPort, byteData, sendSize, pbyteReadData, pReadSize );
	/*
	PvSerialPortIPEngine aSerial;
	PvIPEngineSerial aPort = PvIPEngineSerial0;
	if( _tcscmp(szPort,_T("UART0"))==0 )
	{
		aPort = PvIPEngineSerial0;
	}
	else
	if( _tcscmp(szPort,_T("UART1"))==0 )
	{
		aPort = PvIPEngineSerial1;
	}
	else
	if( _tcscmp(szPort,_T("BULK1"))==0 )
	{
		aPort = PvIPEngineSerialBulk0;
	}
	PvResult aResult = aSerial.Open( this, aPort );
	if( aResult.IsOK() )
	{
		//Write
		PvUInt32 aByteWritten;
		bReval = aSerial.Write(byteData,sendSize,aByteWritten).IsOK();
		if( sendSize!=aByteWritten ) bReval = FALSE;

		//Read
		size_t bufferSize = 0;
		if( pReadSize )
		{
			bufferSize = *pReadSize;
		}
		PvUInt32 aBytes,aBytesRead;
		for( int i=0; i<5; i++ )
		{
			Sleep(50);	//一応50mSec
			aSerial.GetRxBytesReady(aBytes);
			if( bufferSize==0 || aBytes==bufferSize )
				break;
		}


		PBYTE byteReadData = new BYTE [aBytes];
		aSerial.Read(byteReadData,aBytes,aBytesRead);
		if( pbyteReadData && pReadSize )
		{
			if( aBytesRead<=bufferSize )
			{
				memcpy( pbyteReadData, byteReadData, aBytesRead );
				*pReadSize = aBytesRead;
			}
			else
			{
				*pReadSize = 0;
			}
		}
		delete [] byteReadData;

		aSerial.Close();
	}
	*/
	delete [] byteData;

	return bReval;

}
BOOL StDeviceEBus::SerialControl( LPCTSTR szPort, PBYTE pbyteSendData, size_t sendSize, PBYTE pbyteReadData, size_t *pReadSize )
{
	BOOL bReval = FALSE;

	PvSerialPortIPEngine aSerial;
	PvIPEngineSerial aPort = PvIPEngineSerial0;
	if( _tcscmp(szPort,_T("UART0"))==0 )
	{
		aPort = PvIPEngineSerial0;
	}
	else
	if( _tcscmp(szPort,_T("UART1"))==0 )
	{
		aPort = PvIPEngineSerial1;
	}
	else
	if( _tcscmp(szPort,_T("BULK0"))==0 )
	{
		aPort = PvIPEngineSerialBulk0;
	}
	PvResult aResult = aSerial.Open( this, aPort );

//{
//TCHAR szText[128];
//_stprintf_s(szText,_countof(szText),_T("  aSerial.Open(0x%x)=%d aPort=%d\n"), (PvDevice *)this, aResult.IsOK(), aPort);
//OutputDebugString(szText);
//}
//if( !aResult.IsOK() ){
//TCHAR szText[128];
//_stprintf_s(szText,_countof(szText),_T("  aSerial.OpenResult=%s (%s)\n"), aResult.GetCodeString().GetUnicode(), aResult.GetDescription().GetUnicode());
//OutputDebugString(szText);
//}

	if( aResult.IsOK() )
	{
		//Write
		PvUInt32 aByteWritten;
		bReval = aSerial.Write(pbyteSendData,sendSize,aByteWritten).IsOK();
		if( sendSize!=aByteWritten ) bReval = FALSE;

		//Read
		size_t bufferSize = 0;
		if( pReadSize )
		{
			bufferSize = *pReadSize;
		}
		PvUInt32 aBytes,aBytesRead;
		//for( int i=0; i<5; i++ )
		//10回にしました 10 x50mSec = 500mSec
		//▼1.0.0.1060
		//for( int i=0; i<10; i++ )
		INT nOnceDelayTime = 50;
		INT nLoop = m_nSerialControlDelayTime/nOnceDelayTime;
		for( int i=0; i<nLoop; i++, Sleep(nOnceDelayTime) )
		//▲1.0.0.1060
		{
			//▼1.0.0.1060
			//Sleep(50);	//一応50mSec
			//▲1.0.0.1060
			aSerial.GetRxBytesReady(aBytes);
//{
//TCHAR szText[128];
//_stprintf_s(szText,_countof(szText),_T("[%i] aBytes=%i bufferSize=%i\n"), i,aBytes, bufferSize);
//OutputDebugString(szText);
//}
			if( bufferSize==0 || aBytes==bufferSize )
				break;
		}


		PBYTE byteReadData = new BYTE [aBytes];
		aSerial.Read(byteReadData,aBytes,aBytesRead);
		if( pbyteReadData && pReadSize )
		{
//{
//TCHAR szText[128];
//_stprintf_s(szText,_countof(szText),_T("  aBytesRead=%i\n"), aBytesRead);
//OutputDebugString(szText);
//}
			if( aBytesRead<=bufferSize )
			{
				memcpy( pbyteReadData, byteReadData, aBytesRead );
				*pReadSize = aBytesRead;
			}
			else
			{
				*pReadSize = 0;
			}
		}
		delete [] byteReadData;

		aSerial.Close();
	}

	return bReval;

}

//--------------------------------------------------------------------
//SetGenICamControl
//--------------------------------------------------------------------
BOOL StDeviceEBus::SetGenICamControl( LPCTSTR szFeature, LPCTSTR szData )
{
	BOOL bReval = FALSE;

	PvGenParameterArray *aArray = GetGenParameters();
	if( !aArray ) return bReval;
	PvGenParameter *aParameter = aArray->Get(szFeature);
	if( !aParameter ) return bReval;
	PvGenType aType;
	PvResult aResult = aParameter->GetType(aType);
	if( !aResult.IsOK() ) return bReval;
	switch(aType)
	{
	case PvGenTypeInteger:
		{
			PvGenInteger *aTypeInteger = (PvGenInteger *)aParameter;
			PvInt64 aData = _tcstol(szData,NULL,0);
			bReval = aTypeInteger->SetValue(aData).IsOK();
		}
		break;
	case PvGenTypeEnum:
		{
			PvGenEnum *aTypeEnum = (PvGenEnum *)aParameter;
			CString szEnumData(szData);
			//数字だけなら数値変換して設定
			CString res = szEnumData.SpanIncluding(_T("0123456789"));
			if(res == szEnumData)
			{
				PvInt64 aData = _tcstol(szData,NULL,0);
				bReval = aTypeEnum->SetValue(aData).IsOK();
			}
			else
			{
				bReval = aTypeEnum->SetValue(szData).IsOK();
			}
		}
		break;
	case PvGenTypeBoolean:
		{
			PvGenBoolean *aTypeBoolean = (PvGenBoolean *)aParameter;
			bool aData = (bool)_tcstol(szData,NULL,0);
			bReval = aTypeBoolean->SetValue(aData).IsOK();
		}
		break;
	case PvGenTypeFloat:
		{
			PvGenFloat *aTypeFloat = (PvGenFloat *)aParameter;
			double aData = (double)_ttof(szData);
			bReval = aTypeFloat->SetValue(aData).IsOK();
		}
		break;
	case PvGenTypeString:
		{
			PvGenString *aTypeString = (PvGenString *)aParameter;
			bReval = aTypeString->SetValue(szData).IsOK();
		}
		break;
	case PvGenTypeCommand:
		{
			PvGenCommand *aTypeCommand = (PvGenCommand *)aParameter;
			bReval = aTypeCommand->Execute().IsOK();
		}
		break;
	default:
		break;
	}
	return bReval;

}

BOOL StDeviceEBus::SetGenICamControl( LPCTSTR szFeature, INT nData )
{
	BOOL bReval = FALSE;

	PvGenParameterArray *aArray = GetGenParameters();
	if( !aArray ) return bReval;
	PvGenParameter *aParameter = aArray->Get(szFeature);
	if( !aParameter ) return bReval;
	PvGenType aType;
	PvResult aResult = aParameter->GetType(aType);
	if( !aResult.IsOK() ) return bReval;
	switch(aType)
	{
	case PvGenTypeInteger:
		{
			PvGenInteger *aTypeInteger = (PvGenInteger *)aParameter;
			bReval = aTypeInteger->SetValue(nData).IsOK();
		}
		break;
	case PvGenTypeEnum:
		{
			PvGenEnum *aTypeEnum = (PvGenEnum *)aParameter;
			bReval = aTypeEnum->SetValue(nData).IsOK();
		}
		break;
	case PvGenTypeBoolean:
		{
			PvGenBoolean *aTypeBoolean = (PvGenBoolean *)aParameter;
			bReval = aTypeBoolean->SetValue((bool)nData).IsOK();
		}
		break;
	case PvGenTypeFloat:
		{
			PvGenFloat *aTypeFloat = (PvGenFloat *)aParameter;
			bReval = aTypeFloat->SetValue((double)nData).IsOK();
		}
		break;
	case PvGenTypeCommand:
		{
			PvGenCommand *aTypeCommand = (PvGenCommand *)aParameter;
			bReval = aTypeCommand->Execute().IsOK();
		}
		break;
	default:
		break;
	}
	return bReval;

}

BOOL StDeviceEBus::SetGenICamControl( LPCTSTR szFeature, double dblData )
{
	BOOL bReval = FALSE;

	PvGenParameterArray *aArray = GetGenParameters();
	if( !aArray ) return bReval;
	PvGenParameter *aParameter = aArray->Get(szFeature);
	if( !aParameter ) return bReval;
	PvGenType aType;
	PvResult aResult = aParameter->GetType(aType);
	if( !aResult.IsOK() ) return bReval;
	switch(aType)
	{
	case PvGenTypeInteger:
		{
			PvGenInteger *aTypeInteger = (PvGenInteger *)aParameter;
			bReval = aTypeInteger->SetValue((INT)dblData).IsOK();
		}
		break;
	case PvGenTypeEnum:
		{
			PvGenEnum *aTypeEnum = (PvGenEnum *)aParameter;
			bReval = aTypeEnum->SetValue((INT)dblData).IsOK();
		}
		break;
	case PvGenTypeFloat:
		{
			PvGenFloat *aTypeFloat = (PvGenFloat *)aParameter;
			bReval = aTypeFloat->SetValue(dblData).IsOK();
		}
		break;
	case PvGenTypeCommand:
		{
			PvGenCommand *aTypeCommand = (PvGenCommand *)aParameter;
			bReval = aTypeCommand->Execute().IsOK();
		}
		break;
	default:
		break;
	}
	return bReval;

}

BOOL StDeviceEBus::SetGenICamControl( LPCTSTR szFeature, bool bData )
{
	BOOL bReval = FALSE;

	PvGenParameterArray *aArray = GetGenParameters();
	if( !aArray ) return bReval;
	PvGenParameter *aParameter = aArray->Get(szFeature);
	if( !aParameter ) return bReval;
	PvGenType aType;
	PvResult aResult = aParameter->GetType(aType);
	if( !aResult.IsOK() ) return bReval;
	switch(aType)
	{
	case PvGenTypeBoolean:
		{
			PvGenBoolean *aTypeBoolean = (PvGenBoolean *)aParameter;
			bReval = aTypeBoolean->SetValue(bData).IsOK();
		}
		break;
	case PvGenTypeCommand:
		{
			PvGenCommand *aTypeCommand = (PvGenCommand *)aParameter;
			bReval = aTypeCommand->Execute().IsOK();
		}
		break;
	default:
		break;
	}
	return bReval;

}

//--------------------------------------------------------------------
//GetGenICamControl
//--------------------------------------------------------------------
BOOL StDeviceEBus::GetGenICamControl( LPCTSTR szFeature, LPTSTR szData, size_t size )
{
	BOOL bReval = FALSE;

	PvGenParameterArray *aArray = GetGenParameters();
	if( !aArray ) return bReval;
	PvGenParameter *aParameter = aArray->Get(szFeature);
	if( !aParameter ) return bReval;
	PvGenType aType;
	PvResult aResult = aParameter->GetType(aType);
	if( !aResult.IsOK() ) return bReval;
	PvString aString;
	switch(aType)
	{
	case PvGenTypeInteger:
		{
			bReval = FALSE;
		}
		break;
	case PvGenTypeEnum:
		{
			PvGenEnum *aTypeEnum = (PvGenEnum *)aParameter;
			bReval = aTypeEnum->GetValue(aString).IsOK();
			if( bReval )
			{
				_tcscpy_s( szData, size, aString.GetUnicode() );
			}
		}
		break;
	case PvGenTypeBoolean:
		{
			bReval = FALSE;
		}
		break;
	case PvGenTypeFloat:
		{
			bReval = FALSE;
		}
		break;
	case PvGenTypeString:
		{
			PvGenString *aTypeString = (PvGenString *)aParameter;
			bReval = aTypeString->GetValue(aString).IsOK();
			if( bReval )
			{
				_tcscpy_s( szData, size, aString.GetUnicode() );
			}
		}
		break;
	case PvGenTypeCommand:
		{
			bReval = FALSE;
		}
		break;
	default:
		break;
	}
	return bReval;

}

BOOL StDeviceEBus::GetGenICamControl( LPCTSTR szFeature, INT &nData )
{
	BOOL bReval = FALSE;

	PvGenParameterArray *aArray = GetGenParameters();
	if( !aArray ) return bReval;
	PvGenParameter *aParameter = aArray->Get(szFeature);
	if( !aParameter ) return bReval;
	PvGenType aType;
	PvResult aResult = aParameter->GetType(aType);
	if( !aResult.IsOK() ) return bReval;
	PvInt64 aData;
	switch(aType)
	{
	case PvGenTypeInteger:
		{
			PvGenInteger *aTypeInteger = (PvGenInteger *)aParameter;
			bReval = aTypeInteger->GetValue(aData).IsOK();
			if( bReval )
			{
				nData = aData;
			}
		}
		break;
	case PvGenTypeEnum:
		{
			PvGenEnum *aTypeEnum = (PvGenEnum *)aParameter;
			bReval = aTypeEnum->GetValue(aData).IsOK();
			if( bReval )
			{
				nData = aData;
			}
		}
		break;
	case PvGenTypeBoolean:
		{
			PvGenBoolean *aTypeBoolean = (PvGenBoolean *)aParameter;
			bool aValue;
			bReval = aTypeBoolean->GetValue(aValue).IsOK();
			if( bReval )
			{
				nData = (INT)aValue;
			}
		}
		break;
	case PvGenTypeFloat:
		{
			bReval = FALSE;
		}
		break;
	default:
		break;
	}
	return bReval;

}

BOOL StDeviceEBus::GetGenICamControl( LPCTSTR szFeature, double &dblData )
{
	BOOL bReval = FALSE;

	PvGenParameterArray *aArray = GetGenParameters();
	if( !aArray ) return bReval;
	PvGenParameter *aParameter = aArray->Get(szFeature);
	if( !aParameter ) return bReval;
	PvGenType aType;
	PvResult aResult = aParameter->GetType(aType);
	if( !aResult.IsOK() ) return bReval;
	switch(aType)
	{
	case PvGenTypeInteger:
		{
			bReval = FALSE;
		}
		break;
	case PvGenTypeEnum:
		{
			bReval = FALSE;
		}
		break;
	case PvGenTypeFloat:
		{
			PvGenFloat *aTypeFloat = (PvGenFloat *)aParameter;
			bReval = aTypeFloat->GetValue(dblData).IsOK();
		}
		break;
	default:
		break;
	}
	return bReval;

}

BOOL StDeviceEBus::GetGenICamControl( LPCTSTR szFeature, bool &bData )
{
	BOOL bReval = FALSE;

	PvGenParameterArray *aArray = GetGenParameters();
	if( !aArray ) return bReval;
	PvGenParameter *aParameter = aArray->Get(szFeature);
	if( !aParameter ) return bReval;
	PvGenType aType;
	PvResult aResult = aParameter->GetType(aType);
	if( !aResult.IsOK() ) return bReval;
	switch(aType)
	{
	case PvGenTypeBoolean:
		{
			PvGenBoolean *aTypeBoolean = (PvGenBoolean *)aParameter;
			bReval = aTypeBoolean->GetValue(bData).IsOK();
		}
		break;
	default:
		break;
	}
	return bReval;

}

//▼1.0.0.1029
BOOL StDeviceEBus::GetFunction( LPCTSTR szFunc, LPCTSTR szArgument, LPTSTR szData, size_t size )
{
	BOOL bReval = FALSE;
	CString szFeature = szFunc;
	szFeature.Trim();
	CString szArg = szArgument;
	szArg.Trim();
	if( szFeature==_T("DataStream") )
	{
		PvStream *aStream = GetStream();
		PvGenParameterArray *aArray = aStream->GetParameters();
		if( !aArray ) return bReval;
		PvGenParameter *aParameter = aArray->Get(PvString(szArg));
		if( !aParameter ) return bReval;
		PvGenType aType;
		PvResult aResult = aParameter->GetType(aType);
		if( !aResult.IsOK() ) return bReval;
		PvString aString;
		switch(aType)
		{
		case PvGenTypeInteger:
			{
				bReval = FALSE;
			}
			break;
		case PvGenTypeEnum:
			{
				PvGenEnum *aTypeEnum = (PvGenEnum *)aParameter;
				bReval = aTypeEnum->GetValue(aString).IsOK();
				if( bReval )
				{
					_tcscpy_s( szData, size, aString.GetUnicode() );
				}
			}
			break;
		case PvGenTypeBoolean:
			{
				bReval = FALSE;
			}
			break;
		case PvGenTypeFloat:
			{
				bReval = FALSE;
			}
			break;
		case PvGenTypeString:
			{
				PvGenString *aTypeString = (PvGenString *)aParameter;
				bReval = aTypeString->GetValue(aString).IsOK();
				if( bReval )
				{
					_tcscpy_s( szData, size, aString.GetUnicode() );
				}
			}
			break;
		case PvGenTypeCommand:
			{
				bReval = FALSE;
			}
			break;
		default:
			break;
		}
	}
	//▼1.0.0.1071
	else
	if( szFeature==_T("GetMACAddressStr") || szFeature==_T("MACAddressStr") )
	{
		_tcscpy_s( szData, size, mMACAddressStr.GetUnicode() );
		bReval = TRUE;
	}
	else
	if( szFeature==_T("GetIPAddressStr") || szFeature==_T("IPAddressStr") )
	{
		_tcscpy_s( szData, size, mIPAddressStr.GetUnicode() );
		bReval = TRUE;
	}
	else
	if( szFeature==_T("GetSubnetMaskStr") || szFeature==_T("SubnetMaskStr") )
	{
		_tcscpy_s( szData, size, mSubnetMaskStr.GetUnicode() );
		bReval = TRUE;
	}
	//▲1.0.0.1071
	return bReval;

}

BOOL StDeviceEBus::GetFunction( LPCTSTR szFunc, LPCTSTR szArgument, INT &nData )
{
	BOOL bReval = FALSE;
	CString szFeature = szFunc;
	szFeature.Trim();
	CString szArg = szArgument;
	szArg.Trim();
	if( szFeature==_T("DataStream") )
	{
		PvStream *aStream = GetStream();
		PvGenParameterArray *aArray = aStream->GetParameters();

		if( !aArray ) return bReval;
		PvGenParameter *aParameter = aArray->Get(PvString(szArg));
		if( !aParameter ) return bReval;
		PvGenType aType;
		PvResult aResult = aParameter->GetType(aType);
		if( !aResult.IsOK() ) return bReval;
		PvInt64 aData;
		switch(aType)
		{
		case PvGenTypeInteger:
			{
				PvGenInteger *aTypeInteger = (PvGenInteger *)aParameter;
				bReval = aTypeInteger->GetValue(aData).IsOK();
				if( bReval )
				{
					nData = aData;
				}
			}
			break;
		case PvGenTypeEnum:
			{
				PvGenEnum *aTypeEnum = (PvGenEnum *)aParameter;
				bReval = aTypeEnum->GetValue(aData).IsOK();
				if( bReval )
				{
					nData = aData;
				}
			}
			break;
		case PvGenTypeBoolean:
			{
				PvGenBoolean *aTypeBoolean = (PvGenBoolean *)aParameter;
				bool aValue;
				bReval = aTypeBoolean->GetValue(aValue).IsOK();
				if( bReval )
				{
					nData = (INT)aValue;
				}
			}
			break;
		case PvGenTypeFloat:
			{
				bReval = FALSE;
			}
			break;
		default:
			break;
		}
	}
	return bReval;

}


BOOL StDeviceEBus::GetFunction( LPCTSTR szFunc, LPCTSTR szArgument, double &dblData )
{
	BOOL bReval = FALSE;
	CString szFeature = szFunc;
	szFeature.Trim();
	CString szArg = szArgument;
	szArg.Trim();
	if( szFeature==_T("DataStream") )
	{
		PvStream *aStream = GetStream();
		PvGenParameterArray *aArray = aStream->GetParameters();

		if( !aArray ) return bReval;
		PvGenParameter *aParameter = aArray->Get(PvString(szArg));
		if( !aParameter ) return bReval;
		PvGenType aType;
		PvResult aResult = aParameter->GetType(aType);
		if( !aResult.IsOK() ) return bReval;
		switch(aType)
		{
		case PvGenTypeInteger:
			{
				bReval = FALSE;
			}
			break;
		case PvGenTypeEnum:
			{
				bReval = FALSE;
			}
			break;
		case PvGenTypeFloat:
			{
				PvGenFloat *aTypeFloat = (PvGenFloat *)aParameter;
				bReval = aTypeFloat->GetValue(dblData).IsOK();
			}
			break;
		default:
			break;
		}




	}



	return bReval;

}
//▲1.0.0.1029

//▼1.0.0.1060
BOOL StDeviceEBus::function( LPCTSTR szFunc, LPCTSTR szArgument )
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="SetSerialControlDelayTime" )
	{
		m_nSerialControlDelayTime = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}

	//▼1.0.0.1072d
#ifdef GENCOMP_OLD
#else
	else
	if( szFunction=="SetCheckGenICamControlMode" || szFunction=="CheckGenICamControlMode" )
	{
		m_nCheckGenICamControlMode = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetCheckGenICamControlValue" || szFunction=="CheckGenICamControlValue" )
	{
		CString szArg(szArgument);
		if( szArg.Left(1).Compare(_T("\""))==0 )
		{
			//文字列
			szArg = szArg.Mid(1);
			if( szArg.Right(1)==_T("\"") )
				szArg = szArg.Left(szArg.GetLength()-1);
			m_nCheckGenICamControlValueMode = 1;
			m_szCheckGenICamControlValue = (LPCTSTR)szArg;
		}
		else
		{
			//数値
			m_nCheckGenICamControlValueMode = 0;
			m_dblCheckGenICamControlValue = _ttof(szArg);
		}
		bReval = TRUE;
	}
#endif
	//▲1.0.0.1072d


	return bReval;

}
//▲1.0.0.1060


