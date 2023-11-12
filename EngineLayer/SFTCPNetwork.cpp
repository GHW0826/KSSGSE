#include "stdafx.h"
#include "SFTCPNetwork.h"
#include "INetworkCallback.h"
#include "SFPacketProtocolManager.h"

SFTCPNetwork::SFTCPNetwork(void)
{
	// lfds611_queue_new(&m_pQueue, 1000);
}
SFTCPNetwork::~SFTCPNetwork(void) {
	// lfds611_queue_delete(m_pQueue, NULL, NULL);

	if (pTCPCallBack_) {
		delete pTCPCallBack_;
		pTCPCallBack_ = nullptr;
	}
	if (pTCPClient_) {
		pTCPClient_->ShutDown();
		pTCPClient_ = nullptr;
	}
}

bool SFTCPNetwork::Initialize(char* szModuleName, INetworkCallback* pTCPCallBack) {

	pTCPClient_ = SFEngine::GetInstance();
	if (NET_ERROR_CODE::SUCCESS != pTCPClient_->CreateEngine(szModuleName, false)) {
		return false;
	}

	pTCPCallBack_ = pTCPCallBack;
	return true;
}

bool SFTCPNetwork::Start(char* szIP, unsigned short Port) {
	return pTCPClient_->Start(szIP, Port);
}

bool SFTCPNetwork::Update() {

	while (1) {
		BasePacket* pPacket = LogicGatewaySingleton::instance()->PopPacket(0);

		if (pPacket != nullptr) {
			switch (pPacket->GetPacketType()) {
				case SFPACKET_DATA: {
					pTCPCallBack_->HandleNetworkMessage(pPacket);
					pTCPClient_->ReleasePacket(pPacket);
					break;
				}
				case SFPACKET_RPC: {
					// lfds611_queue_guaranteed_enqueue(m_pQueue, pPacket);
					break;
				}
				case SFPACKET_CONNECT: {
					pTCPCallBack_->HandleConnect(pPacket->GetSerial(), pPacket->GetSessionDesc());
					delete pPacket;
					break;
				}
				case  SFPACKET_DISCONNECT: {
					pTCPCallBack_->HandleDisconnect(pPacket->GetSerial(), pPacket->GetSessionDesc());
					delete pPacket;
					break;
				}
			}
		}
		else {
			break;
		}
	}

	return TRUE;
}

bool SFTCPNetwork::SendRequest(BasePacket* pPacket) {
	/*int HeaderSize = sizeof(SFPacketHeader);							  //
																		  //
	SFPacket PacketSend;												  //
																		  //
	PacketSend.SetPacketID(PacketID);									  //
																		  //
	PacketSend.MakePacket((BYTE*)pMessage, BufSize, CGSF_PACKET_OPTION);  //
	*/

	pPacket->SetSerial(pTCPCallBack_->GetSerial());
	return pTCPClient_->SendRequest(pPacket);
}

void SFTCPNetwork::SetLogicDispatcher(ILogicDispatcher* pDispatcher) {
	pTCPClient_->SetLogicDispathcer(pDispatcher);
}

void SFTCPNetwork::SetPacketProtocol(IPacketProtocol* pProtocol) {
	_ConnectorInfo connectorInfo;
	connectorInfo.connectorId = 1;
	connectorInfo.packetProtocolId = 0;

	pTCPClient_->AddPacketProtocol(0, pProtocol);
	pTCPClient_->GetPacketProtocolManager()->AddConnectorInfo(&connectorInfo);
}

bool SFTCPNetwork::IsConnected() {
	return pTCPCallBack_->IsConnected();
}

BasePacket* SFTCPNetwork::GetRPCResult() {
	BasePacket* pPacket = nullptr;
	//	lfds611_queue_dequeue(m_pQueue, (void**)&pPacket);
	return pPacket;
}