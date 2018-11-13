#pragma once
#include "../CommonHeader/StString.h"
#include "StControl.h"
#include "StStruct.h"

class StTerminal : public StControl

{
public:
	StTerminal(void);
	StTerminal( LPCTSTR szClassName, LPCTSTR szVarName, LPCTSTR szAddress=NULL );
	~StTerminal(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	//Å•1.0.0.1021
	void SetStructLightChannel(CStructLightChannel *pStructLightChannel){m_pStructLightChannel = pStructLightChannel;}
	//Å£1.0.0.1021
	//Å•1.0.0.1053
	BOOL GetAllport(INT &nData);
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArgument, INT &nData);
	BOOL ExecutePublic(LPCTSTR szFunc, LPCTSTR szArgument);
	//Å£1.0.0.1053
	//Å•1.3.0.70
	BOOL GetLightStatus(const char *szID, INT &nData);
	//Å£1.0.0.70
	//Å•1.0.0.1073 beta2
	StString GetErrorMessage(void){return m_szLastErrorMessage;}
	//Å£1.0.0.1073 beta2

private:
	StString m_Address;

	void Initialize(void);
	BOOL Execute(LPCTSTR szFunc=NULL, LPCTSTR szArgument=NULL);


	//Å•1.0.0.1021
	CStructLightChannel *m_pStructLightChannel;
	BOOL GetCommand(LPCTSTR szID, CString &szCommand,LPCTSTR szArgument);
	//Å£1.0.0.1021

	//Data-----------------
	INT m_nCurrentR;
	INT m_nCurrentG;
	INT m_nCurrentB;
	INT m_nCurrentOnOffR;
	INT m_nCurrentOnOffG;
	INT m_nCurrentOnOffB;

	INT m_nR;
	INT m_nG;
	INT m_nB;
	INT m_nOnOffR;
	INT m_nOnOffG;
	INT m_nOnOffB;
	//Å•1.0.0.1021
	INT m_nCurrentW;
	INT m_nCurrentOnOffW;
	INT m_nW;
	INT m_nOnOffW;
	//Å£1.0.0.1021
	//Å•1.0.0.1053
	INT m_nIOPortCount;
	//Å£1.0.0.1053

	//Å•1.0.0.1073 beta2
	StString m_szLastErrorMessage;
	//Å£1.0.0.1073 beta2
};

