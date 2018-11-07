#pragma once
#include "../CommonHeader/StString.h"
#include "../CommonHeader/StBuffer.h"
#include "StDeviceBase.h"


class StCheckBase
{
public:
	StCheckBase(void);
	StCheckBase(LPCTSTR szClassName, LPCTSTR szVarName, StDeviceBase *aDeviceBase, void *aDialogEx = NULL);
	virtual ~StCheckBase(void);

	virtual BOOL Execute(void) = 0;
	//��1.0.0.1025
	//virtual BOOL GetFunction(LPCTSTR szFunc, LPCTSTR szArg, double &dblValue) = 0;
	//��1.0.0.1025

	INT GetJudge(void){return m_iJudge;}

	//�N���X��
	StString m_ClassName;
	//�ϐ���
	StString m_VarName;

	//void SetDialogPtr(CDialogEx *pDlg){
	//	m_pDialog=pDlg;
	//}
	//CDialogEx * GetDialogPtr(void){return m_pDialog;}

	//�C���X�^���X�쐬���ɂ̓n���h�����킩��Ȃ����߂ɑ���ł��Ȃ��B
	//�C���X�^���X�쐬��ɃZ�b�g����K�v����
	void SetMainHandle(void *pHandle){m_pDialog = pHandle;}
	void *GetMainHandle(void){ return m_pDialog;}

	INT GetElementColorIndex(INT iPos);

	BOOL SaveBitmapFile(StBuffer *pBuffer, LPCTSTR szFileFullName);
	//��1.0.0.1039
	BOOL SaveImageFile(StBuffer *aBuffer, LPCTSTR szFileFullName);
	//��1.0.0.1039

	//BOOL SaveCSVFile(StBuffer *pBuffer, LPCTSTR szFileFullName);
	//BOOL SaveCSVFile(StBuffer *pBuffer, LPCTSTR szFilePath, LPCTSTR szFileName );
	BOOL SaveCSVFile(StBuffer *pBuffer, LPCTSTR szFilePath, LPCTSTR szFileName, INT nScopeLeft=-1, INT nScopeRight=-1 );
	//��1.0.0.1039
	BOOL SaveHistogramFile(StBuffer *pBuffer, LPCTSTR szFilePath, LPCTSTR szFileName, INT nScopeLeft=-1, INT nScopeRight=-1 );
	//��1.0.0.1039

	BOOL GetDeviceModelName(StString &szText)
	{
		if( !m_DeviceBase ) return FALSE;
		//szText = m_DeviceBase->mDeviceModelName;
		BOOL bReval = m_DeviceBase->GetDeviceModelName(szText);
		return bReval;
	}
	BOOL GetSerialNumber(StString &szText)
	{
		if( !m_DeviceBase ) return FALSE;
		//szText = m_DeviceBase->mSerialNumber;
		BOOL bReval = m_DeviceBase->GetSerialNumber(szText);
		return bReval;
	}
	//��1.0.0.1047
	BOOL GetHinmokuCode(StString &szText)
	{
		if( !m_DeviceBase ) return FALSE;
		return m_DeviceBase->GetHinmokuCode(szText);
	}
	//��1.0.0.1047

	//��1.0.0.1020
	StResult GetErrorCode(void){return m_nErrorCode;}
	//��1.0.0.1020

	//��1.0.0.1062
	void SetScriptCheckMode(INT nScriptCheckMode){m_nScriptCheckMode=nScriptCheckMode;}
	//��1.0.0.1062

protected:
	INT m_iJudge;	//-1:������ 0:NG 1:OK 2:������

	//��1.0.0.1020
	StResult m_nErrorCode;
	//��1.0.0.1020

	StBuffer *m_Buffer;
	StDeviceBase *m_DeviceBase;

	//���O

	//�摜����
	//��1.0.0.1028
	//BOOL GetSNAverage( StBuffer *aBuffer, INT iStart, INT iEnd, double *dblOutAverage, double *dblOutSDAve, double *dblOutSDMax, PINT nOutSDMPix );
	BOOL GetSNAverage( StBuffer *aBuffer, INT iStart, INT iEnd, double *dblOutAverage, double *dblOutSDAve, double *dblOutSDMax, PINT nOutSDMPix
								, double *pdblOutLineAverageMin=NULL, double *pdblOutLineAverageMax=NULL, PINT pnOutLineAverageMinPos=NULL, PINT pnOutLineAverageMaxPos=NULL );
	//��1.0.0.1028
	BOOL GetSpecialBehavior( StBuffer *aBuffer, INT iStart, INT iEnd, INT iMinLevel, INT iMaxLevel, INT iInOutMode
	//��1.0.0.1044
	                                   //, INT iColorMode, PINT piContinuous, PINT piLine, PINT piColor );
	                                   , INT iColorMode, PINT piContinuous, PINT piLine, PINT piColor, double *pdblLineAverage );
	//��1.0.0.1044
	//BOOL GetLineAverageDifference( StBuffer *aBuffer, INT iStart, INT iEnd, double * pdblDifference, PINT piHighLow, PINT piLine, size_t *pElementSize
	//										, double *pdblLineAverage, PINT piLineMin, PINT piLineMax, size_t bufferSize );
	BOOL GetLineAverageDifference( StBuffer *aBuffer, INT iStart, INT iEnd, double * pdblDifference, PINT piHighLow, PINT piLine, size_t *pElementSize
											, double **pdblLineAverage, INT **piLineMin, INT **piLineMax, size_t bufferSize );
	//��1.0.0.1041
	//BOOL GetWidthLineAverageDifference( StBuffer *aBuffer, INT iStart, INT iEnd, INT iWidth, INT iSeparate, size_t *pElementSize
	BOOL GetWidthLineAverageDifference( StBuffer *aBuffer, INT iStart, INT iEnd, INT iWidth, INT iSeparate, INT nMode, size_t *pElementSize
	//��1.0.0.1041
	//��1.0.0.1043
											//, double * pdblDifference, PINT piLine, double * pdblDifferenceRatio, PINT piLineRatio, double *pdblAverage=NULL );
											, double * pdblDifference, PINT piLine, double * pdblDifferenceRatio, PINT piLineRatio, double *pdblAverage=NULL, double dblLimitDiffRatio=0.0, PINT piLineContinuousCount=NULL, PINT piLineContinuousStart=NULL, PINT piLineContinuousEnd=NULL );
	//��1.0.0.1043
	//��1.0.0.1040
	//��1.0.0.1041
	//BOOL GetWidthLineAverageDifference( double *pdblBuffer, INT iSize, INT iStart, INT iEnd, INT iWidth, INT iSeparate, size_t *pElementSize
	BOOL GetWidthLineAverageDifference( double *pdblBuffer, INT iSize, INT iStart, INT iEnd, INT iWidth, INT iSeparate, INT nMode, size_t *pElementSize
	//��1.0.0.1041
	//��1.0.0.1043
											//, double * pdblDifference, PINT piLine, double * pdblDifferenceRatio, PINT piLineRatio, double *pdblAverage=NULL );
											, double * pdblDifference, PINT piLine, double * pdblDifferenceRatio, PINT piLineRatio, double *pdblAverage=NULL, double dblLimitDiffRatio=0.0, PINT piLineContinuousCount=NULL, PINT piLineContinuousStart=NULL, PINT piLineContinuousEnd=NULL );
	//��1.0.0.1043
	//��1.0.0.1040

	BOOL GetMinimumLineAverage( StBuffer *aBuffer, INT iStart, INT iEnd, size_t *pElementSize, double * pdblMinAverage, PINT piLine, PINT piElement );

	BOOL GetEffectivePixel( StBuffer *aBuffer, INT iStart, INT iEnd, INT iLevel, size_t *pElementSize, PINT piEffectiveStart, PINT piEffectiveEnd );


	//��1.0.0.1043
	BOOL GetWidthLineAverageDifference2( StBuffer *aBuffer, INT iStart, INT iEnd, INT iWidth, INT iSeparate, INT nMode, size_t *pElementSize
											, double * pdblDifference, PINT piLine, double * pdblDifferenceRatio, PINT piLineRatio, double *pdblAverage=NULL );
	//��1.0.0.1043


	//��1.0.0.1067b
	////��1.0.0.1058
	//////��1.0.0.1044
	////BOOL GetMinLevel( StBuffer *aBuffer, INT nStart, INT nEnd, int *pnMin );
	//////��1.0.0.1044
	//BOOL GetMinLevel( StBuffer *aBuffer, INT nStart, INT nEnd, int *pnMin, int *pnMinLine = NULL );
	////��1.0.0.1058

	BOOL GetMinLevel( StBuffer *aBuffer, INT nStart, INT nEnd, int *pnMin, int *pnMinLine = NULL, int nBunch = 1 );
	//��1.0.0.1067b

	//��1.0.0.1048
	BOOL GetChannelAverage( StBuffer *aBuffer, PINT pnStart, INT nWidth, INT nChannel, double *pAverage );
	//��1.0.0.1048

//private:


	//StBuffer����Element�擾
	INT GetElementFromBuffer( StBuffer *pBuffer );
	//StBuffer����PixelSize�擾
	INT GetPixelSize( StBuffer *pBuffer );

	//��1.0.0.1055
	INT GetPixelBits(StBuffer *pBuffer);
	//��1.0.0.1055

	//StBuffer����ColorIndex�擾
	BOOL SetElementColorIndexFromBuffer(StBuffer *pBuffer);
	INT GetElementColorIndexFromBuffer(StBuffer *pBuffer, INT iPos);	//0:R 1:G 2:B 3:A -1:Mono 

	//StInputSerialDlg�̂悤��CDialogEx��StCheckBase�������p�����Ă���ꍇ
	//StInputSerialDlg��StCheckBase�ŃL���X�g����ƈႤ�|�C���^�ɂȂ��Ă��܂��B
	//���̂���StCheckBase��CDialogEx�̃|�C���^��ۑ����Ă����B
	//�p���̋L�q�Ő��CDialogEx��������Ă��邩��Ǝv����B
	//���StCheckBase�������Ƃ���Ȃ������B�B�B
	//�i���j
	//���StCheckBase���L�q�����CDialogEx::OnOK();�Ńt���[�Y����ȂǕs�����
	//�����m_pDialog�̃n���h����ۑ�����B
	void *m_pDialog;

	//��1.0.0.1044
	BOOL WriteDataEBus(BYTE byteDeviceCode, BYTE bytePage, BYTE byteAddress, BYTE byteData);
	BOOL WriteDataMultiCamAscii(StString &szCommand, BYTE byteData);
	BOOL WriteDataMultiCamAscii(StString &szCommand);
	BOOL WriteDataMultiCamBinary(BYTE byteDeviceCode, BYTE bytePage, BYTE byteAddress, BYTE byteData);
	//��1.0.0.1044

	//��1.0.0.1062
	INT m_nScriptCheckMode;
	//��1.0.0.1062

private:
	INT m_nColorIndex[4];	//0:R 1:G 2:B 3:A -1:Mono 

};


/*
�����ԍ�
  �@�@�@�@�@�@�@�@�@     �@CMD          INS
1�V���A�����́@�@�@�@       ��           X
2�X�C�b�`����@�@�@�@       ��           X
3�{�����[�������@�@�@�@     ��           ��
4SN�����@�@�@�@�@�@�@�@     ��           ��
5�L����f�����@�@�@�@�@     ��           ��
6��f���@�@�@�@�@�@�@�@     ��           ��
7����d���@�@�@�@�@�@�@     ��           ��
8Gain�@�@�@�@�@�@�@�@�@     ��           ��
9���b�gNo���́@�@�@  �@     ��           X
10SN�V���b�N�@�@�@  �@      ��           X
11�V���A������2�@�@�@       ��           X
12�J�����ʐM�@�@�@   �@     X            ��
13FPS�����@�@�@   �@        X            ��
14���苓�������@�@�@        ��           ��
15DDT�����@�@�@   �@        ��           ��
16DustCheck                 ��           X			//1.48

18Gain      �@�@�@   �@     X            ��
19�Z���T�[�o�[�W�������́@�@��           X
20�Z���T�[�V���A�����́@  �@��           X
*/

/*
�R�}���h

TIM
PRNT


*/



