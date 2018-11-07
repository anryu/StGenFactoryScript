#pragma once
#include "stcommonbase.h"
#include "StDeviceBase.h"
#include "../CommonHeader/StGenDeviceManager.h"
#include "../CommonHeader/StGenInterface.h"

class StInterfaceGenICam :
	public StCommonBase
{
public:
	StInterfaceGenICam(void);
	//Å•1.0.0.1054
	StInterfaceGenICam(LPCTSTR szClassName, LPCTSTR szVarName, void * aDevice);
	//Å£1.0.0.1054
	StInterfaceGenICam(LPCTSTR szClassName, LPCTSTR szVarName, LPCTSTR aInterfaceID);
	~StInterfaceGenICam(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);

	BOOL Find( LPCTSTR aInterfaceID );
	BOOL IsFind(void){return m_Interface!=NULL;}

private:
	StGenDeviceManager *mpDeviceManager;
	BOOL Initialize(void);
	BOOL SetGenControl( LPCTSTR szFeature, LPCTSTR szData );

	StString m_InterfaceID;
	StGenInterface *m_Interface;
	//Å•1.0.0.1054
	void *m_Device;
	BOOL SetInterface( void );
	//Å£1.0.0.1054
	//--------------------------------------------------------------------
	//SetGenICamControl
	//--------------------------------------------------------------------
	BOOL SetGenICamControl( LPCTSTR szFeature, LPCTSTR szData );
	BOOL SetGenICamControl( LPCTSTR szFeature, INT nData );
	BOOL SetGenICamControl( LPCTSTR szFeature, double dblData );
	BOOL SetGenICamControl( LPCTSTR szFeature, bool bData );
	BOOL SetGenICamControl( LPCTSTR szFeature );
//Å•1.0.0.1052
public:
//Å£1.0.0.1052
	//--------------------------------------------------------------------
	//GetGenICamControl
	//--------------------------------------------------------------------
	BOOL GetGenICamControl( LPCTSTR szFeature, LPTSTR szData, size_t size );
	BOOL GetGenICamControl( LPCTSTR szFeature, INT &nData );
	BOOL GetGenICamControl( LPCTSTR szFeature, double &dblData );
	BOOL GetGenICamControl( LPCTSTR szFeature, bool &bData );

};

