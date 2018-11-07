#pragma once
#include "stdafx.h"
#include <vector>
#include <string>
#include "StResult.h"

using namespace GenApi;
using namespace GenICam;
//Å•1.0.0.73 GenApi3.0
#ifdef _GENAPI3
using namespace GenTL;
#else
using namespace GenICam::Client;
#endif
//Å£1.0.0.73 GenApi3.0

//#define TL_FUNCTION_DEFINITION  "SimpleTLFunctions.h"
#define TL_FUNCTION_DEFINITION  "StTLFunctions.h"

// declare prototypes
//Å•1.0.0.73 GenApi3.0
#ifdef _GENAPI3
#define GC_SYMBOL_LINK(name) typedef GC_ERROR (GC_CALLTYPE* GC_##name)
#else
#define GC_SYMBOL_LINK(name) typedef GenICam::Client::GC_ERROR (GC_CALLTYPE* GC_##name)
#endif
//Å£1.0.0.73 GenApi3.0
#define DECLARE(params) params;
#include TL_FUNCTION_DEFINITION
#undef DECLARE
#undef GC_SYMBOL_LINK

class StGenGCLib
{
public:
	StGenGCLib(StString ctiName);
	~StGenGCLib(void);

	StResult GCGetInfo( TL_INFO_CMD iInfoCmd,INFO_DATATYPE * piType,void * pBuffer,size_t * piSize);
	StResult GetLastError( GC_ERROR * piErrorCode, char * sErrorText, size_t * piSize );

    //bool UpdateTLList(bool *pbChanged);
    //uint32_t GetNumTLs() {return static_cast<uint32_t>(m_vTLList.size());}
    //std::string GetTLPathName(uint32_t index);
	
	gcstring RetrieveXML(PORT_HANDLE hPort, gcstring &gcstrXml);
private:
    std::vector< std::string > m_vTLList;
	StString m_ctiName;
	StResult GCOpen(void);
	StResult GCClose(void);
	bool IsLoaded(void) const {return (NULL != m_hGCLib);}
	bool LinkFunctions( void );
	void UnlinkFunctions( void );

	HMODULE     m_hGCLib;

	//GC_ERROR lastError; 
	//TL_HANDLE hSystem;


public:
    // declare functions
#define GC_SYMBOL_LINK(name) GC_##name f_##name ;
#define DECLARE(params);
#include TL_FUNCTION_DEFINITION
#undef DECLARE
#undef GC_SYMBOL_LINK


};

