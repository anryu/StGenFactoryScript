#pragma once
#include "stcheckbase.h"
#include <vector>


typedef struct _S_PTOUCH_TABLE
{
	CString szID;
	CString szText;
}S_PTOUCH_TABLE, *PS_PTOUCH_TABLE;


class StPTouch :
	public StCheckBase
{
public:
	StPTouch(void);
	StPTouch(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog=NULL);
	~StPTouch(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
 
	BOOL Execute(void);

	void SetDeviceModelName(LPCTSTR szText){m_szDeviceModelName=szText;}
	void SetHinmokuCode(LPCTSTR szText){m_szHinmokuCode=szText;}
	void SetSerialNumber(LPCTSTR szText){m_szSerialNumber=szText;}

	//---------------------------------------------
	INT GetConfirmMessage(void){return m_nConfirmMessage;}
	void GetLabel(CString &szLabel){szLabel = m_szLabel;}
	INT GetIDCount(void){return m_vIDTable.size();}

	BOOL GetID(INT nIndex, CString &szID);
	BOOL GetText(CString szID, CString &szText);
	//---------------------------------------------


private:
	void Initialize(void);
	CString m_szDeviceModelName;
	CString m_szHinmokuCode;	
	CString m_szSerialNumber;
	CString m_szLabel;
	INT m_nConfirmMessage;
	std::vector<S_PTOUCH_TABLE> m_vIDTable;

	BOOL GetTextFromText(CString szInText, CString &szOutText);

	//Å•1.0.0.1047
	CString m_szHinmokuFileName;
	BOOL GetModelNameFromHinmokuFile( CString &szModelName );

	BOOL GetHinmokuCode2(CString &szHinmokuCode);
	BOOL GetSerialNumber2(CString &szSerialNumber);
	BOOL GetDeviceModelName2(CString &szDeviceModelName);

	//Å£1.0.0.1047

};

