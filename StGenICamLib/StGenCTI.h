#pragma once

#include <vector>
#include <Base/GCTypes.h>

#include "StString.h"

class StGenCTI
{
public:
	StGenCTI(void);
	~StGenCTI(void);

	bool UpdateCTIList( bool *pbChanged );
	StString GetGCLibPathName( uint32_t index );
	uint32_t GetNumGCLibs() {return static_cast<uint32_t>(m_vGCLibList.size());}
private:
	std::vector < std::string > m_vGCLibList;
	//std::vector <StString> m_vGCLibList;
};

