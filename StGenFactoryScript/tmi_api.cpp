#include "stdafx.h"		//Visual C++�ō쐬����ꍇ�A�ǉ����ĉ������B
#include "Tmi_Api.h"

//////////////////////////////
// 			�\�z			//
//////////////////////////////

TMI_API_IF::TMI_API_IF()
{
	m_TMI_APIDll        = NULL;
	m_TMI_HandleOpen 	= NULL;
	m_TMI_HandleClose 	= NULL;
	m_TMI_Voltage		= NULL;
	m_TMI_VoltageQ	 	= NULL;
	m_TMI_Current		= NULL;
	m_TMI_CurrentQ		= NULL;
	m_TMI_MainOutput	= NULL;
	m_TMI_MainOutputQ	= NULL;
	m_TMI_Delay			= NULL;
	m_TMI_DelayQ		= NULL;
	m_TMI_OutputSel		= NULL;
	m_TMI_OutputSelQ	= NULL;
	m_TMI_TrackingOnOff = NULL;
	m_TMI_TrackingOnOffQ= NULL;
	m_TMI_TrackingMode  = NULL;
	m_TMI_TrackingModeQ = NULL;
	m_TMI_TrackingGroup = NULL;
	m_TMI_TrackingGroupQ= NULL;
	m_TMI_TrackingData  = NULL;
	m_TMI_TrackingDataQ = NULL;
	m_TMI_DelayTime		= NULL;
	m_TMI_DelayTimeQ	= NULL;
	m_TMI_Display		= NULL;
	m_TMI_DisplayQ		= NULL;
	m_TMI_Preset		= NULL;
	m_TMI_PresetQ		= NULL;
	m_TMI_MoniDataQ		= NULL;
	m_TMI_AdrQ			= NULL;
	m_TMI_RemoteLocal	= NULL;
	m_TMI_LocalLockOut	= NULL;
	m_TMI_DataBackUp	= NULL;
	m_TMI_SRQEnable		= NULL;
	m_TMI_ModelNameQ	= NULL;
	m_TMI_Refresh		= NULL;
	m_TMI_TimeOut		= NULL;
	m_TMI_AllPresetQ	= NULL;
	m_TMI_GetSTB		= NULL;
	m_TMI_Out			= NULL;
	m_TMI_In			= NULL;

}

//////////////////////////////
//			����			//
//////////////////////////////

TMI_API_IF::~TMI_API_IF()
{
	if(m_TMI_APIDll != NULL)
		FreeLibrary(m_TMI_APIDll);
}

//////////////////////////////////////////////////
// �֐��� :	IFInitialize						//
// �@�\   : IF�̏������A						//
//			���C�u�����y�ъ֐��̃n���h���̎擾	//
// ����	  : �Ȃ�								//
// �߂�l : TRUE	����I��					//
//		    FAULSE	�ُ�I��					//
//////////////////////////////////////////////////

BOOL TMI_API_IF::IFInitialize()
{
//	m_TMI_APIDll=LoadLibrary((LPCSTR)"Tmi_Api.dll");
	m_TMI_APIDll=LoadLibrary((LPCWSTR)_T("Tmi_Api.dll"));
	if( m_TMI_APIDll == NULL )
		return FALSE;

	m_TMI_HandleOpen = (long ( __stdcall* )( char* , char* ))
					GetProcAddress(m_TMI_APIDll , "TMI_HandleOpen");
	if( m_TMI_HandleOpen == NULL )
		return FALSE;

	m_TMI_HandleClose = (int ( __stdcall* )( long ))
					GetProcAddress(m_TMI_APIDll , "TMI_HandleClose");
	if( m_TMI_HandleClose == NULL )
		return FALSE;

	m_TMI_Voltage = (int ( __stdcall* )( long , char , char , double ))
					GetProcAddress(m_TMI_APIDll , "TMI_Voltage");
	if( m_TMI_Voltage == NULL )
		return FALSE;

	m_TMI_VoltageQ = (int ( __stdcall* )( long , char , char , double* ))
					GetProcAddress(m_TMI_APIDll , "TMI_VoltageQ");
	if( m_TMI_VoltageQ == NULL )
		return FALSE;

	m_TMI_Current = (int ( __stdcall* )( long , char  , char  , double ))
					GetProcAddress(m_TMI_APIDll , "TMI_Current");
	if( m_TMI_Current == NULL )
		return FALSE;

	m_TMI_CurrentQ = (int ( __stdcall* )( long , char  , char  , double* ))
					GetProcAddress(m_TMI_APIDll , "TMI_CurrentQ");
	if( m_TMI_CurrentQ == NULL )
		return FALSE;

	m_TMI_MainOutput = (int ( __stdcall* )( long , char ))
					GetProcAddress(m_TMI_APIDll , "TMI_MainOutput");
	if( m_TMI_MainOutput == NULL )
		return FALSE;

	m_TMI_MainOutputQ = (int ( __stdcall* )( long , char* ))
					GetProcAddress(m_TMI_APIDll , "TMI_MainOutputQ");
	if( m_TMI_MainOutputQ == NULL )
		return FALSE;

	m_TMI_Delay = (int ( __stdcall* )( long , char ))
					GetProcAddress(m_TMI_APIDll , "TMI_Delay");
	if( m_TMI_Delay == NULL )
		return FALSE;

	m_TMI_DelayQ = (int ( __stdcall* )( long , char* ))
					GetProcAddress(m_TMI_APIDll , "TMI_DelayQ");
	if( m_TMI_DelayQ == NULL )
		return FALSE;

	m_TMI_OutputSel = (int ( __stdcall* )( long , char , char ))
					GetProcAddress(m_TMI_APIDll , "TMI_OutputSel");
	if( m_TMI_OutputSel == NULL )
		return FALSE;

	m_TMI_OutputSelQ = (int ( __stdcall* )( long , char , char* ))
					GetProcAddress(m_TMI_APIDll , "TMI_OutputSelQ");
	if( m_TMI_OutputSelQ == NULL )
		return FALSE;

	m_TMI_TrackingOnOff = (int ( __stdcall* )( long , char ))
					GetProcAddress(m_TMI_APIDll , "TMI_TrackingOnOff");
	if( m_TMI_TrackingOnOff == NULL )
		return FALSE;

	m_TMI_TrackingOnOffQ = (int ( __stdcall* )( long , char* ))
					GetProcAddress(m_TMI_APIDll , "TMI_TrackingOnOffQ");
	if( m_TMI_TrackingOnOffQ == NULL )
		return FALSE;

	m_TMI_TrackingMode = (int ( __stdcall* )( long , char ))
					GetProcAddress(m_TMI_APIDll , "TMI_TrackingMode");
	if( m_TMI_TrackingMode == NULL )
		return FALSE;

	m_TMI_TrackingModeQ = (int ( __stdcall* )( long , char* ))
					GetProcAddress(m_TMI_APIDll , "TMI_TrackingModeQ");
	if( m_TMI_TrackingModeQ == NULL )
		return FALSE;

	m_TMI_TrackingGroup = (int ( __stdcall* )( long , char , char ))
					GetProcAddress(m_TMI_APIDll , "TMI_TrackingGroup");
	if( m_TMI_TrackingGroup == NULL )
		return FALSE;

	m_TMI_TrackingGroupQ = (int ( __stdcall* )( long , char , char* ))
					GetProcAddress(m_TMI_APIDll , "TMI_TrackingGroupQ");
	if( m_TMI_TrackingGroupQ == NULL )
		return FALSE;

	m_TMI_TrackingData = (int ( __stdcall* )( long , char , char , double ))
					GetProcAddress(m_TMI_APIDll , "TMI_TrackingData");
	if( m_TMI_TrackingData == NULL )
		return FALSE;

	m_TMI_TrackingDataQ = (int ( __stdcall* )( long , char , char , double* ))
					GetProcAddress(m_TMI_APIDll , "TMI_TrackingDataQ");
	if( m_TMI_TrackingDataQ == NULL )
		return FALSE;

	m_TMI_DelayTime = (int ( __stdcall* )( long , char , double ))
					GetProcAddress(m_TMI_APIDll , "TMI_DelayTime");
	if( m_TMI_DelayTime == NULL )
		return FALSE;

	m_TMI_DelayTimeQ = (int ( __stdcall* )( long , char , double* ))
					GetProcAddress(m_TMI_APIDll , "TMI_DelayTimeQ");
	if( m_TMI_DelayTimeQ == NULL )
		return FALSE;

	m_TMI_Display = (int ( __stdcall* )( long , char  ))
					GetProcAddress(m_TMI_APIDll , "TMI_Display");
	if( m_TMI_Display == NULL )
		return FALSE;

	m_TMI_DisplayQ = (int ( __stdcall* )( long , char*  ))
					GetProcAddress(m_TMI_APIDll , "TMI_DisplayQ");
	if( m_TMI_DisplayQ == NULL )
		return FALSE;

	m_TMI_Preset = (int ( __stdcall* )( long , char  ))
					GetProcAddress(m_TMI_APIDll , "TMI_Preset");
	if( m_TMI_Preset == NULL )
		return FALSE;

	m_TMI_PresetQ = (int ( __stdcall* )( long , char*  ))
					GetProcAddress(m_TMI_APIDll , "TMI_PresetQ");
	if( m_TMI_PresetQ == NULL )
		return FALSE;

	m_TMI_MoniDataQ = (int ( __stdcall* )( long , char , double * , double * , char * ))
					GetProcAddress(m_TMI_APIDll , "TMI_MoniDataQ");
	if( m_TMI_MoniDataQ == NULL )
		return FALSE;

	m_TMI_AdrQ = (int ( __stdcall* )( long , char* ))
					GetProcAddress(m_TMI_APIDll , "TMI_AdrQ");
	if( m_TMI_AdrQ == NULL )
		return FALSE;

	m_TMI_RemoteLocal = (int ( __stdcall* )( long ))
					GetProcAddress(m_TMI_APIDll , "TMI_RemoteLocal");
	if( m_TMI_RemoteLocal == NULL )
		return FALSE;

	m_TMI_LocalLockOut = (int ( __stdcall* )( long  ))
					GetProcAddress(m_TMI_APIDll , "TMI_LocalLockOut");
	if( m_TMI_LocalLockOut == NULL )		return FALSE;

	m_TMI_DataBackUp = (int ( __stdcall* )( long  ))
					GetProcAddress(m_TMI_APIDll , "TMI_DataBackUp");
	if( m_TMI_DataBackUp == NULL )
		return FALSE;

	m_TMI_SRQEnable = (int ( __stdcall* )( long , char  ))
					GetProcAddress(m_TMI_APIDll , "TMI_SRQEnable");
	if( m_TMI_SRQEnable == NULL )
		return FALSE;

	m_TMI_ModelNameQ = (int ( __stdcall* )( long , char * ))
					GetProcAddress(m_TMI_APIDll , "TMI_ModelNameQ");
	if( m_TMI_ModelNameQ == NULL )
		return FALSE;

	m_TMI_Refresh = (int ( __stdcall* )( long ))
					GetProcAddress(m_TMI_APIDll , "TMI_Refresh");
	if( m_TMI_Refresh == NULL )
		return FALSE;

	m_TMI_TimeOut = (int ( __stdcall* )( long ,int ))
					GetProcAddress(m_TMI_APIDll , "TMI_TimeOut");
	if( m_TMI_TimeOut == NULL )
		return FALSE;

	m_TMI_AllPresetQ = (int ( __stdcall* )( long ,double * ))
					GetProcAddress(m_TMI_APIDll , "TMI_AllPresetQ");
	if( m_TMI_AllPresetQ == NULL )
		return FALSE;

	m_TMI_GetSTB = (int ( __stdcall* )( long , char * , char * , char * , int * ,int *))
					GetProcAddress(m_TMI_APIDll , "TMI_GetSTB");
	if( m_TMI_GetSTB == NULL )
		return FALSE;

	m_TMI_Out = (int ( __stdcall* )( long , char* ))
					GetProcAddress(m_TMI_APIDll , "TMI_Out");
	if( m_TMI_Out == NULL )
		return FALSE;

	m_TMI_In = (int ( __stdcall* )( long  , char* ))
					GetProcAddress(m_TMI_APIDll , "TMI_In");
	if( m_TMI_In == NULL )
		return FALSE;

	return TRUE;
} 

//////////////////////////////////////////////////
// �֐���:HandleOpen							//
// �@�\  :�n���h�����I�[�v��������				//
// ����  :char *str=�d�����					//
//		 :chat *ptr=�{�[�hNo.,PC�A�h���X		//
//				    �T�u�A�h���X				//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////
long TMI_API_IF::HandleOpen( char *str , char *ptr )
{

	if(m_TMI_HandleOpen == NULL) return DLL_ERROR;

	return m_TMI_HandleOpen(str,ptr);

}

//////////////////////////////////////////////////
// �֐���:HandleCLOSE							//
// �@�\  :�n���h�����N���[�Y������				//
// ����  :long hID=ID No.						//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::HandleClose( long hID )
{
	if(m_TMI_HandleClose == NULL) return DLL_ERROR;

	return m_TMI_HandleClose( hID );

}

//////////////////////////////////////////////////
// �֐���:Refresh								//
// �@�\  :Refresh������							//
// ����  :long hID=ID No.						//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::Refresh( long hID )
{
	if(m_TMI_Refresh == NULL) return DLL_ERROR;

	return m_TMI_Refresh( hID );

}

//////////////////////////////////////////////////
// �֐���:TimeOut								//
// �@�\  :Time Out���Ԑݒ�						//
// ����  :int Time:�^�C���A�E�g����				//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::TimeOut( long hID, int Time )
{
	if(m_TMI_TimeOut == NULL) return DLL_ERROR;

	return m_TMI_TimeOut( hID , Time );

}

//////////////////////////////////////////////////
// �֐���:Voltage								//
// �@�\  :�d���l�̐ݒ�							//
// ����  :long hID=ID No.						//
//		 :char ch=�o��CH						//
//		 :char preset=PRESET�ʒu				//
//		 :double data=�ݒ�f�[�^				//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::Voltage( long hID , char ch , char preset ,double data )

{
	if(m_TMI_Voltage == NULL) return DLL_ERROR;
	return m_TMI_Voltage( hID , ch , preset , data );

}

//////////////////////////////////////////////////
// �֐���:VoltageQ								//
// �@�\  :�d���l�̐ݒ�N�G��					//
// ����  :long hID=ID No.						//
//		 :char ch=�o��CH						//
//		 :char preset=PRESET�ʒu				//
// �߂�l:double *data=�ݒ�l					//
//		 :0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::VoltageQ( long hID , char ch , char preset ,double *data )

{
	if(m_TMI_VoltageQ == NULL) return DLL_ERROR;
	return m_TMI_VoltageQ( hID , ch , preset , data );

}

//////////////////////////////////////////////////
// �֐���:Current								//
// �@�\  :�d���l�̐ݒ�							//
// ����  :long hID=ID No.						//
//		 :char ch=�o��CH						//
//		 :char preset=PRESET�ʒu				//
//		 :double data=�ݒ�f�[�^				//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::Current( long hID , char ch , char preset ,double data )

{
	if(m_TMI_Voltage == NULL) return DLL_ERROR;
	return m_TMI_Current( hID , ch , preset , data );

}

//////////////////////////////////////////////////
// �֐���:CurrentQ								//
// �@�\  :�d���l�̐ݒ�N�G��					//
// ����  :long hID=ID No.						//
//		 :char ch=�o��CH						//
//		 :char preset=PRESET�ʒu				//
// �߂�l:double *data=�ݒ�l					//
//		 :0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::CurrentQ( long hID , char ch , char preset ,double *data )

{
	if(m_TMI_CurrentQ == NULL) return DLL_ERROR;
	return m_TMI_CurrentQ( hID , ch , preset , data );

}

//////////////////////////////////////////////////
// �֐���:MainOutput							//
// �@�\  :�o�͂�ON OFF							//
// ����  :long hID=ID No.						//
//		 :char onoff=�o�͂�ONOFF				//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::MainOutput( long hID , char onoff )

{
	if(m_TMI_MainOutput == NULL) return DLL_ERROR;
	return m_TMI_MainOutput( hID , onoff );

}

//////////////////////////////////////////////////
// �֐���:MainOutputQ							//
// �@�\  :�o�͂�ON OFF�̃N�G��					//
// ����  :long hID=ID No.						//
//		 :char *ans=�N�G��						//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::MainOutputQ( long hID , char *ans )

{
	if(m_TMI_MainOutputQ == NULL) return DLL_ERROR;
	return m_TMI_MainOutputQ( hID , ans );

}

//////////////////////////////////////////////////
// �֐���:Delay									//
// �@�\  :Delay��ON OFF							//
// ����  :long hID=ID No.						//
//		 :char onoff=Delay��ONOFF				//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::Delay( long hID , char onoff )

{
	if(m_TMI_Delay == NULL) return DLL_ERROR;
	return m_TMI_Delay( hID , onoff );

}

//////////////////////////////////////////////////
// �֐���:DelayQ								//
// �@�\  :Delay��ON OFF�̃N�G��					//
// ����  :long hID=ID No.						//
//		 :char *ans=�N�G��						//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::DelayQ( long hID , char *ans )

{
	if(m_TMI_DelayQ == NULL) return DLL_ERROR;
	return m_TMI_DelayQ( hID , ans );

}

//////////////////////////////////////////////////
// �֐���:OutputSel								//
// �@�\  :OutputSelect��ON OFF					//
// ����  :long hID=ID No.						//
//		 :char ch=CHA:1,CHB:2,CHC:3,CHD:4		//
//		 :char onoff=�o�͂�ONOFF				//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::OutputSel( long hID , char ch , char onoff )

{
	if(m_TMI_OutputSel == NULL) return DLL_ERROR;
	return m_TMI_OutputSel( hID , ch , onoff );

}
//////////////////////////////////////////////////
// �֐���:OutputSelQ							//
// �@�\  :OutputSelect�̃N�G��					//
// ����  :long hID=ID No.						//
//		 :char ch=CHA:1,CHB:2,CHC:3,CHD:4		//
//		 :char *ans=�N�G��						//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::OutputSelQ( long hID , char ch , char *ans )

{
	if(m_TMI_OutputSelQ == NULL) return DLL_ERROR;
	return m_TMI_OutputSelQ( hID , ch , ans );

}


//////////////////////////////////////////////////
// �֐���:TrackingOnOff							//
// �@�\  :Tracking��ON OFF						//
// ����  :long hID=ID No.						//
//		 :char onoff=�o�͂�ONOFF				//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::TrackingOnOff( long hID , char onoff )

{
	if(m_TMI_TrackingOnOff == NULL) return DLL_ERROR;
	return m_TMI_TrackingOnOff( hID , onoff );

}

//////////////////////////////////////////////////
// �֐���:TrackingOnOffQ						//
// �@�\  :Tracking��ON OFF�̃N�G��				//
// ����  :long hID=ID No.						//
//		 :char *ans=�N�G��						//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::TrackingOnOffQ( long hID , char *ans )

{
	if(m_TMI_TrackingOnOffQ == NULL) return DLL_ERROR;
	return m_TMI_TrackingOnOffQ( hID , ans );

}

//////////////////////////////////////////////////
// �֐���:TrackingMode							//
// �@�\  :TrackingMode�̐؂�ւ�				//
// ����  :long hID=ID No.						//
//		 :char mode=0:abs, 1:%					//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::TrackingMode( long hID , char mode )

{
	if(m_TMI_TrackingMode == NULL) return DLL_ERROR;
	return m_TMI_TrackingMode( hID , mode );

}

//////////////////////////////////////////////////
// �֐���:TrackingModeQ							//
// �@�\  :Tracking��Mode�̃N�G��				//
// ����  :long hID=ID No.						//
//		 :char *ans=�N�G��						//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::TrackingModeQ( long hID , char *ans )

{
	if(m_TMI_TrackingModeQ == NULL) return DLL_ERROR;
	return m_TMI_TrackingModeQ( hID , ans );

}

//////////////////////////////////////////////////
// �֐���:TrackingGroup							//
// �@�\  :TrackingGroup�̐ݒ�					//
// ����  :long hID=ID No.						//
//		 :char ch=�o��Ch CHA:1,CHB:2,CHC:3,CHD:4//
//		 :char TrackingSet 0:OFF,1:+,2:-		//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::TrackingGroup( long hID , char ch , char TrackingSet )

{
	if(m_TMI_TrackingGroup == NULL) return DLL_ERROR;
	return m_TMI_TrackingGroup( hID , ch ,TrackingSet );

}

//////////////////////////////////////////////////
// �֐���:TrackingGroupQ						//
// �@�\  :TrackingGroup�̃N�G��					//
// ����  :long hID=ID No.						//
//		 :char ch=�o��Ch CHA:1,CHB:2,CHC:3,CHD:4//
//		 :char *ans=�N�G��						//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::TrackingGroupQ( long hID , char ch , char *ans )

{
	if(m_TMI_TrackingGroupQ == NULL) return DLL_ERROR;
	return m_TMI_TrackingGroupQ( hID , ch , ans );

}

//////////////////////////////////////////////////
// �֐���:TrackingData							//
// �@�\  :TrackingData�����l�̐ݒ�			    //
// ����  :long hID=ID No.						//
//		 :char ch=�o��Ch CHA:1,CHB:2,CHC:3,CHD:4//
//		 :char va=0:�d��,1:�d��					//
//		 :double Data=�����l�f�[�^				//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::TrackingData( long hID , char ch , char va , double Data )

{
	if(m_TMI_TrackingData == NULL) return DLL_ERROR;
	return m_TMI_TrackingData( hID , ch , va , Data );

}

//////////////////////////////////////////////////
// �֐���:TrackingDataQ							//
// �@�\  :TrackingData�����l�̃N�G��			//
// ����  :long hID=ID No.						//
//		 :char ch=�o��Ch CHA:1,CHB:2,CHC:3,CHD:4//
//		 :char va=0:�d��,1:�d��					//
//		 										//
// �߂�l::double *Data=�����l�f�[�^			//
//         0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::TrackingDataQ( long hID , char ch , char va , double *Data )

{
	if(m_TMI_TrackingDataQ == NULL) return DLL_ERROR;
	return m_TMI_TrackingDataQ( hID , ch , va , Data );

}


//////////////////////////////////////////////////
// �֐���:DelayTime								//
// �@�\  :DelayTime�̐ݒ�					    //
// ����  :long hID=ID No.						//
//		 :char ch=�o��Ch CHA:1,CHB:2,CHC:3,CHD:4//
//		 :double Data=Delay����					//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::DelayTime( long hID , char ch , double Data )

{
	if(m_TMI_DelayTime == NULL) return DLL_ERROR;
	return m_TMI_DelayTime( hID , ch , Data );

}

//////////////////////////////////////////////////
// �֐���:DelayTimeQ							//
// �@�\  :DelayTime�̃N�G��			    		//
// ����  :long hID=ID No.						//
//		 :char ch=�o��Ch CHA:1,CHB:2,CHC:3,CHD:4//
//		 :double Data=Delay����					//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::DelayTimeQ( long hID , char ch , double *Data )

{
	if(m_TMI_DelayTimeQ == NULL) return DLL_ERROR;
	return m_TMI_DelayTimeQ( hID , ch , Data );

}

//////////////////////////////////////////////////
// �֐���:Display								//
// �@�\  :Display�̐ݒ�						    //
// ����  :long hID=ID No.						//
//		 :char ch=�o��Ch CHA:1,CHB:2,CHC:3,CHD:4//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::Display( long hID , char ch  )

{
	if(m_TMI_Display == NULL) return DLL_ERROR;
	return m_TMI_Display( hID , ch  );

}

//////////////////////////////////////////////////
// �֐���:DisplayQ								//
// �@�\  :Display�̃N�G��					    //
// ����  :long hID=ID No.						//
//		 :char *ans=���݂̕\��Display			//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::DisplayQ( long hID , char *ans  )

{
	if(m_TMI_DisplayQ == NULL) return DLL_ERROR;
	return m_TMI_DisplayQ( hID , ans  );

}

//////////////////////////////////////////////////
// �֐���:Preset								//
// �@�\  :Preset�̐ݒ�						    //
// ����  :long hID=ID No.						//
//		 :char preset=PRESET1:1,2:2,3:3,4:4		//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::Preset( long hID , char preset  )

{
	if(m_TMI_Preset == NULL) return DLL_ERROR;
	return m_TMI_Preset( hID , preset  );

}

//////////////////////////////////////////////////
// �֐���:PresetQ								//
// �@�\  :Preset�̃N�G��					    //
// ����  :long hID=ID No.						//
//		 :char *ans=�I��Preset�ʒu				//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::PresetQ( long hID , char *ans  )

{
	if(m_TMI_PresetQ == NULL) return DLL_ERROR;
	return m_TMI_PresetQ( hID , ans  );

}

//////////////////////////////////////////////////
// �֐���:MainDataQ								//
// �@�\  :�o�̓��j�^�l						    //
// ����  :long hID=ID No.						//
//		 :char ch=�o��CHA:1,CHB:2,CHC:3,CHD:4	//
//		 :double *Voltage=�d���l				//
//		 :double *Current=�d���l				//
//		 :char   *cv_cc  =CV/CC					//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::MoniDataQ( long hID , char ch , double *Voltage , double *Current ,char *cv_cc  )

{
	if(m_TMI_MoniDataQ == NULL) return DLL_ERROR;
	return m_TMI_MoniDataQ( hID , ch , Voltage , Current , cv_cc );

}

//////////////////////////////////////////////////
// �֐���:Adr									//
// �@�\  :�A�h���X�̃N�G��						//
// ����  :long hID=ID No.						//
//		 :char *adr=�V�X�e���A�h���X�l			//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::AdrQ( long hID , char *adr  )

{
	if(m_TMI_AdrQ == NULL) return DLL_ERROR;
	return m_TMI_AdrQ( hID , adr  );

}

//////////////////////////////////////////////////
// �֐���:RemoteLocal							//
// �@�\  :�����[�g�E���[�J���̐ݒ�				//
// ����  :long hID=ID No.						//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::RemoteLocal( long hID  )

{
	if(m_TMI_RemoteLocal == NULL) return DLL_ERROR;
	return m_TMI_RemoteLocal( hID );

}

//////////////////////////////////////////////////
// �֐���:LocalRockOut							//
// �@�\  :���[�J�����b�N�A�E�g					//
// ����  :long hID=ID No.						//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::LocalLockOut( long hID )

{
	if(m_TMI_LocalLockOut == NULL) return DLL_ERROR;
	return m_TMI_LocalLockOut( hID );

}

//////////////////////////////////////////////////
// �֐���:DataBackUp							//
// �@�\  :�ݒ�lEEPROM�֏�������				//
// ����  :long hID=ID No.						//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::DataBackUp( long hID )

{
	if(m_TMI_DataBackUp == NULL) return DLL_ERROR;
	return m_TMI_DataBackUp( hID );

}

//////////////////////////////////////////////////
// �֐���:SRQEnable								//
// �@�\  :�T�[�r�X���N�G�X�g�C�l�[�u��			//
// ����  :long hID=ID No.						//
//		 :char OnOff:1:Enable 0:DisEnable		//
// �߂�l:0=����,Else=���s						//
//////////////////////////////////////////////////

int TMI_API_IF::SRQEnable( long hID , char OnOff  )

{
	if(m_TMI_SRQEnable == NULL) return DLL_ERROR;
	return m_TMI_SRQEnable( hID , OnOff  );

}

//////////////////////////////////////////////////
// �֐���:ModelNameQ							//
// �@�\  :���f���l�[���̓ǂݍ���				//
// ����  :long hID=ID No.						//
// �߂�l:0=����,Else=���s						//
//		 : char *Model=���f����					//
//////////////////////////////////////////////////

int TMI_API_IF::ModelNameQ( long hID , char *Model  )

{
	if(m_TMI_ModelNameQ == NULL) return DLL_ERROR;
	return m_TMI_ModelNameQ( hID , Model  );

}

//////////////////////////////////////////////////
// �֐���:GetSTB								//
// �@�\  :STB�̓ǂݍ���							//
// ����  :char *STB:�X�e�[�^�X�o�C�g			//
//		  char *Query:�N�G��					//
//		  char *Time_Stamp:�^�C���X�^���v		//
//////////////////////////////////////////////////

int TMI_API_IF::GetSTB(long hID, char *STB , char *Query , char *Time_Stamp ,int *PCAdr , int *SubAdr)
{
	if(m_TMI_GetSTB == NULL) return DLL_ERROR;
	return m_TMI_GetSTB( hID,STB ,Query , Time_Stamp , PCAdr , SubAdr );
}

//////////////////////////////////////////////////
// �֐���:AllPrersetQ							//
// �@�\  :�S�Ă�Preset�̓ǂݍ���				//
// ����  :long hID=ID No.						//
// �߂�l:0=����,Else=���s						//
//		 : char *Model=���f����					//
//////////////////////////////////////////////////

int TMI_API_IF::AllPresetQ( long hID , double *Preset_Mem  )

{
	if(m_TMI_AllPresetQ == NULL) return DLL_ERROR;
	return m_TMI_AllPresetQ( hID , Preset_Mem  );

}

//////////////////////////////////////////////////
// �֐���:Out									//
// �@�\  :�R�}���h���M							//
//		  long hID :ID�ԍ�						//
//		  char *str:���M�R�}���h				//
//////////////////////////////////////////////////

int TMI_API_IF::Out( long hID , char *str )
{
	if(m_TMI_Out == NULL) return DLL_ERROR;
	return	m_TMI_Out( hID ,str );
}

//////////////////////////////////////////////////
// �֐���:In									//
// �@�\  :���b�Z�[�W��M						//
//		  int hID :ID�ԍ�						//
//		  char &Query:��M���b�Z�[�W			//
//////////////////////////////////////////////////

int TMI_API_IF::In( long hID , char *Query )
{
	if(m_TMI_In == NULL) return DLL_ERROR;
	return	m_TMI_In( hID ,Query );
}

