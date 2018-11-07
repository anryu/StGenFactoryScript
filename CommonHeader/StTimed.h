#pragma once

#include "StGenUtilsLib.h"

class ST_GENUTILSLIB_API StTimed
{
public:
	StTimed(void);
	~StTimed(void);

	void ResetTime(void);
	double GetTime(void);
private:
	void * mhData;

};

