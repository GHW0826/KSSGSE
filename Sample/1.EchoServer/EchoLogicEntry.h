#pragma once
#ifndef __ECHOLOGICENTRY__
#define __ECHOLOGICENTRY__

#include "ILogicEntry.h"

class BasePacket;

// ��Ŷ ó���� ����ϴ� Ŭ����
class EchoLogicEntry : public ILogicEntry
{
public:
	EchoLogicEntry(void);
	virtual ~EchoLogicEntry(void);

	// �ʱ�ȭ �۾��� �����ϴ� �޼ҵ�
	// DB ������ �ʿ��� ������ ����ٸ� �ش� �޼ҵ忡�� ����� �ʱ�ȭ �۾��� ������.
	virtual bool Initialize() override;

	// ���� ��Ʈ��ũ �̺�Ʈ�� �ٷ�� �޼ҵ�
	virtual bool ProcessPacket(BasePacket* pBasePacket) override;

	virtual bool ProcessConnectorPacket(BasePacket* pPacket);
};


#endif
