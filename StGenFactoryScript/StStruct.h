#pragma once
#include "stdafx.h"
#include <windows.h>
#include <vector>
#include "../CommonHeader/StPixelType.h"
#include "StCommon.h"

class CCheckType
{
public:
	CCheckType(DWORD dwID, LPCTSTR szName, LPCTSTR szTailText, BOOL hasCCDLotNo = TRUE)
	{
		m_dwID = dwID;
		m_strName = szName;
		m_strTailText = szTailText;
		m_hasCCDLotNo = hasCCDLotNo;
	};
	virtual ~CCheckType()
	{

	};

	DWORD GetID(){return(m_dwID);};
	LPCTSTR GetName(){return(m_strName);};
	LPCTSTR GetTailText(){return(m_strTailText);};
	BOOL	HasCCDLotNo(){return(m_hasCCDLotNo);};
protected:
	DWORD m_dwID;
	LPCTSTR m_strName;
	LPCTSTR m_strTailText;
	BOOL	m_hasCCDLotNo;
};

class CCameraType
{
public:
	CCameraType( LPCTSTR szCfgFineName, LPCTSTR szIniFileName, LPCTSTR szCameraTypeName)
	{
		if( szCfgFineName )
			_tcscpy_s(m_szCameraCFGFileName,_countof(m_szCameraCFGFileName), szCfgFineName);
		else
			_tcscpy_s(m_szCameraCFGFileName,_countof(m_szCameraCFGFileName), _T(""));
		if( szIniFileName )
			_tcscpy_s(m_szCameraINIFileName,_countof(m_szCameraINIFileName), szIniFileName);
		else
			_tcscpy_s(m_szCameraINIFileName,_countof(m_szCameraINIFileName), _T(""));
		_tcscpy_s(m_szCameraTypeName,_countof(m_szCameraTypeName), szCameraTypeName);
		_tcscpy_s(m_szCameraDisplayName,_countof(m_szCameraDisplayName), _T(""));
		_tcscpy_s(m_szDeviceVendorName,_countof(m_szDeviceVendorName), _T(""));
		_tcscpy_s(m_szDeviceManufacturerInfo,_countof(m_szDeviceManufacturerInfo), _T(""));
		//dwCameraTypeCode = 0;

		m_wMaxSizeX = 0;
		m_wMaxSizeY = 0;
		m_wColor = 0;
		m_wBayer = 1;	//1: Mono,  2: RGGB,  3: GRBG,  4: GBRG,  5: BGGR
		m_iCustomerCode = 0;
		m_iGamma = 0;
		//▼1.0.0.22
		m_iConfigurationImageSizeDefaultImpl = 0;
		//▲1.0.0.22
		m_wWaitFrameCount = 0;
		m_wGetImageWaitTime = 0;
		m_dblCommonAcquisitionFrameRate = 0.0;
		m_dblCommonExposureTime = 0.0;
		_tcscpy_s(m_szCommonPixelFormat,_countof(m_szCommonPixelFormat), _T(""));
		//▼1.0.0.21
		m_dblCommonBlackLevel = 8.0;
		//▲1.0.0.21
		_tcscpy_s(m_szRegisterMapFile,_countof(m_szRegisterMapFile), _T(""));
		_tcscpy_s(m_szXMLFile,_countof(m_szXMLFile), _T(""));

		m_iCXPPower = 0;
		m_iTEXIOPower = 0;
		//▼2016/02/08 1.0.0.15 起動時のAccessModeを設定
		//m_iAccessModeDefault = -1;
		//▲2016/02/08 1.0.0.15 起動時のAccessModeを設定

		//-----------------------------------------
		_tcscpy_s(m_szNextCfgFile,_countof(m_szNextCfgFile), _T(""));
		_tcscpy_s(m_szNextSubTitle,_countof(m_szNextSubTitle), _T(""));
		_tcscpy_s(m_szMainModeTitle,_countof(m_szMainModeTitle), _T(""));
		m_iNextInsertIndex = -1;

		//▼1.0.0.15
		_tcscpy_s(m_szCfgFileVersion,_countof(m_szCfgFileVersion), _T(""));
		//▲1.0.0.15

	};
	virtual ~CCameraType()
	{
	};
	LPCTSTR GetIniFileName(){return(m_szCameraINIFileName);}
	LPCTSTR GetCfgFileName(){return(m_szCameraCFGFileName);}
	LPCTSTR GetCameraTypeName(){return(m_szCameraTypeName);}
	LPCTSTR GetDeviceVendorName(){return(m_szDeviceVendorName);}
	LPCTSTR GetDeviceManufacturerInfo(){return(m_szDeviceManufacturerInfo);}
	//DWORD GetCameraTypeCode(){return(dwCameraTypeCode);}
	//▼1.0.0.15
	LPCTSTR GetCfgFileVersion(){return(m_szCfgFileVersion);}
	void SetCfgFileVersion(LPCTSTR szCfgFileVersion)
	{
		_tcscat_s( m_szCfgFileVersion, _countof(m_szCfgFileVersion), szCfgFileVersion );
	}
	//▲1.0.0.15

	//---Data--------------------------------------------
	WORD	m_wMaxSizeX;
	WORD	m_wMaxSizeY;
	WORD	m_wColor;
	WORD	m_wBayer;
	INT		m_iCustomerCode;
	INT		m_iGamma;
	//▼1.0.0.22
	INT		m_iConfigurationImageSizeDefaultImpl;
	//▲1.0.0.22
	WORD	m_wWaitFrameCount;
	WORD	m_wGetImageWaitTime;
	//-------------------------------------------------

	double GetCommonAcquisitionFrameRate(){return m_dblCommonAcquisitionFrameRate;}
	void SetCommonAcquisitionFrameRate(double dblData){m_dblCommonAcquisitionFrameRate=dblData;}
	double GetCommonExposureTime(){return m_dblCommonExposureTime;}
	void SetCommonExposureTime(double dblData){m_dblCommonExposureTime=dblData;}
	LPCTSTR GetCommonPixelFormat(){return(m_szCommonPixelFormat);}
	void SetCommonPixelFormat(LPCTSTR szPixelFormat)
	{
		_tcscat_s( m_szCommonPixelFormat, _countof(m_szCommonPixelFormat), szPixelFormat );
	}
	//▼1.0.0.21
	double GetCommonBlackLevel(){return m_dblCommonBlackLevel;}
	void SetCommonBlackLevel(double dblData){m_dblCommonBlackLevel=dblData;}
	//▲1.0.0.21
	LPCTSTR GetCameraDisplayName(){return(m_szCameraDisplayName);}
	void SetCameraDisplayName(LPCTSTR szCameraDisplayName)
	{
		_tcscat_s( m_szCameraDisplayName, _countof(m_szCameraDisplayName), szCameraDisplayName );
	}
	void SetDeviceVendorName(LPCTSTR szDeviceVendorName)
	{
		_tcscat_s( m_szDeviceVendorName, _countof(m_szDeviceVendorName), szDeviceVendorName );
	}
	void SetDeviceManufacturerInfo(LPCTSTR szDeviceManufacturerInfo)
	{
		_tcscat_s( m_szDeviceManufacturerInfo, _countof(m_szDeviceManufacturerInfo), szDeviceManufacturerInfo );
	}

	LPCTSTR GetRegisterMapFile(){return(m_szRegisterMapFile);}
	void SetRegisterMapFile(LPCTSTR szRegisterMapFile)
	{
		_tcscat_s( m_szRegisterMapFile, _countof(m_szRegisterMapFile), szRegisterMapFile );
	}
	LPCTSTR GetXMLFile(){return(m_szXMLFile);}
	void SetXMLFile(LPCTSTR szXMLFile)
	{
		_tcscat_s( m_szXMLFile, _countof(m_szXMLFile), szXMLFile );
	}

	INT GetCXPPower(){return m_iCXPPower;}
	void SetCXPPower(INT iData){m_iCXPPower=iData;}
	INT GetTEXIOPower(){return m_iTEXIOPower;}
	void SetTEXIOPower(INT iData){m_iTEXIOPower=iData;}
	//▼2016/02/08 1.0.0.15 起動時のAccessModeを設定
	//INT GetAccessModeDefault(){return m_iAccessModeDefault;}
	//void SetAccessModeDefault(INT iData){m_iAccessModeDefault=iData;}
	//▲2016/02/08 1.0.0.15 起動時のAccessModeを設定

	//---------------------------------
	LPCTSTR GetNextCfgFile(){return(m_szNextCfgFile);}
	void SetNextCfgFile(LPCTSTR szFile)
	{
		_tcscat_s( m_szNextCfgFile, _countof(m_szNextCfgFile), szFile );
	}
	LPCTSTR GetNextSubTitle(){return(m_szNextSubTitle);}
	void SetNextSubTitle(LPCTSTR szFile)
	{
		_tcscat_s( m_szNextSubTitle, _countof(m_szNextSubTitle), szFile );
	}
	LPCTSTR GetMainModeTitle(){return(m_szMainModeTitle);}
	void SetMainModeTitle(LPCTSTR szFile)
	{
		_tcscat_s( m_szMainModeTitle, _countof(m_szMainModeTitle), szFile );
	}

	INT GetNextInsertIndex(){return m_iNextInsertIndex;}
	void SetNextInsertIndex(INT iData){m_iNextInsertIndex=iData;}


protected:
	TCHAR	m_szCameraINIFileName[MAX_PATH];
	TCHAR	m_szCameraCFGFileName[MAX_PATH];
	TCHAR	m_szCameraTypeName[MAX_PATH];
	TCHAR	m_szCameraDisplayName[MAX_PATH];
	TCHAR	m_szDeviceVendorName[MAX_PATH];
	TCHAR	m_szDeviceManufacturerInfo[MAX_PATH];
	//DWORD	dwCameraTypeCode;

	//▼1.0.0.15
	TCHAR	m_szCfgFileVersion[MAX_PATH];
	//▲1.0.0.15

	double m_dblCommonAcquisitionFrameRate;
	double m_dblCommonExposureTime;
	TCHAR	m_szCommonPixelFormat[MAX_PATH];
	//▼1.0.0.21
	double m_dblCommonBlackLevel;
	//▲1.0.0.21
	TCHAR	m_szRegisterMapFile[MAX_PATH];
	TCHAR	m_szXMLFile[MAX_PATH];

	INT		m_iCXPPower;
	INT		m_iTEXIOPower;
	//▼2016/02/08 1.0.0.15 起動時のAccessModeを設定
	//INT		m_iAccessModeDefault;
	//▲2016/02/08 1.0.0.15 起動時のAccessModeを設定

	//-----------------------------------------
	TCHAR	m_szNextCfgFile[MAX_PATH];
	TCHAR	m_szNextSubTitle[MAX_PATH];
	TCHAR	m_szMainModeTitle[MAX_PATH];
	INT		m_iNextInsertIndex;

};









/*
//-----------------------------------------------------------------------------
//Color Array
//-----------------------------------------------------------------------------
#define	STCAM_COLOR_ARRAY_NONE	0x0000
#define	STCAM_COLOR_ARRAY_MONO	0x0001
#define	STCAM_COLOR_ARRAY_RGGB	0x0002
#define	STCAM_COLOR_ARRAY_GRBG	0x0003
#define	STCAM_COLOR_ARRAY_GBRG	0x0004
#define	STCAM_COLOR_ARRAY_BGGR	0x0005
#define	STCAM_COLOR_ARRAY_RGB	0x0010
#define	STCAM_COLOR_ARRAY_RGBA	0x0011
#define	STCAM_COLOR_ARRAY_BGR	0x0012
#define	STCAM_COLOR_ARRAY_BGRA	0x0013

BOOL GetColorInformation( const StPixelType aPixelType, PWORD pwColorArray, PINT piBitsPerPixel )
{
	*piBitsPerPixel = (aPixelType>>16)&0xFF;
	WORD wColorArray = STCAM_COLOR_ARRAY_NONE;
	switch(aPixelType)
	{
	case(StPixelMono8):
	case(StPixelMono8Signed):
	case(StPixelMono10):
	case(StPixelMono10Packed):
	case(StPixelMono12):
	case(StPixelMono12Packed):
	case(StPixelMono14):
	case(StPixelMono16):
		wColorArray = STCAM_COLOR_ARRAY_MONO;
		break;
	case(StPixelBayerGR8):
	case(StPixelBayerGR10):
	case(StPixelBayerGR12):
	case(StPixelBayerGR10Packed):
	case(StPixelBayerGR12Packed):
	case(StPixelBayerGR16):
		wColorArray = STCAM_COLOR_ARRAY_GRBG;
		break;
	case(StPixelBayerRG8):
	case(StPixelBayerRG10):
	case(StPixelBayerRG12):
	case(StPixelBayerRG10Packed):
	case(StPixelBayerRG12Packed):
	case(StPixelBayerRG16):
		wColorArray = STCAM_COLOR_ARRAY_RGGB;
		break;
	case(StPixelBayerGB8):
	case(StPixelBayerGB10):
	case(StPixelBayerGB12):
	case(StPixelBayerGB10Packed):
	case(StPixelBayerGB12Packed):
	case(StPixelBayerGB16):
		wColorArray = STCAM_COLOR_ARRAY_GBRG;
		break;
	case(StPixelBayerBG8):
	case(StPixelBayerBG10):
	case(StPixelBayerBG12):
	case(StPixelBayerBG10Packed):
	case(StPixelBayerBG12Packed):
	case(StPixelBayerBG16):
		wColorArray = STCAM_COLOR_ARRAY_BGGR;
		break;

	case(StPixelRGB8Packed):
	case(StPixelRGB10Packed):
	case(StPixelRGB12Packed):
	case(StPixelRGB16Packed):
	case(StPixelRGB12V1Packed):
	case(StPixelRGB8Planar):
	case(StPixelRGB10Planar):
	case(StPixelRGB12Planar):
	case(StPixelRGB16Planar):
		wColorArray = STCAM_COLOR_ARRAY_RGB;
		break;
	case(StPixelRGBA8Packed):
		wColorArray = STCAM_COLOR_ARRAY_RGBA;
		break;
	case(StPixelBGR8):
	case(StPixelBGR10):
	case(StPixelBGR10p):
	case(StPixelBGR12):
	case(StPixelBGR12p):
	case(StPixelBGR14):
	case(StPixelBGR16):
	case(StPixelBGR10V1Packed):
	case(StPixelBGR10V2Packed):
		wColorArray = STCAM_COLOR_ARRAY_BGR;
		break;
	case(StPixelBGRa8):
	case(StPixelBGRa10):
	case(StPixelBGRa10p):
	case(StPixelBGRa12):
	case(StPixelBGRa12p):
	case(StPixelBGRa14):
	case(StPixelBGRa16):
		wColorArray = STCAM_COLOR_ARRAY_BGRA;
		break;

	}
	*pwColorArray = wColorArray;
	return TRUE;
}
*/



/*
typedef struct _S_TARGET_CAMERA_TYPE
{

	//DWORD   dwCfgVersion;
	TCHAR	szCfgVersion[MAX_PATH];

	TCHAR	szCameraINIFileName[MAX_PATH];
	TCHAR	szCameraCFGFileName[MAX_PATH];

	TCHAR	szCameraSettingFileName[MAX_PATH];

	DWORD	dwCameraTypeCode;
	TCHAR	szCameraTypeName[MAX_PATH];

	WORD	wClockPerLine;
	WORD	wLinePerFrame;
	DWORD	dwLineRate_Hz;
	WORD	wMaxSizeX;
	WORD	wMaxSizeY;
	BYTE	byteTaps;
	BYTE	byteBayer;
	BYTE	byteColor;
	BYTE	byteOption;
	BYTE	byteCDSTiming;	//CDSタイミング調整を行うかどうかのフラグ
	BYTE	byteCMOSIS;		//CMOSYSv2 クランプ調整、フラグ
	BYTE	byteCMOSISv3;	//CMOSYSv3 クランプ調整、フラグ
	BYTE	byteAptina;		//Aptina アプティナフラグ　DigitalGain調整
	BYTE	byteE2V;		//E2V E2Vフラグ　DigitalGain調整

	BYTE	bytePoE;
	BYTE	byteIOType;		//IO検査 0:６ピン 1:１２ピン
	BYTE	byteIRISFlg;	//IRIS検査有無 0:なし、1:あり
	BYTE	byteIRISType;
	WORD	wVersion;	//0x000F以上はバックアップEEPROMに書込み用

	double	dblMaxFramerate;
	DWORD	AcquisitionFrameRate;
	// シャッターゲイン更新時の待機フレーム数をINIファイルで設定できるように
	DWORD		dwWaitFrameCount;
	// 画像取得ウェイト時間をINIファイルで設定できるように
	DWORD		dwGetImageWaitTime;

	BYTE	byteTemperatureCheck;
	BYTE	byteVerticalNoiseCorrect;

	INT		iDigitalGainCheck;

	//クロック情報
//	INT		iLineRate_Hz;

}S_TARGET_CAMERA_TYPE, *PS_TARGET_CAMERA_TYPE;
*/

class CStructPortName
{
public:
	//▼1.0.0.1017
	//CStructPortName( BOOL bReadFile=FALSE, INT nCount=0)
	CStructPortName( BOOL bReadFile=FALSE, INT nCount=0, INT nMode=0)
	//▲1.0.0.1017
		: m_nSelectorCount(nCount)
		, m_bReadFile(bReadFile)
		//▼1.0.0.1017
		, m_nMode(nMode)
		//▲1.0.0.1017
	{
		if( m_bReadFile )
		{
			for( int i=0; i<m_nSelectorCount; i++ )
			{
				CString szString;
				//▼1.0.0.1017
				//szString.Format(_T("JIGU%d"),i+1);
				if( m_nMode==0 )
				{
					szString.Format(_T("JIGU%d"),i+1);
				}
				else
				{
					szString.Format(_T("TERM%d"),i+1);
				}
				//▲1.0.0.1017
				m_vSelectorName.push_back(szString);
			}

			TCHAR szIniFileName[MAX_PATH];
			CStCommon::GetIniFileName(szIniFileName, _countof(szIniFileName));

			//▼1.0.0.1017
			TCHAR szSection[MAX_PATH];
			TCHAR szDefault[MAX_PATH];
			_stprintf_s(szSection,_countof(szSection),m_nMode==0?_T("PortName"):_T("Address"));
			_stprintf_s(szDefault,_countof(szDefault),m_nMode==0?_T("COM1"):_T(""));
			//▲1.0.0.1017
			for( unsigned int i=0; i<m_vSelectorName.size(); i++ )
			{
				TCHAR szTmp[128];
				//▼1.0.0.1017
				//GetPrivateProfileString(_T("PortName"), m_vSelectorName[i], _T("COM1"), szTmp, _countof(szTmp), szIniFileName);
				GetPrivateProfileString( szSection, m_vSelectorName[i], szDefault, szTmp, _countof(szTmp), szIniFileName );
				//▲1.0.0.1017
				m_vPortName.push_back(CString(szTmp));
			}
		}
	}
	virtual ~CStructPortName()
	{
		if( m_bReadFile )
		{

			//▼1.0.0.1017
			TCHAR szSection[MAX_PATH];
			_stprintf_s(szSection,_countof(szSection),m_nMode==0?_T("PortName"):_T("Address"));
			//▲1.0.0.1017

			TCHAR szIniFileName[MAX_PATH];
			CStCommon::GetIniFileName(szIniFileName, _countof(szIniFileName));
			for( unsigned int i=0; i<m_vSelectorName.size(); i++ )
			{
				//▼1.0.0.1017
				//WritePrivateProfileString(_T("PortName"), m_vSelectorName[i], m_vPortName[i], szIniFileName);
				WritePrivateProfileString( szSection, m_vSelectorName[i], m_vPortName[i], szIniFileName );
				//▲1.0.0.1017
			}
		}
	}



	INT GetPortSelectorCount(void){return m_vSelectorName.size();}
	BOOL GetSelectorName(INT nIndex, CString &szSelectorName)
	{
		if( nIndex>=(INT)m_vSelectorName.size() ) return FALSE;
		szSelectorName = m_vSelectorName[nIndex];
		return TRUE;
	}

	BOOL GetPortName(LPCTSTR szSelectorName, CString &szPortName)
	{
		BOOL bReval = FALSE;
		for( unsigned int i=0; i<m_vSelectorName.size(); i++ )
		{
			if( m_vSelectorName[i]==szSelectorName )
			{
				bReval = GetPortName( i, szPortName );
				break;
			}
		}
		return bReval;
	}

	BOOL GetPortName(INT nIndex, CString &szPortName)
	{
		if( nIndex>=(INT)m_vPortName.size() ) return FALSE;
		szPortName = m_vPortName[nIndex];
		return TRUE;
	}

	BOOL SetPortName(LPCTSTR szSelectorName, LPCTSTR szPortName)
	{
		BOOL bReval = FALSE;
		for( unsigned int i=0; i<m_vSelectorName.size(); i++ )
		{
			if( m_vSelectorName[i]==szSelectorName )
			{
				bReval = SetPortName( i, szPortName );
				break;
			}
		}
		return bReval;
	}

	BOOL SetPortName(INT nIndex, LPCTSTR szPortName)
	{
		if( nIndex>=(INT)m_vPortName.size() ) return FALSE;
		m_vPortName[nIndex] = CString(szPortName);
		return TRUE;
	}

	void Copy(CStructPortName *pSrc)
	{
		m_nSelectorCount = pSrc->GetPortSelectorCount();
		m_vSelectorName.clear();
		m_vPortName.clear();
		for( int i=0; i<m_nSelectorCount; i++ )
		{
			CString szSelectorName("");
			pSrc->GetSelectorName(i,szSelectorName);
			m_vSelectorName.push_back(szSelectorName);
			CString szPortName("");
			pSrc->GetPortName(szSelectorName,szPortName);
			m_vPortName.push_back(szPortName);
		}
	}

private:
	INT m_nSelectorCount;
	std::vector <CString> m_vSelectorName;
	std::vector <CString> m_vPortName;

	BOOL m_bReadFile;
	
	//▼1.0.0.1017
	INT m_nMode;
	//▲1.0.0.1017

};

//▼1.0.0.1020
class CStructLightChannel
{
public:
	CStructLightChannel(void)
	{
			
		PTCHAR szName[] = {_T("Red"), _T("Green"), _T("Blue"), _T("White")};
		for( int i=0; i<_countof(szName); i++ )
		{
			CString szString;
			szString = szName[i];
			m_vSelectorName.push_back(szString);
		}

		TCHAR szIniFileName[MAX_PATH];
		CStCommon::GetIniFileName(szIniFileName, _countof(szIniFileName));

		TCHAR szSection[MAX_PATH];
		_stprintf_s(szSection,_countof(szSection),_T("LightChannel"));
		for( unsigned int i=0; i<m_vSelectorName.size(); i++ )
		{
			INT nData = GetPrivateProfileInt(szSection, m_vSelectorName[i], i, szIniFileName);
			m_vChannelIndex.push_back(nData);
		}
	}
	virtual ~CStructLightChannel()
	{
		//if( m_bReadFile )
		//{

		//	//▼1.0.0.1017
		//	TCHAR szSection[MAX_PATH];
		//	_stprintf_s(szSection,_countof(szSection),m_nMode==0?_T("PortName"):_T("Address"));
		//	//▲1.0.0.1017

		//	TCHAR szIniFileName[MAX_PATH];
		//	CStCommon::GetIniFileName(szIniFileName, _countof(szIniFileName));
		//	for( unsigned int i=0; i<m_vSelectorName.size(); i++ )
		//	{
		//		//▼1.0.0.1017
		//		//WritePrivateProfileString(_T("PortName"), m_vSelectorName[i], m_vPortName[i], szIniFileName);
		//		WritePrivateProfileString( szSection, m_vSelectorName[i], m_vPortName[i], szIniFileName );
		//		//▲1.0.0.1017
		//	}
		//}
	}

	BOOL WriteFileLightChannel(void)
	{
		BOOL bReval = TRUE;
		TCHAR szSection[MAX_PATH];
		_stprintf_s(szSection,_countof(szSection),_T("LightChannel"));

		TCHAR szIniFileName[MAX_PATH];
		CStCommon::GetIniFileName(szIniFileName, _countof(szIniFileName));
		for( unsigned int i=0; i<m_vSelectorName.size(); i++ )
		{
			TCHAR szValue[64];
			_stprintf_s(szValue,_countof(szValue), _T("%i"), m_vChannelIndex[i]);
			WritePrivateProfileString( szSection, m_vSelectorName[i], szValue, szIniFileName );
		}

		return bReval;
	}

	INT GetPortSelectorCount(void){return m_vSelectorName.size();}
	BOOL GetSelectorName(INT nIndex, CString &szSelectorName)
	{
		if( nIndex>=(INT)m_vSelectorName.size() ) return FALSE;
		szSelectorName = m_vSelectorName[nIndex];
		return TRUE;
	}

	BOOL GetChannelIndex(LPCTSTR szSelectorName, INT &nChannelIndex)
	{
		BOOL bReval = FALSE;
		for( unsigned int i=0; i<m_vSelectorName.size(); i++ )
		{
			if( m_vSelectorName[i]==szSelectorName )
			{
				bReval = GetIndex( i, nChannelIndex );
				break;
			}
		}
		return bReval;
	}

	BOOL GetIndex(INT nIndex, INT &nChannelIndex)
	{
		if( nIndex>=(INT)m_vChannelIndex.size() ) return FALSE;
		nChannelIndex = m_vChannelIndex[nIndex];
		return TRUE;
	}

	BOOL SetChannelIndex(LPCTSTR szSelectorName, INT nChannelIndex)
	{
		BOOL bReval = FALSE;
		for( unsigned int i=0; i<m_vSelectorName.size(); i++ )
		{
			if( m_vSelectorName[i]==szSelectorName )
			{
				bReval = SetIndex( i, nChannelIndex );
				break;
			}
		}
		return bReval;
	}

	BOOL SetIndex(INT nIndex, INT nChannelIndex)
	{
		if( nIndex>=(INT)m_vChannelIndex.size() ) return FALSE;
		m_vChannelIndex[nIndex] = nChannelIndex;
		return TRUE;
	}

	void Copy(CStructLightChannel *pSrc)
	{
		INT nSelectorCount = pSrc->GetPortSelectorCount();
		m_vSelectorName.clear();
		m_vChannelIndex.clear();
		for( int i=0; i<nSelectorCount; i++ )
		{
			CString szSelectorName("");
			pSrc->GetSelectorName(i,szSelectorName);
			m_vSelectorName.push_back(szSelectorName);
			CString szPortName("");
			INT nIndex = 0;
			pSrc->GetChannelIndex(szSelectorName,nIndex);
			m_vChannelIndex.push_back(nIndex);
		}
	}

private:
	std::vector <CString> m_vSelectorName;
	std::vector <INT> m_vChannelIndex;

};
//▲1.0.0.1020


