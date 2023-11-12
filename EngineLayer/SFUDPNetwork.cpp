#include "stdafx.h"
#include "SFUDPNetwork.h"

#pragma warning (disable : 4244) 

HINSTANCE g_pP2PHandle = 0;

SFUDPNetwork::SFUDPNetwork()
	: p2pModule_(nullptr)
{
	g_pP2PHandle = 0;
}
SFUDPNetwork::~SFUDPNetwork(void) {
	if (g_pP2PHandle)
		::FreeLibrary(g_pP2PHandle);
}

bool SFUDPNetwork::Initialize(IUDPNetworkCallback* pCallback, TCHAR* szP2PModuleName) {

	g_pP2PHandle = ::LoadLibrary(szP2PModuleName);
	if (g_pP2PHandle == nullptr)
		return false;

	/*
	GETP2PMANAGER_FUNC* pfunc;
	pfunc = (GETP2PMANAGER_FUNC*)::GetProcAddress(g_pP2PHandle, "GetP2PManager");
	p2pModule_ = pfunc();

	p2pModule_->Initialize(pCallback);
	*/
	return true;
}

bool SFUDPNetwork::Update() {
	/*
	if (p2pModule_)
		return p2pModule_->Update();
		*/
	return false;
}

bool SFUDPNetwork::Start() {
	GetPrivateProfileStringA("UDPInfo", "IP", nullptr, szIP_, 20, "./Connection.ini");
	port_ = GetPrivateProfileIntA("UDPInfo", "PORT", 0, "./Connection.ini");

	/*
	if (p2pModule_)
		p2pModule_->RunP2P(szIP_, port_);
		*/
	return true;
}

bool SFUDPNetwork::Send(unsigned char* pData, unsigned int Length) {
	/*
	p2pModule_->BroadCast(pData, Length);
	*/
	return true;
}


bool SFUDPNetwork::AddPeer(int serial, int externalIP, short externalPort, int localIP, short localPort) {

	/*
	PeerAddressInfo info;
	info.externalIP = externalIP;
	info.externalPort = externalPort;
	info.localIP = localIP;
	info.localPort = localPort;

	BYTE peerIndex = 0;
	p2pModule_->AddPeer(&info, peerIndex);
	serialPeerMap_.insert(std::make_pair(serial, peerIndex));
	*/

	return true;
}

bool SFUDPNetwork::DeletePeer(int serial) {

	SerialPeerMap::iterator iter = serialPeerMap_.find(serial);
	if (iter == serialPeerMap_.end())
		return false;

	/*
	p2pModule_->RemovePeer(iter->first);
	serialPeerMap_.erase(iter->second);
	*/
	return true;
}