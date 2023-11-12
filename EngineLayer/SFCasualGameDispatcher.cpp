#include "stdafx.h"
#include "SFCasualGameDispatcher.h"
#include "SFEngine.h"
#include "IRPCService.h"

bool SFCasualGameDispatcher::bLogicEnd_= false;

//캐쥬얼 게임 프레임 워크의 로직 쓰레드 수는 하나임
SFCasualGameDispatcher::SFCasualGameDispatcher(void)
	: nLogicThreadCnt_(1),
	rpcThreadGroupId_(-1),
	logicThreadGroupId_(-1),
	pRPCService_(nullptr)
{}
SFCasualGameDispatcher::~SFCasualGameDispatcher(void)
{}


void SFCasualGameDispatcher::Dispatch(BasePacket* pPacket) {
	if (pPacket->GetPacketType() == SFPACKET_RPC && SFEngine::GetInstance()->IsServer()) {
		RPCGatewaySingleton::instance()->PushPacket(pPacket);
	}
	else {
		LogicGatewaySingleton::instance()->PushPacket(pPacket);
	}
}


bool SFCasualGameDispatcher::CreateLogicSystem(ILogicEntry* pLogicEntry) {
	logicThreadGroupId_ = ACE_Thread_Manager::instance()->spawn_n(nLogicThreadCnt_, (ACE_THR_FUNC)LogicThreadProc, this, THR_NEW_LWP, ACE_DEFAULT_THREAD_PRIORITY);

	LogicEntrySingleton::instance()->SetLogic(pLogicEntry);

	rpcThreadGroupId_ = ACE_Thread_Manager::instance()->spawn_n(1, (ACE_THR_FUNC)RPCThreadProc, this, THR_NEW_LWP, ACE_DEFAULT_THREAD_PRIORITY);

	return true;
}

bool SFCasualGameDispatcher::ShutDownLogicSystem() {
	
	bLogicEnd_ = true;
	for (int i = 0; i < nLogicThreadCnt_; i++) {
		BasePacket* pCommand = PacketPoolSingleton::instance()->Alloc();
		pCommand->SetSerial(-1);
		pCommand->SetPacketType(SFPACKET_SERVERSHUTDOWN);
		LogicGatewaySingleton::instance()->PushPacket(pCommand);
	}

	LogicEntrySingleton::instance()->DestroyLogic();

	RPCGatewaySingleton::instance()->PushPacket(NULL);
	ACE_Thread_Manager::instance()->wait_grp(rpcThreadGroupId_);

	return true;
}


bool SFCasualGameDispatcher::AddRPCService(IRPCService* pService) {
	pRPCService_ = pService;
	return true;
}



void SFCasualGameDispatcher::LogicThreadProc(void* Args) {
	UNREFERENCED_PARAMETER(Args);

	LogicEntry* pEntry = LogicEntrySingleton::instance();
	while (bLogicEnd_ == false) {
		//로직게이트웨이 큐에서 패킷을 꺼낸다.
		//로직엔트리 객체의 ProcessPacket 메소드를 호출해서 패킷 처리를 수행한다.
		BasePacket* pPacket = LogicGatewaySingleton::instance()->PopPacket();
		pEntry->ProcessPacket(pPacket);

		//20150113
		//DB 처리를 로직엔트리 객체에 위임해서 엔진레이어가 데이터베이스 레이어에 종속되지 않도록 한다.
		//DB 패킷의 수거는 로직레이어에서 처리하도록 한다.
		//차후 구조를 수정할 것이다.
		if (pPacket->GetPacketType() != SFPACKET_DB) {
			ReleasePacket(pPacket);
		}
	}
}

void SFCasualGameDispatcher::RPCThreadProc(void* Args) {
	SFCasualGameDispatcher* pDisPatcher = (SFCasualGameDispatcher*)Args;
	while (bLogicEnd_ == false) {
		//로직게이트웨이 큐에서 패킷을 꺼낸다.
		//BasePacket* pPacket = RPCGatewaySingleton::instance()->PopPacket();

		//20140608 임시..
		SFPacket* pPacket = (SFPacket*)RPCGatewaySingleton::instance()->PopPacket();
		if (pPacket) {
			pDisPatcher->pRPCService_->ProcessRPCService(pPacket);
			delete pPacket;
		}
	}
}