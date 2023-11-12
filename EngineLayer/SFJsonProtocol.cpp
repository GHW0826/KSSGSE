#include "stdafx.h"
#include "SFJsonProtocol.h"
#include "SFJsonPacket.h"

SFJsonProtocol::SFJsonProtocol(void)
	: pBuffer_(nullptr)
{}
SFJsonProtocol::~SFJsonProtocol(void) {
	if (pBuffer_)
		delete pBuffer_;
}

bool SFJsonProtocol::Initialize(int ioBufferSize, unsigned short packetSize, int packetOption) {
	builder_.PrepareBuffer(ioBufferSize);

	ioSize_ = ioBufferSize;
	packetSize_ = packetSize;
	packetOption_ = packetOption;

	pBuffer_ = new char[packetSize_];
	memset(pBuffer_, 0, packetSize_);
	return true;
}

bool SFJsonProtocol::AddTransferredData(char* pBuffer, DWORD dwTransferred) {
	builder_.PushBuffer(pBuffer, dwTransferred);
	return true;
}

bool SFJsonProtocol::Reset() {
	return true;
}

bool SFJsonProtocol::Encode(BasePacket* pPacket, char** ppBuffer, int& bufferSize) {
	SFJsonPacket* pJsonPacket = (SFJsonPacket*)pPacket;
	JsonObjectNode& ObjectNode = pJsonPacket->GetData();

	memset(pBuffer_, 0, packetSize_);

	pJsonPacket->Encode(packetSize_, packetOption_);

	//////////////////////////////////////////////////
	//header copy
	//////////////////////////////////////////////////
	memcpy(pBuffer_, pJsonPacket->GetHeader(), sizeof(SFPacketHeader));
	unsigned int writtenSize = 
		JsonBuilder::MakeBuffer(ObjectNode, pBuffer_ + sizeof(SFPacketHeader), packetSize_ - sizeof(SFPacketHeader));
	*((unsigned short*)pBuffer_ + 5) = writtenSize;

	*ppBuffer = pBuffer_;
	bufferSize = sizeof(SFPacketHeader) + writtenSize;
	return true;
}

bool SFJsonProtocol::GetCompleteNode(SFJsonPacket* pPacket) {
	if (builder_.GetUsedBufferSize() < sizeof(SFPacketHeader))
		return false;

	memcpy(pPacket->GetHeader(), builder_.GetBuffer(), sizeof(SFPacketHeader));

	SFPacketHeader* pHeader = pPacket->GetHeader();
	pPacket->SetPacketID(pHeader->packetID);

	if (pHeader->dataSize > packetSize_ - sizeof(SFPacketHeader))
		return false;

	if (builder_.GetUsedBufferSize() < pHeader->dataSize + sizeof(SFPacketHeader))
		return false;

	builder_.IncReadOffset(sizeof(SFPacketHeader));

	return builder_.PopCompleteNode(pPacket->GetData(), pHeader->dataSize);
}

BasePacket* SFJsonProtocol::GetPacket(int& ErrorCode) {
	ErrorCode = PACKETIO_ERROR_NONE;
	SFJsonPacket* pPacket = (SFJsonPacket*)CreatePacket();
	if (false == GetCompleteNode(pPacket)) {
		// ErrorCode = SFProtocol::eIncompletePacket; //
		DisposePacket(pPacket);
		return nullptr;
	}

	return pPacket;
}

BasePacket* SFJsonProtocol::CreatePacket() {
	return new SFJsonPacket();
}

bool SFJsonProtocol::DisposePacket(BasePacket* pPacket) {
	if (pPacket)
		delete pPacket;
	return true;
}