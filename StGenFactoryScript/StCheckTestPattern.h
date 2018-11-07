#pragma once
#include "stcheckbase.h"
class StCheckTestPattern :
	public StCheckBase
{
public:
	StCheckTestPattern(void);
	StCheckTestPattern(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog=NULL);
	~StCheckTestPattern(void);

	//---ãKäiê›íË-------------------------------
	void SetStart(INT nData){m_nStart = nData;}
	void SetEnd(INT nData){m_nEnd = nData;}
	void SetFileName(LPCTSTR szFineName){m_szFileName = szFineName;}

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
   	//Å•1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);
	//Å£1.0.0.1025

	BOOL Execute(void);

	INT GetX(void){return m_nResultX;}
	INT GetY(void){return m_nResultY;}

private:
	void Initialize(void);
	BOOL GetTestPatternData(void);
	BOOL CheckTestPattern(void);

	INT m_nStart;
	INT m_nEnd;

	CString m_szFileName;
	CString m_szFilePath;

	//Result
	INT m_nResultX;
	INT m_nResultY;

	//Buffer 
	PINT m_pImageBuffer;
	size_t m_nImageLineSize;

};

