#include "stdafx.h"
#include "SFCasualGameDispatcher.h"
#include "SFEngine.h"
#include "IRPCService.h"

bool SFCasualGameDispatcher::bLogicEnd_= false;

//ĳ��� ���� ������ ��ũ�� ���� ������ ���� �ϳ���
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
		//��������Ʈ���� ť���� ��Ŷ�� ������.
		//������Ʈ�� ��ü�� ProcessPacket �޼ҵ带 ȣ���ؼ� ��Ŷ ó���� �����Ѵ�.
		BasePacket* pPacket = LogicGatewaySingleton::instance()->PopPacket();
		pEntry->ProcessPacket(pPacket);

		//20150113
		//DB ó���� ������Ʈ�� ��ü�� �����ؼ� �������̾ �����ͺ��̽� ���̾ ���ӵ��� �ʵ��� �Ѵ�.
		//DB ��Ŷ�� ���Ŵ� �������̾�� ó���ϵ��� �Ѵ�.
		//���� ������ ������ ���̴�.
		if (pPacket->GetPacketType() != SFPACKET_DB) {
			ReleasePacket(pPacket);
		}
	}
}

void SFCasualGameDispatcher::RPCThreadProc(void* Args) {
	SFCasualGameDispatcher* pDisPatcher = (SFCasualGameDispatcher*)Args;
	while (bLogicEnd_ == false) {
		//��������Ʈ���� ť���� ��Ŷ�� ������.
		//BasePacket* pPacket = RPCGatewaySingleton::instance()->PopPacket();

		//20140608 �ӽ�..
		SFPacket* pPacket = (SFPacket*)RPCGatewaySingleton::instance()->PopPacket();
		if (pPacket) {
			pDisPatcher->pRPCService_->ProcessRPCService(pPacket);
			delete pPacket;
		}
	}
}