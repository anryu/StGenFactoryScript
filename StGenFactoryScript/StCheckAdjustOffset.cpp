#include "StdAfx.h"
#include "StCheckAdjustOffset.h"
#include "StInspectMain.h"


StCheckAdjustOffset::StCheckAdjustOffset(void)
	: m_pParent(NULL)
{
	Initialize();
}
StCheckAdjustOffset::StCheckAdjustOffset(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void* pParent)
	:StCheckBase( szClassName, szVarName, aDeviceBase )
	, m_pParent(pParent)
{
	Initialize();
}


StCheckAdjustOffset::~StCheckAdjustOffset(void)
{
	Delete();
}

void StCheckAdjustOffset::Initialize(void)
{
	m_nStartValue = 2045;
	m_nMaxValue = 8192;
	m_nChannelCount = 2;
	m_szCMDL=_T("da_m%dl");
	m_szCMDH=_T("da_m%dh");
	m_nLBIT = 4;
	//▼1.0.0.1047
	//m_szTrigger = _T("litr");
	m_szTrigger = _T("");
	//▲1.0.0.1047

	m_nTimer = 1000;
	m_nStartLine = 0;
	m_nEndLine = 15999;
	m_nBase = 1;
	m_nInc = 1;

	m_nDiffValue = 16;
	m_nCtrl = 0;
	//▼1.0.0.1067b
	m_nBunch = 1;	//1以上
	m_nEvenOddReverseMode = 0;
	//▲1.0.0.1067b

	//for( int i = 0; i<_countof(m_nGetMin); i++ )
	//{
	//	m_nGetMin[i] = -1;
	//}

	m_pOffsetResult = NULL;
	m_pOffsetData = NULL;
}

void StCheckAdjustOffset::Delete(void)
{
	if( m_pOffsetResult ) delete [] m_pOffsetResult;
	m_pOffsetResult = NULL;
	if( m_pOffsetData ) delete [] m_pOffsetData;
	m_pOffsetData = NULL;
}

void StCheckAdjustOffset::DisplayListBox(CString szMessge)
{
	if( !m_pParent ) return;
	StInspectMain *aParent = (StInspectMain *)m_pParent;
	aParent->DisplayListBox(szMessge);
}

BOOL StCheckAdjustOffset::OffsetInit(void)
{
	BOOL bReval = FALSE;

	m_nGetCount = -1;

	do
	{
		Delete();
		int i;
		CString tmp;
		m_pOffsetResult = new S_ST_OFFSET_RESULT[m_nChannelCount];
		if( !m_pOffsetResult )
		{
			break;
		}
		m_pOffsetData = new S_ST_OFFSET_DATA[m_nChannelCount];
		if( !m_pOffsetData )
		{
			break;
		}
		for( i=0; i<m_nChannelCount; i++ )
		{
			m_pOffsetResult[i].wChannel = i+1;
			m_pOffsetResult[i].iMin = -1;
			m_pOffsetResult[i].iResult = -1;
			m_pOffsetData[i].iSetData = m_nStartValue;
			for( int j=0; j<_countof(m_pOffsetData[i].iSaveData); j++ )
			{
				m_pOffsetData[i].iSaveData[j] = -1;
			}
			for( int j=0; j<_countof(m_pOffsetData[i].iSaveOffset); j++ )
			{
				//▼1.0.0.1058
				//m_pOffsetData[i].iSaveData[j] = -1;
				m_pOffsetData[i].iSaveOffset[j] = -1;
				//▲1.0.0.1058
			}

			bReval = SendOffsetData( i, m_pOffsetData[i].iSetData );
		}
		bReval = SendTrigger();
	}while(0);

	if( !bReval )
	{
		if( m_pOffsetResult ) delete [] m_pOffsetResult;
		m_pOffsetResult = NULL;
		if( m_pOffsetData ) delete [] m_pOffsetData;
		m_pOffsetData = NULL;
	}

	return bReval;
}

BOOL StCheckAdjustOffset::SendOffsetData(INT nChannel, INT nData)
{
	BOOL bReval = FALSE;

	if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//GigE用
	{
		bReval = WriteDataEBus( 0, 0, m_nOffsetAddress[0][nChannel], (nData<<(8-m_nLBIT))&0xff );
		if( bReval )
			bReval = WriteDataEBus( 0, 0, m_nOffsetAddress[1][nChannel], nData>>m_nLBIT );
	}
	else
	if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL用
	{
		if( m_nCtrl==1 )
		{
			//▼1.0.0.1047
			//StString szCommand = m_szTrigger;
			//bReval = WriteDataMultiCamAscii( szCommand, (nData<<(8-m_nLBIT))&0xff );
			CString szCommand;
			if( m_szCMDL.GetLength()>0 )
			{
				szCommand.Format(m_szCMDL,nChannel+1);
			}
			StString aCommand(szCommand);
			if( aCommand.GetLength()>0 )
				bReval = WriteDataMultiCamAscii( aCommand, (nData<<(8-m_nLBIT))&0xff );
			if( bReval )
			{
				szCommand.Empty();
				if( m_szCMDL.GetLength()>0 )
					szCommand.Format(m_szCMDH,nChannel+1);
				aCommand = szCommand;
				if( aCommand.GetLength()>0 )
					bReval = WriteDataMultiCamAscii( aCommand, nData>>m_nLBIT );
				else
					bReval = FALSE;			
			}
			//▲1.0.0.1047
		}
		else
		{
			bReval = WriteDataMultiCamBinary( 0, 0, m_nOffsetAddress[0][nChannel], (nData<<(8-m_nLBIT))&0xff );
			if( bReval )
				bReval = WriteDataMultiCamBinary( 0, 0, m_nOffsetAddress[1][nChannel], nData>>m_nLBIT );
		}
	}
	else
	if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )	//GenICam用
	{


	}
	if( !bReval )
	{
		m_nErrorCode = StResult::Code::NOT_SUPPORTED;
	}

	return bReval;
}

BOOL StCheckAdjustOffset::SendTrigger(void)
{
	BOOL bReval = FALSE;

	if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//GigE用
	{
		bReval = WriteDataEBus( 0, 0, m_nTriggerAddress, 1<<m_nTriggerBit );
	}
	else
	if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL用
	{
		if( m_nCtrl==1 )
		{
			//▼1.0.0.1047
			//StString szCommand = m_szTrigger;
			//bReval = WriteDataMultiCamAscii( szCommand  );
			bReval = TRUE;
			if( m_szTrigger.GetLength()>0 )
			{
				StString szCommand = m_szTrigger;
				bReval = WriteDataMultiCamAscii( szCommand  );
			}
			//▲1.0.0.1047
		}
		else
		{
			bReval = WriteDataMultiCamBinary( 0, 0, m_nTriggerAddress, 1<<m_nTriggerBit );
		}
	}
	else
	if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )	//GenICam用
	{


	}
	if( !bReval )
	{
		m_nErrorCode = StResult::Code::NOT_SUPPORTED;
	}

	return bReval;
}

BOOL StCheckAdjustOffset::OffsetSetValue(void)
{
	BOOL bReval = FALSE;



	return bReval;
}



//---------------------------------------------------------------------------------
//	int data			:今回の取得データ（MIN）
//	int offset			:今回のオフセット
//	int *pData			:Offset[0]のときのデータ値　Offset[1]のときのデータ値
//	int *pOffset		:[0]データが１以下の時の最小オフセット値　[1]データが2以上の時の最大オフセット値
//	int *pNextOffset	:次回のオフセット
//	return				:OFFSET値がきまったかどうか
//オフセット値を小さくするとデータは大きくなる。
//最小値が１以下の時と２以上の時のオフセットを求めていく。オフセット差が１になるまで
//pOffset[0],pOffset[1]が-1の時はまだデータが入っていない
//pOffset[1]よりpOffset[0]のほうが必ず大きいはず
//---------------------------------------------------------------------------------
BOOL StCheckAdjustOffset::CheckOffset( int data, int offset, int *pData, int *pOffset, int *pNextOffset )
{
	BOOL bRet=FALSE;
	if( data<=m_nBase )	//m_nOffset_Bace以下
	{
		pOffset[0] = offset;	//pOffset[0]とpOffset[1]の差は必ず縮まっていく
		pData[0] = data;
		if( pOffset[1]<0 )
		{
			*pNextOffset = max( offset-m_nDiffValue, 0 );
			return FALSE;
		}
	}
	else
	{						//data>m_nOffset_Bace
		pOffset[1] = offset;
		pData[1] = data;
		if( pOffset[0]<0 )
		{
			*pNextOffset = min( offset+m_nDiffValue, m_nMaxValue );
			return FALSE;
		}
	}

	//pOffset[0],pOffset[1]に両方値が入っている時

	//オフセット値が決まった時 小さいほうが最終OFFSET値
	if( pOffset[0]-pOffset[1]<2 )
	{
		*pNextOffset = min(pOffset[0],pOffset[1]);	//必ずpOffset[1]が入るはず
		bRet = TRUE;
	}
	else
	{
		*pNextOffset = (pOffset[0]+pOffset[1])/2;
		bRet = FALSE;
	}
	return bRet;
}

/*
BOOL StCheckAdjustOffset::OffsetGetValue(void)
{
	BOOL bReval = FALSE;

	if( m_nGetMin[0]>=0 )
	{

		char text[128];
			
		char tmpCat[128];
		sprintf( text, "mindata=" );
		for( int i=0; i<m_nChannelCount; i++ ){
			if( i!=0 ) strcat( text,":");
			sprintf( tmpCat, "ch%d(%d)", i+1, m_nGetMin[i] );
			strcat( text,tmpCat);
		}
		
		//m_pDoc->m_pScriptDlg->m_scriptlist.AddString(text);
		//m_offsetlist.AddString(text);
		//int lines = m_offsetlist.GetCount();
		//m_offsetlist.SetTopIndex(max(0,lines-19));	//19はリストボックスの高さによる。
		//if( m_pDoc->allLogFileOpen( m_pDoc->m_sAllLogOutFull ) ){
		//	m_pDoc->m_LogFile.WriteString( text );
		//	m_pDoc->m_LogFile.WriteString("\n");
		//	m_pDoc->m_LogFile.Close();
		//}


		//BOOL bRet[MAX_CH] = {FALSE,FALSE};
		//int nextOffset[MAX_CH] = {0,0};
		
		PINT pnNextOffset = new INT [m_nChannelCount];
		BOOL bCheck=TRUE;

		for( int i=0;  i<m_nChannelCount; i++ )
		{
			BOOL bCheckOffset = CheckOffset( m_nGetMin[i], m_pOffsetData[i].iSetData, &m_pOffsetData[i].iSaveData[0], &m_pOffsetData[i].iSaveOffset[0], &pnNextOffset[i] );
			if( bCheckOffset ) bCheck = FALSE;

			m_pOffsetResult[i].iResult =  m_pOffsetData[i].iSetData;
			m_pOffsetResult[i].iMin = m_nGetMin[i];
		}

		INT iInc = 0;
		if( bCheck==TRUE )
		{
			iInc = m_nInc;
		}
		for( INT i=0;  i<m_nChannelCount; i++ )
		{
			m_pOffsetData[i].iSetData = max( 0, min( m_nRange, pnNextOffset[i] + iInc ) );
		}

		//EvenOddともTRUEでも最終設定値をセットしておく
		for( INT i=0;  i<m_nChannelCount; i++ )
		{
			SendOffsetData( i, m_pOffsetData[i].iSetData );
		}
		SendTrigger();

		//bCheck = TRUE;
		//for( INT i=0;  i<m_nChannelCount; i++ ){
		//	if( bRet[i]==FALSE ) bCheck=FALSE;
		//}
		if( bCheck==TRUE )
		{
			//for( INT i=0;  i<m_nChannelCount; i++ )
			//{
			//	m_pOffsetResult[i].iResult = iSetData[i];
			//}
			m_iResult=1;
		}
		else
		{
			SetTimer( OFFSET_GETRES, g_iOffset_TM, NULL );	//値取得
		}
		for( INT i=0;  i<m_nChannelCount; i++ )
		{
			m_nGetMin[i]=-1;
		}
		//----▲Ver1.22---------

	}
	if( m_iResult!=0)
	{
		//終了
		//Alllogに各色の最後の設定値、出力値を表示
		//Finalaverage_R,Finashd_ad_R
		FinalResulttoAlllog();
		OnOK();
		return;
	}

	return bReval;
}

BOOL StCheckAdjustOffset::OffsetGetResult(void)
{
	BOOL bReval = FALSE;

	m_nGetCount++;
	if( m_nGetCount>1 )
	{

		BOOL bGetBuffer = m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー

		if( !bGetBuffer )
		{
			//m_nGetCount--;
			return FALSE;
		}

		int minData[2]={-1,-1};
		bReval = GetMinLevel( m_Buffer, m_nStartLine, m_nEndLine, minData );
		if( !bReval )
		{
			//m_nGetCount--;
			return FALSE;
		}

		if( minData[0]>=0 )
		{
			if( m_nChannelCount==1 ){
				m_nGetMin[0] = minData[0] < minData[1] ? minData[0] : minData[1];
			}
			else
			{
				for( int i=0;  i<m_nChannelCount && i<_countof(minData); i++ )
					m_nGetMin[i] = minData[i];
			}

			m_nGetCount = 0;

		}
	}

	return bReval;

}
*/
BOOL StCheckAdjustOffset::GetResultAndSetValue(void)
{

	BOOL bReval = FALSE;
	BOOL bResult = FALSE;

	PINT pnGetMin = NULL;
	//▼1.0.0.1058
	PINT pnGetMinLine = NULL;
	//▲1.0.0.1058
	do
	{
		//▼1.0.0.1047
		//pnGetMin = new INT [m_nChannelCount];
		//if( !pnGetMin ) break;
		if( !pnGetMin )
		{
			pnGetMin = new INT [m_nChannelCount];
			if( !pnGetMin ) break;
		}
		//▲1.0.0.1047

		//Sleep(m_nTimer);
		INT nNGCount=0;
		INT nCount=0;


		//画像取得し、値取得 m_nGetMin-----------------------
		memset(pnGetMin,0xff,sizeof(INT)*m_nChannelCount);
		//▼1.0.0.1058
		if( !pnGetMinLine )
		{
			pnGetMinLine = new INT [m_nChannelCount];
			if( pnGetMinLine )
				memset(pnGetMinLine,0xff,sizeof(*pnGetMinLine)*m_nChannelCount);
		}
		//▲1.0.0.1058
		do
		{
			//if( m_Buffer ) delete m_Buffer;
			//m_Buffer = new StBuffer;

			BOOL bGetBuffer = m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー
			if( !bGetBuffer )
			{
				Sleep(m_nTimer);
				nNGCount++;
			}
			else
			{
				if( nCount>0 )
				{
					int minData[2]={-1,-1};
					//▼1.0.0.1058
					//bReval = GetMinLevel( m_Buffer, m_nStartLine, m_nEndLine, minData );
					int minLine[2]={-1,-1};
					//▼1.0.0.1067b
					//bReval = GetMinLevel( m_Buffer, m_nStartLine, m_nEndLine, minData, minLine );
					////▲1.0.0.1058
					bReval = GetMinLevel( m_Buffer, m_nStartLine, m_nEndLine, minData, minLine, m_nBunch );
					//▲1.0.0.1067b
					if( !bReval )
					{
						nNGCount++;
						//m_nGetCount--;
						//return FALSE;
					}

					if( minData[0]>=0 )
					{
						if( m_nChannelCount==1 )
						{
							//▼1.0.0.1058
							//pnGetMin[0] = minData[0] < minData[1] ? minData[0] : minData[1];
							if( minData[0] < minData[1] )
							{
								pnGetMin[0] = minData[0];
								pnGetMinLine[0] = minLine[0];
							}
							else
							{
								pnGetMin[0] = minData[1];
								pnGetMinLine[0] = minLine[1];
							}
							//▲1.0.0.1058
//TCHAR text[128];
//_stprintf_s( text,_countof(text), _T("mindata(%d)  minLineX(%d) Y(%d)\n"), pnGetMin[0], minLine[0]&0xffff, minLine[0]>>16 );
//TRACE(text);
						}
						else
						{

							//▼1.0.0.1067b
							//EvenOdd入れ替え
							if( m_nEvenOddReverseMode>0 )
							{
								for( int i=0;  i<m_nChannelCount && i<_countof(minData); i+=2 )
								{
									INT nData = minData[i];
									minData[i] = minData[i+1];
									minData[i+1] = nData;
									nData = minLine[i];
									minLine[i] = minLine[i+1];
									minLine[i+1] = nData;
								}
							}
							//▲1.0.0.1067b

							//▼1.0.0.1058
							for( int i=0;  i<m_nChannelCount && i<_countof(minData); i++ )
							//	pnGetMin[i] = minData[i];
							{
								pnGetMin[i] = minData[i];
								pnGetMinLine[i] = minLine[i];
							}
							//▲1.0.0.1058
						}
					}
					break;
				}
				nCount++;
			}
		}while(nNGCount<10);
		//-----------------------------------------------------------------------

		//取得した値m_nGetMinから設定値取得-----------------------
		if( pnGetMin[0]>=0 )
		{

			//▼1.0.0.1058
			TCHAR text[128];
			TCHAR tmpCat[128];
			_stprintf_s( text, _T(" min=") );
			for( int i=0; i<m_nChannelCount; i++ )
			{
				if( i!=0 ) _tcscat_s( text,_countof(text),_T(":"));
				_stprintf_s( tmpCat,_countof(tmpCat), _T("ch%d(%d)X(%d:%d)"), i+1, pnGetMin[i], pnGetMinLine[i]&0xffff, pnGetMin[i], pnGetMinLine[i]>>16 );
				_tcscat_s( text,_countof(text),tmpCat);
			}
			DisplayListBox(text);
			//▲1.0.0.1058
//TRACE(text);
//TRACE(TEXT("\n"));

			//m_pDoc->m_pScriptDlg->m_scriptlist.AddString(text);
			//m_offsetlist.AddString(text);
			//int lines = m_offsetlist.GetCount();
			//m_offsetlist.SetTopIndex(max(0,lines-19));	//19はリストボックスの高さによる。
			//if( m_pDoc->allLogFileOpen( m_pDoc->m_sAllLogOutFull ) ){
			//	m_pDoc->m_LogFile.WriteString( text );
			//	m_pDoc->m_LogFile.WriteString("\n");
			//	m_pDoc->m_LogFile.Close();
			//}


			//BOOL bRet[MAX_CH] = {FALSE,FALSE};
			//int nextOffset[MAX_CH] = {0,0};
		
			PINT pnNextOffset = new INT [m_nChannelCount];
			BOOL bCheck=TRUE;

			for( int i=0;  i<m_nChannelCount; i++ )
			{
				BOOL bCheckOffset = CheckOffset( pnGetMin[i], m_pOffsetData[i].iSetData, &m_pOffsetData[i].iSaveData[0], &m_pOffsetData[i].iSaveOffset[0], &pnNextOffset[i] );
				//▼1.0.0.1047
				//if( bCheckOffset ) bCheck = FALSE;
				if( !bCheckOffset ) bCheck = FALSE;
				//▲1.0.0.1047
				m_pOffsetResult[i].iResult =  m_pOffsetData[i].iSetData;
				m_pOffsetResult[i].iMin = pnGetMin[i];
			}

			INT iInc = 0;
			if( bCheck==TRUE )
			{
				iInc = m_nInc;
			}

			//▼1.0.0.1058
			BOOL bChange = FALSE;
			//▲1.0.0.1058
			for( INT i=0;  i<m_nChannelCount; i++ )
			{
				//▼1.0.0.1058
				INT nDefault = m_pOffsetData[i].iSetData;
				//▲1.0.0.1058
				//▼1.0.0.1047
				//m_pOffsetData[i].iSetData = max( 0, min( m_nRange, pnNextOffset[i] + iInc ) );
				m_pOffsetData[i].iSetData = max( 0, min( m_nMaxValue, pnNextOffset[i] + iInc ) );
				//▲1.0.0.1047
				//▼1.0.0.1058
				if( nDefault!=m_pOffsetData[i].iSetData )
				{
					bChange = TRUE;
				}
				//▲1.0.0.1058
			}
			delete [] pnNextOffset;

			//▼1.0.0.1058
			if( !bChange && !bCheck )
			{
				bReval = FALSE;
				break;
			}
			//▲1.0.0.1058

			//EvenOddともTRUEでも最終設定値をセットしておく
			//▼1.0.0.1058
			_stprintf_s( text, _T("offsetdata=") );
			//▲1.0.0.1058
			for( INT i=0; i<m_nChannelCount; i++ )
			{
				SendOffsetData( i, m_pOffsetData[i].iSetData );
				//▼1.0.0.1058
				if( i!=0 ) _tcscat_s( text,_countof(text),_T(":"));
				_stprintf_s( tmpCat,_countof(tmpCat), _T("ch%d(%d)"), i+1, m_pOffsetData[i].iSetData );
				_tcscat_s( text,_countof(text),tmpCat);
				//▲1.0.0.1058
//_stprintf_s( text, _T("offsetdata=") );
//{
//	if( i!=0 ) _tcscat_s( text,_countof(text),_T(":"));
//	_stprintf_s( tmpCat,_countof(tmpCat), _T("ch%d(%d)\n"), i+1, m_pOffsetData[i].iSetData );
//	_tcscat_s( text,_countof(text),tmpCat);
//TRACE(text);
//}
			}
			//▼1.0.0.1058
			DisplayListBox(text);
			//▲1.0.0.1058





			SendTrigger();

			Sleep(m_nTimer);

			if( bCheck==TRUE )
			{
				for( INT i=0;  i<m_nChannelCount; i++ )
				{
					m_pOffsetResult[i].iResult =  m_pOffsetData[i].iSetData;
				}
				bResult = TRUE;
			}
			//else
			//{
			//	SetTimer( OFFSET_GETRES, g_iOffset_TM, NULL );	//値取得
			//}
			//for( INT i=0;  i<m_nChannelCount; i++ )
			//{
			//	m_nGetMin[i]=-1;
			//}


		}

		//-----------------------------------------------------------------------


	}while(bResult==FALSE);

	//▼1.0.0.1047
	//delete [] pnGetMin;
	if( pnGetMin )	delete [] pnGetMin;
	//▲1.0.0.1047

	//▼1.0.0.1058
	if( pnGetMinLine )	delete [] pnGetMinLine;
	//▲1.0.0.1058

	return bReval;

}


//-------------------------------------------------------------------------------
//検査実行
//-------------------------------------------------------------------------------
BOOL StCheckAdjustOffset::Execute(void)
{
	BOOL bReval = FALSE;
	m_iJudge = 2;	//2:処理中
	if( m_Buffer ) delete m_Buffer;
	m_Buffer = new StBuffer;

	//初期値設定
	bReval = OffsetInit();
	if( bReval )
	{
		bReval = GetResultAndSetValue();
	}

	if( bReval )
	{
		m_iJudge = 1;	//1:OK
	}
	else
	{
		m_iJudge = 0;	//0:NG
	}


	//do
	//{

	//	OffsetSetValue();


	//	//Sleep();
	//}while(1);


	return bReval;
}

BOOL StCheckAdjustOffset::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);

	if( szFunction=="SetStartValue" )
	{
		m_nStartValue = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMaxValue" )
	{
		m_nMaxValue = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetChannelCount" )
	{
		m_nChannelCount = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetCMDL" )
	{
		m_szCMDL = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetCMDH" )
	{
		//▼1.0.0.1047
		//m_szCMDL = szArgument;
		m_szCMDH = szArgument;
		//▲1.0.0.1047
		bReval = TRUE;
	}
	else
	if( szFunction=="SetLBIT" )
	{
		m_nLBIT = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTrigger" )
	{
		m_szTrigger = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTimer" )
	{
		m_nTimer = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetStartLine" )
	{
		m_nStartLine = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetEndLine" )
	{
		m_nEndLine = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetBase" )
	{
		m_nBase = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetInc" )
	{
		m_nInc = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetDiffValue" )
	{
		m_nDiffValue = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTriggerAddress" )
	{
		//▼1.0.0.1058
		//m_nTriggerAddress = _ttoi(szArgument);
		m_nTriggerAddress = _tcstol(szArgument,NULL,0);
		//▲1.0.0.1058

		bReval = TRUE;
	}
	else
	if( szFunction=="SetTriggerBit" )
	{
		m_nTriggerBit = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetCtrl" )
	{
		m_nCtrl = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction.Left(10)=="SetAddress" || szFunction.Left(7)=="Address" )
	{
		if( szFunction.Left(3)=="Set" )
			szFunction = szFunction.Mid(3);
		if( szFunction.Left(7)=="Address" )
			szFunction = szFunction.Mid(7);
		INT nLW=-1;
		if( szFunction.Left(1)=="L" )
		{
			nLW = 0;
		}
		else
		if( szFunction.Left(1)=="H" )
		{
			nLW = 1;
		}
		if( nLW>=0 )
		{
			szFunction = szFunction.Mid(1);
			//▼1.0.0.1058
			//INT nData = _ttoi(szFunction) - 1;
			INT nData = _ttoi(szFunction);
			//▲1.0.0.1058
			if( nData>=0 && nData<_countof(m_nOffsetAddress[0]) )
			{
				//▼1.0.0.1058
				//m_nOffsetAddress[nLW][nData] =  _ttoi(szArgument);
				m_nOffsetAddress[nLW][nData] = _tcstol(szArgument,NULL,0);
				//▲1.0.0.1058
				bReval = TRUE;
			}
		}
	}
	//▼1.0.0.1067b
	else
	if( szFunction=="SetBunch" || szFunction=="Bunch" )
	{
		m_nBunch = _ttoi(szArgument);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetEvenOddReverseMode" || szFunction=="EvenOddReverseMode" )
	{
		m_nEvenOddReverseMode = _ttoi(szArgument);
		bReval = TRUE;
	}
	//▲1.0.0.1067b
	return bReval;

}

BOOL StCheckAdjustOffset::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetChannelCount" )
	{
		dblValue = m_nChannelCount;
		bReval = TRUE;
	}
	else
	if( szFunction=="GetResultOffset" )
	{
		INT nChannel = _ttoi(szArg);
		dblValue = m_pOffsetResult[nChannel].iResult;
		bReval = TRUE;
	}

	return bReval;
}
