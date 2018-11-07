#pragma once
#include "StGenUtilsLib.h"

class ST_GENUTILSLIB_API StString
{
public:
	StString(void);
	//virtual ~StString(void);
	StString( const StString & aValue );
	StString( const char * aValue );
	StString( const wchar_t * aValue );
 	~StString(void);

	const StString &operator = ( const StString & aValue );
	const StString &operator += ( const StString & aValue );
	const StString &operator = ( const char * aValue );
	const StString &operator += ( const char * aValue );
	const StString &operator = ( const wchar_t * aValue );
	const StString &operator += ( const wchar_t * aValue );


    bool operator == ( const char *aValue ) const;
    bool operator != ( const char *aValue ) const;
    bool operator == ( const wchar_t *aValue ) const;
    bool operator != ( const wchar_t *aValue ) const;
    bool operator == ( const StString & aValue ) const;
    bool operator != ( const StString & aValue ) const;

	operator const char *() const;
    operator const wchar_t *() const;
 
	const char *GetAscii() const;
	const wchar_t *GetUnicode() const;

	//��2013/12/05 StString�Ɍ��݂̕����Z�b�g�ɑΉ�������������擾����@�\��ǉ�
//	LPCTSTR GetString()
//	{
//#ifdef _UNICODE
//		return(GetUnicode());
//#else
//		return(GetAscii());
//#endif
//
//	};
	//��2013/12/05 StString�Ɍ��݂̕����Z�b�g�ɑΉ�������������擾����@�\��ǉ�

	unsigned int GetLength() const;

private:
	void *mhThis;

};
