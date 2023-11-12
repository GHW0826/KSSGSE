#pragma once
#ifndef __ILOGICENTRY__
#define __ILOGICENTRY__

#include <map>

class BasePacket;
class INetworkCallback;

class ILogicEntry
{
	typedef std::map<int, INetworkCallback*> mapConnectorCallback;
public:
	ILogicEntry(void) {}
	virtual ~ILogicEntry(void) {}

	virtual bool Initialize() = 0;
	virtual bool ProcessPacket(BasePacket* pPacket) = 0;
	virtual ILogicEntry* Clone() { return nullptr; }

	bool AddConnectorCallback(int identifier, INetworkCallback* pCallback, int packetProtocolId);
	virtual bool ProcessConnectorPacket(BasePacket* pPacket);
protected:
	mapConnectorCallback mapConnectorCallback_;
};


#endif

