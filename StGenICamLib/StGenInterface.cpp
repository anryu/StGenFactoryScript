#include "StdAfx.h"
#include "StInterface.h"
//#include "StGenDevice.h"
//#include "MainFrm.h"

#include "StDeviceInfo.h"
#include "StGenPort.h"


//------------------------------------------------------------------------
//--StGenInterfaceValue----------------------------------------------------------------------
//------------------------------------------------------------------------
class StGenInterfaceValue
{
public:
	StGenInterfaceValue(void);
	virtual ~StGenInterfaceValue(void);

    const StString& GetDisplayName() const{return m_strDisplayName;}
    const StString& GetPortName() const{return m_strPortName;}
	const StString& GetID(void) const{return m_strID;}
	
	StInt32 GetNumDeviceInfo( void );
	StGenDeviceInfo * GetGenDeviceInfo(StInt32 index);

	BOOL GetGenEnumParameter( StString lFeature, int & iData, bool Verify, bool IgnoreCache );
	BOOL GetGenEnumParameter( StString lFeature, StString & strData, bool Verify, bool IgnoreCache );
	BOOL SetGenEnumParameter( StString lFeature, int iData );
	BOOL SetGenEnumParameter( StString lFeature, StString strData );
	BOOL GetGenFloatParameter( StString lFeature, double & dblData, bool Verify, bool IgnoreCache );
	BOOL SetGenFloatParameter( StString lFeature, double dblData );
	BOOL ExecuteCommand(StString &aFeature);

	//▼1.0.0.1051
	BOOL GetGenIntParameter( StString lFeature, int & iData, bool Verify, bool IgnoreCache );
	BOOL GetGenIntMinMaxParameter( StString lFeature, int & iMin, int & iMax );
	BOOL SetGenIntParameter( StString lFeature, int iData );
	BOOL GetGenBoolParameter( StString lFeature, bool & bData, bool Verify, bool IgnoreCache );
	BOOL SetGenBoolParameter( StString lFeature, bool bData );
	BOOL GetGenStringParameter( StString lFeature, StString & strData, bool Verify, bool IgnoreCache );
	BOOL SetGenStringParameter( StString lFeature, StString & strData );
	BOOL SetGenCommandParameter( StString lFeature );
	//▲1.0.0.1051


	bool Read( StInt64 iAddress, char *pBuffer, size_t size );
	bool Write( StInt64 iAddress, char *pBuffer, size_t size );

//private:
	std::vector< StGenDeviceInfo * > m_vDeviceInfoList;

	//IF_HANDLE m_hIF;
	//StGenGCLib* m_psGCLib;
	CNodeMapRef *m_pNodeMap;
	StGenPort *m_pGenTLPort;

    StString m_strDisplayName;
    StString m_strPortName;
    StString m_strID;

};

StGenInterfaceValue::StGenInterfaceValue()
    : m_strID( "" )
    , m_strDisplayName( "" )
    , m_strPortName( "" )
	//, m_hIF( NULL )
{
	m_pNodeMap = NULL;
	m_pGenTLPort = NULL;
}

StGenInterfaceValue::~StGenInterfaceValue()
{
}

StInt32 StGenInterfaceValue::GetNumDeviceInfo( void )
{
	return (StInt32)m_vDeviceInfoList.size();
}

StGenDeviceInfo * StGenInterfaceValue::GetGenDeviceInfo(StInt32 index)
{
	StGenDeviceInfo *pDevice = NULL;
	if( (unsigned int)index<m_vDeviceInfoList.size() )
		pDevice = m_vDeviceInfoList[index];
	return pDevice;
}


BOOL StGenInterfaceValue::GetGenEnumParameter(StString lFeature, int & iData, bool Verify, bool IgnoreCache)
{
	BOOL bReval = FALSE;
	if( m_pNodeMap )
	{
		INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
		if( ptrINode->GetPrincipalInterfaceType()==intfIEnumeration )
		{
			CEnumerationPtr ptrEnum = ptrINode;
			iData = (int)ptrEnum->GetIntValue(Verify, IgnoreCache);
			bReval = TRUE;
		}
	}
	return bReval;
}

BOOL StGenInterfaceValue::GetGenEnumParameter(StString lFeature, StString & strData, bool Verify, bool IgnoreCache)
{
	BOOL bReval = FALSE;
	if( m_pNodeMap )
	{
		INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
		if( ptrINode->GetPrincipalInterfaceType()==intfIEnumeration )
		{
			CEnumerationPtr ptrEnum = ptrINode;
			strData = ptrEnum->ToString(Verify, IgnoreCache);
			bReval = TRUE;
		}
	}
	return bReval;
}

BOOL StGenInterfaceValue::SetGenEnumParameter(StString lFeature, int iData)
{
	BOOL bReval = FALSE;
	if( m_pNodeMap )
	{
		INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
		if( ptrINode->GetPrincipalInterfaceType()==intfIEnumeration )
		{
			CEnumerationPtr ptrEnum = ptrINode;
			ptrEnum->SetIntValue(iData);
			bReval = TRUE;
		}
	}
	return bReval;
}

BOOL StGenInterfaceValue::SetGenEnumParameter(StString lFeature, StString strData)
{
	BOOL bReval = FALSE;
	if( m_pNodeMap )
	{
		INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
		if( ptrINode->GetPrincipalInterfaceType()==intfIEnumeration )
		{
			CEnumerationPtr ptrEnum = ptrINode;
			gcstring szData = strData.GetAscii();
			ptrEnum->FromString(szData);
			bReval = TRUE;
		}
	}
	return bReval;
}

BOOL StGenInterfaceValue::GetGenFloatParameter(StString lFeature, double & dblData, bool Verify, bool IgnoreCache)
{
	BOOL bReval = FALSE;
	if( m_pNodeMap )
	{
		INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
		if( ptrINode->GetPrincipalInterfaceType()==intfIFloat )
		{
			CFloatPtr ptrFloat = ptrINode;
			dblData = ptrFloat->GetValue( Verify, IgnoreCache );
			bReval = TRUE;
		}
	}
	return bReval;
}

BOOL StGenInterfaceValue::SetGenFloatParameter(StString lFeature, double dblData)
{
	BOOL bReval = FALSE;
	if( m_pNodeMap )
	{
		INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
		if( ptrINode->GetPrincipalInterfaceType()==intfIFloat )
		{
			CFloatPtr ptrFloat = ptrINode;
			ptrFloat->SetValue(dblData);
			bReval = TRUE;
		}
	}
	return bReval;
}

BOOL StGenInterfaceValue::ExecuteCommand(StString &aFeature)
{
	BOOL bReval = FALSE;
	if( m_pNodeMap )
	{
		INode *ptrINode = m_pNodeMap->_GetNode(aFeature.GetAscii());
//TRACE(TEXT("aFeature(%s) \n"), aFeature.GetUnicode());
//TRACE(TEXT("m_pNodeMap(0x%x) \n"), m_pNodeMap);
//TRACE(TEXT("ptrINode(0x%x) \n"), ptrINode);
if( !ptrINode )
{
	NodeList_t allNodes;
	m_pNodeMap->_GetNodes(allNodes);
	unsigned int allNum = allNodes.size();
	unsigned int iCount=0;
	for( unsigned int a=0; a<allNum; a++ )
	{
//TRACE(TEXT("  GetName(%s) \n"), CString(allNodes[a]->GetName()));
	}

}

try
{
		if( ptrINode && ptrINode->GetPrincipalInterfaceType()==intfICommand )
		{
			CCommandPtr ptrCommand = ptrINode;
			ptrCommand->Execute();
			bReval = TRUE;
		}
}
catch(GenICam::GenericException &e)
{
 		TCHAR szTmp[256];
		MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
		OutputDebugString(szTmp);
		OutputDebugString(_T("\n"));

}
	}
	return bReval;
}

//▼1.0.0.1051
BOOL StGenInterfaceValue::GetGenIntParameter( StString lFeature, int & iData, bool Verify, bool IgnoreCache )
{
	GenApi::INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=intfIInteger ) return false;

    CIntegerPtr ptrInteger = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
            iData = (int)ptrInteger->GetValue( Verify, IgnoreCache );
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}
BOOL StGenInterfaceValue::GetGenIntMinMaxParameter( StString lFeature, int & iMin, int & iMax )
{
	GenApi::INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=intfIInteger ) return false;

    CIntegerPtr ptrInteger = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
            iMin = (int)ptrInteger->GetMin();
            iMax = (int)ptrInteger->GetMax();
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

BOOL StGenInterfaceValue::SetGenIntParameter( StString lFeature, int iData )
{
	GenApi::INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=intfIInteger ) return false;

    CIntegerPtr ptrInteger = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
            ptrInteger->SetValue(iData);
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

BOOL StGenInterfaceValue::GetGenBoolParameter( StString lFeature, bool & bData, bool Verify, bool IgnoreCache )
{
	GenApi::INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIBoolean ) return false;

    CBooleanPtr ptrBoolean = ptrINode;

    //if(ptrBoolean.IsValid())
    {
        try
        {
            bData = (bool)ptrBoolean->GetValue( Verify, IgnoreCache );
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

BOOL StGenInterfaceValue::SetGenBoolParameter( StString lFeature, bool bData )
{
	GenApi::INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIBoolean ) return false;

    CBooleanPtr ptrBoolean = ptrINode;

    //if(ptrBoolean.IsValid())
    {
        try
        {
            ptrBoolean->SetValue(bData);
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

BOOL StGenInterfaceValue::GetGenStringParameter( StString lFeature, StString & strData, bool Verify, bool IgnoreCache )
{
	GenApi::INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIString ) return false;

	CStringPtr ptrString = ptrINode;
    //if(ptrInteger.IsValid())
    {
        try
        {
            strData = ptrString->ToString( Verify, IgnoreCache );
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

BOOL StGenInterfaceValue::SetGenStringParameter( StString lFeature, StString &strData )
{
	GenApi::INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIString ) return false;

	CStringPtr ptrString = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
			gcstring strValue = strData;
            ptrString->FromString(strValue);
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

BOOL StGenInterfaceValue::SetGenCommandParameter( StString lFeature )
{
	GenApi::INode *ptrINode = m_pNodeMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfICommand ) return false;

    CCommandPtr ptrCommand = ptrINode;

    //if(ptrCommand.IsValid())
    {
        try
        {
            ptrCommand->Execute();
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

//▲1.0.0.1051

bool StGenInterfaceValue::Read( StInt64 iAddress, char *pBuffer, size_t size )
{
	bool bReval = false;
	if( m_pGenTLPort )
	{
		m_pGenTLPort->Read(pBuffer, iAddress, size);
		bReval = m_pGenTLPort->GetLastValue();
	}
    return bReval;
}

bool StGenInterfaceValue::Write( StInt64 iAddress, char *pBuffer, size_t size )
{
	bool bReval = false;
	if( m_pGenTLPort )
	{
		m_pGenTLPort->Write(pBuffer, iAddress, size);
		bReval = m_pGenTLPort->GetLastValue();
	}
    return bReval;
}



//------------------------------------------------------------------------
//--StInterface----------------------------------------------------------------------
//------------------------------------------------------------------------
StInterface::StInterface(StTL* psTL,StString strID)
	: m_psTL(psTL)
	//, m_strID(strID)
	//, m_strDisplayName("")
	//, m_strPortName("")
	, m_hIF(NULL)
	, m_pNodeMap(NULL)
	, m_pGenTLPort(NULL)
{
	m_psGCLib = psTL->GetGenGCLib();

	StGenInterfaceValue *pIF = new StGenInterfaceValue;
	mhData = (void *)pIF;

//TRACE(TEXT("StInterface  mhData (0x%x) this(0x%x)\n"), mhData, this);
	if( pIF )
	{
		pIF->m_strID = strID;
	}
	m_hIF = NULL;
}


StInterface::~StInterface(void)
{
	DeleteDeviceList();
	IFClose();

	if( mhData )
	{
		StGenInterfaceValue * pValue = (StGenInterfaceValue *)mhData;
		delete pValue;
	}


}


StResult StInterface::IFOpen( void )
{
	StResult lResult = StResult::Code::OK;
    if (!IsOpen())
    {
		IF_HANDLE hIF = NULL;
        lResult = StResult::Code::GENERIC_ERROR;

		StGenInterfaceValue * pValue = (StGenInterfaceValue *)mhData;

//TRACE(TEXT("f_TLOpenInterface(%s)\n"),  pValue->m_strID.GetUnicode());

		GC_ERROR status = m_psGCLib->f_TLOpenInterface(m_psTL->GetHandle(), pValue->m_strID.GetAscii(), &hIF);

		if (0 == status)
        {
			m_hIF = hIF;
            // update informations                
			QueryInformations();

			//Node作成
			gcstring strXML;
			m_psGCLib->RetrieveXML(hIF, strXML);

			CNodeMapRef *pNodeMap = new CNodeMapRef();
			StGenPort *pGenTLPort = NULL;
			if( pNodeMap )
			{
				pGenTLPort = new StGenPort(m_psGCLib, hIF);
				if( pGenTLPort ) 
				{
					pNodeMap->_LoadXMLFromString(strXML);
					if( pNodeMap->_Connect(pGenTLPort, pValue->m_strPortName.GetAscii()) )
					{
						m_pGenTLPort = (void *)pGenTLPort;
						m_pNodeMap = pNodeMap;
						pValue->m_pNodeMap = m_pNodeMap;
						pValue->m_pGenTLPort = pGenTLPort;
					}
				}
				if( m_pGenTLPort==NULL )
				{
					if( pNodeMap )	delete pNodeMap;
					if( pGenTLPort )	delete pGenTLPort;
				}
			}

			lResult = StResult::Code::OK;
        }
		else
		{
           //m_strDisplayName = str;
		}
    }
	return lResult;
}


StResult StInterface::IFClose( void )
{
	StResult lResult = StResult::Code::GENERIC_ERROR;

	if( m_pNodeMap )
	{
		delete m_pNodeMap;
		m_pNodeMap = NULL;
		StGenInterfaceValue * pValue = (StGenInterfaceValue *)mhData;
		pValue->m_pNodeMap = NULL;
	}
	if( m_pGenTLPort )
	{
		StGenPort *pGenTLPort = (StGenPort *)m_pGenTLPort;
		delete pGenTLPort;
		m_pGenTLPort = NULL;
		StGenInterfaceValue * pValue = (StGenInterfaceValue *)mhData;
		pValue->m_pGenTLPort = NULL;
	}

	if (IsOpen())
    {
        GC_ERROR status = m_psGCLib->f_IFClose(m_hIF);
		if( GC_ERR_SUCCESS == status )
		{
			m_hIF = NULL;
			lResult = StResult::Code::OK;
		}
    }

	return lResult;
}


void StInterface::QueryInformations( void )
{
	if( !IsOpen() ) return;
	if( !mhData ) return;


    //if (m_psTL && m_psGCLib->f_TLGetInterfaceInfo)

	if (m_psGCLib->f_IFGetInfo)
    {

		StGenInterfaceValue * pValue = (StGenInterfaceValue *)mhData;
		
		//pValue->m_hIF = m_hIF;

		GC_ERROR status;
        size_t iSize = 0;
        INFO_DATATYPE iType = INFO_DATATYPE_UNKNOWN;

        status = m_psGCLib->f_IFGetInfo(m_hIF, INTERFACE_INFO_DISPLAYNAME, &iType, NULL, &iSize);
		//▼1.0.0.4
		//KAYA - Problem
        //if ((0 == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        if ((0 == status) && (iSize > 0) )
		//▲1.0.0.4
        {
			char* str = new char[iSize];
            status = m_psGCLib->f_IFGetInfo(m_hIF, INTERFACE_INFO_DISPLAYNAME, &iType, str, &iSize);
            //m_strDisplayName = str;
            pValue->m_strDisplayName = str;
            delete[] (str);
        }

        iType = INFO_DATATYPE_UNKNOWN;
        iSize = 0;
        status = m_psGCLib->f_GCGetPortInfo(m_hIF,PORT_INFO_PORTNAME, &iType, NULL, &iSize);
        if ((GC_ERR_SUCCESS == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            status = m_psGCLib->f_GCGetPortInfo(m_hIF,PORT_INFO_PORTNAME, &iType, str, &iSize);
            pValue->m_strPortName = str;
            //m_strPortName = str;
            delete[] (str);
        }
    }
}

StResult StInterface::UpdateDeviceList( bool *pbChanged )
{
	StResult lResult(StResult::Code::OK);
	if( !IsOpen() ) return StResult::Code::GENERIC_ERROR;

    if (m_psGCLib->f_TLGetInfo)
    {
		//int iTimeout=250;
		uint64_t iTimeout=500;
		//int iTimeout=10000;
		bool8_t bChanged=false;
		GC_ERROR status = m_psGCLib->f_IFUpdateDeviceList(m_hIF, &bChanged, iTimeout);
		if( status!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
		if( pbChanged ) *pbChanged = true;

		uint32_t iNumDevices=0;
		status = m_psGCLib->f_IFGetNumDevices(m_hIF, &iNumDevices);
		if( status!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;

		DeleteDeviceList();

		StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;

		for (unsigned int i = 0; i < iNumDevices; i++)
        {
			StDeviceInfo* pDeviceInfo = new StDeviceInfo( this, GetDeviceID(i));
			if( pDeviceInfo==NULL ) return StResult::Code::GENERIC_ERROR;
			pDeviceInfo->IFQueryInformations();	//カメラ情報取得
			pIF->m_vDeviceInfoList.push_back(pDeviceInfo);
		}
	}
	return lResult;
}

void StInterface::DeleteDeviceList( void )
{
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	for( uint32_t i=0; i<pIF->m_vDeviceInfoList.size(); i++ )
	{
		delete pIF->m_vDeviceInfoList[i];
	}
	if( pIF->m_vDeviceInfoList.size()>0 )
		pIF->m_vDeviceInfoList.clear();

}

StString StInterface::GetDeviceID( uint32_t index )
{
    StString strID = "";

    if(IsOpen())
    {
        size_t iSize = 0;
        GC_ERROR status = m_psGCLib->f_IFGetDeviceID(m_hIF, index, NULL, &iSize );
        if (0 == status && iSize > 0)
        {
            char* str = new char[iSize];
			if( str )
			{
				status = m_psGCLib->f_IFGetDeviceID(m_hIF, index, str, &iSize );
				strID = str;
				delete [] str;
			}
        }
    }
    return strID;
}

uint32_t StInterface::GetNumDevices( void )
{
    uint32_t num = 0;

    if (IsOpen())    
    {
        GC_ERROR status = m_psGCLib->f_IFGetNumDevices(m_hIF, &num);
    }
    return num;
}

/*
//
StResult StInterface::InvalidateDeviceList( bool * pbChanged )
{
	bool bChanged=false;
	StResult lResult(StResult::Code::GENERIC_ERROR);
	if( IsOpen() || IFOpen() )
	{
		if (m_psGCLib->f_TLGetInfo)
		{
			uint64_t iTimeout=500;
			//int iTimeout=1000;
			bool8_t bDevChanged=false;
			GC_ERROR status = m_psGCLib->f_IFUpdateDeviceList(m_hIF, &bDevChanged, iTimeout);
			if( status!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
			if( !bDevChanged )	//変化なし
			{
				if( pbChanged ) *pbChanged = bDevChanged;
				return StResult::Code::OK;
			}

			uint32_t iNumDevices=0;
			status = m_psGCLib->f_IFGetNumDevices(m_hIF, &iNumDevices);
			if( status!=GC_ERR_SUCCESS ) return StResult::Code::GENERIC_ERROR;
			//削除
			do
			{
				bool bDelete=false;
	//			for( std::vector<StGenDevice*>::iterator it = m_vDevList.begin(); it != m_vDevList.end(); it++ )
	//			{
	//				bool bExist = false;
	//				for (unsigned int i = 0; i < iNumDevices; i++)
	//				{
	//					if( (*it)->GetID()==GetDeviceID(i) )
	//					{
	//						bExist = true;
	//						break;
	//					}
	//				}
	//				if( !bExist )
	//				{
	//					//Delete
	//					//(*it)->AllStopPolling
	////					((CStCamGenAppDoc*)(*it)->GetDocument())
	//					//ここでCMainFrameを使用しないほうがいいとは思うが
	//					CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	//					pFrame->AllStopPolling(*it);

	//					//Disconnectを行う

	//					pFrame->EraseDevice(*it);

	//					delete (*it);
	//					m_vDevList.erase(it);
	//					bChanged = true;
	//					//削除したら最初からおこなう、フリーズしたため
	//					bDelete = true;
	//					break;
	//				}
	//			}
				if( !bDelete ) break;
			}while(1);

			//作成
			for (unsigned int i = 0; i < iNumDevices; i++)
			{
				//StString aID = GetDeviceID(i);
				//INT iNumDevList = m_vDevList.size();
				//bool bExist = false;
				//for( unsigned int j = 0; j < iNumDevList; j++ )
				//{
				//	if( m_vDevList[j]->GetID()==aID )
				//	{
				//		bExist = true;
				//		break;
				//	}
				//}
				//if( !bExist )
				//{
				//	//Create
				//	StGenDevice* pDevice = new StGenDevice(m_psGCLib, this, aID);
				//	if( pDevice==NULL ) return StResult::Code::GENERIC_ERROR;
				//	m_vDevList.push_back(pDevice);
				//	bChanged = true;
				//}
			}
			lResult = StResult::Code::OK;
		}
	}
	if( pbChanged ) *pbChanged = bChanged;
	return lResult;
}
*/

//------------------------------------------------------------------------
//--StGenInterface----------------------------------------------------------------------
//------------------------------------------------------------------------
StGenInterface::StGenInterface(void)
{
	
	//StGenGCLib *pGCLib = (StGenGCLib *)hTL;
	//mhThis = (void *)new StTL(pGCLib);
}

StGenInterface::~StGenInterface(void)
{
	//if( mhThis )
	//{
	//	StTL *pTL = (StTL *)mhThis;
	//	delete pTL;
	//}
}






const StString StGenInterface::GetID() const
{
	if( !mhData ) return "";
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->m_strID;
}

const StString StGenInterface::GetDisplayName() const
{
	if( !mhData ) return "";
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->m_strDisplayName;
}

const StString StGenInterface::GetPortName() const
{
	if( !mhData ) return "";
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->m_strPortName;
}

StInt32 StGenInterface::GetNumDeviceInfo( void )
{
	if( !mhData ) return 0;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->GetNumDeviceInfo();
}

StGenDeviceInfo * StGenInterface::GetGenDeviceInfo(StInt32 index)
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->GetGenDeviceInfo(index);
}

BOOL StGenInterface::GetGenEnumParameter(StString lFeature, int & iData, bool Verify, bool IgnoreCache)
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->GetGenEnumParameter(lFeature, iData, Verify, IgnoreCache);
}

BOOL StGenInterface::GetGenEnumParameter(StString lFeature, StString & strData, bool Verify, bool IgnoreCache)
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->GetGenEnumParameter(lFeature, strData, Verify, IgnoreCache);
}

BOOL StGenInterface::SetGenEnumParameter(StString lFeature, int iData)
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->SetGenEnumParameter(lFeature, iData);
}

BOOL StGenInterface::SetGenEnumParameter(StString lFeature, StString strData)
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->SetGenEnumParameter(lFeature, strData);
}

BOOL StGenInterface::GetGenFloatParameter(StString lFeature, double & dblData, bool Verify, bool IgnoreCache)
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->GetGenFloatParameter(lFeature, dblData, Verify, IgnoreCache);
}

BOOL StGenInterface::SetGenFloatParameter(StString lFeature, double dblData)
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->SetGenFloatParameter(lFeature, dblData);
}

BOOL StGenInterface::ExecuteCommand(StString aFeature)
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->ExecuteCommand(aFeature);
}

//▼1.0.0.1051
BOOL StGenInterface::GetGenIntParameter( StString lFeature, int & iData, bool Verify, bool IgnoreCache )
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->GetGenIntParameter(lFeature, iData, Verify, IgnoreCache);
}
BOOL StGenInterface::GetGenIntMinMaxParameter( StString lFeature, int & iMin, int & iMax )
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->GetGenIntMinMaxParameter(lFeature, iMin, iMax);
}
BOOL StGenInterface::SetGenIntParameter( StString lFeature, int iData )
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->SetGenIntParameter(lFeature, iData);
}

BOOL StGenInterface::GetGenBoolParameter( StString lFeature, bool & bData, bool Verify, bool IgnoreCache )
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->GetGenBoolParameter(lFeature, bData, Verify, IgnoreCache);
}
BOOL StGenInterface::SetGenBoolParameter( StString lFeature, bool bData )
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->SetGenBoolParameter(lFeature, bData);
}
BOOL StGenInterface::GetGenStringParameter( StString lFeature, StString & strData, bool Verify, bool IgnoreCache )
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->GetGenStringParameter(lFeature, strData, Verify, IgnoreCache);
}
BOOL StGenInterface::SetGenStringParameter( StString lFeature, StString & strData )
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->SetGenStringParameter(lFeature, strData);
}
BOOL StGenInterface::SetGenCommandParameter( StString lFeature )
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->SetGenCommandParameter(lFeature);
}
//▲1.0.0.1051


bool StGenInterface::Read( StInt64 iAddress, char *pBuffer, size_t size )
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->Read( iAddress, pBuffer, size );
}

bool StGenInterface::Write( StInt64 iAddress, char *pBuffer, size_t size )
{
	if( !mhData ) return NULL;
	StGenInterfaceValue *pIF = (StGenInterfaceValue *)mhData;
	return pIF->Write( iAddress, pBuffer, size );
}


