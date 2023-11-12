#pragma once
#ifndef __SFLOGICGATEWAY__
#define __SFLOGICGATEWAY__

#include "SFIOCPQueue.h"

class BasePacket;

class SFLogicGateway
{
public:
	SFLogicGateway(void);
	virtual ~SFLogicGateway(void);

	bool PushPacket(BasePacket* pPacket);
	BasePacket* PopPacket(int WaitTime = INFINITE);
private:
	SFIOCPQueue<BasePacket> IOCPQueue_;

};

#endif 
