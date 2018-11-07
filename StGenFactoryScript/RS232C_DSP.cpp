// RS232C_OM.cpp: CRS232C_OM �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RS232C_DSP.h"
//#include "Common.h"
/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/


const WORD mcwRcvBufferSize = 0x20;

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CRS232C_DSP::CRS232C_DSP():CRS232C(TEXT(""))
{

	
}

CRS232C_DSP::~CRS232C_DSP()
{

	
}


//��2009/06/29 v1018 [ILA]DSP�ƒ��ڒʐM���邽�߂̊֐���ǉ�
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//DWORD CRS232C_DSP::mSonyDSPReadCategory(BYTE byteSwitch, BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd, PBYTE pbyteData)
//{
//	DWORD dwErrorCode = NO_ERROR;
//
//	BOOL	blnDirWriteFg = FALSE;
//	BYTE	byteSndCmdNo = CMD_SONY_DSP;
//	WORD	wSndDataLen = 4;
//	BYTE	pbyteSndData[] = {byteSwitch, byteCategoryNo, byteStart, byteEnd};
//	
//	BYTE	byteRcvCmdNo = 0;
//	WORD	wRcvDataLen = 0;
//	WORD	wDataLen = byteEnd - byteStart + 1;
//	WORD	wMaxRcvBytes = wDataLen + 4;
//	PBYTE	pbyteTmpData = new BYTE[wMaxRcvBytes];
//
//	dwErrorCode = mSendAndRcv(
//			blnDirWriteFg,
//			byteSndCmdNo,
//			wSndDataLen,
//			pbyteSndData,
//			&byteRcvCmdNo,
//			&wRcvDataLen,
//			pbyteTmpData,
//			wMaxRcvBytes);
//
//	if(dwErrorCode == NO_ERROR)
//	{
//		//��M�f�[�^���R�s�[
//		memcpy(pbyteData, &pbyteTmpData[4], wDataLen);
//	}
//	delete[] pbyteTmpData;
//
//	return(dwErrorCode);
//}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//DWORD CRS232C_DSP::SonyDSPReadRegisterCategory(BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd, PBYTE pbyteData)
//{
//	return(mSonyDSPReadCategory(0x01, byteCategoryNo, byteStart, byteEnd, pbyteData));
//}
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//DWORD CRS232C_DSP::SonyDSPReadEEPROMCategory(BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd, PBYTE pbyteData)
//{
//	return(mSonyDSPReadCategory(0x03, byteCategoryNo, byteStart, byteEnd, pbyteData));
//}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//DWORD CRS232C_DSP::mSonyDSPWriteCategory(BYTE byteSwitch, BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd, PBYTE pbyteData, WORD nDataSize)
//{
//	DWORD dwErrorCode = NO_ERROR;
//
//	BOOL	blnDirWriteFg = TRUE;
//	BYTE	byteSndCmdNo = CMD_SONY_DSP;
//	WORD	wSndDataLen = 4 + nDataSize;
//	PBYTE	pbyteSndData = new BYTE[wSndDataLen];
//
//	//���M�f�[�^���R�s�[
//	pbyteSndData[0] = byteSwitch;
//	pbyteSndData[1] = byteCategoryNo;
//	pbyteSndData[2] = byteStart;
//	pbyteSndData[3] = byteEnd;
//	if(0 < nDataSize)
//	{
//		memcpy(&pbyteSndData[4], pbyteData, 1 + byteEnd - byteStart);
//	}
//	
//	BYTE	byteRcvCmdNo = 0;
//	WORD	wRcvDataLen = 0;
//	WORD	wMaxRcvBytes = nDataSize + 4;
//	PBYTE	pbyteTmpData = new BYTE[wMaxRcvBytes];
//
//	dwErrorCode = mSendAndRcv(
//			blnDirWriteFg,
//			byteSndCmdNo,
//			wSndDataLen,
//			pbyteSndData,
//			&byteRcvCmdNo,
//			&wRcvDataLen,
//			pbyteTmpData,
//			wMaxRcvBytes);
//
//	if(dwErrorCode == NO_ERROR)
//	{
//		//��M�f�[�^���R�s�[
//		//memcpy(pbyteData, &pbyteTmpData[4], wDataLen);
//	}
//	delete[] pbyteTmpData;
//	delete[] pbyteSndData;
//
//	return(dwErrorCode);
//}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//DWORD CRS232C_DSP::SonyDSPWriteRegisterCategory(BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd, PBYTE pbyteData)
//{
//	return(mSonyDSPWriteCategory(0x02, byteCategoryNo, byteStart, byteEnd, pbyteData, 1 + byteEnd - byteStart));
//}
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//DWORD CRS232C_DSP::SonyDSPWriteEEPROMCategory(BYTE byteCategoryNo, BYTE byteStart, BYTE byteEnd)
//{
//	return(mSonyDSPWriteCategory(0x04, byteCategoryNo, byteStart, byteEnd, NULL, 0));
//}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//DWORD CRS232C_DSP::mSonyDSPEEPROMAll(BYTE byteSwitch)
//{
//	DWORD dwErrorCode = NO_ERROR;
//
//	BOOL	blnDirWriteFg = TRUE;
//	BYTE	byteSndCmdNo = CMD_SONY_DSP;
//	WORD	wSndDataLen = 1;
//	BYTE	pbyteSndData[] = {byteSwitch};
//
//	
//	BYTE	byteRcvCmdNo = 0;
//	WORD	wRcvDataLen = 0;
//	WORD	wDataLen = 1;
//	WORD	wMaxRcvBytes = 1;
//	PBYTE	pbyteTmpData = new BYTE[wMaxRcvBytes];
//
//	dwErrorCode = mSendAndRcv(
//			blnDirWriteFg,
//			byteSndCmdNo,
//			wSndDataLen,
//			pbyteSndData,
//			&byteRcvCmdNo,
//			&wRcvDataLen,
//			pbyteTmpData,
//			wMaxRcvBytes);
//
//	if(dwErrorCode == NO_ERROR)
//	{
//		//��M�f�[�^���R�s�[
//		//memcpy(pbyteData, &pbyteTmpData[4], wDataLen);
//	}
//	delete[] pbyteTmpData;
//
//	return(dwErrorCode);
//}
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//DWORD CRS232C_DSP::SonyDSPWriteEEPROMAll()
//{
//	return(mSonyDSPEEPROMAll(0x07));
//}
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//DWORD CRS232C_DSP::SonyDSPClearEEPROMAll()
//{
//	return(mSonyDSPEEPROMAll(0xF7));
//}
//��2009/06/29 v1018 [ILA]DSP�ƒ��ڒʐM���邽�߂̊֐���ǉ�

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//DWORD CRS232C_DSP::ReadI2C(BYTE byteSlvAdd,BYTE byteStartAdd,BYTE byteEndAdd,PBYTE pbyteData)
//{
//	DWORD dwErrorCode = NO_ERROR;
//
//	BOOL	blnDirWriteFg = FALSE;
//	BYTE	byteSndCmdNo = CMD_I2C;
//	WORD	wSndDataLen = 3;
//	BYTE	pbyteSndData[] = {byteSlvAdd,byteStartAdd,byteEndAdd};
//	
//	BYTE	byteRcvCmdNo = 0;
//	WORD	wRcvDataLen = 0;
//	WORD	wI2CDataLen = byteEndAdd - byteStartAdd + 1;
//	WORD	wMaxRcvBytes = wI2CDataLen + 3;
//	PBYTE	pbyteTmpData = new BYTE[wMaxRcvBytes];
//
//	dwErrorCode = mSendAndRcv(
//			blnDirWriteFg,
//			byteSndCmdNo,
//			wSndDataLen,
//			pbyteSndData,
//			&byteRcvCmdNo,
//			&wRcvDataLen,
//			pbyteTmpData,
//			wMaxRcvBytes);
//
//	if(dwErrorCode == NO_ERROR)
//	{
//		//��M�f�[�^���R�s�[
//		memcpy(pbyteData, &pbyteTmpData[3], wI2CDataLen);
//	}
//	delete[] pbyteTmpData;
//
//	return(dwErrorCode);
//}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//DWORD CRS232C_DSP::mWriteI2CShortSize(BYTE byteSlvAdd,BYTE byteStartAdd,BYTE byteEndAdd,PBYTE pbyteData)
//{
//	DWORD dwErrorCode = NO_ERROR;
//
//	BOOL	blnDirWriteFg = TRUE;
//	BYTE	byteSndCmdNo = CMD_I2C;
//	
//	BYTE	byteRcvCmdNo = 0;
//	WORD	wRcvDataLen = 0;
//	WORD	wI2CDataLen = byteEndAdd - byteStartAdd + 1;
//	WORD	wDataLen = wI2CDataLen + 3;
//	PBYTE	pbyteSndData = new BYTE[wDataLen];
//	PBYTE	pbyteTmpData = new BYTE[wDataLen];
//
//	//���M�f�[�^���쐬
//	pbyteSndData[0] = byteSlvAdd;
//	pbyteSndData[1] = byteStartAdd;
//	pbyteSndData[2] = byteEndAdd;
//	memcpy(&pbyteSndData[3], pbyteData, wI2CDataLen);
//
//
//	dwErrorCode = mSendAndRcv(
//			blnDirWriteFg,
//			byteSndCmdNo,
//			wDataLen,
//			pbyteSndData,
//			&byteRcvCmdNo,
//			&wRcvDataLen,
//			pbyteTmpData,
//			wDataLen);
//
//	delete[] pbyteSndData;
//	delete[] pbyteTmpData;
//
//	return(dwErrorCode);
//
//}

//-----------------------------------------------------------------------------
//16�o�C�g�P�ʂɋ�؂��đ��M����
//-----------------------------------------------------------------------------
//DWORD CRS232C_DSP::WriteI2C(BYTE byteSlvAdd, BYTE byteStartAdd, BYTE byteEndAdd, PBYTE pbyteData)
//{
//	DWORD dwErrorCode = NO_ERROR;
//	BYTE byteCurStart = byteStartAdd;
//	do{
//		BYTE byteCurEnd = byteCurStart | 0x0F;
//		if(byteEndAdd < byteCurEnd) byteCurEnd = byteEndAdd;
//		
//		dwErrorCode = mWriteI2CShortSize(byteSlvAdd,byteCurStart,byteCurEnd,&pbyteData[byteCurStart - byteStartAdd]);
//
//		if(
//			(NO_ERROR != dwErrorCode) ||
//			(byteCurEnd == byteEndAdd)
//		)
//			break;
//
//		byteCurStart = byteCurEnd + 1;
//	}while(1);
//
//	return(dwErrorCode);
//}

//��2009/03/04 v1015 [ILA]�����A�h���X2�o�C�gEEPROM�Ή�
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//DWORD CRS232C_DSP::ReadI2CWord(BYTE byteSlvAdd, WORD wStartAdd, WORD wEndAdd, PBYTE pbyteData)
//{
//	DWORD dwErrorCode = NO_ERROR;
//
//	BOOL	blnDirWriteFg = FALSE;
//	BYTE	byteSndCmdNo = CMD_I2C_WORD;
//	WORD	wSndDataLen = 5;
//	BYTE	pbyteSndData[] = {byteSlvAdd, wStartAdd >> 8, wStartAdd & 0xFF, wEndAdd >> 8, wEndAdd & 0xFF};
//	
//	BYTE	byteRcvCmdNo = 0;
//	WORD	wRcvDataLen = 0;
//	WORD	wI2CDataLen = wEndAdd - wStartAdd + 1;
//	WORD	wMaxRcvBytes = wI2CDataLen + wSndDataLen;
//	PBYTE	pbyteTmpData = new BYTE[wMaxRcvBytes];
//
//	dwErrorCode = mSendAndRcv(
//			blnDirWriteFg,
//			byteSndCmdNo,
//			wSndDataLen,
//			pbyteSndData,
//			&byteRcvCmdNo,
//			&wRcvDataLen,
//			pbyteTmpData,
//			wMaxRcvBytes);
//
//	if(dwErrorCode == NO_ERROR)
//	{
//		//��M�f�[�^���R�s�[
//		memcpy(pbyteData, &pbyteTmpData[wSndDataLen], wI2CDataLen);
//	}
//	delete[] pbyteTmpData;
//
//	return(dwErrorCode);
//}
//��2009/03/04 v1015 [ILA]�����A�h���X2�o�C�gEEPROM�Ή�
//��2009/03/04 v1015 [ILA]�����A�h���X2�o�C�gEEPROM�Ή�
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//DWORD CRS232C_DSP::WriteI2CWord(BYTE byteSlvAdd, WORD wStartAdd, WORD wEndAdd, PBYTE pbyteData)
//{
//	DWORD dwErrorCode = NO_ERROR;
//	WORD wCurStart = wStartAdd;
//	do{
//		WORD wCurEnd = wCurStart | 0x1F;
//		if(wEndAdd < wCurEnd) wCurEnd = wEndAdd;
//		
//		dwErrorCode = mWriteI2CWordShortSize(byteSlvAdd, wCurStart, wCurEnd, &pbyteData[wCurStart - wStartAdd]);
//
//		if(
//			(NO_ERROR != dwErrorCode) ||
//			(wCurEnd == wEndAdd)
//		)
//			break;
//
//		wCurStart = wCurEnd + 1;
//	}while(1);
//
//	return(dwErrorCode);
//}
//��2009/03/04 v1015 [ILA]�����A�h���X2�o�C�gEEPROM�Ή�

//��2009/03/04 v1015 [ILA]�����A�h���X2�o�C�gEEPROM�Ή�
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//DWORD CRS232C_DSP::mWriteI2CWordShortSize(BYTE byteSlvAdd, WORD wStartAdd, WORD wEndAdd, PBYTE pbyteData)
//{
//	DWORD dwErrorCode = NO_ERROR;
//
//	BOOL	blnDirWriteFg = TRUE;
//	BYTE	byteSndCmdNo = CMD_I2C_WORD;
//	
//	BYTE	byteRcvCmdNo = 0;
//	WORD	wRcvDataLen = 0;
//	WORD	wI2CDataLen = wEndAdd - wStartAdd + 1;
//	WORD	wDataLen = wI2CDataLen + 5;
//	PBYTE	pbyteSndData = new BYTE[wDataLen];
//	PBYTE	pbyteTmpData = new BYTE[wDataLen];
//
//	//���M�f�[�^���쐬
//	pbyteSndData[0] = byteSlvAdd;
//	pbyteSndData[1] = wStartAdd >> 8;
//	pbyteSndData[2] = wStartAdd & 0xFF;
//	pbyteSndData[3] = wEndAdd >> 8;
//	pbyteSndData[4] = wEndAdd & 0xFF;
//	memcpy(&pbyteSndData[5], pbyteData, wI2CDataLen);
//
//
//	dwErrorCode = mSendAndRcv(
//			blnDirWriteFg,
//			byteSndCmdNo,
//			wDataLen,
//			pbyteSndData,
//			&byteRcvCmdNo,
//			&wRcvDataLen,
//			pbyteTmpData,
//			wDataLen);
//
//	delete[] pbyteSndData;
//	delete[] pbyteTmpData;
//
//	return(dwErrorCode);
//
//}

//��2009/03/04 v1015 [ILA]�����A�h���X2�o�C�gEEPROM�Ή�
//-----------------------------------------------------------------------------
//1�o�C�g���M->1+���o�C�g��M
//-----------------------------------------------------------------------------
DWORD CRS232C_DSP::SetByteGetBytes(BYTE byteCmdNo, BYTE byteSetData, WORD wDataCount, PBYTE pbyteGetData)
{
	DWORD dwErrorCode = NO_ERROR;

	BOOL	blnDirWriteFg = FALSE;
	BYTE	byteSndCmdNo = byteCmdNo;
	WORD	wSndDataLen = 1;
	BYTE	pbyteSndData[] = {byteSetData};
	
	BYTE	byteRcvCmdNo = 0;
	WORD	wRcvDataLen = 0;
	WORD	wMaxRcvBytes = wDataCount + 1;
	PBYTE	pbyteTmpData = new BYTE[wMaxRcvBytes];

	dwErrorCode = mSendAndRcv(
			blnDirWriteFg,
			byteSndCmdNo,
			wSndDataLen,
			pbyteSndData,
			&byteRcvCmdNo,
			&wRcvDataLen,
			pbyteTmpData,
			wMaxRcvBytes);

	if(dwErrorCode == NO_ERROR)
	{
		//�擪1�o�C�g�������ăR�s�[
		memcpy(pbyteGetData,&pbyteTmpData[1],wDataCount);
	}
	delete[] pbyteTmpData;

	return(dwErrorCode);
}
//��2010/06/01 v2027 [ILB]FPGA�ݒ�p
//-----------------------------------------------------------------------------
//2�o�C�g���M->2+���o�C�g��M
//-----------------------------------------------------------------------------
DWORD CRS232C_DSP::SetWORDGetBytes(BYTE byteCmdNo, WORD wSetData, WORD wDataCount, PBYTE pbyteGetData)
{
	DWORD dwErrorCode = NO_ERROR;

	BOOL	blnDirWriteFg = FALSE;
	BYTE	byteSndCmdNo = byteCmdNo;
	WORD	wSndDataLen = 2;
	BYTE	pbyteSndData[] = {wSetData & 0xFF, wSetData >> 8};
	
	BYTE	byteRcvCmdNo = 0;
	WORD	wRcvDataLen = 0;
	WORD	wMaxRcvBytes = wDataCount + 2;
	PBYTE	pbyteTmpData = new BYTE[wMaxRcvBytes];

	dwErrorCode = mSendAndRcv(
			blnDirWriteFg,
			byteSndCmdNo,
			wSndDataLen,
			pbyteSndData,
			&byteRcvCmdNo,
			&wRcvDataLen,
			pbyteTmpData,
			wMaxRcvBytes);

	if(dwErrorCode == NO_ERROR)
	{
		//�擪2�o�C�g�������ăR�s�[
		memcpy(pbyteGetData,&pbyteTmpData[2],wDataCount);
	}
	delete[] pbyteTmpData;

	return(dwErrorCode);
}
//��2010/06/01 v2027 [ILB]FPGA�ݒ�p
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DWORD CRS232C_DSP::GetBytes(BYTE byteCmdNo,WORD wDataCount,PBYTE pbyteData)
{
	DWORD dwErrorCode = NO_ERROR;

	BOOL	blnDirWriteFg = FALSE;
	BYTE	byteSndCmdNo = byteCmdNo;
	WORD	wSndDataLen = 0;
	BYTE	pbyteSndData[1];
	
	BYTE	byteRcvCmdNo = 0;
	WORD	wRcvDataLen = 0;
	WORD	wMaxRcvBytes = wDataCount;
	PBYTE	pbyteTmpData = new BYTE[wMaxRcvBytes];


	dwErrorCode = mSendAndRcv(
			blnDirWriteFg,
			byteSndCmdNo,
			wSndDataLen,
			pbyteSndData,
			&byteRcvCmdNo,
			&wRcvDataLen,
			pbyteTmpData,
			wMaxRcvBytes);

	if(dwErrorCode == NO_ERROR)
	{
		memcpy(pbyteData, pbyteTmpData, wDataCount);
	}
	delete[] pbyteTmpData;

	return(dwErrorCode);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DWORD CRS232C_DSP::SetBytes(BYTE byteCmdNo,WORD wDataCount,PBYTE pbyteData)
{
	DWORD dwErrorCode = NO_ERROR;

	BOOL	blnDirWriteFg = TRUE;
	BYTE	byteSndCmdNo = byteCmdNo;
	
	BYTE	byteRcvCmdNo = 0;
	WORD	wRcvDataLen = 0;
	WORD	wMaxRcvBytes = wDataCount;
	PBYTE	pbyteTmpData = new BYTE[wMaxRcvBytes];

	dwErrorCode = mSendAndRcv(
			blnDirWriteFg,
			byteSndCmdNo,
			wDataCount,
			pbyteData,
			&byteRcvCmdNo,
			&wRcvDataLen,
			pbyteTmpData,
			wMaxRcvBytes);

	delete[] pbyteTmpData;

	return(dwErrorCode);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DWORD CRS232C_DSP::SetWord(BYTE byteCmdNo,WORD wData)
{
	BYTE	pbyteSndData[] = {
				wData & 0xFF,
				wData >> 8};
	return(SetBytes(byteCmdNo, 2, pbyteSndData));
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DWORD CRS232C_DSP::GetWord(BYTE byteCmdNo,PWORD pwData)
{
	DWORD	dwErrorCode = NO_ERROR;
	BYTE	pbyteTmpData[2];

	dwErrorCode = GetBytes(byteCmdNo, 2, pbyteTmpData);
	if(dwErrorCode == NO_ERROR)
	{
		*pwData = (pbyteTmpData[1] << 8) | pbyteTmpData[0];
	}

	return(dwErrorCode);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DWORD CRS232C_DSP::Set2Word(BYTE byteCmdNo,WORD wData1,WORD wData2)
{
	BYTE	pbyteSndData[] = {
				wData1 & 0xFF,
				wData1 >> 8,
				wData2 & 0xFF,
				wData2 >> 8};
	return(SetBytes(byteCmdNo, 4, pbyteSndData));
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DWORD CRS232C_DSP::Get2Word(BYTE byteCmdNo,PWORD pwData1,PWORD pwData2)
{
	DWORD	dwErrorCode = NO_ERROR;
	BYTE	pbyteTmpData[4];

	dwErrorCode = GetBytes(byteCmdNo, 4, pbyteTmpData);
	if(dwErrorCode == NO_ERROR)
	{
		*pwData1 = (pbyteTmpData[1] << 8) | pbyteTmpData[0];
		*pwData2 = (pbyteTmpData[3] << 8) | pbyteTmpData[2];
	}

	return(dwErrorCode);
}
//-----------------------------------------------------------------------------
//Send And Rcv
//-----------------------------------------------------------------------------
DWORD CRS232C_DSP::mSendAndRcv(
			BOOL	blnDirWriteFg,
			BYTE	byteSndCmdNo,
			WORD	wSndDataLen,
			PBYTE	pbyteSndData,
			PBYTE	pbyteRcvCmdNo,
			PWORD	pwRcvDataLen,
			PBYTE	pbyteRcvData,
			WORD	wMaxRcvBytes)
{
	DWORD	dwErrorCode = NO_ERROR;

	//��2007/03/22 v1004 [FLC]�d����������1�o�C�g���M�����s��Ώ�
	ClearRcvBuff();
	//��2007/03/22 v1004 [FLC]�d����������1�o�C�g���M�����s��Ώ�

	dwErrorCode = mFormatSend(
					blnDirWriteFg,
					byteSndCmdNo,
					wSndDataLen,
					pbyteSndData);

	if(dwErrorCode == NO_ERROR)
	{
		*pbyteRcvCmdNo = byteSndCmdNo;
		dwErrorCode = mFormatRcv(
					pbyteRcvCmdNo,
					pwRcvDataLen,
					pbyteRcvData,
					wMaxRcvBytes);

		if(dwErrorCode != NO_ERROR)
		{
			ClearRcvBuff();
		}
	}

	return(dwErrorCode);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DWORD CRS232C_DSP::mFormatSend(
			BOOL	blnDirWriteFg,
			BYTE	byteCmdNo,
			WORD	wDataLen,
			PBYTE	pbyteData)
{
	DWORD	dwErrorCode = NO_ERROR;
	
	WORD	wPos;
	DWORD	dwSendLen = wDataLen + 6;
	DWORD	dwSendedLen = 0;
	PBYTE	pbyteSendBuff = new BYTE[dwSendLen];
	PBYTE	pbyteSendBuffPos = pbyteSendBuff;
	BYTE	byteCheckSum = 0x00;

	*pbyteSendBuffPos++ = RS232C_FORMAT_SOF;
	*pbyteSendBuffPos++ = byteCmdNo;
	*pbyteSendBuffPos++ = (blnDirWriteFg?0x80:0x00) | (wDataLen >> 8);
	*pbyteSendBuffPos++ = wDataLen & 0x00FF;

	//�f�[�^���R�s�[
	for(wPos=0;wPos<wDataLen;wPos++)
		*pbyteSendBuffPos++ = *pbyteData++;


	//�`�F�b�N�T���Z�o
	for(wPos=0;wPos<wDataLen + 3;wPos++)
	{
		byteCheckSum = (BYTE)((byteCheckSum + pbyteSendBuff[wPos + 1]) & 0xFF);
	}
	*pbyteSendBuffPos++ = byteCheckSum;
	*pbyteSendBuffPos++ = RS232C_FORMAT_EOF;

	//���M
	dwErrorCode = Send(dwSendLen,pbyteSendBuff,&dwSendedLen);



	delete[] pbyteSendBuff;

	return(dwErrorCode);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DWORD CRS232C_DSP::mFormatRcv(
			PBYTE	pbyteCmdNo,
			PWORD	pwDataLen,
			PBYTE	pbyteData,
			WORD	wMaxRcvBytes)
{
	DWORD	dwErrorCode = NO_ERROR;
	DWORD	dwRcvErrorCode = NO_ERROR;
	DWORD	dwTmpRcvBufferSize = wMaxRcvBytes + 6;
	PBYTE	pbyteTmpRcv = new BYTE[dwTmpRcvBufferSize];
	DWORD	dwRcvBytesCount = 0;
	BYTE	byteCheckSum = 0;
	WORD	wDataPartLen = 0;

	do{
		BOOL	bRecieveEndFg = FALSE;
		DWORD	dwRcvedLen = 0;

		//��2009/07/16 v1019 [ILA]�ԐM�f�[�^0���ݒ�ɑΉ�
		if(wDataPartLen == 0)	dwTmpRcvBufferSize = 6;
		else					dwTmpRcvBufferSize = (wDataPartLen < wMaxRcvBytes)?wMaxRcvBytes:wDataPartLen;
		//��2009/07/16 v1019 [ILA]�ԐM�f�[�^0���ݒ�ɑΉ�

		//��M����
		dwErrorCode = Rcv(dwTmpRcvBufferSize,pbyteTmpRcv,&dwRcvedLen);
		if(dwErrorCode)
		{
			break;
		}
		else if(dwRcvedLen == 0)
		{
			dwErrorCode = ERROR_PC_RCV_TIMEOUT;
			break;
		}

		for(DWORD dwCurProcessByteNo = 0;dwCurProcessByteNo < dwRcvedLen;dwCurProcessByteNo++)
		{
			BYTE byteTmpRcv = pbyteTmpRcv[dwCurProcessByteNo];
			if(dwRcvBytesCount == 0)
			{
				//SOF�`�F�b�N
				if(byteTmpRcv != RS232C_FORMAT_SOF)
				{
					dwErrorCode = ERROR_PC_RCV_SOF;
					break;
				}
			}
			else if(dwRcvBytesCount == 1)
			{
				//�R�}���h�ԍ�
				byteCheckSum = byteTmpRcv;
				if(*pbyteCmdNo != byteTmpRcv)
				{
					dwErrorCode = ERROR_PC_RCV_CMD;
					*pbyteCmdNo = byteTmpRcv;
				}
			}
			else if(dwRcvBytesCount == 2)
			{
				//�f�[�^��H
				byteCheckSum = (BYTE)((byteCheckSum + byteTmpRcv) & 0xFF);
				wDataPartLen = byteTmpRcv;
			}
			else if(dwRcvBytesCount == 3)
			{
				//�f�[�^��H
				byteCheckSum = (BYTE)((byteCheckSum + byteTmpRcv) & 0xFF);
				wDataPartLen <<= 8;
				wDataPartLen |= (WORD)byteTmpRcv;

				if(
					(wDataPartLen > wMaxRcvBytes) &&
					(*pbyteCmdNo != CMD_ERROR)
				)
				{
					dwErrorCode = ERROR_PC_RCV_DATALEN;
					break;
				}
			}
			else if(dwRcvBytesCount == wDataPartLen + (DWORD)4)
			{
				//�`�F�b�N�T��
				
				//��2011/05/17 v2033 [JHA]��M�`�F�b�N�T���m�F�s����C��
				//if(byteCheckSum != byteCheckSum)
				if(byteTmpRcv != byteCheckSum)
				//��2011/05/17 v2033 [JHA]��M�`�F�b�N�T���m�F�s����C��
				{
					dwErrorCode = ERROR_PC_RCV_CHECKSUM;
					break;
				}
			}
			else if(dwRcvBytesCount == wDataPartLen + (DWORD)5)
			{
				//EOF�`�F�b�N
				if(byteTmpRcv != RS232C_FORMAT_EOF)
				{
					dwErrorCode = ERROR_PC_RCV_EOF;
					break;
				}

				*pwDataLen = wDataPartLen;
				bRecieveEndFg = TRUE;
				break;
			}
			else
			{
				//�f�[�^
				byteCheckSum = (BYTE)((byteCheckSum + byteTmpRcv) & 0xFF);
				if(*pbyteCmdNo != CMD_ERROR)
				{
					*pbyteData++ = byteTmpRcv;
				}
				else
				{
					if(dwRcvBytesCount == 4)
					{
						dwRcvErrorCode = 0xE0000000 | byteTmpRcv;
					}
				}
			}

			dwRcvBytesCount++;
		}
		if(dwErrorCode != NO_ERROR) break;
		if(bRecieveEndFg) break;
	}while(1);

	delete[] pbyteTmpRcv;

	if(*pbyteCmdNo != CMD_ERROR)
		return(dwErrorCode);
	else
		return(dwRcvErrorCode);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
CRS232C_HCA::CRS232C_HCA():CRS232C(TEXT(""))
{

	
}

CRS232C_HCA::~CRS232C_HCA()
{

	
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DWORD CRS232C_HCA::ReadI2C(BYTE byteSlvAdd,BYTE byteStartAdd,BYTE byteEndAdd,PBYTE pbyteData)
{
	//��2011/01/21 v2029 [HCA]�ǂݍ��ݎ��ɊԈ���ď������݃t���O���ݒ肳��Ă��Ă��������܂�Ȃ��悤�ɕύX
	DWORD dwErrorCode = mSendAndRcv(
		(~OM_DRWP_RW_WRITE) & byteSlvAdd,
		byteStartAdd,
		byteEndAdd - byteStartAdd + 1,
		pbyteData);
	//��2011/01/21 v2029 [HCA]�ǂݍ��ݎ��ɊԈ���ď������݃t���O���ݒ肳��Ă��Ă��������܂�Ȃ��悤�ɕύX

	return(dwErrorCode);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DWORD CRS232C_HCA::WriteI2C(BYTE byteSlvAdd,BYTE byteStartAdd,BYTE byteEndAdd,PBYTE pbyteData)
{
	DWORD dwErrorCode = mSendAndRcv(
		OM_DRWP_RW_WRITE | byteSlvAdd,
		byteStartAdd,
		byteEndAdd - byteStartAdd + 1,
		pbyteData);

	return(dwErrorCode);
}

//-----------------------------------------------------------------------------
//Send And Rcv
//-----------------------------------------------------------------------------
DWORD CRS232C_HCA::mSendAndRcv(
			BYTE	byteDRWPCode,
			BYTE	byteCmdNo,
			BYTE	byteDataLen,
			PBYTE	pbyteData)
{
	DWORD	dwErrorCode = NO_ERROR;

	ClearRcvBuff();

	dwErrorCode = mFormatSend(
					byteDRWPCode,
					byteCmdNo,
					byteDataLen,
					pbyteData);

	if(dwErrorCode==NO_ERROR)
	{
		dwErrorCode=mFormatRcv(
					byteDRWPCode,
					byteCmdNo,
					byteDataLen,
					pbyteData);
	}

	return(dwErrorCode);
}
//-----------------------------------------------------------------------------
//�J���������N�p
//-----------------------------------------------------------------------------
DWORD CRS232C_HCA::mFormatSend(
			BYTE	byteDRWPCode,
			BYTE	byteCmdNo,
			BYTE	byteDataLen,
			PBYTE	pbyteData)
{
	DWORD	dwErrorCode = NO_ERROR;
	BOOL	bWriteMode = byteDRWPCode & OM_DRWP_RW_WRITE;
	DWORD	dwSendLen = bWriteMode?byteDataLen+5:6;
	PBYTE	pbyteSendBuff = new BYTE[dwSendLen];
	PBYTE	pbyteSendBuffPos = pbyteSendBuff;
	DWORD	dwSended = 0;
	
	*pbyteSendBuffPos++ = RS232C_FORMAT_SOF;
	*pbyteSendBuffPos++ = byteDRWPCode;
	*pbyteSendBuffPos++ = byteCmdNo;
	*pbyteSendBuffPos++ = byteDataLen;

	if(bWriteMode)
	{
		//�������݃��[�h
		while(byteDataLen--)
			*pbyteSendBuffPos++ = *pbyteData++;
	}
	else
	{
		//�ǂݍ��݃��[�h
		*pbyteSendBuffPos++ = 0x00;	//�_�~�[�f�[�^
	}
	*pbyteSendBuffPos++ = RS232C_FORMAT_EOF;
	//���M
	dwErrorCode = Send(dwSendLen,pbyteSendBuff, &dwSended);


	delete[] pbyteSendBuff;

	return(dwErrorCode);

}

//-----------------------------------------------------------------------------
//�J���������N�p
//-----------------------------------------------------------------------------
DWORD CRS232C_HCA::mFormatRcv(
			BYTE	byteDRWPCode,
			BYTE	byteCmdNo,
			BYTE	byteDataLen,
			PBYTE	pbyteData)
{
	BOOL	bReval = TRUE;
	BOOL	bWriteMode = byteDRWPCode & OM_DRWP_RW_WRITE;
	DWORD	dwRcvLen = bWriteMode?4:byteDataLen+3;
	PBYTE	pbyteRcvBuff = new BYTE[dwRcvLen];
	PBYTE	pbyteRcvBuffPos = pbyteRcvBuff;
	DWORD	dwRcvNum = 0;

	DWORD dwErrorCode = 0;

	memset(pbyteRcvBuff,0,dwRcvLen);

	do{
		//��M����(1�o�C�g�Î�M���Ȃ��Ǝ�M�ł��Ȃ����Ƃ������B���́H�j
		dwErrorCode=Rcv(dwRcvLen,pbyteRcvBuff, &dwRcvNum);

		if(NO_ERROR != dwErrorCode)
		{
			break;
		}


		//SOF�`�F�b�N
		if(*pbyteRcvBuffPos++ != RS232C_FORMAT_SOF)
		{
			dwErrorCode = ERROR_PC_RCV_SOF;
			break;
		}

		if(bWriteMode)
		{
			//�������ݎ�
			if((*pbyteRcvBuffPos++ != 0x00) && (*pbyteRcvBuffPos++ != 0x01))
			{
				dwErrorCode = ERROR_PC_RCV_DATALEN;
				break;
			}

			switch(*pbyteRcvBuffPos++)
			{
			case(0x10):
				dwErrorCode = ERROR_RCV_CAM_RCV;
				break;
			case(0x11):
				dwErrorCode = ERROR_RCV_PERIPHERAL;
				break;
			case(0x07):
				dwErrorCode = ERROR_RCV_CAM_BUSY;
				break;
			}
			if(dwErrorCode != NO_ERROR)
				break;
		}
		else
		{
			//�ǂݍ��ݎ�
			if(*pbyteRcvBuffPos != byteDataLen)
			{
				dwErrorCode = ERROR_PC_RCV_DATALEN;
				if(*pbyteRcvBuffPos++ == 0)
				{
					switch(*pbyteRcvBuffPos++)
					{
					case(0x10):
						dwErrorCode = ERROR_RCV_CAM_RCV;
						break;
					case(0x11):
						dwErrorCode = ERROR_RCV_PERIPHERAL;
						break;
					case(0x07):
						dwErrorCode = ERROR_RCV_CAM_BUSY;
						break;
					}
				}
				break;
			}
			pbyteRcvBuffPos++;

			//��M�f�[�^���R�s�[
			while(byteDataLen--)
				*pbyteData++ = *pbyteRcvBuffPos++;
		}

		//ETX�`�F�b�N
		if(*pbyteRcvBuffPos++ != RS232C_FORMAT_EOF)
		{
			dwErrorCode = ERROR_PC_RCV_EOF;
			break;
		}

	}while(0);

	delete[] pbyteRcvBuff;
	return(dwErrorCode);
}

*/



