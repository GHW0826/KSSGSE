#pragma once

#ifndef __SFCOMPRESSLZF__
#define __SFCOMPRESSLZF__

class SFCompressLzf
{
public:
	SFCompressLzf(void);
	virtual ~SFCompressLzf(void);

	bool Compress(BYTE* pDest, int& DestLen, BYTE* pSrc, int SrcLen);
	bool Uncompress(BYTE* pDest, int& DestLen, BYTE* pSrc, int SrcLen);
};

#endif