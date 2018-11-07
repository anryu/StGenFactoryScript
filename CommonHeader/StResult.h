/******************************************************************************
* StResult
* ----------------------------------------------------------------------------
* 
******************************************************************************/

#ifndef __StResult_H__
#define	__StResult_H__
#include "StGenUtilsLib.h"
#include "StType.h"
#include "StString.h"

class ST_GENUTILSLIB_API StResult
{
public:
	// コンストラクタ
	StResult (const StResult &aResult);
	StResult (StUInt32 aCode, const StString &aDescription);
	StResult (StUInt32 aCode);
	StResult ();
	
	// デストラクタ
	~StResult ();
	
	// メンバ関数
	StUInt32 GetCode (void) const;
	StString GetCodeString (void) const;
	StString GetDescription (void) const;
	bool IsFailure (void) const;
	bool IsOK (void) const;
	bool IsPending (void) const;
	bool IsSuccess (void) const;
	operator const char * (void) const;
	bool operator!= (const StUInt32 aCode) const;
	bool operator!= (const StResult &aB) const;
	bool operator== (const StUInt32 aCode) const;
	bool operator== (const StResult &aB) const;
	const StResult &  operator|= (const StResult &aB);
	void SetCode (StUInt32 aIn);
	void SetDescription (const StString &aDescription);
	
	const StResult &operator = ( const StResult & aB );
	const StResult &operator = ( const StUInt32 aCode );
	
	// メンバ定数
	struct Code {
		static const StUInt32 ABORTED = 1;
		static const StUInt32 AUTO_ABORTED = 2;
		static const StUInt32 BUFFER_TOO_SMALL = 3;
		static const StUInt32 CANCEL = 4;
		static const StUInt32 CANNOT_OPEN_FILE = 5;
		static const StUInt32 ERR_OVERFLOW = 6;
		static const StUInt32 FILE_ERROR = 7;
		static const StUInt32 GENERIC_ERROR = 8;
		static const StUInt32 GENICAM_XML_ERROR = 9;
		static const StUInt32 IMAGE_ERROR = 10;
		static const StUInt32 INVALID_DATA_FORMAT = 11;
		static const StUInt32 INVALID_PARAMETER = 12;
		static const StUInt32 MISSING_PACKETS = 13;
		static const StUInt32 NO_DRIVER = 14;
		static const StUInt32 NO_LICENSE = 15;
		static const StUInt32 NOT_CONNECTED = 16;
		static const StUInt32 NOT_ENOUGH_MEMORY = 17;
		static const StUInt32 NOT_FOUND = 18;
		static const StUInt32 NOT_IMPLEMENTED = 19;
		static const StUInt32 NOT_INITIALIZED = 20;
		static const StUInt32 NOT_SUPPORTED = 21;
		static const StUInt32 OK = 22;
		static const StUInt32 PENDING = 23;
		static const StUInt32 RESENDS_FAILURE = 24;
		static const StUInt32 STATE_ERROR = 25;
		static const StUInt32 TIMEOUT = 26;
		static const StUInt32 TOO_MANY_CONSECUTIVE_RESENDS = 27;
		static const StUInt32 TOO_MANY_RESENDS = 28;
		static const StUInt32 CONTROL_ERROR = 29;
		//▼1.0.0.23
		static const StUInt32 NOT_FOUND_FILE = 30;
		static const StUInt32 NOT_FOUND_DIRECTORY = 31;
		//▲1.0.0.23

		static const StUInt32 NOT_SELECTDEVICE = 100;

		static const StUInt32 NOT_CREATESTREAM = 101;
		static const StUInt32 NOT_STARTSTREAM = 102;
		static const StUInt32 NOT_STARTACQUISITION = 103;
		static const StUInt32 CREATE_ERROR = 104;

		static const StUInt32 JIGU_CONTROL_ERROR = 105;

		static const StUInt32 CHECK_ERROR = 106;
		static const StUInt32 CHECK_NG = 107;


	};	

private:
	StUInt32	mCode;
	StString	mDescription;
};

#endif	/* __StResult_H__ */

