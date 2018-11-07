#pragma once
#include <vector>
#define _PLEORA

#ifdef _PLEORA

#include <PvSystem.h>

#else
#include "../CommonHeader/StGenDeviceManager.h"
#include "../CommonHeader/StGenInterface.h"
#endif
//#include "StCustomDevice.h"
#include "StDeviceEBus.h"
#include "StDeviceMultiCam.h"
//▼1.0.0.1044
#include "StDeviceGenICam.h"
//▲1.0.0.1044
#include "StCheckBase.h"
#include "StSerialComm.h"
#include "StTexioControl.h"
#include "StImageMessageDlg.h"
#include "StStruct.h"
//▼1.0.0.1023 繰返し用変数追加
#include "StValue.h"
//▲1.0.0.1023 繰返し用変数追加
//▼1.0.0.1025
#include "StCommonBase.h"
//▲1.0.0.1025
//▼1.0.0.1052
#include "StInterfaceGenICam.h"
//▲1.0.0.1052

//▼1.0.0.1062
//#include "ErrorListDlg.h"
//▲1.0.0.1062


//typedef struct _CHECKBASE_INFO
//{
//	StCheckBase *aCheckBase;
//	StString varName;
//}CHECKBASE_INFO, *PCHECKBASE_INFO;

class StInspectMain
{
public:
	//StInspectMain(void);
	StInspectMain::StInspectMain(void *pParent = NULL);
	~StInspectMain(void);

	void SetScriptFileName(LPCTSTR pszFileName){mszScriptFileName = pszFileName;}
	//▼1.0.0.1061
	void SetSubScriptPath(LPCTSTR pszFileName){mszSubScriptPath = pszFileName;}
	//▲1.0.0.1061
	//▼1.0.0.1069
	void ClearScriptModelFileNames(void)
	{
		m_vScriptModelFileNames.clear();
	}
	void AddScriptModelFileName(LPCTSTR pszFileName)
	{
		m_vScriptModelFileNames.push_back(pszFileName);
	}
	//▲1.0.0.1069
	//▼1.0.0.1072
	void ClearCheckListIndex(void)
	{
		m_vCheckListIndex.clear();
	}
	void AddCheckListIndex(INT nIdex)
	{
		m_vCheckListIndex.push_back(nIdex);
	}

	void ClearCheckMode(void)
	{
		m_vCheckMode.clear();
	}
	void AddCheckMode(INT nCheckMode)
	{
		m_vCheckMode.push_back(nCheckMode);
	}
	//▲1.0.0.1072

	//▼1.0.0.1072a
	//void AddScriptFileName(LPCTSTR pszFileName)
	//{
	//	m_vScriptFileNames.push_back(pszFileName);
	//}
	void AddScriptFileInfo(LPCTSTR pszFileInfo)
	{
		m_vScriptFileInfos.push_back(pszFileInfo);
	}
	//▲1.0.0.1072a

	BOOL Execute(void);
	BOOL WaitForInspection(DWORD dwTimeout);
	BOOL GetResult(PDWORD pdwResult);
	virtual DWORD Function();
	//void SetParent(void *pParent){mpParent=pParent;}
	void DisplayListBox(LPCTSTR szText);

	void SetCheckMode(unsigned int uiCheckMode){mCheckMode = uiCheckMode;}
	void SetFactoryMode(unsigned int uiFactoryMode){mFactoryMode = uiFactoryMode;}

	void SetStructPortName(CStructPortName *pStructPortName){m_pStructPortName=pStructPortName;}
	CStructPortName * GetStructPortName(void){return m_pStructPortName;}

	//▼1.0.0.1017
	void SetStructAddress(CStructPortName *pStructAddress){m_pStructAddress=pStructAddress;}
	CStructPortName * GetStructAddress(void){return m_pStructAddress;}
	//▲1.0.0.1017

	//▼1.0.0.1020
	void SetStructLightChannel(CStructLightChannel *pStructLightChannel){m_pStructLightChannel=pStructLightChannel;}
	CStructLightChannel * GetStructLightChannel(void){return m_pStructLightChannel;}
	//▲1.0.0.1020

	//▼1.0.0.1062
	void SetScriptCheckMode(int nScriptCheckMode){m_nScriptCheckMode = nScriptCheckMode;}
	INT GetErrorListCount(void){return m_vScriptCheckErrorText.size();}
	BOOL GetErrorListString(CString &szText, INT nIndex)
	{
		if( nIndex>=m_vScriptCheckErrorText.size() ) return FALSE;
		szText = m_vScriptCheckErrorText[nIndex];
		return TRUE;
	}
	//▲1.0.0.1062

	//PTouch
	void *GetPTouch()
	{
		void *aPTouch = NULL;
		for( unsigned int i=0; i<m_vecCheckBase.size(); i++ )
		{
			if( m_vecCheckBase[i]->m_ClassName==_T("StPTouch") )
			{
				aPTouch = (void *)m_vecCheckBase[i];
				break;
			}
		}
		return aPTouch;
	}
	BOOL IsExecutePrintPTouch(void){return m_bExecutePrintPTouch;}
	void SetExecutePrintPTouch(BOOL bData){m_bExecutePrintPTouch = bData;}
	void SetPTouchErrorCode(INT nCode){m_nPTouchErrorCode = nCode;}

	//▼1.0.0.1008
	INT GetRetryMode(void){return m_nRetryMode;}
	//▲1.0.0.1008

	//▼1.0.0.1023 繰返し用変数追加
	INT GetNextLine(void){INT nNextLine = m_nNextLine;m_nNextLine=-1;return nNextLine;}
	//▲1.0.0.1023 繰返し用変数追加
	//▼1.0.0.1026
	void SetSoftwareVersion(LPCTSTR szSoftwareVersion){m_szSoftwareVersion = szSoftwareVersion;}
	//▲1.0.0.1026

protected:
	static unsigned WINAPI Link( void *aParam );

private:
	HANDLE mHandle;
	unsigned int mID;
	//bool mStop;
	DWORD mReturnValue;
	void * mpParent;
	CString mszScriptFileName;
	//▼1.0.0.1061
	CString mszSubScriptPath;
	//▲1.0.0.1061
	//▼1.0.0.1069
	std::vector<CString> m_vScriptModelFileNames;
	//▲1.0.0.1069
	//▼1.0.0.1072
	std::vector<INT> m_vCheckListIndex;
	std::vector<INT> m_vCheckMode;
	//▲1.0.0.1072

	//▼1.0.0.1072a
	//std::vector<CString> m_vScriptFileNames;
	std::vector<CString> m_vScriptFileInfos;
	//▲1.0.0.1072a


	//コメントアウト部-----------------------------
	unsigned int mCheckMode;	//0:出荷検査 1:工程内検査......	検査開始後には変更できない
	unsigned int mFactoryMode;	//0:Japan 1:Korea......	検査開始後には変更できない
	//bool mbCommentout;
	//std::vector<unsigned int> m_vCommentout;	//数値 D0:出荷検査 D1:工程内検査

	//▼1.0.0.1062
	INT m_nScriptCheckMode;
	BOOL CheckVariableName(CString szVarName);
	//▲1.0.0.1062

	//Execut Function------------------
	//Return 0:クラス宣言かどうか　次ExecuteFunction実行
	//Return 1:クラスの実行かどうか　次ExecuteClassFunction実行
	//Return 2:クラス以外の実行かどうか　　次ExecuteDeclaration実行

	//スクリプト記述をLOGに記述するかどうか
	INT m_nWriteScriptLogMode;

	//▼1.0.0.1008
	INT m_nRetryMode;
	INT m_nNGContinuousMode;
	//▲1.0.0.1008

	//▼1.0.0.1009 排他処理の為
	StBuffer m_DisplayBuffer;
	HANDLE m_DisplayBufferMutex;
	//▲1.0.0.1009

	//▼1.0.0.1012 NG終了時に実行するコマンド
	std::vector<CString> m_vNGEndProcessString;
	//▲1.0.0.1012

	//▼1.0.0.1013 スクリプト終了時メッセージダイアログを表示するかどうかのフラグ
	INT m_nNoOKMessage;
	INT m_nNoNGMessage;
	//▲1.0.0.1013

	//▼1.0.0.1025
	BOOL SepareteString(CString szInputString, CString &szClass, CString &szVariable, CString &szArgument );
	//▲1.0.0.1025
	BOOL SepareteString(CString &szInputString, CString &szVariable, CString &szArgument );
	//▼1.0.0.1072d
#ifdef GENCOMP_OLD
	BOOL ExecuteDeclaration(CString &szString);
#else
	BOOL ExecuteDeclaration(CString &szString, PBOOL pbCheck=NULL);
#endif
	//▲1.0.0.1072d
	//▼1.0.0.1023
	//BOOL ExecuteClassFunction(CString &szString);
	BOOL ExecuteClassFunction(CString &szString, INT nScriptLine=-1);
	//▲1.0.0.1023
	BOOL ExecuteFunction(CString &szString);
	//▼1.0.0.1023 繰返し用変数追加
	//BOOL ExecuteValueFunction(CString &szString);
	//▲1.0.0.1023 繰返し用変数追加
	//▼1.0.0.1026
	CString m_szSoftwareVersion;
	//▲1.0.0.1026

	//----Data--------------------
	//▼1.0.0.1069
	BOOL ScriptModelFileLoad(void);
	//▲1.0.0.1069

	BOOL ScriptFileLoad();
	//▼1.0.0.1061
	//▼1.0.0.1062
	//BOOL pushScriptLine(CString &szLine, std::vector<INT> &vCommentout, bool *pbCommentout);
	//▼1.0.0.1072
	//BOOL pushScriptLine(CString &szLine, std::vector<INT> &vCommentout, bool *pbCommentout, UINT unLineIndex, UINT unFileIndex=0);
	BOOL pushScriptLine(CString &szLine, std::vector<INT> &vCommentout, bool *pbCommentout, UINT unLineIndex, UINT unFileIndex=0, INT nCheckLineIndex=-1);
	//▲1.0.0.1072
	//▲1.0.0.1062
	//▼1.0.0.1069
	//BOOL CallScriptFile(CString &szLine, std::vector<INT> &vCommentout, bool *pbCommentout);
	BOOL CallScriptFile(CString &szLine, std::vector<INT> &vCommentout, bool *pbCommentout, bool *pbError=NULL);
	//▲1.0.0.1069
	std::vector<CString> m_vScriptString;
	//▼1.0.0.1062
	std::vector<CString> m_vScriptFilename; 
	std::vector<UINT> m_vScriptLineIndex; //31:24 FileIndex(Sub 1～)  23:0 LineIndex
	std::vector<CString> m_vScriptCheckErrorText;
	//CErrorListDlg *m_pErrorListDlg;
	//▲1.0.0.1062
	//▼1.0.0.1072
	std::vector<INT> m_vScriptCheckLineIndex;
	//▲1.0.0.1072

	//Camera
	CString mDeviceModelName;

	//
	CString mSerialNumber;
	//▼1.0.0.1015
	CString mHinmokuCode;
	CString mSensorSerialNumber;
	//▲1.0.0.1015
	CString mSaveFilePath;
	CString mLogFilePath;
	CString mLogFileName;
	CString mCurrentDateFormat;	//Rename用に保存しておく
	BOOL CreateLogPathName(void);
	BOOL CreateLogFileName(void);
	BOOL OutputLogFile(LPCTSTR szText);
	//▼1.0.0.1015
	BOOL LogFileNameDeleteNG(void);
	//▲1.0.0.1015


//#ifdef _PLEORA
	PvDeviceInfo * DiscoveryDeviceInfo( PvSystem &aSystem, LPCTSTR aCameraTypeName, INT nTimeout, StDeviceEBus *aDeviceEBus=NULL);


//#else
//	StGenDeviceManager mDeviceManager;
//	StGenDeviceInfo * DiscoveryDeviceInfo( StGenDeviceManager &aDeviceManager, CCameraType * aCameraType, INT nTimeout, StGenInterface **aInterface);
//#endif
	//StCustomDevice mCustomDevice;
	StDeviceEBus mCustomDevice;
	void ImageCallback( StBuffer *aBuffer, void *pValue );

	//Device -------------------
	std::vector <StDeviceBase *> m_vecDeviceBase;

	//Inspection-----------------------------
	std::vector <StCheckBase *> m_vecCheckBase;

	//SerialComm-----------------------------
	//▼1.0.0.1017
	//std::vector <StSerialComm *> m_vecSerialComm;
	//▼1.0.0.1020 クラス継承の元のハンドルを保存
	//std::vector <StControl *> m_vecSerialComm;
	std::vector <StControl *> m_vecControl;
	//▲1.0.0.1020
	//▲1.0.0.1017

	//TexioControl-----------------------------
	std::vector <StTexioControl *> m_vecTexioControl;

	//▼1.0.0.1048
	//ImageMessageDlg-----------------------------
	//std::vector <StImageMessageDlg *> m_vecImageMessageDlg;
	std::vector <CCustomDialogEx *> m_vecCustomDlg;
	//▲1.0.0.1048

	//▼1.0.0.1025
	std::vector <StCommonBase *> m_vecCommonBase;
	//▲1.0.0.1025

	//▼1.0.0.1023 繰返し用変数追加
	std::vector <StValue *> m_vecValue;
	//▼1.0.0.1072d
#ifdef GENCOMP_OLD
	BOOL StValueFunction(LPCTSTR szString);
#else
	BOOL StValueFunction(LPCTSTR szString, PBOOL pbCheck);
#endif
	//▲1.0.0.1072d
	INT GetLoopEndLine(LPCTSTR szVarName, INT nStart);
	INT m_nNextLine;

	BOOL Operation( CString szString, double &dblValue, BOOL &bVariable );
	void ChangeArgument( CString &szString );
	//▲1.0.0.1023 繰返し用変数追加

	//▼1.0.0.1030
	INT GetIFEndLine(LPCTSTR szVarName, INT nStart);
	INT GetIFElseIfLine(LPCTSTR szVarName, INT nStart, INT nEnd);
	INT GetIFElseLine(LPCTSTR szVarName, INT nStart, INT nEnd);
	//▲1.0.0.1030

	//▼1.0.0.1025
	BOOL GetCheckFunction(LPCTSTR szString, CString &szValue, double &dblValue, INT &nMode);
	//▼1.0.0.1072d
#ifdef GENCOMP_OLD
	BOOL GetDeviceFunction(LPCTSTR szString, CString &szValue, double &dblValue, INT &nMode);
	BOOL GetFunction(CString szString, CString &szValue);
#else
	BOOL GetDeviceFunction(LPCTSTR szString, CString &szValue, double &dblValue, INT &nMode, PBOOL pbCheck );
	BOOL GetFunction(CString szString, CString &szValue, PBOOL pbCheck);
#endif
	//▲1.0.0.1072d
	BOOL ChangeStringToOutputString(CString szSrc, CString &szDst);
	//▲1.0.0.1025

	//▼1.0.0.1031
	BOOL GetControlFunction(LPCTSTR szString, CString &szValue, double &dblValue, INT &nMode);
	//▲1.0.0.1031

	BOOL DeleteDeviceBase(INT nDeviceBaseIndex);
	BOOL DeleteSerialComm(INT nSerialCommIndex);
	BOOL DeleteTexioControl(INT nTexioControlIndex);
	BOOL DeleteCheckBase(INT nCheckBaseIndex);
	//▼1.0.0.1048
	//BOOL DeleteImageMessageDlg(INT nImageMessageDlgIndex);
	BOOL DeleteCustomDlg(INT nCustomDlgIndex);
	//▲1.0.0.1048
	//▼1.0.0.1023 繰返し用変数追加
	BOOL DeleteValue(INT nValueIndex);
	//▲1.0.0.1023 繰返し用変数追加
	//▼1.0.0.1025
	BOOL DeleteCommonBase(INT nIndex);
	//▲1.0.0.1025

	void AllDelete(void);
	//▼1.0.0.1020
	StCheckBase * GetCheckBase(LPCTSTR szName);
	//▲1.0.0.1020
	StDeviceBase * GetDeviceBase(LPCTSTR szName);
	//▼1.0.0.1017
	//StSerialComm * GetSerialComm(LPCTSTR szName);
	StControl * GetSerialComm(LPCTSTR szName);
	//▲1.0.0.1017
	StTexioControl * GetTexioControl(LPCTSTR szName);
	//▼1.0.0.1052
	StCommonBase * GetCommonBase(LPCTSTR szName);
	//▲1.0.0.1052

	//ErrorMessage
	CString szErrorMessage;


	//LogファイルPATHNAME、FILENAME
	//CString mszLogFileName;	//model + "_" + serial + "_" + date + ".log"
	//CString mszLogFilePath;	//mszLogFilePath + "\\" + model + "\\" + date + "\\log"
	//CString mszLogFileFull; //mszLogFilePath + "\\" + model + "\\" + date + "\\log\\" + model + "_" + serial + "_" + date + ".log"

	//SerialPort (JIGU)
	CStructPortName *m_pStructPortName;

	//▼1.00.1017
	CStructPortName *m_pStructAddress;
	//▲1.00.1017
	//▼1.00.1020
	CStructLightChannel *m_pStructLightChannel;
	//▲1.00.1020

	//PTouch
	BOOL m_bExecutePrintPTouch;
	INT m_nPTouchErrorCode;

	//eBUS-------------------------------
	std::vector <void *> m_vecControlDlg;
	
	BOOL ConnectEBus(StDeviceEBus *pDevice, LPCTSTR szArgument);
	BOOL DisonnectEBus(StDeviceEBus *pDevice);
	BOOL StartEBus(StDeviceEBus *pDevice);
	BOOL StopEBus(StDeviceEBus *pDevice);
	BOOL ControlDialogEBus(StDeviceEBus *pDevice, LPCTSTR szArgument);
	BOOL SerialControlEBus( StDeviceEBus *pDevice, LPCTSTR szPort, LPCTSTR szData );
	BOOL SetGenICamControlEBus( StDeviceEBus *pDevice, LPCTSTR szPort, LPCTSTR szData );

	//MultiCam-------------------------------
	BOOL OpenMultiCam(StDeviceMultiCam *pDevice);
	BOOL CloseMultiCam(StDeviceMultiCam *pDevice);
	BOOL StartMultiCam(StDeviceMultiCam *pDevice);
	BOOL StopMultiCam(StDeviceMultiCam *pDevice);

	//▼1.0.0.1044
	//GenICam-------------------------------
	BOOL OpenGenICam(StDeviceGenICam *pDevice, LPCTSTR szArgument);
	BOOL CloseGenICam(StDeviceGenICam *pDevice);
	BOOL StartGenICam(StDeviceGenICam *pDevice);
	BOOL StopGenICam(StDeviceGenICam *pDevice);
	//▲1.0.0.1044

	//▼1.0.0.1047
	INT m_nRuledLine;
	//▲1.0.0.1047

	//▼1.0.0.1072d
#ifdef GENCOMP_OLD
#else
	//GenICam通信の値(結果)
	StString m_szResultGenICamControlCommandName;	//コマンド名
	INT m_nResultGenICamControlValueMode;			//0: 数値 1:文字列
	StString m_szResultGenICamControlValue;
	double m_dblResultGenICamControlValue;
#endif
	//▲1.0.0.1072d
};

