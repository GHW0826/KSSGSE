#pragma once
#ifndef __SFPACKETSENDGATEWAY__
#define __SFPACKETSENDGATEWAY__
#include "SFIOCPQueue.h"

class IPacketTask;

class SFPacketSendGateway
{
public:
	SFPacketSendGateway(void);
	virtual ~SFPacketSendGateway(void);

	bool PushTask(IPacketTask* pPacketTask);
	IPacketTask* PopTask(int WaitTime = INFINITE);

private:
	SFIOCPQueue<IPacketTask> IOCPQueue_;
};

#endif