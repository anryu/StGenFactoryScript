// *****************************************************************************
//
// 
//
// *****************************************************************************

#include "stdafx.h"

#include "StDeviceEBus.h"
#include "CallbackThread.h"
#include <PvPipeline.h>
#include <PvBuffer.h>


// ==========================================================================
CallbackThread::CallbackThread( StDeviceEBus *aDevice )
	: mDevice( aDevice )
	, mHandle( 0 )
	, mID( 0 )
	, mStop( false )
	, mReturnValue( 0 )
{
}

// ==========================================================================
CallbackThread::~CallbackThread()
{
	if ( mHandle != INVALID_HANDLE_VALUE )
	{
		Stop();
	}
}

// ==========================================================================
void CallbackThread::Start()
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
void CallbackThread::Stop()
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
void CallbackThread::SetPriority( int aPriority )
{
	ASSERT( mHandle != INVALID_HANDLE_VALUE );
	::SetThreadPriority( mHandle, aPriority );
}

// ==========================================================================
int CallbackThread::GetPriority() const
{
	ASSERT( mHandle != INVALID_HANDLE_VALUE );
	return ::GetThreadPriority( mHandle );
}

// ==========================================================================
bool CallbackThread::IsStopping() const
{
	ASSERT( mHandle != INVALID_HANDLE_VALUE );
	return mStop;
}

// ==========================================================================
bool CallbackThread::IsDone()
{
	if ( ( mHandle == INVALID_HANDLE_VALUE ) ||
		 ( mID == 0 ) )
	{
		return true;
	}

	return ( ::WaitForSingleObject( mHandle, 0 ) == WAIT_OBJECT_0 );
}

// ==========================================================================
unsigned long WINAPI CallbackThread::Link( void *aParam )
{
	CallbackThread *lThis = reinterpret_cast<CallbackThread *>( aParam );
	lThis->mReturnValue = lThis->Function();
	return lThis->mReturnValue;
}

// ==========================================================================
DWORD CallbackThread::GetReturnValue()
{
	return mReturnValue;
}

// ==========================================================================
DWORD CallbackThread::Function()
{
	//ASSERT( mDisplayWnd != NULL );
	ASSERT( mDevice != NULL );

    // Timestamp used to limit display rate
	//DWORD lPrevious = 0;

	PvPipeline *lPipeline = mDevice->GetPipeline();

//static int count =0;
	for ( ;; )
	{
		// Check if we were signaled to terminate
        if ( IsStopping() )
		{
			break;
		}
//TRACE(TEXT("@@@@@@@@@ RetrieveNextBuffer Count=%d\n"), count++ );
		PvBuffer *lBuffer = NULL;
        PvResult  lOperationResult;
        // Try retrieving a buffer, using default timeout
        PvResult lResult = lPipeline->RetrieveNextBuffer( &lBuffer, 0xFFFFFFFF, &lOperationResult );
        if ( lResult.IsOK() )
        {
			//バッファがたまっている場合解放。
			do
			{
				if( lPipeline->GetOutputQueueSize()==0 ) break;
				PvBuffer *lBuffer2 = NULL;
				PvResult  lOperationResult2;
				lResult = lPipeline->RetrieveNextBuffer( &lBuffer2, 0, &lOperationResult2 );
				if( !lResult.IsOK() ) break;
				if( lOperationResult2.IsOK() )
				{
					lPipeline->ReleaseBuffer( lBuffer );
					lBuffer = lBuffer2;
					lOperationResult = lOperationResult2;
				}
				else
				{
					lPipeline->ReleaseBuffer( lBuffer2 );
				}
			}while(1);

            if ( lOperationResult.IsOK() )
            {
				mDevice->OnImageCallback(lBuffer);
            }
            // Release buffer back to the pipeline
            lPipeline->ReleaseBuffer( lBuffer );
		}
	}

	return 0;
}

