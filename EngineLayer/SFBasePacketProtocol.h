#pragma once
#ifndef __SFBASEPACKETPROTOCOL__
#define __SFBASEPACKETPROTOCOL__

class SFBasePacketProtocol
{
public:
	SFBasePacketProtocol();
	virtual ~SFBasePacketProtocol();

	void CopyBaseProtocol(SFBasePacketProtocol& source);

	inline int GetIOSize() { return ioSize_; }
	inline unsigned short GetPacketSize() { return packetSize_; }
	inline int GetPacketOption() { return packetOption_; }
protected:
	int ioSize_;
	unsigned short packetSize_;
	int packetOption_;
};

#endif 