#pragma once

#include "StGenGCLib.h"
#include "StTL.h"
#include "StInterface.h"

#include "StGenDeviceInfo.h"

//class StGenDevice;

class StDeviceInfo : public StGenDeviceInfo
{
public:
	//StInterface(StGenGCLib* psGCLib,StGenTL* psTL,StString strID);
	StDeviceInfo(StInterface* psIF,StString strID);
	~StDeviceInfo(void);

	//StResult IFOpen(void);
	//StResult IFClose(void);
    //bool IsOpen(void) const {return (NULL != m_hDEV);}

	//void QueryInformations( void );
	//StResult UpdateDeviceList( bool *pbChanged );
	//void DeleteDeviceList( void );
	//StString GetDeviceID( uint32_t index );
	StInterface * GetInterface(void){return m_psIF;}
	//const DEV_HANDLE GetHandle(void){return m_hDEV;}
	//uint32_t GetNumDevices( void );

//	StGenGCLib* GetGenGCLib(void){return m_psGCLib;}


	//StTL* GetTL(void){return m_psTL;}
	//StResult InvalidateDeviceList( bool * pbChanged );

	bool IFQueryInformations( void );


private:
    StGenGCLib* m_psGCLib;
    StInterface* m_psIF;
	//DEV_HANDLE m_hDEV;

};

