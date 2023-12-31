#include "ACEEngine.h"
#include <assert.h>
#include "SingltonObject.h"
#include "ProactorWorkerThread.h"
#include "ProactorAcceptor.h"

#ifdef _DEBUG
#pragma comment(lib, "aced.lib")
#else
#pragma comment(lib, "ace.lib")
#endif

constexpr int PROACTOR_THREAD_GROUP_ID = 9783;

INetworkEngine* CreateNetworkEngine(bool bServer, IEngine* pEngine) {
	if (bServer)
		return new ACEServerEngine(pEngine);
	else
		return new ACEClientEngine(pEngine);
}


ACEEngine::ACEEngine(IEngine* pEngine)
	: INetworkEngine(pEngine),
	timeOutHandler_(this),
	acceptorIndex_(1),
	connectorIndex_(0),
	workThreadGroupID_(-1)
{	
	ProactorServiceManagerSinglton::instance();
}
ACEEngine::~ACEEngine(void)
{}


bool ACEEngine::SendRequest(BasePacket* pPacket) {
	return ProactorServiceManagerSinglton::instance()->SendRequest(pPacket);
}

bool ACEEngine::Disconnect(int serial) {
	ProactorServiceManagerSinglton::instance()->Disconnect(serial);
	return true;
}

bool ACEEngine::NetworkOpen() {
	for (auto& acceptor : mapAcceptor_) {
		ProactorAcceptor* pAcceptor = acceptor.second;
		ACE_INET_Addr listen_addr;
		listen_addr.set(pAcceptor->GetPort());
		if (0 != pAcceptor->open(listen_addr, 0, 1, ACE_DEFAULT_BACKLOG, 1, 0, 1, 1, 1024)) {
			delete pAcceptor;
			return false;
		}
	}

	return true;
}


int ACEEngine::AddConnector(int connectorIndex, char* szIP, unsigned short port) {

	ACE_SOCK_Stream* stream = new ACE_SOCK_Stream();
	ACE_INET_Addr connectAddr(port, szIP);
	ACE_SOCK_Connector connector;
	int result = connector.connect(*stream, connectAddr);
	if (-1 == result)
		return -1;

	_SessionDesc sessionDesc;
	sessionDesc.identifier = connectorIndex;
	sessionDesc.sessionType = 1;

	ProactorService* pService = new ProactorService();
	pService->SetOwner(this);
	pService->SetSessionDesc(sessionDesc);

	ACE_Message_Block mb;
	pService->open(stream->get_handle(), mb);
	if (stream) {
		delete stream;
		stream = nullptr;
	}

	return pService->GetSerial();
}

constexpr int DEFAULT_LISTENER_INDEX = 1;
int ACEEngine::AddListener(char* szIP, unsigned short port, bool bDefaultListener) {
	ProactorAcceptor* pAcceptor = new ProactorAcceptor(this, szIP, port);

	if (bDefaultListener == true) {
		mapAcceptor_.insert(std::make_pair(DEFAULT_LISTENER_INDEX, pAcceptor));
		pAcceptor->SetAcceptorNum(DEFAULT_LISTENER_INDEX);
		return DEFAULT_LISTENER_INDEX;
	}

	++acceptorIndex_;
	pAcceptor->SetAcceptorNum(acceptorIndex_);

	mapAcceptor_.insert(std::make_pair(acceptorIndex_, pAcceptor));

	return acceptorIndex_;
}

long ACEEngine::AddTimer(unsigned int timerID, unsigned int startTime, unsigned int period) {
	ACE_Time_Value interval(period / 1000, (period % 1000) * 1000);
	ACE_Time_Value start(startTime / 1000, (startTime % 1000) * 1000);

	return ACE_Proactor::instance()->schedule_timer(timeOutHandler_, (void*)timerID, start, interval);
}

bool ACEEngine::CancelTimer(int timerID) {

	if (timerID < 0) {
		ACE_Proactor::instance()->cancel_timer(timeOutHandler_);
		return true;
	}

	return ACE_Proactor::instance()->cancel_timer(timerID) == 1;
}


bool ACEEngine::CheckTimerImpl() {
	return true;
}

bool ACEEngine::Shutdown() {

	ACE_Proactor::instance()->end_event_loop();

	ACE_Thread_Manager::instance()->wait_grp(workThreadGroupID_);

	return true;
}


//////////////////////////////////////////////////////////////////////////////////////

ACEServerEngine::ACEServerEngine(IEngine* pEngine)
	: ACEEngine(pEngine)
{}

bool ACEServerEngine::Init(int ioThreadCnt) {

	workThreadGroupID_ = ACE_Thread_Manager::instance()->spawn_n(ioThreadCnt, (ACE_THR_FUNC)ProactorWorkerThread, NULL, THR_NEW_LWP, ACE_DEFAULT_THREAD_PRIORITY);
	if (workThreadGroupID_ == -1) {
		assert(0);
		return false;
	}

	return true;
}

bool ACEServerEngine::Start(char* szIP, unsigned short port) {
	return NetworkOpen();
}

ACEClientEngine::ACEClientEngine(IEngine* pEngine)
	: ACEEngine(pEngine)
{}

bool ACEClientEngine::Init(int ioThreadCnt) {
	workThreadGroupID_ = ACE_Thread_Manager::instance()->spawn_n(ioThreadCnt, (ACE_THR_FUNC)ProactorWorkerThread, NULL, THR_NEW_LWP, ACE_DEFAULT_THREAD_PRIORITY);
	if (workThreadGroupID_ == -1) {
		assert(0);
		return false;
	}

	return true;
}

bool ACEClientEngine::Start(char* szIP, unsigned short port) {
	return AddConnector(1, szIP, port) >= 0;
}