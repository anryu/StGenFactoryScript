#pragma once
#include <PvDevice.h>
#include <PvStream.h>
#include <PvPipeline.h>
#include <PvAcquisitionStateManager.h>
#include "CallbackThread.h"
#include "../CommonHeader/StBuffer.h"
#include "StDeviceBase.h"

#include <iostream>

//class ListenerBase{
//public:
//    virtual void Invoke(StBuffer* pBuffer, void *pUser) = 0;    
//};

//template <class T>
//class MemFuncListener : public ListenerBase
//{
//public:
//    MemFuncListener(T* obj, void (T::*cbf)(StBuffer* , void *)){
//        m_obj = obj;
//        m_cbf = cbf;
//    };
//    
//	virtual void Invoke(StBuffer* pBuffer, void *pUser)
//	{
//        (m_obj->*m_cbf)(pBuffer,pUser);
//    };
//private:
//    T* m_obj;
//    void (T::*m_cbf)(StBuffer* , void *);
//};

class StDeviceEBus :
	public StDeviceBase, public PvDevice	//StDeviceBaseを先に記述
{
public:
	StDeviceEBus(void);
	StDeviceEBus(LPCTSTR szClassName, LPCTSTR szVarName);
	~StDeviceEBus(void);
	BOOL ConnectCustom( PvDeviceInfo *aDI );
	BOOL DisconnectCustom(void);
	void GetIPAddressStr( PvString &aString );
	void GetMACAddressStr( PvString &aString );
	//▼1.0.0.1017
	void GetMACAddressCapitalStr( PvString &aString );
	//▲1.0.0.1017
	void GetSubnetMaskStr( PvString &aString );
	void StartAcquisition(void);
	void StopAcquisition(void);
	bool IsStarted(void);
	BOOL SerialControl( LPCTSTR szPort, LPCTSTR szData, PBYTE pbyteReadData=NULL, size_t *pReadSize=NULL );
	BOOL SerialControl( LPCTSTR szPort, PBYTE pbyteSendData, size_t sendSize, PBYTE pbyteReadData, size_t *pReadSize );

	//▼1.0.0.1060
	BOOL function( LPCTSTR szFunc, LPCTSTR szArgument );
	//▲1.0.0.1060

	//--------------------------------------------------------------------
	//SetGenICamControl
	//--------------------------------------------------------------------
	BOOL SetGenICamControl( LPCTSTR szFeature, LPCTSTR szData );
	BOOL SetGenICamControl( LPCTSTR szFeature, INT nData );
	BOOL SetGenICamControl( LPCTSTR szFeature, double dblData );
	BOOL SetGenICamControl( LPCTSTR szFeature, bool bData );

	//--------------------------------------------------------------------
	//GetGenICamControl
	//--------------------------------------------------------------------
	BOOL GetGenICamControl( LPCTSTR szFeature, LPTSTR szData, size_t size );
	BOOL GetGenICamControl( LPCTSTR szFeature, INT &nData );
	BOOL GetGenICamControl( LPCTSTR szFeature, double &dblData );
	BOOL GetGenICamControl( LPCTSTR szFeature, bool &bData );

	//--------------------------------------------------------------------
	//GetFunction
	//--------------------------------------------------------------------
	BOOL GetFunction( LPCTSTR szFunc, LPCTSTR szArgument, LPTSTR szData, size_t size );
	BOOL GetFunction( LPCTSTR szFunc, LPCTSTR szArgument, INT &nData );
	BOOL GetFunction( LPCTSTR szFunc, LPCTSTR szArgument, double &dblData );


//	template <class T>
//	void RegisterCallback(T *obj, void (T::*cbf)(StBuffer*, void *), void *pUser )
//	{
//		UnregisterCallback();
//		m_Listener = new MemFuncListener<T>(obj, cbf);
//		m_pUser = pUser;
//	}
//
//	void UnregisterCallback()
//	{
//		if( m_Listener )
//		{
//			delete m_Listener;
//			m_Listener = NULL;
//		}
//		m_pUser = NULL;
//	}
//
//private:
//	ListenerBase* m_Listener;
//	void *m_pUser;

public:
	void OnImageCallback(PvBuffer *aBuffer);
	PvPipeline * GetPipeline(void){return &mPipeline;}

private:	
	PvStream mStream;
	PvPipeline mPipeline;

	PvString mIPAddressStr;
	PvString mMACAddressStr;
	//▼1.0.0.1017
	PvString mMACAddressCapitalStr;
	//▲1.0.0.1017
	PvString mSubnetMaskStr;

	void StartStreaming();
	void StopStreaming();

	//▼1.0.0.1030
	PvStream * GetStream(void){return &mStream;}
	//▲1.0.0.1030

	//排他処理必要か？時間がないのでとりあえずしていない。
	//StBuffer m_ConvBuffer;	//表示処理をOnTimer処理にした場合は、m_ConvBufferを使用して常に残しておく必要あり。。。。

	CallbackThread *mCallbackThread;

	PvUInt64 GetPayloadSize();
    PvAcquisitionStateManager *mAcquisitionStateManager;

	BOOL ConvertToStBufferFromPvBuffer( StBuffer *aOutBuffer, PvBuffer *aInBuffer );

	//▼1.0.0.1060
	INT m_nSerialControlDelayTime;
	//▲1.0.0.1060

};
