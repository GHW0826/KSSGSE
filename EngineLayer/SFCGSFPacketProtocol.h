#pragma once
#ifndef __SFCGSFPACKETPROTOCOL__
#define __SFCGSFPACKETPROTOCOL__

#include "SFBasePacketProtocol.h"

class SFCommand;
class SFPacketIOBuffer;

class SFCGSFPacketProtocol : public SFBasePacketProtocol
{
public:
	SFCGSFPacketProtocol(void);
	virtual ~SFCGSFPacketProtocol(void);

	bool Initialize(int ioBufferSize, unsigned short packetDataSize, int packetOption);

	bool Reset();
	BasePacket* GetPacket(int& ErrorCode);
	bool AddTransferredData(char* pBuffer, DWORD dwTransferred);
	bool Encode(BasePacket* pPacket, char** ppBuffer, int& bufferSize);
	static bool DisposePacket(BasePacket* pPacket);

protected:

private:
	SFPacketIOBuffer* pPacketIOBuffer_;
};



#endif