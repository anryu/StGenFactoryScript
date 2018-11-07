/******************************************************************************
* Lib （全クラス共通）
* ----------------------------------------------------------------------------
* 
******************************************************************************/

#ifndef __STGENUTILSLIB_H__
#define	__STGENUTILSLIB_H__

#ifdef ST_GENUTILSLIB_EXPORTS

    // Define the export symbol
	#if !defined(ST_LIB_STATIC) && defined( WIN32 )
		#define ST_GENUTILSLIB_API __declspec( dllexport )
		#define ST_GENUTILSLIB_CALL __stdcall
	#else
		#define ST_GENUTILSLIB_API
		#define ST_GENUTILSLIB_CALL
	#endif // ST_LIB_STATIC

#else // ST_GICAMLIB_EXPORTS
	#define ST_GENUTILSLIB_CALL
	#define ST_GENUTILSLIB_API
	//#define ST_GICAM_CALL
	//#define ST_GICAM_API

    // Define the import symbol

	//#define ST_GICAM_API __declspec( dllimport )

	//#if !defined(ST_LIB_STATIC) && defined( WIN32 )
	//	#define ST_GICAM_API __declspec( dllimport )
	//#else
	//	#define ST_GICAM_API
	//#endif // ST_LIB_STATIC

 //   #if defined ( ST_DEBUG )
 //       #define ST_DEBUG
 //       #define ST_DEBUG_ENABLED
 //   #endif // ST_DEBUG

 //   // Define the suffix used for the static version of the library file
 //   #ifdef ST_LIB_STATIC
 //       #define ST_SUFFIX_STATIC "s"
 //   #else // ST_LIB_STATIC
 //       #define ST_SUFFIX_STATIC
 //   #endif // ST_LIB_STATIC
 //       
 //   // Define the suffix used for the debug version of the library file
 //   #if defined( _ST_DEBUG_ ) && defined( ST_DEBUG_ENABLED )
 //       #define ST_SUFFIX_DEBUG "Dbg"
 //   #else // defined( _ST_DEBUG_ ) && defined( ST_DEBUG_ENABLED )
 //       #define ST_SUFFIX_DEBUG
 //   #endif // defined( _ST_DEBUG_ ) && defined( ST_DEBUG_ENABLED )

 //   // Define the suffix used for the 64-bit version of the library file
 //   #ifdef _ST_64_
 //       #define ST_SUFFIX_64 "64"
 //   #else // _ST_64_
 //       #define ST_SUFFIX_64
 //   #endif // _ST_64_

 //   // With debug and/or static, there is an hypen after the library name
 //   #if defined( ST_LIB_STATIC ) || ( defined( _ST_DEBUG_ ) && defined( ST_DEBUG_ENABLED ) )
 //       #define ST_SUFFIX_HYPHEN "_"
 //   #else
 //       #define ST_SUFFIX_HYPHEN
 //   #endif

 //   // Ask the compiler to link the required version of the library
 //   #pragma comment( lib, "StGICam" ST_SUFFIX_64 ST_SUFFIX_HYPHEN ST_SUFFIX_STATIC ST_SUFFIX_DEBUG ".lib" )

 //   // Undefined the symbols used to link the required library file
 //   #undef ST_SUFFIX_STATIC
 //   #undef ST_SUFFIX_DEBUG
 //   #undef ST_SUFFIX_64
 //   #undef ST_SUFFIX_HYPHEN

#endif // ST_BUFFER_EXPORTS

#endif	/* __STGENUTILSLIB_H__ */

