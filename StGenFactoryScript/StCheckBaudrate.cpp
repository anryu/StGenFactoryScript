#include "StdAfx.h"
#include "StCheckBaudrate.h"
#include "StInspectMain.h"


StCheckBaudrate::StCheckBaudrate(void)
{
	Initialize();
}

StCheckBaudrate::StCheckBaudrate(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase)
	:StCheckBase( szClassName, szVarName, aDeviceBase )
{
	Initialize();
}


StCheckBaudrate::~StCheckBaudrate(void)
{
}

void StCheckBaudrate::Initialize(void)
{

}

//AutoBaudrate
BOOL StCheckBaudrate::Execute(void)
{
	BOOL bReval=FALSE;
	StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
	if( !aDevice->IsOpened() ) return FALSE;
	StSerialComm *aSerialComm = aDevice->GetSerialComm();
	if( !aSerialComm ) return FALSE;

	//aSerialComm->function(_T("Send"), );


	return bReval;
}
BOOL StCheckBaudrate::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval=FALSE;


	return bReval;
}