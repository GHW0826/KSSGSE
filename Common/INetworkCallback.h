#pragma once
#ifndef __INETWORKCALLBACK__
#define __INETWORKCALLBACK__

#include "EngineStructure.h"
#include <iostream>

#pragma warning( push )
#pragma warning( disable:4100 )

class BasePacket;

class INetworkCallback
{
public:
	INetworkCallback(void)
	: bConnected_(false),
	serial_(-1)
	{}
	virtual ~INetworkCallback(void) {}

	virtual bool HandleNetworkMessage(BasePacket* pPacket) = 0;
	virtual bool HandleRPC(BasePacket* pPacket) { return false; }
	virtual void HandleConnect(int serial, _SessionDesc m_sessiondesc) {
		std::cout << "Connected" << std::endl;
		serial_ = serial;
		bConnected_ = true;
	}

	virtual void HandleDisconnect(int serial, _SessionDesc m_sessiondesc) {
		bConnected_ = false;
		std::cout << "Disconnected" << std::endl;
	}

	bool IsConnected()			{ return bConnected_; }
	int GetSerial()				{ return serial_; }
	void SetSerial(int serial)	{ serial_ = serial; }
private:
	int serial_;
	bool bConnected_;
};

#pragma warning( pop )

#endif