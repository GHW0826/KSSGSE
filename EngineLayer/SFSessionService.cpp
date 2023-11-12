#include "stdafx.h"
#include "SFSessionService.h"


bool SFSessionService::OnReceive(char* pData, unsigned short Length, _SessionDesc& desc) {
	return pPacketProtocol_->OnReceive(serial_, pData, Length, desc);
}