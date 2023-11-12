#pragma once
#ifndef __SFPACKETPROTOCOLMANAGER__
#define __SFPACKETPROTOCOLMANAGER__

#include <list>

class IPacketProtocol;

class SFPacketProtocolManager
{
	typedef std::map<int, IPacketProtocol*> mapPacketProtocol;
	typedef std::list<_ListenerInfo> listListenerInfo;
	typedef std::list<_ConnectorInfo> listConnectorInfo;
public:
	SFPacketProtocolManager();
	virtual ~SFPacketProtocolManager();

	bool AddListenerInfo(int listenerId, int packetProtocolId);
	bool AddPacketProtocol(int packetProtocolId, IPacketProtocol* pProtocol);
	bool AddConnectorInfo(_ConnectorInfo* pInfo);

	bool SetConnectorPacketProtocol(int connectorId, int packetProtocolId);

	IPacketProtocol* GetPacketProtocol(int packetProtocolId) {
		const auto& iter = mapPacketProtocol_.find(packetProtocolId);
		if (iter != mapPacketProtocol_.end())
			return iter->second;
		return nullptr;
	}

	IPacketProtocol* GetPacketProtocolWithListenerId(int listenerId) {
		for (auto& iter : listListenerInfo_) {
			if (iter.listenerId == listenerId)
				return GetPacketProtocol(iter.packetProtocolId);
		}
		return nullptr;
	}

	IPacketProtocol* GetPacketProtocolWithConnectorId(int connectorId) {
		for (auto& iter : listConnectorInfo_) {
			if (iter.connectorId == connectorId)
				return GetPacketProtocol(iter.packetProtocolId);
		}
		return nullptr;
	}

private:
	mapPacketProtocol	mapPacketProtocol_;
	listListenerInfo	listListenerInfo_;
	listConnectorInfo	listConnectorInfo_;
};

#endif
