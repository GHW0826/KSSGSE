#pragma once
#ifndef __SFTCPNETWORK__
#define __SFTCPNETWORK__

#include "SFEngine.h"
// #include "liblfds611.h"

class INetworkCallback;

class SFTCPNetwork
{
public:
	SFTCPNetwork(void);
	virtual ~SFTCPNetwork(void);

	bool Initialize(char* szModuleName, INetworkCallback* pTCPCallBack);
	bool Start(char* szIP, unsigned short Port);
	bool Update();
	bool SendRequest(BasePacket* pPacket);

	SFEngine* GetNetwork();

	void SetLogicDispatcher(ILogicDispatcher* pDispatcher);

	void SetPacketProtocol(IPacketProtocol* pProtocol);

	bool IsConnected();

	BasePacket* GetRPCResult();
private:
	SFEngine* pTCPClient_; // TCP
	INetworkCallback* pTCPCallBack_;
	// struct lfds611_queue_state* m_pQueue;
};

#endif
