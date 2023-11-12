#include "InterlockedValue.h"
#include <Windows.h>

InterlockedValue::InterlockedValue()
{}
InterlockedValue::InterlockedValue(long initialValue) 
	: value_(initialValue)
{}
bool InterlockedValue::Acquire()
{
	return CompareExchange(1, 0);
}

void InterlockedValue::Release() {
	Exchange(0);
}

bool InterlockedValue::CompareExchange(long exchange, long comparand) {
	return (comparand == InterlockedCompareExchange(&value_, exchange, comparand));
}

void InterlockedValue::Exchange(long exchange) {
	InterlockedExchange(&value_, exchange);
}

bool InterlockedValue::Compare(long comparand) {
	return (value_ == comparand);
}