#include "stdafx.h"
#include "SFNetworkEntry.h"
#include "SFTCPNetwork.h"
#include "SFUDPNetwork.h"
#include "SFCasualGameDispatcher.h"
#include "SFIni.h"
#include "XML/StringConversion.h"

SFNetworkEntry* SFNetworkEntry::pNetworkEntry_ = nullptr;

SFNetworkEntry::SFNetworkEntry(void)
	: pTCPNetwork_(nullptr)
	, pUDPNetwork_(nullptr)
{}
SFNetworkEntry::~SFNetworkEntry(void)
{}

SFNetworkEntry* SFNetworkEntry::GetInstance() {
	if (pNetworkEntry_ == nullptr)
		pNetworkEntry_ = new SFNetworkEntry();
	return pNetworkEntry_;
}

bool SFNetworkEntry::TCPSend(BasePacket* pPacket) {
	if (pTCPNetwork_)
		return (bool)pTCPNetwork_->SendRequest(pPacket);
	return false;
}

bool SFNetworkEntry::UDPSend(unsigned char* pMessage, int BufSize) {
	if (pUDPNetwork_)
		return pUDPNetwork_->Send((unsigned char*)pMessage, BufSize);
	return false;
}

bool SFNetworkEntry::Initialize(
	INetworkCallback* pTCPCallBack,
	IPacketProtocol* pProtocol,
	ILogicDispatcher* pDispatcher,
	IUDPNetworkCallback* pUDPCallback
) {
	if (pTCPCallBack == nullptr)
		return false;

	SFIni ini;
	WCHAR szEngine[MAX_PATH];
	TCHAR szP2PModuleName[MAX_PATH];
	ini.SetPathName(_T("./Connection.ini"));
	ini.GetString(L"Engine", L"NAME", szEngine, MAX_PATH);
	ini.GetString(L"P2PModule", L"NAME", szP2PModuleName, MAX_PATH);

	std::string str = StringConversion::ToASCII(szEngine);

	pTCPNetwork_ = new SFTCPNetwork();
	pTCPNetwork_->Initialize((char*)str.c_str(), pTCPCallBack);

	if (pUDPCallback) {
		pUDPNetwork_ = new SFUDPNetwork();
		pUDPNetwork_->Initialize(pUDPCallback, szP2PModuleName);
	}

	pTCPNetwork_->SetPacketProtocol(pProtocol);

	if (pDispatcher == nullptr)
		pDispatcher = new SFCasualGameDispatcher();

	pTCPNetwork_->SetLogicDispatcher(pDispatcher);

	return true;
}

bool SFNetworkEntry::ShutDown() {
	if (pTCPNetwork_)
		delete pTCPNetwork_;
	return true;
}

bool SFNetworkEntry::Run() {

	SFIni ini;
	WCHAR szIP[20];
	USHORT Port;

	ini.SetPathName(_T("./Connection.ini"));
	ini.GetString(L"ServerInfo", L"IP", szIP, 20);
	Port = ini.GetInt(L"ServerInfo", L"PORT", 0);

	std::string str = StringConversion::ToASCII(szIP);
	if (pTCPNetwork_->Start((char*)str.c_str(), Port) == false) {
		return false;
	}

	if (pUDPNetwork_) {
		pUDPNetwork_->Start();
	}

	return Update();;
}

bool SFNetworkEntry::Update() {
	pTCPNetwork_->Update();

	if (pUDPNetwork_)
		pUDPNetwork_->Update();
	return true;
}

bool SFNetworkEntry::IsConnected() {
	return pTCPNetwork_->IsConnected();
}

bool SFNetworkEntry::AddPeer(int serial, int externalIP, short externalPort, int localIP, short localPort) {
	return pUDPNetwork_->AddPeer(serial, externalIP, externalPort, localIP, localPort);
}

bool SFNetworkEntry::DeletePeer(int serial) {
	return pUDPNetwork_->DeletePeer(serial);
}

