#pragma once
#ifndef __INETWORKENGINE__
#define __INETWORKENGINE__

#pragma warning( push )
#pragma warning (disable : 4100) 

#ifdef NETWORKENGINE_EXPORTS
#define NETWORKENGINEDECL __declspec(dllexport)
#else
#define NETWORKENGINEDECL __declspec(dllimport)
#endif

class IEngine;
class BasePacket;

class INetworkEngine
{
public:
	INetworkEngine(IEngine* pEngine) 
		: pEngine_(pEngine) 
	{}
	virtual ~INetworkEngine() {};

	virtual bool Init(int ioThreadCnt) = 0;

	virtual bool Start(char* szIP, unsigned short Port) = 0;

	// 엔진 종료
	virtual bool Shutdown() = 0;

	// Desc : 데이터 전송
	virtual bool SendRequest(BasePacket* pPacket) = 0;

	// 강제로 세션을 종료하고 싶을 때
	virtual bool Disconnect(int Serial) = 0;

	// 내부적으로 타이머 이벤트를 발생시킬 수 있는지 확인
	virtual bool CheckTimerImpl()																{ return false; } //default timer 사용
	virtual long AddTimer(unsigned int TimerID, unsigned int StartTime, unsigned int Period)	{ return false; } //타이머 생성
	virtual bool CancelTimer(int timerID)														{ return false; }

	// 다른 서버와 연결 가능하도록 커넥터를 추가한다.
	virtual int AddConnector(int connectorId, char* szIP, unsigned short port)					{ return -1; }

	//서버 리스너를 생성.
	virtual int AddListener(char* szIP, unsigned short port, bool bDefaultListener = false)		{ return -1; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	IEngine* GetEngine()	{ return pEngine_; }
protected:
	IEngine* pEngine_;
};

#pragma warning( pop )

typedef INetworkEngine* (CREATENETWORKENGINE)(bool Server, IEngine* pEngine);
extern "C" NETWORKENGINEDECL INetworkEngine* CreateNetworkEngine(bool Server, IEngine * pEngine);

#endif