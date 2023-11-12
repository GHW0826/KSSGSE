#pragma once

#ifndef __IENGINE__
#define __IENGINE__

#include "EngineStructure.h"

// #pragma warning은 컴파일러가 경고를 출력하는 방법을 바꿔줌
// #pragma warning(push)는 모든 경고 상태를 저장하고 
// #pragma warning(pop)은 push에서 저장된 경고 상태를 다시 불러와 적용.
// 즉, push, pop 사이에만 적용되는 경고 상태를 지정
// disiable: 번호 : 경고를 출력하지 않습니다.

#pragma warning( push )

// 경고 번호가 0~999번까지는 해당 숫자에 4000을 더한 값을 줄인 것과 같음
// #pragma warning(once:706) -> #pragma warning(once:4706)
#pragma warning (disable : 4100) 

class ISessionService;

class IEngine
{
public:
	IEngine(void) : maxUserAccept_(5000) {}
	virtual ~IEngine(void) {}

	virtual ISessionService* CreateSessionService(_SessionDesc& desc) = 0;

	// 세션이 생성되었을 때
	//  return value : 해당 클라이언트의 고유 시리얼
	virtual bool OnConnect(int Serial, _SessionDesc& desc) = 0;

	// 세션이 종료되었을 때
	//   Arg1 : 종료되는 클라이언트의 고유 시리얼
	virtual bool OnDisconnect(int Serial, _SessionDesc& desc) = 0;

	// 타이머 이벤트가 발생할 때
	virtual bool OnTimer(const void* arg) { return false; }

	int GetMaxUserAccept() { return maxUserAccept_; }
	void SetMaxUserAccept(int maxUserAccept) { maxUserAccept_ = maxUserAccept; }
private:
	int maxUserAccept_;
};

#pragma warning( pop )

#endif