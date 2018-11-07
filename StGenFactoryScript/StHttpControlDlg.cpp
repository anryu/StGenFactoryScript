// StHttpControlDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StGenFactoryScript.h"
#include "StHttpControlDlg.h"
#include "afxdialogex.h"
#include <winhttp.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <libxml/xmlreader.h>
#include <libxml/HTMLparser.h>

//#include "../libxml2-2.7.8.win32/include/libxml/xmlreader.h"

// StHttpControlDlg ダイアログ

IMPLEMENT_DYNAMIC(StHttpControlDlg, CCustomDialogEx)

StHttpControlDlg::StHttpControlDlg(CWnd* pParent /*=NULL*/)
	: CCustomDialogEx(StHttpControlDlg::IDD, pParent)
{
	m_ClassName = _T("StHttpControlDlg");
}

StHttpControlDlg::~StHttpControlDlg()
{
}

void StHttpControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CCustomDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(StHttpControlDlg, CCustomDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_HTTPPUT, &StHttpControlDlg::OnBnClickedButtonHttpput)
	ON_BN_CLICKED(IDC_BUTTON_HTTPGET, &StHttpControlDlg::OnBnClickedButtonHttpget)
	ON_BN_CLICKED(IDOK, &StHttpControlDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// StHttpControlDlg メッセージ ハンドラー
BOOL StHttpControlDlg::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
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
	return bReval;
}


void StHttpControlDlg::OnBnClickedButtonHttpput()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	BOOL bReval = FALSE;
	
	HINTERNET hSession=NULL;
	HINTERNET hConnect=NULL;
	HINTERNET hRequest=NULL;
	CEdit objEdit;
	do
	{
		CString szCommand;
		objEdit.Attach(GetDlgItem(IDC_EDIT_HTTPPUT)->m_hWnd);
		objEdit.GetWindowText(szCommand);
		objEdit.Detach();

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
		hConnect = WinHttpConnect(hSession, m_Address.GetUnicode(), INTERNET_DEFAULT_HTTP_PORT,0);
		assert(hConnect);


		// HTTP リクエストハンドルを作成 
		//hRequest = WinHttpOpenRequest( hConnect,L"PUT", L"/", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
		hRequest = WinHttpOpenRequest( hConnect,L"PUT", szCommand, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

		// Send a request. 
		BOOL bResults = WinHttpSendRequest( hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0); 

		// リクエスト終了
		bResults = WinHttpReceiveResponse( hRequest, NULL);

		bReval = bResults;


	}while(0);
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
	
	CString szResult = _T("OK");
	if( !bReval )
	{
		szResult = _T("Error");
	}
	objEdit.Attach(GetDlgItem(IDC_EDIT_HTTPPUTRESULT)->m_hWnd);
	objEdit.SetWindowText(szResult);
	objEdit.Detach();

}

LPBYTE ReadData(HINTERNET hRequest, LPDWORD lpdwSize)
{
	LPBYTE lpData = NULL;
	LPBYTE lpPrev = NULL;
	DWORD  dwSize;
	DWORD  dwTotalSize = 0;
	DWORD  dwTotalSizePrev = 0;

	for (;;) {
		WinHttpQueryDataAvailable(hRequest, &dwSize);
		if (dwSize > 0) {
			dwTotalSizePrev = dwTotalSize;
			dwTotalSize += dwSize;
			lpData = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, dwTotalSize);
			if (lpPrev != NULL) {
				CopyMemory(lpData, lpPrev, dwTotalSizePrev);
				HeapFree(GetProcessHeap(), 0, lpPrev);
			}
			WinHttpReadData(hRequest, lpData + dwTotalSizePrev, dwSize, NULL);
			lpPrev = lpData;
		}
		else
			break;
	}

	*lpdwSize= dwTotalSize;

	return lpData;
}

BOOL SaveHTMLFileForTmp(const char *pHtml, size_t aFileSize, LPTSTR szFileName, size_t aNameSize )
{
	BOOL bReval = FALSE;
	TCHAR tchrEnvVariable[MAX_PATH];
	DWORD dwResult = GetEnvironmentVariable(
			_T("temp"),
			tchrEnvVariable,
			sizeof(tchrEnvVariable)
			);
	if (dwResult == 0) {
		dwResult = GetEnvironmentVariable(
				_T("tmp"),
				tchrEnvVariable,
				sizeof(tchrEnvVariable)
				);
		if( dwResult==0 ) swprintf( tchrEnvVariable, MAX_PATH, _T(".") );
	}
	TCHAR aFileName[MAX_PATH];
	//TMPファイル削除
	_stprintf_s( aFileName, _countof(aFileName), _T("%s\\%s"), tchrEnvVariable, _T("StGenFactoryScriptTmp.html") );
	DeleteFile(aFileName);
	//TMPファイル作成
	do
	{
		FILE	*fp = NULL;
		if( _tfopen_s(&fp,aFileName,_T("wb"))!=0 || NULL == fp )
		{
			break;
		}

		//保存
		size_t wSize = fwrite(pHtml,sizeof(BYTE),aFileSize,fp);

		if( wSize==aFileSize ) bReval = TRUE;

		fclose(fp);

	}while(0);

	if( bReval )
	{
		_tcscpy_s(szFileName,aNameSize,aFileName);
	}

	return bReval;
}


//long  SJIS2UTF16Tbl( long code )
//{
//  return( (((long)sjis2utf16_bin[2*code])<<8) + (long)sjis2utf16_bin[2*code+1] );
//}



void FindInfo(xmlNode*& element)
{ 
	for (htmlNodePtr node = element; node != NULL; node = node->next)
	{ 
		//▼1.0.0.1049
		if( node->name )
		{
		//▲1.0.0.1049

		const xmlChar *aText = node->name;

		StString szText = (const char *)aText;


//TRACE(_T(" @@@@@@@ node->name=%s\n"), szText.GetUnicode());

		//<p><p> 
//		count = (char *)node->next->next->children->content; 
		//CString a = count; 
//		printf("%s\n",count); 


//		if(xmlStrcasecmp(node->name, (const xmlChar*)"label") == 0)
		{ 
			
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
					}

			//		attr->children->name

					//if(xmlStrcasecmp(attr->name, (const xmlChar*)"HREF") == 0){
					//	printf((char *)node->children->content, "%s\n");
					//}
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
				}
			}

			//char* count; 

			//for(xmlAttrPtr attr = node->properties; attr != NULL; attr = attr->next)
			//{ 
			//	if(xmlStrcasecmp(node->name, (const xmlChar*)"p") == 0)
			//	{ 
			//		count = (char *)node->next->next->children->content; 
					//string a = count; 
			//		printf("%s\n",count); 
			//	} 
			//} 
		} 
		//▼1.0.0.1049
		}
		//▲1.0.0.1049
		//再起読み込み 
		if(node->children != NULL) 
		{
			FindInfo(node->children); 
		} 
	} 
}




void StHttpControlDlg::OnBnClickedButtonHttpget()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	BOOL bReval = FALSE;
	
	HINTERNET hSession=NULL;
	HINTERNET hConnect=NULL;
	HINTERNET hRequest=NULL;
	CEdit objEdit;
	do
	{
		CString szCommand;
		objEdit.Attach(GetDlgItem(IDC_EDIT_HTTPGET)->m_hWnd);
		objEdit.GetWindowText(szCommand);
		objEdit.Detach();

		CString szURL;
		objEdit.Attach(GetDlgItem(IDC_EDIT_HTTPADDRESS)->m_hWnd);
		objEdit.GetWindowText(szURL);
		objEdit.Detach();



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

			//StString szText = (LPSTR)lpData;
			objEdit.Attach(GetDlgItem(IDC_EDIT_HTTPGETDISPLAY)->m_hWnd);
//TRACE(szText.GetUnicode());
//TRACE(_T("\n"));
			objEdit.SetWindowText(szText.GetUnicode());
			//objEdit.SetWindowText(szText);
			//objEdit.SetWindowText(szText);
			objEdit.Detach();



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
				if( doc )
				{

					htmlNodePtr root = xmlDocGetRootElement(doc); 
					if (root != NULL)
					{ 
						FindInfo(root);
					} 
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
	
	if( !bReval )
	{
		CString szResult = _T("Error");
		objEdit.Attach(GetDlgItem(IDC_EDIT_HTTPGETDISPLAY)->m_hWnd);
		objEdit.SetWindowText(szResult);
		objEdit.Detach();
	}

}


void StHttpControlDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CCustomDialogEx::OnOK();
}


BOOL StHttpControlDlg::OnInitDialog()
{
	CCustomDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	CEdit objEdit;
	objEdit.Attach(GetDlgItem(IDC_EDIT_HTTPADDRESS)->m_hWnd);
	objEdit.SetWindowText(m_Address.GetUnicode());
	objEdit.Detach();

	//▼1.0.0.1053
	objEdit.Attach(GetDlgItem(IDC_EDIT_HTTPGET)->m_hWnd);
	objEdit.SetWindowText(_T("/"));
	objEdit.Detach();
	//▲1.0.0.1053




	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
