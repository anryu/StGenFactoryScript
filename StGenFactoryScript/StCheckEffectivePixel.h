#pragma once
#include "stcheckbase.h"
class StCheckEffectivePixel :
	public StCheckBase
{
public:
	StCheckEffectivePixel(void);
	StCheckEffectivePixel(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog=NULL);
	~StCheckEffectivePixel(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
  	//��1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);
	//��1.0.0.1025

	BOOL Execute(void);

	//---�p�����[�^�ݒ�-------------------------------
	void SetStart(INT nData);
	void SetEnd(INT nData);
	void SetLevel(INT nData);

	//---�K�i�ݒ�-------------------------------
	void SetEffectiveStart(INT nData);
	void SetEffectiveEnd(INT nData);

	//---���ʎ擾-------------------------------
	INT GetElement(void){return m_nElement;}
	INT GetResultEffectiveStart(INT nIndex){return m_nResultEffectiveStart[nIndex];}
	INT GetResultEffectiveEnd(INT nIndex){return m_nResultEffectiveEnd[nIndex];}

private:
	void Initialize(void);

	//�p�����[�^
	INT m_nStart;
	INT m_nEnd;
	INT m_nLevel;

	//�K�i�l-----
	INT m_nEffectiveStart;
	INT m_nEffectiveEnd;

	//����-----
	size_t m_nElement;

	//INT m_nColorIndex[4];	//0:R 1:G 2:B 3:A -1:Mono 
	INT m_nResultEffectiveStart[4];
	INT m_nResultEffectiveEnd[4];


};

