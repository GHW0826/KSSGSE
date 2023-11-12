#pragma once
#ifndef __SFSESSIONSERVICE__
#define __SFSESSIONSERVICE__

#include "ISessionService.h"

class SFSessionService : public ISessionService
{
public:
	SFSessionService(IPacketProtocol* pProtocol) : ISessionService(pProtocol) {}
	virtual ~SFSessionService(void) {}

	virtual bool OnReceive(char* pData, unsigned short Length, _SessionDesc& desc) override;
	//virtual bool SendRequest(ISession *pSession, BasePacket* pPacket) override;
};

#endif