#include "StdAfx.h"
#include "GenTLZip.h"
//#include "../include/zlib.h"
#include "zlib/include/zlib.h"

/*
#ifdef _WIN64
#ifdef	_DEBUG
//#pragma comment(lib, "../lib/x64/zlibstaticd.lib")
#pragma comment(lib, "../lib/x64/zlibstaticd.lib")
#else	//_DEBUG
//#pragma comment(lib, "../lib/x64/zlibstatic.lib")
#pragma comment(lib, "../lib/x64/zlibstatic.lib")
#endif	//_DEBUG
#else //_WIN64
#ifdef	_DEBUG
//#pragma comment(lib, "../lib/x86/zlibstaticd.lib")
#pragma comment(lib, "../lib/x86/zlibstaticd.lib")
#else	//_DEBUG
//#pragma comment(lib, "../lib/x86/zlibstatic.lib")
#pragma comment(lib, "../lib/x86/zlibstatic.lib")
#endif	//_DEBUG
#endif //_WIN64
*/



////////////////////////////////////////////////////////////////////////////////////////////
// declaration
////////////////////////////////////////////////////////////////////////////////////////////

static const unsigned long sZIPMagic = 0x04034b50;
static int zlib_uncompress (Bytef * dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
static bool zlib_Unzip( const std::string& aCompressed, std::string& aUncompressed );


////////////////////////////////////////////////////////////////////////////////////////////
// C implementation (GigEVision Validation)
////////////////////////////////////////////////////////////////////////////////////////////

int zlib_uncompress (Bytef * dest, uLongf *destLen, const Bytef *source, uLong sourceLen)
{
    z_stream stream;
    int err;

    stream.next_in = (Bytef*)source;
    stream.avail_in = (uInt)sourceLen;
    /* Check for source > 64K on 16-bit machine: */
    if ((uLong)stream.avail_in != sourceLen) return Z_BUF_ERROR;

    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;
    if ((uLong)stream.avail_out != *destLen) return Z_BUF_ERROR;

    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;

    err = inflateInit2(&stream, -8);
    if (err != Z_OK) return err;

    err = inflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        inflateEnd(&stream);
        if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0))
            return Z_DATA_ERROR;
        return err;
    }
    *destLen = stream.total_out;

    err = inflateEnd(&stream);
    return err;
}

// ==============================================================================
//bool zlib_unzip( const std::string& aCompressed, std::string& aUncompressed )
bool zlib_unzip( const char * aCompressed, std::string& aUncompressed )
{
#pragma pack( push, 2 )
    struct ZIPHeader
    {
        unsigned long   mSignature;
        unsigned short  mVersion;
        unsigned short  mFlags;
        unsigned short  mCompression;
        unsigned short  mTime;
        unsigned short  mDate;
        unsigned long   mCRC;
        unsigned long   mCompressedSize;
        unsigned long   mUncompressedSize;
        unsigned short  mFileNameLength;
        unsigned short  mExtraFieldLength;
    };
#pragma pack( pop )

    //const ZIPHeader* lHeader = (ZIPHeader*)aCompressed.data();
    const ZIPHeader* lHeader = (ZIPHeader*)aCompressed;

    uLong lCompressedSize = lHeader->mCompressedSize;
    //const Byte* lCompressed = (Byte*)aCompressed.data() +
    const Byte* lCompressed = (Byte*)aCompressed +
                              sizeof( ZIPHeader ) +
                              lHeader->mFileNameLength +
                              lHeader->mExtraFieldLength;

    uLong lUncompressedSize = lHeader->mUncompressedSize;
    Byte* lUncompressed = new Byte[ lUncompressedSize ];

    bool lResult = false;
    try
    {
        if ( zlib_uncompress( lUncompressed,
                              &lUncompressedSize,
                              lCompressed,
                              lCompressedSize ) != Z_OK )
            goto unzip_end;

        // Copy the date to the output
        aUncompressed = "";
        aUncompressed.append( (char*)lUncompressed, lUncompressedSize );
        lResult = true;
    }
    catch( ... )
    {
        // just in case
    }

unzip_end:
    delete [] lUncompressed;
    return lResult;
}


GenTLZip::GenTLZip(void)
{
}

GenTLZip::~GenTLZip(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////
// implementation
////////////////////////////////////////////////////////////////////////////////////////////

bool GenTLZip::UncompressGenICamXML( std::string &aData, char *aZipData )
{
	bool  lResult = false;
    if ( ( *(unsigned long*)aData.c_str() ) == sZIPMagic )
    {
        // ZIP File
        lResult = zlib_unzip( aZipData, aData );       
        //lResult = zlib_unzip( aData, aData );       
        //GENTLTEST_CHECK_MESSAGE("Unable to uncompress ZIP file", false);
    }
    else
    {
        // Raw compressed data
        unsigned long lUncompressedSize = (unsigned long)aData.size() * 100;
        char * lUncompressed = new char[ lUncompressedSize + 1];

        if ( ::uncompress( (Byte*)lUncompressed,
                           &lUncompressedSize,
                           (Byte*)aData.c_str(),
                           (uLong)aData.size() ) != Z_OK )
        {
            delete [] lUncompressed;
            //GENTLTEST_CHECK_MESSAGE("Unable to uncompress XML data", false);
        } else {

       	 	lUncompressed[ lUncompressedSize ] = '\0';
        	aData = lUncompressed;
        	delete [] lUncompressed;
			lResult = true;
	    }
    }
	return(lResult);
}
