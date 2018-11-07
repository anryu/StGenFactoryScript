#include "StdAfx.h"
#include "StBufferInfo.h"


StBufferInfo::StBufferInfo(StBuffer *aBuffer)
	: mStBuffer(aBuffer)
{
}


StBufferInfo::~StBufferInfo(void)
{
}

INT StBufferInfo::GetElement(void)
{
	INT nElement = 0;

	switch(mStBuffer->GetPixelType())
	{
	case StPixelMono8:
	case StPixelMono8Signed:
	case StPixelMono10:
	case StPixelMono10Packed:
	case StPixelMono12:
	case StPixelMono12Packed:
	case StPixelMono14:
	case StPixelMono16:
	case StPixelBayerGR8:
	case StPixelBayerRG8:
	case StPixelBayerGB8:
	case StPixelBayerBG8:
	case StPixelBayerGR10:
	case StPixelBayerRG10:
	case StPixelBayerGB10:
	case StPixelBayerBG10:
	case StPixelBayerGR12:
	case StPixelBayerRG12:
	case StPixelBayerGB12:
	case StPixelBayerBG12:
	case StPixelBayerGR10Packed:
	case StPixelBayerRG10Packed:
	case StPixelBayerGB10Packed:
	case StPixelBayerBG10Packed:
	case StPixelBayerGR12Packed:
	case StPixelBayerRG12Packed:
	case StPixelBayerGB12Packed:
	case StPixelBayerBG12Packed:
	case StPixelBayerGR16:
	case StPixelBayerRG16:
	case StPixelBayerGB16:
	case StPixelBayerBG16:
		nElement = 1;
		break;
	case StPixelRGB8Packed:
	case StPixelBGR8:
	case StPixelRGB10Packed:
	case StPixelBGR10:
	case StPixelRGB12Packed:
	case StPixelBGR12:
	case StPixelRGB16Packed:
		nElement = 3;
		break;
	case StPixelRGBA8Packed:
	case StPixelBGRa8:
		nElement = 4;
		break;
	};
	return nElement;
}

INT StBufferInfo::GetPixelSize(void)
{
	INT nPixelSize = 0;

	switch(mStBuffer->GetPixelType())
	{
	case StPixelMono8:
	case StPixelMono8Signed:
	case StPixelBayerGR8:
	case StPixelBayerRG8:
	case StPixelBayerGB8:
	case StPixelBayerBG8:
		nPixelSize = 8;
		break;
	case StPixelMono10:
	case StPixelMono12:
	case StPixelMono14:
	case StPixelMono16:
	case StPixelBayerGR10:
	case StPixelBayerRG10:
	case StPixelBayerGB10:
	case StPixelBayerBG10:
	case StPixelBayerGR12:
	case StPixelBayerRG12:
	case StPixelBayerGB12:
	case StPixelBayerBG12:
	case StPixelBayerGR16:
	case StPixelBayerRG16:
	case StPixelBayerGB16:
	case StPixelBayerBG16:
		nPixelSize = 16;
		break;
	case StPixelMono10Packed:
	case StPixelMono12Packed:
	case StPixelBayerGR10Packed:
	case StPixelBayerRG10Packed:
	case StPixelBayerGB10Packed:
	case StPixelBayerBG10Packed:
	case StPixelBayerGR12Packed:
	case StPixelBayerRG12Packed:
	case StPixelBayerGB12Packed:
	case StPixelBayerBG12Packed:
		nPixelSize = 12;
		break;
	case StPixelRGB8Packed:
	case StPixelBGR8:
		nPixelSize = 24;
		break;
	case StPixelRGBA8Packed:
	case StPixelBGRa8:
		nPixelSize = 32;
		break;
	case StPixelRGB10Packed:
		nPixelSize = 30;
		break;
	case StPixelBGR10:
	case StPixelBGR12:
	case StPixelRGB16Packed:
		nPixelSize = 48;
		break;
	case StPixelRGB12Packed:
		nPixelSize = 36;
		break;
	};
	return nPixelSize;
}

//óLå¯BITSêî
INT StBufferInfo::GetPixelBits(void)
{
	INT nPixelSize = 0;

	switch(mStBuffer->GetPixelType())
	{
	case StPixelMono8:
	case StPixelMono8Signed:
	case StPixelBayerGR8:
	case StPixelBayerRG8:
	case StPixelBayerGB8:
	case StPixelBayerBG8:
		nPixelSize = 8;
		break;
	case StPixelMono10:
	case StPixelBayerGR10:
	case StPixelBayerRG10:
	case StPixelBayerGB10:
	case StPixelBayerBG10:
	case StPixelMono10Packed:
	case StPixelBayerGR10Packed:
	case StPixelBayerRG10Packed:
	case StPixelBayerGB10Packed:
	case StPixelBayerBG10Packed:
		nPixelSize = 10;
		break;
	case StPixelMono12:
	case StPixelBayerGR12:
	case StPixelBayerRG12:
	case StPixelBayerGB12:
	case StPixelBayerBG12:
	case StPixelMono12Packed:
	case StPixelBayerGR12Packed:
	case StPixelBayerRG12Packed:
	case StPixelBayerGB12Packed:
	case StPixelBayerBG12Packed:
		nPixelSize = 12;
		break;
	case StPixelMono14:
		nPixelSize = 14;
		break;
	case StPixelMono16:
	case StPixelBayerGR16:
	case StPixelBayerRG16:
	case StPixelBayerGB16:
	case StPixelBayerBG16:
		nPixelSize = 16;
		break;
	case StPixelRGB8Packed:
	case StPixelBGR8:
		nPixelSize = 8;
		break;
	case StPixelRGBA8Packed:
	case StPixelBGRa8:
		//nPixelSize = 32;
		nPixelSize = 8;
		break;
	case StPixelRGB10Packed:
		nPixelSize = 10;
		break;
	case StPixelBGR10:
		nPixelSize = 10;
		break;
	case StPixelBGR12:
		nPixelSize = 12;
		break;
	case StPixelRGB16Packed:
		nPixelSize = 16;
		break;
	case StPixelRGB12Packed:
		nPixelSize = 12;
		break;
	};
	return nPixelSize;
}

//StBufferÇ©ÇÁColorIndexéÊìæ
//0:R 1:G 2:B 3:A -1:Mono 
INT StBufferInfo::GetElementColorIndex(INT iPos)
{
	INT nColorIndex = -1;
	switch(mStBuffer->GetPixelType())
	{
	case StPixelMono8:
	case StPixelMono8Signed:
	case StPixelMono10:
	case StPixelMono10Packed:
	case StPixelMono12:
	case StPixelMono12Packed:
	case StPixelMono14:
	case StPixelMono16:
	case StPixelBayerGR8:
	case StPixelBayerRG8:
	case StPixelBayerGB8:
	case StPixelBayerBG8:
	case StPixelBayerGR10:
	case StPixelBayerRG10:
	case StPixelBayerGB10:
	case StPixelBayerBG10:
	case StPixelBayerGR12:
	case StPixelBayerRG12:
	case StPixelBayerGB12:
	case StPixelBayerBG12:
	case StPixelBayerGR10Packed:
	case StPixelBayerRG10Packed:
	case StPixelBayerGB10Packed:
	case StPixelBayerBG10Packed:
	case StPixelBayerGR12Packed:
	case StPixelBayerRG12Packed:
	case StPixelBayerGB12Packed:
	case StPixelBayerBG12Packed:
	case StPixelBayerGR16:
	case StPixelBayerRG16:
	case StPixelBayerGB16:
	case StPixelBayerBG16:
		nColorIndex = -1;
		break;
	case StPixelRGBA8Packed:
		if( iPos == 3 ) nColorIndex=3;
	case StPixelRGB8Packed:
	case StPixelRGB10Packed:
	case StPixelRGB12Packed:
	case StPixelRGB16Packed:
		if( iPos>=0 && iPos<3 )
			nColorIndex = iPos;
		break;
	case StPixelBGRa8:
		if( iPos == 3 ) nColorIndex=3;
	case StPixelBGR8:
	case StPixelBGR10:
	case StPixelBGR12:
		if( iPos==0 )
			nColorIndex = 2;
		else
		if( iPos==1 )
			nColorIndex = 1;
		else
		if( iPos==2 )
			nColorIndex = 0;
		break;
	};
	return nColorIndex;
}

//Å•1.0.0.1035
BOOL StBufferInfo::GetColor(void)
{
	BOOL bColor = TRUE;

	switch(mStBuffer->GetPixelType())
	{
	case StPixelMono8:
	case StPixelMono8Signed:
	case StPixelMono10:
	case StPixelMono10Packed:
	case StPixelMono12:
	case StPixelMono12Packed:
	case StPixelMono14:
	case StPixelMono16:
	case StPixelMono8_16Bit:
	case StPixelMono10pmsb_16Bit:
	case StPixelMono12pmsb_16Bit:
	case StPixelMono14pmsb_16Bit:
	case StPixelMono16_16Bit:
	//case StPixelMono10p:
	//case StPixelMono12p:
		bColor = FALSE;
		break;
	}

	return bColor;
}
//Å£1.0.0.1035
