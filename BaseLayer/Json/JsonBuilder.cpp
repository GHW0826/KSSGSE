#include "JsonBuilder.h"
#include "JsonNode.h"

//const char* Seperator = "\r\n";
//const int SeperatorLen = strlen(Seperator);
JsonBuilder::JsonBuilder()
	: bufferPtr_(nullptr),
	writtenOffset_(0),
	readOffset_(0),
	ioSize_(0)
{}
JsonBuilder::~JsonBuilder() {
	if (nullptr != bufferPtr_) {
		delete[] bufferPtr_;
		bufferPtr_ = nullptr;
	}
}

void JsonBuilder::PrepareBuffer(int ioSize) {
	bufferPtr_ = new char[ioSize];
	memset(bufferPtr_, 0, ioSize);
	ioSize_ = ioSize;
}

bool JsonBuilder::PushBuffer(const char* buffer, size_t bufferLen) {
	return AddBuffer(buffer, (unsigned int)bufferLen);
}

bool JsonBuilder::PopCompleteNode(JsonObjectNode& node, unsigned short dataSize) {
	// std::string totalContent(&m_bufferPtr[m_readOffset], m_writtenOffset);	//
	// size_t pos = totalContent.find(Seperator);								//
	// if (std::string::npos == pos)											//
	//		return false;

	unsigned int usedSize = node.Parse(&bufferPtr_[readOffset_], dataSize);
	if (0 == usedSize) {
		readOffset_ += dataSize;
		return false;
	}

	/*if (usedSize != dataSize) //
	{						    //
		SFASSERT(0);		    //
		return false;		    //
	}*/						    //

	readOffset_ += dataSize;
	if (readOffset_ == writtenOffset_) {
		readOffset_ = 0;
		writtenOffset_ = 0;
	}
	return true;
}

bool JsonBuilder::AddBuffer(const char* buffer, unsigned int bufferLen) {
	if ((int)(writtenOffset_ + bufferLen) > ioSize_)
		return false;

	memcpy(&bufferPtr_[writtenOffset_], buffer, bufferLen);
	writtenOffset_ += bufferLen;
	return true;
}

unsigned int JsonBuilder::MakeBuffer(const JsonObjectNode& node, char* buffer, unsigned int bufferLen) {
	tstring contentStr = node.ToString();
	if (bufferLen < contentStr.length())
		return 0;
	memcpy(buffer, contentStr.c_str(), contentStr.length());
	return (unsigned int)(contentStr.length());
	// memcpy(&buffer[contentStr.length()], Seperator, SeperatorLen); //
	// return (unsigned int)(contentStr.length()+SeperatorLen);		 //
}