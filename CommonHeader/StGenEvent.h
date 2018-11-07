#pragma once

#include "StGenGCLib.h"
#include "StGenInterface.h"
#include "StGenDevice.h"


//class StGenEvent

typedef struct {
	void (*nFuncCallback)(void *pVoid, void *pEvent);
	void *pClassPointer;
	void *pUserPointer;
}CallbackInfo;


	static unsigned WINAPI Link( void *aParam );



class StGenEvent
{
public:
	//StGenEvent(void);
	//StGenEvent(StGenGCLib* psGCLib,StGenDevice * psDev,StString strID);
	//StGenEvent(StGenGCLib* psGCLib,StGenDevice * psDev,StString strID, EVENT_TYPE_LIST aType);
	//StGenEvent(StGenGCLib* psGCLib,StGenDevice * psDev,StString strID, EVENT_TYPE_LIST aType, void (*nFuncCallback)(void *pVoid, char *pBuffer, unsigned int aSize), void *pUserPointer);
	StGenEvent(StGenGCLib* psGCLib,StGenDevice * psDev,StString strID, EVENT_TYPE_LIST aType, void (*nFuncCallback)(void *pVoid, void *pEvent), void *pUserPointer);
	~StGenEvent(void);

	bool EventFlush(void);

	void EventGetData(void);

	//bool EventGetDataInfo(void);
	bool EventGetDataInfo(EVENT_DATA_INFO_CMD iInfoCmd, void *pBuffer, size_t *piBufferSize);
	void EventGetInfo(void);
	void EventKill(void);
	bool RegisterEvent(void);
	void UnregisterEvent(void);

	//-----Adapter--------------------------
	bool SetEventAdapterRegisterMain(void);
	bool SetEventAdapterRegister(GenApi::NodeList_t & aNodeList, int deps=0 );


    const StString& GetID(void) const {return m_strID;}

	bool StartEvent(void);
	void StopEvent(void);
	//void SetEventCallback(void (*nFuncCallback)(void *pVoid, char *pBuffer, unsigned int aSize), void *pUserPointer);
	//void SetEventCallback(void (*nFuncCallback)(void *pVoid, void *pEvent), void *pUserPointer);
	void SetEventCallback(void (*nFuncCallback)(void *pVoid, void *pEvent), void *pClassPointer, void *pUserPointer);
	//void SetEventCallbackDevCon(void (*nFuncCallback)(void *pVoid, void *pEvent), void *pUserPointer);
	StGenDevice * GetGenDevice(){return m_psDev;}

	DWORD TestExecute();

	//-----------Adapter------------------
	void SetEventAdapterCallback(void (*nFuncCallback)(void *pVoid, void *pEvent), void *pClassPointer, void *pUserPointer);
	void AdapterCallback(INode* pNode);



	int GetTimeout(){return m_aTimeout;}


	bool IsStopping() const;

	virtual DWORD Function();


protected:
	static unsigned WINAPI Link( void *aParam );
	//static unsigned long WINAPI Link( void *aParam );
	//static unsigned long Link( void *aParam );


//	virtual DWORD Function();


	//bool IsStopping() const;
	bool IsDone();
	DWORD GetReturnValue();

private:
    StGenGCLib* m_psGCLib;
	StGenDevice * m_psDev;
	EVENT_HANDLE m_hEvent;
    StString m_strID;
	//GenApi::CNodeMapRef* m_pEventMap;

	EVENT_TYPE_LIST m_eventType;


	//void FetchEventNodeMap(void);
 //   void ReleaseEventNodeMap(void);


	HANDLE mHandle;
	//DWORD mID;
	unsigned int mID;
	bool mStop;
	DWORD mReturnValue;

	//Callback“o˜^
	//void (*m_nFuncCallback)(void *pVoid, char *pBuffer, unsigned int aSize);
	//void (*m_nFuncCallback)(void *pVoid, void *pEvent);
	//void *m_pUserPointer;
	//DevCon Callback“o˜^
	//void (*m_nFuncCallbackDevCon)(void *pVoid, void *pEvent);
	//void *m_pUserPointerDevCon;

	std::vector<CallbackInfo *> m_vCallbackInfo;
	//-----------Adapter------------------
	std::vector<CallbackInfo *> m_vAdapterCallbackInfo;


	int m_aTimeout;
	size_t m_aBufferSize;
	char *m_pBuffer;

};

