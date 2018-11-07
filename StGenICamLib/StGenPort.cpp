#include "StdAfx.h"
#include "StGenPort.h"

using namespace GenICam;
//¥1.0.0.73 GenApi3.0
#ifdef _GENAPI3
using namespace GenTL;
#else
using namespace GenICam::Client;
#endif
//£1.0.0.73 GenApi3.0
using namespace GenApi;

//StGenPort::StGenPort(void)
//{
//}

// ---------------------------------------------------------------------------
/// \brief CTOR
///
// ---------------------------------------------------------------------------
StGenPort::StGenPort (StGenGCLib* psGCLib, PORT_HANDLE hPort)
	: m_psGCLib(psGCLib)
	, m_hPort ( hPort )
	, m_bLastValue( true )
	, m_gcLastStatus( GC_ERR_SUCCESS )
{

}

// ---------------------------------------------------------------------------
/// \brief DTOR
///
// ---------------------------------------------------------------------------
StGenPort::~StGenPort ( void )
{

}

// ---------------------------------------------------------------------------
/// \brief Reads a chunk of bytes from the port
///
/// \param pBuffer 
/// \param Address 
/// \param Length 
// ---------------------------------------------------------------------------
void  StGenPort::Read(void *pBuffer, int64_t Address, int64_t Length)
{
	bool bReval= true;
	size_t iSize = (size_t)Length;
	GC_ERROR status = GC_ERR_SUCCESS;
	status = m_psGCLib->f_GCReadPort ( m_hPort, Address, pBuffer, &iSize);
	if ((status < 0) || (iSize != Length))
		bReval = false;
		//throw RUNTIME_EXCEPTION("Unable to Read Register");
	m_bLastValue = bReval;
	if( status<0 )	m_gcLastStatus = status;
		
}

// ---------------------------------------------------------------------------
/// \brief Writes a chunk of bytes to the port
///
/// \param pBuffer 
/// \param Address 
/// \param Length 
// ---------------------------------------------------------------------------
void  StGenPort::Write(const void *pBuffer,int64_t Address, int64_t Length)
{
	bool bReval= true;
	size_t iSize = (size_t)Length;
	GC_ERROR status = GC_ERR_SUCCESS;
	status =  m_psGCLib->f_GCWritePort ( m_hPort, Address, pBuffer, &iSize);
	if ((status < 0) || (iSize != Length))
		bReval = false;
		//throw RUNTIME_EXCEPTION("Unable to Write Register");
	m_bLastValue = bReval;
	if( status<0 )	m_gcLastStatus = status;

}

// ---------------------------------------------------------------------------
/// \brief Retrieve the access mode for this port
///
/// \return GenApi::EAccessMode
// ---------------------------------------------------------------------------
EAccessMode StGenPort::GetAccessMode() const
{
	return RW;
}

// ---------------------------------------------------------------------------
void  StGenPort::GetPortInfo(PORT_INFO_CMD iInfoCmd,INFO_DATATYPE * piType, void * pBuffer, size_t * piSize)
{
	//size_t iSize = (size_t)Length;
	GC_ERROR status = GC_ERR_SUCCESS;
	status =  m_psGCLib->f_GCGetPortInfo( m_hPort, iInfoCmd, piType, pBuffer, piSize);
	//if ((status < 0) || (iSize != Length))
	//	throw RUNTIME_EXCEPTION("Unable to Write Register");

}
