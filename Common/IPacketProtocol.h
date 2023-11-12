#pragma once
#ifndef __IPACKETPROTOCOL__
#define __IPACKETPROTOCOL__

#include "EngineStructure.h"

class ISession;
class BasePacket;

class IPacketProtocol
{
public:
	IPacketProtocol() {}
	virtual ~IPacketProtocol() {}
	virtual IPacketProtocol* Clone() = 0;

	virtual bool OnReceive(int Serial, char* pBuffer, unsigned int dwTransferred, _SessionDesc& pDesc) = 0;
	virtual bool Encode(BasePacket* pPacket, char** ppBuffer, int& bufferSize) = 0;
	// virtual BasePacket* CreatePacket() = 0;

	virtual bool DisposePacket(BasePacket* pPacket) = 0;
};

#endif