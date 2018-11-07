#pragma once

#include "StGenGCLib.h"
#include "StTL.h"

#include "StGenStream.h"

//class StGenDevice;

class StStream : public StGenStream
{
public:
	//StInterface(StGenGCLib* psGCLib,StGenTL* psTL,StString strID);
	StStream( StGenGCLib* psGCLib, DEV_HANDLE hDevice, StString strID );
	//StStream( StGenGCLib* psGCLib, StGenDevice * psDevice, DEV_HANDLE hDevice, StString strID );
	~StStream(void);
//
//	StResult IFOpen(void);
//	StResult IFClose(void);
//    bool IsOpen(void) const {return (NULL != m_hIF);}
//
//
	bool QueryInformations( void );
//	StResult UpdateDeviceList( bool *pbChanged );
//	void DeleteDeviceList( void );
//	StString GetDeviceID( uint32_t index );
//	const IF_HANDLE GetHandle(void){return m_hIF;}
//	uint32_t GetNumDevices( void );
//
//	StGenGCLib* GetGenGCLib(void){return m_psGCLib;}
//	StTL* GetTL(void){return m_psTL;}
//
//	StResult InvalidateDeviceList( bool * pbChanged );
//
//private:
//    StGenGCLib* m_psGCLib;
//    StTL* m_psTL;
//	IF_HANDLE m_hIF;
};

