#pragma once
#ifndef __SFPACKET__
#define __SFPACKET__

#include "SFFastCRC.h"
#include "BasePacket.h"
#include "SFStructure.h"

class SFCompress;

class SFPacket : public BasePacket
{
public:
	SFPacket(USHORT packetID);
	SFPacket();
	virtual ~SFPacket(void);

	bool Initialize();
	void ResetBuffer();
	void ResetDataBuffer();

	virtual void Release() override;
	static bool GetDataCRC(BYTE* pDataBuf, DWORD dwDataSize, DWORD& dwDataCRC);
	bool CheckDataCRC();


	virtual bool Encode(unsigned short packetSize, int packetOption) override;
	bool Decode(unsigned short packetSize, int& ErrorCode);
	//virtual bool Decode(char* pBuf, unsigned int nSize) override; //

	void ReadStart() { usCurrentReadPosition_ = sizeof(SFPacketHeader); }
	void ReadEnd() {}


	inline void SetPacketID(unsigned short packetID) { BasePacket::SetPacketID(packetID);  pHeader_->packetID = packetID; }

	inline int GetDataSize() { return pHeader_->dataSize; }
	inline void SetDataSize(USHORT size) { pHeader_->dataSize = size; }

	inline int GetPacketSize() { return sizeof(SFPacketHeader) + pHeader_->dataSize; }

	SFPacketHeader* GetHeader();
	BYTE* GetData();

	SFPacket& operator << (BYTE Data);
	SFPacket& operator << (CHAR Data);
	SFPacket& operator << (SHORT Data);
	SFPacket& operator << (USHORT Data);
	SFPacket& operator << (int Data);
	SFPacket& operator << (DWORD Data);
	SFPacket& operator << (float Data);
	SFPacket& operator << (char* szStr);
	void Write(const BYTE* pBuffer, int bufferSize);

	SFPacket& operator >> (BYTE& Data);
	SFPacket& operator >> (CHAR& Data);
	SFPacket& operator >> (SHORT& Data);
	SFPacket& operator >> (USHORT& Data);
	SFPacket& operator >> (int& Data);
	SFPacket& operator >> (DWORD& Data);
	SFPacket& operator >> (float& Data);
	SFPacket& operator >> (char* szStr);
	void Read(BYTE* pBuffer, int bufferSize);

	virtual BasePacket* Clone() override;

protected:

private:
	SFPacketHeader* pHeader_;
	BYTE* pPacketBuffer_;
	USHORT usCurrentReadPosition_;

	static SFFastCRC FastCRC_;
	bool bEncoded_;

	static USHORT packetMaxSize_;
};

#endif
