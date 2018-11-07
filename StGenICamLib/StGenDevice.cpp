#include "StdAfx.h"

#include "StInterface.h"
//#include "StDevice.h"

#include "StDeviceInfo.h"
//#include "StDevice.h"
#include "StGenDevice.h"
#include "StStream.h"

#include "StGenPort.h"
//#include "StGenEvent.h"

#include "StString.h"


//#include <iostream>
//#include <fstream>
#include <algorithm>	//std::transform

//#include <stdio.h>
//#include <wchar.h>


//#define __TEMP_ZIP
#ifdef __TEMP_ZIP

#include "zip.h"
//#include "lib/zlib.h"
#include "zlib/include/zlib.h"
#ifdef _DEBUG
#pragma comment(lib,"../zlib/Debug/zlib.lib")
//#pragma comment(lib,"./zlib/lib/zlib.lib")
#else
//#pragma comment(lib,"./lib/release/zlib.lib")
#pragma comment(lib,"./zlib/lib/zlib.lib")
#endif

#else

#include "GenTLZip.h"

#endif

//------------------------------------------------------------------------
//--StGenDeviceValue----------------------------------------------------------------------
//------------------------------------------------------------------------
class StGenDeviceValue
{
public:
	StGenDeviceValue(void);
	virtual ~StGenDeviceValue(void);

	const StString& GetID(void) const{return m_strID;}
    const StString& GetVendor() const{return m_strVendor;}
    const StString& GetModel() const{return m_strModel;}
    const StString& GetDisplayName() const{return m_strDisplayName;}
    const StString& GetPortName() const{return m_strPortName;}
    const StString& GetXMLFileName() const{return m_strXMLFileName;}
    const DEV_HANDLE GetHandle() const{return m_hDevice;}
	void SetHandle(DEV_HANDLE hDevice)
	{
		m_hDevice = hDevice;
//TRACE(_T(" @@@@@@@@@@@   SetHandle=0x%x\n"), m_hDevice );
//if( m_hDevice!=NULL )
//	Sleep(1);
	}

	bool IsOpen(void) const
	{
//TRACE(_T(" @@@@@@@@@@@   IsOpen=0x%x\n"), m_hDevice );
		return m_hDevice!=NULL;
	}
	void QueryInformations( void );

	//▼1.0.0.25
	bool OpenPort( void );
	void ClosePort( void );
	//▲1.0.0.25

	void InitInformations( void );

	void SetDeviceInfo(StDeviceInfo *pDevInfo){m_pDevInfo = pDevInfo;}
	StDeviceInfo * GetDeviceInfo(void){return m_pDevInfo;}


	//StInt32 GetNumStream( void );
	//StGenStream * GetGenStream(StInt32 index);

//private:
	//std::vector< StGenStream * > m_vStreamList;

    StString m_strID;
    StString m_strVendor;
    StString m_strModel;
    StString m_strDisplayName;
    StString m_strPortName;

	StString m_strXMLFileName;

	bool FetchNodeMap( void );
    void ReleaseNodeMap(void);

	bool TLParamsLocked( bool lock );
	bool GetGenIntParameter( StString lFeature, int & iData, bool Verify, bool IgnoreCache );
	bool GetGenIntMinMaxParameter( StString lFeature, int & iMin, int & iMax );
	bool SetGenIntParameter( StString lFeature, int iData );
	bool GetGenEnumParameter( StString lFeature, int & iData, bool Verify, bool IgnoreCache );
	bool GetGenEnumParameter( StString lFeature, StString & strData, bool Verify, bool IgnoreCache );
	//▼1.0.0.28 beta1
	bool GetGenEnumIndexParameter( StString lFeature, INT nIndex, StString & strData, bool Verify, bool IgnoreCache );
	//▲1.0.0.28 beta1
	bool SetGenEnumParameter( StString lFeature, int iData );
	bool SetGenEnumParameter( StString lFeature, StString strData );
	bool GetGenFloatParameter( StString lFeature, double & dblData, bool Verify, bool IgnoreCache );
	bool GetGenFloatMinMaxParameter( StString lFeature, double & dblMin, double & dblMax );
	bool SetGenFloatParameter( StString lFeature, double dblData );
	bool GetGenBoolParameter( StString lFeature, bool & bData, bool Verify, bool IgnoreCache );
	bool SetGenBoolParameter( StString lFeature, bool bData );
	bool SetGenCommandParameter( StString lFeature );
	bool GetGenStringParameter( StString lFeature, StString & strData, bool Verify, bool IgnoreCache );
	bool SetGenStringParameter( StString lFeature, StString & strData );
	bool GetPayloadSize( int & iData );
	bool Read( DWORD dwAddress, char *pBuffer, size_t size );
	bool Write( DWORD dwAddress, char *pBuffer, size_t size );

	StInt32 GetNumStreams( void );
	StString GetStreamID( uint32_t index );
	StResult CreateStream(void);
	void DeleteStream(void);
	StInt32 GetNumStream(void);
	StGenStream *GetGenStream(StInt32 index);

private:
	DEV_HANDLE m_hDevice;
	StDeviceInfo *m_pDevInfo;
	GenApi::CNodeMapRef* m_pDeviceMap;
	bool GetNodeMap(PORT_HANDLE hPort, CNodeMapRef& rNodeMap);
	StGenPort *m_psPort;
	gcstring RetrieveXML(PORT_HANDLE hPort, gcstring &gcstrXml);

	std::vector< StStream * > m_vDSList;

};

StGenDeviceValue::StGenDeviceValue()
    : m_strID( "" )
    , m_strVendor( "" )
    , m_strModel( "" )
    , m_strDisplayName( "" )
    , m_strPortName( "" )
	, m_strXMLFileName( "" )
	, m_hDevice(NULL)
	, m_pDevInfo(NULL)
	, m_pDeviceMap(NULL)
	, m_psPort(NULL)
{
}

StGenDeviceValue::~StGenDeviceValue()
{
}
void StGenDeviceValue::InitInformations( void )
{
    m_strID = "";
    m_strVendor = "";
    m_strModel = "";
    m_strDisplayName = "";
    m_strPortName = "";
	m_strXMLFileName = "";
}
void StGenDeviceValue::QueryInformations( void )
{
//	if( !IsOpen() ) return;
	if( !m_pDevInfo ) return;

	StInterface *pInterface = m_pDevInfo->GetInterface();
	StGenGCLib* psGCLib = pInterface->GetGenGCLib(); 

	if (psGCLib->f_DevGetInfo)
    {
        GC_ERROR status;
        size_t iSize = 0;
        INFO_DATATYPE iType = INFO_DATATYPE_UNKNOWN;

		status = psGCLib->f_DevGetInfo(m_hDevice, DEVICE_INFO_VENDOR, &iType, NULL, &iSize);
        if ((0 == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            status = psGCLib->f_DevGetInfo(m_hDevice, DEVICE_INFO_VENDOR, &iType, str, &iSize);
            m_strVendor = str;
            delete[] (str);
        }

		iType = INFO_DATATYPE_UNKNOWN;
		iSize = 0;
		status = psGCLib->f_DevGetInfo(m_hDevice, DEVICE_INFO_MODEL, &iType, NULL, &iSize);
        if ((0 == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            status = psGCLib->f_DevGetInfo(m_hDevice, DEVICE_INFO_MODEL, &iType, str, &iSize);
            m_strModel = str;
            delete[] (str);
        }

		iType = INFO_DATATYPE_UNKNOWN;
		iSize = 0;
		status = psGCLib->f_DevGetInfo(m_hDevice, DEVICE_INFO_DISPLAYNAME, &iType, NULL, &iSize);
        if ((0 == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            status = psGCLib->f_DevGetInfo(m_hDevice, DEVICE_INFO_DISPLAYNAME, &iType, str, &iSize);
            m_strDisplayName = str;
            delete[] (str);
        }

        iType = INFO_DATATYPE_UNKNOWN;
        iSize = 0;
        status = psGCLib->f_GCGetPortInfo(m_hDevice,PORT_INFO_PORTNAME, &iType, NULL, &iSize);
        if ((GC_ERR_SUCCESS == status) && (iSize > 0) && (iType == INFO_DATATYPE_STRING))
        {
            char* str = new char[iSize];
            status = psGCLib->f_GCGetPortInfo(m_hDevice,PORT_INFO_PORTNAME, &iType, str, &iSize);
            m_strPortName = str;
            delete[] (str);
        }
    }
}

//▼1.0.0.25
bool StGenDeviceValue::OpenPort( void )
{
	if( m_psPort ) return false;

    GC_ERROR status;
    PORT_HANDLE hRemoteDevicePort = NULL;

	StInterface *pInterface = m_pDevInfo->GetInterface();
	StGenGCLib* psGCLib = pInterface->GetGenGCLib(); 

	for( int i=0; i<20; i++ )
	{
		status = psGCLib->f_DevGetPort( m_hDevice, &hRemoteDevicePort);
		if( hRemoteDevicePort ) break;
		Sleep(500);
	}

	bool bReval=true;
    m_psPort = new StGenPort(psGCLib, hRemoteDevicePort);
	if( !m_psPort ) bReval = false;

	return bReval;
}

void StGenDeviceValue::ClosePort( void )
{
	if( m_psPort )
	{
		delete m_psPort;
		m_psPort = NULL;
	}
}
//▲1.0.0.25


bool StGenDeviceValue::FetchNodeMap( void )
{
    GC_ERROR status;
    PORT_HANDLE hRemoteDevicePort = NULL;

    m_pDeviceMap = new GenApi::CNodeMapRef;
	StInterface *pInterface = m_pDevInfo->GetInterface();
	StGenGCLib* psGCLib = pInterface->GetGenGCLib(); 

    //retrieve access to device 


	//CXP(Euresysボード)で電源投入後すぐにConnectした場合はDevGetPortで正常にhRemoteDevicePortが取得できないっぽい。。。。
	//Euresysボードが原因かカメラが原因かはわからない
	//500mSec x 10回ではダメ、500mSec x 20回だとOK
	for( int i=0; i<20; i++ )
	{
		status = psGCLib->f_DevGetPort( m_hDevice, &hRemoteDevicePort);
		if( hRemoteDevicePort ) break;
		Sleep(500);
	}
	//if( !hRemoteDevicePort )
	//	Sleep(1);
	bool bReval = GetNodeMap(hRemoteDevicePort, *m_pDeviceMap);
	if( !bReval )
	{
		delete m_pDeviceMap;
		m_pDeviceMap = NULL;
	}
	return bReval;
}


#define DEBUG_XML
#ifdef DEBUG_XML
BOOL  RetrieveXML2(gcstring &gcstrXml)
{
	BOOL bReval=FALSE;
	TCHAR	szIniFileName[MAX_PATH];
	TCHAR	szXMLFileName[MAX_PATH];
	//StGenDevice.iniファイルを読み込み
	GetModuleFileName(NULL,szIniFileName,_countof(szIniFileName));
	_tcscpy_s(_tcsrchr(szIniFileName,'.'),_countof(szIniFileName)-wcslen(szIniFileName),_T(""));
	_tcscat_s( szIniFileName, _countof(szIniFileName),_T(".ini"));
	//-------------------------------------------------------------------------------
	bReval = GetPrivateProfileString(_T("XML"), _T("Filename"), _T(""), szXMLFileName, _countof(szXMLFileName), szIniFileName);
	if( !bReval ) return FALSE;
	if( _tcscmp(szXMLFileName,_T("Camera"))==0 ) return FALSE;
	if( _tcscmp(szXMLFileName,_T("NULL"))==0 || _tcscmp(szXMLFileName,_T("Non"))==0 )
	{
		gcstrXml = "";
		return TRUE;
	}
	TCHAR	szFullXMLFileName[MAX_PATH];
	GetModuleFileName(NULL,szFullXMLFileName,_countof(szFullXMLFileName));
	_tcscpy_s(_tcsrchr(szFullXMLFileName,'\\'),_countof(szFullXMLFileName)-wcslen(szFullXMLFileName),_T(""));
	_tcscat_s( szFullXMLFileName, _countof(szFullXMLFileName),_T("\\"));
	_tcscat_s( szFullXMLFileName, _countof(szFullXMLFileName),szXMLFileName);

	bReval = FALSE;
	FILE *fp;
	errno_t errRet = _tfopen_s( &fp, szFullXMLFileName, _T("rb") );
	if( fp )
	{
		fseek(fp, 0, SEEK_END);
		fpos_t fsize;
		fgetpos(fp, &fsize); 
		fseek(fp, 0, SEEK_SET);

		PBYTE pBuff = new BYTE [(unsigned int)fsize+1];
		if( pBuff )
		{
			pBuff[fsize]=0;	//最後のNULL必要
			unsigned long origsize = fread(pBuff, 1, (size_t)fsize, fp); // データを読み込む
			if( origsize==fsize )
			{
				PTCHAR psz = _tcsrchr(szXMLFileName,'.');
				if( psz!=NULL && 0 == _tcscmp( psz, _T(".zip")) )
				{

					std::string szTmp = (char *)pBuff;

					//BOOL bReval = GenTLZip::UncompressGenICamXML( szTmp );
					BOOL bReval = GenTLZip::UncompressGenICamXML( szTmp, (char *)pBuff );
					if( bReval )
					{
						gcstrXml = szTmp.c_str();
					}
				}
				else
				{
					gcstrXml = (char *)pBuff;
					bReval = TRUE;
				}

			}
			delete [] pBuff;
		}
		fclose( fp );
	}
	return bReval;
}
#endif


// ---------------------------------------------------------------------------
/// \brief Load xml from Client::port and instantiate the nodemap
///
/// \param [in] hPort Retrieve the nodemap from the provided port implementation
///
/// \return Nodemap, NULL in case of failure
// ---------------------------------------------------------------------------
//bool StGenDevice::GetNodeMap(PORT_HANDLE hPort, CNodeMapRef& rNodeMap, PORT_HANDLE hEventPort, CNodeMapRef& rEventNodeMap)
bool StGenDeviceValue::GetNodeMap(PORT_HANDLE hPort, CNodeMapRef& rNodeMap)
{
	bool bReval=true;

	if( m_psPort )
		return false;


	//StGenPort *pPortImpl = NULL;
    if (0 != hPort)
    {
        //try
		StInterface *pInterface = m_pDevInfo->GetInterface();
		StGenGCLib* psGCLib = pInterface->GetGenGCLib(); 
		do
        {
            gcstring strXML;
#ifdef DEBUG_XML
BOOL bDebReval = RetrieveXML2(strXML);
if( !bDebReval )
{
	RetrieveXML(hPort, strXML);
}
#else
			RetrieveXML(hPort, strXML);
#endif
            if (strXML.size())
            {
                rNodeMap._LoadXMLFromString(strXML);
			}
            else
            {
                bReval = false;
            }
			
			if( bReval==false )
				break;
            //Connect to the port
            //CPort *pPortImpl = new CPort(this, hPort);

            m_psPort = new StGenPort(psGCLib, hPort);
			if( !m_psPort )
				bReval = false;
			if( bReval==false )
				break;
 
			GenApi::IPort* pGenApiPort = dynamic_cast<GenApi::IPort *>(m_psPort);

            size_t iSize = 0;
            INFO_DATATYPE iType = INFO_DATATYPE_UNKNOWN;
            if (GC_ERR_SUCCESS == psGCLib->f_GCGetPortInfo(hPort, PORT_INFO_PORTNAME, &iType, NULL, &iSize))
            {
                char* szPortName = new char[iSize];
                if (GC_ERR_SUCCESS == psGCLib->f_GCGetPortInfo(hPort, PORT_INFO_PORTNAME, &iType, szPortName, &iSize))
                {
                    gcstring gcstrPortName = szPortName;
                    bReval = rNodeMap._Connect(pGenApiPort, gcstrPortName);
                }
                else
                {
                    bReval = false;
                }
				if( szPortName ) delete[] (szPortName);
				
            }
            else
            {
                bReval = false;
            }      

			//▼1.0.0.28　EventTimestampをDeviceViewに表示させる
			/*
			if( bReval )
			{
				if( m_pEventAdapter ){
					//m_pEventAdapter->DetachNodeMap();
					delete m_pEventAdapter;
					m_pEventAdapter = NULL;
				}

	            m_pEventAdapter =
#ifdef EVENTADAPTERU3V
	//				new CEventAdapterU3V(rNodeMap._Ptr);
#else
					//new CEventAdapterGeneric(m_pEventMap->_Ptr);
					new CEventAdapterGeneric(rNodeMap._Ptr);
					//new CEventAdapterGeneric;
#endif
				//DeviceでConnectするとEventとしてつかえなくなる？ためm_pEventMapを使用した
				//m_pEventAdapter->AttachNodeMap(m_pEventMap->_Ptr ); // attach the same nodemap again, should not break anything
//				m_pEventAdapter->AttachNodeMap(rNodeMap._Ptr ); // attach the same nodemap again, should not break anything
			}
			//▲1.0.0.28　EventTimestampをDeviceViewに表示させる
			*/
		}while(0);
   //     catch (GenericException &E)
   //     {
   //         //cerr << "Error " << E.GetDescription() << endl;
			//bReval =  false;
   //     }
    }
    else
    {
        //cerr << "Port not initialized!" << endl;
        bReval = false;
    }
	if( bReval==false )
	{
		if( m_psPort ) delete m_psPort;
		m_psPort = NULL;
	}
	return bReval;
}

// ---------------------------------------------------------------------------
/// \brief Retrieve the GenICam xml from a given port
///
/// \param [in]     hPort           Port to read from
///
/// \return string containing the XML
// ---------------------------------------------------------------------------

gcstring StGenDeviceValue::RetrieveXML(PORT_HANDLE hPort, gcstring &gcstrXml)
{
	gcstrXml = "";
	StInterface *pInterface = m_pDevInfo->GetInterface();
	StGenGCLib* psGCLib = pInterface->GetGenGCLib(); 
	if( psGCLib==NULL ) return "";
    if (0 != hPort)
    {
        bool8_t bPortNotImplemented = false;
        INFO_DATATYPE dataType = INFO_DATATYPE_BOOL8;
        size_t iSize = sizeof(bPortNotImplemented);
        if ((GC_ERR_SUCCESS != psGCLib->f_GCGetPortInfo(hPort, PORT_INFO_ACCESS_NI, &dataType, (void*)&bPortNotImplemented, &iSize))
            || bPortNotImplemented )
        {
            //HandleError("Port not implemented!");
            return "";
        }

        size_t iUrlLength = 0;
        if ((GC_ERR_SUCCESS == psGCLib->f_GCGetPortURL(hPort, NULL, &iUrlLength)) && (iUrlLength > 0))
        {
            char* szUrl = new char[iUrlLength];
            if (GC_ERR_SUCCESS == psGCLib->f_GCGetPortURL(hPort, szUrl, &iUrlLength))
            {
                // Read XML Address
                std::string strXMLAddress = szUrl;
                delete[] (szUrl);

                std::string strLocation  = strXMLAddress.substr(0, 6);
                std::transform(strLocation.begin(), strLocation.end(), strLocation.begin(), (int(*)(int)) toupper);
                std::string strXMLFilename;

                if ("LOCAL:" == strLocation)
                {
                    size_t iOffset = 6;
                    size_t iLength = (size_t)strXMLAddress.find(";", iOffset);
                    std::string strFilename = strXMLAddress.substr(iOffset, iLength - iOffset);

					//XMLファイル名保存
					m_strXMLFileName = strFilename.c_str();


                    iOffset = iLength+1;
                    iLength = strXMLAddress.find(";", iOffset);
                    char *pEnd = 0;
                    uint64_t iAddr = _strtoui64(strXMLAddress.substr(iOffset, iLength-iOffset).c_str(), &pEnd, 16);

                    iOffset = iLength+1;
                    iLength = strXMLAddress.size();
                    size_t iXMLSize = (size_t)_strtoui64(strXMLAddress.substr(iOffset, iLength-iOffset).c_str(), &pEnd, 16);
//sentech
					size_t iReadXMLSize = iXMLSize;
					if(iReadXMLSize & 0x03)
					{
						iReadXMLSize += 4 - (iReadXMLSize & 0x03);
					}
                    iReadXMLSize &= 0xFFFFFFFC;  // make sure we read from a dword boundary

                    void *pXML = malloc(iReadXMLSize + 1);
					memset( pXML, 0, iReadXMLSize + 1 );

                    if (GC_ERR_SUCCESS != psGCLib->f_GCReadPort(hPort, iAddr, pXML, &iReadXMLSize))
                    {
                        //HandleError("could not read from port!");
                        return "";
                    }

					gcstrXml="";

					//zipなら解凍
					//unsigned int loc = strFilename.find( ".zip", 0 );
					if( strFilename.find( ".zip", 0 ) != std::string::npos )
					{


#ifdef __TEMP_ZIP

						char fileFullpathName[MAX_PATH];//, TargetPath[MAX_PATH];

						//strcpy_s( TargetPath, sizeof(TargetPath)/ sizeof(TargetPath[0]), "./"); //PATHを指定

						TCHAR tchrEnvVariable[MAX_PATH];
						TCHAR *pEnv[] = {_T("TEMP"),_T("TMP")};
						DWORD dwResult=0;
						for( int i=0; i<_countof(pEnv) && dwResult==0 ; i++ )
						{
							dwResult = GetEnvironmentVariable(
									pEnv[i],
									tchrEnvVariable,
									sizeof(tchrEnvVariable)
									);
						}
						
						if( dwResult==0 ) swprintf( tchrEnvVariable, MAX_PATH, _T(".") );
						_tcscat_s( tchrEnvVariable, sizeof(tchrEnvVariable)/ sizeof(tchrEnvVariable[0]), _T("\\") );
						//_tcscat_s( tchrEnvVariable, sizeof(tchrEnvVariable)/ sizeof(tchrEnvVariable[0]), _T("/") );

						StString aTargetPath = tchrEnvVariable;
						strcpy_s( fileFullpathName, sizeof(fileFullpathName)/ sizeof(fileFullpathName[0]), aTargetPath.GetAscii()); //PATHを指定
						strcat_s( fileFullpathName, sizeof(fileFullpathName)/ sizeof(fileFullpathName[0]), strFilename.c_str() );

						FILE *fp;
						do
						{
							//念のため削除
							StString aZipPathName = fileFullpathName;
							DeleteFile(aZipPathName.GetUnicode());

							errno_t errRet = fopen_s( &fp, fileFullpathName, "wb" );
							if( fp==NULL ) break;

							//fwrite( pbyteXML, 1, dwFileSize, fp );  
							fwrite( pXML, iXMLSize, 1, fp );  
							fclose( fp );
							char szTmp[MAX_PATH];

							PBYTE pbyteXML = (PBYTE)pXML;

							char fileUnzipName[MAX_PATH];	//解凍されたフルパスファイル名
							WORD namelentgh = *(PWORD)&pbyteXML[0x1A];
							memcpy( szTmp, &pbyteXML[0x1E], namelentgh );
							szTmp[namelentgh] = 0;
							strcpy_s( fileUnzipName, sizeof(fileUnzipName)/ sizeof(fileUnzipName[0]), aTargetPath.GetAscii());
							strcat_s( fileUnzipName, sizeof(fileUnzipName)/ sizeof(fileUnzipName[0]), szTmp );

							std::string strZipFilename = fileFullpathName;
							std::string strTargetPath = aTargetPath.GetAscii();
							//念のため削除
							StString aTargetPathName = fileUnzipName;
							DeleteFile(aTargetPathName.GetUnicode());

							BOOL bReval = Unzip(strZipFilename, strTargetPath);

							fopen_s( &fp, fileUnzipName, "rb" );
							if( fp )
							{
								fseek(fp, 0, SEEK_END);
								fpos_t fsize;
								fgetpos(fp, &fsize); 
								fseek(fp, 0, SEEK_SET);

								PBYTE pBuff = new BYTE [(unsigned int)fsize+1];
								if( pBuff )
								{
									pBuff[fsize]=0;	//最後のNULL必要
									unsigned long origsize = fread(pBuff, 1, (size_t)fsize, fp); // データを読み込む
									if( origsize==fsize )
									{
										gcstrXml = (char *)pBuff;
									}
									delete [] pBuff;
								}

								fclose( fp );
							}
							//StString aTargetPathName = fileUnzipName;
							DeleteFile(aTargetPathName.GetUnicode());
							//StString aZipPathName = strZipFilename.c_str();
							DeleteFile(aZipPathName.GetUnicode());

						}while(0);
#else

						std::string szTmp = (char *)pXML;

						//BOOL bReval = GenTLZip::UncompressGenICamXML( szTmp );
						BOOL bReval = GenTLZip::UncompressGenICamXML( szTmp, (char *)pXML );
						if( bReval )
						{
						//	CString aTmp = szTmp;
							gcstrXml = szTmp.c_str();
						}
						else
						{
							return "";

						}
#endif
					}
					else
					{
						//xmlならそのままコピー
						gcstrXml = (char *)pXML;
					}

//

                    free(pXML);

                    return gcstrXml;
                }
                else
                {
                    //cerr << "Not a local address!" << endl;
                    return "";
                }
            }
            else
            {
                delete[] (szUrl);
                //HandleError("Could not query port URL!");
                return "";
            }
        }
        else
        {
            //HandleError("Could not query size needed for port URL!");
            return "";
        }
    }
    else
    {
        //cerr << "Port not initialized!" << endl;
        return "";
    }
}

void StGenDeviceValue::ReleaseNodeMap( void )
{
	//if( m_pEventAdapter )
	//{
	//	m_pEventAdapter->DetachNodeMap();
	//	delete m_pEventAdapter;
	//	m_pEventAdapter = NULL;
	//}
	if( m_psPort )
	{
		delete m_psPort;
		m_psPort = NULL;
	}

	if(m_pDeviceMap)
    {
        m_pDeviceMap->_Destroy();
        delete m_pDeviceMap;
        m_pDeviceMap = NULL;
    }
}






bool StGenDeviceValue::TLParamsLocked( bool lock )
{
	//▼1.0.0.1062
	if( !m_pDeviceMap ) return false;
	//▲1.0.0.1062
    CIntegerPtr ptrTLParamsLocked = m_pDeviceMap->_GetNode("TLParamsLocked");
    if (ptrTLParamsLocked.IsValid())
    {
        try
        {
            ptrTLParamsLocked->SetValue(lock ? 1 : 0 );
            return true;
        }
        catch (GenICam::GenericException &e)
        {
 			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool StGenDeviceValue::GetGenIntParameter( StString lFeature, int & iData, bool Verify, bool IgnoreCache )
{
	//▼1.0.0.1062
	if( !m_pDeviceMap ) return false;
	//▲1.0.0.1062
	GenApi::INode *ptrINode = m_pDeviceMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	//if( aType!=GenApi::EInterfaceType::intfIInteger ) return false;
	if( aType!=intfIInteger ) return false;

    CIntegerPtr ptrInteger = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
            iData = (int)ptrInteger->GetValue( Verify, IgnoreCache );
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

bool StGenDeviceValue::GetGenIntMinMaxParameter( StString lFeature, int & iMin, int & iMax )
{
	//▼1.0.0.1062
	if( !m_pDeviceMap ) return false;
	//▲1.0.0.1062
	GenApi::INode *ptrINode = m_pDeviceMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=intfIInteger ) return false;

    CIntegerPtr ptrInteger = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
            iMin = (int)ptrInteger->GetMin();
            iMax = (int)ptrInteger->GetMax();
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

bool StGenDeviceValue::SetGenIntParameter( StString lFeature, int iData )
{
	//▼1.0.0.1062
	if( !m_pDeviceMap ) return false;
	//▲1.0.0.1062
	GenApi::INode *ptrINode = m_pDeviceMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=intfIInteger ) return false;

    CIntegerPtr ptrInteger = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
            ptrInteger->SetValue(iData);
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

bool StGenDeviceValue::GetGenEnumParameter( StString lFeature, int & iData, bool Verify, bool IgnoreCache )
{
	//▼1.0.0.1062
	if( !m_pDeviceMap ) return false;
	//▲1.0.0.1062
	GenApi::INode *ptrINode = m_pDeviceMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIEnumeration ) return false;

    CEnumerationPtr ptrEnumeration = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
			iData = (int)ptrEnumeration->GetIntValue(Verify,IgnoreCache);
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

bool StGenDeviceValue::GetGenEnumParameter( StString lFeature, StString & strData, bool Verify, bool IgnoreCache )
{
	if( !m_pDeviceMap )
		return false;

	GenApi::INode *ptrINode = m_pDeviceMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIEnumeration ) return false;

    CEnumerationPtr ptrEnumeration = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
            strData = ptrEnumeration->ToString(Verify,IgnoreCache);
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

//▼1.0.0.28 beta1
bool StGenDeviceValue::GetGenEnumIndexParameter( StString lFeature, INT nIndex, StString & strData, bool Verify, bool IgnoreCache )
{
	if( !m_pDeviceMap )
		return false;

	GenApi::INode *ptrINode = m_pDeviceMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIEnumeration ) return false;

    CEnumerationPtr ptrEnumeration = ptrINode;

    //if(ptrInteger.IsValid())
    {
		INT nCount=0;
        try
        {
			GenApi::NodeList_t aNodeList;
			ptrEnumeration->GetEntries(aNodeList);

			for( NodeList_t::iterator ptr=aNodeList.begin(); ptr!=aNodeList.end(); ptr++ )
			{
				CEnumEntryPtr ptrEnumEntry(*ptr);
				if( ptrEnumEntry->GetAccessMode()!=NI )
				{
					if( nCount==nIndex )
					{
						//strData = (*ptr)->GetName();
						strData = ptrEnumEntry->GetSymbolic();
//TRACE(_T(" @@@@@@@@@@@@@@@@@GetName=%s\n"),strData.GetUnicode());
						return true;
					}
					nCount++;
				}
			}
            return false;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}
//▲1.0.0.28 beta1

bool StGenDeviceValue::SetGenEnumParameter( StString lFeature, int iData )
{
	//▼1.0.0.1062
	if( !m_pDeviceMap ) return false;
	//▲1.0.0.1062
	GenApi::INode *ptrINode = m_pDeviceMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIEnumeration ) return false;

    CEnumerationPtr ptrEnumeration = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
            ptrEnumeration->SetIntValue(iData);
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

bool StGenDeviceValue::SetGenEnumParameter( StString lFeature, StString strData )
{
	//▼1.0.0.1062
	if( !m_pDeviceMap ) return false;
	//▲1.0.0.1062
	GenApi::INode *ptrINode = m_pDeviceMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIEnumeration ) return false;

    CEnumerationPtr ptrEnumeration = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
			gcstring strValue = strData;
//TRACE(_T(" @@@@@@@@@@@@@@@@@ptrEnumeration=%s\n"),strData.GetUnicode());
            ptrEnumeration->FromString(strValue);
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

bool StGenDeviceValue::GetGenFloatParameter( StString lFeature, double & dblData, bool Verify, bool IgnoreCache )
{
	//▼1.0.0.1062
	if( !m_pDeviceMap ) return false;
	//▲1.0.0.1062
	GenApi::INode *ptrINode = m_pDeviceMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIFloat ) return false;

    CFloatPtr ptrFloat = ptrINode;

    {
        try
        {
            dblData = ptrFloat->GetValue( Verify, IgnoreCache );
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

bool StGenDeviceValue::GetGenFloatMinMaxParameter( StString lFeature, double & dblMin, double & dblMax )
{
	//▼1.0.0.1062
	if( !m_pDeviceMap ) return false;
	//▲1.0.0.1062
	GenApi::INode *ptrINode = m_pDeviceMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIFloat ) return false;

    CFloatPtr ptrFloat = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
            dblMin = ptrFloat->GetMin();
            dblMax = ptrFloat->GetMax();
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

bool StGenDeviceValue::SetGenFloatParameter( StString lFeature, double dblData )
{
	//▼1.0.0.1062
	if( !m_pDeviceMap ) return false;
	//▲1.0.0.1062
	GenApi::INode *ptrINode = m_pDeviceMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIFloat ) return false;

    CFloatPtr ptrFloat = ptrINode;

    {
        try
        {
            ptrFloat->SetValue(dblData);
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

bool StGenDeviceValue::GetGenBoolParameter( StString lFeature, bool & bData, bool Verify, bool IgnoreCache )
{
	//▼1.0.0.1062
	if( !m_pDeviceMap ) return false;
	//▲1.0.0.1062
	GenApi::INode *ptrINode = m_pDeviceMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIBoolean ) return false;

    CBooleanPtr ptrBoolean = ptrINode;

    //if(ptrBoolean.IsValid())
    {
        try
        {
            bData = (bool)ptrBoolean->GetValue( Verify, IgnoreCache );
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

bool StGenDeviceValue::SetGenBoolParameter( StString lFeature, bool bData )
{
	//▼1.0.0.1062
	if( !m_pDeviceMap ) return false;
	//▲1.0.0.1062
	GenApi::INode *ptrINode = m_pDeviceMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIBoolean ) return false;

    CBooleanPtr ptrBoolean = ptrINode;

    //if(ptrBoolean.IsValid())
    {
        try
        {
            ptrBoolean->SetValue(bData);
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}


bool StGenDeviceValue::GetGenStringParameter( StString lFeature, StString & strData, bool Verify, bool IgnoreCache )
{
	//▼1.0.0.1062
	if( !m_pDeviceMap ) return false;
	//▲1.0.0.1062
	GenApi::INode *ptrINode = m_pDeviceMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIString ) return false;

	CStringPtr ptrString = ptrINode;
    //if(ptrInteger.IsValid())
    {
        try
        {
            strData = ptrString->ToString( Verify, IgnoreCache );
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}

bool StGenDeviceValue::SetGenStringParameter( StString lFeature, StString &strData )
{
	//▼1.0.0.1062
	if( !m_pDeviceMap ) return false;
	//▲1.0.0.1062
	GenApi::INode *ptrINode = m_pDeviceMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=GenApi::EInterfaceType::intfIString ) return false;

	CStringPtr ptrString = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
			gcstring strValue = strData;
            ptrString->FromString(strValue);
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}


bool StGenDeviceValue::SetGenCommandParameter( StString lFeature )
{
	//▼1.0.0.1062
	if( !m_pDeviceMap ) return false;
	//▲1.0.0.1062
	GenApi::INode *ptrINode = m_pDeviceMap->_GetNode(lFeature.GetAscii());
	if( !ptrINode ) return false;

	GenApi::EInterfaceType aType = ptrINode->GetPrincipalInterfaceType();
	if( aType!=4 ) return false;

    CCommandPtr ptrCommand = ptrINode;

    //if(ptrInteger.IsValid())
    {
        try
        {
			EAccessMode aMode = ptrCommand->GetAccessMode();


            ptrCommand->Execute();
            return true;
        }
        catch (GenICam::GenericException &e)
        {
			TCHAR szTmp[256];
			MultiByteToWideChar(CP_ACP, 0, e.GetDescription(), -1, szTmp, _countof(szTmp) );
			OutputDebugString(szTmp);
			OutputDebugString(_T("\n"));
            return false;
        }
    }
    return false;
}


bool StGenDeviceValue::GetPayloadSize( int & iData )
{

    bool bDefinesPayloadSize = false;
    size_t iPayloadSize = 0;

    // first ask the stream to tell us the payloadsize
 	StInterface *pInterface = m_pDevInfo->GetInterface();
	StGenGCLib* psGCLib = pInterface->GetGenGCLib(); 
    if (IsOpen() && psGCLib->f_DSGetInfo)
    {
        GC_ERROR status;
        INFO_DATATYPE iType = INFO_DATATYPE_UNKNOWN;
        size_t iSize = 0;

		StGenStream * aStream = GetGenStream(0);
		
        iType = INFO_DATATYPE_BOOL8;
        iSize = sizeof(bDefinesPayloadSize);
        status = psGCLib->f_DSGetInfo(aStream->GetHandle(), STREAM_INFO_DEFINES_PAYLOADSIZE, &iType, &bDefinesPayloadSize, &iSize);
        if ((0 == status) && bDefinesPayloadSize)
        {
            iType = INFO_DATATYPE_SIZET;
            iSize = sizeof(iPayloadSize);
            status = psGCLib->f_DSGetInfo(m_vDSList[0]->GetHandle(), STREAM_INFO_PAYLOAD_SIZE, &iType, &iPayloadSize, &iSize);
        }
		
    }

    if (!bDefinesPayloadSize)
    {
		int iSize=0;
       // if (!GetGenIntParameter( "PayloadSize", iSize, false, false ))
        if (!GetGenIntParameter( "PayloadSize", iSize, false, true ))
        {
            //unsigned int width = 0;
            //unsigned int height = 0;
            //unsigned int bpp = 0;
            //if (m_psDev->InquireImageDimension(width, height, bpp))
            //{
            //    iPayloadSize = width * height * bpp;
            //}
        }
		else
		{
			iPayloadSize = iSize;
			bDefinesPayloadSize=true;
		}

    }
	if( bDefinesPayloadSize ) iData = iPayloadSize;
    return bDefinesPayloadSize;

}

bool StGenDeviceValue::Read( DWORD dwAddress, char *pBuffer, size_t size )
{
	bool bReval = false;
	if( m_psPort )
	{
		m_psPort->Read(pBuffer, dwAddress, size);
		bReval = m_psPort->GetLastValue();
	}
    return bReval;
}

bool StGenDeviceValue::Write( DWORD dwAddress, char *pBuffer, size_t size )
{
	bool bReval = false;
	if( m_psPort )
	{
		m_psPort->Write(pBuffer, dwAddress, size);
		bReval = m_psPort->GetLastValue();
	}
    return bReval;
}



StInt32 StGenDeviceValue::GetNumStreams( void )
{
    uint32_t num = 0;
    if (IsOpen())    
    {
 		StInterface *pInterface = m_pDevInfo->GetInterface();
		StGenGCLib* psGCLib = pInterface->GetGenGCLib(); 
        psGCLib->f_DevGetNumDataStreams(m_hDevice, &num);
    }
    return num;
}

StString StGenDeviceValue::GetStreamID( uint32_t index )
{
    StString strID = "";

    if(IsOpen())
    {
		StInterface *pInterface = m_pDevInfo->GetInterface();
		StGenGCLib* psGCLib = pInterface->GetGenGCLib(); 
		   size_t iSize = 0;
        GC_ERROR status = psGCLib->f_DevGetDataStreamID(m_hDevice, index, NULL, &iSize );
        if (0 == status && iSize > 0)
        {
            char* str = new char[iSize];
            status = psGCLib->f_DevGetDataStreamID(m_hDevice, index, str, &iSize );
            strID = str;
            delete[] (str);
        }
    }
    return strID;
}


StResult StGenDeviceValue::CreateStream( void )
{

	StResult lResult = StResult::Code::OK;
	uint32_t numStream = GetNumStreams();
	if( numStream==0 ) return StResult::Code::GENERIC_ERROR;
	StInterface *pInterface = m_pDevInfo->GetInterface();
	StGenGCLib* psGCLib = pInterface->GetGenGCLib(); 

	StStream* pStream=NULL;
	for( uint32_t i=0; i<numStream; i++ )
	{
		pStream = new StStream(psGCLib, GetHandle(), GetStreamID(i));
		if( pStream==NULL )
		{
			//lResult = StResult::Code::GENERIC_ERROR;
			lResult = StResult::Code::NOT_CREATESTREAM;
			break;
		}
		m_vDSList.push_back(pStream);

		//Open処理-----CreateStreamに移動した-------------------------------
		if( !pStream->IsOpen() )
			 pStream->Open();
	}
	return lResult;

}


void StGenDeviceValue::DeleteStream( void )
{
	//StResult lResult = StResult::Code::OK;
	uint32_t numStream = m_vDSList.size();
	if( numStream==0 ) return;
	for( uint32_t i=0; i<numStream; i++ )
	{
		//Open処理-----CreateStreamに移動したので-------------------------------
		if( m_vDSList[i]->IsOpen() )
			 m_vDSList[i]->Close();
		delete m_vDSList[i];
	}
	m_vDSList.clear();
	//return lResult;
}

StInt32 StGenDeviceValue::GetNumStream( void )
{
	return (StInt32)m_vDSList.size();
}


StGenStream * StGenDeviceValue::GetGenStream(StInt32 index)
{
	StGenStream *pStream = NULL;
	if( (unsigned int)index<m_vDSList.size() )
		pStream = m_vDSList[index];
	return pStream;
}


//------------------------------------------------------------------------
//--StDevice----------------------------------------------------------------------
//------------------------------------------------------------------------
//StDevice::StDevice(void)
//{
//}
//
//StDevice::StDevice(void)
//{
//}






//------------------------------------------------------------------------
//--StGenDevice----------------------------------------------------------------------
//------------------------------------------------------------------------
StGenDevice::StGenDevice(void)
{
	StGenDeviceValue *pDevice = new StGenDeviceValue;
	mhData = (void *)pDevice;

}


StGenDevice::~StGenDevice(void)
{
	//DeleteStreamList();
	DevClose();

	if( mhData )
	{
		StGenDeviceValue * pValue = (StGenDeviceValue *)mhData;
		delete pValue;
	}

	//念のため？
	//if( m_pEventAdapter )
	//{
	//	m_pEventAdapter->DetachNodeMap();
	//	delete m_pEventAdapter;
	//}

	//if( m_pGenEvent ) delete m_pGenEvent;

	//if( m_hMutex ) CloseHandle(m_hMutex);

}

StResult StGenDevice::DevOpen( StGenDeviceInfo *aDevInfo )
{
	//StResult lResult = StResult::Code::OK;
	StResult lResult = StResult::Code::GENERIC_ERROR;


    if (!IsOpen())
    {
		StDeviceInfo * pDevInfo = (StDeviceInfo *)aDevInfo; 
		StInterface *pInterface = pDevInfo->GetInterface();
		StGenGCLib* psGCLib = pInterface->GetGenGCLib(); 

		StGenDeviceValue *pDeviceValue = (StGenDeviceValue *)mhData;
//{
//TCHAR szMsg[1024];
//swprintf(szMsg,TEXT("DevOpen pDevInfo = 0x%x\n"),pDevInfo);
//OutputDebugString(szMsg);
//swprintf(szMsg,TEXT("DevOpen pInterface = 0x%x\n"),pInterface);
//OutputDebugString(szMsg);
//swprintf(szMsg,TEXT("DevOpen psGCLib = 0x%x\n"),psGCLib);
//OutputDebugString(szMsg);
//swprintf(szMsg,TEXT("DevOpen pDeviceValue = 0x%x\n"),pDeviceValue);
//OutputDebugString(szMsg);
//}


		DEV_HANDLE hDEV = NULL;
        lResult = StResult::Code::GENERIC_ERROR;
		DEVICE_ACCESS_FLAGS iOpenFlags = DEVICE_ACCESS_EXCLUSIVE;

		GC_ERROR status = psGCLib->f_IFOpenDevice(pInterface->GetHandle(), pDevInfo->GetID().GetAscii(), iOpenFlags, &hDEV);

		if (0 == status)
        {

			pDeviceValue->SetDeviceInfo(pDevInfo);
			pDeviceValue->SetHandle(hDEV);
            // update informations                
			pDeviceValue->QueryInformations();

            bool bReval = pDeviceValue->FetchNodeMap();
			if( bReval )
				lResult = StResult::Code::OK;
			else
				lResult = StResult::Code::GENERIC_ERROR;

			//▼1.0.0.25
			if( !lResult.IsOK() )
			{
				bReval = pDeviceValue->OpenPort();
				if( bReval )
					lResult = StResult::Code::OK;
			}
			//▲1.0.0.25


			//StResult eResult = CreateEvent();
			//if( eResult.IsOK() )
			//{
			//	StartEvent();
			//}

			////DevOpenでCreateStreamsするようにした
			//lResult = CreateStream();

			//if( lResult.IsOK() )
			//	lResult = StResult::Code::OK;
        }
		//else
		//{
		//	lResult = StResult::Code::ABORTED;
		//}
    }
	return lResult;
}

StResult StGenDevice::DevClose( void )
{
	StResult lResult = StResult::Code::OK;
	if (IsOpen())
    {

		StResult lResult = StResult::Code::GENERIC_ERROR;


		//WaitForSingleObject(m_hMutex,INFINITE);
		/*
		//▼1.0.0.28　EventTestTimestamp等のEvent項目をDeviceViewに表示
		//DeregisterをおこなわないとEventAdapter削除時にEventのコールバックがおこなわれてしまう。
		AllDeregister();
		Sleep(100);	//AllDeregisterとdelete m_pEventAdapterをするのに間隔をあける （必要ないかも？？）
		//▲1.0.0.28　EventTestTimestamp等のEvent項目をDeviceViewに表示

		//ExposureEndTimestampコールバックしているとDeregisterしていてもコールバックされるためフラグを作成

		if( m_pEventAdapter )
		{
			m_pEventAdapter->DetachNodeMap();
			delete m_pEventAdapter;
			m_pEventAdapter=NULL;
		}

		if( m_bStart == true )
		{
			StopAcquisition();
		}

		//StopPolling

		DeleteThread();

		//Stream
		StopStream();
		DeleteStream();

		//Event
		StopEvent();
		DeleteEvent();
		*/

 		StGenDeviceValue *pDeviceValue = (StGenDeviceValue *)mhData;
		if( pDeviceValue )
		{
			StDeviceInfo * pDevInfo = (StDeviceInfo *)pDeviceValue->GetDeviceInfo(); 
			if( pDevInfo )
			{
				StInterface *pInterface = pDevInfo->GetInterface();
				if( pInterface )
				{
					StGenGCLib* psGCLib = pInterface->GetGenGCLib(); 
					if( psGCLib )
					{
						GC_ERROR status = psGCLib->f_DevClose(pDeviceValue->GetHandle());
						if( GC_ERR_SUCCESS == status )
						{
							pDeviceValue->SetHandle(NULL);
							lResult = StResult::Code::OK;
						}
					}
				}
			}
			pDeviceValue->ReleaseNodeMap();
			pDeviceValue->InitInformations();
		}
		//ReleaseMutex(m_hMutex);

    }
	return lResult;
}

bool StGenDevice::IsOpen( void )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->IsOpen();
}

const StString StGenDevice::GetVendor() const
{
	if( !mhData ) return "";
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->m_strVendor;
}

const StString StGenDevice::GetModel() const
{
	if( !mhData ) return "";
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->m_strModel;
}

const StString StGenDevice::GetDisplayName() const
{
	if( !mhData ) return "";
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->m_strDisplayName;
}

const StString StGenDevice::GetPortName() const
{
	if( !mhData ) return "";
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->m_strPortName;
}
const StString StGenDevice::GetXMLFileName() const
{
	if( !mhData ) return "";
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->m_strXMLFileName;
}




bool StGenDevice::TLParamsLocked( bool lock )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->TLParamsLocked( lock );
}

bool StGenDevice::GetGenIntParameter( StString lFeature, int & iData, bool Verify, bool IgnoreCache )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->GetGenIntParameter( lFeature, iData, Verify, IgnoreCache );
}

bool StGenDevice::GetGenIntMinMaxParameter( StString lFeature, int & iMin, int & iMax )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->GetGenIntMinMaxParameter( lFeature, iMin, iMax );
}

bool StGenDevice::SetGenIntParameter( StString lFeature, int iData )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->SetGenIntParameter( lFeature, iData );
}

bool StGenDevice::GetGenEnumParameter( StString lFeature, int & iData, bool Verify, bool IgnoreCache )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->GetGenEnumParameter( lFeature, iData, Verify, IgnoreCache );
}

bool StGenDevice::GetGenEnumParameter( StString lFeature, StString & strData, bool Verify, bool IgnoreCache )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->GetGenEnumParameter( lFeature, strData, Verify, IgnoreCache );
}

//▼1.0.0.28 beta1
bool StGenDevice::GetGenEnumIndexParameter( StString lFeature, int nIndex, StString & strData, bool Verify, bool IgnoreCache )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->GetGenEnumIndexParameter( lFeature, nIndex, strData, Verify, IgnoreCache );
}
//▲1.0.0.28 beta1

bool StGenDevice::SetGenEnumParameter( StString lFeature, int iData )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->SetGenEnumParameter( lFeature, iData );
}

bool StGenDevice::SetGenEnumParameter( StString lFeature, StString strData )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->SetGenEnumParameter( lFeature, strData );
}

bool StGenDevice::GetGenFloatParameter( StString lFeature, double & dblData, bool Verify, bool IgnoreCache )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->GetGenFloatParameter( lFeature, dblData, Verify, IgnoreCache );
}

bool StGenDevice::GetGenFloatMinMaxParameter( StString lFeature, double & dblMin, double & dblMax )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->GetGenFloatMinMaxParameter( lFeature, dblMin, dblMax );
}

bool StGenDevice::SetGenFloatParameter( StString lFeature, double dblData )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->SetGenFloatParameter( lFeature, dblData );
}

bool StGenDevice::GetGenBoolParameter( StString lFeature, bool & bData, bool Verify, bool IgnoreCache )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->GetGenBoolParameter( lFeature, bData, Verify, IgnoreCache );
}

bool StGenDevice::SetGenBoolParameter( StString lFeature, bool bData )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->SetGenBoolParameter( lFeature, bData );
}

bool StGenDevice::GetGenStringParameter( StString lFeature, StString & strData, bool Verify, bool IgnoreCache )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->GetGenStringParameter( lFeature, strData, Verify, IgnoreCache );
}

bool StGenDevice::SetGenStringParameter( StString lFeature, StString & strData )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->SetGenStringParameter( lFeature, strData );
}


bool StGenDevice::SetGenCommandParameter( StString lFeature )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->SetGenCommandParameter( lFeature );
}


bool StGenDevice::GetPayloadSize( int & iData )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->GetPayloadSize( iData );
}

StResult StGenDevice::CreateStream( void )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->CreateStream();
}
void StGenDevice::DeleteStream( void )
{
	if( !mhData ) return;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->DeleteStream();
}
StInt32 StGenDevice::GetNumStream( void )
{
	if( !mhData ) return 0;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->GetNumStream();
}


StGenStream * StGenDevice::GetGenStream(StInt32 index)
{
	if( !mhData ) return NULL;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->GetGenStream(index);
}

bool StGenDevice::Read( DWORD dwAddress, char *pBuffer, size_t size )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->Read( dwAddress, pBuffer, size );
}

bool StGenDevice::Write( DWORD dwAddress, char *pBuffer, size_t size )
{
	if( !mhData ) return false;
	StGenDeviceValue *pDevice = (StGenDeviceValue *)mhData;
	return pDevice->Write( dwAddress, pBuffer, size );
}





/*
void StGenDevice::FetchNodeMap( void )
{
    GC_ERROR status;
    PORT_HANDLE hRemoteDevicePort = NULL;

    m_pDeviceMap = new GenApi::CNodeMapRef;

    //retrieve access to device 
    status = m_psGCLib->f_DevGetPort( m_hDEV, &hRemoteDevicePort);
    bool bReval = GetNodeMap(hRemoteDevicePort, *m_pDeviceMap);
	if( !bReval )
	{
		delete m_pDeviceMap;
		m_pDeviceMap = NULL;
	}
}



void StGenDevice::ReleaseNodeMap( void )
{
	if( m_pEventAdapter )
	{
		m_pEventAdapter->DetachNodeMap();
		delete m_pEventAdapter;
		m_pEventAdapter = NULL;
	}
	//▼1.0.0.28　EventTestTimestamp等のEvent項目をDeviceViewに表示
	//if(m_pEventMap)
	//{
	//	m_pEventMap->_Destroy();
	//	delete m_pEventMap;
	//	m_pEventMap = NULL;
	//}
	//▲1.0.0.28　EventTestTimestamp等のEvent項目をDeviceViewに表示
	if( m_psPort )
	{
		delete m_psPort;
		m_psPort = NULL;
	}

	if(m_pDeviceMap)
    {
        m_pDeviceMap->_Destroy();
        delete m_pDeviceMap;
        m_pDeviceMap = NULL;
    }
}

StResult StGenDevice::StartAcquisition( void )
{

 	StResult lResult = StResult::Code::NOT_STARTACQUISITION;

	bool bReval = SetGenCommandParameter( "AcquisitionStart" );

	if( bReval ) lResult = StResult::Code::OK;

	//else
	//{
	//	//Portで直
	//	GenICam::Client::PORT_HANDLE hRemoteDevicePort = NULL;
	////GC_ERROR status = 
	//	m_psGCLib->f_DevGetPort( m_hDEV, &hRemoteDevicePort);
	//	StGenPort *pPortImpl = new StGenPort(m_psGCLib, hRemoteDevicePort);


	//	unsigned int uiAddress=0x11000004;
	//	unsigned int uiData=1;
	//	//pPortImpl->Read( &uiData, uiAddress, sizeof(uiData) );
	//	pPortImpl->Write( &uiData, uiAddress, sizeof(uiData) );

	//	bool bValue = pPortImpl->GetLastValue();

	//	delete pPortImpl;

	//	if( bValue )
	//		lResult = StResult::Code::OK;
	//}

	if( lResult.IsOK() )
	{
		m_bStart = true;
	}

	return lResult;
}


StResult StGenDevice::StopAcquisition( void )
{
	StResult lResult = StResult::Code::GENERIC_ERROR;

	bool bReval = SetGenCommandParameter( "AcquisitionStop" );

	if( bReval ) lResult = StResult::Code::OK;


	//else
	//{
	//	//Portで直
	//	GenICam::Client::PORT_HANDLE hRemoteDevicePort = NULL;
	////GC_ERROR status = 
	//	m_psGCLib->f_DevGetPort( m_hDEV, &hRemoteDevicePort);
	//	StGenPort *pPortImpl = new StGenPort(m_psGCLib, hRemoteDevicePort);


	//	unsigned int uiAddress=0x11000004;
	//	unsigned int uiData=0;
	//	//pPortImpl->Read( &uiData, uiAddress, sizeof(uiData) );
	//	pPortImpl->Write( &uiData, uiAddress, sizeof(uiData) );

	//	bool bValue = pPortImpl->GetLastValue();

	//	delete pPortImpl;

	//	if( bValue ) lResult = StResult::Code::OK;
	//}


	if( lResult.IsOK() )
	{
		m_bStart = false;
	}

	return lResult;
}

StResult StGenDevice::CreateStream(void)
{
	StResult lResult = StResult::Code::OK;
	uint32_t numStream = GetNumStreams();
	if( numStream==0 ) return StResult::Code::GENERIC_ERROR;
	StGenStream* pStream=NULL;
	for( uint32_t i=0; i<numStream; i++ )
	{
		pStream = new StGenStream(m_psGCLib, this, GetStreamID(i));
		if( pStream==NULL )
		{
			//lResult = StResult::Code::GENERIC_ERROR;
			lResult = StResult::Code::NOT_CREATESTREM;
			break;
		}
		m_vDSList.push_back(pStream);

		//Open処理-----CreateStreamに移動した-------------------------------
		if( !pStream->IsOpen() )
			 pStream->Open();
	}
	return lResult;
}

StResult StGenDevice::DeleteStream(void)
{
	StResult lResult = StResult::Code::OK;
	uint32_t numStream = m_vDSList.size();
	if( numStream==0 ) return true;
	for( uint32_t i=0; i<numStream; i++ )
	{
		//Open処理-----CreateStreamに移動したので-------------------------------
		if( m_vDSList[i]->IsOpen() )
			 m_vDSList[i]->Close();
		delete m_vDSList[i];
	}
	m_vDSList.clear();
	return lResult;
}

StResult StGenDevice::StartStream(void)
{
	StResult lResult = StResult::Code::NOT_STARTSTREM;
	uint32_t numStream = m_vDSList.size();
	if( numStream==0 ) return lResult;
	for( uint32_t i=0; i<numStream; i++ )
	{
		bool bOpen = true;
		//Open処理-----StreamOpenに移動-------------------------------
		if (m_vDSList[i]->IsOpen() )
		//if (m_vDSList[i]->IsOpen() || m_vDSList[i]->Open())
		{
		}
		else{
			bOpen=false;
		   //break;
		}

		bool bStart = false;
		if( bOpen )
		{
			//StartStream----------------------------------------
			bStart = m_vDSList[i]->StartStream();
			//if( !bStart ) return lResult;
		}

		//Error時のClose処理---------------------------
		if( !bOpen || !bStart)
		{
			//for( uint32_t i=0; i<numStream; i++ )
			//{
			//	if (m_vDSList[i]->IsOpen())
			//	{
			//		m_vDSList[i]->Close();
			//	}
			//}
			return lResult;
		}

	}
	lResult = StResult::Code::OK;
    return lResult;
}

StResult StGenDevice::StopStream(void)
{
	StResult lResult = StResult::Code::OK;
	uint32_t numStream = m_vDSList.size();
	if( numStream==0 ) return lResult;

	for( uint32_t i=0; i<numStream; i++ )
	{
		//m_vDSList[i]->StopAcquisition();
	}
	for( uint32_t i=0; i<numStream; i++ )
	{
		m_vDSList[i]->StopStream();
		//if (m_vDSList[i]->IsOpen())
		//{
		//	m_vDSList[i]->Close();
		//}
	}
	return lResult;
}

uint32_t StGenDevice::GetNumStreams( void )
{
    uint32_t num = 0;
    if (IsOpen())    
    {
        m_psGCLib->f_DevGetNumDataStreams(m_hDEV, &num);
    }
    return num;
}

StString StGenDevice::GetStreamID( uint32_t index )
{
    StString strID = "";

    if(IsOpen())
    {
        size_t iSize = 0;
        GC_ERROR status = m_psGCLib->f_DevGetDataStreamID(m_hDEV, index, NULL, &iSize );
        if (0 == status && iSize > 0)
        {
            char* str = new char[iSize];
            status = m_psGCLib->f_DevGetDataStreamID(m_hDEV, index, str, &iSize );
            strID = str;
            delete[] (str);
        }
    }
    return strID;
}
*/
#if 1

/*
DWORD GetUnzipSize( char *pZip )
{
	DWORD dwSize = 0;
	UINT *pHeader = (UINT *)pZip;
	if( *pHeader==0x04034b50 )	//ZIPローカルファイルヘッダ
	{
		PDWORD pdwSize = (PDWORD)&pZip[22];
		dwSize = *pdwSize;
	}
	//else
	//if( *pHeader==0x02014b50 )	//ZIPセントラルディレクトリファイルヘッダ
	//{
	//	PDWORD pdwSize = (PDWORD)&pZip[24];
	//	dwSize = *pdwSize;
	//}

	return dwSize;
}


// [4] Uncompress (zlib)
size_t Uncompress_zlib( void* dest_memory, size_t dest_size, const void* src_memory, size_t src_size )
{
   z_stream  stream;
   memset( &stream, 0, sizeof(z_stream) );
   stream.next_in= (Bytef*)src_memory;
   stream.avail_in= src_size;
   stream.next_out= (Bytef*)dest_memory;
   stream.avail_out= dest_size;

   inflateInit2( &stream, MAX_WBITS );
   int result= inflate( &stream, Z_FINISH );
   //if( result != ZSTREAM_END && result != Z_OK )
   if( result != Z_OK )
   {
      // error
      return  0;
   }
   inflateEnd( &stream );

   assert( dest_size == stream.total_out );

   return  stream.total_out;  // uncompressed_size
}

*/

/**
 * 圧縮されているデータを伸長.
 */
/*
int
decompress(
        unsigned char *src,
        unsigned long srcSize,
        unsigned char *dest,
        unsigned long destMaxSize,
        unsigned long *destSize)
{
    z_stream z;
 
    int status = Z_OK;
 
    // メモリ管理はZLIBに任せる
    z.zalloc = Z_NULL;
    z.zfree = Z_NULL;
    z.opaque = Z_NULL;
 
    z.next_in = Z_NULL;
    z.avail_in = 0;
 
    //status = inflateInit2(&z, MAX_WBITS + 32);
    status = inflateInit2(&z, -MAX_WBITS);
    //status = inflateInit2(&z, MAX_WBITS + 16);
    //status = inflateInit2(&z, MAX_WBITS);
    if (status != Z_OK) {
 //       debugs_format(99, 5, "inflateInit: %s\n", (z.msg) ? z.msg : "???");
        return status;
    }
 
    // 入力データを設定
    z.next_in = src;
    z.avail_in = srcSize;
 
    // 出力先を設定
    z.next_out = dest;
    z.avail_out = destMaxSize;
 
    // 伸長（展開）
    status = inflate(&z, Z_NO_FLUSH);
    // 成功ではない、またはデータの最後まで読み取れていない場合は失敗
    if (status != Z_OK && status != Z_STREAM_END) {
 //       debugs_format(99, 5, "inflate: %s\n", (z.msg) ? z.msg : "???");
        return status;
    }
 
    // 出力サイズを設定
    *destSize = z.total_out;
 
    // 終了処理
    status = inflateEnd(&z);
    if (status != Z_OK) {
 //       debugs_format(99, 5, "inflateEnd: %s\n", (z.msg) ? z.msg : "???");
        return status;
    }
 
    // 出力サイズを設定
    *destSize = z.total_out;
 
    return status;
} 




*/






// ---------------------------------------------------------------------------
/// \brief Retrieve the GenICam xml from a given port
///
/// \param [in]     hPort           Port to read from
///
/// \return string containing the XML
// ---------------------------------------------------------------------------
/*
gcstring StGenDevice::RetrieveXML(PORT_HANDLE hPort, gcstring &gcstrXml)
{
	gcstrXml = "";
	if( m_psGCLib==NULL ) return "";
    if (0 != hPort)
    {
        bool8_t bPortNotImplemented = false;
        INFO_DATATYPE dataType = INFO_DATATYPE_BOOL8;
        size_t iSize = sizeof(bPortNotImplemented);
        if ((GC_ERR_SUCCESS != m_psGCLib->f_GCGetPortInfo(hPort, PORT_INFO_ACCESS_NI, &dataType, (void*)&bPortNotImplemented, &iSize))
            || bPortNotImplemented )
        {
            //HandleError("Port not implemented!");
            return "";
        }

        size_t iUrlLength = 0;
        if ((GC_ERR_SUCCESS == m_psGCLib->f_GCGetPortURL(hPort, NULL, &iUrlLength)) && (iUrlLength > 0))
        {
            char* szUrl = new char[iUrlLength];
            if (GC_ERR_SUCCESS == m_psGCLib->f_GCGetPortURL(hPort, szUrl, &iUrlLength))
            {
                // Read XML Address
                std::string strXMLAddress = szUrl;
                delete[] (szUrl);

                std::string strLocation  = strXMLAddress.substr(0, 6);
                std::transform(strLocation.begin(), strLocation.end(), strLocation.begin(), (int(*)(int)) toupper);
                std::string strXMLFilename;

                if ("LOCAL:" == strLocation)
                {
                    size_t iOffset = 6;
                    size_t iLength = (size_t)strXMLAddress.find(";", iOffset);
                    std::string strFilename = strXMLAddress.substr(iOffset, iLength - iOffset);

                    iOffset = iLength+1;
                    iLength = strXMLAddress.find(";", iOffset);
                    char *pEnd = 0;
                    uint64_t iAddr = _strtoui64(strXMLAddress.substr(iOffset, iLength-iOffset).c_str(), &pEnd, 16);

                    iOffset = iLength+1;
                    iLength = strXMLAddress.size();
                    size_t iXMLSize = (size_t)_strtoui64(strXMLAddress.substr(iOffset, iLength-iOffset).c_str(), &pEnd, 16);
//sentech
					size_t iReadXMLSize = iXMLSize;
					if(iReadXMLSize & 0x03)
					{
						iReadXMLSize += 4 - (iReadXMLSize & 0x03);
					}
                    iReadXMLSize &= 0xFFFFFFFC;  // make sure we read from a dword boundary

                    void *pXML = malloc(iReadXMLSize + 1);
					memset( pXML, 0, iReadXMLSize + 1 );

                    if (GC_ERR_SUCCESS != m_psGCLib->f_GCReadPort(hPort, iAddr, pXML, &iReadXMLSize))
                    {
                        //HandleError("could not read from port!");
                        return "";
                    }

					gcstrXml="";

					//zipなら解凍
					//unsigned int loc = strFilename.find( ".zip", 0 );
					if( strFilename.find( ".zip", 0 ) != std::string::npos )
					{


#ifdef __TEMP_ZIP

						char fileFullpathName[MAX_PATH];//, TargetPath[MAX_PATH];

						//strcpy_s( TargetPath, sizeof(TargetPath)/ sizeof(TargetPath[0]), "./"); //PATHを指定

						TCHAR tchrEnvVariable[MAX_PATH];
						TCHAR *pEnv[] = {_T("TEMP"),_T("TMP")};
						DWORD dwResult=0;
						for( int i=0; i<_countof(pEnv) && dwResult==0 ; i++ )
						{
							dwResult = GetEnvironmentVariable(
									pEnv[i],
									tchrEnvVariable,
									sizeof(tchrEnvVariable)
									);
						}
						
						if( dwResult==0 ) swprintf( tchrEnvVariable, MAX_PATH, _T(".") );
						_tcscat_s( tchrEnvVariable, sizeof(tchrEnvVariable)/ sizeof(tchrEnvVariable[0]), _T("\\") );
						//_tcscat_s( tchrEnvVariable, sizeof(tchrEnvVariable)/ sizeof(tchrEnvVariable[0]), _T("/") );

						StString aTargetPath = tchrEnvVariable;
						strcpy_s( fileFullpathName, sizeof(fileFullpathName)/ sizeof(fileFullpathName[0]), aTargetPath.GetAscii()); //PATHを指定
						strcat_s( fileFullpathName, sizeof(fileFullpathName)/ sizeof(fileFullpathName[0]), strFilename.c_str() );

						FILE *fp;
						do
						{
							//念のため削除
							StString aZipPathName = fileFullpathName;
							DeleteFile(aZipPathName.GetUnicode());

							errno_t errRet = fopen_s( &fp, fileFullpathName, "wb" );
							if( fp==NULL ) break;

							//fwrite( pbyteXML, 1, dwFileSize, fp );  
							fwrite( pXML, iXMLSize, 1, fp );  
							fclose( fp );
							char szTmp[MAX_PATH];

							PBYTE pbyteXML = (PBYTE)pXML;

							char fileUnzipName[MAX_PATH];	//解凍されたフルパスファイル名
							WORD namelentgh = *(PWORD)&pbyteXML[0x1A];
							memcpy( szTmp, &pbyteXML[0x1E], namelentgh );
							szTmp[namelentgh] = 0;
							strcpy_s( fileUnzipName, sizeof(fileUnzipName)/ sizeof(fileUnzipName[0]), aTargetPath.GetAscii());
							strcat_s( fileUnzipName, sizeof(fileUnzipName)/ sizeof(fileUnzipName[0]), szTmp );

							std::string strZipFilename = fileFullpathName;
							std::string strTargetPath = aTargetPath.GetAscii();
							//念のため削除
							StString aTargetPathName = fileUnzipName;
							DeleteFile(aTargetPathName.GetUnicode());

							BOOL bReval = Unzip(strZipFilename, strTargetPath);

							fopen_s( &fp, fileUnzipName, "rb" );
							if( fp )
							{
								fseek(fp, 0, SEEK_END);
								fpos_t fsize;
								fgetpos(fp, &fsize); 
								fseek(fp, 0, SEEK_SET);

								PBYTE pBuff = new BYTE [(unsigned int)fsize+1];
								if( pBuff )
								{
									pBuff[fsize]=0;	//最後のNULL必要
									unsigned long origsize = fread(pBuff, 1, (size_t)fsize, fp); // データを読み込む
									if( origsize==fsize )
									{
										gcstrXml = (char *)pBuff;
									}
									delete [] pBuff;
								}

								fclose( fp );
							}
							//StString aTargetPathName = fileUnzipName;
							DeleteFile(aTargetPathName.GetUnicode());
							//StString aZipPathName = strZipFilename.c_str();
							DeleteFile(aZipPathName.GetUnicode());

						}while(0);
#else

						std::string szTmp = (char *)pXML;

						//BOOL bReval = GenTLZip::UncompressGenICamXML( szTmp );
						BOOL bReval = GenTLZip::UncompressGenICamXML( szTmp, (char *)pXML );
						if( bReval )
						{
						//	CString aTmp = szTmp;
							gcstrXml = szTmp.c_str();
						}
						else
						{
							return "";

						}
#endif
					}
					else
					{
						//xmlならそのままコピー
						gcstrXml = (char *)pXML;
					}

//

                    free(pXML);

                    return gcstrXml;
                }
                else
                {
                    //cerr << "Not a local address!" << endl;
                    return "";
                }
            }
            else
            {
                delete[] (szUrl);
                //HandleError("Could not query port URL!");
                return "";
            }
        }
        else
        {
            //HandleError("Could not query size needed for port URL!");
            return "";
        }
    }
    else
    {
        //cerr << "Port not initialized!" << endl;
        return "";
    }
}




struct featureName{
    StString 	aName;
	unsigned int	iID;
	unsigned int	nIndex;
};

GenApi::CNodePtr StGenDevice::GetSortNodePtr(NodeList_t& aNodes, unsigned int index, CNodeMapRef& rNodeMap)
{
	
	unsigned int nodeNum = aNodes.size();
	if( nodeNum<=index ) return NULL;

	featureName *fname = new featureName[nodeNum];
	if( fname==NULL ) return NULL;

	for( unsigned int i=0; i<nodeNum; i++ )
	{
		fname[i].aName = aNodes[i]->GetName();
		fname[i].iID = -1;
		fname[i].nIndex = i;
	}

	NodeList_t allNodes;
	rNodeMap._GetNodes(allNodes);
	unsigned int allNum = allNodes.size();
	unsigned int iCount=0;
	for( unsigned int a=0; a<allNum; a++ )
	{
		for( unsigned int i=0; i<nodeNum; i++ )
		{
			if( fname[i].aName==allNodes[a]->GetName() )
			{
				fname[i].iID = a;
				iCount++;
				break;
			}
		}
		if( iCount>=nodeNum ) break;
	}

	//Sort
	featureName dammy;
	for( unsigned int i=0; i<nodeNum-1; i++ )
	{
		for( unsigned int j=i+1; j<nodeNum; j++ )
		{
			if( fname[i].iID>fname[j].iID )
			{
				dammy = fname[i];
				fname[i] = fname[j];
				fname[j] = dammy;
			}
		}
	}

	//return;
	unsigned int aIndex = fname[index].nIndex;
	delete [] fname;
	if( aIndex>=nodeNum ) return NULL;
	GenApi::CNodePtr pNode = aNodes[aIndex];
	return pNode;

}

GenApi::CNodePtr StGenDevice::GetSortNodePtr(NodeList_t& aNodes, unsigned int index)
{
	if( m_pDeviceMap==NULL) return NULL;
	unsigned int nodeNum = aNodes.size();
	if( nodeNum<=index ) return NULL;

	featureName *fname = new featureName[nodeNum];
	if( fname==NULL ) return NULL;

	for( unsigned int i=0; i<nodeNum; i++ )
	{
		fname[i].aName = aNodes[i]->GetName();
		fname[i].iID = -1;
		fname[i].nIndex = i;
	}

	NodeList_t allNodes;
	m_pDeviceMap->_GetNodes(allNodes);
	unsigned int allNum = allNodes.size();
	unsigned int iCount=0;
	for( unsigned int a=0; a<allNum; a++ )
	{
		for( unsigned int i=0; i<nodeNum; i++ )
		{
			if( fname[i].aName==allNodes[a]->GetName() )
			{
				fname[i].iID = a;
				iCount++;
				break;
			}
		}
		if( iCount>=nodeNum ) break;
	}

	//Sort
	featureName dammy;
	for( unsigned int i=0; i<nodeNum-1; i++ )
	{
		for( unsigned int j=i+1; j<nodeNum; j++ )
		{
			if( fname[i].iID>fname[j].iID )
			{
				dammy = fname[i];
				fname[i] = fname[j];
				fname[j] = dammy;
			}
		}
	}


	//return;
	unsigned int aIndex = fname[index].nIndex;
	delete [] fname;
	if( aIndex>=nodeNum ) return NULL;
	GenApi::CNodePtr pNode = aNodes[aIndex];
	return pNode;

}

*/

// ---------------------------------------------------------------------------
/// \brief Load xml from Client::port and instantiate the nodemap
///
/// \param [in] hPort Retrieve the nodemap from the provided port implementation
///
/// \return Nodemap, NULL in case of failure
// ---------------------------------------------------------------------------
//bool StGenDevice::GetNodeMap(PORT_HANDLE hPort, CNodeMapRef& rNodeMap, PORT_HANDLE hEventPort, CNodeMapRef& rEventNodeMap)

/*
bool StGenDevice::GetNodeMap(PORT_HANDLE hPort, CNodeMapRef& rNodeMap)
{
	bool bReval=true;

	if( m_psPort ) return false;


	//StGenPort *pPortImpl = NULL;
    if (0 != hPort)
    {
        //try
		do
        {
            gcstring strXML;
			RetrieveXML(hPort, strXML);
            if (strXML.size())
            {
                rNodeMap._LoadXMLFromString(strXML);
			}
            else
            {
                bReval = false;
            }
			
			if( bReval==false ) break;
            //Connect to the port
            //CPort *pPortImpl = new CPort(this, hPort);

            m_psPort = new StGenPort(m_psGCLib, hPort);
			if( !m_psPort ) bReval = false;
			if( bReval==false ) break;
 
			GenApi::IPort* pGenApiPort = dynamic_cast<GenApi::IPort *>(m_psPort);

            size_t iSize = 0;
            INFO_DATATYPE iType = INFO_DATATYPE_UNKNOWN;
            if (GC_ERR_SUCCESS == m_psGCLib->f_GCGetPortInfo(hPort, PORT_INFO_PORTNAME, &iType, NULL, &iSize))
            {
                char* szPortName = new char[iSize];
                if (GC_ERR_SUCCESS == m_psGCLib->f_GCGetPortInfo(hPort, PORT_INFO_PORTNAME, &iType, szPortName, &iSize))
                {
                    gcstring gcstrPortName = szPortName;
                    //delete[] (szPortName);
                    bReval = rNodeMap._Connect(pGenApiPort, gcstrPortName);
//                    bReval = rNodeMap._Connect(pGenApiPort);
                }
                else
                {
                    //delete[] (szPortName);
                    //HandleError("Could not query port name!");
                    bReval = false;
                }
				if( szPortName ) delete[] (szPortName);
				
            }
            else
            {
                //HandleError("Could not query size needed for port name!");
                bReval = false;
            }      

			//▼1.0.0.28　EventTimestampをDeviceViewに表示させる
			if( bReval )
			{
				if( m_pEventAdapter ){
					//m_pEventAdapter->DetachNodeMap();
					delete m_pEventAdapter;
					m_pEventAdapter = NULL;
				}

	            m_pEventAdapter =
#ifdef EVENTADAPTERU3V
	//				new CEventAdapterU3V(rNodeMap._Ptr);
#else
					//new CEventAdapterGeneric(m_pEventMap->_Ptr);
					new CEventAdapterGeneric(rNodeMap._Ptr);
					//new CEventAdapterGeneric;
#endif
				//DeviceでConnectするとEventとしてつかえなくなる？ためm_pEventMapを使用した
				//m_pEventAdapter->AttachNodeMap(m_pEventMap->_Ptr ); // attach the same nodemap again, should not break anything
//				m_pEventAdapter->AttachNodeMap(rNodeMap._Ptr ); // attach the same nodemap again, should not break anything
			}
			//▲1.0.0.28　EventTimestampをDeviceViewに表示させる

		}while(0);
   //     catch (GenericException &E)
   //     {
   //         //cerr << "Error " << E.GetDescription() << endl;
			//bReval =  false;
   //     }
    }
    else
    {
        //cerr << "Port not initialized!" << endl;
        bReval = false;
    }
	if( bReval==false )
	{
		if( m_psPort ) delete m_psPort;
		m_psPort = NULL;
	}
	return bReval;
}


CNodeMapRef* StGenDevice::GetNodeMap( PORT_HANDLE hPort )
{
    CNodeMapRef* NodeMap = new CNodeMapRef;
    if (GetNodeMap(hPort, *NodeMap))
    {
        return NodeMap;
    }
    else
    {
        //cerr << "Could not initialized!" << endl;
        return 0;
    }
}
*/
#endif




/*
StResult StGenDevice::CreateEvent(void)
{
	if( m_pGenEvent ) return StResult::Code::ABORTED;
	StResult lResult = StResult::Code::OK;

	//m_pGenEvent = new StGenEvent(m_psGCLib, this, "", EVENT_TYPE_LIST::EVENT_FEATURE_DEVEVENT, EventCallback);

#ifdef EVENT_FEATURE_DEVEVENT
	m_pGenEvent = new StGenEvent(m_psGCLib, this, "", EVENT_TYPE_LIST::EVENT_FEATURE_DEVEVENT, NULL, NULL);
#else
	m_pGenEvent = new StGenEvent(m_psGCLib, this, "", EVENT_TYPE_LIST::EVENT_REMOTE_DEVICE, NULL, NULL);
#endif
	if( !m_pGenEvent ) 
		lResult = StResult::Code::ABORTED;

	return lResult;
}

StResult StGenDevice::DeleteEvent(void)
{
	StResult lResult = StResult::Code::OK;
	if( m_pGenEvent )
	{
		delete m_pGenEvent;
		m_pGenEvent = NULL;
	}
	return lResult;
}

StResult StGenDevice::StartEvent(void)
{
	if( !m_pGenEvent ) return StResult::Code::ABORTED;
	StResult lResult = StResult::Code::OK;

	bool bReval = m_pGenEvent->StartEvent();
	if( !bReval ) return StResult::Code::ABORTED;

	return lResult;
}

StResult StGenDevice::StopEvent(void)
{
	if( !m_pGenEvent ) return StResult::Code::ABORTED;
	StResult lResult = StResult::Code::OK;

	m_pGenEvent->StopEvent();

	return lResult;
}


//void StGenDevice::EventCallback(char *pBuffer, unsigned int aSize)
//{
	//if( !m_pGenEvent ) return StResult::Code::ABORTED;
	//StResult lResult = StResult::Code::OK;

	//m_pGenEvent->StopEvent();

	//return lResult;
//}

//void StGenDevice::SetEventCallback(void (*nFuncCallback)(void *pVoid, char *pBuffer, unsigned int aSize), void *pUserPointer)
void StGenDevice::SetEventCallback(void (*nFuncCallback)(void *pVoid, void *pEvent), void *pClassPointer, void *pUserPointer)
{

	 //m_nFuncCallback = nFuncCallback;
	m_pGenEvent->SetEventCallback(nFuncCallback, pClassPointer, pUserPointer);
}

//-----Adapter-------------------------------------
void StGenDevice::SetEventAdapterCallback(void (*nFuncCallback)(void *pVoid, void *pEvent), void *pClassPointer, void *pUserPointer)
{

	 //m_nFuncCallback = nFuncCallback;
	m_pGenEvent->SetEventAdapterCallback(nFuncCallback, pClassPointer, pUserPointer);
}



//void StGenDevice::SetEventCallbackDevCon(void (*nFuncCallback)(void *pVoid, void *pEvent), void *pUserPointer)
//{
//
//	 //m_nFuncCallback = nFuncCallback;
//	m_pGenEvent->SetEventCallbackDevCon(nFuncCallback, pUserPointer);
//}

BOOL StGenDevice::CreateThread(void)
{
	BOOL bReval=FALSE;
	if( m_lThreadDisplay==NULL )
	{
		StGenStream *pStream = GetGenStream(0);
		if( pStream )
		{
			CaptureThread *lThreadDisplay = new CaptureThread( GetDocument(), pStream );
			SetCaptureThread(lThreadDisplay);
			// Start threads
			lThreadDisplay->Start();
			lThreadDisplay->SetPriority( THREAD_PRIORITY_ABOVE_NORMAL );
			bReval=TRUE;
		}
	}
	return TRUE;
}

void StGenDevice::DeleteThread(void)
{
	if( m_lThreadDisplay ){

		m_lThreadDisplay->Stop();
		m_lThreadDisplay->IsDone(INFINITE);
		//Sleep(1000);
		delete m_lThreadDisplay;
		m_lThreadDisplay = NULL;
	}
}

//void StGenDevice::SetResister(StString aFeature)
//{
//    CNodePtr ptrMyPort = Camera._GetNode(aFeature.GetAscii());
//	GenApi::Register( ptrMyPort, CallbackTarget3a, &CCallbackTarget::UpdateFeature );
//}
*/