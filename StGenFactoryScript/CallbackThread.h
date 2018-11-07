// *****************************************************************************
//
// 
//
// *****************************************************************************

#pragma once

class StDeviceEBus;

class CallbackThread
{
public:

	CallbackThread( StDeviceEBus *aDevice );
	~CallbackThread();

	void Start();
	void Stop();

	void SetPriority( int aPriority );
	int GetPriority() const;

	bool IsDone();
	DWORD GetReturnValue();

    StDeviceEBus *mDevice;

protected:

	static unsigned long WINAPI Link( void *aParam );
	virtual DWORD Function();

	bool IsStopping() const;

private:

	HANDLE mHandle;
	DWORD mID;

	bool mStop;

	DWORD mReturnValue;

};
