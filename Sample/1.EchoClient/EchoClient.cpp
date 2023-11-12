#pragma once
#include "EchoCallback.h"
#include "SFNetworkEntry.h"
#include "SFPacketProtocol.h"
#include "ACEHeader.h"
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "ACEHeader.h"
#include "Macro.h"

void ProcessInput() {
	//EchoInputThread에서 유저의 입력을 받아 서버로 패킷을 보냄.
	// 패킷 프로토콜을 JSON으로 선언하였으므로 SFJsonPacket 객체를 생성해 유저의 입력을 담아 전송
	// 서버는 받은 데이터를 그대로 클라에 전다랗고 클라는 콜백 객체인 EchoCallback에서 서버가 보낸 데이터를 처리.
	int inputThreadID = ACE_Thread_Manager::instance()->spawn_n(1, (ACE_THR_FUNC)EchoInputThread, NULL, THR_NEW_LWP, ACE_DEFAULT_THREAD_PRIORITY);
	SFASSERT(inputThreadID != -1);

	// 
	while (SFNetworkEntry::GetInstance()->IsConnected() && g_bExit == false) {
		SFNetworkEntry::GetInstance()->Update();
		Sleep(1);
	}
	ACE_Thread_Manager::instance()->wait_grp(inputThreadID);
}


int _tmain(int argc, _TCHAR* argv[])
{
	EchoCallback* pCallback = new EchoCallback();

	// SFNetworkEntry는 내부적으로 SFEngine 객체를 사용 클라이언트를 위한 기능이 추가된 래핑 클래스
	// 서버와 마찬가지로 패킷 프로토콜을 지정하고 콜백 객체를 생성해 객체에 넘겨줌.
	// Run을 호출해 네트워크 연결을 시도.
	SFNetworkEntry::GetInstance()->Initialize(pCallback, new SFPacketProtocol<SFJsonProtocol>);
	SFNetworkEntry::GetInstance()->Run();

	// 여기서 유저의 입력을 처리하고 네트워크 이벤트를 가져오기 위한 임시 구현 함수
	// 메소드 호출은 메인 스레드에서 하므로 서버로 데이터를 전송하기 위해서는 별도의 스레드를 생성해야 한다.
	// 별도의 라이브러리를 생성하기 위해 ACE 라이브러리 활용
	// loop에 sleep(1)을 넣은 이유는 CPU 낭비를 막기 위함. GUI 코드에서는 sleep가 없어야 함.
	ProcessInput();

	SFNetworkEntry::GetInstance()->ShutDown();

	return 0;
}