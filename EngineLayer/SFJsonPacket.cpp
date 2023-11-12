#include "stdafx.h"
#include "SFJsonPacket.h"
#include "SFJsonProtocol.h"

SFJsonPacket::SFJsonPacket(USHORT usPacketId) {
	packetHeader_.packetID = usPacketId;
}
SFJsonPacket::SFJsonPacket(void)
{}
SFJsonPacket::~SFJsonPacket(void)
{}

BasePacket* SFJsonPacket::Clone() {
	SFJsonPacket* pClone = new SFJsonPacket();
	pClone->CopyBaseHeader(this);
	pClone->packetHeader_ = packetHeader_;
	pClone->node_ = node_;

	return pClone;
}

void SFJsonPacket::Release() {
	SFJsonProtocol::DisposePacket(this);
}