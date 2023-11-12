#pragma once
#ifndef __SFCOMPRESSOR__
#define __SFCOMPRESSOR__

template<typename T>
class SFCompressor
{
public:
	SFCompressor(void) {}
	virtual ~SFCompressor(void) {}

	static T* GetCompressor() { return &Compress_; }

protected:

private:
	static T Compress_;
};

template<typename T>
T SFCompressor<T>::Compress_;


#endif