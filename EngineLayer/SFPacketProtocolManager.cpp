#include "stdafx.h"
#include "SFPacketProtocolManager.h"
#include "IPacketProtocol.h"

SFPacketProtocolManager::SFPacketProtocolManager()
{}
SFPacketProtocolManager::~SFPacketProtocolManager() {
	for (auto& iter : mapPacketProtocol_) {
		IPacketProtocol* pProtocol = iter.second;
		delete pProtocol;
	}
	mapPacketProtocol_.clear();
}

bool SFPacketProtocolManager::SetConnectorPacketProtocol(int connectorId, int packetProtocolId) {

	for (auto& iter : listConnectorInfo_) {
		if (iter.connectorId == connectorId) {
			iter.packetProtocolId = packetProtocolId;
			return true;
		}
	}

	return false;
}

bool SFPacketProtocolManager::AddListenerInfo(int listenerId, int packetProtocolId) {
	_ListenerInfo info;
	info.listenerId = listenerId;
	info.packetProtocolId = packetProtocolId;

	listListenerInfo_.emplace_back(info);

	// LOG(INFO) << "SFPacketProtocolManager::AddListenerInfo. listenerId: " << listenerId << ", packetProtocolId: " << packetProtocolId;
	// google::FlushLogFiles(google::GLOG_INFO);
	return true;
}

bool SFPacketProtocolManager::AddPacketProtocol(int packetProtocolId, IPacketProtocol* pProtocol) {
	mapPacketProtocol_.insert(std::make_pair(packetProtocolId, pProtocol));
	return true;
}

bool SFPacketProtocolManager::AddConnectorInfo(_ConnectorInfo* pInfo) {
	listConnectorInfo_.push_back(*pInfo);
	return true;
}
