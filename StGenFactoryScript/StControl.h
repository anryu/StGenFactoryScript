#pragma once
#include "../CommonHeader/StString.h"

class StControl
{
public:
	StControl(void);
	StControl( LPCTSTR szClassName, LPCTSTR szVarName );
	~StControl(void);

//protected:
	//�N���X��
	StString m_ClassName;	//StSerialComm or StTerminal
	//�ϐ���
	StString m_VarName;	//aSerialComm or aTerminal

	//��1.0.0.1062
	void SetScriptCheckMode(INT nScriptCheckMode){m_nScriptCheckMode=nScriptCheckMode;}
	//��1.0.0.1062

	//��1.0.0.1020 �N���X�p���̌��̃n���h����ۑ�
	//StSerialCom�N���X��StControl�ŃL���X�g����ƃA�h���X���ς���Ă��܂�����
	void SetMainHandle(HANDLE aHandle){m_MainHandle = aHandle;}
	HANDLE GetMainHandle(void){return m_MainHandle;}

	//��1.0.0.1062
protected:
	INT m_nScriptCheckMode;
	//��1.0.0.1062

private:
	HANDLE m_MainHandle;

	void Initialize(void);

	//��1.0.0.1020
};

