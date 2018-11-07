#pragma once

//using namespace GenApi;

#include "StGenICamLib.h"

#include "StString.h"
#include "StResult.h"
#include "StType.h"

#include "StGenDeviceInfo.h"

//#include "StGenInterface.h"
//#include "StatisticsView.h"

class StGenStream;
//class StGenPort;
//class StGenEvent;
//class CaptureThread;

class ST_GENICAMLIB_API StGenDevice
{
public:

	StGenDevice(void);
	virtual ~StGenDevice(void);

	StResult DevOpen(StGenDeviceInfo *aDevInfo);
	StResult DevClose(void);
    bool IsOpen(void);
	//{
	//	return (NULL != m_hDEV);
	//}

    const StString GetVendor() const;
	//{return m_strVendor;}
    const StString GetModel() const;
	//{return m_strModel;}
    const StString GetDisplayName() const;
	//const {return m_strDisplayName;}
    const StString GetPortName() const;
	//const {return m_strPortName;}
    const StString GetXMLFileName() const;


	bool TLParamsLocked( bool lock );
	bool GetGenIntParameter( StString lFeature, int & iData, bool Verify=false, bool IgnoreCache=false );
	bool GetGenIntMinMaxParameter( StString lFeature, int & iMin, int & iMax );
	bool SetGenIntParameter( StString lFeature, int iData );
	bool GetGenEnumParameter( StString lFeature, int & iData, bool Verify=false, bool IgnoreCache=false );
	bool GetGenEnumParameter( StString lFeature, StString & strData, bool Verify=false, bool IgnoreCache=false );
	//▼1.0.0.28 beta1
	bool GetGenEnumIndexParameter( StString lFeature, int nIndex, StString & strData, bool Verify=false, bool IgnoreCache=false );
	//▲1.0.0.28 beta1
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
	bool Read( DWORD dwAddress, char *pBuffer, size_t size );
	bool Write( DWORD dwAddress, char *pBuffer, size_t size );

	StResult CreateStream(void);
	void DeleteStream(void);
	StInt32 GetNumStream( void );
	StGenStream * GetGenStream(StInt32 index);
protected:


protected:
	void *mhData;





	/*
	StString GetStreamID( uint32_t index );

	GenApi::CNodeMapRef* GetRemoteNodeMap(void) {return m_pDeviceMap;}

	const DEV_HANDLE GetHandle(void){return m_hDEV;}
	uint32_t GetNumStreams( void );
	StGenStream * GetGenStream(uint32_t index){
		if( m_vDSList.size()>index) return m_vDSList[index];
		else return NULL;
	}
	void clearGenStreamList(void){	m_vDSList.clear();	}

	//Acquisition関連
    StResult StartAcquisition(void);
    StResult StopAcquisition(void);
    bool IsStarted(void) const {return m_bStart;}

	//CDocument関連付け
	void SetDocument(CDocument * lDoc){m_lDoc=lDoc;}
	CDocument * GetDocument(void){return m_lDoc;}
	//void SetFrameWnd(CFrameWnd * lFrameWnd){m_lFrameWnd=lFrameWnd;}
	//CFrameWnd * GetFrameWnd(void){return m_lFrameWnd;}

	
	//CaptureThread関連付け
	void SetCaptureThread(CaptureThread * lThreadDisplay){m_lThreadDisplay=lThreadDisplay;}
	CaptureThread * GetCaptureThread(void){return m_lThreadDisplay;}


	BOOL CreateThread(void);
	void DeleteThread(void);



	//Stream関連
	StResult CreateStream(void);
	StResult DeleteStream(void);
	StResult StartStream(void);
	StResult StopStream(void);

	//GenParameter関連
	bool GetGenIntParameter( StString lFeature, int & iData );
	bool GetGenIntMinMaxParameter( StString lFeature, int & iMin, int & iMax );
	bool SetGenIntParameter( StString lFeature, int iData );
	bool GetGenEnumParameter( StString lFeature, int & iData );
	bool GetGenEnumParameter( StString lFeature, StString & strData );
	bool SetGenEnumParameter( StString lFeature, int iData );
	bool SetGenEnumParameter( StString lFeature, StString strData );
	bool GetGenFloatParameter( StString lFeature, double & dblData );
	bool GetGenFloatMinMaxParameter( StString lFeature, double & dblMin, double & dblMax );
	bool SetGenFloatParameter( StString lFeature, double dblData );

	bool SetGenCommandParameter( StString lFeature );

	bool GetPayloadSize( int & iData );

	//
	//bool GetNodeMap(PORT_HANDLE hPort, CNodeMapRef& rNodeMap)

	gcstring RetrieveXML(PORT_HANDLE hPort, gcstring &strXML);
	bool GetNodeMap(PORT_HANDLE hPort, CNodeMapRef& rNodeMap);
	CNodeMapRef* GetNodeMap( PORT_HANDLE hPort );
	StGenPort* GetGenPort(void){return m_psPort;}
	*/
	//bool TLParamsLocked( bool lock /*= true*/ );
	/*
	StGenGCLib* GetGenGCLib(void){return m_psGCLib;}

	//Callback
	void (*pmfnP)(GenApi::INode* pNode, void *pValue);
	void *m_pValue;
	void SetCallbackFunction(void (*pfnP)(GenApi::INode* pNode, void *pValue), void *pValue)
	{
		pmfnP = pfnP;
		m_pValue = pValue;
	}
    virtual void Callback( GenApi::INode* pNode )
    {
		WaitForSingleObject(this->m_hMutex,INFINITE);
		if( IsOpen() )
		{
			if( pmfnP )
				pmfnP(pNode, m_pValue);
		}
		ReleaseMutex(this->m_hMutex);
    }


	GenApi::CNodePtr GetSortNodePtr(NodeList_t& aNodes, unsigned int index, CNodeMapRef& rNodeMap);
	GenApi::CNodePtr GetSortNodePtr(NodeList_t& aNodes, unsigned int index);



	//EVENT

	void SetEventCallback(void (*nFuncCallback)(void *pVoid, void *pEvent), void *pClassPointer, void *pUserPointer);


	//-----Adapter-------------------------------------
	//CEventAdapterGeneric *m_pEventAdapter;


	void SetEventAdapterCallback(void (*nFuncCallback)(void *pVoid, void *pEvent), void *pClassPointer, void *pUserPointer);
	//▼1.0.0.28　EventTestTimestamp等のEvent項目をDeviceViewに表示
	//▼Callbackリスト追加-------------------------------------
	BOOL m_bCallbackEnable;
	std::vector< intptr_t > m_vRegisterHandleList;
	void pushRegisterHandleList(intptr_t a){m_vRegisterHandleList.push_back(a);}
	int GetRegisterHandleListCount(void){ return m_vRegisterHandleList.size();}
	intptr_t GetRegisterHandle(int index)
	{
		if( (unsigned int)index<m_vRegisterHandleList.size() )
			return m_vRegisterHandleList[index];
		return NULL;
	}
	void AllDeregister(void)
	{
		m_bCallbackEnable = FALSE;
		for( unsigned int i=0; i<m_vRegisterHandleList.size(); i++ )
		{
			Deregister(m_vRegisterHandleList[i]);
		}
		m_vRegisterHandleList.clear();
	}
	//▲1.0.0.28　EventTestTimestamp等のEvent項目をDeviceViewに表示

	//--Polling--------------------------------------
	//std::vector< StPollingInfo * > m_vPollingInfoList;

	StGenInterface *GetGenInterface(void){return m_psIF;}
	StGenEvent *GetGenEvent(void){return m_pGenEvent;}
	//▼1.0.0.28　EventTestTimestamp等のEvent項目をDeviceViewに表示
	//GenApi::CNodeMapRef* GetEventMap(void){return m_pEventMap;}
	//▲1.0.0.28　EventTestTimestamp等のEvent項目をDeviceViewに表示

	const StString& GetID(void) const {return m_strID;}

	//Status関連
	//CStatisticsViewInfo *m_pStatisticsViewInfo;
	//void SetStatisticsViewInfo(CStatisticsViewInfo * aInfo){m_pStatisticsViewInfo = aInfo;}
	//CStatisticsViewInfo *GetStatisticsViewInfo(void){return m_pStatisticsViewInfo;}


private:
    StGenGCLib* m_psGCLib;
    //StGenTL* m_psTL;
	StGenInterface * m_psIF;
	DEV_HANDLE m_hDEV;
    StString m_strID;
	GenApi::CNodeMapRef* m_pDeviceMap;
	StGenPort *m_psPort;

	void IFQueryInformations( void );
	void QueryInformations( void );
    void FetchNodeMap(void);
    void ReleaseNodeMap(void);

	HANDLE m_hMutex;

    StString m_strVendor;
    StString m_strModel;
    StString m_strDisplayName;
    StString m_strPortName;

	bool m_bStart;

	//CDocument関連付け
	CDocument *m_lDoc;
	//CFrameWnd *m_lFrameWnd;
	//CaptureThread関連付け
	CaptureThread *m_lThreadDisplay;

	std::vector< StGenStream * > m_vDSList;

	//Event
	StGenEvent *m_pGenEvent;
	StResult CreateEvent(void);
	StResult DeleteEvent(void);
	StResult StartEvent(void);
	StResult StopEvent(void);
	//▼1.0.0.28　EventTestTimestamp等のEvent項目をDeviceViewに表示
	//GenApi::CNodeMapRef* m_pEventMap;
	//▲1.0.0.28　EventTestTimestamp等のEvent項目をDeviceViewに表示
	*/
};

