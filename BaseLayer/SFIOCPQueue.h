#pragma once
#ifndef __SFIOCPQUEUE__
#define __SFIOCPQUEUE__

template <typename T>
class SFIOCPQueue
{
public:
	SFIOCPQueue(void)
	{
		Initialize();
	}

	virtual ~SFIOCPQueue(void)
	{
		Finally();
	}

	bool Push(T* pMsg)
	{
		if (0 == ::PostQueuedCompletionStatus(hIOCP_, 0, (ULONG_PTR)pMsg, NULL)) {
			//DWORD dwError =GetLastError();
			return false;
		}
		return true;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	T* Pop()
	{
		ULONG_PTR pCompletionKey = 0;
		DWORD NumberOfBytesTransferred = 0;
		LPOVERLAPPED pOverlapped = NULL;

		::GetQueuedCompletionStatus(hIOCP_, &NumberOfBytesTransferred, &pCompletionKey, &pOverlapped, INFINITE);

		return (T*)(pCompletionKey);
	}

	T* Pop(int Wait = INFINITE)
	{
		ULONG_PTR pCompletionKey = 0;
		DWORD NumberOfBytesTransferred = 0;
		LPOVERLAPPED pOverlapped = NULL;

		//SFASSERT( FALSE != ::GetQueuedCompletionStatus(m_hIOCP, &NumberOfBytesTransferred, &pCompletionKey, &pOverlapped, Wait));

		if (FALSE != ::GetQueuedCompletionStatus(hIOCP_, &NumberOfBytesTransferred, &pCompletionKey, &pOverlapped, Wait))
			return (T*)(pCompletionKey);

		return nullptr;
	}

protected:
	//virtual
	bool Initialize()
	{
		hIOCP_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
		return hIOCP_ != NULL;
	}

	//virtual 
	bool Finally()
	{
		CloseHandle(hIOCP_);
		return true;
	}

private:
	HANDLE hIOCP_;
};

#endif