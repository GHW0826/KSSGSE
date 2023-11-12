#pragma once
#ifndef __SFRPCGATEWAY__
#define __SFRPCGATEWAY__

#include "SFIOCPQueue.h"

class BasePacket;

class SFRPCGateway
{
public:
	SFRPCGateway(void);
	virtual ~SFRPCGateway(void);

	BOOL PushPacket(BasePacket* pPacket);
	BasePacket* PopPacket(int WaitTime = INFINITE);

private:
	SFIOCPQueue<BasePacket> IOCPQueue_;
	//MPSCQueue<SFCommand> m_IOCPQueue; //
};

#endif

