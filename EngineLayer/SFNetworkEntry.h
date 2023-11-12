#pragma once
#ifndef __SFNETWORKENTRY__
#define __SFNETWORKENTRY__

class SFTCPNetwork;
class SFUDPNetwork;
class INetworkCallback;
class IUDPNetworkCallback;
class BasePacket;
class ILogicDispatcher;
class IPacketProtocol;
class IRPCInterface;

class SFNetworkEntry
{
public:
	SFNetworkEntry(void);
	virtual ~SFNetworkEntry(void);

	static SFNetworkEntry* GetInstance();

	bool Initialize(
		INetworkCallback* pTCPCallBack,
		IPacketProtocol* pProtocol,
		ILogicDispatcher* pDispatcher = nullptr,
		IUDPNetworkCallback* pUDPCallback = nullptr);
	bool ShutDown();

	bool Run();
	bool Update();
	bool IsConnected();

	bool TCPSend(BasePacket* pPacket);
	bool UDPSend(unsigned char* pMessage, int BufSize);
	//bool TCPSend( USHORT PacketID, NetworkMessage* pMessage, int Size );

	SFTCPNetwork* GetTCPNetwork() { return pTCPNetwork_; }
	SFUDPNetwork* GetUDPNetwork() { return pUDPNetwork_; }

	bool AddPeer(int serial, int externalIP, short externalPort, int localIP, short localPort);
	bool DeletePeer(int serial);
private:
	static SFNetworkEntry* pNetworkEntry_;
	SFTCPNetwork* pTCPNetwork_; //TCP
	SFUDPNetwork* pUDPNetwork_; // UDP
};

#endif