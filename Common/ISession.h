#pragma once
#ifndef __ISESSION__
#define __ISESSION__

#include "INetworkEngine.h"
#include "IEngine.h"
#include "ISessionService.h"

constexpr int SESSION_STATE_NULL		= 0;
constexpr int SESSION_STATE_CONNECT		= 1;
constexpr int SESSION_STATE_AUTENTICATE	= 2;

class ISession
{
public:
	ISession()
	: channelNum_(-1),
	pOwner_(nullptr),
	pSessionService_(nullptr),
	sessionState_(SESSION_STATE_NULL)
	{}
	virtual ~ISession() {};


	void OnConnect(int serial, _SessionDesc& desc) {
		pSessionService_ = pOwner_->GetEngine()->CreateSessionService(desc);
		pSessionService_->SetSerial(serial);
		pOwner_->GetEngine()->OnConnect(serial, desc);
	}

	void OnDisconnect(int serial, _SessionDesc& desc) {
		pOwner_->GetEngine()->OnDisconnect(serial, desc);
		delete pSessionService_;
	}

	bool OnReceive(char* pData, unsigned short length, _SessionDesc& desc) {
		return pSessionService_->OnReceive(pData, length, desc);
	}

	virtual bool SendRequest(BasePacket* pPacket) = 0;

	void SetLoginState(int state)			{ sessionState_ = state; }
	void SetOwner(INetworkEngine* pOwner)	{ pOwner_ = pOwner; }
	IEngine* GetEngine()					{ return pOwner_->GetEngine(); }
	int GetLoginState()						{ return sessionState_; }
	IPacketProtocol* GetPacketProtocol()	{ return pSessionService_->GetPacketProtocol(); }

	int channelNum_;
private:
	INetworkEngine* pOwner_;
	ISessionService* pSessionService_;
	int sessionState_;
};


#endif