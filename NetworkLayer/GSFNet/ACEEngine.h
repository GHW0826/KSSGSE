#pragma once
#ifndef __ACEENGINE__
#define __ACEENGINE__
#include <map>
#include "INetworkEngine.h"
#include "ACETimeOutHandler.h"
#include "INetworkEngine.h"

class ProactorAcceptor;

using mapAcceptor = std::map<int, ProactorAcceptor*>;

class ACEEngine : public INetworkEngine
{
public:
	ACEEngine(IEngine* pEngine);
	virtual ~ACEEngine(void);


	virtual bool Shutdown() override;

	virtual bool SendRequest(BasePacket* pPacket) override;

	virtual bool Disconnect(int serial) override;

	virtual bool CheckTimerImpl() override; //default timer »ç¿ë

	virtual long AddTimer(unsigned int timerID, unsigned int startTime, unsigned int period) override;
	virtual bool CancelTimer(int timerID) override;

	virtual int AddConnector(int connectorIndex, char* szIP, unsigned short port) override;
	virtual int AddListener(char* szIP, unsigned short port, bool bDefaultListener = false) override;

protected:
	bool NetworkOpen();

	int acceptorIndex_;
	int connectorIndex_;
	int workThreadGroupID_;
	mapAcceptor mapAcceptor_;
private:
	ACETimeOutHandler timeOutHandler_;
};

class ACEServerEngine : public ACEEngine
{
public:
	ACEServerEngine(IEngine* pEngine);

	virtual bool Init(int ioThreadCnt) override;
	virtual bool Start(char* szIP, unsigned short port) override;
};

class ACEClientEngine : public ACEEngine
{
public:
	ACEClientEngine(IEngine* pEngine);

	virtual bool Init(int ioThreadCnt) override;
	virtual bool Start(char* szIP, unsigned short port) override;
};

#endif

