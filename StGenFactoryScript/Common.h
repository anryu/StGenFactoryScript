#ifdef __cplusplus
extern "C"{
#endif

//#define LOGOUT


BOOL gbCheckDirectoryExist(LPCTSTR lpszPath,BOOL bCreate);
BOOL gbCheckFileExist(LPCTSTR lpszFullPathFileName);
BOOL gbCheckDllFileExist(LPCTSTR lpszFileName);
BOOL gbShowErrorMsg(HWND hwnd, DWORD dwErrorCode);
BOOL WriteINIFile(LPCTSTR szFileName,TCHAR *szSectionName,TCHAR *szKeyName, TCHAR *szValue);
BOOL ErrorMsgProc(LPCTSTR szErrorType,LPCTSTR szErrorMsg,BOOL bShowMsg);
	BOOL	bGetSlopeAndIntercept(DOUBLE dblX1,DOUBLE dblY1,DOUBLE dblX2,DOUBLE dlbY2,DOUBLE *pdblSlope,DOUBLE *pdblIntercept);
	BOOL	bGetIntersectionOfTowLines(DOUBLE dblSlope1,DOUBLE dblIntercept1,DOUBLE dblSlope2,DOUBLE dblIntercept2,DOUBLE *pdblX,DOUBLE *pdblY);


//▼2007/09/27 v0.49 Beta1 作業者入力機能を追加
void RemoveCharFromString(LPTSTR szTarget, LPCTSTR szSearch);
void Trim(LPTSTR szTarget);
//▲2007/09/27 v0.49 Beta1 作業者入力機能を追加


//▼2007/10/15 v0.50 Beta03 リストをクリップボードへコピーする機能を追加
BOOL gbCopyToClipBoard(LPCTSTR szData);
//▲2007/10/15 v0.50 Beta03 リストをクリップボードへコピーする機能を追加

//▼2009/01/26 v0.64 Beta01 異なるカメラリンクボードに同じCOMポートが割り当てられてしまうMultiCam不具合対処
BOOL IsUniqueCOMPortName(LPCTSTR szPortName);
//▲2009/01/26 v0.64 Beta01 異なるカメラリンクボードに同じCOMポートが割り当てられてしまうMultiCam不具合対処

INT PtouchLabelPrint( CString strSerial, CString strQRCode, CString strModelName, CString strMAC );

BOOL GetErrorLogFileName( LPTSTR szFileName );
BOOL LogFileWriteString( LPCTSTR  text );
BOOL LogFileWriteCode( BYTE errdata );
BOOL LogFileWriteRecieve( PBYTE perrdata, INT iLen );
BOOL LogFileDelete( void );


//▼1.3.0.10多言語対応
BOOL SetLocale(LCID nLocale);
LCID GetLocale(void);
INT	GetSupportedLangCount(void);
LCID	GetSupportedLCID(INT index);
//▲1.3.0.10多言語対応

BOOL GetModuleFilePath(CString &szPath);


#ifdef __cplusplus
}
#endif