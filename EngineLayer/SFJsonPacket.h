#pragma once
#include "Json/JsonNode.h"
#include "BasePacket.h"

class SFJsonPacket : public BasePacket
{
	friend class SFJsonProtocol;
public:
	SFJsonPacket(USHORT usPacketId);
	virtual ~SFJsonPacket(void);

	JsonObjectNode&	GetData()			{ return node_; }
	SFPacketHeader* GetHeader()			{ return &packetHeader_; }
	virtual void Release() override;
	virtual BasePacket* Clone() override;
protected:

private:
	SFJsonPacket();

	SFPacketHeader packetHeader_;
	JsonObjectNode node_;
};