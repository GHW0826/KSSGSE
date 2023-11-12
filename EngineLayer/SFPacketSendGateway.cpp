#include "stdafx.h"
#include "SFPacketSendGateway.h"


SFPacketSendGateway::SFPacketSendGateway(void)
{
}


SFPacketSendGateway::~SFPacketSendGateway(void)
{
}

bool SFPacketSendGateway::PushTask(IPacketTask* pPacketTask)
{
	return IOCPQueue_.Push(pPacketTask);
}

IPacketTask* SFPacketSendGateway::PopTask(int WaitTime)
{
	return IOCPQueue_.Pop(WaitTime);
}
