#pragma once
#ifndef __SFIOBUFFER__
#define __SFIOBUFFER__

class SFIOBuffer
{
public:
	SFIOBuffer();
	virtual ~SFIOBuffer();
	
	///////////////////////////////////////////////////////////////////////

	void	InitIOBuf();
	bool	AllocIOBuf(int iBufSize);
	bool	FreeIOBuf();

	void	CalcUsedBufferSize();

	///////////////////////////////////////////////////////////////////////

	int		AppendData(const char* pBuffer, int iSize);
	int		GetData(char* pBuffer, int iSize);
	int		RemoveData(int iSize);

	///////////////////////////////////////////////////////////////////////

	inline char* GetBuffer() { return	pBuffer_; }
	inline int	GetBufSize() { return iBufSize_; }

	inline int GetUsedBufferSize() { return iUsedBufferSize_; }
	inline int GetEmptyBufferSize() { return iBufSize_ - iUsedBufferSize_; }

	int GetHead() { return iHead_; }
	inline void SetHead(int Head) { iHead_ = Head; CalcUsedBufferSize(); }

private:
	int		iHead_;
	int		iTail_;
	int		iBufSize_;
	char*	pBuffer_;
	int		iUsedBufferSize_;
};

#endif