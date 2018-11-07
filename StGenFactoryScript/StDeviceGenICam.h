#pragma once
#include "stdevicebase.h"
#include "CaptureThread.h"
#include "../CommonHeader/StGenDeviceManager.h"
#include "../CommonHeader/StGenDevice.h"
#include "../CommonHeader/StGenStream.h"
//Å•1.0.0.1051
#include "../CommonHeader/StGenInterface.h"
//Å£1.0.0.1051

class StDeviceGenICam :
	public StDeviceBase, public StGenDevice
{
public:
	StDeviceGenICam(void);
	StDeviceGenICam(LPCTSTR szClassName, LPCTSTR szVarName);
	~StDeviceGenICam(void);

	BOOL function( LPCTSTR szFunc, LPCTSTR szArgument );
	void GetLastError(CString &szError);

	//--------------------------------------------------------------------
	//SetGenICamControl
	//--------------------------------------------------------------------
	BOOL SetGenICamControl( LPCTSTR szFeature, LPCTSTR szData );
	BOOL SetGenICamControl( LPCTSTR szFeature, INT nData );
	BOOL SetGenICamControl( LPCTSTR szFeature, double dblData );
	BOOL SetGenICamControl( LPCTSTR szFeature, bool bData );

	//--------------------------------------------------------------------
	//GetGenICamControl
	//--------------------------------------------------------------------
	BOOL GetGenICamControl( LPCTSTR szFeature, LPTSTR szData, size_t size );
	BOOL GetGenICamControl( LPCTSTR szFeature, INT &nData );
	BOOL GetGenICamControl( LPCTSTR szFeature, double &dblData );
	BOOL GetGenICamControl( LPCTSTR szFeature, bool &bData );

	//--------------------------------------------------------------------
	//Control
	//--------------------------------------------------------------------
	BOOL WriteMain( DWORD dwAddress, char * nValue, size_t nSize, BOOL bString=FALSE );
	BOOL ReadMain( DWORD dwAddress, char * nValue, size_t nSize, BOOL bString=FALSE );


	//Å•1.0.0.1046
	INT GetEndian(void){return m_nEndian;}

	INT GetLastReadCheckData(void){return m_nLastReadCheckData;}
	BOOL ReadCheck( DWORD dwAddress=0, PBOOL bJudge=FALSE );
	BOOL ReadCheck( LPCTSTR szAddress=NULL, PBOOL bJudge=FALSE );
	BOOL GetReadCheckJudge(void){return m_bReadCheckJudge;}

	BOOL GetFunction( LPCTSTR szFunc, LPCTSTR szArgument, LPTSTR szData, size_t size );
	BOOL GetFunction( LPCTSTR szFunc, LPCTSTR szArgument, INT &nData );
	BOOL GetFunction( LPCTSTR szFunc, LPCTSTR szArgument, double &dblData );
	BOOL IsReadCheckData(void){return !m_szReadCheckData.IsEmpty();}
	//Å£1.0.0.1046

	BOOL IsOpened(void){return IsOpen();}
	INT GetDisplayHeight(void){return m_nDisplayHeight;}

	//Å•1.0.0.1051
	StGenDeviceInfo *GetDeviceInfo(void){return mpGenDeviceInfo;}
	//StGenInterface *GetInterface(void);
	BOOL SetFunction( LPCTSTR szFunc, LPCTSTR szArgument, LPCTSTR szData );



	//Å£1.0.0.1051

public:
	void OnImageCallback(StBuffer *aBuffer);

private:	
	void initialize(void);

	BOOL CustomOpen( LPCTSTR szModelName );
	BOOL Open( StGenDeviceInfo *pGenDeviceInfo );
	BOOL Close( void );
	BOOL Start( void );
	BOOL Stop( void );

	StGenDeviceInfo *mpGenDeviceInfo;
	StGenDeviceManager *mpDeviceManager;
	StGenDeviceInfo * DiscoveryDeviceInfo( StGenDeviceManager &aDeviceManager, LPCTSTR szModelName, INT nTimeout);

	StResult CreateStreams(void);
	StResult DeleteStreams(void);
	StResult StartAcquisition(int iBufferCount);
	StResult StopAcquisition(void);

	BOOL IsStarted(void){return m_bStarted;}
	BOOL IsStreamCreated(){return m_bStreamCreated;}

	BOOL m_bStreamCreated;
	BOOL m_bStarted;
	INT m_nBufferCount;
	INT m_nDisplayHeight;
	CString m_szLastError;
	//Å•1.0.0.1046
	INT m_nEndian;
	void SetEndian(INT nEndian){m_nEndian = nEndian;}

	//INT m_nReadCheckData;
	CString m_szReadCheckData;
	DWORD m_dwReadCheckAddress;
	INT m_nLastReadCheckData;
	BOOL m_bReadCheckJudge;
	//Å£1.0.0.1046
	//Å•1.0.0.1058
	INT m_nWriteNoCheck;
	//Å£1.0.0.1058
	BOOL StartStreaming(void);
	void StopStreaming(void);

	BOOL WriteMessage(LPCTSTR szData);
	BOOL SetGenControl(LPCTSTR szFunction, LPCTSTR szArgument);

	CaptureThread *mCaptureThread;

};

