#include "StdAfx.h"
#include "StInterfaceGenICam.h"
#include "../CommonHeader/StGenTL.h"
#include "../CommonHeader/StGenInterface.h"
#include "StDeviceGenICam.h"

StInterfaceGenICam::StInterfaceGenICam(void)
	: m_InterfaceID(_T(""))
	//Å•1.0.0.1054
	, m_Device(NULL)
	//Å£1.0.0.1054
{
	Initialize();
}

//Å•1.0.0.1054
StInterfaceGenICam::StInterfaceGenICam(LPCTSTR szClassName, LPCTSTR szVarName, void * aDevice)
	:StCommonBase( szClassName, szVarName )
	, m_Device(aDevice)
	, m_InterfaceID(_T(""))
{
	Initialize();
}
//Å£1.0.0.1054

StInterfaceGenICam::StInterfaceGenICam(LPCTSTR szClassName, LPCTSTR szVarName, LPCTSTR aInterfaceID)
	:StCommonBase( szClassName, szVarName )
	, m_InterfaceID(aInterfaceID)
	//Å•1.0.0.1054
	, m_Device(NULL)
	//Å£1.0.0.1054
{
	Initialize();
	Find(m_InterfaceID.GetUnicode());
}

StInterfaceGenICam::~StInterfaceGenICam(void)
{
	if( mpDeviceManager ) delete mpDeviceManager;
	mpDeviceManager = NULL;
	m_Interface = NULL;
}

BOOL StInterfaceGenICam::Initialize(void)
{
	mpDeviceManager = NULL;
	return TRUE;
}


BOOL StInterfaceGenICam::Find( LPCTSTR aInterfaceID )
{
	if( mpDeviceManager ) delete mpDeviceManager;
	m_Interface = NULL;
	mpDeviceManager = new StGenDeviceManager;
	//StGenInterface *pTargetGenInterface = NULL;
	if( mpDeviceManager )
	{
		//Interfaceåüçı--------------------------------------------------------------------------------------
		do
		{
			mpDeviceManager->UpdateList();
			int iGenTLSize = mpDeviceManager->GetGenTLSize();

			for( int i=0; i<iGenTLSize && m_Interface==NULL; i++ )
			{
				StGenTL *pGenTL = (StGenTL *)mpDeviceManager->GetGenTL(i);
				StString aName = pGenTL->GetName();
				aName = pGenTL->GetDisplayName();

				StInt32 iNumInterface = pGenTL->GetNumInterfaces();
				for( int j=0; j<iNumInterface && m_Interface==NULL; j++ )
				{
					StGenInterface *pGenIF = pGenTL->GetGenInterface(j);
					//ëOàÍívå^ÇÃî‰ärÇ…ïœçX
					//if( pGenIF->GetID()==aInterfaceID )
					CString szID = pGenIF->GetID();
					if( szID.Find(aInterfaceID)==0 )
					{
						m_Interface = pGenIF;
					}
				}
			}
		}while(0);
		if( !m_Interface )
		{
			delete mpDeviceManager;
			mpDeviceManager = NULL;
		}
	}
	return m_Interface!=NULL;
}





BOOL StInterfaceGenICam::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="SetID" )
	{
		m_InterfaceID = szArgument;
		bReval = Find(m_InterfaceID.GetUnicode());
	}
	else
	{
		bReval = SetGenControl( szFunction, szArgument );
	}
	return bReval;

}

BOOL StInterfaceGenICam::SetInterface( void )
{
	BOOL bReval = FALSE;
	if( m_Device )
	{
		StDeviceBase *aDeviceBase = (StDeviceBase *)m_Device;
		if( aDeviceBase->m_ClassName=="StDeviceGenICam" )
		{
			StDeviceGenICam *aDevice = (StDeviceGenICam *)aDeviceBase;
			if( aDevice->GetDeviceInfo() )
			{
				m_Interface = aDevice->GetDeviceInfo()->GetInterface();
				bReval = TRUE;
			}
		}
	}
	return bReval;
}

BOOL StInterfaceGenICam::SetGenControl( LPCTSTR szFeature, LPCTSTR szData )
{
	BOOL bReval = FALSE;

	//Å•1.0.0.1054
	SetInterface();
	//Å£1.0.0.1054
	if( !m_Interface ) return FALSE;

	bReval = SetGenICamControl( szFeature, szData );
	if( !bReval )
	{
		INT nData = _ttoi(szData);
		bReval = SetGenICamControl( szFeature, nData );
	}
	if( !bReval )
	{
		double dblData = _ttof(szData);
		bReval = SetGenICamControl( szFeature, dblData );
	}
	if( !bReval )
	{
		bool bData = _tcscmp(szData,_T("true"))==0?true:(_tcscmp(szData,_T("false"))==0?false:(bool)_ttoi(szData));
		bReval = SetGenICamControl( szFeature, bData );
	}
	if( !bReval )
	{
		bReval = SetGenICamControl( szFeature );
	}
	return bReval;

}

//--------------------------------------------------------------------
//SetGenICamControl
//--------------------------------------------------------------------
BOOL StInterfaceGenICam::SetGenICamControl( LPCTSTR szFeature, LPCTSTR szData )
{
	BOOL bReval = FALSE;
	StString szString = szData;
	bReval = m_Interface->SetGenStringParameter( szFeature, szString );
	if( !bReval )
		bReval = m_Interface->SetGenEnumParameter( szFeature, szString );

	return bReval;
}

BOOL StInterfaceGenICam::SetGenICamControl( LPCTSTR szFeature, INT nData )
{
	BOOL bReval = FALSE;

	bReval = m_Interface->SetGenIntParameter( szFeature, nData );
	if( !bReval )
		bReval = m_Interface->SetGenEnumParameter( szFeature, nData );

	return bReval;

}

BOOL StInterfaceGenICam::SetGenICamControl( LPCTSTR szFeature, double dblData )
{
	BOOL bReval = FALSE;
	bReval = m_Interface->SetGenFloatParameter( szFeature, dblData );

	return bReval;

}

BOOL StInterfaceGenICam::SetGenICamControl( LPCTSTR szFeature, bool bData )
{
	BOOL bReval = FALSE;
	bReval = m_Interface->SetGenBoolParameter( szFeature, bData );

	return bReval;

}

BOOL StInterfaceGenICam::SetGenICamControl( LPCTSTR szFeature )
{
	BOOL bReval = FALSE;
	bReval = m_Interface->SetGenCommandParameter( szFeature );

	return bReval;

}



//--------------------------------------------------------------------
//GetGenICamControl
//--------------------------------------------------------------------
BOOL StInterfaceGenICam::GetGenICamControl( LPCTSTR szFeature, LPTSTR szData, size_t size )
{
	//Å•1.0.0.1054
	SetInterface();
	if( !m_Interface ) return FALSE;
	//Å£1.0.0.1054

	BOOL bReval = FALSE;
	StString szString;
	//Å•1.0.0.1061
	//bReval = m_Interface->GetGenStringParameter( szFeature, szString );
	//if( !bReval )
	//	bReval = m_Interface->GetGenEnumParameter( szFeature, szString );
	bReval = m_Interface->GetGenStringParameter( szFeature, szString, true, true );
	if( !bReval )
		bReval = m_Interface->GetGenEnumParameter( szFeature, szString, true, true );
	//Å£1.0.0.1061
	if( bReval )
	{
		_tcscpy_s(szData,size,szString.GetUnicode());
	}
	return bReval;

}

BOOL StInterfaceGenICam::GetGenICamControl( LPCTSTR szFeature, INT &nData )
{
	//Å•1.0.0.1054
	SetInterface();
	if( !m_Interface ) return FALSE;
	//Å£1.0.0.1054
	BOOL bReval = FALSE;
	INT nTmp;
	//Å•1.0.0.1061
	//bReval = m_Interface->GetGenIntParameter( szFeature, nTmp );
	//if( !bReval )
	//	bReval = m_Interface->GetGenEnumParameter( szFeature, nTmp );
	bReval = m_Interface->GetGenIntParameter( szFeature, nTmp, true, true );
	if( !bReval )
		bReval = m_Interface->GetGenEnumParameter( szFeature, nTmp, true, true );
	//Å£1.0.0.1061
	if( bReval )
		nData = nTmp;

	return bReval;

}

BOOL StInterfaceGenICam::GetGenICamControl( LPCTSTR szFeature, double &dblData )
{
	//Å•1.0.0.1054
	SetInterface();
	if( !m_Interface ) return FALSE;
	//Å£1.0.0.1054
	BOOL bReval = FALSE;
	double dblTmp;
	//Å•1.0.0.1061
	//bReval = m_Interface->GetGenFloatParameter( szFeature, dblTmp );
	bReval = m_Interface->GetGenFloatParameter( szFeature, dblTmp, true, true );
	//Å£1.0.0.1061
	if( bReval )
		dblData = dblTmp;

	return bReval;

}

BOOL StInterfaceGenICam::GetGenICamControl( LPCTSTR szFeature, bool &bData )
{
	//Å•1.0.0.1054
	SetInterface();
	if( !m_Interface ) return FALSE;
	//Å£1.0.0.1054
	BOOL bReval = FALSE;
	bool bTmp;
	//Å•1.0.0.1061
	//bReval = m_Interface->GetGenBoolParameter( szFeature, bTmp );
	bReval = m_Interface->GetGenBoolParameter( szFeature, bTmp, true, true );
	//Å£1.0.0.1061
	if( bReval )
		bData = bTmp;

	return bReval;

}
