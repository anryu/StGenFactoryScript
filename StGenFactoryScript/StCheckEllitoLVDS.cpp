#include "StdAfx.h"
#include "StCheckEllitoLVDS.h"

#include "StInspectMain.h"
#include "StCheckControlRetry.h"
#include "StCommon.h"

extern INT nLMapTable[8][15]; //[Clock][LVDS]

StCheckEllitoLVDS::StCheckEllitoLVDS(void)
{
	Initialize();
}

StCheckEllitoLVDS::StCheckEllitoLVDS( LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase)
	: StCheckBase( szClassName, szVarName, aDeviceBase )
{
	Initialize();
}

StCheckEllitoLVDS::~StCheckEllitoLVDS(void)
{
}

void StCheckEllitoLVDS::Initialize(void)
{
	//m_pControlRetry = NULL;
	//m_szSavePath = _T(".");

	//memset(m_nEditClockID,0,sizeof(m_nEditClockID));
	//memset(m_nSpinClockID,0,sizeof(m_nSpinClockID));
	//memset(m_nEditSyncID,0,sizeof(m_nEditSyncID));
	//memset(m_nSpinSyncID,0,sizeof(m_nSpinSyncID));
	//memset( m_nButtonDelayID, 0, sizeof(m_nButtonDelayID) );
	//memset( m_nSliceSETable, 0, sizeof(m_nSliceSETable) );

	memset(m_delay,0,sizeof(m_delay));
	memset(m_clock,0,sizeof(m_clock));
	memset(m_sync,0,sizeof(m_sync));
	memset(m_wLMAP,0,sizeof(m_wLMAP));

}

//-------------------------------------------------------------------------------
//検査実行
//-------------------------------------------------------------------------------
extern UINT GetIntFromEllitoFormat(LPCTSTR szText);

BOOL StCheckEllitoLVDS::Execute(void)
{
	BOOL bReval = FALSE;
	if( !m_DeviceBase )
	{
		m_iJudge = 0;
		return FALSE;
	}
		
	m_iJudge = 2;	//2:処理中
	StCheckControlRetry *aControlRetry = NULL;
	StDeviceGenICam *aDeviceGenICam = NULL;

	INT nDefRetry = -1;
	CString szReturn(_T(""));
	DWORD dwDefTimeout[5];
	memset(dwDefTimeout,-1,sizeof(dwDefTimeout));
	INT iDefAFE0Data = -1;
	INT nAFE0 = 0xAFE0;
	do
	{
		double dblValue=0.0;
		if( m_DeviceBase->m_ClassName=="StDeviceEBus" )	//GigE用
		{
			m_iJudge = 0;
			break;
		}
		else
		if( m_DeviceBase->m_ClassName=="StDeviceMultiCam" )	//CL用
		{
			StDeviceMultiCam *aDevice = (StDeviceMultiCam *)m_DeviceBase;
			if( !aDevice ) break;
			if( aDevice->IsOpened() )
			{
				aControlRetry = (StCheckControlRetry *)aDevice->GetControlRetry();
			}
			if( !aControlRetry ) break;


			nDefRetry = aControlRetry->GetRetry();
			aControlRetry->SetRetry(0);

			aControlRetry->GetReturn(szReturn);
			aControlRetry->SetRecieveData(_T(""));

			
			aControlRetry->GetCommTimeout(&dwDefTimeout[0],&dwDefTimeout[1],&dwDefTimeout[2],&dwDefTimeout[3],&dwDefTimeout[4]);
			aControlRetry->SetCommTimeout(50,50,0,0,0);


			TCHAR cmdText[64];
			CString szRcvData;
			BOOL bReval = TRUE;
			for( int i=0; i<5; i++ )
			{
				_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"RED %d\",0x0d"), i * 3 );	
				aControlRetry->SetSendData(cmdText);
				bReval = aControlRetry->Execute();
				if( !bReval ) break;
				aControlRetry->GetLastRecieveData(szRcvData);
				UINT unValue = GetIntFromEllitoFormat(szRcvData);
				if( i==0 )
				{
					m_delay[0] = (unValue >> 9 ) & 0x07;
				}
				else
				{
					for( int j=0; j<4; j++ )
					{
						m_delay[ (i-1)*8 + 3-j + 1] = ( unValue >> (j*3) ) & 0x07;
					}
				}
				if( i>0 )
				{
					_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"RED %d\",0x0d"), i * 3 + 1 );	
					aControlRetry->SetSendData(cmdText);
					bReval = aControlRetry->Execute();
					if( !bReval ) break;
					aControlRetry->GetLastRecieveData(szRcvData);	//戻り値(17文字)  CRLF 00000000 CRLF USER>
					unValue = GetIntFromEllitoFormat(szRcvData);
					for( int j=0; j<4; j++ )
					{
						m_delay[ (i-1)*8 + 3-j + 5 ] = ( unValue >> (j*3) ) & 0x07;
					}
				}
				_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"RED %d\",0x0d"), i * 3 + 2 );	
				aControlRetry->SetSendData(cmdText);
				bReval = aControlRetry->Execute();
				if( !bReval ) break;
				aControlRetry->GetLastRecieveData(szRcvData);	//戻り値(17文字)  CRLF 00000000 CRLF USER>
				unValue = GetIntFromEllitoFormat(szRcvData);

				m_sync[i] = ( unValue >> 3 ) & 0x07;
				m_clock[i] = ( unValue >> 0 ) & 0x07;
			}
			if( !bReval ) break;

			//LMAP受信
			for( int i=0; i<sizeof(m_wLMAP)/sizeof(m_wLMAP[0]); i++ )
			{
				_stprintf_s( cmdText, _countof(cmdText), _T("0x02,\"LMAP %d\",0x0d"), i );	
				aControlRetry->SetSendData(cmdText);
				bReval = aControlRetry->Execute();
				if( !bReval ) break;
				aControlRetry->GetLastRecieveData(szRcvData);	//戻り値(19文字)  CRLF 0x00000000 CRLF USER>
				m_wLMAP[i] = (WORD)GetIntFromEllitoFormat(szRcvData);
			}
		}
		else
		if( m_DeviceBase->m_ClassName=="StDeviceGenICam" )	//GenICam用
		{
			aDeviceGenICam = (StDeviceGenICam *)m_DeviceBase;
			if( !aDeviceGenICam ) break;
			if( !aDeviceGenICam->IsOpened() ) break;

			//bReval = aDeviceGenICam->ReadMain(nAFE0,(char *)&iDefAFE0Data,sizeof(iDefAFE0Data));
			//if( !bReval ) break;

			//0xAFE0に0x000Aをセット
			UINT uiData = 0x000A; 
			bReval = aDeviceGenICam->WriteMain(nAFE0,(char *)&uiData,sizeof(uiData));
			if( !bReval ) break;


			UINT unValue;
			for( int i=0; i<5; i++ )
			{
				DWORD dwAddress = 0xB000 + (i * 3) * 4;
				bReval = aDeviceGenICam->ReadMain(dwAddress,(char *)&unValue,sizeof(unValue));
				if( !bReval ) break;

				if( i==0 )
				{
					m_delay[0] = (unValue >> 9 ) & 0x07;
				}
				else
				{
					for( int j=0; j<4; j++ )
					{
						m_delay[ (i-1)*8 + 3-j + 1] = ( unValue >> (j*3) ) & 0x07;
					}
				}
				if( i>0 )
				{
					dwAddress = 0xB000 + (i * 3 + 1 ) * 4;
					bReval = aDeviceGenICam->ReadMain(dwAddress,(char *)&unValue,sizeof(unValue));
					if( !bReval ) break;
					for( int j=0; j<4; j++ )
					{
						m_delay[ (i-1)*8 + 3-j + 5 ] = ( unValue >> (j*3) ) & 0x07;
					}
				}

				dwAddress = 0xB000 + (i * 3 + 2) * 4;
				bReval = aDeviceGenICam->ReadMain(dwAddress,(char *)&unValue,sizeof(unValue));
				if( !bReval ) break;

				m_sync[i] = ( unValue >> 3 ) & 0x07;
				m_clock[i] = ( unValue >> 0 ) & 0x07;
			}
			if( !bReval ) break;

			//LMAP受信
			uiData = 0x0009;
			bReval = aDeviceGenICam->WriteMain(nAFE0,(char *)&uiData,sizeof(uiData));
			if( !bReval ) break;

			for( int i=0; i<_countof(m_wLMAP) && bReval; i++ )
			{
				DWORD dwAddress = 0xB000 + i * 4;
				bReval = aDeviceGenICam->ReadMain(dwAddress,(char *)&unValue,sizeof(unValue));
				if( bReval )
					m_wLMAP[i] = unValue;
			}
		}
		if( bReval )
		{
			for( int i=0; i<sizeof(m_wLMAP)/sizeof(m_wLMAP[0]); i++ )
			{
				//INT nLine = (i+7)/8;
				INT nClock = m_clock[(i+7)/8];
				INT nLVDS = 0;
				for( int j=0; j<15; j++ )
				{
					if( nLMapTable[nClock][j]==m_delay[i] )
					{
						nLVDS = j;
						break;
					}
				}
				//_stprintf_s( szText, _countof(szText), _T("Data%02d, 0x%04X, "),i,m_wLMAP[i] );
				INT nAtMark = -1;
				INT nMark[15];	//0:@ 1:0 2:X 3:x
				memset(nMark,-1,sizeof(nMark));
				for( int j=0; j<15; j++ )
				{
					INT nShift = (3-((j+1)/4))*4 + 3 - ((j+1)&3);
					nMark[j] = ((m_wLMAP[i]>>nShift)&1)>0?(j==nLVDS?0:1):(j==nLVDS?2:3);
					if( nMark[j]==0 ) nAtMark=j;
				}
				if( nAtMark<0 ) m_iJudge = 0;
				else
				{
					//Before (Left)
					if(nAtMark>0)
					{ 
						if( nMark[nAtMark-1]>1 ) m_iJudge = 0;
					}
					else
					{
						 if( nMark[11]>1 ) m_iJudge = 0;	//12個で位相１周
					}
					//After (Right)
					if(nAtMark<14)
					{ 
						if( nMark[nAtMark+1]>1 ) m_iJudge = 0;
					}
					else
					{
						 if( nMark[3]>1 ) m_iJudge = 0;	//12個で位相１周
					}
				}
				if( m_iJudge==0 ) break;
			}
			if( m_iJudge==2 ) m_iJudge = 1;
		}
		break;
	}while(0);
	if( aControlRetry )
	{
		if( nDefRetry>=0 )
		{
			aControlRetry->SetRetry(nDefRetry);
		}
		if( szReturn.GetLength()>0 )
		{
			aControlRetry->SetRecieveData(szReturn);
		}
		if( dwDefTimeout[0]!=0xffffffff )
		{
			aControlRetry->SetCommTimeout(dwDefTimeout[0],dwDefTimeout[1],dwDefTimeout[2],dwDefTimeout[3],dwDefTimeout[4]);
		}
	}
	else
	if( aDeviceGenICam )
	{
		if( iDefAFE0Data>=0 )
		{
			aDeviceGenICam->WriteMain(nAFE0,(char *)&iDefAFE0Data,sizeof(iDefAFE0Data));
		}
	}

	if( m_iJudge==2 ) m_iJudge = 0;	//1:OK 0:NG

	return bReval;
}

BOOL StCheckEllitoLVDS::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	return bReval;
}

BOOL StCheckEllitoLVDS::GetResultText( INT nIndex, LPTSTR szResult, size_t size )
{
	if( nIndex>=_countof(m_delay) ) return FALSE;
	if( nIndex>=_countof(m_wLMAP) ) return FALSE;
	TCHAR szTmp[128];

	INT nClock = m_clock[(nIndex+7)/8];
	INT nLVDS = 0;
	for( int j=0; j<15; j++ )
	{
		if( nLMapTable[nClock][j]==m_delay[nIndex] )
		{
			nLVDS = j;
			break;
		}
	}
	_stprintf_s( szResult, size, _T("0x%04X, "),m_wLMAP[nIndex] );
	for( int j=0; j<15; j++ )
	{
		INT nShift = (3-((j+1)/4))*4 + 3 - ((j+1)&3);
		_stprintf_s( szTmp, _countof(szTmp), _T("%s,"), ((m_wLMAP[nIndex]>>nShift)&1)>0?(j==nLVDS?_T("@"):_T("0")):(j==nLVDS?_T("X"):_T("x")) );
		if( j==12 )
		{
			_tcscat_s(szResult,size,_T("  "));
		}
		_tcscat_s(szResult,size,szTmp);
	}

	return TRUE;
}
