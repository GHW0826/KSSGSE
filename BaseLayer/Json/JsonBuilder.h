#pragma once
#ifndef __JSONBUILDER__
#define __JSONBUILDER__

class JsonObjectNode;

class JsonBuilder
{
public:
	JsonBuilder();
	~JsonBuilder();
public:
	bool PushBuffer(const char* buffer, size_t bufferLen);
	bool PopCompleteNode(JsonObjectNode& node, unsigned short dataSize);

	static unsigned int MakeBuffer(const JsonObjectNode& node, char* buffer, unsigned int bufferLen);
	void PrepareBuffer(int ioSize);

	unsigned int GetUsedBufferSize()		{ return writtenOffset_ - readOffset_; }
	char* GetBuffer()						{ return &bufferPtr_[readOffset_]; }
	void IncReadOffset(unsigned int inc)	{ readOffset_ += inc; }
protected:
	bool AddBuffer(const char* buffer, unsigned int bufferLen);
private:
	char* bufferPtr_;
	unsigned int writtenOffset_;
	unsigned int readOffset_;
	int ioSize_;
};


#endif // #ifdef __JSONBUILDER__ 
