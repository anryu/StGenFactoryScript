#pragma once
//#include <MultiCamCpp.h>	//C++を使用した場合、なぜか通信がうまくいかない。COM設定もできない
#include <multicam.h>
#include "StDeviceBase.h"
//#include "multicam.h"
#include "StSerialComm.h"

//using namespace Euresys::MultiCam;

class StDeviceMultiCam : public StDeviceBase
{
public:
	StDeviceMultiCam(void);
	StDeviceMultiCam(LPCTSTR szClassName, LPCTSTR szVarName);
	~StDeviceMultiCam(void);


	BOOL function( LPCTSTR szFunc, LPCTSTR szArgument );
	//void OnImageCallback(Channel &Ch, SignalInfo &Info);
	void OnImageCallback( PMCSIGNALINFO Info);
	//void OnAcqFailureCallback(Channel &Ch, SignalInfo &Info);
	void GetLastError(CString &szError);

	//BOOL IsOpened(){return m_pChannel!=NULL;}
	BOOL IsOpened(){return m_hChannel!=NULL;}
	void GetPortName(CString &szComPortName){szComPortName=m_szComPortName;}
	//▼1.0.0.1015
	void SetComPortName(LPCTSTR szComPortName){m_szComPortName = szComPortName;}
	//▲1.0.0.1015

	INT GetWidth(){return m_nSizeX;}
	INT GetHeight(){return m_nSizeY;}
	bool IsStarted(void);

	//▼1.0.0.1015
	void SetSerialComm(StSerialComm *pSerialComm);
	StSerialComm * GetSerialComm(void);
	//void SetSerialComm(StSerialComm *pSerialComm){m_pSerialComm = pSerialComm;}
	//▲1.0.0.1015

	//▼1.0.0.1020
	void SetControlRetry(void *pHandle){m_pControlRetry = pHandle;}
	void * GetControlRetry(void){return m_pControlRetry;}
	//▲1.0.0.1020
	//▼1.0.0.1033
	BOOL GetFramerate( double *pdblData );
	//▲1.0.0.1033

private:
	void DriverOpen(void);
	void DriverClose(void);
	void BoardOpen(void);

	BOOL Open( void );
	BOOL Close( void );
	BOOL Start( void );
	BOOL Stop( void );

	BOOL SetBoardID( LPCTSTR szModelName );
	BOOL SetBoardID( INT nBoardID );
	BOOL SetBoardTopology( LPCTSTR szBoardTopologyName );
	BOOL SetBoardTopology( INT nBoardTopology );
	BOOL SetConnect( LPCTSTR szConnect );

	StSerialComm *m_pSerialComm;

	void initialize(void);
    //Channel *m_pChannel;	//未使用にした
	MCHANDLE m_hChannel;


	//BOOL SerialControl( LPCTSTR szData, PBYTE pbyteReadData=NULL, size_t *pReadSize=NULL );
	//BOOL SerialControl( PBYTE pbyteSendData, size_t sendSize, PBYTE pbyteReadData, size_t *pReadSize );

	StPixelType GetPixelType(void);
	BOOL ConvertToStBufferFromBuffer( StBuffer *aOutBuffer, unsigned int unBuffer, int nWidth, int nHeight, int nPitch, int nSize );
	CString m_szCameraFile;
	CString m_szCameraFilePath;
	INT m_nBoardID;
	INT m_nBoardTopology;
	CString m_szConnect;
	CString m_szLastError;

	//
	INT m_nSizeX;
	INT m_nSizeY;
	INT m_nBufferPitch;
	INT m_nBufferSize;
	UINT m_unImageBuffer;
	CString m_szComPortName;

	INT64 m_nColorFormat;
	INT64 m_nSpectrum;
	INT64 m_nColorRegistration;

	//▼1.0.0.1020
	void * m_pControlRetry;
	//▲1.0.0.1020


};

