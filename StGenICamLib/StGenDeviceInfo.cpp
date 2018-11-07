#include "StdAfx.h"
#include "StInterface.h"
#include "StDeviceInfo.h"



//------------------------------------------------------------------------
//--StGenDeviceInfoValue----------------------------------------------------------------------
//------------------------------------------------------------------------
class StGenDeviceInfoValue
{
public:
	StGenDeviceInfoValue(void);
	virtual ~StGenDeviceInfoValue(void);

	const StString& GetID(void) const{return m_strID;}
    const StString& GetVendor() const{return m_strVendor;}
    const StString& GetModel() const{return m_strModel;}
    const StString& GetDisplayName() const{return m_strDisplayName;}
	
	//Å•1.0.0.1051
	StGenInterface *GetInterface(void){return m_psIF;}
	StGenInterface *m_psIF;
	//Å£1.0.0.1051

//private:
    StString m_strID;
    StString m_strVendor;
    StString m_strModel;
    StString m_strDisplayName;
};

StGenDeviceInfoValue::StGenDeviceInfoValue()
    : m_strID( "" )
    , m_strVendor( "" )
    , m_strModel( "" )
    , m_strDisplayName( "" )
	//Å•1.0.0.1051
	, m_psIF(NULL)
	//Å£1.0.0.1051
{
}

StGenDeviceInfoValue::~StGenDeviceInfoValue()
{
}

//StInt32 StGenDeviceInfoValue::GetNumDeviceInfo( void )
//{
//	//return (StInt32)m_vIFList.size();
//	return 0;
//}

//------------------------------------------------------------------------
//--StDeviceInfo----------------------------------------------------------------------
//------------------------------------------------------------------------
StDeviceInfo::StDeviceInfo(StInterface* psIF,StString strID)
	: m_psIF(psIF)
{
	StTL *psTL = psIF->GetTL();
	m_psGCLib = psTL->GetGenGCLib();

	StGenDeviceInfoValue *pDevInfo = new StGenDeviceInfoValue;
	mhData = (void *)pDevInfo;

	if( pDevInfo )
	{
		pDevInfo->m_strID = strID;
		//Å•1.0.0.1051
		pDevInfo->m_psIF = m_psIF;
		//Å£1.0.0.1051
	}
}


StDeviceInfo::~StDeviceInfo(void)
{
	if( mhData )
	{
		StGenDeviceInfoValue * pValue = (StGenDeviceInfoValue *)mhData;
		delete pValue;
	}
}

bool StDeviceInfo::IFQueryInformations( void )
{
	bool bReval = false;
	if( !m_psIF ) return bReval;
	IF_HANDLE hIF = m_psIF->GetHandle();
	if( !hIF ) return bReval;
	if( !mhData ) return bReval;
	StGenDeviceInfoValue * pValue = (StGenDeviceInfoValue *)mhData;

	if (m_psGCLib->f_IFGetDeviceInfo)
    {
        GC_ERROR status;
        size_t iSize = 0;
        INFO_DATATYPE iType = INFO_DATATYPE_UNKNOWN;

		status = m_psGCLib->f_IFGetDeviceInfo(hIF, pValue->GetID().GetAscii(), DEVICE_INFO_VENDOR, &iType, NULL, &iSize);
        if ((0 == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            status = m_psGCLib->f_IFGetDeviceInfo(hIF, pValue->GetID().GetAscii(), DEVICE_INFO_VENDOR, &iType, str, &iSize);
            pValue->m_strVendor = str;
            delete[] (str);
        }

		iType = INFO_DATATYPE_UNKNOWN;
		iSize = 0;
		status = m_psGCLib->f_IFGetDeviceInfo(hIF, pValue->GetID().GetAscii(), DEVICE_INFO_MODEL, &iType, NULL, &iSize);
        if ((0 == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            status = m_psGCLib->f_IFGetDeviceInfo(hIF, pValue->GetID().GetAscii(), DEVICE_INFO_MODEL, &iType, str, &iSize);
            pValue->m_strModel = str;
            delete[] (str);
        }

		iType = INFO_DATATYPE_UNKNOWN;
		iSize = 0;
		status = m_psGCLib->f_IFGetDeviceInfo(hIF, pValue->GetID().GetAscii(), DEVICE_INFO_DISPLAYNAME, &iType, NULL, &iSize);
        if ((0 == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            status = m_psGCLib->f_IFGetDeviceInfo(hIF, pValue->GetID().GetAscii(), DEVICE_INFO_DISPLAYNAME, &iType, str, &iSize);
            pValue->m_strDisplayName = str;
            delete[] (str);
        }
		bReval = true;
    }
	return bReval;
}



//------------------------------------------------------------------------
//--StGenDeviceInfo----------------------------------------------------------------------
//------------------------------------------------------------------------
StGenDeviceInfo::StGenDeviceInfo(void)
{
}


StGenDeviceInfo::~StGenDeviceInfo(void)
{
}

const StString StGenDeviceInfo::GetID() const
{
	if( !mhData ) return "";
	StGenDeviceInfoValue *pDevInfo = (StGenDeviceInfoValue *)mhData;
	return pDevInfo->m_strID;
}

const StString StGenDeviceInfo::GetVendor() const
{
	if( !mhData ) return "";
	StGenDeviceInfoValue *pDevInfo = (StGenDeviceInfoValue *)mhData;
	return pDevInfo->m_strVendor;
}
const StString StGenDeviceInfo::GetModel() const
{
	if( !mhData ) return "";
	StGenDeviceInfoValue *pDevInfo = (StGenDeviceInfoValue *)mhData;
	return pDevInfo->m_strModel;
}
const StString StGenDeviceInfo::GetDisplayName() const
{
	if( !mhData ) return "";
	StGenDeviceInfoValue *pDevInfo = (StGenDeviceInfoValue *)mhData;
	return pDevInfo->m_strDisplayName;
}

//Å•1.0.0.1051
StGenInterface *StGenDeviceInfo::GetInterface(void)
{
	if( !mhData ) return NULL;
	StGenDeviceInfoValue *pDevInfo = (StGenDeviceInfoValue *)mhData;
	return pDevInfo->GetInterface();
}
//Å£1.0.0.1051
