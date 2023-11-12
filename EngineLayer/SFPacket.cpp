#include "stdafx.h"
#include "SFPacket.h"

#include "SFEncrytion.h"
#include "SFEncryptionXOR.h"
#include "SFCompressor.h"
#include "SFCompressLzf.h"
#include "SFCGSFPacketProtocol.h"
#include "SFFastCRC.h"

USHORT SFPacket::packetMaxSize_ = MAX_PACKET_SIZE;

SFFastCRC SFPacket::FastCRC_;

SFPacket::SFPacket(USHORT packetID) {
	pPacketBuffer_ = new BYTE[packetMaxSize_];
	pHeader_ = (SFPacketHeader*)pPacketBuffer_;

	Initialize();

	pHeader_->packetID = packetID;
}
SFPacket::SFPacket()
{
	pPacketBuffer_ = new BYTE[packetMaxSize_];
	pHeader_ = (SFPacketHeader*)pPacketBuffer_;
	pHeader_->packetID = 0;

	Initialize();
}
SFPacket::~SFPacket(void) {
	SF_SAFE_RELEASE(pPacketBuffer_);
}

bool SFPacket::Initialize() {
	pHeader_->packetOption = 0;
	pHeader_->dataCRC = 0;
	pHeader_->dataSize = 0;

	ResetBuffer();

	usCurrentReadPosition_ = sizeof(SFPacketHeader);

	bEncoded_ = false;

	return true;
}

void SFPacket::ResetBuffer() {
	memset(pPacketBuffer_, 0, packetMaxSize_);
}

void SFPacket::ResetDataBuffer() {
	memset(pPacketBuffer_ + sizeof(SFPacketHeader), 0, packetMaxSize_ - sizeof(SFPacketHeader));
}

bool SFPacket::Encode(unsigned short packetSize, int packetOption) {

	if (bEncoded_ == true)
		return true;

	if (GetDataSize() < 0 || GetDataSize() > packetMaxSize_ - sizeof(SFPacketHeader)) {
		SFASSERT(0);
		return false;
	}

	if (GetDataSize() == 0) {
		return true;
	}

	BYTE pDestBuf[MAX_PACKET_SIZE] = { 0, };
	int destSize = packetSize - sizeof(SFPacketHeader);

	DWORD dwResult = 0;
	if (packetOption & PACKET_OPTION_COMPRESS && GetDataSize() >= PACKET_COMPRESS_LIMIT) {
		dwResult = SFCompressor<SFCompressLzf>::GetCompressor()->Compress(pDestBuf, destSize, GetData(), GetDataSize());

		if (dwResult != TRUE) {
			SFASSERT(0);
			return false;
		}

		memcpy(GetData(), pDestBuf, destSize);
		SetDataSize((USHORT)destSize);
	}
	else {
		packetOption = packetOption & (~PACKET_OPTION_COMPRESS);
	}

	if (packetOption & PACKET_OPTION_ENCRYPTION) {
		if (false == SFEncrytion<SFEncryptionXOR>::Encrypt((BYTE*)GetData(), GetDataSize())) {
			SFASSERT(0);
			return false;
		}
	}

	DWORD dwDataCRC = 0;

	if (packetOption & PACKET_OPTION_DATACRC) {
		BOOL Result = SFPacket::GetDataCRC((BYTE*)GetData(), GetDataSize(), dwDataCRC);
		if (false == Result) {
			SFASSERT(0);
			return false;
		}

		pHeader_->dataCRC = dwDataCRC;
	}

	pHeader_->SetPacketOption(packetOption);

	bEncoded_ = true;

	return true;
}

bool SFPacket::Decode(unsigned short packetSize, int& errorCode) {

	SFPacketHeader* pHeader = GetHeader();
	if (true == pHeader->CheckDataCRC()) {
	
		bool result = CheckDataCRC();
		if (true != result)	{
			// LOG(WARNING) << "Packet CRC Check Fail!!";
			errorCode = PACKETIO_ERROR_DATA_CRC;
			return FALSE;
		}
	}

	if (true == pHeader->CheckEncryption()) {
		if (false == SFEncrytion<SFEncryptionXOR>::Decrypt((BYTE*)GetData(), GetDataSize())) {
			SFASSERT(0);
			errorCode = PACKETIO_ERROR_DATA_ENCRYPTION;
			return FALSE;
		}
	}

	if (true == pHeader->CheckCompressed()) {
		BYTE pSrcBuf[MAX_IO_SIZE] = { 0, };
		int destSize = packetSize;

		memcpy(pSrcBuf, GetData(), GetDataSize());
		ResetDataBuffer();

		if (false == SFCompressor<SFCompressLzf>::GetCompressor()->Uncompress(GetData(), destSize, pSrcBuf, GetDataSize())) {
			//SFLOG_WARN(L"Packet Uncompress Fail!! %d %d", pHeader->DataCRC, dwDataCRC);
			errorCode = PACKETIO_ERROR_DATA_COMPRESS;
			return false;
		}

		if (destSize + sizeof(SFPacketHeader) > packetSize) {
			errorCode = PACKETIO_ERROR_DATA_COMPRESS;
			return false;
		}

		SetDataSize((USHORT)destSize);
	}

	BasePacket::SetPacketID(pHeader->packetID);

	return true;
}

bool SFPacket::GetDataCRC(BYTE* pDataBuf, DWORD DataSize, DWORD& dwDataCRC) {
	bool Result = FastCRC_.GetZLibCRC((BYTE*)pDataBuf, DataSize, dwDataCRC);
	if (true != Result) {
		SFASSERT(0);
		return false;
	}

	return true;
}

bool SFPacket::CheckDataCRC() {
	DWORD dwDataCRC = 0;

	bool result = GetDataCRC((BYTE*)GetData(), GetDataSize(), dwDataCRC);
	if (true != result || dwDataCRC != GetHeader()->dataCRC) {
		SFASSERT(0);
		return false;
	}

	//DWORD dwResult = SFChecksum::FromBuffer((BYTE*)GetDataBuffer(), SrcSize, dwDataCRC); //
	/*if(ERROR_SUCCESS != dwResult)	//
	{								//
	SFASSERT(0);					//
	return FALSE;					//
	}*/

	return true;
}

BasePacket* SFPacket::Clone() {
	SFPacket* pClone = new SFPacket();
	pClone->CopyBaseHeader(this);
	memcpy(pClone->pHeader_, pHeader_, sizeof(SFPacketHeader));
	memcpy(pClone->pPacketBuffer_, pPacketBuffer_, packetMaxSize_);
	pClone->bEncoded_ = bEncoded_;
	pClone->usCurrentReadPosition_ = usCurrentReadPosition_;

	return pClone;
}

void SFPacket::Release() {
	SFCGSFPacketProtocol::DisposePacket(this);
}

SFPacket& SFPacket::operator << (BYTE Data) {
	if (GetPacketSize() + sizeof(BYTE) > packetMaxSize_)
		return *this;

	memcpy(&pPacketBuffer_[GetPacketSize()], &Data, sizeof(BYTE));
	pHeader_->dataSize += sizeof(BYTE);

	return *this;
}

SFPacket& SFPacket::operator << (CHAR Data) {
	if (GetPacketSize() + sizeof(CHAR) > packetMaxSize_)
		return *this;

	memcpy(&pPacketBuffer_[GetPacketSize()], &Data, sizeof(CHAR));
	pHeader_->dataSize += sizeof(CHAR);

	return *this;
}

SFPacket& SFPacket::operator << (SHORT Data) {
	if (GetPacketSize() + sizeof(SHORT) > packetMaxSize_)
		return *this;

	memcpy(&pPacketBuffer_[GetPacketSize()], &Data, sizeof(SHORT));
	pHeader_->dataSize += sizeof(SHORT);

	return *this;
}

SFPacket& SFPacket::operator << (USHORT Data) {
	if (GetPacketSize() + sizeof(USHORT) > packetMaxSize_)
		return *this;

	memcpy(&pPacketBuffer_[GetPacketSize()], &Data, sizeof(USHORT));
	pHeader_->dataSize += sizeof(USHORT);

	return *this;
}

SFPacket& SFPacket::operator << (int Data) {
	if (GetPacketSize() + sizeof(int) > packetMaxSize_)
		return *this;

	memcpy(&pPacketBuffer_[GetPacketSize()], &Data, sizeof(int));
	pHeader_->dataSize += sizeof(int);

	return *this;
}

SFPacket& SFPacket::operator << (DWORD Data) {
	if (GetPacketSize() + sizeof(DWORD) > packetMaxSize_)
		return *this;

	memcpy(&pPacketBuffer_[GetPacketSize()], &Data, sizeof(DWORD));
	pHeader_->dataSize += sizeof(DWORD);

	return *this;
}

SFPacket& SFPacket::operator << (float Data) {
	if (GetPacketSize() + sizeof(float) > packetMaxSize_)
		return *this;

	memcpy(&pPacketBuffer_[GetPacketSize()], &Data, sizeof(float));
	pHeader_->dataSize += sizeof(float);

	return *this;
}

SFPacket& SFPacket::operator << (char* szStr) {
	int len = (int)strlen(szStr);

	if (len <= 0 || GetPacketSize() + len + 1 > packetMaxSize_)
		return *this;

	memcpy(&pPacketBuffer_[GetPacketSize()], szStr, len);
	pHeader_->dataSize += (USHORT)len;
	pPacketBuffer_[GetPacketSize()] = 0;
	pHeader_->dataSize++;

	return *this;
}

void SFPacket::Write(const BYTE* pBuffer, int bufferSize) {
	if (GetPacketSize() + bufferSize > packetMaxSize_)
		return;

	memcpy(&pPacketBuffer_[GetPacketSize()], pBuffer, bufferSize);
	pHeader_->dataSize += (USHORT)bufferSize;
}


SFPacket& SFPacket::operator >> (BYTE& Data) {
	if (usCurrentReadPosition_ + sizeof(BYTE) > packetMaxSize_)
		return *this;

	memcpy(&Data, &pPacketBuffer_[usCurrentReadPosition_], sizeof(BYTE));
	usCurrentReadPosition_ += sizeof(BYTE);

	return *this;
}

SFPacket& SFPacket::operator >> (CHAR& Data) {
	if (usCurrentReadPosition_ + sizeof(CHAR) > packetMaxSize_)
		return *this;

	memcpy(&Data, &pPacketBuffer_[usCurrentReadPosition_], sizeof(CHAR));
	usCurrentReadPosition_ += sizeof(CHAR);

	return *this;
}

SFPacket& SFPacket::operator >> (SHORT& Data) {
	if (usCurrentReadPosition_ + sizeof(SHORT) > packetMaxSize_)
		return *this;

	memcpy(&Data, &pPacketBuffer_[usCurrentReadPosition_], sizeof(SHORT));
	usCurrentReadPosition_ += sizeof(SHORT);

	return *this;
}

SFPacket& SFPacket::operator >> (USHORT& Data) {
	if (usCurrentReadPosition_ + sizeof(USHORT) > packetMaxSize_)
		return *this;

	memcpy(&Data, &pPacketBuffer_[usCurrentReadPosition_], sizeof(USHORT));
	usCurrentReadPosition_ += sizeof(USHORT);

	return *this;
}

SFPacket& SFPacket::operator >> (int& Data) {
	if (usCurrentReadPosition_ + sizeof(int) > packetMaxSize_)
		return *this;

	memcpy(&Data, &pPacketBuffer_[usCurrentReadPosition_], sizeof(int));
	usCurrentReadPosition_ += sizeof(int);

	return *this;
}

SFPacket& SFPacket::operator >> (DWORD& Data) {
	if (usCurrentReadPosition_ + sizeof(DWORD) > packetMaxSize_)
		return *this;

	memcpy(&Data, &pPacketBuffer_[usCurrentReadPosition_], sizeof(DWORD));
	usCurrentReadPosition_ += sizeof(DWORD);

	return *this;
}

SFPacket& SFPacket::operator >> (float& Data) {
	if (usCurrentReadPosition_ + sizeof(float) > packetMaxSize_)
		return *this;

	memcpy(&Data, &pPacketBuffer_[usCurrentReadPosition_], sizeof(float));
	usCurrentReadPosition_ += sizeof(float);

	return *this;
}

SFPacket& SFPacket::operator >> (char* szStr) {
	int strLen = (int)strlen((char*)&pPacketBuffer_[usCurrentReadPosition_]);
	if (usCurrentReadPosition_ + strLen + 1 > packetMaxSize_)
		return *this;

	strcpy_s(szStr, strLen + 1, (char*)&pPacketBuffer_[usCurrentReadPosition_]);
	usCurrentReadPosition_ += (USHORT)(strLen + 1);

	return *this;
}

void SFPacket::Read(BYTE* pBuffer, int bufferSize) {
	if (usCurrentReadPosition_ + bufferSize > packetMaxSize_)
		return;

	memcpy(pBuffer, &pPacketBuffer_[usCurrentReadPosition_], bufferSize);
	usCurrentReadPosition_ += (USHORT)bufferSize;
}

SFPacketHeader* SFPacket::GetHeader() 
{ 
	return pHeader_; 
}

BYTE* SFPacket::GetData() { 
	return pPacketBuffer_ + sizeof(SFPacketHeader); 
}
