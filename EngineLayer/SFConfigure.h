#pragma once
#ifndef __SFCONFIGURE__
#define __SFCONFIGURE__

class IXMLStreamWriter;
class IXMLStreamReader;

//////////////////////////////////////////////////////////////
//Shouter Structure
//////////////////////////////////////////////////////////////
typedef struct tag_EngineConfig
{
	std::wstring engineName;
	std::wstring serverIP;
	unsigned short serverPort;
	unsigned short maxAccept;

	tag_EngineConfig() {
		Clear();
	}

	void Clear() {
		serverPort = 25251;
		maxAccept = 5000;
	}

}_EngineConfig;

class SFConfigure
{
public:
	SFConfigure(void);
	virtual ~SFConfigure(void);

	bool Initialize();

	bool Read(const WCHAR* pFileName);
	bool Write(const WCHAR* pFileName);

	_EngineConfig* GetConfigureInfo() { return &ConfigureInfo_; }

	// void Serialize(IXMLStreamWriter* out) const;
	// void Deserialize(IXMLStreamReader* in);

protected:


private:
	_EngineConfig ConfigureInfo_;
};


#endif