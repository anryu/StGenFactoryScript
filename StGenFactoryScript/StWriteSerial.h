#pragma once
#include "stcheckbase.h"
class StWriteSerial : public StCheckBase
{
public:
	StWriteSerial(void);
	StWriteSerial(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void* pParent=NULL, void *aDialog=NULL);
	~StWriteSerial(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);

	void SetSerialMode(INT nSerialMode){m_nSerialMode = nSerialMode;}
	BOOL SetSerialMode(LPCTSTR szSerialMode);
	INT GetSerialMode(void){return m_nSerialMode;}
	void SetMode(INT nMode){m_nMode = nMode;}
	BOOL SetMode(LPCTSTR szMode);
	INT GetMode(void){return m_nMode;}

	void SetSerialNumber(LPCTSTR szSerialNumber){m_szSerialNumber = szSerialNumber;}
	void SetSensorSerialNumber(LPCTSTR szSensorSerialNumber){m_szSensorSerialNumber = szSensorSerialNumber;}

	BOOL Execute(void);

private:
	void Initialize(void);
	//BOOL WriteSerialNumberEach(CString szSerial, INT nSerial);
	BOOL WriteBinary(void *pComm, LPCTSTR szData);

	//▼1.0.0.1057
	//BOOL WriteSerialNumberEach( void *pComm, CString szSerial, INT nSerial );
	BOOL WriteSerialNumberEach( void *pComm, CString szSerial, INT nSerial, INT nQuadBit=0 );
	//▲1.0.0.1057
	BOOL WriteText(void *pComm, LPCTSTR szData);

	INT m_nSerialMode; //D0:カメラシリアル D1:センサーシリアル
	INT m_nMode;       //0:Binary 1:Ascii 2:Takano

	CString m_szSerialNumber;
	CString m_szSensorSerialNumber;

	//Binary用アドレス
	CString m_szDeviceCode;
	CString m_szPage;
	CString m_szYearAddress;
	CString m_szMonthAddress;
	CString m_szSN1Address;
	CString m_szSN2Address;
	CString m_szSN3Address;
	CString m_szSN4Address;
	//▼1.0.0.1057
	CString m_szMonthAddress2;
	CString m_szSN1Address2;
	//▲1.0.0.1057

	//Ascii、Takano用
	CString m_szCommand;

	//▼1.0.0.1044
	//GenICam用アドレス
	DWORD m_dwAddress;
	INT m_nLength;
	//▲1.0.0.1044

	//▼1.0.0.1022
	CString m_szRecieve;
	//▲1.0.0.1022


};

