#include "StdAfx.h"

#include "StGenGCLib.h"
#include "StTL.h"
#include "StGenTL.h"

#include "StInterface.h"
//#include "StGenDevice.h"


//------------------------------------------------------------------------
//--StGenTLValue----------------------------------------------------------------------
//------------------------------------------------------------------------
class StGenTLValue
{
public:
	StGenTLValue(void);
	virtual ~StGenTLValue(void);

	const StString& GetVendor() const{return m_strVendor;}
    const StString& GetModel() const{return m_strModel;}
    const StString& GetVersion() const{return m_strVersion;}
    const StString& GetName() const{return m_strFileName;}
    const StString& GetPathName() const{return m_strPathName;}
    const StString& GetDisplayName() const{return m_strDisplayName;}
    const StString& GetPortName() const{return m_strPortName;}
	const StString& GetID(void) const{return m_strID;}
	
	StInt32 GetNumInterfaces( void );
	StGenInterface * GetGenInterface(StInt32 index);


//private:
	std::vector< StGenInterface * > m_vIFList;
    StString m_strVendor;
    StString m_strModel;
    StString m_strVersion;
    StString m_strFileName;
    StString m_strPathName;
    StString m_strDisplayName;
    StString m_strPortName;
    StString m_strID;
};



StGenTLValue::StGenTLValue()
    : m_strVendor( "" )
    , m_strModel( "" )
    , m_strVersion( "" )
    , m_strFileName( "" )
    , m_strPathName( "" )
    , m_strDisplayName( "" )
    , m_strPortName( "" )
{
}

StGenTLValue::~StGenTLValue()
{
}

StInt32 StGenTLValue::GetNumInterfaces( void )
{
	return (StInt32)m_vIFList.size();
}

StGenInterface * StGenTLValue::GetGenInterface(StInt32 index)
{
	StGenInterface *pIF = NULL;
	if( (unsigned int)index<m_vIFList.size() )
		pIF = m_vIFList[index];
	return pIF;
}



//------------------------------------------------------------------------
//--StTL----------------------------------------------------------------------
//------------------------------------------------------------------------
StTL::StTL(HANDLE hLib)
{
	m_psGCLib = (StGenGCLib*)hLib;
	mhThis = (void *)new StGenTLValue;
	m_hTL = NULL;
}


StTL::~StTL(void)
{

	DeleteInterfaceList();
	TLClose();

	if( mhThis )
	{
		StGenTLValue * pValue = (StGenTLValue *)mhThis;
		delete pValue;
	}

}

StResult StTL::TLOpen(void)
{
	StResult lResult = StResult::Code::GENERIC_ERROR;
	if( m_psGCLib )
	{
		TL_HANDLE hSystem = NULL;
		GC_ERROR status = m_psGCLib->f_TLOpen(&hSystem);
		if( GC_ERR_SUCCESS == status )
		{
			m_hTL = hSystem;
			QueryInformations();	//TL情報取得
			lResult = StResult::Code::OK;
		}
	}
	return lResult;
}

StResult StTL::TLClose(void)
{
	StResult lResult = StResult::Code::GENERIC_ERROR;
	if( IsOpen() )
	{
		GC_ERROR status = m_psGCLib->f_TLClose(m_hTL);
		if( GC_ERR_SUCCESS == status )
		{
			m_hTL = NULL;
			lResult = StResult::Code::OK;
		}
	}
	return lResult;
}

StResult StTL::TLGetInfo(TL_INFO_CMD iInfoCmd,INFO_DATATYPE * piType,void * pBuffer,size_t * piSize)
{
	StResult lResult = StResult::Code::GENERIC_ERROR;

	if( IsOpen() )
	{
		GC_ERROR status = m_psGCLib->f_TLGetInfo(m_hTL,iInfoCmd,piType,pBuffer,piSize);
		if( GC_ERR_SUCCESS == status )
		{
			lResult = StResult::Code::OK;
		}
	}
	return lResult;
}

StResult StTL::TLGetInterfaceID(uint32_t iIndex, char * sIfaceID, size_t * piSize)
{
	StResult lResult = StResult::Code::GENERIC_ERROR;

	if( IsOpen() )
	{
		GC_ERROR status = m_psGCLib->f_TLGetInterfaceID(m_hTL,iIndex,sIfaceID,piSize);
		if( GC_ERR_SUCCESS == status )
		{
			lResult = StResult::Code::OK;
		}
	}
	return lResult;
}

StResult StTL::TLGetInterfaceInfo(const char * sIfaceID, INTERFACE_INFO_CMD iInfoCmd, INFO_DATATYPE * piType, void * pBuffer, size_t * piSize)
{
	StResult lResult = StResult::Code::GENERIC_ERROR;

	if( IsOpen() )
	{
		GC_ERROR status = m_psGCLib->f_TLGetInterfaceInfo(m_hTL, sIfaceID,iInfoCmd,piType,pBuffer,piSize);
		if( GC_ERR_SUCCESS == status )
		{
			lResult = StResult::Code::OK;
		}
	}
	return lResult;
}

StResult StTL::TLGetNumInterfaces(uint32_t * piNumIfaces)
{
	StResult lResult = StResult::Code::GENERIC_ERROR;

	if( IsOpen() )
	{
		GC_ERROR status = m_psGCLib->f_TLGetNumInterfaces(m_hTL, piNumIfaces);
		if( GC_ERR_SUCCESS == status )
		{
			lResult = StResult::Code::OK;
		}
	}
	return lResult;
}

StResult StTL::TLOpenInterface(const char * sIfaceID,IF_HANDLE * phIface)
{
	StResult lResult = StResult::Code::GENERIC_ERROR;

	if( IsOpen() )
	{
		GC_ERROR status = m_psGCLib->f_TLOpenInterface(m_hTL, sIfaceID, phIface);
		if( GC_ERR_SUCCESS == status )
		{
			lResult = StResult::Code::OK;
		}
	}
	return lResult;
}

StResult StTL::TLUpdateInterfaceList(bool8_t * pbChanged,uint64_t iTimeout)
{
	StResult lResult = StResult::Code::GENERIC_ERROR;

	if( IsOpen() )
	{
		GC_ERROR status = m_psGCLib->f_TLUpdateInterfaceList(m_hTL, pbChanged, iTimeout);
		if( GC_ERR_SUCCESS == status )
		{
			lResult = StResult::Code::OK;
		}
	}
	return lResult;
}

void StTL::QueryInformations( void )
{
	if( !IsOpen() ) return;
	if( !mhThis ) return;

    if (m_psGCLib->f_TLGetInfo)
    {

		StGenTLValue * pValue = (StGenTLValue *)mhThis;


        GC_ERROR status;
        INFO_DATATYPE iType = INFO_DATATYPE_UNKNOWN;
        size_t iSize = 0;

        status = m_psGCLib->f_TLGetInfo(m_hTL, TL_INFO_ID, &iType, NULL, &iSize);
        if ((GC_ERR_SUCCESS == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            //m_strID.reserve(iSize);
            status = m_psGCLib->f_TLGetInfo(m_hTL,TL_INFO_ID, &iType, str, &iSize);
            //m_strID = str;
            pValue->m_strID = str;
            delete [] str;
        }

        iType = INFO_DATATYPE_UNKNOWN;
        iSize = 0;
        status = m_psGCLib->f_TLGetInfo(m_hTL, TL_INFO_VENDOR, &iType, NULL, &iSize);
        if ((GC_ERR_SUCCESS == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            //m_strVendor.reserve(iSize);
            status = m_psGCLib->f_TLGetInfo(m_hTL,TL_INFO_VENDOR, &iType, str, &iSize);
            //m_strVendor = str;
            pValue->m_strVendor = str;
            delete[] (str);
        }

        iType = INFO_DATATYPE_UNKNOWN;
        iSize = 0;
        status = m_psGCLib->f_TLGetInfo(m_hTL,TL_INFO_MODEL, &iType, NULL, &iSize);
        if ((GC_ERR_SUCCESS == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            //m_strModel.reserve(iSize);
            status = m_psGCLib->f_TLGetInfo(m_hTL,TL_INFO_MODEL, &iType, str, &iSize);
            //m_strModel = str;
            pValue->m_strModel = str;
            delete[] (str);
        }

        iType = INFO_DATATYPE_UNKNOWN;
        iSize = 0;
        status = m_psGCLib->f_TLGetInfo(m_hTL,TL_INFO_VERSION, &iType, NULL, &iSize);
        if ((GC_ERR_SUCCESS == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            //m_strVersion.reserve(iSize);
            status = m_psGCLib->f_TLGetInfo(m_hTL,TL_INFO_VERSION, &iType, str, &iSize);
            //m_strVersion = str;
            pValue->m_strVersion = str;
            delete[] (str);
        }

        iType = INFO_DATATYPE_UNKNOWN;
        iSize = 0;
        status = m_psGCLib->f_TLGetInfo(m_hTL,TL_INFO_NAME, &iType, NULL, &iSize);
        if ((GC_ERR_SUCCESS == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            //m_strFileName.reserve(iSize);
            status = m_psGCLib->f_TLGetInfo(m_hTL,TL_INFO_NAME, &iType, str, &iSize);
            //m_strFileName = str;
            pValue->m_strFileName = str;
            delete[] (str);
        }

        iType = INFO_DATATYPE_UNKNOWN;
        iSize = 0;
        status = m_psGCLib->f_TLGetInfo(m_hTL,TL_INFO_PATHNAME, &iType, NULL, &iSize);
        if ((GC_ERR_SUCCESS == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            //m_strPathName.reserve(iSize);
            status = m_psGCLib->f_TLGetInfo(m_hTL,TL_INFO_PATHNAME, &iType, str, &iSize);
            //m_strPathName = str;
            pValue->m_strPathName = str;
            delete[] (str);
        }

        iType = INFO_DATATYPE_UNKNOWN;
        iSize = 0;
        status = m_psGCLib->f_TLGetInfo(m_hTL,TL_INFO_DISPLAYNAME, &iType, NULL, &iSize);
        if ((GC_ERR_SUCCESS == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            //m_strDisplayName.reserve(iSize);
            status = m_psGCLib->f_TLGetInfo(m_hTL,TL_INFO_DISPLAYNAME, &iType, str, &iSize);
            //m_strDisplayName = str;
            pValue->m_strDisplayName = str;
            delete[] (str);
        }

		//PortName
        iType = INFO_DATATYPE_UNKNOWN;
        iSize = 0;
        status = m_psGCLib->f_GCGetPortInfo(m_hTL,PORT_INFO_PORTNAME, &iType, NULL, &iSize);
        if ((GC_ERR_SUCCESS == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            //m_strDisplayName.reserve(iSize);
            status = m_psGCLib->f_GCGetPortInfo(m_hTL,PORT_INFO_PORTNAME, &iType, str, &iSize);
            //m_strPortName = str;
            pValue->m_strPortName = str;
            delete[] (str);
        }
    }
}

StResult StTL::UpdateInterfaceList( bool * pbChanged )
{
	StResult lResult(StResult::Code::OK);
	if( !IsOpen() ) return StResult::Code::GENERIC_ERROR;


    if (m_psGCLib->f_TLGetInfo)
    {
		int iTimeout=250;
		//int iTimeout=500;
		bool bChanged;
		GC_ERROR status = m_psGCLib->f_TLUpdateInterfaceList(m_hTL, &bChanged, iTimeout);

//{TCHAR text[256];
//swprintf_s( text,256,TEXT("f_TLUpdateInterfaceList ==> [%d]  \n"),status);
//OutputDebugString(text);}

		if( status!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
		if( pbChanged ) *pbChanged = bChanged;

		if( GetNumInterfaces() > 0 )	//Euresys用に追加
		{
			if( pbChanged ) *pbChanged = bChanged;
			if( !bChanged ) return lResult;
		}

		uint32_t iNumIfaces=0;
		status = m_psGCLib->f_TLGetNumInterfaces(m_hTL, &iNumIfaces);
//{TCHAR text[256];
//swprintf_s( text,256,TEXT("f_TLGetNumInterfaces ==> [%d] [%d] \n"),status,iNumIfaces);
//OutputDebugString(text);}
		if( status!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;

		//EuresysのCTIファイルでは最初InterfaceがあってもbChanged＝FALSEになる
		if( GetNumInterfaces()==0 && iNumIfaces>0 )
		{
			bChanged = true;
			if( pbChanged ) *pbChanged = bChanged;
		}

		DeleteInterfaceList();

		StGenTLValue *pTL = (StGenTLValue *)mhThis;
		
        for (unsigned int i = 0; i < iNumIfaces; i++)
        {
			StInterface* pInterface = new StInterface( this, GetInterfaceID(i) );
			if( pInterface==NULL ) return StResult::Code::GENERIC_ERROR;
			pTL->m_vIFList.push_back(pInterface);
		}
	}
	return lResult;
}

void StTL::DeleteInterfaceList( void )
{
	StGenTLValue *pTL = (StGenTLValue *)mhThis;
	if( pTL->m_vIFList.size()==0 ) return; 

	for( uint32_t i=0; i<pTL->m_vIFList.size(); i++ )
	{
		if( pTL->m_vIFList[i] )
			delete pTL->m_vIFList[i];
	}
	pTL->m_vIFList.clear();

}

StString StTL::GetInterfaceID( uint32_t index )
{
    StString strID = "";
    
    if(IsOpen())
    {
        size_t iSize = 0;
        GC_ERROR status = m_psGCLib->f_TLGetInterfaceID(m_hTL, index, NULL, &iSize );
        if (0 == status && iSize > 0)
        {
            char* str = new char[iSize];
            status = m_psGCLib->f_TLGetInterfaceID(m_hTL, index, str, &iSize );
            strID = str;
            delete[] (str);
        }
    }
    return strID;
}

//uint32_t StTL::GetNumInterfaces( void )
//{
//    uint32_t num = 0;
//    if (IsOpen())    
//    {
//        //m_psGCLib->f_TLGetNumInterfaces(m_hTL, &num);
//    }
//    return num;
//}

StResult StTL::UpdateAllList( bool * pbChanged )
{
	StResult lResult(StResult::Code::OK);
	



	bool bChanged = false;
	if( pbChanged ) *pbChanged = false;
	if(IsOpen() || TLOpen())
	{
		UpdateInterfaceList(&bChanged);
		if( pbChanged ) *pbChanged |= bChanged;
		StInt32 numIF = GetNumInterfaces();
		for( StInt32 i=0; i<numIF; i++ )
		{
			StInterface *pInterface = (StInterface *)GetGenInterface(i);
			if( pInterface->IsOpen() || pInterface->IFOpen() )
			{
				pInterface->UpdateDeviceList(&bChanged);
				if( pbChanged ) *pbChanged |= bChanged;
				StInt32 numDev = pInterface->GetNumDevices();

//TRACE(_T(" @@@@@ 1 GetName=%s \n"),  pInterface->GetDisplayName().GetUnicode());

				for( StInt32 iDev=0; iDev<numDev; iDev++ )
				{
				//	//インターフェースから情報読み込み
				//	StGenDevice *pDevice = pInterface->GetGenDevice(iDev);
				//	//if( pDevice->IsOpen() || pDevice->DevOpen() )
				//	if( pDevice!=NULL && pDevice->IsOpen() )
				//	{
				//	}
				}
				
//TRACE(_T(" @@@@@ 2 GetName=%s \n"),  pInterface->GetDisplayName().GetUnicode());

			}
		}
	}
	
	return lResult;
}

StResult StTL::InvalidateInterfaceList( bool * pbChanged )
{

	StResult lResult(StResult::Code::OK);
	if( !IsOpen() ) return StResult::Code::GENERIC_ERROR;

	bool bChanged=false;
	/*
    if (m_psGCLib->f_TLGetInfo)
    {
		//int iTimeout=250;
		int iTimeout=500;
		GC_ERROR status = m_psGCLib->f_TLUpdateInterfaceList(m_hTL, &bChanged, iTimeout);
		if( status!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
		if( bChanged )	//変化なし
		{

			uint32_t iNumIfaces=0;
			status = m_psGCLib->f_TLGetNumInterfaces(m_hTL, &iNumIfaces);
			if( status!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;

			//削除 
			do
			{
				bool bDelete=false;
				for( std::vector<StGenInterface*>::iterator it = m_vIFList.begin(); it != m_vIFList.end(); it++ )
				{
					bool bExist = false;
					for (unsigned int i = 0; i < iNumIfaces; i++)
					{
						if( (*it)->GetID()==GetInterfaceID(i) )
						{
							bExist = true;
							break;
						}
					}
					if( !bExist )
					{
						delete (*it);
						m_vIFList.erase(it);
						bChanged = true;
						//削除したら最初からおこなう
						bDelete = true;
						break;
					}
				}
				if( !bDelete ) break;
			}while(1);

			//作成
			for (unsigned int i = 0; i < iNumIfaces; i++)
			{
				StString aID = GetInterfaceID(i);
				uint32_t iNumIFList = m_vIFList.size();
				bool bExist = false;
				for( unsigned int j = 0; j < iNumIFList; j++ )
				{
					if( m_vIFList[j]->GetID()==aID )
					{
						bExist = true;
						break;
					}
				}
				if( !bExist )
				{
					//Create
					StGenInterface* pInterface = new StGenInterface(m_psGCLib, this, aID);
					if( pInterface==NULL ) return StResult::Code::GENERIC_ERROR;
					m_vIFList.push_back(pInterface);
					bChanged = true;
				}
			}
		}

		//InvalidateDevice
		for( std::vector<StGenInterface*>::iterator it = m_vIFList.begin(); it != m_vIFList.end(); it++ )
		{
			bool bChangedDev=false;
			(*it)->InvalidateDeviceList( &bChangedDev );
			if( bChangedDev ) bChanged=true;
		}
	}
	*/
	if( pbChanged ) *pbChanged = bChanged;
	return lResult;

}

//------------------------------------------------------------------------
//--StGenTL----------------------------------------------------------------------
//------------------------------------------------------------------------
StGenTL::StGenTL(void)
{
	
	//StGenGCLib *pGCLib = (StGenGCLib *)hTL;
	//mhThis = (void *)new StTL(pGCLib);
}

StGenTL::~StGenTL(void)
{
	//if( mhThis )
	//{
	//	StTL *pTL = (StTL *)mhThis;
	//	delete pTL;
	//}
}
const StString& StGenTL::GetVendor() const
{
	//if( !mhThis ) return StString("");
	StGenTLValue *pTL = (StGenTLValue *)mhThis;
	return pTL->m_strVendor;
}
const StString& StGenTL::GetModel() const
{
	StGenTLValue *pTL = (StGenTLValue *)mhThis;
	return pTL->GetModel();
}
const StString& StGenTL::GetVersion() const
{
	StGenTLValue *pTL = (StGenTLValue *)mhThis;
	return pTL->GetVersion();
}
const StString& StGenTL::GetName() const
{
	StGenTLValue *pTL = (StGenTLValue *)mhThis;
	return pTL->GetName();
}

const StString& StGenTL::GetPathName() const
{
	StGenTLValue *pTL = (StGenTLValue *)mhThis;
	return pTL->GetPathName();
}
const StString& StGenTL::GetDisplayName() const
{
	StGenTLValue *pTL = (StGenTLValue *)mhThis;
	return pTL->GetDisplayName();
}
const StString& StGenTL::GetPortName() const
{
	StGenTLValue *pTL = (StGenTLValue *)mhThis;
	return pTL->GetPortName();
}
const StString& StGenTL::GetID() const
{
	StGenTLValue *pTL = (StGenTLValue *)mhThis;
	return pTL->GetID();
}
StInt32 StGenTL::GetNumInterfaces( void )
{
	if( !mhThis ) return 0;
	StGenTLValue *pTL = (StGenTLValue *)mhThis;
	return pTL->GetNumInterfaces();
}
StGenInterface * StGenTL::GetGenInterface(StInt32 index)
{
	if( !mhThis ) return NULL;
	StGenTLValue *pTL = (StGenTLValue *)mhThis;
	return pTL->GetGenInterface(index);
}

