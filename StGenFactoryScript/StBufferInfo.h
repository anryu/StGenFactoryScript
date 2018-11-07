#pragma once

#include "../CommonHeader/StBuffer.h"

class StBufferInfo
{
public:
	StBufferInfo(StBuffer *aBuffer);
	~StBufferInfo(void);

	INT GetElement(void);
	INT GetPixelSize(void);
	INT GetPixelBits(void);
	INT GetElementColorIndex(INT iPos);

	//Å•1.0.0.1035
	BOOL GetColor(void);
	//Å£1.0.0.1035

private:
	StBuffer *mStBuffer;
};

