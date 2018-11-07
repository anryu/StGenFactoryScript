#pragma once
#include "stcheckbase.h"
#include <vector>

class StCheckSerialVersion :
	public StCheckBase
{
public:
	StCheckSerialVersion(void);
	StCheckSerialVersion(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog=NULL);
	~StCheckSerialVersion(void);
	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
   	//▼1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, CString &szValue);
	//▲1.0.0.1025

	BOOL Execute(void);

	BOOL IsCheckDeviceVersion(void){return m_IsCheckDeviceVersion;}
	BOOL IsCheckDeviceSerialNumber(void){return m_IsCheckDeviceSerialNumber;}
	BOOL IsCheckDeviceFirmwareVersion(void){return m_IsCheckDeviceFirmwareVersion;}
	//▼1.0.0.1043
	BOOL IsCheckSensorSerialNumber(void){return m_IsCheckSensorSerialNumber;}
	//▲1.0.0.1025

	//▼1.0.0.1071
	BOOL IsCheckDeviceModelName(void){return m_IsCheckDeviceModelName;}
	//▲1.0.0.1071

	void GetResultDeviceVersion(LPTSTR szData, size_t size)
	{
		_tcscpy_s(szData,size,m_szResultDeviceVersion);
	}
	void GetResultDeviceSerialNumber(LPTSTR szData, size_t size)
	{
		_tcscpy_s(szData,size,m_szResultDeviceSerialNumber);
	}
	void GetResultDeviceFirmwareVersion(LPTSTR szData, size_t size)
	{
		_tcscpy_s(szData,size,m_szResultDeviceFirmwareVersion);
	}
	//▼1.0.0.1071
	void GetResultDeviceModelName(LPTSTR szData, size_t size)
	{
		_tcscpy_s(szData,size,m_szResultDeviceModelName);
	}
	//▲1.0.0.1071
	//▼1.0.0.1043
	void GetResultSensorSerialNumber(LPTSTR szData, size_t size)
	{
		_tcscpy_s(szData,size,m_szResultSensorSerialNumber);
	}
	void GetEllitoSerialNumberFilepath(LPTSTR szData, size_t size)
	{
		_tcscpy_s(szData,size,m_szEllitoSerialNumberFilepath);
	}
	void GetEllitoSerialNumberFilename(LPTSTR szData, size_t size)
	{
		_tcscpy_s(szData,size,m_szEllitoSerialNumberFilename);
	}
	//▲1.0.0.1043

private:
	void Initialize(void);

	CString m_szDeviceVersion;
	CString m_szDeviceSerialNumber;
	CString m_szDeviceFirmwareVersion;

	CString m_szResultDeviceVersion;
	CString m_szResultDeviceSerialNumber;
	CString m_szResultDeviceFirmwareVersion;

	BOOL m_IsCheckDeviceVersion;
	BOOL m_IsCheckDeviceSerialNumber;
	BOOL m_IsCheckDeviceFirmwareVersion;

	//▼1.0.0.1071
	CString m_szDeviceModelName;
	CString m_szResultDeviceModelName;
	BOOL m_IsCheckDeviceModelName;
	//▲1.0.0.1071

	//▼1.0.0.15 カメラリンク用
	INT m_nMode;       //0:Binary 1:Ascii 2:Takano
	CString m_szSensorSerialNumber;
	CString m_szResultSensorSerialNumber;
	BOOL m_IsCheckSensorSerialNumber;

	BOOL ReadText(void *pComm, LPCTSTR szCommand, LPTSTR szData, size_t dataSize);
	//▲1.0.0.15

	//▼1.0.0.1029
	BOOL AddSensorSerialETX( LPCTSTR szETX );
	BOOL ClearSensorSerialETX( void );
	std::vector<CString> m_vCheckETX;
	//▲1.0.0.1029

	//▼1.0.0.1037
	CString m_sYearAddress;
	CString m_sMonthAddress;
	CString m_sSN1Address;
	CString m_sSN2Address;
	CString m_sSN3Address;
	CString m_sSN4Address;
	CString m_sPDTDevice;
	CString m_sPDTPage;

	BOOL ReadBinary(void *pComm, CString &szAddress, PBYTE piSerial);
	//▲1.0.0.1037
	//▼1.0.0.1043
	CString m_szEllitoSerialNumberFilepath;
	CString m_szEllitoSerialNumberFilename;
	//▲1.0.0.1043
	//▼1.0.0.1058
	//GenICam用アドレス
	DWORD m_dwAddress;

	//TAKANO用
	DWORD m_dwSelectorAddress;
	DWORD m_nDeviceSerialNumberSelectorValue;
	DWORD m_nSensorSerialNumberSelectorValue;
	INT m_nSensorSerialNumberLength;
	//▲1.0.0.1058

};

