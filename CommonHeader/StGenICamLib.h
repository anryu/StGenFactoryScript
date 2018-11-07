/******************************************************************************
* Lib （全クラス共通）
* ----------------------------------------------------------------------------
* 
******************************************************************************/

#ifndef __STGENICAMLIB_H__
#define	__STGENICAMLIB_H__

#ifdef ST_GENICAMLIB_EXPORTS

    // Define the export symbol
	#if !defined(ST_LIB_STATIC) && defined( WIN32 )
		#define ST_GENICAMLIB_API __declspec( dllexport )
		#define ST_GENICAMLIB_CALL __stdcall
	#else
		#define ST_GENICAMLIB_API
		#define ST_GENICAMLIB_CALL
	#endif // ST_LIB_STATIC

#else // ST_GENICAMLIB_EXPORTS
	#define ST_GENICAMLIB_CALL
	#define ST_GENICAMLIB_API

#endif // ST_BUFFER_EXPORTS

#endif	/* __STGENICAMLIB_H__ */
