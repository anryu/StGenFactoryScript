#pragma once

#include <GenApi/GenApi.h>

using namespace GenApi;

#include "StGenGCLib.h"

class StGenPort : public IPort
{
public:
	//StGenPort(void);
	StGenPort (StGenGCLib* psGCLib, PORT_HANDLE hPort);
	~StGenPort(void);

	//! Reads a chunk of bytes from the port
	virtual void  Read(void *pBuffer, int64_t Address, int64_t Length);

	//! Writes a chunk of bytes to the port
	virtual void  Write(const void *pBuffer, int64_t Address, int64_t Length);

	virtual GenApi::EAccessMode GetAccessMode( void ) const;

	virtual void  GetPortInfo(PORT_INFO_CMD iInfoCmd,INFO_DATATYPE * piType, void * pBuffer, size_t * piSize);

	bool GetLastValue(){return m_bLastValue;}
	GC_ERROR GetLastStatus(){return m_gcLastStatus;}


private:
    StGenGCLib* m_psGCLib;
	PORT_HANDLE   m_hPort;

	//SimpleTL* m_psTL;

	bool m_bLastValue;
	GC_ERROR m_gcLastStatus;
};

