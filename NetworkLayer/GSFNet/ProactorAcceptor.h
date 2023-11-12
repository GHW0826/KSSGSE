#pragma once
#ifndef __PROACTORACCEPTOR__
#define __PROACTORACCEPTOR__

#include <ace/Asynch_Acceptor.h>
#include <ace/INet_Addr.h>
#include "ProactorService.h"
#include <string>

class INetworkEngine;
class IProactorAgent;

class ProactorAcceptor : public ACE_Asynch_Acceptor<ProactorService>
{
public:
	ProactorAcceptor(INetworkEngine* pOwner, char* szIP, unsigned short port);
	virtual ~ProactorAcceptor(void);

	virtual int validate_connection(const ACE_Asynch_Accept::Result& Result, const ACE_INET_Addr& Remote, const ACE_INET_Addr& Local) override;
	virtual ProactorService* make_handler() override;

	void SetAcceptorNum(int acceptorNum)	{ acceptorNum_ = acceptorNum; }
	unsigned short GetPort()				{ return port_; }
private:
	INetworkEngine* pOwner_;
	int acceptorNum_;
	std::string szIP_;
	unsigned short port_;
};

#endif