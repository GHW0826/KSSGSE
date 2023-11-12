#include "stdafx.h"
#include "SFLogicGateway.h"

SFLogicGateway::SFLogicGateway(void)
{}
SFLogicGateway::~SFLogicGateway(void)
{}

bool SFLogicGateway::PushPacket(BasePacket* pPacket) {
	return IOCPQueue_.Push(pPacket);
}

BasePacket* SFLogicGateway::PopPacket(int waitTime) {
	return IOCPQueue_.Pop(waitTime);
}