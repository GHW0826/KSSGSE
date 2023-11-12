#include "stdafx.h"
#include "SFServerConnectionManager.h"
#include "SFEngine.h"
#include "SFPacketProtocolManager.h"
#include "XML/StringConversion.h"

SFServerConnectionManager::SFServerConnectionManager()
	: hThread_(nullptr)
	, hTimerEvent_(nullptr)
	, dwThreadID_(0)
	, bThreadEnd_(false)
{}
SFServerConnectionManager::~SFServerConnectionManager() {
	if (hThread_) {
		bThreadEnd_ = true;
		WaitForSingleObject(hThread_, INFINITE);
		CloseHandle(hThread_);
	}
}


bool SFServerConnectionManager::LoadConnectorList(WCHAR* szFileName) {
	/*
	CMarkup xml;
	bool result = xml.Load(szFileName);

	if (result == false)
		return false;

	while (xml.FindChildElem(L"SERVER")) {
		_ConnectorInfo connectorInfo;
		xml.IntoElem();

		xml.FindChildElem(L"IP");
		connectorInfo.szIP = xml.GetChildData();

		xml.FindChildElem(L"PORT");
		connectorInfo.port = _ttoi(xml.GetChildData().c_str());

		xml.FindChildElem(L"IDENTIFER");
		connectorInfo.connectorId = _ttoi(xml.GetChildData().c_str());

		xml.FindChildElem(L"DESC");
		connectorInfo.szDesc = xml.GetChildData();

		xml.OutOfElem();

		AddConnectorInfo(connectorInfo);
	}
	*/
	return true;
}

void SFServerConnectionManager::AddConnectorInfo(_ConnectorInfo& connectorInfo) {
	listConnectorInfo_.push_back(connectorInfo);
	SFEngine::GetInstance()->GetPacketProtocolManager()->AddConnectorInfo(&connectorInfo);

	// LOG(INFO) << "AddConnectorInfo. " << "connectID: " << connectorInfo.connectorId << ", ProtocolID: " << (int)connectorInfo.packetProtocolId << ", IP: " << connectorInfo.szIP.data() << ", Port: " << connectorInfo.port;
	// google::FlushLogFiles(google::GLOG_INFO);
}

bool SFServerConnectionManager::SetupServerReconnectSys() {
	hTimerEvent_ = CreateEvent(NULL, FALSE, FALSE, L"ServerReconnectEvent");

	// LOG(INFO) << "SetupServerReconnectSys. " << "First Connect";
	// google::FlushLogFiles(google::GLOG_INFO);

	for (auto& iter : listConnectorInfo_) {
		_ConnectorInfo& info = iter;

		// LOG(INFO) << "connectID: " << info.connectorId << ", ProtocolID: " << (int)info.packetProtocolId << ", IP: " << info.szIP.data() << ", Port: " << info.port;
		// google::FlushLogFiles(google::GLOG_INFO);

		int serial = -1;
		serial = SFEngine::GetInstance()->AddConnector(info.connectorId, (char*)StringConversion::ToASCII(info.szIP.c_str()).c_str(), info.port);
		if (serial >= 0) {
			info.connected = true;
		}
	}

	hThread_ = (HANDLE)_beginthreadex(0, NULL, ServerReconnectProc, this, 0, (unsigned*)&dwThreadID_);

	if (!hThread_) {
		int errorNum = (GetLastError() == ERROR_SUCCESS) ? ERROR_MAX_THRDS_REACHED : GetLastError();
		// LOG(FATAL) << "ServerReconnectProc Thread Creation Fail. Error : " << errorNum;
		return false;
	}

	// LOG(INFO) << "ServerReconnectProc success";

	return true;
}


UINT SFServerConnectionManager::ServerReconnectProc(LPVOID arg) {

	SFServerConnectionManager* pConnectionManager = reinterpret_cast<SFServerConnectionManager*>(arg);
	while (WaitForSingleObject(pConnectionManager->hTimerEvent_, 1000) != WAIT_OBJECT_0 && pConnectionManager->bThreadEnd_ == false) {
		for (auto& iter : pConnectionManager->listConnectorInfo_) {
			_ConnectorInfo& info = iter;
			if (info.connected == false) {
				int serial = -1;
				serial = SFEngine::GetInstance()->AddConnector(info.connectorId, (char*)StringConversion::ToASCII(info.szIP.c_str()).c_str(), info.port);
				if (serial >= 0) {
					info.connected = true;
				}
			}
		}
	}

	return 0;
}


bool SFServerConnectionManager::SetConnectorState(int connectorId, bool connected) {
	for (auto& iter : listConnectorInfo_) {
		_ConnectorInfo& info = iter;
		if (info.connectorId == connectorId) {
			info.connected = connected;
			return true;
		}
	}

	return false;
}