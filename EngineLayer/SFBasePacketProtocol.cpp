#include "stdafx.h"
#include "SFBasePacketProtocol.h"


SFBasePacketProtocol::SFBasePacketProtocol()
{
}


SFBasePacketProtocol::~SFBasePacketProtocol()
{
}

void SFBasePacketProtocol::CopyBaseProtocol(SFBasePacketProtocol& source)
{
	ioSize_ = source.GetIOSize();
	packetSize_ = source.GetPacketSize();
	packetOption_ = source.GetPacketOption();
}
