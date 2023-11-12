#include "stdAfx.h"
#include "SFPacketProtocol.h"
#include "SFEngine.h"
#include "ILogicDispatcher.h"

void SendLogicLayer(BasePacket* pPacket) {
	SFEngine::GetInstance()->GetLogicDispatcher()->Dispatch(pPacket);
}