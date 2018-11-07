#pragma once
#include "stcheckbase.h"

#define MAX_CH 2

typedef struct _ST_OFFSET_RESULT
{
	WORD		wChannel;		//�`�����l��
	int			iMin;			//�ŏ��l
	int			iResult;		//�I�t�Z�b�g�l
}S_ST_OFFSET_RESULT, *PS_ST_OFFSET_RESULT;

typedef struct _ST_OFFSET_DATA
{
	INT		iSetData;		//�`�����l��
	int		iSaveData[2];		//�ŏ��l
	int		iSaveOffset[2];		//�I�t�Z�b�g�l
}S_ST_OFFSET_DATA, *PS_ST_OFFSET_DATA;

class StCheckAdjustOffset :
	public StCheckBase
{
public:
	StCheckAdjustOffset(void);
	StCheckAdjustOffset(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void* pParent=NULL);
	~StCheckAdjustOffset(void);

	BOOL Execute(void);

	BOOL function(LPCTSTR szFunc, LPCTSTR szArgument);
	BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue);
private:
	void Initialize(void);
	void Delete(void);
	void DisplayListBox(CString szMessge);
	void* m_pParent;

	BOOL OffsetInit(void);

	BOOL GetResultAndSetValue(void);

	BOOL OffsetSetValue(void);
	//BOOL OffsetGetValue(void);
	//BOOL OffsetGetResult(void);
	BOOL CheckOffset( int data, int offset, int *pData, int *pOffset, int *pNextOffset );
	//BOOL SendData(LPCTSTR szCommand);
	BOOL SendOffsetData(INT nPage, INT nData);
	BOOL SendTrigger(void);

	//�l
	INT m_nStartValue;
	INT m_nMaxValue;
	INT m_nChannelCount;
	CString m_szCMDL;
	CString m_szCMDH;
	INT m_nLBIT;
	INT m_nTimer;
	INT m_nStartLine;
	INT m_nEndLine;
	INT m_nBase;
	INT m_nInc;
	INT m_nDiffValue;
	//��1.0.0.1047
	//INT m_nRange;
	//��1.0.0.1047
	INT m_nOffsetAddress[2][16];	//0:Low 1:High

	CString m_szTrigger;
	INT m_nTriggerAddress;
	INT m_nTriggerBit;

	INT m_nGetCount;


	//�擾�l
	//INT m_nGetMin[2];

	INT m_nCtrl;	//0:Ascii 1:Bynary

	//��1.0.0.1067b
	INT m_nBunch;	//1�ȏ�
	INT m_nEvenOddReverseMode;	//0:�ʏ� 1:Reverse
	//��1.0.0.1067b

	PS_ST_OFFSET_RESULT m_pOffsetResult;
	PS_ST_OFFSET_DATA m_pOffsetData;



};

