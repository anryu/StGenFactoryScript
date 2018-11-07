#include "StdAfx.h"
#include "StDeviceGenICam.h"
#include "../CommonHeader/StTimed.h"
#include "../CommonHeader/StGenTL.h"
#include "../CommonHeader/StGenInterface.h"


StDeviceGenICam::StDeviceGenICam(void)
{
	mpDeviceManager = NULL;
	initialize();
}

StDeviceGenICam::StDeviceGenICam(LPCTSTR szClassName, LPCTSTR szVarName)
	: StDeviceBase( szClassName, szVarName)
{
	mpDeviceManager = NULL;
	initialize();
}

StDeviceGenICam::~StDeviceGenICam(void)
{
	//▼1.0.0.1046
	if( IsStarted() )
	{
		Stop();
	}

	if( IsOpened() )
	{
		Close();
	}

	UnregisterCallback();
	if( mpDeviceManager )
	{
		delete mpDeviceManager;
		mpDeviceManager = NULL;
	}
	//▲1.0.0.1046

}

void StDeviceGenICam::initialize(void)
{
	m_bStreamCreated = FALSE;
	m_bStarted = FALSE;
	m_nBufferCount = 10;
	m_szLastError = _T("");
	m_nDisplayHeight = 0;

	mCaptureThread = NULL;
	//▼1.0.0.1046
	m_nEndian = -1;
	m_szReadCheckData = "";

	//mpDeviceManager = NULL;
	mCaptureThread = NULL;
	//▲1.0.0.1046
	//▼1.0.0.1058
	m_nWriteNoCheck = 0;
	//▲1.0.0.1058
}

void StDeviceGenICam::GetLastError(CString &szError)
{
	szError = m_szLastError;
}

BOOL StDeviceGenICam::function( LPCTSTR szFunc, LPCTSTR szArgument )
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="Open" )
	{
		bReval = CustomOpen(szArgument);
		if( !bReval )
			m_szLastError = _T("GenICamオープンエラー.");
	}
	else
	if( szFunction=="Close" )
	{
		bReval = Close();
		if( !bReval )
			m_szLastError = _T("GenICamクローズエラー");
	}
	else
	if( szFunction=="Start" )
	{
		bReval = Start();
		if( !bReval )
			m_szLastError = _T("GenICamスタートエラー");
	}
	else
	if( szFunction=="Stop" )
	{
		bReval = Stop();
		if( !bReval )
			m_szLastError = _T("GenICamストップエラー");
	}
	else
	if( szFunction=="SetBufferCount" || szFunction=="BufferCount" )
	{
		m_nBufferCount = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetDisplayHeight" || szFunction=="DisplayHeight" )
	{
		m_nDisplayHeight = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetEndian" || szFunction=="Endian" )
	{
		INT nEndian;
		if( _tcscmp(szArgument,_T("Little"))==0 )
			nEndian = 0;
		else
		if( _tcscmp(szArgument,_T("Big"))==0 )
			nEndian = 1;
		else
		if( _tcscmp(szArgument,_T("Auto"))==0 )
			nEndian = -1;
		else
			nEndian = _ttoi(szArgument);
		SetEndian(nEndian);
		bReval = TRUE;
	}
	else
	if( szFunction=="Write" )
	{
		bReval = WriteMessage(szArgument);
		//▼1.0.0.1060
		if( !bReval )
			m_szLastError = _T("GenICam Write エラー");
		//▲1.0.0.1058
	}
	//▼1.0.0.1058
	else
	if( szFunction=="WriteNoCheck" )
	{
		m_nWriteNoCheck = _ttoi(szArgument);
		bReval = TRUE;
	}
	//▲1.0.0.1058

	//▼1.0.0.1046
	else
	if( szFunction=="SetReadCheckData" || szFunction=="ReadCheckData" )
	{
		//m_nReadCheckData = _tcstol(szArgument,NULL,0);
		m_szReadCheckData = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetReadCheckAddress" || szFunction=="ReadCheckAddress" )
	{
		m_dwReadCheckAddress = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	//▲1.0.0.1046
	//▼1.0.0.1072d
#ifdef GENCOMP_OLD
#else
	else
	if( szFunction=="SetCheckGenICamControlMode" || szFunction=="CheckGenICamControlMode" )
	{
		m_nCheckGenICamControlMode = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetCheckGenICamControlValue" || szFunction=="CheckGenICamControlValue" )
	{
		CString szArg(szArgument);
		if( szArg.Left(1).Compare(_T("\"")) )
		{
			//文字列
			szArg = szArg.Mid(1);
			if( szArg.Right(1)==_T("\"") )
				szArg = szArg.Left(szArg.GetLength()-1);
			m_nCheckGenICamControlValueMode = 1;
			m_szCheckGenICamControlValue = (LPCTSTR)szArg;
		}
		else
		{
			//数値
			m_nCheckGenICamControlValueMode = 0;
			m_dblCheckGenICamControlValue = _ttof(szArg);
		}
		bReval = TRUE;
	}
#endif
	//▲1.0.0.1072d
	else
	{
		bReval = SetGenControl( szFunction, szArgument );
	}

	return bReval;


}

BOOL StDeviceGenICam::CustomOpen( LPCTSTR szModelName )
{
	BOOL bReval = FALSE;
	StGenDeviceInfo *pTargetGenDeviceInfo = NULL;



	if( mpDeviceManager ) delete mpDeviceManager;
	mpDeviceManager = new StGenDeviceManager;
	//mpGenInterface = DiscoveryInterface( *mpDeviceManager );
	if( mpDeviceManager )
	{
		//カメラ検索--------------------------------------------------------------------------------------

		for( int i=0; i<3; i++ )
		{
			//if( !mpDeviceManager )
			//{
			//	mpDeviceManager = new StGenDeviceManager;
			//	mpGenInterface = DiscoveryInterface( *mpDeviceManager );


			//}
			//Euresysボードではなぜか１回目失敗する。(電源OFF状態からONにした時）
			//StString aID = mpGenInterface->GetID();

			pTargetGenDeviceInfo = DiscoveryDeviceInfo( *mpDeviceManager, szModelName, 10000 );
			if( pTargetGenDeviceInfo==NULL )
			{
				if( mpDeviceManager ) delete mpDeviceManager;
				mpDeviceManager = NULL;

				//▼1.0.0.1046
				mpDeviceManager = new StGenDeviceManager;
				if( !mpDeviceManager ) break;
				//▲1.0.0.1046

				//mpGenInterface = NULL;
			}
			else break;
		}
		if( pTargetGenDeviceInfo )
		{
			bReval = Open(pTargetGenDeviceInfo);
			//▼1.0.0.1046
			if( bReval && (m_nEndian<0 || m_nEndian>=2) )
			{
				INT nEndian = -1;
				TCHAR szText[128];
				BOOL bEndian = GetGenICamControl(_T("DeviceRegistersEndianness"),szText,_countof(szText));
				if( bEndian )
				{
					if( _tcscmp(szText,_T("Little"))==0 )
						nEndian = 0;
					else
					if( _tcscmp(szText,_T("Big"))==0 )
						nEndian = 1;
				}
				if( nEndian<0 )
				{
					bEndian = GetGenICamControl(_T("DeviceTLType"),szText,_countof(szText));
					if( bEndian )
					{
						if( _tcscmp(szText,_T("USB3Vision"))==0 )
							nEndian = 0;
						else
						if( _tcscmp(szText,_T("CoaXPress"))==0 )
							nEndian = 1;
						else
						if( _tcscmp(szText,_T("GigEVision"))==0 )
							nEndian = 1;
					}
				}
				if( nEndian==0 || nEndian==1 )
					m_nEndian = nEndian;
			}
			//▲1.0.0.1046
		}
		//▼1.0.0.1046
		else
		{
			if( mpDeviceManager ) delete mpDeviceManager;
			mpDeviceManager = NULL;
		}
		//▲1.0.0.1046

	}
	return bReval;
}

BOOL StDeviceGenICam::Open(StGenDeviceInfo *pGenDeviceInfo)
{
	StResult aResult = DevOpen(pGenDeviceInfo);
	if( aResult.IsOK() )
	{
		mpGenDeviceInfo = pGenDeviceInfo;
	}
	return aResult.IsOK();
}
BOOL StDeviceGenICam::Close(void)
{
	StResult aResult = DevClose();
	if( aResult.IsOK() )
	{
		mpGenDeviceInfo = NULL;
	}

	if( mpDeviceManager )
	{
		delete mpDeviceManager;
		mpDeviceManager = NULL;
	}

	return aResult.IsOK();
}

StGenDeviceInfo * StDeviceGenICam::DiscoveryDeviceInfo( StGenDeviceManager &aDeviceManager, LPCTSTR szModelName, INT nTimeout )
{

	StString aCameraTypeName = szModelName;

	StGenDeviceInfo *pTargetGenDeviceInfo=NULL;
	int iCameraNum=0;
	StTimed aTime;
	do
	{
		aDeviceManager.UpdateList();
		int iGenTLSize = aDeviceManager.GetGenTLSize();

		for( int i=0; i<iGenTLSize && pTargetGenDeviceInfo==NULL; i++ )
		{
			StGenTL *pGenTL = (StGenTL *)aDeviceManager.GetGenTL(i);
			StString aName = pGenTL->GetName();
			aName = pGenTL->GetDisplayName();

			StInt32 iNumInterface = pGenTL->GetNumInterfaces();
			for( int j=0; j<iNumInterface && pTargetGenDeviceInfo==NULL; j++ )
			{
				StGenInterface *pGenIF = pGenTL->GetGenInterface(j);
		
				StInt32 iNumDeviceInfo = pGenIF->GetNumDeviceInfo();
				for( int k=0; k<iNumDeviceInfo && pTargetGenDeviceInfo==NULL; k++ )
				{
					StGenDeviceInfo *pGenDeviceInfo = pGenIF->GetGenDeviceInfo(k);
					//StString cName = pGenDeviceInfo->GetDisplayName();
					//StString dName = pGenDeviceInfo->GetVendor();
					StString modelName = pGenDeviceInfo->GetModel();

					//Euresys CoaXのボードが挿さっているとカメラが接続されていなくても
					//Deviceと認識される。その場合modelNameが""となっている。
					if( modelName.GetLength()>0 )
					{
						if( aCameraTypeName.GetLength()==0 || modelName==aCameraTypeName )
						{
							pTargetGenDeviceInfo=pGenDeviceInfo;
							//if( aInterface ) *aInterface = pGenIF;
							iCameraNum++;
						}
					}
				}
			}
		}
		if( aTime.GetTime()>nTimeout || pTargetGenDeviceInfo ) break;
		Sleep(1000);
	}while(pTargetGenDeviceInfo==NULL);
	
	return pTargetGenDeviceInfo;
}

StResult StDeviceGenICam::CreateStreams(void)
{
	StResult aResult = CreateStream();
	if( aResult.IsOK() )
	{
		//CreateThread(aDisplayWnd);
		m_bStreamCreated = true;
		//m_Preview = aDisplayWnd;
	}
	return aResult;
}

StResult StDeviceGenICam::DeleteStreams(void)
{
	//DeleteThread();
	DeleteStream();
	m_bStreamCreated = false;
	//m_Preview = NULL;
	return StResult::Code::OK;
}

BOOL StDeviceGenICam::StartStreaming(void)
{
	if( !mCaptureThread )
		mCaptureThread = new CaptureThread( this );

	if( mCaptureThread )
		mCaptureThread->Start();

	return mCaptureThread!=NULL;
}

void StDeviceGenICam::StopStreaming(void)
{
	//スレッド終了させて、StopStreamさせる。
	if( mCaptureThread )
	{
		mCaptureThread->Stop();
		delete mCaptureThread;
		mCaptureThread = NULL;
	}

	if( GetNumStream()>0 )
	{
		StGenStream * aStream = GetGenStream(0);
		aStream->StopStream();
	}
}

void StDeviceGenICam::OnImageCallback(StBuffer *aBuffer)
{
	if( m_Listener )
	{
		//OnStBufferCallback(&m_ConvBuffer);
		OnStBufferCallback(aBuffer);
	}
}

INT GetLinePitch(INT aWidth, StPixelType aPixelType);

StResult StDeviceGenICam::StartAcquisition(int iBufferCount)
{
	StResult lResult = StResult::Code::NOT_STARTACQUISITION;
	if( !IsStreamCreated() ) return lResult;
	if( GetNumStream()==0 )	return lResult;
	bool bReval = false;
	//Buffer作成
	StGenStream * aStream = GetGenStream(0);
	aStream->SetBufferCount(iBufferCount);
	int lPayloadSizeValue = 0;

	if( m_nDisplayHeight>0 )
	{
		INT nWidth = 0;
		GetGenICamControl( _T("Width"), nWidth );
		INT nType = 0;
		GetGenICamControl( _T("PixelFormat"), nType );
		INT LinePitch = GetLinePitch( nWidth, (StPixelType) nType);
		lPayloadSizeValue = m_nDisplayHeight * nWidth;
	}
	else
	{
		bReval = GetPayloadSize( lPayloadSizeValue );
		if( !bReval ) return lResult;
	}
	aStream->SetBufferSize(lPayloadSizeValue);

	lResult = aStream->StartStream();
	if( lResult.IsOK() ){

		//m_lThreadDisplay->Start();
		//m_lThreadDisplay->SetPriority( THREAD_PRIORITY_ABOVE_NORMAL );

		TLParamsLocked(true);
		//lResult = StartAcquisition();
		bReval = SetGenCommandParameter( "AcquisitionStart" );
		if( bReval )
		{
			lResult = StResult::Code::OK;
		}
		else
		{
			TLParamsLocked(false);
		}
	}

	//StartAcquisition
	if( lResult.IsOK() )
	{
		m_bStarted = true;
	}

	return lResult;

}

StResult StDeviceGenICam::StopAcquisition(void)
{
	if( !IsStarted() ) return StResult::Code::OK;

	StResult lResult = StResult::Code::GENERIC_ERROR;

	bool bReval = SetGenCommandParameter( "AcquisitionStop" );

	if( bReval ) 
	{
		TLParamsLocked(false);
		m_bStarted = false;
		//if( GetNumStream()==0 )	return lResult;
		//StGenStream * aStream = GetGenStream(0);

		//Sleep(1500);	//とりあえず？StopStream中に画像データが転送してきてバッファ解放と処理が重なってしまうためエラーが起きる
						//どこで解放処理をおこなうか？

		//lResult = aStream->StopStream();

		lResult = StResult::Code::OK;
	}

	return lResult;

}

BOOL StDeviceGenICam::Start(void)
{
	BOOL bReval = FALSE;

	//Start
	bReval = CreateStreams().IsOK();
	if( bReval )
	{
		bReval = StartStreaming();
	}
	if( bReval )
	{
		bReval = StartAcquisition(m_nBufferCount).IsOK();
	}
	return bReval;
}

BOOL StDeviceGenICam::Stop(void)
{
	BOOL bReval = FALSE;

	//Stop
	bReval = StopAcquisition().IsOK();
	//if( bReval )
	{
//TRACE(TEXT("@@@@@@@@@ StopStreaming IN\n") );
		StopStreaming();
//TRACE(TEXT("@@@@@@@@@ StopStreaming OUT\n") );
		bReval = DeleteStreams().IsOK();
//TRACE(TEXT("@@@@@@@@@ DeleteStreams OUT\n") );
	}
	return bReval;
}

//--------------------------------------------------------------------
//SetGenICamControl
//--------------------------------------------------------------------
BOOL StDeviceGenICam::SetGenICamControl( LPCTSTR szFeature, LPCTSTR szData )
{
	BOOL bReval = FALSE;
	StString szString = szData;
	bReval = SetGenStringParameter( szFeature, szString );
	if( !bReval )
		bReval = SetGenEnumParameter( szFeature, szString );

	return bReval;
}

BOOL StDeviceGenICam::SetGenICamControl( LPCTSTR szFeature, INT nData )
{
	BOOL bReval = FALSE;

	bReval = SetGenIntParameter( szFeature, nData );
	if( !bReval )
		bReval = SetGenEnumParameter( szFeature, nData );

	return bReval;

}

BOOL StDeviceGenICam::SetGenICamControl( LPCTSTR szFeature, double dblData )
{
	BOOL bReval = FALSE;
	bReval = SetGenFloatParameter( szFeature, dblData );

	return bReval;

}

BOOL StDeviceGenICam::SetGenICamControl( LPCTSTR szFeature, bool bData )
{
	BOOL bReval = FALSE;
	bReval = SetGenBoolParameter( szFeature, bData );

	return bReval;

}

BOOL StDeviceGenICam::SetGenControl( LPCTSTR szFeature, LPCTSTR szData )
{
	BOOL bReval = FALSE;
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
	return bReval;

}


//--------------------------------------------------------------------
//GetGenICamControl
//--------------------------------------------------------------------
BOOL StDeviceGenICam::GetGenICamControl( LPCTSTR szFeature, LPTSTR szData, size_t size )
{
	BOOL bReval = FALSE;
	StString szString;
	bReval = GetGenStringParameter( szFeature, szString );
	if( !bReval )
		bReval = GetGenEnumParameter( szFeature, szString );
	if( bReval )
	{
		_tcscpy_s(szData,size,szString.GetUnicode());
	}
	return bReval;

}

BOOL StDeviceGenICam::GetGenICamControl( LPCTSTR szFeature, INT &nData )
{
	BOOL bReval = FALSE;
	INT nTmp;
	bReval = GetGenIntParameter( szFeature, nTmp );
	if( !bReval )
		bReval = GetGenEnumParameter( szFeature, nTmp );
	if( bReval )
		nData = nTmp;

	return bReval;

}

BOOL StDeviceGenICam::GetGenICamControl( LPCTSTR szFeature, double &dblData )
{
	BOOL bReval = FALSE;
	double dblTmp;
	bReval = GetGenFloatParameter( szFeature, dblTmp );
	if( bReval )
		dblData = dblTmp;

	return bReval;

}

BOOL StDeviceGenICam::GetGenICamControl( LPCTSTR szFeature, bool &bData )
{
	BOOL bReval = FALSE;
	bool bTmp;
	bReval = GetGenBoolParameter( szFeature, bTmp );
	if( bReval )
		bData = bTmp;

	return bReval;

}

//--------------------------------------------------------------------
//WriteMain
//--------------------------------------------------------------------
BOOL StDeviceGenICam::WriteMain( DWORD dwAddress, char * pValue, size_t nSize, BOOL bString )
{
	//エンディアン入れ替え
	if( !bString && m_nEndian==1 && (nSize&3)==0 )
	{
		int *pnValue = (int *)pValue;
		for( int i=0; i<nSize/4; i++ )
		{
			*pnValue = htonl(*pnValue);
			pnValue++;
		}
	}
	BOOL bReval = Write( dwAddress, pValue, nSize );

	//念のため戻す
	if( !bString && m_nEndian==1 && (nSize&3)==0 )
	{
		int *pnValue = (int *)pValue;
		for( int i=0; i<nSize/4; i++ )
		{
			*pnValue = htonl(*pnValue);
			pnValue++;
		}
	}
	//▼1.0.0.1058
	if( m_nWriteNoCheck!=0 )
	{
		bReval= TRUE;
		if( m_nWriteNoCheck!=-1 ) m_nWriteNoCheck--;
	}
	//▲1.0.0.1058
	return bReval;

}

//--------------------------------------------------------------------
//ReadMain
//--------------------------------------------------------------------
BOOL StDeviceGenICam::ReadMain( DWORD dwAddress, char * pValue, size_t nSize, BOOL bString )
{
	BOOL bReval = Read( dwAddress, pValue, nSize );
	if( bReval && !bString && m_nEndian==1 && (nSize&3)==0 )
	{
		//BigEndianの為入れ替え
		int *pnValue = (int *)pValue;
		for( int i=0; i<nSize/4; i++ )
		{
			*pnValue = htonl(*pnValue);
			pnValue++;
		}
	}
	return bReval;

}

//--------------------------------------------------------------------
//WriteMessage
//--------------------------------------------------------------------
BOOL StDeviceGenICam::WriteMessage( LPCTSTR szData )
{
	BOOL bReval = FALSE;
	CString szString = szData;
	INT nComma = szString.Find(_T(","));
	if( nComma<0 ) return FALSE;
	TCHAR szText[256];
	_tcscpy_s( szText, _countof(szText), szString.Left(nComma) );
	DWORD dwAddress = (DWORD)_tcstol(szText,NULL,0);
	//▼1.0.0.1054
	//INT nValue = _ttoi(szString.Mid(nComma+1));
	INT nValue = _tcstol(szString.Mid(nComma+1),NULL,0);
	//▲1.0.0.1054
	//if( dwAddress==0xa120 )
	//	Sleep(1);

	bReval = WriteMain( dwAddress, (char *)&nValue, sizeof(nValue) );

	return bReval;

}

//--------------------------------------------------------------------
//ReadCheck
//--------------------------------------------------------------------
BOOL StDeviceGenICam::ReadCheck( DWORD dwAddress, PBOOL bJudge )
{
	if( dwAddress==0 )
	{
		dwAddress = m_dwReadCheckAddress;
	}
	INT nValue;
	BOOL bReval = ReadMain( dwAddress, (char *)&nValue, sizeof(nValue) );
	if( bReval )
	{
		m_nLastReadCheckData = nValue;
		if( !m_szReadCheckData.IsEmpty() )
		{
			INT nReadCheckData = _tcstol(m_szReadCheckData,NULL,0);
			if( nReadCheckData != nValue )
			{
				m_bReadCheckJudge = FALSE;
				m_szLastError = _T("Compare error.");
			}
			else
			{
				m_bReadCheckJudge = TRUE;
			}
			if( bJudge ) *bJudge = m_bReadCheckJudge;
		}
	}
	else
	{
		m_szLastError = _T("Control error.");
	}
	return bReval;

}

BOOL StDeviceGenICam::ReadCheck( LPCTSTR szAddress, PBOOL bJudge )
{
	DWORD dwAddress=0;
	if( szAddress )
	{
		dwAddress = _tcstol(szAddress,NULL,0);
	}
	return ReadCheck( dwAddress, bJudge );
}

//▼1.0.0.1046
BOOL StDeviceGenICam::GetFunction( LPCTSTR szFunc, LPCTSTR szArgument, LPTSTR szData, size_t size )
{
	BOOL bReval = FALSE;
	CString szFeature = szFunc;
	szFeature.Trim();
	CString szArg = szArgument;
	szArg.Trim();
	if( szFeature==_T("GetReadData") )
	{

	}
	else
	if( szFeature==_T("DataStream") )
	{
		StGenStream *aStream = GetGenStream(0);
		StResult aResult = aStream->GetGenICamControl(szArgument,szData,size);
		bReval = aResult.IsOK();
	}
	return bReval;

}

BOOL StDeviceGenICam::GetFunction( LPCTSTR szFunc, LPCTSTR szArgument, INT &nData )
{
	BOOL bReval = FALSE;
	CString szFeature = szFunc;
	szFeature.Trim();
	CString szArg = szArgument;
	szArg.Trim();
	if( szFeature==_T("GetReadData") )
	{
		nData = m_nLastReadCheckData;
		bReval = TRUE;

	}
	else
	if( szFeature==_T("DataStream") )
	{
		StGenStream *aStream = GetGenStream(0);
		StResult aResult = aStream->GetGenICamControl(szArgument,nData);
		bReval = aResult.IsOK();
	}
	return bReval;

}

BOOL StDeviceGenICam::GetFunction( LPCTSTR szFunc, LPCTSTR szArgument, double &dblData )
{
	BOOL bReval = FALSE;
	CString szFeature = szFunc;
	szFeature.Trim();
	CString szArg = szArgument;
	szArg.Trim();
	if( szFeature==_T("GetReadData") )
	{
		dblData = (double)m_nLastReadCheckData;
		bReval = TRUE;
	}

	else
	if( szFeature==_T("DataStream") )
	{
		StGenStream *aStream = GetGenStream(0);
		if( aStream )
		{
			StResult aResult = aStream->GetGenICamControl(szArgument,dblData);
			//HANDLE hPort = aStream->GetHandle();
			//StString aPortName = aStream->GetPortName();
			//StString aID = aStream->GetID();
			bReval = aResult.IsOK();
		}
	}

	return bReval;

}
//▲1.0.0.1046
//▼1.0.0.1051
BOOL StDeviceGenICam::SetFunction( LPCTSTR szFunc, LPCTSTR szArgument, LPCTSTR szData )
{
	BOOL bReval = FALSE;
	CString szFeature = szFunc;
	szFeature.Trim();
	CString szArg = szArgument;
	szArg.Trim();


	if( szFeature==_T("DataStream") )
	{
		StGenStream *aStream = GetGenStream(0);
		if( aStream )
		{
			StResult aResult = aStream->SetGenICamControl(szArgument,szData);
			bReval = aResult.IsOK();
		}
	}
	return bReval;
}
//▲1.0.0.1051
