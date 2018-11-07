// *****************************************************************************
//
// 
//
// *****************************************************************************

#pragma once

class StDeviceGenICam;

class CaptureThread
{
public:
	CaptureThread( StDeviceGenICam *aDevice );
	~CaptureThread(void);
	
	void Start();
	void Stop();

	void SetPriority( int aPriority );
	int GetPriority() const;

	bool IsDone(DWORD dwMilliSeconds = 0);
	DWORD GetReturnValue();

    StDeviceGenICam *mDevice;

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

