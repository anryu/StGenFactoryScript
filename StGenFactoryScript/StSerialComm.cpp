#include "StdAfx.h"
#include "StSerialComm.h"


StSerialComm::StSerialComm(void)
	//▼1.0.0.1017
	: StControl()
	//: m_ClassName(_T(""))
	//, m_VarName(_T(""))
	//▲1.0.0.1017
{
	Initialize();
}

StSerialComm::StSerialComm( LPCTSTR szClassName, LPCTSTR szVarName, LPCTSTR szPort )
	//▼1.0.0.1017
	: StControl(szClassName,szVarName)
	//: m_ClassName(szClassName)
	//, m_VarName(szVarName)
	//▲1.0.0.1017
{
	if( szPort )	
		SetPortName(szPort);
	Initialize();
}

void StSerialComm::Initialize(void)
{
	m_szRecieveCmpData = _T("");
	m_szRecieveResultData = _T("");

	m_szSendSTX = "";
	m_szSendETX = "0d";	//0d初期値にしておく。すでに治具で使用しているため
	m_szRecieveSTX = "";
	m_szRecieveETX = "";
	//▼1.0.0.1015
	m_szRcvReplaceSpace = "";
	//▲1.0.0.1015

	//▼1.0.0.1015
	m_nBaudRate = 9600;
	//m_szBaudRateCommand = "";
	//m_szBaudRateReturn = "";

	m_nDelayTime = 0;
	//▲1.0.0.1015

	//▼1.0.0.1022
	m_nRecieveResultValue = -1;
	//▲1.0.0.1022
	//▼1.0.0.1043
	m_nRecieveDelayTime = 0;
	//▲1.0.0.1043

	//▼1.0.0.1047
	m_bControl = FALSE;
	m_bCompareCheck = FALSE;
	//▲1.0.0.1047

	//▼1.0.0.1048
	m_szParity = _T("");
	m_dblStopBits = -1.0;
	m_nByteSize = -1;
	//▲1.0.0.1048

}

StSerialComm::~StSerialComm(void)
{
}

BOOL IsNumerical( LPCTSTR szData )
{
	BOOL bNumerical=FALSE;
	CString szString(szData);
	if( szString.Left(2)==_T("0x") || szString.Left(2)==_T("0X") )
	{
		bNumerical = TRUE;
		//szString = szString.Mid(2);
	}

	if( bNumerical==FALSE )
	{
		CString szRemCommaString = szString;
		szRemCommaString.Replace(_T(","),_T(""));
		szRemCommaString.Replace(_T(" "),_T(""));	//必要？
		if(  (szRemCommaString.GetLength()&1)==0 )
		{
			CString res = szRemCommaString.SpanIncluding(_T("0123456789abcdefABCDEF"));
			if(res == szRemCommaString)
			{
				bNumerical = TRUE;
			}
		}
	}

	//▼1.0.0.1021
	if( bNumerical==TRUE )
	{
		if( szString.GetLength()==0 ) bNumerical=FALSE;
	}
	//▲1.0.0.1021

	return bNumerical;
}

//▼1.0.0.1022
//--------------------------------------------
// 0:バイナリーモード format:02 00 01 01 00 03 or 02,00,01,01,00,03 0x020001010003 など
// 1:Ascii    format: GAIN=1 or "GAIN=1" など( "でくぎられているか、あるいは必ず数値以外が入っていなければいけない)
// 2:バイナリー  ASCII混合(TAKANO)       format:0x02,"RID1"0x0d など（"と,両方ある場合）
//--------------------------------------------
INT GetNumericalMode( LPCTSTR szData )
{

	INT nMode = -1;
	CString szString(szData);
	INT nCommaPos = szString.Find(_T(","));
	INT nDblQPos = szString.Find(_T("\""));

	//▼1.0.0.1033
	//if( nCommaPos>=0 && nCommaPos>=0 )
	if( nCommaPos>=0 && nDblQPos>=0 )
	//▲1.0.0.1033
		nMode = 2;
	if( nMode<0 )
	{
		nMode = IsNumerical( szData )==TRUE?0:1;
	}

	return nMode;
}


//全てカンマ区切り
//文字列をBINARY変換 -> 
//▼1.0.0.1031
//BOOL changeTextToBin(LPCTSTR szData,PBYTE pRecv, size_t size)
BOOL changeTextToBin(LPCTSTR szData,PBYTE pRecv, size_t size, PINT pnAsteriskPos)
//▲1.0.0.1031
{
	BOOL bReval = TRUE;
	CString szText(szData);

	INT nPos = 0;
	do
	{
		if( nPos>=(INT)size )
		{
			bReval = FALSE;
			break;
		}
		INT nCammaPos = szText.Find(_T(","));
		CString szText2 = szText;
		if( nCammaPos>=0 )
		{
			szText2 = szText.Left(nCammaPos);
			szText = szText.Mid(nCammaPos+1);
		}
		else
		{
			szText = _T("");
		}
		if( szText2.Left(1)!=_T("\"") )
		{
			//Bin
			pRecv[nPos++] = (BYTE)_tcstol(szText2,NULL,0);
		}
		else
		{	
			//Ascii
			szText2 = szText2.Mid(1);
			INT nQPos = szText2.Find(_T("\""));
			if( nQPos>=0 )
			{
				szText2 = szText2.Left(nQPos);
			}
			INT nLength = szText2.GetLength();
			if( nPos+nLength>=size)
			{
				bReval = FALSE;
				break;
			}
			//sprintf( (char *)&pRecv[nPos], "%s", (const char *)szText2 );
			StString sTmp = szText2;
			strcpy((char *)&pRecv[nPos],sTmp.GetAscii());

			//▼1.0.0.1031
			if( pnAsteriskPos )
			{
				*pnAsteriskPos = -1;
				const char *pAscii = sTmp.GetAscii();
				for( int i=0; i<sTmp.GetLength(); i++ )
				{
					if( pAscii[i]=='*' )
					{
						*pnAsteriskPos = i + nPos;
					}
				}
			}
			//▲1.0.0.1031

			nPos += nLength;
		}
		if( szText.GetLength()==0 ) break;
	}while(1);
	return bReval;
}


//全てカンマ区切り
//BINARYを文字列変換 -> 
void GetRecieveText(CString &szDst, LPCSTR szSrc,LPCSTR szRef)
{
	//BOOL bReval = TRUE;
	szDst = _T("");
	CString szRef1(szRef);	//カンマ区切り
	INT nPos = 0;
	TCHAR szTmp[64];

	INT nSrcLen = strlen(szSrc);
	do
	{

		INT nCammaPos = szRef1.Find(_T(","));
		CString szText2 = szRef1;
		if( nCammaPos>=0 )
		{
			szText2 = szRef1.Left(nCammaPos);
			szRef1 = szRef1.Mid(nCammaPos+1);
		}
		else
		{
			szRef1 = _T("");
		}

		if( szText2.Left(1)!=_T("\"") )
		{
			//Bin
			if( szDst.GetLength()>0 )
				szDst += _T(",");
			_stprintf_s(szTmp,_countof(szTmp),_T("0x%02x"),szSrc[nPos++]);
			szDst += szTmp;
		}
		else
		{
			//Ascii
			//
			szText2 = szText2.Mid(1);
			INT nQPos = szText2.Find(_T("\""));
			if( nQPos>=0 )
			{
				szText2 = szText2.Left(nQPos);
			}
			INT nLength = szText2.GetLength();
			
			//szRef1からBin部の長さを取得し、
			INT nNextBinLength = 0;
			CString szCheck(szRef1);
			BYTE byteTmp[256];
			memset(byteTmp,0,sizeof(byteTmp));
			do
			{
				INT nCammaPos2 = szCheck.Find(_T(","));
				CString szCheck2 = szCheck;
				if( nCammaPos2>=0 )
				{
					szCheck2 = szCheck2.Left(nCammaPos2);
					szCheck = szCheck.Mid(nCammaPos2+1);
				}
				else
				{
					break;
				}
				if( szCheck2.Left(1)!=_T("\"") )
				{
					byteTmp[nNextBinLength] = (BYTE)_tcstol(szCheck2,NULL,0);
					nNextBinLength++;
				}
				else
				{
					break;
				}
			}while(1);

			//代入szSrc[nPos]以降にあるかどうか確認
			//CString szCheck(szSrc[nPos]);
			INT nTextLen = 0;
			INT nLoop = strlen(&szSrc[nPos]) - nNextBinLength + 1;
			PBYTE pSrc = (PBYTE)&szSrc[nPos];

			//▼1.0.0.1024
			if( nNextBinLength>0 )
			{
			//▲1.0.0.1024
				for( int i=0; i<nLoop; i++ )
				{

//TRACE(TEXT("@@@@@@@@@ pSrc[%x:%x:] \n"),pSrc[0],pSrc[1]  );
//TRACE(TEXT("@@@@@@@@@ byteTmp[%x:%x:] \n"),byteTmp[0],byteTmp[1]  );

					if( memcmp( &pSrc[i], byteTmp, nNextBinLength )==0 )
					{
						nTextLen = i;
						break;
					}
				}
			//▼1.0.0.1024
			}
			else
			{
				nTextLen = strlen((char *)pSrc);
			}
			//▲1.0.0.1024
			//-----------------------------------------------
			if( nTextLen>0 )
			{
				if( szDst.GetLength()>0 )
					szDst += _T(",");
				szDst += _T("\"");

				memset(byteTmp,0,sizeof(byteTmp));
				memcpy(byteTmp,pSrc,nTextLen);
				szDst += (const char *)byteTmp;

				szDst += _T("\"");

				nPos+=nTextLen;
			}

		}
		//if( nCammaPos<0 ) break;
		if( nPos>=nSrcLen ) break;

	}while(1);
	//return bReval;

}
//▲1.0.0.1022



//▼1.0.0.1015
BOOL MakeRecieveCmpBin( StString &szRecieveCmpData, PWORD *ppwCmpRcvData, size_t &rcvSize )
{
	BOOL bReval = FALSE;
	//size_t rcvSize = 4;
	//PWORD pwCmpRcvData=NULL;
	if( szRecieveCmpData.GetLength()>0 )
	{

		CString szRevString(szRecieveCmpData.GetUnicode());
		if( szRevString.Left(2)==_T("0x") || szRevString.Left(2)==_T("0X") )
		{
			szRevString = szRevString.Mid(2);
		}
		szRevString.Replace(_T(","),_T(""));
		szRevString.Replace(_T(" "),_T(""));
		rcvSize = szRevString.GetLength()/2;

		PWORD pwRcvData = new WORD [rcvSize];
		//*ppwCmpRcvData = new WORD [rcvSize];
		if( pwRcvData )
		{
			for( int i=0; i<(int)rcvSize; i++ )
			{
				if( szRevString.Mid(i*2, 2).Compare(_T("XX")) == 0 )
				{
					pwRcvData[i] = 0xffff;
				}
				else
				{
					pwRcvData[i] = (WORD)_tcstol( szRevString.Mid(i*2, 2), nullptr, 16 )&0xff;
				}
			}
			bReval = TRUE;
		}
		*ppwCmpRcvData = pwRcvData;
	}
	return TRUE;
}
//▲1.0.0.1015

//▼1.0.0.1022
//*********************************************************
// 第１引数にワイルドカードを含む文字列を指定する。
// ２つの文字列が一致すれば真、さもなくば偽を返す。
//*********************************************************
int // 一致すれば 真
strmatch
	(
		const char *ptn, // ワイルドカード
		const char *str  // 比較する文字列
	)
{
	switch( *ptn )
	{
		case '\0':
			return '\0' == *str;
		case '*':
			return strmatch( ptn+1, str )
			    || (('\0' != *str) && strmatch( ptn, str+1 ));
		case '?':
			return ('\0' != *str)
			    && strmatch( ptn+1, str+1 );
		default:
			return ((unsigned char)*ptn == (unsigned char)*str)
			    && strmatch( ptn+1, str+1 );
	}
}//strmatch


bool // 一致すれば 真
wildvalue
	(
		const char *ptn, // ワイルドカード
		const char *str  // 比較する文字列
		, int *value
	)
{

	int nstrlength = strlen(str);
	//int nptnlength = strlen(ptn);
	//int nstrpos=0;
	//int nptnpos=0;

	char *strbuf = new char [nstrlength + 1];
	if( !strbuf ) return false;
	memset(strbuf,0,nstrlength + 1);
	int pos = 0;
	bool reval = true;
	do
	{

		switch( *ptn )
		{
			case '\0':
				if( *str != '\0' )
					reval = false;
				break;
			case '*':
				do
				{
					if( ptn[1] == *str )
					{
						break;
					}
					else
					{
						strbuf[pos++] = *str++;
					}
				}while(1);
				ptn++;
				//return strmatch( ptn+1, str ) || (('\0' != *str) && strmatch( ptn, str+1 ));
			case '?':
				strbuf[pos++] = *str++;
				ptn++;
				//return ('\0' != *str) && strmatch( ptn+1, str+1 );
			default:
				if( *ptn++!=*str++ )
				{
					reval = false;
					break;
				}
		}
		if( *str == '\0' )
		{
			if( *str != '\0' )
				reval = false;
			break;
		}
	}while(1);

	if( reval && strlen(strbuf)>0 )
	{
		int base = 10;
		//頭に0があると８進数とみなしてしまう（TAKANOでは頭に0があることがあるためBASEを変える
		if( memcmp(strbuf,(const void *)"0x",2)==0 || memcmp(strbuf,(const void *)"0X",2)==0 
			|| memcmp(strbuf,(const void *)"0b",2)==0 || memcmp(strbuf,(const void *)"0B",2)==0 
			)
		{
			base = 0;
		}
		*value = strtol(strbuf,NULL,base);
	}
	delete [] strbuf;
	return reval;
}
//▲1.0.0.1022


BOOL StSerialComm::SendData( LPCTSTR szData )
{

	BOOL bReval = FALSE;

	//16進数値か文字列か判別
	BOOL bNumerical=FALSE;

	/*
	//カンマを削除
	if( szString.Left(2)==_T("0x") )
	{
		bNumerical = TRUE;
		szString = szString.Mid(2);
	}
	if( bNumerical==FALSE )
	{
		CString szRemCommaString = szString;
		szRemCommaString.Replace(_T(","),_T(""));
		if(  (szRemCommaString.GetLength()&1)==0 )
		{
			//CString res = szString.SpanIncluding(_T("0123456789abcdefABCDEF"));
			CString res = szRemCommaString.SpanIncluding(_T("0123456789abcdefABCDEF"));
			if(res == szRemCommaString)
			{
				bNumerical = TRUE;
			}
		}
	}
	*/

	//▼1.0.0.1047
	m_bControl = TRUE;
	m_bCompareCheck = TRUE;
	//▲1.0.0.1047

	//▼1.0.0.1022
	//bNumerical = IsNumerical( szData );
	INT nNumericalMode = GetNumericalMode( szData );
	//if( bNumerical )
	if( nNumericalMode==0 )
	//▲1.0.0.1022
	{
		CString szString(szData);
		if( szString.Left(2)==_T("0x") || szString.Left(2)==_T("0X") )
		{
			szString = szString.Mid(2);
		}

		szString.Replace(_T(","),_T(""));
		szString.Replace(_T(" "),_T(""));	//必要？
		//数値変換
		size_t sendSize = szString.GetLength()/2;
		PBYTE  pbyteSend = new BYTE [sendSize];
		if( pbyteSend )
		{
			for( int i=0; i<sendSize; i++ )
			{
				//▼1.0.0.1015
				//pbyteSend[i] = (BYTE)_tcstol(szString.Mid(i*2,2),NULL,0)&0xFF;
				pbyteSend[i] = (BYTE)_tcstol(szString.Mid(i*2,2),NULL,16)&0xFF;
				//▲1.0.0.1015
			}
			size_t rcvSize = 4;
			PWORD pwCmpRcvData=NULL;

			//▼1.0.0.1015
			MakeRecieveCmpBin( m_szRecieveCmpData, &pwCmpRcvData, rcvSize );
			//if( m_szRecieveCmpData.GetLength()>0 )
			//{

			//	CString szRevString(m_szRecieveCmpData.GetUnicode());
			//	if( szRevString.Left(2)==_T("0x") || szRevString.Left(2)==_T("0X") )
			//	{
			//		szRevString = szRevString.Mid(2);
			//	}
			//	szRevString.Replace(_T(","),_T(""));
			//	szRevString.Replace(_T(" "),_T(""));
			//	rcvSize = szRevString.GetLength()/2;
			//	pwCmpRcvData = new WORD [rcvSize];
			//	if( pwCmpRcvData )
			//	{
			//		for( int i=0; i<rcvSize; i++ )
			//		{
			//			if( szRevString.Mid(i*2, 2).Compare(_T("XX")) == 0 )
			//			{
			//				pwCmpRcvData[i] = 0xffff;
			//			}
			//			else
			//			{
			//				pwCmpRcvData[i] = (WORD)_tcstol( szRevString.Mid(i*2, 2), nullptr, 16 )&0xff;
			//			}
			//		}
			//	}
			//}
			//▲1.0.0.1015

			PBYTE pbyteRcv = new BYTE [rcvSize];
			//BYTE byteRcv[4];
			//size_t rcvSize = sizeof(byteRcv);
			//bReval = SendBin( pbyteSend, sendSize, byteRcv, &rcvSize );
			if( pbyteRcv )
			{
				size_t getSize = rcvSize;

				bReval = SendBin( pbyteSend, sendSize, pbyteRcv, &getSize );
				//▼1.0.0.1047
				m_bControl = bReval;
				//▲1.0.0.1047
				if( bReval )
				{
					CString tmp;
					CString szRecieve(_T(""));
					for( int i=0; i<getSize; i++ )
					{
						if( i>0 )	szRecieve+=_T(",");
						tmp.Format(_T("%02x"), pbyteRcv[i] );
						szRecieve += tmp;
					}
					m_szRecieveResultData = szRecieve;


					if( pwCmpRcvData )
					{
						if( getSize != rcvSize )
							bReval = FALSE;
						if( bReval )
						{
							//比較
							for( int j=0; j<(int)rcvSize; j++ )
							{
								if( (pwCmpRcvData[j]>>8)>0 )	//XXの位置は比較しない
									continue;
								if( pwCmpRcvData[j]!=(WORD)pbyteRcv[j] )
								{
									bReval = FALSE;
									break;
								}
							}
						}
						//▼1.0.0.1047
						m_bCompareCheck = bReval;
						//▲1.0.0.1047
					}
				}
				delete [] pbyteRcv;
			}
			if( pwCmpRcvData )	delete [] pwCmpRcvData;
			delete [] pbyteSend;
		}
	}
	else
	//▼1.0.0.1022
	if( nNumericalMode==1 )
	//▲1.0.0.1022
	{
		//文字列
		//bReval = SendText( szData );

		//▼1.0.0.1015
		//TCHAR szRecieveData[256];
		BYTE szRecieveData[256];
		memset(szRecieveData, 0, sizeof(szRecieveData));
		//▲1.0.0.1015

		//▼1.0.0.1021
		//bReval = SendText( szData, szRecieveData, _countof(szRecieveData) );
		//▼1.0.0.1044
		//BOOL bRcvNumerical = IsNumerical( m_szRecieveCmpData );
		//文字列送信なので、リターンも文字列固定
		BOOL bRcvNumerical = FALSE;
		//▲1.0.0.1044
		PBYTE pbyteEndData = NULL;
		if( bRcvNumerical==FALSE )
		{
//			pbyteEndData = (PBYTE)m_szRecieveCmpData.GetAscii();
		}
		bReval = SendText( szData, szRecieveData, _countof(szRecieveData), pbyteEndData );
		//▲1.0.0.1021
//if( _tcscmp(_T("ctrl=1"), szData)==0 )
//{
//	Sleep(1);
//}
		//▼1.0.0.1047
		m_bControl = bReval;
		//▲1.0.0.1047

		if( bReval )
		{

			//▼1.0.0.1015
			//▼1.0.0.1021
			//m_szRecieveCmpDataが""の場合bRcvNumerical=TRUEになるのでレシーブデータで判断
//if( m_szRecieveCmpData.GetLength()>0 )
//	bRcvNumerical = IsNumerical( CString(szRecieveData) );

			//BOOL bRcvNumerical = IsNumerical( m_szRecieveCmpData );
			//▲1.0.0.1021
			if( bRcvNumerical )
			{
				if( m_szRecieveCmpData.GetLength()>0 )
				{

					size_t rcvSize = 4;
					PWORD pwCmpRcvData=NULL;
					MakeRecieveCmpBin( m_szRecieveCmpData, &pwCmpRcvData, rcvSize );
//TRACE(TEXT("@@@@@@@@@ szRecieveData [%02x:%02x]\n"),szRecieveData[0],szRecieveData[1],pbyteSend[2],pbyteSend[3],pbyteSend[4],pbyteSend[5] );

					for( int i=0; i<rcvSize; i++ )
					{
						if( szRecieveData[i]!=pwCmpRcvData[i] )
						{
							bReval=FALSE;
							break;
						}
					}

					//▼1.0.0.1047
					m_bCompareCheck = bReval;
					//▲1.0.0.1047

					//if( memcmp( szRecieveData, pwCmpRcvData, rcvSize ) )
					//{
					//	bReval=FALSE;
					//}
					if( pwCmpRcvData ) delete [] pwCmpRcvData;
				}
				m_szRecieveResultData = _T("");
				for( int i=0; i<_countof(szRecieveData); i++ )
				{
					if( szRecieveData[i]==0 ) break;
					TCHAR szData[16];
					_stprintf_s( szData, _countof(szData), _T("%02x"), szRecieveData[i] );
					m_szRecieveResultData += szData;
				}
			}
			else
			{
				m_szRecieveResultData = (char *)szRecieveData;
				if( m_szRecieveCmpData.GetLength()>0 )
				{

//TRACE(TEXT("@@@@@@@@@ m_szRecieveCmpData[%s]\n"),m_szRecieveCmpData.GetUnicode()  );
//TRACE(TEXT("@@@@@@@@@ m_szRecieveResultData[%s]\n"),m_szRecieveResultData.GetUnicode()  );

					//▼1.0.0.1022
					//if( m_szRecieveCmpData!=m_szRecieveResultData )
					//{
					//	bReval=FALSE;
					//}
					bReval = (BOOL)strmatch(m_szRecieveCmpData.GetAscii(),m_szRecieveResultData.GetAscii() );
					int value = -1;
					if( bReval )
					{
						wildvalue(m_szRecieveCmpData.GetAscii(),m_szRecieveResultData.GetAscii(),&value);
					}
					m_nRecieveResultValue = value;
					//▲1.0.0.1022

					//▼1.0.0.1047
					m_bCompareCheck = bReval;
					//▲1.0.0.1047

				}
			}

			//m_szRecieveResultData = szRecieveData;
			//if( m_szRecieveCmpData.GetLength()>0 )
			//{
			//	if( m_szRecieveCmpData!=szRecieveData )
			//	{
			//		bReval=FALSE;
			//	}
			//}
			//▲1.0.0.1015
		}
	}

	//▼1.0.0.1022
	else
	if( nNumericalMode==2 )	//Binary Ascii混合モード
	{
		//ASCIIに変換

		BYTE szRecieveData[256];
		memset(szRecieveData, 0, sizeof(szRecieveData));

		char szSendData[256];
		memset(szSendData,0,sizeof(szSendData));
	//	changeTextToBin(m_szRecieveCmpData,(PBYTE)szSendData,_countof(szSendData));
		//▼1.0.0.1031
		//changeTextToBin(szData,(PBYTE)szSendData,_countof(szSendData));
		changeTextToBin(szData,(PBYTE)szSendData,_countof(szSendData),NULL);
		//▲1.0.0.1031
//TRACE(TEXT("@@@@@@@@@ szSendData=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n") ,szSendData[0],szSendData[1],szSendData[2],szSendData[3],szSendData[4],szSendData[5] );
		PBYTE pbyteEndData = NULL;
		bReval = SendText( szSendData, szRecieveData, _countof(szRecieveData), pbyteEndData );
//TRACE(TEXT("@@@@@@@@@ szRecieveData=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n") ,szRecieveData[0],szRecieveData[1],szRecieveData[2],szRecieveData[3],szRecieveData[4],szRecieveData[5] );

		//▼1.0.0.1047
		m_bControl = bReval;
		//▲1.0.0.1047

		if( bReval )
		{
			char szCmpData[256];
			memset(szCmpData,0,sizeof(szCmpData));
			if( m_szRecieveCmpData.GetLength()>0 )
			{
				//▼1.0.0.1031
				//changeTextToBin(m_szRecieveCmpData,(PBYTE)szCmpData,_countof(szCmpData));
				INT nAsteriskPos=-1;
				changeTextToBin(m_szRecieveCmpData,(PBYTE)szCmpData,_countof(szCmpData), &nAsteriskPos);
				//▲1.0.0.1031
//TRACE(TEXT("@@@@@@@@@ m_szRecieveCmpData=%s\n") ,m_szRecieveCmpData.GetUnicode() );
//TRACE(TEXT("@@@@@@@@@ szCmpData=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n") ,szCmpData[0],szCmpData[1],szCmpData[2],szCmpData[3],szCmpData[4],szCmpData[5] );
				bReval = (BOOL)strmatch( szCmpData, (char *)szRecieveData );

				//▼1.0.0.1022
				int value = -1;
				if( bReval )
				{
					wildvalue(szCmpData,(char *)szRecieveData,&value);
				}
				m_nRecieveResultValue = value;
				//▲1.0.0.1022
				//▼1.0.0.1047
				m_bCompareCheck = bReval;
				//▲1.0.0.1047
			}
			CString szDst;
			GetRecieveText(szDst,(LPCSTR)szRecieveData,m_szRecieveCmpData.GetAscii());

			m_szRecieveResultData = szDst;
		}
	}
	//▲1.0.0.1022
	return bReval;

}



//▼1.0.0.1021
INT nCheckData( const unsigned char *pSrcData, INT nSrcLen, const unsigned char *pCmpData, int nCmpLen)
{
	INT nReval = -1;
	INT nLoop = nSrcLen - nCmpLen + 1;
	for( INT i=nLoop-1; i>=0 && nReval<0; i-- )
	{
		BOOL bReval = (memcmp( &pSrcData[i], pCmpData, nCmpLen )==0);
		if( bReval ) nReval = i;
	}
	return nReval;
}
//▲1.0.0.1021

//▼1.0.0.1015
//Open-Closeなし
//-----------------------------------------------------
//バイナリ送信
//-----------------------------------------------------
//▼1.0.0.1021
//BOOL StSerialComm::SendBin2( const PBYTE pbyteSend, size_t sendSize, PBYTE pbyteRcv, size_t *rcvSize )
BOOL StSerialComm::SendBin2( const PBYTE pbyteSend, size_t sendSize, PBYTE pbyteRcv, size_t *rcvSize, const PBYTE pbyteRevEndData )
//▲1.0.0.1021
{
	BOOL bReval = FALSE;
	DWORD	dwSendedLen = 0;
//TRACE(TEXT("@@@@@@@@@ Send [%02x:%02x:%02x:%02x:%02x:%02x]\n"),pbyteSend[0],pbyteSend[1],pbyteSend[2],pbyteSend[3],pbyteSend[4],pbyteSend[5] );
	DWORD dwErrorCode = Send( sendSize, pbyteSend, &dwSendedLen );
	if(dwErrorCode==NO_ERROR)
	{
		//▼1.0.0.1038
		//if( m_nDelayTime>0 )
		//{
		//	Sleep(m_nDelayTime);
		//}
		//▲1.0.0.1038
		size_t rcvBufferSize = *rcvSize;
		DWORD	dwRcvNum = 0;
		//▼1.0.0.1043
		if( m_nRecieveDelayTime>0 )
		{
			Sleep(m_nRecieveDelayTime);
		}
		//▲1.0.0.1043

		dwErrorCode= Rcv( rcvBufferSize, pbyteRcv, &dwRcvNum);
		//▼1.0.0.1038
		if( dwErrorCode!=NO_ERROR )
		{
			if( m_nDelayTime>0 )
			{
				INT nDelayTime = 100;
				INT nCount = m_nDelayTime/nDelayTime;
				for( int i=0; i<nCount; i++ )
				{
					Sleep(nDelayTime);
					dwErrorCode= Rcv( rcvBufferSize, pbyteRcv, &dwRcvNum);
					if( dwErrorCode==NO_ERROR ) break;
				}
			}
		}
		//▲1.0.0.1038
		//▼1.0.0.1021
		if( pbyteRevEndData!=NULL )
		{
			INT nRevLen = dwRcvNum;
			INT nCmpLen = strlen((const char *)pbyteRevEndData);
			INT nPos = nCheckData( pbyteRcv, nRevLen, pbyteRevEndData, nCmpLen );
			for( int iPos=dwRcvNum+1; iPos<rcvBufferSize && dwErrorCode==NO_ERROR && nPos<0; iPos++ )
			{
				dwErrorCode = Rcv( 1, &pbyteRcv[iPos+1], &dwRcvNum );
				if( dwErrorCode==NO_ERROR )
				{
					nPos = nCheckData( pbyteRcv, ++nRevLen, pbyteRevEndData, nCmpLen );
				}
			}
			dwRcvNum = nRevLen;

		}
		//▲1.0.0.1021

		if(dwErrorCode==NO_ERROR)
		{
			bReval = TRUE;
			*rcvSize = dwRcvNum;
		}
		else
		{
			bReval = FALSE;
		}

	}

	else
	{
		bReval = FALSE;
	}
	return bReval;
}
//▲1.0.0.1015

//通信ごとにOpen-Closeを繰り返す
//-----------------------------------------------------
//バイナリ送信
//-----------------------------------------------------
//▼1.0.0.1021
//BOOL StSerialComm::SendBin( const PBYTE pbyteSend, size_t sendSize, PBYTE pbyteRcv, size_t *rcvSize )
BOOL StSerialComm::SendBin( const PBYTE pbyteSend, size_t sendSize, PBYTE pbyteRcv, size_t *rcvSize, const PBYTE pbyteRevEndData )
//▲1.0.0.1021
{
	BOOL bReval = FALSE;

	Close();
	DWORD dwErrorCode = Open();
	if(dwErrorCode == NO_ERROR)
	{
		//▼1.0.0.1015
		//DWORD	dwSendedLen = 0;
		//dwErrorCode = Send( sendSize, pbyteSend, &dwSendedLen);
		//if(dwErrorCode==NO_ERROR)
		//{
		//	size_t rcvBufferSize = *rcvSize;
		//	DWORD	dwRcvNum = 0;
		//	dwErrorCode= Rcv( rcvBufferSize, pbyteRcv, &dwRcvNum);
		//	if(dwErrorCode==NO_ERROR)
		//	{
		//		bReval = TRUE;
		//		*rcvSize = dwRcvNum;
		//	}
		//}
		//▼1.0.0.1021
		//bReval = SendBin2(pbyteSend,sendSize,pbyteRcv,rcvSize);
		bReval = SendBin2(pbyteSend,sendSize,pbyteRcv,rcvSize,pbyteRevEndData);
		//▲1.0.0.1021
		//▲1.0.0.1015
		Close();
	}
	return bReval;
}


INT GetData(LPCTSTR szData, PBYTE szRev, size_t bufSize)
{
	CString szString(szData);
	INT nLength = szString.GetLength()/2;

	//▼1.0.0.15
	if( !IsNumerical( szData ) )
	{
		nLength = _tcslen(szData);
	}
	//▲1.0.0.15

	if( szRev )
	{
		for( int i=0; i<nLength && i<bufSize; i++ )
		{
			szRev[i] = (BYTE)_tcstol( szString.Mid(i*2, 2), nullptr, 16 )&0xff;
		}
	}
	return nLength;
}

//-----------------------------------------------------
//文字列送信
//-----------------------------------------------------
BOOL StSerialComm::SendText( LPCTSTR szText, LPTSTR szRcvText, size_t rcvBufferSize )
{
	BOOL bReval = FALSE;

	//Close();
	//DWORD dwErrorCode = Open();
	//if(dwErrorCode == NO_ERROR)
	{
		PBYTE  pbyteSend = NULL;
		PBYTE  pbyteRcv = NULL;

		do
		{
			if( rcvBufferSize==0 ) rcvBufferSize=128;	//仮に入れておく szRcvTextの戻りが必要ないとき

			CString szString(szText);

			INT nSTXSize = GetData(m_szSendSTX, NULL, 0 );
			INT nETXSize = GetData(m_szSendETX, NULL, 0 );

			size_t sendSize = szString.GetLength();
			pbyteSend = new BYTE [sendSize + nSTXSize + nETXSize + 1];	//+1は必要。。nETXSize=0の時strcpy_sでエラー出さないため
			if( !pbyteSend ) break;
			memset(pbyteSend,0,sendSize + nSTXSize + nETXSize + 1);
			StString aString(szString);

			GetData(m_szSendSTX, pbyteSend, nSTXSize );
			strcpy_s( (char *)pbyteSend + nSTXSize, sendSize + 1, aString.GetAscii() );
			GetData(m_szSendETX, pbyteSend + nSTXSize + sendSize, nETXSize );

			//改行コードを入れる。CR (0x0D)  m_szSendETXで対応の為削除
			//pbyteSend[sendSize+1] = pbyteSend[sendSize];	//NULL移動
			
			//pbyteSend[sendSize] = 0x0D;	//CR


			INT nRevSTXSize = GetData(m_szRecieveSTX, NULL, 0 );
			INT nRevETXSize = GetData(m_szRecieveETX, NULL, 0 );

			pbyteRcv = new BYTE [rcvBufferSize+1];
			if( !pbyteRcv ) break;


			memset( pbyteRcv, 0, rcvBufferSize+1 );
			size_t rcvSize = rcvBufferSize;

//TRACE(TEXT("@@@@@@ Snd:") );
//for( int i=0; i<sendSize + nSTXSize + nETXSize; i++ )
//{
//TRACE(TEXT(" %02X"), pbyteSend[i] );
//}
//TRACE(TEXT(" \n") );


			//bReval = SendBin( pbyteSend, sendSize+1, pbyteRcv, &rcvSize );
			bReval = SendBin( pbyteSend, sendSize + nSTXSize + nETXSize, pbyteRcv, &rcvSize );
			//bReval = SendBin( pbyteSend, sendSize + nSTXSize + nETXSize + 1, pbyteRcv, &rcvSize );
			if( bReval )
			{
//TRACE(TEXT("@@@@@@ Rcv:") );
//for( int i=0; i<rcvSize; i++ )
//{
//TRACE(TEXT(" %02X"), pbyteRcv[i] );
//}
//TRACE(TEXT(" \n") );
				if( szRcvText )
				{
					//m_szRecieveSTX,m_szRecieveETXの比較は無視
					INT nLength = strlen((const char *)pbyteRcv);
					pbyteRcv[nLength - nRevETXSize] = 0;
					aString = (char *)&pbyteRcv[nRevSTXSize];
					_tcscpy_s( szRcvText, rcvBufferSize, aString.GetUnicode() );



				}
			}

		}while(0);
		if(pbyteSend) delete [] pbyteSend;
		if(pbyteRcv) delete [] pbyteRcv;
		//Close();

	}
	return bReval;
}

//▼1.0.0.1015
//▼1.0.0.1021
//BOOL StSerialComm::SendText( LPCTSTR szText, PBYTE szRcvBin, size_t rcvBufferSize )
BOOL StSerialComm::SendText( LPCTSTR szText, PBYTE szRcvBin, size_t rcvBufferSize, const PBYTE pbyteRevEndData )
//▲1.0.0.1021
{
	BOOL bReval = FALSE;

	//Close();
	//DWORD dwErrorCode = Open();
	//if(dwErrorCode == NO_ERROR)
	{
		PBYTE  pbyteSend = NULL;
		PBYTE  pbyteRcv = NULL;

		PBYTE pRecieveSTX = NULL;
		PBYTE pRecieveETX = NULL;

		PBYTE pRcvReplaceSpace = NULL;


		do
		{
			if( rcvBufferSize==0 ) rcvBufferSize=128;	//仮に入れておく szRcvTextの戻りが必要ないとき

			CString szString(szText);

			INT nSTXSize = GetData(m_szSendSTX, NULL, 0 );
			INT nETXSize = GetData(m_szSendETX, NULL, 0 );

			size_t sendSize = szString.GetLength();
			pbyteSend = new BYTE [sendSize + nSTXSize + nETXSize + 1];	//+1は必要。。nETXSize=0の時strcpy_sでエラー出さないため
			if( !pbyteSend ) break;
			memset(pbyteSend,0,sendSize + nSTXSize + nETXSize + 1);
			StString aString(szString);

			GetData(m_szSendSTX, pbyteSend, nSTXSize );
			strcpy_s( (char *)pbyteSend + nSTXSize, sendSize + 1, aString.GetAscii() );
			GetData(m_szSendETX, pbyteSend + nSTXSize + sendSize, nETXSize );

			//改行コードを入れる。CR (0x0D)  m_szSendETXで対応の為削除
			//pbyteSend[sendSize+1] = pbyteSend[sendSize];	//NULL移動
			
			//pbyteSend[sendSize] = 0x0D;	//CR


			INT nRevSTXSize = GetData(m_szRecieveSTX, NULL, 0 );
			INT nRevETXSize = GetData(m_szRecieveETX, NULL, 0 );

			if( nRevSTXSize>0 )
			{
				pRecieveSTX = new BYTE [nRevSTXSize];
				if( !pRecieveSTX ) break;
				GetData(m_szRecieveSTX, pRecieveSTX, nRevSTXSize );
			}

			if( nRevETXSize>0 )
			{
				pRecieveETX = new BYTE [nRevETXSize];
				if( !pRecieveETX ) break;
				GetData(m_szRecieveETX, pRecieveETX, nRevETXSize );
			}

			pbyteRcv = new BYTE [rcvBufferSize+1];
			if( !pbyteRcv ) break;


			memset( pbyteRcv, 0, rcvBufferSize+1 );
			size_t rcvSize = rcvBufferSize;

//TRACE(TEXT("@@@@@@ Snd:") );
//for( int i=0; i<sendSize + nSTXSize + nETXSize; i++ )
//{
//TRACE(TEXT(" %02X"), pbyteSend[i] );
//}
//TRACE(TEXT(" \n") );


			//bReval = SendBin( pbyteSend, sendSize+1, pbyteRcv, &rcvSize );
			//▼1.0.0.1021
			//bReval = SendBin( pbyteSend, sendSize + nSTXSize + nETXSize, pbyteRcv, &rcvSize );
			bReval = SendBin( pbyteSend, sendSize + nSTXSize + nETXSize, pbyteRcv, &rcvSize, pbyteRevEndData );
			//▲1.0.0.1021
			//bReval = SendBin( pbyteSend, sendSize + nSTXSize + nETXSize + 1, pbyteRcv, &rcvSize );
			if( bReval )
			{
//TRACE(TEXT("@@@@@@ Rcv:") );
//for( int i=0; i<rcvSize; i++ )
//{
//TRACE(TEXT(" %02X"), pbyteRcv[i] );
//}
//TRACE(TEXT(" \n") );
				if( szRcvBin )
				{
					//TextではなくBinの場合がある。Takano---------

					//m_szRecieveSTX,m_szRecieveETXの比較は無視
					//INT nLength = strlen((const char *)pbyteRcv);

					PBYTE pRcvStart = pbyteRcv;
					if( nRevSTXSize>0 )
					{
						if( memcmp( pbyteRcv, pRecieveSTX, nRevSTXSize )==0 )
						{
							pRcvStart += nRevSTXSize;
							rcvSize -= nRevSTXSize;
						}
					}

					if( nRevETXSize>0 )
					{
						if( memcmp( pRcvStart + rcvSize - nRevETXSize, pRecieveETX, nRevETXSize )==0 )
						{
							rcvSize -= nRevETXSize;
						}
					}

					//置き換え  SetRcvReplaceSpace -> ' '

					INT nRcvReplaceSpaceSize = GetData(m_szRcvReplaceSpace, NULL, 0 );
					if( nRcvReplaceSpaceSize>0 )
					{
						pRcvReplaceSpace = new BYTE [nRcvReplaceSpaceSize];
						if( !pRcvReplaceSpace ) break;
						GetData(m_szRcvReplaceSpace, pRcvReplaceSpace, nRcvReplaceSpaceSize );
					}

                    INT loop = rcvSize>rcvBufferSize?rcvBufferSize:rcvSize;
					for( int i=0; i<loop; i++ )
					{
						if( nRcvReplaceSpaceSize>0 && i<loop-nRcvReplaceSpaceSize+1 )
						{
							if( memcmp( &pRcvStart[i], pRcvReplaceSpace, nRcvReplaceSpaceSize ) == 0 )
							{
								*szRcvBin++ = ' ';	//Spaceに置き換え
								i += nRcvReplaceSpaceSize-1;
								continue;
							}
						}
						*szRcvBin++ = pRcvStart[i];
					}
				}
			}
		}while(0);
		if(pbyteSend) delete [] pbyteSend;
		if(pbyteRcv) delete [] pbyteRcv;
		//Close();

		if(pRecieveSTX) delete [] pRecieveSTX;
		if(pRecieveETX) delete [] pRecieveETX;

		if(pRcvReplaceSpace) delete [] pRcvReplaceSpace;

	}
	return bReval;
}


//▲1.0.0.1015


//▼1.0.0.1022
BOOL StSerialComm::SendText( const char * szText, PBYTE szRcvBin, size_t rcvBufferSize, const PBYTE pbyteRevEndData )
{
	BOOL bReval = FALSE;

	{
		PBYTE  pbyteSend = NULL;
		PBYTE  pbyteRcv = NULL;

		PBYTE pRecieveSTX = NULL;
		PBYTE pRecieveETX = NULL;

		PBYTE pRcvReplaceSpace = NULL;
		do
		{
			if( rcvBufferSize==0 ) rcvBufferSize=128;	//仮に入れておく szRcvTextの戻りが必要ないとき

			INT nSTXSize = GetData(m_szSendSTX, NULL, 0 );
			INT nETXSize = GetData(m_szSendETX, NULL, 0 );

			size_t sendSize = strlen(szText);

			pbyteSend = new BYTE [sendSize + nSTXSize + nETXSize + 1];	//+1は必要。。nETXSize=0の時strcpy_sでエラー出さないため
			if( !pbyteSend ) break;
			memset(pbyteSend,0,sendSize + nSTXSize + nETXSize + 1);
			StString aString(szText);

			GetData(m_szSendSTX, pbyteSend, nSTXSize );
			strcpy_s( (char *)pbyteSend + nSTXSize, sendSize + 1, aString.GetAscii() );
			GetData(m_szSendETX, pbyteSend + nSTXSize + sendSize, nETXSize );

			INT nRevSTXSize = GetData(m_szRecieveSTX, NULL, 0 );
			INT nRevETXSize = GetData(m_szRecieveETX, NULL, 0 );

			if( nRevSTXSize>0 )
			{
				pRecieveSTX = new BYTE [nRevSTXSize];
				if( !pRecieveSTX ) break;
				GetData(m_szRecieveSTX, pRecieveSTX, nRevSTXSize );
			}

			if( nRevETXSize>0 )
			{
				pRecieveETX = new BYTE [nRevETXSize];
				if( !pRecieveETX ) break;
				GetData(m_szRecieveETX, pRecieveETX, nRevETXSize );
			}

			pbyteRcv = new BYTE [rcvBufferSize+1];
			if( !pbyteRcv ) break;


			memset( pbyteRcv, 0, rcvBufferSize+1 );
			size_t rcvSize = rcvBufferSize;

//TRACE(TEXT("@@@@@@@@@ SendBin=["));
//for( int i=0; i<sendSize; i++ )
//{
//TRACE(TEXT("%02x:"),pbyteSend[i]);
//}
//TRACE(TEXT("]\n"));

			bReval = SendBin( pbyteSend, sendSize + nSTXSize + nETXSize, pbyteRcv, &rcvSize, pbyteRevEndData );
			if( bReval )
			{
				if( szRcvBin )
				{
					//TextではなくBinの場合がある。Takano---------

					//m_szRecieveSTX,m_szRecieveETXの比較は無視
					//INT nLength = strlen((const char *)pbyteRcv);

					PBYTE pRcvStart = pbyteRcv;
					if( nRevSTXSize>0 )
					{
						if( memcmp( pbyteRcv, pRecieveSTX, nRevSTXSize )==0 )
						{
							pRcvStart += nRevSTXSize;
							rcvSize -= nRevSTXSize;
						}
					}

					if( nRevETXSize>0 )
					{
						if( memcmp( pRcvStart + rcvSize - nRevETXSize, pRecieveETX, nRevETXSize )==0 )
						{
							rcvSize -= nRevETXSize;
						}
					}

					//置き換え  SetRcvReplaceSpace -> ' '

					INT nRcvReplaceSpaceSize = GetData(m_szRcvReplaceSpace, NULL, 0 );
					if( nRcvReplaceSpaceSize>0 )
					{
						pRcvReplaceSpace = new BYTE [nRcvReplaceSpaceSize];
						if( !pRcvReplaceSpace ) break;
						GetData(m_szRcvReplaceSpace, pRcvReplaceSpace, nRcvReplaceSpaceSize );
					}

                    INT loop = rcvSize>rcvBufferSize?rcvBufferSize:rcvSize;
					for( int i=0; i<loop; i++ )
					{
						if( nRcvReplaceSpaceSize>0 && i<loop-nRcvReplaceSpaceSize+1 )
						{
							if( memcmp( &pRcvStart[i], pRcvReplaceSpace, nRcvReplaceSpaceSize ) == 0 )
							{
								*szRcvBin++ = ' ';	//Spaceに置き換え
								i += nRcvReplaceSpaceSize-1;
								continue;
							}
						}
						*szRcvBin++ = pRcvStart[i];
					}
				}
			}
		}while(0);
		if(pbyteSend) delete [] pbyteSend;
		if(pbyteRcv) delete [] pbyteRcv;

		if(pRecieveSTX) delete [] pRecieveSTX;
		if(pRecieveETX) delete [] pRecieveETX;

		if(pRcvReplaceSpace) delete [] pRcvReplaceSpace;

	}
	return bReval;
}
//▲1.0.0.1022


BOOL StSerialComm::SetRecieveData( LPCTSTR szData )
{
	m_szRecieveCmpData = szData;
	return TRUE;
}

BOOL StSerialComm::GetRecieveResultData( StString &szData )
{
	szData = m_szRecieveResultData;
	return TRUE;
}

//▼1.0.0.1022
BOOL StSerialComm::GetRecieveResultValue( INT &szData )
{
	szData = m_nRecieveResultValue;
	return TRUE;
}
//▲1.0.0.1022

//▼1.0.0.1015
//void StSerialComm::SetBaudRate(INT nBaudRate)
//{
//	//m_nBaudRate = nBaudRate;
//}
//▲1.0.0.1015

BOOL StSerialComm::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction == _T("Send") )
	{
		//▼1.0.0.1061
		bReval = TRUE;
		if( !m_nScriptCheckMode )
		//▲1.0.0.1061
		bReval = SendData(szArgument);
	}
	else
	if( szFunction == _T("SetRecieveData") )
	{
		bReval = SetRecieveData( szArgument );
	}
	//▼1.0.0.1015
	else
	if( szFunction=="SetBaudRate" )
	{
		m_nBaudRate = _tcstol(szArgument,NULL,0);
		if( m_nBaudRate>0 )
		//▼1.0.0.1061
		if( !m_nScriptCheckMode )
		//▲1.0.0.1061
			SetBaudRate(m_nBaudRate);
		bReval = TRUE;
	}
	//else
	//if( szFunction=="SetBaudRateCommand" )
	//{
	//	m_szBaudRateCommand = szArgument;
	//	bReval = TRUE;
	//}
	//else
	//if( szFunction=="SetBaudRateReturn" )
	//{
	//	m_szBaudRateReturn = szArgument;
	//	bReval = TRUE;
	//}

	else
	if( szFunction=="AutoBaudRate" )
	{
		//▼1.0.0.1061
		bReval = TRUE;
		if( !m_nScriptCheckMode )
		//▲1.0.0.1061
		bReval = AutoBaudRate(szArgument);
	}
	//▲1.0.0.1015
	//タカノ向け対応
	else
	if( szFunction=="SetSendSTX" )
	{
		SetSendSTX(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetSendETX" )
	{
		SetSendETX(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetRecieveSTX" )
	{
		SetRecieveSTX(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetRecieveETX" )
	{
		SetRecieveETX(szArgument);
		bReval = TRUE;
	}

	//▼1.0.0.1015
	else
	if( szFunction=="SetRcvReplaceSpace" )
	{
		SetRcvReplaceSpace(szArgument);
		bReval = TRUE;
	}

	else
	if( szFunction=="SetDelayTime" )
	{
		m_nDelayTime = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	//▲1.0.0.1015


	//▼1.0.0.1043
	else
	if( szFunction=="SetRecieveDelayTime" )
	{
		m_nRecieveDelayTime = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	//▲1.0.0.1043

	//▼1.0.0.1048
	else
	if( szFunction=="SetParity" || szFunction=="Parity" )
	{
		m_szParity = szArgument;
		SetParity(m_szParity);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetStopBits" || szFunction=="StopBits" )
	{
		m_dblStopBits = _ttof(szArgument);
		SetStopBits(m_dblStopBits);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetByteSize" || szFunction=="ByteSize" )
	{
		m_nByteSize = _tcstol(szArgument,NULL,0);
		SetByteSize(m_nByteSize);
		bReval = TRUE;
	}
	//▲1.0.0.1048

	return bReval;
}

BOOL StSerialComm::TestData( void )
{
	BOOL bReval = FALSE;
	DWORD dwErrorCode = Open();
	if(dwErrorCode == NO_ERROR)
	{
		BYTE byteData = 0;
		BYTE byteRcvData[4];
		size_t rcvSize = sizeof(byteRcvData);
		BYTE byteSndData[6] = { 2, 0, 7, 1, 0, 3 };
		//---Read
		memset(byteRcvData,0,sizeof(byteRcvData));
		DWORD	dwSendedLen = 0;
		bReval = SendBin2(byteSndData,sizeof(byteSndData),byteRcvData,&rcvSize);
		if( bReval )
		{
			if( byteRcvData[0] != 2 || byteRcvData[3] != 3 || byteRcvData[1] != 1 )
				bReval = FALSE;
		}

		Close();
	}
	return bReval;
}

//▼1.0.0.1015
//Binary用
#define FORMAT_STX 0x02
#define FORMAT_ETX 0x03
BOOL StSerialComm::WriteBitData( BYTE byteAddr, BYTE byteDevPage, INT istbit, INT ilenbit, WORD dwData )
{
	//return TestData();

	BOOL bReval = FALSE;
	//size_t rcvBufferSize = *rcvSize;

	DWORD dwErrorCode = Open();
	if(dwErrorCode == NO_ERROR)
	{
		bReval = TRUE;
		BYTE byteData = 0;
		BYTE byteRcvData[4];
		size_t rcvSize = sizeof(byteRcvData);
		BYTE byteSndData[6] = { FORMAT_STX, 0, 0, 1, 0, FORMAT_ETX };
		byteSndData[1] = byteDevPage;
		byteSndData[2] = byteAddr;
		if( ilenbit<8 )
		{
			//---Read
			memset(byteRcvData,0,sizeof(byteRcvData));
			DWORD	dwSendedLen = 0;
			bReval = SendBin2(byteSndData,sizeof(byteSndData),byteRcvData,&rcvSize);
			if( bReval )
			{
				if( byteRcvData[0] != FORMAT_STX || byteRcvData[3] != FORMAT_ETX || byteRcvData[1] != 1 )
					bReval = FALSE;
			}

			if( bReval )
			{
	 			byteData = byteRcvData[2];
				BYTE byteMask = (1<<ilenbit)-1;
				byteMask = ~(byteMask<<istbit);
				byteData = byteData&byteMask;
			}

		}

		if( bReval )
		{
			byteData |= (BYTE)(dwData<<istbit); 

			//---Write
			byteSndData[1] |= 2;
			byteSndData[4] = byteData;
			rcvSize = sizeof(byteRcvData);
			memset(byteRcvData,0,sizeof(byteRcvData));

//TRACE(TEXT("@@@@@@@@@ SendBin2 [%02x:%02x:%02x:%02x:%02x:%02x:]\n"),byteSndData[0],byteSndData[1],byteSndData[2],byteSndData[3],byteSndData[4],byteSndData[5] );
			bReval = SendBin2(byteSndData,sizeof(byteSndData),byteRcvData,&rcvSize);
			if( bReval )
			{
				if( byteRcvData[0] != FORMAT_STX || byteRcvData[3] != FORMAT_ETX || byteRcvData[1] != 0 || byteRcvData[2] != 1 )
					bReval = FALSE;
			}
		}

		Close();
	}

	return bReval;
}

//-----------------------------------------------------
//BaudRate判定
//-----------------------------------------------------
BOOL StSerialComm::AutoBaudRate( LPCTSTR szData )
{

	BOOL bReval = FALSE;
	INT nBaudRate[] = {9600,38400,58600,115200};

	//16進数値か文字列か判別
	BOOL bNumerical=FALSE;

	bNumerical = IsNumerical( szData );

	if( bNumerical )
	{
		CString szString(szData);
		if( szString.Left(2)==_T("0x") || szString.Left(2)==_T("0X") )
		{
			szString = szString.Mid(2);
		}

		szString.Replace(_T(","),_T(""));
		szString.Replace(_T(" "),_T(""));	//必要？
		//数値変換
		size_t sendSize = szString.GetLength()/2;
		PBYTE  pbyteSend = new BYTE [sendSize];
		if( pbyteSend )
		{
			for( int i=0; i<sendSize; i++ )
			{
				pbyteSend[i] = (BYTE)_tcstol(szString.Mid(i*2,2),NULL,0)&0xFF;
			}
			size_t rcvSize = 4;
			PWORD pwCmpRcvData=NULL;

			//▼1.0.0.1015
			MakeRecieveCmpBin( m_szRecieveCmpData, &pwCmpRcvData, rcvSize );
			//if( m_szRecieveCmpData.GetLength()>0 )
			//{

			//	CString szRevString(m_szRecieveCmpData.GetUnicode());
			//	if( szRevString.Left(2)==_T("0x") || szRevString.Left(2)==_T("0X") )
			//	{
			//		szRevString = szRevString.Mid(2);
			//	}
			//	szRevString.Replace(_T(","),_T(""));
			//	szRevString.Replace(_T(" "),_T(""));
			//	rcvSize = szRevString.GetLength()/2;
			//	pwCmpRcvData = new WORD [rcvSize];
			//	if( pwCmpRcvData )
			//	{
			//		for( int i=0; i<rcvSize; i++ )
			//		{
			//			if( szRevString.Mid(i*2, 2).Compare(_T("XX")) == 0 )
			//			{
			//				pwCmpRcvData[i] = 0xffff;
			//			}
			//			else
			//			{
			//				pwCmpRcvData[i] = (WORD)_tcstol( szRevString.Mid(i*2, 2), nullptr, 16 )&0xff;
			//			}
			//		}
			//	}
			//}
			//▲1.0.0.1015

			PBYTE pbyteRcv = new BYTE [rcvSize];
			//BYTE byteRcv[4];
			//size_t rcvSize = sizeof(byteRcv);
			//bReval = SendBin( pbyteSend, sendSize, byteRcv, &rcvSize );
			if( pbyteRcv )
			{
				size_t getSize = rcvSize;
				for( int i=0; i<_countof(nBaudRate); i++ )
				{
					SetBaudRate(nBaudRate[i]);

					bReval = SendBin( pbyteSend, sendSize, pbyteRcv, &getSize );
					if( bReval )
					{
						//CString tmp;
						//CString szRecieve(_T(""));
						//for( int i=0; i<getSize; i++ )
						//{
						//	if( i>0 )	szRecieve+=_T(",");
						//	tmp.Format(_T("%02x"), pbyteRcv[i] );
						//	szRecieve += tmp;
						//}
						//m_szRecieveResultData = szRecieve;

						if( pwCmpRcvData )
						{
							if( getSize != rcvSize )
								bReval = FALSE;
							if( bReval )
							{
								//比較
								for( int j=0; j<(int)rcvSize; j++ )
								{
									if( (pwCmpRcvData[j]>>8)>0 )	//XXの位置は比較しない
										continue;
									if( pwCmpRcvData[j]!=(WORD)pbyteRcv[j] )
									{
										bReval = FALSE;
										break;
									}
								}
							}
						}
						m_nBaudRate = nBaudRate[i];
						break;
					}
				}
				delete [] pbyteRcv;
			}
			if( pwCmpRcvData )	delete [] pwCmpRcvData;
			delete [] pbyteSend;
		}
	}
	else
	{
		for( int i=0; i<_countof(nBaudRate); i++ )
		{
			//文字列
			//bReval = SendText( szData );
			TCHAR szRecieveData[256];
			SetBaudRate(nBaudRate[i]);
			bReval = SendText( szData, szRecieveData, _countof(szRecieveData) );
			if( bReval )
			{
				m_szRecieveResultData = szRecieveData;
				if( m_szRecieveCmpData.GetLength()>0 )
				{
					if( m_szRecieveCmpData!=szRecieveData )
					{
						bReval=FALSE;
						break;
					}
				}
				m_nBaudRate = nBaudRate[i];
				break;
			}
		}
	}
	return bReval;

}

//▲1.0.0.1015



