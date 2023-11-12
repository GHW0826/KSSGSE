#pragma once
#ifndef __BASEPACKET__
#define __BASEPACKET__

#include "stdafx.h"
#include "EngineStructure.h"

constexpr auto SFPACKET_NONE			= 0x00000001;
constexpr auto SFPACKET_CONNECT			= 0x00000002;
constexpr auto SFPACKET_DATA			= 0x00000004;
constexpr auto SFPACKET_RPC				= 0x00000008;
constexpr auto SFPACKET_TIMER			= 0x00000010;
constexpr auto SFPACKET_SHOUTER			= 0x00000020;
constexpr auto SFPACKET_DISCONNECT		= 0x00000040;
constexpr auto SFPACKET_DB				= 0x00000080;
constexpr auto SFPACKET_SERVERSHUTDOWN	= 0x00000100;
constexpr auto SFPACKET_AUTHENTICATE	= 0x00000200;

class BasePacket
{
public:
	BasePacket()
		:	serial_(-1),
			packetType_(static_cast<unsigned int>(SFPACKET_NONE)),
			packetID_(static_cast<unsigned short>(-1))
	{ }
	virtual ~BasePacket() {}

	void SetSerial(int serial) { serial_ = serial; }
	int GetSerial() { return serial_; }

	void SetPacketType(unsigned int PacketType){ packetType_ = PacketType; }
	unsigned int GetPacketType(){ return packetType_; }

	inline unsigned short GetPacketID(){return packetID_;}
	inline void SetPacketID(unsigned short packetID){ packetID_ = packetID;}
	
	void SetSessionDesc(_SessionDesc& sessiondesc)
	{ 
		sessiondesc_.identifier = sessiondesc.identifier;
		sessiondesc_.sessionType = sessiondesc.sessionType;
		sessiondesc_.port = sessiondesc.port;
		sessiondesc_.szIP = sessiondesc.szIP;
		sessiondesc_.pSession = sessiondesc.pSession;
	}
	_SessionDesc& GetSessionDesc(){ return sessiondesc_; }

	void CopyBaseHeader(BasePacket* pSource)
	{
		SetSerial(pSource->GetSerial());
		SetPacketID(pSource->GetPacketID());
		SetPacketType(pSource->GetPacketType());
	}

	virtual BasePacket* Clone(){ return nullptr; }


	// C/C++ 컴파일러는 사용되지 않는 매개변수 또는 변수에 대한 경고를 발생시키는 경향이 있음.
	// 이러한 경고를 무시하고자 할 때, 위의 매크로를 사용
	virtual bool Encode(unsigned short packetSize, int packetOption) { 
		UNREFERENCED_PARAMETER(packetSize); 
		UNREFERENCED_PARAMETER(packetOption);  
		return true; 
	}
	virtual bool Decode(char* pBuf, unsigned int nSize) { 
		UNREFERENCED_PARAMETER(pBuf); 
		UNREFERENCED_PARAMETER(nSize); 
		return true; 
	}
	virtual void Release() {};

protected:

private:
	int serial_;
	_SessionDesc sessiondesc_;
	unsigned int packetType_;
	unsigned short packetID_;
};

#endif