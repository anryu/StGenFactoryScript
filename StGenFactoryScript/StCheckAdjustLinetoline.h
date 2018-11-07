#pragma once
#include "stcheckbase.h"
#include <vector>

class StCheckAdjustLinetoline :
	public StCheckBase
{
public:
	StCheckAdjustLinetoline(void);
	StCheckAdjustLinetoline(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void* pParent=NULL, void *aDialog=NULL);
	~StCheckAdjustLinetoline(void);

	BOOL Execute(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
 	//Å•1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);
	//Å£1.0.0.1025

	BOOL AdjustExecute(void);

	//åüç∏Thread
	BOOL GetThreadStop(void){return m_bThreadStop;}


	INT GetResultCount(void){return m_ResultCount;}
	INT GetStandardCount(void){return m_nAdjustCount;}

	//Å•1.0.0.1020 beta1
	double GetResultMinAverage(void){return m_ResultMinAverage;}
	INT GetResultValue(void){return (INT)m_SetValue;}
	INT GetResultLine(void){return (INT)m_ResultLine;}
	//Å£1.0.0.1020 beta1


private:
	void Initialize(void);
	//Å•1.0.0.1044
	//BOOL WriteDataEBus(BYTE byteDeviceCode, BYTE bytePage, BYTE byteAddress, BYTE byteData);
	////Å•1.0.0.1015
	//BOOL WriteDataMultiCamAscii(StString &szCommand, BYTE byteData);
	//BOOL WriteDataMultiCamBinary(BYTE byteDeviceCode, BYTE bytePage, BYTE byteAddress, BYTE byteData);
	////Å£1.0.0.1015
	//Å£1.0.0.1044

	//ÉpÉâÉÅÅ[É^
	INT m_nStart;
	INT m_nEnd;
	INT m_nTimer;

	INT m_nDeviceCode;
	INT m_nPage;
	INT m_nAddress;

	INT m_nInitValue;
	double m_dblMinAverage;
	double m_dblMaxAverage;
	INT m_nAdjustCount;

	BYTE m_SetValue;
	double m_ResultMinAverage;
	INT m_ResultLine;
	INT m_ResultElement;
	INT m_ResultCount;
	void *m_pParent;

	//Å•1.0.0.1015
	StString m_szCommand;
	//Å£1.0.0.1015


	//åüç∏Thread
	HANDLE m_hThread;
	unsigned int m_mThreadID;
	BOOL m_bThreadStop;

	std::vector<CString> m_vDisplayString;
	HANDLE hDisplayMutex;
	void DisplayBufferListBox(void);
	void DisplayListBox(CString szMessge);

};

