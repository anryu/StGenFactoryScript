#include "StdAfx.h"
#include "StDeviceBase.h"


StDeviceBase::StDeviceBase(void)
	: m_Listener( NULL )
	, m_OutBuffer( NULL )
{
	m_hEvent = CreateEvent(NULL, FALSE/*FALSE:自動リセット*/, FALSE/*初期値 FALSE:非シグナル状態*/, _T("GETIMAGE"));
	//▼1.0.0.1036
	//mDeviceModelName = "";
	m_hWaitEvent = CreateEvent(NULL, FALSE/*FALSE:自動リセット*/, FALSE/*初期値 FALSE:非シグナル状態*/, _T("GETIMAGE"));
	Initialize();
	//▲1.0.0.1036
}

StDeviceBase::StDeviceBase(LPCTSTR szClassName, LPCTSTR szVarName)
	: m_ClassName(szClassName)
	, m_VarName(szVarName)
	, m_Listener( NULL )
	, m_OutBuffer( NULL )
{
	m_hEvent = CreateEvent(NULL, FALSE/*FALSE:自動リセット*/, FALSE/*初期値 FALSE:非シグナル状態*/, _T("GETIMAGE"));
	//▼1.0.0.1036
	//mDeviceModelName = "";
	m_hWaitEvent = CreateEvent(NULL, FALSE/*FALSE:自動リセット*/, FALSE/*初期値 FALSE:非シグナル状態*/, _T("GETIMAGE"));
	Initialize();
	//▲1.0.0.1036
}

// =============================================================================
StDeviceBase::~StDeviceBase(void)
{
	if( m_hEvent)
		CloseHandle(m_hEvent);
	//▼1.0.0.1036
	if( m_hWaitEvent)
		CloseHandle(m_hWaitEvent);
	//▲1.0.0.1036
}

//▼1.0.0.1036
// =============================================================================
void StDeviceBase::Initialize(void)
{
	mDeviceModelName = "";
	m_nWaitFrame = 0;

	//▼1.0.0.1062
	m_bVirturalConnect = FALSE;
	m_bVirturalStart = FALSE;

	m_nScriptCheckMode = 0;
	//▲1.0.0.1062

	//▼1.0.0.1072d
#ifdef GENCOMP_OLD
#else
	m_nCheckGenICamControlMode = 0;			//0:Checkしない 1:Checkする 
	m_dblCheckGenICamControlValue = 0;
	m_szCheckGenICamControlValue = _T("");
	m_nCheckGenICamControlValueMode = -1;	//0: 数値 1:文字列
#endif
	//▲1.0.0.1072d
}
//▲1.0.0.1036


// =============================================================================
void StDeviceBase::OnStBufferCallback(StBuffer *aBuffer)
{
	//▼1.0.0.1036
	if( m_nWaitFrame==1 )
		SetEvent(m_hWaitEvent);
	if( m_nWaitFrame>0 ) m_nWaitFrame--;
	//▲1.0.0.1036

	if( m_OutBuffer )
	{
		//aBuffer -> m_OutBufferにコピー
		//▼1.0.0.1047
		//m_OutBuffer->CreateCopy(*aBuffer);
		StResult aResult = m_OutBuffer->CreateCopy(*aBuffer);
		if( !aResult.IsOK() )
		{
			if( m_OutBuffer->GetAllocSize() == aBuffer->GetAllocSize()
				&& m_OutBuffer->GetWidth() == aBuffer->GetWidth()
				//&& m_OutBuffer->GetHeight() == aBuffer->GetHeight()
				&& m_OutBuffer->GetPixelType() == aBuffer->GetPixelType() )
			{
				memcpy( (char *)m_OutBuffer->GetDataPointer(), (char *)aBuffer->GetDataPointer(), m_OutBuffer->GetAllocSize() );
			}
		}
		//▲1.0.0.1047

		//▼1.0.0.1046
		if( m_OutBuffer->GetHeight()==0 )
		{
			m_OutBuffer->SetHeight(aBuffer->GetDeliveredImageHeight());
		}
		//▲1.0.0.1046


//TRACE(TEXT("@@@aBuffer->GetHeight(%d)\n"),aBuffer->GetHeight());
//TRACE(TEXT("@@@m_OutBuffer->GetWidth(%d) GetHeight(%d)\n"),m_OutBuffer->GetWidth(),m_OutBuffer->GetHeight());
//TRACE(TEXT("@@@aBuffer->GetLinePitch(%d)\n"),aBuffer->GetLinePitch());
//TRACE(TEXT("@@@m_OutBuffer->GetLinePitch(%d)\n"),m_OutBuffer->GetLinePitch());
		SetEvent(m_hEvent);//コピーが終わったのでシグナル状態にする
		m_OutBuffer = NULL;
	}
	if( m_Listener )
	{
		m_Listener->Invoke(aBuffer,m_pUser);
	}

}


// =============================================================================
BOOL StDeviceBase::GetStBuffer(StBuffer *aOutBuffer)
{
//TRACE(TEXT("@@@m_OutBuffer(0x%08X)\n"),m_OutBuffer);
	//必要ある？？？？
//	do
//	{
//		if( !m_OutBuffer )
//			break;
//		else
//		{
//TRACE(TEXT("@@@m_OutBuffer(0x%x)\n"),m_OutBuffer);
//Sleep(1);
//		}
//	}while(1);

	if( m_OutBuffer )
		return FALSE;

	m_OutBuffer = aOutBuffer;
	ResetEvent( m_hEvent );	//非シグナル状態にする
	DWORD dwObject = ::WaitForSingleObject( m_hEvent, 5000 );	//シグナル状態になったら抜ける
	//抜けたら自動で非シグナル状態になっている
	m_OutBuffer = NULL;	//非正常の場合NULLになっていないため
	BOOL bReval=FALSE;
	if( dwObject==WAIT_OBJECT_0 )
	{
		bReval = TRUE;
	}
	return bReval;
}

//▼1.0.0.1036
BOOL StDeviceBase::WaitFrame(INT nWaitFrame, INT nTimeout)
{
	BOOL bReval=FALSE;
	m_nWaitFrame = nWaitFrame;
	ResetEvent( m_hWaitEvent );	//非シグナル状態にする
	DWORD dwObject = ::WaitForSingleObject( m_hWaitEvent, nTimeout );	//シグナル状態になったら抜ける
	m_nWaitFrame = 0;
	if( dwObject==WAIT_OBJECT_0 )
	{
		bReval = TRUE;
	}
	return bReval;
}
//▲1.0.0.1036
