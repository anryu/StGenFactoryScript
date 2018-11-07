#include "StdAfx.h"
#include "StWriteSerial.h"
#include "StInspectMain.h"


StWriteSerial::StWriteSerial(void)
{
	Initialize();
}

StWriteSerial::StWriteSerial(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void* pParent, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
//	, m_pParent(pParent)
{
	Initialize();
}

StWriteSerial::~StWriteSerial(void)
{
}

void StWriteSerial::Initialize(void)
{
	m_nSerialMode = 0; //0:カメラシリアル 1:センサーシリアル
	m_nMode = 0;       //0:Binary 1:Ascii 2:Takano

	m_szDeviceCode = _T("");
	m_szPage = _T("");
	m_szYearAddress = _T("");
	m_szMonthAddress = _T("");
	m_szSN1Address = _T("");
	m_szSN2Address = _T("");
	m_szSN3Address = _T("");
	m_szSN4Address = _T("");
	//▼1.0.0.1057
	m_szMonthAddress2 = _T("");
	m_szSN1Address2 = _T("");
	//▲1.0.0.1057
	m_szCommand  = _T("");
	//▼1.0.0.1022
	m_szRecieve = _T("");
	//▲1.0.0.1022
	//▼1.0.0.1046
	//GenICam用アドレス
	m_dwAddress = 0;
	m_nLength = 0;
	//▲1.0.0.1046

}

BOOL StWriteSerial::SetSerialMode(LPCTSTR szSerialMode)
{
	BOOL bReval = TRUE;
	SetSerialMode(_ttoi(szSerialMode));
	return bReval;
}
BOOL StWriteSerial::SetMode(LPCTSTR szMode)
{
	BOOL bReval = TRUE;
	SetMode(_ttoi(szMode));
	return bReval;
}



//-------------------------------------------------------------------------------
//書込み実行
//-------------------------------------------------------------------------------
BOOL StWriteSerial::Execute(void)
{
	BOOL bReval = FALSE;

	do
	{
		m_iJudge = 2;	//2:処理中
		if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//GigE用
		{
			bReval = FALSE;
			break;
		}
		else
		if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL用
		{

			StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
			if( !aDevice->IsOpened() ) break;

			StSerialComm *aSerialComm = aDevice->GetSerialComm();

			//CString szPortName;
			//aDevice->GetPortName(szPortName);
			//CString szVarName = m_VarName;
			//szVarName += _T("_ControlRetry");
			//StSerialComm aSerialComm(_T("StSerialComm"),szVarName,szPortName);

			//▼1.0.0.1022
			CString szText("");
			if( m_nSerialMode&1 )
			{
				szText = m_szSerialNumber;
			}
			else
			if( (m_nSerialMode>>1)&1 )
			{
				szText = m_szSensorSerialNumber;
			}
			//▲1.0.0.1022

			if( m_nMode == 0 )	//Binary
			{
				//▼1.0.0.1022
				//if( m_nSerialMode&1 )
				//{
				//	bReval = WriteBinary( (void *)aSerialComm, m_szSerialNumber );
				//	if( !bReval )
				//	{
				//		//m_szLastError = _T("Write ");
				//		break;
				//	}
				//}
				//else
				//if( (m_nSerialMode>>1)&1 )
				//{
				//	//SensorSerialNumber
				//	bReval = WriteBinary( (void *)aSerialComm, m_szSensorSerialNumber );
				//	if( !bReval )
				//	{
				//		break;
				//	}
				//}
				if( szText.GetLength()>0 )
				{
					//▼1.0.0.1024
					//bReval = WriteBinary( (void *)aSerialComm, szText );
					//if( !bReval )
					//{
					//	break;
					//}
					BOOL bJudge = WriteBinary( (void *)aSerialComm, szText );
					if( bJudge )
						m_iJudge = 1;	//OK
					else
						m_iJudge = 0;	//0:NG
					bReval = TRUE;
					//▲1.0.0.1024
				}
				//▲1.0.0.1022
			}
			else
			if( m_nMode == 2 )	//Takano
			{
				//▼1.0.0.1022
				//aSerialComm->function(_T("SetSendSTX"),_T("02"));
				//aSerialComm->function(_T("SetSendETX"),_T("0d"));
				//aSerialComm->function(_T("SetRecieveSTX"),_T("0d0a"));
				//aSerialComm->function(_T("SetRecieveETX"),_T("USER>"));
				//if( m_nSerialMode&1 )
				//{
				//	bReval = WriteText( (void *)aSerialComm, m_szSerialNumber );
				//	if( !bReval )
				//	{
				//		break;
				//	}
				//}
				//else
				//if( (m_nSerialMode>>1)&1 )
				//{
				//	bReval = WriteText( (void *)aSerialComm, m_szSensorSerialNumber );
				//	if( !bReval )
				//	{
				//		break;
				//	}
				//}
				//break;
				if( szText.GetLength()>0 )
				{
					//▼1.0.0.1024
					//bReval = WriteText( (void *)aSerialComm, szText );
					//if( !bReval )
					//{
					//	break;
					//}
					BOOL bJudge = WriteText( (void *)aSerialComm, szText );
					if( bJudge )
						m_iJudge = 1;	//OK
					else
						m_iJudge = 0;	//0:NG
					bReval = TRUE;
					//▲1.0.0.1024
				}
				//▲1.0.0.1022
			}
		}
		//▼1.0.0.1044
		else
		if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )	//GenICam用
		{

			StDeviceGenICam *aDevice = (StDeviceGenICam *)m_DeviceBase;
			if( !aDevice->IsOpened() ) break;

			CString szText("");
			if( m_nSerialMode&1 )
			{
				szText = m_szSerialNumber;
			}
			else
			if( (m_nSerialMode>>1)&1 )
			{
				szText = m_szSensorSerialNumber;
			}

			if( szText.GetLength()>0 )
			{
				if( m_nLength>0 && (m_nLength&3)==0 && m_dwAddress!=0 )
				{
					char *pbyteData = new char [m_nLength];
					if( pbyteData )
					{
						memset(pbyteData,0,sizeof(BYTE)*m_nLength);
						StString aSerialNumber = szText;
						strcpy_s( pbyteData, m_nLength, aSerialNumber.GetAscii());
						//▼1.0.0.1058
						//bReval = aDevice->WriteMain(m_dwAddress,pbyteData,m_nLength,TRUE);
						bReval = aDevice->WriteMain(m_dwAddress,pbyteData,m_nLength,FALSE);
						//▲1.0.0.1058
						delete pbyteData;
						if( bReval )
							m_iJudge = 1;	//OK
						else
							m_iJudge = 0;	//0:NG
					}
				}
			}
		}
		//▲1.0.0.1044


	}while(0);
	//▼1.0.0.1024
	//if( bReval )
	//	m_iJudge = 1;	//OK
	//else
	//	m_iJudge = 0;	//0:NG
	if( m_iJudge==2 )
	{
		m_iJudge=0;
	}
	//▲1.0.0.1024
	return bReval;
}

//▼1.0.0.1057
#define _ALPHABET {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'}
//▲1.0.0.1057

//-------------------------------------------------------------------------------
//Binary用書込み
//-------------------------------------------------------------------------------
BOOL StWriteSerial::WriteBinary(void *pComm, LPCTSTR szData)
{
	CString szSerialNumber = szData;
	//▼1.0.0.1057
	CHAR pchrAlphabet[24] = _ALPHABET;
	//▲1.0.0.1057

	//SerialNumber
	INT iSerial[6];
	memset(iSerial,0,sizeof(iSerial));
	INT nLength = szSerialNumber.GetLength();
	//year
	if( nLength>1 )
		iSerial[0] = _tcstol(szSerialNumber.Left(2), NULL, 10);
	//month
	if( nLength>2 )
	{
		//▼1.0.0.1057
		//PTCHAR szMonth[] = {_T(" "),_T("A"),_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),_T("G"),_T("H"),_T("J"),_T("K"),_T("L"),_T("M")};
		//for( int i=0; i<_countof(szMonth); i++ )
		//{
		//	if( szSerialNumber.Mid(2,1)==szMonth[i] )
		//	{
		//		iSerial[1] = i;
		//		break;
		//	}
		//}
		for( int i=0; i<_countof(pchrAlphabet); i++ )
		{
			if( szSerialNumber.GetAt(2)==pchrAlphabet[i] )
			{
				iSerial[1] = (i%12) + ((i/12)<<4);
				break;
			}
		}
		//▲1.0.0.1057

	}
	//serial
	//▼1.0.0.1057
	if( nLength>3 )
	{
		INT nSerial = -1;
		for( int i=0; i<10 && nSerial<0; i++ )
		{
			if( szSerialNumber.GetAt(3) == TEXT('0')+i )
			{
				nSerial = i;
				break;
			}
		}
		for( int i=0; i<_countof(pchrAlphabet) && nSerial<0 ; i++ )
		{
			if( szSerialNumber.GetAt(3) == pchrAlphabet[i] )
			{
				nSerial = i + 10;
				break;
			}
		}
		if( nSerial>=0 )
		{
			iSerial[2] = nSerial;
		}
	}

	//for( int i=2; i<6; i++ )
	for( int i=3; i<6; i++ )
	//▲1.0.0.1057
	{
		if( nLength>i+1 )
			iSerial[i] = _tcstol(szSerialNumber.Mid(i+1,1), NULL, 16);
	}

	BOOL bReval = TRUE;
	CString szAddress;
	for( int i=0; i<6 && bReval == TRUE; i++ )
	{
		//▼1.0.0.1057
		CString szAppendAddress(_T(""));
		//▲1.0.0.1057
		if( i==0 ) szAddress = m_szYearAddress;
		//▼1.0.0.1057
		//else if( i==1 ) szAddress = m_szMonthAddress;
		//else if( i==2 ) szAddress = m_szSN1Address;
		else if( i==1 )
		{
			szAddress = m_szMonthAddress;
			szAppendAddress = m_szMonthAddress2;
		}
		else if( i==2 )
		{
			szAddress = m_szSN1Address;
			szAppendAddress = m_szSN1Address2;
		}
		//▲1.0.0.1057
		else if( i==3 ) szAddress = m_szSN2Address;
		else if( i==4 ) szAddress = m_szSN3Address;
		else if( i==5 ) szAddress = m_szSN4Address;

		bReval = WriteSerialNumberEach( pComm, szAddress, iSerial[i] );
		//▼1.0.0.1057
		if( bReval && szAppendAddress.GetLength()>0 )
		{
			bReval = WriteSerialNumberEach( pComm, szAppendAddress, iSerial[i], 1 );
		}
		//▲1.0.0.1057
	}

	return bReval;
}

//▼1.0.0.1057
//BOOL StWriteSerial::WriteSerialNumberEach( void *pComm, CString szSerial, INT nSerial )
BOOL StWriteSerial::WriteSerialNumberEach( void *pComm, CString szSerial, INT nSerial, INT nQuadBit )
//▲1.0.0.1057
{
	CString szAddr(_T(""));
	INT istbit = 0;
	INT ilenbit = 0;
	INT iPos = szSerial.Find(_T("LSB"));
	if( iPos>=0 )
	{
		ilenbit = 4;
		istbit = 0;
		if( iPos>0 ) szAddr = szSerial.Left(iPos);
	}
	else
	{
		iPos = szSerial.Find(_T("MSB"));
		if( iPos>=0 )
		{
			ilenbit = 4;
			istbit = 4;
			if( iPos>0 ) szAddr = szSerial.Left(iPos);
		}
	}

	//▼1.0.0.1057
	if( iPos<0 )
	{
		CString szQSB;
		for( int i=0; i<4; i++ )
		{
			szQSB.Format(_T("Q%dSB"),i);
			iPos = szSerial.Find(szQSB);
			if( iPos>0 )
			{
				ilenbit = 2;
				istbit = 2*i;
				break;
			}
		}
	}
	//▲1.0.0.1057

	if( iPos<0 )
	{
		ilenbit = 8;
		istbit = 0;
		szAddr = szSerial;
	}
	//▼1.0.0.1057
	nSerial >>= (nQuadBit*4); 
	nSerial &= (1<<ilenbit)-1;
	//▲1.0.0.1057

	//Address
	WORD wAddr = _tcstol(szAddr,NULL,0);

	//DeviceCode, Page
	WORD wDeviceCode = _tcstol(m_szDeviceCode,NULL,0);
	WORD wPage = _tcstol(m_szPage,NULL,0);

	StSerialComm *pSerialComm = (StSerialComm *)pComm;
	//BOOL WriteBitData( BYTE byteAddr, BYTE byteDevPage, INT istbit, INT ilenbit, WORD dwData );
	BOOL bReval = pSerialComm->WriteBitData((BYTE)wAddr,(BYTE)wDeviceCode,istbit,ilenbit,nSerial);

	return bReval;
}

//-------------------------------------------------------------------------------
//Ascii,Takano用書込み
//-------------------------------------------------------------------------------
BOOL StWriteSerial::WriteText(void *pComm, LPCTSTR szData)
{
	StSerialComm *pSerialComm = (StSerialComm *)pComm;

	//▼1.0.0.1022
	//CString szText = m_szCommand + _T(" ") + szData;
	//TCHAR szRcvText[32];
	//memset(szRcvText,0,sizeof(szRcvText));
	//BOOL bReval = pSerialComm->SendText( szText, szRcvText, _countof(szRcvText) );

	CString szText;
	szText.Format(m_szCommand,szData);
	pSerialComm->SetRecieveData(m_szRecieve);
	BOOL bReval = pSerialComm->SendData( szText );
	//▲1.0.0.1022

	return bReval;
}

//-------------------------------------------------------------------------------
//function
//-------------------------------------------------------------------------------
BOOL StWriteSerial::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction == _T("Execute") )
	{
	}
	else
	if( szFunction=="SetSerialMode" )
	{
		bReval = SetSerialMode(szArgument);
	}
	else
	if( szFunction=="SetMode" )
	{
		bReval = SetMode(szArgument);
	}
	else
	if( szFunction=="SetDeviceCode" )
	{
		m_szDeviceCode = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetPage" )
	{
		m_szPage = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetYearAddress" )
	{
		m_szYearAddress = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMonthAddress" )
	{
		m_szMonthAddress = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetSN1Address" )
	{
		m_szSN1Address = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetSN2Address" )
	{
		m_szSN2Address = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetSN3Address" )
	{
		m_szSN3Address = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetSN4Address" )
	{
		m_szSN4Address = szArgument;
		bReval = TRUE;
	}
	//▼1.0.0.1057
	else
	if( szFunction=="SetMonthAddress2" )
	{
		m_szMonthAddress2 = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetSN1Address2" )
	{
		m_szSN1Address2 = szArgument;
		bReval = TRUE;
	}
	//▲1.0.0.1057
	else
	if( szFunction=="SetCommand" )
	{
		m_szCommand = szArgument;
		bReval = TRUE;
	}
	//▼1.0.0.1022
	else
	if( szFunction=="ReturnData" || szFunction=="SetReturnData" )
	{
		m_szRecieve = szArgument;
		bReval = TRUE;
	}
	//▲1.0.0.1022

	//▼1.0.0.1046
	else
	if( szFunction=="Address" || szFunction=="SetAddress" )
	{
		m_dwAddress = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="Length" || szFunction=="SetLength" )
	{
		m_nLength = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	//▲1.0.0.1046

	return bReval;

}