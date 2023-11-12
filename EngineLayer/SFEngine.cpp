#include "stdafx.h"
#include "SFEngine.h"

#include "ACEHeader.h"
#include "IPacketProtocol.h"
#include "INetworkEngine.h"
#include "SFCasualGameDispatcher.h"
#include "SFPacketProtocolManager.h"
#include "SFServerConnectionManager.h"
#include "SFSessionService.h"

#include "XML/StringConversion.h"
#include "EngineConstant.h"

SFEngine* SFEngine::pEngine_ = nullptr;

SFEngine::SFEngine()
	: packetSendThreadId_(-1)
	, pNetworkEngine_(0)
	, isServer_(false)
	, pPacketProtocolManager_(nullptr)
	, pServerConnectionManager_(nullptr) {

	ACE::init();
	// PacketDelayedSendTask::instance()->Init(100);	

	// google::InitGoogleLogging("CGSF");
	Config_.Read(L"EngineConfig.xml");
#ifdef _DEBUG
	_putenv_s("GLOG_logbufsecs", "0");
	// google::LogToStderr();
#endif

	engineHandle_ = 0;

	pPacketProtocolManager_ = new SFPacketProtocolManager();
}

SFEngine::~SFEngine(void) {
	// ACE::fini();

	if (pNetworkEngine_)
		delete pNetworkEngine_;

	if (engineHandle_)
		FreeLibrary(engineHandle_);
}

SFEngine* SFEngine::GetInstance() {
	if (pEngine_ == nullptr)
		pEngine_ = new SFEngine();
	return pEngine_;
}

NET_ERROR_CODE SFEngine::Intialize(ILogicEntry* pLogicEntry, ILogicDispatcher* pDispatcher) {

	//	LOG(INFO) << "Engine Initialize... ";
	//	SetLogFolder();

	if (pLogicEntry == nullptr)
		return NET_ERROR_CODE::ENGINE_INIT_LOGIC_ENTRY_NULL;
	if (pDispatcher == nullptr)
		pDispatcher = new SFCasualGameDispatcher();
	
	SetLogicDispathcer(pDispatcher);

	pServerConnectionManager_ = new SFServerConnectionManager();
	// LOG(INFO) << "Logic Entry Initialize";
	if (false == pLogicEntry->Initialize()) {
		// LOG(ERROR) << "LogicEntry Intialize Fail!!";
		return NET_ERROR_CODE::ENGINE_INIT_LOGIC_ENTRY_FAIL;
	}


	_EngineConfig* pInfo = Config_.GetConfigureInfo();

	// LOG(INFO) << "Basic Port: " << pInfo->serverPort;

	std::string szNetworkEngineName = StringConversion::ToASCII(pInfo->engineName);
	
	// LOG(INFO) << "NetworkEngine Create : " << szNetworkEngineName.c_str();
	// LOG(INFO) << "MaxAccept : " << pInfo->maxAccept;
	if (pInfo->maxAccept <= 0 || pInfo->maxAccept > MAX_USER_ACCEPT) {
		// LOG(WARNING) << "MaxAccept Value Abnormal : " << pInfo->maxAccept;
		SetMaxUserAccept(DEFALUT_USER_ACCEPT);
	}
	else
		SetMaxUserAccept(pInfo->maxAccept);

	auto errorCode = CreateEngine((char*)"GSFNet.dll", true); // (char*)szNetworkEngineName.c_str(), true);
	if (errorCode != NET_ERROR_CODE::SUCCESS) {
		// LOG(ERROR) << "NetworkEngine : " << szNetworkEngineName.c_str() << " Creation FAIL!!";
		return errorCode;
	}

	// LOG(INFO) << "NetworkEngine : " << szNetworkEngineName.c_str() << " Creation Success!!";

	if (false == pDispatcher->CreateLogicSystem(pLogicEntry)) {
		// LOG(ERROR) << "Logic System Creation FAIL!!";
		return NET_ERROR_CODE::ENGINE_INIT_CREAT_LOGIC_SYSTEM_FAIL;
	}

	CreatePacketSendThread();
	// LOG(INFO) << "Engine Initialize Complete!! ";
	return NET_ERROR_CODE::SUCCESS;
}

bool SFEngine::CreatePacketSendThread()
{
	// packetSendThreadId_ = ACE_Thread_Manager::instance()->spawn_n(1, (ACE_THR_FUNC)PacketSendThread, this, THR_NEW_LWP, ACE_DEFAULT_THREAD_PRIORITY);

	return TRUE;
}

NET_ERROR_CODE SFEngine::CreateEngine(char* szModuleName, bool server) {
	isServer_ = server;

	engineHandle_ = ::LoadLibraryA(szModuleName);
	if (engineHandle_ == 0)
		return NET_ERROR_CODE::ENGINE_INIT_CREAT_ENGINE_LOAD_DLL_FAIL;

	CREATENETWORKENGINE* pfunc;
	pfunc = (CREATENETWORKENGINE*)::GetProcAddress(engineHandle_, "CreateNetworkEngine");
	pNetworkEngine_ = pfunc(server, this);
	if (pNetworkEngine_ == nullptr)
		return NET_ERROR_CODE::ENGINE_INIT_CREAT_ENGINE_FUNC_NULL;

	SYSTEM_INFO si;
	GetSystemInfo(&si);
	int ioThreadCnt = si.dwNumberOfProcessors * 2;
	if (pNetworkEngine_->Init(ioThreadCnt) == false)
		return NET_ERROR_CODE::ENGINE_INIT_CREAT_ENGINE_INIT_FAIL;

	return NET_ERROR_CODE::SUCCESS;
}

ISessionService* SFEngine::CreateSessionService(_SessionDesc& desc) {
	IPacketProtocol* pSourceProtocol = nullptr;
	if (desc.sessionType == 0) // SessionType::listener)
		pSourceProtocol = pPacketProtocolManager_->GetPacketProtocolWithListenerId(desc.identifier);
	else
		pSourceProtocol = pPacketProtocolManager_->GetPacketProtocolWithConnectorId(desc.identifier);

	IPacketProtocol* pCloneProtocol = pSourceProtocol->Clone();
	return new SFSessionService(pCloneProtocol);
}

bool SFEngine::ReleasePacket(BasePacket* pPacket)
{
	pPacket->Release();
	return true;
}

bool SFEngine::OnConnect(int serial, _SessionDesc& desc) {
	BasePacket* pPacket = new BasePacket();
	pPacket->SetSessionDesc(desc);
	pPacket->SetPacketType(SFPACKET_CONNECT);
	pPacket->SetSerial(serial);

	pLogicDispatcher_->Dispatch(pPacket);

	return true;
}

bool SFEngine::OnDisconnect(int serial, _SessionDesc& desc) {
	BasePacket* pPacket = new BasePacket();
	pPacket->SetSessionDesc(desc);
	pPacket->SetPacketType(SFPACKET_DISCONNECT);
	pPacket->SetSerial(serial);

	pLogicDispatcher_->Dispatch(pPacket);

	return true;
}

bool SFEngine::Start(int protocolId)
{
	bool bResult = false;
	_EngineConfig* pInfo = Config_.GetConfigureInfo();
	// LOG(INFO) << "Engine Starting... IP : " << (char*)StringConversion::ToASCII(pInfo->serverIP).c_str() << " Port : " << pInfo->serverPort;

	if (protocolId >= 0) {
		IPacketProtocol* pProtocol = pPacketProtocolManager_->GetPacketProtocol(protocolId);

		if (pProtocol == NULL) {
			// LOG(ERROR) << "Engine Start Fail. PacketProtocol None";
			return false;
		}

		int listenerId = -1;
		if (pInfo->serverPort != 0) {
			listenerId = AddListener((char*)StringConversion::ToASCII(pInfo->serverIP).c_str(), pInfo->serverPort, protocolId, true);
			if (listenerId <= 0) {
				// LOG(ERROR) << "Engine Start Fail. m_pNetworkEngine->AddListener fail";
				return false;
			}
		}
	}

	bResult = pNetworkEngine_->Start((char*)StringConversion::ToASCII(pInfo->serverIP).c_str(), pInfo->serverPort);

	if (bResult == false) {
		// LOG(ERROR) << "Engine Start Fail!!";
		return false;
	}
	// LOG(INFO) << "Engine Start!!";

	return true;
}

bool SFEngine::ShutDown()
{
	// 올바르게 종료되는지 조사하기 위해 각 단계별로 로그를 남깁니다.
	// LOG(INFO) << "Engine Shut Down!!";

	pLogicDispatcher_->ShutDownLogicSystem();
	// LOG(INFO) << "Engine Shut Down Step (1) ShutDownLogicSystem";

	if (packetSendThreadId_ >= 0) {
		PacketSendSingleton::instance()->PushTask(NULL);
		// LOG(INFO) << "Engine Shut Down Step (2) instance()->PushTask(NULL)";

		ACE_Thread_Manager::instance()->wait_grp(packetSendThreadId_);
		// LOG(INFO) << "Engine Shut Down Step (3) wait_grp(m_packetSendThreadId)";
	}

	if (pNetworkEngine_) {
		pNetworkEngine_->Shutdown();
		// LOG(INFO) << "Engine Shut Down Step (4) m_pNetworkEngine->Shutdown()";

		//delete pNetworkEngine_; //
	}

	if (pServerConnectionManager_) {
		delete pServerConnectionManager_;
		// LOG(INFO) << "Engine Shut Down Step (5) delete Server Connecton Manager";
	}

	delete this;
	// LOG(INFO) << "Engine Shut Down Step (6) Engine Delete";

	// google::ShutdownGoogleLogging();

	return true;
}

int SFEngine::AddListener(char* szIP, unsigned short port, int packetProtocolId, bool bDefaultListener) {
	int listenerId = GetNetworkEngine()->AddListener(szIP, port, bDefaultListener);

	if (listenerId) {
		pPacketProtocolManager_->AddListenerInfo(listenerId, packetProtocolId);
	}

	return listenerId;
}

int SFEngine::AddConnector(int connectorId, char* szIP, unsigned short port) {
	return GetNetworkEngine()->AddConnector(connectorId, szIP, port);
}


bool SFEngine::AddPacketProtocol(int packetProtocolId, IPacketProtocol* pProtocol) {
	return pPacketProtocolManager_->AddPacketProtocol(packetProtocolId, pProtocol);
}

bool SFEngine::SendRequest(BasePacket* pPacket)
{
	return GetNetworkEngine()->SendRequest(pPacket);
}



bool SFEngine::Start(char* szIP, unsigned short port) {

	_EngineConfig* pInfo = Config_.GetConfigureInfo();
	/*
	if (szIP != nullptr && port != 0)
		LOG(INFO) << "Engine Starting... IP : " << szIP << " Port : " << port;
	else
		LOG(INFO) << "Engine Starting... IP : " << (char*)StringConversion::ToASCII(pInfo->serverIP).c_str() << " Port : " << pInfo->serverPort;
	*/
	bool bResult = false;
	if (port != 0)
		bResult = pNetworkEngine_->Start(szIP, port);
	else
		bResult = pNetworkEngine_->Start((char*)StringConversion::ToASCII(pInfo->serverIP).c_str(), pInfo->serverPort);

	if (bResult == false) {
		// LOG(ERROR) << "Engine Start Fail!!";
		return false;
	}
	// LOG(INFO) << "Engine Start!!";

	return true;
}