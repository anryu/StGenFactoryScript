#include "StdAfx.h"
#include "CaptureThread.h"
//#include "Preview.h"
//#include "StCustomDevice.h"

//#ifdef _PLEORA
//#include <PvStream.h>
//#else
#include "StDeviceGenICam.h"
//#include "../CommonHeader/StGenDevice.h"
#include "../CommonHeader/StGenStream.h"
//#endif

//CaptureThread::CaptureThread(void)
//CaptureThread::CaptureThread( StImageDisplay *aDisplayWnd, StGenStream *aStream )
// ==========================================================================
//CaptureThread::CaptureThread( CPreview *aDisplayWnd, StGenStream *aStream )
CaptureThread::CaptureThread( StDeviceGenICam *aDevice )
	: mDevice( aDevice )
	, mHandle( 0 )
	, mID( 0 )
	, mStop( false )
	, mReturnValue( 0 )
{
}

// ==========================================================================
CaptureThread::~CaptureThread(void)
{
	if ( mHandle != INVALID_HANDLE_VALUE )
	{
		Stop();
	}
}

// ==========================================================================
void CaptureThread::Start()
{
	mHandle = ::CreateThread(
		NULL,				// Security attributes
		0,					// Stack size, 0 is default
		Link,				// Start address
		this,				// Parameter
		0,					// Creation flags
		&mID );				// Thread ID
}

// ==========================================================================
void CaptureThread::Stop()
{
	ASSERT( mHandle != INVALID_HANDLE_VALUE );

	mStop = true;
	DWORD lRetVal = ::WaitForSingleObject( mHandle, INFINITE );
	ASSERT( lRetVal != WAIT_TIMEOUT  );

	::CloseHandle( mHandle );
	mHandle = INVALID_HANDLE_VALUE;

	mID = 0;
}

// ==========================================================================
void CaptureThread::SetPriority( int aPriority )
{
	ASSERT( mHandle != INVALID_HANDLE_VALUE );
	::SetThreadPriority( mHandle, aPriority );
}

// ==========================================================================
int CaptureThread::GetPriority() const
{
	ASSERT( mHandle != INVALID_HANDLE_VALUE );
	return ::GetThreadPriority( mHandle );
}

// ==========================================================================
bool CaptureThread::IsStopping() const
{
	ASSERT( mHandle != INVALID_HANDLE_VALUE );
	return mStop;
}

// ==========================================================================
bool CaptureThread::IsDone(DWORD dwMilliSeconds)
{
	if ( ( mHandle == INVALID_HANDLE_VALUE ) ||
		 ( mID == 0 ) )
	{
		return true;
	}

	return ( ::WaitForSingleObject( mHandle, dwMilliSeconds ) == WAIT_OBJECT_0 );
}

// ==========================================================================
unsigned long WINAPI CaptureThread::Link( void *aParam )
{
	CaptureThread *lThis = reinterpret_cast<CaptureThread *>( aParam );
	lThis->mReturnValue = lThis->Function();
	return lThis->mReturnValue;
}

// ==========================================================================
DWORD CaptureThread::GetReturnValue()
{
	return mReturnValue;
}

// ==========================================================================
DWORD CaptureThread::Function()
{

    // Timestamp used to limit display rate
	DWORD lPrevious = 0;

	StGenStream * aStream = (StGenStream *)(mDevice->GetGenStream(0));

	for ( ;; )
	{
		// Check if we were signaled to terminate
        if ( IsStopping() )
		{
			//aStream->
TRACE(_T(" @@@@@@@@@@@@@@@@@IsStopping\n"));
			break;
		}

		StBuffer * lBuffer = NULL;
        StResult lResult = aStream->RetrieveBuffer( &lBuffer, 2000 );
        if ( lResult.IsOK() )
        {

			//保存されているバッファを解放
			INT aCount=0;
			lResult = aStream->GetOutputQueueCount(&aCount);
			for( int i=0; i<aCount; i++ )
			{
				StBuffer * lNextBuffer = NULL;
				StResult lNextResult = aStream->RetrieveBuffer( &lNextBuffer, 0 );
				if( lNextResult.IsOK() )
				{
					aStream->QueueBuffer( lBuffer );
					lBuffer = lNextBuffer;
				}
				else
				{
					break;
				}
			}
			mDevice->OnImageCallback(lBuffer);

			//if( mDisplayWnd )
			//{
			//	((CPreview *)mDisplayWnd)->StGenImageDisplay(lBuffer);
			//}
			aStream->QueueBuffer( lBuffer );
		}

	}

	return 0;
}

