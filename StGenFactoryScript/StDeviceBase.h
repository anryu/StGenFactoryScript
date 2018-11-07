#pragma once
#include "../CommonHeader/StString.h"
#include "../CommonHeader/StBuffer.h"



class ListenerBase{
public:
    virtual void Invoke(StBuffer* pBuffer, void *pUser) = 0;    
};

template <class T>
class MemFuncListener : public ListenerBase
{
public:
    MemFuncListener(T* obj, void (T::*cbf)(StBuffer* , void *)){
        m_obj = obj;
        m_cbf = cbf;
    };
    
	virtual void Invoke(StBuffer* pBuffer, void *pUser)
	{
        (m_obj->*m_cbf)(pBuffer,pUser);
    };
private:
    T* m_obj;
    void (T::*m_cbf)(StBuffer* , void *);
};


class StDeviceBase
{
public:
	StDeviceBase(void);
	StDeviceBase(LPCTSTR szClassName, LPCTSTR szVarName);
	virtual ~StDeviceBase(void);

	//クラス名
	StString m_ClassName;		//StDeviceEbus
	//変数名
	StString m_VarName;			//aDevice

	//StBufferの取得（コピー）
	BOOL GetStBuffer(StBuffer *aOutBuffer);

	//------------------------------------------------------------------
	BOOL GetDeviceModelName(StString &aDeviceModelName)
	{
		aDeviceModelName = mDeviceModelName;
		return TRUE;
	}
	BOOL GetSerialNumber(StString &aSerialNumber)
	{
		aSerialNumber = mSerialNumber;
		return TRUE;
	}
	//▼1.0.0.1015 移動
	BOOL GetHinmokuCode(StString &aHinmokuCode)
	{
		aHinmokuCode = mHinmokuCode;
		return TRUE;
	}
	//▲1.0.0.1015
	//------------------------------------------------------------------
	template <class T>
	void RegisterCallback(T *obj, void (T::*cbf)(StBuffer*, void *), void *pUser )
	{
		UnregisterCallback();
		m_Listener = new MemFuncListener<T>(obj, cbf);
		m_pUser = pUser;
	}

	void UnregisterCallback()
	{
		if( m_Listener )
		{
			delete m_Listener;
			m_Listener = NULL;
		}
		m_pUser = NULL;
	}

	//▼1.0.0.1015 移動
	//GigEの場合Connect時に読み込みして設定
	//モデル名
	void SetDeviceModelName(LPCTSTR szText){mDeviceModelName = szText;}
	//シリアル番号
	void SetSerialNumber(LPCTSTR szText){mSerialNumber = szText;}
	//品目コード
	void SetHinmokuCode(LPCTSTR szText){mHinmokuCode = szText;}
	//▲1.0.0.1015

	//▼1.0.0.1036
	BOOL WaitFrame(INT nWaitFrame, INT nTimeout);
	//▲1.0.0.1036

	//▼1.0.0.1062
	BOOL VirturalConnect(void)
	{
		BOOL bReval = !m_bVirturalConnect;
		m_bVirturalConnect=TRUE;
		return bReval;
	}
	BOOL VirturalDisconnect(void)
	{
		BOOL bReval = m_bVirturalConnect;
		m_bVirturalConnect=FALSE;
		return bReval;
	}
	BOOL VirturalStart(void)
	{
		BOOL bReval = FALSE;
		if( m_bVirturalConnect && !m_bVirturalStart )
			bReval = TRUE;
		m_bVirturalStart=TRUE;
		return bReval;
	}
	BOOL VirturalStop(void)
	{
		BOOL bReval = FALSE;
		if( m_bVirturalConnect && m_bVirturalStart )
			bReval = TRUE;
		m_bVirturalStart=FALSE;
		return bReval;
	}
	BOOL IsVirturalConnected(void){return m_bVirturalConnect;}
	BOOL IsVirturalStarted(void){return m_bVirturalStart;}
	//▲1.0.0.1062

	//▼1.0.0.1062
	void SetScriptCheckMode(INT nScriptCheckMode){m_nScriptCheckMode=nScriptCheckMode;}
	//▲1.0.0.1062


	//▼1.0.0.1072d
#ifdef GENCOMP_OLD
#else
	void SetCheckGenICamControlMode(INT nCheckGenICamControlMode){m_nCheckGenICamControlMode=nCheckGenICamControlMode;}
	void SetCheckGenICamControlValue(INT nCheckGenICamControlValueMode, double dblCheckGenICamControlValue, LPCTSTR szCheckGenICamControlValue)
	{
		m_nCheckGenICamControlValueMode = nCheckGenICamControlValueMode;
		if( m_nCheckGenICamControlValueMode==0 )
			m_dblCheckGenICamControlValue = dblCheckGenICamControlValue;
		else
		{
			if( szCheckGenICamControlValue )
				m_szCheckGenICamControlValue = szCheckGenICamControlValue;
			else
				m_szCheckGenICamControlValue = _T("");
		}
	}
	INT GetCheckGenICamControlMode(void){return m_nCheckGenICamControlMode;}
	INT GetCheckGenICamControlValueMode(void){return m_nCheckGenICamControlValueMode;}
	double GetCheckGenICamControlValue(void){return m_dblCheckGenICamControlValue;}
	void GetCheckGenICamControlValue(LPCTSTR &szValue)
	{
		szValue = m_szCheckGenICamControlValue.GetUnicode();
	}
#endif
	//▲1.0.0.1072d

private:
	StBuffer *m_OutBuffer;
	HANDLE m_hEvent;
	//▼1.0.0.1036
	void Initialize(void);
	INT m_nWaitFrame;
	HANDLE m_hWaitEvent;
	//▲1.0.0.1036

	//▼1.0.0.1062
	BOOL m_bVirturalConnect;
	BOOL m_bVirturalStart;
	//▲1.0.0.1062

protected:
	//モデル名
	//void SetDeviceModelName(LPCTSTR szText){mDeviceModelName = szText;}
	StString mDeviceModelName;		//STC-
	//シリアル番号
	//void SetSerialNumber(LPCTSTR szText){mSerialNumber = szText;}
	StString mSerialNumber;		    //16D1111
	//品目コード
	StString mHinmokuCode;		    //

	ListenerBase* m_Listener;
	void *m_pUser;

	void OnStBufferCallback(StBuffer *aBuffer);

	//▼1.0.0.1062
	INT m_nScriptCheckMode;
	//▲1.0.0.1062

	//▼1.0.0.1072d
#ifdef GENCOMP_OLD
#else
	INT m_nCheckGenICamControlMode;			//0:Checkしない 1:Checkする 
	double m_dblCheckGenICamControlValue;
	StString m_szCheckGenICamControlValue;
	INT m_nCheckGenICamControlValueMode;	//0: 数値 1:文字列
#endif
	//▲1.0.0.1072d

};

