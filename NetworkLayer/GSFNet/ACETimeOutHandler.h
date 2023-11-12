#pragma once
#ifndef __ACETIMEOUTHANDLER__
#define __ACETIMEOUTHANDLER__

#include "ACEHeader.h"
#include "INetworkEngine.h"
#include "IEngine.h"

class ACETimeOutHandler : public ACE_Handler
{
public:
	ACETimeOutHandler(INetworkEngine* pOwner)
	: pOwner_(pOwner)
	{}
	virtual ~ACETimeOutHandler(void) {}

	virtual void handle_time_out(const ACE_Time_Value& tv,	const void* arg) {
		pOwner_->GetEngine()->OnTimer(arg);
	}
private:
	INetworkEngine* pOwner_;
};


#endif