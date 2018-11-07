#include "StdAfx.h"
#include "StGenStream.h"


#include "StGenInterface.h"
//#include "StDevice.h"
#include "StStream.h"
//▼2017/10/20
#include "StGenPort.h"
//▲2017/10/20


//------------------------------------------------------------------------
//--StGenStreamValue----------------------------------------------------------------------
//------------------------------------------------------------------------
class StGenStreamValue
{
public:
	StGenStreamValue(void);
	virtual ~StGenStreamValue(void);

    bool Open(void);
    void Close(void);
    bool IsOpen(void) const {
		return (NULL != m_hDS);
	}
	bool QueryInformations( void );


    const StString& GetDisplayName() const{return m_strDisplayName;}
    const StString& GetPortName() const{return m_strPortName;}
	const StString& GetID(void) const{return m_strID;}
	const DS_HANDLE GetHandle(void) const{return m_hDS;}
	
	//StInt32 GetNumDeviceInfo( void );
	//StGenDeviceInfo * GetGenDeviceInfo(StInt32 index);

//private:
	//std::vector< StGenDeviceInfo * > m_vDeviceInfoList;

	bool SetBufferCount(int iCount);
	StInt32 GetBufferCount(void);
	bool SetBufferSize(int iSize);
	StInt32 GetBufferSize(void);

	StResult StartStream(void);
	StResult StopStream(void);
	StResult RetrieveBuffer(StBuffer ** aBuffer, StUInt32 aTimeout = 0, StResult * aOperationResult = NULL);
	StResult QueueBuffer(StBuffer * aBuffer);
	//▼2017/10/17
	StResult GetOutputQueueCount(PINT aCount);
	//▲2017/10/17

	//▼2017/10/20
	//--------------------------------------------------------------------
	//SetGenICamControl
	//--------------------------------------------------------------------
	StResult SetGenICamControl( LPCTSTR szFeature, LPCTSTR szData );
	StResult SetGenICamControl( LPCTSTR szFeature, INT nData );
	StResult SetGenICamControl( LPCTSTR szFeature, double dblData );
	StResult SetGenICamControl( LPCTSTR szFeature, bool bData );

	//--------------------------------------------------------------------
	//GetGenICamControl
	//--------------------------------------------------------------------
	StResult GetGenICamControl( LPCTSTR szFeature, LPTSTR szData, size_t size );
	StResult GetGenICamControl( LPCTSTR szFeature, INT &nData );
	StResult GetGenICamControl( LPCTSTR szFeature, double &dblData );
	StResult GetGenICamControl( LPCTSTR szFeature, bool &bData );
	//▲2017/10/20

    StString m_strDisplayName;
    StString m_strPortName;
    StString m_strID;
	StInt32 m_iBufferCount;
	StInt32 m_iBufferSize;

	StGenGCLib* m_psGCLib;
	StGenDevice * m_psDevice;
	DEV_HANDLE m_hDevice;
	DS_HANDLE m_hDS;
	EVENT_HANDLE m_hEvent;
	StBuffer *mpBuffers;

private:
	//▼2017/10/20
	CNodeMapRef *m_pNodeMap;
	StGenPort *m_pGenPort;

	StResult GetGenIntParameter( StString lFeature, int & iData, bool Verify=false, bool IgnoreCache=false );
	StResult GetGenIntMinMaxParameter( StString lFeature, int & iMin, int & iMax );
	StResult SetGenIntParameter( StString lFeature, int iData );
	StResult GetGenEnumParameter( StString lFeature, int & iData, bool Verify=false, bool IgnoreCache=false );
	StResult GetGenEnumIndexParameter( StString lFeature, INT nIndex, StString & strData, bool Verify=false, bool IgnoreCache=false );
	StResult GetGenEnumParameter( StString lFeature, StString & strData, bool Verify=false, bool IgnoreCache=false );
	StResult SetGenEnumParameter( StString lFeature, int iData );
	StResult SetGenEnumParameter( StString lFeature, StString strData );
	StResult GetGenFloatParameter( StString lFeature, double & dblData, bool Verify=false, bool IgnoreCache=false );
	StResult GetGenFloatMinMaxParameter( StString lFeature, double & dblMin, double & dblMax );
	StResult SetGenFloatParameter( StString lFeature, double dblData );
	StResult GetGenBoolParameter( StString lFeature, bool & bData, bool Verify=false, bool IgnoreCache=false );
	StResult SetGenBoolParameter( StString lFeature, bool bData );
	StResult GetGenStringParameter( StString lFeature, StString & strData, bool Verify=false, bool IgnoreCache=false );
	StResult SetGenStringParameter( StString lFeature, StString & strData );
	StResult SetGenCommandParameter( StString lFeature );
	//▲2017/10/20

};

StGenStreamValue::StGenStreamValue()
    : m_strID( "" )
    , m_strDisplayName( "" )
    , m_strPortName( "" )
	, m_iBufferCount( 0 )
	, m_iBufferSize( 0 )

	, m_psGCLib(NULL)
	, m_psDevice(NULL)
	, m_hDevice(NULL)
	, m_hDS(NULL)
	, m_hEvent(NULL)
	, mpBuffers(NULL)

	//▼2017/10/20
	, m_pNodeMap(NULL)
	, m_pGenPort(NULL)
	//▲2017/10/20

{
}

StGenStreamValue::~StGenStreamValue()
{
}

bool StGenStreamValue::QueryInformations( void )
{
	bool bReval = false;
	if( !IsOpen() ) return bReval;

	if (m_psGCLib)
    {
        GC_ERROR status;
        size_t iSize = 0;
        INFO_DATATYPE iType = INFO_DATATYPE_UNKNOWN;

		status = m_psGCLib->f_GCGetPortInfo(m_hDS,PORT_INFO_PORTNAME, &iType, NULL, &iSize);
        if ((GC_ERR_SUCCESS == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            status = m_psGCLib->f_GCGetPortInfo(m_hDS,PORT_INFO_PORTNAME, &iType, str, &iSize);
            m_strPortName = str;
            delete[] (str);
			bReval = true;
        }
    }
	return bReval;
}


bool StGenStreamValue::Open(void)
{

    if (!IsOpen())
    {
        GC_ERROR status = m_psGCLib->f_DevOpenDataStream(m_hDevice, m_strID.GetAscii(), &m_hDS);
        if (GC_ERR_SUCCESS == status)
        {
            // update informations                
			//▼2017/10/20
			//QueryInformations();
			bool bReval = QueryInformations();
			if( bReval )
			{
				//CNodeMapRef *m_pNodeMap = NULL;
				//StGenPort *m_pGenPort = NULL;
				do
				{
					m_pNodeMap = new CNodeMapRef();
					if( !m_pNodeMap ) break;
					m_pGenPort = new StGenPort(m_psGCLib, m_hDS);
					if( !m_pGenPort ) break;

					gcstring strXML;
					m_psGCLib->RetrieveXML(m_hDS, strXML);
					m_pNodeMap->_LoadXMLFromString(strXML);
					m_pNodeMap->_Connect(m_pGenPort, m_strPortName.GetAscii());

				}while(0);

				if( !bReval )
				{
					if(m_pNodeMap)	delete m_pNodeMap;
					m_pNodeMap = NULL;
					if(m_pGenPort)	delete m_pGenPort;
					m_pGenPort = NULL;
				}
			}
			//▲2017/10/20


            return bReval;
        }
        return false;
    }
    else
        return true;   
}

void StGenStreamValue::Close( void )
{
	////StopStream();
    if (IsOpen())
    {
        //if (IsGrabbing() && WaitForSingleObject(m_evTerminate, 0))
        //{
        //    StopAcquisition();
        //}
    
		if( m_pNodeMap )
		{
			m_pNodeMap->_Destroy();
			delete m_pNodeMap;
			m_pNodeMap = NULL;
		}

		if( m_pGenPort )
		{
			delete m_pGenPort;
			m_pGenPort = NULL;
		}

        m_psGCLib->f_DSClose(m_hDS);
        m_hDS = NULL;
    }
}


bool StGenStreamValue::SetBufferCount(int iCount)
{
	m_iBufferCount = iCount;
	return true;
}

StInt32 StGenStreamValue::GetBufferCount(void)
{
	return m_iBufferCount;
}

bool StGenStreamValue::SetBufferSize(int iSize)
{
	m_iBufferSize = iSize;
	return true;
}

StInt32 StGenStreamValue::GetBufferSize(void)
{
	return m_iBufferSize;
}

StResult StGenStreamValue::StartStream(void)
{
    GC_ERROR status;
    //SimpleStream *pClass = (SimpleStream *)pClassInst;
    DS_HANDLE hDatastream = GetHandle();

	if( m_iBufferCount==0 ) return StResult::Code::NOT_INITIALIZED;
	if( m_iBufferSize==0 ) return StResult::Code::NOT_INITIALIZED;

	// Create and register New Buffer Event
	//EVENT_NEW_BUFFER_DATA NewImageEventData;
	EVENT_HANDLE hEventNewBuffer = NULL;
	status = m_psGCLib->f_GCRegisterEvent( hDatastream, EVENT_NEW_BUFFER, &hEventNewBuffer);
	if (status < 0)
	{
		//pClass->GetsTL()->HandleError( "Error No Event registered: ");
		return StResult::Code::NOT_CREATESTREAM;
	}

	m_hEvent = hEventNewBuffer;


	//バッファの確保
    mpBuffers = new StBuffer[ m_iBufferCount ];
	if( !mpBuffers ) return StResult::Code::NOT_ENOUGH_MEMORY;

    for ( INT i = 0; i < m_iBufferCount; i++ )
    {
        mpBuffers[ i ].Alloc( m_iBufferSize );
    }

	BUFFER_HANDLE hBuffer=NULL;
    for ( INT i = 0; i < m_iBufferCount; i++ )
    {
        status = m_psGCLib->f_DSAnnounceBuffer( hDatastream, (void *)mpBuffers[i].GetDataPointer(), mpBuffers[i].GetAllocSize(),
            (void *)&mpBuffers[i], &hBuffer);
        if (status < 0)
        {
            return StResult::Code::NOT_CREATESTREAM;
        }
		mpBuffers[i].SetContext(hBuffer);
		//Port

 		StString aPortName;
        INFO_DATATYPE iType = INFO_DATATYPE_UNKNOWN;
        size_t iSize = 0;
        status = m_psGCLib->f_GCGetPortInfo(hBuffer,PORT_INFO_PORTNAME, &iType, NULL, &iSize);
        if ((GC_ERR_SUCCESS == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            //m_strDisplayName.reserve(iSize);
            status = m_psGCLib->f_GCGetPortInfo(hBuffer,PORT_INFO_PORTNAME, &iType, str, &iSize);
            aPortName = str;
            delete[] (str);
			mpBuffers[i].SetPortName(aPortName);
        }
    }

	status = m_psGCLib->f_DSFlushQueue( hDatastream, ACQ_QUEUE_ALL_TO_INPUT);
	if (status < 0)
	{
		return StResult::Code::NOT_CREATESTREAM;
	}

	// Start Acquisition
	status = m_psGCLib->f_DSStartAcquisition(hDatastream, ACQ_START_FLAGS_DEFAULT, 0xFFFFFFFFFFFFFFFF);
	if (status < 0)
	{
		return StResult::Code::NOT_CREATESTREAM;
	}

	return StResult::Code::OK;
}

StResult StGenStreamValue::StopStream(void)
{
    GC_ERROR status;

    DS_HANDLE hDatastream = GetHandle();

       // ResetEvent(pClass->m_evTerminate);

      //  pRecentBuffer = NULL;

	// Stop Acquisition
	// first stop Datastream ...
	status = m_psGCLib->f_DSStopAcquisition(hDatastream, ACQ_STOP_FLAGS_DEFAULT);
	if (status < 0)
	{
		//pClass->GetsTL()->HandleError( "DSStopAcquisition failed: " );
	}
	// ... then Stop Device
	//pClass->GetDevice()->StopAcquisition();

	// Cleanup
	status = m_psGCLib->f_GCUnregisterEvent( hDatastream, EVENT_NEW_BUFFER);
	if (status < 0)
	{
		//pClass->GetsTL()->HandleError( "Error unregistering Event: ");
	}
    m_hEvent = NULL;

	status = m_psGCLib->f_DSFlushQueue( hDatastream, ACQ_QUEUE_ALL_DISCARD);
	if (status < 0)
	{
		//pClass->GetsTL()->HandleError( "DSFlushQueue failed: " );
	}
    

	//▼2014/07/23 最後のデータポインタ保存----------------------------------
	//mLastDataPointer = NULL;
	//mi64LastFrameID = 0;
	//▲2014/07/23 最後のデータポインタ保存----------------------------------

	// revoke buffers from transport layer
	if( mpBuffers )
	{
		for ( INT i = 0; i < m_iBufferCount; i++ )
		{
			//▼1.0.0.4
			//KAYA - Problem
			void* pBuffer = NULL;
			void* pPrivate;

//▼1.0.0.38 GenApi3.0
#ifdef _GENAPI3
			status = m_psGCLib->f_DSRevokeBuffer( hDatastream, (BUFFER_HANDLE)mpBuffers[i].GetContext(), &pBuffer, &pPrivate);
#else
			status = m_psGCLib->f_DSRevokeBuffer( hDatastream, (GenICam::Client::BUFFER_HANDLE)mpBuffers[i].GetContext(), &pBuffer, &pPrivate);
#endif
//▲1.0.0.38 GenApi3.0

			//status = m_psGCLib->f_DSRevokeBuffer( hDatastream, (GenICam::Client::BUFFER_HANDLE)mpBuffers[i].GetContext(), NULL, NULL);
			//▲1.0.0.4
	
			if (status < 0)
			{
				//return false;
			}
			mpBuffers[ i ].Free();
		}
		delete [] mpBuffers;
		mpBuffers = NULL;
	}
	return StResult::Code::OK;
}

INT GetBitsPerPixel(StPixelType aPixelType)
{
	//32bit
	INT iBitsPerPixel = (aPixelType&STBITSPERPIXELMASK)>>16;
	if( iBitsPerPixel==0 )
	{
		//16Bit
		switch(aPixelType)
		{
		case StPixelMono8_16Bit:
		case StPixelBayerGR8_16Bit:
		case StPixelBayerRG8_16Bit:
		case StPixelBayerGB8_16Bit:
		case StPixelBayerBG8_16Bit:
			iBitsPerPixel = 8;
			break;
		case StPixelMono10pmsb_16Bit:
		case StPixelBayerGR10pmsb_16Bit:
		case StPixelBayerRG10pmsb_16Bit:
		case StPixelBayerGB10pmsb_16Bit:
		case StPixelBayerBG10pmsb_16Bit:
			iBitsPerPixel = 10;
			break;
		case StPixelMono12pmsb_16Bit:
		case StPixelBayerGR12pmsb_16Bit:
		case StPixelBayerRG12pmsb_16Bit:
		case StPixelBayerGB12pmsb_16Bit:
		case StPixelBayerBG12pmsb_16Bit:
			iBitsPerPixel = 12;
			break;
		case StPixelMono14pmsb_16Bit:
		case StPixelBayerGR14pmsb_16Bit:
		case StPixelBayerRG14pmsb_16Bit:
		case StPixelBayerGB14pmsb_16Bit:
		case StPixelBayerBG14pmsb_16Bit:
			iBitsPerPixel = 14;
			break;
		case StPixelMono16_16Bit:
		case StPixelBayerGR16_16Bit:
		case StPixelBayerRG16_16Bit:
		case StPixelBayerGB16_16Bit:
		case StPixelBayerBG16_16Bit:
			iBitsPerPixel = 16;
			break;
		}
	}
	return iBitsPerPixel;
}



INT GetLinePitch(INT aWidth, StPixelType aPixelType)
{
	INT iBitsPerLine = aWidth * GetBitsPerPixel(aPixelType);
	INT iLinePitch = (iBitsPerLine>>3) + ((iBitsPerLine&7)>0?1:0);
	return iLinePitch;
}

StResult StGenStreamValue::RetrieveBuffer(StBuffer ** aBuffer, StUInt32 aTimeout, StResult * aOperationResult)
{
	//if( mhStream==NULL ) return StResult::Code::INVALID_PARAMETER;
	if( m_psGCLib==NULL ) return StResult::Code::INVALID_PARAMETER;
	if( m_hEvent==NULL ) return StResult::Code::INVALID_PARAMETER;

	EVENT_NEW_BUFFER_DATA lEventData;
	size_t lSize = sizeof lEventData;
	GC_ERROR lGenTLRtn = m_psGCLib->f_EventGetData(m_hEvent, &lEventData, &lSize, aTimeout);

	if (lGenTLRtn != GC_ERR_SUCCESS) {
		*aBuffer = NULL;
		return StResult::Code::TIMEOUT;
	}

	//▼2017/10/10
	INFO_DATATYPE iType;
	size_t sData;
	uint64_t i64Data;
	size_t bufSize;

	BUFFER_HANDLE hBuffer = (BUFFER_HANDLE)lEventData.BufferHandle;	//BUFFER_HANDLE

	bufSize = sizeof(sData);

	GC_ERROR err = m_psGCLib->f_DSGetBufferInfo(m_hDS, hBuffer, BUFFER_INFO_PAYLOADTYPE, &iType, &sData, &bufSize);

	size_t		srcSize = 0;

	bool IsChunkData = false;
	if(PAYLOAD_TYPE_CHUNK_DATA == sData || PAYLOAD_TYPE_CHUNK_ONLY == sData)
	{
		IsChunkData = true;
	}

	*aBuffer = (StBuffer *)lEventData.pUserPointer;

	if(IsChunkData)
	{
		//チャンクのレイアウトが変化しているかどうかを確認する
		SINGLE_CHUNK_DATA *pChunkArray=NULL;
		do
		{
			uint64_t	nChunklayoutID = 0;
			bufSize = sizeof(nChunklayoutID);
			err = m_psGCLib->f_DSGetBufferInfo(m_hDS, hBuffer, BUFFER_INFO_CHUNKLAYOUTID, &iType, &nChunklayoutID, &bufSize);
			if(err != GC_ERR_SUCCESS) break;

			
			size_t iNumChunks = 0;
			if((nChunklayoutID == 0) || ((*aBuffer)->GetChunklayoutID() != nChunklayoutID))
			{

				(*aBuffer)->ChunkFree();

				err = m_psGCLib->f_DSGetBufferChunkData(m_hDS, hBuffer, NULL, &iNumChunks);
				if(err != GC_ERR_SUCCESS) break;
	
				pChunkArray = new SINGLE_CHUNK_DATA[iNumChunks];
				memset(pChunkArray,0,sizeof(SINGLE_CHUNK_DATA)*iNumChunks );

				//SINGLE_CHUNK_DATA *pChunkData = (*aBuffer)->GetChunk(iNumChunks);

				if(pChunkArray != NULL)
				{
					//m_nNumChunks = iNumChunks;

					err = m_psGCLib->f_DSGetBufferChunkData(m_hDS, hBuffer, pChunkArray, &iNumChunks);
					if(err != GC_ERR_SUCCESS) break;
					(*aBuffer)->SetChunkDataBuffer((void *)pChunkArray,iNumChunks);

					//m_nChunklayoutID = nChunklayoutID;

				}
				else
				{
					break;
				}
			}

			//TODO:チャンクデータをNodeMapに割り当てる場合
			//予めCChunkAdapterGenericのコンストラクタでNodeMapを設定しておいて、
			//ここでAttachBuffer()を呼び出しバッファとチャンク情報を渡せば良さそう
			
			//下記情報は表示時に処理するか.....
			//チャンクが存在する場合は、データの先頭のチャンクのサイズを画像データのサイズとする
			for(size_t i = 0; i < iNumChunks; i++)
			{
				UINT ChunkID = (UINT)pChunkArray[i].ChunkID;
				PUINT pOffset = (PUINT)&pChunkArray[i].ChunkOffset;
			}
		}while(0);

		if( pChunkArray ) 
			delete [] pChunkArray;
	}


	//ChunkのみでImageがない場合がある。。。。。。。。。。。。。。。。。。。。。
	if( PAYLOAD_TYPE_CHUNK_ONLY == sData )
	{
		//DSGetBufferInfoで処理する。。。。。
	}

	//*aBuffer = (StBuffer *)lEventData.pUserPointer;
	//BUFFER_HANDLE hBuffer = (BUFFER_HANDLE)lEventData.BufferHandle;	//BUFFER_HANDLE
	//BUFFER_HANDLE(BUFFER_STRUCT)からStBufferに変換？？
	//INFO_DATATYPE iType;
	//size_t sData;
	//uint64_t i64Data;
	//size_t bufSize;

	//▲2017/10/10

	//Width
	bufSize = sizeof(sData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_WIDTH, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//TRACE(TEXT("@@@@@@@@@ BUFFER_INFO_WIDTH=%i\n"),sData );
	(*aBuffer)->SetWidth((uint32_t)sData);
	//Height
	bufSize = sizeof(sData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_HEIGHT, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//TRACE(TEXT("@@@@@@@@@ BUFFER_INFO_HEIGHT=%i\n"),sData );
	(*aBuffer)->SetHeight((uint32_t)sData);
	//PixelType
	bufSize = sizeof(i64Data);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_PIXELFORMAT, &iType, &i64Data, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//TRACE(TEXT("@@@@@@@@@ BUFFER_INFO_PIXELFORMAT=0x%08x\n"),i64Data );
	(*aBuffer)->SetPixelType((StPixelType)i64Data);
	//Timestamp
	bufSize = sizeof(i64Data);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_TIMESTAMP, &iType, &i64Data, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//TRACE(TEXT("@@@@@@@@@ BUFFER_INFO_TIMESTAMP=0x%08x\n"),i64Data );
	(*aBuffer)->SetTimestamp(i64Data);
	//XOFFSET
	bufSize = sizeof(sData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_XOFFSET, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//TRACE(TEXT("@@@@@@@@@ BUFFER_INFO_XOFFSET=%i\n"),sData );
	(*aBuffer)->SetOffsetX((uint32_t)sData);
	//YOFFSET
	bufSize = sizeof(sData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_YOFFSET, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//TRACE(TEXT("@@@@@@@@@ BUFFER_INFO_YOFFSET=%i\n"),sData );
	(*aBuffer)->SetOffsetY((uint32_t)sData);
	//XPADDING
	bufSize = sizeof(sData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_XPADDING, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//TRACE(TEXT("@@@@@@@@@ BUFFER_INFO_XPADDING=%i\n"),sData );
	(*aBuffer)->SetPaddingX((uint32_t)sData);
	//YPADDING
	bufSize = sizeof(sData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_YPADDING, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//TRACE(TEXT("@@@@@@@@@ BUFFER_INFO_YPADDING=%i\n"),sData );
	(*aBuffer)->SetPaddingY((uint32_t)sData);

	//バッファに書き込まれたサイズ
	bufSize = sizeof(sData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_SIZE_FILLED, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//TRACE(TEXT("@@@@@@@@@ BUFFER_INFO_SIZE_FILLED=%i\n"),sData );
	(*aBuffer)->SetSize((uint32_t)sData);
	//ID
	bufSize = sizeof(i64Data);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_FRAMEID, &iType, &i64Data, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//TRACE(TEXT("@@@@@@@@@ BUFFER_INFO_FRAMEID=%i\n"),i64Data );
	(*aBuffer)->SetID(i64Data);

	//BUFFER_INFO_IMAGEOFFSET
	bufSize = sizeof(sData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_IMAGEOFFSET, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//TRACE(TEXT("@@@@@@@@@ BUFFER_INFO_IMAGEOFFSET=%i\n"),sData );
	(*aBuffer)->SetImageOffset((uint32_t)sData);

	//BUFFER_INFO_PAYLOADTYPE
	bufSize = sizeof(sData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_PAYLOADTYPE, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//TRACE(TEXT("@@@@@@@@@ BUFFER_INFO_PAYLOADTYPE=%i\n"),sData );
	(*aBuffer)->SetPayloadType((uint32_t)sData);


	//▼2017/10/10
	//BUFFER_INFO_IS_INCOMPLETE
	bool bData;
	bufSize = sizeof(bData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_IS_INCOMPLETE, &iType, &bData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) sData = 0;
//TRACE(TEXT("@@@@@@@@@ BUFFER_INFO_IS_INCOMPLETE=%i\n"),bData );
	(*aBuffer)->SetIncompleteImage(bData);

	//BUFFER_INFO_DELIVERED_IMAGEHEIGHT
	bufSize = sizeof(sData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_DELIVERED_IMAGEHEIGHT, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) sData = 0;

//TRACE(TEXT("@@@@@@@@@ BUFFER_INFO_DELIVERED_IMAGEHEIGHT=%d\n"),sData );

	(*aBuffer)->SetDeliveredImageHeight((uint32_t)sData);
	//▲2017/10/10

//TRACE(TEXT("@@@@@@@@@ (*aBuffer)=0x%08x\n"),(*aBuffer) );


	//LinePitch
	//bufSize = sizeof(sData);
	//err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_PAYLOADTYPE, &iType, &sData, &bufSize );
	//if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;

	//sData = (*aBuffer)->GetWidth();
	//(*aBuffer)->SetLinePitch((uint32_t)sData);
	//LinePitchをWidthとPixelTypeから計算する。
	(*aBuffer)->SetLinePitch((uint32_t)GetLinePitch((*aBuffer)->GetWidth(), (*aBuffer)->GetPixelType()));


//TRACE(TEXT("@@@@@@@@@ \n") );


	//▼2014/07/23 最後のデータポインタ保存----------------------------------
//	mLastDataPointer = (*aBuffer)->GetDataPointer();
//	mi64LastFrameID = (*aBuffer)->GetID();
	//▲2014/07/23 最後のデータポインタ保存----------------------------------

	return StResult::Code::OK;
}
StResult StGenStreamValue::QueueBuffer(StBuffer * aBuffer)
{
	if( m_hDS==NULL ) return StResult::Code::INVALID_PARAMETER;
	GC_ERROR err = GC_ERR_SUCCESS;

    err = m_psGCLib->f_DSQueueBuffer( m_hDS, aBuffer->GetContext() ); //BUFFER_HANDLE(GetContext)
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
	return StResult::Code::OK;
}

//▼2017/10/17
StResult StGenStreamValue::GetOutputQueueCount(PINT aCount)
{
	if( m_hDS==NULL ) return StResult::Code::INVALID_PARAMETER;
	GC_ERROR err = GC_ERR_SUCCESS;

	INFO_DATATYPE aDataType;
	size_t nCount;
	size_t aSize = sizeof(aCount);

    err = m_psGCLib->f_DSGetInfo( m_hDS, STREAM_INFO_NUM_AWAIT_DELIVERY, &aDataType, &nCount, &aSize ); //BUFFER_HANDLE(GetContext)
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
	*aCount = nCount;
	return StResult::Code::OK;
}
//▲2017/10/17

//▼2017/10/20
StResult StGenStreamValue::GetGenIntParameter( StString lFeature, int & iData, bool Verify, bool IgnoreCache )
{
	StResult aResult = StResult::Code::INVALID_PARAMETER;
	INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return aResult;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=intfIInteger ) return aResult;

    CIntegerPtr ptrInteger = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
            iData = (int)ptrInteger->GetValue( Verify, IgnoreCache );
            aResult = StResult::Code::OK;
        }
        catch (GenICam::GenericException &e)
        {
        }
    }
    return aResult;
}

StResult StGenStreamValue::GetGenIntMinMaxParameter( StString lFeature, int & iMin, int & iMax )
{
	StResult aResult = StResult::Code::INVALID_PARAMETER;
	INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return aResult;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=intfIInteger ) return aResult;

    CIntegerPtr ptrInteger = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
            iMin = (int)ptrInteger->GetMin();
            iMax = (int)ptrInteger->GetMax();
            aResult = StResult::Code::OK;
        }
        catch (GenICam::GenericException &e)
        {
        }
    }
    return aResult;
}

StResult StGenStreamValue::SetGenIntParameter( StString lFeature, int iData )
{
	StResult aResult = StResult::Code::INVALID_PARAMETER;
	INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return aResult;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=intfIInteger ) return aResult;

    CIntegerPtr ptrInteger = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
            ptrInteger->SetValue(iData);
            aResult = StResult::Code::OK;
        }
        catch (GenICam::GenericException &e)
        {
        }
    }
    return aResult;
}

StResult StGenStreamValue::GetGenEnumParameter( StString lFeature, int & iData, bool Verify, bool IgnoreCache )
{
	StResult aResult = StResult::Code::INVALID_PARAMETER;
	INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return aResult;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIEnumeration ) return aResult;

    CEnumerationPtr ptrEnumeration = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
			iData = (int)ptrEnumeration->GetIntValue(Verify,IgnoreCache);
            aResult = StResult::Code::OK;
        }
        catch (GenICam::GenericException &e)
        {
			//TCHAR szTmp[256];
			//MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			//OutputDebugString(szTmp);
			//OutputDebugString(_T("\n"));
			//return false;
        }
    }
    return aResult;
}

StResult StGenStreamValue::GetGenEnumParameter( StString lFeature, StString & strData, bool Verify, bool IgnoreCache )
{
	StResult aResult = StResult::Code::INVALID_PARAMETER;
	INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return aResult;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIEnumeration ) return aResult;

    CEnumerationPtr ptrEnumeration = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
            strData = ptrEnumeration->ToString(Verify,IgnoreCache);
            aResult = StResult::Code::OK;
        }
        catch (GenICam::GenericException &e)
        {
			//TCHAR szTmp[256];
			//MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			//OutputDebugString(szTmp);
			//OutputDebugString(_T("\n"));
   //         return false;
        }
    }
    return aResult;
}

StResult StGenStreamValue::GetGenEnumIndexParameter( StString lFeature, INT nIndex, StString & strData, bool Verify, bool IgnoreCache )
{
	StResult aResult = StResult::Code::INVALID_PARAMETER;
	INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return aResult;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIEnumeration ) return aResult;

    CEnumerationPtr ptrEnumeration = ptrINode;

    //if(ptrInteger.IsValid())
    {
		INT nCount=0;
        try
        {
			GenApi::NodeList_t aNodeList;
			ptrEnumeration->GetEntries(aNodeList);

			for( NodeList_t::iterator ptr=aNodeList.begin(); ptr!=aNodeList.end(); ptr++ )
			{
				CEnumEntryPtr ptrEnumEntry(*ptr);
				if( ptrEnumEntry->GetAccessMode()!=NI )
				{
					if( nCount==nIndex )
					{
						strData = ptrEnumEntry->GetSymbolic();
						aResult = StResult::Code::OK;
						break;
					}
					nCount++;
				}
			}
        }
        catch (GenICam::GenericException &e)
        {
			//TCHAR szTmp[256];
			//MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			//OutputDebugString(szTmp);
			//OutputDebugString(_T("\n"));
			//return false;
        }
    }
    return aResult;
}

StResult StGenStreamValue::SetGenEnumParameter( StString lFeature, int iData )
{
	StResult aResult = StResult::Code::INVALID_PARAMETER;
	INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return aResult;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIEnumeration ) return aResult;

    CEnumerationPtr ptrEnumeration = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
            ptrEnumeration->SetIntValue(iData);
            aResult = StResult::Code::OK;
        }
        catch (GenICam::GenericException &e)
        {
			//TCHAR szTmp[256];
			//MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			//OutputDebugString(szTmp);
			//OutputDebugString(_T("\n"));
			//return false;
        }
    }
    return aResult;
}

StResult StGenStreamValue::SetGenEnumParameter( StString lFeature, StString strData )
{
	StResult aResult = StResult::Code::INVALID_PARAMETER;
	INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return aResult;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIEnumeration ) return aResult;

    CEnumerationPtr ptrEnumeration = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
			gcstring strValue = strData;
            ptrEnumeration->FromString(strValue);
            aResult = StResult::Code::OK;
        }
        catch (GenICam::GenericException &e)
        {
			//TCHAR szTmp[256];
			//MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			//OutputDebugString(szTmp);
			//OutputDebugString(_T("\n"));
			//return false;
        }
    }
    return aResult;
}

StResult StGenStreamValue::GetGenFloatParameter( StString lFeature, double & dblData, bool Verify, bool IgnoreCache )
{
	StResult aResult = StResult::Code::INVALID_PARAMETER;
	INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return aResult;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIFloat ) return aResult;

    CFloatPtr ptrFloat = ptrINode;

    {
        try
        {
            dblData = ptrFloat->GetValue( Verify, IgnoreCache );
            aResult = StResult::Code::OK;
        }
        catch (GenICam::GenericException &e)
        {
			//TCHAR szTmp[256];
			//MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			//OutputDebugString(szTmp);
			//OutputDebugString(_T("\n"));
			//return false;
        }
    }
    return aResult;
}

StResult StGenStreamValue::GetGenFloatMinMaxParameter( StString lFeature, double & dblMin, double & dblMax )
{
	StResult aResult = StResult::Code::INVALID_PARAMETER;
	INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return aResult;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIFloat ) return aResult;

    CFloatPtr ptrFloat = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
            dblMin = ptrFloat->GetMin();
            dblMax = ptrFloat->GetMax();
            aResult = StResult::Code::OK;
        }
        catch (GenICam::GenericException &e)
        {
			//TCHAR szTmp[256];
			//MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			//OutputDebugString(szTmp);
			//OutputDebugString(_T("\n"));
			//return false;
        }
    }
    return aResult;
}

StResult StGenStreamValue::SetGenFloatParameter( StString lFeature, double dblData )
{
	StResult aResult = StResult::Code::INVALID_PARAMETER;
	INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return aResult;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIFloat ) return aResult;

    CFloatPtr ptrFloat = ptrINode;

    {
        try
        {
            ptrFloat->SetValue(dblData);
            aResult = StResult::Code::OK;
        }
        catch (GenICam::GenericException &e)
        {
			//TCHAR szTmp[256];
			//MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			//OutputDebugString(szTmp);
			//OutputDebugString(_T("\n"));
			//return false;
        }
    }
    return aResult;
}

StResult StGenStreamValue::GetGenBoolParameter( StString lFeature, bool & bData, bool Verify, bool IgnoreCache )
{
	StResult aResult = StResult::Code::INVALID_PARAMETER;
	INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return aResult;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIBoolean ) return aResult;

    CBooleanPtr ptrBoolean = ptrINode;

    //if(ptrBoolean.IsValid())
    {
        try
        {
            bData = (bool)ptrBoolean->GetValue( Verify, IgnoreCache );
            aResult = StResult::Code::OK;
        }
        catch (GenICam::GenericException &e)
        {
			//TCHAR szTmp[256];
			//MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			//OutputDebugString(szTmp);
			//OutputDebugString(_T("\n"));
			//return false;
        }
    }
    return aResult;
}

StResult StGenStreamValue::SetGenBoolParameter( StString lFeature, bool bData )
{
	StResult aResult = StResult::Code::INVALID_PARAMETER;
	INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return aResult;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIBoolean ) return aResult;

    CBooleanPtr ptrBoolean = ptrINode;

    //if(ptrBoolean.IsValid())
    {
        try
        {
            ptrBoolean->SetValue(bData);
            aResult = StResult::Code::OK;
        }
        catch (GenICam::GenericException &e)
        {
			//TCHAR szTmp[256];
			//MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			//OutputDebugString(szTmp);
			//OutputDebugString(_T("\n"));
			//return false;
        }
    }
    return aResult;
}


StResult StGenStreamValue::GetGenStringParameter( StString lFeature, StString & strData, bool Verify, bool IgnoreCache )
{
	StResult aResult = StResult::Code::INVALID_PARAMETER;
	INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return aResult;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIString ) return aResult;

	CStringPtr ptrString = ptrINode;
    //if(ptrString.IsValid())
    {
        try
        {
            strData = ptrString->ToString( Verify, IgnoreCache );
            aResult = StResult::Code::OK;
        }
        catch (GenICam::GenericException &e)
        {
			//TCHAR szTmp[256];
			//MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			//OutputDebugString(szTmp);
			//OutputDebugString(_T("\n"));
			//return false;
        }
    }
    return aResult;
}

StResult StGenStreamValue::SetGenStringParameter( StString lFeature, StString &strData )
{
	StResult aResult = StResult::Code::INVALID_PARAMETER;
	INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return aResult;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIString ) return aResult;

	CStringPtr ptrString = ptrINode;

    //if(ptrString.IsValid())
    {
        try
        {
			gcstring strValue = strData;
            ptrString->FromString(strValue);
            aResult = StResult::Code::OK;
        }
        catch (GenICam::GenericException &e)
        {
			//TCHAR szTmp[256];
			//MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			//OutputDebugString(szTmp);
			//OutputDebugString(_T("\n"));
			//return false;
        }
    }
    return aResult;
}


StResult StGenStreamValue::SetGenCommandParameter( StString lFeature )
{
	StResult aResult = StResult::Code::INVALID_PARAMETER;
	INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return aResult;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfICommand ) return aResult;

    CCommandPtr ptrCommand = ptrINode;

    //if(ptrCommand.IsValid())
    {
        try
        {
            ptrCommand->Execute();
            aResult = StResult::Code::OK;
        }
        catch (GenICam::GenericException &e)
        {
			//TCHAR szTmp[256];
			//MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			//OutputDebugString(szTmp);
			//OutputDebugString(_T("\n"));
			//return false;
        }
    }
    return aResult;
}

//--------------------------------------------------------------------
//SetGenICamControl
//--------------------------------------------------------------------
StResult StGenStreamValue::SetGenICamControl( LPCTSTR szFeature, LPCTSTR szData )
{

	if( m_hDS==NULL ) return StResult::Code::NOT_CREATESTREAM;
	GC_ERROR err = GC_ERR_SUCCESS;
	StResult aResult = StResult::Code::INVALID_PARAMETER;

	StString aName = szFeature;
	INode *ptrINode = m_pNodeMap->_GetNode(aName.GetAscii());
	if( !ptrINode ) return aResult;
	if( !ptrINode->IsFeature() )  return aResult;
	_EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	GenApi::_EAccessMode aAccessMode = ptrINode->GetAccessMode();
	if( aAccessMode!=WO && aAccessMode!=RW )  return aResult;

	StString strData = szData;
	if( aType==GenApi::intfICategory )
	{
	}
	else
	if( aType==GenApi::intfIInteger )
	{
	}
	else
	if( aType==GenApi::intfIBoolean )
	{
	}
	else
	if( aType==GenApi::intfIString  )
	{
		aResult = SetGenStringParameter( szFeature, strData );
	}
	else
	if( aType==GenApi::intfIFloat  )
	{
	}
	else
	if( aType==GenApi::intfIEnumeration  )
	{
		aResult = SetGenEnumParameter( szFeature, strData );
	}
	return aResult;
}

StResult StGenStreamValue::SetGenICamControl( LPCTSTR szFeature, INT nData )
{
	if( m_hDS==NULL ) return StResult::Code::NOT_CREATESTREAM;
	GC_ERROR err = GC_ERR_SUCCESS;
	StResult aResult = StResult::Code::INVALID_PARAMETER;

	StString aName = szFeature;
	INode *ptrINode = m_pNodeMap->_GetNode(aName.GetAscii());
	if( !ptrINode ) return aResult;
	if( !ptrINode->IsFeature() )  return aResult;
	_EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	GenApi::_EAccessMode aAccessMode = ptrINode->GetAccessMode();
	if( aAccessMode!=WO && aAccessMode!=RW )  return aResult;

	if( aType==GenApi::intfICategory )
	{
	}
	else
	if( aType==GenApi::intfIInteger )
	{
		aResult = SetGenIntParameter( szFeature, nData );
	}
	else
	if( aType==GenApi::intfIBoolean )
	{
		aResult = SetGenBoolParameter( szFeature, (bool)nData );
	}
	else
	if( aType==GenApi::intfIString  )
	{
	}
	else
	if( aType==GenApi::intfIFloat )
	{
		aResult = SetGenFloatParameter( szFeature, (double)nData );
	}
	else
	if( aType==GenApi::intfIEnumeration  )
	{
		aResult = SetGenEnumParameter( szFeature, nData );
	}
	return aResult;
}
StResult StGenStreamValue::SetGenICamControl( LPCTSTR szFeature, double dblData )
{
	if( m_hDS==NULL ) return StResult::Code::NOT_CREATESTREAM;
	GC_ERROR err = GC_ERR_SUCCESS;
	StResult aResult = StResult::Code::INVALID_PARAMETER;

	StString aName = szFeature;
	INode *ptrINode = m_pNodeMap->_GetNode(aName.GetAscii());
	if( !ptrINode ) return aResult;
	if( !ptrINode->IsFeature() )  return aResult;
	_EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	GenApi::_EAccessMode aAccessMode = ptrINode->GetAccessMode();
	if( aAccessMode!=WO && aAccessMode!=RW )  return aResult;

	if( aType==GenApi::intfICategory )
	{
	}
	else
	if( aType==GenApi::intfIInteger )
	{
		aResult = SetGenIntParameter( szFeature, (int)dblData );
	}
	else
	if( aType==GenApi::intfIBoolean )
	{
		aResult = SetGenBoolParameter( szFeature, (bool)dblData );
	}
	else
	if( aType==GenApi::intfIString  )
	{
	}
	else
	if( aType==GenApi::intfIFloat )
	{
		aResult = SetGenFloatParameter( szFeature, dblData );
	}
	else
	if( aType==GenApi::intfIEnumeration  )
	{
		aResult = SetGenEnumParameter( szFeature, (int)dblData );
	}
	return aResult;
}
StResult StGenStreamValue::SetGenICamControl( LPCTSTR szFeature, bool bData )
{
	if( m_hDS==NULL ) return StResult::Code::NOT_CREATESTREAM;
	GC_ERROR err = GC_ERR_SUCCESS;
	StResult aResult = StResult::Code::INVALID_PARAMETER;

	StString aName = szFeature;
	INode *ptrINode = m_pNodeMap->_GetNode(aName.GetAscii());
	if( !ptrINode ) return aResult;
	if( !ptrINode->IsFeature() )  return aResult;
	_EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	GenApi::_EAccessMode aAccessMode = ptrINode->GetAccessMode();
	if( aAccessMode!=WO && aAccessMode!=RW )  return aResult;

	if( aType==GenApi::intfICategory )
	{
	}
	else
	if( aType==GenApi::intfIInteger )
	{
		aResult = SetGenIntParameter( szFeature, (int)bData );
	}
	else
	if( aType==GenApi::intfIBoolean )
	{
		aResult = SetGenBoolParameter( szFeature, bData );
	}
	else
	if( aType==GenApi::intfIString  )
	{
	}
	else
	if( aType==GenApi::intfIFloat )
	{
		aResult = SetGenFloatParameter( szFeature, (double)bData );
	}
	else
	if( aType==GenApi::intfIEnumeration  )
	{
		aResult = SetGenEnumParameter( szFeature, (int)bData );
	}
	return aResult;
}

//--------------------------------------------------------------------
//GetGenICamControl
//--------------------------------------------------------------------
StResult StGenStreamValue::GetGenICamControl( LPCTSTR szFeature, LPTSTR szData, size_t size )
{
	if( m_hDS==NULL ) return StResult::Code::NOT_CREATESTREAM;
	GC_ERROR err = GC_ERR_SUCCESS;
	StResult aResult = StResult::Code::INVALID_PARAMETER;

	StString aName = szFeature;
	INode *ptrINode = m_pNodeMap->_GetNode(aName.GetAscii());
	if( !ptrINode ) return aResult;
	if( !ptrINode->IsFeature() )  return aResult;
	_EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	GenApi::_EAccessMode aAccessMode = ptrINode->GetAccessMode();
	if( aAccessMode!=RO && aAccessMode!=RW )  return aResult;

	StString strData;
	if( aType==GenApi::intfICategory )
	{
	}
	else
	if( aType==GenApi::intfIInteger )
	{
	}
	else
	if( aType==GenApi::intfIBoolean )
	{
	}
	else
	if( aType==GenApi::intfIString  )
	{
		aResult = GetGenStringParameter( szFeature, strData );
	}
	else
	if( aType==GenApi::intfIFloat  )
	{
	}
	else
	if( aType==GenApi::intfIEnumeration  )
	{
		aResult = GetGenEnumParameter( szFeature, strData );
	}
	if( aResult.IsOK() )
	{
		_tcscpy_s( szData,size,strData.GetUnicode() );
	}
	return aResult;
}
StResult StGenStreamValue::GetGenICamControl( LPCTSTR szFeature, INT &nData )
{
	if( m_hDS==NULL ) return StResult::Code::NOT_CREATESTREAM;
	GC_ERROR err = GC_ERR_SUCCESS;
	StResult aResult = StResult::Code::INVALID_PARAMETER;

	StString aName = szFeature;
	INode *ptrINode = m_pNodeMap->_GetNode(aName.GetAscii());
	if( !ptrINode ) return aResult;
	if( !ptrINode->IsFeature() )  return aResult;
	_EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	GenApi::_EAccessMode aAccessMode = ptrINode->GetAccessMode();
	if( aAccessMode!=RO && aAccessMode!=RW )  return aResult;

	if( aType==GenApi::intfICategory )
	{
	}
	else
	if( aType==GenApi::intfIInteger )
	{
		aResult = GetGenIntParameter( szFeature, nData );
	}
	else
	if( aType==GenApi::intfIBoolean )
	{
		bool bData;
		aResult = GetGenBoolParameter( szFeature, bData );
		if( aResult.IsOK())
		{
			nData = (INT)bData;
		}
	}
	else
	if( aType==GenApi::intfIString  )
	{
	}
	else
	if( aType==GenApi::intfIFloat )
	{
		double dblData;
		aResult = GetGenFloatParameter( szFeature, dblData );
		if( aResult.IsOK())
		{
			nData = (INT)dblData;
		}
	}
	else
	if( aType==GenApi::intfIEnumeration  )
	{
		aResult = GetGenEnumParameter( szFeature, nData );
	}
	return aResult;
}
StResult StGenStreamValue::GetGenICamControl( LPCTSTR szFeature, double &dblData )
{
	if( m_hDS==NULL ) return StResult::Code::NOT_CREATESTREAM;
	GC_ERROR err = GC_ERR_SUCCESS;
	StResult aResult = StResult::Code::INVALID_PARAMETER;

	StString aName = szFeature;
	INode *ptrINode = m_pNodeMap->_GetNode(aName.GetAscii());
	if( !ptrINode ) return aResult;
	if( !ptrINode->IsFeature() )  return aResult;
	_EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	GenApi::_EAccessMode aAccessMode = ptrINode->GetAccessMode();
	if( aAccessMode!=RO && aAccessMode!=RW )  return aResult;

	if( aType==GenApi::intfICategory )
	{
	}
	else
	if( aType==GenApi::intfIInteger )
	{
		int nData;
		aResult = GetGenIntParameter( szFeature, nData );
		if( aResult.IsOK() )
		{
			dblData = nData;
		}
	}
	else
	if( aType==GenApi::intfIBoolean )
	{
		bool bData;
		aResult = GetGenBoolParameter( szFeature, bData );
		if( aResult.IsOK() )
		{
			dblData = bData;
		}
	}
	else
	if( aType==GenApi::intfIString  )
	{
	}
	else
	if( aType==GenApi::intfIFloat )
	{
		aResult = GetGenFloatParameter( szFeature, dblData );
	}
	else
	if( aType==GenApi::intfIEnumeration  )
	{
		int nData;
		aResult = GetGenEnumParameter( szFeature, nData );
		if( aResult.IsOK() )
		{
			dblData = nData;
		}
	}
	return aResult;
}
StResult StGenStreamValue::GetGenICamControl( LPCTSTR szFeature, bool &bData )
{
	if( m_hDS==NULL ) return StResult::Code::NOT_CREATESTREAM;
	GC_ERROR err = GC_ERR_SUCCESS;
	StResult aResult = StResult::Code::INVALID_PARAMETER;

	StString aName = szFeature;
	INode *ptrINode = m_pNodeMap->_GetNode(aName.GetAscii());
	if( !ptrINode ) return aResult;
	if( !ptrINode->IsFeature() )  return aResult;
	_EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	GenApi::_EAccessMode aAccessMode = ptrINode->GetAccessMode();
	if( aAccessMode!=RO && aAccessMode!=RW )  return aResult;

	if( aType==GenApi::intfICategory )
	{
	}
	else
	if( aType==GenApi::intfIInteger )
	{
		int nData;
		aResult = GetGenIntParameter( szFeature, nData );
		if( aResult.IsOK() )
		{
			bData = (bool)nData;
		}
	}
	else
	if( aType==GenApi::intfIBoolean )
	{
		aResult = GetGenBoolParameter( szFeature, bData );
	}
	else
	if( aType==GenApi::intfIString  )
	{
	}
	else
	if( aType==GenApi::intfIFloat )
	{
		double dblData;
		aResult = GetGenFloatParameter( szFeature, dblData );
		if( aResult.IsOK() )
		{
			bData = (bool)dblData;
		}
	}
	else
	if( aType==GenApi::intfIEnumeration  )
	{
		int nData;
		aResult = GetGenEnumParameter( szFeature, nData );
		if( aResult.IsOK() )
		{
			bData = (bool)nData;
		}
	}
	return aResult;
}
//▲2017/10/20


//------------------------------------------------------------------------
//--StStream----------------------------------------------------------------------
//------------------------------------------------------------------------
/*
class StStream : public StGenStream
{
public:
	//StGenStream(void);
	//StStream(StGenGCLib* psGCLib,StGenDevice * psDev,StString strID);
	StStream(StGenDevice * psDev,StString strID);
	~StStream(void);

    bool Open(void);
    void Close(void);
    bool IsOpen(void) const {
		return (NULL != m_hDS);
	}

	//▼2014/07/23 追加----------------------------------
	//void *SetBufferCount(int iCount){ mNumBuffers = iCount;}
	//int GetBufferCount(void){ return mNumBuffers; }
	//▲2014/07/23 追加----------------------------------

	
    bool StartStream(void);
    bool StopStream(void);
	//StResult RetrieveBuffer( StBuffer ** aBuffer, uint32_t aTimeout, StResult * aOperationResult );
	//StResult RetrieveBuffer( StBuffer ** aBuffer, StUInt32 aTimeout = 0, StResult * aOperationResult = NULL );
	//StResult QueueBuffer( StBuffer * aBuffer );

    const StString& GetID(void) const {return m_strID;}
    //SimpleDevice* GetDevice(void) {return m_psDev;}

    //size_t GetPayloadSize(void);
    //uint64_t GetNumDelivered(void);
    //uint64_t GetNumQueueUnderrun(void);
    //uint32_t GetNumAnnounced(void);
    //uint32_t GetNumQueued(void);
    //uint32_t GetNumAwaitDelivery(void);
    //bool IsGrabStarted(void);
    //bool IsGrabbing(void) {return (NULL != m_hAcqThread);}


    void QueryInformations(void);
	const DS_HANDLE GetHandle(void){return m_hDS;}
	StBuffer *GetGenBuffer( int index )
	{
		if( !mpBuffers ) return NULL;
		if( index>=mNumBuffers ) return NULL;
		return mpBuffers+index;
	}
    const StString& GetPortName() const {return m_strPortName;}

	//▼2014/07/23 最後のデータポインタ保存----------------------------------
	const void *GetLastDataPointer(void){return mLastDataPointer;}
	StUInt64 GetLastFrameID(void){return mi64LastFrameID;}
	//▲2014/07/23 最後のデータポインタ保存----------------------------------

private:
    StGenGCLib* m_psGCLib;
	StGenDevice * m_psDev;
	DS_HANDLE m_hDS;
	EVENT_HANDLE m_hEvent;
    StString m_strID;
	StString m_strPortName;
	//StBuffer
	int mNumBuffers;
	StBuffer *mpBuffers;

	//▼2014/07/23 最後のデータポインタ保存----------------------------------
	//BufferView表示用にDataPointerを保存しておく
	const void *mLastDataPointer;
	StUInt64 mi64LastFrameID;
	//▲2014/07/23 最後のデータポインタ保存----------------------------------

	//StResult RetrieveBuffer( StBuffer ** aBuffer, StUInt32 aTimeout, StResult * aOperationResult );

    //std::vector<ImageBuffer* > m_vBuffers;

};
*/


//StGenStream::StGenStream(void)
//StStream::StStream( StGenGCLib* psGCLib, StGenDevice * psDevice, DEV_HANDLE hDevice, StString strID )
StStream::StStream( StGenGCLib* psGCLib, DEV_HANDLE hDevice, StString strID )
	//: m_psGCLib(psGCLib)
	//, m_psDev(psDev)
	//, m_strID(strID)
	//, m_strPortName("")
	//, m_hDS(NULL)
	//, m_hEvent(NULL)
	//, mNumBuffers(4)
	//, mpBuffers(NULL)
	////▼2014/07/23 最後のデータポインタ保存----------------------------------
	//, mLastDataPointer(NULL)
	//, mi64LastFrameID(0)
	////▲2014/07/23 最後のデータポインタ保存----------------------------------
{
	StGenStreamValue *pStream = new StGenStreamValue;
	mhData = (void *)pStream;
	if( mhData )
	{
		pStream->m_psGCLib = psGCLib;
		//pStream->m_psDevice = psDevice;
		pStream->m_strID = strID;
		pStream->m_hDevice = hDevice;
	}

}

StStream::~StStream(void)
{
	if( mhData )
	{
		StGenStreamValue * pValue = (StGenStreamValue *)mhData;
		delete pValue;
	}
}

bool StStream::QueryInformations( void )
{
	if( !mhData ) return false;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->QueryInformations();
}


//bool StStream::Open( void )
//{
//    if (!IsOpen())
//    {
//        GC_ERROR status = m_psGCLib->f_DevOpenDataStream(m_psDev->GetHandle(), m_strID.GetAscii(), &m_hDS);
//        if (GC_ERR_SUCCESS == status)
//        {
//            // update informations                
//            QueryInformations();
//
//            return true;
//        }
//        return false;
//    }
//    else
//        return true;   
//}
//
//void StStream::Close( void )
//{
//
//
//	//StopStream();
//    if (IsOpen())
//    {
//        //if (IsGrabbing() && WaitForSingleObject(m_evTerminate, 0))
//        //{
//        //    StopAcquisition();
//        //}
//    
//        m_psGCLib->f_DSClose(m_hDS);
//        m_hDS = NULL;
//    }
//}


//bool StStream::StartStream( void )
//{
//
//    GC_ERROR status;
//    //SimpleStream *pClass = (SimpleStream *)pClassInst;
//    DS_HANDLE hDatastream = GetHandle();
//
//	if( mNumBuffers==0 ) return false;
//
//	// Create and register New Buffer Event
//	//EVENT_NEW_BUFFER_DATA NewImageEventData;
//	EVENT_HANDLE hEventNewBuffer = NULL;
//	status = m_psGCLib->f_GCRegisterEvent( hDatastream, EVENT_NEW_BUFFER, &hEventNewBuffer);
//	if (status < 0)
//	{
//		//pClass->GetsTL()->HandleError( "Error No Event registered: ");
//		return false;
//	}
//
//	m_hEvent = hEventNewBuffer;
//
//
//
//
//	int lPayloadSizeValue = 0;
//	//bool bReval = m_psDev->GetGenIntParameter( "PayloadSize", lPayloadSizeValue );
//	bool bReval = m_psDev->GetPayloadSize( lPayloadSizeValue );
//
//	if(!bReval || lPayloadSizeValue==0 ) return false;
//	//バッファの確保
//    mpBuffers = new StBuffer[ mNumBuffers ];
//	if( !mpBuffers ) return false;
//
//    for ( INT i = 0; i < mNumBuffers; i++ )
//    {
//        mpBuffers[ i ].Alloc( (StInt32)lPayloadSizeValue );
//    }
//
//	BUFFER_HANDLE hBuffer=NULL;
//    for ( INT i = 0; i < mNumBuffers; i++ )
//    {
//        status = m_psGCLib->f_DSAnnounceBuffer( hDatastream, (void *)mpBuffers[i].GetDataPointer(), mpBuffers[i].GetAllocSize(),
//            (void *)&mpBuffers[i], &hBuffer);
//        if (status < 0)
//        {
//            return false;
//        }
//		mpBuffers[i].SetContext(hBuffer);
//		//Port
//
// 		StString aPortName;
//        INFO_DATATYPE iType = INFO_DATATYPE_UNKNOWN;
//        size_t iSize = 0;
//        status = m_psGCLib->f_GCGetPortInfo(hBuffer,PORT_INFO_PORTNAME, &iType, NULL, &iSize);
//        if ((GC_ERR_SUCCESS == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
//        {
//            char* str = new char[iSize];
//            //m_strDisplayName.reserve(iSize);
//            status = m_psGCLib->f_GCGetPortInfo(hBuffer,PORT_INFO_PORTNAME, &iType, str, &iSize);
//            aPortName = str;
//            delete[] (str);
//			mpBuffers[i].SetPortName(aPortName);
//        }
//    }
//
//
//	status = m_psGCLib->f_DSFlushQueue( hDatastream, ACQ_QUEUE_ALL_TO_INPUT);
//	if (status < 0)
//	{
//		//pClass->GetsTL()->HandleError( "Error in DSFlushQueue: ");
//		return false;
//	}
//
//
//	// Start Acquisition
//	status = m_psGCLib->f_DSStartAcquisition(hDatastream, ACQ_START_FLAGS_DEFAULT, 0xFFFFFFFFFFFFFFFF);
//	if (status < 0)
//	{
//		//pClass->GetsTL()->HandleError( "DSStartAcquisition failed: " );
//
//		//GC_ERROR aError;
//		//char errText[256];
//		//size_t txtLength = sizeof(errText);
//
//		//m_psGCLib->f_GCGetLastError(&aError,errText,&txtLength);
//		//StString aText = errText;
//
////TRACE(_T(" @@@@@aText=%s\n"),  aText.GetUnicode() );
//	
//		return false;
//	}
//
//
//    //m_psDev->TLParamsLocked(true);
//
//
//
//
//	//Thread------
//
//
//
//
//
//
//	/*
//
//	BUFFER_HANDLE hBuffer=NULL;
//	void * pDataPointer = (void *)aBuffer->GetDataPointer();
//	void *pPrivate=NULL;
//	//BOOL bCheckList = bCheckAnnounceBufferList(pDataPointer, &pPrivate);
//
//	if( bCheckList==FALSE ){
//		//プライベートポインタに(StBuffer *)を入れておく
//		//AnnounceBuffer
//		err = m_psGCLib->f_DSAnnounceBuffer( m_hDS, pDataPointer, aBuffer->GetSize(), (void *)aBuffer, &hBuffer );
//		if( err==GC_ERR_SUCCESS )
//		{
//			mStList.push_back(pDataPointer, hBuffer);
//			//mAnnounceBufferList.push_back(pDataPointer);
//		}
//	}
//	else
//	{
//		hBuffer = (HANDLE)pPrivate;
//	}
//
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//
//	//QueueBuffer
//	err = m_psGCLib->f_DSQueueBuffer( m_hDS, hBuffer );
//
//    for ( INT i = 0; i < mNumBuffers; i++ )
//    {
//        QueueBuffer( mpBuffers + i );
//		//printf( "QueueBuffer[%d]=%d ErrorCode=%s \n",i,res.IsOK(),res.GetCodeString().GetAscii() );
//    }
//
//
//    // Create and register New Buffer Event
//    EVENT_NEW_BUFFER_DATA NewImageEventData;
//    EVENT_HANDLE pEventNewBuffer = NULL;
//    status = m_psGCLib->f_GCRegisterEvent( hDatastream, EVENT_NEW_BUFFER, &pEventNewBuffer);
//    if (status < 0)
//    {
//        //pClass->GetsTL()->HandleError( "Error No Event registered: ");
//    }
//
//    m_psDev->TLParamsLocked(true);
//	*/
//
//
//	return true;
//}
//
//bool StStream::StopStream( void )
//{
//    GC_ERROR status;
//
//    DS_HANDLE hDatastream = GetHandle();
//
//       // ResetEvent(pClass->m_evTerminate);
//
//      //  pRecentBuffer = NULL;
//
//	// Stop Acquisition
//	// first stop Datastream ...
//	status = m_psGCLib->f_DSStopAcquisition(hDatastream, ACQ_STOP_FLAGS_DEFAULT);
//	if (status < 0)
//	{
//		//pClass->GetsTL()->HandleError( "DSStopAcquisition failed: " );
//	}
//	// ... then Stop Device
//	//pClass->GetDevice()->StopAcquisition();
//
//	// Cleanup
//	status = m_psGCLib->f_GCUnregisterEvent( hDatastream, EVENT_NEW_BUFFER);
//	if (status < 0)
//	{
//		//pClass->GetsTL()->HandleError( "Error unregistering Event: ");
//	}
//    m_hEvent = NULL;
//
//	status = m_psGCLib->f_DSFlushQueue( hDatastream, ACQ_QUEUE_ALL_DISCARD);
//	if (status < 0)
//	{
//		//pClass->GetsTL()->HandleError( "DSFlushQueue failed: " );
//	}
//    
//
//	//▼2014/07/23 最後のデータポインタ保存----------------------------------
//	mLastDataPointer = NULL;
//	mi64LastFrameID = 0;
//	//▲2014/07/23 最後のデータポインタ保存----------------------------------
//
//	// revoke buffers from transport layer
//	if( mpBuffers )
//	{
//		for ( INT i = 0; i < mNumBuffers; i++ )
//		{
//			status = m_psGCLib->f_DSRevokeBuffer( hDatastream, (GenICam::Client::BUFFER_HANDLE)mpBuffers[i].GetContext(), NULL, NULL);
//		
//			if (status < 0)
//			{
//				//return false;
//			}
//			mpBuffers[ i ].Free();
//    //for ( INT i = 0; i < mNumBuffers; i++ )
//    //{
//    //    mpBuffers[ i ].Alloc( (StInt32)lPayloadSizeValue );
//    //}
//
//
//
//	
//		}
//		delete [] mpBuffers;
//		mpBuffers = NULL;
//	}
//    //m_psDev->TLParamsLocked(false);
//
//	return true;
//}


//StResult StStream::RetrieveBuffer( StBuffer ** aBuffer, StUInt32 aTimeout, StResult * aOperationResult )
//{
//	//if( mhStream==NULL ) return StResult::Code::INVALID_PARAMETER;
//	if( m_hEvent==NULL ) return StResult::Code::INVALID_PARAMETER;
//
//	EVENT_NEW_BUFFER_DATA lEventData;
//	size_t lSize = sizeof lEventData;
//	GC_ERROR lGenTLRtn = m_psGCLib->f_EventGetData(m_hEvent, &lEventData, &lSize, aTimeout);
//
////aa
//
//
//
////{TCHAR text[256];
////swprintf_s( text,256,TEXT("RetrieveBuffer ==>lGenTLRtn=0x%x  \n"),lGenTLRtn);
////OutputDebugString(text);}
//
//	if (lGenTLRtn != GC_ERR_SUCCESS) {
//		*aBuffer = NULL;
//		return StResult::Code::TIMEOUT;
//	}
//	
//	*aBuffer = (StBuffer *)lEventData.pUserPointer;
//	BUFFER_HANDLE hBuffer = (BUFFER_HANDLE)lEventData.BufferHandle;	//BUFFER_HANDLE
//	//BUFFER_HANDLE(BUFFER_STRUCT)からStBufferに変換？？
//	INFO_DATATYPE iType;
//	size_t sData;
//	uint64_t i64Data;
//	size_t bufSize;
//	//Width
//	bufSize = sizeof(sData);
//	GC_ERROR err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_WIDTH, &iType, &sData, &bufSize );
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//	(*aBuffer)->SetWidth((uint32_t)sData);
//	//Height
//	bufSize = sizeof(sData);
//	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_HEIGHT, &iType, &sData, &bufSize );
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//	(*aBuffer)->SetHeight((uint32_t)sData);
//	//PixelType
//	bufSize = sizeof(i64Data);
//	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_PIXELFORMAT, &iType, &i64Data, &bufSize );
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//	(*aBuffer)->SetPixelType((StPixelType)i64Data);
//	//Timestamp
//	bufSize = sizeof(i64Data);
//	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_TIMESTAMP, &iType, &i64Data, &bufSize );
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//	(*aBuffer)->SetTimestamp(i64Data);
//	//XOFFSET
//	bufSize = sizeof(sData);
//	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_XOFFSET, &iType, &sData, &bufSize );
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//	(*aBuffer)->SetOffsetX((uint32_t)sData);
//	//YOFFSET
//	bufSize = sizeof(sData);
//	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_YOFFSET, &iType, &sData, &bufSize );
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//	(*aBuffer)->SetOffsetY((uint32_t)sData);
//	//XPADDING
//	bufSize = sizeof(sData);
//	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_XPADDING, &iType, &sData, &bufSize );
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//	(*aBuffer)->SetPaddingX((uint32_t)sData);
//	//YPADDING
//	bufSize = sizeof(sData);
//	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_YPADDING, &iType, &sData, &bufSize );
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//	(*aBuffer)->SetPaddingY((uint32_t)sData);
//
//	//バッファに書き込まれたサイズ
//	bufSize = sizeof(sData);
//	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_SIZE_FILLED, &iType, &sData, &bufSize );
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//	(*aBuffer)->SetSize((uint32_t)sData);
//	//ID
//	bufSize = sizeof(i64Data);
//	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_FRAMEID, &iType, &i64Data, &bufSize );
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//	(*aBuffer)->SetID(i64Data);
//
//	//BUFFER_INFO_IMAGEOFFSET
//	bufSize = sizeof(sData);
//	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_IMAGEOFFSET, &iType, &sData, &bufSize );
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
////TRACE(_T(" @@@@@BUFFER_INFO_IMAGEOFFSET=%d\n"),  sData );
//	(*aBuffer)->SetImageOffset((uint32_t)sData);
//
//	//BUFFER_INFO_PAYLOADTYPE
//	bufSize = sizeof(sData);
//	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_PAYLOADTYPE, &iType, &sData, &bufSize );
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
////TRACE(_T(" @@@@@BUFFER_INFO_PAYLOADTYPE=%d\n"),  sData );
//	(*aBuffer)->SetPayloadType((uint32_t)sData);
//
//
//	/*
//	//BUFFER_INFO_BASE
//	PBYTE pBase=NULL;
//	bufSize = sizeof(pBase);
//	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_BASE, &iType, &pBase, &bufSize );
//TRACE(_T(" @@@@@BUFFER_INFO_BASE=0x%x\n"),  pBase );
//	//if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//	//(*aBuffer)->SetImageOffset((uint32_t)sData);
//
//	//BUFFER_INFO_DELIVERED_IMAGEHEIGHT
//	bufSize = sizeof(sData);
//	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_DELIVERED_IMAGEHEIGHT, &iType, &sData, &bufSize );
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//TRACE(_T(" @@@@@BUFFER_INFO_DELIVERED_IMAGEHEIGHT=%d\n"),  sData );
//	*/
//
//	//▼2014/07/23 最後のデータポインタ保存----------------------------------
//	mLastDataPointer = (*aBuffer)->GetDataPointer();
//	mi64LastFrameID = (*aBuffer)->GetID();
//	//▲2014/07/23 最後のデータポインタ保存----------------------------------
//
//	return StResult::Code::OK;
//
//}



//StResult StStream::QueueBuffer( StBuffer * aBuffer )
//{
//	if( m_hDS==NULL ) return StResult::Code::INVALID_PARAMETER;
//	GC_ERROR err = GC_ERR_SUCCESS;
//
//    err = m_psGCLib->f_DSQueueBuffer( m_hDS, aBuffer->GetContext() ); //BUFFER_HANDLE(GetContext)
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//	return StResult::Code::OK;
//
//
//	/*
//	BUFFER_HANDLE hBuffer=NULL;
//	void * pDataPointer = (void *)aBuffer->GetDataPointer();
//	void *pPrivate=NULL;
//	//BOOL bCheckList = bCheckAnnounceBufferList(pDataPointer, &pPrivate);
//
//	err = m_psGCLib->f_DSAnnounceBuffer( m_hDS, pDataPointer, aBuffer->GetSize(), (void *)aBuffer, &hBuffer );
//
//
//
//    GC_ERROR status = m_psGCLib->f_DSQueueBuffer( m_hDS, aBuffer->GetContext());
//
//
//
//	if( bCheckList==FALSE ){
//		//プライベートポインタに(StBuffer *)を入れておく
//		//AnnounceBuffer
//		err = m_psGCLib->f_DSAnnounceBuffer( m_hDS, pDataPointer, aBuffer->GetSize(), (void *)aBuffer, &hBuffer );
//		if( err==GC_ERR_SUCCESS )
//		{
//			mStList.push_back(pDataPointer, hBuffer);
//			//mAnnounceBufferList.push_back(pDataPointer);
//		}
//	}
//	else
//	{
//		hBuffer = (HANDLE)pPrivate;
//	}
//
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//
//	//QueueBuffer
//	err = m_psGCLib->f_DSQueueBuffer( m_hDS, hBuffer );
//
//	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
//	return StResult::Code::OK;
//	*/
//}

//------------------------------------------------------------------------
//--StGenStream----------------------------------------------------------------------
//------------------------------------------------------------------------
StGenStream::StGenStream(void)
{
	//StGenStreamValue *pDevice = new StGenStreamValue;
	//mhData = (void *)pDevice;
}
StGenStream::~StGenStream(void)
{
	//if( mhData )
	//{
	//	StGenStreamValue * pValue = (StGenStreamValue *)mhData;
	//	delete pValue;
	//}
}

bool StGenStream::Open( void )
{

	if( !mhData ) return false;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->Open();
}

void StGenStream::Close( void )
{

	if( !mhData ) return;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	pStream->Close();
}
bool StGenStream::IsOpen( void ) const
{
	if( !mhData ) return false;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->IsOpen();
}

bool StGenStream::SetBufferCount(int iCount)
{
	if( !mhData ) return false;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->SetBufferCount( iCount );
}

StInt32 StGenStream::GetBufferCount(void)
{
	if( !mhData ) return 0;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->GetBufferCount();
}

bool StGenStream::SetBufferSize(int iSize)
{
	if( !mhData ) return false;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->SetBufferSize( iSize );
}

StInt32 StGenStream::GetBufferSize(void)
{
	if( !mhData ) return 0;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->GetBufferSize();
}

StResult StGenStream::StartStream(void)
{
	if( !mhData ) return StResult::Code::NOT_ENOUGH_MEMORY;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->StartStream();
}

StResult StGenStream::StopStream(void)
{
	if( !mhData ) return StResult::Code::NOT_ENOUGH_MEMORY;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->StopStream();
}

StResult StGenStream::RetrieveBuffer(StBuffer ** aBuffer, StUInt32 aTimeout, StResult * aOperationResult)
{
	if( !mhData ) return StResult::Code::NOT_ENOUGH_MEMORY;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->RetrieveBuffer(aBuffer,aTimeout,aOperationResult);
}
StResult StGenStream::QueueBuffer(StBuffer * aBuffer)
{
	if( !mhData ) return StResult::Code::NOT_ENOUGH_MEMORY;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->QueueBuffer(aBuffer);
}

//▼2017/10/17
StResult StGenStream::GetOutputQueueCount(PINT aCount)
{
	if( !mhData ) return StResult::Code::NOT_ENOUGH_MEMORY;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->GetOutputQueueCount(aCount);
}
//▲2017/10/17

const HANDLE StGenStream::GetHandle(void)
{
	if( !mhData ) return NULL;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->GetHandle();
}

//▼2017/10/20
//--------------------------------------------------------------------
//SetGenICamControl
//--------------------------------------------------------------------
StResult StGenStream::SetGenICamControl( LPCTSTR szFeature, LPCTSTR szData )
{
	if( !mhData ) return StResult::Code::NOT_CREATESTREAM;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->SetGenICamControl(szFeature, szData);
}
StResult StGenStream::SetGenICamControl( LPCTSTR szFeature, INT nData )
{
	if( !mhData ) return StResult::Code::NOT_CREATESTREAM;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->SetGenICamControl(szFeature, nData);

}
StResult StGenStream::SetGenICamControl( LPCTSTR szFeature, double dblData )
{
	if( !mhData ) return StResult::Code::NOT_CREATESTREAM;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->SetGenICamControl(szFeature, dblData);

}
StResult StGenStream::SetGenICamControl( LPCTSTR szFeature, bool bData )
{
	if( !mhData ) return StResult::Code::NOT_CREATESTREAM;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->SetGenICamControl(szFeature, bData);

}

//--------------------------------------------------------------------
//GetGenICamControl
//--------------------------------------------------------------------
StResult StGenStream::GetGenICamControl( LPCTSTR szFeature, LPTSTR szData, size_t size )
{
	if( !mhData ) return StResult::Code::NOT_CREATESTREAM;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->GetGenICamControl(szFeature, szData, size);
}
StResult StGenStream::GetGenICamControl( LPCTSTR szFeature, INT &nData )
{
	if( !mhData ) return StResult::Code::NOT_CREATESTREAM;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->GetGenICamControl(szFeature, nData);
}
StResult StGenStream::GetGenICamControl( LPCTSTR szFeature, double &dblData )
{
	if( !mhData ) return StResult::Code::NOT_CREATESTREAM;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->GetGenICamControl(szFeature, dblData);
}
StResult StGenStream::GetGenICamControl( LPCTSTR szFeature, bool &bData )
{
	if( !mhData ) return StResult::Code::NOT_CREATESTREAM;
	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
	return pStream->GetGenICamControl(szFeature, bData);
}

//const StString &StGenStream::GetPortName(void)
//{
//	if( !mhData ) return NULL;
//	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
//	return pStream->GetPortName();
//}
//const StString &StGenStream::GetID(void)
//{
//	if( !mhData ) return NULL;
//	StGenStreamValue *pStream = (StGenStreamValue *)mhData;
//	return pStream->GetID();
//}
//▲2017/10/20


