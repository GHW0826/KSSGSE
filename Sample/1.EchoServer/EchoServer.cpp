#include "stdafx.h"
#include "EchoLogicEntry.h"
// #include "SFJsonProtocol.h"
#include "SFCGSFPacketProtocol.h"
// #include "SFClassicDispatcher.h"
#include "SFPacketProtocol.h"

#pragma comment(lib, "EngineLayer.lib")

int _tmain(int argc, _TCHAR* argv[])
{

	EchoLogicEntry* pLogicEntry = new EchoLogicEntry();

	// ��ü ������ SFEngine ��ü �ʱ�ȭ �޼ҵ忡 ����
	auto errorCode = SFEngine::GetInstance()->Intialize(pLogicEntry);
	// auto errorCode = SFEngine::GetInstance()->Intialize(pLogicEntry, new SFClassicDispatcher()); //
	if (errorCode != NET_ERROR_CODE::SUCCESS)
		return 0;
	
	SFEngine::GetInstance()->AddPacketProtocol(0, new SFPacketProtocol<SFCGSFPacketProtocol>);
	SFEngine::GetInstance()->Start(0);

	int result = getchar();
	SFEngine::GetInstance()->ShutDown();
	if (pLogicEntry)
		delete pLogicEntry;

	return 0;
}