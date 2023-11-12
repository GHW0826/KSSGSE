#pragma once

#ifndef __IENGINE__
#define __IENGINE__

#include "EngineStructure.h"

// #pragma warning�� �����Ϸ��� ��� ����ϴ� ����� �ٲ���
// #pragma warning(push)�� ��� ��� ���¸� �����ϰ� 
// #pragma warning(pop)�� push���� ����� ��� ���¸� �ٽ� �ҷ��� ����.
// ��, push, pop ���̿��� ����Ǵ� ��� ���¸� ����
// disiable: ��ȣ : ��� ������� �ʽ��ϴ�.

#pragma warning( push )

// ��� ��ȣ�� 0~999�������� �ش� ���ڿ� 4000�� ���� ���� ���� �Ͱ� ����
// #pragma warning(once:706) -> #pragma warning(once:4706)
#pragma warning (disable : 4100) 

class ISessionService;

class IEngine
{
public:
	IEngine(void) : maxUserAccept_(5000) {}
	virtual ~IEngine(void) {}

	virtual ISessionService* CreateSessionService(_SessionDesc& desc) = 0;

	// ������ �����Ǿ��� ��
	//  return value : �ش� Ŭ���̾�Ʈ�� ���� �ø���
	virtual bool OnConnect(int Serial, _SessionDesc& desc) = 0;

	// ������ ����Ǿ��� ��
	//   Arg1 : ����Ǵ� Ŭ���̾�Ʈ�� ���� �ø���
	virtual bool OnDisconnect(int Serial, _SessionDesc& desc) = 0;

	// Ÿ�̸� �̺�Ʈ�� �߻��� ��
	virtual bool OnTimer(const void* arg) { return false; }

	int GetMaxUserAccept() { return maxUserAccept_; }
	void SetMaxUserAccept(int maxUserAccept) { maxUserAccept_ = maxUserAccept; }
private:
	int maxUserAccept_;
};

#pragma warning( pop )

#endif