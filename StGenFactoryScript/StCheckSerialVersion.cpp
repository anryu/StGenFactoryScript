#include "StdAfx.h"
#include "StCheckSerialVersion.h"
//#include <PvString.h>
//▼1.0.0.15
//#include "StDeviceEBus.h"
#include "StInspectMain.h"
//▲1.0.0.15

StCheckSerialVersion::StCheckSerialVersion(void)
{
	Initialize();
}

StCheckSerialVersion::StCheckSerialVersion(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
{
	Initialize();
}

StCheckSerialVersion::~StCheckSerialVersion(void)
{
}

void StCheckSerialVersion::Initialize(void)
{
	m_szDeviceVersion = _T("");
	m_szDeviceSerialNumber = _T("");
	m_szDeviceFirmwareVersion = _T("");

	m_szResultDeviceVersion = _T("");
	m_szResultDeviceSerialNumber = _T("");
	m_szResultDeviceFirmwareVersion = _T("");

	m_IsCheckDeviceVersion = FALSE;
	m_IsCheckDeviceSerialNumber = FALSE;
	m_IsCheckDeviceFirmwareVersion = FALSE;

	//▼1.0.0.1071
	m_szDeviceModelName = _T("");
	m_szResultDeviceModelName = _T("");
	m_IsCheckDeviceModelName = FALSE;
	//▲1.0.0.1071

	//▼1.0.0.15
	m_nMode = 0;       //0:Binary 1:Ascii 2:Takano
	m_szSensorSerialNumber = _T("");
	m_szResultSensorSerialNumber = _T("");
	m_IsCheckSensorSerialNumber = FALSE;
	//▲1.0.0.15

	//▼1.0.0.1037
	m_sYearAddress = _T("0x07");
	m_sMonthAddress = _T("0x06LSB");
	m_sSN1Address = _T("0x04LSB");
	m_sSN2Address = _T("0x05MSB");
	m_sSN3Address = _T("0x05LSB");
	m_sSN4Address = _T("0x06MSB");
	m_sPDTDevice = _T("0");
	m_sPDTPage = _T("0");
	//▲1.0.0.1037

	//▼1.0.0.1043
	m_szEllitoSerialNumberFilepath = _T("");
	m_szEllitoSerialNumberFilename = _T("");
	//▲1.0.0.1043
	//▼1.0.0.1058
	//GenICam用アドレス
	m_dwAddress = 0xB000;
	m_dwSelectorAddress = 0xB000;
	m_nDeviceSerialNumberSelectorValue = 11;
	m_nSensorSerialNumberSelectorValue = 12;
	m_nSensorSerialNumberLength = 0;
	//▲1.0.0.1058

}

//-------------------------------------------------------------------------------
//検査実行
//-------------------------------------------------------------------------------
BOOL StCheckSerialVersion::Execute(void)
{
	BOOL bReval = FALSE;

	m_iJudge = 2;	//2:処理中

	do{
		if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//GigE用
		{
			TCHAR szText[128];
			StDeviceEBus *aDevice = (StDeviceEBus *)m_DeviceBase;
			if( m_szDeviceVersion.GetLength()>0 )
			{
				bReval = aDevice->GetGenICamControl(_T("DeviceVersion"),szText,_countof(szText));
				if( bReval )
				{
					m_szResultDeviceVersion = szText;
					if( m_szDeviceVersion!=m_szResultDeviceVersion )
						m_iJudge = 0;
				}
				m_IsCheckDeviceVersion = TRUE;
				if( !bReval || m_iJudge == 0)
					break;
			}
			//-----------------------------------------------------------
			if( m_szDeviceFirmwareVersion.GetLength()>0 )
			{
				bReval = aDevice->GetGenICamControl(_T("DeviceFirmwareVersion"),szText,_countof(szText));
				if( bReval )
				{
					m_szResultDeviceFirmwareVersion = szText;
					if( m_szDeviceFirmwareVersion!=m_szResultDeviceFirmwareVersion ) m_iJudge = 0;
				}
				m_IsCheckDeviceFirmwareVersion = TRUE;
				if( !bReval || m_iJudge == 0)
					break;
			}
			//-----------------------------------------------------------
			if( m_szDeviceSerialNumber.GetLength()>0 )
			{
				bReval = aDevice->GetGenICamControl(_T("DeviceID"),szText,_countof(szText));
				if( !bReval )
				{
					bReval = aDevice->GetGenICamControl(_T("DeviceSerialNumber"),szText,_countof(szText));
				}
				if( bReval )
				{
					m_szResultDeviceSerialNumber = szText;
					if( m_szDeviceSerialNumber!=m_szResultDeviceSerialNumber ) m_iJudge = 0;
				}
				m_IsCheckDeviceSerialNumber = TRUE;
				if( !bReval || m_iJudge == 0)
					break;
			}
			//▼1.0.0.1071
			//-----------------------------------------------------------
			if( m_szDeviceModelName.GetLength()>0 )
			{
				bReval = aDevice->GetGenICamControl(_T("DeviceModelName"),szText,_countof(szText));
				if( bReval )
				{
					m_szResultDeviceModelName = szText;
					if( m_szDeviceModelName!=m_szResultDeviceModelName ) m_iJudge = 0;
				}
				m_IsCheckDeviceModelName = TRUE;
				if( !bReval || m_iJudge == 0)
					break;
			}
			//▲1.0.0.1071

			bReval = TRUE;
		}
		else
		if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )
		{

			//▼1.0.0.15
			TCHAR szText[128];
			StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
			if( !aDevice->IsOpened() ) break;

			CString szPortName;
			aDevice->GetPortName(szPortName);
			CString szVarName = m_VarName;
			szVarName += _T("_ControlRetry");
			StSerialComm aSerialComm(_T("StSerialComm"),szVarName,szPortName);
			
			if( m_nMode == 0 )	//Binary
			{
				//▼1.0.0.1037
				if( m_szDeviceSerialNumber.GetLength()>0 )
				{

					StSerialComm *aSerialComm2 = aDevice->GetSerialComm();

					int iSerial[6];
					int iReadSerialNumber[6];
					memset(iReadSerialNumber,0,sizeof(iReadSerialNumber));
					char *szMonth[] = {"","A","B","C","D","E","F","G","H","J","K","L","M"};
					int srlCount=_countof(iSerial);

					INT nCheck=0;
					m_szResultDeviceSerialNumber.Empty();
					for( int i=0; i<srlCount; i++ )
					{
						CString szSerial("");
						if( i==0 ) szSerial = m_sYearAddress;
						else if( i==1 ) szSerial = m_sMonthAddress;
						else if( i==2 ) szSerial = m_sSN1Address;
						else if( i==3 ) szSerial = m_sSN2Address;
						else if( i==4 ) szSerial = m_sSN3Address;
						else if( i==5 ) szSerial = m_sSN4Address;

						if( szSerial.GetLength()>0 )
						{
							//読み込み
							BYTE byteReadData=0;
							bReval = ReadBinary( aSerialComm2, szSerial, &byteReadData );

							if( !bReval )
							{
								nCheck |= (1<<i);
							}
							else
							{
								iReadSerialNumber[i] = byteReadData;
								//if( iReadSerialNumber[i]!=iSerial[i] )
								//{
								//	nCheck |= (1<<i);
								//}
							}
						}
						CString szTmp;
						if( i==0 )
							szTmp.Format(_T("%02d"), iReadSerialNumber[i] );
						else if( i==1 )
							szTmp = szMonth[iReadSerialNumber[i]];
						else
							szTmp.Format(_T("%01X"), iReadSerialNumber[i] );
						m_szResultDeviceSerialNumber += szTmp;
					}

					if( nCheck>0 ) m_iJudge = 0;
					if( m_szDeviceSerialNumber!=m_szResultDeviceSerialNumber ) m_iJudge = 0;
					m_IsCheckDeviceSerialNumber = TRUE;
					if( !bReval || m_iJudge == 0)
						break;
				}
				//▲1.0.0.1037
			}
			else
			if( m_nMode == 2 )	//Takano
			{

				aSerialComm.function(_T("SetSendSTX"),_T("02"));
				aSerialComm.function(_T("SetSendETX"),_T("0d"));
				aSerialComm.function(_T("SetRecieveSTX"),_T("0d0a"));
				//aSerialComm.function(_T("SetRecieveETX"),_T("USER>"));
				aSerialComm.function(_T("SetRecieveETX"),_T("0d0a555345523e"));	//CR LF USER>

				if( m_szDeviceVersion.GetLength()>0 )
				{
				}
				//-----------------------------------------------------------
				if( m_szDeviceFirmwareVersion.GetLength()>0 )
				{
				}
				//-----------------------------------------------------------
				if( m_szDeviceSerialNumber.GetLength()>0 )
				{
					memset(szText,0,sizeof(szText));
					bReval = ReadText((void *)&aSerialComm, _T("RSNC"), szText, _countof(szText));
					if( bReval )
					{
						m_szResultDeviceSerialNumber = szText;
						//TAKANOの場合大文字になる。。
						if( m_szDeviceSerialNumber.MakeUpper()!=m_szResultDeviceSerialNumber ) m_iJudge = 0;
					}
					m_IsCheckDeviceSerialNumber = TRUE;
					if( !bReval || m_iJudge == 0)
						break;
				}
				//-----------------------------------------------------------
				if( m_szSensorSerialNumber.GetLength()>0 )
				{
					//▼1.0.0.1043
					//aSerialComm.function(_T("SetDelayTime"),CString(_T("8000")));
					aSerialComm.function(_T("SetRecieveDelayTime"),CString(_T("8000")));
					//▲1.0.0.1043
					memset(szText,0,sizeof(szText));
					bReval = ReadText((void *)&aSerialComm, _T("RSNS"), szText, _countof(szText));
					//▼1.0.0.1043
					//aSerialComm.function(_T("SetDelayTime"),CString(_T("0")));
					aSerialComm.function(_T("SetRecieveDelayTime"),CString(_T("0")));
					//▲1.0.0.1043
					if( bReval )
					{
						m_szResultSensorSerialNumber = szText;

						//▼1.0.0.1044

						//▼1.0.0.1043
						//▼1.0.0.1029
						////if( m_szSensorSerialNumber.MakeUpper()!=m_szResultSensorSerialNumber ) m_iJudge = 0;
						//if( m_vCheckETX.size()>0 )
						//{
						//	BOOL bCheck = FALSE;
						//	for( int i=0; i<m_vCheckETX.size(); i++ )
						//	{
						//		CString szCheckSerialNumber = m_szSensorSerialNumber;
						//		szCheckSerialNumber += m_vCheckETX[i];
						//		INT nSize = szCheckSerialNumber.GetLength();
						//		if( szCheckSerialNumber==m_szResultSensorSerialNumber.Left(nSize) )
						//		{
						//			bCheck = TRUE;
						//			break;
						//		}
						//	}
						//	if( bCheck==FALSE ) m_iJudge = 0;
						//}
						//else
						//{
						//	INT nSize = m_szSensorSerialNumber.GetLength();
						//	if( m_szSensorSerialNumber!=m_szResultSensorSerialNumber.Left(nSize) )
						//		m_iJudge = 0;
						//}
						//▲1.0.0.1029

						//入力したセンサーシリアル末尾にbがある場合、ファイルを読み込んで、bを削除していいか確認する。
						//bを削除する場合と削除しない場合両方対応
						//BOOL bEraseCheckb = FALSE;
						//if( m_szSensorSerialNumber.Right(1).Compare(_T("b") )==0 )
						//{
						//	//bがつくかどうかをファイルからシリアルを持ってきて判断する。
						//	CString szSerialNumberFile = m_szEllitoSerialNumberFilepath;
						//	if( szSerialNumberFile.Right(1).Compare(_T("\\") )!=0 )
						//	{
						//		szSerialNumberFile += _T("\\");
						//	}
						//	szSerialNumberFile += m_szEllitoSerialNumberFilename;

						//	CStdioFile scriptFile;
						//	if( scriptFile.Open(szSerialNumberFile,CFile::modeRead,NULL)!=0 )
						//	{
						//		CString tmp;
						//		while( scriptFile.ReadString(tmp) )
						//		{
						//			tmp.Trim();
						//			if( !tmp.IsEmpty() && tmp.Left(1).Compare(_T("#"))!=0 )
						//			{
						//				if( tmp==m_szSensorSerialNumber.Left(m_szSensorSerialNumber.GetLength()-1) )
						//				{
						//					bEraseCheckb = TRUE;
						//					break;
						//				}
						//			}
						//		}
						//	}
						//}



						//BOOL bCheck = FALSE;
						//for( int i=0; i<(bEraseCheckb?2:1) && bCheck==FALSE; i++ )
						//{
						//	CString szOrgCheckSerialNumber = m_szSensorSerialNumber;
						//	if( i==1 )	//bがついてない場合
						//	{
						//		szOrgCheckSerialNumber = szOrgCheckSerialNumber.Left(szOrgCheckSerialNumber.GetLength()-1);
						//	}
						//	if( m_vCheckETX.size()>0 )
						//	{
						//		for( int i=0; i<m_vCheckETX.size(); i++ )
						//		{
						//			CString szCheckSerialNumber = szOrgCheckSerialNumber;
						//			szCheckSerialNumber += m_vCheckETX[i];
						//			INT nSize = szCheckSerialNumber.GetLength();
						//			if( szCheckSerialNumber==m_szResultSensorSerialNumber.Left(nSize) )
						//			{
						//				bCheck = TRUE;
						//				break;
						//			}
						//		}
						//		//if( bCheck==FALSE ) m_iJudge = 0;
						//	}
						//	else
						//	{
						//		INT nSize = szOrgCheckSerialNumber.GetLength();
						//		if( szOrgCheckSerialNumber==m_szResultSensorSerialNumber.Left(nSize) )
						//			bCheck = TRUE;
						//	}

						//}

						//末尾にアスタリスクがあるので削除
						while(m_szResultSensorSerialNumber.Right(1)==_T("*") ) 
						{
							m_szResultSensorSerialNumber = m_szResultSensorSerialNumber.Left(m_szResultSensorSerialNumber.GetLength()-1);
						}

						INT nPattern = -1;
						BOOL bCheck = FALSE;
						if( m_szSensorSerialNumber.Right(1).Compare(_T("b") )!=0 )	//入力値末尾にbがない場合。
						{
							nPattern = 2;
						}
						else
						if( m_szSensorSerialNumber.Right(1).Compare(_T("b") )==0 )	//入力値末尾にbがある場合。
						{
							//パターンA）
							//白印字あり
							//かつ、カメラからReadしたセンサーシリアル末尾にb有り。
							//b有り品は必ず白印字がある。
							//シリアル重複品。

							BOOL bPatternA = FALSE;
							//bがつくかどうかをファイルからシリアルを持ってきて判断する。
							CString szSerialNumberFile = m_szEllitoSerialNumberFilepath;
							if( szSerialNumberFile.Right(1).Compare(_T("\\") )!=0 )
							{
								szSerialNumberFile += _T("\\");
							}
							szSerialNumberFile += m_szEllitoSerialNumberFilename;

							CStdioFile scriptFile;
							if( scriptFile.Open(szSerialNumberFile,CFile::modeRead,NULL)!=0 )
							{
								CString tmp;
								while( scriptFile.ReadString(tmp) )
								{
									tmp.Trim();
									if( !tmp.IsEmpty() && tmp.Left(1).Compare(_T("#"))!=0 )
									{
										if( tmp==m_szSensorSerialNumber )
										{
											bPatternA = TRUE;
											break;
										}
									}
								}
								scriptFile.Close();
							}

							//パターンAの場合
							if( bPatternA )
							{
								nPattern = 0;
							}
							else
							{
								nPattern = 1;
							}
						}

						CString szCheckOrgSensorSerialNumber = m_szSensorSerialNumber;
						if( nPattern==1 )	//PatternB
						{
							//末尾bをとる。
							szCheckOrgSensorSerialNumber = szCheckOrgSensorSerialNumber.Left(m_szSensorSerialNumber.GetLength()-1);
						}

						for( int i=0; i<m_vCheckETX.size()+1; i++ )
						{
							CString szCheckSrcSensorSerialNumber = szCheckOrgSensorSerialNumber;
							if( i>0 )
							{
								szCheckSrcSensorSerialNumber += m_vCheckETX[i-1];
							}

							//INT nLength = szCheckSrcSensorSerialNumber.GetLength();
							//if( szCheckSrcSensorSerialNumber==m_szResultSensorSerialNumber.Left(nLength) )
							if( szCheckSrcSensorSerialNumber==m_szResultSensorSerialNumber )
							{
								bCheck = TRUE;
								break;
							}
						}

						//▲1.0.0.1044

						if( bCheck==FALSE ) m_iJudge = 0;
						//▲1.0.0.1043

					}
					m_IsCheckSensorSerialNumber = TRUE;
					if( !bReval || m_iJudge == 0)
						break;
				}
			}
			//▲1.0.0.15
			bReval = TRUE;

		}
		//▼1.0.0.1058
		else
		if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )
		{
			if( m_nMode == 2 )	//Takano
			{
				StDeviceGenICam *aDevice = (StDeviceGenICam *)m_DeviceBase;
				if( !aDevice->IsOpened() ) break;

				if( m_szDeviceVersion.GetLength()>0 )
				{
				}
				//-----------------------------------------------------------
				if( m_szDeviceFirmwareVersion.GetLength()>0 )
				{
				}
				//-----------------------------------------------------------
				if( m_szDeviceSerialNumber.GetLength()>0 )
				{
					bReval = aDevice->WriteMain(m_dwSelectorAddress, (char *)&m_nDeviceSerialNumberSelectorValue, sizeof(m_nDeviceSerialNumberSelectorValue) );
					if( !bReval )
						break;
					INT nLength = m_szDeviceSerialNumber.GetLength();
					if( nLength & 3 ) nLength +=  4 - nLength & 3;

					char *pbyteData = new char [nLength + 1];
					if( pbyteData )
					{
						memset( pbyteData, 0 , nLength + 1);
						bReval = aDevice->ReadMain(m_dwAddress, pbyteData, nLength,FALSE);	//SensorSerialにあわせてFALSE
						if( bReval )
						{

//TRACE(_T(" @@@@@@@ pbyteData=%x:%x:%x:%x:%x:%x:%x:%x:\n"), pbyteData[0], pbyteData[1], pbyteData[2], pbyteData[3], pbyteData[4], pbyteData[5], pbyteData[6], pbyteData[7]);

							m_szResultDeviceSerialNumber = pbyteData;
							//TAKANOの場合大文字になる。。
							if( m_szDeviceSerialNumber.MakeUpper()!=m_szResultDeviceSerialNumber ) m_iJudge = 0;
						}
						delete [] pbyteData;
						m_IsCheckDeviceSerialNumber = TRUE;
						if( !bReval || m_iJudge == 0)
							break;
					}
				}
				//-----------------------------------------------------------
				if( m_szSensorSerialNumber.GetLength()>0 )
				{
					bReval = aDevice->WriteMain(m_dwSelectorAddress, (char *)&m_nSensorSerialNumberSelectorValue, sizeof(m_nSensorSerialNumberSelectorValue) );
					if( !bReval )
						break;

					//INT nLength = m_szSensorSerialNumber.GetLength();
					//nLength += 1;	//bがつく場合がある。
					INT nLength = m_nSensorSerialNumberLength;
					if( nLength & 3 ) nLength +=  4 - nLength & 3;
					char *pbyteData = new char [nLength + 1];
					if( pbyteData )
					{
						memset( pbyteData, 0 , nLength + 1);
						bReval = aDevice->ReadMain(m_dwAddress, pbyteData, nLength,FALSE);	//FALSEにしないと正常動作しない
						if( bReval )
						{

							m_szResultSensorSerialNumber = pbyteData;

							//末尾にアスタリスクがあるので削除
							while(m_szResultSensorSerialNumber.Right(1)==_T("*") ) 
							{
								m_szResultSensorSerialNumber = m_szResultSensorSerialNumber.Left(m_szResultSensorSerialNumber.GetLength()-1);
							}


							INT nPattern = -1;
							BOOL bCheck = FALSE;
							if( m_szSensorSerialNumber.Right(1).Compare(_T("b") )!=0 )	//入力値末尾にbがない場合。
							{
								nPattern = 2;
							}
							else
							if( m_szSensorSerialNumber.Right(1).Compare(_T("b") )==0 )	//入力値末尾にbがある場合。
							{
								//パターンA）
								//白印字あり
								//かつ、カメラからReadしたセンサーシリアル末尾にb有り。
								//b有り品は必ず白印字がある。
								//シリアル重複品。

								BOOL bPatternA = FALSE;
								//bがつくかどうかをファイルからシリアルを持ってきて判断する。
								CString szSerialNumberFile = m_szEllitoSerialNumberFilepath;
								if( szSerialNumberFile.Right(1).Compare(_T("\\") )!=0 )
								{
									szSerialNumberFile += _T("\\");
								}
								szSerialNumberFile += m_szEllitoSerialNumberFilename;

								CStdioFile scriptFile;
								if( scriptFile.Open(szSerialNumberFile,CFile::modeRead,NULL)!=0 )
								{
									CString tmp;
									while( scriptFile.ReadString(tmp) )
									{
										tmp.Trim();
										if( !tmp.IsEmpty() && tmp.Left(1).Compare(_T("#"))!=0 )
										{
											if( tmp==m_szSensorSerialNumber )
											{
												bPatternA = TRUE;
												break;
											}
										}
									}
									scriptFile.Close();
								}

								//パターンAの場合
								if( bPatternA )
								{
									nPattern = 0;
								}
								else
								{
									nPattern = 1;
								}
							}

							CString szCheckOrgSensorSerialNumber = m_szSensorSerialNumber;
							if( nPattern==1 )	//PatternB
							{
								//末尾bをとる。
								szCheckOrgSensorSerialNumber = szCheckOrgSensorSerialNumber.Left(m_szSensorSerialNumber.GetLength()-1);
							}

							for( int i=0; i<m_vCheckETX.size()+1; i++ )
							{
								CString szCheckSrcSensorSerialNumber = szCheckOrgSensorSerialNumber;
								if( i>0 )
								{
									szCheckSrcSensorSerialNumber += m_vCheckETX[i-1];
								}

								if( szCheckSrcSensorSerialNumber==m_szResultSensorSerialNumber )
								{
									bCheck = TRUE;
									break;
								}
							}


							if( bCheck==FALSE ) m_iJudge = 0;

						}
						delete [] pbyteData;

						m_IsCheckSensorSerialNumber = TRUE;
						if( !bReval || m_iJudge == 0)
							break;

					}
				}
			}
		}
		//▲1.0.0.1058
	}while(0);


	if( !bReval ) m_iJudge = 0;
	else
	if( m_iJudge == 2 ) m_iJudge = 1;

	return bReval;

}

//▼1.0.0.15 カメラリンク用
//-------------------------------------------------------------------------------
//Ascii,Takano用書込み カメラリンク用
//-------------------------------------------------------------------------------
BOOL StCheckSerialVersion::ReadText(void *pComm, LPCTSTR szCommand, LPTSTR szData, size_t dataSize)
{
	//CString szText = szCommand;
	//szText += _T(" ");
	//szText += szData;

	StSerialComm *pSerialComm = (StSerialComm *)pComm;
	//▼1.0.0.1034
	//TCHAR szRcvText[32];
	TCHAR szRcvText[256];
	//▲1.0.0.1034
	memset(szRcvText,0,sizeof(szRcvText));
	BOOL bReval = pSerialComm->SendText( szCommand, szRcvText, _countof(szRcvText) );
	if( bReval )
	{
		_tcscpy_s(szData,dataSize,szRcvText);
	}
	return bReval;
}
//▲1.0.0.15

//-------------------------------------------------------------------------------
//function
//-------------------------------------------------------------------------------
BOOL StCheckSerialVersion::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="SetDeviceVersion" )
	{
		m_szDeviceVersion = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetDeviceSerialNumber" )
	{
		m_szDeviceSerialNumber = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetDeviceFirmwareVersion" )
	{
		m_szDeviceFirmwareVersion = szArgument;
		bReval = TRUE;
	}
	//▼1.0.0.1071
	else
	if( szFunction=="SetDeviceModelName" )
	{
		m_szDeviceModelName = szArgument;
		bReval = TRUE;
	}
	//▲1.0.0.1071
	//▼1.0.0.15
	else
	if( szFunction=="SetSensorSerialNumber" )
	{
		m_szSensorSerialNumber = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMode" )
	{
		m_nMode = _ttoi(szArgument);
		bReval = TRUE;
	}
	//▲1.0.0.15

	//▼1.0.0.1029
	else
	if( szFunction=="AddSensorSerialETX" )
	{
		bReval = AddSensorSerialETX(szArgument);
	}
	else
	if( szFunction=="ClearSensorSerialETX" )
	{
		bReval = ClearSensorSerialETX();
	}
	//▲1.0.0.1029
	//▼1.0.0.1037
	else
	if( szFunction=="SetYearAddress" )
	{
		m_sYearAddress = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMonthAddress" )
	{
		m_sMonthAddress = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetSN1Address" )
	{
		m_sSN1Address = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetSN2Address" )
	{
		m_sSN2Address = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetSN3Address" )
	{
		m_sSN3Address = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetSN4Address" )
	{
		m_sSN4Address = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetDeviceCode" )
	{
		m_sPDTDevice = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetPage" )
	{
		m_sPDTPage = szArgument;
		bReval = TRUE;
	}
	//▲1.0.0.1037
	//▼1.0.0.1043
	else
	if( szFunction=="SetEllitoSerialNumberFilepath" )
	{
		m_szEllitoSerialNumberFilepath = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetEllitoSerialNumberFilename" )
	{
		m_szEllitoSerialNumberFilename = szArgument;
		bReval = TRUE;
	}
	//▲1.0.0.1043
	//▼1.0.0.1058
	else
	if( szFunction=="Address" || szFunction=="SetAddress" )
	{
		m_dwAddress = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SelectorAddress" || szFunction=="SetSelectorAddress" )
	{
		m_dwSelectorAddress = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="DeviceSerialNumberSelectorValue" || szFunction=="SetDeviceSerialNumberSelectorValue" )
	{
		m_nDeviceSerialNumberSelectorValue = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SensorSerialNumberSelectorValue" || szFunction=="SetSensorSerialNumberSelectorValue" )
	{
		m_nSensorSerialNumberSelectorValue = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SensorSerialNumberLength" || szFunction=="SetSensorSerialNumberLength" )
	{
		m_nSensorSerialNumberLength = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	//▲1.0.0.1058
	else
	if( szFunction=="Execute" )
	{
		//bReval = Execute();
	}

	return bReval;

}

//▼1.0.0.1025
BOOL StCheckSerialVersion::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, CString &szValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetDeviceVersion" )
	{
		szValue = m_szResultDeviceVersion;
		bReval = TRUE;
	}
	else
	if( szFunction=="GetDeviceSerialNumber" )
	{
		szValue = m_szResultDeviceSerialNumber;
		bReval = TRUE;
	}
	else
	if( szFunction=="GetDeviceFirmwareVersion" )
	{
		szValue = m_szResultDeviceFirmwareVersion;
		bReval = TRUE;
	}
	else
	if( szFunction=="GetSensorSerialNumber" )
	{
		szValue = m_szResultSensorSerialNumber;
		bReval = TRUE;
	}
	//▼1.0.0.1071
	else
	if( szFunction=="GetDeviceModelName" )
	{
		szValue = m_szResultDeviceModelName;
		bReval = TRUE;
	}
	//▲1.0.0.1071

	return bReval;
}
//▲1.0.0.1025

//▼1.0.0.1029
BOOL StCheckSerialVersion::AddSensorSerialETX( LPCTSTR szETX )
{
	m_vCheckETX.push_back(szETX);
	return TRUE;
}

BOOL StCheckSerialVersion::ClearSensorSerialETX( void )
{
	m_vCheckETX.clear();
	return TRUE;
}
//▲1.0.0.1029

//▼1.0.0.1037
BOOL StCheckSerialVersion::ReadBinary(void *pComm, CString &szAddress, PBYTE pData)
{
	CString szAddr("");
	int istbit = 0;
	int ilenbit = 0;
	int iPos = szAddress.Find(_T("LSB"),0);
	if( iPos>=0 )
	{
		ilenbit = 4;
		istbit = 0;
		if( iPos>0 )	szAddr = szAddress.Left(iPos);
	}
	else
	{
		iPos = szAddress.Find(_T("MSB"),0);
		if( iPos>=0 )
		{
			ilenbit = 4;
			istbit = 4;
			if( iPos>0 )	szAddr = szAddress.Left(iPos);
		}
	}
	if( iPos<0 )
	{
		ilenbit = 8;
		istbit = 0;
		szAddr = szAddress;
	}
	//address
	WORD wAddr =  _tcstol(szAddr,NULL,0);
	TCHAR *e;
	if( szAddr.Find(_T("0x"),0) ==0 )
	{
		wAddr = _tcstol(szAddr.Mid(2),&e,16);	//
	}
	else
	{
		wAddr = _ttoi(szAddr);
	}
	//DeviceCode, Page
	WORD wDeviceCode = _tcstol(m_sPDTDevice,NULL,16);
	BYTE byteDevPage=(BYTE)(wDeviceCode<<2);

	WORD wPage = _tcstol(m_sPDTPage,NULL,0);
	byteDevPage |= (BYTE)wPage;

	int ret = 0;
	WORD wSerial=0;
	BYTE byteSend[6]={2,0,0,1,0,3};
	byteSend[2] = wAddr;
	size_t sendSize = sizeof(byteSend);
	BYTE byteRcv[4];
	size_t rcvSize = sizeof(byteRcv);

	StSerialComm *pSerialComm = (StSerialComm *)pComm;
	BOOL bReval = pSerialComm->SendBin(byteSend,sendSize,byteRcv,&rcvSize);
	if( bReval )
	{
		if( rcvSize!=sizeof(byteRcv) ) bReval = FALSE;
		if( byteRcv[0]!=2 || byteRcv[1]!=1 || byteRcv[3]!=3 ) bReval = FALSE;
	}
	if( bReval )
	{
		WORD wMask = ((1<<ilenbit)-1);
		*pData = (byteRcv[2]>>istbit)&wMask;
	}
	return bReval;
}
//▲1.0.0.1037


