#pragma once
#ifndef __PROACTORSERVICEMANAGER__
#define __PROACTORSERVICEMANAGER__

#include "IDMap.h"
#include "Lock.h"
#include "BasePacket.h"

template <typename LockStrategy, typename T, int MaxIDCount>
class ProactorServiceManager : public SFIDMap<LockStrategy, T, MaxIDCount>
{
public:
	ProactorServiceManager() {}
	virtual ~ProactorServiceManager() {}

	bool SendRequest(BasePacket* pPacket) {

		SFLockHelper lock(&lock_);
		T* pProactorService = SFIDMap<LockStrategy, T, MaxIDCount>::Get(pPacket->GetSerial());
		if (pProactorService != nullptr) {
			pProactorService->SendRequest(pPacket);
		}
		return true;
	}

	bool Disconnect(int serial) {

		SFLockHelper lock(&lock_);
		T* pProactorService = SFIDMap<LockStrategy, T, MaxIDCount>::Get(serial);
		if (pProactorService != nullptr) {
			pProactorService->Disconnect();
			return true;
		}
		return false;
	}

	bool BroadCast(int ownerSerial, int destSerial[], int destSize, char* buffer, unsigned int bufferSize) {

		SFLockHelper lock(&lock_);
		for (int i = 0; i < destSize; i++) {
			T* pProactorService = SFIDMap<LockStrategy, T, MaxIDCount>::Get(destSerial[i]);
			if (pProactorService != nullptr) {
				pProactorService->SendInternal(buffer, bufferSize);
			}
		}

		return true;
	}

	bool BroadCastAll(int ownerSerial, char* buffer, unsigned int bufferSize) {

		SFLockHelper lock(&lock_);
		auto iter = SFIDMap<LockStrategy, T, MaxIDCount>::IDMap_.begin();
		for (; iter != SFIDMap<LockStrategy, T, MaxIDCount>::IDMap_.end(); iter++) {
			int serial = iter.advance();
			T* pProactorService = SFIDMap::Get(serial);
			if (pProactorService != nullptr) {
				pProactorService->SendInternal(buffer, bufferSize);
			}
		}
		return true;
	}

	int Register(T* p) {
		SFLockHelper lock(&lock_);
		return SFIDMap<LockStrategy, T, MaxIDCount>::Register(p);
	}

	void UnRegister(int id) {
		SFLockHelper lock(&lock_);
		SFIDMap<LockStrategy, T, MaxIDCount>::UnRegister(id);
	}
private:
	SFLock lock_;
};

#endif