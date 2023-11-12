#pragma once
#ifndef __SINGLETONOBJECT__
#define __SINGLETONOBJECT__

#include "ACEHeader.h"
#include "ProactorService.h"
#include "ProactorServiceManager.h"

constexpr int MAX_CGSF_CONCURRENT_USER = 10000;

using ProactorServiceManagerSinglton =
	ACE_Singleton<ProactorServiceManager<ACE_Null_Mutex, ProactorService, MAX_CGSF_CONCURRENT_USER>, ACE_Thread_Mutex>;


#endif