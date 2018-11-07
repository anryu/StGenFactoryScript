#pragma once

//#include "StGenInterface.h"
#include "StGenDevice.h"
#include "StBuffer.h"

class StGenStream
{
public:
	StGenStream(void);
	~StGenStream(void);

    bool Open(void);
    void Close(void);
    bool IsOpen(void) const;

	bool SetBufferCount(int iCount);
	StInt32 GetBufferCount(void);
	bool SetBufferSize(int iSize);
	StInt32 GetBufferSize(void);

	
    StResult StartStream(void);
    StResult StopStream(void);
	//StResult RetrieveBuffer( StBuffer ** aBuffer, uint32_t aTimeout, StResult * aOperationResult );
	StResult RetrieveBuffer( StBuffer ** aBuffer, StUInt32 aTimeout = 0, StResult * aOperationResult = NULL );
	StResult QueueBuffer( StBuffer * aBuffer );
	//▼2017/10/17
	StResult GetOutputQueueCount(PINT aCount);
	//▲2017/10/17

    //const StString& GetID(void) const {return m_strID;}
    //SimpleDevice* GetDevice(void) {return m_psDev;}

    //size_t GetPayloadSize(void);
    //uint64_t GetNumDelivered(void);
    //uint64_t GetNumQueueUnderrun(void);
    //uint32_t GetNumAnnounced(void);
    //uint32_t GetNumQueued(void);
    //uint32_t GetNumAwaitDelivery(void);
    //bool IsGrabStarted(void);
    //bool IsGrabbing(void) {return (NULL != m_hAcqThread);}


 //   void QueryInformations(void);
	const HANDLE GetHandle(void);
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

	//const StString &StGenStream::GetPortName(void);
	//const StString &StGenStream::GetID(void);
	//▲2017/10/20



	//StBuffer *GetGenBuffer( int index )
	//{
	//	if( !mpBuffers ) return NULL;
	//	if( index>=mNumBuffers ) return NULL;
	//	return mpBuffers+index;
	//}
    //const StString& GetPortName() const {return m_strPortName;}

	//▼2014/07/23 最後のデータポインタ保存----------------------------------
	//const void *GetLastDataPointer(void){return mLastDataPointer;}
	//StUInt64 GetLastFrameID(void){return mi64LastFrameID;}
	//▲2014/07/23 最後のデータポインタ保存----------------------------------

protected:
	void *mhData;
 //   StGenGCLib* m_psGCLib;
	//StGenDevice * m_psDev;
	//DS_HANDLE m_hDS;
	//EVENT_HANDLE m_hEvent;
 //   StString m_strID;
	//StString m_strPortName;
	////StBuffer
	//int mNumBuffers;
	//StBuffer *mpBuffers;

	////▼2014/07/23 最後のデータポインタ保存----------------------------------
	////BufferView表示用にDataPointerを保存しておく
	//const void *mLastDataPointer;
	//StUInt64 mi64LastFrameID;
	//▲2014/07/23 最後のデータポインタ保存----------------------------------

	//StResult RetrieveBuffer( StBuffer ** aBuffer, StUInt32 aTimeout, StResult * aOperationResult );

    //std::vector<ImageBuffer* > m_vBuffers;

};

