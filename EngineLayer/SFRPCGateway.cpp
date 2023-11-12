#include "stdafx.h"
#include "SFRPCGateway.h"


SFRPCGateway::SFRPCGateway()
{}
SFRPCGateway::~SFRPCGateway()
{}

BOOL SFRPCGateway::PushPacket(BasePacket* pPacket) {
	return IOCPQueue_.Push(pPacket);
}

BasePacket* SFRPCGateway::PopPacket(int WaitTime) {
	return IOCPQueue_.Pop(WaitTime);
}
