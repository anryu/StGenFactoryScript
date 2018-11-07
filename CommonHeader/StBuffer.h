#pragma once

//#include "ImageBuffer.h"
//#include "StResultCode.h"
#include "StGenUtilsLib.h"
#include "StPixelType.h"
#include "StType.h"
#include "StResult.h"

class ST_GENUTILSLIB_API StBuffer
{
public:
	StBuffer(void);
	~StBuffer(void);

	const void * GetDataPointer() const;
	//StResult GetDataPointer( PBYTE * pValue );

	//PvUInt8 * GetDataPointer();
	StUInt64 GetID() const;
	StResult SetID( StUInt64 aValue );
	//PvUInt32 GetAcquiredSize() const;
	StUInt32 GetSize() const;
	StResult SetSize( StUInt32 aValue );
	StResult Alloc( StUInt32 aSize );
	void Free();
	StResult Attach( void * aBuffer, StUInt32 aSize );
	void * Detach( void );

	//PvUInt16 GetBlockID() const;
	StUInt64 GetTimestamp() const;
	StResult SetTimestamp( StUInt64 aValue );

	StPixelType GetPixelType() const;
	StResult GetPixelType( StPixelType &aValue );
	StResult SetPixelType( StPixelType aValue );
	StUInt32 GetWidth() const;
	StResult SetWidth( StUInt32 aValue );
	StUInt32 GetHeight() const;
	StResult SetHeight( StUInt32 aValue );
	StUInt32 GetOffsetX() const;
	StResult SetOffsetX( StUInt32 aValue );
	StUInt32 GetOffsetY() const;
	StResult SetOffsetY( StUInt32 aValue );
	StUInt32 GetPaddingX() const;
	StResult SetPaddingX( StUInt32 aValue );
	StUInt32 GetPaddingY() const;
	StResult SetPaddingY( StUInt32 aValue );
	StUInt32 GetImageOffset() const;
	StResult SetImageOffset( StUInt32 aValue );
	StUInt32 GetPayloadType() const;
	StResult SetPayloadType( StUInt32 aValue );

	//Å•2017/10/10
	StBool8 GetIncompleteImage() const;
	StResult SetIncompleteImage( StBool8 aValue );
	StUInt32 GetDeliveredImageHeight() const;
	StResult SetDeliveredImageHeight( StUInt32 aValue );
	//Å£2017/10/10

	StUInt32 GetLinePitch() const;
	StResult SetLinePitch( StUInt32 aValue );
	void * GetContext() const;
	StResult SetContext( void * aValue );
	StUInt32 GetAllocSize( void );
    const StString GetPortName() const;
    StResult SetPortName( StString & aValue );

	//Å•2017/10/10
	StUInt32 GetChunkCount() const;
	StResult SetChunkCount( StUInt32 aValue );

	StUInt32 GetChunklayoutID() const;
	StResult SetChunklayoutID( StUInt32 aValue );

	StUInt64 GetChunkID( StUInt32 aIndex ) const;
	StResult SetChunkID( StUInt32 aIndex, StUInt32 aValue );
	const void * GetChunkBuffer(StUInt32 aID, StUInt32 &aSize ) const;

	StResult ChunkFree( void );
	StResult SetChunkDataBuffer( void *pTopDataBuffer, StUInt32 aCount );
	//Å£2017/10/10


	//Å•í«â¡
	StResult CreateCopy( StBuffer & aBuffer );
	//Å£í«â¡

	//Å•1.0.0.33
	StUInt32 GetPixelBits() const;
	//Å£1.0.0.33



private:
	void * mhData;
};

