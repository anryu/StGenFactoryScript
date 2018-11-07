// *****************************************************************************
//
//     Copyright (c) 2012, Sensor Technology., All rights reserved.
//
// *****************************************************************************
#ifndef __STPIXELTYPE_H__
#define __STPIXELTYPE_H__

//#include <StTypes.h>
//#include <StBufferLib.h>


//
// Color
//

#define STPIXELMONO                  ( 0x01000000 )
#define STPIXELRGB                   ( 0x02000000 ) // Pre GEV 1.1, kept for bw comptability
#define STPIXELCOLOR                 ( 0x02000000 ) // GEV 1.1
#define STPIXELCUSTOM                ( 0x80000000 )
#define STPIXELCOLORMASK             ( 0xFF000000 )

//
// Effective number of bits per pixel (including padding)
//

#define STPIXEL8BIT                  ( 0x00080000 )
#define STPIXEL12BIT                 ( 0x000C0000 )
#define STPIXEL16BIT                 ( 0x00100000 )
#define STPIXEL24BIT                 ( 0x00180000 )
#define STPIXEL30BIT                 ( 0x001E0000 )
#define STPIXEL32BIT                 ( 0x00200000 )
#define STPIXEL36BIT                 ( 0x00240000 )
#define STPIXEL48BIT                 ( 0x00300000 )
#define STPIXEL64BIT                 ( 0x00400000 )
#define STBITSPERPIXELMASK           ( 0x00FF0000 )

//
// Pixel type ID
//

#define STPIXELIDMASK                ( 0x0000FFFF )


typedef enum 
{

	StPixelUndefined =          ( 0 ),
	StPixelMono8 =              ( STPIXELMONO | STPIXEL8BIT   | 0x0001 ),
	StPixelMono8Signed =        ( STPIXELMONO | STPIXEL8BIT   | 0x0002 ),
	StPixelMono10 =             ( STPIXELMONO | STPIXEL16BIT  | 0x0003 ),
	StPixelMono10Packed =       ( STPIXELMONO | STPIXEL12BIT  | 0x0004 ),
	StPixelMono12 =             ( STPIXELMONO | STPIXEL16BIT  | 0x0005 ),
	StPixelMono12Packed =       ( STPIXELMONO | STPIXEL12BIT  | 0x0006 ),
	StPixelMono14 =             ( STPIXELMONO | STPIXEL16BIT  | 0x0025 ),
	StPixelMono16 =             ( STPIXELMONO | STPIXEL16BIT  | 0x0007 ),
	StPixelBayerGR8 =           ( STPIXELMONO | STPIXEL8BIT   | 0x0008 ),
	StPixelBayerRG8 =           ( STPIXELMONO | STPIXEL8BIT   | 0x0009 ),
	StPixelBayerGB8 =           ( STPIXELMONO | STPIXEL8BIT   | 0x000A ),
	StPixelBayerBG8 =           ( STPIXELMONO | STPIXEL8BIT   | 0x000B ),
	StPixelBayerGR10 =          ( STPIXELMONO | STPIXEL16BIT  | 0x000C ),
	StPixelBayerRG10 =          ( STPIXELMONO | STPIXEL16BIT  | 0x000D ),
	StPixelBayerGB10 =          ( STPIXELMONO | STPIXEL16BIT  | 0x000E ),
	StPixelBayerBG10 =          ( STPIXELMONO | STPIXEL16BIT  | 0x000F ),
	StPixelBayerGR12 =          ( STPIXELMONO | STPIXEL16BIT  | 0x0010 ),
	StPixelBayerRG12 =          ( STPIXELMONO | STPIXEL16BIT  | 0x0011 ),
	StPixelBayerGB12 =          ( STPIXELMONO | STPIXEL16BIT  | 0x0012 ),
	StPixelBayerBG12 =          ( STPIXELMONO | STPIXEL16BIT  | 0x0013 ),
	StPixelBayerGR10Packed =    ( STPIXELMONO | STPIXEL12BIT  | 0x0026 ), // GEV 1.1
	StPixelBayerRG10Packed =    ( STPIXELMONO | STPIXEL12BIT  | 0x0027 ), // GEV 1.1
	StPixelBayerGB10Packed =    ( STPIXELMONO | STPIXEL12BIT  | 0x0028 ), // GEV 1.1
	StPixelBayerBG10Packed =    ( STPIXELMONO | STPIXEL12BIT  | 0x0029 ), // GEV 1.1
	StPixelBayerGR12Packed =    ( STPIXELMONO | STPIXEL12BIT  | 0x002A ), // GEV 1.1
	StPixelBayerRG12Packed =    ( STPIXELMONO | STPIXEL12BIT  | 0x002B ), // GEV 1.1
	StPixelBayerGB12Packed =    ( STPIXELMONO | STPIXEL12BIT  | 0x002C ), // GEV 1.1
	StPixelBayerBG12Packed =    ( STPIXELMONO | STPIXEL12BIT  | 0x002D ), // GEV 1.1
	StPixelBayerGR16 =          ( STPIXELMONO | STPIXEL16BIT  | 0x002E ), // GEV 1.1
	StPixelBayerRG16 =          ( STPIXELMONO | STPIXEL16BIT  | 0x002F ), // GEV 1.1
	StPixelBayerGB16 =          ( STPIXELMONO | STPIXEL16BIT  | 0x0030 ), // GEV 1.1
	StPixelBayerBG16 =          ( STPIXELMONO | STPIXEL16BIT  | 0x0031 ), // GEV 1.1
	StPixelRGB8Packed =         ( STPIXELCOLOR  | STPIXEL24BIT  | 0x0014 ),
	StPixelBGR8Packed =         ( STPIXELCOLOR  | STPIXEL24BIT  | 0x0015 ),
	StPixelRGBA8Packed =        ( STPIXELCOLOR  | STPIXEL32BIT  | 0x0016 ),
	StPixelBGRA8Packed =        ( STPIXELCOLOR  | STPIXEL32BIT  | 0x0017 ),
	StPixelRGB10Packed =        ( STPIXELCOLOR  | STPIXEL48BIT  | 0x0018 ),
	StPixelBGR10Packed =        ( STPIXELCOLOR  | STPIXEL48BIT  | 0x0019 ),
	StPixelRGB12Packed =        ( STPIXELCOLOR  | STPIXEL48BIT  | 0x001A ),
	StPixelBGR12Packed =        ( STPIXELCOLOR  | STPIXEL48BIT  | 0x001B ),
	StPixelRGB16Packed =        ( STPIXELCOLOR  | STPIXEL48BIT  | 0x0033 ), // GEV 1.1
	StPixelBGR10V1Packed =      ( STPIXELCOLOR  | STPIXEL32BIT  | 0x001C ),
	StPixelBGR10V2Packed =      ( STPIXELCOLOR  | STPIXEL32BIT  | 0x001D ),
	StPixelRGB12V1Packed =      ( STPIXELCOLOR  | STPIXEL36BIT  | 0x0034 ), // GEV 1.1
	StPixelYUV411Packed =       ( STPIXELCOLOR  | STPIXEL12BIT  | 0x001E ),
	StPixelYUV422Packed =       ( STPIXELCOLOR  | STPIXEL16BIT  | 0x001F ),
	StPixelYUV422YUYVPacked =   ( STPIXELCOLOR  | STPIXEL16BIT  | 0x0032 ), // GEV 1.1
	StPixelYUV444Packed =       ( STPIXELCOLOR  | STPIXEL24BIT  | 0x0020 ),
	StPixelRGB8Planar =         ( STPIXELCOLOR  | STPIXEL24BIT  | 0x0021 ),
	StPixelRGB10Planar =        ( STPIXELCOLOR  | STPIXEL48BIT  | 0x0022 ),
	StPixelRGB12Planar =        ( STPIXELCOLOR  | STPIXEL48BIT  | 0x0023 ),
	StPixelRGB16Planar =        ( STPIXELCOLOR  | STPIXEL48BIT  | 0x0024 ),

	StPixelBGR8 = ( STPIXELCOLOR  | STPIXEL24BIT  | 0x0015 ),
	StPixelBGR10 = ( STPIXELCOLOR  | STPIXEL48BIT  | 0x0019 ),
	StPixelBGR10p = ( STPIXELCOLOR  | STPIXEL30BIT  | 0x0048 ),
	StPixelBGR12 = ( STPIXELCOLOR  | STPIXEL48BIT  | 0x001B ),
	StPixelBGR12p = ( STPIXELCOLOR  | STPIXEL36BIT  | 0x0049 ),
	StPixelBGR14 = ( STPIXELCOLOR  | STPIXEL48BIT  | 0x004A ),
	StPixelBGR16 = ( STPIXELCOLOR  | STPIXEL48BIT  | 0x004b ),
	StPixelBGRa8 = ( STPIXELCOLOR  | STPIXEL32BIT  | 0x0017 ),
	StPixelBGRa10 = ( STPIXELCOLOR  | STPIXEL64BIT  | 0x004C ),
	StPixelBGRa10p = ( STPIXELCOLOR  | STPIXEL32BIT  | 0x004D ),
	StPixelBGRa12 = ( STPIXELCOLOR  | STPIXEL64BIT  | 0x004E ),
	StPixelBGRa12p = ( STPIXELCOLOR  | STPIXEL48BIT  | 0x004F ),
	StPixelBGRa14 = ( STPIXELCOLOR  | STPIXEL64BIT  | 0x0050 ),
	StPixelBGRa16 = ( STPIXELCOLOR  | STPIXEL64BIT  | 0x0051 ),

	// Not GiGE Vision standard - used to control destination/display
	//StPixelWinRGB16 =           ( STPIXELCOLOR | STPIXEL16BIT  | 0x8000 ), 
	//StPixelWinRGB32 =           ( STPIXELCOLOR | STPIXEL32BIT  | 0x8001 ),
	//StPixelWinRGB24 =           ( STPIXELCOLOR | STPIXEL24BIT  | 0x8002 ),
	//StPixelWinBGR32 =           ( STPIXELCOLOR | STPIXEL32BIT  | 0x8003 ),
	//StPixelWinBGR24 =           ( STPIXELCOLOR | STPIXEL24BIT  | 0x8004 )

	StPixelMono8_16Bit = 0x0101,
	StPixelMono10pmsb_16Bit = 0x0102,
	StPixelMono12pmsb_16Bit = 0x0103,
	StPixelMono14pmsb_16Bit = 0x0104,
	StPixelMono16_16Bit = 0x0105,
	StPixelBayerGR8_16Bit = 0x0311,
	StPixelBayerGR10pmsb_16Bit = 0x0312,
	StPixelBayerGR12pmsb_16Bit = 0x0313,
	StPixelBayerGR14pmsb_16Bit = 0x0314,
	StPixelBayerGR16_16Bit = 0x0315,
	StPixelBayerRG8_16Bit = 0x0321,
	StPixelBayerRG10pmsb_16Bit = 0x0322,
	StPixelBayerRG12pmsb_16Bit = 0x0323,
	StPixelBayerRG14pmsb_16Bit = 0x0324,
	StPixelBayerRG16_16Bit = 0x0325,
	StPixelBayerGB8_16Bit = 0x0331,
	StPixelBayerGB10pmsb_16Bit = 0x0332,
	StPixelBayerGB12pmsb_16Bit = 0x0333,
	StPixelBayerGB14pmsb_16Bit = 0x0334,
	StPixelBayerGB16_16Bit = 0x0335,
	StPixelBayerBG8_16Bit = 0x0341,
	StPixelBayerBG10pmsb_16Bit = 0x0342,
	StPixelBayerBG12pmsb_16Bit = 0x0343,
	StPixelBayerBG14pmsb_16Bit = 0x0344,
	StPixelBayerBG16_16Bit = 0x0345,
	StPixelBayerRGB8_16Bit = 0x0401,
	StPixelBayerRGB10pmsb_16Bit = 0x0402,
	StPixelBayerRGB12pmsb_16Bit = 0x0403,
	StPixelBayerRGB14pmsb_16Bit = 0x0404,
	StPixelBayerRGB16_16Bit = 0x0405,

} StPixelType;

typedef enum
{
	StColorArrayUnknown	= 0x0000,
	StColorArrayMono	= 0x0001,	//モノクロ 
	StColorArrayRGGB	= 0x0002,	//RGrGbB配列 
	StColorArrayGRBG	= 0x0003,	//GrRBGb配列 
	StColorArrayGBRG	= 0x0004,	//GbBRGr配列 
	StColorArrayBGGR	= 0x0005,	//BGbGrR配列 
}StColorArray;

//ST_BUFFER_API StUInt32 StGetPixelBitCount( StPixelType aType );

#endif // __STPIXELTYPE_H__

