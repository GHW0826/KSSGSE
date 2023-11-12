#pragma once
#ifndef __SFOBJECTPOOL__
#define __SFOBJECTPOOL__

#include <queue>
#include "SFLock.h"

template<typename T> class SFObjectPool
{
public:
	SFObjectPool(int MaxCount) {
		Init(MaxCount);
	}

	SFObjectPool() {}

	virtual ~SFObjectPool(void);

	void Init(int MaxCount);

	T* Alloc();
	bool Release(T* pObj);

protected:

private:
	SFLock Lock_;
	std::queue<T*> Queue_;
};

template<typename T>
void SFObjectPool<T>::Init(int MaxCount)
{
	SFASSERT(MaxCount > 0);

	for (int i = 0; i < MaxCount; i++) {
		T* pObj = new T;
		Queue_.push(pObj);
	}
}

template<typename T>
SFObjectPool<T>::~SFObjectPool()
{
	T* pObj = nullptr;
	while (!Queue_.empty()) {
		pObj = Queue_.front();
		delete pObj;
		Queue_.pop();
	}
}

template<typename T> T* SFObjectPool<T>::Alloc()
{
	SFLockHelper Helper(&Lock_);

	T* pObj = nullptr;

	if (Queue_.empty()) {
		pObj = new T;
		return pObj;
	}

	pObj = Queue_.front();
	Queue_.pop();

	return pObj;
}


template<typename T> bool SFObjectPool<T>::Release(T* pObj)
{
	if (nullptr == pObj)
		return false;

	SFLockHelper Helper(&Lock_);

	Queue_.push(pObj);

	return true;
}

#endif