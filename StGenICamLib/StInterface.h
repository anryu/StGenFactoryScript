#pragma once

#include "StGenGCLib.h"
#include "StTL.h"

#include "StGenInterface.h"

//class StGenDevice;

class StInterface : public StGenInterface
{
public:
	//StInterface(StGenGCLib* psGCLib,StGenTL* psTL,StString strID);
	StInterface(StTL* psTL,StString strID);
	~StInterface(void);

	StResult IFOpen(void);
	StResult IFClose(void);
    bool IsOpen(void) const {return (NULL != m_hIF);}

 //   const StString& GetID() const {
	//	return m_strID;
	//}
 //   const StString& GetDisplayName() const {
	//	return m_strDisplayName;
	//}
 //   const StString& GetPortName() const {return m_strPortName;}

	void QueryInformations( void );
	StResult UpdateDeviceList( bool *pbChanged );
	void DeleteDeviceList( void );
	StString GetDeviceID( uint32_t index );
	const IF_HANDLE GetHandle(void){return m_hIF;}
	uint32_t GetNumDevices( void );
	//StGenDevice * GetGenDevice(uint32_t index)
	//{
	//	if( m_vDevList.size()>index) return m_vDevList[index];
	//	else return NULL;
	//}
	//void clearGenDeviceList(void)
	//{
	//	m_vDevList.clear();
	//}

	StGenGCLib* GetGenGCLib(void){return m_psGCLib;}
	StTL* GetTL(void){return m_psTL;}

	//StResult InvalidateDeviceList( bool * pbChanged );


private:
    StGenGCLib* m_psGCLib;
    StTL* m_psTL;
	IF_HANDLE m_hIF;
    //StString m_strID;

    //StString m_strDisplayName;
    //StString m_strPortName;

	//std::vector< StGenDevice * > m_vDevList;

	CNodeMapRef *m_pNodeMap;
	void *m_pGenTLPort;


};

