#pragma once

#include "StGenUtilsLib.h"

class ST_GENUTILSLIB_API StSaveFileImage
{
public:
	StSaveFileImage(void);
	~StSaveFileImage(void);

	bool executeJpeg(LPCTSTR filename, const unsigned char *pBGRImage, int nWidth, int nHeight, double dblRatioWidth=1.0, double dblRatioHeight=1.0);
	bool executeBitmap(LPCTSTR filename, const unsigned char *pBGRImage, int nWidth, int nHeight, double dblRatioWidth=1.0, double dblRatioHeight=1.0);
	bool executeBitmapMono8(LPCTSTR filename, const unsigned char *pImage, int nWidth, int nHeight, double dblRatioWidth=1.0, double dblRatioHeight=1.0);
	//-------------
	bool executeFileSave(LPCTSTR filename, const unsigned char *pBGRImage, int nWidth, int nHeight, double dblRatioWidth=1.0, double dblRatioHeight=1.0);
	bool executeFileSaveGrayScale(LPCTSTR filename, const unsigned char *pImage, int nWidth, int nHeight, int bitPerPixel);
	//-------------

	bool executePNGFileSaveGrayScale(LPCTSTR filename, const unsigned char *pImage, int nWidth, int nHeight, int nPixelSize, int nPixelBits, int nEndian=0, int nBitFormat=0);
	bool executeTIFFFileSaveGrayScale(LPCTSTR filename, const unsigned char *pImage, int nWidth, int nHeight, int nPixelSize, int nPixelBits, int nBitFormat=0);

};

