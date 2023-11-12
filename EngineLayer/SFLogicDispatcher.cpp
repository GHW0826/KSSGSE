#include "stdafx.h"
#include "SFLogicDispatcher.h"
#include "SFEngine.h"

bool SFLogicDispatcher::ReleasePacket(BasePacket* pPacket) {

	//����� ��Ŷ�� �����Ѵ�. ��Ŷ�� Ÿ�Կ� ���� ������ ���°� �ٸ�
	switch (pPacket->GetPacketType())
	{
	case SFPACKET_DATA:
		SFEngine::GetInstance()->ReleasePacket(pPacket);
		break;
	case SFPACKET_CONNECT:
	case SFPACKET_DISCONNECT:
	case SFPACKET_TIMER:
	case SFPACKET_SHOUTER:
	case SFPACKET_SERVERSHUTDOWN:
		delete pPacket;
		break;

	default:
		SFASSERT(0);
		return false;
	}

	return true;
}