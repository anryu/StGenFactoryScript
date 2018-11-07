#include "StdAfx.h"
#include "StLightControl.h"
#include "StInspectMain.h"

/*
unsigned __stdcall LightControlThread( void *pParam )
{
	StLightControl * aCheck = (StLightControl *)pParam;

	//初期値セット
	BOOL bReval = aCheck->SendInitValue();
	if( bReval )
	{
		aCheck->LightAdjustExecute();
	}
	_endthreadex(0);
	return 0;
}
*/

//▼1.0.0.1023
LPCTSTR szColorName[_COLORCOUNT] = {_T("R"),_T("G"),_T("B"),_T("W")};
//▲1.0.0.1023
//▼1.0.0.1067
extern void DoEvents();
//▲1.0.0.1067


StLightControl::StLightControl(void)
	: m_pParent(NULL)
{
	Initialize();
}

StLightControl::StLightControl(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void* pParent, void *aDialog)
	:StCheckBase( szClassName, szVarName, aDeviceBase, aDialog )
	, m_pParent(pParent)
{
	Initialize();
}

StLightControl::~StLightControl(void)
{
}

void StLightControl::Initialize(void)
{
	m_hThread = NULL;
	m_pSerialComm = NULL;
	//▼1.0.0.1017
	m_pTerminal = NULL;
	//▲1.0.0.1017

	m_nStart = 0;
	m_nEnd = 4095;
	m_nTimer = 1000;
	m_szColor = _T("");

	for( int i=0; i<_countof(m_nMaxValue); i++  )
		m_nMaxValue[i] = 1023;
	for( int i=0; i<_countof(m_nInitValue); i++  )
		m_nInitValue[i] = 0;
	for( int i=0; i<_countof(m_nTarget); i++  )
		m_nTarget[i] = 210;
	for( int i=0; i<_countof(m_nTargetRange); i++  )
		m_nTargetRange[i] = 10;
	//▼1.0.0.1023
	m_szInfoFileName = _T("LightInfo.txt");
	//▲1.0.0.1023
	//▼1.0.0.1024
	m_nLogMode = 0;
	//▲1.0.0.1024

}

BOOL StLightControl::function(LPCTSTR szFunc, LPCTSTR szArgument)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction == _T("Execute") )
	{
	}
	else
	if( szFunction=="SetSerialComm" )
	{
		//m_nStart = _tcstol(szArgument,NULL,0);
		//bReval = TRUE;
	}
	else
	if( szFunction=="SetStartLine" )
	{
		m_nStart = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetEndLine" )
	{
		m_nEnd = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTimer" )
	{
		m_nTimer = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetColor" )
	{
		m_szColor = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMaxValueR" )
	{
		m_nMaxValue[0] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMaxValueG" )
	{
		m_nMaxValue[1] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetMaxValueB" )
	{
		m_nMaxValue[2] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetInitValueR" )
	{
		m_nInitValue[0] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetInitValueG" )
	{
		m_nInitValue[1] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetInitValueB" )
	{
		m_nInitValue[2] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTargetR" )
	{
		m_nTarget[0] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTargetG" )
	{
		m_nTarget[1] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTargetB" )
	{
		m_nTarget[2] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTargetRangeR" )
	{
		m_nTargetRange[0] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTargetRangeG" )
	{
		m_nTargetRange[1] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTargetRangeB" )
	{
		m_nTargetRange[2] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}

	//▼1.0.0.1020
	else
	if( szFunction=="SetMaxValueW" )
	{
		m_nMaxValue[3] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetInitValueW" )
	{
		m_nInitValue[3] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTargetW" )
	{
		m_nTarget[3] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	else
	if( szFunction=="SetTargetRangeW" )
	{
		m_nTargetRange[3] = _tcstol(szArgument,NULL,0);
		bReval = TRUE;
	}
	//▲1.0.0.1020

	//▼1.0.0.1023
	else
	if( szFunction=="InfoFileName" )
	{
		m_szInfoFileName = szArgument;
		bReval = TRUE;
	}
	else
	if( szFunction=="SaveFileExecute" )
	{
		bReval = SaveFileExecute();
	}
	else
	if( szFunction=="LoadFileExecute" )
	{
		bReval = LoadFileExecute();
	}
	//▲1.0.0.1023

	return bReval;

}

BOOL StLightControl::Execute(void)
{

	//▼1.0.0.1017
	//BOOL bReval = SendInitValue();
	BOOL bReval = FALSE;
	if( m_pSerialComm )
	{
		bReval = SendInitValue();
	}
	else
	{
		bReval = SetInitValueTerminal();
	}
	//▲1.0.0.1017
	if( bReval )
	{
		bReval = LightAdjustExecute();
	}
	return bReval;
}

BOOL StLightControl::SetShadeGo(INT nColor)
{
	//▼1.0.0.1017
	if( m_pSerialComm==NULL ) return FALSE;
	//▲1.0.0.1017
	BOOL bReval = FALSE;
	INT nShadeGo = 3 - nColor;
	CString szSend;
	szSend.Format(_T("shd_go=%d"),nShadeGo);	//照明を可変する色設定
	bReval = m_pSerialComm->SendData(szSend);

	DisplayListBox(szSend);

	return bReval;
}

BOOL StLightControl::SetShadeValue(INT nValue)
{
	BOOL bReval = FALSE;
	CString szSend;
	szSend.Format(_T("shd_ad0=%d"),nValue&0xff);	//値設定
	bReval = m_pSerialComm->SendData(szSend);
	DisplayListBox(szSend);
	if( bReval )
	{
		szSend.Format(_T("shd_ad1=%d"),(nValue>>8)&3);	//値設定
		bReval = m_pSerialComm->SendData(szSend);
		DisplayListBox(szSend);
	}
	if( bReval )
	{
		Sleep(100);
		szSend.Format(_T("shd_set"));	//値設定
		bReval = m_pSerialComm->SendData(szSend);	//確定
		DisplayListBox(szSend);
	}
	return bReval;
}

//▼1.0.0.1017
BOOL StLightControl::SetLightControlTerminal(INT nColor, INT nValue)
{
	if( !m_pTerminal ) return FALSE;
	BOOL bReval = FALSE;


	//▼1.0.0.1021

	//TCHAR szCommand[64];
	////▼1.0.0.1020
	////_stprintf_s( szCommand, _countof(szCommand),_T("ch%dval=%d"), nColor+1,nValue );
	//_stprintf_s( szCommand, _countof(szCommand),_T("ch%dval=%d"), m_vChannelIndex[nColor]+1,nValue );
	////▲1.0.0.1020
	//bReval = m_pTerminal->function(_T("Command"), szCommand);
	//DisplayListBox(szCommand);

	LPCTSTR szCommand[] = {_T("SetR"),_T("SetG"),_T("SetB"),_T("SetW")};
	if( nColor>=0 && nColor<_countof(szCommand) )
	{
		TCHAR szValue[64];
		_stprintf_s(szValue,_countof(szValue), _T("%i"),nValue);
		bReval = m_pTerminal->function(szCommand[nColor], szValue);
		TCHAR szText[64];
		_stprintf_s(szText,_countof(szText), _T("%s=%i"),szCommand[nColor],nValue);
		DisplayListBox(szText);
	}

	//▲1.0.0.1021
	return bReval;
}

BOOL StLightControl::SetInitValueTerminal(void)
{
	BOOL bReval = FALSE;
	if( !m_pTerminal ) return FALSE;
	INT nShade = 0;
	INT nColorLength = m_szColor.GetLength();
	//if( nColorLength==0 ) return FALSE;

	//▼1.0.0.1021
	//▼1.0.0.1023
	//LPCTSTR szColor[_COLORCOUNT] = {_T("R"),_T("G"),_T("B"),_T("W")};
	//▲1.0.0.1023
	//▲1.0.0.1021
	for( int i=0; i<nColorLength; i++ )
	{
		//▼1.0.0.1021
		////▼1.0.0.1020
		////if( m_szColor.Mid(i,1).Compare(_T("R"))==0 ) nShade |= 1;
		////else if( m_szColor.Mid(i,1).Compare(_T("G"))==0 ) nShade |= 2;
		////else if( m_szColor.Mid(i,1).Compare(_T("B"))==0 ) nShade |= 4;
		//if( m_szColor.Mid(i,1).Compare(_T("R"))==0 ) nShade |= (1<<m_vChannelIndex[0]);
		//else if( m_szColor.Mid(i,1).Compare(_T("G"))==0 ) nShade |= (1<<m_vChannelIndex[1]);
		//else if( m_szColor.Mid(i,1).Compare(_T("B"))==0 ) nShade |= (1<<m_vChannelIndex[2]);
		//else if( m_szColor.Mid(i,1).Compare(_T("W"))==0 ) nShade |= (1<<m_vChannelIndex[3]);
		////▲1.0.0.1020

		//▼1.0.0.1023
		//for( int j=0; j<_countof(szColor); j++ )
		for( int j=0; j<_countof(szColorName); j++ )
		{
			//if( m_szColor.Mid(i,1).Compare(szColor[j])==0 )
			if( m_szColor.Mid(i,1).Compare(szColorName[j])==0 )
		//▲1.0.0.1023
			{
				nShade |= (1<<j);
				break;
			}
		}
		//▲1.0.0.1021

	}
	CString szCommand(_T(""));
	CString szTmp;
	//▼1.0.0.1021
	////▼1.0.0.1020
	////for( int i=0; i<3; i++ )
	//for( int i=0; i<4; i++ )
	////▲1.0.0.1020
	//▼1.0.0.1023
	//for( int i=0; i<_countof(szColor); i++ )
	for( int i=0; i<_countof(szColorName); i++ )
	//▲1.0.0.1023
	//▲1.0.0.1021
	{
		//▼1.0.0.1021
		//if( szCommand.GetLength() )
		//{
		//	szCommand += _T("&");
		//}
		//szTmp.Format(_T("ch%donoff=%d"), i+1,((nShade>>i)&1) );
		//szCommand += szTmp;
		TCHAR szValue[32], szColorCommand[32];
		_stprintf_s( szValue, _countof(szValue), _T("%i"), ((nShade>>i)&1));
		//▼1.0.0.1023
		//_stprintf_s( szColorCommand, _countof(szColorCommand), _T("OnOff%s"), szColor[i]);
		_stprintf_s( szColorCommand, _countof(szColorCommand), _T("OnOff%s"), szColorName[i]);
		//▲1.0.0.1023
		m_pTerminal->function(szColorCommand,szValue);
		//▲1.0.0.1021
	}

	//初期値設定
	//▼1.0.0.1021
	////▼1.0.0.1020
	////for( int i=0; i<3; i++ )
	//for( int i=0; i<4; i++ )
	////▲1.0.0.1020
	//▼1.0.0.1023
	//for( int i=0; i<_countof(szColor); i++ )
	for( int i=0; i<_countof(szColorName); i++ )
	//▲1.0.0.1023
	//▲1.0.0.1021
	{
		m_dblResultAverage[i] = -1.0;
		m_nResult[i] = -1;

		if( ((nShade>>i)&1)==0 )
		{
			continue;
		}

		//▼1.0.0.1021
		//if( szCommand.GetLength() )
		//{
		//	szCommand += _T("&");
		//}

		////▼1.0.0.1020
		////szTmp.Format(_T("ch%dval=%d"), i+1, m_nInitValue[i] );
		//INT nIndex = 0;
		//for( int j=0; j<m_vChannelIndex.size(); j++ )
		//{
		//	if( i==m_vChannelIndex[j] )
		//	{
		//		nIndex = j;
		//		break;
		//	}
		//}
		//szTmp.Format(_T("ch%dval=%d"), i+1, m_nInitValue[nIndex] );
		////▲1.0.0.1020
		//szCommand += szTmp;

		TCHAR szValue[32];
		_stprintf_s( szValue, _countof(szValue), _T("%i"), m_nInitValue[i]);
		//▼1.0.0.1023
		m_pTerminal->function(szColorName[i],szValue);
		//▲1.0.0.1023
		//▲1.0.0.1021

	}
	//▼1.0.0.1021
	//bReval = m_pTerminal->function(_T("Command"), szCommand);
	//DisplayListBox(szCommand);
	CString szExecute(_T("Execute"));
	bReval = m_pTerminal->function(szExecute,_T(""));
	DisplayListBox(szExecute);
	//▲1.0.0.1021

	return bReval;
}
//▲1.0.0.1017

//▼1.0.0.1023
BOOL StLightControl::SetValueTerminal(CString szColor,PINT pnValue)
{
	BOOL bReval = FALSE;
	if( !m_pTerminal ) return FALSE;
	INT nShade = 0;
	INT nColorLength = szColor.GetLength();

	for( int i=0; i<nColorLength; i++ )
	{
		for( int j=0; j<_countof(szColorName); j++ )
		{
			if( szColor.Mid(i,1).Compare(szColorName[j])==0 )
			{
				nShade |= (1<<j);
				break;
			}
		}
	}
	CString szCommand(_T(""));
	CString szTmp;
	for( int i=0; i<_countof(szColorName); i++ )
	{
		TCHAR szValue[32], szColorCommand[32];
		_stprintf_s( szValue, _countof(szValue), _T("%i"), ((nShade>>i)&1));
		_stprintf_s( szColorCommand, _countof(szColorCommand), _T("OnOff%s"), szColorName[i]);
		m_pTerminal->function(szColorCommand,szValue);
	}

	for( int i=0; i<_countof(szColorName); i++ )
	{
		if( ((nShade>>i)&1)==0 )
		{
			continue;
		}
		TCHAR szValue[32];
		_stprintf_s( szValue, _countof(szValue), _T("%i"), pnValue[i]);
		m_pTerminal->function(szColorName[i],szValue);
	}
	CString szExecute(_T("Execute"));
	bReval = m_pTerminal->function(szExecute,_T(""));
	DisplayListBox(szExecute);

	return bReval;
}
//▲1.0.0.1023

BOOL StLightControl::SendInitValue(void)
{
	BOOL bReval = FALSE;
	if( !m_pSerialComm ) return FALSE;
	INT nShade = 0;
	INT nColorLength = m_szColor.GetLength();
	if( nColorLength==0 ) return FALSE;
	for( int i=0; i<nColorLength; i++ )
	{
		//▼1.0.0.1023
		//if( m_szColor.Mid(i,1).Compare(_T("R"))==0 ) nShade |= 1;
		//else if( m_szColor.Mid(i,1).Compare(_T("G"))==0 ) nShade |= 2;
		//else if( m_szColor.Mid(i,1).Compare(_T("B"))==0 ) nShade |= 4;
		for( int j=0; j<_countof(szColorName); j++ )
		{
			if( m_szColor.Mid(i,1).Compare(szColorName[j])==0 )
			{
				nShade |= (1<<j);
				break;
			}
		}
		//▲1.0.0.1023
	}
	if( nShade==0 ) return FALSE;
	CString szSend;
	szSend.Format(_T("shade=%d"),nShade);	//照明をつける色設定
	bReval = m_pSerialComm->SendData(szSend);
	DisplayListBox(szSend);
	if( !bReval ) return FALSE;

	//初期値設定
	for( int i=0; i<3; i++ )
	{
		m_dblResultAverage[i] = -1.0;
		m_nResult[i] = -1;

		if( ((nShade>>i)&1)==0 )
		{
			continue;
		}
		bReval = SetShadeGo(i);
		if( !bReval ) return FALSE;
		bReval = SetShadeValue(m_nInitValue[i]);
		if( !bReval ) return FALSE;
	}

	return bReval;
}


//▼1.0.0.1023
BOOL StLightControl::SendValue(CString szColor,PINT pnValue)
{
	BOOL bReval = FALSE;
	if( !m_pSerialComm ) return FALSE;
	INT nShade = 0;
	INT nColorLength = m_szColor.GetLength();
	if( nColorLength==0 ) return FALSE;
	for( int i=0; i<nColorLength; i++ )
	{
		for( int j=0; j<_countof(szColorName); j++ )
		{
			if( m_szColor.Mid(i,1).Compare(szColorName[j])==0 )
			{
				nShade |= (1<<i);
				break;
			}
		}
	}
	if( nShade==0 ) return FALSE;
	CString szSend;
	szSend.Format(_T("shade=%d"),nShade);	//照明をつける色設定
	bReval = m_pSerialComm->SendData(szSend);
	DisplayListBox(szSend);
	if( !bReval ) return FALSE;

	for( int i=0; i<3; i++ )	//ラインセンサー治具は３チャンネル
	{
		if( ((nShade>>i)&1)==0 )
		{
			continue;
		}
		bReval = SetShadeGo(i);
		if( !bReval ) return FALSE;
		bReval = SetShadeValue(pnValue[i]);
		if( !bReval ) return FALSE;
	}

	return bReval;
}
//▲1.0.0.1023



void StLightControl::DisplayListBox(CString szMessge)
{
	if( !m_pParent ) return;
	StInspectMain *aParent = (StInspectMain *)m_pParent;
	aParent->DisplayListBox(szMessge);
}

typedef struct _LightControlResultList
{
	double dblAverage;
	double dblDifference;
	INT nSetting;
	//INT iResult;
}LightControlResultList, *PLightControlResultList;

BOOL StLightControl::LightAdjustExecute(void)
{

	BOOL bReval = TRUE;
	m_iJudge = 2;	//2:処理中

	INT nShade = 0;
	INT nColorLength = m_szColor.GetLength();
	if( nColorLength==0 ) return FALSE;

	INT nGo = 0;

	for( int i=0; i<nColorLength && bReval; i++ )	//
	{
		//▼1.0.0.1023
		//if( m_szColor.Mid(i,1).Compare(_T("R"))==0 ) nGo = 0;
		//else if( m_szColor.Mid(i,1).Compare(_T("G"))==0 ) nGo = 1;
		//else if( m_szColor.Mid(i,1).Compare(_T("B"))==0 ) nGo = 2;
		////▼1.0.0.1020
		//else if( m_szColor.Mid(i,1).Compare(_T("W"))==0 ) nGo = 3;
		////▲1.0.0.1020
		for( int j=0; j<_countof(szColorName); j++ )
		{
			if( m_szColor.Mid(i,1).Compare(szColorName[j])==0 )
			{
				nGo = j;
				break;
			}
		}
		//▲1.0.0.1023

		//▼1.0.0.1017
		//SetShadeGo(nGo);
		if( m_pSerialComm )
			SetShadeGo(nGo);
		//▲1.0.0.1017

		//LightAdjustColor(nGo);
		//▼1.0.0.1043
		//INT nSetValue = m_nMaxValue[nGo];
		double dblSetValue = m_nMaxValue[nGo];
		//▲1.0.0.1043
		double dblCheckAverage = 0.0;
		//▼1.0.0.1043
		//INT nBeforeValue = 0;
		double dblBeforeValue = 0.0;
		//▲1.0.0.1043

		std::vector<LightControlResultList> vResultList;

		do
		{
			//▼1.0.0.1067
			DoEvents();
			//▲1.0.0.1067

			//設定
			//▼1.0.0.1017
			//SetShadeValue(nSetValue);
			if( m_pSerialComm )
				//▼1.0.0.1043
				//SetShadeValue(nSetValue);
				SetShadeValue((INT)(dblSetValue+0.5));
				//▲1.0.0.1043
			else
				//▼1.0.0.1043
				//SetLightControlTerminal(nGo, nSetValue);
				SetLightControlTerminal(nGo, (INT)(dblSetValue+0.5));
				//▲1.0.0.1043
			//▲1.0.0.1017

			//待ち時間
			Sleep(m_nTimer);

			//画像取得
			if( m_Buffer ) delete m_Buffer;
			m_Buffer = new StBuffer;
			bReval = m_DeviceBase->GetStBuffer(m_Buffer);	//変換して取得もありかも。現時点ではコピー
			//平均値取得
			double dblResultAverage[4];
			if( bReval )
			{
				bReval = GetSNAverage( m_Buffer, m_nStart, m_nEnd, dblResultAverage, NULL, NULL, NULL );
			}

			if( bReval )
			{
				dblCheckAverage = dblResultAverage[2-nGo];
				INT nElement = GetElementFromBuffer(m_Buffer);
				if( nElement==1 )	dblCheckAverage = dblResultAverage[0];	//Mono

				LightControlResultList aResult;
				aResult.dblAverage = dblCheckAverage;
				//▼1.0.0.1043
				//aResult.nSetting = nSetValue;
				aResult.nSetting = (INT)(dblSetValue+0.5);
				//▲1.0.0.1043
				aResult.dblDifference = fabs((double)m_nTarget[nGo]-dblCheckAverage);
				vResultList.push_back(aResult);

#ifdef _DEBUG
				CString szText;
				szText.Format(_T(" Average=%f"), dblCheckAverage );
				DisplayListBox(szText);
#endif

				//Check
				//▼1.0.0.1043
				//INT nNextValue = nSetValue;
				//if( (double)m_nTarget[nGo]<dblCheckAverage )
				//{
				//	nNextValue -= abs(nSetValue - nBeforeValue)/2;
				//}
				//else
				//if( (double)m_nTarget[nGo]>dblCheckAverage )
				//{
				//	nNextValue += abs(nSetValue - nBeforeValue)/2;
				//}

				////▼1.0.0.1021
				//if( nNextValue>m_nMaxValue[nGo] )
				//{
				//	nNextValue=m_nMaxValue[nGo];
				//}
				////▲1.0.0.1021

				//nBeforeValue = nSetValue;
				//nSetValue = nNextValue;
				//if( nBeforeValue==nSetValue )
				//	break;	//終了



				INT dblNextValue = dblSetValue;
				if( (double)m_nTarget[nGo]<dblCheckAverage )
				{
					dblNextValue -= fabs(dblSetValue - dblBeforeValue)/2;
				}
				else
				if( (double)m_nTarget[nGo]>dblCheckAverage )
				{
					dblNextValue += fabs(dblSetValue - dblBeforeValue)/2;
				}

				if( dblNextValue>m_nMaxValue[nGo] )
				{
					dblNextValue=(double)m_nMaxValue[nGo];
				}

				dblBeforeValue = dblSetValue;
				dblSetValue = dblNextValue;

				if( (INT)(dblBeforeValue+0.5)==(INT)(dblSetValue+0.5) )
					break;	//終了
				//▲1.0.0.1043
			}

		}while(bReval);

		if( bReval )
		{
			double dblBestDifference = 9999.0;
			INT nBestIndex = -1;
			//vResultListリストから一番いい数値を求める
			for( int j=0; j<(int)vResultList.size(); j++ )	//
			{
				if( vResultList[j].dblDifference<dblBestDifference )
				{
					dblBestDifference = vResultList[j].dblDifference;
					nBestIndex = j;
				}
			}

			m_dblResultAverage[nGo] = vResultList[nBestIndex].dblAverage;
			m_nResult[nGo] = vResultList[nBestIndex].nSetting;

			//▼1.0.0.1017
			//bReval = SetShadeValue(vResultList[nBestIndex].nSetting);
			if( m_pSerialComm )
				bReval = SetShadeValue(vResultList[nBestIndex].nSetting);
			else
				bReval = SetLightControlTerminal(nGo, vResultList[nBestIndex].nSetting);
			//▲1.0.0.1017


			if( dblBestDifference>(double)m_nTargetRange[nGo] )
			{
				m_iJudge = 0;
			}

		}
		vResultList.clear();
		if( m_iJudge==0 ) break;
	}

	if( bReval )
	{
		if( m_iJudge == 2 ) m_iJudge = 1;
	}
	else	 m_iJudge = 0;

	return bReval;
}

//▼1.0.0.1023 .hから移動
INT StLightControl::GetResultValue(CString szColor)
{
	INT nGo=-1;
	for( int i=0; i<_countof(szColorName); i++ )
	{
		if( szColor.Compare(szColorName[i])==0 )
		{
			nGo = i;
			break;
		}
	}
	if( nGo<0 ) return -1;
	return m_nResult[nGo];
}

double StLightControl::GetResultAverage(CString szColor)
{
	INT nGo=-1;
	for( int i=0; i<_countof(szColorName); i++ )
	{
		if( szColor.Compare(szColorName[i])==0 )
		{
			nGo = i;
			break;
		}
	}
	if( nGo<0 ) return -1.0;
	return m_dblResultAverage[nGo];
}
//▲1.0.0.1023 .hから移動

//▼1.0.0.1023
BOOL StLightControl::SaveFileExecute(void)
{
	//▼1.0.0.1064
	//▼1.0.0.1061
	//if( !m_nScriptCheckMode ) return TRUE;
	//▲1.0.0.1061
	if( m_nScriptCheckMode ) return TRUE;
	//▲1.0.0.1064

	BOOL bReval = FALSE;
	TCHAR szFileName[MAX_PATH];
	TCHAR szSectionName[MAX_PATH];
	TCHAR szValue[MAX_PATH];

	//▼1.0.0.1024
	//_stprintf_s(szFileName,_countof(szFileName),_T(".\\%s"), m_szInfoFileName );
	_stprintf_s(szFileName,_countof(szFileName),_T("%s"), m_szInfoFileName );
	if( m_szInfoFileName.Find(_T(":\\"))<0 )
	{
		_stprintf_s(szFileName,_countof(szFileName),_T(".\\%s"), m_szInfoFileName );
	}
	//▲1.0.0.1024

	_tcscpy_s(szSectionName,_countof(szSectionName),_T("LightInformation"));

	for( int i=0; i<_countof(m_nResult); i++ )
	{
		_stprintf_s(szValue,_countof(szValue),_T("%i"),m_nResult[i]);
		bReval = WritePrivateProfileString(szSectionName,szColorName[i],szValue,szFileName);
		if( !bReval ) break;
	}
	return bReval;
}

BOOL StLightControl::LoadFileExecute(void)
{
	//▼1.0.0.1064
	//▼1.0.0.1061
	//if( !m_nScriptCheckMode ) return TRUE;
	//▲1.0.0.1061
	if( m_nScriptCheckMode ) return TRUE;
	//▲1.0.0.1064


	BOOL bReval = FALSE;
	TCHAR szFileName[MAX_PATH];
	TCHAR szSectionName[MAX_PATH];
	TCHAR szValue[MAX_PATH];

	//▼1.0.0.1024
	//_stprintf_s(szFileName,_countof(szFileName),_T(".\\%s"), m_szInfoFileName );
	_stprintf_s(szFileName,_countof(szFileName),_T("%s"), m_szInfoFileName );
	if( m_szInfoFileName.Find(_T(":\\"))<0 )
	{
		_stprintf_s(szFileName,_countof(szFileName),_T(".\\%s"), m_szInfoFileName );
	}
	//▲1.0.0.1024
	_tcscpy_s(szSectionName,_countof(szSectionName),_T("LightInformation"));

	for( int i=0; i<_countof(m_nResult); i++ )
	{
		m_nResult[i] = GetPrivateProfileInt(szSectionName,szColorName[i],-1,szFileName);
	}
	//設定
	if( m_pSerialComm )
	{
		CString szColor("");
		INT nValue[3];
		for( int i=0; i<_countof(nValue); i++ )
		{
			if( m_nResult[i]<0 )
			{
				nValue[i] = 0;
			}
			else
			{
				szColor += szColorName[i];
				nValue[i] = m_nResult[i];
			}
		}
		bReval = SendValue(szColor,nValue);
		
		//▼1.0.0.1024
		m_nLogMode = _countof(nValue);
		//▲1.0.0.1024
	}
	else
	{
		CString szColor("");
		INT nValue[4];
		for( int i=0; i<_countof(nValue); i++ )
		{
			if( m_nResult[i]<0 )
			{
				nValue[i] = 0;
			}
			else
			{
				szColor += szColorName[i];
				nValue[i] = m_nResult[i];
			}
		}
		bReval = SetValueTerminal(szColor,nValue);
		//▼1.0.0.1024
		m_nLogMode = _countof(nValue);
		//▲1.0.0.1024

	}

	return bReval;
}
//▲1.0.0.1023

//▼1.0.0.1024
BOOL StLightControl::GetLightResultString(INT nIndex, CString &szText)
{
	BOOL bReval = FALSE;
	if( nIndex<_countof(szColorName) && nIndex<_countof(m_nResult) )
	{
		szText.Format(_T("%s=%i"), szColorName[nIndex], m_nResult[nIndex] );
		bReval = TRUE;
	}
	return bReval;
}
//▲1.0.0.1024

//▼1.0.0.1025
BOOL StLightControl::GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue)
{
	BOOL bReval = FALSE;
	CString szFunction(szFunc);
	if( szFunction=="GetValue" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_COLORCOUNT )
		{
			dblValue = m_nResult[nIndex];
			bReval = TRUE;
		}
	}
	else
	if( szFunction=="GetAverage" )
	{
		INT nIndex = _ttoi(szArg);
		if( nIndex>=0 && nIndex<_COLORCOUNT )
		{
			dblValue = m_dblResultAverage[nIndex];
			bReval = TRUE;
		}
	}
	return bReval;
}
//▲1.0.0.1025



