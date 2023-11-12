#include "stdafx.h"
#include "SFIOBuffer.h"

SFIOBuffer::SFIOBuffer(void)
	: iBufSize_(0)
	, pBuffer_(nullptr)
	, iUsedBufferSize_(0)
{}

SFIOBuffer::~SFIOBuffer(void)
{
	FreeIOBuf();
}

VOID SFIOBuffer::InitIOBuf()
{
	iTail_ = 0;
	iHead_ = 0;

	iUsedBufferSize_ = 0;

	if (pBuffer_)
		memset(pBuffer_, 0, sizeof(char) * iBufSize_);
}

int SFIOBuffer::AppendData(const char* pBuffer, int iSize)
{
	if (iUsedBufferSize_ + iSize >= iBufSize_)
		return -1;

	int iAddedData = 0;
	int iAdded = 0;

	while (iSize > 0) {
		if (iSize > iBufSize_ - iTail_)
			iAddedData = iBufSize_ - iTail_;
		else
			iAddedData = iSize;

		if (iAddedData) {
			memcpy(pBuffer_ + iTail_, pBuffer, iAddedData);

			iAdded += iAddedData;

			iSize -= iAddedData;
			pBuffer += iAddedData;
			iTail_ += iAddedData;

			if (iTail_ >= iBufSize_)
				iTail_ -= iBufSize_;
		}
	}

	CalcUsedBufferSize();

	return iAdded;
}

int SFIOBuffer::GetData(char* pBuffer, int iSize)
{
	if (GetUsedBufferSize() < iSize)
		iSize = GetUsedBufferSize();

	if (iSize <= 0)
		return 0;

	if (iHead_ + iSize > iBufSize_) {
		int iDiff = iBufSize_ - iHead_;
		memcpy(pBuffer, pBuffer_ + iHead_, iDiff);
		memcpy(pBuffer + iDiff, pBuffer_, iSize - iDiff);
	}
	else {
		memcpy(pBuffer, pBuffer_ + iHead_, iSize);
	}

	iHead_ += iSize;
	if (iHead_ >= iBufSize_) {
		iHead_ -= iBufSize_;
	}

	CalcUsedBufferSize();

	return iSize;
}

int	SFIOBuffer::RemoveData(int iSize)
{
	if (GetUsedBufferSize() < iSize)
		iSize = GetUsedBufferSize();

	if (iSize <= 0)
		return 0;

	iHead_ += iSize;
	if (iHead_ >= iBufSize_) {
		iHead_ -= iBufSize_;
	}

	CalcUsedBufferSize();

	return iSize;
}

bool SFIOBuffer::AllocIOBuf(int iBufSize)
{
	if (iBufSize <= 0)
		iBufSize = MAX_IO_SIZE;

	if (iBufSize > MAX_IO_SIZE) {
		return false;
	}

	pBuffer_ = new char[iBufSize];
	if (pBuffer_ == nullptr) {
		return false;
	}

	iBufSize_ = iBufSize;

	InitIOBuf();

	return TRUE;
}

bool SFIOBuffer::FreeIOBuf()
{
	if (pBuffer_) {
		delete[] pBuffer_;
		pBuffer_ = nullptr;
	}

	iBufSize_ = 0;
	iHead_ = iTail_ = 0;
	iUsedBufferSize_ = 0;

	return true;
}

void SFIOBuffer::CalcUsedBufferSize()
{
	if (iHead_ > iTail_)
		iUsedBufferSize_ = iBufSize_ - iHead_ + iTail_;
	else
		iUsedBufferSize_ = iTail_ - iHead_;
}
