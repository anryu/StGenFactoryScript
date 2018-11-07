#include "StdAfx.h"
#include "StGenGCLib.h"


//#include <iostream>
//#include <fstream>
#include <algorithm>
#include "../zlib/include/zlib.h"

#ifdef _WIN64

#else //_WIN64
//#pragma comment(lib,"../zlib/lib/zlib.lib")
#endif //_WIN64



StGenGCLib::StGenGCLib(StString ctiName)
	: m_ctiName(ctiName)
	, m_hGCLib(NULL)
//	, lastError(GC_ERR_SUCCESS)
//	, hSystem(NULL)
{
	GCOpen();
}


StGenGCLib::~StGenGCLib(void)
{
	GCClose();
}

//GC_ERROR StGenGCLib::GetLastError( GC_ERROR * piErrorCode, char * sErrorText, size_t * piSize )
//{
//	//return GetLastError( piErrorCode, sErrorText, piSize );
//	return lastError;
//}

StResult StGenGCLib::GCOpen( void )
{
	//TCHAR szTmp[256];
	//MultiByteToWideChar(CP_ACP, 0, m_strPathName.c_str(), -1, szTmp, _countof(szTmp) );
	HMODULE hLib = NULL;
    if (!IsLoaded())
        hLib = LoadLibrary(m_ctiName.GetUnicode());
        //hLib = LoadLibrary(m_strPathName.c_str());
    
    if (!hLib)
        return StResult::Code::FILE_ERROR;

    m_hGCLib = hLib;

    if (LinkFunctions())
    {
        GC_ERROR status = f_GCInitLib();
		if( 0 != status ) return StResult::Code::CANNOT_OPEN_FILE;
    }
	return StResult::Code::OK;

}

bool StGenGCLib::LinkFunctions( void )
{
    unsigned int tried = 0;
    unsigned int made = 0;

    if (!IsLoaded())
        return false;

#define GC_SYMBOL_LINK(name) \
    tried++; \
    if ((f_##name = (GC_##name)GetProcAddress(m_hGCLib, #name)) \
    != NULL) \
    made++;
#define DECLARE(params)
#include TL_FUNCTION_DEFINITION
#undef DECLARE
#undef GC_SYMBOL_LINK

    return tried == made;
}

void StGenGCLib::UnlinkFunctions( void )
{
#define GC_SYMBOL_LINK(name) \
    f_##name = NULL;
#define DECLARE(params)
#include TL_FUNCTION_DEFINITION
#undef DECLARE
#undef UI_SYMBOL_LINK
}

StResult StGenGCLib::GCClose( void )
{
	if( m_hGCLib ) 
	{
        GC_ERROR status = f_GCCloseLib();
		if( 0 != status ) return StResult::Code::GENERIC_ERROR;
		m_hGCLib = NULL;
	}

	return StResult::Code::OK;
}

StResult StGenGCLib::GCGetInfo( TL_INFO_CMD iInfoCmd,INFO_DATATYPE * piType,void * pBuffer,size_t * piSize)
{
	GC_ERROR status = f_GCGetInfo ( iInfoCmd,piType,pBuffer,piSize );
	if( 0 != status ) return StResult::Code::GENERIC_ERROR;
	return StResult::Code::OK;
}

StResult StGenGCLib::GetLastError( GC_ERROR * piErrorCode, char * sErrorText, size_t * piSize )
{
	GC_ERROR status = f_GCGetLastError( piErrorCode,sErrorText,piSize );
	if( 0 != status ) return StResult::Code::GENERIC_ERROR;
	return StResult::Code::OK;
}


#if 1

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



/**
 * 圧縮されているデータを伸長.
 */
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
gcstring StGenGCLib::RetrieveXML(PORT_HANDLE hPort, gcstring &gcstrXml)
{
	gcstrXml = "";
	//if( m_psGCLib==NULL ) return "";
    if (0 != hPort)
    {
        bool8_t bPortNotImplemented = false;
        INFO_DATATYPE dataType = INFO_DATATYPE_BOOL8;
        size_t iSize = sizeof(bPortNotImplemented);
        if ((GC_ERR_SUCCESS != f_GCGetPortInfo(hPort, PORT_INFO_ACCESS_NI, &dataType, (void*)&bPortNotImplemented, &iSize))
            || bPortNotImplemented )
        {
            //HandleError("Port not implemented!");
            return "";
        }

        size_t iUrlLength = 0;
        if ((GC_ERR_SUCCESS == f_GCGetPortURL(hPort, NULL, &iUrlLength)) && (iUrlLength > 0))
        {
            char* szUrl = new char[iUrlLength];
            if (GC_ERR_SUCCESS == f_GCGetPortURL(hPort, szUrl, &iUrlLength))
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

                    if (GC_ERR_SUCCESS != f_GCReadPort(hPort, iAddr, pXML, &iReadXMLSize))
                    {
                        //HandleError("could not read from port!");
                        return "";
                    }
//char *pBuf= (char *)pXML;
//for( int i=0; i<2; i++ )
//{
//TRACE(_T("  %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n"),pBuf[0],pBuf[1],pBuf[2],pBuf[3],pBuf[4],pBuf[5],pBuf[6],pBuf[7],pBuf[8],pBuf[9],pBuf[10],pBuf[11],pBuf[12],pBuf[13],pBuf[14],pBuf[15]);
//pBuf+=16;
//}

//                    ((BYTE *)pXML)[iXMLSize] = '\0';

					gcstrXml="";

					//zipなら解凍
					//unsigned int loc = strFilename.find( ".zip", 0 );
					if( strFilename.find( ".zip", 0 ) != std::string::npos )
					{


#ifdef ZIP_FILE
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
						
						//dwResult = GetEnvironmentVariable(
						//		_T("temp"),
						//		tchrEnvVariable,
						//		sizeof(tchrEnvVariable)
						//		);
						//if (dwResult == 0) {
						//	dwResult = GetEnvironmentVariable(
						//			_T("tmp"),
						//			tchrEnvVariable,
						//			sizeof(tchrEnvVariable)
						//			);
						//}

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

/*
							//リネーム
							char fileRenameName[MAX_PATH];	//リネームされたフルパスファイル名
							strcpy_s( fileRenameName, sizeof(fileRenameName)/ sizeof(fileRenameName[0]), fileFullpathName);
							char * pZipStr = strstr( fileRenameName, ".zip" );
							if( pZipStr ){
								strcpy( pZipStr,".xml" );
								//
								StString aFileName = fileRenameName;
								DeleteFile(aFileName.GetUnicode());
								//DeleteFile(fileRenameName);
								rename(fileUnzipName, fileRenameName); 
							}

							//削除 fileFullpathName
							StString aFileFullPathName = fileRenameName;
							DeleteFile(aFileFullPathName.GetUnicode());
							//DeleteFile(fileFullpathName);

							//fileFullpathNameにfileRenameNameを入れる
							strcpy_s( fileFullpathName, sizeof(fileFullpathName)/ sizeof(fileFullpathName[0]), fileRenameName);
*/
						}while(0);
#else









//なぜかメモリ上での解凍ができない？？


//TEST
//FILE *fp;
//fp = fopen( "C:\\test12.zip", "wb" );
//if( fp != NULL ){
//fwrite( pXML, iXMLSize,1,fp );
//fclose( fp );
//}


//ZEXTERN int ZEXPORT uncompress OF((Bytef *dest,   uLongf *destLen,
//									const Bytef *source, uLong sourceLen));
//int dammy;
//uLongf destLen=sizeof(dammy);
//int ret = uncompress((Bytef *)&dammy,&destLen,(const Bytef *)pXML,iXMLSize+5);
//
//TRACE(_T("  uncompress=%d[Z_OK=%d Z_MEM_ERROR=%d Z_BUF_ERROR=%d]    destLen=%d\n"), ret,Z_OK,Z_MEM_ERROR,Z_BUF_ERROR, destLen);

						
						
						
uLongf destLen = (uLongf)GetUnzipSize( (char *)pXML );
if( destLen>0 )
{
	void *pUnzipXML = malloc(destLen+1);
	if( pUnzipXML )
	{
		//size_t ucSize = Uncompress_zlib( pUnzipXML, destLen, pXML, iXMLSize );

		unsigned long destSize=0;
int
status = decompress(
        (unsigned char *)pXML,
        iXMLSize,
        (unsigned char *)pUnzipXML,
        destLen,
        &destSize);


		//if( ucSize==destLen )
		if( destSize==destLen )
		{
			gcstrXml = (char *)pUnzipXML;
		}

//		int 
//		ret = uncompress((Bytef *)pUnzipXML,&destLen,(const Bytef *)pXML,iXMLSize);
//TRACE(_T("  uncompress=%d[Z_OK=%d Z_MEM_ERROR=%d Z_BUF_ERROR=%d Z_DATA_ERROR=%d]    destLen=%d\n"), ret,Z_OK,Z_MEM_ERROR,Z_BUF_ERROR, Z_DATA_ERROR,destLen);
//
//		if( ret == Z_OK )
//		{
//			gcstrXml = (char *)pUnzipXML;
//		}
		free(pUnzipXML);
	}
}

#endif

					}
					else
					{


#ifdef TEST 
//compress( buffer, &buffer_size, src_memory, src_size );
uLongf zipBufSize = 100000;
char *pzipXML = (char *)malloc(zipBufSize);
		int 
		ret = compress((Bytef *)pzipXML,&zipBufSize,(const Bytef *)pXML,iXMLSize);
		if( ret == Z_OK )
		{

FILE *fp;
fp = fopen( "C:\\test22.zip", "wb" );
if( fp != NULL ){
fwrite( pzipXML, zipBufSize,1,fp );
fclose( fp );
}

//解凍

uLongf destLen = (uLongf)GetUnzipSize( (char *)pzipXML );
if( destLen>0 )
{
	void *pUnzipXML = malloc(destLen+1);
	if( pUnzipXML )
	{
		size_t ucSize = Uncompress_zlib( pUnzipXML, destLen, pzipXML, zipBufSize );
		if( ucSize==destLen )
		{
			gcstrXml = (char *)pUnzipXML;
		}



//		int 
//		ret = uncompress((Bytef *)pUnzipXML,&destLen,(const Bytef *)pXML,iXMLSize);
//TRACE(_T("  uncompress=%d[Z_OK=%d Z_MEM_ERROR=%d Z_BUF_ERROR=%d Z_DATA_ERROR=%d]    destLen=%d\n"), ret,Z_OK,Z_MEM_ERROR,Z_BUF_ERROR, Z_DATA_ERROR,destLen);
//
//		if( ret == Z_OK )
//		{
//			gcstrXml = (char *)pUnzipXML;
//		}
		free(pUnzipXML);
	}
}



		}








#else

						//xmlならそのままコピー
						//gcstring gcstrXml = (char *)pXML;
						gcstrXml = (char *)pXML;
#endif
					}

//ファイル名指定
//FILE	*fp;
//TCHAR szName[256];
//swprintf(szName,256,_T("c:\\u3v-test.xml"));
//if(NULL != (fp = _wfopen(szName,_T("wb"))))
//{
//	fwrite(pXML,sizeof(BYTE),iXMLSize,fp);
//	fclose(fp);
//}



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
#endif

//bool StGenGCLib::UpdateTLList( bool *pbChanged )
//{
//    std::vector< std::string > NewTLList;
//    
//    std::string directory = std::getenv("GENICAM_GENTL32_PATH");
//    directory.append("\\");
//
//    std::string ctiPath = directory;
//    ctiPath.append("*.cti");
//
//    WIN32_FIND_DATA find_data;
//
//	TCHAR szTmp[256];
//	MultiByteToWideChar(CP_ACP, 0, ctiPath.c_str(), -1, szTmp, _countof(szTmp) );
//    HANDLE hFileSearch = FindFirstFile(szTmp, &find_data);
//    //HANDLE hFileSearch = FindFirstFile(ctiPath.c_str(), &find_data);
//
//    if (INVALID_HANDLE_VALUE == hFileSearch)
//    {
//        NewTLList.clear();
//    } 
//    else 
//    {
//        // List all the files in the directory with some info about them.
//        do
//        {
//            if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//            {
//                std::cout<< "found directory "<< find_data.cFileName << std::endl;
//            }
//            else
//            {
//                std::cout << "found file " << find_data.cFileName << std::endl;
//                ctiPath = directory;
//				char cTmp[256];
//				WideCharToMultiByte(CP_ACP, 0, find_data.cFileName, -1, cTmp, _countof(cTmp), NULL, NULL);
//				ctiPath.append(cTmp);
//
//                //ctiPath.append(find_data.cFileName);
//                //SimpleTL* element = new SimpleTL(ctiPath);
//                NewTLList.push_back(ctiPath);
//            }
//        }
//        while (FindNextFile(hFileSearch, &find_data) != 0);
//
//        DWORD dwError = GetLastError();
//        if (dwError != ERROR_NO_MORE_FILES) 
//        {
//            std::cerr << "FindFirstFile reported error" << std::endl;
//        }
//
//        FindClose(hFileSearch);
//    }
//
//    // create a copy of the previous known TL
//    std::vector< std::string > OldTLlist;
//    OldTLlist.insert(OldTLlist.end(), m_vTLList.begin(), m_vTLList.end());
//
//    // empty the member list and fill it with current
//    m_vTLList.clear();
//
//    // now check the list of new TL against the already existing list 
//    // and set HasChanged Flag in case there is a difference
//    bool bChanged = false;
//    bool bMatch = false;
//    for (std::vector < std::string >::iterator Iter1 = NewTLList.begin(); Iter1 != NewTLList.end(); )
//    {
//        for (std::vector < std::string >::iterator Iter2 = OldTLlist.begin(); Iter2 != OldTLlist.end(); )
//        {
//            if ((*Iter1) == (*Iter2))
//            {
//                m_vTLList.insert(m_vTLList.end(), *Iter2);
//                //delete *Iter1;
//                NewTLList.erase(Iter1);
//                OldTLlist.erase(Iter2);
//                Iter1 = NewTLList.begin();
//                Iter2 = OldTLlist.begin();
//                bMatch = true;
//                break;
//            }
//            else
//                Iter2++;
//        }
//
//        if (!bMatch)
//            Iter1++;
//        else
//            bMatch = false;
//    }
//
//    // if there are still new TLs or any old TLs left, a change happened
//    bChanged = (NewTLList.size() > 0) || (OldTLlist.size() > 0);
//
//    if (NewTLList.size())
//    {
//        m_vTLList.insert(m_vTLList.end(), NewTLList.begin(), NewTLList.end());
//    }
//
//    while (OldTLlist.size())
//    {
//        //delete *(OldTLlist.end());
//        OldTLlist.erase(OldTLlist.end());
//    }
//    
//    if (pbChanged)
//        *pbChanged = bChanged;
//
//    return true;
//}