#include "stdafx.h"		//Visual C++で作成する場合、追加して下さい。
#include "Tmi_Api.h"

//////////////////////////////
// 			構築			//
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
//			消滅			//
//////////////////////////////

TMI_API_IF::~TMI_API_IF()
{
	if(m_TMI_APIDll != NULL)
		FreeLibrary(m_TMI_APIDll);
}

//////////////////////////////////////////////////
// 関数名 :	IFInitialize						//
// 機能   : IFの初期化、						//
//			ライブラリ及び関数のハンドルの取得	//
// 引数	  : なし								//
// 戻り値 : TRUE	正常終了					//
//		    FAULSE	異常終了					//
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
// 関数名:HandleOpen							//
// 機能  :ハンドルをオープンさせる				//
// 引数  :char *str=電源種別					//
//		 :chat *ptr=ボードNo.,PCアドレス		//
//				    サブアドレス				//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////
long TMI_API_IF::HandleOpen( char *str , char *ptr )
{

	if(m_TMI_HandleOpen == NULL) return DLL_ERROR;

	return m_TMI_HandleOpen(str,ptr);

}

//////////////////////////////////////////////////
// 関数名:HandleCLOSE							//
// 機能  :ハンドルをクローズさせる				//
// 引数  :long hID=ID No.						//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::HandleClose( long hID )
{
	if(m_TMI_HandleClose == NULL) return DLL_ERROR;

	return m_TMI_HandleClose( hID );

}

//////////////////////////////////////////////////
// 関数名:Refresh								//
// 機能  :Refreshさせる							//
// 引数  :long hID=ID No.						//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::Refresh( long hID )
{
	if(m_TMI_Refresh == NULL) return DLL_ERROR;

	return m_TMI_Refresh( hID );

}

//////////////////////////////////////////////////
// 関数名:TimeOut								//
// 機能  :Time Out時間設定						//
// 引数  :int Time:タイムアウト時間				//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::TimeOut( long hID, int Time )
{
	if(m_TMI_TimeOut == NULL) return DLL_ERROR;

	return m_TMI_TimeOut( hID , Time );

}

//////////////////////////////////////////////////
// 関数名:Voltage								//
// 機能  :電圧値の設定							//
// 引数  :long hID=ID No.						//
//		 :char ch=出力CH						//
//		 :char preset=PRESET位置				//
//		 :double data=設定データ				//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::Voltage( long hID , char ch , char preset ,double data )

{
	if(m_TMI_Voltage == NULL) return DLL_ERROR;
	return m_TMI_Voltage( hID , ch , preset , data );

}

//////////////////////////////////////////////////
// 関数名:VoltageQ								//
// 機能  :電圧値の設定クエリ					//
// 引数  :long hID=ID No.						//
//		 :char ch=出力CH						//
//		 :char preset=PRESET位置				//
// 戻り値:double *data=設定値					//
//		 :0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::VoltageQ( long hID , char ch , char preset ,double *data )

{
	if(m_TMI_VoltageQ == NULL) return DLL_ERROR;
	return m_TMI_VoltageQ( hID , ch , preset , data );

}

//////////////////////////////////////////////////
// 関数名:Current								//
// 機能  :電流値の設定							//
// 引数  :long hID=ID No.						//
//		 :char ch=出力CH						//
//		 :char preset=PRESET位置				//
//		 :double data=設定データ				//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::Current( long hID , char ch , char preset ,double data )

{
	if(m_TMI_Voltage == NULL) return DLL_ERROR;
	return m_TMI_Current( hID , ch , preset , data );

}

//////////////////////////////////////////////////
// 関数名:CurrentQ								//
// 機能  :電流値の設定クエリ					//
// 引数  :long hID=ID No.						//
//		 :char ch=出力CH						//
//		 :char preset=PRESET位置				//
// 戻り値:double *data=設定値					//
//		 :0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::CurrentQ( long hID , char ch , char preset ,double *data )

{
	if(m_TMI_CurrentQ == NULL) return DLL_ERROR;
	return m_TMI_CurrentQ( hID , ch , preset , data );

}

//////////////////////////////////////////////////
// 関数名:MainOutput							//
// 機能  :出力のON OFF							//
// 引数  :long hID=ID No.						//
//		 :char onoff=出力のONOFF				//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::MainOutput( long hID , char onoff )

{
	if(m_TMI_MainOutput == NULL) return DLL_ERROR;
	return m_TMI_MainOutput( hID , onoff );

}

//////////////////////////////////////////////////
// 関数名:MainOutputQ							//
// 機能  :出力のON OFFのクエリ					//
// 引数  :long hID=ID No.						//
//		 :char *ans=クエリ						//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::MainOutputQ( long hID , char *ans )

{
	if(m_TMI_MainOutputQ == NULL) return DLL_ERROR;
	return m_TMI_MainOutputQ( hID , ans );

}

//////////////////////////////////////////////////
// 関数名:Delay									//
// 機能  :DelayのON OFF							//
// 引数  :long hID=ID No.						//
//		 :char onoff=DelayのONOFF				//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::Delay( long hID , char onoff )

{
	if(m_TMI_Delay == NULL) return DLL_ERROR;
	return m_TMI_Delay( hID , onoff );

}

//////////////////////////////////////////////////
// 関数名:DelayQ								//
// 機能  :DelayのON OFFのクエリ					//
// 引数  :long hID=ID No.						//
//		 :char *ans=クエリ						//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::DelayQ( long hID , char *ans )

{
	if(m_TMI_DelayQ == NULL) return DLL_ERROR;
	return m_TMI_DelayQ( hID , ans );

}

//////////////////////////////////////////////////
// 関数名:OutputSel								//
// 機能  :OutputSelectのON OFF					//
// 引数  :long hID=ID No.						//
//		 :char ch=CHA:1,CHB:2,CHC:3,CHD:4		//
//		 :char onoff=出力のONOFF				//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::OutputSel( long hID , char ch , char onoff )

{
	if(m_TMI_OutputSel == NULL) return DLL_ERROR;
	return m_TMI_OutputSel( hID , ch , onoff );

}
//////////////////////////////////////////////////
// 関数名:OutputSelQ							//
// 機能  :OutputSelectのクエリ					//
// 引数  :long hID=ID No.						//
//		 :char ch=CHA:1,CHB:2,CHC:3,CHD:4		//
//		 :char *ans=クエリ						//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::OutputSelQ( long hID , char ch , char *ans )

{
	if(m_TMI_OutputSelQ == NULL) return DLL_ERROR;
	return m_TMI_OutputSelQ( hID , ch , ans );

}


//////////////////////////////////////////////////
// 関数名:TrackingOnOff							//
// 機能  :TrackingのON OFF						//
// 引数  :long hID=ID No.						//
//		 :char onoff=出力のONOFF				//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::TrackingOnOff( long hID , char onoff )

{
	if(m_TMI_TrackingOnOff == NULL) return DLL_ERROR;
	return m_TMI_TrackingOnOff( hID , onoff );

}

//////////////////////////////////////////////////
// 関数名:TrackingOnOffQ						//
// 機能  :TrackingのON OFFのクエリ				//
// 引数  :long hID=ID No.						//
//		 :char *ans=クエリ						//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::TrackingOnOffQ( long hID , char *ans )

{
	if(m_TMI_TrackingOnOffQ == NULL) return DLL_ERROR;
	return m_TMI_TrackingOnOffQ( hID , ans );

}

//////////////////////////////////////////////////
// 関数名:TrackingMode							//
// 機能  :TrackingModeの切り替え				//
// 引数  :long hID=ID No.						//
//		 :char mode=0:abs, 1:%					//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::TrackingMode( long hID , char mode )

{
	if(m_TMI_TrackingMode == NULL) return DLL_ERROR;
	return m_TMI_TrackingMode( hID , mode );

}

//////////////////////////////////////////////////
// 関数名:TrackingModeQ							//
// 機能  :TrackingのModeのクエリ				//
// 引数  :long hID=ID No.						//
//		 :char *ans=クエリ						//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::TrackingModeQ( long hID , char *ans )

{
	if(m_TMI_TrackingModeQ == NULL) return DLL_ERROR;
	return m_TMI_TrackingModeQ( hID , ans );

}

//////////////////////////////////////////////////
// 関数名:TrackingGroup							//
// 機能  :TrackingGroupの設定					//
// 引数  :long hID=ID No.						//
//		 :char ch=出力Ch CHA:1,CHB:2,CHC:3,CHD:4//
//		 :char TrackingSet 0:OFF,1:+,2:-		//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::TrackingGroup( long hID , char ch , char TrackingSet )

{
	if(m_TMI_TrackingGroup == NULL) return DLL_ERROR;
	return m_TMI_TrackingGroup( hID , ch ,TrackingSet );

}

//////////////////////////////////////////////////
// 関数名:TrackingGroupQ						//
// 機能  :TrackingGroupのクエリ					//
// 引数  :long hID=ID No.						//
//		 :char ch=出力Ch CHA:1,CHB:2,CHC:3,CHD:4//
//		 :char *ans=クエリ						//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::TrackingGroupQ( long hID , char ch , char *ans )

{
	if(m_TMI_TrackingGroupQ == NULL) return DLL_ERROR;
	return m_TMI_TrackingGroupQ( hID , ch , ans );

}

//////////////////////////////////////////////////
// 関数名:TrackingData							//
// 機能  :TrackingData増減値の設定			    //
// 引数  :long hID=ID No.						//
//		 :char ch=出力Ch CHA:1,CHB:2,CHC:3,CHD:4//
//		 :char va=0:電圧,1:電流					//
//		 :double Data=増減値データ				//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::TrackingData( long hID , char ch , char va , double Data )

{
	if(m_TMI_TrackingData == NULL) return DLL_ERROR;
	return m_TMI_TrackingData( hID , ch , va , Data );

}

//////////////////////////////////////////////////
// 関数名:TrackingDataQ							//
// 機能  :TrackingData増減値のクエリ			//
// 引数  :long hID=ID No.						//
//		 :char ch=出力Ch CHA:1,CHB:2,CHC:3,CHD:4//
//		 :char va=0:電圧,1:電流					//
//		 										//
// 戻り値::double *Data=増減値データ			//
//         0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::TrackingDataQ( long hID , char ch , char va , double *Data )

{
	if(m_TMI_TrackingDataQ == NULL) return DLL_ERROR;
	return m_TMI_TrackingDataQ( hID , ch , va , Data );

}


//////////////////////////////////////////////////
// 関数名:DelayTime								//
// 機能  :DelayTimeの設定					    //
// 引数  :long hID=ID No.						//
//		 :char ch=出力Ch CHA:1,CHB:2,CHC:3,CHD:4//
//		 :double Data=Delay時間					//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::DelayTime( long hID , char ch , double Data )

{
	if(m_TMI_DelayTime == NULL) return DLL_ERROR;
	return m_TMI_DelayTime( hID , ch , Data );

}

//////////////////////////////////////////////////
// 関数名:DelayTimeQ							//
// 機能  :DelayTimeのクエリ			    		//
// 引数  :long hID=ID No.						//
//		 :char ch=出力Ch CHA:1,CHB:2,CHC:3,CHD:4//
//		 :double Data=Delay時間					//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::DelayTimeQ( long hID , char ch , double *Data )

{
	if(m_TMI_DelayTimeQ == NULL) return DLL_ERROR;
	return m_TMI_DelayTimeQ( hID , ch , Data );

}

//////////////////////////////////////////////////
// 関数名:Display								//
// 機能  :Displayの設定						    //
// 引数  :long hID=ID No.						//
//		 :char ch=出力Ch CHA:1,CHB:2,CHC:3,CHD:4//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::Display( long hID , char ch  )

{
	if(m_TMI_Display == NULL) return DLL_ERROR;
	return m_TMI_Display( hID , ch  );

}

//////////////////////////////////////////////////
// 関数名:DisplayQ								//
// 機能  :Displayのクエリ					    //
// 引数  :long hID=ID No.						//
//		 :char *ans=現在の表示Display			//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::DisplayQ( long hID , char *ans  )

{
	if(m_TMI_DisplayQ == NULL) return DLL_ERROR;
	return m_TMI_DisplayQ( hID , ans  );

}

//////////////////////////////////////////////////
// 関数名:Preset								//
// 機能  :Presetの設定						    //
// 引数  :long hID=ID No.						//
//		 :char preset=PRESET1:1,2:2,3:3,4:4		//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::Preset( long hID , char preset  )

{
	if(m_TMI_Preset == NULL) return DLL_ERROR;
	return m_TMI_Preset( hID , preset  );

}

//////////////////////////////////////////////////
// 関数名:PresetQ								//
// 機能  :Presetのクエリ					    //
// 引数  :long hID=ID No.						//
//		 :char *ans=選択Preset位置				//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::PresetQ( long hID , char *ans  )

{
	if(m_TMI_PresetQ == NULL) return DLL_ERROR;
	return m_TMI_PresetQ( hID , ans  );

}

//////////////////////////////////////////////////
// 関数名:MainDataQ								//
// 機能  :出力モニタ値						    //
// 引数  :long hID=ID No.						//
//		 :char ch=出力CHA:1,CHB:2,CHC:3,CHD:4	//
//		 :double *Voltage=電圧値				//
//		 :double *Current=電流値				//
//		 :char   *cv_cc  =CV/CC					//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::MoniDataQ( long hID , char ch , double *Voltage , double *Current ,char *cv_cc  )

{
	if(m_TMI_MoniDataQ == NULL) return DLL_ERROR;
	return m_TMI_MoniDataQ( hID , ch , Voltage , Current , cv_cc );

}

//////////////////////////////////////////////////
// 関数名:Adr									//
// 機能  :アドレスのクエリ						//
// 引数  :long hID=ID No.						//
//		 :char *adr=システムアドレス値			//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::AdrQ( long hID , char *adr  )

{
	if(m_TMI_AdrQ == NULL) return DLL_ERROR;
	return m_TMI_AdrQ( hID , adr  );

}

//////////////////////////////////////////////////
// 関数名:RemoteLocal							//
// 機能  :リモート・ローカルの設定				//
// 引数  :long hID=ID No.						//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::RemoteLocal( long hID  )

{
	if(m_TMI_RemoteLocal == NULL) return DLL_ERROR;
	return m_TMI_RemoteLocal( hID );

}

//////////////////////////////////////////////////
// 関数名:LocalRockOut							//
// 機能  :ローカルロックアウト					//
// 引数  :long hID=ID No.						//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::LocalLockOut( long hID )

{
	if(m_TMI_LocalLockOut == NULL) return DLL_ERROR;
	return m_TMI_LocalLockOut( hID );

}

//////////////////////////////////////////////////
// 関数名:DataBackUp							//
// 機能  :設定値EEPROMへ書き込み				//
// 引数  :long hID=ID No.						//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::DataBackUp( long hID )

{
	if(m_TMI_DataBackUp == NULL) return DLL_ERROR;
	return m_TMI_DataBackUp( hID );

}

//////////////////////////////////////////////////
// 関数名:SRQEnable								//
// 機能  :サービスリクエストイネーブル			//
// 引数  :long hID=ID No.						//
//		 :char OnOff:1:Enable 0:DisEnable		//
// 戻り値:0=成功,Else=失敗						//
//////////////////////////////////////////////////

int TMI_API_IF::SRQEnable( long hID , char OnOff  )

{
	if(m_TMI_SRQEnable == NULL) return DLL_ERROR;
	return m_TMI_SRQEnable( hID , OnOff  );

}

//////////////////////////////////////////////////
// 関数名:ModelNameQ							//
// 機能  :モデルネームの読み込み				//
// 引数  :long hID=ID No.						//
// 戻り値:0=成功,Else=失敗						//
//		 : char *Model=モデル名					//
//////////////////////////////////////////////////

int TMI_API_IF::ModelNameQ( long hID , char *Model  )

{
	if(m_TMI_ModelNameQ == NULL) return DLL_ERROR;
	return m_TMI_ModelNameQ( hID , Model  );

}

//////////////////////////////////////////////////
// 関数名:GetSTB								//
// 機能  :STBの読み込み							//
// 引数  :char *STB:ステータスバイト			//
//		  char *Query:クエリ					//
//		  char *Time_Stamp:タイムスタンプ		//
//////////////////////////////////////////////////

int TMI_API_IF::GetSTB(long hID, char *STB , char *Query , char *Time_Stamp ,int *PCAdr , int *SubAdr)
{
	if(m_TMI_GetSTB == NULL) return DLL_ERROR;
	return m_TMI_GetSTB( hID,STB ,Query , Time_Stamp , PCAdr , SubAdr );
}

//////////////////////////////////////////////////
// 関数名:AllPrersetQ							//
// 機能  :全てのPresetの読み込み				//
// 引数  :long hID=ID No.						//
// 戻り値:0=成功,Else=失敗						//
//		 : char *Model=モデル名					//
//////////////////////////////////////////////////

int TMI_API_IF::AllPresetQ( long hID , double *Preset_Mem  )

{
	if(m_TMI_AllPresetQ == NULL) return DLL_ERROR;
	return m_TMI_AllPresetQ( hID , Preset_Mem  );

}

//////////////////////////////////////////////////
// 関数名:Out									//
// 機能  :コマンド送信							//
//		  long hID :ID番号						//
//		  char *str:送信コマンド				//
//////////////////////////////////////////////////

int TMI_API_IF::Out( long hID , char *str )
{
	if(m_TMI_Out == NULL) return DLL_ERROR;
	return	m_TMI_Out( hID ,str );
}

//////////////////////////////////////////////////
// 関数名:In									//
// 機能  :メッセージ受信						//
//		  int hID :ID番号						//
//		  char &Query:受信メッセージ			//
//////////////////////////////////////////////////

int TMI_API_IF::In( long hID , char *Query )
{
	if(m_TMI_In == NULL) return DLL_ERROR;
	return	m_TMI_In( hID ,Query );
}

