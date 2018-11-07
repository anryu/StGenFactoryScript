#include "StdAfx.h"
#include "StDeviceMultiCam.h"
#include "Common.h"


INT g_MultiCamOpen = 0;

void WINAPI GlobalCallback(PMCSIGNALINFO SigInfo)
{
	if( SigInfo && SigInfo->Context )
	{
		StDeviceMultiCam *aDevice = (StDeviceMultiCam *)SigInfo->Context;
		aDevice->OnImageCallback( SigInfo );
	}
}

//▼1.0.0.1043
//RGBPL -> BGRに変換
template <class X>BOOL ChangeFormatRGBPLColor( X* pxOutBuffer, const X* pxInBuffer, INT64 nColorFormat, INT nSizeX, INT nSizeY )
{
	BOOL bReval = FALSE;
	for( int iColor=0; iColor<3; iColor++ )
	{
		const X* pInTop = pxInBuffer + nSizeX * nSizeY * iColor;

		X* pOutTop = pxOutBuffer + (2-iColor);
		for( int y=0; y<nSizeY; y++ )
		{
			for( int x=0; x<nSizeX; x++ )
			{
				*pOutTop = *pInTop++;
				pOutTop += 3;
			}
		}
	}
	return bReval;
}
//▲1.0.0.1043


StDeviceMultiCam::StDeviceMultiCam(void)
{
	initialize();
}

StDeviceMultiCam::StDeviceMultiCam(LPCTSTR szClassName, LPCTSTR szVarName)
	: StDeviceBase( szClassName, szVarName)
{
	initialize();
}

void StDeviceMultiCam::initialize(void)
{
	DriverOpen();	//コンストラクタでのみ使用
	m_hChannel = NULL;

	//m_pChannel = NULL;
	m_szCameraFile = "";
	m_szCameraFilePath = "";
	m_nBoardID = 0;
	m_nBoardTopology = 0;
	m_szConnect = "M";
	m_szLastError = "";
	m_szComPortName = "COM2";
	m_pSerialComm = NULL;
	//▼1.0.0.1020
	 m_pControlRetry = NULL;
	//▲1.0.0.1020
}


StDeviceMultiCam::~StDeviceMultiCam(void)
{
	UnregisterCallback();

	if( IsStarted() )
	{
		Stop();
		Sleep(1000);
	}

	if( IsOpened() )
	{
		Close();
	}

	DriverClose();	//デストラクタでのみ使用
}

void StDeviceMultiCam::GetLastError(CString &szError)
{
	szError = m_szLastError;
}

//コンストラクタでのみ使用　　-----　　DriverOpenはStGenFactoryScrit.cppでおこなった
void StDeviceMultiCam::DriverOpen(void)
{
	MCSTATUS aStatus = MC_OK;
	if( g_MultiCamOpen==0 )
	{
		//Euresys::MultiCam::Initialize();
		aStatus = McOpenDriver(NULL);
	}
	if( aStatus == MC_OK )
		g_MultiCamOpen++;
}

//デストラクタでのみ使用
void StDeviceMultiCam::DriverClose(void)
{
	if( g_MultiCamOpen==1 )
	{
		// Perform MultiCam termination
		//Euresys::MultiCam::Terminate();
		McCloseDriver();
	}
	g_MultiCamOpen--;
}

void StDeviceMultiCam::BoardOpen(void)
{


}



#define EURESYS_SURFACE_COUNT 4
BOOL StDeviceMultiCam::Open( void )
{
	if( IsOpened() )
		Close();

	BOOL bReval = FALSE;
	MCSTATUS Status;
	//BOOL bBoardInfo = FALSE;
	INT iBoardCount = 0;
	INT iBoardNoCurrent = -1;
	Status = McGetParamInt( MC_CONFIGURATION, MC_BoardCount, &iBoardCount );
	for( INT iBoardNo=0; iBoardNo<iBoardCount; iBoardNo++ )
	{
		INT iBoardType = 0;
		McGetParamInt( MC_BOARD + iBoardNo, MC_BoardType, &iBoardType );
		//Boards[iBoardNo]->GetParam( MC_BoardType, iBoardType );
		if( iBoardType==m_nBoardID )
		{
			iBoardNoCurrent = iBoardNo;
			break;
		}
	}
	if( iBoardNoCurrent<0 )
		return FALSE;

	//COM2固定　　---★管理者モードで実行しないとエラーになる。
	StString szComPortName(m_szComPortName);
	//Boards[iBoardNoCurrent]->SetParam( MC_SerialControlA, szComPortName.GetAscii() );	//この記述なぜだめ？？文字列だから？？
	Status = McSetParamStr(MC_BOARD + iBoardNoCurrent, MC_SerialControlA, szComPortName.GetAscii());
	if( Status!=MC_OK )
		return FALSE;

	//Boards[iBoardNoCurrent]->SetParam( MC_BoardTopology, m_nBoardTopology );
	Status = McSetParamInt( MC_BOARD + iBoardNoCurrent, MC_BoardTopology, m_nBoardTopology );
	if( Status!=MC_OK )
		return FALSE;


    // Create a channel and associate it with the first connector on the first board
	//m_pChannel = new Channel(Boards[iBoardNoCurrent], "A");
	StString aConnect = m_szConnect;
	//m_pChannel = new Channel(Boards[iBoardNoCurrent], aConnect.GetAscii());
	Status = McCreate( MC_CHANNEL, &m_hChannel );
	if( Status!=MC_OK )
		return FALSE;
    // Choose the video standard
 	//StString aCameraFile = m_szCameraFile;	//./CamFiles/
	//StString aCameraFile =".\\CamFiles\\";

	do
	{

		Status = McSetParamInt( m_hChannel, MC_DriverIndex, iBoardNoCurrent );
		if( Status!=MC_OK )
			break;

		Status = McSetParamStr( m_hChannel, MC_Connector, aConnect.GetAscii() );
		if( Status!=MC_OK )
			break;


		CString szFilePath;
		if( m_szCameraFilePath.IsEmpty() )
		{
			GetModuleFilePath(szFilePath);
			szFilePath += _T("\\CONFIG\\CamFiles\\");
		}
		else
		{
			szFilePath = m_szCameraFilePath;
			szFilePath += _T("\\");
		}
		szFilePath += m_szCameraFile;
		StString aCameraFile = szFilePath;
		//m_pChannel->SetParam(MC_CamFile, aCameraFile.GetAscii());
		Status = McSetParamStr( m_hChannel, MC_CamFile, aCameraFile.GetAscii());
		if( Status!=MC_OK )
			break;

		//MC_ColorFormat
		//m_pChannel->GetParam(MC_ColorFormat, m_nColorFormat);
		INT32 nValue;
		Status = McGetParamInt( m_hChannel, MC_ColorFormat, &nValue);
		if( Status!=MC_OK )
			break;
		m_nColorFormat = nValue;

		//MC_Spectrum
		//m_pChannel->GetParam(MC_Spectrum, m_nSpectrum);
		Status = McGetParamInt( m_hChannel, MC_Spectrum, &nValue);
		if( Status!=MC_OK )
			break;
		m_nSpectrum = nValue;

		//MC_ColorRegistration
		//m_pChannel->GetParam(MC_ColorRegistration, m_nColorRegistration);
		Status = McGetParamInt( m_hChannel, MC_ColorRegistration, &nValue);
		if( Status!=MC_OK )
			break;
		m_nColorRegistration = nValue;

		//MC_ImageSizeX
		//INT64 aWidth;
		//m_pChannel->GetParam(MC_ImageSizeX, aWidth);
		//m_nSizeX = (INT)aWidth;
		Status = McGetParamInt( m_hChannel, MC_ImageSizeX, &nValue);
		if( Status!=MC_OK )
			break;
		m_nSizeX = nValue;

		//MC_ImageSizeY
		//INT64 aHeight;
		//m_pChannel->GetParam(MC_ImageSizeY, aHeight);
		//m_nSizeY = (INT)aHeight;
		Status = McGetParamInt( m_hChannel, MC_ImageSizeY, &nValue);
		if( Status!=MC_OK )
			break;
		m_nSizeY = nValue;

		//MC_BufferPitch
		//INT64 aBufferPitch;
		//m_pChannel->GetParam(MC_BufferPitch, aBufferPitch);
		//m_nBufferPitch = (INT)aBufferPitch;
		Status = McGetParamInt( m_hChannel, MC_BufferPitch, &nValue);
		if( Status!=MC_OK )
			break;
		m_nBufferPitch = nValue;

		//MC_BufferSize
		//INT64 aBufferSize;
		//m_pChannel->GetParam(MC_BufferSize, aBufferSize);
		//m_nBufferSize = (INT)aBufferSize;
		Status = McGetParamInt( m_hChannel, MC_BufferSize, &nValue);
		if( Status!=MC_OK )
			break;
		m_nBufferSize = nValue;

		//MC_SurfaceCount
		//m_pChannel->SetParam(MC_SurfaceCount, EURESYS_SURFACE_COUNT);
		Status = McSetParamInt( m_hChannel, MC_SurfaceCount, EURESYS_SURFACE_COUNT);
		if( Status!=MC_OK )
			break;

		// Register the callback function called when a new image is available
		//m_pChannel->RegisterCallback(this, &StDeviceMultiCam::OnImageCallback, MC_SIG_SURFACE_PROCESSING);
		Status = McRegisterCallback(m_hChannel, &GlobalCallback, this);

		// The function called when an acquisition failure occurs
		// m_pChannel->RegisterCallback(this, &StDeviceMultiCam::OnAcqFailureCallback, MC_SIG_ACQUISITION_FAILURE);

		// Enable the signals corresponding to the callback functions
		//m_pChannel->SetParam(MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
		Status = McSetParamInt( m_hChannel, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
		if( Status!=MC_OK )
			break;
		//m_pChannel->SetParam(MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);
		Status = McSetParamInt( m_hChannel, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);
		if( Status!=MC_OK )
			break;

		// Prepare the channel in order to minimize the acquisition sequence startup latency
		//m_pChannel->Prepare();

		bReval = TRUE;
	}while(0);
	return bReval;
}

BOOL StDeviceMultiCam::Close( void )
{
	if( !IsOpened() ) return FALSE;

	BOOL bReval = TRUE;
	//delete m_pChannel;
	//m_pChannel = NULL;
	int channelStatus;
	MCSTATUS Status = McGetParamInt( m_hChannel, MC_ChannelState, &channelStatus);
	if( channelStatus == MC_ChannelState_ACTIVE )
	{
		Status = McSetParamInt( m_hChannel, MC_ChannelState, MC_ChannelState_IDLE );
	}
	Sleep(100);
	McRegisterCallback( m_hChannel, NULL, this );
	Sleep(100);
	McDelete(m_hChannel);
	m_hChannel = NULL;

	return bReval;
}

BOOL StDeviceMultiCam::Start( void )
{
	if( !IsOpened() ) return FALSE;
	BOOL bReval = TRUE;
    if( !IsStarted() )
	{
        //m_pChannel->SetActive();
		MCSTATUS Status = McSetParamInt( m_hChannel, MC_ChannelState, MC_ChannelState_ACTIVE );
		if( Status!=MC_OK )
		{
			bReval = FALSE;
		}
	}
	return bReval;
}

BOOL StDeviceMultiCam::Stop( void )
{
	if( !IsOpened() ) return FALSE;

	BOOL bReval = TRUE;
    if( IsStarted() )
	{
        //m_pChannel->SetIdle();
		MCSTATUS Status = McSetParamInt( m_hChannel, MC_ChannelState, MC_ChannelState_IDLE );
		if( Status!=MC_OK )
		{
			bReval = FALSE;
		}
	}
	return bReval;
}

bool StDeviceMultiCam::IsStarted(void)
{
	if( !IsOpened() ) return FALSE;

    int activity;
	bool bReval;
    //m_pChannel->GetParam(MC_ChannelState, activity);
	MCSTATUS Status = McGetParamInt( m_hChannel, MC_ChannelState, &activity);
    if (activity == MC_ChannelState_ACTIVE)
        bReval = true;
    else
        bReval = false;
	return bReval;
}


//void StDeviceMultiCam::OnImageCallback(Channel &Ch, SignalInfo &Info)
void StDeviceMultiCam::OnImageCallback( PMCSIGNALINFO Info )
{
	/*
    try
    {
		int nBufferPitch;
		unsigned int unImageBuffer;
        // Update the bitmap image with the acquired image data 
        Info.Surf->GetParam(MC_SurfacePitch, nBufferPitch);
        Info.Surf->GetParam(MC_SurfaceAddr, unImageBuffer);

		StBuffer aConvBuffer;	//表示処理をOnTimer処理にした場合は、m_ConvBufferを使用して常に残しておく必要あり。。。。
		ConvertToStBufferFromBuffer(&aConvBuffer, unImageBuffer, m_nSizeX, m_nSizeY, nBufferPitch, m_nBufferSize);

		OnStBufferCallback(&aConvBuffer);

		aConvBuffer.Detach();

    }
    //catch (Euresys::MultiCam::Exception &e) 
    catch (...) 
    {
       // ExceptionDisplay("Error during callback processing", e.What());
    }
	*/
	MCSTATUS Status;
	void * unImageBuffer = NULL;
	switch( Info->Signal )
	{
	case MC_SIG_SURFACE_PROCESSING:
		Status = McGetParamInt( Info->SignalInfo, MC_SurfaceAddr, (PINT32)&unImageBuffer);
		{
//PBYTE pBuffer = (PBYTE)unImageBuffer;
//TRACE(TEXT("@@@@@@@@@ MulticamData=[%02x:%02x:%02x:%02x:%02x:%02x]\n") , pBuffer[0], pBuffer[1], pBuffer[2], pBuffer[3], pBuffer[4], pBuffer[5] );


		StBuffer aConvBuffer;	//表示処理をOnTimer処理にした場合は、m_ConvBufferを使用して常に残しておく必要あり。。。。
		//▼1.0.0.1043
		//ConvertToStBufferFromBuffer(&aConvBuffer, (unsigned int)unImageBuffer, m_nSizeX, m_nSizeY, m_nBufferPitch, m_nBufferSize);

		unsigned int unBuffer = (unsigned int)unImageBuffer;
		BYTE *pbyteBuffer = NULL;
		INT nBufferPitch = m_nBufferPitch;
		INT nBufferSize = m_nBufferSize;
		if( m_nColorFormat==MC_ColorFormat_RGB24PL )
		{
			pbyteBuffer = new BYTE [m_nSizeX * m_nSizeY * 3];
			ChangeFormatRGBPLColor( (BYTE *)pbyteBuffer, (const BYTE *)unImageBuffer, m_nColorFormat, m_nSizeX, m_nSizeY );
			unBuffer = (unsigned int)pbyteBuffer;
			nBufferPitch *= 3;	//PLの場合は３倍する。
			nBufferSize *= 3;
		}
		else
		if( m_nColorFormat==MC_ColorFormat_RGB30PL || m_nColorFormat==MC_ColorFormat_RGB36PL )
		{
			pbyteBuffer = new BYTE [m_nSizeX * m_nSizeY * 6];
			ChangeFormatRGBPLColor( (WORD *)pbyteBuffer, (const WORD *)unImageBuffer, m_nColorFormat, m_nSizeX, m_nSizeY );
			unBuffer = (unsigned int)pbyteBuffer;
			nBufferPitch *= 3;
			nBufferSize *= 3;
		}
		ConvertToStBufferFromBuffer(&aConvBuffer, unBuffer, m_nSizeX, m_nSizeY, nBufferPitch, nBufferSize);
		//▲1.0.0.1043

		OnStBufferCallback(&aConvBuffer);

		aConvBuffer.Detach();

		//▼1.0.0.1043
		if( pbyteBuffer ) 
		{
			delete [] pbyteBuffer;
			pbyteBuffer = NULL;
		}
		//▲1.0.0.1043

		}
		break;
	case MC_SIG_ACQUISITION_FAILURE:

		break;
	}
}

//▼1.0.0.1033
BOOL StDeviceMultiCam::GetFramerate( double *pdblData )
{
	if( !IsOpened() ) return FALSE;

	BOOL bReval = TRUE;
    if( IsStarted() )
	{
		MCSTATUS Status = McGetParamFloat( m_hChannel, MC_PerSecond_Fr, pdblData );
		if( Status!=MC_OK )
		{
			bReval = FALSE;
		}
	}
	return bReval;
}
//▲1.0.0.1033

/*
//
void StDeviceMultiCam::OnAcqFailureCallback(Channel &Ch, SignalInfo &Info)
{
}
*/
StPixelType StDeviceMultiCam::GetPixelType(void)
{
	StPixelType aPixelType = StPixelUndefined;

	switch(m_nColorFormat)
	{
	case MC_ColorFormat_Y8:
	case MC_ColorFormat_RAW8:
		aPixelType = StPixelMono8;
		if( m_nSpectrum==MC_Spectrum_COLOR )
		{
			if( m_nColorRegistration==MC_ColorRegistration_GR ) aPixelType = StPixelBayerGR8;
			else if( m_nColorRegistration==MC_ColorRegistration_RG ) aPixelType = StPixelBayerRG8;
			else if( m_nColorRegistration==MC_ColorRegistration_GB ) aPixelType = StPixelBayerGB8;
			else if( m_nColorRegistration==MC_ColorRegistration_BG ) aPixelType = StPixelBayerBG8;
		}
		break;
	case MC_ColorFormat_Y10:
	case MC_ColorFormat_RAW10:
		aPixelType = StPixelMono10;
		if( m_nSpectrum==MC_Spectrum_COLOR )
		{
			if( m_nColorRegistration==MC_ColorRegistration_GR ) aPixelType = StPixelBayerGR10;
			else if( m_nColorRegistration==MC_ColorRegistration_RG ) aPixelType = StPixelBayerRG10;
			else if( m_nColorRegistration==MC_ColorRegistration_GB ) aPixelType = StPixelBayerGB10;
			else if( m_nColorRegistration==MC_ColorRegistration_BG ) aPixelType = StPixelBayerBG10;
		}
		break;
	case MC_ColorFormat_Y12:
	case MC_ColorFormat_RAW12:
		aPixelType = StPixelMono12;
		if( m_nSpectrum==MC_Spectrum_COLOR )
		{
			if( m_nColorRegistration==MC_ColorRegistration_GR ) aPixelType = StPixelBayerGR12;
			else if( m_nColorRegistration==MC_ColorRegistration_RG ) aPixelType = StPixelBayerRG12;
			else if( m_nColorRegistration==MC_ColorRegistration_GB ) aPixelType = StPixelBayerGB12;
			else if( m_nColorRegistration==MC_ColorRegistration_BG ) aPixelType = StPixelBayerBG12;
		}
		break;
	case MC_ColorFormat_Y14:
	case MC_ColorFormat_RAW14:
		aPixelType = StPixelMono14;
		break;
	case MC_ColorFormat_Y16:
	case MC_ColorFormat_RAW16:
		aPixelType = StPixelMono16;
		if( m_nSpectrum==MC_Spectrum_COLOR )
		{
			if( m_nColorRegistration==MC_ColorRegistration_GR ) aPixelType = StPixelBayerGR16;
			else if( m_nColorRegistration==MC_ColorRegistration_RG ) aPixelType = StPixelBayerRG16;
			else if( m_nColorRegistration==MC_ColorRegistration_GB ) aPixelType = StPixelBayerGB16;
			else if( m_nColorRegistration==MC_ColorRegistration_BG ) aPixelType = StPixelBayerBG16;
		}
		break;
	case MC_ColorFormat_RGB24:
		//▼1.0.0.1043
		//MulticamではMC_ColorFormat_RGB24の設定でBGR順のデータが出てきているようだ。。
		//aPixelType = StPixelRGB8Packed;
		aPixelType = StPixelBGR8Packed;
		//▲1.0.0.1043
		break;

	case MC_ColorFormat_ARGB32:	//??
		//▼1.0.0.1043
		//MulticamではMC_ColorFormat_RGB24の設定でBGR順のデータが出てきているようだ。。
		//aPixelType = StPixelRGBA8Packed;
		aPixelType = StPixelBGRA8Packed;
		//▲1.0.0.1043
		break;
	//▼1.0.0.1043

	case MC_ColorFormat_RGB24PL:
		aPixelType = StPixelBGR8Packed;
		break;

	case MC_ColorFormat_RGB30PL:
		aPixelType = StPixelBGR10;
		break;

	case MC_ColorFormat_RGB36PL:
		aPixelType = StPixelBGR12;
		break;

	//▲1.0.0.1043
	};
	return aPixelType;

}

BOOL StDeviceMultiCam::ConvertToStBufferFromBuffer( StBuffer *aOutBuffer, unsigned int unBuffer, int nWidth, int nHeight, int nPitch, int nSize )
{
	BOOL bReval = FALSE;
	StPixelType aPixelType = GetPixelType();
	if( aPixelType==StPixelUndefined )
		return bReval;
	aOutBuffer->Attach((void *)unBuffer, nSize);	//最初でなければならない
	aOutBuffer->SetPixelType(aPixelType);
	aOutBuffer->SetWidth(nWidth);
	aOutBuffer->SetHeight(nHeight);
	aOutBuffer->SetLinePitch(nPitch);
	aOutBuffer->SetSize(nSize);
	aOutBuffer->SetID(0);
	aOutBuffer->SetTimestamp(0);
	bReval = TRUE;
	return bReval;
}

typedef struct _EURESYS_BOARD_INFO
{
	LPCTSTR szBoardName;
	INT  nBoardID;
}EURESYS_BOARD_INFO, *PEURESYS_BOARD_INFO;

EURESYS_BOARD_INFO gBoardInfo[] = 
{
	_T("Value"), 7,
	_T("Expert2"), 19,
	_T("Full"), 44,
};
EURESYS_BOARD_INFO gBoardTopologyInfo[] = 
{
	_T("MONO"), 24,
	_T("DUO"), 25,
	_T("BASE_MEDIUM_FULL"), 39,
	_T("DECA"), 40,
	_T("MONO_DECA"), 41,
	_T("MONO_SLOW"), 45,
	_T("DUO_SLOW"), 46,
};


BOOL StDeviceMultiCam::SetBoardID( LPCTSTR szModelName )
{
	BOOL bReval = FALSE;
	for( int i=0; i<_countof(gBoardInfo); i++ )
	{
		if( _tcscmp(gBoardInfo[i].szBoardName, szModelName) == 0 )
		{
			m_nBoardID = gBoardInfo[i].nBoardID;
			bReval = TRUE;
			break;
		}
	}
	return bReval;
}

BOOL StDeviceMultiCam::SetBoardID( INT nBoardID )
{
	m_nBoardID = nBoardID;
	return TRUE;
}

BOOL StDeviceMultiCam::SetBoardTopology( LPCTSTR szBoardTopologyName )
{
	BOOL bReval = FALSE;
	for( int i=0; i<_countof(gBoardTopologyInfo); i++ )
	{
		if( _tcscmp(gBoardTopologyInfo[i].szBoardName, szBoardTopologyName) == 0 )
		{
			m_nBoardTopology = gBoardTopologyInfo[i].nBoardID;
			bReval = TRUE;
			break;
		}
	}
	return TRUE;
}

BOOL StDeviceMultiCam::SetBoardTopology( INT nBoardTopology )
{
	m_nBoardTopology = nBoardTopology;
	return TRUE;

}

BOOL StDeviceMultiCam::SetConnect( LPCTSTR szConnect )
{
	m_szConnect = szConnect;
	return TRUE;
}

//▼1.0.0.1015
void StDeviceMultiCam::SetSerialComm(StSerialComm *pSerialComm)
{
	m_pSerialComm = pSerialComm;
	m_pSerialComm->SetPortName(m_szComPortName);

}
StSerialComm * StDeviceMultiCam::GetSerialComm(void)
{
	return m_pSerialComm;
}
//▲1.0.0.1015

BOOL StDeviceMultiCam::function( LPCTSTR szFunc, LPCTSTR szArgument )
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);


	if( szFunction=="CameraFile" )
	{
		//./CamFiles/ 
		m_szCameraFile = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetCameraFilePath" )
	{
		m_szCameraFilePath = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetBoardID" )
	{

		bReval = SetBoardID( szArgument );
		if( !bReval )
		{
			INT nBoardID = _ttoi(szArgument);
			if( nBoardID>0 )
				bReval = SetBoardID( _ttoi(szArgument) );
		}

	}
	else
	if( szFunction=="SetBoardTopology" )
	{
		bReval = SetBoardTopology( szArgument );
		if( !bReval )
		{
			INT nBoardTopology = _ttoi(szArgument);
			if( nBoardTopology>0 )
				bReval = SetBoardTopology( _ttoi(szArgument) );
		}
	}
	//▼1.0.0.1065 復旧
	//▼1.0.0.1062
	else
	if( szFunction=="SetSerialComm" )	//COM2固定にしておく
	{
	}
	else
	if( szFunction=="Open" )
	{
		bReval = Open();
		if( !bReval )
			m_szLastError = _T("Multicamオープンエラー.管理者モードで起動してください。");

	}
	else
	if( szFunction=="Close" )
	{
		bReval = Close();
		if( !bReval )
			m_szLastError = _T("Multicamクローズエラー");
	}
	else
	if( szFunction=="Start" )
	{
		bReval = Start();
		if( !bReval )
			m_szLastError = _T("Multicamスタートエラー");
	}
	else
	if( szFunction=="Stop" )
	{
		bReval = Stop();
		if( !bReval )
			m_szLastError = _T("Multicamストップエラー");
	}
	//▲1.0.0.1062
	//▲1.0.0.1065
	//▼1.0.0.1015
	else
	if( szFunction=="SetComPortName" )
	{
		SetComPortName(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetConnect" )
	{
		SetConnect(szArgument);
		bReval = TRUE;
	}
	//▲1.0.0.1015


	return bReval;

}

/*
BOOL StDeviceMultiCam::SerialControl( LPCTSTR szData, PBYTE pbyteReadData, size_t *pReadSize )
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

	bReval = SerialControl( byteData, sendSize, pbyteReadData, pReadSize );

	delete [] byteData;

	return bReval;

}

BOOL StDeviceMultiCam::SerialControl( PBYTE pbyteSendData, size_t sendSize, PBYTE pbyteReadData, size_t *pReadSize )
{
	BOOL bReval = FALSE;

	//PvSerialPortIPEngine aSerial;
	//PvIPEngineSerial aPort = PvIPEngineSerial0;
	//if( _tcscmp(szPort,_T("UART0"))==0 )
	//{
	//	aPort = PvIPEngineSerial0;
	//}
	//else
	//if( _tcscmp(szPort,_T("UART1"))==0 )
	//{
	//	aPort = PvIPEngineSerial1;
	//}
	//else
	//if( _tcscmp(szPort,_T("BULK1"))==0 )
	//{
	//	aPort = PvIPEngineSerialBulk0;
	//}





	PvResult aResult = aSerial.Open( this, aPort );
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
		for( int i=0; i<10; i++ )
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

	return bReval;

}
*/