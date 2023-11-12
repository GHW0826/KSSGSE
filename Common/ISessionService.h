#pragma once
#ifndef __ISESSIONSERVICE__
#define __ISESSIONSERVICE__

#include "IPacketProtocol.h"

class BasePacket;
class ISession;

class ISessionService
{
public:
	ISessionService(IPacketProtocol* pProtocol)
		:pPacketProtocol_(pProtocol),
		serial_(-1)
	{}
	virtual ~ISessionService() { 
		delete pPacketProtocol_; 
	};

	virtual bool OnReceive(char* pData, unsigned short Length, _SessionDesc& desc) = 0;
	// virtual bool SendRequest(ISession* pSession, BasePacket* pPacket) = 0;

	void SetSerial(int serial) { serial_ = serial; }

	IPacketProtocol* GetPacketProtocol() { return pPacketProtocol_; }

protected:
	IPacketProtocol* pPacketProtocol_;
	int serial_;

private:

};

#endif