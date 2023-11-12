#pragma once
#ifndef __SFLOCK__
#define __SFLOCK__

class SFLock
{
public:
	SFLock(void) {
		InitializeCriticalSection(&CS_);
	}

	virtual ~SFLock(void) {
		DeleteCriticalSection(&CS_);
	}

	void Lock() { 
		EnterCriticalSection(&CS_);
	}
	void Unlock() {
		LeaveCriticalSection(&CS_);
	}

protected:

private:
	CRITICAL_SECTION CS_;
};

class SFLockHelper
{
public:
	SFLockHelper(SFLock* pCS)
	{
		pCS->Lock();
		pCS_ = pCS;
	}

	virtual ~SFLockHelper(void)
	{
		pCS_->Unlock();
	}

protected:

private:
	SFLock* pCS_;
};

#endif