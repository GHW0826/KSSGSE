#pragma once
#ifndef __LOCK__
#define __LOCk__

#include <Windows.h>

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
private:
	CRITICAL_SECTION CS_;
};

class SFLockHelper
{
public:
	SFLockHelper(SFLock* pCS) {
		pCS->Lock();
		pCS_ = pCS;
	}
	virtual ~SFLockHelper(void) {
		pCS_->Unlock();
	}
private:
	SFLock* pCS_;
};

#endif