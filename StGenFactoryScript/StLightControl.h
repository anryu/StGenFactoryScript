#pragma once
#include "stcheckbase.h"
#include "StSerialComm.h"
#include <vector>
//��1.0.0.1017
#include "StTerminal.h"
//��1.0.0.1017

//��1.0.0.1023
#define _COLORCOUNT 4
//��1.0.0.1023


class StLightControl :	public StCheckBase
{
public:
	StLightControl(void);
	//StLightControl(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog=NULL);
	StLightControl(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void* pParent=NULL, void *aDialog=NULL);
	~StLightControl(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	//��1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);
	//��1.0.0.1025

	BOOL Execute(void);

	void SetSerialComm(StSerialComm *pSerialComm)
	{
		m_pSerialComm = pSerialComm;
	}
	//��1.0.0.1017
	void SetTerminal(StTerminal *pTerminal)
	{
		m_pTerminal = pTerminal;
	}
	BOOL SetLightControlTerminal(INT nColor, INT nValue);
	BOOL SetInitValueTerminal(void);
	BOOL LightAdjustTerminalExecute(void);
	//��1.0.0.1017

	BOOL LightAdjustExecute(void);

	BOOL SendInitValue(void);
	BOOL SetShadeGo(INT nColor);
	BOOL SetShadeValue(INT nValue);

	BOOL GetColorIndex(INT nIndex, CString &szColor)
	{
		if( nIndex >= m_szColor.GetLength() )
			return FALSE;
		szColor = m_szColor.Mid(nIndex,1);
		return TRUE;
	}
	//��1.0.0.1023 .h����ړ�
	//INT GetResultValue(CString szColor)
	//{
	//	INT nGo=-1;
	//	if( szColor.Compare(_T("R"))==0 ) nGo = 0;
	//	else if( szColor.Compare(_T("G"))==0 ) nGo = 1;
	//	else if( szColor.Compare(_T("B"))==0 ) nGo = 2;
	//	//��1.0.0.1021
	//	else if( szColor.Compare(_T("W"))==0 ) nGo = 3;
	//	//��1.0.0.1021
	//	if( nGo<0 ) return -1;
	//	return m_nResult[nGo];
	//}

	//double GetResultAverage(CString szColor)
	//{
	//	INT nGo=-1;
	//	if( szColor.Compare(_T("R"))==0 ) nGo = 0;
	//	else if( szColor.Compare(_T("G"))==0 ) nGo = 1;
	//	else if( szColor.Compare(_T("B"))==0 ) nGo = 2;
	//	//��1.0.0.1021
	//	else if( szColor.Compare(_T("W"))==0 ) nGo = 3;
	//	//��1.0.0.1021
	//	if( nGo<0 ) return -1.0;
	//	return m_dblResultAverage[nGo];
	//}
	INT GetResultValue(CString szColor);
	double GetResultAverage(CString szColor);
	//��1.0.0.1023 .h����ړ�

	//��1.0.0.1021
	//��1.0.0.1020
	//void ClearLightChannel(void)
	//{
	//	m_vChannelName.clear();
	//	m_vChannelIndex.clear();
	//}
	//void AddLightChannel(LPCTSTR szChannel, INT nIndex )
	//{
	//	CString szString = szChannel;
	//	m_vChannelName.push_back(szString);
	//	m_vChannelIndex.push_back(nIndex);
	//}
	//��1.0.0.1020
	//��1.0.0.1021

	//��1.0.0.1024
	INT GetLogMode(void){return m_nLogMode;}
	void SetLogMode(INT nData){m_nLogMode = nData;}
	BOOL GetLightResultString( INT nIndex, CString &szText);
	//��1.0.0.1024

private:
	void Initialize(void);
	void DisplayListBox(CString szMessge);
	void* m_pParent;
	//�p�����[�^
	StSerialComm *m_pSerialComm;
	//��1.0.0.1017
	StTerminal *m_pTerminal;
	//��1.0.0.1017

	//��1.0.0.1021
	//��1.0.0.1020
	//std::vector<CString> m_vChannelName;
	//std::vector<INT> m_vChannelIndex;
	//��1.0.0.1020
	//��1.0.0.1021

	INT m_nStart;
	INT m_nEnd;
	INT m_nTimer;
	CString m_szColor;

	//��1.0.0.1020
	//INT m_nMaxValue[3];	//RGB��
	//INT m_nInitValue[3];
	//INT m_nTarget[3];
	//INT m_nTargetRange[3];
	//��1.0.0.1023
	//INT m_nMaxValue[4];	//RGB��
	//INT m_nInitValue[4];
	//INT m_nTarget[4];
	//INT m_nTargetRange[4];
	INT m_nMaxValue[_COLORCOUNT];	//RGB��
	INT m_nInitValue[_COLORCOUNT];
	INT m_nTarget[_COLORCOUNT];
	INT m_nTargetRange[_COLORCOUNT];
	//��1.0.0.1023
	//��1.0.0.1020

	//Result
	//��1.0.0.1023
	//INT m_nResult[3];
	//double m_dblResultAverage[3];
	INT m_nResult[_COLORCOUNT];
	double m_dblResultAverage[_COLORCOUNT];
	//��1.0.0.1023

	//��1.0.0.1023
	BOOL SendValue(CString szColor,PINT pnValue);
	BOOL SetValueTerminal(CString szColor,PINT pnValue);

	CString m_szInfoFileName;
	BOOL SaveFileExecute(void);
	BOOL LoadFileExecute(void);
	//��1.0.0.1023
	//��1.0.0.1024
	INT m_nLogMode;
	//��1.0.0.1024

	//����Thread
	HANDLE m_hThread;
	unsigned int m_mThreadID;
	BOOL m_bThreadStop;

	//Display
	//void DisplayBufferListBox(void);
	//HANDLE hDisplayMutex;
	//std::vector<CString> m_vDisplayString;
	//void DisplayListBox(CString szMessge);



};

