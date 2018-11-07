#pragma once

#include "StGenICamLib.h"

#include "StString.h"
#include "StType.h"
//#include "StResult.h"

class StGenInterface;

class ST_GENICAMLIB_API StGenTL
{
public:
	//StGenTL(StGenDeviceManager* psDeviceManager);
	StGenTL(void);
	virtual ~StGenTL(void);

	const StString& GetVendor() const;
    const StString& GetModel() const;
    const StString& GetVersion() const;
    const StString& GetName() const;
    const StString& GetPathName() const;
    const StString& GetDisplayName() const;
    const StString& GetPortName() const;
	const StString& GetID(void) const;


	StInt32 GetNumInterfaces( void );
	//StString GetInterfaceID( uint32_t index );
	StGenInterface * GetGenInterface(StInt32 index);

protected:
	void *mhThis;

};

/*
class StGenInterface;

class StGenTL
{
public:
	//StGenTL(void);
	StGenTL(StGenGCLib* psGCLib);
	~StGenTL(void);
	
	StResult TLOpen(void);
	StResult TLClose(void);
    bool IsOpen(void) const {return (NULL != m_hTL);}

	const StString& GetVendor() const {return m_strVendor;}
    const StString& GetModel() const {return m_strModel;}
    const StString& GetVersion() const {return m_strVersion;}
    const StString& GetName() const {return m_strFileName;}
    const StString& GetPathName() const {return m_strPathName;}
    const StString& GetDisplayName() const {return m_strDisplayName;}
    const StString& GetPortName() const {return m_strPortName;}


	StResult UpdateAllList( bool * pbChanged );
	void QueryInformations( void );
	StResult UpdateInterfaceList( bool * pbChanged );
	void DeleteInterfaceList( void );
	StString GetInterfaceID( uint32_t index );
	const TL_HANDLE GetHandle(void){return m_hTL;}
	uint32_t GetNumInterfaces( void );
	StGenInterface * GetGenInterface(uint32_t index){
		if( m_vIFList.size()>index) return m_vIFList[index];
		else return NULL;
	}
	StGenGCLib* GetGenGCLib(void){return m_psGCLib;}
	const StString& GetID(void) const {return m_strID;}

	//StResult InvalidateDeviceList( StGenInterface *pIF, bool * pbChanged );
	StResult InvalidateInterfaceList( bool * pbChanged );



private:
    StGenGCLib* m_psGCLib;
	TL_HANDLE m_hTL;

	std::vector< StGenInterface * > m_vIFList;

    StString m_strID;

    StString m_strVendor;
    StString m_strModel;
    StString m_strVersion;
    StString m_strFileName;
    StString m_strPathName;
    StString m_strDisplayName;
    StString m_strPortName;

	StResult TLGetInfo(TL_INFO_CMD iInfoCmd,INFO_DATATYPE * piType,void * pBuffer,size_t * piSize);
	StResult TLGetInterfaceID(uint32_t iIndex, char * sIfaceID, size_t * piSize);
	StResult TLGetInterfaceInfo(const char * sIfaceID, INTERFACE_INFO_CMD iInfoCmd, INFO_DATATYPE * piType, void * pBuffer, size_t * piSize);
	StResult TLGetNumInterfaces(uint32_t * piNumIfaces);
	StResult TLOpenInterface(const char * sIfaceID,IF_HANDLE * phIface);
	StResult TLUpdateInterfaceList(bool8_t * pbChanged,uint64_t iTimeout);




};
*/
