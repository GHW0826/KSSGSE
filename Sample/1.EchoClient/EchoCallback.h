#pragma once
#ifndef __ECHOCALLBACK__
#define __ECHOCALLBACK__

#include "INetworkCallback.h"

class EchoCallback : public INetworkCallback
{
public:
	EchoCallback(void);
	virtual ~EchoCallback(void);

	virtual bool HandleNetworkMessage(BasePacket* pPacket) override;
};

#endif