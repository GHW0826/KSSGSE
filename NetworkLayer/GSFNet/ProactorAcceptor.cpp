#include "ProactorAcceptor.h"
#include "SingltonObject.h"
#include "INetworkEngine.h"
#include "ProactorService.h"

ProactorAcceptor::ProactorAcceptor(INetworkEngine* pOwner, char* szIP, unsigned short port)
	: pOwner_(pOwner)
	, acceptorNum_(-1)
	, port_(port) {
	if (szIP)
		szIP_ = szIP;
	//ACEAllocator* AceMemoryPool = new ACEAllocator(100, sizeof(SFProactorService)); //
}
ProactorAcceptor::~ProactorAcceptor(void)
{}


int ProactorAcceptor::validate_connection(
	const ACE_Asynch_Accept::Result& result,
	const ACE_INET_Addr& remote,
	const ACE_INET_Addr& local
) {
	struct in_addr* remote_addr = reinterpret_cast<struct in_addr*>(remote.get_addr());
	struct in_addr* local_addr = reinterpret_cast <struct in_addr*>(local.get_addr());

	ACE_UNUSED_ARG(result);
	ACE_UNUSED_ARG(remote);
	ACE_UNUSED_ARG(local);

	return 0;
}

ProactorService* ProactorAcceptor::make_handler() {
	_SessionDesc sessionDesc;
	sessionDesc.identifier = acceptorNum_;
	sessionDesc.sessionType = 0;

	ProactorService* pProactorService = new ProactorService();
	pProactorService->SetOwner(pOwner_);
	pProactorService->SetSessionDesc(sessionDesc);
	return pProactorService;
}