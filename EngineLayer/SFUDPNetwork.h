#pragma once
#ifndef __SFUDPNETWORK__
#define __SFUDPNETWORK__

#include <map>

class IP2PManager;
class IUDPNetworkCallback;

enum eP2PWorkMode {
	P2P_CLIENT = 0,
	P2P_SERVER,
};

class SFUDPNetwork
{
	using SerialPeerMap = std::map<int, unsigned char>;
public:
	SFUDPNetwork();
	virtual ~SFUDPNetwork(void);

	bool Initialize(IUDPNetworkCallback* pCallback, TCHAR* szP2PModuleName);
	bool Start();
	bool Update();

	bool Send(unsigned char* pData, unsigned int Length);
	bool AddPeer(int serial, int externalIP, short externalPort, int localIP, short localPort);
	bool DeletePeer(int serial);
private:
	IP2PManager* p2pModule_;
	SerialPeerMap serialPeerMap_;
	char szIP_[20];
	unsigned short port_;
};

#endif
