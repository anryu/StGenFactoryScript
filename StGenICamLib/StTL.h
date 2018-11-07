#pragma once

//#include <Windows.h>
#include "StGenDeviceManager.h"
#include "StGenGCLib.h"
#include "StGenTL.h"
#include "StType.h"
#include "StString.h"
#include "StResult.h"

class StTL : public StGenTL
{
public:
	//StGenTL(void);
	StTL(HANDLE hLib);
	~StTL(void);
	
	StResult TLOpen(void);
	StResult TLClose(void);
    bool IsOpen(void) const {return (NULL != m_hTL);}

	StResult UpdateAllList( bool * pbChanged );
	void QueryInformations( void );
	StResult UpdateInterfaceList( bool * pbChanged );
	void DeleteInterfaceList( void );
	StString GetInterfaceID( uint32_t index );
	const TL_HANDLE GetHandle(void){return m_hTL;}

	StGenGCLib* GetGenGCLib(void){return m_psGCLib;}
	//const StString& GetID(void) const {return m_strID;}

	//StResult InvalidateDeviceList( StGenInterface *pIF, bool * pbChanged );
	StResult InvalidateInterfaceList( bool * pbChanged );



private:
    StGenGCLib* m_psGCLib;
	TL_HANDLE m_hTL;

	//std::vector< StGenInterface * > m_vIFList;

    //StString m_strID;

    //StString m_strVendor;
    //StString m_strModel;
    //StString m_strVersion;
    //StString m_strFileName;
    //StString m_strPathName;
    //StString m_strDisplayName;
    //StString m_strPortName;

	StResult TLGetInfo(TL_INFO_CMD iInfoCmd,INFO_DATATYPE * piType,void * pBuffer,size_t * piSize);
	StResult TLGetInterfaceID(uint32_t iIndex, char * sIfaceID, size_t * piSize);
	StResult TLGetInterfaceInfo(const char * sIfaceID, INTERFACE_INFO_CMD iInfoCmd, INFO_DATATYPE * piType, void * pBuffer, size_t * piSize);
	StResult TLGetNumInterfaces(uint32_t * piNumIfaces);
	StResult TLOpenInterface(const char * sIfaceID,IF_HANDLE * phIface);
	StResult TLUpdateInterfaceList(bool8_t * pbChanged,uint64_t iTimeout);




};



