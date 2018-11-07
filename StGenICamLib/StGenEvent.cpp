#include "StdAfx.h"
#include "StGenEvent.h"


//#define _DEBUGTEST





//DWORD WINAPI Function(StGenEvent *pGenEvent);
/*
//unsigned WINAPI Link( void *aParam )
UINT WINAPI Link1( void *aParam )
{
	StGenEvent *lThis = reinterpret_cast<StGenEvent *>( aParam );
	unsigned Value = Function(lThis);
	return Value;
}

*/






//StGenEvent::StGenEvent(void)
//{
//}
//StGenEvent::StGenEvent(StGenGCLib* psGCLib,StGenDevice * psDev,StString strID, EVENT_TYPE_LIST aType, void (*nFuncCallback)(void *pVoid, char *pBuffer, unsigned int aSize), void *pUserPointer)
StGenEvent::StGenEvent(StGenGCLib* psGCLib,StGenDevice * psDev,StString strID, EVENT_TYPE_LIST aType, void (*nFuncCallback)(void *pVoid, void *pEvent), void *pUserPointer)
	: m_psGCLib(psGCLib)
	, m_psDev(psDev)
	, m_strID(strID)
	, m_hEvent(NULL)
	, m_eventType(aType)
	, mStop( false )
{
	m_aTimeout = 2000;
	m_pBuffer = NULL;
	m_aBufferSize = 0;
	//m_nFuncCallback = nFuncCallback;
	//m_pUserPointer = pUserPointer;
}

StGenEvent::~StGenEvent(void)
{
	if( m_pBuffer ) delete [] m_pBuffer;
//	UnregisterEvent();

	for( unsigned int i=0; i<m_vCallbackInfo.size(); i++ )
	{
		delete m_vCallbackInfo[i];
	}
	m_vCallbackInfo.clear();

	//-----------Adapter------------------
	for( unsigned int i=0; i<m_vAdapterCallbackInfo.size(); i++ )
	{
		delete m_vAdapterCallbackInfo[i];
	}
	m_vAdapterCallbackInfo.clear();



}

bool StGenEvent::EventFlush(void)
{
	return false;
}
void StGenEvent::EventGetData(void)
{

}

//enum EVENT_DATA_INFO_CMD_LIST
//{
//EVENT_DATA_ID           = 0,      /* Event specific    Unique Event ID (String or Number)*/
//EVENT_DATA_VALUE        = 1,      /* Event specific Data */
//EVENT_DATA_NUMID        = 2,      /* UNINT64   Numeric representation of the unique Event ID, GenTL v1.3. */
//
//EVENT_DATA_CUSTOM_ID    = 1000    /* Starting value for GenTL Producer custom IDs. */
//};
//typedef int32_t EVENT_DATA_INFO_CMD;

bool StGenEvent::EventGetDataInfo(EVENT_DATA_INFO_CMD iInfoCmd, void *pBuffer, size_t *piBufferSize)
{
	if( !m_psGCLib ) return false;
	if( !m_pBuffer ) return false;

	//void *pOutBuffer=pBuffer;
	size_t iOutSize=0;
	INFO_DATATYPE iType;
	if( iInfoCmd==EVENT_DATA_ID )
	{
		//String
		iType = INFO_DATATYPE_STRING;
//TRACE(_T(" @@@@@INFO_DATATYPE_STRING Size=%d\n"), *piBufferSize);
		
	}
	else
	if( iInfoCmd==EVENT_DATA_VALUE )
	//if( iInfoCmd==EVENT_DATA_DEVEVENT )
	{
		//Data type: BUFFER (optional data)
		iType = INFO_DATATYPE_BUFFER;
//TRACE(_T(" @@@@@INFO_DATATYPE_BUFFER Size=%d\n"), *piBufferSize );
	}
	else
	if( iInfoCmd==EVENT_DATA_NUMID )
	{
		//UINT64
		iType = INFO_DATATYPE_UINT64;
//TRACE(_T(" @@@@@EVENT_DATA_NUMID Size=%d\n"), *piBufferSize );
	}
	else
	{
		return false;
	}


	//if( iType == INFO_DATATYPE_BUFFER )
	//{
	//	pOutBuffer=NULL;
	//	if( pBuffer )
	//	{
	//		pOutBuffer=new char [*piBufferSize];
	//	}
	//}

	GC_ERROR status = m_psGCLib->f_EventGetDataInfo(m_hEvent,m_pBuffer,m_aBufferSize,iInfoCmd,&iType,pBuffer,piBufferSize);

	//if( iType==INFO_DATATYPE_BUFFER && pOutBuffer!=NULL )
	//{
	//	if( pOutBuffer )
	//	memcpy( pBuffer, pOutBuffer, *piBufferSize );
	//}

//TRACE(_T(" @@@@@EventGetDataInfo iBufferSize=%d\n"),*piBufferSize );

	//GC_ERROR EventGetDataInfo ( EVENT_HANDLE hEvent,
	//		const void * pInBuffer,
	//		size_t iInSize,
	//		EVENT_DATA_INFO_CMD iInfoCmd,
	//		INFO_DATATYPE * piType,
	//		void * pOutBuffer,
	//		size_t * piOutSize )

	if (status < 0)
	{
		return false;
	}
	return true;
}

void StGenEvent::EventGetInfo(void)
{
	//GC_ERROR EventGetInfo ( EVENT_HANDLE hEvent,
	//	EVENT_INFO_CMD iInfoCmd,
	//	INFO_DATATYPE * piType,
	//	void * pBuffer,
	//	size_t * piSize )
	  //enum EVENT_INFO_CMD_LIST
   //   {
   //     EVENT_EVENT_TYPE            = 0,    /* INT32      The event type of the event handle (EVENT_TYPE enum value). */
   //     EVENT_NUM_IN_QUEUE          = 1,    /* SIZET      Number of events in the event data queue. */
   //     EVENT_NUM_FIRED             = 2,    /* UINT64     Number of events that were fired since the creation of the module. */
   //     EVENT_SIZE_MAX              = 3,    /* SIZET      Max size of data carried with an event in bytes. GenTL v1.2 */
   //     EVENT_INFO_DATA_SIZE_MAX    = 4,    /* SIZET      Max size of data provided through EventGetDataInfo in bytes. GenTL v1.2 */

   //     EVENT_INFO_CUSTOM_ID        = 1000  /* Starting value for GenTL Producer custom IDs. */
   //   };



	//GC_ERROR status = m_psGCLib->f_EventGetInfo( m_hEvent,  );
	//char buffer[32];
	//size_t bufsize = sizeof(buffer);
	//INFO_DATATYPE iType = INFO_DATATYPE_STRING;
	//status = m_psGCLib->f_EventGetDataInfo(m_hEvent, m_pBuffer, m_aBufferSize, EVENT_DATA_ID, &iType, buffer, &bufsize);



}

void StGenEvent::EventKill(void)
{
	if( !m_psGCLib ) return;
	if( !m_hEvent ) return;
	GC_ERROR status = m_psGCLib->f_EventKill( m_hEvent );
}


bool StGenEvent::RegisterEvent(void)
{
    GC_ERROR status;
	bool bReval = false;

	if( !m_psGCLib ) return bReval;
	if( !m_psDev ) return bReval;
	EVENT_HANDLE hEvent = NULL;
	//GCRegisterEvent
	//EVENT_TYPE_LIST::EVENT_CUSTOM_ID = 100
	//EVENT_TYPE_LIST::EVENT_NEW_BUFFER = 1
	//EVENT_TYPE_LIST::EVENT_FEATURE_INVALIDATE = 2
	//EVENT_TYPE_LIST::EVENT_FEATURE_CHANGE = 3
	//EVENT_TYPE_LIST::EVENT_ERROR = 0
	//EVENT_TYPE_LIST::EVENT_FEATURE_DEVEVENT = 4
	status = m_psGCLib->f_GCRegisterEvent( m_psDev->GetHandle(), m_eventType, &hEvent);
	if (status < 0)
	{
		return false;
	}


	size_t aBufferSize;
	size_t aSize = sizeof(aBufferSize);
	INFO_DATATYPE iType = INFO_DATATYPE_SIZET;
	status = m_psGCLib->f_EventGetInfo(hEvent, EVENT_SIZE_MAX, &iType, &aBufferSize, &aSize );

	if (status < 0)
	{
		return false;
	}

	if( m_pBuffer ) delete [] m_pBuffer;
	m_pBuffer = new char [aBufferSize];
	if( !m_pBuffer ) return false;

	m_hEvent = hEvent;
	m_aBufferSize = aBufferSize;

	//if( m_eventType == EVENT_TYPE_LIST::EVENT_FEATURE_INVALIDATE )
	//{
	//}
	//else
	//if( m_eventType == EVENT_TYPE_LIST::EVENT_FEATURE_CHANGE )
	//{
	//}
	//else
	//if( m_eventType == EVENT_TYPE_LIST::EVENT_FEATURE_DEVEVENT )
	//{
	//}
	//else
	//{
	//}


	bReval = true;
	return bReval;
}

void StGenEvent::UnregisterEvent(void)
{
	if( !m_psGCLib ) return;
	if( !m_psDev ) return;
	if( !m_hEvent ) return;

	//GCUnregisterEvent
	GC_ERROR status = m_psGCLib->f_GCUnregisterEvent( m_psDev->GetHandle(), m_eventType);
	m_hEvent = NULL;

}

//-----Adapter--------------------------
bool StGenEvent::SetEventAdapterRegisterMain(void)
{
	bool bReval = false;

	//▼1.0.0.28　EventTestTimestamp等のEvent項目をDeviceViewに表示
	//GenApi::CNodeMapRef *pEventMap = m_psDev->GetEventMap();
	GenApi::CNodeMapRef *pEventMap = m_psDev->GetRemoteNodeMap();
	//▲1.0.0.28　EventTestTimestamp等のEvent項目をDeviceViewに表示
	if( !pEventMap ) return bReval;

	GenApi::CNodePtr pNode = pEventMap->_GetNode("Root");
	NodeList_t aNodeList;
	pNode->GetChildren(aNodeList);

	bReval = SetEventAdapterRegister(aNodeList);

	return bReval;
}

struct featureName{
    StString 	aName;
	unsigned int	iID;
	unsigned int	nIndex;
};
GenApi::CNodePtr GetSortNodePtr2(CNodeMapRef *pNodeMap, NodeList_t& aNodes, unsigned int index)
{
	//if( m_pDeviceMap==NULL) return NULL;
	unsigned int nodeNum = aNodes.size();
	if( nodeNum<=index ) return NULL;

	featureName *fname = new featureName[nodeNum];
	if( fname==NULL ) return NULL;

	for( unsigned int i=0; i<nodeNum; i++ )
	{
		fname[i].aName = aNodes[i]->GetName();
		fname[i].iID = -1;
		fname[i].nIndex = i;
	}

	NodeList_t allNodes;
	pNodeMap->_GetNodes(allNodes);
	unsigned int allNum = allNodes.size();
	unsigned int iCount=0;
	for( unsigned int a=0; a<allNum; a++ )
	{
		for( unsigned int i=0; i<nodeNum; i++ )
		{
			if( fname[i].aName==allNodes[a]->GetName() )
			{
				fname[i].iID = a;
				iCount++;
				break;
			}
		}
		if( iCount>=nodeNum ) break;
	}

	//Sort
	featureName dammy;
	for( unsigned int i=0; i<nodeNum-1; i++ )
	{
		for( unsigned int j=i+1; j<nodeNum; j++ )
		{
			if( fname[i].iID>fname[j].iID )
			{
				dammy = fname[i];
				fname[i] = fname[j];
				fname[j] = dammy;
			}
		}
	}


	//return;
	unsigned int aIndex = fname[index].nIndex;
	delete [] fname;
	if( aIndex>=nodeNum ) return NULL;
	GenApi::CNodePtr pNode = aNodes[aIndex];
	return pNode;

}

bool StGenEvent::SetEventAdapterRegister(GenApi::NodeList_t & aNodeList, int deps)
{

	int nodeNum = aNodeList.size();
	if( nodeNum==0 ) return true;
	NodeList_t nChildNodeList;
	//GenApi::CNodeMapRef *pEventMap = m_psDev->GetEventMap();
	GenApi::CNodeMapRef *pEventMap = m_psDev->GetRemoteNodeMap();

	if( pEventMap )
	{

		int nodeNum = aNodeList.size();
		for( int i=0; i<nodeNum; i++ )
		{
		//▼1.0.0.30 例外処理追加　
		try
		{
		//▲1.0.0.30 例外処理追加　

			//GenApi::CNodePtr pNode = aNodeList[i];
			GenApi::CNodePtr pNode = GetSortNodePtr2( pEventMap, aNodeList, i );

			StString cName = pNode->GetName();
//TRACE(_T(" @@@@@@@@@@@@@@@@@ 1 aName=%s \n"), cName.GetUnicode() );

			//GenApi::INode *ptrINode = pDeviceMap->_GetNode(aName.GetAscii());

			//if( 
			//	//cName == "EventExposureEndTimestamp" ||
			//	//cName == "EventTestTimestamp" ||
			//	cName == "Width"
			//	)
			//{
			//	Sleep(1);
			//}


//StString cName = pNode->GetName();
//StString aaEventID = pNode->GetEventID();
//
//TRACE(_T(" @@@@@@@@@@@@@@@@@ 1 aName[%d]=%s aEventID=%s \n"), i, cName.GetUnicode(), aaEventID.GetUnicode() );
//TRACE(_T("                    IsFeature[%d] GetAccessMode=%d[NI:%d NA:%d] \n"), pNode->IsFeature(), pNode->GetAccessMode(), GenApi::NI,GenApi::NA );

			//aVisibility
			EVisibility aVisibility = pNode->GetVisibility();
			if( EVisibility::Beginner!=aVisibility && EVisibility::Expert!=aVisibility && EVisibility::Guru!=aVisibility )
				continue;

			//IsFeature
			if( !pNode->IsFeature() ) continue;





			GenApi::_EAccessMode aAccessMode = pNode->GetAccessMode();
			if( aAccessMode!=GenApi::NI )
			{

				GenApi::_EInterfaceType aType = pNode->GetPrincipalInterfaceType();
				bool bCallback = false;
				if( aType==GenApi::intfICategory )		bCallback = false;
				else if( aType==GenApi::intfIInteger )	bCallback = true;
				else if( aType==GenApi::intfIBoolean )	bCallback = true;
				else if( aType==GenApi::intfIString )	bCallback = true;
				else if( aType==GenApi::intfIFloat )		bCallback = true;
				else if( aType==GenApi::intfIEnumeration )		bCallback = true;
				else if( aType==GenApi::intfIRegister )		bCallback = true;




 			//StString aName = aNode->GetName();
			//GenApi::INode *ptrINode = pEventMap->_GetNode(aName.GetAscii());

				StString aEventID = pNode->GetEventID();
				StString aName = pNode->GetName();
				////if( aEventID!="" )
				//if( aName == "EventExposureEndTimestamp" ||
				//	aName == "EventTestTimestamp"
				//	)
				if( bCallback )
				{
//TRACE(_T(" @@@@@@@@@@@@@@@@@ 2 aName=%s aEventID=%s \n"), aName.GetUnicode(), aEventID.GetUnicode() );

					//▼1.0.0.28　EventTestTimestamp等のEvent項目をDeviceViewに表示
					//これをおこなうとGenTLControlViewのRegister登録が無効になってしまう。２つ同時に登録はできないみたい
					//GenApi::Register( pNode, *this, &StGenEvent::AdapterCallback );
					//▲1.0.0.28　EventTestTimestamp等のEvent項目をDeviceViewに表示
				}
			}
			pNode->GetChildren(nChildNodeList);
			if( nChildNodeList.size()>0 )
			{
				SetEventAdapterRegister(nChildNodeList, deps+1);
			}

		//▼1.0.0.30 例外処理追加　
		}
		catch(GenICam::GenericException &e)
		{
			CString errDsc(e.GetDescription());
//TRACE(_T(" @@@@@@@@@@@@@@@@@Error=%s \n"), errDsc);
		}	//try
		//▲1.0.0.30 例外処理追加　

		}
	}
	return true;
}

bool StGenEvent::StartEvent(void)
{
//#ifndef _DEBUGTEST
	RegisterEvent();
//#endif


//return true;
//	mHandle = (HANDLE)AfxBeginThread(Link1,this);

	/*
	GenApi::CNodeMapRef *pDeviceMap = GetGenDevice()->GetRemoteNodeMap();

	CEventAdapterGeneric aEventAdapter(pDeviceMap->_Ptr);

	unsigned char data[] = {0x00, 0x00, 0xFF, 0x4F, 0x67, 0xDA, 0xB9, 0x4D, 0x9F, 0x21, 0x00, 0x00};
	WORD wEventID = *(PWORD)(&data[2]);
	aEventAdapter.DeliverMessage( data, sizeof(data), wEventID);



return true;
*/

	SetEventAdapterRegisterMain();

	mHandle = (HANDLE)_beginthreadex(
		NULL,
		0,
		Link,
		this,
		0,
		&mID);
	

/*
	mHandle = ::CreateThread(
		NULL,				// Security attributes
		0,					// Stack size, 0 is default
		Link,				// Start address
		this,				// Parameter
		0,					// Creation flags
		&mID );				// Thread ID
*/
	return true;
}

void StGenEvent::StopEvent(void)
{
	ASSERT( mHandle != INVALID_HANDLE_VALUE );

	mStop = true;
	DWORD lRetVal = ::WaitForSingleObject( mHandle, INFINITE );
	ASSERT( lRetVal != WAIT_TIMEOUT  );

	CloseHandle( mHandle );
	mHandle = INVALID_HANDLE_VALUE;

	mID = 0;

	UnregisterEvent();
}

bool StGenEvent::IsStopping() const
{
	ASSERT( mHandle != INVALID_HANDLE_VALUE );
	return mStop;
}

// ==========================================================================
bool StGenEvent::IsDone()
{
	if ( ( mHandle == INVALID_HANDLE_VALUE ) ||
		 ( mID == 0 ) )
	{
		return true;
	}

	return ( ::WaitForSingleObject( mHandle, 0 ) == WAIT_OBJECT_0 );
}


// ==========================================================================
//unsigned long WINAPI StGenEvent::Link( void *aParam )
unsigned WINAPI StGenEvent::Link( void *aParam )
//unsigned long StGenEvent::Link( void *aParam )
{
	StGenEvent *lThis = reinterpret_cast<StGenEvent *>( aParam );
	lThis->mReturnValue = lThis->Function();
	return lThis->mReturnValue;
}

// ==========================================================================
DWORD StGenEvent::GetReturnValue()
{
	return mReturnValue;
}

// ==========================================================================
DWORD StGenEvent::Function()
//DWORD WINAPI Function(StGenEvent *pGenEvent)
{

/*
Sleep(5000);

	GenApi::CNodeMapRef *pDeviceMap = GetGenDevice()->GetRemoteNodeMap();

	CEventAdapterGeneric aEventAdapter(pDeviceMap->_Ptr);

	unsigned char data[] = {0x00, 0x00, 0xFF, 0x4F, 0x67, 0xDA, 0xB9, 0x4D, 0x9F, 0x21, 0x00, 0x00};
	WORD wEventID = *(PWORD)(&data[2]);
	aEventAdapter.DeliverMessage( data, sizeof(data), wEventID);

return 0;
*/







	//ASSERT( mDisplayWnd != NULL );
	//ASSERT( mStream != NULL );

    // Timestamp used to limit display rate
//Sleep(5000);
	DWORD lPrevious = 0;

	//GenApi::CNodeMapRef *pDeviceMap = GetGenDevice()->GetRemoteNodeMap();

	//CEventAdapterGeneric aEventAdapter(pDeviceMap->_Ptr);
 //  // aEventAdapter.AttachNodeMap(pDeviceMap->_Ptr ); // attach the same nodemap again, should not break anything



	//unsigned char data[] = {0x00, 0x00, 0xFF, 0x4F, 0x67, 0xDA, 0xB9, 0x4D, 0x9F, 0x21, 0x00, 0x00};
	//WORD wEventID = *(PWORD)(&data[2]);
	//aEventAdapter.DeliverMessage( data, sizeof(data), wEventID);



	for ( ;; )
	{
		// Check if we were signaled to terminate
        //if ( pGenEvent->IsStopping() )
        if ( IsStopping() )
		{
			break;
		}
		//size_t lSize;
		GC_ERROR status = GC_ERR_SUCCESS;

#ifdef _DEBUGTEST
//Sleep(m_aTimeout);
Sleep(5000);
#else
		size_t aBufferSize = m_aBufferSize;
		status = m_psGCLib->f_EventGetData(m_hEvent, m_pBuffer, &aBufferSize, m_aTimeout);
//TRACE(_T(" @@@@@f_EventGetData m_aBufferSize=%d\n"),m_aBufferSize );
#endif
		if( status == GC_ERR_SUCCESS )
		{




#ifdef _DEBUGTEST
			
//TestExecute();
	unsigned char data[] = {0x00, 0x00, 0xFF, 0x4F, 0x67, 0xDA, 0xB9, 0x4D, 0x9F, 0x21, 0x00, 0x00};
	WORD wEventID = *(PWORD)(&data[2]);
	//bool bOpen;

	//CEventAdapterGeneric *pEventAdapter = 
	//			GetGenDevice()->m_pEventAdapter;
//				pGenEvent->GetGenDevice()->m_pEventAdapter;

//TRACE(_T(" @@@@@@@@ 1    pDevice=0x%x\n"), pDevice);
//TRACE(_T(" @@@@@@@@ 1    pEventAdapter=0x%x wEventID=%d\n"), pEventAdapter, wEventID);

	CEventAdapterGeneric aEventAdapter(pDeviceMap->_Ptr);
	aEventAdapter.DeliverMessage( data, sizeof(data), wEventID);

	
	break;
#else









			//EVENT_DATA_INFO_CMD  
				//EVENT_DATA_ID           = 0,      /* Event specific    Unique Event ID (String or Number)*/
				//EVENT_DATA_VALUE        = 1,      /* Event specific Data */
				//EVENT_DATA_NUMID        = 2,      /* UNINT64   Numeric representation of the unique Event ID, GenTL v1.3. */

				//GC_API EventGetDataInfo        ( EVENT_HANDLE hEvent, const void *pInBuffer, size_t iInSize, 
				//EVENT_DATA_INFO_CMD iInfoCmd, INFO_DATATYPE *piType, void *pOutBuffer, size_t *piOutSize );

			//EVENT_FEATURE_DEVEVENT
			//EVENT_DATA_ID     Data type: STRING (Event ID)
			//
			
			//EVENT_DATA_VALUE  Data type: BUFFER (optional data)
			//アドレス0と一致しているバッファの始まって、遠隔デバイスのnodemapイベント・ポートで、アドレス指定可能なデータと一致します。
			char buffer[32];
			size_t bufsize = sizeof(buffer);
			INFO_DATATYPE iType = INFO_DATATYPE_STRING;
			status = m_psGCLib->f_EventGetDataInfo(m_hEvent, m_pBuffer, m_aBufferSize, EVENT_DATA_ID, &iType, buffer, &bufsize);
//StString aString = buffer;
//TRACE(_T(" @@@@@EVENT_DATA_ID [%s] \n"), aString.GetUnicode());
			//status = m_psGCLib->f_EventGetDataInfo(m_hEvent, m_pBuffer, m_aBufferSize, EVENT_DATA_VALUE, &iType, buffer, &bufsize);

			/*
			if( m_nFuncCallbackDevCon )
			{
				//m_nFuncCallback(m_pUserPointer, m_pBuffer,m_aBufferSize);
				m_nFuncCallbackDevCon(m_pUserPointerDevCon, this);
				//m_nFuncCallback();
			}

			if( m_nFuncCallback )
			{
				//m_nFuncCallback(m_pUserPointer, m_pBuffer,m_aBufferSize);
				m_nFuncCallback(m_pUserPointer, this);
				//m_nFuncCallback();
			}
			*/


			//==================DeliverMessage==============================================
			//EVENT_DATA_VALUE-----------------------------------
			unsigned char *pDataValue=NULL;
			size_t iValueSize=0;
			//iSize=0;
			BOOL bReval = EventGetDataInfo(EVENT_DATA_VALUE,NULL,&iValueSize);
			if( bReval )
			{
				pDataValue = new unsigned char [iValueSize+16];
			}
			if( pDataValue )
			{
				memset( pDataValue,0,iValueSize+16 );
				bReval = EventGetDataInfo(EVENT_DATA_VALUE,pDataValue,&iValueSize);
			}

			if( iValueSize>0 )
			{
				WORD wEventID = *(PWORD)(&pDataValue[2]);


//TRACE(_T(" @@@@@@@@@@@@@@@@@wEventID=%d wEventID=0x%x\n"), wEventID,wEventID);
//for( unsigned int i=0; i<iValueSize; i++ )
//{
//TRACE(_T("%02X "), pDataValue[i]&0xff);
//}
//TRACE(_T("\n"));
//TRACE(_T(" @@@@@@@@ 2    m_pEventAdapter=0x%x  EventID=%d\n"), &aEventAdapter, wEventID);


		//		CEventAdapterGeneric aEventAdapter(pDeviceMap->_Ptr);
		//		aEventAdapter.DeliverMessage( pDataValue, iValueSize, wEventID);

				CEventAdapterGeneric * aAdapter = GetGenDevice()->m_pEventAdapter;
				//DeliverMessageでCallBackがよびだされる
				aAdapter->DeliverMessage( pDataValue, iValueSize, wEventID);

			}
			if( pDataValue ) delete [] pDataValue;


/*
			for( unsigned int i=0; i<m_vCallbackInfo.size(); i++ )
			{
				//void (*nFuncCallback)(void *pVoid, void *pEvent);
				//nFuncCallback = m_vCallbackInfo[i]->nFuncCallback;
				//nFuncCallback( m_vCallbackInfo[i]->pUserPointer, this );
				m_vCallbackInfo[i]->nFuncCallback( m_vCallbackInfo[i]->pUserPointer, this );
			}

*/


#endif

		}
	}
	//aEventAdapter.DetachNodeMap();

	return 0;
}



DWORD StGenEvent::TestExecute()
{

//	unsigned char data[] = {0x00, 0x00, 0xFF, 0x4F, 0x67, 0xDA, 0xB9, 0x4D, 0x9F, 0x21, 0x00, 0x00};
//	WORD wEventID = *(PWORD)(&data[2]);
//	//bool bOpen;
//
//	CEventAdapterGeneric *pEventAdapter = 
//				GetGenDevice()->m_pEventAdapter;
//
////TRACE(_T(" @@@@@@@@ 1    pDevice=0x%x\n"), pDevice);
//TRACE(_T(" @@@@@@@@ 1    pEventAdapter=0x%x wEventID=%d\n"), pEventAdapter, wEventID);
//
//	pEventAdapter->DeliverMessage( data, sizeof(data), wEventID);
//

	return 0;

}

/*
bool StGenEvent::RetrieveEventData( StGenBuffer ** aBuffer, StUInt32 aTimeout )
{
	//if( mhStream==NULL ) return StResult::Code::INVALID_PARAMETER;
	if( m_hEvent==NULL ) return false;

	EVENT_NEW_BUFFER_DATA lEventData;
	size_t lSize = sizeof lEventData;
	GC_ERROR lGenTLRtn = m_psGCLib->f_EventGetData(m_hEvent, &lEventData, &lSize, aTimeout);


//{TCHAR text[256];
//swprintf_s( text,256,TEXT("RetrieveBuffer ==>lGenTLRtn=0x%x  \n"),lGenTLRtn);
//OutputDebugString(text);}

	if (lGenTLRtn != GC_ERR_SUCCESS) {
		*aBuffer = NULL;
		return StResult::Code::TIMEOUT;
	}
	
	*aBuffer = (StGenBuffer *)lEventData.pUserPointer;
	BUFFER_HANDLE hBuffer = (BUFFER_HANDLE)lEventData.BufferHandle;	//BUFFER_HANDLE
	//BUFFER_HANDLE(BUFFER_STRUCT)からStBufferに変換？？
	INFO_DATATYPE iType;
	size_t sData;
	uint64_t i64Data;
	size_t bufSize;
	//Width
	bufSize = sizeof(sData);
	GC_ERROR err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_WIDTH, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
	(*aBuffer)->SetWidth((uint32_t)sData);
	//Height
	bufSize = sizeof(sData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_HEIGHT, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
	(*aBuffer)->SetHeight((uint32_t)sData);
	//PixelType
	bufSize = sizeof(i64Data);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_PIXELFORMAT, &iType, &i64Data, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
	(*aBuffer)->SetPixelType((StPixelType)i64Data);
	//Timestamp
	bufSize = sizeof(i64Data);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_TIMESTAMP, &iType, &i64Data, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
	(*aBuffer)->SetTimestamp(i64Data);
	//XOFFSET
	bufSize = sizeof(sData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_XOFFSET, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
	(*aBuffer)->SetOffsetX((uint32_t)sData);
	//YOFFSET
	bufSize = sizeof(sData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_YOFFSET, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
	(*aBuffer)->SetOffsetY((uint32_t)sData);
	//XPADDING
	bufSize = sizeof(sData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_XPADDING, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
	(*aBuffer)->SetPaddingX((uint32_t)sData);
	//YPADDING
	bufSize = sizeof(sData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_YPADDING, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
	(*aBuffer)->SetPaddingY((uint32_t)sData);

	//バッファに書き込まれたサイズ
	bufSize = sizeof(sData);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_SIZE_FILLED, &iType, &sData, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
	(*aBuffer)->SetSize((uint32_t)sData);
	//ID
	bufSize = sizeof(i64Data);
	err = m_psGCLib->f_DSGetBufferInfo ( m_hDS, hBuffer, BUFFER_INFO_SIZE_FILLED, &iType, &i64Data, &bufSize );
	if( err!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
	(*aBuffer)->SetID(i64Data);


	return StResult::Code::OK;

}
*/

//void StGenEvent::SetEventCallback(void (*nFuncCallback)(void *pVoid, char *pBuffer, unsigned int aSize), void *pUserPointer)
void StGenEvent::SetEventCallback(void (*nFuncCallback)(void *pVoid, void *pEvent), void *pClassPointer, void *pUserPointer)
{
	//m_nFuncCallback = nFuncCallback;
	//m_pUserPointer = pUserPointer;

	CallbackInfo * pCallbackInfo = new CallbackInfo;
	pCallbackInfo->nFuncCallback = nFuncCallback;
	pCallbackInfo->pClassPointer = pClassPointer;
	pCallbackInfo->pUserPointer = pUserPointer;
	m_vCallbackInfo.push_back(pCallbackInfo);
}

//void StGenEvent::SetEventCallbackDevCon(void (*nFuncCallback)(void *pVoid, void *pEvent), void *pUserPointer)
//{
//	m_nFuncCallbackDevCon = nFuncCallback;
//	m_pUserPointerDevCon = pUserPointer;
//}


//-----------Adapter------------------

void StGenEvent::SetEventAdapterCallback(void (*nFuncCallback)(void *pVoid, void *pEvent), void *pClassPointer, void *pUserPointer)
{

	CallbackInfo * pCallbackInfo = new CallbackInfo;
	pCallbackInfo->nFuncCallback = nFuncCallback;
	pCallbackInfo->pClassPointer = pClassPointer;
	pCallbackInfo->pUserPointer = pUserPointer;
	m_vAdapterCallbackInfo.push_back(pCallbackInfo);
}


void StGenEvent::AdapterCallback(GenApi::INode* pNode)
{

	try
	{
		//LPTCSTR szText[];
		TCHAR szText[256];
	
		StString aString = pNode->GetName();
	//TRACE(_T(" @@@@@GetName=%s\n"), aString.GetUnicode() );

	//return;
		aString += " : ";
		GenApi::_EInterfaceType aType = pNode->GetPrincipalInterfaceType();
		if( aType==GenApi::intfIInteger )
		{
			GenApi::CIntegerPtr ptrInteger = pNode;
			int64_t aValue = ptrInteger->GetValue();
			_stprintf_s(szText,_countof(szText),_T("%I64d(0x%08x)"), aValue, aValue );
		}
		else
		if( aType==GenApi::intfIBoolean )
		{
			GenApi::CBooleanPtr ptrBoolean = pNode;
			bool bData = ptrBoolean->GetValue();
			_stprintf_s(szText,_countof(szText),_T("%s"), bData?_T("true"):_T("false") );
		}
		else
		if( aType==GenApi::intfIString  )
		{
			GenApi::CStringPtr ptrString = pNode;
			StString strData = ptrString->GetValue();
			_stprintf_s(szText,_countof(szText),_T("%s"), strData.GetUnicode() );

		}
		else
		if( aType==GenApi::intfIFloat  )
		{
			GenApi::CFloatPtr ptrFloat = pNode;
			double dblValue = ptrFloat->GetValue();
			_stprintf_s(szText,_countof(szText),_T("%f"), dblValue );

		}
		else
		if( aType==GenApi::intfIEnumeration  )
		{
			GenApi::CEnumerationPtr ptrEnumeration = pNode;
			unsigned int uiData = (unsigned int)ptrEnumeration->GetIntValue();
			GenApi::IEnumEntry *ptrEnumEntry = ptrEnumeration->GetEntry(uiData);
			StString strData = ptrEnumEntry->GetNode()->GetDisplayName();
			_stprintf_s(szText,_countof(szText),_T("%s(%u)"), strData.GetUnicode(), uiData );
		}
		else
		{
			_stprintf_s(szText,_countof(szText),_T("") );
		}

		aString += szText;

		for( unsigned int i=0; i<m_vCallbackInfo.size(); i++ )
		{
			m_vAdapterCallbackInfo[i]->nFuncCallback( m_vCallbackInfo[i]->pUserPointer, (void *)aString.GetUnicode() );
		}


	}
	catch(...)
	{


	}

}