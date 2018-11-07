// PowerCtrl.cpp: CPowerCtrl �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "StGEFactory.h"
#include "PowerCtrl.h"

//��1.3.0.10�����ꃂ�[�h�Ή�
#include "HookMsgBox.h"
//��1.3.0.10�����ꃂ�[�h�Ή�

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//��2007/07/13 v0.47 Beta9 CLC232B�Ή�(�d���̍ő�d���l��400mA����500mA�ɕύX)
const double gcdblCurrentMaxValue = 0.5;
//��2007/07/13 v0.47 Beta9 CLC232B�Ή�(�d���̍ő�d���l��400mA����500mA�ɕύX)
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CPowerCtrl::CPowerCtrl()
{
	m_hTMI = -1;
	m_dwPCAddress = 1;
	m_dwSystemAddress = 1;
	m_byteChannelForCamera = 1;	//A
//	m_byteChannelForJigu = 2;	//B

	//��2007/06/01 v0.46 Beta6 �W�b�^�����ύX(���s�ʒu�A�����l�A�ҋ@����)
	//�d���𓊓����Ă���̌o�ߎ��Ԃ��L��
	m_dwPowerOnTime = GetTickCount();
	//��2007/06/01 v0.46 Beta6 �W�b�^�����ύX(���s�ʒu�A�����l�A�ҋ@����)

	//��1.0.0.1060
	m_dblCurrentLimit = gcdblCurrentMaxValue;
	//��1.0.0.1060
}

CPowerCtrl::~CPowerCtrl()
{
	bDisconnect();
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL	CPowerCtrl::bConnect(void)
{
	BOOL bReval = TRUE;
	CHAR		pchrPWASet[256];
	INT			iReval = 0;

	//������(DLL�����݂��Ȃ��ꍇ�G���[)
	bReval = m_TMI_API_IF.IFInitialize();
	if(!bReval) return(bReval);

	//IF��ށAPC�A�h���X,�V�X�e���A�h���X���w�肷�镶����
	sprintf_s(pchrPWASet,sizeof(pchrPWASet)/sizeof(pchrPWASet[0]),"USB:%u:%u",m_dwPCAddress,m_dwSystemAddress);
	do{
		m_hTMI = m_TMI_API_IF.HandleOpen("PW-A",pchrPWASet);
		if(m_hTMI < 0)
		{
			/*
			if(IDCANCEL == ::MessageBox(
				NULL,
				TEXT("�ʐM�\�ȓd����������܂���ł����B\r\n")
				TEXT("�蓮�̏ꍇ:[�L�����Z��]�{�^�����N���b�N\r\n")
				TEXT("�����̏ꍇ:USB�P�[�u����}���Ȃ����Đ��b�o���Ă���[�Ď��s]�{�^�����N���b�N"),
				NULL,
				MB_RETRYCANCEL))
				*/
			//��1.3.0.10�����ꃂ�[�h�Ή�
			//CString szText;
			//szText.LoadStringW(IDS_NOT_FOUND_POWER_UNIT);
			//if( IDCANCEL == AfxMessageBoxHooked( szText, MB_RETRYCANCEL) )
			if(IDCANCEL == AfxMessageBox(
				TEXT("�ʐM�\�ȓd����������܂���ł����B\r\n")
				TEXT("�蓮�̏ꍇ:[�L�����Z��]�{�^�����N���b�N\r\n")
				TEXT("�����̏ꍇ:USB�P�[�u����}���Ȃ����Đ��b�o���Ă���[�Ď��s]�{�^�����N���b�N"),
				MB_RETRYCANCEL))
				 return(FALSE);
			//��1.3.0.10�����ꃂ�[�h�Ή�

		}
		else
		{
			break;
		}
	}while(1);

	iReval = m_TMI_API_IF.Preset(m_hTMI,1);	//������1
	if(iReval < 0)
	{
		bDisconnect();
		return(FALSE);
	}

	iReval = m_TMI_API_IF.Voltage(m_hTMI,m_byteChannelForCamera,1,12.0);	//������1 �`�����l��A 12.0V
	if(iReval < 0)
	{
		bDisconnect();
		return(FALSE);
	}
	//��1.0.0.1060
	////��2007/07/13 v0.47 Beta9 CLC232B�Ή�(�d���̍ő�d���l��400mA����500mA�ɕύX)
	////iReval = m_TMI_API_IF.Current(m_hTMI,m_byteChannelForCamera,1,0.40);		//������1 �`�����l��A 0.4A
	//iReval = m_TMI_API_IF.Current(m_hTMI,m_byteChannelForCamera,1, gcdblCurrentMaxValue);		//������1 �`�����l��A 0.4A
	////��2007/07/13 v0.47 Beta9 CLC232B�Ή�(�d���̍ő�d���l��400mA����500mA�ɕύX)
	iReval = m_TMI_API_IF.Current(m_hTMI,m_byteChannelForCamera,1, m_dblCurrentLimit);		//������1 �`�����l��A 0.4A
	//��1.0.0.1060
	if(iReval < 0)
	{
		bDisconnect();
		return(FALSE);
	}

	return(bReval);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL	CPowerCtrl::bDisconnect(void)
{
	BOOL bReval = TRUE;
	if(m_hTMI >= 0)
	{
		m_TMI_API_IF.RemoteLocal(m_hTMI);
		m_TMI_API_IF.HandleClose(m_hTMI);
		m_hTMI = -1;
	}
	return(bReval);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL	CPowerCtrl::bGetCurrent(DOUBLE *pdblCurrent)
{
	//��1.0.0.1018
	//BOOL bReval = TRUE;
	//LONG		lReval = 0;
	//DOUBLE		dblVoltage;
	////DOUBLE		dblPrevCurrent;
	//CHAR		chrCVCC;

	//if(m_hTMI < 0)	return(FALSE);

	////���݂̓d���l���擾
	//lReval = m_TMI_API_IF.MoniDataQ(m_hTMI,m_byteChannelForCamera,&dblVoltage,pdblCurrent,&chrCVCC);
	//if(lReval < 0)
	//	bReval = FALSE;

	//return(bReval);
	//��1.0.0.1018
	return bGetCurrent(m_byteChannelForCamera, pdblCurrent);
}

//��1.0.0.1018
BOOL	CPowerCtrl::bGetCurrent(BYTE byteChannel, DOUBLE *pdblCurrent)
{
	BOOL bReval = TRUE;
	LONG		lReval = 0;
	DOUBLE		dblVoltage;
	CHAR		chrCVCC;

	if(m_hTMI < 0)	return(FALSE);

	//���݂̓d���l���擾
	lReval = m_TMI_API_IF.MoniDataQ(m_hTMI,byteChannel,&dblVoltage,pdblCurrent,&chrCVCC);
	if(lReval < 0)
		bReval = FALSE;

	return(bReval);
}
//��1.0.0.1018


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL	CPowerCtrl::bPowerOn(void)
//��1.0.0.1018
{
	return bPowerOn(m_byteChannelForCamera);
}

BOOL	CPowerCtrl::bPowerOn(BYTE byteChannel)
//��1.0.0.1018
{
	BOOL bReval = TRUE;
	INT			iReval = 0;

	//��2007/06/01 v0.46 Beta6 �W�b�^�����ύX(���s�ʒu�A�����l�A�ҋ@����)
	//�d���𓊓����Ă���̌o�ߎ��Ԃ��L��
	m_dwPowerOnTime = GetTickCount();
	//��2007/06/01 v0.46 Beta6 �W�b�^�����ύX(���s�ʒu�A�����l�A�ҋ@����)

	if(m_hTMI < 0)	return(FALSE);

	//	�d���l�̐ݒ�
	//��1.0.0.1060
	////��1.0.0.1018
	////iReval = m_TMI_API_IF.Current(m_hTMI,m_byteChannelForCamera,1,400.0);
	//iReval = m_TMI_API_IF.Current(m_hTMI,byteChannel,1,gcdblCurrentMaxValue);
	////��1.0.0.1018
	iReval = m_TMI_API_IF.Current(m_hTMI,byteChannel,1,m_dblCurrentLimit);
	//��1.0.0.1060
	if(iReval < 0)	bReval = FALSE;

	//��2008/02/29 v0.54 Beta05 �d��ON�̏��Ԃ��u����->�J�����v�A�d��OFF�̏��Ԃ��u�J����->����v�ɕύX

	//���C���A�E�g�v�b�gON
	//��1.0.0.1018
	//iReval = m_TMI_API_IF.MainOutput(m_hTMI,1);
	//if(iReval < 0)	bReval = FALSE;
	//��1.0.0.1018

	//�A�E�g�v�b�g�Z���N�g B ON
//	iReval = m_TMI_API_IF.OutputSel(m_hTMI,m_byteChannelForJigu,1);
//	if(iReval < 0)	bReval = FALSE;

//	Sleep(500);

	//�A�E�g�v�b�g�Z���N�g A ON
	//��1.0.0.1018
	//iReval = m_TMI_API_IF.OutputSel(m_hTMI,m_byteChannelForCamera,1);
	iReval = m_TMI_API_IF.OutputSel(m_hTMI,byteChannel,1);
	//��1.0.0.1018
	if(iReval < 0)	bReval = FALSE;
	/*
	//�A�E�g�v�b�g�Z���N�g A ON
	iReval = m_TMI_API_IF.OutputSel(m_hTMI,m_byteChannelForCamera,1);
	if(iReval < 0)	bReval = FALSE;

	//�A�E�g�v�b�g�Z���N�g B ON
	iReval = m_TMI_API_IF.OutputSel(m_hTMI,m_byteChannelForJigu,1);
	if(iReval < 0)	bReval = FALSE;

	//���C���A�E�g�v�b�gON
	iReval = m_TMI_API_IF.MainOutput(m_hTMI,1);
	if(iReval < 0)	bReval = FALSE;
	*/
	//��2008/02/29 v0.54 Beta05 �d��ON�̏��Ԃ��u����->�J�����v�A�d��OFF�̏��Ԃ��u�J����->����v�ɕύX


	//A�`�����l���̏���\��
	//��1.0.0.1018
	//iReval = m_TMI_API_IF.Display(m_hTMI,m_byteChannelForCamera);
	iReval = m_TMI_API_IF.Display(m_hTMI,byteChannel);
	//��1.0.0.1018
	if(iReval < 0)	bReval = FALSE;


	//�d�������ҋ@
	DWORD dwRetry = 10;
	do{
		DOUBLE dblCurrent;
		bGetCurrent(byteChannel,&dblCurrent);
		if(dblCurrent > 0.05)	break;
		Sleep(500);
	}while(dwRetry--);

	//��2006/10/11 v0.40 ������ �d���������莞�ԑҋ@����悤�ɕύX(E42,43�Ō����J�n���ɒʐM�G���[��������������)
	Sleep(1000);
	//��2006/10/11 v0.40 ������ �d���������莞�ԑҋ@����悤�ɕύX(E42,43�Ō����J�n���ɒʐM�G���[��������������)

	return(bReval);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL	CPowerCtrl::bPowerOff(void)
//��1.0.0.1018
{
	return bPowerOff(m_byteChannelForCamera);
}

BOOL	CPowerCtrl::bPowerOff(BYTE byteChannel)
//��1.0.0.1018

{
	BOOL bReval = TRUE;
	INT			iReval = 0;
	
	if(m_hTMI < 0)	return(FALSE);

	//�A�E�g�v�b�g�Z���N�g A ON
	//��1.0.0.1018
	//iReval = m_TMI_API_IF.OutputSel(m_hTMI,m_byteChannelForCamera,0);
	iReval = m_TMI_API_IF.OutputSel(m_hTMI,byteChannel,0);
	//��1.0.0.1018
	if(iReval < 0)	bReval = FALSE;
 
	//���C���A�E�g�v�b�gOFF
	//��1.0.0.1018
	//iReval = m_TMI_API_IF.MainOutput(m_hTMI,0);
	//if(iReval < 0)	bReval = FALSE;
	//��1.0.0.1018

	return(bReval);
}
//-----------------------------------------------------------------------------
//12V->24V  or  24V->12V
//-----------------------------------------------------------------------------
BOOL	CPowerCtrl::bSetVoltage(double dblVoltage)
{
	BOOL bReval = TRUE;
	INT			iReval = 0;

//	iReval = m_TMI_API_IF.Voltage(m_hTMI,m_byteChannelForCamera,1,18.0);	//������1 �`�����l��A 18.0V
//	Sleep(500);
	iReval = m_TMI_API_IF.Voltage(m_hTMI,m_byteChannelForCamera,1,dblVoltage);	//������1 �`�����l��A 24.0V
	return bReval;

}

//��1.3.0.9
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL	CPowerCtrl::bGetVoltage(double *pdblVoltage)
{
	BOOL bReval = TRUE;
	LONG		lReval = 0;
	DOUBLE		dblCurrent;
	//DOUBLE		dblPrevCurrent;
	CHAR		chrCVCC;

	if(m_hTMI < 0)	return(FALSE);

	//���݂̓d���l���擾
	lReval = m_TMI_API_IF.MoniDataQ(m_hTMI,m_byteChannelForCamera,pdblVoltage,&dblCurrent,&chrCVCC);
	if(lReval < 0)
		bReval = FALSE;

	return(bReval);
}
//��1.3.0.9
//��1.0.0.1060
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL CPowerCtrl::bSetCurrentLimit(double dblCurrentLimit)
{
	BOOL bReval = TRUE;
	if(m_hTMI >= 0)
	{
		LONG		lReval = 0;
		lReval = m_TMI_API_IF.m_TMI_Current(m_hTMI,m_byteChannelForCamera,1,dblCurrentLimit);
		if(lReval < 0)
			bReval = FALSE;
	}
	if( bReval ) m_dblCurrentLimit = dblCurrentLimit;

	return(bReval);
}

BOOL CPowerCtrl::bGetCurrentLimit(double *pdblCurrentLimit)
{
	BOOL bReval = TRUE;
	if(m_hTMI >= 0)
	{
		LONG		lReval = 0;
		lReval = m_TMI_API_IF.m_TMI_CurrentQ(m_hTMI,m_byteChannelForCamera,1,pdblCurrentLimit);
		if(lReval < 0)
			bReval = FALSE;
	}
	else
	{
		*pdblCurrentLimit = m_dblCurrentLimit;
	}
	return(bReval);
}
//��1.0.0.1060

//��1.0.0.1018
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL	CPowerCtrl::bMainPowerOn(void)
{
	BOOL bReval = TRUE;
	INT			iReval = 0;

	if(m_hTMI < 0)	return(FALSE);

	//�d���𓊓����Ă���̌o�ߎ��Ԃ��L��
	m_dwPowerOnTime = GetTickCount();

	//���C���A�E�g�v�b�gON
	iReval = m_TMI_API_IF.MainOutput(m_hTMI,1);
	if(iReval < 0)	bReval = FALSE;

	return(bReval);
}

BOOL	CPowerCtrl::bMainPowerOff(void)
{
	BOOL bReval = TRUE;
	INT			iReval = 0;
	
	if(m_hTMI < 0)	return(FALSE);

	//���C���A�E�g�v�b�gOFF
	iReval = m_TMI_API_IF.MainOutput(m_hTMI,0);
	if(iReval < 0)	bReval = FALSE;

	return(bReval);
}
//��1.0.0.1018
