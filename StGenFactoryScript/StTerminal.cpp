#include "StdAfx.h"
#include "StTerminal.h"

//#include <windows.h>
//#include <tchar.h>
//#include <stdio.h>
#include <winhttp.h>
#include <assert.h>
//▼1.0.0.1053
#include <libxml/xmlreader.h>
#include <libxml/HTMLparser.h>
//▲1.0.0.1053



//▼1.0.0.1073 beta2
#define _NO_TERMINAL_ADDR  _T("Nothing terminal address! Setting the terminal address.")
#define _TERMINAL_CTRL_ERROR  _T("Terminal Control Error! Check the terminal address.")
//▲1.0.0.1073 beta2


StTerminal::StTerminal(void)
	: StControl()
	, m_Address(_T(""))
{
	Initialize();
}

StTerminal::StTerminal( LPCTSTR szClassName, LPCTSTR szVarName, LPCTSTR szAddress )
	: StControl(szClassName,szVarName)
{
	if( szAddress )	
		m_Address = szAddress;
	Initialize();
}

void StTerminal::Initialize(void)
{
	m_nCurrentR = -1;
	m_nCurrentG = -1;
	m_nCurrentB = -1;
	m_nCurrentOnOffR = -1;
	m_nCurrentOnOffG = -1;
	m_nCurrentOnOffB = -1;

	m_nR = -1;
	m_nG = -1;
	m_nB = -1;
	m_nOnOffR = -1;
	m_nOnOffG = -1;
	m_nOnOffB = -1;
	//▼1.0.0.1021
	m_nCurrentW = -1;
	m_nCurrentOnOffW = -1;
	m_nW = -1;
	m_nOnOffW = -1;
	//▲1.0.0.1021
	//▼1.0.0.1053
	m_nIOPortCount = 12;
	//▲1.0.0.1053

	//▼1.0.0.1073 beta2
	m_szLastErrorMessage = _T("");
	//▲1.0.0.1073 beta2

}

StTerminal::~StTerminal(void)
{
}

//▼1.0.0.1021
/*
BOOL GetCommand(INT nIndex, CString &szCommand,LPCTSTR szArgument)
{
	BOOL bReval = TRUE;
	if( nIndex==0 )
	{
		szCommand.Format(_T("ch1onoff=%s"),szArgument);
	}
	else
	if( nIndex==1 )
	{
		szCommand.Format(_T("ch2onoff=%s"),szArgument);
	}
	else
	if( nIndex==2 )
	{
		szCommand.Format(_T("ch3onoff=%s"),szArgument);
	}
	else
	if( nIndex==3 )
	{
		szCommand.Format(_T("ch1val=%s"),szArgument);
	}
	else
	if( nIndex==4 )
	{
		szCommand.Format(_T("ch2val=%s"),szArgument);
	}
	else
	if( nIndex==5 )
	{
		szCommand.Format(_T("ch3val=%s"),szArgument);
	}
	else
	{
		bReval = FALSE;
	}
	return bReval;
}
*/

BOOL StTerminal::GetCommand(LPCTSTR szID, CString &szCommand,LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szColorName;
	INT nMode=0;
	if( _tcscmp(szID,_T("SetOnOffR"))==0 )
	{
		szColorName = _T("Red");
	}
	else
	if(_tcscmp(szID,_T("SetOnOffG"))==0 )
	{
		szColorName = _T("Green");
	}
	else
	if( _tcscmp(szID,_T("SetOnOffB"))==0 )
	{
		szColorName = _T("Blue");
	}
	else
	if( _tcscmp(szID,_T("SetOnOffW"))==0 )
	{
		szColorName = _T("White");
	}
	else
	if( _tcscmp(szID,_T("SetR"))==0 )
	{
		nMode=1;
		szColorName = _T("Red");
	}
	else
	if( _tcscmp(szID,_T("SetG"))==0 )
	{
		nMode=1;
		szColorName = _T("Green");
	}
	else
	if( _tcscmp(szID,_T("SetB"))==0 )
	{
		nMode=1;
		szColorName = _T("Blue");
	}
	else
	if( _tcscmp(szID,_T("SetW"))==0 )
	{
		nMode=1;
		szColorName = _T("White");
	}

	if( szColorName.GetLength()>0 )
	{
		if( m_pStructLightChannel )
		{
			INT nIndex = -1;
			m_pStructLightChannel->GetChannelIndex(szColorName, nIndex);
			if( nIndex>=0 )
			{
				szCommand.Format(_T("ch%i%s=%s"),nIndex+1,nMode==0?_T("onoff"):_T("val"), szArgument);
				bReval = TRUE;
			}
		}
	}

	//▼1.0.0.1053

	if( _tcscmp(szID,_T("SetAllport"))==0 )
	{
		TCHAR szText[64];
		DWORD dwIOPort = _tcstol(szArgument,NULL,0);
		for( int i=0; i<m_nIOPortCount; i++ )
		{
			//JIGU->CAM:==>  JIGU<-CAM:<==
			if( i>0 )
			{
				szCommand += _T("&");
			}
			_stprintf_s(szText,_countof(szText),_T("dir%i=%s"),i+1,((dwIOPort>>i)&1)?_T("<="):_T("=>"));
			szCommand += szText;
		}
		bReval = TRUE;
	}
	else
	if( _tcscmp(szID,_T("SetInport"))==0 )
	{
		DWORD dwIOPort = _tcstol(szArgument,NULL,0);
		szCommand.Format(_T("dir%i==>"),dwIOPort+1);
		bReval = TRUE;
	}
	else
	if( _tcscmp(szID,_T("SetOutport"))==0 )
	{
		DWORD dwIOPort = _tcstol(szArgument,NULL,0);
		szCommand.Format(_T("dir%i=<="),dwIOPort+1);
		bReval = TRUE;
	}
	else
	if( _tcscmp(szID,_T("SetAllOutportValue"))==0 )
	{
		TCHAR szText[64];
		DWORD dwIOPort = _tcstol(szArgument,NULL,0);
		for( int i=0; i<m_nIOPortCount; i++ )
		{
			if( i>0 )
			{
				szCommand += _T("&");
			}
			_stprintf_s(szText,_countof(szText),_T("output_value%i=%s"),i,((dwIOPort>>i)&1)?_T("1"):_T("0"));
			szCommand += szText;
		}
		bReval = TRUE;
	}
	else
	if( _tcscmp(szID,_T("SetOutportValue"))==0 )
	{
		DWORD dwIOPort = _tcstol(szArgument,NULL,0);
		szCommand.Format(_T("output_value%i=1"),dwIOPort);
		bReval = TRUE;
	}




	//▲1.0.0.1053


	return bReval;
}
//▲1.0.0.1021

BOOL ExecuteCommand( LPCTSTR szAddress, LPCTSTR szCommand )
{
	BOOL bReval = FALSE;

	HINTERNET hSession=NULL;
	HINTERNET hConnect=NULL;
	HINTERNET hRequest=NULL;
	do
	{
		if(!WinHttpCheckPlatform())
		{
			break;
			//printf("WinHTTP not supprted.\n");
		}

		// セッションハンドルを取得
		hSession = WinHttpOpen( L"WinHTTP Example/1.0",WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,WINHTTP_NO_PROXY_NAME,WINHTTP_NO_PROXY_BYPASS,0);
		assert(hSession);

		//▼1.0.0.1073 beta2
		if( !hSession ) break;
		//▲1.0.0.1073 beta2

		// HTTP サーバの指定 
		//hConnect = WinHttpConnect(hSession, L"www.google.com", INTERNET_DEFAULT_HTTP_PORT,0);
		hConnect = WinHttpConnect(hSession, szAddress, INTERNET_DEFAULT_HTTP_PORT,0);
		assert(hConnect);

		//▼1.0.0.1073 beta2
		if( !hConnect ) break;
		//▲1.0.0.1073 beta2

		// HTTP リクエストハンドルを作成 
		//hRequest = WinHttpOpenRequest( hConnect,L"PUT", L"/", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
		hRequest = WinHttpOpenRequest( hConnect,L"PUT", szCommand, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
		//▼1.0.0.1073 beta2
		if( !hRequest ) break;
		//▲1.0.0.1073 beta2

		// Send a request. 
		BOOL bResults = WinHttpSendRequest( hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0); 
		//▼1.0.0.1073 beta2
		if( !bResults ) break;
		//▲1.0.0.1073 beta2

		// リクエスト終了
		bResults = WinHttpReceiveResponse( hRequest, NULL);

		//▼1.0.0.1073 beta2
		//PUTではレスポンスがないか？
		/*
		//ResponseのHeader部取得
		if( bResults )
		{
			DWORD dwSize=0;
			bResults = WinHttpQueryHeaders( hRequest, 
					WINHTTP_QUERY_RAW_HEADERS_CRLF,
					WINHTTP_HEADER_NAME_BY_INDEX,
					NULL,
					&dwSize,
					WINHTTP_NO_HEADER_INDEX

				  //DWORD dwInfoLevel,
				  //LPCWSTR pwszName,
				  //LPVOID lpBuffer,
				  //LPDWORD lpdwBufferLength,
				  //LPDWORD lpdwIndex
				);

			LPBYTE lpHeader = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, dwSize);

			bResults = WinHttpQueryHeaders( hRequest, 
					WINHTTP_QUERY_RAW_HEADERS_CRLF,
					WINHTTP_HEADER_NAME_BY_INDEX,
					lpHeader,
					&dwSize,
					WINHTTP_NO_HEADER_INDEX
				);


			HeapFree(GetProcessHeap(), 0, lpHeader);


			//if( bResults )
			//{
			//	LPBYTE lpHeader = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, dwSize);

			//	//ResponseのBody部取得
			//	DWORD dwNumberOfBytesRead = 0;
			//	bResults = WinHttpReadData( hRequest, 
			//		  lpHeader,
			//		  dwSize,
			//		  &dwNumberOfBytesRead
			//		);

			//	if( dwNumberOfBytesRead==0 )
			//		bResults = FALSE;
			//	HeapFree(GetProcessHeap(), 0, lpHeader);
			//}

			//if( bResults )
			//{

			//	//ResponseのBody部取得
			//	BYTE byteData[256];
			//	DWORD dwNumberOfBytesRead = 0;
			//	bResults = WinHttpReadData( hRequest, 
			//		  byteData,
			//		  sizeof(byteData),
			//		  &dwNumberOfBytesRead
			//		);

			//	if( dwNumberOfBytesRead==0 )
			//		bResults = FALSE;
			//}


		}
		*/
		//▲1.0.0.1073 beta2

		bReval = bResults;

	}while(0);
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
	return bReval;
}




BOOL StTerminal::Execute(LPCTSTR szFunc, LPCTSTR szArgument)
{
	//▼1.0.0.1062
	if( m_nScriptCheckMode ) return TRUE;
	//▲1.0.0.1062

	BOOL bReval = FALSE;
	
	//▼1.0.0.1073 beta2
	//if( m_Address.GetLength()==0 ) return FALSE;
	if( m_Address.GetLength()==0 )
	{
		m_szLastErrorMessage = _NO_TERMINAL_ADDR;
		return FALSE;
	}
	//▲1.0.0.1073 beta2

	CString szFunction(szFunc);

	CString szCommand(_T("/?"));

	do
	{
		if( szFunc )
		{
			CString szTmp("");
			//▼1.0.0.1021
			/*
			CString szFunction(szFunc);
			if( szFunction == _T("SetOnOffR") )
			{
				GetCommand(0,szTmp,szArgument);
			}
			else
			if( szFunction == _T("SetOnOffG") )
			{
				GetCommand(1,szTmp,szArgument);
			}
			else
			if( szFunction == _T("SetOnOffB") )
			{
				GetCommand(2,szTmp,szArgument);
			}
			else
			if( szFunction == _T("SetR") )
			{
				GetCommand(3,szTmp,szArgument);
			}
			else
			if( szFunction == _T("SetG") )
			{
				GetCommand(4,szTmp,szArgument);
			}
			else
			if( szFunction == _T("SetB") )
			{
				GetCommand(5,szTmp,szArgument);
			}
			else
			{
				szTmp=szFunction;
			}
			*/

			bReval = GetCommand(szFunc,szTmp,szArgument);
			if( !bReval )
			{
				szTmp=szFunction;
			}
			//▲1.0.0.1021
			if( szTmp.GetLength()==0 ) break;
			szCommand += szTmp;
		}
		else
		{
			//▼1.0.0.1021
			//PINT pnValue[] = {&m_nOnOffR,&m_nOnOffG,&m_nOnOffB,&m_nR,&m_nG,&m_nB};
			PINT pnValue[] = {&m_nOnOffR,&m_nOnOffG,&m_nOnOffB,&m_nOnOffW,&m_nR,&m_nG,&m_nB,&m_nW};
			LPCTSTR szEachCommand[] = {_T("SetOnOffR"),_T("SetOnOffG"),_T("SetOnOffB"),_T("SetOnOffW"),_T("SetR"),_T("SetG"),_T("SetB"),_T("SetW")};
			//▲1.0.0.1021
			BOOL bFirst=TRUE;
			for( int i=0; i<_countof(pnValue); i++ )
			{
				CString szTmp("");
				if( *pnValue[i]>=0 )
				{
					CString szValue;
					szValue.Format(_T("%i"),*pnValue[i]);
					//▼1.0.0.1021
					//GetCommand(i,szTmp,szValue);
					GetCommand(szEachCommand[i],szTmp,szValue);
					//▲1.0.0.1021
				}
				if( szTmp.GetLength() )
				{
					if( bFirst )
					{
						bFirst=FALSE;
					}
					else
					{
						szCommand += _T("&");
					}
					szCommand += szTmp;
				}
			}
		}
		//ここでszCommand実行-------------------
		bReval = ExecuteCommand( m_Address, szCommand );

		//▼1.0.0.1073 beta2
		if( !bReval )
		{
			m_szLastErrorMessage = _TERMINAL_CTRL_ERROR;
		}
		//▲1.0.0.1073 beta2

	}while(0);


	return bReval;
}



BOOL StTerminal::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);

	if( szFunction == _T("OnOffR") )
	{
		m_nOnOffR = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction == _T("SetOnOffR") )
	{
		bReval = Execute( szFunc, szArgument);
	}
	else
	if( szFunction == _T("R") )
	{
		m_nR = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction == _T("SetR") )
	{
		bReval = Execute( szFunc, szArgument);
	}
	else
	if( szFunction == _T("OnOffG") )
	{
		m_nOnOffG = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction == _T("SetOnOffG") )
	{
		bReval = Execute( szFunc, szArgument);
	}
	else
	if( szFunction == _T("G") )
	{
		m_nG = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction == _T("SetG") )
	{
		bReval = Execute( szFunc, szArgument);
	}
	else
	if( szFunction == _T("OnOffB") )
	{
		m_nOnOffB = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction == _T("SetOnOffB") )
	{
		bReval = Execute( szFunc, szArgument);
	}
	else
	if( szFunction == _T("B") )
	{
		m_nB = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction == _T("SetB") )
	{
		bReval = Execute( szFunc, szArgument);
	}
	//▼1.0.0.1021
	else
	if( szFunction == _T("OnOffW") )
	{
		m_nOnOffW = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction == _T("SetOnOffW") )
	{
		bReval = Execute( szFunc, szArgument);
	}
	else
	if( szFunction == _T("W") )
	{
		m_nW = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction == _T("SetW") )
	{
		bReval = Execute( szFunc, szArgument);
	}
	//▲1.0.0.1021

	//▼1.0.0.1053
	else
	if( szFunction == _T("SetIOPortCount") || szFunction == _T("IOPortCount") )
	{
		m_nIOPortCount = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction == _T("SetOutport") )
	{
		bReval = Execute( szFunc, szArgument);
	}
	//▲1.0.0.1053

	else
	if( szFunction == _T("SetAddress") )
	{
		m_Address = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction == _T("Address") )
	{
		m_Address = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction == _T("Command") )
	{
		bReval = Execute(szArgument);
	}
	else
	if( szFunction=="Execute" )
	{
		bReval = Execute();
	}

	return bReval;

}

//▼1.0.0.1053
//StHttpControlDlg.cpp
extern LPBYTE ReadData(HINTERNET hRequest, LPDWORD lpdwSize);
extern BOOL SaveHTMLFileForTmp(const char *pHtml, size_t aFileSize, LPTSTR szFileName, size_t aNameSize );

void FindInfo_input_value(xmlNode*& element,PINT pnInputValue)
{ 
//	char* count; 
	for (htmlNodePtr node = element; node != NULL; node = node->next)
	{ 
		if( node->name )
		{
			const xmlChar *aText = node->name;

			StString szText = (const char *)aText;
//TRACE(_T(" @@@@@@@ node->name=%s\n"), szText.GetUnicode());
			
			INT nInPortNumber = -1;
			for(xmlAttrPtr attr = node->properties; attr != NULL; attr = attr->next)
			{
				aText = attr->name;
				if( aText )
				{
					szText = (const char *)aText;
//TRACE(_T(" @@@@@@@ attr->name=%s\n"), szText.GetUnicode());
				}

				aText = attr->children->content;
				if( aText )
				{
					szText = (const char *)aText;
//TRACE(_T(" @@@@@@@ attr->children->content=%s\n"), szText.GetUnicode());
					char *szCompareText = "input_value";
					int length = strlen(szCompareText);
					if( memcmp( szText.GetAscii(), szCompareText, length )==0 )
					{
						const char *szContent = szText.GetAscii();
						nInPortNumber = atoi(&szContent[length]);
					}
				}
			}
			aText = node->content;
			if( aText )
			{
				szText = (const char *)aText;
//TRACE(_T(" @@@@@@@ node->content=%s\n"), szText.GetUnicode());
			}

			//aText = node->content;
			if( node->children )
			{
				aText = node->children->content;
				if( aText )
				{
					szText = (const char *)aText;
//TRACE(_T(" @@@@@@@ node->children->content=%s\n"), szText.GetUnicode());

					if( nInPortNumber>=0 )
					{
						INT nValue = atoi(szText);
						*pnInputValue |= nValue<<nInPortNumber;
					}
				}
			}
		}
		if(node->children != NULL) 
		{
			FindInfo_input_value(node->children, pnInputValue); 
		} 
	} 
}

//▼1.0.0.1073 beta2
BOOL FindContentCheck(xmlNode*& element, const char * szName, const char * szCheckContent, size_t size)
{ 
	BOOL bReval = FALSE;

	for (htmlNodePtr node = element; node != NULL && !bReval; node = node->next)
	{ 
		if( node->name )
		{
			const xmlChar *aText = node->name;
			if( strcmp((const char *)aText, szName)==0 )
			{
				const char *szTitleContent=NULL;
				//StString szText;
				//if( node->content )
				//{
				//	szText = (const char *)node->content;
				//	TRACE(_T(" @@@@@@@ node->content=%s\n"), szText.GetUnicode());
				//}
				//if( node->doc && node->doc->name )
				//{
				//	szText = (const char *)node->doc->name;
				//	TRACE(_T(" @@@@@@@ node->doc->name=%s\n"), szText.GetUnicode());
				//}
				//if( node->next && node->next->name )
				//{
				//	szText = (const char *)node->next->name;
				//	TRACE(_T(" @@@@@@@ node->next->name=%s\n"), szText.GetUnicode());
				//}
				//if( node->children && node->children->name )
				//{
				//	szText = (const char *)node->children->name;
				//	TRACE(_T(" @@@@@@@ node->children->name=%s\n"), szText.GetUnicode());
				//}
				if( node->children && node->children->content )
				{
					szTitleContent = (const char *)node->children->content;
					//StString szText = (const char *)node->children->content;
					//TRACE(_T(" @@@@@@@ node->children->content=%s\n"), szText.GetUnicode());
				}

				if( szTitleContent )
				{
					size_t sizeContent = strlen((const char *)szTitleContent);
					if( sizeContent==size )
					{
						if( memcmp(szTitleContent,szCheckContent,sizeContent)==0 )
						{
							bReval = TRUE;
							break;
						}
					}
				}
			}
		}
		if(node->children != NULL && !bReval ) 
		{
			bReval = FindContentCheck(node->children, szName, szCheckContent, size ); 
		} 

	}
	return bReval;
}
//▲1.0.0.1073 beta2

BOOL StTerminal::GetAllport(INT &nData)
{
	BOOL bReval = FALSE;
	
	//▼1.0.0.1073 beta2
	if( m_Address.GetLength()==0 )
	{
		m_szLastErrorMessage = _NO_TERMINAL_ADDR;
		return FALSE;
	}
	//▲1.0.0.1073 beta2


	HINTERNET hSession=NULL;
	HINTERNET hConnect=NULL;
	HINTERNET hRequest=NULL;
	CEdit objEdit;
	do
	{
		CString szCommand(_T("/"));
		//CString szCommand;
		//objEdit.Attach(GetDlgItem(IDC_EDIT_HTTPGET)->m_hWnd);
		//objEdit.GetWindowText(szCommand);
		//objEdit.Detach();

		CString szURL(m_Address.GetUnicode());
		//objEdit.Attach(GetDlgItem(IDC_EDIT_HTTPADDRESS)->m_hWnd);
		//objEdit.GetWindowText(szURL);
		//objEdit.Detach();


		if(!WinHttpCheckPlatform())
		{
			break;
			//printf("WinHTTP not supprted.\n");
		}
		// セッションハンドルを取得
		hSession = WinHttpOpen( L"WinHTTP Example/1.0",WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,WINHTTP_NO_PROXY_NAME,WINHTTP_NO_PROXY_BYPASS,0);
		assert(hSession);

		// HTTP サーバの指定 
		//hConnect = WinHttpConnect(hSession, L"www.google.com", INTERNET_DEFAULT_HTTP_PORT,0);
		hConnect = WinHttpConnect(hSession, szURL, INTERNET_DEFAULT_HTTP_PORT,0);
		assert(hConnect);


		// HTTP リクエストハンドルを作成 
		//hRequest = WinHttpOpenRequest( hConnect,L"PUT", L"/", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
		hRequest = WinHttpOpenRequest( hConnect,L"GET", szCommand, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

		// Send a request. 
		BOOL bResults = WinHttpSendRequest( hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, WINHTTP_IGNORE_REQUEST_TOTAL_LENGTH, 0); 

		// リクエスト終了
		bResults = WinHttpReceiveResponse( hRequest, NULL);

		bReval = bResults;

		if( bReval )
		{
			DWORD dwSize = 0;
			WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, NULL, &dwSize, WINHTTP_NO_HEADER_INDEX);
			LPBYTE lpHeader = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, dwSize);
			WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, lpHeader, &dwSize, WINHTTP_NO_HEADER_INDEX);
			//MessageBoxW(NULL, (LPWSTR)lpHeader, L"ヘッダ", MB_OK);
			HeapFree(GetProcessHeap(), 0, lpHeader);

			LPBYTE lpData = ReadData(hRequest, &dwSize);

			//MessageBoxA(NULL, (LPSTR)lpData, "ボディ", MB_OK);
			//CString szText = (LPSTR)lpData;
			char *pText = new char [dwSize+1];
			memcpy( pText, (LPSTR)lpData, dwSize );
			pText[dwSize] = 0;



 			StString szText = pText;


			////ShiftJIS -> WCHAR 変換
			//PWCHAR pwText = new WCHAR [dwSize+1];
			//if( pwText )
			//{
			//	pwText[dwSize] = NULL;
			//	for( int i=0; i<dwSize; i++ )
			//	{
			//		if( lpData[i]


			//	}
			//}



//			CString szText = (LPCTSTR)lpData;
			delete [] pText;

			////StString szText = (LPSTR)lpData;
			//objEdit.Attach(GetDlgItem(IDC_EDIT_HTTPGETDISPLAY)->m_hWnd);
			//objEdit.SetWindowText(szText.GetUnicode());
			////objEdit.SetWindowText(szText);
			////objEdit.SetWindowText(szText);
			//objEdit.Detach();



			TCHAR szFileName[MAX_PATH];
			BOOL bSaveFile = SaveHTMLFileForTmp( (const char *)lpData, dwSize, szFileName, _countof(szFileName) );

			/*
			xmlTextReaderPtr reader;
			reader = xmlNewTextReader(lpData);
			//xmlNodePtr new_node = xmlNewNode(NULL, "html");
			//xmlNodePtr new_text = xmlNewText(lpData);
			//xmlAddChild(new_node, new_text);
			xmlTextReaderPtr reader;
			//xmlTextReaderRead();
			*/
			HeapFree(GetProcessHeap(), 0, lpData);




			if( bSaveFile )
			{
				StString aFileName = szFileName;
				//char* file = "C:\\anryu\\StGenFactoryScriptTmpIO.html"; 
				const char* file = aFileName.GetAscii(); 
				char* enc = "utf-8"; 
				htmlDocPtr doc;
				doc = htmlReadFile(file, enc, HTML_PARSE_RECOVER);
				INT nValue=-1;
				if( doc )
				{
					nValue=0;
					htmlNodePtr root = xmlDocGetRootElement(doc); 
					if (root != NULL)
					{ 
						FindInfo_input_value(root,&nValue);
						
						nData = nValue;

						bReval = TRUE;
					} 

					//▼1.0.0.1073 beta2
					//IO治具という文字列が検出できない場合はNGとする.
					//192.168.2.53のアドレス指定をして、データ自体はなぜか取得できてしまうから。自分のPCだけかもしれない。
					if( bReval )
					{
						//IO治具　バイナリエディタでみて49 4F E6 B2 BB E5 85 B7 となっていた   (UTF-8)
						const char szCompare[] = {0x49, 0x4F, 0xE6, 0xB2, 0xBB, 0xE5, 0x85, 0xB7};
						bReval = FindContentCheck(root, "title", szCompare, sizeof(szCompare));
						if( !bReval )
						{
							m_szLastErrorMessage = _TERMINAL_CTRL_ERROR;
						}
					}
					//▲1.0.0.1073 beta2
					xmlFreeDoc(doc); 
				}
				xmlCleanupParser();
				xmlCleanupCharEncodingHandlers(); 


				/*
				//XML解析用------------------------------

				//  Readerの作成
				xmlTextReaderPtr reader = xmlNewTextReaderFilename(aFileName.GetAscii());
				//xmlTextReaderPtr reader = xmlNewTextReaderFilename("http://192.168.1.223/");
				//xmlTextReaderPtr reader = xmlNewTextReaderFilename("C:\\anryu\\StGenFactoryScriptTmpIO.html");
				//xmlTextReaderPtr reader = xmlNewTextReaderFilename("C:\\anryu\\StGenFactoryScriptTmpYahoo.html");

				//xmlTextReaderPtr reader = xmlReaderForFile(aFileName.GetAscii(),NULL, 0);
				//  次のノードに移動
				int ret = xmlTextReaderRead(reader);
				ret = 1;
				while (ret == 1) 
				{
					//  現在のノードを処理(processNodeは別途定義)
					//xmlElementType nodeType;
					int nodeType;
					nodeType = xmlTextReaderNodeType(reader);

					xmlChar *name = xmlTextReaderName(reader);
					//processNode(reader);

					//state = STATE_NONE;

					StString aString;
					if( name )
						aString = (char *)name;
TRACE(TEXT("@@@@@@@@@ nodeType=%i \n"), nodeType );
TRACE(TEXT("@@@@@@@@@ name=%s \n"), aString.GetUnicode() );

					if (nodeType == XML_READER_TYPE_ELEMENT)
					{ 

						if ( xmlStrcmp(name, BAD_CAST "label") == 0 )
						{
						//	state = STATE_ITEM;
						}
						else if( xmlStrcmp(name, BAD_CAST "input") == 0 )
						{
						//	state = STATE_PRICE;
						}




					}
					else if (nodeType == XML_READER_TYPE_NONE) {   // 0

					}
					else if (nodeType == XML_READER_TYPE_ATTRIBUTE) {   // 2

					}

					else if (nodeType == XML_READER_TYPE_TEXT) {          //  テキスト
					
						xmlChar *value = xmlTextReaderValue(reader);
						aString = (char *)value;
TRACE(TEXT("@@@@@@@@@ value=%s \n"), aString.GetUnicode() );
						xmlFree(value);
					}

					else if (nodeType == XML_READER_TYPE_CDATA) {   // 4

					}
					else if (nodeType == XML_READER_TYPE_ENTITY_REFERENCE) {   // 5

					}
					else if (nodeType == XML_READER_TYPE_ENTITY) {   // 6

					}
					else if (nodeType == XML_READER_TYPE_PROCESSING_INSTRUCTION) {   // 7

					}
					else if (nodeType == XML_READER_TYPE_COMMENT) {   // 8

					}
					else if (nodeType == XML_READER_TYPE_DOCUMENT) {   // 9

					}
					else if (nodeType == XML_READER_TYPE_DOCUMENT_TYPE) {   // 10

					}
					else if (nodeType == XML_READER_TYPE_DOCUMENT_FRAGMENT) {   // 11

					}
					else if (nodeType == XML_READER_TYPE_NOTATION) {   // 12

					}
					else if (nodeType == XML_READER_TYPE_WHITESPACE) {   // 13

					}
					else if (nodeType == XML_READER_TYPE_SIGNIFICANT_WHITESPACE) {   // 14

					}

					else if (nodeType == XML_READER_TYPE_END_ELEMENT) {   //  終了

					}

					else if (nodeType == XML_READER_TYPE_END_ENTITY) {   // 16

					}
					else if (nodeType == XML_READER_TYPE_XML_DECLARATION ) {   // 17

					}



					xmlFree(name);

					//  次のノードに移動
					ret = xmlTextReaderRead(reader);
				}


				xmlFreeTextReader(reader);
				*/

				DeleteFile(szFileName);
				
			}
		}
	}while(0);
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	return bReval;
}

//▼1.3.0.70
bool FindInfo_light_value(xmlNode*& element,const char *featureName, PINT pnValue)
{ 
	bool bReval = false;
	//char* count;

	for (htmlNodePtr node = element; node != NULL && bReval==false; node = node->next)
	{ 
		bool bGetNextVaule = false;
		if( node->name )
		{
			const xmlChar *aText = node->name;

			StString szText = (const char *)aText;
//TRACE(_T(" @@@@@@@ node->name=%s\n"), szText.GetUnicode());
			
			INT nInPortNumber = -1;
			for(xmlAttrPtr attr = node->properties; attr != NULL; attr = attr->next)
			{
				aText = attr->name;
				if( aText )
				{
					szText = (const char *)aText;
//TRACE(_T(" @@@@@@@ attr->name=%s\n"), szText.GetUnicode());
				}

				aText = attr->children->content;
				if( aText )
				{
					if( !bGetNextVaule )
					{
						szText = (const char *)aText;
//TRACE(_T(" @@@@@@@ attr->children->content=%s\n"), szText.GetUnicode());


						//int length = strlen(featureName);
						if( strcmp( (const char *)aText, featureName )==0 )
						{
							bGetNextVaule = true;
							//const char *szContent = szText.GetAscii();
							//pnValue = atoi(&szContent[length]);
						}
					}
					else
					{
						*pnValue = atoi((const char *)aText);
						bReval = true;
						break;
					}
				}
			}
			aText = node->content;
			if( aText )
			{
				szText = (const char *)aText;
//TRACE(_T(" @@@@@@@ node->content=%s\n"), szText.GetUnicode());

				//CH1ONOFF:1
				int length = strlen(featureName);
				if( memcmp( szText.GetAscii(), featureName, length )==0 )
				{
					const char *szContent = szText.GetAscii();
					*pnValue = atoi(&szContent[length]);
					bReval = true;
				}
			}

			//aText = node->content;
			if( node->children )
			{
				aText = node->children->content;
				if( aText )
				{
					szText = (const char *)aText;
//TRACE(_T(" @@@@@@@ node->children->content=%s\n"), szText.GetUnicode());

					//if( nInPortNumber>=0 )
					//{
					//	INT nValue = atoi(szText);
					//	*pnInputValue |= nValue<<nInPortNumber;
					//}
				}
			}
		}
		if( bReval ) break;
		if(node->children != NULL) 
		{
			FindInfo_light_value(node->children, featureName, pnValue); 
		} 
	} 
	return bReval;
}


BOOL StTerminal::GetLightStatus(const char *szID, INT &nData)
{
	BOOL bReval = FALSE;
	
	//▼1.0.0.1073 beta2
	if( m_Address.GetLength()==0 )
	{
		m_szLastErrorMessage = _NO_TERMINAL_ADDR;
		return FALSE;
	}
	//▲1.0.0.1073 beta2

	HINTERNET hSession=NULL;
	HINTERNET hConnect=NULL;
	HINTERNET hRequest=NULL;
	CEdit objEdit;
	do
	{
		CString szCommand(_T("/"));

		CString szURL(m_Address.GetUnicode());


		if(!WinHttpCheckPlatform())
		{
			break;
			//printf("WinHTTP not supprted.\n");
		}
		// セッションハンドルを取得
		hSession = WinHttpOpen( L"WinHTTP Example/1.0",WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,WINHTTP_NO_PROXY_NAME,WINHTTP_NO_PROXY_BYPASS,0);
		assert(hSession);
		//▼1.0.0.1073 beta2
		if( !hSession ) break;
		//▲1.0.0.1073 beta2

		// HTTP サーバの指定 
		hConnect = WinHttpConnect(hSession, szURL, INTERNET_DEFAULT_HTTP_PORT,0);
		assert(hConnect);
		//▼1.0.0.1073 beta2
		if( !hConnect ) break;
		//▲1.0.0.1073 beta2


		// HTTP リクエストハンドルを作成 
		hRequest = WinHttpOpenRequest( hConnect,L"GET", szCommand, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
		//▼1.0.0.1073 beta2
		if( !hRequest ) break;
		//▲1.0.0.1073 beta2

		// Send a request. 
		BOOL bResults = WinHttpSendRequest( hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, WINHTTP_IGNORE_REQUEST_TOTAL_LENGTH, 0); 

		//▼1.0.0.1073 beta2
		if( !bResults ) break;
		//▲1.0.0.1073 beta2

		// リクエスト終了
		bResults = WinHttpReceiveResponse( hRequest, NULL);

		bReval = bResults;

		if( bReval )
		{
			DWORD dwSize = 0;
			WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, NULL, &dwSize, WINHTTP_NO_HEADER_INDEX);
			LPBYTE lpHeader = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, dwSize);
			WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, lpHeader, &dwSize, WINHTTP_NO_HEADER_INDEX);
			HeapFree(GetProcessHeap(), 0, lpHeader);

			LPBYTE lpData = ReadData(hRequest, &dwSize);

			//MessageBoxA(NULL, (LPSTR)lpData, "ボディ", MB_OK);
			//CString szText = (LPSTR)lpData;
			char *pText = new char [dwSize+1];
			memcpy( pText, (LPSTR)lpData, dwSize );
			pText[dwSize] = 0;

 			StString szText = pText;
			delete [] pText;

			TCHAR szFileName[MAX_PATH];
			BOOL bSaveFile = SaveHTMLFileForTmp( (const char *)lpData, dwSize, szFileName, _countof(szFileName) );

			HeapFree(GetProcessHeap(), 0, lpData);

			if( bSaveFile )
			{
				StString aFileName = szFileName;
				//char* file = "C:\\anryu\\StGenFactoryScriptTmpIO.html"; 
				const char* file = aFileName.GetAscii(); 
				char* enc = "utf-8"; 
				htmlDocPtr doc;
				doc = htmlReadFile(file, enc, HTML_PARSE_RECOVER);
				INT nValue=-1;
				if( doc )
				{
					nValue=0;
					htmlNodePtr root = xmlDocGetRootElement(doc); 
					if (root != NULL)
					{ 
						//FindInfo_input_value(root,&nValue);
						FindInfo_light_value(root,szID,&nValue);
						nData = nValue;

						bReval = TRUE;
					} 

					//▼1.0.0.1073 beta2
					//PTPという文字列が検出できない場合はNGとする.
					//IO冶具と同じ処理をする
					if( bReval )
					{
						//PHP
						const char szCompare[] = "PHP";
						bReval = FindContentCheck(root, "title", szCompare, strlen(szCompare));
						if( !bReval )
						{
							m_szLastErrorMessage = _TERMINAL_CTRL_ERROR;
						}
					}
					//▲1.0.0.1073 beta2



					xmlFreeDoc(doc); 
				}
				xmlCleanupParser();
				xmlCleanupCharEncodingHandlers(); 


				/*
				//XML解析用------------------------------

				//  Readerの作成
				xmlTextReaderPtr reader = xmlNewTextReaderFilename(aFileName.GetAscii());
				//xmlTextReaderPtr reader = xmlNewTextReaderFilename("http://192.168.1.223/");
				//xmlTextReaderPtr reader = xmlNewTextReaderFilename("C:\\anryu\\StGenFactoryScriptTmpIO.html");
				//xmlTextReaderPtr reader = xmlNewTextReaderFilename("C:\\anryu\\StGenFactoryScriptTmpYahoo.html");

				//xmlTextReaderPtr reader = xmlReaderForFile(aFileName.GetAscii(),NULL, 0);
				//  次のノードに移動
				int ret = xmlTextReaderRead(reader);
				ret = 1;
				while (ret == 1) 
				{
					//  現在のノードを処理(processNodeは別途定義)
					//xmlElementType nodeType;
					int nodeType;
					nodeType = xmlTextReaderNodeType(reader);

					xmlChar *name = xmlTextReaderName(reader);
					//processNode(reader);

					//state = STATE_NONE;

					StString aString;
					if( name )
						aString = (char *)name;
TRACE(TEXT("@@@@@@@@@ nodeType=%i \n"), nodeType );
TRACE(TEXT("@@@@@@@@@ name=%s \n"), aString.GetUnicode() );

					if (nodeType == XML_READER_TYPE_ELEMENT)
					{ 

						if ( xmlStrcmp(name, BAD_CAST "label") == 0 )
						{
						//	state = STATE_ITEM;
						}
						else if( xmlStrcmp(name, BAD_CAST "input") == 0 )
						{
						//	state = STATE_PRICE;
						}




					}
					else if (nodeType == XML_READER_TYPE_NONE) {   // 0

					}
					else if (nodeType == XML_READER_TYPE_ATTRIBUTE) {   // 2

					}

					else if (nodeType == XML_READER_TYPE_TEXT) {          //  テキスト
					
						xmlChar *value = xmlTextReaderValue(reader);
						aString = (char *)value;
TRACE(TEXT("@@@@@@@@@ value=%s \n"), aString.GetUnicode() );
						xmlFree(value);
					}

					else if (nodeType == XML_READER_TYPE_CDATA) {   // 4

					}
					else if (nodeType == XML_READER_TYPE_ENTITY_REFERENCE) {   // 5

					}
					else if (nodeType == XML_READER_TYPE_ENTITY) {   // 6

					}
					else if (nodeType == XML_READER_TYPE_PROCESSING_INSTRUCTION) {   // 7

					}
					else if (nodeType == XML_READER_TYPE_COMMENT) {   // 8

					}
					else if (nodeType == XML_READER_TYPE_DOCUMENT) {   // 9

					}
					else if (nodeType == XML_READER_TYPE_DOCUMENT_TYPE) {   // 10

					}
					else if (nodeType == XML_READER_TYPE_DOCUMENT_FRAGMENT) {   // 11

					}
					else if (nodeType == XML_READER_TYPE_NOTATION) {   // 12

					}
					else if (nodeType == XML_READER_TYPE_WHITESPACE) {   // 13

					}
					else if (nodeType == XML_READER_TYPE_SIGNIFICANT_WHITESPACE) {   // 14

					}

					else if (nodeType == XML_READER_TYPE_END_ELEMENT) {   //  終了

					}

					else if (nodeType == XML_READER_TYPE_END_ENTITY) {   // 16

					}
					else if (nodeType == XML_READER_TYPE_XML_DECLARATION ) {   // 17

					}



					xmlFree(name);

					//  次のノードに移動
					ret = xmlTextReaderRead(reader);
				}


				xmlFreeTextReader(reader);
				*/

				DeleteFile(szFileName);
				
			}
		}
	}while(0);
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	return bReval;
}
//▲1.3.0.70

BOOL StTerminal::GetFunction(LPCTSTR szFunc, LPCTSTR szArgument, INT &nData)
{
	BOOL bReval = FALSE;


	CString szFunction(szFunc);
	if( szFunction == _T("GetAllport") )
	{
		bReval = GetAllport(nData);
	}
	//▼1.3.0.70
	else
	if( szFunction == _T("GetLightValue") || szFunction == _T("LightValue")  )
	{
		char szID[32];
		sprintf_s(szID,_countof(szID),"ch%dval",_ttoi(szArgument));
		bReval = GetLightStatus( szID, nData );
	}
	else
	if( szFunction == _T("GetLightOnOff") || szFunction == _T("LightOnOff")  )
	{
		char szID[32];
		sprintf_s(szID,_countof(szID),"CH%dONOFF:",_ttoi(szArgument));
		bReval = GetLightStatus( szID, nData );
	}
	else
	{
		LPCTSTR szCommand[] = {_T("GetLightValue"),_T("LightValue"),_T("GetLightOnOff"),_T("LightOnOff")};
		for( int i=0; i<_countof(szCommand); i++ )
		{
			if( szFunction.Left(_tcslen(szCommand[i])) == szCommand[i] )
			{
				INT nChannel = _ttoi(szFunction.Mid(_tcslen(szCommand[i])));
				char szID[32];
				if( i<2 ) sprintf_s(szID,_countof(szID),"ch%dval",nChannel);
				else	  sprintf_s(szID,_countof(szID),"CH%dONOFF:",nChannel);
				bReval = GetLightStatus( szID, nData );
				break;
			}
		}
	}
	//▲1.3.0.70
	return bReval;
}

BOOL StTerminal::ExecutePublic(LPCTSTR szFunc=NULL, LPCTSTR szArgument=NULL)
{
	return Execute( szFunc, szArgument );
}
//▲1.0.0.1053

