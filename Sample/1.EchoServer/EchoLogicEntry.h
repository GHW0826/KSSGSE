#pragma once
#ifndef __ECHOLOGICENTRY__
#define __ECHOLOGICENTRY__

#include "ILogicEntry.h"

class BasePacket;

// 패킷 처리를 담당하는 클래스
class EchoLogicEntry : public ILogicEntry
{
public:
	EchoLogicEntry(void);
	virtual ~EchoLogicEntry(void);

	// 초기화 작업을 수행하는 메소드
	// DB 접속이 필요한 서버를 만든다면 해당 메소드에서 기능의 초기화 작업을 수행함.
	virtual bool Initialize() override;

	// 실제 네트워크 이벤트를 다루는 메소드
	virtual bool ProcessPacket(BasePacket* pBasePacket) override;

	virtual bool ProcessConnectorPacket(BasePacket* pPacket);
};


#endif
