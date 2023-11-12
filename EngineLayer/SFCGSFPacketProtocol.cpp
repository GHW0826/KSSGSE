#include "stdafx.h"
#include "SFCGSFPacketProtocol.h"
#include "SFPacketIOBuffer.h"
#include "SFPacket.h"

SFCGSFPacketProtocol::SFCGSFPacketProtocol(void)
{}
SFCGSFPacketProtocol::~SFCGSFPacketProtocol(void) {
	if (pPacketIOBuffer_)
		delete pPacketIOBuffer_;
	pPacketIOBuffer_ = nullptr;
}

bool SFCGSFPacketProtocol::Initialize(int ioBufferSize, unsigned short packetSize, int packetOption) {
	pPacketIOBuffer_ = new SFPacketIOBuffer();
	pPacketIOBuffer_->AllocIOBuf(ioBufferSize);

	ioSize_ = ioBufferSize;
	packetSize_ = packetSize;
	packetOption_ = packetOption;

	return true;
}

BasePacket* SFCGSFPacketProtocol::GetPacket(int& errorCode) {

	SFPacket* pPacket = PacketPoolSingleton::instance()->Alloc();
	if (pPacket == nullptr) {
		errorCode = PACKETIO_ERROR_MEMORY_ALLOC;
		return nullptr;
	}

	pPacket->Initialize();
	if (false == pPacketIOBuffer_->GetPacket(*pPacket->GetHeader(), (char*)pPacket->GetData(), packetSize_, errorCode)) {
		PacketPoolSingleton::instance()->Release(pPacket);
		return nullptr;
	}

	return pPacket;
}

bool SFCGSFPacketProtocol::AddTransferredData(char* pBuffer, DWORD dwTransferred) {
	pPacketIOBuffer_->AppendData(pBuffer, dwTransferred);
	return true;
}

bool SFCGSFPacketProtocol::Reset() {
	pPacketIOBuffer_->InitIOBuf();
	return true;
}

bool SFCGSFPacketProtocol::Encode(BasePacket* pPacket, char** ppBuffer, int& bufferSize) {
	SFPacket* pSFPacket = (SFPacket*)pPacket;
	if (false == pSFPacket->Encode(packetSize_, packetOption_))
		return false;

	*ppBuffer = (char*)pSFPacket->GetHeader();
	bufferSize = pSFPacket->GetPacketSize();

	return true;
}

bool SFCGSFPacketProtocol::DisposePacket(BasePacket* pPacket) {
	SFPacket* pSFPacket = static_cast<SFPacket*>(pPacket);

	SFASSERT(pSFPacket != nullptr);
	return PacketPoolSingleton::instance()->Release(pSFPacket);
}