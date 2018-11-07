#pragma once
#include "stcheckbase.h"
class StCheckSN :
	public StCheckBase
{
public:
	StCheckSN(void);
	StCheckSN(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialog=NULL);
	~StCheckSN(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	//Å•1.0.0.1025
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg,double &dblValue);
	//Å£1.0.0.1025
	BOOL Execute(void);

	//---ÉpÉâÉÅÅ[É^ê›íË-------------------------------
	void SetStart(INT nData);
	void SetEnd(INT nData);

	//---ãKäiê›íË-------------------------------
	void SetAverageMin(double dblData);
	void SetAverageMax(double dblData);
	void SetSNMax(double dblData);
	void SetSN(double dblData);

	//---åãâ éÊìæ-------------------------------
	INT GetElement(void){return m_nElement;}
	double GetAverage(INT nIndex){return m_dblResultAverage[nIndex];}
	double GetSN(INT nIndex){return m_dblResultSN[nIndex];}
	double GetSNMax(INT nIndex){return m_dblResultSNMax[nIndex];}
	INT GetSDMPix(INT nIndex){return m_nResultSDMPix[nIndex];}
	//INT GetColorIndex(INT nIndex){return m_nColorIndex[nIndex];}
	//Å•1.0.0.1028
	double GetLineAverageMin(INT nIndex){return m_dblReslutLineAverageMin[nIndex];}
	double GetLineAverageMax(INT nIndex){return m_dblReslutLineAverageMax[nIndex];}
	//Å£1.0.0.1028

private:
	void Initialize(void);

	//ÉpÉâÉÅÅ[É^
	INT m_nStart;
	INT m_nEnd;

	//ãKäiíl-----
	double m_nPermAverageMin;
	double m_nPermAverageMax;
	
	//Å•1.0.0.1028
	double m_dblPermLineAverageMin;
	double m_dblPermLineAverageMax;
	//Å£1.0.0.1028

	//Å•1.0.0.1043
	double m_dblPermAverageColorMin[3];
	double m_dblPermAverageColorMax[3];

	double m_dblPermLineAverageColorMin[3];
	double m_dblPermLineAverageColorMax[3];
	//Å£1.0.0.1043


	double m_nPermSNMax;
	double m_nPermSN;

	//åãâ -----
	INT m_nElement;
	//INT m_nColorIndex[4];	//0:R 1:G 2:B 3:A -1:Mono 
	double m_dblResultAverage[4];
	double m_dblResultSN[4];
	double m_dblResultSNMax[4];
	INT m_nResultSDMPix[4];


	//Å•1.0.0.1028
	double m_dblReslutLineAverageMin[4];
	double m_dblReslutLineAverageMax[4];
	INT m_nResultLineAverageMinPos[4];
	INT m_nResultLineAverageMaxPos[4];
	//Å£1.0.0.1028

	BOOL m_bResult;
};

