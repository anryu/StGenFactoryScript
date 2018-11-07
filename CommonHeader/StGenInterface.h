#pragma once

#include "StGenICamLib.h"

//#include "StGenGCLib.h"
//#include "StGenTL.h"
#include "StString.h"
#include "StType.h"

class StGenDeviceInfo;

class ST_GENICAMLIB_API StGenInterface
{
public:
	StGenInterface(void);
	virtual ~StGenInterface(void);

    const StString GetID() const;
    const StString GetDisplayName() const;
    const StString GetPortName() const;

	StInt32 GetNumDeviceInfo( void );
	StGenDeviceInfo * GetGenDeviceInfo(StInt32 index);

	//あまりよくない作り---------------CXPの電源制御の為、OpenCloseを作成
	//DeviceOpen中は使用できない。。。
	//StResult IFOpen(StGenDeviceInfo *pDeviceInfo);
	//StResult IFClose(void);

	BOOL GetGenEnumParameter( StString lFeature, int & iData, bool Verify=false, bool IgnoreCache=false );
	BOOL GetGenEnumParameter( StString lFeature, StString & strData, bool Verify=false, bool IgnoreCache=false );
	BOOL SetGenEnumParameter( StString lFeature, int iData );
	BOOL SetGenEnumParameter( StString lFeature, StString strData );
	BOOL GetGenFloatParameter( StString lFeature, double & dblData, bool Verify=false, bool IgnoreCache=false );
	BOOL SetGenFloatParameter( StString lFeature, double dblData );
	BOOL ExecuteCommand(StString aFeature);

	//▼1.0.0.1051
	BOOL GetGenIntParameter( StString lFeature, int & iData, bool Verify=false, bool IgnoreCache=false );
	BOOL GetGenIntMinMaxParameter( StString lFeature, int & iMin, int & iMax );
	BOOL SetGenIntParameter( StString lFeature, int iData );
	BOOL GetGenBoolParameter( StString lFeature, bool & bData, bool Verify=false, bool IgnoreCache=false );
	BOOL SetGenBoolParameter( StString lFeature, bool bData );
	BOOL GetGenStringParameter( StString lFeature, StString & strData, bool Verify=false, bool IgnoreCache=false );
	BOOL SetGenStringParameter( StString lFeature, StString & strData );
	BOOL SetGenCommandParameter( StString lFeature );
	//▲1.0.0.1051


	bool Read( StInt64 iAddress, char *pBuffer, size_t size );
	bool Write( StInt64 iAddress, char *pBuffer, size_t size );


protected:
	void *mhData;

};

