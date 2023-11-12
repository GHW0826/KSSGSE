#pragma once
#ifndef __SFJSONPROTOCOL__
#define __SFJSONPROTOCOL__

#include "Json/JsonBuilder.h"
#include "SFBasePacketProtocol.h"

class ISession;
class BasePacket;
class SFJsonPacket;

class SFJsonProtocol : public SFBasePacketProtocol
{
public:
	SFJsonProtocol(void);
	virtual ~SFJsonProtocol(void);

	bool Initialize(int ioBufferSize, unsigned short packetSize, int packetOption);

	bool Reset();
	BasePacket* GetPacket(int& errorCode);
	bool AddTransferredData(char* pBuffer, DWORD dwTransferred);
	bool Encode(BasePacket* pPacket, char** ppBuffer, int& bufferSize);

	static bool DisposePacket(BasePacket* pPacket);
	BasePacket* CreatePacket();

protected:

private:
	JsonBuilder builder_;
	char* pBuffer_;
	bool GetCompleteNode(SFJsonPacket* pPacket);
};


#endif // #ifndef __SFJSONPROTOCOL__

