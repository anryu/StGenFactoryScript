// PowerCtrl.h: CPowerCtrl �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POWERCTRL_H__737F7043_378C_493D_9690_784127A8363E__INCLUDED_)
#define AFX_POWERCTRL_H__737F7043_378C_493D_9690_784127A8363E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tmi_Api.h"

class CPowerCtrl  
{
public:
	CPowerCtrl();
	virtual ~CPowerCtrl();
	TMI_API_IF	m_TMI_API_IF;
	LONG		m_hTMI;
	BYTE	m_byteChannelForCamera;
//	BYTE	m_byteChannelForJigu;
	DWORD	m_dwPCAddress,m_dwSystemAddress;



	//��2007/06/01 v0.46 Beta6 �W�b�^�����ύX(���s�ʒu�A�����l�A�ҋ@����)
	//�d���𓊓����Ă���̌o�ߎ��Ԃ��L��
	DWORD m_dwPowerOnTime;

	DWORD	GetCameraTickCount()
	{
		DWORD dwTmp = GetTickCount();
		if(dwTmp < m_dwPowerOnTime)
		{
			dwTmp = 0xFFFFFFFF - m_dwPowerOnTime;
			dwTmp++;
		}
		else
		{
			dwTmp -= m_dwPowerOnTime;
		}
		return(dwTmp);
	};
	//��2007/06/01 v0.46 Beta6 �W�b�^�����ύX(���s�ʒu�A�����l�A�ҋ@����)

	BOOL	bConnect(void);
	//��1.0.0.1069
	BOOL	IsConnected(void){return m_hTMI>=0;}
	void    SetPCAddress(DWORD dwAddress){m_dwPCAddress = dwAddress;}
	void    SetSystemAddress(DWORD dwAddress){m_dwSystemAddress = dwAddress;}
	//��1.0.0.1069
	BOOL	bDisconnect(void);
	BOOL	bGetCurrent(DOUBLE *pdblCurrent);
	//��1.0.0.1018
	BOOL	bGetCurrent(BYTE byteChannel,DOUBLE *pdblCurrent);
	//��1.0.0.1018
	BOOL	bPowerOn(void);
	BOOL	bPowerOff(void);
	//��1.0.0.1018
	BOOL	bPowerOn(BYTE byteChannel);
	BOOL	bPowerOff(BYTE byteChannel);
	//��1.0.0.1018
	BOOL	bSetVoltage(double dblVoltage);
	//��1.3.0.9
	BOOL	bGetVoltage(double *pdblVoltage);
	//��1.3.0.9

	//��1.0.0.1013
	BYTE	bGetChannel(void){return m_byteChannelForCamera;}
	void	bSetChannel(BYTE aChannel){m_byteChannelForCamera = aChannel;}
	//��1.0.0.1013
	//��1.0.0.1060
	BOOL bSetCurrentLimit(double dblCurrentLimit);
	BOOL bGetCurrentLimit(double *pdblCurrentLimit);
	double m_dblCurrentLimit;
	//��1.0.0.1060
	//��1.0.0.1018
	BOOL	bMainPowerOn(void);
	BOOL	bMainPowerOff(void);
	//��1.0.0.1018

};

#endif // !defined(AFX_POWERCTRL_H__737F7043_378C_493D_9690_784127A8363E__INCLUDED_)
