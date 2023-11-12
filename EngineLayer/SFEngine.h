#pragma once

#ifndef __SFENGINE__
#define __SFENGINE__

#include "ErrorCode.h"
#include "ILogicEntry.h"
#include "IEngine.h"

#include "Macro.h"
#include "SFConfigure.h"

class IPacketProtocol;
class INetworkEngine;
class ILogicDispatcher;
class SFPacketProtocolManager;
class SFServerConnectionManager;

class SFEngine : public IEngine
{
	friend class SFTCPNetwork;
	typedef std::map<int, long> mapTimer;
public:
	virtual ~SFEngine(void);

	NET_ERROR_CODE Intialize(ILogicEntry* pLogicEntry, ILogicDispatcher* pDispatcher = nullptr);
	virtual ISessionService* CreateSessionService(_SessionDesc& desc) override;

	bool ReleasePacket(BasePacket* pPacket);

	virtual bool OnConnect(int serial, _SessionDesc& desc) override;
	virtual bool OnDisconnect(int serial, _SessionDesc& desc) override;

	int  AddListener(char* szIP, unsigned short port, int packetProtocolId, bool bDefaultListener = false);
	bool AddPacketProtocol(int packetProtocolId, IPacketProtocol* pProtocol);
	int  AddConnector(int connectorId, char* szIP, unsigned short port);

	bool SendRequest(BasePacket* pPacket);
	// bool SendRequest(BasePacket* pPacket, std::vector<int>& ownerList);

	bool Start(int protocolId);
	bool ShutDown();

	void SetLogFolder(TCHAR* szPath = nullptr);
	void SetLogicDispathcer(ILogicDispatcher* pDispatcher)		{ pLogicDispatcher_ = pDispatcher; }

	bool						IsServer()						{ return isServer_; }
	ILogicDispatcher*			GetLogicDispatcher()			{ return pLogicDispatcher_; }
	INetworkEngine*				GetNetworkEngine()				{ return pNetworkEngine_; }
	SFServerConnectionManager*	GetServerConnectionManager()	{ return pServerConnectionManager_; }
	SFPacketProtocolManager*	GetPacketProtocolManager()		{ return pPacketProtocolManager_; }

	static SFEngine* GetInstance();
protected:
	bool CreatePacketSendThread();
	NET_ERROR_CODE CreateEngine(char* szModuleName, bool server = false);
private:
	SFEngine();
	bool Start(char* szIP, unsigned short port); //클라이언트 전용, 이후 deprecated 될 것임	

private:
	SFConfigure					Config_;

	int							packetSendThreadId_;
	bool						isServer_;

	HINSTANCE					engineHandle_;
	ILogicDispatcher*			pLogicDispatcher_;
	INetworkEngine*				pNetworkEngine_;
	SFPacketProtocolManager*	pPacketProtocolManager_;
	SFServerConnectionManager*	pServerConnectionManager_;

	static SFEngine*			pEngine_;
};

#endif