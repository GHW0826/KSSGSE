#pragma once
#ifndef __IRPCSERVICE__
#define __IRPCSERVICE__

class SFPacket;

class IRPCService
{
public:
	IRPCService() {}
	virtual ~IRPCService() {}

	virtual void ProcessRPCService(SFPacket* pSFPacket) = 0;
};


#endif