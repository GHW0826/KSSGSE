#pragma once
#ifndef __IDMAP__
#define __IDMAP__

#include <ace/Map_Manager.h>
#include "Queue.h"

template <typename LockStrategy, typename T, int MaxIDCount>
class SFIDMap
{
protected:
	using IDMap = ACE_Map_Manager<int, T*, LockStrategy>;
public:
	SFIDMap(void) : idleIdQueue_(1) {}
	virtual ~SFIDMap(void) {}

	int Register(T* p) {
		unsigned int id = idleIdQueue_.Pop();
		if (id == INVALID_ID)
			return INVALID_ID;
		IDMap_.bind(id, p);
		return id;
	}

	void UnRegister(int id) {
		T* t = nullptr;
		if (-1 != IDMap_.find(id, t)) {
			IDMap_.unbind(id);
			idleIdQueue_.Push(id);
		}
	}

	T* Get(int id) {
		T* t = nullptr;
		if (-1 == IDMap_.find(id, t))
			return nullptr;
		return t;
	}

	int Size() {
		return (int)IDMap_.current_size();
	}

protected:
	IDMap IDMap_;
	IDQueue<MaxIDCount> idleIdQueue_;
};


#endif