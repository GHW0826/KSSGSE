#pragma once
#ifndef __QUEUE__
#define __QUEUE__
#include <ace/Containers.h>
#include <Assert.h>

template<class T>
class Queue
{
public:
	void Push(T t) {
		Queue_.enqueue_tail(t);
	}

	/////////////////////////////////////////
	// Description:
	//   when the queue is empty, return false
	/////////////////////////////////////////
	bool Pop(T& t) {
		return (-1 != Queue_.dequeue_head(t));
	}
	bool Head(T*& t) {
		return (-1 != Queue_.get(t));
	}
	int Size() {
		return Queue_.size();
	}
protected:
	ACE_Unbounded_Queue<T>	Queue_;
};

constexpr unsigned int INVALID_ID = 0xffffffff;

template<int MaxIDCount>
class IDQueue
{
public:
	IDQueue(int offset) 
		: offset_(offset) {
		for (int i = 0; i < MaxIDCount; ++i)
			idleIdQueue_.Push(i + offset_);
	}

	inline int Pop() {
		int id = INVALID_ID;
		idleIdQueue_.Pop(id);
		return id;
	}
	inline void Push(int id) {
		if (IsValidId(id)) {
			idleIdQueue_.Push(id);
		}
		else {
			assert(0);
		}
	}

	inline bool IsValidId(int id) const {
		return (INVALID_ID != id) && (id >= offset_) && (id < (MaxIDCount + offset_));
	}

private:
	int			offset_;
	Queue<int>	idleIdQueue_;
};


#endif
