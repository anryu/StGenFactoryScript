//#include "TMI.h"
#ifndef _TMI_H
#define _TMI_H

#define CHA	   		1
#define CHB	   		2
#define CHC	   		3
#define CHD	   		4
#define PRESET1 	1
#define PRESET2 	2
#define PRESET3 	3
#define PRESET4 	4
#define DLL_ERROR  -1
#define ON			1
#define OFF			0


class TMI_API_IF
{
public:
	
	long HandleOpen( char * , char * );
	int HandleClose( long );
	int Refresh( long );
	int TimeOut( long, int );
	int Voltage( long , char , char , double );
	int VoltageQ( long , char , char , double *);
	int Current( long , char , char , double );
	int CurrentQ( long , char , char , double *);
	int MainOutput( long , char );
	int MainOutputQ( long , char *);
	int Delay( long , char );
	int DelayQ( long , char *);
	int OutputSel( long , char , char );
	int OutputSelQ( long , char , char *);
	int TrackingOnOff( long , char );
	int TrackingOnOffQ( long , char *);
	int TrackingMode( long , char );
	int TrackingModeQ( long , char *);
	int TrackingGroup( long , char , char );
	int TrackingGroupQ( long , char , char *);
	int TrackingData( long , char , char , double );
	int TrackingDataQ( long , char, char , double * );
	int DelayTime( long , char , double );
	int DelayTimeQ( long , char , double * );
	int Display( long , char );
	int DisplayQ( long , char *);
	int Preset( long , char );
	int PresetQ( long , char *);
	int MoniDataQ( long , char, double * , double * ,char * );
	int AdrQ( long , char * );
	int RemoteLocal( long );
	int LocalLockOut( long );
	int DataBackUp( long );
	int SRQEnable( long, char);
	int ModelNameQ( long , char * );
	int AllPresetQ( long , double * );
    int GetSTB( long, char * ,char * , char * , int * , int *);
	int Out( long , char *);
	int In( long , char *);

	BOOL IFInitialize();

	TMI_API_IF();
	virtual ~TMI_API_IF();
	
	HINSTANCE m_TMI_APIDll;
	long ( __stdcall* m_TMI_HandleOpen )( char* , char* );
	int ( __stdcall* m_TMI_HandleClose )( long );
	int ( __stdcall* m_TMI_Refresh )( long );
	int ( __stdcall* m_TMI_TimeOut )( long , int);
	int ( __stdcall* m_TMI_Voltage )( long , char , char , double );
	int ( __stdcall* m_TMI_VoltageQ )( long , char , char , double * );
	int ( __stdcall* m_TMI_Current )( long , char , char , double );
	int ( __stdcall* m_TMI_CurrentQ )( long , char , char , double * );
	int ( __stdcall* m_TMI_MainOutput )( long , char );
	int ( __stdcall* m_TMI_MainOutputQ )( long , char * );
	int ( __stdcall* m_TMI_Delay )( long , char );
	int ( __stdcall* m_TMI_DelayQ )( long , char * );
	int ( __stdcall* m_TMI_OutputSel )( long , char , char );
	int ( __stdcall* m_TMI_OutputSelQ )( long , char , char * );
	int ( __stdcall* m_TMI_TrackingOnOff )( long , char );
	int ( __stdcall* m_TMI_TrackingOnOffQ )( long , char * );
	int ( __stdcall* m_TMI_TrackingMode )( long , char );
	int ( __stdcall* m_TMI_TrackingModeQ )( long , char * );
	int ( __stdcall* m_TMI_TrackingGroup )( long , char , char );
	int ( __stdcall* m_TMI_TrackingGroupQ )( long , char ,char * );
	int ( __stdcall* m_TMI_TrackingData )( long , char , char , double );
	int ( __stdcall* m_TMI_TrackingDataQ )( long , char ,char , double * );
	int ( __stdcall* m_TMI_DelayTime )( long , char , double );
	int ( __stdcall* m_TMI_DelayTimeQ )( long , char ,double * );
	int ( __stdcall* m_TMI_Display )( long , char );
	int ( __stdcall* m_TMI_DisplayQ )( long , char * );
	int ( __stdcall* m_TMI_Preset )( long , char );
	int ( __stdcall* m_TMI_PresetQ )( long , char * );
	int ( __stdcall* m_TMI_MoniDataQ )( long , char  , double * , double * , char * );
	int ( __stdcall* m_TMI_AdrQ )( long , char * );
	int ( __stdcall* m_TMI_RemoteLocal )( long );
	int ( __stdcall* m_TMI_LocalLockOut )( long );
	int ( __stdcall* m_TMI_DataBackUp )( long );
	int ( __stdcall* m_TMI_SRQEnable )( long , char );
	int ( __stdcall* m_TMI_ModelNameQ )( long , char * );
	int ( __stdcall* m_TMI_AllPresetQ )( long , double * );
	int ( __stdcall* m_TMI_GetSTB )(long, char * , char * , char * , int * , int * );
	int ( __stdcall* m_TMI_Out )( long , char * );
	int ( __stdcall* m_TMI_In )( long , char * );


};

#endif //_TMI_H

