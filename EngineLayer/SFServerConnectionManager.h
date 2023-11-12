#pragma once
#ifndef __SFSERVERCONNECTIONMANGER__
#define __SFSERVERCONNECTIONMANGER__

class SFServerConnectionManager
{
public:
	SFServerConnectionManager();
	virtual ~SFServerConnectionManager();

	bool LoadConnectorList(WCHAR* szFileName);
	void AddConnectorInfo(_ConnectorInfo& connectorInfo);

	bool SetupServerReconnectSys();
	bool SetConnectorState(int connectorId, bool connected);
protected:
	UINT static __stdcall ServerReconnectProc(LPVOID arg);

private:
	std::list<_ConnectorInfo> listConnectorInfo_;
	HANDLE hTimerEvent_;
	HANDLE hThread_;
	bool  bThreadEnd_;
	DWORD  dwThreadID_;

	SFLock lock_;
};

#endif

