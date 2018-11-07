#pragma once
//▼1.0.0.1017
#include "StControl.h"
//▲1.0.0.1017
#include "rs232c_dsp.h"
#include "../CommonHeader/StString.h"

class StSerialComm :

	//▼1.0.0.1020
	//継承を逆にした。。vecStControlに代入したときにアドレスが変わってしまうから
	//
	//▼1.0.0.1017
	public StControl, 
	//▲1.0.0.1017
	public CRS232C_DSP
	//public CRS232C_DSP,
	//public StControl
	//▲1.0.0.1020

{
public:
	StSerialComm(void);
	StSerialComm( LPCTSTR szClassName, LPCTSTR szVarName, LPCTSTR szPort );
	~StSerialComm(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	BOOL SendData( LPCTSTR szData );
	BOOL SetRecieveData( LPCTSTR szData );
	BOOL GetRecieveResultData( StString &szData );
	//▼1.0.0.1022
	BOOL GetRecieveResultValue( INT &szData );
	//▲1.0.0.1022


	//void GetArgument(CString &szArgument){szArgument = m_szArgument;}

	//▼1.0.0.1017
	//StString m_ClassName;	//StSerialCommのみ
	//StString m_VarName;
	//▲1.0.0.1017

	//▼1.0.0.1021
	//BOOL SendBin( const PBYTE byteSend, size_t sendSize, PBYTE byteRcv, size_t *rcvSize );
	BOOL SendBin( const PBYTE byteSend, size_t sendSize, PBYTE byteRcv, size_t *rcvSize, const PBYTE pbyteRevEndData=NULL );
	//▲1.0.0.1021	
	BOOL SendText( LPCTSTR szText, LPTSTR szRcvText=NULL, size_t rcvBufferSize=0 );
	//▼1.0.0.1015
	//▼1.0.0.1021
	//BOOL SendText( LPCTSTR szText, PBYTE szRcvBin=NULL, size_t rcvBufferSize=0 );
	BOOL SendText( LPCTSTR szText, PBYTE szRcvBin=NULL, size_t rcvBufferSize=0, const PBYTE pbyteRevEndData=NULL );
	//▲1.0.0.1021
	//▲1.0.0.1015

	//▼1.0.0.1022
	BOOL SendText( const char * szText, PBYTE szRcvBin=NULL, size_t rcvBufferSize=0, const PBYTE pbyteRevEndData=NULL );
	//▲1.0.0.1022

	//▼1.0.0.1015
	BOOL WriteBitData( BYTE byteAddr, BYTE byteDevPage, INT istbit, INT ilenbit, WORD dwData );
	BOOL AutoBaudRate( LPCTSTR szData );
	//▲1.0.0.1015

	//▼1.0.0.1047
	BOOL GetControl(void){return m_bControl;}
	BOOL GetCompareCheck(void){return m_bCompareCheck;}
	//▲1.0.0.1047

private:

	void Initialize(void);

	//▼1.0.0.1015
	//▼1.0.0.1021
	//BOOL SendBin2( const PBYTE pbyteSend, size_t sendSize, PBYTE pbyteRcv, size_t *rcvSize );
	BOOL SendBin2( const PBYTE pbyteSend, size_t sendSize, PBYTE pbyteRcv, size_t *rcvSize, const PBYTE pbyteRevEndData=NULL );
	//▲1.0.0.1021
	//▲1.0.0.1015

	StString m_szRecieveCmpData;
	StString m_szLastError;

	//結果保存
	StString m_szRecieveResultData;

	//カメラリンク用　eBUSでは使用しない----------------
	CString m_szSendSTX;
	CString m_szSendETX;
	CString m_szRecieveSTX;
	CString m_szRecieveETX;
	void SetSendSTX(LPCTSTR szData){m_szSendSTX = szData;}
	void SetSendETX(LPCTSTR szData){m_szSendETX = szData;}
	void SetRecieveSTX(LPCTSTR szData){m_szRecieveSTX = szData;}
	void SetRecieveETX(LPCTSTR szData){m_szRecieveETX = szData;}
	//▼1.0.0.1015
	CString m_szRcvReplaceSpace;
	void SetRcvReplaceSpace(LPCTSTR szData){m_szRcvReplaceSpace = szData;}
	//▲1.0.0.1015

	//▼1.0.0.1015
	INT m_nBaudRate; 

	INT m_nDelayTime; 
	//▲1.0.0.1015

	//▼1.0.0.1048
	CString m_szParity;		//Default:NONE POE通信ではEVEN その他ODD
	double m_dblStopBits;		//Default:1  POE通信では2 
	INT m_nByteSize;		//Default:8  POE通信では7 
	//▲1.0.0.1048

	BOOL TestData( void );

	//▼1.0.0.1022
	INT m_nRecieveResultValue;
	//▲1.0.0.1022

	//▼1.0.0.1043
	INT m_nRecieveDelayTime;
	//▲1.0.0.1043


	//▼1.0.0.1047
	BOOL m_bControl;
	BOOL m_bCompareCheck;
	//▲1.0.0.1047



};

