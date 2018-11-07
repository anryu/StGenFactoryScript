#pragma once

#include "StGenUtilsLib.h"
#include "StResult.h"
#include "StBuffer.h"

class ST_GENUTILSLIB_API StTransformsImage
{
public:
	StTransformsImage(void);
	~StTransformsImage(void);

	static StResult Transform(StBuffer *srcBuffer, StBuffer *dstBuffer);
};

