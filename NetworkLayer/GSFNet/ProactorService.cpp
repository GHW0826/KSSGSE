#include "ProactorService.h"
#include "SingltonObject.h"
#include "InterlockedValue.h"
#include <Assert.h>

volatile int g_currentSessionCnt = 0;

ProactorService::ProactorService()
	: bServiceCloseFlag_(false)
	, pTimerLock_(0)
{
	InterlockedIncrement((LONG*)&g_currentSessionCnt);
}

ProactorService::~ProactorService(void) {
	if (pTimerLock_) {
		delete pTimerLock_;
		pTimerLock_ = nullptr;
	}
	InterlockedDecrement((LONG*)&g_currentSessionCnt);
}

void ProactorService::open(ACE_HANDLE h, ACE_Message_Block& MessageBlock) {
	this->handle(h);

	if (this->asyncReader_.open(*this) != 0 || this->asyncWriter_.open(*this) != 0) {
		delete this;
		return;
	}

	pTimerLock_ = new InterlockedValue();
	pTimerLock_->Release();

	bServiceCloseFlag_ = false;
	if (g_currentSessionCnt > GetEngine()->GetMaxUserAccept()) {
		if (this->handle() != ACE_INVALID_HANDLE)
			ACE_OS::closesocket(this->handle());

		this->handle(ACE_INVALID_HANDLE);
		delete this;
		return;
	}

	serial_ = ProactorServiceManagerSinglton::instance()->Register(this);
	if (serial_ == INVALID_ID) {
		if (this->handle() != ACE_INVALID_HANDLE)
			ACE_OS::closesocket(this->handle());

		this->handle(ACE_INVALID_HANDLE);
		delete this;
		return;
	}

	RegisterTimer();

	sockaddr_in addr;
	int addrLen = sizeof(addr);

	ACE_OS::getpeername(this->handle(), (sockaddr*)&addr, &addrLen);

	sessionDesc_.port = ntohs(addr.sin_port);
	char* szIP = inet_ntoa(addr.sin_addr);
	if (szIP != nullptr)
		sessionDesc_.szIP = inet_ntoa(addr.sin_addr);


	ISession::OnConnect(this->serial_, sessionDesc_);

	PostRecv();
}

void ProactorService::PostRecv() {
	ACE_Message_Block* pBlock;

	ACE_NEW_NORETURN(pBlock, ACE_Message_Block(2048));
	if (this->asyncReader_.read(*pBlock, pBlock->space()) != 0) {
		pBlock->release();
		ReserveClose();
	}
}

void ProactorService::handle_read_stream(const ACE_Asynch_Read_Stream::Result& result) {
	ACE_Message_Block& block = result.message_block();
	if (!result.success() || result.bytes_transferred() == 0) {
		block.release();
		ReserveClose();
	}
	else {
		if (false == ISession::OnReceive(block.rd_ptr(), (unsigned short)block.length(), sessionDesc_)) {
			block.release();
			ReserveClose();
			return;
		}
		//20150322 memory leak fix
		block.release();

		PostRecv();
	}
}

void ProactorService::handle_write_stream(const ACE_Asynch_Write_Stream::Result& result) {
	result.message_block().release();
}

void ProactorService::RegisterTimer() {
	ACE_Time_Value intervalTime(5, 0);
	ACE_Proactor::instance()->schedule_timer(*this, 0, ACE_Time_Value::zero, intervalTime);
}

void ProactorService::UnregisterTimer() {
	ACE_Proactor::instance()->cancel_timer(*this);
}

void ProactorService::Disconnect() {
	if (this->handle() != ACE_INVALID_HANDLE)
		ACE_OS::closesocket(this->handle());

	this->handle(ACE_INVALID_HANDLE);

	bServiceCloseFlag_ = true;
}

void ProactorService::ReserveClose() {
	ProactorServiceManagerSinglton::instance()->UnRegister(serial_);

	if (this->handle() != ACE_INVALID_HANDLE)
		ACE_OS::closesocket(this->handle());

	this->handle(ACE_INVALID_HANDLE);

	ISession::OnDisconnect(this->serial_, sessionDesc_);

	bServiceCloseFlag_ = true;
}

void ProactorService::handle_time_out(const ACE_Time_Value& tv, const void* arg) {
	ACE_UNUSED_ARG(tv);
	ACE_UNUSED_ARG(arg);

	if (bServiceCloseFlag_ == true && pTimerLock_->Acquire() == true) {
		UnregisterTimer();
		delete this;
	}
}

void ProactorService::SendInternal(char* pBuffer, int bufferSize) {
	ACE_Message_Block* pBlock = nullptr;
	ACE_NEW_NORETURN(pBlock, ACE_Message_Block(bufferSize));
	pBlock->copy((const char*)pBuffer, bufferSize);

	if (nullptr == pBlock->cont()) {
		asyncWriter_.write(*pBlock, pBlock->length());
	}
	else {
		asyncWriter_.writev(*pBlock, pBlock->total_length());
	}
}

bool ProactorService::SendRequest(BasePacket* pPacket) {
	IPacketProtocol* pProtocol = GetPacketProtocol();
	if (nullptr == pProtocol)
		return false;

	char* pBuffer = nullptr;
	int bufferSize = 0;
	if (false == pProtocol->Encode(pPacket, &pBuffer, bufferSize))
		return false;

	SendInternal(pBuffer, bufferSize);

	return true;
}