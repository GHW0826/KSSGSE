#include "stdafx.h"
#include "ILogicEntry.h"

#include "INetworkCallback.h"
#include "SFEngine.h"
#include "SFServerConnectionManager.h"
#include "SFPacketProtocolManager.h"

/*
#include "EngineInterface\INetworkCallback.h"
*/

bool ILogicEntry::ProcessConnectorPacket(BasePacket* pPacket) {

	_SessionDesc& desc = pPacket->GetSessionDesc();
	const auto& callback = mapConnectorCallback_.find(desc.identifier);
	if (callback != mapConnectorCallback_.end()) {
		INetworkCallback* pCallback = callback->second;
		switch (pPacket->GetPacketType()) {
			case SFPACKET_DATA: {
				pCallback->HandleNetworkMessage(pPacket);
				break;
			}
			case SFPACKET_CONNECT: {
				pCallback->HandleConnect(pPacket->GetSerial(), pPacket->GetSessionDesc());
				break;
			}
			case  SFPACKET_DISCONNECT: {
				pCallback->HandleDisconnect(pPacket->GetSerial(), pPacket->GetSessionDesc());
				SFServerConnectionManager* pManager = SFEngine::GetInstance()->GetServerConnectionManager();
				pManager->SetConnectorState(desc.identifier, false);
				break;
			}
			default: {
				return false;
			}
		}
	}

	return true;
}

bool ILogicEntry::AddConnectorCallback(int identifier, INetworkCallback* pCallback, int packetProtocolId) {
	SFPacketProtocolManager* pManager = SFEngine::GetInstance()->GetPacketProtocolManager();
	if (false == pManager->SetConnectorPacketProtocol(identifier, packetProtocolId))
		return false;

	mapConnectorCallback_.insert(std::make_pair(identifier, pCallback));
	return true;
}